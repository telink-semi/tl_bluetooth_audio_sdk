/********************************************************************************************************
 * @file    tlkmdi_interphone.c
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
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "tlkmdi_interphone.h"
#include "tlkmdi_interphone_handler.h"

#if TLKMW_INTERPHONE_EN
tlkmdi_interphone_env_t s_tlk_mdi_interphone_env = {0};

/**
 * @brief     Receive a2dp frame.
 * @param[in] aclHandle - The ACL connection handle.
 * @param[in] p_data - A pointer to the data buffer.
 * @param[in] len - The length of the data.
 * @return    None
 */
void tlkmdi_interphone_rcv_a2dp_frame(uint16_t aclHandle, uint8_t *p_data, uint16_t len)
{
    uint16_t cur_handle = 0;
    if (tlkmdi_interphone_is_busy()) {
        cur_handle = tlkmdi_interphone_linkmgr_getRunningHandle();
    }

    if (cur_handle != aclHandle) {
        // tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "btif_receive_a2dp_frame, cur_handle != aclHandle");
        return;
    }

    bt_music_receive_a2dp_frames(p_data, len);
    return;
}

/**
 * @brief   Retrieves the ACL handle associated with the interphone.
 * @param   None
 * @return  uint16_t - The ACL handle.
 */
uint16_t tlkmdi_interphone_get_acl_handle(void)
{
    return tlkmdi_interphone_linkmgr_getRunningHandle();
}

/**
 * @brief     Controls the volume change callback for the interphone.
 * @param[in] handle - The connection handle.
 * @param[in] vol - The new volume level.
 * @return    None.
 */
static void tlkmdi_interphone_vol_change_cb(uint16_t handle, uint8_t vol)
{
    (void)vol;
    if (handle != tlkmdi_interphone_get_acl_handle()) {
        return;
    }
    bt_audio_set_music_vol_percent_by_handle(handle);
}

/**
 * @brief     Controls the voice functionality of the interphone.
 * @param[in] acl_handle - The ACL connection handle.
 * @param[in] is_start - Indicates whether to start (1) or stop (0) the voice functionality.
 * @param[in] codec - The codec type to be used.
 * @return    None.
 */
static void tlkmdi_interphone_voice_control(uint16_t acl_handle, uint8_t is_start, uint8_t codec)
{
    tlkapi_printf(APP_LOG_EN, "tlkmdi_interphone_voice_control is_start  %d", is_start);

    if (is_start) {
        // gpio_set_high_level(GPIO_CHN1);
        btif_set_hfp_codec(codec);

        int ret = bt_voice_audio_path_init();
        if (!ret) {
            tlkapi_printf(APP_LOG_EN, "bt_voice_audio_path_init fail: %d", ret);
            return;
        }
        // tlkmdi_interphone_voice_enable(INTERPHONE_MODE_AG);
        // tlkmdi_interphone_voice_ag_enable();

        // s_tlk_mdi_interphone_env.bt_ag_sco_started = true;
        g_codec_cfg.sample_rate = 16000;
        tlkmw_audio_btif_inform_host_audio_en(acl_handle, false);
        bt_audio_set_voice_vol_percent_by_handle(acl_handle);
        // tlkmdi_interphone_btvoice_alg_init();
        bt_audio_register_get_pcm_data_callback(bt_voice_get_playback_data);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, bt_audio_main);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, bt_audio_main_loop);
        bt_audio_task_register_run_cb(NULL, 1);
        // gpio_set_low_level(GPIO_CHN1);
    } else {
        // gpio_set_high_level(GPIO_CHN1);
        s_tlk_mdi_interphone_env.bt_ag_sco_started = false;
        //no mesh and no ag should close dsp
        tlkmdi_interphone_voice_disable(INTERPHONE_MODE_AG);

        bt_audio_task_register_run_cb(NULL, 0);
        bt_audio_register_get_pcm_data_callback(NULL);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, NULL);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, NULL);
        // tlkmdi_interphone_btvoice_alg_deinit();
        int ret = bt_voice_audio_path_deinit();
        if (!ret) {
            tlkapi_printf(APP_LOG_EN, "bt_voice_audio_path_deinit fail: %d", ret);
        }
        tlkmw_audio_btif_inform_host_audio_dis(acl_handle);
        // gpio_set_low_level(GPIO_CHN1);
    }
}

/**
 * @brief     Controls the mesh functionality of the interphone.
 * @param[in] isStart - Indicates whether to start (1) or stop (0) the mesh functionality.
 * @return    None.
 */
static void tlkmdi_interphone_mesh_control(uint8_t isStart)
{
    tlkapi_printf(APP_LOG_EN, "tlkmdi_interphone_mesh_control %d", isStart);

    if (isStart) {
        tlkmdi_interphone_spk_enable(INTERPHONE_MODE_MESH);
        tlkmdi_interphone_voice_enable(INTERPHONE_MODE_MESH);
        s_tlk_mdi_interphone_env.mesh_started    = true;
        s_tlk_mdi_interphone_env.mesh_first_flag = true;
        audio_tx_dma_en(INTERPHONE_I2S_TX_DMA);
        audio_rx_dma_en(INTERPHONE_I2S_RX_DMA);
        tlkmdi_mute_i2s_tx_buff();
        tlkmdi_mute_i2s_rx_buff();
    } else {
        s_tlk_mdi_interphone_env.mesh_started = false;
        audio_tx_dma_dis(INTERPHONE_I2S_TX_DMA);
        audio_rx_dma_dis(INTERPHONE_I2S_RX_DMA);
        //no mesh and no ag should close dsp and mic
        tlkmdi_interphone_voice_disable(INTERPHONE_MODE_MESH);
    }
}

/**
 * @brief     Controls the Bluetooth music functionality of the interphone.
 * @param[in] handle  - The connection handle.
 * @param[in] isStart - Indicates whether to start (1) or stop (0) the music functionality.
 * @return    None.
 */
static void tlkmdi_interphone_bt_music_control(uint16_t handle, uint8_t isStart)
{
    if (!isStart) {
        tlkmdi_interphone_btmusic_alg_deinit();
        bt_audio_task_register_run_cb(NULL, 0);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, NULL);
        bt_audio_register_get_pcm_data_callback(NULL);
        bt_music_audio_path_deinit();
        tlkmw_audio_btif_inform_host_audio_dis(handle);
        tlkmdi_interphone_spk_disable_mode(INTERPHONE_MODE_MUSIC);
        s_tlk_mdi_interphone_env.bt_music_started = false;
        return;
    }

    if (SEPID_AAC == btp_a2dpsnk_getCurrCodec(handle)) {
        s_tlk_mdi_interphone_env.codec_type = SEPID_AAC;
        one_frame_times                     = 7500;
    } else if (SEPID_SBC == btp_a2dpsnk_getCurrCodec(handle)) {
        s_tlk_mdi_interphone_env.codec_type = SEPID_SBC;
        one_frame_times                     = 1000;
    }
    bt_audio_set_music_vol_percent_by_handle(handle);
    bt_music_audio_path_init();
    tlkmdi_interphone_btmusic_alg_init();
#if (TLKBTP_CFG_A2DPSNK_ENABLE)
    btp_a2dpsnk_regRecvDataCB(tlkmdi_interphone_rcv_a2dp_frame);
#endif
    bt_audio_task_register_run_cb(NULL, 0);
    bt_audio_register_get_pcm_data_callback(bt_music_get_playback_data);
    // tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, tlkmdi_interphone_mainloop); //3ms loop onetime
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, tlkmdi_interphone_timer_irq);
    bt_audio_task_register_run_cb(NULL, 1);
    tlkmw_audio_btif_inform_host_audio_en(handle, true);

    tlkmdi_interphone_set_looptick();
    audio_codec_flag_set(CODEC_FLAG_MUSIC, 1);
    bt_music_calibrate_enable(1);
    tlkmdi_interphone_mute_BtDownBuff();

    tlkmdi_interphone_sync_BtDownBuff(CODEC_SPK_FIFO_SAMPLES / 2);
    // tlkmdi_interphone_spk_enable(INTERPHONE_MODE_MUSIC);
    //put last location to prevent fifo irq happening and get bt data before init completed
    s_tlk_mdi_interphone_env.bt_music_started = true;
}

/**
 * @brief   Initializes the interphone module.
 * @param   None
 * @return  int - Returns 0 if initialization is successful; otherwise an error code.
 */
int tlkmdi_interphone_init(void)
{
    tlkapi_printf(APP_LOG_EN, "tlkmdi_interphone_init");

    tlkmdi_interphone_linkmgr_init(tlkmdi_interphone_bt_music_control);
    tlkmdi_interphone_meshmgr_init(tlkmdi_interphone_mesh_control);
    tlkmdi_interphone_hfmgr_init(tlkmdi_interphone_voice_control);
    tlkmdi_audio_btif_regMusicVolChgCB(tlkmdi_interphone_vol_change_cb);
    tlkmdi_interphone_buff_init();
    tlkmdi_interphone_i2s_init();
    tlkmdi_interphone_alg_init_temp(); //avoid 441to48 asrc init costing too long time to block other operation
    return TLK_ENONE;
}

/**
 * @brief     Starts the interphone for a given handle with specified parameters.
 * @param[in] handle The connection handle.
 * @param[in] param  Additional parameters for starting the interphone.
 * @return    int - Returns 0 if start is successful; otherwise an error code.
 */
int tlkmdi_interphone_start(uint16_t handle, uint32_t param)
{
    uint8_t start_mode = (param >> 28); //high 4bit
    handle             = tlkmdi_interphone_linkmgr_getFirstPausedHandle();
    if (handle != 0) {
        if (start_mode != TLKAUD_START_MODE_PLAY_PREV && start_mode != TLKAUD_START_MODE_PLAY_NEXT) {
            tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PLAY);
        }
    }
    if (start_mode == TLKAUD_START_MODE_PLAY_PREV) {
        tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_BACKWARD);
    } else if (start_mode == TLKAUD_START_MODE_PLAY_NEXT) {
        tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_FORWARD);
    }
    return TLK_ENONE;
}

/**
 * @brief     Closes the interphone for a given handle.
 * @param[in] handle The connection handle.
 * @return    int - Returns 0 if close is successful; otherwise an error code.
 */
int tlkmdi_interphone_close(uint16_t handle)
{
    if (!s_tlk_mdi_interphone_env.busy) {
        return -TLK_EPARAM;
    }
    handle = tlkmdi_interphone_linkmgr_getRunningHandle();
    if (handle != 0) {
        tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PAUSE);
    }
    return TLK_ENONE;
}

/**
 * @brief   Skips to the next interphone track.
 * @param   None
 * @return  bool - Returns true if skip is successful; otherwise false.
 */
bool tlkmdi_interphone_next(void)
{
    if (!s_tlk_mdi_interphone_env.busy) {
        return false;
    }
    uint16_t linkHandle = tlkmdi_interphone_linkmgr_getValidHandle();
    if (linkHandle == 0) {
        return false;
    }
    tlkmdi_audio_btif_avrcp_sendKey(linkHandle, AUD_BTIF_AVRCP_KEYID_FORWARD);

    return true;
}

/**
 * @brief   Skips to the previous interphone track.
 * @param   None
 * @return  bool - Returns true if skip is successful; otherwise false.
 */
bool tlkmdi_interphone_previous(void)
{
    if (!s_tlk_mdi_interphone_env.busy) {
        return false;
    }
    uint16_t linkHandle = tlkmdi_interphone_linkmgr_getValidHandle();
    if (linkHandle == 0) {
        return false;
    }
    tlkmdi_audio_btif_avrcp_sendKey(linkHandle, AUD_BTIF_AVRCP_KEYID_BACKWARD);
    return true;
}

/**
 * @brief     Operates on the interphone module with an opcode and data.
 * @param[in] handle  The connection handle.
 * @param[in] opcode  Operation code.
 * @param[in] pdata   Pointer to the data buffer.
 * @param[in] dataLen Length of the data.
 * @return    bool - Returns true if operation is successful; otherwise false.
 */
bool tlkmdi_interphone_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    (void)handle;
    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        if (tlkmdi_interphone_api_is_mesh_enable()) {
            // inc mesh vol func
            return true;
        }
        uint16_t musicHandle = tlkmdi_interphone_linkmgr_getRunningHandle();
        if (musicHandle != 0) {
            tlkmdi_audio_btif_VolumeOperate(musicHandle, true, true);
            return true;
        }
        uint16_t agHandle = tlkmdi_interphone_hfmgr_get_handle();
        if (agHandle != 0) {
            tlkmdi_audio_btif_VolumeOperate(agHandle, true, false);
            return true;
        }
        return false;
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        if (tlkmdi_interphone_api_is_mesh_enable()) {
            // dec mesh vol func
            return true;
        }
        uint16_t musicHandle = tlkmdi_interphone_linkmgr_getRunningHandle();
        if (musicHandle != 0) {
            tlkmdi_audio_btif_VolumeOperate(musicHandle, false, true);
            return true;
        }
        uint16_t agHandle = tlkmdi_interphone_hfmgr_get_handle();
        if (agHandle != 0) {
            tlkmdi_audio_btif_VolumeOperate(agHandle, false, false);
            return true;
        }
        return false;
    } break;
    case TLKAUD_OPCODE_IS_SUPPORT_TONE_MIX:
    {
        return true;
    }
    case TLKAUD_OPCODE_IS_CUSTOMIZED_PLAYPAUSE:
    {
        return true;
    }
    case TLKAUD_OPCODE_TRIGGER_CUSTOMIZED_PLAYPAUSE:
    {
        handle = tlkmdi_interphone_linkmgr_getRunningHandle();
        if (handle != 0) {
            if (btp_avrcp_remoteIsPlaying(handle)) {
                tlk_printf("handle[%d] avrcp is playing, send pause", handle);
                tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PAUSE);
            } else {
                tlk_printf("handle[%d] avrcp is not playing, send play", handle);
                tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PLAY);
            }
        } else {
            handle = tlkmdi_interphone_linkmgr_getFirstPausedHandle();
            if (handle != 0) {
                tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PLAY);
            }
        }
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
 * @brief   Checks if the interphone module is busy.
 * @param   None
 * @return  bool - Returns true if the interphone module is busy; otherwise false.
 */
bool tlkmdi_interphone_is_busy(void)
{
    return s_tlk_mdi_interphone_env.busy;
}

/**
 * @brief     Switches the interphone status for a given handle.
 * @param[in] handle The connection handle.
 * @param[in] status The target status to switch to.
 * @return    bool - Returns true if the switch is successful; otherwise false.
 */
bool tlkmdi_interphone_switch(uint16_t handle, uint8_t status)
{
    (void)handle;

    tlkapi_trace(0xffffffff, "<test>", "tlkmdi_interphone_switch: handle:%d, status: %d", handle, status);

    if (status == TLK_STATE_OPENED) {
        // gpio_set_high_level(GPIO_CHN0);
        s_tlk_mdi_interphone_env.busy = true;

        tlkmdi_interphone_linkmgr_resume();
        tlkmdi_interphone_meshmgr_resume();
        tlkmdi_interphone_hfmgr_resume();

#if TX_FIFO_IRQ_ENABLE
        tlkmdi_interphone_fifo_irq_init(SPK_FRAME_SIZE * sizeof(codec_int));
#else
        tlkmdi_interphone_fifo_irq_init(MIC_FRAME_SIZE * sizeof(adc_mono_int));
#endif
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_FIFO, tlkmdi_interphone_fifo_irq_handler_func);

// tlkmdi_interphone_open_spk();
#if TLK_MW_DSP_COMM_ENABLE
        d25f_init_ipc_buffer();
        ipc_msg_register_data_process_done_cb(tlkmdi_interphone_dsp_msg_process_callback, VAD_NN_NS, NN_NS_16K_20MS_ID);
#endif

        // tlkmdi_interphone_voice_enable(tlkmdi_interphone_get_mode());
        // gpio_set_low_level(GPIO_CHN0);
    } else {
        // gpio_set_high_level(GPIO_CHN0);
        s_tlk_mdi_interphone_env.busy = false;
        tlkmdi_interphone_linkmgr_pause();
        tlkmdi_interphone_meshmgr_pause();
        tlkmdi_interphone_hfmgr_pause();

        tlkmdi_interphone_alg_deinit();
        tlkmdi_interphone_spk_disable();
#if TLK_MW_DSP_COMM_ENABLE
        tlkmdi_interphone_clear_dsp_status();
        tlkmw_dsp_pause();
        ipc_msg_register_data_process_done_cb(NULL, VAD_NN_NS, NN_NS_16K_20MS_ID);
#endif
#if TX_FIFO_IRQ_ENABLE
        audio_fifo_irq_dis(AUDIO_TX_FIFO0);
#else
        audio_fifo_irq_dis(AUDIO_RX_FIFO0);
#endif
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_FIFO, NULL);
        tlkmdi_interphone_mute_BtDownBuff();
        tlkdrv_codec_muteSpkBuff();
        tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_BOTH);
        // gpio_set_low_level(GPIO_CHN0);
    }
    return 1;
}

#endif //TLKMW_INTERPHONE_EN
