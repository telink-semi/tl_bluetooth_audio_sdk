/********************************************************************************************************
 * @file    bt_music_forward.c
 *
 * @brief   BT A2DP forward module: Phone -> Car -> Headphone, SBC format
 *
 *          Dynamic timer interval control strategy:
 *          1. Parse SBC params on connection to compute base interval
 *          2. Accumulate STARTUP_THRESHOLD packets before starting timer
 *          3. Each tx cycle, adjust timer interval based on buffer level
 *          4. Send silence when buffer is empty to keep link alive
 * 
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/common/tlkaudio_debug_data.h"
#include "tlkmw/audio/tlkmw_audio.h"
#include "drivers.h"
#include "tlkmw/sys_dev/tlkmw_sysdev.h"
#include "tlkalg/audio/audio_alg_interface.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "tlkmw/tlkmw.h"

#if (TLKMW_BT_1_TO_2_FORWARD_EN)

#define A2DP_BUFF_FRAME_SIZE     650u
#define A2DP_BUFF_NUM            20u
#define A2DP_BUFF_TOTAL_SIZE     (A2DP_BUFF_FRAME_SIZE * A2DP_BUFF_NUM)

#define A2DP_FWD_TARGET_LEVEL    (A2DP_BUFF_NUM / 2) /* = 10 */
#define A2DP_FWD_DEAD_ZONE       0u
#define A2DP_FWD_STEP_US         200u
#define A2DP_FWD_MAX_DELTA_US    3000u
#define A2DP_FWD_AVG_ALPHA       40u
#define A2DP_FWD_STARTUP_THRESH  (A2DP_BUFF_NUM / 2) /* = 10 */

#define A2DP_FWD_DEFAULT_BASE_US 14510u

#define A2DP_RTP_HDR_SIZE        12u
#define A2DP_SBC_MEDIA_HDR_OFF   12u
#define SBC_NUM_FRAMES_MASK      0x0Fu

#define READ_U16_BE(buf, off)    ((uint16_t)(((buf)[(off)] << 8) | (buf)[(off) + 1]))
#define READ_U32_BE(buf, off)    ((uint32_t)(((uint32_t)(buf)[(off)] << 24) | ((uint32_t)(buf)[(off) + 1] << 16) | ((uint32_t)(buf)[(off) + 2] << 8) | (uint32_t)(buf)[(off) + 3]))

#ifndef CLAMP
#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))
#endif

typedef enum
{
    FWD_STATE_IDLE      = 0,
    FWD_STATE_BUFFERING = 1,
    FWD_STATE_RUNNING   = 2,
} fwd_state_t;

typedef struct
{
    uint8_t *p_a2dp_buff;
    uint16_t a2dp_buff_frame_size;
    uint16_t a2dp_buff_num;
    uint16_t a2dp_buff_wptr;
    uint16_t a2dp_buff_rptr;
} a2dp_buff_context_t;

typedef struct
{
    uint32_t sample_rate;
    uint8_t  blocks;
    uint8_t  subbands;
    uint8_t  channel_mode;
    uint16_t frame_size;
} fwd_sbc_cfg_t;

typedef struct
{
    fwd_state_t   state;
    fwd_sbc_cfg_t sbc;
    uint8_t       frame_num;
    uint32_t      base_us;
    uint32_t      current_us;
    int32_t       level_avg_x100;
} bt_music_fwd_ctrl_t;

a2dp_buff_context_t g_a2dp_buf_ctx;

static uint8_t *s_bt_a2dp_buf_ptr = NULL;

static volatile uint8_t s_a2dp_tx_evt = 0;

static bt_music_fwd_ctrl_t s_bt_music_fwd_ctrl;

void bt_music_forward_timer_reload(uint32_t interval_us);
void bt_music_forward_timer_start(uint32_t interval_us);
void bt_music_forward_timer_stop(void);
void bt_music_forward_send_silence(uint16_t acl_handle);

static void     fwd_ctrl_recalc_base(void);
static void     fwd_ctrl_update_avg(int cur_level);
static uint32_t fwd_ctrl_calc_interval(int smooth_level);

void bt_music_forward_timer_reload(uint32_t interval_us)
{
    tlkmdi_audio_task_set_next_irq(interval_us);
}

void bt_music_forward_timer_start(uint32_t interval_us)
{
    tlkmdi_audio_task_set_next_irq(interval_us);
}

void bt_music_forward_timer_stop(void)
{
    tlkmdi_audio_stop_timer();
}

void bt_music_forward_send_silence(uint16_t acl_handle)
{
    (void)acl_handle;
}

void bt_music_forward_set_a2dp_tx_evt(void)
{
    s_a2dp_tx_evt = 1;
    tlksys_task_setEvtFromIsr(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_APP1);
}

void bt_music_forward_clear_a2dp_tx_evt(void)
{
    s_a2dp_tx_evt = 0;
}

uint8_t bt_music_forward_is_a2dp_tx_evt_set(void)
{
    return s_a2dp_tx_evt;
}

audio_ram_code void bt_music_forward_audio_irq_task(void)
{
#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PA0, 0);
    gpio_write(GPIO_PA0, 1);
    gpio_write(GPIO_PA0, 0);
#endif

    bt_music_forward_set_a2dp_tx_evt();
}

void bt_music_forward_update_sbc_param(uint8_t *data)
{
    if (data == NULL) {
        return;
    }

    uint8_t  bitpool, blocks, subbands, ch_mode;
    uint16_t sample_rate, frame_size;

    bitpool = data[2];

    switch (data[1] & 0xC0u) {
    case 0x00u:
        sample_rate = 16000u;
        break;
    case 0x40u:
        sample_rate = 32000u;
        break;
    case 0x80u:
        sample_rate = 44100u;
        break;
    default:
        sample_rate = 48000u;
        break;
    }

    switch (data[1] & 0x30u) {
    case 0x00u:
        blocks = 4u;
        break;
    case 0x10u:
        blocks = 8u;
        break;
    case 0x20u:
        blocks = 12u;
        break;
    default:
        blocks = 16u;
        break;
    }

    subbands = ((data[1] & 0x01u) == 0u) ? 4u : 8u;

    ch_mode = data[1] & 0x0Cu;

    switch (ch_mode) {
    case 0x00u:
        frame_size = 4u + subbands / 2u + (blocks * bitpool) / 8u;
        break;
    case 0x04u:
        frame_size = 4u + subbands + (blocks * bitpool) / 4u;
        break;
    case 0x08u:
        frame_size = 4u + subbands + (blocks * bitpool) / 8u;
        break;
    default:
        frame_size = 4u + subbands + (blocks * bitpool + subbands) / 8u;
        break;
    }

    s_bt_music_fwd_ctrl.sbc.sample_rate  = sample_rate;
    s_bt_music_fwd_ctrl.sbc.blocks       = blocks;
    s_bt_music_fwd_ctrl.sbc.subbands     = subbands;
    s_bt_music_fwd_ctrl.sbc.channel_mode = ch_mode;
    s_bt_music_fwd_ctrl.sbc.frame_size   = frame_size;

    fwd_ctrl_recalc_base();

    tlkapi_printf(APP_LOG_EN, "[FWD] SBC: %uHz blk=%u sub=%u ch=%u fs=%u n=%u base=%uus\r\n", sample_rate, blocks, subbands, ch_mode, frame_size, s_bt_music_fwd_ctrl.frame_num,
                  s_bt_music_fwd_ctrl.base_us);
}

/* ========================================================================== */
/*  A2DP packet receive (registered as SNK data callback)                     */
/* ========================================================================== */

audio_ram_code void bt_music_forward_receive_a2dp_packet(uint16_t acl_handle, uint8_t *p_data, uint16_t len)
{
#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PA2, 0);
    gpio_write(GPIO_PA2, 1);
    gpio_write(GPIO_PA2, 0);
#endif

    if (acl_handle != btp_a2dp_getSnkHandle()) {
        tlk_printf("a2dp frame handle error aclHandle:%d, needHandle:%d", acl_handle, btp_a2dp_getSnkHandle());
        return;
    }

    if (p_data == NULL || len <= A2DP_SBC_MEDIA_HDR_OFF) {
        tlkapi_printf(APP_LOG_EN, "[FWD] invalid packet: len=%u\r\n", len);
        return;
    }

    // static uint32_t pkt_cnt = 0;
    // tlkapi_printf(APP_LOG_EN, "[FWD] rx pkt #%u, len=%u\n", ++pkt_cnt, len);

    bt_music_forward_copy_data_to_a2dp_buffer(p_data, len);
    if (s_bt_music_fwd_ctrl.sbc.sample_rate == 0u) {
        bt_music_forward_update_sbc_param(p_data + 13);
    }

    uint8_t frame_num = p_data[A2DP_SBC_MEDIA_HDR_OFF] & SBC_NUM_FRAMES_MASK;

    // tlkapi_printf(APP_LOG_EN, "[FWD] frame num %u", frame_num);
    if (frame_num > 0u && frame_num != s_bt_music_fwd_ctrl.frame_num && s_bt_music_fwd_ctrl.state != FWD_STATE_RUNNING) {
        s_bt_music_fwd_ctrl.frame_num = frame_num;
        fwd_ctrl_recalc_base();
        tlkapi_printf(APP_LOG_EN, "[FWD] n=%u base=%uus\r\n", frame_num, s_bt_music_fwd_ctrl.base_us);
    }

    if (s_bt_music_fwd_ctrl.state == FWD_STATE_BUFFERING) {
        uint16_t avail_frames = bt_music_forward_get_available_frames();
        if (avail_frames >= A2DP_FWD_STARTUP_THRESH) {
            s_bt_music_fwd_ctrl.state          = FWD_STATE_RUNNING;
            s_bt_music_fwd_ctrl.current_us     = s_bt_music_fwd_ctrl.base_us;
            s_bt_music_fwd_ctrl.level_avg_x100 = (int32_t)avail_frames * 100;
            bt_music_forward_timer_start(s_bt_music_fwd_ctrl.current_us);
            tlkapi_printf(APP_LOG_EN, "[FWD] startup done, avail_frames=%u, timer=%uus\r\n", avail_frames, s_bt_music_fwd_ctrl.current_us);
        }
    }
}

/* ========================================================================== */
/*  A2DP TX handler (called in BT host task loop)                             */
/* ========================================================================== */
void bt_music_forward_a2dp_tx_evt_handler(void)
{
    if (s_bt_music_fwd_ctrl.state != FWD_STATE_RUNNING) {
        return;
    }

    if (!bt_music_forward_is_a2dp_tx_evt_set()) {
        return;
    }

#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PA1, 0);
    gpio_write(GPIO_PA1, 1);
    gpio_write(GPIO_PA1, 0);
#endif

    bt_music_forward_clear_a2dp_tx_evt();

    int cur_level = (int)bt_music_forward_get_available_frames();
    fwd_ctrl_update_avg(cur_level);
    int smooth_level = (int)(s_bt_music_fwd_ctrl.level_avg_x100 / 100);

    uint16_t len        = 0;
    uint8_t *p_a2dp_buf = bt_music_forward_get_data_from_a2dp_buffer(&len);

    //tlkapi_printf(APP_LOG_EN, "len=%u, %x %x %x\r\n", len, p_a2dp_buf[0], p_a2dp_buf[1], p_a2dp_buf[2]);

    if (len == 0 || p_a2dp_buf == NULL) {
        tlkapi_printf(APP_LOG_EN, "[FWD] buffer empty, sending silence\r\n");
        // uint16_t acl_handle = tlkmdi_bt_music_forward_get_acl_handle();
        // bt_music_forward_send_silence(acl_handle);
    }

    if (p_a2dp_buf != NULL && len > A2DP_RTP_HDR_SIZE) {
        uint16_t acl_handle = tlkmdi_bt_music_forward_get_acl_handle();
        uint16_t seq_num    = READ_U16_BE(p_a2dp_buf, 2);
        uint32_t time_stamp = READ_U32_BE(p_a2dp_buf, 4);

#if SCO_FORWARD_GPIO_DEBUG
        gpio_write(GPIO_PB5, 0);
        gpio_write(GPIO_PB5, 1);
        gpio_write(GPIO_PB5, 0);
#endif

#if (TLKBTP_CFG_A2DPSRC_ENABLE)
        btp_a2dpsrc_sendMediaData(acl_handle, seq_num, time_stamp, p_a2dp_buf + A2DP_RTP_HDR_SIZE, len - A2DP_RTP_HDR_SIZE);
#endif
    } else {
        tlkapi_printf(APP_LOG_EN, "[FWD] buffer empty, sending silence\r\n");
        // uint16_t acl_handle = tlkmdi_bt_music_forward_get_acl_handle();
        // bt_music_forward_send_silence(acl_handle);
    }

    s_bt_music_fwd_ctrl.current_us = fwd_ctrl_calc_interval(smooth_level);
    bt_music_forward_timer_reload(s_bt_music_fwd_ctrl.current_us);

    static uint16_t debug_cnt = 0;
    if (++debug_cnt >= 100) {
        debug_cnt = 0;
        tlkapi_printf(APP_LOG_EN, "[FWD] Lvl:%d, Avg:%d, Interval:%uus\r\n", cur_level, smooth_level, s_bt_music_fwd_ctrl.current_us);
    }

    // if (cur_level < 3) {
    //     tlkapi_printf(APP_LOG_EN, "[FWD] cur_level:%d\r\n", cur_level);
    // }
}

void bt_music_forward_switch_in(uint16_t handle)
{
    (void)handle;

    s_bt_a2dp_buf_ptr = (uint8_t *)tlkmdi_audmem_calloc(A2DP_BUFF_TOTAL_SIZE);
    if (s_bt_a2dp_buf_ptr == NULL) {
        tlkapi_printf(APP_LOG_EN, "[FWD] malloc failed\r\n");
        return;
    }
    bt_music_forward_a2dp_buff_init(s_bt_a2dp_buf_ptr);

    uint8_t *p = (uint8_t *)&s_bt_music_fwd_ctrl;
    for (int i = 0; i < (int)sizeof(s_bt_music_fwd_ctrl); i++) {
        p[i] = 0;
    }
    s_bt_music_fwd_ctrl.state          = FWD_STATE_BUFFERING;
    s_bt_music_fwd_ctrl.frame_num      = 5u;
    s_bt_music_fwd_ctrl.base_us        = A2DP_FWD_DEFAULT_BASE_US;
    s_bt_music_fwd_ctrl.current_us     = A2DP_FWD_DEFAULT_BASE_US;
    s_bt_music_fwd_ctrl.level_avg_x100 = A2DP_FWD_TARGET_LEVEL * 100;

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
    btp_a2dpsnk_regRecvDataCB(bt_music_forward_receive_a2dp_packet);
#endif

    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, bt_music_forward_audio_irq_task);

    tlkapi_printf(APP_LOG_EN, "[FWD] switch in, buffering...\r\n");
}

void bt_music_forward_switch_out(uint16_t handle)
{
    (void)handle;

    bt_music_forward_timer_stop();
    s_bt_music_fwd_ctrl.state = FWD_STATE_IDLE;
    s_a2dp_tx_evt             = 0;

    if (s_bt_a2dp_buf_ptr != NULL) {
        tlkmdi_audmem_free(s_bt_a2dp_buf_ptr);
        s_bt_a2dp_buf_ptr = NULL;
    }

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
    btp_a2dpsnk_regRecvDataCB(NULL);
#endif

    tlkapi_printf(APP_LOG_EN, "[FWD] switch out\r\n");
}

/* ========================================================================== */
/*  Ring buffer operations                                                    */
/* ========================================================================== */

void bt_music_forward_a2dp_buff_init(uint8_t *p_a2dp_buff)
{
    g_a2dp_buf_ctx.p_a2dp_buff          = p_a2dp_buff;
    g_a2dp_buf_ctx.a2dp_buff_frame_size = A2DP_BUFF_FRAME_SIZE;
    g_a2dp_buf_ctx.a2dp_buff_num        = A2DP_BUFF_NUM;
    g_a2dp_buf_ctx.a2dp_buff_wptr       = 0;
    g_a2dp_buf_ctx.a2dp_buff_rptr       = 0;
}

uint8_t bt_music_forward_copy_data_to_a2dp_buffer(uint8_t *p_a2dp_buff, uint16_t len)
{
    if (p_a2dp_buff == NULL || len == 0) {
        return 0;
    }

    if (bt_music_forward_get_free_frames() == 0) {
        tlkapi_printf(APP_LOG_EN, "[FWD] buffer full, dropping packet\r\n");
        return 0;
    }

    uint16_t offset = g_a2dp_buf_ctx.a2dp_buff_wptr * g_a2dp_buf_ctx.a2dp_buff_frame_size;
    uint8_t *p_buff = g_a2dp_buf_ctx.p_a2dp_buff + offset;

    uint16_t copy_len = (len <= g_a2dp_buf_ctx.a2dp_buff_frame_size) ? len : g_a2dp_buf_ctx.a2dp_buff_frame_size;

    p_buff[0] = len & 0xFF;
    p_buff[1] = len >> 8;
    tmemcpy(p_buff + 2, p_a2dp_buff, copy_len);

    //tlkapi_printf(APP_LOG_EN, "rx len=%u, %x %x %x\r\n", len, p_a2dp_buff[0], p_a2dp_buff[1], p_a2dp_buff[2]);

    g_a2dp_buf_ctx.a2dp_buff_wptr = (g_a2dp_buf_ctx.a2dp_buff_wptr + 1u) % g_a2dp_buf_ctx.a2dp_buff_num;

    return (uint8_t)copy_len;
}

uint8_t *bt_music_forward_get_data_from_a2dp_buffer(uint16_t *p_len)
{
    if (g_a2dp_buf_ctx.a2dp_buff_rptr == g_a2dp_buf_ctx.a2dp_buff_wptr) {
        if (p_len != NULL) {
            *p_len = 0;
        }
        return NULL;
    }

    uint16_t offset = g_a2dp_buf_ctx.a2dp_buff_rptr * g_a2dp_buf_ctx.a2dp_buff_frame_size;
    uint8_t *p_buff = g_a2dp_buf_ctx.p_a2dp_buff + offset;

    g_a2dp_buf_ctx.a2dp_buff_rptr = (g_a2dp_buf_ctx.a2dp_buff_rptr + 1u) % g_a2dp_buf_ctx.a2dp_buff_num;

    uint16_t len = ((uint16_t)p_buff[1] << 8) | p_buff[0];
    if (p_len != NULL) {
        *p_len = len;
    }

    return p_buff + 2;
}

uint8_t *bt_music_forward_read_data_from_a2dp_buffer(void)
{
    if (g_a2dp_buf_ctx.a2dp_buff_rptr == g_a2dp_buf_ctx.a2dp_buff_wptr) {
        return NULL;
    }
    uint16_t offset = g_a2dp_buf_ctx.a2dp_buff_rptr * g_a2dp_buf_ctx.a2dp_buff_frame_size;
    return g_a2dp_buf_ctx.p_a2dp_buff + offset;
}

uint16_t bt_music_forward_get_available_frames(void)
{
    if (g_a2dp_buf_ctx.a2dp_buff_wptr >= g_a2dp_buf_ctx.a2dp_buff_rptr) {
        return g_a2dp_buf_ctx.a2dp_buff_wptr - g_a2dp_buf_ctx.a2dp_buff_rptr;
    } else {
        return g_a2dp_buf_ctx.a2dp_buff_num - g_a2dp_buf_ctx.a2dp_buff_rptr + g_a2dp_buf_ctx.a2dp_buff_wptr;
    }
}

uint16_t bt_music_forward_get_free_frames(void)
{
    return g_a2dp_buf_ctx.a2dp_buff_num - bt_music_forward_get_available_frames() - 1u;
}

void bt_music_forward_reset_a2dp_buffer(void)
{
    g_a2dp_buf_ctx.a2dp_buff_rptr = 0;
    g_a2dp_buf_ctx.a2dp_buff_wptr = 0;
}

/* ========================================================================== */
/*  Internal: dynamic interval computation                                    */
/* ========================================================================== */

// static void fwd_ctrl_recalc_base(void)
// {
//     if (s_fwd_ctrl.sbc.sample_rate == 0u || s_fwd_ctrl.frame_num == 0u) {
//         return;
//     }

//     uint64_t frame_us = ((uint64_t)s_fwd_ctrl.sbc.blocks
//                        * (uint64_t)s_fwd_ctrl.sbc.subbands
//                        * 1000000ULL)
//                        / s_fwd_ctrl.sbc.sample_rate;

//     s_fwd_ctrl.base_us    = (uint32_t)(frame_us * (uint64_t)s_fwd_ctrl.frame_num);
//     s_fwd_ctrl.current_us = s_fwd_ctrl.base_us;
// }

static void fwd_ctrl_recalc_base(void)
{
    if (s_bt_music_fwd_ctrl.sbc.sample_rate == 0u || s_bt_music_fwd_ctrl.frame_num == 0u) {
        return;
    }
    uint64_t frame_us = ((uint64_t)s_bt_music_fwd_ctrl.sbc.blocks * (uint64_t)s_bt_music_fwd_ctrl.sbc.subbands * 1000000ULL) / s_bt_music_fwd_ctrl.sbc.sample_rate;
    uint32_t new_base = (uint32_t)(frame_us * (uint64_t)s_bt_music_fwd_ctrl.frame_num);

    if (new_base > s_bt_music_fwd_ctrl.base_us * 120 / 100) {
        new_base = s_bt_music_fwd_ctrl.base_us * 120 / 100;
    } else if (new_base < s_bt_music_fwd_ctrl.base_us * 80 / 100) {
        new_base = s_bt_music_fwd_ctrl.base_us * 80 / 100;
    }

    s_bt_music_fwd_ctrl.base_us    = new_base;
    s_bt_music_fwd_ctrl.current_us = s_bt_music_fwd_ctrl.base_us;
}

static void fwd_ctrl_update_avg(int cur_level)
{
    int32_t alpha                      = (int32_t)A2DP_FWD_AVG_ALPHA;
    s_bt_music_fwd_ctrl.level_avg_x100 = (s_bt_music_fwd_ctrl.level_avg_x100 * (100 - alpha) + cur_level * 100 * alpha) / 100;
}

static uint32_t fwd_ctrl_calc_interval(int smooth_level)
{
    if (smooth_level <= 3) {
        return s_bt_music_fwd_ctrl.base_us * 2;
    }
    if (smooth_level <= 5) {
        return s_bt_music_fwd_ctrl.base_us + A2DP_FWD_MAX_DELTA_US;
    }

    int32_t delta = smooth_level - (int32_t)A2DP_FWD_TARGET_LEVEL;

    int32_t eff = delta;
    int32_t adj = -eff * (int32_t)A2DP_FWD_STEP_US;
    adj         = CLAMP(adj, -(int32_t)A2DP_FWD_MAX_DELTA_US, (int32_t)A2DP_FWD_MAX_DELTA_US);

    int32_t next = (int32_t)s_bt_music_fwd_ctrl.base_us + adj;
    next         = CLAMP(next, (int32_t)(s_bt_music_fwd_ctrl.base_us / 2u), (int32_t)(s_bt_music_fwd_ctrl.base_us * 2u));

    return (uint32_t)next;
}

/*
static uint32_t fwd_ctrl_calc_interval(int smooth_level)
{
    int32_t delta = smooth_level - (int32_t)A2DP_FWD_TARGET_LEVEL;

    if (delta > -(int32_t)A2DP_FWD_DEAD_ZONE &&
        delta <  (int32_t)A2DP_FWD_DEAD_ZONE) {
        return s_fwd_ctrl.base_us;
    }

    int32_t eff;
    if (delta > 0) {
        eff =  (delta - (int32_t)A2DP_FWD_DEAD_ZONE);
    } else {
        eff = -((-delta) - (int32_t)A2DP_FWD_DEAD_ZONE);
    }

    int32_t adj = -eff * (int32_t)A2DP_FWD_STEP_US;
    adj = CLAMP(adj, -(int32_t)A2DP_FWD_MAX_DELTA_US,
                      (int32_t)A2DP_FWD_MAX_DELTA_US);

    int32_t next = (int32_t)s_fwd_ctrl.base_us + adj;
    next = CLAMP(next, (int32_t)(s_fwd_ctrl.base_us / 2u),
                       (int32_t)(s_fwd_ctrl.base_us * 2u));

    return (uint32_t)next;
}
*/

#endif
