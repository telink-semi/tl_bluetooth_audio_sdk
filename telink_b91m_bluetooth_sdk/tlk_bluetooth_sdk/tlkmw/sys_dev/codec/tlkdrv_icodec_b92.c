/********************************************************************************************************
 * @file    tlkdrv_icodec_b92.c
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
#if (MCU_CORE_TYPE == MCU_CORE_B92 && TLKDRV_CODEC_ICODEC_ENABLE)
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkdrv_codec.h"
/* ziyu codec temp code*/
#include "tlkmw/audio/common/codec/tlkmw_codec_cfg.h"

/* ziyu codec temp code*/

typedef struct
{
    uint8_t  IsOpen;
    uint8_t  resv;
    uint8_t  Channel;
    uint8_t  BitDepth;
    uint32_t SampleRate;
} tlkdrv_codec_cfg_t;

typedef struct
{
    tlkdrv_codec_cfg_t codec_spk_cfg;
    tlkdrv_codec_cfg_t codec_mic_cfg;
} tlkdrv_icodec_t;

static bool tlkdrv_icodec_isOpen(uint8_t subDev);
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

extern uint16_t gTlkDrvCodecSpkBuffLen;
extern uint16_t gTlkDrvCodecMicBuffLen;
extern uint8_t *gpTlkDrvCodecSpkBuffer;
extern uint8_t *gpTlkDrvCodecMicBuffer;

static tlkdrv_icodec_t sTlkDrvIcodecCtrl = {0};


uint32_t b92_audio_freq_table[7]     = {8000, 16000, 24000, 32000, 44100, 48000, 0xffffffff};
uint8_t  b92_audio_bitdepth_table[3] = {16, 20, 0xff};
uint8_t  b92_audio_channal_table[4]  = {1, 2, 3, 0xff};

/**
 * @brief Convert audio sample rate to index
 * @param[in] audio_rate - Audio sample rate
 * @return Audio rate index, or 0xff if not supported
 */
static inline int audio_sample_rate_to_index(int audio_rate)
{
    int rate_index = audio_rate == 8000  ? AUDIO_8K :
                     audio_rate == 16000 ? AUDIO_16K :
                     audio_rate == 24000 ? AUDIO_24K :
                     audio_rate == 32000 ? AUDIO_32K :
                     audio_rate == 48000 ? AUDIO_48K :
                     audio_rate == 44100 ? AUDIO_44P1K :
                                           0xff;
    return rate_index;
}

/**
 * @brief Convert audio index to sample rate
 * @param[in] index - Audio rate index
 * @return Audio sample rate, or 0 if not supported
 */
inline int audio_index_to_sample_rate(int index)
{
    int audio_rate = index == AUDIO_8K    ? 8000 :
                     index == AUDIO_16K   ? 16000 :
                     index == AUDIO_24K   ? 24000 :
                     index == AUDIO_32K   ? 32000 :
                     index == AUDIO_48K   ? 48000 :
                     index == AUDIO_44P1K ? 44100 :
                                            0;
    return audio_rate;
}

/**
 * @brief Check if bit depth is supported
 * @param[in] value - Bit depth value
 * @return Operation result status
 */
static int icodec_bitdepth_check(uint8_t value)
{
    uint8_t count = sizeof(b92_audio_bitdepth_table) / sizeof(uint8_t);
    for (uint8_t i = 0; i < count; i++) {
        if (value == b92_audio_bitdepth_table[i]) {
            break;
        } else if (b92_audio_bitdepth_table[i] == 0xff) {
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
    uint8_t count = sizeof(b92_audio_channal_table) / sizeof(uint8_t);
    for (uint8_t i = 0; i < count; i++) {
        if (value == b92_audio_channal_table[i]) {
            break;
        } else if (b92_audio_channal_table[i] == 0xff) {
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
    uint8_t count = sizeof(b92_audio_freq_table) / sizeof(int);

    for (uint8_t i = 0; i < count; i++) {
        if (value == b92_audio_freq_table[i]) {
            break;
        } else if (b92_audio_freq_table[i] == 0xffffffff) {
            return -TLK_EPARAM;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Mute audio DAC
 * @return None
 */
void audio_dac_mute(void)
{
    reg_codec_sidetone |= (FLD_AUDIO_CODEC_INT_MUTE_L | FLD_AUDIO_CODEC_INT_MUTE_R);
}

/**
 * @brief Unmute audio DAC
 * @return None
 */
void audio_dac_unmute(void)
{
    reg_codec_sidetone &= ~(FLD_AUDIO_CODEC_INT_MUTE_L | FLD_AUDIO_CODEC_INT_MUTE_R);
}

/**
 * @brief Check if internal codec device is open
 * @param[in] subDev - Sub-device type
 * @return True if device is open, false otherwise
 */
static bool tlkdrv_icodec_isOpen(uint8_t subDev)
{
    if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        return sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen == 1;
    } else if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        return sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen == 1;
    }
    return true;
}

/**
 * @brief Open internal codec device
 * @param[in] subDev - Sub-device type
 * @return Operation result status
 */
static int tlkdrv_icodec_open(uint8_t subDev)
{
/* ziyu codec temp code*/
#if TLKMW_CODEC_CFG_DYNAMIC_POWER_ON
    audio_codec_init();
#endif
    /* ziyu codec temp code*/
    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen) {
            return -TLK_EREPEAT;
        }
        tlkdrv_icodec_mic_enable(true);
        sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen = true;
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        if (sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen) {
            return -TLK_EREPEAT;
        }
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
    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen = false;
        audio_rx_dma_dis(TLKDRV_CODEC_MIC_DMA);
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen = false;
        audio_dac_mute();
        audio_tx_dma_dis(TLKDRV_CODEC_SPK_DMA);
    }

#if (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
    gpio_function_en(TLKDRV_ICODEC_DMIC_DATA_PIN);
    gpio_set_low_level(TLKDRV_ICODEC_DMIC_DATA_PIN);
    gpio_output_dis(TLKDRV_ICODEC_DMIC_DATA_PIN);
    gpio_input_dis(TLKDRV_ICODEC_DMIC_DATA_PIN);
    gpio_set_up_down_res(TLKDRV_ICODEC_DMIC_DATA_PIN, GPIO_PIN_UP_DOWN_FLOAT);

    gpio_function_en(TLKDRV_ICODEC_DMIC_CLK0_PIN);
    gpio_set_low_level(TLKDRV_ICODEC_DMIC_CLK0_PIN);
    gpio_output_dis(TLKDRV_ICODEC_DMIC_CLK0_PIN);
    gpio_input_dis(TLKDRV_ICODEC_DMIC_CLK0_PIN);
    gpio_set_up_down_res(TLKDRV_ICODEC_DMIC_CLK0_PIN, GPIO_PIN_UP_DOWN_FLOAT);

    gpio_function_en(TLKDRV_ICODEC_DMIC_CLK1_PIN);
    gpio_set_low_level(TLKDRV_ICODEC_DMIC_CLK1_PIN);
    gpio_output_dis(TLKDRV_ICODEC_DMIC_CLK1_PIN);
    gpio_input_dis(TLKDRV_ICODEC_DMIC_CLK1_PIN);
    gpio_set_up_down_res(TLKDRV_ICODEC_DMIC_CLK1_PIN, GPIO_PIN_UP_DOWN_FLOAT);
#endif


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

/**
 * @brief Enable microphone for internal codec
 * @param[in] enMic - Enable microphone flag
 * @return Operation result status
 */
static int tlkdrv_icodec_mic_enable(bool enMic)
{
    uint8_t micSrc    = 0;
    uint8_t micDWdith = 0;
    if (!enMic) {
        return TLK_ENONE;
    }

    if (sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth == 16) {
        micDWdith = CODEC_BIT_16_DATA;
    } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth == 20) {
        micDWdith = CODEC_BIT_20_DATA;
    }

#if (CODEC_INPUT_MODE == CODEC_INPUT_LINEIN)
    micSrc = 0;
#elif (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
    micSrc = BIT(2);
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
    micSrc = BIT(3);
    audio_set_stream0_dmic_pin((gpio_func_pin_e)TLKDRV_ICODEC_DMIC_DATA_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK0_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK1_PIN);

#endif

    if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
        micSrc |= LINE_STREAM0_STEREO;
    } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
        micSrc |= LINE_STREAM0_MONO_R;
    } else {
        micSrc |= LINE_STREAM0_MONO_L;
    }

    uint8_t micSRate = audio_sample_rate_to_index(sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate);

    if (gpTlkDrvCodecMicBuffer == NULL || gTlkDrvCodecMicBuffLen == 0) {
        return -TLK_EPARAM;
    }

    audio_codec_stream0_input_t inputParam;
    inputParam.input_src = micSrc; // The output must be consistent with the number and direction of the input channels, otherwise an
                                   // exception will occur: INPUT_MONO ==> OUTPUT_MON0 or INPUT_STEREO ==> OUTPUT_STEREO
    inputParam.sample_rate   = micSRate;
    inputParam.fifo_num      = TLKDRV_CODEC_MIC_FIFO;
    inputParam.data_width    = micDWdith;
    inputParam.dma_num       = TLKDRV_CODEC_MIC_DMA;
    inputParam.data_buf      = gpTlkDrvCodecMicBuffer;
    inputParam.data_buf_size = gTlkDrvCodecMicBuffLen;
    audio_codec_stream0_input_init(&inputParam);

#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
    audio_set_adc_pga_l_gain(CODEC_IN_GAIN_15P0_DB);
    audio_set_adc_pga_r_gain(CODEC_IN_GAIN_15P0_DB);
    audio_set_stream0_dig_gain0(CODEC_IN_D_GAIN0_6_DB);
    audio_set_stream0_dig_gain1(CODEC_IN_D_GAIN1_6_DB);
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
    audio_set_stream0_dig_gain0(CODEC_IN_D_GAIN0_18_DB);
    audio_set_stream0_dig_gain1(CODEC_IN_D_GAIN1_6_DB);
#endif

    audio_rx_dma_chain_init(inputParam.fifo_num, inputParam.dma_num, (unsigned short *)inputParam.data_buf, inputParam.data_buf_size);
    audio_rx_dma_en(TLKDRV_CODEC_MIC_DMA);

    return TLK_ENONE;
}

/**
 * @brief Enable speaker for internal codec
 * @param[in] enSpk - Enable speaker flag
 * @return Operation result status
 */
static int tlkdrv_icodec_spk_enable(bool enSpk)
{
    uint8_t spkSrc    = 0;
    uint8_t spkDWdith = 0;

    if (!enSpk) {
        return TLK_ENONE;
    }

    if (sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth == 16) {
        spkDWdith = CODEC_BIT_16_DATA;
    } else if (sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth == 20) {
        spkDWdith = CODEC_BIT_20_DATA;
    }

    if (sTlkDrvIcodecCtrl.codec_spk_cfg.Channel == 0x03) {
        spkSrc = CODEC_DAC_STEREO;
    } else if (sTlkDrvIcodecCtrl.codec_spk_cfg.Channel == 0x02) {
        spkSrc = CODEC_DAC_MONO_R;
    } else {
        spkSrc = CODEC_DAC_MONO_L;
    }

    uint8_t spkSRate = audio_sample_rate_to_index(sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate);

    if (gpTlkDrvCodecSpkBuffer == NULL || gTlkDrvCodecSpkBuffLen == 0) {
        return -TLK_EPARAM;
    }

    audio_codec_output_t outputParam;
    outputParam.output_src = spkSrc;
    // The output must be consistent with the number and direction of the input channels, otherwise an
    // exception will occur: INPUT_MONO ==> OUTPUT_MON0 or INPUT_STEREO ==> OUTPUT_STEREO
    outputParam.sample_rate = spkSRate;
    outputParam.fifo_num    = TLKDRV_CODEC_SPK_FIFO;
    outputParam.data_width  = spkDWdith;
    outputParam.dma_num     = TLKDRV_CODEC_SPK_DMA;
    outputParam.mode        = HP_MODE;
#if (AUDIO_CODEC_LOOPBACK)
    outputParam.data_buf      = gpTlkDrvCodecMicBuffer;
    outputParam.data_buf_size = gTlkDrvCodecMicBuffLen;
#else
    outputParam.data_buf      = gpTlkDrvCodecSpkBuffer;
    outputParam.data_buf_size = gTlkDrvCodecSpkBuffLen;
#endif
    audio_codec_stream_output_init(&outputParam);
    audio_tx_dma_chain_init(outputParam.fifo_num, outputParam.dma_num, (unsigned short *)outputParam.data_buf, outputParam.data_buf_size);
    audio_tx_dma_en(TLKDRV_CODEC_SPK_DMA);
    audio_dac_unmute();

    return TLK_ENONE;
}

const tlkdrv_codec_modinf_t gcTlkDrvIcodecInf = {
    .IsOpen = tlkdrv_icodec_isOpen,
    .Open   = tlkdrv_icodec_open,
    .Close  = tlkdrv_icodec_close,
    .Config = tlkdrv_icodec_config,
};

#endif
