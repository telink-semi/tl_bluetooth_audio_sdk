/********************************************************************************************************
 * @file    tlkmdi_audu2h_voice.c
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
#if ((TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "tlklib/usb/tlkusb_stdio.h"
#include "tlklib/usb/uac/tlkusb_uac.h"

#include "tlklib/usb/uac/tlkusb_uacDefine.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacctr.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"
#include "stack/bt/host/bth/bth_define.h"
#include "stack/ble/ble.h"
#include "tlkalg/audio/audio_alg_interface.h"
#include "tlkalg/audio/ppm/tlkalg_u2s_ppm.h"
#include "stack/bt/host/btp/hfp/btp_hfp.h"


#define TLKMDI_AUDU2H_VOICE_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_U2H_VOICE << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_AUDU2H_VOICE_DBG_SIGN "[MDI]"

static tlkmdi_audu2h_voice_ctrl_t sTlkMdiAudU2hVoiceCtrl = {0};

uint8_t *spTlkMdiU2hAsrc48to16Buff = NULL;
uint8_t *spTlkMdiU2hAsrc16to48Buff = NULL;
uint8_t *spTlkMdiU2hPpmSpkBuff     = NULL;
uint8_t *spTlkMdiU2hPpmMicBuff     = NULL;

/**
 * @brief       Initializes the U2H voice module.
 * @param       None
 * @return      Returns 0 if successful, otherwise returns an error code.
 */
int tlkmdi_audu2h_voice_init(void)
{
    memset(&sTlkMdiAudU2hVoiceCtrl, 0, sizeof(tlkmdi_audu2h_voice_ctrl_t));
    bt_audio_set_voice_vol_percent(10);
    //ziyu NOTE: TODO:?
    //this api is used to control the vol step of HF's Speaker.
    //but here is used to control AG's MicPhone(sao cao zuo)
    btif_register_sco_data_callback();

    return TLK_ENONE;
}

/**
 * @brief       Starts the U2H voice processing for a given connection handle.
 * @param[in]   handle  - The connection handle.
 * @param[in]   param   - Additional parameters for voice start.
 * @return      Returns 0 if successful, otherwise returns an error code.
 */
int tlkmdi_audu2h_voice_start(uint16_t handle, uint32_t param)
{
    (void)handle;
    (void)param;
    if (sTlkMdiAudU2hVoiceCtrl.enable) {
        return -TLK_EREPEAT;
    }
    return TLK_ENONE;
}

/**
 * @brief       Closes the U2H voice connection for a given handle.
 * @param[in]   handle  - The connection handle.
 * @return      Returns 0 if successful, otherwise returns an error code.
 */
int tlkmdi_audu2h_voice_close(uint16_t handle)
{
    (void)handle;
    if (!sTlkMdiAudU2hVoiceCtrl.enable) {
        return -TLK_EREPEAT;
    }
    return TLK_ENONE;
}

/**
 * @brief       Timer function to handle periodic tasks for U2H voice.
 * @param       None
 * @return      None
 */
void tlkmdi_audu2h_voice_timer(void)
{
    if (sTlkMdiAudU2hVoiceCtrl.enable) {
        if (bth_handle_searchConnSco(sTlkMdiAudU2hVoiceCtrl.handle) != NULL) {
            if (sTlkMdiAudU2hVoiceCtrl.waitFlag & TLKMDI_AUDU2H_VOICE_WAIT_SCO_ESTB) {
                sTlkMdiAudU2hVoiceCtrl.waitFlag &= ~TLKMDI_AUDU2H_VOICE_WAIT_SCO_ESTB;
                sTlkMdiAudU2hVoiceCtrl.waitTimer = 0;
            }
        }
        if ((sTlkMdiAudU2hVoiceCtrl.waitFlag & TLKMDI_AUDU2H_VOICE_WAIT_SCO_ESTB)) {
            if (sTlkMdiAudU2hVoiceCtrl.waitTimer != 0 && clock_time_exceed(sTlkMdiAudU2hVoiceCtrl.waitTimer, TLKMW_U2H_VOICE_WAIT_SCO_TIMEOUT)) {
                tlkapi_trace(TLKMDI_AUDU2H_VOICE_DBG_FLAG, TLKMDI_AUDU2H_VOICE_DBG_SIGN, "tlkmdi_audu2h_voice_timer sco establish timeout");
                tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_U2H_VOICE, sTlkMdiAudU2hVoiceCtrl.handle);
            }
        }
    }
}

/**
 * @brief       Checks if the U2H voice module is busy.
 * @param       None
 * @return      Returns true if busy, false otherwise.
 */
bool tlkmdi_audu2h_voice_isBusy(void)
{
    return sTlkMdiAudU2hVoiceCtrl.enable;
}

/**
 * @brief       Initializes the U2H voice algorithm buffers.
 * @param[in]   enable  - Indicates whether to enable the buffers.
 * @return      Returns true if successful, false otherwise.
 */
bool tlkmdi_audu2h_voice_initBuffer(bool enable)
{
    audio_alg_interface_t *p_audio_alg_if;

    if (enable) {
#if TLKALG_ASRC_48TO16_16BIT_ENABLE
        // p_audio_alg_if            = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
        p_audio_alg_if            = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
        uint16_t asrc_48to16_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        spTlkMdiU2hAsrc48to16Buff = (uint8_t *)tlkalg_malloc_func(asrc_48to16_size);
        p_audio_alg_if->audio_alg_init(spTlkMdiU2hAsrc48to16Buff, ALG_CHANNEL_STEREO);
        if (spTlkMdiU2hAsrc48to16Buff == NULL) {
            return false;
        }
#endif

        if (48000 == tlkusb_uac_get_iso_in_SampleRate()) {
#if TLKALG_ASRC_16TO48_16BIT_ENABLE
            // p_audio_alg_if            = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
            p_audio_alg_if            = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
            uint16_t asrc_16to48_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
            spTlkMdiU2hAsrc16to48Buff = (uint8_t *)tlkalg_malloc_func(asrc_16to48_size);
            p_audio_alg_if->audio_alg_init(spTlkMdiU2hAsrc16to48Buff, ALG_CHANNEL_LEFT);
            if (spTlkMdiU2hAsrc16to48Buff == NULL) {
                return false;
            }
#endif
        }

#if TLK_ALG_PPM_ENABLE
#if TLKALG_PPM_SPK_ENABLE
        p_audio_alg_if        = audio_alg_get_interface_by_type(ALG_PPM_SPK);
        uint16_t ppm_spk_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        spTlkMdiU2hPpmSpkBuff = (uint8_t *)tlkalg_malloc_func(ppm_spk_size);
        p_audio_alg_if->audio_alg_init(spTlkMdiU2hPpmSpkBuff, ALG_CHANNEL_STEREO);
        if (spTlkMdiU2hPpmSpkBuff == NULL) {
            return false;
        }
#endif
#if TLKALG_PPM_MIC_ENABLE
        p_audio_alg_if        = audio_alg_get_interface_by_type(ALG_PPM_MIC);
        uint16_t ppm_mic_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
        spTlkMdiU2hPpmMicBuff = (uint8_t *)tlkalg_malloc_func(ppm_mic_size);
        p_audio_alg_if->audio_alg_init(spTlkMdiU2hPpmMicBuff, ALG_CHANNEL_LEFT);
        if (spTlkMdiU2hPpmMicBuff == NULL) {
            return false;
        }
#endif
        tlkalg_u2s_ppm_clear_status();
        tlkalg_u2s_ppm_set_val(0);
#endif

        tlkusb_uac_set_state(TLKUSB_UAC_VOICE);
    } else {
        tlkusb_uacmic_ppm_stop();
        tlkusb_uacspk_ppm_stop();
        tlkusb_uac_set_state(TLKUSB_UAC_IDLE);

#if TLKALG_ASRC_48TO16_16BIT_ENABLE
        if (spTlkMdiU2hAsrc48to16Buff != NULL) {
            // p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
            tlkalg_free_func(spTlkMdiU2hAsrc48to16Buff);
            p_audio_alg_if->audio_alg_deinit();
            spTlkMdiU2hAsrc48to16Buff = NULL;
        }
#endif

#if TLKALG_ASRC_16TO48_16BIT_ENABLE
        if (spTlkMdiU2hAsrc16to48Buff != NULL) {
            // p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
            tlkalg_free_func(spTlkMdiU2hAsrc16to48Buff);
            p_audio_alg_if->audio_alg_deinit();
            spTlkMdiU2hAsrc16to48Buff = NULL;
        }
#endif

#if TLK_ALG_PPM_ENABLE
#if TLKALG_PPM_SPK_ENABLE
        if (spTlkMdiU2hPpmSpkBuff != NULL) {
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
            tlkalg_free_func(spTlkMdiU2hPpmSpkBuff);
            p_audio_alg_if->audio_alg_deinit();
            spTlkMdiU2hPpmSpkBuff = NULL;
        }
#endif
#if TLKALG_PPM_MIC_ENABLE
        if (spTlkMdiU2hPpmMicBuff != NULL) {
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_MIC);
            tlkalg_free_func(spTlkMdiU2hPpmMicBuff);
            p_audio_alg_if->audio_alg_deinit();
            spTlkMdiU2hPpmMicBuff = NULL;
        }
#endif
#endif
    }

    return true;
}

/**
 * @brief       Switches the voice state based on the provided status.
 * @param[in]   handle  - The connection handle.
 * @param[in]   status  - The new status to set.
 * @return      Returns true if the switch is successful, false otherwise.
 */
bool tlkmdi_audu2h_voice_switch(uint16_t handle, uint8_t status)
{
    bool enable;
    int  ret = TLK_ENONE;

    uint8_t number[5] = {0, 0x01, 0x02, 0x03, 0x04};
    if (status == TLK_STATE_OPENED) {
        enable = true;
    } else {
        enable = false;
    }
    if (enable && sTlkMdiAudU2hVoiceCtrl.enable) {
        sTlkMdiAudU2hVoiceCtrl.enable = false;
    }

    if (!tlkmdi_audu2h_voice_initBuffer(enable)) {
        return false;
    }

    if (status == TLK_STATE_CLOSED) {
        tlkmdi_audu2h_voice_close(handle);
    }

    sTlkMdiAudU2hVoiceCtrl.handle    = handle;
    sTlkMdiAudU2hVoiceCtrl.enable    = enable;
    sTlkMdiAudU2hVoiceCtrl.waitFlag  = 0;
    sTlkMdiAudU2hVoiceCtrl.waitTimer = 0;

    tlkapi_trace(TLKMDI_AUDU2H_VOICE_DBG_FLAG, TLKMDI_AUDU2H_VOICE_DBG_SIGN, "tlkmdi_audu2h_voice_switch: status[%d] micSampleRate: %d", status,
                 tlkusb_uac_get_iso_in_SampleRate());

    if (enable) {
        ret = bt_voice_audio_path_init();
        if (!ret) {
            tlkapi_trace(TLKMDI_AUDU2H_VOICE_DBG_FLAG, TLKMDI_AUDU2H_VOICE_DBG_SIGN, "bt_voice_audio_path_init fail: %d", ret);
            return false;
        }

        ret = tlkmdi_bthfpag_insertCall(number, 5, false);
        if (ret == TLK_ENONE) {
            ret = tlkmdi_bthfpag_createSco(bth_handle_getBtAddr(sTlkMdiAudU2hVoiceCtrl.handle));
        }
        if (ret != TLK_ENONE) {
            return false;
        } else {
            sTlkMdiAudU2hVoiceCtrl.waitFlag |= TLKMDI_AUDU2H_VOICE_WAIT_SCO_ESTB;
            sTlkMdiAudU2hVoiceCtrl.waitTimer = clock_time() | 1;
        }
        tlkmw_audio_btif_inform_host_audio_en(handle, false);

        bt_audio_register_get_pcm_data_callback(bt_voice_get_playback_data);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, bt_audio_main);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, bt_audio_main_loop);
        bt_audio_task_register_run_cb(NULL, 1);


    } else {
        tlkmdi_bthfpag_hungupCall();

        bt_voice_audio_path_deinit();

        bt_audio_task_register_run_cb(NULL, 0);
        bt_audio_register_get_pcm_data_callback(NULL);

        tlkmw_audio_btif_inform_host_audio_dis(handle);
        audio_codec_flag_set(CODEC_FLAG_VOICE, 0);
    }

    tlkapi_trace(TLKMDI_AUDU2H_VOICE_DBG_FLAG, TLKMDI_AUDU2H_VOICE_DBG_SIGN, "tlkmdi_audu2hVoice_switch: %d %d", handle, status);

    return true;
}

#define TLKMDI_U2H_USE_SINE 0

/**
 * @brief       Retrieves PCM data from the U2H voice module.
 * @param[out]  pBuffer - Buffer to store the retrieved PCM data.
 * @param[in]   dataLen - Length of the buffer.
 * @return      Returns 0 if successful, otherwise returns an error code.
 */
audio_ram_code int tlkmdi_audu2h_voice_getPcmData(uint8_t *pBuffer, uint16_t dataLen)
{
    uint32_t refTime = clock_time() | 1;
    int      ret     = 1;
    if (dataLen > 120) {
        dataLen = 120;
    }
    uint16_t length = dataLen * 2 * 2 * 3; //48K -> 16K 120*2chn*2Byte*3mul
    int16_t  pcm_stereo_48K[120 * 2 * 3];
    int16_t  pcm_stereo_16K[120 * 2];
    int16_t  pcm_mono_16K[120];


    tlkusb_uac_copy_data(refTime);
    uint16_t used_samples = tlkusb_uac_get_ppm_out_used_samples();

    /*7.5ms samples needed to encode a frame*/
    if (used_samples < (length / 2)) {
        tmemset(pBuffer, 0, dataLen * 2);
        return ret;
    }

    int read_samples = tlkusb_uac_read_ppm_out_samples(pcm_stereo_48K, length / 2);

    // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
    p_audio_alg_if->audio_alg_process((uint8_t *)pcm_stereo_48K, (uint8_t *)pcm_stereo_16K, dataLen * 3, 0, 0);

    for (int i = 0; i < dataLen; i++) {
        pcm_mono_16K[i] = pcm_stereo_16K[2 * i + 1];
    }

    /*aujust volume*/
    int16_t  audio_volume = tlkusb_uacspk_getVolume();
    int16_t *pData        = (int16_t *)pcm_mono_16K;
    for (int index = 0; index < read_samples; index++) {
        pData[index] = (pData[index] * audio_volume + 0x2000) >> 14;
    }

    tmemcpy(pBuffer, pcm_mono_16K, dataLen * 2);

    return dataLen;
}

/**
 * @brief       Fills the U2H voice module with speaker data.
 * @param[in]   pBuffer - Buffer containing the speaker data.
 * @param[in]   dataLen - Length of the speaker data.
 * @return      Returns the number of bytes filled.
 */
audio_ram_code int tlkmdi_u2h_voice_fillSpkData(uint8_t *pBuffer, uint16_t dataLen)
{
    (void)dataLen;
    uint32_t refTime    = clock_time() | 1;
    uint16_t sample_num = 120; //dataLen / sizeof(codec_int);

    short *psrc = (short *)pBuffer;
    short  pcm_mono_16k[120];

    for (int i = 0; i < sample_num; i++) { //stereo to mono
        pcm_mono_16k[i] = psrc[2 * i];
    }

    //upresample to 48k
    if (48000 == tlkusb_uac_get_iso_in_SampleRate()) {
        short pcm_mono_48k[360];
        // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
        p_audio_alg_if->audio_alg_process((uint8_t *)pcm_mono_16k, (uint8_t *)pcm_mono_48k, sample_num, 0, 0);

        tlkusb_uac_write_iso_in_samples((int16_t *)pcm_mono_48k, sample_num * 3);

    } else {
        tlkusb_uac_write_iso_in_samples((int16_t *)pcm_mono_16k, sample_num);
    }

    tlkusb_uacmic_ppm_process(refTime);

    return 0;
}

/**
 * @brief       Operates on the U2H voice module with the given opcode and data.
 * @param[in]   opcode  - The operation code.
 * @param[in]   pdata   - Pointer to the data buffer.
 * @param[in]   dataLen - Length of the data buffer.
 * @return      Returns true if the operation was successful, false otherwise.
 */
bool tlkmdi_audu2h_voice_operate(uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        tlkusb_uacctrl_volume_up();
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        tlkusb_uacctrl_volume_down();
    } break;
    default:
    {
        return false;
    } break;
    }
    return true;
}

/**
 * @brief       Disables the U2H voice functionality.
 * @param       None
 * @return      None
 */
void tlkmdi_u2h_voice_disable(void)
{
    if (sTlkMdiAudU2hVoiceCtrl.enable) {
        tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_U2H_VOICE, sTlkMdiAudU2hVoiceCtrl.handle);
    }
}
#endif //#if (TLK_MDI_AUDU2H_VOICE_ENABLE)
