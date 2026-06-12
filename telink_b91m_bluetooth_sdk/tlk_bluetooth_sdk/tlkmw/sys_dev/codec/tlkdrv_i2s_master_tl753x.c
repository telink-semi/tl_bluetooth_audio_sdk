/********************************************************************************************************
 * @file    tlkdrv_i2s_master_tl753x.c
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
#if (MCU_CORE_TYPE == MCU_CORE_TL753X)
#include "tl_common.h"
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkdrv_codec.h"
#if (TLKDRV_CODEC_I2S_MASTER_ENABLE)


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
} tlkdrv_iismst_t;

static bool tlkdrv_iismst_isOpen(uint8_t subDev);
static int  tlkdrv_iismst_init(uint8_t subDev);
static int  tlkdrv_iismst_open(uint8_t subDev);
static int  tlkdrv_iismst_close(uint8_t subDev);
static int  tlkdrv_iismst_config(uint8_t subDev, uint8_t opcode, uint32_t param0, uint32_t param1);

static int  tlkdrv_iismst_enable(uint8_t bitDepth, uint8_t channel, uint32_t sampleRate, bool enMic, bool enSpk);
static void tlkdrv_iismst_disable(void);
static int  tlkdrv_iismst_setChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int  tlkdrv_iismst_getChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int  tlkdrv_iismst_setBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int  tlkdrv_iismst_getBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int  tlkdrv_iismst_setSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1);
static int  tlkdrv_iismst_getSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1);


extern uint16_t gTlkDrvCodecSpkBuffLen;
extern uint16_t gTlkDrvCodecMicBuffLen;
extern uint8_t *gpTlkDrvCodecSpkBuffer;
extern uint8_t *gpTlkDrvCodecMicBuffer;

unsigned short audio_i2s_192k_config[5] = {1, 7, 0, 64, 64};
unsigned short audio_i2s_96k_config[5]  = {1, 7, 1, 64, 64};
/* sampling rate = pll1_clk(default 86.016MHz) * (1 / 7) / (2 * 2) / (64)  = 48KHz */
unsigned short audio_i2s_48k_config[5]   = {1, 7, 2, 64, 64};
unsigned short audio_i2s_44p1k_config[5] = {1, 7, 2, 64, 64};
unsigned short audio_i2s_32k_config[5]   = {1, 7, 3, 64, 64};
unsigned short audio_i2s_24k_config[5]   = {1, 7, 4, 64, 64};
unsigned short audio_i2s_16k_config[5]   = {1, 7, 6, 64, 64};


uint32_t tl753x_codec_hd_freq_table[8]     = {16000, 24000, 32000, 44100, 48000, 96000, 192000, 0xffffffff};
uint8_t  tl753x_codec_hd_bitdepth_table[5] = {16, 20, 24, 32, 0xff};

i2s_pin_config_t i2s_pin_config = {
    .bclk_pin       = (gpio_func_pin_e)TLKDRV_I2S_MST_BCLIK_PIN,
    .adc_lr_clk_pin = (gpio_func_pin_e)TLKDRV_I2S_MST_ADC_CLK_PIN,
    .adc_dat_pin    = (gpio_func_pin_e)TLKDRV_I2S_MST_ADC_DAT_PIN,
    .dac_lr_clk_pin = (gpio_func_pin_e)TLKDRV_I2S_MST_DAC_CLK_PIN,
    .dac_dat_pin    = (gpio_func_pin_e)TLKDRV_I2S_MST_DAC_DAT_PIN,
};

audio_i2s_config_t audio_i2s0_config = {
    .i2s_select        = I2S0,
    .i2s_mode          = I2S_I2S_MODE,
    .pin_config        = &i2s_pin_config,
    .data_width        = I2S_BIT_24_DATA,
    .master_slave_mode = I2S_AS_MASTER_EN,
    .sample_rate       = audio_i2s_48k_config,
    .io_mode           = I2S_5_LINE_MODE,
};


const tlkdrv_codec_modinf_t gcTlkDrvIisMstInf_b92 = {
    .IsOpen = tlkdrv_iismst_isOpen,
    .Init   = tlkdrv_iismst_init,
    .Open   = tlkdrv_iismst_open,
    .Close  = tlkdrv_iismst_close,
    .Config = tlkdrv_iismst_config,
};
static tlkdrv_iismst_t sTlkDrvIisMstCtrl = {0};

static bool tlkdrv_iismst_isOpen(uint8_t subDev)
{
    (void)subDev;

    if (!sTlkDrvIisMstCtrl.isInit || !sTlkDrvIisMstCtrl.isOpen) {
        return false;
    }
    if (sTlkDrvIisMstCtrl.isOpen) {
        return true;
    } else {
        return false;
    }
}

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

static int tlkdrv_iismst_open(uint8_t subDev)
{
    if (!sTlkDrvIisMstCtrl.isInit) {
        return -TLK_ESTATUS;
    }
    if (sTlkDrvIisMstCtrl.isOpen) {
        return -TLK_EREPEAT;
    }
    audio_init(PLL_AUDIO_CLK_86P016M);

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

    //    tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iismst_enable");
    tlkdrv_iismst_enable(sTlkDrvIisMstCtrl.bitDepth, sTlkDrvIisMstCtrl.channel, sTlkDrvIisMstCtrl.sampleRate, sTlkDrvIisMstCtrl.micIsEn, sTlkDrvIisMstCtrl.spkIsEn);

    sTlkDrvIisMstCtrl.isOpen = true;
    return TLK_ENONE;
}

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
    default:
        break;
    }
    return ret;
}

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

static int tlkdrv_iismst_getChannelDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;

    return sTlkDrvIisMstCtrl.channel;
}

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

static int tlkdrv_iismst_getBitDepthDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;
    return sTlkDrvIisMstCtrl.bitDepth;
}

static int tlkdrv_iismst_setSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param1;
    uint32_t sampleRate = param0;

    uint8_t count = sizeof(tl753x_codec_hd_freq_table) / sizeof(int);

    for (uint8_t i = 0; i < count; i++) {
        if (sampleRate == tl753x_codec_hd_freq_table[i]) {
            break;
        } else if (tl753x_codec_hd_freq_table[i] == 0xffffffff) {
            return -TLK_EPARAM;
        }
    }

    sTlkDrvIisMstCtrl.sampleRate = sampleRate;
    return TLK_ENONE;
}

static int tlkdrv_iismst_getSampleRateDeal(uint8_t subDev, uint32_t param0, uint32_t param1)
{
    (void)subDev;
    (void)param0;
    (void)param1;
    return sTlkDrvIisMstCtrl.sampleRate;
}

static int tlkdrv_iismst_enable(uint8_t bitDepth, uint8_t channel, uint32_t sampleRate, bool enMic, bool enSpk)
{
    (void)channel;
    uint8_t dataWdith = 0xFF;

    const uint16_t *pSampleParam = NULL;

    if (!enMic && !enSpk) {
        tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iismst_enable: mic or spk is all disable");
        return -TLK_EPARAM;
    }

    if (sampleRate == 16000) {
        pSampleParam = audio_i2s_16k_config;
    } else if (sampleRate == 32000) {
        pSampleParam = audio_i2s_32k_config;
    } else if (sampleRate == 44100) {
        pSampleParam = audio_i2s_44p1k_config;
    } else if (sampleRate == 48000) {
        pSampleParam = audio_i2s_48k_config;
    }
    if (bitDepth == 16) {
        dataWdith = I2S_BIT_16_DATA;
    } else if (bitDepth == 20) {
        dataWdith = I2S_BIT_20_DATA;
    } else if (bitDepth == 24) {
        dataWdith = I2S_BIT_24_DATA;
    } else if (bitDepth == 32) {
        dataWdith = I2S_BIT_32_DATA;
    }

    if (pSampleParam == NULL || dataWdith == 0xFF) {
        tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iismst_enable: sampleRate or bitDepth is not supported");
        return -TLK_EPARAM;
    }

    audio_i2s0_config.data_width  = dataWdith;
    audio_i2s0_config.sample_rate = (uint16_t *)pSampleParam;

    audio_i2s_config_init(&audio_i2s0_config);

    audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_I2S0_RX, FIFO_RX_I2S_RX_CHN01_20_OR_24); /* rx fifo source select i2s0 */
    audio_rx_dma_chain_init(TLKDRV_CODEC_MIC_FIFO, gTlkdrvCodecMicDmaChn, (unsigned short *)gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen);


    audio_matrix_set_i2s_tx_route(I2S0_CHN0, I2S_TX_ROUTE_FIFO, I2S_TX_FIFO0_20_OR_24_STEREO + TLKDRV_CODEC_SPK_FIFO); /* i2s0_ch0 tx sel fifo */
    audio_matrix_set_i2s_tx_route(I2S0_CHN1, I2S_TX_ROUTE_FIFO, I2S_TX_FIFO0_20_OR_24_STEREO + TLKDRV_CODEC_SPK_FIFO); /* i2s0_ch1 tx sel fifo */
#if (AUDIO_CODEC_LOOPBACK)
    audio_tx_dma_chain_init(TLKDRV_CODEC_SPK_FIFO, gTlkdrvCodecSpkDmaChn, (unsigned short *)gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen);
#else
    audio_tx_dma_chain_init(TLKDRV_CODEC_SPK_FIFO, gTlkdrvCodecSpkDmaChn, (unsigned short *)gpTlkDrvCodecSpkBuffer, gTlkDrvCodecSpkBuffLen);
#endif

    audio_rx_dma_en(gTlkdrvCodecMicDmaChn);
    audio_tx_dma_en(gTlkdrvCodecSpkDmaChn);

    audio_i2s_clk_en(audio_i2s0_config.i2s_select);

    return TLK_ENONE;
}

static void tlkdrv_iismst_disable(void)
{
    tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_iismst_disable: 001");

    audio_i2s_clk_dis(audio_i2s0_config.i2s_select);

    audio_rx_dma_dis(gTlkdrvCodecMicDmaChn);
    audio_tx_dma_dis(gTlkdrvCodecSpkDmaChn);
}


#endif // #if (TLKDRV_CODEC_I2S_MASTER_ENABLE)

#endif // #if (MCU_CORE_TYPE == MCU_CORE_B92)
