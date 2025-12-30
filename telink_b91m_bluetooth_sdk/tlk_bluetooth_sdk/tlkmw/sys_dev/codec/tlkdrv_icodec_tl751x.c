/********************************************************************************************************
 * @file    tlkdrv_icodec_tl751x.c
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
#include "tlkdrv_icodec_tl751x.h"
#if (MCU_CORE_TYPE == MCU_CORE_TL751X && TLKDRV_CODEC_ICODEC_ENABLE)
#include <stdlib.h>
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"


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
static tlkdrv_icodec_t        sTlkDrvIcodecCtrl    = {0};
static tlkdrv_icodec_status_t sTlkDrvIcodecAdcStat = {0};
static tlkdrv_icodec_status_t sTlkDrvIcodecDacStat = {0};

uint32_t tl751x_codec_hd_freq_table[8]     = {16000, 44100, 48000, 96000, 192000, 384000, 768000, 0xffffffff};
uint8_t  tl751x_codec_hd_bitdepth_table[4] = {16, 24, 0xff};
uint8_t  tl751x_codec_hd_channal_table[4]  = {1, 2, 3, 0xff};
uint8_t  tl751x_codec_cur_power_mode       = 0;
uint32_t st_audio_sys_clk;

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
 * @brief Initialize TL751X internal codec hardware
 * @param None
 * @return None
 */
void tlkdrv_tl751x_codec_hd_init(void)
{
    sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_36P864M;
    if (audio_hd_anc_enable) {
        sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_184P32M;
    }

    clock_pll_audio_init(sTlkDrvIcodecCtrl.sys_mclk);
    audio_init(sTlkDrvIcodecCtrl.sys_mclk); /* must configured first. */

    if (CODEC_INPUT_MODE == CODEC_INPUT_DMIC) {
        audio_codec0_power_on(AUDIO_CODEC0_DIGITAL_AND_DAC, AUDIO_CODEC0_1P8V); /* power on codec digital and dac. */
    } else {
        audio_codec0_power_on(AUDIO_CODEC0_ADC_AND_DAC, AUDIO_CODEC0_1P8V); /* power on adc and dac. */
    }
}

/**
 * @brief Deinitialize TL751X internal codec hardware
 * @param None
 * @return None
 */
void tlkdrv_tl751x_codec_hd_deinit(void)
{
    g_audio_pll_is_used = 0;
}

/**
 * @brief Initialize DAC sidetone
 * @param[in] samplerate - Sample rate
 * @return Operation result status
 */
int tlkdrv_tl751x_dac_sidetone_init(int samplerate)
{
    int ret = 1;
    // audio_matrix_set_side_tone_route(SIDE_TONE_CHN6, SIDE_TONE_ROUTE_ADC0, SIDE_TONE_ADC_RIGHT_32_BIT);
    // audio_matrix_set_side_tone_route(SIDE_TONE_CHN7, SIDE_TONE_ROUTE_ADC0, SIDE_TONE_ADC_RIGHT_32_BIT);
    audio_side_tone_set_dac_gain(SIDE_TONE_CHN6, SIDETONE_0DB);
    audio_side_tone_set_adc_gain(SIDE_TONE_CHN6, SIDETONE_0DB);
    audio_side_tone_set_dac_gain(SIDE_TONE_CHN7, SIDETONE_0DB);
    audio_side_tone_set_adc_gain(SIDE_TONE_CHN7, SIDETONE_0DB);

    switch (sTlkDrvIcodecCtrl.sys_mclk) {
    case (PLL_AUDIO_CLK_33P8688M):
        audio_side_tone_set_req_count(SIDE_TONE_CHN67, 33868800, samplerate, 2);
        break;
    case (PLL_AUDIO_CLK_36P864M):
        audio_side_tone_set_req_count(SIDE_TONE_CHN67, 36864000, samplerate, 2);
        break;
    case (PLL_AUDIO_CLK_147P456M):
        audio_side_tone_set_req_count(SIDE_TONE_CHN67, 147456000, samplerate, 2);
        break;
    case (PLL_AUDIO_CLK_169P344M):
        audio_side_tone_set_req_count(SIDE_TONE_CHN67, 169344000, samplerate, 2);
        break;
    case (PLL_AUDIO_CLK_184P32M):
        audio_side_tone_set_req_count(SIDE_TONE_CHN67, 184320000, samplerate, 2);
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}

/**
 * @brief Convert audio sample rate to index value
 * @param[in] audio_rate - Audio sample rate in Hz
 * @return Index value corresponding to the given audio rate, or 0x00 if rate is not supported
 */
static inline int audio_sample_rate_to_index(int audio_rate)
{
    int rate_index;
#if 0
    rate_index = audio_rate == 16000 ? AUDIO_16K :
                     audio_rate == 48000 ? AUDIO_48K :
                    		 	 	 	   0xffffffff;
#else
    if (audio_rate == 16000) {
        rate_index = AUDIO_16K;
    } else if (audio_rate == 48000) {
        rate_index = AUDIO_48K;
    } else if (audio_rate == 96000) {
        rate_index = AUDIO_96K;
    } else if (audio_rate == 192000) {
        rate_index = AUDIO_192K;
    } else if (audio_rate == 384000) {
        rate_index = AUDIO_384K;
    } else if (audio_rate == 768000) {
        rate_index = AUDIO_768K;
    } else if (audio_rate == 44100) {
        rate_index = AUDIO_44P1K;
    } else {
        rate_index = 0x00;
    }

#endif
    return rate_index;
}

/**
 * @brief Check if the given bit depth value is supported
 * @param[in] value - Bit depth value to check
 * @return TLK_ENONE if supported, TLK_ENOSUPPORT if not supported
 */
static int icodec_bitdepth_check(uint8_t value)
{
    uint8_t count = sizeof(tl751x_codec_hd_bitdepth_table) / sizeof(uint8_t);
    for (uint8_t i = 0; i < count; i++) {
        if (value == tl751x_codec_hd_bitdepth_table[i]) {
            break;
        } else if (tl751x_codec_hd_bitdepth_table[i] == 0xff) {
            return -TLK_ENOSUPPORT;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Check if the given channel value is supported
 * @param[in] value - Channel value to check
 * @return TLK_ENONE if supported, TLK_EPARAM if not supported
 */
static int icodec_channal_check(uint8_t value)
{
    uint8_t count = sizeof(tl751x_codec_hd_channal_table) / sizeof(uint8_t);
    for (uint8_t i = 0; i < count; i++) {
        if (value == tl751x_codec_hd_channal_table[i]) {
            break;
        } else if (tl751x_codec_hd_channal_table[i] == 0xff) {
            return -TLK_EPARAM;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Check if the given frequency value is supported
 * @param[in] value - Frequency value to check
 * @return TLK_ENONE if supported, TLK_EPARAM if not supported
 */
static int icodec_freq_check(uint32_t value)
{
    uint8_t count = sizeof(tl751x_codec_hd_freq_table) / sizeof(int);

    for (uint8_t i = 0; i < count; i++) {
        if (value == tl751x_codec_hd_freq_table[i]) {
            break;
        } else if (tl751x_codec_hd_freq_table[i] == 0xffffffff) {
            return -TLK_EPARAM;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief Mute the audio DAC
 */
void audio_dac_mute(void)
{
    reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A1) |= FLD_CODEC_DAC_DAC_SOFT_MUTE;
    reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A2) |= FLD_CODEC_DAC_DAC_SOFT_MUTE;
}

/**
 * @brief Unmute the audio DAC
 */
void audio_dac_unmute(void)
{
    reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A1) &= (~FLD_CODEC_DAC_DAC_SOFT_MUTE);
    reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A2) &= (~FLD_CODEC_DAC_DAC_SOFT_MUTE);
}

/**
 * @brief Reset DMIC A pin configuration for codec0
 * @param[in] dmic0_data - GPIO pin for DMIC0 data
 * @param[in] dmic0_clk1 - GPIO pin for DMIC0 clock1
 * @param[in] dmic0_clk2 - GPIO pin for DMIC0 clock2
 */
void audio_codec0_reset_dmic_a_pin(gpio_func_pin_e dmic0_data, gpio_func_pin_e dmic0_clk1, gpio_func_pin_e dmic0_clk2)
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
 * @brief Check if the codec device is open
 * @param[in] subDev - Sub-device identifier
 * @return True if device is initialized and enabled, false otherwise
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
 * @brief Initialize the codec device
 * @param[in] subDev - Sub-device identifier
 * @return TLK_ENONE if initialization succeeds
 */
static int tlkdrv_icodec_init(uint8_t subDev)
{
    (void)subDev;
    if (!sTlkDrvIcodecCtrl.isInit) {
        STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkdrv_icodec_t)));
        memset(&sTlkDrvIcodecCtrl, 0, sizeof(tlkdrv_icodec_t));
    }

    //    sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_36P864M;
    //    if (audio_hd_anc_enable)
    //    {
    //    	sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_184P32M;
    //    }
    //
    //    clock_pll_audio_init(sTlkDrvIcodecCtrl.sys_mclk);

    sTlkDrvIcodecCtrl.isInit = true;

    return TLK_ENONE;
}

/**
 * @brief Open and configure the codec device
 * @param[in] subDev - Sub-device identifier
 * @return TLK_ENONE if operation succeeds, error code otherwise
 */
static int tlkdrv_icodec_open(uint8_t subDev)
{
    if (!sTlkDrvIcodecCtrl.isInit) {
        return -TLK_ESTATUS;
    }

    if (g_sys_power_on_codec_dis) {
        tlkdrv_tl751x_codec_hd_init();
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_BOTH)) {
        if (((sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate == 44100) || (sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate == 44100)) &&
            (sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate != sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate)) {
            tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_open(Err):Codec NO Support!!!", NULL, 0);
            return -TLK_ENOSUPPORT;
        }
    }


#if AUDIO_HD_HAC_EN
    if (audio_hd_anc_enable) {
        if (sTlkDrvIcodecCtrl.sys_mclk != PLL_AUDIO_CLK_184P32M) {
            sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_184P32M;
            st_audio_sys_clk           = 36864000;
            clock_pll_audio_init(sTlkDrvIcodecCtrl.sys_mclk);
        }
    } else {
        if (sTlkDrvIcodecCtrl.sys_mclk != PLL_AUDIO_CLK_36P864M) {
            sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_36P864M;
            st_audio_sys_clk           = 36864000;
            clock_pll_audio_init(sTlkDrvIcodecCtrl.sys_mclk);
        }
    }
#else
    if ((sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate != 44100) && (sTlkDrvIcodecCtrl.codec_spk_cfg.SampleRate != 44100)) {
        if (sTlkDrvIcodecCtrl.sys_mclk != PLL_AUDIO_CLK_36P864M) {
            sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_36P864M;
            clock_pll_audio_init(sTlkDrvIcodecCtrl.sys_mclk);
        }

    } else {
        if (sTlkDrvIcodecCtrl.sys_mclk != PLL_AUDIO_CLK_33P8688M) {
            sTlkDrvIcodecCtrl.sys_mclk = PLL_AUDIO_CLK_33P8688M;
            clock_pll_audio_init(sTlkDrvIcodecCtrl.sys_mclk);
        }
    }
#endif


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
 * @brief Close the codec device
 * @param[in] subDev - Sub-device identifier
 * @return TLK_ENONE if operation succeeds
 */
static int tlkdrv_icodec_close(uint8_t subDev)
{
    if (!sTlkDrvIcodecCtrl.isInit) {
        return -TLK_ESTATUS;
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_MIC) != 0) {
        sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen = false;
        sTlkDrvIcodecCtrl.codec_mic_cfg.IsEn   = false;
        audio_rx_dma_dis(TLKDRV_CODEC_MIC_DMA);
//        audio_codec0_set_input_snr_opt(AUDIO_AMIC_ADC_A1_A2,0);
#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        audio_codec0_set_micbias(AUDIO_AMIC_ADC_A1_A2, 0);
        audio_codec0_adc_power_down(AUDIO_AMIC_ADC_A1_A2);
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        audio_codec0_dmic_clk_en(AUDIO_DMIC_ADC_A1_A2, 0);
        audio_codec0_reset_dmic_a_pin((gpio_func_pin_e)TLKDRV_ICODEC_DMIC_DATA_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK0_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK1_PIN);

        if (g_sys_power_on_codec_dis) {
            audio_codec0_adc_power_down(AUDIO_DMIC_ADC_A1_A2);
            reg_audio_codec0_cr_vic = reg_audio_codec0_cr_vic | 0x0f;
            BM_CLR(reg_audio_clk_en_0, FLD_CLK_CODEC0_EN); /* After clock disable, cannot access codec registers. */

            pm_audio_pll_power_down();
            g_audio_pll_is_used = 0;
        }
#endif
    }

    if ((subDev & TLKDRV_CODEC_SUBDEV_SPK) != 0) {
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen = false;
        sTlkDrvIcodecCtrl.codec_spk_cfg.IsEn   = false;
        audio_dac_mute();
        audio_codec0_set_output_again(AUDIO_DAC_A1_A2, AUDIO_OUT_A_GAIN_m28_DB);
        audio_codec0_set_output_dgain(AUDIO_DAC_A1_A2, AUDIO_OUT_D_GAIN_m64_DB);
        audio_codec0_set_output_snr_opt(0);
        audio_tx_dma_dis(TLKDRV_CODEC_SPK_DMA);
    }

    return TLK_ENONE;
}

/**
 * @brief Configure the codec device
 * @param[in] subDev - Sub-device identifier
 * @param[in] opcode - Configuration operation code
 * @param[in] param0 - First parameter for configuration
 * @param[in] param1 - Second parameter for configuration
 * @return TLK_ENONE if operation succeeds, error code otherwise
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
 * @param[in] subDev - Sub-device identifier
 * @param[in] param0 - Channel count parameter
 * @param[in] param1 - Reserved parameter
 * @return TLK_ENONE if operation succeeds, error code otherwise
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
 * @param[in] subDev - Sub-device identifier
 * @param[in] param0 - Reserved parameter
 * @param[in] param1 - Reserved parameter
 * @return Current channel count
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
 * @param[in] subDev - Sub-device identifier
 * @param[in] param0 - Bit depth parameter
 * @param[in] param1 - Reserved parameter
 * @return TLK_ENONE if operation succeeds, error code otherwise
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
 * @param[in] subDev - Sub-device identifier
 * @param[in] param0 - Reserved parameter
 * @param[in] param1 - Reserved parameter
 * @return Current bit depth setting
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
 * @param[in] subDev - Sub-device identifier
 * @param[in] param0 - Sample rate parameter
 * @param[in] param1 - Reserved parameter
 * @return TLK_ENONE if operation succeeds, error code otherwise
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
 * @param[in] subDev - Sub-device identifier
 * @param[in] param0 - Reserved parameter
 * @param[in] param1 - Reserved parameter
 * @return Current sample rate setting
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
 * @brief ADC delay state machine handler
 * @param[in] pCtrl - Codec status control pointer
 * @return None
 */
static inline void tlkdrv_icodec_adcDelayStateMachine(tlkdrv_icodec_status_t *pCtrl)
{
    if (pCtrl->state == 2) {
        audio_codec0_set_input_mute(pCtrl->type, 0);

        pCtrl->delayMs = CODEC_STAT0_DELAY_US / 1000 + 1;
        pCtrl->state   = 1;
    } else if (pCtrl->state == 1) {
        pCtrl->state = 0;
    }
}

/**
 * @brief DAC delay state machine handler
 * @param[in] pCtrl - Codec status control pointer
 * @return None
 */
static inline void tlkdrv_icodec_dacDelayStateMachine(tlkdrv_icodec_status_t *pCtrl)
{
    if (pCtrl->state == 2) {
        audio_codec0_set_output_mute(pCtrl->type, 0);
        pCtrl->delayMs = CODEC_STAT0_DELAY_US / 1000 + 1;
        pCtrl->state   = 1;
    } else if (pCtrl->state == 1) {
        pCtrl->state = 0;
    }
}

/**
 * @brief Generic delay state machine handler
 * @param[in] pCtrl - Codec status control pointer
 * @return None
 */
static inline void tlkdrv_icodec_delayStateMachine(tlkdrv_icodec_status_t *pCtrl)
{
    if (pCtrl == &sTlkDrvIcodecAdcStat) {
        tlkdrv_icodec_adcDelayStateMachine(pCtrl);
    } else if (pCtrl == &sTlkDrvIcodecDacStat) {
        tlkdrv_icodec_dacDelayStateMachine(pCtrl);
    }
}

/**
 * @brief Delay timer callback function
 * @param[in] timerHandle - Timer handle
 * @param[in] arg - Argument pointer
 * @return None
 */
static void tlkdrv_icodec_delayTimer(TlkOsTimerHandle_t timerHandle, void *arg)
{
    tlkdrv_icodec_status_t *pCtrl     = (tlkdrv_icodec_status_t *)arg;
    uint32_t                timerType = tlkdrv_codec_timerHandle2type(timerHandle);
    pCtrl->delayMs -= pCtrl->timerOnceMs;
    if (pCtrl->delayMs <= 0) {
        pCtrl->delayMs = 0; //delay timeout: do state machine
        tlkdrv_icodec_delayStateMachine(pCtrl);
    }
    if (pCtrl->delayMs == 0) {
        pCtrl->timerOnceMs = 0;
        tlkdrv_codec_destroyTimer(timerType);
    } else {
        pCtrl->timerOnceMs = pCtrl->delayMs;
        tlkdrv_codec_startTimerOnce(timerType, pCtrl->delayMs, tlkdrv_icodec_delayTimer, pCtrl);
    }
}

/**
 * @brief Initialize codec status
 * @param[in] mode - Codec mode
 * @param[in] src - Source identifier
 * @return None
 */
static void codec_status_init(uint8_t mode, uint8_t src)
{
    if (mode & CODEC_DAC_STATUS) { ///SPK
        sTlkDrvIcodecDacStat.state = 2;
        sTlkDrvIcodecDacStat.delayMs += CODEC_STAT1_DELAY_US / 1000 + 1;
        sTlkDrvIcodecDacStat.type = src;
        if (sTlkDrvIcodecDacStat.timerOnceMs == 0) {
            sTlkDrvIcodecDacStat.timerOnceMs = sTlkDrvIcodecDacStat.delayMs;
            tlkdrv_codec_startTimerOnce(TLKDRV_CODEC_ADAPT_TIMER_DAC_DELAY, sTlkDrvIcodecDacStat.timerOnceMs, tlkdrv_icodec_delayTimer, &sTlkDrvIcodecDacStat);
        }
    } else if (mode & CODEC_ADC_STATUS) { ///MIC
        sTlkDrvIcodecAdcStat.type = src;
        if (src & 0x30) {
            sTlkDrvIcodecAdcStat.state = 2;
            sTlkDrvIcodecAdcStat.delayMs += CODEC_STAT1_DELAY_US / 1000 + 1;
        } else {
            audio_codec0_set_input_mute(sTlkDrvIcodecAdcStat.type, 0); /* adc un-mute. */
            sTlkDrvIcodecAdcStat.state = 1;
            sTlkDrvIcodecAdcStat.delayMs += CODEC_STAT0_DELAY_US / 1000 + 1;
        }
        if (sTlkDrvIcodecAdcStat.timerOnceMs == 0) {
            sTlkDrvIcodecAdcStat.timerOnceMs = sTlkDrvIcodecAdcStat.delayMs;
            tlkdrv_codec_startTimerOnce(TLKDRV_CODEC_ADAPT_TIMER_ADC_DELAY, sTlkDrvIcodecAdcStat.timerOnceMs, tlkdrv_icodec_delayTimer, &sTlkDrvIcodecAdcStat);
        }
    } else {
        tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "codec_status_init(Param Err):%x", mode);
        return;
    }
}

/**
 * @brief      This function serves to init codec0 input.
 * @param[in]  input_config - codec0 input config.
 * @return     none
 */
void audio_codec0_input_init1(audio_codec0_input_config_t *input_config)
{
    unsigned char channel    = input_config->input_src & BIT_RNG(0, 3);
    unsigned char input_type = 0; /* 0: analog data, 1: digital data. */

    audio_codec0_set_input_mute(input_config->input_src, 1); /* adc mute. */

    if (tl751x_codec_cur_power_mode) ///VAD mode
    {
        audio_codec0_set_adc_power_mode(AUDIO_AMIC_ADC_A1, tl751x_codec_cur_power_mode);
        audio_codec0_set_input_wl(AUDIO_AMIC_ADC_A1, AUDIO_CODEC0_BIT_16_DATA);
        audio_codec0_set_input_fs(AUDIO_AMIC_ADC_A1, AUDIO_16K);
        audio_codec0_input_hpf_en(AUDIO_AMIC_ADC_A1, 1);
    } else {
        audio_codec0_set_input_wl(input_config->input_src, input_config->data_format);
        audio_codec0_set_input_fs(input_config->input_src, input_config->sample_rate);
        audio_codec0_input_hpf_en(input_config->input_src, 1);
    }

    if (input_config->input_src & BIT(5)) /* amic enable bias and set differential input. */
    {
        input_type = 0;                                       /* analog data. */
        audio_codec0_set_micbias(input_config->input_src, 1); /* enable bias output. */
        audio_codec0_set_adc_capacitor_mode(input_config->input_src, AUDIO_CODEC0_ADC_CAPACITOR_COUPLED);
        audio_codec0_set_adc_mode(input_config->input_src, AUDIO_CODEC0_ADC_DIFFERENTIAL); /* adc differential input. */
        audio_codec0_set_input_snr_opt(input_config->input_src, 1);                        /* Avoid difference in ADC Sampling Data When a BDT active/reset occurs. */
    } else if (input_config->input_src & BIT(4))                                           /* line_in set differential input. */
    {
        input_type = 0; /* analog data. */
        audio_codec0_set_adc_capacitor_mode(input_config->input_src, AUDIO_CODEC0_ADC_CAPACITOR_COUPLED);
        audio_codec0_set_adc_mode(input_config->input_src, AUDIO_CODEC0_ADC_DIFFERENTIAL); /* adc differential input. */
        audio_codec0_set_input_snr_opt(input_config->input_src, 1);                        /* Avoid difference in ADC Sampling Data When a BDT active/reset occurs. */
    } else {
        input_type = 1;                                       /* digital data. */
        audio_codec0_dmic_clk_en(input_config->input_src, 1); /* enable dmic clock. */
    }

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                                       /* ADC_A1. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC0_MST_EN); /* audio adc A master en, codec adc A is slave. */
        if (0 == input_type)                                      /* analog data. */
        {
            BM_CLR(reg_audio_codec0_cr_adca12, FLD_CODEC0_SB_ADCA1); /* adc A1 channel active. */
        }
        reg_audio_codec0_cr_mic_adca_12_sel =
            (reg_audio_codec0_cr_mic_adca_12_sel & ~(FLD_CODEC0_ADCA1_MIC_SEL)) | MASK_VAL(FLD_CODEC0_ADCA1_MIC_SEL, input_type); /* select ADC_A1 input data type. */
        BM_CLR(reg_audio_codec0_cr_adca_ai_sb, FLD_CODEC0_SB_AICR_ADCA12);                                                        /* ADCA12 audio interface active. */
        break;
    case AUDIO_DMIC_ADC_A2:                                       /* ADC_A2. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC0_MST_EN); /* audio adc A master en, codec adc A is slave. */
        if (0 == input_type)                                      /* analog data. */
        {
            BM_CLR(reg_audio_codec0_cr_adca12, FLD_CODEC0_SB_ADCA2); /* adc A2 channel active. */
        }
        reg_audio_codec0_cr_mic_adca_12_sel =
            (reg_audio_codec0_cr_mic_adca_12_sel & ~(FLD_CODEC0_ADCA2_MIC_SEL)) | MASK_VAL(FLD_CODEC0_ADCA2_MIC_SEL, input_type); /* select ADC_A2 input data type. */
        BM_CLR(reg_audio_codec0_cr_adca_ai_sb, FLD_CODEC0_SB_AICR_ADCA12);                                                        /* ADCA12 audio interface active. */
        break;
    case AUDIO_DMIC_ADC_B1:                                       /* ADC_B1. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC1_MST_EN); /* audio adc B master en, codec adc B is slave. */
        if (0 == input_type)                                      /* analog data. */
        {
            BM_CLR(reg_audio_codec0_cr_adcb12, FLD_CODEC0_SB_ADCB1); /* adc B1 channel active. */
        } else if (1 == input_type)                                  /* digital data, set dmic1 clk(dmic0 clk default is 3072KHZ). */
        {
            audio_codec0_set_dmic_b_clk(AUDIO_CODEC0_DMIC_CLK_3072KHZ);
        }
        reg_audio_codec0_cr_mic_adcb_12_sel =
            (reg_audio_codec0_cr_mic_adcb_12_sel & ~(FLD_CODEC0_ADCB1_MIC_SEL)) | MASK_VAL(FLD_CODEC0_ADCB1_MIC_SEL, input_type); /* select ADC_B1 input data type. */
        BM_CLR(reg_audio_codec0_cr_adcb_ai_sb, FLD_CODEC0_SB_AICR_ADCB12);                                                        /* ADCB12 audio interface active. */
        break;
    case AUDIO_DMIC_ADC_B2:                                       /* ADC_B2. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC1_MST_EN); /* audio adc B master en, codec adc B is slave. */
        if (0 == input_type)                                      /* analog data. */
        {
            BM_CLR(reg_audio_codec0_cr_adcb12, FLD_CODEC0_SB_ADCB2); /* adc B2 channel active. */
        } else if (1 == input_type)                                  /* digital data, set dmic1 clk(dmic0 clk default is 3072KHZ). */
        {
            audio_codec0_set_dmic_b_clk(AUDIO_CODEC0_DMIC_CLK_3072KHZ);
        }
        reg_audio_codec0_cr_mic_adcb_12_sel =
            (reg_audio_codec0_cr_mic_adcb_12_sel & ~(FLD_CODEC0_ADCB2_MIC_SEL)) | MASK_VAL(FLD_CODEC0_ADCB2_MIC_SEL, input_type); /* select ADC_B1 input data type. */
        BM_CLR(reg_audio_codec0_cr_adcb_ai_sb, FLD_CODEC0_SB_AICR_ADCB12);                                                        /* ADCB12 audio interface active. */
        break;
    case AUDIO_DMIC_ADC_A1_A2:                                    /* ADC_A1_A2. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC0_MST_EN); /* audio adc A master en, codec adc A is slave. */
        if (0 == input_type)                                      /* analog data. */
        {
            reg_audio_codec0_cr_adca12 = (reg_audio_codec0_cr_adca12 & (~(FLD_CODEC0_SB_ADCA1 | FLD_CODEC0_SB_ADCA2))) |
                                         MASK_VAL(FLD_CODEC0_SB_ADCA1, 0, FLD_CODEC0_SB_ADCA2, 0); /* adc A1/A2 channel active. */
        }
        reg_audio_codec0_cr_mic_adca_12_sel = (reg_audio_codec0_cr_mic_adca_12_sel & ~((FLD_CODEC0_ADCA1_MIC_SEL | FLD_CODEC0_ADCA2_MIC_SEL))) |
                                              MASK_VAL(FLD_CODEC0_ADCA1_MIC_SEL, input_type, FLD_CODEC0_ADCA2_MIC_SEL, input_type); /* select ADC_A1/ADC_A2 input data type. */
        BM_CLR(reg_audio_codec0_cr_adca_ai_sb, FLD_CODEC0_SB_AICR_ADCA12);                                                          /* ADCA12 audio interface active. */
        break;
    case AUDIO_DMIC_ADC_B1_B2:                                    /* ADC_B1_B2. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC1_MST_EN); /* audio adc B master en, codec adc B is slave. */
        if (0 == input_type)                                      /* analog data. */
        {
            reg_audio_codec0_cr_adcb12 = (reg_audio_codec0_cr_adcb12 & (~(FLD_CODEC0_SB_ADCB1 | FLD_CODEC0_SB_ADCB2))) |
                                         MASK_VAL(FLD_CODEC0_SB_ADCB1, 0, FLD_CODEC0_SB_ADCB2, 0); /* adc B1/B2 channel active. */
        } else if (1 == input_type)                                                                /* digital data, set dmic1 clk(dmic0 clk default is 3072KHZ). */
        {
            audio_codec0_set_dmic_b_clk(AUDIO_CODEC0_DMIC_CLK_3072KHZ);
        }
        reg_audio_codec0_cr_mic_adcb_12_sel = (reg_audio_codec0_cr_mic_adcb_12_sel & ~(FLD_CODEC0_ADCB1_MIC_SEL | FLD_CODEC0_ADCB2_MIC_SEL)) |
                                              MASK_VAL(FLD_CODEC0_ADCB1_MIC_SEL, input_type, FLD_CODEC0_ADCB2_MIC_SEL, input_type); /* select ADC_B1 input data type. */
        BM_CLR(reg_audio_codec0_cr_adcb_ai_sb, FLD_CODEC0_SB_AICR_ADCB12);                                                          /* ADCB12 audio interface active. */
        break;
    default:
        break;
    }
}

/**
 * @brief Set DMIC digital gain
 * @param[in] dmic_d_gain - Gain value to set
 * @return 1 if successful, 0 if gain value is too large
 */
uint8_t tlkdrv_icodec_dmic_d_gain(uint8_t dmic_d_gain)
{
    if (dmic_d_gain > AUDIO_IN_D_GAIN_40_DB) {
        return 0;
    }
    audio_codec0_input_config_t inputParam;
    if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
        inputParam.input_src = AUDIO_DMIC_ADC_A1_A2;
    } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
        inputParam.input_src = AUDIO_DMIC_ADC_A2;
    } else {
        inputParam.input_src = AUDIO_DMIC_ADC_A1;
    }
    audio_codec0_set_input_dgain(inputParam.input_src, dmic_d_gain);
    return 1;
}
#if (TLKDRV_VOICE_MIC == TLKDRV_CODEC_MICA)
static int tlkdrv_icodec_mic_enable(bool enMic)
{
    uint8_t micSrc    = 0xFF;
    uint8_t micDWdith = 0xFF;
    int     micSRate  = 0xffffffff;

    tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_mic_enable:", &enMic, 1);

    if (enMic) {
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth == 16) {
            micDWdith = AUDIO_CODEC0_BIT_16_DATA;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth == 24) {
            micDWdith = AUDIO_CODEC0_BIT_24_DATA;
        }
        //        tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_MIC: <micDWdith> ",&micDWdith,1);

#if (CODEC_INPUT_MODE == CODEC_INPUT_LINEIN)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = AUDIO_LINEIN_ADC_A1_A2;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = AUDIO_LINEIN_ADC_A2;
        } else {
            micSrc = AUDIO_LINEIN_ADC_A1;
        }
#elif (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = AUDIO_AMIC_ADC_A1_A2;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = AUDIO_AMIC_ADC_A2;
        } else {
            micSrc = AUDIO_AMIC_ADC_A1;
        }
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = AUDIO_DMIC_ADC_A1_A2;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = AUDIO_DMIC_ADC_A2;
        } else {
            micSrc = AUDIO_DMIC_ADC_A1;
        }
        audio_codec0_set_dmic_a_pin((gpio_func_pin_e)TLKDRV_ICODEC_DMIC_DATA_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK0_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK1_PIN);
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
        audio_codec0_input_config_t inputParam;
        inputParam.input_src   = micSrc;
        inputParam.sample_rate = micSRate;
        inputParam.data_format = micDWdith;

        audio_codec0_input_init1(&inputParam);
        audio_codec0_set_input_snr_opt(inputParam.input_src, 0);

#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        if (g_sys_work_mode) //test mode
        {
            audio_codec0_set_input_again(inputParam.input_src, AUDIO_IN_A_GAIN_0_DB);
            audio_codec0_set_input_dgain(inputParam.input_src, AUDIO_IN_D_GAIN_0_DB);
            //			audio_codec0_set_input_snr_opt(inputParam.input_src,1);
        } else {
            audio_codec0_set_input_again(inputParam.input_src, AUDIO_IN_A_GAIN_8_DB);
            audio_codec0_set_input_dgain(inputParam.input_src, AUDIO_IN_D_GAIN_12_DB);
        }
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        audio_codec0_set_input_dgain(inputParam.input_src, AUDIO_IN_D_GAIN_24_DB);
#endif //#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)

        codec_status_init(CODEC_ADC_STATUS, inputParam.input_src);

        /* matrix input config. */
        unsigned char src_tmp = inputParam.input_src & 0x0f;
        if (inputParam.data_format == AUDIO_CODEC0_BIT_16_DATA) {
            if (src_tmp == AUDIO_DMIC_ADC_A1_A2) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCA, FIFO_RX_CODEC0_ADCA_A1_A2_16BIT);
            } else if (src_tmp == AUDIO_DMIC_ADC_A1) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCA, FIFO_RX_CODEC0_ADCA_A1_16BIT);
            } else if (src_tmp == AUDIO_DMIC_ADC_A2) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCA, FIFO_RX_CODEC0_ADCA_A2_16BIT);
            }
        } else {
            if (src_tmp == AUDIO_DMIC_ADC_A1_A2) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCA, FIFO_RX_CODEC0_ADCA_A1_A2_32BIT);
            } else if (src_tmp == AUDIO_DMIC_ADC_A1) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCA, FIFO_RX_CODEC0_ADCA_A1_32BIT);
            } else if (src_tmp == AUDIO_DMIC_ADC_A2) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCA, FIFO_RX_CODEC0_ADCA_A2_32BIT);
            }
        }

        /* rx dma init. */
        audio_rx_dma_chain_init(TLKDRV_CODEC_MIC_FIFO, TLKDRV_CODEC_MIC_DMA, (unsigned short *)gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen);
        reg_audio_clk_rst_en_h &= (~FLD_CLK_RST_MATRIX_EN);
        reg_audio_clk_rst_en_h |= FLD_CLK_RST_MATRIX_EN;
        audio_rx_dma_en(TLKDRV_CODEC_MIC_DMA); /* the rx dma enable must precede the adc enable. */
    }

    return TLK_ENONE;
}
#elif (TLKDRV_VOICE_MIC == TLKDRV_CODEC_MICB)
static int tlkdrv_icodec_mic_enable(bool enMic)
{
    uint8_t micSrc    = 0xFF;
    uint8_t micDWdith = 0xFF;
    int     micSRate  = 0xffffffff;

    tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_mic_enable:", &enMic, 1);

    if (enMic) {
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth == 16) {
            micDWdith = AUDIO_CODEC0_BIT_16_DATA;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.BitDepth == 24) {
            micDWdith = AUDIO_CODEC0_BIT_24_DATA;
        }

#if (CODEC_INPUT_MODE == CODEC_INPUT_LINEIN)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = AUDIO_LINEIN_ADC_B1_B2;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = AUDIO_LINEIN_ADC_B2;
        } else {
            micSrc = AUDIO_LINEIN_ADC_B1;
        }
#elif (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = AUDIO_AMIC_ADC_B1_B2;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = AUDIO_AMIC_ADC_B2;
        } else {
            micSrc = AUDIO_AMIC_ADC_B1;
        }
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x03) {
            micSrc = AUDIO_DMIC_ADC_B1_B2;
        } else if (sTlkDrvIcodecCtrl.codec_mic_cfg.Channel == 0x02) {
            micSrc = AUDIO_DMIC_ADC_B2;
        } else {
            micSrc = AUDIO_DMIC_ADC_B1;
        }
        audio_codec0_set_dmic_a_pin((gpio_func_pin_e)TLKDRV_ICODEC_DMIC_DATA_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK0_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK1_PIN);
#endif

        micSRate = audio_sample_rate_to_index(sTlkDrvIcodecCtrl.codec_mic_cfg.SampleRate);
        //      tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_MIC: <micSRate> ",&micSRate,4);

        if (gpTlkDrvCodecMicBuffer == NULL || gTlkDrvCodecMicBuffLen == 0 || micSrc == 0xFFU || micSRate == (int)0xffffffff || micDWdith == 0xFFU) {
            tlkapi_error(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "Param Err:gpTlkDrvCodecMicBuffer[%x],gTlkDrvCodecMicBuffLen[%x],micSrc[%x],micSRate[%x],micDWdith[%x]",
                         gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen, micSrc, micSRate, micDWdith);
            tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_Mic: enable_4 ", 0, 0);
            return -TLK_EPARAM;
        }
    }

    if (enMic && gpTlkDrvCodecMicBuffer != NULL && gTlkDrvCodecMicBuffLen != 0) {
        audio_codec0_input_config_t inputParam;
        inputParam.input_src   = micSrc;
        inputParam.sample_rate = micSRate;
        inputParam.data_format = micDWdith;

        audio_codec0_input_init1(&inputParam);
        audio_codec0_set_input_snr_opt(inputParam.input_src, 0);

#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        if (0) //test mode
        {
            audio_codec0_set_input_again(inputParam.input_src, AUDIO_IN_A_GAIN_0_DB);
            audio_codec0_set_input_dgain(inputParam.input_src, AUDIO_IN_D_GAIN_0_DB);
            //			audio_codec0_set_input_snr_opt(inputParam.input_src,1);
        } else {
            audio_codec0_set_input_again(inputParam.input_src, AUDIO_IN_A_GAIN_8_DB);
            audio_codec0_set_input_dgain(inputParam.input_src, AUDIO_IN_D_GAIN_12_DB);
        }
#elif (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        audio_codec0_set_input_dgain(inputParam.input_src, AUDIO_IN_D_GAIN_24_DB);
#endif //#if (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)

        codec_status_init(CODEC_ADC_STATUS, inputParam.input_src);

        /* matrix input config. */
        unsigned char src_tmp = inputParam.input_src & 0x0f;
        if (inputParam.data_format == AUDIO_CODEC0_BIT_16_DATA) {
            if (src_tmp == AUDIO_DMIC_ADC_B1_B2) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCB, FIFO_RX_CODEC0_ADCB_B1_B2_16BIT);
            } else if (src_tmp == AUDIO_DMIC_ADC_B1) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCB, FIFO_RX_CODEC0_ADCB_B1_16BIT);
            } else if (src_tmp == AUDIO_DMIC_ADC_B2) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCB, FIFO_RX_CODEC0_ADCB_B2_16BIT);
            }
        } else {
            if (src_tmp == AUDIO_DMIC_ADC_B1_B2) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCB, FIFO_RX_CODEC0_ADCB_B1_B2_32BIT);
            } else if (src_tmp == AUDIO_DMIC_ADC_B1) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCB, FIFO_RX_CODEC0_ADCB_B1_32BIT);
            } else if (src_tmp == AUDIO_DMIC_ADC_B2) {
                audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCB, FIFO_RX_CODEC0_ADCB_B2_32BIT);
            }
        }

        /* rx dma init. */
        audio_rx_dma_chain_init(TLKDRV_CODEC_MIC_FIFO, TLKDRV_CODEC_MIC_DMA, (unsigned short *)gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen);
        reg_audio_clk_rst_en_h &= (~FLD_CLK_RST_MATRIX_EN);
        reg_audio_clk_rst_en_h |= FLD_CLK_RST_MATRIX_EN;
        audio_rx_dma_en(TLKDRV_CODEC_MIC_DMA); /* the rx dma enable must precede the adc enable. */
    }

    return TLK_ENONE;
}
#endif

/**
 * @brief      This function serves to init codec0 output config.
 * @param[in]  output_config - codec0 output config.
 * @return     none
 */
void audio_codec0_output_init1(audio_codec0_output_config_t *output_config)
{
    audio_codec0_set_output_mute(output_config->output_dst, 1); /* dac mute. */
    audio_codec0_set_output_wl(output_config->output_dst, output_config->data_format);
    audio_codec0_set_output_fs(output_config->sample_rate);
    BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_DAC_MST_EN); /* audio dac master en, codec dac is slave. */

    reg_audio_codec_dac_cr_daca_ai = (reg_audio_codec_dac_cr_daca_ai & (~(FLD_CODEC_DAC_SB_AICR_DACA | FLD_CODEC_DACA_SLAVE))) |
                                     MASK_VAL(FLD_CODEC_DAC_SB_AICR_DACA, 0, FLD_CODEC_DACA_SLAVE, 1); /* DACA audio interface active and select slave. */

    switch (output_config->output_dst) {
    case AUDIO_DAC_A1:                                                 /* DAC_A1. */
        BM_CLR(reg_audio_codec_dac_cr_daca12, FLD_CODEC_DAC_SB_DACA1); /* DAC A1 active. */
        BM_CLR(reg_audio_codec_dac_cr_hpa, FLD_CODEC_DAC_SB_HPA1);     /* Headphone A1 output stage is active. */
        break;
    case AUDIO_DAC_A2:                                                 /* DAC_A2. */
        BM_CLR(reg_audio_codec_dac_cr_daca12, FLD_CODEC_DAC_SB_DACA2); /* DAC A2 active. */
        BM_CLR(reg_audio_codec_dac_cr_hpa, FLD_CODEC_DAC_SB_HPA2);     /* Headphone A2 output stage is active. */
        break;
    case AUDIO_DAC_A1_A2: /* DAC_A1_A2. */
        reg_audio_codec_dac_cr_daca12 = (reg_audio_codec_dac_cr_daca12 & (~(FLD_CODEC_DAC_SB_DACA1 | FLD_CODEC_DAC_SB_DACA2))) |
                                        MASK_VAL(FLD_CODEC_DAC_SB_DACA1, 0, FLD_CODEC_DAC_SB_DACA2, 0); /* DAC A1/A2 active. */
        reg_audio_codec_dac_cr_hpa = (reg_audio_codec_dac_cr_hpa & (~(FLD_CODEC_DAC_SB_HPA1 | FLD_CODEC_DAC_SB_HPA2))) |
                                     MASK_VAL(FLD_CODEC_DAC_SB_HPA1, 0, FLD_CODEC_DAC_SB_HPA2, 0); /* Headphone A1/A2 output stage is active. */
        break;
    default:
        break;
    }
}

/**
 * @brief Enable speaker functionality
 * @param[in] enSpk - Enable speaker
 * @return TLK_ENONE if operation succeeds, error code otherwise
 */
static int tlkdrv_icodec_spk_enable(bool enSpk)
{
    uint8_t spkSrc    = 0xFF;
    uint8_t spkDWdith = 0xFF;
    int     spkSRate  = 0xffffffff;

    tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_icodec_spk_enable:", &enSpk, 1);
    if (enSpk) {
        if (sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth == 16) {
            spkDWdith = AUDIO_CODEC0_BIT_16_DATA;
        } else if (sTlkDrvIcodecCtrl.codec_spk_cfg.BitDepth == 24) {
            spkDWdith = AUDIO_CODEC0_BIT_24_DATA;
        }
        //tlkapi_send_string_data(APP_LOG_EN,"tlkdrv_icodec_SPK: <spkDWdith> ",&spkDWdith,1);

        if (sTlkDrvIcodecCtrl.codec_spk_cfg.Channel == 0x03) {
            spkSrc = AUDIO_DAC_A1_A2;
        } else if (sTlkDrvIcodecCtrl.codec_spk_cfg.Channel == 0x02) {
            spkSrc = AUDIO_DAC_A2;
        } else {
            spkSrc = AUDIO_DAC_A1;
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
        audio_codec0_output_config_t outputParam;
        outputParam.output_dst  = spkSrc;
        outputParam.sample_rate = spkSRate;
        outputParam.data_format = spkDWdith;


        /* matrix output config. */
        if (outputParam.data_format == AUDIO_CODEC0_BIT_16_DATA) {
            if (outputParam.output_dst == AUDIO_DAC_A1_A2) {
                audio_matrix_set_dac_route(outputParam.output_dst, DAC_ROUTE_FIFO, DAC_FIFO_STEREO_16BIT_FIFO0);
            } else {
                audio_matrix_set_dac_route(outputParam.output_dst, DAC_ROUTE_FIFO, DAC_FIFO_MONO_16BIT_FIFO0);
            }
        } else {
            if (outputParam.output_dst == AUDIO_DAC_A1_A2) {
                audio_matrix_set_dac_route(outputParam.output_dst, DAC_ROUTE_FIFO, DAC_FIFO_STEREO_24BIT_FIFO0);
            } else {
                audio_matrix_set_dac_route(outputParam.output_dst, DAC_ROUTE_FIFO, DAC_FIFO_MONO_24BIT_FIFO0);
            }
        }


        audio_codec0_output_init1(&outputParam);
        audio_codec0_set_output_snr_opt(1);

#if (CODEC_CUSTOMERS_SPEC_REQ) ///-3db
        if (g_sys_work_mode)   //test mode
        {
            audio_codec0_set_output_again(outputParam.output_dst, AUDIO_OUT_A_GAIN_0_DB);
            audio_codec0_set_output_dgain(outputParam.output_dst, AUDIO_OUT_D_GAIN_0_DB);
        } else {
            audio_codec0_set_output_again(outputParam.output_dst, AUDIO_OUT_A_GAIN_0_DB);
            audio_codec0_set_output_dgain(outputParam.output_dst, AUDIO_OUT_D_GAIN_m4_DB);
        }
#else
        audio_codec0_set_output_again(outputParam.output_dst, AUDIO_OUT_A_GAIN_0_DB);
        audio_codec0_set_output_dgain(outputParam.output_dst, AUDIO_OUT_D_GAIN_0_DB);
#endif

        codec_status_init(CODEC_DAC_STATUS, outputParam.output_dst);


        /* tx dma init. */
        tlkapi_send_string_data(APP_LOG_EN, "[APP] audio_tx_dma_chain_init", &gTlkDrvCodecMicBuffLen, 4);
#if (AUDIO_CODEC_LOOPBACK)
        audio_tx_dma_chain_init(TLKDRV_CODEC_SPK_FIFO, TLKDRV_CODEC_SPK_DMA, (unsigned short *)gpTlkDrvCodecMicBuffer, gTlkDrvCodecMicBuffLen);
#else
        audio_tx_dma_chain_init(TLKDRV_CODEC_SPK_FIFO, TLKDRV_CODEC_SPK_DMA, (unsigned short *)gpTlkDrvCodecSpkBuffer, gTlkDrvCodecSpkBuffLen);
#endif
        reg_audio_clk_rst_en_h &= (~FLD_CLK_RST_MATRIX_EN);
        reg_audio_clk_rst_en_h |= FLD_CLK_RST_MATRIX_EN;
        audio_tx_dma_en(TLKDRV_CODEC_SPK_DMA);

        //        audio_dac_unmute();
    }

    return TLK_ENONE;
}

/*********************** ANC ****************************/
#if TLKALG_ANC_ENABLE && TLK_MW_DSP_COMM_ENABLE
#define ASRC_FS_IN  768000
#define ASRC_FS_OUT 768000
#define PPM         1

// wz_iir, 1 stage, 768kHz
int ava_wz_iir[24][5] = {
    {-168435455, 0, 0, 0, 0}, {0x10000000, 0, 0, 0, 0}, {0x10000000, 0, 0, 0, 0}, {0x10000000, 0, 0, 0, 0}, {0x10000000, 0, 0, 0, 0},
};

// wz_fir, 128/390 taps, 768kHz
signed short ava_wz_fir[390] = {
    92, 93, 94, 93, 92, 91, 88, 85, 82, 79, 76, 72, 69, 66, 64, 62, 60, 59, 59, 59, 60, 61, 62, 64, 67, 69, 72, 75, 77, 79, 81, 83, 84, 85, 85, 84, 84, 82, 80, 78, 76, 73, 70,
    67, 64, 61, 58, 56, 54, 52, 51, 51, 50, 50, 51, 52, 54, 55, 57, 60, 62, 64, 67, 69, 71, 73, 75, 76, 77, 78, 79, 79, 80, 80, 79, 79, 78, 78, 77, 76, 75, 74, 73, 72, 71, 70,
    69, 68, 67, 66, 65, 63, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 39, 38, 37, 37, 37, 38, 39, 41, 43, 45, 48, 51, 55, 58, 62, 65, 69, 72, 75, 78, 80, 82, 83, 84, 84,
    84, 83, 82, 80, 78, 75, 73, 70, 67, 65, 62, 60, 58, 56, 55, 54, 54, 54, 54, 55, 56, 57, 59, 61, 62, 64, 66, 67, 69, 69, 70, 70, 70, 69, 67, 65, 63, 61, 57, 54, 51, 47, 43,
    39, 36, 32, 29, 26, 23, 21, 19, 18, 17, 16, 16, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 28, 28, 28, 28, 28, 27, 26, 25, 23, 22, 20, 19, 17, 15, 14, 12, 11, 9,  8,
    7,  6,  5,  4,  3,  3,  2,  2,  1,  1,  1,  0,  0,  0,  0,  -1, -1, -1, -1, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,
    -4, -4, -4, -5, -5, -5, -5, -4, -4, -4, -3, -3, -2, -2, -1, 0,  1,  2,  2,  3,  4,  5,  6,  7,  7,  8,  8,  8,  8,  8,  8,  8,  7,  6,  5,  5,  4,  3,  2,  0,  -1, -2, -2,
    -3, -4, -5, -5, -5, -5, -5, -5, -5, -4, -4, -3, -2, -1, 0,  1,  2,  2,  3,  4,  5,  5,  6,  6,  6,  6,  6,  6,  5,  5,  4,  3,  2,  2,  1,  0,  -1, -2, -2, -3, -3, -4, -4,
    -4, -4, -4, -4, -4, -3, -3, -2, -1, 0,  1,  2,  3,  4,  4,  5,  6,  7,  8,  8,  8,  9,  9,  9,  9,  9,  9,  8,  8,  7,  7,  6,  5,  4,  4,  3,  2,  2,  1,  1};

signed short ava_wz_fir_trans[390] = {
    28,   33,   39,   45,   51,   57,   63,   69,   75,   80,   86,   90,   95,   99,   102,  105,  108,  110,  112,  114,  115,  116,  117,  118,  118,  119,  120,  122,
    123,  125,  128,  130,  133,  137,  140,  144,  149,  153,  157,  162,  166,  170,  174,  178,  181,  184,  186,  188,  189,  190,  191,  191,  191,  191,  191,  191,
    190,  190,  191,  191,  192,  194,  196,  198,  201,  205,  209,  213,  218,  223,  228,  234,  239,  243,  248,  252,  256,  259,  261,  262,  263,  263,  263,  262,
    260,  257,  254,  251,  248,  244,  240,  237,  233,  230,  227,  224,  222,  220,  219,  218,  217,  217,  216,  216,  216,  215,  214,  213,  212,  209,  207,  203,
    199,  195,  189,  183,  177,  170,  163,  156,  148,  141,  133,  126,  118,  112,  105,  99,   94,   89,   85,   81,   77,   74,   71,   68,   65,   62,   59,   55,
    51,   46,   40,   34,   27,   19,   11,   2,    -8,   -19,  -30,  -41,  -53,  -65,  -77,  -89,  -100, -112, -123, -133, -143, -153, -162, -170, -178, -185, -193, -199,
    -206, -212, -219, -225, -232, -239, -246, -254, -262, -271, -280, -290, -300, -310, -321, -331, -342, -353, -364, -374, -385, -394, -404, -412, -420, -428, -435, -441,
    -447, -452, -456, -460, -464, -467, -471, -474, -477, -480, -484, -487, -491, -496, -501, -506, -511, -517, -523, -529, -535, -541, -547, -552, -558, -563, -568, -572,
    -575, -578, -581, -583, -584, -585, -585, -585, -584, -583, -582, -580, -579, -577, -575, -574, -572, -571, -570, -569, -568, -567, -566, -565, -563, -562, -560, -557,
    -555, -552, -548, -543, -538, -533, -527, -520, -512, -504, -496, -487, -478, -469, -459, -450, -440, -430, -421, -411, -402, -393, -384, -376, -367, -359, -350, -342,
    -333, -324, -315, -306, -297, -287, -277, -266, -255, -244, -232, -220, -208, -196, -184, -172, -159, -147, -136, -124, -113, -102, -92,  -82,  -72,  -63,  -54,  -45,
    -36,  -28,  -19,  -11,  -2,   7,    16,   26,   36,   47,   58,   69,   81,   93,   106,  118,  130,  143,  155,  167,  178,  189,  199,  208,  217,  225,  232,  238,
    244,  250,  255,  260,  264,  269,  274,  280,  286,  292,  300,  308,  317,  327,  338,  349,  361,  374,  386,  398,  410,  421,  431,  440,  446,  451,  453,  453,
    450,  444,  435,  423,  408,  391,  370,  347,  322,  295,  267,  238,  208,  178,  148,  119,  91,   65,   41,   19};

// cz_iir, 4 stages, 768kHz
int ava_cz_iir[24][5] = {{43382098, -82085258, 43382098, -519261786, 261765041},
                         {52254459, -96453510, 52254459, -507964658, 247584611},
                         {9194574, -14494036, 9194574, -497962403, 233422061},
                         {25369785, 25369785, 0, -246732049, 0},
                         {0x10000000, 0, 0, 0, 0}};
// cz_fir, 1/150 taps, 768kHz
signed short ava_cz_fir[150] = {16384, 0};

int wz_iir_bypass[24][5] = {
    {0x10000000, 0, 0, 0, 0}, {0x10000000, 0, 0, 0, 0}, {0x10000000, 0, 0, 0, 0}, {0x10000000, 0, 0, 0, 0}, {0x10000000, 0, 0, 0, 0},
};

short wz_fir390_bypass[390] = {
    //390
    16384, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

signed short drop_coef_anc[9] = {0, 0, 0, 0, 1, -4, 12, -58, 2100};
extern int   anc_mic_buff[ANC_RECORD_BUFF_SIZE];
extern int   anc_spk_buff[ANC_PLAY_BUFF_SIZE];

/**
 * @brief Enable ANC (Active Noise Cancellation) feature
 * @param[in] music - Music mode flag
 * @return None
 */
void tlkdrv_icodec_anc_enable(bool music)
{
    sTlkDrvIcodecCtrl.codec_spk_cfg.IsEn   = true;
    sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen = true;

    // sTlkDrvIcodecCtrl.codec_mic_cfg.IsEn = true;
    // sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen = true;

    audio_anc_clk_en(ANC0);
    audio_anc_set_mode(ANC0, ANC_MODE_HB);

    // resample setting
    if (music) {
        audio_anc_set_adder2_mode(ANC0, ANC_ERR_MIC_MINUS_CZ1_TO_ADDR2);
        audio_anc_set_adder3_mode(ANC0, ANC_WZ_PLUS_RESAMPLE_TO_HEADPHONE);
        audio_anc_set_adder3_priority(ANC0, ANC_WZ_FIRST);

        audio_matrix_set_anc_src_route(ANC0, ANC_SRC_ROUTE_FIFO, ANC_SRC_DATA_FIFO0); //fifo choose
        audio_anc_set_resample_in_out_fs(ANC0, ANC_RESAMPLE_DAC_DECISION_FS, ANC_RESAMPLE_IN_FS_48K, ANC_RESAMPLE_OUT_FS_768K);
        //resample buffer, note bufferlen == 0????, DMA choose
        audio_tx_dma_chain_init(TLKDRV_CODEC_SPK_FIFO, TLKDRV_CODEC_SPK_DMA, (unsigned short *)gpTlkDrvCodecSpkBuffer, gTlkDrvCodecSpkBuffLen);
        audio_tx_dma_en(TLKDRV_CODEC_SPK_DMA);
    } else {
        audio_anc_set_adder2_mode(ANC0, ANC_ERR_MIC_TO_ADDR2);
        audio_anc_set_adder3_mode(ANC0, ANC_WZ_TO_HEADPHONE);
        audio_anc_set_adder3_priority(ANC0, ANC_WZ_FIRST);

        audio_anc_set_resample_out_fs(ANC0, ANC_RESAMPLE_OUT_FS_768K);
    }

    /***********************anc filter config****************************/
    audio_anc_set_wz_iir_taps(ANC0, 5);
    audio_anc_update_wz_iir_coef(ANC0, wz_iir_bypass, 5);
    audio_anc_set_wz_fir_taps(ANC0, 390);
    audio_anc_update_wz_fir_coef(ANC0, ava_wz_fir, 390);

    audio_anc_set_cz_iir_taps(ANC0, 0, 5);
    audio_anc_update_cz_iir_coef(ANC0, 0, ava_cz_iir, 5);
    audio_anc_set_cz_fir_taps(ANC0, 0, 150);
    audio_anc_update_cz_fir_coef(ANC0, 0, ava_cz_fir, 150);

    //cz1
    // audio_anc_set_cz_iir_taps(ANC0, 1, 5);
    // audio_anc_update_cz_iir_coef(ANC0, 1, wz_iir_bypass, 5);
    // audio_anc_set_cz_fir_taps(ANC0, 1, 0x96);// fir_taps=150
    // audio_anc_update_cz_fir_coef(ANC0, 1, wz_fir390_bypass, 0x96);

    audio_anc_set_ref_mic_gain(ANC0, 0x4000, 14);
    audio_anc_set_wz_fir_gain(ANC0, 0x4000, 14);

    audio_anc_latch_wz_fir_gain(ANC0);
    audio_anc_latch_ref_mic_gain(ANC0);

    //err_mic&ref_mic -> fifo
#if (TLKDRV_ANC_MIC == TLKDRV_CODEC_MICA)
    audio_matrix_set_anc_ref_route(ANC0, ANC_REF_ROUTE_CODEC0_ADCA, ANC_REF_ADC_LEFT_32_BIT);
    audio_matrix_set_anc_err_route(ANC0, ANC_ERR_ROUTE_CODEC0_ADCA, ANC_ERR_ADC_RIGHT_32_BIT);
#elif (TLKDRV_ANC_MIC == TLKDRV_CODEC_MICB)
    audio_matrix_set_anc_ref_route(ANC0, ANC_REF_ROUTE_CODEC0_ADCB, ANC_REF_ADC_LEFT_32_BIT);
    audio_matrix_set_anc_err_route(ANC0, ANC_ERR_ROUTE_CODEC0_ADCB, ANC_ERR_ADC_RIGHT_32_BIT);
#endif
    audio_matrix_set_rx_fifo_route(TLKDRV_ANC0_MIC_FIFO, FIFO_RX_ROUTE_ANC0, FIFO_RX_ANC_POST_PRE_32BIT);

    /***********************hac config****************************/
    audio_hac_clk_en(HAC_CHN0);
    audio_hac_set_data_src(HAC_CHN0, HAC_INPUT_DATA_MATRIX);
    audio_hac_set_data_dst(HAC_CHN0, HAC_OUTPUT_DATA_MATRIX);

    audio_hac_set_in_data_rate(HAC_CHN0, 36864000 / 768000 / 1);  /* 36.864MHz/FS_IN/CH_NUM. */
    audio_hac_set_out_data_rate(HAC_CHN0, 36864000 / 768000 / 1); /* 36.864MHz/FS_OUT/CH_NUM. */
    audio_hac_asrc_fs_in_out(HAC_CHN0, ASRC_FS_IN, ASRC_FS_OUT, PPM, 1);
    audio_asrc_set_droop_step((audio_asrc_chn_e)HAC_CHN0, ASRC_DROOP_STEP_17TAPS);
    audio_asrc_update_droop_coef((audio_asrc_chn_e)HAC_CHN0, drop_coef_anc, sizeof(drop_coef_anc) / sizeof(drop_coef_anc[0]));
    audio_hac_bypass_eq(HAC_CHN0); /* Bypass the EQ function by configuring the specified parameters. */

    audio_matrix_set_hac_route(HAC_CHN0, HAC_DATA_ROUTE_ANC0_SPEAKER, HAC_DATA_FORMAT_INVALID); //anc out ----> hac
    // if ((HAC_CHN0 == HAC_CHN2) || (HAC_CHN0 == HAC_CHN3)) {
    //     audio_hac_set_tdm_num(HAC_CHN0, 1);
    // }
    audio_hac_asrc_ch_en(HAC_CHN0, 1);
    audio_hac_bypass_asrc(HAC_CHN0, 1);

    /***********************sidetone config****************************/
    audio_side_tone_dma_en(SIDE_TONE_CHN67);
    audio_side_tone_ch_en(SIDE_TONE_CHN6);
    audio_side_tone_set_dac_gain(SIDE_TONE_CHN6, SIDETONE_0DB);
    audio_side_tone_set_adc_gain(SIDE_TONE_CHN6, SIDETONE_0DB);
    audio_side_tone_set_req_count(SIDE_TONE_CHN67, 36864000, 768000, 1);
    audio_matrix_set_side_tone_route(SIDE_TONE_CHN6, SIDE_TONE_ROUTE_HAC_DATA0, SIDE_TONE_DATA_FORMAT_INVALID);

    /*********************** adc config ****************************/
    audio_codec0_input_config_t codec0_input_config = {
#if (TLKDRV_ANC_MIC == TLKDRV_CODEC_MICA)
        .input_src = AUDIO_AMIC_ADC_A1_A2,
#elif (TLKDRV_ANC_MIC == TLKDRV_CODEC_MICB)
        .input_src = AUDIO_AMIC_ADC_B1_B2,
#endif
        .data_format = AUDIO_CODEC0_BIT_24_DATA,
        .sample_rate = AUDIO_768K,
    };
    audio_codec0_input_init(&codec0_input_config);
    audio_codec0_set_input_filter_mode(codec0_input_config.input_src, LOW_LATENCY_FILTER_MODE);
    audio_codec0_set_input_again(codec0_input_config.input_src, AUDIO_IN_A_GAIN_0_DB);
    audio_codec0_set_input_dgain(codec0_input_config.input_src, AUDIO_IN_D_GAIN_0_DB);

    /*********************** dac config ****************************/
    audio_codec0_output_config_t codec0_output_config = {
        .output_dst  = AUDIO_DAC_A1_A2,
        .data_format = AUDIO_CODEC0_BIT_24_DATA,
        .sample_rate = AUDIO_768K,
    };
    audio_codec0_output_init(&codec0_output_config);
    audio_codec0_set_output_filter_mode(LOW_LATENCY_FILTER_MODE);

    audio_matrix_set_dac_route(AUDIO_DAC_A1, DAC_ROUTE_FIFO, DAC_FIFO_MONO_24BIT_FIFO1);
    audio_tx_dma_chain_init(TLKDRV_ANC0_SPK_FIFO, TLKDRV_ANC0_SPK_DMA, (unsigned short *)anc_spk_buff, sizeof(anc_spk_buff));
    // audio_tx_dma_en(TLKDRV_ANC0_SPK_DMA);
    audio_rx_dma_chain_init(TLKDRV_ANC0_MIC_FIFO, TLKDRV_ANC0_MIC_DMA, (unsigned short *)anc_mic_buff, sizeof(anc_mic_buff));
    reg_audio_clk_rst_en_h &= (~FLD_CLK_RST_MATRIX_EN);
    reg_audio_clk_rst_en_h |= FLD_CLK_RST_MATRIX_EN;
    // audio_rx_dma_en(TLKDRV_ANC0_MIC_DMA);

    audio_codec0_set_output_again(codec0_output_config.output_dst, AUDIO_OUT_A_GAIN_0_DB);
    audio_codec0_set_output_dgain(codec0_output_config.output_dst, AUDIO_OUT_D_GAIN_0_DB);
}

/**
 * @brief Disable ANC (Active Noise Cancellation) feature
 * @param None
 * @return None
 */
void tlkdrv_icodec_anc_disable(void)
{
    sTlkDrvIcodecCtrl.codec_spk_cfg.IsEn   = false;
    sTlkDrvIcodecCtrl.codec_spk_cfg.IsOpen = false;

    sTlkDrvIcodecCtrl.codec_mic_cfg.IsEn   = false;
    sTlkDrvIcodecCtrl.codec_mic_cfg.IsOpen = false;

    audio_side_tone_ch_dis(SIDE_TONE_CHN6);
}
#endif

#if AUDIO_HD_HAC_EN
uint32_t     tl751x_audio_freq_table[10] = {16000, 24000, 32000, 44100, 48000, 96000, 192000, 384000, 768000, 0xffffffff};
signed short drop_coef[9]                = {0, 0, 0, 0, 1, -4, 12, -58, 2100};

tlkdrv_asrc_t        g_hac_asrc_spk_st;
tlkdrv_asrc_config_t g_hac_asrc_spk_para;

void *g_hd_hac_asrc_buffer = NULL;

volatile int hac_init = 0;

/**
 * @brief Set HAC ASRC input/output frequencies
 * @param[in] p_asrc - Pointer to HAC configuration structure
 * @return None
 */
void audio_hac_asrc_fs_in_out_setting(tlkdrv_hac_config_t *p_asrc)
{
    int  frac_advance = 0;
    int  den_rate     = 0;
    char int_advance  = 0;

    int fs_in  = p_asrc->hac_input_freq;
    int fs_out = p_asrc->hac_output_freq;
    int ppm    = p_asrc->asrc_para.asrc_ppm;

    if (fs_in == fs_out) {
        den_rate     = 1000000;
        frac_advance = 1000000 + ppm * 16;
        int_advance  = 15;
    } else if ((fs_in == 32000 && fs_out == 16000) || (fs_in == 96000 && fs_out == 48000)) {
        den_rate     = 1000000;
        frac_advance = 1000000 + ppm * 32;
        int_advance  = 31;
    } else if ((fs_in == 48000 && fs_out == 16000) || (fs_in == 96000 && fs_out == 32000)) {
        den_rate     = 1000000;
        frac_advance = 1000000 + ppm * 48;
        int_advance  = 47;
    } else if (fs_in == 96000 && fs_out == 16000) {
        den_rate     = 1000000;
        frac_advance = 1000000 + ppm * 96;
        int_advance  = 95;
    } else if ((fs_in == 16000 && fs_out == 32000) || (fs_in == 48000 && fs_out == 96000)) {
        den_rate     = 1000000;
        frac_advance = 1000000 + ppm * 8;
        int_advance  = 7;
    } else if (fs_in == 48000 && fs_out == 32000) {
        den_rate     = 1000000;
        frac_advance = 1000000 + ppm * 24;
        int_advance  = 23;
    } else if ((fs_in == 16000 && fs_out == 48000) || (fs_in == 32000 && fs_out == 96000)) {
        den_rate     = 3000000;
        frac_advance = 1000000 + ppm * 16;
        int_advance  = 5;
    } else if (fs_in == 32000 && fs_out == 48000) {
        den_rate     = 3000000;
        frac_advance = 2000000 + ppm * 32;
        int_advance  = 10;
    } else if (fs_in == 16000 && fs_out == 96000) {
        den_rate     = 3000000;
        frac_advance = 2000000 + ppm * 8;
        int_advance  = 2;
    } else if (fs_in == 44100 && fs_out == 16000) {
        den_rate     = 10000000;
        frac_advance = 1000000 + ppm * 441;
        int_advance  = 44;
    } else if (fs_in == 44100 && fs_out == 32000) {
        den_rate     = 20000000;
        frac_advance = 1000000 + ppm * 441;
        int_advance  = 22;
    } else if (fs_in == 44100 && fs_out == 48000) {
        den_rate     = 10000000;
        frac_advance = 7000000 + ppm * 147;
        int_advance  = 14;
    } else if (fs_in == 44100 && fs_out == 96000) {
        den_rate     = 20000000;
        frac_advance = 7000000 + ppm * 147;
        int_advance  = 7;
    } else if (fs_in == 16000 && fs_out == 44100) {
        den_rate     = 1378125; /* 459375 * 3 */
        frac_advance = 1109375 + ppm * 8;
        int_advance  = 5;
    }

    else if (fs_in == 32000 && fs_out == 44100) {
        den_rate     = 1378125; /* 459375 * 3 */
        frac_advance = 840625 + ppm * 16;
        int_advance  = 11;
    }

    else if (fs_in == 48000 && fs_out == 44100) {
        den_rate     = 459375;
        frac_advance = 190625 + ppm * 8;
        int_advance  = 17;
    }

    else if (fs_in == 96000 && fs_out == 44100) {
        den_rate     = 459375;
        frac_advance = 381250 + ppm * 16;
        int_advance  = 34;
    }

    p_asrc->asrc_para.den_rate     = den_rate;
    p_asrc->asrc_para.frac_advance = frac_advance;
    p_asrc->asrc_para.int_advance  = int_advance;
}

/**
 * @brief Optimize HAC ASRC input/output frequencies
 * @param[in] p_asrc - Pointer to HAC configuration structure
 * @return None
 */
void audio_hac_asrc_fs_in_out_opt(tlkdrv_hac_config_t *p_asrc)
{
    audio_hac_asrc_fs_in_out_setting(p_asrc);

#if (AUDIO_HD_HAC_MODE == AUDIO_HD_HAC_INDEPENDENT) ////Hardware ASRC acceleration
    audio_hac_set_interval(p_asrc->hac_chn, 192000, p_asrc->hac_count);
#else
    audio_hac_set_interval(p_asrc->hac_chn, p_asrc->hac_input_freq, p_asrc->hac_count);
#endif

    audio_hac_set_frac_adc(p_asrc->hac_chn, p_asrc->asrc_para.frac_advance);
    audio_hac_set_den_rate(p_asrc->hac_chn, p_asrc->asrc_para.den_rate);
    audio_hac_set_int_adv(p_asrc->hac_chn, p_asrc->asrc_para.int_advance);
    audio_hac_lag_int_config_done(p_asrc->hac_chn);
}

/**
 * @brief Set PPM value for HAC configuration
 * @param[in] p_asrc - Pointer to HAC configuration structure
 * @param[in] ppm - PPM value to set
 * @return None
 */
void tlkdrv_hac_set_ppm(tlkdrv_hac_config_t *p_asrc, int ppm)
{
    static int old_ppm = 0;
    if (abs_ram(ppm) <= 500) {
        if (old_ppm != ppm) {
            p_asrc->asrc_para.asrc_ppm        = ppm;
            p_asrc->asrc_para.ppm_updata_flag = 1;
            old_ppm                           = ppm;
        } else {
            //    		tlkapi_info(TLKDRV_CODEC_DBG_FLAG,
            //						 TLKDRV_CODEC_DBG_SIGN,
            //						 "tlkdrv_hac_set_ppm:PPM is not updated: %d %d",
            //						 ppm,
            //						 old_ppm);
            return;
        }

    } else {
        tlkapi_info(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_hac_set_ppm2: ERR(PPM>500)or(PPM<-500)) %d", ppm);
    }
}

/**
 * @brief Get PPM value from HAC configuration
 * @param[in] p_asrc - Pointer to HAC configuration structure
 * @return Current PPM value
 */
int tlkdrv_hac_get_ppm(tlkdrv_hac_config_t *p_asrc)
{
    return p_asrc->asrc_para.asrc_ppm;
}

int tlkdrv_hac_set_sample(tlkdrv_hac_config_t *p_asrc, uint32_t in_sample, uint32_t out_sample)
{
    uint8_t count = sizeof(tl751x_audio_freq_table) / sizeof(uint32_t);

    if (!(in_sample && out_sample)) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkdrv_hac_set_sample error: Invalid Parameters %d %d", in_sample, out_sample);
        return -TLK_EPARAM;
    }

    for (uint8_t i = 0; i < count; i++) {
        if (tl751x_audio_freq_table[i] == in_sample) {
            p_asrc->hac_input_freq         = in_sample;
            p_asrc->asrc_para.asrc_in_rate = st_audio_sys_clk / p_asrc->hac_input_freq / p_asrc->hac_count;
            break;
        } else if (tl751x_audio_freq_table[i] == 0xffffffff) {
            tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_hac_set_sample : ERR:in_sample", &in_sample, 4);
            return -TLK_EPARAM;
        }
    }

    for (uint8_t i = 0; i < count; i++) {
        if (tl751x_audio_freq_table[i] == out_sample) {
            p_asrc->hac_output_freq         = out_sample;
            p_asrc->asrc_para.asrc_out_rate = st_audio_sys_clk / p_asrc->hac_output_freq / p_asrc->hac_count;
            break;
        } else if (tl751x_audio_freq_table[i] == 0xffffffff) {
            tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_hac_set_sample : ERR:out_sample", &out_sample, 4);
            return -TLK_EPARAM;
        }
    }

    return TLK_ENONE;
}

/**
 * @brief Set HAC parameters
 * @param[in] hac_asrc_st - Pointer to HAC configuration structure
 * @param[in] fifo_chn - FIFO channel
 * @param[in] in_dma - Input DMA channel
 * @param[in] out_dma - Output DMA channel
 * @param[in] chn - HAC channel
 * @param[in] tdm_ch - TDM channel
 * @param[in] bit_width - Bit width
 * @param[in] in_src - Input source buffer
 * @param[in] out_des - Output destination buffer
 * @param[in] packet_length - Packet length
 * @return None
 */
void tlk_hac_para_setting(tlkdrv_hac_config_t *hac_asrc_st, audio_fifo_chn_e fifo_chn, dma_chn_e in_dma, dma_chn_e out_dma, audio_hac_chn_e chn, unsigned char tdm_ch,
                          audio_codec0_data_select_e bit_width, signed short *in_src, signed short *out_des, uint16_t packet_length)
{
    hac_asrc_st->hac_chn                   = chn;
    hac_asrc_st->hac_count                 = tdm_ch;
    hac_asrc_st->input_para.asrc_fifo      = fifo_chn;
    hac_asrc_st->input_para.asrc_in_dma    = in_dma;
    hac_asrc_st->input_para.asrc_out_dma   = out_dma;
    hac_asrc_st->input_para.asrc_bit_width = bit_width;
    hac_asrc_st->input_para.data_len       = packet_length;
    hac_asrc_st->input_para.in_addr        = in_src;
    hac_asrc_st->input_para.out_addr       = out_des;
}

/**
 * @brief Configure HAC settings
 * @param[in] hac_asrc_st - Pointer to HAC configuration structure
 * @return None
 */
void tlkdrv_hac_setting(tlkdrv_hac_config_t *hac_asrc_st)
{
    if (hac_asrc_st == NULL) {
        tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_hac_setting:Invalid Parameters ", 0, 0);
        return;
    }

    ///HAC status settings
    //    tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_hac_setting:hac_chn ", &hac_asrc_st->hac_chn, 1);
    audio_hac_clk_en(hac_asrc_st->hac_chn);

    audio_asrc_set_droop_step(hac_asrc_st->hac_chn, ASRC_DROOP_STEP_17TAPS);
    audio_asrc_update_droop_coef(hac_asrc_st->hac_chn, drop_coef, sizeof(drop_coef) / sizeof(drop_coef[0]));
    audio_hac_bypass_eq(hac_asrc_st->hac_chn); /* Bypass the EQ function by configuring the specified parameters. */

    audio_hac_set_data_src(hac_asrc_st->hac_chn, HAC_INPUT_DATA_MATRIX);
    audio_hac_set_data_dst(hac_asrc_st->hac_chn, HAC_OUTPUT_DATA_MATRIX);

//    tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_hac_setting:asrc_in_rate ", &hac_asrc_st->asrc_para.asrc_in_rate, 2);
#if (AUDIO_HD_HAC_MODE == AUDIO_HD_HAC_INDEPENDENT) ////Hardware ASRC acceleration
    hac_asrc_st->asrc_para.asrc_in_rate = st_audio_sys_clk / 192000 / hac_asrc_st->hac_count;
    audio_hac_set_in_data_rate(hac_asrc_st->hac_chn, hac_asrc_st->asrc_para.asrc_in_rate);
    audio_hac_set_out_data_rate(hac_asrc_st->hac_chn, 1);
#else
    {
        audio_hac_set_in_data_rate(hac_asrc_st->hac_chn, hac_asrc_st->asrc_para.asrc_in_rate);
        audio_hac_set_out_data_rate(hac_asrc_st->hac_chn, hac_asrc_st->asrc_para.asrc_out_rate);
    }
#endif

    ///HAC routing settings
    if (hac_asrc_st->input_para.asrc_bit_width == AUDIO_CODEC0_BIT_16_DATA) {
        audio_matrix_set_hac_route(hac_asrc_st->hac_chn, HAC_DATA_ROUTE_FIFO0 + hac_asrc_st->input_para.asrc_fifo, HAC_16_BIT);                        /* hac route from adc. */
        audio_matrix_set_rx_fifo_route(hac_asrc_st->input_para.asrc_fifo, FIFO_RX_ROUTE_HAC_DATA0 + hac_asrc_st->hac_chn, FIFO_RX_HAC23_STEREO_16BIT); /* rx fifo route from hac. */
    } else {
        audio_matrix_set_hac_route(hac_asrc_st->hac_chn, HAC_DATA_ROUTE_FIFO0 + hac_asrc_st->input_para.asrc_fifo, HAC_20_OR_24_BIT); /* hac route from adc. */
        audio_matrix_set_rx_fifo_route(hac_asrc_st->input_para.asrc_fifo, FIFO_RX_ROUTE_HAC_DATA0 + hac_asrc_st->hac_chn,
                                       FIFO_RX_HAC23_STEREO_20_OR_24BIT); /* rx fifo route from hac. */
    }

    ///HAC ASRC Conversion Settings
#if (AUDIO_HD_HAC_MODE == AUDIO_HD_HAC_INDEPENDENT) ////Hardware ASRC acceleration
    audio_hac_asrc_fs_in_out_opt(hac_asrc_st);
#else
    audio_hac_asrc_fs_in_out(hac_asrc_st->hac_chn, hac_asrc_st->hac_input_freq, hac_asrc_st->hac_output_freq, hac_asrc_st->asrc_para.asrc_ppm, hac_asrc_st->hac_count);
#endif
    audio_hac_set_tdm_tx_dma_ch(hac_asrc_st->input_para.asrc_fifo, HAC_TDM_TX_DMA_2CH);
    audio_hac_set_tdm_num(hac_asrc_st->hac_chn, 2);
    audio_hac_asrc_ch_en(hac_asrc_st->hac_chn, 1);
}

/**
 * @brief Get TX FIFO count for HAC
 * @param[in] fifo_chn - FIFO channel
 * @return FIFO count value
 */
unsigned int audio_hac_get_txfifo_cnt1(audio_fifo_chn_e fifo_chn)
{
    unsigned int  fifo_cnt = 0;
    unsigned char times_cn = 0;

    BM_SET(reg_audio_hac_tx_fifo_cnt_ind(fifo_chn), FLD_HAC_TX_FIFO_INDICATE); /* Write 1 means software need to read txfifo_cnt */

    while (BM_IS_CLR(reg_audio_hac_tx_fifo_cnt_ind(fifo_chn), FLD_HAC_TX_FIFO_INDICATE)) {
        times_cn++;
        delay_us(1);
        if (times_cn > 5) {
            tlkapi_info(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "audio_hac_get_txfifo_cnt1: %d", times_cn);
            break;
        }
    }

    fifo_cnt = reg_audio_hac_tx_fifo_cnt_ind(fifo_chn) & FLD_HAC_TX_FIFO_CNT;
    BM_CLR(reg_audio_hac_tx_fifo_cnt_ind(fifo_chn), FLD_HAC_TX_FIFO_INDICATE); /* clear tx data done */

    return fifo_cnt;
}

/**
 * @brief Get buffer size needed for HAC processing
 * @param[in] input_samp - Input sample rate
 * @param[in] output_samp - Output sample rate
 * @param[in] frame_len - Frame length
 * @param[in] data_width - Data width
 * @return Required buffer size
 */
int tlkdrv_hac_get_size(int input_samp, int output_samp, int frame_len, unsigned char data_width)
{
    unsigned char data_type = 0;

    if (!(input_samp && output_samp && frame_len)) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkdrv_hac_get_size error: Invalid Parameters");
        return -TLK_EPARAM;
    }

    if (data_width == 16) {
        data_type = sizeof(short) * 2;
    } else if (data_width == 24 || data_width == 20 || data_width == 32) {
        data_type = sizeof(int) * 2;
    } else {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkdrv_hac_get_size error: Invalid data_width");
        return -TLK_EPARAM;
    }

    int hac_struct_size = (sizeof(tlkdrv_hac_config_t) + 7) / 8 * 8;
    int input_size      = (frame_len * data_type + 7) / 8 * 8;
    int output_size     = (input_size * output_samp / input_samp + data_type * 0x10 + 7) / 8 * 8;

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ASRC size??input_size %d??output_size %d", input_size, output_size);
    return (hac_struct_size + input_size + output_size);
}

/**
 * @brief Initialize HAC module
 * @param[in] t_hac_st - Pointer to HAC configuration structure
 * @param[in] sysclk - System clock frequency
 * @param[in] input_sample - Input sample rate
 * @param[in] output_sample - Output sample rate
 * @param[in] frame_len - Frame length
 * @param[in] data_width - Data width
 * @return None
 */
void tlkdrv_hac_init(tlkdrv_hac_config_t *t_hac_st, uint32_t sysclk, int input_sample, int output_sample, int frame_len, unsigned char data_width)
{
    unsigned char data_type          = 0;
    unsigned char hac_data_width_tmp = 0;
    hac_init                         = 0;

    if (!((input_sample) && (output_sample) && (frame_len) && (data_width))) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, " tlkdrv_hac_init:***init failed*** [%d]", input_sample);
        return;
    }


    if (st_audio_sys_clk != sysclk) {
        st_audio_sys_clk = sysclk;
    }

    if (data_width == 16) {
        data_type          = sizeof(short) * 2;
        hac_data_width_tmp = AUDIO_CODEC0_BIT_16_DATA;
    } else if (data_width == 24 || data_width == 20 || data_width == 32) {
        data_type          = sizeof(int) * 2;
        hac_data_width_tmp = AUDIO_CODEC0_BIT_24_DATA;
    }

    int hac_struct_size = (sizeof(tlkdrv_hac_config_t) + 7) / 8 * 8;
    int input_size      = (frame_len * data_type + 7) / 8 * 8;

    ///System parameter settings
    if (t_hac_st) {
        tlk_hac_para_setting(t_hac_st,
                             FIFO2,    ///HAC ASRC audio FIFO
                             DMA10,    ///HAC ASRC input DMA
                             DMA11,    ///HAC ASRC output DMA
                             HAC_CHN2, ///HAC ASRC channel
                             2,        ///STEREO
                             hac_data_width_tmp,
                             (signed short *)((signed char *)t_hac_st + hac_struct_size),              ///Input data buffer
                             (signed short *)((signed char *)t_hac_st + hac_struct_size + input_size), ///Output data buffer
                             frame_len                                                                 ///frame length
        );
    }
    //	else
    //	{
    //		tlk_hac_para_setting(
    //			t_hac_st,
    //			FIFO3,                           ///HAC ASRC audio FIFO
    //			DMA14,                            ///HAC ASRC input DMA
    //			DMA15,                            ///HAC ASRC output DMA
    //			HAC_CHN3,                        ///HAC ASRC channel
    //			2,                               ///STEREO
    //			(signed short *)hac_input_buff,  ///Input data buffer
    //			(signed short *)hac_output_buff, ///Output data buffer
    //			frame_len                		///frame length
    //		);
    //	}


    tlkdrv_hac_set_sample(t_hac_st, input_sample, output_sample);
    tlkdrv_hac_setting(t_hac_st);
    hac_init = 1;

    tlkapi_info(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "tlkdrv_hac_init: %x,%d %d %d %d", t_hac_st, sysclk, input_sample, output_sample, frame_len);
}

/**
 * @brief Deinitialize HAC module
 * @param[in] hac_asrc_st - Pointer to HAC configuration structure
 * @return None
 */
void tlkdrv_hac_deinit(tlkdrv_hac_config_t *hac_asrc_st)
{
    audio_tx_dma_dis(hac_asrc_st->input_para.asrc_in_dma);
    audio_rx_dma_dis(hac_asrc_st->input_para.asrc_out_dma);

    audio_hac_clear_fifo_cnt(hac_asrc_st->hac_chn, 1);

    hac_init = 0;
}

/**
 * @brief Process HAC data
 * @param[in] hac_asrc_st - Pointer to HAC configuration structure
 * @param[in] in_buff - Input buffer
 * @param[in] out_buff - Output buffer
 * @param[in] input_num - Number of input samples
 * @return Number of output samples
 */
uint16_t tlkdrv_hac_process(tlkdrv_hac_config_t *hac_asrc_st, int *in_buff, int *out_buff, int input_num)
{
    static int        first_hd_frame_flag = 0;
    unsigned char     hac_data_width_tmp  = 0;
    float             tmp                 = (float)hac_asrc_st->hac_output_freq / hac_asrc_st->hac_input_freq;
    volatile uint16_t out_len             = (uint16_t)(tmp * hac_asrc_st->input_para.data_len + 0.5);
    volatile uint16_t alg_hac_fifo_len    = audio_hac_get_txfifo_cnt1(hac_asrc_st->input_para.asrc_fifo) / hac_asrc_st->hac_count;

    if (!hac_init) {
        first_hd_frame_flag = 0;
        return out_len      = 0;
    }
    if (first_hd_frame_flag && alg_hac_fifo_len) {
        if (alg_hac_fifo_len != out_len) {
            //			tlkapi_info(TLKDRV_CODEC_DBG_FLAG,
            //						 TLKDRV_CODEC_DBG_SIGN,
            //						 "tlkdrv_hac_process4: %d %d %d",
            //						 reg_audio_dma_tx_rptr(hac_asrc_st->input_para.asrc_fifo),
            //						 alg_hac_fifo_len,
            //						 out_len);
            out_len = (abs_ram(alg_hac_fifo_len - out_len) < 2) ? alg_hac_fifo_len : out_len;
        }
    } else {
        first_hd_frame_flag = 1;
    }

    audio_tx_dma_dis(hac_asrc_st->input_para.asrc_in_dma);
    audio_rx_dma_dis(hac_asrc_st->input_para.asrc_out_dma);
    audio_hac_clear_fifo_cnt(hac_asrc_st->hac_chn, 1);

    if (hac_asrc_st->input_para.asrc_bit_width == AUDIO_CODEC0_BIT_16_DATA) {
        hac_data_width_tmp = 4;
    } else {
        hac_data_width_tmp = 8;
    }

    tlkmdi_audio_enterCritical();
    tmemcpy((void *)hac_asrc_st->input_para.in_addr, (const void *)in_buff, input_num * hac_data_width_tmp);
    tmemcpy((void *)out_buff, (const void *)hac_asrc_st->input_para.out_addr, out_len * hac_data_width_tmp);
    if (hac_asrc_st->asrc_para.ppm_updata_flag) {
        audio_hac_asrc_fs_in_out_opt(hac_asrc_st);
        hac_asrc_st->asrc_para.ppm_updata_flag = 0;
    }
    tlkmdi_audio_leaveCritical();

    hac_asrc_st->input_para.data_len = input_num;

    /* rx dma config */
    audio_clear_fifo((AUDIO_RX_FIFO0 << hac_asrc_st->input_para.asrc_fifo), 1);
    audio_rx_dma_chain_init(hac_asrc_st->input_para.asrc_fifo, hac_asrc_st->input_para.asrc_out_dma, (unsigned short *)hac_asrc_st->input_para.out_addr,
                            (out_len + 1) * hac_data_width_tmp);
    reg_dma_llp(hac_asrc_st->input_para.asrc_out_dma)        = 0; /* dis llp */
    reg_audio_dma_rx_max(hac_asrc_st->input_para.asrc_fifo)  = 0xffff;
    reg_audio_dma_rx_wptr(hac_asrc_st->input_para.asrc_fifo) = 0x0000;
    audio_clear_fifo((AUDIO_RX_FIFO0 << hac_asrc_st->input_para.asrc_fifo), 0);
    audio_fifo_ptr_en((AUDIO_RX_FIFO0 << hac_asrc_st->input_para.asrc_fifo));


    /* tx dma config */
    audio_clear_fifo((AUDIO_TX_FIFO0 << hac_asrc_st->input_para.asrc_fifo), 1);
    audio_tx_dma_chain_init(hac_asrc_st->input_para.asrc_fifo, hac_asrc_st->input_para.asrc_in_dma, (unsigned short *)hac_asrc_st->input_para.in_addr,
                            hac_asrc_st->input_para.data_len * hac_data_width_tmp);
    reg_dma_llp(hac_asrc_st->input_para.asrc_in_dma)         = 0; /* dis llp */
    reg_audio_dma_tx_max(hac_asrc_st->input_para.asrc_fifo)  = 0xffff;
    reg_audio_dma_tx_rptr(hac_asrc_st->input_para.asrc_fifo) = 0x0000;
    audio_clear_fifo((AUDIO_TX_FIFO0 << hac_asrc_st->input_para.asrc_fifo), 0);
    audio_fifo_ptr_en((AUDIO_TX_FIFO0 << hac_asrc_st->input_para.asrc_fifo));
    audio_hac_clear_fifo_cnt(hac_asrc_st->hac_chn, 0);


    audio_rx_dma_en(hac_asrc_st->input_para.asrc_out_dma);
    audio_tx_dma_en(hac_asrc_st->input_para.asrc_in_dma);
    // DBG_COMMON_CHN3_TOGGLE;


    return out_len;
}
#endif //#if AUDIO_HD_HAC_EN

#endif // #if (MCU_CORE_TYPE == MCU_CORE_TL751X && TLKDRV_CODEC_ICODEC_ENABLE)
