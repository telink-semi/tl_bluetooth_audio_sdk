/********************************************************************************************************
 * @file    tlkmdi_a2dp_to_bis.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "tlkmdi_a2dp_to_bis_bt_music.h"
#include "tlkmdi_a2dp_to_bis_le_music.h"
#include "tlkmdi_a2dp_to_bis.h"

#if (TLK_MW_LEA_A2DP_TO_BIS_ENABLE)
#include "tlkmw/audio/le_audio/le_audio_common.h"
#include "stack/ble/ble.h"
#endif

#if (MCU_DUAL_CORE_ENABLE)
#include "stack/multiCoreComm/service/service_mailbox.h"
#endif

#if (TLK_CFG_A2DP_TO_BIS_ENABLE)


#define TLKMDI_BT_MUSIC_DBG_FLAG            ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_SNK << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BT_MUSIC_DBG_SIGN            "[MDI BT_M]"

#define TLKMDI_BT_MUSIC_ENC_FIFO_SIZE       BT_MUSIC_ENC_FIFO_SIZE
#define TLKMDI_A2DP_TO_BIS_BUFF_MAX_SAMPLES (CODEC_SPK_FIFO_SAMPLES - 1)

STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkmdi_a2dp_to_bis_bt_env_t)));
static tlkmdi_a2dp_to_bis_bt_env_t s_tlk_mdi_a2dp_to_bis_bt_env = {0};
static tlkmdi_a2dp_to_bis_ctl      s_a2dp_to_bis_ctl            = {0};

#if TLKALG_ASRC_441TO16_16BIT_ENABLE
static uint8_t *s_alg_asrc_441to16_buffer = NULL;
#endif
uint8_t                 g_a2dp_to_bis_bt_enable_flag = 0;
tlkmdi_audio_path_scene g_audio_scene                = TLKMDI_AUDIO_PATH_A2DP2BIS;
#if (!TLK_CFG_TEMP_DRAM_OPTM_TPSLL)
codec_int g_middle_spk_buff[CODEC_SPK_FIFO_SAMPLES];
#else
_attribute_iram_data_ codec_int g_middle_spk_buff[CODEC_SPK_FIFO_SAMPLES];
#endif
extern uint32_t dec_intval_us;

#if (MCU_DUAL_CORE_ENABLE)
u32 tlkMdiBigInt = 0;
u32 tlkMdiBigRef = 0;
#endif

/**
 * @brief       Set the audio scene for A2DP to BIS
 * @param[in]   scene - Audio scene to set
 * @return      None
 */
void tlkmdi_a2dp_to_bis_set_audio_scene(tlkmdi_audio_path_scene scene)
{
    if (scene >= TLKMDI_AUDIO_PATH_MAX) {
        return;
    }
    g_audio_scene = scene;
}

/**
 * @brief       Get the current audio scene for A2DP to BIS
 * @param       None
 * @return      Current audio scene
 */
tlkmdi_audio_path_scene tlkmdi_a2dp_to_bis_get_audio_scene(void)
{
    return g_audio_scene;
}

/**
 * @brief       Get ACL handle for A2DP to BIS
 * @param       None
 * @return      ACL handle
 */
uint16_t tlkmdi_a2dp_to_bis_bt_get_acl_handle(void)
{
    return s_tlk_mdi_a2dp_to_bis_bt_env.acl_handle;
}

/**
 * @brief       Check if A2DP to BIS is enabled
 * @param       None
 * @return      Non-zero if enabled, zero if disabled
 */
uint16_t tlkmdi_a2dp_to_bis_is_enabled(void)
{
    return s_tlk_mdi_a2dp_to_bis_bt_env.enable;
}

/**
 * @brief       Initialize buffer and algorithm for A2DP to BIS
 * @param       None
 * @return      true if successful, false otherwise
 */
bool a2dp_to_bis_buff_and_alg_init(void)
{
    bool ret = true;

#if TLKALG_ASRC_441TO16_16BIT_ENABLE
    audio_alg_interface_t *p_audio_alg_if = NULL;
    // p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO48);
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO16_16BIT);
    if (s_alg_asrc_441to16_buffer == NULL) {
        uint16_t asrc_441to16_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        s_alg_asrc_441to16_buffer      = (uint8_t *)tlkalg_malloc_func(asrc_441to16_mem_size);
        p_audio_alg_if->audio_alg_init(s_alg_asrc_441to16_buffer, ALG_CHANNEL_STEREO);

        tlkapi_trace(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "asrc_441to16_mem_size: %d", asrc_441to16_mem_size);
    }
#endif

    return ret;
}

/**
 * @brief       Deinitialize buffer and algorithm for A2DP to BIS
 * @param       None
 * @return      true if successful, false otherwise
 */
bool a2dp_to_bis_buff_and_alg_deinit(void)
{
    bool ret = true;

#if TLKALG_ASRC_441TO16_16BIT_ENABLE
    audio_alg_interface_t *p_audio_alg_if = NULL;
    if (s_alg_asrc_441to16_buffer != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO16_16BIT);
        tlkalg_free_func(s_alg_asrc_441to16_buffer);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_asrc_441to16_buffer = NULL;
    }
#endif
    return ret;
}

/**
 * @brief  receive a2dp data, called by A2DP profile
 *
 * @param[in] aclHandle - ACL connection handle
 * @param[in] p_data - A2DP data ptr
 * @param[in] len    - A2DP packet data len
 * @return BT_AUDIO_SUCCESS is successful, others are failed.
 */
void tlkmdi_a2dp_to_bis_rcv_a2dp_frame(uint16_t aclHandle, uint8_t *p_data, uint16_t len)
{
    (void)aclHandle;

    bt_music_receive_a2dp_frames(p_data, len);
    return;
}

/**
 * @brief       Callback function for volume change events
 * @param[in]   handle - Connection handle
 * @param[in]   vol - Volume level
 * @return      None
 */
static void tlkmdi_a2dp_to_bis_vol_change_cb(uint16_t handle, uint8_t vol)
{
    (void)vol;
    if (handle != s_tlk_mdi_a2dp_to_bis_bt_env.acl_handle) {
        return;
    }
    bt_audio_set_music_vol_percent_by_handle(handle);
}

/**
 * @brief       Callback function for state change events
 * @param[in]   handle - Connection handle
 * @param[in]   state - New state
 * @return      None
 */
static void tlkmdi_a2dp_to_bis_state_change_cb(uint16_t handle, uint8_t state)
{
    if (state == TLK_STATE_OPENED) {
        if (!s_tlk_mdi_a2dp_to_bis_bt_env.enable) {
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_A2DP_TO_BIS, handle);
        } else if (btp_a2dpsnk_getStatus(handle) == BTP_A2DP_STATUS_STREAM) {
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_A2DP_TO_BIS, handle);
        }
    } else if (state == TLK_STATE_CLOSED || TLK_STATE_PAUSED) {
        tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_A2DP_TO_BIS, handle);
    }
}

#if (MCU_DUAL_CORE_ENABLE)
void tlksys_dualcore_a2dp_to_bis_ref_cb(u8 *data)
{
    BYTE_TO_UINT32(tlkMdiBigRef, data);
}

void tlksys_dualcore_a2dp_to_bis_int_cb(u8 *data)
{
    BYTE_TO_UINT32(tlkMdiBigInt, data);
}
#endif

/*
 * @brief  Initial the A2DP sink block, register the data callback.
 * @param[in] None
 * @return TLK_ENONE is success,other value is false.
 */
int tlkmdi_a2dp_to_bis_init(void)
{
    tlkapi_trace(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "tlkmdi_a2dp_to_bis_init");
    tlkmdi_audio_btif_addMusicStateChgCB(tlkmdi_a2dp_to_bis_state_change_cb, false);
    tlkmdi_audio_btif_regMusicVolChgCB(tlkmdi_a2dp_to_bis_vol_change_cb);
#if (MCU_DUAL_CORE_ENABLE)
    tlk_mailbox_register_message_cb(TLK_MESSAGE_FROM_N22_TO_D25F_BCST_INT, tlksys_dualcore_a2dp_to_bis_int_cb);
    tlk_mailbox_register_message_cb(TLK_MESSAGE_FROM_N22_TO_D25F_BCST_REF, tlksys_dualcore_a2dp_to_bis_ref_cb);
#endif
    return TLK_ENONE;
}

/**
 * @brief       Start A2DP to BIS streaming
 * @param[in]   handle - Connection handle
 * @param[in]   param - Additional parameters (unused)
 * @return      TLK_ENONE if successful, -TLK_EREPEAT if already enabled
 */
int tlkmdi_a2dp_to_bis_start(uint16_t handle, uint32_t param)
{
    (void)param;
    if (s_tlk_mdi_a2dp_to_bis_bt_env.enable) {
        return -TLK_EREPEAT;
    }
    tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PLAY);

    return TLK_ENONE;
}

/**
 * @brief       Close A2DP to BIS streaming
 * @param[in]   handle - Connection handle
 * @return      TLK_ENONE
 */
int tlkmdi_a2dp_to_bis_close(uint16_t handle)
{
    tlkapi_trace(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "tlkmdi_a2dp_to_bis_close");

    tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PAUSE);

    return TLK_ENONE;
}

/**
 * @brief       Skip to next track
 * @param       None
 * @return      TLK_ENONE if successful, -TLK_EREPEAT if not enabled
 */
bool tlkmdi_a2dp_to_bis_next(void)
{
    if (!s_tlk_mdi_a2dp_to_bis_bt_env.enable) {
        return -TLK_EREPEAT;
    }

    tlkmdi_audio_btif_avrcp_sendKey(s_tlk_mdi_a2dp_to_bis_bt_env.acl_handle, AUD_BTIF_AVRCP_KEYID_FORWARD);

    return TLK_ENONE;
}

/**
 * @brief       Skip to previous track
 * @param       None
 * @return      TLK_ENONE if successful, -TLK_EREPEAT if not enabled
 */
bool tlkmdi_a2dp_to_bis_previous(void)
{
    if (!s_tlk_mdi_a2dp_to_bis_bt_env.enable) {
        return -TLK_EREPEAT;
    }

    tlkmdi_audio_btif_avrcp_sendKey(s_tlk_mdi_a2dp_to_bis_bt_env.acl_handle, AUD_BTIF_AVRCP_KEYID_BACKWARD);
    return TLK_ENONE;
}

/**
 * @brief       Handle audio operations for A2DP to BIS
 * @param[in]   handle - Connection handle
 * @param[in]   opcode - Operation code
 * @param[in]   pdata - Pointer to data (unused)
 * @param[in]   dataLen - Length of data (unused)
 * @return      true if operation was handled, false otherwise
 */
bool tlkmdi_a2dp_to_bis_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    (void)handle;
    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        tlkmdi_audio_btif_VolumeOperate(handle, true, true);
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        tlkmdi_audio_btif_VolumeOperate(handle, false, true);
    } break;
    case TLKAUD_OPCODE_IS_SUPPORT_TONE_MIX:
    {
        return true;
    }
    default:
    {
        return false;
    } break;
    }
    return true;
}

/**
 * @brief       Process BT operations for A2DP to BIS
 * @param[in]   opcode - Operation code (unused)
 * @param[in]   param - Pointer to parameters (unused)
 * @return      TLK_ENONE if successful, -TLK_EREPEAT if not enabled
 */
bool tlkmdi_a2dp_to_bis_bt_process_operate(uint8_t opcode, void *param)
{
    (void)param;
    (void)opcode;
    if (!s_tlk_mdi_a2dp_to_bis_bt_env.enable) {
        return -TLK_EREPEAT;
    }

    return TLK_ENONE;
}

/**
 * @brief       Mute the middle buffer
 * @param       None
 * @return      None
 */
void tlkmdi_a2dp_to_bis_mute_middle_buff(void)
{
    tmemset(g_middle_spk_buff, 0, CODEC_SPK_FIFO_SAMPLES * sizeof(codec_int));
}

/**
 * @brief       Switch A2DP to BIS state
 * @param[in]   handle - Connection handle
 * @param[in]   status - New status
 * @return      true if successful, false otherwise
 */
bool tlkmdi_a2dp_to_bis_switch(uint16_t handle, uint8_t status)
{
    tlkapi_trace(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "tlkmdi_a2dp_to_bis_switch: handle:%d, status: %d", handle, status);

    if (status == TLK_STATE_CLOSED && s_tlk_mdi_a2dp_to_bis_bt_env.acl_handle != handle) {
        return false;
    }


    tlkapi_trace(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "codec_type: %d", s_tlk_mdi_a2dp_to_bis_bt_env.codec_type);
    if (status == TLK_STATE_OPENED) {
        if (SEPID_AAC == btp_a2dpsnk_getCurrCodec(handle)) {
            s_tlk_mdi_a2dp_to_bis_bt_env.codec_type = SEPID_AAC;
            dec_intval_us                           = 10700;
        } else if (SEPID_SBC == btp_a2dpsnk_getCurrCodec(handle)) {
            dec_intval_us                           = 1500;
            s_tlk_mdi_a2dp_to_bis_bt_env.codec_type = SEPID_SBC;
        }
        a2dp_to_bis_set_dec_ticks(clock_time());
        g_a2dp_to_bis_bt_enable_flag = 1;
        bt_audio_set_music_vol_percent_by_handle(handle);
        s_tlk_mdi_a2dp_to_bis_bt_env.acl_handle = handle;
        s_tlk_mdi_a2dp_to_bis_bt_env.enable     = true;

        bool ret = bt_music_buff_and_alg_init();
#if TLKALG_ASRC_441TO16_16BIT_ENABLE
        ret = a2dp_to_bis_buff_and_alg_init();
#endif
        if (!ret) {
            tlkapi_trace(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "a2dp_to_bis_buff_and_alg_init fail: %d", ret);
            return false;
        }

        tlkmw_audio_btif_inform_host_audio_en(handle, true);
        bt_music_audio_path_init(handle);
#if (TLKBTP_CFG_A2DPSNK_ENABLE)
        btp_a2dpsnk_regRecvDataCB(tlkmdi_a2dp_to_bis_rcv_a2dp_frame);
#endif
        tlkmdi_a2dp_to_bis_mute_middle_buff();

        bt_audio_register_get_pcm_data_callback(bt_music_get_playback_data);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, tlkmdi_a2dp_to_bis_timer_irq_handler);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, a2dp_to_bis_bt_main_loop);
        bt_audio_task_register_run_cb(NULL, 1);

        s_a2dp_to_bis_ctl.sample_rate = btif_a2dp_snk_get_sample_rate(handle);
        if (s_a2dp_to_bis_ctl.sample_rate == 0) {
            //tlkmdi_bt_music_close(handle);
            tlkapi_error(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "mdi get sample rate fail");
            //return false;
        }

        audio_codec_flag_set(CODEC_FLAG_MUSIC, 1);
        bt_music_calibrate_enable(0);
    } else {
        bt_music_close_codec();
        bt_audio_task_register_run_cb(NULL, 0);
        bt_audio_register_get_pcm_data_callback(NULL);
        bt_music_audio_path_deinit();
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, NULL);

        bool ret = bt_music_buff_and_alg_deinit();
#if TLKALG_ASRC_441TO16_16BIT_ENABLE
        ret = a2dp_to_bis_buff_and_alg_deinit();
#endif
        if (!ret) {
            tlkapi_trace(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "a2dp_to_bis_buff_and_alg_deinit fail: %d", ret);
            return false;
        }

        tlkmw_audio_btif_inform_host_audio_dis(handle);
        s_a2dp_to_bis_ctl.bis_sync_flag       = false;
        g_a2dp_to_bis_bt_enable_flag          = 0;
        s_tlk_mdi_a2dp_to_bis_bt_env.enable   = false;
        s_a2dp_to_bis_ctl.timer_big_alig_flag = false; //must stop timer firstly
    }

    return true;
}

/**
 * @brief       Check if A2DP to BIS is busy
 * @param       None
 * @return      true if busy (enabled), false otherwise
 */
bool tlkmdi_a2dp_to_bis_is_busy(void)
{
    return s_tlk_mdi_a2dp_to_bis_bt_env.enable;
}

/**
 * @brief       Start audio timer for A2DP to BIS
 * @param       None
 * @return      None
 */
void tlkmdi_a2dp_to_bis_start_audio_timer(void)
{
    g_bt_audio_task_env.tick_irq = clock_time() + 10000 * SYSTEM_TIMER_TICK_1US;
    tlkmdi_audio_task_set_next_irq(10000);
}

#if (MCU_DUAL_CORE_ENABLE)
u32 tlkmdi_a2dp_to_bis_get_ref(u32 clockTick)
{
    if (tlkMdiBigRef != 0) {
        if ((clockTick - tlkMdiBigRef) < ((u32)BIT(30))) {
            u32 num = (clockTick - tlkMdiBigRef) / tlkMdiBigInt;
            if ((clockTick - tlkMdiBigRef) % tlkMdiBigInt) {
                num = num + 1;
            }
            return (tlkMdiBigRef + num * tlkMdiBigInt);
        } else {
            return tlkMdiBigRef;
        }
    } else {
        return 0;
    }
}

static uint32_t s_a2dp_to_bis_total_delay = 0;

void tlkmdi_a2dp_to_bis_set_total_delay(uint32_t total_delay)
{
    s_a2dp_to_bis_total_delay = total_delay * SYSTEM_TIMER_TICK_1US;
}

uint32_t tlkmdi_a2dp_to_bis_get_timestamp(void)
{
    uint32_t ref = tlkmdi_a2dp_to_bis_get_ref(clock_time());

    return ref + s_a2dp_to_bis_total_delay;
}

// void app_ui_test_api(void)
// {
//     uint8_t temp_buff[1920] = {0};
//     uint16_t sample_num = 480;

//     lea_a2dp_to_bis_input_data_process((int16_t *)temp_buff, sample_num);

//     uint32_t ref = tlkmdi_a2dp_to_bis_get_timestamp();

//     tlk_printf("ref:%x, clock is %x, diff is %d us", ref, clock_time(), (ref-clock_time())/SYSTEM_TIMER_TICK_1US);
//     gpio_toggle(GPIO_PA0);
// }

#endif
/**
 * @brief       Timer interrupt handler for A2DP to BIS
 * @param       None
 * @return      None
 */
// uint32_t bis_sync_ms = 0, bis_sync_sample = 0;
// uint16_t bis_wptr = 0, bis_rptr = 0, bis_ava = 0;
void tlkmdi_a2dp_to_bis_timer_irq_handler(void)
{
    codec_int temp_buff[480];
    int       tdiff_us, tdiff_tick;
    uint16_t  sample_num = 0;
    tlkmdi_audio_stop_timer();

    if (!s_a2dp_to_bis_ctl.timer_big_alig_flag) {
        uint32_t tick_cur                     = clock_time();
        s_a2dp_to_bis_ctl.timer_big_alig_flag = true;
#if (MCU_DUAL_CORE_ENABLE)
        uint32_t ticks = tlkmdi_a2dp_to_bis_get_ref(tick_cur);
#else
        uint32_t ticks = blc_ll_getLatestBigBcstSyncApTick4FutMoments(BIG_HANDLE_0);
#endif

        uint32_t us_val = 0;
        if (ticks != 0) {
            ticks += (2000 * SYSTEM_TIMER_TICK_1US);
            us_val = (ticks - tick_cur) / SYSTEM_TIMER_TICK_1US;
        } else {
            us_val = 10000;
        }

        tlkmdi_audio_task_set_next_irq(us_val);
        g_bt_audio_task_env.tick_irq += us_val * SYSTEM_TIMER_TICK_1US;
        return;
    }

    tdiff_tick = clock_time() - g_bt_audio_task_env.tick_irq;
    tdiff_us   = tdiff_tick / SYSTEM_TIMER_TICK_1US;

    tdiff_us = 10000 - tdiff_us;
    g_bt_audio_task_env.tick_irq += 10000 * SYSTEM_TIMER_TICK_1US;

    tlkmdi_audio_task_set_next_irq(tdiff_us);

// #if !TLKALG_ASRC_441TO48_16BIT_ENABLE
// if (s_a2dp_to_bis_ctl.sample_rate) {
//     sample_num = s_a2dp_to_bis_ctl.sample_rate / 100;
// } else
// #endif
// {
//     sample_num = 480;
// }
#if (TLKALG_ASRC_441TO48_16BIT_ENABLE || AUDIO_HD_HAC_EN)
    sample_num = 480;
#elif TLKALG_ASRC_441TO16_16BIT_ENABLE
    sample_num = 160;
#else
    if (g_codec_cfg.codec_sample_rate) {
        sample_num = g_codec_cfg.codec_sample_rate / 100;
    }
#endif

    tlkmdi_a2dp_to_bis_get_data_middle_buff((uint8_t *)temp_buff, sample_num * sizeof(codec_int));

#if 0
    signed short  bis_sin_48k_stereo[]__attribute__((aligned(4))) = {
        0     , 0     , 4276  , 4276  , 8480  , 8480  , 12539 , 12539 ,
        16383 , 16383 , 19947 , 19947 , 23169 , 23169 , 25995 , 25995 ,
        28377 , 28377 , 30272 , 30272 , 31650 , 31650 , 32486 , 32486 ,
        32767 , 32767 , 32486 , 32486 , 31650 , 31650 , 30272 , 30272 ,
        28377 , 28377 , 25995 , 25995 , 23169 , 23169 , 19947 , 19947 ,
        16383 , 16383 , 12539 , 12539 , 8480  , 8480  , 4276  , 4276  ,
        0     , 0     , -4276 , -4276 , -8480 , -8480 , -12539, -12539,
        -16383, -16383, -19947, -19947, -23169, -23169, -25995, -25995,
        -28377, -28377, -30272, -30272, -31650, -31650, -32486, -32486,
        -32767, -32767, -32486, -32486, -31650, -31650, -30272, -30272,
        -28377, -28377, -25995, -25995, -23169, -23169, -19947, -19947,
        -16383, -16383, -12539, -12539, -8480 , -8480 , -4276 , -4276 ,
    };
    static uint16_t bis_sine_count = 0;
    short *psrc = (short *)temp_buff;
    for (int i = 0; i < sample_num*2; i++) {
        psrc[i] = bis_sin_48k_stereo[bis_sine_count++];
        bis_sine_count %= 96;
    }
#endif

#if (TLK_MW_LEA_A2DP_TO_BIS_ENABLE)

    lea_a2dp_to_bis_input_data_process((int16_t *)temp_buff, sample_num);

    if (!tlkmdi_a2dp_to_bis_get_sync_flag()) {
        // gpio_set_high_level(GPIO_PB7);
#if (MCU_DUAL_CORE_ENABLE)
        uint32_t ticks_diff  = tlkmdi_a2dp_to_bis_get_timestamp() - clock_time();
        uint32_t bis_sync_ms = ticks_diff / SYSTEM_TIMER_TICK_1US / 1000;
        // bis_sync_ms = 20;
        uint32_t bis_sync_sample = (sample_num / 10) * (bis_sync_ms);
#else
        uint32_t bis_sync_sample = 48 * 20;
#endif
        tlkdrv_codec_sync_play_samples(bis_sync_sample);

        // delay_us(1000);
        // bis_wptr = tlkdrv_codec_getSpkOffset()/sizeof(codec_int),
        // bis_rptr = tlkdrv_codec_get_speaker_rptr(),
        // bis_ava = tlkdrv_codec_get_spk_avail_samples();
        tlkmdi_a2dp_to_bis_set_bis_sync_flag();

        // gpio_set_low_level(GPIO_PB7);
    }
#endif

    // uint16_t send_data[3];
    // send_data[0] = tlkdrv_codec_getSpkOffset()/sizeof(codec_int);
    // send_data[1] = tlkdrv_codec_get_speaker_rptr();
    // send_data[2] = tlkdrv_codec_get_spk_avail_samples();
    // spi_master_write_dma(0, (uint8_t *)send_data, 6);
    // while(spi_is_busy(0));


    tlkdrv_codec_fillSpkBuff((uint8_t *)temp_buff, sample_num * sizeof(codec_int));

    // gpio_set_low_level(GPIO_PB0);
}

/**
 * @brief       Get BIS synchronization flag
 * @param       None
 * @return      true if synchronized, false otherwise
 */
bool tlkmdi_a2dp_to_bis_get_sync_flag(void)
{
    return s_a2dp_to_bis_ctl.bis_sync_flag;
}

/**
 * @brief       Set BIS synchronization flag
 * @param       None
 * @return      None
 */
void tlkmdi_a2dp_to_bis_set_bis_sync_flag(void)
{
    s_a2dp_to_bis_ctl.bis_sync_flag = true;
}

/**
 * @brief       Get middle buffer idle size
 * @param       None
 * @return      Idle size in samples
 */
uint32_t tlkmdi_a2dp_to_bis_get_middle_idle_size(void)
{
    uint32_t idle = 0;

    if (s_a2dp_to_bis_ctl.middle_buff_rptr > s_a2dp_to_bis_ctl.middle_buff_wptr) {
        idle = s_a2dp_to_bis_ctl.middle_buff_rptr - s_a2dp_to_bis_ctl.middle_buff_wptr;
    } else {
        idle = CODEC_SPK_FIFO_SAMPLES + s_a2dp_to_bis_ctl.middle_buff_rptr - s_a2dp_to_bis_ctl.middle_buff_wptr;
    }

    return idle;
}

/**
 * @brief       Get middle buffer available samples
 * @param       None
 * @return      Available samples count
 */
uint32_t tlkmdi_a2dp_to_bis_get_middle_available_sample(void)
{
    uint32_t available = 0;

    if (s_a2dp_to_bis_ctl.middle_buff_wptr > s_a2dp_to_bis_ctl.middle_buff_rptr) {
        available = s_a2dp_to_bis_ctl.middle_buff_wptr - s_a2dp_to_bis_ctl.middle_buff_rptr;
    } else {
        available = CODEC_SPK_FIFO_SAMPLES + s_a2dp_to_bis_ctl.middle_buff_wptr - s_a2dp_to_bis_ctl.middle_buff_rptr;
    }

    return available;
}

/**
 * @brief       Get middle buffer write pointer
 * @param       None
 * @return      Write pointer position
 */
uint16_t tlkmdi_a2dp_to_bis_get_mid_buff_wptr(void)
{
    return s_a2dp_to_bis_ctl.middle_buff_wptr;
}

/**
 * @brief       Get middle buffer read pointer
 * @param       None
 * @return      Read pointer position
 */
uint16_t tlkmdi_a2dp_to_bis_get_mid_buff_rptr(void)
{
    return s_a2dp_to_bis_ctl.middle_buff_rptr;
}

/**
 * @brief       Fill middle buffer with data
 * @param[in]   pData - Pointer to data to fill
 * @param[in]   dataLen - Length of data in bytes
 * @return      true if successful, false otherwise
 */
bool tlkmdi_a2dp_to_bis_fill_middle_buff(uint8_t *pData, uint16_t dataLen)
{
    uint32_t wptr;
    //    uint32_t rptr;
    uint16_t unUsed;
    uint16_t offset;
    uint8_t *pBuffer = (uint8_t *)g_middle_spk_buff;

    wptr = s_a2dp_to_bis_ctl.middle_buff_wptr * sizeof(codec_int);
    //    rptr = s_a2dp_to_bis_buff_ctl.middle_buff_rptr * sizeof(codec_int);
    unUsed = tlkmdi_a2dp_to_bis_get_middle_idle_size() * sizeof(codec_int);

    if (unUsed <= dataLen) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "a2dp to bis fill fail: %d %d %d %d", s_a2dp_to_bis_ctl.middle_buff_wptr, s_a2dp_to_bis_ctl.middle_buff_rptr,
                    tlkmdi_a2dp_to_bis_get_middle_idle_size(), dataLen / sizeof(codec_int));
        return false;
    }

    if (wptr + dataLen > CODEC_SPK_FIFO_SAMPLES * sizeof(codec_int)) {
        offset = CODEC_SPK_FIFO_SAMPLES * sizeof(codec_int) - wptr;
    } else {
        offset = dataLen;
    }
    tmemcpy(pBuffer + wptr, pData, offset);
    if (offset < dataLen) {
        tmemcpy(pBuffer, pData + offset, dataLen - offset);
    }

    wptr += dataLen;
    wptr &= (CODEC_SPK_FIFO_SAMPLES * sizeof(codec_int) - 1);

    s_a2dp_to_bis_ctl.middle_buff_wptr = wptr / sizeof(codec_int);

    return true;
}

/**
 * @brief       Get data from middle buffer
 * @param[out]  pData - Pointer to buffer to store data
 * @param[in]   dataLen - Length of data to read in bytes
 * @return      true if successful, false otherwise
 */
bool tlkmdi_a2dp_to_bis_get_data_middle_buff(uint8_t *pData, uint16_t dataLen)
{
    uint32_t wptr;
    uint32_t rptr;
    uint16_t available;
    uint16_t offset;
    uint8_t *pBuffer = (uint8_t *)g_middle_spk_buff;

    wptr      = s_a2dp_to_bis_ctl.middle_buff_wptr * sizeof(codec_int);
    rptr      = s_a2dp_to_bis_ctl.middle_buff_rptr * sizeof(codec_int);
    available = tlkmdi_a2dp_to_bis_get_middle_available_sample() * sizeof(codec_int);

    if (available <= dataLen) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "get fail: %d %d %d %d", rptr, wptr, available, dataLen);
        return false;
    }

    if (rptr + dataLen <= CODEC_SPK_FIFO_SAMPLES * sizeof(codec_int)) {
        offset = dataLen;
    } else {
        offset = CODEC_SPK_FIFO_SAMPLES * sizeof(codec_int) - rptr;
    }
    tmemcpy(pData, pBuffer + rptr, offset);
    if (offset < dataLen) {
        tmemcpy(pData + offset, pBuffer, dataLen - offset);
    }

    rptr += dataLen;
    rptr &= (CODEC_SPK_FIFO_SAMPLES * sizeof(codec_int) - 1);

    s_a2dp_to_bis_ctl.middle_buff_rptr = rptr / sizeof(codec_int);

    return true;
}

/**
 * @brief       Synchronize middle buffer pointers
 * @param[in]   sample - Sample count to synchronize to
 * @return      None
 */
void tlkmdi_a2dp_to_bis_sync_middle_buff(uint16_t sample)
{
    s_a2dp_to_bis_ctl.middle_buff_wptr = (s_a2dp_to_bis_ctl.middle_buff_rptr + sample) & (CODEC_SPK_FIFO_SAMPLES - 1);
}
#endif