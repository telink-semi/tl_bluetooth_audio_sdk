/********************************************************************************************************
 * @file    bt_voice_forward.c
 *
 * @brief   This is the source file for TLSR/TL
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

#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpt_state.h"

#define MAX_SCO_CHANNELS  2
#define SCO_TX_TIMEOUT_US 10000

typedef struct
{
    bool     active;
    uint32_t expire_tick;
} sco_tx_timer_t;

extern uint8_t voice_msbc_silence_pkt[60];
extern void    btif_sco_enc_buff_init(uint8_t queue_id, uint8_t *p_sco_buff);

static uint8_t s_sco_rx_evt[SCO_ENC_QUEUE_NUM];

static sco_tx_timer_t g_sco_timers[MAX_SCO_CHANNELS];
static bool           g_timer_running = false;

#if (SCO_FORWARD_WITH_ALG)
static uint8_t *s_alg_voice_forward_dec_buff = NULL;
static uint8_t *s_alg_voice_forward_enc_buff = NULL;
#endif

/**
 * @brief   Switch in handler for BT voice forwarding mode
 * @param[in]  handle - connection handle
 * @returns    None
 */
void bt_voice_forward_switch_in(uint16_t handle)
{
    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "@ bt_voice_forward_switch_in: %d", handle);

#if (SCO_FORWARD_WITH_ALG)
    bool ret = bt_voice_audio_path_init();
    if (!ret) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "bt_voice_audio_path_init fail: %d", ret);
        return;
    }

    bt_voice_cfg.dec_forward_func = NULL;
    bt_voice_cfg.enc_forward_func = NULL;
    audio_alg_interface_t *p_forward_enc_alg_if;
    audio_alg_interface_t *p_forward_dec_alg_if;
    void                  *p_forward_enc_func;
    void                  *p_forward_dec_func;

    // uint8_t hf_codec = btif_get_hfp_codec(SCO_ENC_QUEUE_ID_HF);

    if (btif_get_hfp_codec(SCO_ENC_QUEUE_ID_AG) == HFP_CODEC_ID_CVSD) {
        p_forward_enc_alg_if = audio_alg_get_interface_by_type(ALG_CVSD_ENC_CH2);
        p_forward_dec_alg_if = audio_alg_get_interface_by_type(ALG_CVSD_DEC_CH2);

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "@forward enter CVSD");
    } else {
        p_forward_enc_alg_if = audio_alg_get_interface_by_type(ALG_MSBC_ENC_CH2);
        p_forward_dec_alg_if = audio_alg_get_interface_by_type(ALG_MSBC_DEC_CH2);

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "@forward enter MSBC");
    }

    p_forward_enc_func = p_forward_enc_alg_if->audio_alg_process;
    p_forward_dec_func = p_forward_dec_alg_if->audio_alg_process;

    extern void bt_voice_register_forward_alg_callback(void *p_enc_func, void *p_dec_func);
    bt_voice_register_forward_alg_callback(p_forward_enc_func, p_forward_dec_func);

    /* forward encode alg init */
    if (s_alg_voice_forward_enc_buff == NULL) {
        s_alg_voice_forward_enc_buff = (uint8_t *)tlkalg_malloc_func(p_forward_enc_alg_if->audio_alg_get_size(0));
        p_forward_enc_alg_if->audio_alg_init(s_alg_voice_forward_enc_buff, 0);
    }

    /* forward decode alg init */
    if (s_alg_voice_forward_dec_buff == NULL) {
        s_alg_voice_forward_dec_buff = (uint8_t *)tlkalg_malloc_func(p_forward_dec_alg_if->audio_alg_get_size(0));
        p_forward_dec_alg_if->audio_alg_init(s_alg_voice_forward_dec_buff, 0);
    }

#else
    for (uint8_t i = 0; i < SCO_ENC_QUEUE_NUM; i++) {
        bt_voice_cfg.spk_enc_wptr[i]      = 0;
        bt_voice_cfg.spk_enc_rptr[i]      = 0;
        bt_voice_cfg.mic_enc_wptr[i]      = 0;
        bt_voice_cfg.mic_enc_rptr[i]      = 0;
        bt_voice_cfg.sco_forward_state[i] = BT_VOICE_FORWARD_ST_IDLE;
    }

    bt_voice_cfg.sco_rx_tick = 0;
    bt_voice_cfg.status      = BT_VOICE_ST_IDLE;
    bt_voice_cfg.init        = 0;
    bt_voice_cfg.sync_init   = 0;
    bt_voice_cfg.mic_state   = 0;

#if SCO_ENC_QUEUE_ENABLE
    for (uint8_t i = 0; i < SCO_ENC_QUEUE_NUM; i++) {
        btif_sco_enc_buff_init(i, g_sco_data_buff[i]);
    }
#endif

    uint8_t *p_bt_voice_enc_buf = (uint8_t *)tlkmdi_audmem_calloc(TLKMDI_BT_VOICE_ENC_BUFF_SIZE);
    if (p_bt_voice_enc_buf == NULL) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "malloc voice enc buff failed");
    } else {
        bt_voice_set_buffer(p_bt_voice_enc_buf);
    }
#endif

    tlkmw_audio_btif_inform_host_audio_en(handle, false);
    bt_audio_set_voice_vol_percent_by_handle(handle);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, bt_voice_forward_audio_irq_task);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, bt_voice_forward_main_loop);
    bt_audio_task_register_run_cb(NULL, 1);
    tlkmdi_audio_stop_timer();
    bt_voice_forward_sco_timer_init();

    bt_voice_set_mode(BT_VOICE_FORWARD_MODE);
}

/**
 * @brief   Switch out handler for BT voice forwarding mode
 * @param[in]  handle - connection handle
 * @returns    None
 */
void bt_voice_forward_switch_out(uint16_t handle)
{
    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "@ bt_voice_forward_switch_out: %d", handle);

    bt_audio_task_register_run_cb(NULL, 0);
    bt_audio_register_get_pcm_data_callback(NULL);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, NULL);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, NULL);
    tlkmw_audio_btif_inform_host_audio_dis(handle);

#if (SCO_FORWARD_WITH_ALG)
    bool ret = bt_voice_audio_path_deinit();
    if (!ret) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "bt_voice_audio_path_deinit fail: %d", ret);
    }

    audio_alg_interface_t *p_forward_enc_alg_if;
    audio_alg_interface_t *p_forward_dec_alg_if;

    if (btif_get_hfp_codec(SCO_ENC_QUEUE_ID_AG) == HFP_CODEC_ID_CVSD) {
        p_forward_enc_alg_if = audio_alg_get_interface_by_type(ALG_CVSD_ENC_CH2);
        p_forward_dec_alg_if = audio_alg_get_interface_by_type(ALG_CVSD_DEC_CH2);
    } else {
        p_forward_enc_alg_if = audio_alg_get_interface_by_type(ALG_MSBC_ENC_CH2);
        p_forward_dec_alg_if = audio_alg_get_interface_by_type(ALG_MSBC_DEC_CH2);
    }

    extern void bt_voice_register_forward_alg_callback(void *p_enc_func, void *p_dec_func);
    bt_voice_register_forward_alg_callback(NULL, NULL);

    if (s_alg_voice_forward_dec_buff != NULL) {
        tlkalg_free_func(s_alg_voice_forward_dec_buff);
        p_forward_enc_alg_if->audio_alg_deinit();
        s_alg_voice_forward_dec_buff = NULL;
    }

    /* decode alg deinit */
    if (s_alg_voice_forward_enc_buff != NULL) {
        tlkalg_free_func(s_alg_voice_forward_enc_buff);
        p_forward_dec_alg_if->audio_alg_deinit();
        s_alg_voice_forward_enc_buff = NULL;
    }
#else
    bt_voice_cfg_t *p_bt_voice_env = get_bt_voice_env_ptr();
    p_bt_voice_env->sco_data_ready = 0;
    bt_voice_reset();
    uint8_t *p_bt_voice_enc_buf = bt_voice_get_buffer();
    if (p_bt_voice_enc_buf != NULL) {
        tlkmdi_audmem_free(p_bt_voice_enc_buf);
        bt_voice_set_buffer(NULL);
    }
#endif
}

audio_ram_code void bt_voice_forward_process_uplink(uint8_t sco_queue_id)
{
    (void)sco_queue_id;
}

audio_ram_code void bt_voice_forward_process_downlink(uint8_t sco_queue_id)
{
    (void)sco_queue_id;
}

void bt_voice_forward_set_sco_rx_evt(uint8_t queue_id)
{
    if (queue_id < SCO_ENC_QUEUE_NUM) {
        s_sco_rx_evt[queue_id] = 1;
    }
}

void bt_voice_forward_clear_sco_rx_evt(uint8_t queue_id)
{
    if (queue_id < SCO_ENC_QUEUE_NUM) {
        s_sco_rx_evt[queue_id] = 0;
    }
}

uint8_t bt_voice_forward_is_sco_rx_evt_set(uint8_t queue_id)
{
    return s_sco_rx_evt[queue_id];
}

audio_ram_code static void stop_hardware_timer(void)
{
    if (g_timer_running) {
        tlkmdi_audio_stop_timer();
        g_timer_running = false;
    }
}

audio_ram_code static void schedule_next_sco_tx_timer(void)
{
    stop_hardware_timer();

    uint32_t cur_tick       = clock_time();
    uint32_t soonest_expire = 0xFFFFFFFF;
    uint8_t  soonest_id     = 0xFF;

    for (uint8_t i = 0; i < MAX_SCO_CHANNELS; i++) {
        if (g_sco_timers[i].active && g_sco_timers[i].expire_tick < soonest_expire) {
            soonest_expire = g_sco_timers[i].expire_tick;
            soonest_id     = i;
        }
    }

    if (soonest_id == 0xFF) {
        return;
    }

    int32_t tick_diff = (int32_t)(soonest_expire - cur_tick);
    if (tick_diff <= 0) {
        return;
    }
    uint32_t tus = tick_diff / TICK_PER_US;
    tlkmdi_audio_task_set_next_irq(tus);
    g_timer_running = true;
}

void bt_voice_forward_sco_start_timer(uint8_t queue_id)
{
    if (queue_id >= MAX_SCO_CHANNELS) {
        return;
    }

    g_sco_timers[queue_id].expire_tick = clock_time() + SCO_TX_TIMEOUT_US * TICK_PER_US;
    g_sco_timers[queue_id].active      = true;
    schedule_next_sco_tx_timer();
}

void bt_voice_forward_sco_stop_timer(uint8_t queue_id)
{
    if (queue_id >= MAX_SCO_CHANNELS) {
        return;
    }

    g_sco_timers[queue_id].active = false;
    schedule_next_sco_tx_timer();
}

void bt_voice_forward_sco_timer_init(void)
{
    for (int i = 0; i < MAX_SCO_CHANNELS; i++) {
        g_sco_timers[i].active = false;
    }
    stop_hardware_timer();
}

audio_ram_code void bt_voice_forward_audio_irq_task_for_channel(uint8_t queue_id)
{
    uint8_t sco_data[64];

    uint16_t offset        = g_sco_enc_buf_ctx[queue_id].sco_enc_buff_rptr * g_sco_enc_buf_ctx[queue_id].sco_enc_buff_frame_size;
    uint8_t *p_sco_enc_buf = g_sco_enc_buf_ctx[queue_id].p_sco_enc_buff + offset;

    uint16_t sco_conhdl_flags = p_sco_enc_buf[1] | (p_sco_enc_buf[2] << 8);
    uint16_t sco_handle       = sco_conhdl_flags & 0x0FFF;
    btif_voice_get_sco_frame(0, sco_handle, sco_data, 60);

    uint8_t tlk_bt_ctrl_h0tl_h2c_write_sync_data(uint16_t conhdl_flags, uint8_t data_total_len, uint8_t * data);
    tlk_bt_ctrl_h0tl_h2c_write_sync_data(sco_conhdl_flags, 60, sco_data);
}

audio_ram_code void bt_voice_forward_audio_irq_task(void)
{
    uint32_t cur_tick = clock_time();

    for (uint8_t i = 0; i < MAX_SCO_CHANNELS; i++) {
        if (g_sco_timers[i].active) {
            if ((int32_t)(cur_tick - g_sco_timers[i].expire_tick) >= 0) {
                g_sco_timers[i].active = false;
                bt_voice_forward_audio_irq_task_for_channel(i);
            }
        }
    }

    //schedule_next_sco_tx_timer();
}

/**
 * @brief  called by bt_voice_forward_main_loop
 * @param[in]  queue_id
 * @returns none
 */
audio_ram_code void bt_voice_forward_sco_rx_evt_handler(uint8_t queue_id)
{
    uint8_t sco_data[128];
    uint8_t sco_enc_buff_rcv = btif_get_sco_enc_buff_available(queue_id);

    if (sco_enc_buff_rcv) {
        uint16_t offset           = g_sco_enc_buf_ctx[queue_id].sco_enc_buff_rptr * g_sco_enc_buf_ctx[queue_id].sco_enc_buff_frame_size;
        uint8_t *p_sco_enc_buf    = g_sco_enc_buf_ctx[queue_id].p_sco_enc_buff + offset;
        uint16_t sco_conhdl_flags = p_sco_enc_buf[1] | (p_sco_enc_buf[2] << 8);
        uint16_t sco_handle       = sco_conhdl_flags & 0x0FFF;

        btif_update_sco_enc_buff_rptr(queue_id);
        btif_voice_recv_sco_frame(0, sco_conhdl_flags, p_sco_enc_buf, 60);
        btif_voice_get_sco_frame(0, sco_handle, sco_data, 60);

        uint8_t tlk_bt_ctrl_h0tl_h2c_write_sync_data(uint16_t conhdl_flags, uint8_t data_total_len, uint8_t * data);
        tlk_bt_ctrl_h0tl_h2c_write_sync_data(sco_conhdl_flags, 60, sco_data);
        bt_voice_forward_sco_start_timer(queue_id);
    }
}

/**
 * @brief  bt audio main loop process
 * @param[in]  none
 * @returns none
 */
audio_ram_code void bt_voice_forward_main_loop(void)
{
#if (TLKBTP_CFG_HFP_ENABLE)
    if (tlkmdi_bt_voice_forward_isBusy()) {
        if (!bt_audio_irq_task_is_running()) {
            if (bt_voice_forward_is_sco_rx_evt_set(SCO_ENC_QUEUE_ID_HF)) {
                bt_voice_forward_sco_rx_evt_handler(SCO_ENC_QUEUE_ID_HF);
                bt_voice_forward_clear_sco_rx_evt(SCO_ENC_QUEUE_ID_HF);
            }

            if (bt_voice_forward_is_sco_rx_evt_set(SCO_ENC_QUEUE_ID_AG)) {
                bt_voice_forward_sco_rx_evt_handler(SCO_ENC_QUEUE_ID_AG);
                bt_voice_forward_clear_sco_rx_evt(SCO_ENC_QUEUE_ID_AG);
            }
        }
    }
#endif
}

/**
 * @brief  SCO DEC -> PCM -> SCO ENC, called by bt_audio_main
 * @param[in]  pcm buffer
 * @returns none
 */
audio_ram_code uint16_t bt_voice_forward_mode_proc_sco_data(int16_t *p_des0, int16_t *p_des1)
{
#if (SCO_FORWARD_WITH_ALG)
    uint16_t ret = 0;
    uint8_t *p_enc_src;
    uint8_t  queue_id  = SCO_ENC_QUEUE_ID_HF;
    int16_t *p_pcm_buf = p_des0;
    (void)p_des1;

    queue_id = bt_voice_cfg.sco_proc_id;

    uint16_t offset                     = bt_voice_cfg.spk_enc_rptr[queue_id] * SPK_ENC_BUFF_SIZE;
    p_enc_src                           = g_spk_enc_buff_ptr[queue_id] + offset;
    bt_voice_cfg.spk_enc_rptr[queue_id] = (bt_voice_cfg.spk_enc_rptr[queue_id] + 1) % SPK_ENC_BUFF_NUM_FORWARD;

#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PB5, 1);
#endif
    if (queue_id == SCO_ENC_QUEUE_ID_HF) {
        ret = bt_voice_cfg.dec_func(p_enc_src, (uint8_t *)p_pcm_buf, 60, 0, 1);
    } else if (queue_id == SCO_ENC_QUEUE_ID_AG) {
        ret = bt_voice_cfg.dec_forward_func(p_enc_src, (uint8_t *)p_pcm_buf, 60, 0, 1);
    }

#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PB5, 0);
#endif

    if (!ret) {
        if (HFP_CODEC_ID_MSBC == btif_get_hfp_codec(queue_id)) {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "#bt_MSBC_DEC %d", ret);
        } else {
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "#bt_CVSD_DEC %d", ret);
        }
    }

    offset             = bt_voice_cfg.mic_enc_wptr[queue_id] * SPK_ENC_BUFF_SIZE;
    uint8_t *p_enc_des = g_mic_enc_buff_ptr[queue_id] + offset;
#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PA2, 1);
#endif

    if (queue_id == SCO_ENC_QUEUE_ID_HF && bt_voice_cfg.enc_forward_func != NULL) {
        bt_voice_cfg.enc_forward_func((uint8_t *)p_pcm_buf, p_enc_des, 240, 0, 3);
    } else if (queue_id == SCO_ENC_QUEUE_ID_AG && bt_voice_cfg.enc_func != NULL) {
        bt_voice_cfg.enc_func((uint8_t *)p_pcm_buf, p_enc_des, 240, 0, 3);
    }

#if SCO_FORWARD_GPIO_DEBUG
    gpio_write(GPIO_PA2, 0);
#endif
    bt_voice_cfg.mic_enc_wptr[queue_id] = (bt_voice_cfg.mic_enc_wptr[queue_id] + 1) % SPK_ENC_BUFF_NUM_FORWARD;
    ret                                 = 3;

    return ret;
#else
    (void)p_des0;
    (void)p_des1;
    return 0;
#endif
}
