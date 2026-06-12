/********************************************************************************************************
 * @file    bt_audio_interface.c
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
#include "tlkmw/audio/tlkmw_audio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/bt_hci.h"

#if (TLKBTP_CFG_HFP_ENABLE || TLKBTP_CFG_A2DPSNK_ENABLE)

#if (SCO_ENC_QUEUE_NUM == 1)
#define ALL_HFP_CODEC_INIT {HFP_CODEC_ID_MSBC}
#elif (SCO_ENC_QUEUE_NUM == 2)
#define ALL_HFP_CODEC_INIT {HFP_CODEC_ID_MSBC, HFP_CODEC_ID_MSBC}
#endif

uint8_t bt_voice_hfp_codec[SCO_ENC_QUEUE_NUM] = ALL_HFP_CODEC_INIT;
uint8_t g_bt_voice_sco_data_is_lost           = 0;

#if SCO_ENC_QUEUE_ENABLE

uint8_t                g_sco_data_buff[SCO_ENC_QUEUE_NUM][SCO_ENC_BUFF_FRAME_SIZE * SCO_ENC_BUFF_NUM];
sco_enc_buff_context_t g_sco_enc_buf_ctx[SCO_ENC_QUEUE_NUM];

#endif

/**
 * @brief  whether poweroff is ongoing
 * @param[in]  none
 * @returns 0-no 1-yes
 */
uint8_t btif_power_off_is_ongoing(void)
{
    return 0;
}

/**
 * @brief  whether has voice
 * @param[in]  none
 * @returns 0-not 1-has voice
 */
uint8_t btif_bt_has_voice(void)
{
    //    extern uint8_t api_bt_get_currernt_has_voice(void);
    //    return api_bt_get_currernt_has_voice();
    return 0;
}

/**
 * @brief  whether has sco
 * @param[in]  none
 * @returns 0-not 1-has sco
 */
uint8_t btif_bt_has_sco(void)
{
    //    extern uint8_t api_bt_get_currernt_has_sco(void);
    //    return api_bt_get_currernt_has_sco();
    return 0;
}

/**
 * @brief  check if a2dp sink is playing
 * @param[in]  handle - ACL connection handle
 * @returns 0-not playing, 1-playing
 */
uint8_t api_bt_get_a2dpsnk_is_playing(uint16_t handle)
{
#if (TLKBTP_CFG_A2DPSNK_ENABLE)
    bool ret_is_playing = false;

    ret_is_playing = btp_avrcp_remoteIsPlaying(handle) || (btp_a2dpsnk_getStatus(handle) == BTP_A2DP_STATUS_STREAM);

    return ret_is_playing;
#else
    (void)handle;
    return 0;
#endif
}

/**
 * @brief  check if device has music
 * @param[in]  none
 * @returns 0-no music, 1-has music
 */
uint8_t app_bt_has_music(void)
{
    uint16_t handle;

    handle = tlkmw_audio_btif_getPlayHandle();

    return api_bt_get_a2dpsnk_is_playing(handle);

#if 0
    uint8_t bt_has_music = 0;
    uint16_t handle;

    if (tlk_tpsll_tph_headset_role_is_single() || tlk_tpsll_tph_headset_is_master()) {
        handle = tlkmw_audio_btif_getPlayHandle();
        bt_has_music = api_bt_get_a2dpsnk_is_playing(handle);

    } else if (tlk_tpsll_tph_headset_is_slave()) {
    }
    return audio_codec_flag_get(CODEC_FLAG_MUSIC) && bt_has_music;
#endif
}

/**
 * @brief  whether has music
 * @param[in]  none
 * @returns 0-not 1-has music
 */
uint8_t btif_bt_has_music(void)
{
    return app_bt_has_music();
}

/**
 * @brief  whether poweroff tick is set
 * @param[in]  none
 * @returns 0-no 1-yes
 */
uint8_t btif_poweroff_tick_is_set(void)
{
    return 0;
}

/**
 * @brief  whether mic is muted
 * @param[in]  none
 * @returns 0-unmute 1-mute
 */
uint8_t btif_mic_is_muted(void)
{
    return 0;
}

/**
 * @brief  get mute flag
 * @param[in]  none
 * @returns 0-unmute 1-mute
 */
uint8_t btif_get_mic_mute_flag(void)
{
    return 0;
}

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
/**
 * @brief  get a2dp sink sample rate
 * @param[in]  acl_handle - ACL connection handle
 * @returns sample rate
 */
uint32_t btif_a2dp_snk_get_sample_rate(uint16_t acl_handle)
{
    return btp_a2dpsnk_getSampleRate(acl_handle);
}
#endif

/**
 * @brief  receive a2dp data, called by A2DP profile
 * @param[in] aclHandle - ACL connection handle
 * @param[in] p_data    - A2DP data ptr
 * @param[in] len       - A2DP packet data len
 * @returns none
 */
void btif_receive_a2dp_frame(uint16_t aclHandle, uint8_t *p_data, uint16_t len)
{
#if TWS_AUDIO_PATH_GPIO_DEBUG
    gpio_write(GPIO_PB0, 0);
    gpio_write(GPIO_PB0, 1);
    gpio_write(GPIO_PB0, 0);
#endif

    uint16_t cur_handle = 0;
    if (tlkmdi_bt_music_is_busy()
#if TLKALG_ANC_ENABLE
        || tlkmdi_anc_btmusic_is_busy()
#endif
    ) {
        cur_handle = tlkmw_audio_btif_getPlayHandle();
    }
#if (TLKMW_INTERPHONE_EN)
    else if (tlkmdi_interphone_is_busy()) {
        cur_handle = tlkmdi_interphone_get_acl_handle();
    }
#endif

    if (cur_handle != aclHandle) {
        return;
    }

    bt_music_receive_a2dp_frames(p_data, len);
}

/**
 * @brief  register a2dp data receive callback
 * @param[in]  none
 * @returns 0-success, others-failure
 */
int btif_register_a2dp_data_recv_callback(void)
{
#if (TLKBTP_CFG_A2DPSNK_ENABLE)
    btp_a2dpsnk_regRecvDataCB(btif_receive_a2dp_frame);
#endif
    return TLK_ENONE;
}

/**
 * @brief  get sco data from stack and push to encode buffer
 * @param[in] id          - frame id
 * @param[in] sco_handle  - sco connection handle
 * @param[in] p_data      - sco data start address
 * @param[in] len         - sco data length
 * @returns none
 */
#if (TLKBTP_CFG_HFP_ENABLE)
audio_ram_code void btif_voice_recv_sco_frame(uint8_t id, uint16_t sco_handle, uint8_t *p_data, uint16_t len)
{
    bt_audio_task_env_t *p_bt_audio_task_env      = get_bt_audio_task_env_ptr();
    p_bt_audio_task_env->sco_packet_is_processing = 1;

    bt_voice_receive_sco_frame(id, p_data, len, sco_handle);
}

/**
 * @brief  get sco data from encode buffer and send to stack
 * @param[in] id          - frame id
 * @param[in] sco_handle  - sco connection handle
 * @param[in] p           - sco data start address
 * @param[in] len         - sco data length
 * @returns none
 */
audio_ram_code void btif_voice_get_sco_frame(uint8_t id, uint16_t sco_handle, uint8_t *p, uint16_t len)
{
    bt_voice_mic_enc_get_frame(id, p, len, sco_handle);
}

#if SCO_ENC_QUEUE_ENABLE

/**
 * @brief  initialize sco encode buffer
 * @param[in] queue_id - queue index
 * @param[in] p_sco_buff - pointer to sco buffer
 */
void btif_sco_enc_buff_init(uint8_t queue_id, uint8_t *p_sco_buff)
{
    if (queue_id >= SCO_ENC_QUEUE_NUM) {
        return;
    }

    g_sco_enc_buf_ctx[queue_id].p_sco_enc_buff          = p_sco_buff;
    g_sco_enc_buf_ctx[queue_id].sco_enc_buff_frame_size = SCO_ENC_BUFF_FRAME_SIZE;
    g_sco_enc_buf_ctx[queue_id].sco_enc_buff_num        = SCO_ENC_BUFF_NUM;
    g_sco_enc_buf_ctx[queue_id].sco_enc_buff_wptr       = 0;
    g_sco_enc_buf_ctx[queue_id].sco_enc_buff_rptr       = 0;
}

/**
 * @brief  get sco encode buffer pointer
 * @param[in] queue_id
 * @return pointer to sco encode buffer
 */
uint8_t *btif_get_sco_enc_buff_ptr(uint8_t queue_id)
{
    if (queue_id >= SCO_ENC_QUEUE_NUM) {
        return 0;
    }

    uint16_t offset    = g_sco_enc_buf_ctx[queue_id].sco_enc_buff_wptr * g_sco_enc_buf_ctx[queue_id].sco_enc_buff_frame_size;
    uint8_t *p_enc_buf = g_sco_enc_buf_ctx[queue_id].p_sco_enc_buff + offset;

    return p_enc_buf;
}

/**
 * @brief  update sco encode buffer read pointer
 * @param[in] queue_id
 */
void btif_update_sco_enc_buff_rptr(uint8_t queue_id)
{
    if (queue_id >= SCO_ENC_QUEUE_NUM) {
        return;
    }

    g_sco_enc_buf_ctx[queue_id].sco_enc_buff_rptr = (g_sco_enc_buf_ctx[queue_id].sco_enc_buff_rptr + 1) % g_sco_enc_buf_ctx[queue_id].sco_enc_buff_num;
}

/**
 * @brief  update sco encode buffer write pointer
 * @param[in] queue_id
 */
void btif_update_sco_enc_buff_wptr(uint8_t queue_id)
{
    if (queue_id >= SCO_ENC_QUEUE_NUM) {
        return;
    }

    g_sco_enc_buf_ctx[queue_id].sco_enc_buff_wptr = (g_sco_enc_buf_ctx[queue_id].sco_enc_buff_wptr + 1) % g_sco_enc_buf_ctx[queue_id].sco_enc_buff_num;
}

/**
 * @brief  get sco encode buffer available count
 * @param[in] queue_id
 * @return available count
 */
uint8_t btif_get_sco_enc_buff_available(uint8_t queue_id)
{
    if (queue_id >= SCO_ENC_QUEUE_NUM) {
        return 0;
    }

    int16_t available = g_sco_enc_buf_ctx[queue_id].sco_enc_buff_wptr - g_sco_enc_buf_ctx[queue_id].sco_enc_buff_rptr;
    if (available < 0) {
        available += g_sco_enc_buf_ctx[queue_id].sco_enc_buff_num;
    }

    return (uint8_t)available;
}

uint8_t btif_get_sco_queue_id(uint16_t sco_handle)
{
    uint8_t queue_id = SCO_ENC_QUEUE_ID_HF;

    extern uint16_t btp_hfp_getHfHandle(void);
    extern uint16_t btp_hfp_getAgHandle(void);
    extern uint16_t bth_handle_getConnScoHandle(uint16_t aclHandle);

    uint16_t aclHandle   = btp_hfp_getHfHandle();
    uint16_t hfScoHandle = bth_handle_getConnScoHandle(aclHandle);
    hfScoHandle          = hfScoHandle & 0x0FFF;

    aclHandle            = btp_hfp_getAgHandle();
    uint16_t agScoHandle = bth_handle_getConnScoHandle(aclHandle);
    agScoHandle          = agScoHandle & 0x0FFF;

    if (sco_handle == hfScoHandle) {
        queue_id = SCO_ENC_QUEUE_ID_HF;
    } else if (sco_handle == agScoHandle) {
        queue_id = SCO_ENC_QUEUE_ID_AG;
    }
#if (TLKMW_BT_1_TO_2_FORWARD_EN)
    return queue_id;
#else
    (void)queue_id;
    return SCO_ENC_QUEUE_ID_HF;
#endif
}

/**
 * @brief  process sco encode buffer, called by main loop
 * @param[in]  none
 * @returns none
 */
void btif_process_sco_enc_buff(void)
{
    uint16_t             conn_handle;
    uint8_t              sco_data[128];
    uint8_t              queue_id            = SCO_ENC_QUEUE_ID_HF;
    bt_audio_task_env_t *p_bt_audio_task_env = get_bt_audio_task_env_ptr();
    uint8_t              sco_enc_buff_rcv    = btif_get_sco_enc_buff_available(queue_id);

    if (p_bt_audio_task_env->sco_packet_is_processing) {
        return;
    }

    if (sco_enc_buff_rcv > 1) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "SCO RX cnt: %d", sco_enc_buff_rcv);
    }

    if (sco_enc_buff_rcv) {
        uint16_t offset        = g_sco_enc_buf_ctx[queue_id].sco_enc_buff_rptr * g_sco_enc_buf_ctx[queue_id].sco_enc_buff_frame_size;
        uint8_t *p_sco_enc_buf = g_sco_enc_buf_ctx[queue_id].p_sco_enc_buff + offset;

        /*
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN,
                     "[SCO] p_sco_enc_buf: %x %x %x %x",
                     p_sco_enc_buf[0], p_sco_enc_buf[1], p_sco_enc_buf[4], p_sco_enc_buf[5]);
        */

        btif_update_sco_enc_buff_rptr(queue_id);
        conn_handle = p_sco_enc_buf[1] | (p_sco_enc_buf[2] << 8);
        conn_handle = conn_handle & 0x0FFF;

        btif_voice_recv_sco_frame(0, conn_handle, p_sco_enc_buf, 60);
        btif_voice_get_sco_frame(0, conn_handle, sco_data, 60);

        if (tlkmdi_audio_hostif_isInProductTest()) {
            tmemcpy(sco_data, p_sco_enc_buf + 4, 60);
        }

        conn_handle = p_sco_enc_buf[1] | (p_sco_enc_buf[2] << 8);
        tlkbt_hci_sendSyncData(conn_handle, sco_data, 60);
    }
}
#endif

/**
 * @brief  h0tl sco data process callback, Support 2-EV3 60 bytes
 * @param[in]  conn_handle - sco connection handle
 * @param[in]  p_data - sco data
 * @param[in]  len - sco data length
 * @returns None
 */
audio_ram_code int btif_h0tl_sco_data_callback(uint16_t conn_handle, uint8_t *p_data, uint16_t len)
{
#if SCO_ENC_QUEUE_ENABLE
    uint8_t sco_data[128];

    sco_data[0] = 0x03;
    sco_data[1] = conn_handle & 0xff;
    sco_data[2] = (conn_handle >> 8) & 0xff;
    sco_data[3] = len;

    if (len > 60) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[ERR] sco data len invalid, len: %d", len);
        return TLK_ENONE;
    }

#if (SLET_bt_sco_push_2_queue)
    log_tick_irq(1, SLET_bt_sco_push_2_queue);
#endif

    tmemcpy(&sco_data[4], p_data, len);

    bt_voice_cfg_t *p_bt_voice_env = get_bt_voice_env_ptr();
    if (p_bt_voice_env->sco_data_ready == 0) {
        p_bt_voice_env->sco_data_ready = 1;
    }

#ifdef SLET_bt_sco_rx
    log_tick_general(SL_BT_VOICE_LOG_EN, SLET_bt_sco_rx);
#endif

#if TWS_AUDIO_PATH_GPIO_DEBUG
    gpio_write(GPIO_PB0, 0);
    gpio_write(GPIO_PB0, 1);
    gpio_write(GPIO_PB0, 0);
#endif

    uint16_t             sco_conn_handle     = conn_handle & 0x0FFF;
    bt_audio_task_env_t *p_bt_audio_task_env = get_bt_audio_task_env_ptr();
    uint8_t              queue_id            = btif_get_sco_queue_id(sco_conn_handle);
    uint8_t             *p_enc_buff          = btif_get_sco_enc_buff_ptr(queue_id);

#if SCO_FORWARD_GPIO_DEBUG
    if (queue_id == SCO_ENC_QUEUE_ID_HF) {
        gpio_write(GPIO_PA0, 0);
        gpio_write(GPIO_PA0, 1);
        gpio_write(GPIO_PA0, 0);
    } else if (queue_id == SCO_ENC_QUEUE_ID_AG) {
        gpio_write(GPIO_PB7, 0);
        gpio_write(GPIO_PB7, 1);
        gpio_write(GPIO_PB7, 0);
    }
#endif

    tmemcpy(p_enc_buff, sco_data, len + 4);
    btif_update_sco_enc_buff_wptr(queue_id);

    if (btif_get_sco_enc_buff_available(queue_id) == 1 && p_bt_audio_task_env->sco_packet_is_processing) {
        g_bt_voice_sco_data_is_lost = 1;
    }

#ifdef SL16_sco_rcv_num
    uint8_t sco_enc_buff_num = btif_get_sco_enc_buff_available(queue_id);
    log_b16(SL_AUDIO_PATH_LOG_EN, SL16_sco_rcv_num, sco_enc_buff_num);
#endif

    if (p_bt_voice_env->bt_voice_mode == BT_VOICE_FORWARD_MODE || p_bt_voice_env->bt_voice_mode == BT_VOICE_FORWARD_WITH_ALG_MODE) {
        bt_voice_forward_set_sco_rx_evt(queue_id);
    }
    tlksys_task_setEvt(TLKSYS_TASKID_AUDIO, TLKSYS_TASK_EVT_AUD_MAIN);

    return TLK_ENONE;
#else // SCO_ENC_QUEUE_ENABLE
    uint8_t sco_data[128];

    sco_data[0] = 0x03;
    sco_data[1] = conn_handle & 0xff;
    sco_data[2] = (conn_handle >> 8) & 0xff;
    sco_data[3] = len;

    if (len > 60) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[ERR] sco data len invalid, len: %d", len);
        return;
    }

    tmemcpy(&sco_data[4], p_data, len);

    btif_voice_recv_sco_frame(0, conn_handle, sco_data, len);

    btif_voice_get_sco_frame(0, conn_handle, sco_data, 60);
#if (MCU_CORE_TYPE == CHIP_TYPE_TL751X)
    tlkbt_hci_sendSyncData(conn_handle, sco_data, 60);
#else
    uint8_t tlk_bt_ctrl_h0tl_h2c_write_sync_data(uint16_t conhdl_flags, uint8_t data_total_len, uint8_t * data);
    tlk_bt_ctrl_h0tl_h2c_write_sync_data(conn_handle, 60, sco_data);
#endif
#endif // SCO_ENC_QUEUE_ENABLE
}

/**
 * @brief  register sco data callback
 * @param[in]  none
 * @returns 0-success, others-failure
 */
int btif_register_sco_data_callback(void)
{
    for (uint8_t i = 0; i < SCO_ENC_QUEUE_NUM; i++) {
        btif_sco_enc_buff_init(i, g_sco_data_buff[i]);
    }

    tlkbt_hci_regScoDataCB(btif_h0tl_sco_data_callback);
    return TLK_ENONE;
}
#endif


/**
 * @brief  disable enhanced power control
 * @param[in]  none
 * @returns none
 */
void btif_hci_power_inc_dis(void)
{
    //bt_power_inc_tick = 0;
    //tlkapi_trace(COMMON_AUDIO_DBG_FLAG, COMMON_AUDIO_DBG_SIGN, "@-------btif_hci_power_inc_dis!");
}

/**
 * @brief  enable enhanced power control
 * @param[in]  none
 * @returns none
 */
void btif_hci_power_inc_en(void)
{
    //bt_power_inc_tick = clock_time() | 1;
    //tlkapi_trace(COMMON_AUDIO_DBG_FLAG, COMMON_AUDIO_DBG_SIGN, "@++++++btif_hci_power_inc_en!");
}

/**
 * @brief  send enhanced power control to peer
 * @param[in]  mode - POWER_INC or POWER_DEC
 * @returns none
 */
void btif_audio_power_control(uint8_t mode)
{
    if (mode == POWER_INC) {
        btif_hci_power_inc_en();
    } else {
        btif_hci_power_inc_dis();
    }
}

/**
 * @brief  set hfp codec
 * @param[in]  hfp_codec - HFP_CODEC_ID_CVSD HFP_CODEC_ID_MSBC
 * @returns none
 */
void btif_set_hfp_codec(uint8_t queue_id, uint8_t hfp_codec)
{
    if (queue_id >= SCO_ENC_QUEUE_NUM) {
        return;
    }
    if (HFP_CODEC_ID_CVSD == hfp_codec || HFP_CODEC_ID_MSBC == hfp_codec) {
        bt_voice_hfp_codec[queue_id] = hfp_codec;
    }
}

/**
 * @brief  get hfp codec
 * @param[in]  none
 * @returns HFP_CODEC_ID_CVSD HFP_CODEC_ID_MSBC
 */
uint8_t btif_get_hfp_codec(uint8_t queue_id)
{
    if (queue_id >= SCO_ENC_QUEUE_NUM) {
        tlk_printf("btif_get_hfp_codec error queue_id: %d", queue_id);
        return HFP_CODEC_ID_MSBC;
    }
    return bt_voice_hfp_codec[queue_id];
}

#endif
