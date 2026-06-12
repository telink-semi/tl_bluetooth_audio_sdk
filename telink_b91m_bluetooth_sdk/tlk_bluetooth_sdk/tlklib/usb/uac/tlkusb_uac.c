/********************************************************************************************************
 * @file    tlkusb_uac.c
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
#include "tlklib/usb/tlkusb_stdio.h"
#if (TLK_USB_UAC_ENABLE || TLK_USB_UAC_HS_ENABLE)
#include "drivers.h"
#include "tlklib/usb/uac/tlkusb_uacDefine.h"
#include "tlklib/usb/uac/tlkusb_uacDesc.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"
#include "stack/stack.h"
#include "tlkalg/audio/ppm/tlkalg_u2s_ppm.h"
#include "tlkalg/audio/asrc_24bit/tlkalg_ppm_calc.h"


extern const tlkusb_modCtrl_t sTlkUsbUacModCtrl;
extern const tlkusb_modDesc_t sTlkUsbUacModDesc;

_attribute_data_retention_sec_ TlkUsbUacEvt2StatusCB sTlkUsbUacEvt2StatusCB = NULL;
TlkUsbReportUacStatusCB                              sTlkUsbReportUacStatusCB;

int                sTlkUsbUacMute      = 0;
uint32_t           sTlkUsbUacSamplRate = 16000;
tlkusb_uac_state_e tlkuac_uac_state    = TLKUSB_UAC_IDLE;

const tlkusb_module_t gTlkUsbUacModule = {
    .modType = TLKUSB_MODTYPE_UAC,
    .pDesc   = &sTlkUsbUacModDesc,
    .pCtrl   = &sTlkUsbUacModCtrl,
};

#if PROJ_TPSLL_AUDIO_DONGLE
_attribute_ble_data_retention_
#endif
    tlkusb_iso_config_t g_tlk_usb_cfg = {0};

/**
 * @brief       This function resets the USB UAC output configuration.
 * @return      none.
 */
void tlkusb_uac_reset_out_config()
{
    g_tlk_usb_cfg.tick_out   = 0;
    g_tlk_usb_cfg.iso_out_en = false;
    g_tlk_usb_cfg.out_w      = 0;
    g_tlk_usb_cfg.out_r      = 0;
    g_tlk_usb_cfg.ppm_out_w  = 0;
    g_tlk_usb_cfg.ppm_out_r  = 0;
}

void tlkusb_uac_reset_out1_config(void)
{
    g_tlk_usb_cfg.tick_out1   = 0;
    g_tlk_usb_cfg.iso_out1_en = false;
    g_tlk_usb_cfg.out1_w      = 0;
    g_tlk_usb_cfg.out1_r      = 0;
}

/**
 * @brief       This function resets the USB UAC input configuration.
 * @return      none.
 */
void tlkusb_uac_reset_in_config()
{
    g_tlk_usb_cfg.tick_in   = 0;
    g_tlk_usb_cfg.iso_in_en = false;
    g_tlk_usb_cfg.in_w      = 0;
    g_tlk_usb_cfg.in_r      = 0;
    g_tlk_usb_cfg.ppm_in_r  = 0;
    g_tlk_usb_cfg.ppm_in_w  = 0;
}

/*iso out*/
/**
 * @brief       This function clears the ISO OUT buffer by setting read pointer equal to write pointer.
 * @return      none.
 */
void tlkusb_uac_clear_iso_out_buffer()
{
    g_tlk_usb_cfg.out_r = g_tlk_usb_cfg.out_w;
}

/**
 * @brief       This function gets the number of used samples in ISO OUT buffer.
 * @return      Number of used samples in ISO OUT buffer.
 */
uint16_t tlkusb_uac_get_iso_out_used_samples()
{
#if TLK_USB_UAC_DUAL_SOUNDCARD_MODE
    if (g_tlk_usb_cfg.iso_out1_en) {
        return (g_tlk_usb_cfg.out1_w - g_tlk_usb_cfg.out1_r) & APP_USB_ISO_OUT_BUFF_IDX_MASK;
    }
#endif
    return (g_tlk_usb_cfg.out_w - g_tlk_usb_cfg.out_r) & APP_USB_ISO_OUT_BUFF_IDX_MASK;
}

/**
 * @brief       This function gets the number of idle samples in ISO OUT buffer.
 * @return      Number of idle samples in ISO OUT buffer.
 */
_attribute_ram_code_sec_ uint16_t tlkusb_uac_get_iso_out_idle_samples()
{
    return (APP_USB_ISO_OUT_BUFF_SIZE - tlkusb_uac_get_iso_out_used_samples());
}

/**
 * @brief       This function synchronizes ISO OUT samples by adjusting the read pointer.
 * @param[in]   num - Number of samples to synchronize.
 * @return      none.
 */
void tlkusb_uac_sync_iso_out_samples(uint16_t num)
{
    if (num > APP_USB_ISO_OUT_BUFF_SIZE) {
        return;
    }

    g_tlk_usb_cfg.out_r = ((g_tlk_usb_cfg.out_w - num + APP_USB_ISO_OUT_BUFF_SIZE) % APP_USB_ISO_OUT_BUFF_SIZE);
}

/**
 * @brief       This function sets the ISO OUT read pointer.
 * @param[in]   sample - Sample position to set the read pointer.
 * @return      none.
 */
void tlkusb_uac_set_iso_out_rptr(uint16_t sample)
{
    g_tlk_usb_cfg.out_r  = (g_tlk_usb_cfg.out_w - sample) & APP_USB_ISO_OUT_BUFF_IDX_MASK;
    g_tlk_usb_cfg.out1_r = (g_tlk_usb_cfg.out1_w - sample) & APP_USB_ISO_OUT_BUFF_IDX_MASK;
}

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
/**
 * @brief       This function writes data to ISO OUT buffer for 24-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
_attribute_ram_code_sec_ uint16_t tlkusb_uac_write_iso_out_samples(int32_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen == 0 || tlkusb_uac_get_iso_out_idle_samples() < dataLen) {
        return 0;
    }

    for (uint16_t index = 0; index < dataLen; index++) {
        g_tlk_usb_cfg.iso_out[g_tlk_usb_cfg.out_w & APP_USB_ISO_OUT_BUFF_IDX_MASK] = pData[index];
        g_tlk_usb_cfg.out_w++;
        g_tlk_usb_cfg.out_w &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
    }

    return dataLen;
}

/**
 * @brief       This function reads data from ISO OUT buffer for 24-bit resolution.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
int32_t tlkusb_uac_get_iso_out_data(void)
{
    if (!tlkusb_uac_get_iso_out_en()) {
        return 0;
    }

    int32_t        data = g_tlk_usb_cfg.iso_out[g_tlk_usb_cfg.out_r & APP_USB_ISO_OUT_BUFF_IDX_MASK];
    int16_t        audio_volume;
    static int16_t real_volume = 0;
    audio_volume               = tlkusb_uacspk_getVolume();
    if (audio_volume != real_volume) {
        if (real_volume > audio_volume) {
            real_volume -= APP_AUDIO_SPK_VOL_GRDL_STEP;
            if (real_volume < audio_volume) {
                real_volume = audio_volume;
            }
        } else if (real_volume < audio_volume) {
            real_volume += APP_AUDIO_SPK_VOL_GRDL_STEP;
            if (real_volume > audio_volume) {
                real_volume = audio_volume;
            }
        }
    }
    data = ((int64_t)data * real_volume + 0x2000) >> 14;
    g_tlk_usb_cfg.out_r++;
    g_tlk_usb_cfg.out_r &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
    return data;
}
#if (TLK_USB_UAC_DUAL_SOUNDCARD_MODE)
int32_t tlkusb_uac_get_iso_out1_data(void)
{
    if (!tlkusb_uac_get_iso_out1_en()) {
        return 0;
    }

    int32_t        data = g_tlk_usb_cfg.iso_out1[g_tlk_usb_cfg.out1_r & APP_USB_ISO_OUT_BUFF_IDX_MASK];
    int16_t        audio_volume;
    static int16_t real_volume = 0;

    audio_volume = tlkusb_uacspk1_getVolume();

    if (audio_volume != real_volume) {
        if (real_volume > audio_volume) {
            real_volume -= APP_AUDIO_SPK_VOL_GRDL_STEP;
            if (real_volume < audio_volume) {
                real_volume = audio_volume;
            }
        } else if (real_volume < audio_volume) {
            real_volume += APP_AUDIO_SPK_VOL_GRDL_STEP;
            if (real_volume > audio_volume) {
                real_volume = audio_volume;
            }
        }
    }
    data = ((int64_t)data * real_volume + 0x2000) >> 14;
    g_tlk_usb_cfg.out1_r++;
    g_tlk_usb_cfg.out1_r &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
    return data;
}
#endif
/**
 * @brief       This function reads data from ISO OUT buffer for 24-bit resolution.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
uint16_t tlkusb_uac_read_iso_out_samples(int32_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen == 0) {
        return 0;
    }

    if (tlkusb_uac_get_iso_out_used_samples() < dataLen) {
        tlkusb_hal_wakeup_usb_thread();
        return 0;
    }

    uint16_t length = dataLen;
#if TLKALG_PPM_CALC_BY_SAMPLE || TLKALG_PPM_USB_CALC_ENABLE
    int32_t ppm_in_buff[dataLen + 2];
#if TLKUAC_PPM_SPK_24BIT_ENABLE
    int32_t ppm_out_buff[dataLen + 2];
#else
    int16_t ppm_in_buff_16b[dataLen + 2];
    int16_t ppm_out_buff_16b[dataLen + 2];
#endif
    int32_t *pdes       = (int32_t *)ppm_in_buff;
    uint16_t stereo_len = length / 2;
#if TLKALG_PPM_CALC_BY_SAMPLE
    uint16_t buff_remain_len = 0;
    // keep other MARGIN_NUM samples in buffer
    // if ppm ±1 sample, next time will get 1 sample less or more
    if (g_ppm_calc_ctl.alg_wptr_spk > g_ppm_calc_ctl.alg_rptr_spk) {
        buff_remain_len = (g_ppm_calc_ctl.alg_wptr_spk - g_ppm_calc_ctl.alg_rptr_spk) / 2;
    } else {
        buff_remain_len = (g_ppm_calc_ctl.alg_wptr_spk + TLKALG_PPM_BUFF_SPK_LEN - g_ppm_calc_ctl.alg_rptr_spk) / 2;
    }
    if (buff_remain_len > MARGIN_NUM + 2 || buff_remain_len < MARGIN_NUM - 2) {
        tlkapi_printf(1, "buff reset,wptr:%x,rptr:%x,buffer len:%x,get data len:%x", g_ppm_calc_ctl.alg_wptr_spk, g_ppm_calc_ctl.alg_rptr_spk, buff_remain_len, stereo_len);
        g_ppm_calc_ctl.alg_wptr_spk = (g_ppm_calc_ctl.alg_rptr_spk + MARGIN_NUM * 2) % TLKALG_PPM_BUFF_SPK_LEN;
    } else if (buff_remain_len != MARGIN_NUM) {
        stereo_len = stereo_len - buff_remain_len + MARGIN_NUM;
        //    	tlkapi_printf(1, "buff_remain_len :%x,get data len:%x",buff_remain_len,stereo_len);
    }
#else
    if (g_ppm_calc_ctl.sample_diff_spk != 0) {
        stereo_len                     = stereo_len - g_ppm_calc_ctl.sample_diff_spk; // /2 for mono to stereo
        g_ppm_calc_ctl.sample_diff_spk = 0;
    }
#endif
    length = stereo_len * 2; // for mono to stereo
#if TLKUAC_PPM_SPK_24BIT_ENABLE
    audio_alg_interface_t *p_alg_ppm = audio_alg_get_interface_by_type(ALG_PPM_SPK_24BIT);
#else
    audio_alg_interface_t *p_alg_ppm = audio_alg_get_interface_by_type(ALG_PPM_SPK);
#endif
    if (g_ppm_state.update_flag_spk) {
        g_ppm_state.update_flag_spk = false;
#if TLKALG_PPM_CALC_BY_SAMPLE
        int ppm_val = -tlkalg_ppm_get_ppm_val();
#else
        int ppm_val = tlkalg_ppm_get_ppm_val();
#endif
        p_alg_ppm->audio_alg_param_set(0, &ppm_val);
        // tlk_printf("ppm val %d", ppm_val);
    }
#else
    int32_t *pdes = (int32_t *)pData;
#endif


    for (uint16_t index = 0; index < length; index++) {
        int32_t out0 = tlkusb_uac_get_iso_out_data();
#if TLK_USB_UAC_DUAL_SOUNDCARD_MODE
        int32_t out1 = tlkusb_uac_get_iso_out1_data();

        pdes[index] = (out0 >> 1) + (out1 >> 1);
#else
        pdes[index] = out0;
#endif
#if (!TLKUAC_PPM_SPK_24BIT_ENABLE) & (TLKALG_PPM_USB_CALC_ENABLE || TLKALG_PPM_CALC_BY_SAMPLE)
        ppm_in_buff_16b[index] = (int16_t)(pdes[index] >> 8);
#endif
    }

#if (TLKALG_PPM_USB_CALC_ENABLE || TLKALG_PPM_CALC_BY_SAMPLE)
#if TLKUAC_PPM_SPK_24BIT_ENABLE
    int out_len = p_alg_ppm->audio_alg_process((uint8_t *)pdes, (uint8_t *)ppm_out_buff, stereo_len, ALG_WIDTH_24, ALG_CHANNEL_STEREO);
#else
    int out_len = p_alg_ppm->audio_alg_process((uint8_t *)ppm_in_buff_16b, (uint8_t *)ppm_out_buff_16b, stereo_len, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
#endif

    for (int j = 0; j < out_len * 2; j++) {
#if TLKUAC_PPM_SPK_24BIT_ENABLE
        g_ppm_calc_ctl.ppm_buff_spk[g_ppm_calc_ctl.alg_wptr_spk++] = ppm_out_buff[j];
#else
        g_ppm_calc_ctl.ppm_buff_spk[g_ppm_calc_ctl.alg_wptr_spk++] = (int32_t)(ppm_out_buff_16b[j] << 8);
#endif
        g_ppm_calc_ctl.alg_wptr_spk %= TLKALG_PPM_BUFF_SPK_LEN;
    }

#if TLKALG_PPM_CALC_BY_SAMPLE
    //feedback alg output result to ppm calc, make ppm calc more accurate
    int diff = out_len - stereo_len;
    if (diff && tlkalg_get_sample_ppm_mode(&g_uac_ppm_ctrl, ASRC_MUSIC_MODE | ASRC_MUSIC1_MODE)) {
        tlkalg_add_sample_ppm_diff_samples(&g_uac_ppm_ctrl, -diff * tlkusb_uac_get_iso_out_Channels());
    }
#elif TLKALG_PPM_USB_CALC_ENABLE
    int diff = out_len - stereo_len;
    if (diff) {
        //         tlk_printf("out in len %d %d ", out_len, stereo_len);
        g_ppm_calc_ctl.sample_diff_spk = diff;
        g_ppm_state.asrc_diff += diff;
    }
#endif
    for (int i = 0; i < dataLen; i++) {
        *pData++ = g_ppm_calc_ctl.ppm_buff_spk[g_ppm_calc_ctl.alg_rptr_spk++];
        g_ppm_calc_ctl.alg_rptr_spk %= TLKALG_PPM_BUFF_SPK_LEN;
    }
#endif

    return dataLen;
}
#else
/**
 * @brief       This function writes data to ISO OUT buffer for 16-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
_attribute_ram_code_sec_ uint16_t tlkusb_uac_write_iso_out_samples(int16_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen == 0 || tlkusb_uac_get_iso_out_idle_samples() < dataLen) {
        return 0;
    }

    for (uint16_t index = 0; index < dataLen; index++) {
        g_tlk_usb_cfg.iso_out[g_tlk_usb_cfg.out_w & APP_USB_ISO_OUT_BUFF_IDX_MASK] = pData[index];
        g_tlk_usb_cfg.out_w++;
        g_tlk_usb_cfg.out_w &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
    }

    return dataLen;
}

/**
 * @brief       This function reads data from ISO OUT buffer for 16-bit resolution.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
uint16_t tlkusb_uac_read_iso_out_samples(int16_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen == 0) {
        return 0;
    }

    if (tlkusb_uac_get_iso_out_used_samples() < dataLen) {
        return 0;
    }

    for (uint16_t index = 0; index < dataLen; index++) {
        int16_t out0 = g_tlk_usb_cfg.iso_out[g_tlk_usb_cfg.out_r & APP_USB_ISO_OUT_BUFF_IDX_MASK];
#if (TLK_USB_UAC_ENABLE)
        uint audio_volume = tlkusb_uacspk_getVolume();
        out0              = (out0 * audio_volume + 0x2000) >> 14;
#endif
        g_tlk_usb_cfg.out_r++;
        g_tlk_usb_cfg.out_r &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
#if TLK_USB_UAC_DUAL_SOUNDCARD_MODE
        int16_t out1 = 0;
        if (tlkusb_uac_get_iso_out1_en()) {
            out1               = g_tlk_usb_cfg.iso_out1[g_tlk_usb_cfg.out1_r & APP_USB_ISO_OUT_BUFF_IDX_MASK];
            uint audio_volume1 = tlkusb_uacspk1_getVolume();
            out1               = (out1 * audio_volume1 + 0x2000) >> 14;
            g_tlk_usb_cfg.out1_r++;
            g_tlk_usb_cfg.out1_r &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
        }

        pData[index] = (out0 >> 1) + (out1 >> 1);
#else
        pData[index] = out0;
#endif
    }
    return dataLen;
}
#endif

/*iso in*/
/**
 * @brief       This function reads data from ISO IN buffer.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
uint16_t tlkusb_uac_read_iso_in_samples(int16_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen == 0 || tlkusb_uac_get_iso_in_used_samples() < dataLen) {
        tlk_printf("tlkusb_uac_read_iso_in_samples no enough data");
        return 0;
    }

    for (uint16_t index = 0; index < dataLen; index++) {
        pData[index] = g_tlk_usb_cfg.iso_in[g_tlk_usb_cfg.in_r & APP_USB_ISO_IN_BUFF_IDX_MASK];
        g_tlk_usb_cfg.in_r++;
        g_tlk_usb_cfg.in_r &= APP_USB_ISO_IN_BUFF_IDX_MASK;
    }

    return dataLen;
}

/**
 * @brief       This function gets the number of used samples in ISO IN buffer.
 * @return      Number of used samples in ISO IN buffer.
 */
_attribute_ram_code_sec_ uint16_t tlkusb_uac_get_iso_in_used_samples()
{
    return (g_tlk_usb_cfg.in_w - g_tlk_usb_cfg.in_r) & APP_USB_ISO_IN_BUFF_IDX_MASK;
}

/**
 * @brief       This function gets the number of idle samples in ISO IN buffer.
 * @return      Number of idle samples in ISO IN buffer.
 */
uint16_t tlkusb_uac_get_iso_in_idle_samples()
{
    return (APP_USB_ISO_IN_BUFF_SIZE - tlkusb_uac_get_iso_in_used_samples());
}

#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
/**
 * @brief       This function writes data to ISO IN buffer for 24-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_iso_in_samples(int32_t *pData, uint16_t dataLen)
{
    if (tlkusb_uac_get_iso_in_idle_samples() < dataLen) {
        return 0;
    }
    for (uint16_t index = 0; index < dataLen; index++) {
        g_tlk_usb_cfg.iso_in[g_tlk_usb_cfg.in_w & APP_USB_ISO_IN_BUFF_IDX_MASK] = pData[index];
        g_tlk_usb_cfg.in_w++;
        g_tlk_usb_cfg.in_w &= APP_USB_ISO_IN_BUFF_IDX_MASK;
    }

    return dataLen;
}
#else
/**
 * @brief       This function writes data to ISO IN buffer for 16-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_iso_in_samples(int16_t *pData, uint16_t dataLen)
{
    if (tlkusb_uac_get_iso_in_idle_samples() < dataLen) {
        return 0;
    }
    uint16_t length = dataLen;
    int16_t *pdes   = pData;
#if TLKALG_UAC_PPM_MIC_ENABLE & TLKALG_PPM_USB_CALC_ENABLE
    audio_alg_interface_t *p_alg_ppm = audio_alg_get_interface_by_type(ALG_PPM_MIC);
    if (g_ppm_state.update_flag_mic) {
        g_ppm_state.update_flag_mic = false;
        int ppm_val                 = -tlkalg_ppm_get_ppm_val();
        p_alg_ppm->audio_alg_param_set(0, &ppm_val);
        // tlk_printf("mic ppm val %d", ppm_val);
    }

    int16_t ppm_out_buffer[16 + 1];
    int     out_len = p_alg_ppm->audio_alg_process((uint8_t *)pData, (uint8_t *)ppm_out_buffer, dataLen, ALG_WIDTH_16, ALG_CHANNEL_LEFT);

    length   = out_len;
    pdes     = ppm_out_buffer;
    int diff = out_len - dataLen;
    if (diff != 0) {
        // tlkapi_printf(APP_LOG_EN, "out in len %d %d ", out_len, sample_num);
        g_ppm_calc_ctl.sample_diff_mic = diff;
        //!!!!!!!!!!!!!!!!!!!!if only mic exist, the asrc_diff need set here!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //if (!spk_en)
        // g_ppm_state.asrc_diff += (diff*3);//because the TLKALG_PPM_TICK_PER_SAMPLE = 500 according to 48k sr in ppm calc
        //16 sr: tick_per_sample = 1500
    }
#endif

    for (uint16_t index = 0; index < length; index++) {
        g_tlk_usb_cfg.iso_in[g_tlk_usb_cfg.in_w & APP_USB_ISO_IN_BUFF_IDX_MASK] = pdes[index];
        g_tlk_usb_cfg.in_w++;
        g_tlk_usb_cfg.in_w &= APP_USB_ISO_IN_BUFF_IDX_MASK;
    }

    return length;
}
#endif


/*ppm out*/
/**
 * @brief       This function gets the number of used samples in PPM OUT buffer.
 * @return      Number of used samples in PPM OUT buffer.
 */
_attribute_ram_code_sec_ uint16_t tlkusb_uac_get_ppm_out_used_samples()
{
    return (g_tlk_usb_cfg.ppm_out_w - g_tlk_usb_cfg.ppm_out_r) & APP_USB_PPM_OUT_BUFF_IDX_MASK;
}

/**
 * @brief       This function gets the number of idle samples in PPM OUT buffer.
 * @return      Number of idle samples in PPM OUT buffer.
 */
_attribute_ram_code_sec_ uint16_t tlkusb_uac_get_ppm_out_idle_samples()
{
    return (APP_USB_PPM_OUT_BUFF_SIZE - tlkusb_uac_get_ppm_out_used_samples());
}

#if ((MCU_CORE_TYPE != MCU_CORE_TL322X))
#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
/**
 * @brief       This function writes data to PPM OUT buffer for 24-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
_attribute_ram_code_sec_noinline_ uint16_t tlkusb_uac_write_ppm_out_samples(uint32_t *pData, uint16_t dataLen)
#else
/**
 * @brief       This function writes data to PPM OUT buffer for 16-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
_attribute_ram_code_sec_noinline_ uint16_t tlkusb_uac_write_ppm_out_samples(uint16_t *pData, uint16_t dataLen)
#endif
{
    if (pData == NULL || dataLen == 0 || tlkusb_uac_get_ppm_out_idle_samples() < dataLen) {
        return 0;
    }

    for (uint16_t index = 0; index < dataLen; index++) {
        g_tlk_usb_cfg.ppm_out[g_tlk_usb_cfg.ppm_out_w & APP_USB_PPM_OUT_BUFF_IDX_MASK] = pData[index];

        g_tlk_usb_cfg.ppm_out_w++;
        g_tlk_usb_cfg.ppm_out_w &= APP_USB_PPM_OUT_BUFF_IDX_MASK;
    }

    return dataLen;
}

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
/**
 * @brief       This function reads data from PPM OUT buffer for 24-bit resolution.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
uint16_t tlkusb_uac_read_ppm_out_samples(int32_t *pData, uint16_t dataLen)
#else
/**
 * @brief       This function reads data from PPM OUT buffer for 16-bit resolution.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
uint16_t tlkusb_uac_read_ppm_out_samples(int16_t *pData, uint16_t dataLen)
#endif
{
    if (pData == NULL || dataLen == 0) {
        return 0;
    }

    if (tlkusb_uac_get_ppm_out_used_samples() < dataLen) {
        return 0;
    }

    for (uint index = 0; index < dataLen; index++) {
        pData[index] = g_tlk_usb_cfg.ppm_out[g_tlk_usb_cfg.ppm_out_r & APP_USB_PPM_OUT_BUFF_IDX_MASK];
        g_tlk_usb_cfg.ppm_out_r++;
        g_tlk_usb_cfg.ppm_out_r &= APP_USB_PPM_OUT_BUFF_IDX_MASK;
    }

    return dataLen;
}
#endif


/*ppm in*/
#if ((MCU_CORE_TYPE != MCU_CORE_TL322X))
/**
 * @brief       This function gets the number of used samples in PPM IN buffer.
 * @return      Number of used samples in PPM IN buffer.
 */
_attribute_ram_code_sec_ uint16_t tlkusb_uac_get_ppm_in_used_samples()
{
    return (g_tlk_usb_cfg.ppm_in_w - g_tlk_usb_cfg.ppm_in_r) & APP_USB_PPM_IN_BUFF_IDX_MASK;
}

/**
 * @brief       This function gets the number of idle samples in PPM IN buffer.
 * @return      Number of idle samples in PPM IN buffer.
 */
_attribute_ram_code_sec_ uint16_t tlkusb_uac_get_ppm_in_idle_samples()
{
    return APP_USB_PPM_IN_BUFF_SIZE - tlkusb_uac_get_ppm_in_used_samples();
}

/**
 * @brief       This function synchronizes PPM IN samples by adjusting the read pointer.
 * @param[in]   num - Number of samples to synchronize.
 * @return      none.
 */
void tlkusb_uac_sync_ppm_in_samples(uint16_t num)
{
    if (num > APP_USB_PPM_IN_BUFF_SIZE) {
        return;
    }

    g_tlk_usb_cfg.ppm_in_r = ((g_tlk_usb_cfg.ppm_in_w - num + APP_USB_PPM_IN_BUFF_SIZE) % APP_USB_PPM_IN_BUFF_SIZE);
}

#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
/**
 * @brief       This function writes data to PPM IN buffer for 24-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_ppm_in_samples(int32_t *pData, uint16_t dataLen)
{
    if (tlkusb_uac_get_ppm_in_idle_samples() < dataLen) {
        /*if ppm in buffer is not enough, drop data*/
        return 0;
    }

    for (uint16_t index = 0; index < dataLen; index++) {
        g_tlk_usb_cfg.ppm_in[g_tlk_usb_cfg.ppm_in_w & APP_USB_PPM_IN_BUFF_IDX_MASK] = pData[index];
        g_tlk_usb_cfg.ppm_in_w++;
        g_tlk_usb_cfg.ppm_in_w &= APP_USB_PPM_IN_BUFF_IDX_MASK;
    }

    return dataLen;
}
#else
/**
 * @brief       This function writes data to PPM IN buffer for 16-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_ppm_in_samples(int16_t *pData, uint16_t dataLen)
{
    if (tlkusb_uac_get_ppm_in_idle_samples() < dataLen) {
        /*if ppm in buffer is not enough, drop data*/
        return 0;
    }

    for (uint16_t index = 0; index < dataLen; index++) {
        g_tlk_usb_cfg.ppm_in[g_tlk_usb_cfg.ppm_in_w & APP_USB_PPM_IN_BUFF_IDX_MASK] = pData[index];
        g_tlk_usb_cfg.ppm_in_w++;
        g_tlk_usb_cfg.ppm_in_w &= APP_USB_PPM_IN_BUFF_IDX_MASK;
    }

    return dataLen;
}
#endif
#endif

#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
/**
 * @brief       This function writes data to IN buffer for 24-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_in_samples(int32_t *pData, uint16_t dataLen)
{
#if (TLKLIB_UAC_PPM_MIC_ENABLE)
    return tlkusb_uac_write_ppm_in_samples(pData, dataLen);
#else
    return tlkusb_uac_write_iso_in_samples(pData, dataLen);
#endif
}
#else
/**
 * @brief       This function writes data to IN buffer for 16-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_in_samples(int16_t *pData, uint16_t dataLen)
{
#if (TLKLIB_UAC_PPM_SPK_ENABLE && !TLKADU_MIDBUF_ENABLE)
    return tlkusb_uac_write_ppm_in_samples(pData, dataLen);
#else
    return tlkusb_uac_write_iso_in_samples(pData, dataLen);
#endif
}
#endif

/**
 * @brief       This function initializes the USB UAC module.
 * @return      TLK_ENONE - success.
 */
int tlkusb_uac_init(void)
{
#if (TLKUSB_UAC_SPK_ENABLE)
    tlkusb_uacspk_init();
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
    tlkusb_uacmic_init();
#endif

    return TLK_ENONE;
}

/**
 * @brief       This function registers the USB UAC status change callback.
 * @param[in]   cb - Callback function to register.
 * @return      none.
 */
void tlkusb_uac_regStatusChangeCB(TlkUsbReportUacStatusCB cb)
{
    sTlkUsbReportUacStatusCB = cb;
}

/**
 * @brief       This function sets the USB UAC state.
 * @param[in]   state - New state to set.
 * @return      none.
 */
void tlkusb_uac_set_state(tlkusb_uac_state_e state)
{
    if ((state != TLKUSB_UAC_MUSIC) && (state != TLKUSB_UAC_VOICE) && (state != TLKUSB_UAC_IDLE)) {
        return;
    }

    tlkuac_uac_state = state;
}

/**
 * @brief       This function gets the current USB UAC state.
 * @return      Current USB UAC state.
 */
_always_inline tlkusb_uac_state_e tlkusb_uac_get_state(void)
{
    return tlkuac_uac_state;
}


#if (TLK_USB_UAC_ENABLE)
/**
 * @brief       This function handles USB UAC interrupts.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void tlkusb_uacirq_handler(void)
{
    uint8_t  irq  = reg_usb_ep_irq_status;
    uint32_t tick = (clock_time() - 2) | 1;

#if (TLKUSB_UAC_MIC_ENABLE)
    if (irq & FLD_USB_EDP7_IRQ) {
        tlkusb_uacmic_fillData(tick);
    }
#endif
#if (TLKUSB_UAC_SPK_ENABLE)
    if (irq & FLD_USB_EDP6_IRQ) {
        tlkusb_uacspk_recvData(tick);
    }

#if (TLK_USB_UAC_DUAL_SOUNDCARD_MODE)
    if (irq & FLD_USB_EDP5_IRQ) {
        tlkusb_uacspk1_recvData(tick);
    }
#endif

#endif

#if TLKALG_PPM_USB_CALC_ENABLE && !TLKALG_PPM_CALC_BY_SAMPLE
    static uint32_t last_tick = 0;
    if ((tick - last_tick >= 500 * SYSTEM_TIMER_TICK_1US) && last_tick != 0) {
        tlkalg_ppm_calc_process(tick);
    }
    last_tick = tick;
#endif

    // DBG_COMMON_CHN7_LOW;

    tlkusb_hal_wakeup_usb_thread_fromIsr();
}
#endif

/**
 * @brief       This function sets the feature mute state.
 * @param[in]   id   - Feature ID.
 * @param[in]   mute - Mute state to set.
 * @return      none.
 */
void tlkusb_uac_setFeatureMute(int id, int mute)
{
    int b = (id >> 8) & 15;
    if ((id & 0xff) < 2) {
        b |= (1 << 4);
    }
    if (mute) {
        sTlkUsbUacMute |= (1 << b);
    } else {
        sTlkUsbUacMute &= ~(1 << b);
    }
}

/**
 * @brief       This function gets the feature mute state.
 * @param[in]   id - Feature ID.
 * @return      Mute state.
 */
uint tlkusb_uac_getFeatureMute(int id)
{
    int b = (id >> 8) & 15;
    if ((id & 0xff) < 2) {
        b |= (1 << 4);
    }
    return (sTlkUsbUacMute >> b) & 1;
}

/**
 * @brief       This function gets the ISO IN sample rate.
 * @return      ISO IN sample rate.
 */
uint32_t tlkusb_uac_get_iso_in_SampleRate()
{
    return g_tlk_usb_cfg.in_sample_rate;
}

/**
 * @brief       This function gets the ISO OUT sample rate.
 * @return      ISO OUT sample rate.
 */
uint32_t tlkusb_uac_get_iso_out_SampleRate()
{
    return g_tlk_usb_cfg.out_sample_rate;
}

uint32_t tlkusb_uac_get_iso_out1_SampleRate()
{
    return g_tlk_usb_cfg.out1_sample_rate;
}

/*TODO: Yating: Use parameters instead of macros.*/
/**
 * @brief       This function gets the ISO IN bit depth.
 * @return      ISO IN bit depth.
 */
uint8_t tlkusb_uac_get_iso_in_BitDepth()
{
    return TLKUSB_AUD_MIC_RESOLUTION_BIT;
}

/**
 * @brief       This function gets the ISO OUT bit depth.
 * @return      ISO OUT bit depth.
 */
uint8_t tlkusb_uac_get_iso_out_BitDepth()
{
    return TLKUSB_AUD_SPK_RESOLUTION_BIT;
}

/**
 * @brief       This function gets the ISO IN channel count.
 * @return      ISO IN channel count.
 */
uint8_t tlkusb_uac_get_iso_in_Channels()
{
    return TLKUSB_AUD_MIC_CHANNEL_COUNT;
}

/**
 * @brief       This function gets the ISO OUT channel count.
 * @return      ISO OUT channel count.
 */
uint8_t tlkusb_uac_get_iso_out_Channels()
{
    return TLKUSB_AUD_SPK_CHANNEL_COUNT;
}

/**
 * @brief       This function checks if ISO IN is enabled.
 * @return      true if ISO IN is enabled, false otherwise.
 */
bool tlkusb_uac_get_iso_in_enable()
{
    return g_tlk_usb_cfg.iso_in_en;
}

/**
 * @brief       This function checks if ISO OUT is enabled.
 * @return      true if ISO OUT is enabled, false otherwise.
 */
bool tlkusb_uac_get_iso_out_enable()
{
    return g_tlk_usb_cfg.iso_out_en || g_tlk_usb_cfg.iso_out1_en;
}

/**
 * @brief       This function updates the pending tick.
 * @return      none.
 */
void tlkusb_uac_update_pending_tick()
{
    g_tlk_usb_cfg.pend_tick = clock_time() | 1;
}

/**
 * @brief       This function registers the USB UAC event to status callback.
 * @param[in]   cb - Callback function to register.
 * @return      none.
 */
void tlkusb_usc_reg_evt2status_cb(TlkUsbUacEvt2StatusCB cb)
{
    if (cb != NULL) {
        sTlkUsbUacEvt2StatusCB = cb;
    }
}

int tlkusb_uac_get_iso_in_en(void)
{
    return g_tlk_usb_cfg.iso_in_en;
}

int tlkusb_uac_get_iso_out_en(void)
{
    return g_tlk_usb_cfg.iso_out_en;
}

int tlkusb_uac_get_iso_out1_en(void)
{
    return g_tlk_usb_cfg.iso_out1_en;
}
#endif // #if (TLK_USB_UAC_ENABLE)
