/********************************************************************************************************
 * @file    tlkdrv_i2s_master_tl721x.c
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
#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkdrv_codec.h"
#if (TLKDRV_CODEC_I2S_MASTER_ENABLE)
#include "tlkdrv_i2s_master_tl721x.h"
#include "tlkdrv_i2s_ex_codec_config.h"
#include "drivers.h"

static bool tlkdrv_iismst_isOpen(uint8_t subDev);
static int  tlkdrv_iismst_init(uint8_t subDev);
static int  tlkdrv_iismst_open(uint8_t subDev);
static int  tlkdrv_iismst_close(uint8_t subDev);
static int  tlkdrv_iismst_config(uint8_t subDev, uint8_t opcode, uint32_t param0, uint32_t param1);

static int  tlkdrv_iismst_enable(uint8_t bitDepth, uint8_t channel, uint32_t sampleRate, bool enMic, bool enSpk);
static void tlkdrv_iismst_disable(void);

static int tlkdrv_iismst_setChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_iismst_getChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_iismst_setBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_iismst_getBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_iismst_setSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int tlkdrv_iismst_getSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1);


extern uint16_t gTlkDrvCodecSpkBuffLen;
extern uint16_t gTlkDrvCodecMicBuffLen;
extern uint8_t *gpTlkDrvCodecSpkBuffer;
extern uint8_t *gpTlkDrvCodecMicBuffer;

extern const uint16_t scTlkDrvCodec8kParam[5];
extern const uint16_t scTlkDrvCodec16kParam[5];
extern const uint16_t scTlkDrvCodec24kParam[5];
extern const uint16_t scTlkDrvCodec32kParam[5];
extern const uint16_t scTlkDrvCodec48kParam[5];
extern const uint16_t scTlkDrvCodec44k1Param[5];

unsigned short audio_i2s_16k_config[5] = {4, 1875, 0, 32, 32};
unsigned short audio_i2s_8k_config[5]  = {4, 625, 3, 32, 32};


const tlkdrv_codec_modinf_t gcTlkDrvIisMstInf_tl721x = {
    .IsOpen = tlkdrv_iismst_isOpen,
    .Init   = tlkdrv_iismst_init,
    .Open   = tlkdrv_iismst_open,
    .Close  = tlkdrv_iismst_close,
    .Config = tlkdrv_iismst_config,
};

static tlkdrv_iismst_t sTlkDrvIisMstCtrl = {0};

/**
 * @brief Check if I2S master sub-device is open
 * @param[in] subDev - Sub-device to check
 * @return True if sub-device is open, false otherwise
 */
static bool tlkdrv_iismst_isOpen(uint8_t subDev)
{
    if (!sTlkDrvIisMstCtrl.isInit || !sTlkDrvIisMstCtrl.isOpen) {
        return false;
    }
    if (subDev == TLKDRV_CODEC_SUBDEV_MIC) {
        if (sTlkDrvIisMstCtrl.micIsEn) {
            return true;
        } else {
            return false;
        }
    } else if (subDev == TLKDRV_CODEC_SUBDEV_SPK) {
        if (sTlkDrvIisMstCtrl.spkIsEn) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

/**
 * @brief Initialize I2S master
 * @param[in] subDev - Sub-device to initialize
 * @return Operation result status
 */
static int tlkdrv_iismst_init(uint8_t subDev)
{
    (void)subDev;
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkdrv_iismst_t)));
    memset(&sTlkDrvIisMstCtrl, 0, sizeof(tlkdrv_iismst_t));

    sTlkDrvIisMstCtrl.isInit     = true;
    sTlkDrvIisMstCtrl.isOpen     = false;
    sTlkDrvIisMstCtrl.isMute     = false;
    sTlkDrvIisMstCtrl.micVol     = 60;
    sTlkDrvIisMstCtrl.bitDepth   = 16;
    sTlkDrvIisMstCtrl.sampleRate = 48000;

    return TLK_ENONE;
}

/**
 * @brief Open I2S master sub-device
 * @param[in] subDev - Sub-device to open
 * @return Operation result status
 */
static int tlkdrv_iismst_open(uint8_t subDev)
{
    if (!sTlkDrvIisMstCtrl.isInit) {
        return -TLK_ESTATUS;
    }
    if (sTlkDrvIisMstCtrl.isOpen) {
        return -TLK_EREPEAT;
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        sTlkDrvIisMstCtrl.micIsEn = true;
    } else {
        sTlkDrvIisMstCtrl.micIsEn = false;
    }
    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        sTlkDrvIisMstCtrl.spkIsEn = true;
    } else {
        sTlkDrvIisMstCtrl.spkIsEn = false;
    }
    tlkdrv_iismst_enable(sTlkDrvIisMstCtrl.bitDepth, sTlkDrvIisMstCtrl.channel, sTlkDrvIisMstCtrl.sampleRate, sTlkDrvIisMstCtrl.micIsEn, sTlkDrvIisMstCtrl.spkIsEn);

    sTlkDrvIisMstCtrl.isOpen = true;
    return TLK_ENONE;
}

/**
 * @brief Close I2S master sub-device
 * @param[in] subDev - Sub-device to close
 * @return Operation result status
 */
static int tlkdrv_iismst_close(uint8_t subDev)
{
    (void)subDev;
    if (!sTlkDrvIisMstCtrl.isInit || !sTlkDrvIisMstCtrl.isOpen) {
        return -TLK_ESTATUS;
    }

    tlkdrv_iismst_disable();

    sTlkDrvIisMstCtrl.isOpen  = false;
    sTlkDrvIisMstCtrl.micIsEn = false;
    sTlkDrvIisMstCtrl.spkIsEn = false;

    return TLK_ENONE;
}

/**
 * @brief Configure I2S master sub-device
 * @param[in] subDev - Sub-device to configure
 * @param[in] opcode - Configuration operation code
 * @param[in] param0 - First parameter
 * @param[in] param1 - Second parameter
 * @return Operation result status
 */
static int tlkdrv_iismst_config(uint8_t subDev, uint8_t opcode, uint32_t param0, uint32_t param1)
{
    int ret = -TLK_ENOSUPPORT;
    switch (opcode) {
    case TLKDRV_CODEC_OPCODE_SET_CHANNEL: // param:[uint8_t]chnCnt-1/2/3
        ret = tlkdrv_iismst_setChannelDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_CHANNEL:
        ret = tlkdrv_iismst_getChannelDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_SET_BIT_DEPTH: // param:[uint8_t]bitDepth-8,16,20,24,32
        ret = tlkdrv_iismst_setBitDepthDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_BIT_DEPTH:
        ret = tlkdrv_iismst_getBitDepthDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_SET_SAMPLE_RATE: // param:[uint32_t]bitDepth-8,16,20,24,32
        ret = tlkdrv_iismst_setSampleRateDeal(subDev, param0, param1);
        break;
    case TLKDRV_CODEC_OPCODE_GET_SAMPLE_RATE:
        ret = tlkdrv_iismst_getSampleRateDeal(subDev, param0, param1);
        break;
    }
    return ret;
}

/**
 * @brief Set channel configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Channel parameter
 * @param[in] param1 - Reserved
 * @return Operation result status
 */
static int tlkdrv_iismst_setChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param1;
    uint8_t channel = param0 & 0xFF;
    if (channel != 0x01 && channel != 0x02 && channel != 0x03) {
        return -TLK_EPARAM;
    }
    sTlkDrvIisMstCtrl.channel = channel;
    return TLK_ENONE;
}

/**
 * @brief Get channel configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Reserved
 * @param[in] param1 - Reserved
 * @return Current channel configuration
 */
static int tlkdrv_iismst_getChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;
    return sTlkDrvIisMstCtrl.channel;
}

/**
 * @brief Set bit depth configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Bit depth parameter
 * @param[in] param1 - Reserved
 * @return Operation result status
 */
static int tlkdrv_iismst_setBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param1;
    uint8_t bitDapth = param0 & 0xFF;
    if (bitDapth != 16 && bitDapth != 20 && bitDapth != 24) {
        return -TLK_ENOSUPPORT;
    }
    sTlkDrvIisMstCtrl.bitDepth = bitDapth;
    return TLK_ENONE;
}

/**
 * @brief Get bit depth configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Reserved
 * @param[in] param1 - Reserved
 * @return Current bit depth configuration
 */
static int tlkdrv_iismst_getBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;
    return sTlkDrvIisMstCtrl.bitDepth;
}

/**
 * @brief Set sample rate configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Sample rate parameter
 * @param[in] param1 - Reserved
 * @return Operation result status
 */
static int tlkdrv_iismst_setSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param1;
    uint32_t sampleRate = param0;
    if (sampleRate != 8000 && sampleRate != 16000 && sampleRate != 24000 && sampleRate != 32000 && sampleRate != 44100 && sampleRate != 48000) {
        return -TLK_EPARAM;
    }
    sTlkDrvIisMstCtrl.sampleRate = sampleRate;
    return TLK_ENONE;
}

/**
 * @brief Get sample rate configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Reserved
 * @param[in] param1 - Reserved
 * @return Current sample rate configuration
 */
static int tlkdrv_iismst_getSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;
    return sTlkDrvIisMstCtrl.sampleRate;
}

/**
 * @brief Enable I2S master with specified parameters
 * @param[in] bitDepth - Bit depth
 * @param[in] channel - Channel configuration
 * @param[in] sampleRate - Sample rate
 * @param[in] enMic - Enable microphone
 * @param[in] enSpk - Enable speaker
 * @return Operation result status
 */
static int tlkdrv_iismst_enable(uint8_t bitDepth, uint8_t channel, uint32_t sampleRate, bool enMic, bool enSpk)
{
    uint8_t dataWdith  = 0xFF;
    uint8_t drvChannel = 0xFF;

    const uint16_t *pSampleParam = NULL;

    if (!enMic && !enSpk) {
        tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iismst_enable: mic or spk is all disable");
        return -TLK_EPARAM;
    }
    if (sampleRate == 8000) {
        pSampleParam = scTlkDrvCodec8kParam;
    } else if (sampleRate == 16000) {
        pSampleParam = scTlkDrvCodec16kParam;
    } else if (sampleRate == 24000) {
        pSampleParam = scTlkDrvCodec24kParam;
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
        drvChannel = I2S_CHANNEL_STEREO;
    } else {
        drvChannel = I2S_CHANNEL_LEFT;
    }

    if (pSampleParam == NULL || dataWdith == 0xFF) {
        tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iismst_enable: sampleRate or bitDepth is not supported");
        return -TLK_EPARAM;
    }

    hal_i2s_config.data_width  = dataWdith;
    hal_i2s_config.sample_rate = (uint16_t *)pSampleParam;

    hal_i2c_init();

    /***initialize the audio module***/
    audio_init();
    /***Initialize i2s configuration***/
    audio_i2s_config_init(&hal_i2s_config);

    hal_i2s_input.i2s_select    = hal_i2s_config.i2s_select;
    hal_i2s_input.data_width    = dataWdith;
    hal_i2s_input.i2s_ch_sel    = drvChannel;
    hal_i2s_input.fifo_chn      = TLKDRV_CODEC_MIC_FIFO;
    hal_i2s_input.dma_num       = TLKDRV_CODEC_MIC_DMA;
    hal_i2s_input.data_buf      = gpTlkDrvCodecMicBuffer;
    hal_i2s_input.data_buf_size = gTlkDrvCodecMicBuffLen;


    hal_i2s_output.i2s_select = hal_i2s_config.i2s_select;
    hal_i2s_output.data_width = dataWdith;
    hal_i2s_output.i2s_ch_sel = drvChannel;
    hal_i2s_output.fifo_chn   = TLKDRV_CODEC_SPK_FIFO;
    hal_i2s_output.dma_num    = TLKDRV_CODEC_SPK_DMA;
#if AUDIO_CODEC_LOOPBACK
    hal_i2s_output.data_buf      = gpTlkDrvCodecMicBuffer;
    hal_i2s_output.data_buf_size = gTlkDrvCodecMicBuffLen;
#else
    hal_i2s_output.data_buf      = gpTlkDrvCodecSpkBuffer;
    hal_i2s_output.data_buf_size = gTlkDrvCodecSpkBuffLen;
#endif

    audio_i2s_input_init(&hal_i2s_input);
    audio_i2s_output_init(&hal_i2s_output);

    audio_rx_dma_chain_init(hal_i2s_input.fifo_chn, hal_i2s_input.dma_num, (unsigned short *)hal_i2s_input.data_buf, hal_i2s_input.data_buf_size);
    audio_tx_dma_chain_init(hal_i2s_output.fifo_chn, hal_i2s_output.dma_num, (unsigned short *)hal_i2s_output.data_buf, hal_i2s_output.data_buf_size);

    audio_i2s_clk_en(hal_i2s_config.i2s_select);

    nau8821_i2c_probe();
    audio_set_i2s_clk_as_mclk(I2S0, GPIO_FC_PE7, 0x01, sys_clk.pll_clk / 12);

    audio_rx_dma_en(hal_i2s_input.dma_num);
    audio_tx_dma_en(hal_i2s_output.dma_num);

    tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iismst_enable: bitDepth[%d], channel[%d], sampleRate[%d]", sTlkDrvIisMstCtrl.bitDepth,
                 sTlkDrvIisMstCtrl.channel, sTlkDrvIisMstCtrl.sampleRate);

    return TLK_ENONE;
}

/**
 * @brief Disable I2S master
 * @param None
 * @return None
 */
static void tlkdrv_iismst_disable(void)
{
    tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iismst_disable: 001");

    audio_power_down();

    audio_rx_dma_dis(TLKDRV_CODEC_MIC_DMA);
    audio_tx_dma_dis(TLKDRV_CODEC_SPK_DMA);

    //    tmemset(gpTlkDrvCodecMicBuffer, 0, gTlkDrvCodecMicBuffLen);
    //    tmemset(gpTlkDrvCodecSpkBuffer, 0, gTlkDrvCodecSpkBuffLen);
}


#endif // #if (TLKDRV_CODEC_I2S_MASTER_ENABLE)

#endif
