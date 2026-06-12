/********************************************************************************************************
 * @file    tlkusb_uac.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef TLKUSB_AUDIO_H
#define TLKUSB_AUDIO_H

#include "tlkusb_uacDefine.h"

typedef void (*TlkUsbUacEvt2StatusCB)(uint8_t evtID);
typedef void (*TlkUsbReportUacStatusCB)(bool isMicEn, bool isSpkEn);

extern TlkUsbUacEvt2StatusCB sTlkUsbUacEvt2StatusCB;

extern TlkUsbReportUacStatusCB sTlkUsbReportUacStatusCB;

typedef struct
{
    bool    iso_in_en;   /*iso in  stream enable*/
    bool    iso_out_en;  /*iso out stream enable*/
    bool    iso_out1_en; /*iso out stream enable*/
    uint8_t out_mute;
    uint8_t out1_mute;
    uint8_t in_mute;

    uint16_t in_volume;
    uint16_t out_volume;
    uint16_t out1_volume;
    uint16_t out_vol_step;
    uint16_t out1_vol_step;
    uint16_t in_vol_step;

    int32_t  in_w;      /**< Write position of bi buffer. */
    int32_t  in_r;      /**< Read position of bi buffer. */
    int32_t  out_w;     /**< Write position of bo buffer. */
    int32_t  out_r;     /**< Read position of bo buffer. */
    int32_t  out1_w;    /**< Write position of bo buffer. */
    int32_t  out1_r;    /**< Read position of bo buffer. */
    uint32_t tick_in;   /**< Tick time of iso-in interrupt. */
    uint32_t tick_out;  /**< Tick time of iso-out interrupt. */
    uint32_t tick_out1; /**< Tick time of iso-out interrupt. */

    uint32_t in_sample_rate;
    uint32_t out_sample_rate;
    uint32_t out1_sample_rate;
    uint32_t pend_tick; /*Just used for Headphone storage during btsco, uac triggers sco setup again between sco disconnection and detach.*/

    int32_t ppm_in_w;  /**< Write position of bi buffer. */
    int32_t ppm_in_r;  /**< Read position of bi buffer. */
    int32_t ppm_out_w; /**< Write position of bo buffer. */
    int32_t ppm_out_r; /**< Read position of bo buffer. */

#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
    uint32_t iso_in[APP_USB_ISO_IN_BUFF_SIZE]; /**< Buffer used to store data fetched from the iso in endpoint */
#if TLKALG_PPM_SPK_ENABLE && TLKALG_PPM_MIC_ENABLE
    uint32_t ppm_in[APP_USB_PPM_IN_BUFF_SIZE]; /**< Buffer used to store data fetched from the iso in endpoint */
#else
    uint32_t ppm_in[16]; /**< Buffer used to store data fetched from the iso in endpoint */
#endif
#else
    uint16_t iso_in[APP_USB_ISO_IN_BUFF_SIZE]; /**< Buffer used to store data fetched from the iso in endpoint */
#if ((MCU_CORE_TYPE != MCU_CORE_TL322X))
    uint16_t ppm_in[APP_USB_PPM_IN_BUFF_SIZE]; /**< Buffer used to store data fetched from the iso in endpoint */
#endif
#endif

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
    uint32_t iso_out[APP_USB_ISO_OUT_BUFF_SIZE]; /**< Buffer used to store data fetched from the iso out endpoint */
#if (TLK_USB_UAC_DUAL_SOUNDCARD_MODE)
    uint32_t iso_out1[APP_USB_ISO_OUT_BUFF_SIZE]; /**< Buffer used to store data fetched from the iso out endpoint */
#endif
#if TLKALG_PPM_SPK_ENABLE && TLKALG_PPM_MIC_ENABLE
    uint32_t ppm_out[APP_USB_PPM_OUT_BUFF_SIZE]; /**< Buffer used to store data fetched from the iso out endpoint */
#else
    uint32_t ppm_out[16]; /**< Buffer used to store data fetched from the iso out endpoint */
#endif
#else
    uint16_t iso_out[APP_USB_ISO_OUT_BUFF_SIZE]; /**< Buffer used to store data fetched from the iso out endpoint */
#if (TLK_USB_UAC_DUAL_SOUNDCARD_MODE)
    uint16_t iso_out1[APP_USB_ISO_OUT_BUFF_SIZE]; /**< Buffer used to store data fetched from the iso out endpoint */
#endif
#if ((MCU_CORE_TYPE != MCU_CORE_TL322X))
    uint16_t ppm_out[APP_USB_PPM_OUT_BUFF_SIZE]; /**< Buffer used to store data fetched from the iso out endpoint */
#endif
#endif

} tlkusb_iso_config_t;

extern tlkusb_iso_config_t g_tlk_usb_cfg;

/**
 * @brief       This function gets the number of used samples in ISO OUT buffer.
 * @return      Number of used samples in ISO OUT buffer.
 */
uint16_t tlkusb_uac_get_iso_out_used_samples();

/**
 * @brief       This function gets the number of idle samples in ISO OUT buffer.
 * @return      Number of idle samples in ISO OUT buffer.
 */
uint16_t tlkusb_uac_get_iso_out_idle_samples();

/**
 * @brief       This function synchronizes ISO OUT samples by adjusting the read pointer.
 * @param[in]   num - Number of samples to synchronize.
 * @return      none.
 */
void tlkusb_uac_sync_iso_out_samples(uint16_t num);

/**
 * @brief       This function gets the number of used samples in ISO IN buffer.
 * @return      Number of used samples in ISO IN buffer.
 */
uint16_t tlkusb_uac_get_iso_in_used_samples();

/**
 * @brief       This function gets the number of idle samples in ISO IN buffer.
 * @return      Number of idle samples in ISO IN buffer.
 */
uint16_t tlkusb_uac_get_iso_in_idle_samples();

/**
 * @brief       This function gets the number of used samples in PPM OUT buffer.
 * @return      Number of used samples in PPM OUT buffer.
 */
uint16_t tlkusb_uac_get_ppm_out_used_samples();

/**
 * @brief       This function gets the number of idle samples in PPM OUT buffer.
 * @return      Number of idle samples in PPM OUT buffer.
 */
uint16_t tlkusb_uac_get_ppm_out_idle_samples();

/**
 * @brief       This function gets the number of used samples in PPM IN buffer.
 * @return      Number of used samples in PPM IN buffer.
 */
uint16_t tlkusb_uac_get_ppm_in_used_samples();

/**
 * @brief       This function gets the number of idle samples in PPM IN buffer.
 * @return      Number of idle samples in PPM IN buffer.
 */
uint16_t tlkusb_uac_get_ppm_in_idle_samples();

/**
 * @brief       This function synchronizes PPM IN samples by adjusting the read pointer.
 * @param[in]   num - Number of samples to synchronize.
 * @return      none.
 */
void tlkusb_uac_sync_ppm_in_samples(uint16_t num);

/**
 * @brief       This function clears the ISO OUT buffer by setting read pointer equal to write pointer.
 * @return      none.
 */
void tlkusb_uac_clear_iso_out_buffer();

/**
 * @brief       This function resets the USB UAC output configuration.
 * @return      none.
 */
void tlkusb_uac_reset_out_config();
void tlkusb_uac_reset_out1_config();

/**
 * @brief       This function resets the USB UAC input configuration.
 * @return      none.
 */
void tlkusb_uac_reset_in_config();

/**
 * @brief       This function sets the ISO OUT read pointer.
 * @param[in]   sample - Sample position to set the read pointer.
 * @return      none.
 */
void tlkusb_uac_set_iso_out_rptr(uint16_t sample);

#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
/**
 * @brief       This function writes data to PPM IN buffer for 24-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_ppm_in_samples(int32_t *pData, uint16_t dataLen);

/**
 * @brief       This function writes data to ISO IN buffer for 24-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_iso_in_samples(int32_t *pData, uint16_t dataLen);
#else

/**
 * @brief       This function writes data to PPM IN buffer for 16-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_ppm_in_samples(int16_t *pData, uint16_t dataLen);

/**
 * @brief       This function writes data to ISO IN buffer for 16-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_iso_in_samples(int16_t *pData, uint16_t dataLen);

/**
 * @brief       This function reads data from ISO IN buffer.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
uint16_t tlkusb_uac_read_iso_in_samples(int16_t *pData, uint16_t dataLen);
#endif

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
/**
 * @brief       This function writes data to ISO OUT buffer for 24-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_iso_out_samples(int32_t *pData, uint16_t dataLen);

/**
 * @brief       This function reads data from PPM OUT buffer for 24-bit resolution.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
uint16_t tlkusb_uac_read_ppm_out_samples(int32_t *pData, uint16_t dataLen);

/**
 * @brief       This function reads data from ISO OUT buffer for 24-bit resolution.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
uint16_t tlkusb_uac_read_iso_out_samples(int32_t *pData, uint16_t dataLen);

/**
 * @brief       This function writes data to PPM OUT buffer for 24-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_ppm_out_samples(uint32_t *pData, uint16_t dataLen);
#else
/**
 * @brief       This function writes data to ISO OUT buffer for 16-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_iso_out_samples(int16_t *pData, uint16_t dataLen);

/**
 * @brief       This function reads data from PPM OUT buffer for 16-bit resolution.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
uint16_t tlkusb_uac_read_ppm_out_samples(int16_t *pData, uint16_t dataLen);

/**
 * @brief       This function reads data from ISO OUT buffer for 16-bit resolution.
 * @param[out]  pData   - Pointer to the buffer to store read data.
 * @param[in]   dataLen - Length of data to read.
 * @return      Number of samples read.
 */
uint16_t tlkusb_uac_read_iso_out_samples(int16_t *pData, uint16_t dataLen);

/**
 * @brief       This function writes data to PPM OUT buffer for 16-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_ppm_out_samples(uint16_t *pData, uint16_t dataLen);
#endif

#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
/**
 * @brief       This function writes data to IN buffer for 24-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_in_samples(int32_t *pData, uint16_t dataLen);
#else
/**
 * @brief       This function writes data to IN buffer for 16-bit resolution.
 * @param[in]   pData   - Pointer to the data to write.
 * @param[in]   dataLen - Length of data to write.
 * @return      Number of samples written.
 */
uint16_t tlkusb_uac_write_in_samples(int16_t *pData, uint16_t dataLen);
#endif

/**
 * @brief       This function gets the ISO IN sample rate.
 * @return      ISO IN sample rate.
 */
uint32_t tlkusb_uac_get_iso_in_SampleRate();

/**
 * @brief       This function gets the ISO OUT sample rate.
 * @return      ISO OUT sample rate.
 */
uint32_t tlkusb_uac_get_iso_out_SampleRate();
uint32_t tlkusb_uac_get_iso_out1_SampleRate();

/**
 * @brief       This function gets the ISO IN bit depth.
 * @return      ISO IN bit depth.
 */
uint8_t tlkusb_uac_get_iso_in_BitDepth();

/**
 * @brief       This function gets the ISO OUT bit depth.
 * @return      ISO OUT bit depth.
 */
uint8_t tlkusb_uac_get_iso_out_BitDepth();

/**
 * @brief       This function gets the ISO IN channel count.
 * @return      ISO IN channel count.
 */
uint8_t tlkusb_uac_get_iso_in_Channels();

/**
 * @brief       This function gets the ISO OUT channel count.
 * @return      ISO OUT channel count.
 */
uint8_t tlkusb_uac_get_iso_out_Channels();

/**
 * @brief       This function checks if ISO IN is enabled.
 * @return      true if ISO IN is enabled, false otherwise.
 */
bool tlkusb_uac_get_iso_in_enable();

/**
 * @brief       This function checks if ISO OUT is enabled.
 * @return      true if ISO OUT is enabled, false otherwise.
 */
bool tlkusb_uac_get_iso_out_enable();

typedef enum
{
    TLKUSB_UAC_IDLE = 0,
    TLKUSB_UAC_MUSIC,
    TLKUSB_UAC_VOICE,
} tlkusb_uac_state_e;

enum
{
    TLK_UAC_MUSIC_START_RSP,
    TLK_UAC_MUSIC_STOP_RSP,
    TLK_UAC_VOICE_START_RSP,
    TLK_UAC_VOICE_STOP_RSP,

    TLK_UAC_EVT_NUM
};

int tlkusb_uac_init(void);


extern void tlkusb_uac_regStatusChangeCB(TlkUsbReportUacStatusCB cb);
extern void tlkusb_usc_reg_evt2status_cb(TlkUsbUacEvt2StatusCB cb);

extern void tlkusb_uacirq_handler(void);

void               tlkusb_uac_set_state(tlkusb_uac_state_e state);
tlkusb_uac_state_e tlkusb_uac_get_state(void);
void               tlkusb_uac_update_pending_tick();

int tlkusb_uac_get_iso_in_en(void);
int tlkusb_uac_get_iso_out_en(void);
int tlkusb_uac_get_iso_out1_en(void);
#endif // TLKUSB_AUDIO_H
