/********************************************************************************************************
 * @file    tlkdrv_icodec_tl721x.c
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
#include "tlkdrv_i2s_ex_codec_config.h"
#include "tlkdrv_icodec_tl721x.h"
#if (MCU_CORE_TYPE == MCU_CORE_TL721X && TLKDRV_CODEC_ICODEC_ENABLE)
#include <stdlib.h>
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"

#ifndef TLKDRV_CODEC_TL721X_ABNORMAL_CFG
#define TLKDRV_CODEC_TL721X_ABNORMAL_CFG 0
#endif

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
//static tlkdrv_icodec_status_t sTlkDrvIcodecAdcStat = {0};
//static tlkdrv_icodec_status_t sTlkDrvIcodecDacStat = {0};

uint32_t tl721x_codec_hd_freq_table[7]     = {8000, 16000, 24000, 32000, 44100, 48000, 0xffffffff};
uint8_t  tl721x_codec_hd_bitdepth_table[4] = {16, 20, 0xff};
uint8_t  tl721x_codec_hd_channal_table[4]  = {1, 2, 3, 0xff};

sdm_pin_config_t sdm_pin_config = {
    .sdm0_p_pin =
        TLKDRV_ICODEC_SDM0P_PIN, //Both the SDM and printf print functions use the PA0 pin. If the SDM function is used, modify the pin used for DEBUG_INFO_TX_PIN in printf.h.
    .sdm0_n_pin = TLKDRV_ICODEC_SDM0N_PIN,
    .sdm1_p_pin = TLKDRV_ICODEC_SDM1P_PIN,
    .sdm1_n_pin = TLKDRV_ICODEC_SDM1N_PIN,
};

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
 * @brief Initialize TL721X internal codec hardware
 * @param None
 * @return None
 */
void tlkdrv_tl721x_codec_hd_init(void) {}

/**
 * @brief Set audio receive write pointer
 * @param[in] rx_fifo_chn - Receive FIFO channel
 * @param[in] val - Write pointer value
 * @return None
 */
void audio_set_rx_wptr(audio_fifo_chn_e rx_fifo_chn, unsigned short val)
{
    reg_rxfifo_wptr(rx_fifo_chn) = val;
}

/**
 * @brief Convert audio sample rate to index
 * @param[in] audio_rate - Audio sample rate
 * @return Index corresponding to the sample rate
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
 * @param[in] value - Bit depth value to check
 * @return Operation result status
 */
static int icodec_bitdepth_check(uint8_t value)
{
    uint8_t count = sizeof(tl721x_codec_hd_bitdepth_table) / sizeof(uint8_t);
    for (uint8_t i = 0; i < count; i++) {
        if (value == tl721x_codec_hd_bitdepth_table[i]) {
            break;
        } else if (tl721x_codec_hd_bitdepth_table[i] == 0xff) {
            return -TLK_ENOSUPPORT;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Check if channel configuration is supported
 * @param[in] value - Channel configuration value to check
 * @return Operation result status
 */
static int icodec_channal_check(uint8_t value)
{
    uint8_t count = sizeof(tl721x_codec_hd_channal_table) / sizeof(uint8_t);
    for (uint8_t i = 0; i < count; i++) {
        if (value == tl721x_codec_hd_channal_table[i]) {
            break;
        } else if (tl721x_codec_hd_channal_table[i] == 0xff) {
            return -TLK_EPARAM;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Check if frequency is supported
 * @param[in] value - Frequency value to check
 * @return Operation result status
 */
static int icodec_freq_check(uint32_t value)
{
    uint8_t count = sizeof(tl721x_codec_hd_freq_table) / sizeof(int);

    for (uint8_t i = 0; i < count; i++) {
        if (value == tl721x_codec_hd_freq_table[i]) {
            break;
        } else if (tl721x_codec_hd_freq_table[i] == 0xffffffff) {
            return -TLK_EPARAM;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Mute audio DAC
 * @param None
 * @return None
 */
void audio_dac_mute(void) {}

/**
 * @brief Unmute audio DAC
 * @param None
 * @return None
 */
void audio_dac_unmute(void) {}

/**
 * @brief Check if internal codec sub-device is open
 * @param[in] subDev - Sub-device to check
 * @return True if sub-device is open, false otherwise
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
 * @brief Initialize external driver audio
 * @param None
 * @return None
 */
static void tlkdrv_ext_drv_audio_init(void)
{
    audio_power_on();
#if TLKDRV_CODEC_TL721X_ABNORMAL_CFG
    audio_set_audio_clk(1, sys_clk.pll_clk / 12); // audio clk=240M*(div_numerator/div_denominator):240M*(1/pll_clk / 24)=24M
#else
    audio_set_audio_clk(1, sys_clk.pll_clk / 24);
#endif
}

/**
 * @brief Initialize internal codec
 * @param[in] subDev - Sub-device to initialize
 * @return Operation result status
 */
static int tlkdrv_icodec_init(uint8_t subDev)
{
    (void)subDev;
    if (!sTlkDrvIcodecCtrl.isInit) {
        STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkdrv_icodec_t)));
        memset(&sTlkDrvIcodecCtrl, 0, sizeof(tlkdrv_icodec_t));
    }

    sTlkDrvIcodecCtrl.isInit = true;
    return TLK_ENONE;
}

/**
 * @brief Open internal codec sub-device
 * @param[in] subDev - Sub-device to open
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

    tlkdrv_ext_drv_audio_init();

    ///MIC enable
    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen) {
            return -TLK_EREPEAT;
        }

#if (RECORDING_CARD_EN)
#if (!TLKMDI_DMIC_POWER_SUSPEND_DIS)
        if (TLKDRV_ICODEC_POWER_PIN != GPIO_NONE_PIN) {
            gpio_function_en((gpio_pin_e)TLKDRV_ICODEC_POWER_PIN);
            gpio_input_dis((gpio_pin_e)TLKDRV_ICODEC_POWER_PIN);
            gpio_set_high_level((gpio_pin_e)TLKDRV_ICODEC_POWER_PIN);
            gpio_output_en((gpio_pin_e)TLKDRV_ICODEC_POWER_PIN);
        }
#endif
#endif
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
 * @brief Reset DMIC pins for audio codec
 * @param[in] dmic0_data - DMIC0 data pin
 * @param[in] dmic0_clk1 - DMIC0 clock1 pin
 * @param[in] dmic0_clk2 - DMIC0 clock2 pin
 * @return None
 */
void audio_codec_reset_dmic_pin(gpio_func_pin_e dmic0_data, gpio_func_pin_e dmic0_clk1, gpio_func_pin_e dmic0_clk2)
{
    /* codec0 dmic0 data. */
    gpio_input_dis((gpio_pin_e)dmic0_data);
    gpio_output_dis((gpio_pin_e)dmic0_data);
    gpio_function_en((gpio_pin_e)dmic0_data);

    /* codec0 dmic0 clock1. */
    gpio_input_dis((gpio_pin_e)dmic0_clk1);
    gpio_output_dis((gpio_pin_e)dmic0_clk1);
    gpio_function_en((gpio_pin_e)dmic0_clk1);

    /* codec0 dmic1 clock2. */
    if (dmic0_clk2 != GPIO_NONE_PIN) {
        gpio_input_dis((gpio_pin_e)dmic0_clk2);
        gpio_output_dis((gpio_pin_e)dmic0_clk2);
        gpio_function_en((gpio_pin_e)dmic0_clk2);
    }
}

/**
 * @brief Close internal codec sub-device
 * @param[in] subDev - Sub-device to close
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
#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)

#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        audio_codec_reset_dmic_pin((gpio_func_pin_e)TLKDRV_ICODEC_DMIC_DATA_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK0_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK1_PIN);
#if (RECORDING_CARD_EN)
#if (!TLKMDI_DMIC_POWER_SUSPEND_DIS)
        if (TLKDRV_ICODEC_POWER_PIN != GPIO_NONE_PIN) {
            gpio_input_dis((gpio_pin_e)TLKDRV_ICODEC_POWER_PIN);
            gpio_output_dis((gpio_pin_e)TLKDRV_ICODEC_POWER_PIN);
            gpio_function_en((gpio_pin_e)TLKDRV_ICODEC_POWER_PIN);
        }
#endif
#endif
#endif
        audio_rx_dma_dis(gTlkdrvCodecMicDmaChn);
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen = false;
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsEn   = false;
        audio_tx_dma_dis(gTlkdrvCodecSpkDmaChn);
    }

    if (!(sTlkDrvIcodecCtrl.codec_mic_cfg.IsEn || sTlkDrvIcodecCtrl.codec_spk_cfg.IsEn)) {
#if RECORDING_CARD_EN
        audio_power_down();
#endif
    }

    return TLK_ENONE;
}

/**
 * @brief Configure internal codec sub-device
 * @param[in] subDev - Sub-device to configure
 * @param[in] opcode - Configuration operation code
 * @param[in] param0 - First parameter
 * @param[in] param1 - Second parameter
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
 * @brief Set channel configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Channel parameter
 * @param[in] param1 - Reserved
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
 * @brief Get channel configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Reserved
 * @param[in] param1 - Reserved
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
 * @brief Set bit depth configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Bit depth parameter
 * @param[in] param1 - Reserved
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
 * @brief Get bit depth configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Reserved
 * @param[in] param1 - Reserved
 * @return Current bit depth configuration
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
 * @brief Set sample rate configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Sample rate parameter
 * @param[in] param1 - Reserved
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
 * @brief Get sample rate configuration
 * @param[in] subDev - Sub-device
 * @param[in] param0 - Reserved
 * @param[in] param1 - Reserved
 * @return Current sample rate configuration
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

signed short sin_48k_stereo_d2[] __attribute__((aligned(4))) = {
    0,     0,     1069,  1069,  2120,  2120,  3134,  3134,  4095,  4095,  4986,  4986,  5792,  5792,  6499,  6499,  7094,  7094,  7568,  7568,  7912,  7912,  8121,  8121,
    8191,  8191,  8121,  8121,  7912,  7912,  7568,  7568,  7094,  7094,  6499,  6499,  5792,  5792,  4986,  4986,  4096,  4096,  3134,  3134,  2120,  2120,  1069,  1069,
    0,     0,     -1069, -1069, -2120, -2120, -3134, -3134, -4095, -4095, -4986, -4986, -5792, -5792, -6499, -6499, -7094, -7094, -7568, -7568, -7912, -7912, -8121, -8121,
    -8191, -8191, -8121, -8121, -7912, -7912, -7568, -7568, -7094, -7094, -6499, -6499, -5792, -5792, -4986, -4986, -4096, -4096, -3134, -3134, -2120, -2120, -1069, -1069,
};

/**
 * @brief Set DMIC digital gain
 * @param[in] gain - Gain value to set
 * @return 1 if successful, 0 if gain value is too large
 */
uint8_t tlkdrv_icodec_dmic_d_gain(uint8_t gain)
{
    if (gain > 40) {
        return 0;
    }
    uint8_t codec_gain_offset = (gain + 3) / 6 * 4;
    audio_set_stream0_dig_gain(CODEC_IN_D_GAIN_0_DB + codec_gain_offset);
    return 1;
}

/**
 * @brief Enable/disable microphone
 * @param[in] enMic - Enable flag
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
#endif
#if TLKDRV_CODEC_TL721X_ABNORMAL_CFG
        sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate = sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate * 2;
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
        audio_set_stream0_dig_gain(CODEC_IN_D_GAIN_18_DB);
#endif //#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)

        /* rx dma init. */
        audio_rx_dma_chain_init(TLKDRV_CODEC_MIC_FIFO, gTlkdrvCodecMicDmaChn, (unsigned short *)gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen);

        //	        audio_mic_mute_en();
        audio_codec_stream0_input_en(gTlkdrvCodecMicDmaChn);
        //	        audio_codec_clr_input_pop(20);
        audio_codec_input_path_en(inputParam.fifo_chn);
        //	        audio_mic_mute_dis();

        //	        audio_rx_dma_en(gTlkdrvCodecMicDmaChn); /* the rx dma enable must precede the adc enable. */
    }

    return TLK_ENONE;
}

/**
 * @brief Enable/disable speaker
 * @param[in] enSpk - Enable flag
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
        tlkapi_info(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_icodec_enable: <SPK>: %d %d %d", spkSrc, spkSRate, spkDWdith);
    }

    if (enSpk && gpTlkDrvCodecSpkBuffer != NULL && gTlkDrvCodecSpkBuffLen != 0) {
        tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_SPK: setting ", 0, 0);
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
		outputParam.data_buf = sin_48k_stereo_d2;
		outputParam.data_buf_size = sizeof(sin_48k_stereo_d2);
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

#endif // #if (MCU_CORE_TYPE == MCU_CORE_TL721X && TLKDRV_CODEC_ICODEC_ENABLE)
