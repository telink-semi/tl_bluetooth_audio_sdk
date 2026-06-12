/********************************************************************************************************
 * @file    tlkdrv_i2s_slave_b92.c
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
#if (MCU_CORE_TYPE == MCU_CORE_B92)
#include "tl_common.h"
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkdrv_codec.h"
#if (TLKDRV_CODEC_I2S_SLAVE_ENABLE)
#include "tlkmw/sys_dev/tlkmw_sysdev.h"

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
} tlkdrv_iisslv_t;

static bool tlkdrv_iisslv_isOpen(uint8_t subDev);
static int  tlkdrv_iisslv_init(uint8_t subDev);
static int  tlkdrv_iisslv_open(uint8_t subDev);
static int  tlkdrv_iisslv_close(uint8_t subDev);
static int  tlkdrv_iisslv_config(uint8_t subDev, uint8_t opcode, uint32_t param0, uint32_t param1);

static int  tlkdrv_iisslv_enable(uint8_t bitDepth, uint8_t channel, uint32_t sampleRate, bool enMic, bool enSpk);
static void tlkdrv_iisslv_disable(void);

static int tlkdrv_iisslv_setChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_iisslv_getChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_iisslv_setBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_iisslv_getBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_iisslv_setSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_iisslv_getSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1);


extern uint16_t gTlkDrvCodecSpkBuffLen;
extern uint16_t gTlkDrvCodecMicBuffLen;
extern uint8_t *gpTlkDrvCodecSpkBuffer;
extern uint8_t *gpTlkDrvCodecMicBuffer;

extern const uint16_t scTlkDrvCodec8kParam[5];
extern const uint16_t scTlkDrvCodec16kParam[5];
extern const uint16_t scTlkDrvCodec32kParam[5];
extern const uint16_t scTlkDrvCodec48kParam[5];
extern const uint16_t scTlkDrvCodec44k1Param[5];


const tlkdrv_codec_modinf_t gcTlkDrvIisSlvInf_b92 = {
    .IsOpen = tlkdrv_iisslv_isOpen,
    .Init   = tlkdrv_iisslv_init,
    .Open   = tlkdrv_iisslv_open,
    .Close  = tlkdrv_iisslv_close,
    .Config = tlkdrv_iisslv_config,
};
static tlkdrv_iisslv_t sTlkDrvIisSlvCtrl = {0};

i2s_pin_config_t pinConfig = {
    .bclk_pin       = TLKDRV_I2S_SLV_BCLIK_PIN,
    .adc_lr_clk_pin = TLKDRV_I2S_SLV_ADC_CLK_PIN,
    .adc_dat_pin    = TLKDRV_I2S_SLV_ADC_DAT_PIN,
    .dac_lr_clk_pin = TLKDRV_I2S_SLV_DAC_CLK_PIN,
    .dac_dat_pin    = TLKDRV_I2S_SLV_DAC_DAT_PIN,
};
audio_i2s_config_t iisConfig = {
    .i2s_select        = TLKDRV_I2S_SLV_CHN,
    .i2s_mode          = I2S_I2S_MODE,
    .pin_config        = &pinConfig,
    .data_width        = I2S_BIT_16_DATA,
    .master_slave_mode = I2S_AS_SLAVE_EN,
    .sample_rate       = (uint16_t *)scTlkDrvCodec32kParam,
};

/**
 * @brief Reset I2S pin configuration
 * @param[in] config - I2S pin configuration structure
 * @return None
 */
void i2s_reset_pin(i2s_pin_config_t *config)
{
    gpio_input_dis((gpio_pin_e)config->bclk_pin);
    gpio_output_dis((gpio_pin_e)config->bclk_pin);
    gpio_function_en((gpio_pin_e)config->bclk_pin);

    if (config->adc_lr_clk_pin != GPIO_NONE_PIN) {
        gpio_input_dis((gpio_pin_e)config->adc_lr_clk_pin);
        gpio_output_dis((gpio_pin_e)config->adc_lr_clk_pin);
        gpio_function_en((gpio_pin_e)config->adc_lr_clk_pin);
    }

    if (config->dac_lr_clk_pin != GPIO_NONE_PIN) {
        gpio_input_dis((gpio_pin_e)config->dac_lr_clk_pin);
        gpio_output_dis((gpio_pin_e)config->dac_lr_clk_pin);
        gpio_function_en((gpio_pin_e)config->dac_lr_clk_pin);
    }

    if (config->adc_dat_pin != GPIO_NONE_PIN) {
        gpio_input_dis((gpio_pin_e)config->adc_dat_pin);
        gpio_output_dis((gpio_pin_e)config->adc_dat_pin);
        gpio_function_en((gpio_pin_e)config->adc_dat_pin);
    }

    if (config->dac_dat_pin != GPIO_NONE_PIN) {
        gpio_input_dis((gpio_pin_e)config->dac_dat_pin);
        gpio_output_dis((gpio_pin_e)config->dac_dat_pin);
        gpio_function_en((gpio_pin_e)config->dac_dat_pin);
    }
}

/**
 * @brief Check if I2S slave device is open
 * @param[in] subDev - Sub-device type
 * @return True if device is open, false otherwise
 */
static bool tlkdrv_iisslv_isOpen(uint8_t subDev)
{
    if (!sTlkDrvIisSlvCtrl.isInit || !sTlkDrvIisSlvCtrl.isOpen) {
        return false;
    }
    if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        if (sTlkDrvIisSlvCtrl.micIsEn) {
            return true;
        } else {
            return false;
        }
    } else if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        if (sTlkDrvIisSlvCtrl.spkIsEn) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

/**
 * @brief Initialize I2S slave device
 * @param[in] subDev - Sub-device type
 * @return Operation result status
 */
static int tlkdrv_iisslv_init(uint8_t subDev)
{
    (void)subDev;
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkdrv_iisslv_t)));
    memset(&sTlkDrvIisSlvCtrl, 0, sizeof(tlkdrv_iisslv_t));

    sTlkDrvIisSlvCtrl.isInit     = true;
    sTlkDrvIisSlvCtrl.isOpen     = false;
    sTlkDrvIisSlvCtrl.isMute     = false;
    sTlkDrvIisSlvCtrl.micVol     = 60;
    sTlkDrvIisSlvCtrl.bitDepth   = 16;
    sTlkDrvIisSlvCtrl.sampleRate = 48000;

    return TLK_ENONE;
}

/**
 * @brief Open I2S slave device
 * @param[in] subDev - Sub-device type
 * @return Operation result status
 */
static int tlkdrv_iisslv_open(uint8_t subDev)
{
    if (!sTlkDrvIisSlvCtrl.isInit) {
        return -TLK_ESTATUS;
    }
    if (sTlkDrvIisSlvCtrl.isOpen) {
        return -TLK_EREPEAT;
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        sTlkDrvIisSlvCtrl.micIsEn = true;
    } else {
        sTlkDrvIisSlvCtrl.micIsEn = false;
    }
    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        sTlkDrvIisSlvCtrl.spkIsEn = true;
    } else {
        sTlkDrvIisSlvCtrl.spkIsEn = false;
    }
    tlkdrv_iisslv_enable(sTlkDrvIisSlvCtrl.bitDepth, sTlkDrvIisSlvCtrl.channel, sTlkDrvIisSlvCtrl.sampleRate, sTlkDrvIisSlvCtrl.micIsEn, sTlkDrvIisSlvCtrl.spkIsEn);

    sTlkDrvIisSlvCtrl.isOpen = true;
    return TLK_ENONE;
}

/**
 * @brief Close I2S slave device
 * @param[in] subDev - Sub-device type
 * @return Operation result status
 */
static int tlkdrv_iisslv_close(uint8_t subDev)
{
    (void)subDev;
    if (!sTlkDrvIisSlvCtrl.isInit || !sTlkDrvIisSlvCtrl.isOpen) {
        return -TLK_ESTATUS;
    }

    tlkdrv_iisslv_disable();

    sTlkDrvIisSlvCtrl.isOpen  = false;
    sTlkDrvIisSlvCtrl.micIsEn = false;
    sTlkDrvIisSlvCtrl.spkIsEn = false;

    return TLK_ENONE;
}

/**
 * @brief Configure I2S slave device
 * @param[in] subDev - Sub-device type
 * @param[in] opcode - Operation code
 * @param[in] param0 - First parameter for operation
 * @param[in] param1 - Second parameter for operation
 * @return Operation result status
 */
static int tlkdrv_iisslv_config(uint8_t subDev, uint8_t opcode, uint32_t param0, uint32_t param1)
{
    int ret = -TLK_ENOSUPPORT;
    switch (opcode) {
    case TLKDRV_CODEC_OPCODE_SET_CHANNEL: // param:[uint8_t]chnCnt-1/2/3
        ret = tlkdrv_iisslv_setChannelDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_CHANNEL:
        ret = tlkdrv_iisslv_getChannelDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_SET_BIT_DEPTH: // param:[uint8_t]bitDepth-8,16,20,24,32
        ret = tlkdrv_iisslv_setBitDepthDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_BIT_DEPTH:
        ret = tlkdrv_iisslv_getBitDepthDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_SET_SAMPLE_RATE: // param:[uint32_t]bitDepth-8,16,20,24,32
        ret = tlkdrv_iisslv_setSampleRateDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_SAMPLE_RATE:
        ret = tlkdrv_iisslv_getSampleRateDeal(subDev, param0, param1);
        break;
    }
    return ret;
}

/**
 * @brief Set I2S slave channel
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Channel parameter
 * @param[in] param1 - Reserved parameter
 * @return Operation result status
 */
static int tlkdrv_iisslv_setChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param1;

    uint8_t channel = param0 & 0xFF;
    if (channel != 0x01 && channel != 0x02 && channel != 0x03) {
        return -TLK_EPARAM;
    }
    sTlkDrvIisSlvCtrl.channel = channel;
    return TLK_ENONE;
}

/**
 * @brief Get I2S slave channel
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Reserved parameter
 * @param[in] param1 - Reserved parameter
 * @return Current channel configuration
 */
static int tlkdrv_iisslv_getChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;

    return sTlkDrvIisSlvCtrl.channel;
}

/**
 * @brief Set I2S slave bit depth
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Bit depth parameter
 * @param[in] param1 - Reserved parameter
 * @return Operation result status
 */
static int tlkdrv_iisslv_setBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param1;

    uint8_t bitDapth = param0 & 0xFF;
    if (bitDapth != 16 && bitDapth != 20 && bitDapth != 24) {
        return -TLK_ENOSUPPORT;
    }
    sTlkDrvIisSlvCtrl.bitDepth = bitDapth;
    return TLK_ENONE;
}

/**
 * @brief Get I2S slave bit depth
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Reserved parameter
 * @param[in] param1 - Reserved parameter
 * @return Current bit depth value
 */
static int tlkdrv_iisslv_getBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;

    return sTlkDrvIisSlvCtrl.bitDepth;
}

/**
 * @brief Set I2S slave sample rate
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Sample rate parameter
 * @param[in] param1 - Reserved parameter
 * @return Operation result status
 */
static int tlkdrv_iisslv_setSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param1;

    uint32_t sampleRate = param0;
    if (sampleRate != 8000 && sampleRate != 16000 && sampleRate != 32000 && sampleRate != 44100 && sampleRate != 48000) {
        return -TLK_EPARAM;
    }
    sTlkDrvIisSlvCtrl.sampleRate = sampleRate;
    return TLK_ENONE;
}

/**
 * @brief Get I2S slave sample rate
 * @param[in] subDev - Sub-device type
 * @param[in] param0 - Reserved parameter
 * @param[in] param1 - Reserved parameter
 * @return Current sample rate value
 */
static int tlkdrv_iisslv_getSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;

    return sTlkDrvIisSlvCtrl.sampleRate;
}

/**
 * @brief Enable I2S slave device
 * @param[in] bitDepth - Bit depth value
 * @param[in] channel - Channel configuration
 * @param[in] sampleRate - Sample rate value
 * @param[in] enMic - Enable microphone flag
 * @param[in] enSpk - Enable speaker flag
 * @return Operation result status
 */
static int tlkdrv_iisslv_enable(uint8_t bitDepth, uint8_t channel, uint32_t sampleRate, bool enMic, bool enSpk)
{
    uint8_t         dataWdith    = 0xFF;
    uint8_t         drvChannel   = 0xFF;
    const uint16_t *pSampleParam = NULL;

    if (!enMic && !enSpk) {
        tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iisslv_enable: mic or spk is all disable");
        return -TLK_EPARAM;
    }

    if (sampleRate == 8000) {
        pSampleParam = scTlkDrvCodec8kParam;
    } else if (sampleRate == 16000) {
        pSampleParam = scTlkDrvCodec16kParam;
    } else if (sampleRate == 32000) {
        pSampleParam = scTlkDrvCodec32kParam;
    } else if (sampleRate == 44100) {
        pSampleParam = scTlkDrvCodec44k1Param;
    } else if (sampleRate == 48000) {
        pSampleParam = scTlkDrvCodec48kParam;
    }
    if (bitDepth == 16) {
        dataWdith = I2S_BIT_16_DATA;
    } else if (bitDepth == 20) {
        dataWdith = I2S_BIT_20_DATA;
    } else if (bitDepth == 24) {
        dataWdith = I2S_BIT_24_DATA;
    }
    if (channel == 0x03) {
        drvChannel = I2S_STEREO;
    } else {
        drvChannel = I2S_MONO;
    }
    if (pSampleParam == NULL || dataWdith == 0xFF) {
        tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iisslv_enable: sampleRate or bitDepth is not supported");
        return -TLK_EPARAM;
    }

    iisConfig.data_width  = dataWdith;
    iisConfig.sample_rate = (uint16_t *)pSampleParam;

    audio_power_on();
    //    audio_i2s_set_mclk(GPIO_FC_PA1, pSampleParam[0], pSampleParam[1]); //only set i2s as master mclk
    audio_i2s_config_init(&iisConfig);


    if (enMic && gpTlkDrvCodecMicBuffer != NULL && gTlkDrvCodecMicBuffLen != 0) {
        audio_i2s_input_output_t inputParam = {
            .i2s_select    = iisConfig.i2s_select,
            .data_width    = dataWdith,
            .i2s_ch_sel    = drvChannel,
            .fifo_chn      = TLKDRV_CODEC_MIC_FIFO,
            .dma_num       = gTlkdrvCodecMicDmaChn,
            .data_buf      = gpTlkDrvCodecMicBuffer,
            .data_buf_size = gTlkDrvCodecMicBuffLen,
        };
        if (gpTlkDrvCodecSpkBuffer == NULL || gTlkDrvCodecSpkBuffLen == 0) {
            tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iisslv_enable: spk not ready");
            return -TLK_EPARAM;
        }
        audio_i2s_input_init(&inputParam);
        audio_rx_dma_chain_init(inputParam.fifo_chn, inputParam.dma_num, (unsigned short *)inputParam.data_buf, inputParam.data_buf_size);
        audio_rx_dma_en(inputParam.dma_num);
    }
    if (enSpk && gpTlkDrvCodecSpkBuffer != NULL && gTlkDrvCodecSpkBuffLen != 0) {
        audio_i2s_input_output_t outputParam = {
            .i2s_select    = iisConfig.i2s_select,
            .data_width    = dataWdith,
            .i2s_ch_sel    = drvChannel,
            .fifo_chn      = TLKDRV_CODEC_SPK_FIFO,
            .dma_num       = gTlkdrvCodecSpkDmaChn,
            .data_buf      = gpTlkDrvCodecSpkBuffer,
            .data_buf_size = gTlkDrvCodecSpkBuffLen,
        };
        if (gpTlkDrvCodecMicBuffer == NULL || gTlkDrvCodecMicBuffLen == 0) {
            tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iisslv_enable: mic not ready");
            return -TLK_EPARAM;
        }
        audio_i2s_output_init(&outputParam);
        audio_tx_dma_chain_init(outputParam.fifo_chn, outputParam.dma_num, (unsigned short *)outputParam.data_buf, outputParam.data_buf_size);
        audio_tx_dma_en(outputParam.dma_num);
    }

    tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iisslv_enable: bitDepth[%d], channel[%d], sampleRate[%d]", sTlkDrvIisSlvCtrl.bitDepth,
                 sTlkDrvIisSlvCtrl.channel, sTlkDrvIisSlvCtrl.sampleRate);

    return TLK_ENONE;
}

/**
 * @brief Disable I2S slave device
 * @return None
 */
static void tlkdrv_iisslv_disable(void)
{
    tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iisslv_disable: 001");

    audio_power_down();
    audio_rx_dma_dis(gTlkdrvCodecMicDmaChn);
    audio_tx_dma_dis(gTlkdrvCodecSpkDmaChn);
    i2s_reset_pin(&pinConfig);
}


#endif // #if (TLKDRV_CODEC_I2S_SLAVE_ENABLE)

#endif // #if (MCU_CORE_TYPE == MCU_CORE_B92)
