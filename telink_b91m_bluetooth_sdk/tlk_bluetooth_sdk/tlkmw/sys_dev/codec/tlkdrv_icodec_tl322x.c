/********************************************************************************************************
 * @file    tlkdrv_icodec_tl322x.c
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
//#include "core/mcu_type.h"
#include "tl_common.h"
#if (MCU_CORE_TYPE == MCU_CORE_TL322X && TLKDRV_CODEC_ICODEC_ENABLE)
#include <stdlib.h>
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"

typedef struct
{
    uint8_t  IsEn;
    uint8_t  IsOpen;
    uint8_t  Channel;
    uint8_t  BitDepth;
    uint32_t SampleRate;

} tlkdrv_codec_cfg_t;

typedef struct
{
    uint8_t            isInit;
    uint8_t            sys_mclk;
    uint8_t            reserved[2];
    tlkdrv_codec_cfg_t codec_spk_cfg;
    tlkdrv_codec_cfg_t codec_mic_cfg;
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
static int tlkdrv_icodec_mic_enable(bool enMic);
static int tlkdrv_icodec_spk_enable(bool enSpk);

//static void tlkdrv_icodec_disable(void);


extern uint16_t gTlkDrvCodecSpkBuffLen;
extern uint16_t gTlkDrvCodecMicBuffLen;
extern uint8_t *gpTlkDrvCodecSpkBuffer;
extern uint8_t *gpTlkDrvCodecMicBuffer;

const tlkdrv_codec_modinf_t gcTlkDrvIcodecInf = {
    .IsOpen = tlkdrv_icodec_isOpen,
    .Init   = tlkdrv_icodec_init,
    .Open   = tlkdrv_icodec_open,
    .Close  = tlkdrv_icodec_close,
    .Config = tlkdrv_icodec_config,
};
static tlkdrv_icodec_t sTlkDrvIcodecCtrl = {0};

uint32_t tl322x_codec_hd_freq_table[7]     = {8000, 16000, 24000, 32000, 44100, 48000, 0xffffffff};
uint8_t  tl322x_codec_hd_bitdepth_table[4] = {16, 20, 0xff};
uint8_t  tl322x_codec_hd_channal_table[4]  = {1, 2, 3, 0xff};

/*
void codec_test_data(void)
{
    sTlkDrvIcodecCtrl.isInit = 1;
    sTlkDrvIcodecCtrl.isOpen = 1;
    sTlkDrvIcodecCtrl.micBitDepth = 16;
    sTlkDrvIcodecCtrl.micChannel = 3;
    sTlkDrvIcodecCtrl.micSampleRate = 48000;
    sTlkDrvIcodecCtrl.spkBitDepth = 16;
    sTlkDrvIcodecCtrl.spkChannel = 3;
    sTlkDrvIcodecCtrl.spkSampleRate = 48000;
}
*/

/**
 * @brief Initialize TL322X codec hardware
 * @return None
 */
void tlkdrv_tl322x_codec_hd_init(void) {}

/**
 * @brief Convert audio sample rate to index
 * @param[in] audio_rate - Audio sample rate
 * @return Audio rate index
 */
static inline int audio_sample_rate_to_index(int audio_rate)
{
    int rate_index;
#if 0
    rate_index = audio_rate == 16000 ? AUDIO_16K :
                     audio_rate == 48000 ? AUDIO_48K :
                    		 	 	 	   0xffffffff;
#else
    if (audio_rate == 8000) {
        rate_index = AUDIO_8K;
    } else if (audio_rate == 16000) {
        rate_index = AUDIO_16K;
    } else if (audio_rate == 24000) {
        rate_index = AUDIO_24K;
    } else if (audio_rate == 32000) {
        rate_index = AUDIO_32K;
    } else if (audio_rate == 44100) {
        rate_index = AUDIO_44P1K;
    } else if (audio_rate == 48000) {
        rate_index = AUDIO_48K;
    } else {
        rate_index = 0x00;
    }

#endif
    return rate_index;
}

/**
 * @brief Check if bit depth is supported
 * @param[in] value - Bit depth value
 * @return Operation result status
 */
static int icodec_bitdepth_check(uint8_t value)
{
    uint8_t count = sizeof(tl322x_codec_hd_bitdepth_table) / sizeof(uint8_t);
    for (uint8_t i = 0; i < count; i++) {
        if (value == tl322x_codec_hd_bitdepth_table[i]) {
            break;
        } else if (tl322x_codec_hd_bitdepth_table[i] == 0xff) {
            return -TLK_ENOSUPPORT;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Check if channel configuration is supported
 * @param[in] value - Channel value
 * @return Operation result status
 */
static int icodec_channal_check(uint8_t value)
{
    uint8_t count = sizeof(tl322x_codec_hd_channal_table) / sizeof(uint8_t);
    for (uint8_t i = 0; i < count; i++) {
        if (value == tl322x_codec_hd_channal_table[i]) {
            break;
        } else if (tl322x_codec_hd_channal_table[i] == 0xff) {
            return -TLK_EPARAM;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Check if frequency is supported
 * @param[in] value - Frequency value
 * @return Operation result status
 */
static int icodec_freq_check(uint32_t value)
{
    uint8_t count = sizeof(tl322x_codec_hd_freq_table) / sizeof(int);

    for (uint8_t i = 0; i < count; i++) {
        if (value == tl322x_codec_hd_freq_table[i]) {
            break;
        } else if (tl322x_codec_hd_freq_table[i] == 0xffffffff) {
            return -TLK_EPARAM;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Mute audio DAC
 * @return None
 */
void audio_dac_mute(void) {}

/**
 * @brief Unmute audio DAC
 * @return None
 */
void audio_dac_unmute(void) {}

/**
 * @brief Check if internal codec device is open
 * @param[in] subDev - Sub-device type
 * @return True if device is open, false otherwise
 */
static bool tlkdrv_icodec_isOpen(uint8_t subDev)
{
    if (!sTlkDrvIcodecCtrl.isInit) {
        return false;
    }
    if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.IsEn) {
            return true;
        } else {
            return false;
        }
    } else if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        if (sTlkDrvIcodecCtrl.codec_spk_cfg.IsEn) {
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
    if (!sTlkDrvIcodecCtrl.isInit) {
        STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkdrv_icodec_t)));
        memset(&sTlkDrvIcodecCtrl, 0, sizeof(tlkdrv_icodec_t));
    }

    audio_init();

    sTlkDrvIcodecCtrl.isInit = true;

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

    if ((subDev & TLKDRV_CODEC_SUBDEV_BOTH)) {
        if (((sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate == 44100) || (sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate == 44100)) &&
            (sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate != sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate)) {
            tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_open(Err):Codec NO Support!!!", NULL, 0);
            return -TLK_ENOSUPPORT;
        }
    }

    ///MIC enable
    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen) {
            return -TLK_EREPEAT;
        }

        sTlkDrvIcodecCtrl.codec_mic_cfg.IsEn = true;
        tlkdrv_icodec_mic_enable(true);
        sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen = true;
    }

    ///SPK enable
    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        if (sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen) {
            return -TLK_EREPEAT;
        }

        sTlkDrvIcodecCtrl.codec_spk_cfg.IsEn = true;
        tlkdrv_icodec_spk_enable(true);
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen = true;
    }

    return TLK_ENONE;
}

/**
 * @brief Close internal codec device
 * @param[in] subDev - Sub-device type
 * @return Operation result status
 */
static int tlkdrv_icodec_close(uint8_t subDev)
{
    if (!sTlkDrvIcodecCtrl.isInit) {
        return -TLK_ESTATUS;
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen = false;
        sTlkDrvIcodecCtrl.codec_mic_cfg.IsEn   = false;
        audio_rx_dma_dis(gTlkdrvCodecMicDmaChn);
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen = false;
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsEn   = false;
        audio_tx_dma_dis(gTlkdrvCodecSpkDmaChn);
    }

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
    (void)param1;
    uint8_t channel = param0 & 0xFF;

    int ret = icodec_channal_check(channel);
    if (ret) {
        return ret;
    }

    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.Channel = channel;
    } else if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        sTlkDrvIcodecCtrl.codec_mic_cfg.Channel = channel;
    } else {
        sTlkDrvIcodecCtrl.codec_spk_cfg.Channel = channel;
        sTlkDrvIcodecCtrl.codec_mic_cfg.Channel = channel;
    }
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
    (void)param0;
    (void)param1;
    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        return sTlkDrvIcodecCtrl.codec_spk_cfg.Channel;
    } else {
        return sTlkDrvIcodecCtrl.codec_mic_cfg.Channel;
    }
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
    (void)param1;
    uint8_t bitDapth = param0 & 0xFF;


    int ret = icodec_bitdepth_check(bitDapth);
    if (ret) {
        return ret;
    }

    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth = bitDapth;
    } else if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth = bitDapth;
    } else {
        sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth = bitDapth;
        sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth = bitDapth;
    }
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
    (void)param0;
    (void)param1;
    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        return sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth;
    } else {
        return sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth;
    }
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
    (void)param1;
    uint32_t sampleRate = param0;

    int ret = icodec_freq_check(sampleRate);
    if (ret) {
        return ret;
    }

    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate = sampleRate;
    } else if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate = sampleRate;
    } else {
        sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate = sampleRate;
        sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate = sampleRate;
    }
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
    (void)param0;
    (void)param1;
    if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        return sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate;
    } else {
        return sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate;
    }
}

signed short sin_16K_d2[] __attribute__((aligned(4))) = {
    0x0, 0x0, 0x30fb,  0x30fb,  0x5a81,  0x5a81,  0x7640,  0x7640,  0x7ffe,  0x7ffe,  0x7640,  0x7640,  0x5a81,  0x5a81,  0x30fb,  0x30fb,
    0x0, 0x0, -0x30fb, -0x30fb, -0x5a81, -0x5a81, -0x7640, -0x7640, -0x7ffe, -0x7ffe, -0x7640, -0x7640, -0x5a81, -0x5a81, -0x30fb, -0x30fb,

};

/**
 * @brief Enable microphone for internal codec
 * @param[in] enMic - Enable microphone flag
 * @return Operation result status
 */
static int tlkdrv_icodec_mic_enable(bool enMic)
{
    uint8_t micSrc    = 0xFF;
    uint8_t micDWdith = 0xFF;
    int     micSRate  = 0xffffffff;

    tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_mic_enable:", &enMic, 1);

    if (enMic) {
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth == 16) {
            micDWdith = CODEC_BIT_16_DATA;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth == 20) {
            micDWdith = CODEC_BIT_20_DATA;
        } else {
            micDWdith = CODEC_BIT_16_DATA;
        }
        //        tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_MIC: <micDWdith> ",&micDWdith,1);

#if (CODEC_INPUT_MODE == CODEC_INPUT_LINEIN)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = LINE_STREAM0_MONO_L;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = LINE_STREAM0_MONO_L;
        } else {
            micSrc = LINE_STREAM0_MONO_L;
        }
#elif (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = AMIC_STREAM0_MONO_L;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = AMIC_STREAM0_MONO_L;
        } else {
            micSrc = AMIC_STREAM0_MONO_L;
        }
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = DMIC_STREAM0_STEREO;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = DMIC_STREAM0_MONO_R;
        } else {
            micSrc = DMIC_STREAM0_MONO_L;
        }
        audio_set_stream0_dmic_pin((gpio_func_pin_e)TLKDRV_ICODEC_DMIC_DATA_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK0_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK1_PIN);
        // audio_codec0_set_dmic_a_pin((gpio_func_pin_e)TLKDRV_ICODEC_DMIC_DATA_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK0_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK1_PIN);
#endif

        micSRate = audio_sample_rate_to_index(sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate);
        //      tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_MIC: <micSRate> ",&micSRate,4);

        if (gpTlkDrvCodecMicBuffer == NULL || gTlkDrvCodecMicBuffLen == 0 || micSrc == 0xFFU || micSRate == (int)0xffffffff || micDWdith == 0xFFU) {
            tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "Param Err:gpTlkDrvCodecMicBuffer[%x],gTlkDrvCodecMicBuffLen[%x],micSrc[%x],micSRate[%x],micDWdith[%x]",
                         gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen, micSrc, micSRate, micDWdith);
            tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_Mic: enable_4 ", 0, 0);
            return -TLK_EPARAM;
        }

        //		tlkapi_info(TLKDRV_CODEC_DBG_FLAG,
        //					 TLKDRV_CODEC_DBG_SIGN,
        //					 "tlkdrv_icodec_enable: <MIC> %d %d %d",
        //					 micSrc,
        //					 micSRate,
        //					 micDWdith);
    }

    if (enMic && gpTlkDrvCodecMicBuffer != NULL && gTlkDrvCodecMicBuffLen != 0) {
        audio_codec_stream0_input_t inputParam;
        inputParam.input_src   = micSrc;
        inputParam.sample_rate = micSRate;
        inputParam.data_width  = micDWdith;
        inputParam.fifo_chn    = TLKDRV_CODEC_MIC_FIFO;
        inputParam.dma_num     = gTlkdrvCodecMicDmaChn;


        audio_codec_stream0_input_init(&inputParam);

#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        audio_set_adc_pga_gain(CODEC_IN_GAIN_9P0_DB);
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        audio_set_stream0_dig_gain(CODEC_IN_D_GAIN_0_DB);
#endif //#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)


        /* rx dma init. */
        audio_rx_dma_chain_init(TLKDRV_CODEC_MIC_FIFO, gTlkdrvCodecMicDmaChn, (unsigned short *)gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen);

        //	        audio_mic_mute_en();
        audio_codec_stream0_input_en(gTlkdrvCodecMicDmaChn);
        //	        audio_codec_clr_input_pop(20);
        audio_codec_input_path_en(inputParam.fifo_chn);

        //	        audio_rx_dma_en(gTlkdrvCodecMicDmaChn); /* the rx dma enable must precede the adc enable. */
    }

    return TLK_ENONE;
}

sdm_pin_config_t sdm_pin_config = {
    .sdm0_p_pin = TLKDRV_ICODEC_SDMP0,
    .sdm0_n_pin = TLKDRV_ICODEC_SDMN0,
    .sdm1_p_pin = TLKDRV_ICODEC_SDMP1,
    .sdm1_n_pin = TLKDRV_ICODEC_SDMN1,
};

/**
 * @brief Enable speaker for internal codec
 * @param[in] enSpk - Enable speaker flag
 * @return Operation result status
 */
static int tlkdrv_icodec_spk_enable(bool enSpk)
{
    uint8_t spkSrc    = 0xFF;
    uint8_t spkDWdith = 0xFF;
    int     spkSRate  = 0xffffffff;


    tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_spk_enable:", &enSpk, 1);
    if (enSpk) {
        if (sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth == 16) {
            spkDWdith = CODEC_BIT_16_DATA;
        } else if (sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth == 20) {
            spkDWdith = CODEC_BIT_20_DATA;
        }
        //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: <spkDWdith> ",&spkDWdith,1);

        if (sTlkDrvIcodecCtrl.codec_spk_cfg.Channel == 0x03) {
            spkSrc = SDM_STEREO;
        } else {
            spkSrc = SDM_MONO;
        }
        //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: <spkSrc> ",&spkSrc,1);

        spkSRate = audio_sample_rate_to_index(sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate);
        //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: <spkSRate> ",&spkSRate,4);

        //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: enable_2 ",0,0);
        if (gpTlkDrvCodecSpkBuffer == NULL || gTlkDrvCodecSpkBuffLen == 0 || spkSrc == 0xFFU || spkSRate == (int)0xffffffff || spkDWdith == 0xFFU) {
            tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "Param Err:gpTlkDrvCodecSpkBuffer[%x],gTlkDrvCodecSpkBuffLen[%x],spkSrc[%x],spkSRate[%x],spkDWdith[%x]",
                         gpTlkDrvCodecSpkBuffer, gTlkDrvCodecSpkBuffLen, spkSrc, spkSRate, spkDWdith);
            //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: enable_4 ",0,0);
            return -TLK_EPARAM;
        }
        //		tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: enable_3 ",0,0);
        //		tlkapi_info(TLKDRV_CODEC_DBG_FLAG,
        //							 TLKDRV_CODEC_DBG_SIGN,
        //							 "tlkdrv_icodec_enable: <SPK>: %d %d %d",
        //							 spkSrc,
        //							 spkSRate,
        //							 spkDWdith);
    }

    //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: enable1 ",0,0);
    if (enSpk && gpTlkDrvCodecSpkBuffer != NULL && gTlkDrvCodecSpkBuffLen != 0) {
        audio_codec_output_t outputParam;
        outputParam.output_src  = spkSrc;
        outputParam.sample_rate = spkSRate;
        outputParam.data_width  = spkDWdith;
        outputParam.dma_num     = gTlkdrvCodecSpkDmaChn;
#if (AUDIO_CODEC_LOOPBACK)
        outputParam.data_buf      = gpTlkDrvCodecMicBuffer;
        outputParam.data_buf_size = gTlkDrvCodecMicBuffLen;
#else
#if 0 ///debug test
		outputParam.data_buf = sin_16K_d2;
		outputParam.data_buf_size = sizeof(sin_16K_d2);
#else
        outputParam.data_buf      = gpTlkDrvCodecSpkBuffer;
        outputParam.data_buf_size = gTlkDrvCodecMicBuffLen;
#endif
#endif

        /****line output init****/
        audio_set_sdm_pin(&sdm_pin_config);
        audio_codec_stream_output_init(&outputParam);

        /* tx dma init. */
        audio_tx_dma_chain_init(TLKDRV_CODEC_SPK_FIFO, outputParam.dma_num, (unsigned short *)outputParam.data_buf, outputParam.data_buf_size);

        audio_codec_stream_output_en(outputParam.dma_num);
    }

    return TLK_ENONE;
}

#endif
