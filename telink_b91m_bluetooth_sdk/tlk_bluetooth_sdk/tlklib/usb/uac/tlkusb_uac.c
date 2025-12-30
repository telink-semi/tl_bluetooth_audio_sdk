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


extern const tlkusb_modCtrl_t sTlkUsbUacModCtrl;
extern const tlkusb_modDesc_t sTlkUsbUacModDesc;

_attribute_data_retention_sec_ TlkUsbUacEvt2StatusCB sTlkUsbUacEvt2StatusCB = NULL;
TlkUsbReportUacStatusCB sTlkUsbReportUacStatusCB;

int                sTlkUsbUacMute      = 0;
uint32_t             sTlkUsbUacSamplRate = 16000;
tlkusb_uac_state_e tlkuac_uac_state    = TLKUSB_UAC_IDLE;

const tlkusb_module_t gTlkUsbUacModule = {
    .modType = TLKUSB_MODTYPE_UAC,
    .pDesc = &sTlkUsbUacModDesc,
    .pCtrl = &sTlkUsbUacModCtrl,
};

tlkusb_iso_config_t g_tlk_usb_cfg = {0};

/**
 * @brief       This function resets the USB UAC output configuration.
 * @return      none.
 */
void tlkusb_uac_reset_out_config()
{
    g_tlk_usb_cfg.tick_out = 0;
    g_tlk_usb_cfg.iso_out_en = false;
    g_tlk_usb_cfg.out_w = 0;
    g_tlk_usb_cfg.out_r = 0;
    g_tlk_usb_cfg.ppm_out_w = 0;
    g_tlk_usb_cfg.ppm_out_r = 0;
}

/**
 * @brief       This function resets the USB UAC input configuration.
 * @return      none.
 */
void tlkusb_uac_reset_in_config()
{
    g_tlk_usb_cfg.tick_in = 0;
    g_tlk_usb_cfg.iso_in_en = false;
    g_tlk_usb_cfg.in_w = 0;
    g_tlk_usb_cfg.in_r = 0;
    g_tlk_usb_cfg.ppm_in_r = 0;
    g_tlk_usb_cfg.ppm_in_w = 0;
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
    g_tlk_usb_cfg.out_r = (g_tlk_usb_cfg.out_w - sample) & APP_USB_ISO_OUT_BUFF_IDX_MASK;
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
uint16_t tlkusb_uac_read_iso_out_samples(int32_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen == 0) {
        return 0;
    }

    if (tlkusb_uac_get_iso_out_used_samples() < dataLen) {
        return 0;
    }

    for (uint16_t index = 0; index < dataLen; index++) {
        pData[index] = g_tlk_usb_cfg.iso_out[g_tlk_usb_cfg.out_r & APP_USB_ISO_OUT_BUFF_IDX_MASK];
        int16_t audio_volume;
        static int16_t real_volume = 0;

        audio_volume = tlkusb_uacspk_getVolume();

         /* adjust volume */
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

        pData[index] = ((int64_t)pData[index] * real_volume + 0x2000) >> 14;
        g_tlk_usb_cfg.out_r++;
        g_tlk_usb_cfg.out_r &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
    }

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
        pData[index] = g_tlk_usb_cfg.iso_out[g_tlk_usb_cfg.out_r & APP_USB_ISO_OUT_BUFF_IDX_MASK];
        g_tlk_usb_cfg.out_r++;
        g_tlk_usb_cfg.out_r &= APP_USB_ISO_OUT_BUFF_IDX_MASK;
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
    for(uint16_t index = 0; index < dataLen; index++) {
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
    for(uint16_t index = 0; index < dataLen; index++) {
        g_tlk_usb_cfg.iso_in[g_tlk_usb_cfg.in_w & APP_USB_ISO_IN_BUFF_IDX_MASK] = pData[index];
        g_tlk_usb_cfg.in_w++;
        g_tlk_usb_cfg.in_w &= APP_USB_ISO_IN_BUFF_IDX_MASK;
    }

    return dataLen;
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

    for(uint16_t index = 0; index < dataLen; index++) {
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

    for(uint16_t index = 0; index < dataLen; index++) {
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
#if (TLKLIB_UAC_PPM_SPK_ENABLE)
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
    uint8_t irq  = reg_usb_ep_irq_status;
    uint32_t tick = clock_time() | 1;

    #if (TLKUSB_UAC_MIC_ENABLE)
    if (irq & FLD_USB_EDP7_IRQ) {
        tlkusb_uacmic_fillData(tick);
    }
    #endif
    #if (TLKUSB_UAC_SPK_ENABLE)
    if (irq & FLD_USB_EDP6_IRQ) {
        tlkusb_uacspk_recvData(tick);
    }
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
    return g_tlk_usb_cfg.iso_out_en;
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
    if (cb != NULL){
        sTlkUsbUacEvt2StatusCB = cb;
    }
}

#endif // #if (TLK_USB_UAC_ENABLE)
