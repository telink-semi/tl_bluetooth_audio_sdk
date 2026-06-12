/********************************************************************************************************
 * @file    tlkdrv_icodec_b91.c
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
#include "core/mcu_type.h"
#if (MCU_CORE_TYPE == MCU_CORE_B91)
#include "tl_common.h"
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkdrv_codec.h"
#if (TLKDRV_CODEC_ICODEC_ENABLE)

typedef struct
{
    uint8_t  isInit;
    uint8_t  isOpen;
    uint8_t  isMute;
    uint8_t  micVol;
    uint8_t  micIsEn;
    uint8_t  spkIsEn;
    uint8_t  channel;
    uint8_t  bitDepth;
    uint32_t sampleRate;
} tlkdrv_icodec_t;

static bool tlkdrv_icodec_isOpen(uint8_t subDev);
static int  tlkdrv_icodec_init(uint8_t subDev);
static int  tlkdrv_icodec_open(uint8_t subDev);
static int  tlkdrv_icodec_close(uint8_t subDev);
static int  tlkdrv_icodec_config(uint8_t subDev, uint8_t opcode, uint32_t param0, uint32_t param1);

static int tlkdrv_icodec_setChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_getChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_setBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_getBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_setSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_icodec_getSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1);

static int  tlkdrv_icodec_enable(uint8_t bitDepth, uint8_t channel, uint32_t sampleRate, bool enMic, bool enSpk);
static void tlkdrv_icodec_disable(void);


extern uint16_t             gTlkDrvCodecSpkBuffLen;
extern uint16_t             gTlkDrvCodecMicBuffLen;
extern uint8_t             *gpTlkDrvCodecSpkBuffer;
extern uint8_t             *gpTlkDrvCodecMicBuffer;
const tlkdrv_codec_modinf_t gcTlkDrvIcodecInf = {
    .IsOpen = tlkdrv_icodec_isOpen,
    .Init   = tlkdrv_icodec_init,
    .Open   = tlkdrv_icodec_open,
    .Close  = tlkdrv_icodec_close,
    .Config = tlkdrv_icodec_config,
};
static tlkdrv_icodec_t sTlkDrvIcodecCtrl = {0};

/**
 * @brief Check if internal codec device is open
 * @param[in] subDev - Sub-device type
 * @return True if device is open, false otherwise
 */
static bool tlkdrv_icodec_isOpen(uint8_t subDev)
{
    if (!sTlkDrvIcodecCtrl.isInit || !sTlkDrvIcodecCtrl.isOpen) {
        return false;
    }
    if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        if (sTlkDrvIcodecCtrl.micIsEn) {
            return true;
        } else {
            return false;
        }
    } else if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        if (sTlkDrvIcodecCtrl.spkIsEn) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

/**
 * @brief Initialize internal codec device
 * @param[in] subDev - Sub-device type
 * @return Operation result status
 */
static int tlkdrv_icodec_init(uint8_t subDev)
{
    (void)subDev;

    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkdrv_icodec_t)));
    memset(&sTlkDrvIcodecCtrl, 0, sizeof(tlkdrv_icodec_t));

    sTlkDrvIcodecCtrl.isInit     = true;
    sTlkDrvIcodecCtrl.isOpen     = false;
    sTlkDrvIcodecCtrl.isMute     = false;
    sTlkDrvIcodecCtrl.micVol     = 100;
    sTlkDrvIcodecCtrl.channel    = 0x01;
    sTlkDrvIcodecCtrl.bitDepth   = 16;
    sTlkDrvIcodecCtrl.sampleRate = 48000;

    return TLK_ENONE;
}

/**
 * @brief Open internal codec device
 * @param[in] subDev - Sub-device type
 * @return Operation result status
 */
static int tlkdrv_icodec_open(uint8_t subDev)
{
    if (!sTlkDrvIcodecCtrl.isInit) {
        return -TLK_ESTATUS;
    }
    if (sTlkDrvIcodecCtrl.isOpen) {
        return -TLK_EREPEAT;
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        sTlkDrvIcodecCtrl.micIsEn = true;
    } else {
        sTlkDrvIcodecCtrl.micIsEn = false;
    }
    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        sTlkDrvIcodecCtrl.spkIsEn = true;
    } else {
        sTlkDrvIcodecCtrl.spkIsEn = false;
    }

    tlkdrv_icodec_enable(sTlkDrvIcodecCtrl.bitDepth, sTlkDrvIcodecCtrl.channel, sTlkDrvIcodecCtrl.sampleRate, sTlkDrvIcodecCtrl.micIsEn, sTlkDrvIcodecCtrl.spkIsEn);

    sTlkDrvIcodecCtrl.isOpen = true;
    return TLK_ENONE;
}

/**
 * @brief Close internal codec device
 * @param[in] subDev - Sub-device type
 * @return Operation result status
 */
static int tlkdrv_icodec_close(uint8_t subDev)
{
    (void)subDev;

    if (!sTlkDrvIcodecCtrl.isInit || !sTlkDrvIcodecCtrl.isOpen) {
        return -TLK_ESTATUS;
    }

    tlkdrv_icodec_disable();

    sTlkDrvIcodecCtrl.isOpen  = false;
    sTlkDrvIcodecCtrl.micIsEn = false;
    sTlkDrvIcodecCtrl.spkIsEn = false;

    return TLK_ENONE;
}

/**
 * @brief Configure internal codec device
 * @param[in] subDev - Sub-device type
 * @param[in] opcode - Operation code
 * @param[in] param0 - First parameter for operation
 * @param[in] param1 - Second parameter for operation
 * @return Operation result status
 */
static int tlkdrv_icodec_config(uint8_t subDev, uint8_t opcode, uint32_t param0, uint32_t param1)
{
    int ret = -TLK_ENOSUPPORT;
    switch (opcode) {
    case TLKDRV_CODEC_OPCODE_SET_CHANNEL: // param:[uint8_t]chnCnt-1/2/3
        ret = tlkdrv_icodec_setChannelDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_CHANNEL:
        ret = tlkdrv_icodec_getChannelDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_SET_BIT_DEPTH: // param:[uint8_t]bitDepth-8,16,20,24,32
        ret = tlkdrv_icodec_setBitDepthDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_BIT_DEPTH:
        ret = tlkdrv_icodec_getBitDepthDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_SET_SAMPLE_RATE: // param:[uint32_t]bitDepth-8,16,20,24,32
        ret = tlkdrv_icodec_setSampleRateDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_SAMPLE_RATE:
        ret = tlkdrv_icodec_getSampleRateDeal(subDev, param0, param1);
        break;
    }
    return ret;
}

/**
 * @brief Set internal codec channel
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Channel parameter
 * @param[in] param1 - Reserved parameter
 * @return Operation result status
 */
static int tlkdrv_icodec_setChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param1;
    uint8_t channel = param0 & 0xFF;
    if (channel != 0x01 && channel != 0x02 && channel != 0x03) {
        return -TLK_EPARAM;
    }
    sTlkDrvIcodecCtrl.channel = channel;
    return TLK_ENONE;
}

/**
 * @brief Get internal codec channel
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Reserved parameter
 * @param[in] param1 - Reserved parameter
 * @return Current channel configuration
 */
static int tlkdrv_icodec_getChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;

    return sTlkDrvIcodecCtrl.channel;
}

/**
 * @brief Set internal codec bit depth
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Bit depth parameter
 * @param[in] param1 - Reserved parameter
 * @return Operation result status
 */
static int tlkdrv_icodec_setBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param1;

    uint8_t bitDapth = param0 & 0xFF;
    if (bitDapth != 8 && bitDapth != 16 && bitDapth != 20 && bitDapth != 24 && bitDapth != 32) {
        return -TLK_EPARAM;
    }
    if (bitDapth != 16) {
        return -TLK_ENOSUPPORT;
    }
    sTlkDrvIcodecCtrl.bitDepth = 16;
    return TLK_ENONE;
}

/**
 * @brief Get internal codec bit depth
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Reserved parameter
 * @param[in] param1 - Reserved parameter
 * @return Current bit depth value
 */
static int tlkdrv_icodec_getBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;

    return sTlkDrvIcodecCtrl.bitDepth;
}

/**
 * @brief Set internal codec sample rate
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Sample rate parameter
 * @param[in] param1 - Reserved parameter
 * @return Operation result status
 */
static int tlkdrv_icodec_setSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param1;

    uint32_t sampleRate = param0;
    if (sampleRate != 8000 && sampleRate != 16000 && sampleRate != 32000 && sampleRate != 44100 && sampleRate != 48000) {
        return -TLK_EPARAM;
    }
    sTlkDrvIcodecCtrl.sampleRate = sampleRate;
    return TLK_ENONE;
}

/**
 * @brief Get internal codec sample rate
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Reserved parameter
 * @param[in] param1 - Reserved parameter
 * @return Current sample rate value
 */
static int tlkdrv_icodec_getSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;

    return sTlkDrvIcodecCtrl.sampleRate;
}

/**
 * @brief Enable internal codec device
 * @param[in] bitDepth - Bit depth value
 * @param[in] channel - Channel configuration
 * @param[in] sampleRate - Sample rate value
 * @param[in] enMic - Enable microphone flag
 * @param[in] enSpk - Enable speaker flag
 * @return Operation result status
 */
static int tlkdrv_icodec_enable(uint8_t bitDepth, uint8_t channel, uint32_t sampleRate, bool enMic, bool enSpk)
{
    (void)enMic;
    (void)enSpk;

    audio_sample_rate_e rate;
    //	audio_in_mode_e ain0_mode;
    //	audio_in_mode_e ain1_mode;
    //	audio_out_mode_e i2s_aout_mode;
    //	i2s_data_select_e bitData;

    if (bitDepth != 16 && bitDepth != 24 && bitDepth != 20 && channel != 0x01 && channel != 0x02 && channel != 0x03) {
        return -TLK_ENOSUPPORT;
    }
    if (sampleRate == 8000) {
        rate = AUDIO_8K;
    } else if (sampleRate == 16000) {
        rate = AUDIO_16K;
    } else if (sampleRate == 24000) {
        rate = AUDIO_24K;
    } else if (sampleRate == 32000) {
        rate = AUDIO_32K;
    } else if (sampleRate == 44100) {
        rate = AUDIO_44EP1K;
    } else if (sampleRate == 48000) {
        rate = AUDIO_48K;
    } else {
        return -TLK_ENOSUPPORT;
    }
    if (bitDepth == 24) {
        if (channel == 0x03) {
            bitDepth = STEREO_BIT_24;
            //			ain0_mode = BIT_20_OR_24_STEREO;
            //			ain1_mode = BIT_20_OR_24_STEREO;
            //			i2s_aout_mode = BIT_20_OR_24_STEREO_FIFO0;
        } else if (channel == 0x01 || channel == 0x02) {
            bitDepth = MONO_BIT_24;
            //			ain0_mode = BIT_20_OR_24_MONO;
            //			ain1_mode = BIT_20_OR_24_MONO;
            //			i2s_aout_mode = BIT_20_OR_24_MONO_FIFO0;
        } else {
            return -TLK_ENOSUPPORT;
        }
        //		bitData = I2S_BIT_24_DATA;
    } else if (bitDepth == 16) {
        if (channel == 0x03) {
            bitDepth = STEREO_BIT_16;
            //			ain0_mode = BIT_16_STEREO;
            //			ain1_mode = BIT_16_STEREO;
            //			i2s_aout_mode = BIT_16_STEREO_FIFO0;
        } else if (channel == 0x01 || channel == 0x02) {
            bitDepth = MONO_BIT_16;
            //			ain0_mode = BIT_16_MONO;
            //			ain1_mode = BIT_16_MONO;
            //			i2s_aout_mode = BIT_16_MONO_FIFO0;
        } else {
            return -TLK_ENOSUPPORT;
        }
        //		bitData = I2S_BIT_16_DATA;
    } else {
        return -TLK_ENOSUPPORT;
    }

    //	tlkapi_sendU32s(TLKDRV_CODEC_DEBUG_ENABLE, "tlkdrv_icodec_enable: 002", bitDepth, channel, sampleRate, 0);

    audio_reset();
    codec_reset();
    dma_chn_dis(gTlkdrvCodecMicDmaChn);
    dma_chn_dis(gTlkdrvCodecSpkDmaChn);

    //	gpio_function_en(I2S_BCK_PC3);
    //	gpio_function_en(I2S_ADC_LR_PC4);
    //	gpio_function_en(I2S_ADC_DAT_PC5);
    //	gpio_function_en(I2S_DAC_LR_PC6);
    //	gpio_function_en(I2S_DAC_DAT_PC7);
    audio_init(AMIC_IN_TO_BUF_TO_LINE_OUT, rate, bitDepth);
    audio_rx_dma_chain_init(gTlkdrvCodecMicDmaChn, (uint16_t *)gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen);
    audio_tx_dma_chain_init(gTlkdrvCodecSpkDmaChn, (uint16_t *)gpTlkDrvCodecSpkBuffer, gTlkDrvCodecSpkBuffLen);

    //	audio_set_codec_supply(CODEC_2P8V);

    return TLK_ENONE;
}

/**
 * @brief Disable internal codec device
 * @return None
 */
static void tlkdrv_icodec_disable(void)
{
    dma_chn_dis(gTlkdrvCodecMicDmaChn);
    dma_chn_dis(gTlkdrvCodecSpkDmaChn);

    audio_reset();
    codec_reset();
    audio_codec_adc_power_down();
    audio_codec_dac_power_down();
}


#endif // #if (TLKDRV_CODEC_ICODEC_ENABLE)
#endif // #if (MCU_CORE_TYPE == MCU_CORE_B91)
