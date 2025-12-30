/********************************************************************************************************
 * @file    tlkdrv_codec.c
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
#include "tlkdrv_codec.h"

#if (CHIP_TYPE == CHIP_TYPE_TL721X)
#include "tlkdrv_i2s_master_tl721x.h"
#elif (CHIP_TYPE == CHIP_TYPE_TL751X)
#include "tlkdrv_icodec_tl751x.h"
#elif (CHIP_TYPE == CHIP_TYPE_TL752X)
#include "tlkdrv_icodec_tl752x.h"
#endif

#if (TLK_DEV_CODEC_ENABLE)

#if DEC_PCM_DATA_TO_DSP
#include "tlkmw/audio/anc/tlkmdi_anc.h"
#endif

static const tlkdrv_codec_modinf_t *tlkdrv_codec_getDev(uint8_t dev);

#if ((TLKHW_TYPE == BOARD_721X_EVK_C1T315A20_V2 || TLKHW_TYPE == BOARD_721X_EVK_C1TXA104_V1_1) && (PROJ_TPSLL_AUDIO_DONGLE))
const uint16_t scTlkDrvCodec8kParam[5]   = {4, 1875, 0, 64, 64}; // AUDIO_RATE_EQUAL	8000
const uint16_t scTlkDrvCodec16kParam[5]  = {8, 1875, 0, 64, 64};
const uint16_t scTlkDrvCodec24kParam[5]  = {4, 625, 0, 64, 64};
const uint16_t scTlkDrvCodec32kParam[5]  = {16, 1875, 0, 64, 64};
const uint16_t scTlkDrvCodec44k1Param[5] = {147, 12500, 0, 64, 64};
const uint16_t scTlkDrvCodec48kParam[5]  = {8, 625, 0, 64, 64};
#else
const uint16_t scTlkDrvCodec8kParam[5]  = {8, 125, 12, 64, 64}; // AUDIO_RATE_EQUAL	8000
const uint16_t scTlkDrvCodec16kParam[5] = {8, 125, 6, 64, 64};
const uint16_t scTlkDrvCodec24kParam[5] = {8, 125, 4, 64, 64};
const uint16_t scTlkDrvCodec32kParam[5] = {8, 125, 3, 64, 64};
const uint16_t scTlkDrvCodec48kParam[5] = {2, 125, 0, 64, 64};

#if (TLKDRV_CODEC_I2S_SLAVE_ENABLE || TLKDRV_CODEC_I2S_MASTER_ENABLE)
///Master Clk Rate for AP525, Minimum is 4 kHz; maximum is 56 MHz
const uint16_t scTlkDrvCodec44k1Param[5] = {1, 17, 2, 64, 64};
//const uint16_t scTlkDrvCodec44k1Param[5] = { 19,517,	 2,	40, 40};
#else
const uint16_t scTlkDrvCodec44k1Param[5] = {76, 235, 11, 64, 64};
#endif
#endif


#if (TLKDRV_CODEC_ICODEC_ENABLE)
extern const tlkdrv_codec_modinf_t gcTlkDrvIcodecInf;
#endif

#if (TLKDRV_CODEC_I2S_MASTER_ENABLE)
#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
extern const tlkdrv_codec_modinf_t gcTlkDrvIisMstInf_tl721x;
#endif
#endif

#if (TLKDRV_CODEC_I2S_SLAVE_ENABLE)
#if (MCU_CORE_TYPE == MCU_CORE_B92)
extern const tlkdrv_codec_modinf_t gcTlkDrvIisSlvInf_b92;
#endif
#endif

static const tlkdrv_codec_modinf_t *spTlkDrvCodecModinf[TLKDRV_CODEC_DEV_MAX] = {
    NULL,
#if (TLKDRV_CODEC_ICODEC_ENABLE)
    &gcTlkDrvIcodecInf,
#endif
#if (TLKDRV_CODEC_I2S_SLAVE_ENABLE)
#if (MCU_CORE_TYPE == MCU_CORE_B92)
    &gcTlkDrvIisSlvInf_b92,
#else
    NULL,
#endif
#endif
#if (TLKDRV_CODEC_I2S_MASTER_ENABLE)
#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
    &gcTlkDrvIisMstInf_tl721x,
#else
    NULL,
#endif
#endif
};


volatile int g_sys_power_on_codec_dis = 0;
volatile int g_sys_work_mode          = 0;

#if TLKALG_ANC_ENABLE
volatile int audio_hd_anc_enable = 1;
#else
volatile int audio_hd_anc_enable = 0;
#endif

#if TLK_DEV_CODEC_SPK_ENABLE
#if TLKALG_ANC_ENABLE
//+5 for share variable:
//                      1-wptr
//                      2-rptr
//                      3-mid buff data len
//                      4-mid buff data len update tick
//                      5-mid buff data len updating flag 1:duiring update  0:finish update
__attribute__((section(".dsp_share_mem_d25f"))) codec_int g_codec_spk_buff[CODEC_SPK_FIFO_SAMPLES + 256];
// uint8_t ava_buff[0x26e0] __attribute__((section(".dsp_share_mem_d25f1")));
// uint8_t ava_scr_buff[0x380] __attribute__((section(".dsp_share_mem_d25f2")));
#else
#if (!PROJ_RECORDING_CARD)
#if (!TLK_CFG_TEMP_DRAM_OPTM_TPSLL)
codec_int g_codec_spk_buff[CODEC_SPK_FIFO_SAMPLES];
#else
_attribute_iram_data_ codec_int g_codec_spk_buff[CODEC_SPK_FIFO_SAMPLES];
#endif
#endif
#endif
#endif

#if TLK_DEV_CODEC_MIC_ENABLE
#if (!PROJ_RECORDING_CARD)
#if (!TLK_CFG_TEMP_DRAM_OPTM_TPSLL)
adc_int g_codec_mic_buff[CODEC_MIC_FIFO_SAMPLES];
#else
_attribute_iram_data_ adc_int g_codec_mic_buff[CODEC_MIC_FIFO_SAMPLES];
#endif
#else
adc_mono_int g_codec_mic_buff[CODEC_MIC_FIFO_SAMPLES];
#endif
#endif

static tlkdrv_codec_ctrl_t sTlkDrvCodecCtrl = {
    .spkIsMute      = true,
    .spkIsForceMute = false,
    .minorDev       = 0xFF,
    .majorDev       = 0XFF,
    .micOffset      = 0,
    .spkOffset      = 0,
};
uint16_t gTlkDrvCodecSpkBuffLen = 0;
uint16_t gTlkDrvCodecMicBuffLen = 0;

uint8_t *gpTlkDrvCodecSpkBuffer = NULL;
uint8_t *gpTlkDrvCodecMicBuffer = NULL;

static uint8_t s_anc_mode_enable = 0;

/**
 * @brief Initialize fade-in effect parameters
 * @param[in] fadeInTimeMs - Fade in duration in milliseconds
 * @return None
 */
static void tlkdrv_codec_fade_in_init(uint16_t fadeInTimeMs)
{
    if (fadeInTimeMs == 0) {
        return;
    }
    if (fadeInTimeMs > 4096) {
        fadeInTimeMs = 4096;
    }
    uint32_t sampleRate                    = tlkdrv_codec_getSampleRate();
    sTlkDrvCodecCtrl.fadeCtrl.samplesPerMs = sampleRate / 1000;
    if (tlkdrv_codec_getChannel() == TLKDRV_CODEC_CHANNEL_STEREO) {
        sTlkDrvCodecCtrl.fadeCtrl.samplesPerMs *= 2;
    }
    sTlkDrvCodecCtrl.fadeCtrl.remainSamples = fadeInTimeMs * (sampleRate / 1000);
    sTlkDrvCodecCtrl.fadeCtrl.setTimeMs     = fadeInTimeMs;
    sTlkDrvCodecCtrl.fadeCtrl.remainTimeMs  = fadeInTimeMs;
    sTlkDrvCodecCtrl.fadeCtrl.nowstep       = 0;
}

/**
 * @brief Decrease remaining samples during fade-in process
 * @param[in] samplesPerMs - Number of samples per millisecond
 * @return True if fade-in process completed, otherwise false
 */
_attribute_ram_code_sec_ static inline bool tlkdrv_codec_fade_in_remainSamplesDec(uint32_t samplesPerMs)
{
    sTlkDrvCodecCtrl.fadeCtrl.remainSamples--;
    if (sTlkDrvCodecCtrl.fadeCtrl.remainSamples % samplesPerMs == 0) {
        sTlkDrvCodecCtrl.fadeCtrl.remainTimeMs--;
        uint16_t setTimeMs                = sTlkDrvCodecCtrl.fadeCtrl.setTimeMs;
        sTlkDrvCodecCtrl.fadeCtrl.nowstep = 256 * (setTimeMs - sTlkDrvCodecCtrl.fadeCtrl.remainTimeMs) / setTimeMs;
    }
    if (sTlkDrvCodecCtrl.fadeCtrl.remainTimeMs == 0) {
        sTlkDrvCodecCtrl.fadeCtrl.remainSamples = 0;
        return true;
    }
    return false;
}

/**
 * @brief Process fade-in effect for 16-bit audio data
 * @param[in,out] pData - Pointer to audio data buffer
 * @param[in] dataLen - Length of audio data in samples
 * @return None
 */
_attribute_ram_code_sec_ static inline void tlkdrv_codec_fade_in_16bit_process(int16_t *pData, uint16_t dataLen)
{
    uint32_t samplesPerMs = sTlkDrvCodecCtrl.fadeCtrl.samplesPerMs;
    for (uint16_t i = 0; i < dataLen; i++) {
        pData[i]   = (int32_t)pData[i] * sTlkDrvCodecCtrl.fadeCtrl.nowstep / 256;
        bool isEnd = tlkdrv_codec_fade_in_remainSamplesDec(samplesPerMs);
        if (isEnd) {
            return;
        }
    }
}

/**
 * @brief Process fade-in effect for 24-bit audio data
 * @param[in,out] pData - Pointer to audio data buffer
 * @param[in] dataLen - Length of audio data in samples
 * @return None
 */
_attribute_ram_code_sec_ static inline void tlkdrv_codec_fade_in_24bit_process(int32_t *pData, uint16_t dataLen)
{
    uint32_t samplesPerMs = sTlkDrvCodecCtrl.fadeCtrl.samplesPerMs;
    for (uint16_t i = 0; i < dataLen; i++) {
        /* avoid  int64_t multiplication*/
        /*24bit data so step only 8bit*/
        pData[i]   = (pData[i] * sTlkDrvCodecCtrl.fadeCtrl.nowstep) >> 8; /** div256  **/
        bool isEnd = tlkdrv_codec_fade_in_remainSamplesDec(samplesPerMs);
        if (isEnd) {
            return;
        }
    }
}

/**
 * @brief Apply fade-in effect to audio data
 * @param[in,out] pData - Pointer to audio data buffer
 * @param[in] dataLen - Length of audio data in bytes
 * @return None
 */
_attribute_ram_code_sec_ static void tlkdrv_codec_fade_in_process(uint8_t *pData, uint16_t dataLen)
{
    if (sTlkDrvCodecCtrl.fadeCtrl.remainSamples == 0) {
        return;
    }
    int bitDepth = tlkdrv_codec_getBitDepth();
    if (bitDepth == TLKDRV_CODEC_BITDEPTH_16) {
        tlkdrv_codec_fade_in_16bit_process((int16_t *)pData, dataLen / 2);
    } else if (bitDepth == TLKDRV_CODEC_BITDEPTH_24) {
        tlkdrv_codec_fade_in_24bit_process((int32_t *)pData, dataLen / 4);
    } else {
        sTlkDrvCodecCtrl.fadeCtrl.remainSamples = 0;
    }
}

/**
 * @brief Power on codec hardware
 * @param None
 * @return None
 */
void tlkdrv_codec_power_on(void)
{
    if (!g_sys_power_on_codec_dis) {
#if (MCU_CORE_TYPE == MCU_CORE_B92)
        audio_codec_init();
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X && TLKDRV_CODEC_ICODEC_ENABLE)
        tlkdrv_tl751x_codec_hd_init();
#endif
    }
}

/**
 * @brief Deinitialize codec hardware
 * @param None
 * @return None
 */
void tlkdrv_codec_hd_deinit(void)
{
#if (MCU_CORE_TYPE == MCU_CORE_TL751X && TLKDRV_CODEC_ICODEC_ENABLE)
    tlkdrv_tl751x_codec_hd_deinit();
#endif
}

/**
 * @brief Enable ANC (Active Noise Cancellation) feature
 * @param[in] resample_enable - Enable/disable resampling
 * @return None
 */
void tlkdrv_codec_anc_enable(bool resample_enable)
{
    (void)resample_enable;
#if (MCU_CORE_TYPE == MCU_CORE_TL751X && TLKDRV_CODEC_ICODEC_ENABLE)
    tlkdrv_icodec_anc_enable(resample_enable);
#endif
    s_anc_mode_enable = 1;
}

/**
 * @brief Disable ANC (Active Noise Cancellation) feature
 * @param None
 * @return None
 */
void tlkdrv_codec_anc_disable(void)
{
#if (MCU_CORE_TYPE == MCU_CORE_TL751X && TLKDRV_CODEC_ICODEC_ENABLE)
    tlkdrv_icodec_anc_disable();
#endif

    s_anc_mode_enable = 0;
}

/**
 * @brief Check if ANC is enabled
 * @param None
 * @return 1 if ANC is enabled, 0 otherwise
 */
uint8_t tlkdrv_codec_anc_is_enabled(void)
{
    return s_anc_mode_enable;
}

/**
 * @brief Initialize codec with specified device and sub-device
 * @param[in] dev - Codec device type
 * @param[in] subDev - Codec sub-device type
 * @return Operation result status
 */
int tlkdrv_codec_init(TLKDRV_CODEC_DEV_ENUM dev, TLKDRV_CODEC_SUBDEV_ENUM subDev)
{
    int                          ret = TLK_ENONE;
    const tlkdrv_codec_modinf_t *pModInf;
    pModInf = tlkdrv_codec_getDev(dev);
    if (pModInf == NULL) {
        return -TLK_ENOSUPPORT;
    }
    if (pModInf->Init != NULL) {
        ret = pModInf->Init(subDev);
    }
    if (ret == TLK_ENONE) {
        sTlkDrvCodecCtrl.majorDev = dev;
        sTlkDrvCodecCtrl.minorDev = subDev;
    }
    return ret;
}

/**
 * @brief Check if codec sub-device is open
 * @param[in] subDev - Codec sub-device type
 * @return True if sub-device is open, false otherwise
 */
bool tlkdrv_codec_isOpen(TLKDRV_CODEC_SUBDEV_ENUM subDev)
{
    const tlkdrv_codec_modinf_t *pModInf;
    pModInf = tlkdrv_codec_getDev(sTlkDrvCodecCtrl.majorDev);
    if (pModInf == NULL || pModInf->IsOpen == NULL) {
        return false;
    }
    return pModInf->IsOpen(subDev);
}

/**
 * @brief Open codec sub-device
 * @param[in] subDev - Codec sub-device type
 * @return Operation result status
 */
int tlkdrv_codec_open(TLKDRV_CODEC_SUBDEV_ENUM subDev)
{
    int                          ret;
    const tlkdrv_codec_modinf_t *pModInf;

    //tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "=== tlkdrv_codec_open 001: %d",
    //sTlkDrvCodecCtrl.majorDev);
    if (subDev == TLKDRV_CODEC_SUBDEV_DEF) {
        subDev = sTlkDrvCodecCtrl.minorDev;
    }

    pModInf = tlkdrv_codec_getDev(sTlkDrvCodecCtrl.majorDev);
    if (pModInf == NULL || pModInf->Open == NULL) {
        return -TLK_ENOSUPPORT;
    }

    ret = pModInf->Open(subDev);
    if (ret == TLK_ENONE) {
        if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) == TLKDRV_CODEC_SUBDEV_SPK) {
            sTlkDrvCodecCtrl.spkOffset = 120 * sizeof(codec_int);
            tlkdrv_codec_muteSpkBuff();
        }

        if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) == TLKDRV_CODEC_SUBDEV_MIC) {
            sTlkDrvCodecCtrl.micOffset = 0;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Close codec sub-device
 * @param[in] subDev - Codec sub-device type
 * @return Operation result status
 */
int tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_ENUM subDev)
{
    const tlkdrv_codec_modinf_t *pModInf;
    pModInf = tlkdrv_codec_getDev(sTlkDrvCodecCtrl.majorDev);
    if (pModInf == NULL || pModInf->Close == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return pModInf->Close(subDev);
}

/**
 * @brief Configure codec sub-device with specified operation
 * @param[in] subDev - Codec sub-device type
 * @param[in] opcode - Operation code
 * @param[in] param0 - First parameter for operation
 * @param[in] param1 - Second parameter for operation
 * @return Operation result status
 */
int tlkdrv_codec_config(TLKDRV_CODEC_SUBDEV_ENUM subDev, uint8_t opcode, uint32_t param0, uint32_t param1)
{
    const tlkdrv_codec_modinf_t *pModInf;
    if (subDev == TLKDRV_CODEC_SUBDEV_DEF) {
        subDev = sTlkDrvCodecCtrl.minorDev;
    }
    pModInf = tlkdrv_codec_getDev(sTlkDrvCodecCtrl.majorDev);
    if (pModInf == NULL || pModInf->Config == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return pModInf->Config(subDev, opcode, param0, param1);
}

/**
 * @brief Set codec channel
 * @param[in] channel - Channel type
 * @return Operation result status
 */
int tlkdrv_codec_setChannel(uint8_t channel)
{
    return tlkdrv_codec_config(TLKDRV_CODEC_SUBDEV_DEF, TLKDRV_CODEC_OPCODE_SET_CHANNEL, channel, 0);
}

/**
 * @brief Set codec bit depth
 * @param[in] bitDepth - Bit depth value
 * @return Operation result status
 */
int tlkdrv_codec_setBitDepth(uint8_t bitDepth)
{
    return tlkdrv_codec_config(TLKDRV_CODEC_SUBDEV_DEF, TLKDRV_CODEC_OPCODE_SET_BIT_DEPTH, bitDepth, 0);
}

/**
 * @brief Get codec bit depth
 * @param None
 * @return Current bit depth value
 */
int tlkdrv_codec_getBitDepth(void)
{
    return tlkdrv_codec_config(TLKDRV_CODEC_SUBDEV_DEF, TLKDRV_CODEC_OPCODE_GET_BIT_DEPTH, 0, 0);
}

/**
 * @brief Set codec sample rate
 * @param[in] sampleRate - Sample rate value
 * @return Operation result status
 */
int tlkdrv_codec_setSampleRate(uint32_t sampleRate)
{
    return tlkdrv_codec_config(TLKDRV_CODEC_SUBDEV_DEF, TLKDRV_CODEC_OPCODE_SET_SAMPLE_RATE, sampleRate, 0);
}

/**
 * @brief Get codec sample rate
 * @param None
 * @return Current sample rate value
 */
uint32_t tlkdrv_codec_getSampleRate(void)
{
    return tlkdrv_codec_config(TLKDRV_CODEC_SUBDEV_DEF, TLKDRV_CODEC_OPCODE_GET_SAMPLE_RATE, 0, 0);
}

/**
 * @brief Get codec channel configuration
 * @param None
 * @return Current channel configuration
 */
uint8_t tlkdrv_codec_getChannel(void)
{
    return tlkdrv_codec_config(TLKDRV_CODEC_SUBDEV_DEF, TLKDRV_CODEC_OPCODE_GET_CHANNEL, 0, 0);
}

/**
 * @brief Set speaker buffer
 * @param[in] pBuffer - Pointer to buffer
 * @param[in] buffLen - Buffer length in bytes
 * @return None
 */
void tlkdrv_codec_setSpkBuffer(uint8_t *pBuffer, uint16_t buffLen)
{
    if (buffLen != 0 && buffLen < 256) {
        return;
    }
    gTlkDrvCodecSpkBuffLen = buffLen;
    gpTlkDrvCodecSpkBuffer = pBuffer;
}

/**
 * @brief Set microphone buffer
 * @param[in] pBuffer - Pointer to buffer
 * @param[in] buffLen - Buffer length in bytes
 * @return None
 */
void tlkdrv_codec_setMicBuffer(uint8_t *pBuffer, uint16_t buffLen)
{
    if (buffLen != 0 && buffLen < 256) {
        return;
    }
    gTlkDrvCodecMicBuffLen = buffLen;
    gpTlkDrvCodecMicBuffer = pBuffer;
}

/**
 * @brief Get speaker buffer offset
 * @param None
 * @return Speaker buffer offset in bytes
 */
uint tlkdrv_codec_getSpkOffset(void)
{
    return sTlkDrvCodecCtrl.spkOffset;
}

/**
 * @brief Get microphone buffer offset
 * @param None
 * @return Microphone buffer offset in bytes
 */
uint tlkdrv_codec_getMicOffset(void)
{
    return sTlkDrvCodecCtrl.micOffset;
}

/**
 * @brief Set speaker buffer offset
 * @param[in] offset - Offset value in bytes
 * @return None
 */
void tlkdrv_codec_setSpkOffset(uint16_t offset)
{
    if (offset >= gTlkDrvCodecSpkBuffLen) {
        return;
    }
    sTlkDrvCodecCtrl.spkOffset = offset / sizeof(codec_int) * sizeof(codec_int);
}

/**
 * @brief Set microphone buffer offset
 * @param[in] offset - Offset value in bytes
 * @return None
 */
void tlkdrv_codec_setMicOffset(uint16_t offset)
{
    if (offset >= gTlkDrvCodecMicBuffLen) {
        return;
    }
    sTlkDrvCodecCtrl.micOffset = offset;
}

/**
 * @brief Get speaker buffer length
 * @param None
 * @return Speaker buffer length in bytes
 */
uint tlkdrv_codec_getSpkBuffLen(void)
{
    return gTlkDrvCodecSpkBuffLen;
}

/**
 * @brief Get microphone buffer length
 * @param None
 * @return Microphone buffer length in bytes
 */
uint tlkdrv_codec_getMicBuffLen(void)
{
    return gTlkDrvCodecMicBuffLen;
}

/**
 * @brief Get speaker idle buffer length
 * @param None
 * @return Idle buffer length in bytes
 */
uint tlkdrv_codec_getSpkIdleLen(void)
{
    uint16_t unUsed;
    uint32_t wptr;
    uint32_t rptr;

    if (gTlkDrvCodecSpkBuffLen == 0 || !tlkdrv_codec_isOpen(TLKDRV_CODEC_SUBDEV_SPK)) {
        // tlkapi_printf(APP_LOG_EN, "tlkdrv_codec_getSpkIdleLen error %d %d", gTlkDrvCodecSpkBuffLen, tlkdrv_codec_isOpen(TLKDRV_CODEC_SUBDEV_SPK));
        return 0;
    }

    wptr = sTlkDrvCodecCtrl.spkOffset;

#if DEC_PCM_DATA_TO_DSP
    if (tlkdrv_codec_anc_is_enabled()) {
        int *psrc = (int *)gpTlkDrvCodecSpkBuffer;
        rptr      = psrc[PCM_BUFF_RPTR_INDEX] * sizeof(int);
    } else
#endif
    {
        rptr = (audio_get_tx_dma_rptr(TLKDRV_CODEC_SPK_DMA)) - ((uint32_t)gpTlkDrvCodecSpkBuffer);
    }

    if (rptr > wptr) {
        unUsed = rptr - wptr;
    } else {
        unUsed = gTlkDrvCodecSpkBuffLen + rptr - wptr;
    }

    return unUsed;
}

/**
 * @brief Get speaker buffer idle size in samples
 * @param None
 * @return Idle buffer size in samples
 */
uint32_t tlkdrv_codec_get_spk_buf_idle_size(void)
{
    uint16_t free_size;

    free_size = tlkdrv_codec_getSpkIdleLen() / sizeof(codec_int);

    return free_size;
}

/**
 * @brief Get speaker data length in buffer
 * @param None
 * @return Data length in bytes
 */
_attribute_ram_code_sec_ uint tlkdrv_codec_getSpkDataLen(void)
{
    uint16_t used;
    uint32_t wptr;
    uint32_t rptr;

    if (gTlkDrvCodecSpkBuffLen == 0 || !tlkdrv_codec_isOpen(TLKDRV_CODEC_SUBDEV_SPK)) {
        return 0;
    }

    wptr = sTlkDrvCodecCtrl.spkOffset;
#if DEC_PCM_DATA_TO_DSP
    if (tlkdrv_codec_anc_is_enabled()) {
        int *psrc = (int *)gpTlkDrvCodecSpkBuffer;
        rptr      = psrc[PCM_BUFF_RPTR_INDEX] * sizeof(int);
    } else
#endif
    {
        rptr = (audio_get_tx_dma_rptr(TLKDRV_CODEC_SPK_DMA)) - ((uint32_t)gpTlkDrvCodecSpkBuffer);
    }

    if (wptr > rptr) {
        used = wptr - rptr;
    } else {
        used = gTlkDrvCodecSpkBuffLen + wptr - rptr;
    }

    return used;
}

/**
 * @brief Get speaker available samples count
 * @param None
 * @return Available samples count
 */
_attribute_ram_code_sec_ uint32_t tlkdrv_codec_get_spk_avail_samples(void)
{
    uint16_t avail_samples;

    avail_samples = tlkdrv_codec_getSpkDataLen() / sizeof(codec_int);

    return avail_samples;
}

/**
 * @brief Get microphone data length in buffer
 * @param None
 * @return Data length in bytes
 */
uint tlkdrv_codec_getMicDataLen(void)
{
    uint16_t used;
    uint32_t wptr;
    uint32_t rptr;

    if (gTlkDrvCodecMicBuffLen == 0 || !tlkdrv_codec_isOpen(TLKDRV_CODEC_SUBDEV_MIC)) {
        return 0;
    }

    rptr = sTlkDrvCodecCtrl.micOffset;
    wptr = (audio_get_rx_dma_wptr(TLKDRV_CODEC_MIC_DMA)) - ((uint32_t)gpTlkDrvCodecMicBuffer);

    if (wptr > rptr) {
        used = wptr - rptr;
    } else {
        used = gTlkDrvCodecMicBuffLen + wptr - rptr;
    }

    return used;
}

/**
 * @brief Get microphone available samples count
 * @param None
 * @return Available samples count
 */
uint32_t tlkdrv_codec_get_mic_avail_samples(void)
{
    uint16_t avail_samples;

    avail_samples = tlkdrv_codec_getMicDataLen() / sizeof(adc_int);

    return avail_samples;
}

/**
 * @brief Read microphone data from buffer
 * @param[out] pBuffer - Pointer to destination buffer
 * @param[in] buffLen - Buffer length in bytes
 * @param[out] pOffset - Pointer to store offset value
 * @return True if successful, false otherwise
 */
bool tlkdrv_codec_readMicData(uint8_t *pBuffer, uint16_t buffLen, uint16_t *pOffset)
{
    //uint32_t wptr;
    uint32_t rptr;
    uint16_t dataLen;
    uint16_t tempLen;

    if (gTlkDrvCodecMicBuffLen == 0 || !tlkdrv_codec_isOpen(TLKDRV_CODEC_SUBDEV_MIC)) {
        return false;
    }

    dataLen = tlkdrv_codec_getMicDataLen();
    if (dataLen < buffLen) {
        return false;
    }

    rptr = sTlkDrvCodecCtrl.micOffset;

    if (rptr + buffLen <= gTlkDrvCodecMicBuffLen) {
        tempLen = buffLen;
    } else {
        tempLen = gTlkDrvCodecMicBuffLen - rptr;
    }

    if (tempLen != 0) {
        tmemcpy(pBuffer, ((uint8_t *)gpTlkDrvCodecMicBuffer) + rptr, tempLen);
    }

    if (tempLen == buffLen) {
        rptr += tempLen;
    } else {
        rptr = buffLen - tempLen;
        tmemcpy(pBuffer + tempLen, ((uint8_t *)gpTlkDrvCodecMicBuffer), rptr);
    }

    rptr &= (gTlkDrvCodecMicBuffLen - 1);

    if (pOffset != NULL) {
        *pOffset = dataLen;
    }

    sTlkDrvCodecCtrl.micOffset = rptr;

    return true;
}

/**
 * @brief Mute speaker buffer
 * @param None
 * @return None
 */
void tlkdrv_codec_muteSpkBuff(void)
{
    if (sTlkDrvCodecCtrl.spkIsMute) {
        return;
    }
    if (gTlkDrvCodecSpkBuffLen != 0) {
        tmemset(gpTlkDrvCodecSpkBuffer, 0, gTlkDrvCodecSpkBuffLen);
    }
    sTlkDrvCodecCtrl.spkIsMute = true;
}

/**
 * @brief Mute microphone buffer
 * @param None
 * @return None
 */
void tlkdrv_codec_muteMicBuff(void)
{
    if (gTlkDrvCodecMicBuffLen != 0) {
        tmemset(gpTlkDrvCodecMicBuffer, 0, gTlkDrvCodecMicBuffLen);
    }
}

/**
 * @brief Enable/disable force mute for speaker
 * @param[in] en - Enable flag (1=enable, 0=disable)
 * @return Operation result status
 */
int tlkdrv_codec_forceMuteEnable(uint8_t en)
{
    sTlkDrvCodecCtrl.spkIsForceMute = en;
    return TLK_ENONE;
}

/**
 * @brief Fill speaker buffer with audio data
 * @param[in] pData - Pointer to audio data
 * @param[in] dataLen - Data length in bytes
 * @return True if successful, false otherwise
 */
bool tlkdrv_codec_fillSpkBuff(uint8_t *pData, uint16_t dataLen)
{
    uint32_t wptr;
    uint32_t rptr;
    uint16_t unUsed;
    uint16_t offset;
    uint8_t *pBuffer = (uint8_t *)gpTlkDrvCodecSpkBuffer;

    if (gTlkDrvCodecSpkBuffLen == 0 || !tlkdrv_codec_isOpen(TLKDRV_CODEC_SUBDEV_SPK)) {
        //        tlkapi_printf(APP_LOG_EN, "gTlkDrvCodecSpkBuffLen: %d, is_open: %d",
        //                      gTlkDrvCodecSpkBuffLen, tlkdrv_codec_isOpen(TLKDRV_CODEC_SUBDEV_SPK));
        return false;
    }
    if (pData == NULL || dataLen == 0 || (dataLen & 0x01) != 0) {
        tlkapi_printf(APP_LOG_EN, "dataLen %d", dataLen);
        return false;
    }

    wptr = sTlkDrvCodecCtrl.spkOffset;

#if DEC_PCM_DATA_TO_DSP
    int *psrc = (int *)gpTlkDrvCodecSpkBuffer;

    if (tlkdrv_codec_anc_is_enabled()) {
        rptr = psrc[PCM_BUFF_RPTR_INDEX] * sizeof(int);
    } else
#endif
    {
        rptr = (audio_get_tx_dma_rptr(TLKDRV_CODEC_SPK_DMA)) - ((uint32_t)gpTlkDrvCodecSpkBuffer);
    }

    if (rptr > wptr) {
        unUsed = rptr - wptr;
    } else {
        unUsed = gTlkDrvCodecSpkBuffLen + rptr - wptr;
    }

    //    tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "fill1: buf_r:%d buf_w:%d buf_free:%d",rptr, wptr, unUsed);

    if (unUsed <= dataLen) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "fill2 fail: %d %d %d %d", rptr, wptr, unUsed, dataLen);
        return false;
    }

    if (sTlkDrvCodecCtrl.spkIsMute) {
        sTlkDrvCodecCtrl.spkIsMute = false;
    }

    //tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "fill data: ok");

    if (wptr + dataLen > gTlkDrvCodecSpkBuffLen) {
        offset = gTlkDrvCodecSpkBuffLen - wptr;
    } else {
        offset = dataLen;
    }
    if (!sTlkDrvCodecCtrl.spkIsForceMute) {
        tlkdrv_codec_fade_in_process(pData, dataLen);
    }


    if (!sTlkDrvCodecCtrl.spkIsForceMute) {
        tmemcpy(pBuffer + wptr, pData, offset);
    } else {
        tmemset(pBuffer + wptr, 0, offset);
    }

    if (offset < dataLen) {
        if (!sTlkDrvCodecCtrl.spkIsForceMute) {
            tmemcpy(pBuffer, pData + offset, dataLen - offset);
        } else {
            tmemset(pBuffer, 0, dataLen - offset);
        }
    }

    wptr += dataLen;
    wptr &= (gTlkDrvCodecSpkBuffLen - 1);

    sTlkDrvCodecCtrl.spkOffset = wptr;

#if DEC_PCM_DATA_TO_DSP
    if (tlkdrv_codec_anc_is_enabled()) {
        psrc[PCM_BUFF_WPTR_INDEX] = sTlkDrvCodecCtrl.spkOffset / sizeof(int);
    }
#endif

    return true;
}

/**
 * @brief Read speaker data from buffer with offset
 * @param[out] pBuffer - Pointer to destination buffer
 * @param[in] buffLen - Buffer length in bytes
 * @param[in] offset - Offset value in bytes
 * @param[in] isBack - Backward flag
 * @return True if successful, false otherwise
 */
bool tlkdrv_codec_backReadSpkData(uint8_t *pBuffer, uint16_t buffLen, uint16_t offset, bool isBack)
{
    uint32_t rptr;
    uint16_t tempLen;

    if (gTlkDrvCodecSpkBuffLen == 0 || !tlkdrv_codec_isOpen(TLKDRV_CODEC_SUBDEV_SPK)) {
        return false;
    }
    if (buffLen == 0 || buffLen > gTlkDrvCodecSpkBuffLen || offset >= gTlkDrvCodecSpkBuffLen) {
        tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_codec_backReadSpkData: fault", 0, 0);
        return false;
    }
    rptr = (uint32_t)((audio_get_tx_dma_rptr(TLKDRV_CODEC_SPK_DMA)) - ((uint32_t)gpTlkDrvCodecSpkBuffer));
    if (offset != 0) {
        if (!isBack) {
            rptr += offset;
            if (rptr >= gTlkDrvCodecSpkBuffLen) {
                rptr -= gTlkDrvCodecSpkBuffLen;
            }
        } else {
            if (rptr >= offset) {
                rptr -= offset;
            } else {
                rptr = gTlkDrvCodecSpkBuffLen + rptr - offset;
            }
        }
    }
    if (rptr >= gTlkDrvCodecSpkBuffLen) {
        rptr = 0;
    }

    if (rptr + buffLen <= gTlkDrvCodecSpkBuffLen) {
        tempLen = buffLen;
    } else {
        tempLen = gTlkDrvCodecSpkBuffLen - rptr;
    }
    if (tempLen != 0) {
        tmemcpy(pBuffer, ((uint8_t *)(gpTlkDrvCodecSpkBuffer)) + rptr, tempLen);
    }
    if (tempLen < buffLen) {
        tmemcpy(pBuffer + tempLen, (uint8_t *)gpTlkDrvCodecSpkBuffer, buffLen - tempLen);
    }

    //	tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "backRead: %d %d %d %d", offset, rptr,
    //sTlkDrvCodecCtrl.spkOffset, tempLen);

    return true;
}

/**
 * @brief Synchronize playback samples
 * @param[in] samples - Number of samples to synchronize
 * @return None
 */
void tlkdrv_codec_sync_play_samples(uint16_t samples)
{
    int16_t  dma_rptr;
    uint16_t samples_in_fifo;
    uint16_t bytes;
    uint16_t type_size = sizeof(codec_int);

#if DEC_PCM_DATA_TO_DSP
    codec_int *psrc = (codec_int *)gpTlkDrvCodecSpkBuffer;
    if (tlkdrv_codec_anc_is_enabled()) {
        dma_rptr = psrc[PCM_BUFF_RPTR_INDEX] * type_size;
    } else
#endif
    {
        dma_rptr = (audio_get_tx_dma_rptr(TLKDRV_CODEC_SPK_DMA)) - ((uint32_t)gpTlkDrvCodecSpkBuffer);
    }

    if (samples <= 32) {
        tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "<play-mute sample too low> %d %d", sTlkDrvCodecCtrl.spkOffset, samples);
    } else {
        samples_in_fifo = samples <= CODEC_SPK_FIFO_SAMPLES / 2 ? samples : CODEC_SPK_FIFO_SAMPLES / 2;
        bytes           = samples_in_fifo * sizeof(codec_int);

        sTlkDrvCodecCtrl.spkOffset = ((dma_rptr + bytes) & (gTlkDrvCodecSpkBuffLen - 1)) / type_size * type_size;

        tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "<play-mute samples number> %d %d %d", sTlkDrvCodecCtrl.spkOffset, samples, samples_in_fifo);
    }

#if DEC_PCM_DATA_TO_DSP
    if (tlkdrv_codec_anc_is_enabled()) {
        psrc[PCM_BUFF_WPTR_INDEX] = sTlkDrvCodecCtrl.spkOffset / type_size;
    }
#endif
}

/**
 * @brief Get speaker read pointer
 * @param None
 * @return Speaker read pointer value
 */
_attribute_ram_code_sec_ uint32_t tlkdrv_codec_get_speaker_rptr(void)
{
#if DEC_PCM_DATA_TO_DSP
    if (tlkdrv_codec_anc_is_enabled()) {
        int *psrc = (int *)gpTlkDrvCodecSpkBuffer;
        return psrc[PCM_BUFF_RPTR_INDEX];
    } else
#endif
    {
        return ((audio_get_tx_dma_rptr(TLKDRV_CODEC_SPK_DMA) - (uint32_t)gpTlkDrvCodecSpkBuffer) / sizeof(codec_int));
    }
}

/**
 * @brief Get mic write pointer
 * @param[in]  none
 * @returns write pointer
 */
uint32_t tlkdrv_codec_get_mic_wptr(void)
{
#if !MCU_CORE_TL752X_TEMP
    return ((audio_get_rx_dma_wptr(TLKDRV_CODEC_MIC_DMA) - (uint32_t)gpTlkDrvCodecMicBuffer) / sizeof(adc_int));
#else
    return 0;
#endif
}

/**
 * @brief Synchronize microphone samples
 * @param[in] samples - Number of samples to synchronize
 * @return Microphone offset value
 */
uint32_t tlkdrv_codec_sync_mic_samples(uint16_t samples)
{
    uint16_t wptr;
    wptr = (audio_get_rx_dma_wptr(TLKDRV_CODEC_MIC_DMA)) - ((uint32_t)gpTlkDrvCodecMicBuffer);

    wptr = wptr / sizeof(adc_int) * sizeof(adc_int); //align ptr by sample

    sTlkDrvCodecCtrl.micOffset = ((wptr - samples * sizeof(adc_int)) & (gTlkDrvCodecMicBuffLen - 1)) / sizeof(adc_int) * sizeof(adc_int);

    return (sTlkDrvCodecCtrl.micOffset / sizeof(adc_int));
}

/**
 * @brief Synchronize speaker samples
 * @param[in] samples - Number of samples to synchronize
 * @return None
 */
_attribute_ram_code_sec_ void tlkdrv_codec_sync_speaker_samples(uint16_t samples)
{
#if !MCU_CORE_TL752X_TEMP
    int16_t  dma_rptr;
    uint16_t bytes;
    uint16_t type_size = sizeof(codec_int);

#if DEC_PCM_DATA_TO_DSP
    if (tlkdrv_codec_anc_is_enabled()) {
        int *psrc = (int *)gpTlkDrvCodecSpkBuffer;
        dma_rptr  = psrc[PCM_BUFF_RPTR_INDEX] * sizeof(int);
    } else
#endif
    {
        dma_rptr = (audio_get_tx_dma_rptr(TLKDRV_CODEC_SPK_DMA)) - ((uint32_t)gpTlkDrvCodecSpkBuffer);
    }

    bytes = samples * sizeof(codec_int);

    sTlkDrvCodecCtrl.spkOffset = (dma_rptr + bytes) & (gTlkDrvCodecSpkBuffLen - 1);

    sTlkDrvCodecCtrl.spkOffset = (sTlkDrvCodecCtrl.spkOffset + (type_size - 1)) & (~(type_size - 1));

#else
    (void)samples;
#endif //#if !MCU_CORE_TL752X_TEMP
}

/**
 * @brief Initialize codec environment
 * @param None
 * @return Operation result status
 */
int tlkdrv_codec_init_env(void)
{
#if (!PROJ_RECORDING_CARD)
#if TLK_DEV_CODEC_SPK_ENABLE
    tlkdrv_codec_setSpkBuffer((uint8_t *)g_codec_spk_buff, CODEC_SPK_FIFO_SAMPLES * sizeof(codec_int));
    tmemset((uint8_t *)g_codec_spk_buff, 0, CODEC_SPK_FIFO_SAMPLES * sizeof(codec_int));
#endif
#if TLK_DEV_CODEC_MIC_ENABLE
    tlkdrv_codec_setMicBuffer((uint8_t *)g_codec_mic_buff, CODEC_MIC_FIFO_SAMPLES * sizeof(adc_int));
    tmemset((uint8_t *)g_codec_mic_buff, 0, CODEC_MIC_FIFO_SAMPLES * sizeof(adc_int));
#endif
#else
    tlkdrv_codec_setMicBuffer((uint8_t *)g_codec_mic_buff, CODEC_MIC_FIFO_SAMPLES * sizeof(adc_mono_int));
#endif


#if (TLKDRV_CODEC_ICODEC_ENABLE)
    tlkdrv_codec_init(TLKDRV_CODEC_DEV_INNER, TLKDRV_CODEC_SUBDEV_BOTH);
#elif (TLKDRV_CODEC_I2S_SLAVE_ENABLE)
    tlkdrv_codec_init(TLKDRV_CODEC_DEV_IISSLV, TLKDRV_CODEC_SUBDEV_BOTH);
#elif (TLKDRV_CODEC_I2S_MASTER_ENABLE)
    tlkdrv_codec_init(TLKDRV_CODEC_DEV_IISMST, TLKDRV_CODEC_SUBDEV_BOTH);
#endif

    return TLK_ENONE;
}

/**
 * @brief Open codec with specified configuration
 * @param[in] subDev - Codec sub-device type
 * @param[in] channel - Channel configuration
 * @param[in] bitDepth - Bit depth value
 * @param[in] sampleRate - Sample rate value
 * @param[in] fadeInTimeMs - Fade in duration in milliseconds
 * @return Operation result status
 */
int tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_ENUM subDev, uint8_t channel, uint8_t bitDepth, uint32_t sampleRate, uint16_t fadeInTimeMs)
{
    int ret = TLK_ENONE;

    uint32_t begin_time = clock_time();

    tlkdrv_codec_init_env();

    if (ret == TLK_ENONE) {
        ret = tlkdrv_codec_setChannel(channel);
    }

    if (ret == TLK_ENONE) {
        ret = tlkdrv_codec_setBitDepth(bitDepth);
    }

    if (ret == TLK_ENONE) {
        ret = tlkdrv_codec_setSampleRate(sampleRate);
    }

    if (ret == TLK_ENONE) {
        ret = tlkdrv_codec_open(subDev);
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        tlkdrv_codec_fade_in_init(fadeInTimeMs);
    }

    uint32_t used_time = clock_time() - begin_time;

    tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_open_codec ret[%d] time[%d] subdev[%d]", ret, used_time, subDev);

    return ret;
}

/**
 * @brief Get codec device interface
 * @param[in] dev - Codec device type
 * @return Pointer to codec device interface
 */
static const tlkdrv_codec_modinf_t *tlkdrv_codec_getDev(uint8_t dev)
{
    if (dev >= TLKDRV_CODEC_DEV_MAX) {
        return NULL;
    }

    return spTlkDrvCodecModinf[dev];
}
#endif
