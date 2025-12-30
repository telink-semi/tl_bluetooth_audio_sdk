/********************************************************************************************************
 * @file    audio.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
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
/** @page AUDIO
 *
 *  API Reference
 *  ===============
 *  Header File: audio.h
 */

#ifndef __AUDIO_H_
#define __AUDIO_H_

#include "lib/include/analog.h"
#include "gpio.h"
#include "dma.h"

/**********************************************************************************************************************
 *                                                Audio anc enum/struct                                               *
 *********************************************************************************************************************/
/*!
 * @name Audio anc enum/struct.
 * @{
 */

/**
 * @brief ANC channel.
 * 
 */
typedef enum
{
    ANC0 = 0x00,
    ANC1 = 1,
} audio_anc_chn_e;

/**
 * @brief ANC mode.
 * 
 */
typedef enum
{
    ANC_MODE_FF = 0x00,
    ANC_MODE_FB,
    ANC_MODE_HB,
} audio_anc_mode_e;

typedef enum
{
    ANC_48K_IN_768K_OUT = 0x00,
    ANC_96K_IN_768K_OUT = 0x01,
    ANC_48K_IN_384K_OUT = 0x01,
    ANC_96K_IN_384K_OUT = 0x02,
} audio_anc_dac_cnt_mode_e;

/**
 * @brief ANC resample input fs.
 * 
 */
typedef enum
{
    ANC_RESAMPLE_IN_FS_48K,
    ANC_RESAMPLE_IN_FS_96K,
} audio_anc_resample_in_fs_e;

/**
 * @brief ANC resample output fs.
 * 
 */
typedef enum
{
    ANC_RESAMPLE_OUT_FS_768K,
    ANC_RESAMPLE_OUT_FS_384K,
} audio_anc_resample_out_fs_e;

/**
 * @brief ANC adder3 output mode select.
 *
 */
typedef enum
{
    ANC_ERR_MIC_MINUS_CZ1_TO_ADDR2, /**< err mic - cz1. */
    ANC_ERR_MIC_TO_ADDR2,           /**< err mic only. */
} audio_anc_set_adder2_mode_e;

/**
 * @brief ANC adder3 output mode select.
 *
 */
typedef enum
{
    ANC_WZ_PLUS_RESAMPLE_TO_HEADPHONE, /**< wz + resample. */
    ANC_WZ_TO_HEADPHONE,               /**< wz only. */
} audio_anc_set_adder3_mode_e;

/**
 * @brief ANC adder3 output data priority.
 *
 */
typedef enum
{
    ANC_WZ_FIRST,
    ANC_RESAMPLE_FIRST,
} audio_anc_adder3_out_pri_e;

/**
 * @brief ANC resample output fs decision.
 *
 */
typedef enum
{
    ANC_RESAMPLE_DAC_DECISION_FS,
    ANC_RESAMPLE_OTHERS_DECISION_FS, /**< i2s_rx, dac and hac, etc.*/
} audio_anc_resample_fs_decision_e;

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio asrc enum/struct                                              *
 *********************************************************************************************************************/
/*!
 * @name Audio asrc enum/struct.
 * @{
 */

/**
 * @brief ASRC channel.
 * 
 */
typedef enum
{
    ASRC0 = 0x00,
    ASRC1,
    ASRC2,
    ASRC3,
} audio_asrc_chn_e;

/**
 * @brief ASRC droop step.
 * 
 */
typedef enum
{
    ASRC_DROOP_STEP_9TAPS,
    ASRC_DROOP_STEP_13TAPS,
    ASRC_DROOP_STEP_17TAPS,
} audio_asrc_droop_step_e;

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio codec enum/struct                                             *
 *********************************************************************************************************************/
/*!
 * @name Audio codec enum/struct.
 * @{
 */

/**
 * @brief codec0 power on mode.
 *
 */
typedef enum
{
    AUDIO_CODEC0_DIGITAL_ONLY = 0x0e,
    AUDIO_CODEC0_ADC_ONLY     = 0x08,
    AUDIO_CODEC0_DAC_ONLY     = 0x04,
    AUDIO_CODEC0_ADC_AND_DAC,
    AUDIO_CODEC0_DIGITAL_AND_DAC,
} audio_codec0_power_e;

/**
 * @brief codec1 power on mode.
 *
 */
typedef enum
{
    AUDIO_CODEC1_DIGITAL_ONLY = 0x00,
} audio_codec1_power_e;

/**
 * @brief ADC power mode.
 *
 */
typedef enum
{
    AUDIO_CODEC0_ADC_NORMAL_POWER,
    AUDIO_CODEC0_ADC_LOW_POWER,
    AUDIO_CODEC0_ADC_ULTRA_LOW_POWER,
} audio_codec0_adc_power_mode_e;

/**
 * @brief codec0 input select.
 *
 */
typedef enum
{
    AUDIO_DMIC_ADC_A1    = 0x00, /**< bit[0-3] channel. */
    AUDIO_DMIC_ADC_A2    = 0x01,
    AUDIO_DMIC_ADC_B1    = 0x02,
    AUDIO_DMIC_ADC_B2    = 0x03,
    AUDIO_DMIC_ADC_A1_A2 = 0x04,
    AUDIO_DMIC_ADC_B1_B2 = 0x05,

    AUDIO_LINEIN_ADC_A1    = 0x00 | BIT(4), /**< bit4 line_in. */
    AUDIO_LINEIN_ADC_A2    = 0x01 | BIT(4),
    AUDIO_LINEIN_ADC_B1    = 0x02 | BIT(4),
    AUDIO_LINEIN_ADC_B2    = 0x03 | BIT(4),
    AUDIO_LINEIN_ADC_A1_A2 = 0x04 | BIT(4),
    AUDIO_LINEIN_ADC_B1_B2 = 0x05 | BIT(4),

    /**
     * If all the bias are connected together (BGA-packaged chip), it is recommended to configure the other unused bias to high resistance
     * when a bias is being used to prevent current leakage. By default, the codec0 micbias has been configured to high-impedance after powered on if codec0 amic is used.
     */
    AUDIO_AMIC_ADC_A1    = 0x00 | BIT(5), /**< bit5: amic. */
    AUDIO_AMIC_ADC_A2    = 0x01 | BIT(5),
    AUDIO_AMIC_ADC_B1    = 0x02 | BIT(5),
    AUDIO_AMIC_ADC_B2    = 0x03 | BIT(5),
    AUDIO_AMIC_ADC_A1_A2 = 0x04 | BIT(5),
    AUDIO_AMIC_ADC_B1_B2 = 0x05 | BIT(5),
} audio_codec0_input_select_e;

/**
 * @brief codec0 micbias output state when Microphone bias in power-down (reg_audio_codec0_cr_adc_mic[5], SB_MICBIAS = 1)
 * @note  If all the bias are connected together (BGA-packaged chip), it is recommended to configure the other unused bias to high resistance
 *        when a bias is being used to prevent current leakage. By default, the codec0 micbias has been configured to high-impedance after powered on if codec0 amic is used.
 *        - bias only for amic.
 */
typedef enum
{
    AUDIO_AMIC_BIAS_PULL_DOWN_ZERO,
    AUDIO_AMIC_BIAS_HIGH_IMPEDANCE,
} audio_codec0_bias_output_state_e;

/**
 * @brief codec1 input select.
 *
 */
typedef enum
{
    AUDIO_CODEC1_DMIC_A1,
    AUDIO_CODEC1_DMIC_A2,
    AUDIO_CODEC1_DMIC_A1_A2,
} audio_codec1_input_select_e;

/**
 * @brief codec0 output channel.
 *
 */
typedef enum
{
    AUDIO_DAC_A1,
    AUDIO_DAC_A2,
    AUDIO_DAC_A1_A2,
} audio_codec0_output_select_e;

/**
 * @brief codec0 voltage supply.
 *
 */
typedef enum
{
    AUDIO_CODEC0_3P3V = 0, /**< Normal Voltage operating 3.3V */
    AUDIO_CODEC0_1P8V = 1, /**< Low Voltage operating 1.8V */
} audio_codec0_volt_supply_e;

/**
 * @brief codec0 adc capacitor mode selection.
 *
 */
typedef enum
{
    AUDIO_CODEC0_ADC_CAPACITOR_LESS,
    AUDIO_CODEC0_ADC_CAPACITOR_COUPLED,
} audio_codec0_adc_capacitor_mode_e;

/**
 * @brief codec0 adc mode selection.
 *
 */
typedef enum
{
    AUDIO_CODEC0_ADC_SINGLE_ENDED,
    AUDIO_CODEC0_ADC_DIFFERENTIAL,
} audio_codec0_adc_mode_e;

/**
 * @brief codec0 dmic clock selection.
 *
 */
typedef enum
{
    AUDIO_CODEC0_DMIC_CLK_3072KHZ = 0x03, /**< DMIC_CLK frequency = 3.072 MHz */
    AUDIO_CODEC0_DMIC_CLK_1024KHZ = 0x0b, /**< DMIC_CLK frequency = 1.024 MHz */
    AUDIO_CODEC0_DMIC_CLK_768KHZ  = 0x0f, /**< DMIC_CLK frequency = 0.768 MHz */
} audio_codec_dmic_clk_sel_e;

/**
 * @brief codec0 input channel analog gain, [-2dB, 24dB], 2dB steps.
 */
typedef enum
{
    AUDIO_IN_A_GAIN_m2_DB, /**< -2dB */
    AUDIO_IN_A_GAIN_0_DB,  /**< 0dB */
    AUDIO_IN_A_GAIN_2_DB,
    AUDIO_IN_A_GAIN_4_DB,
    AUDIO_IN_A_GAIN_6_DB,
    AUDIO_IN_A_GAIN_8_DB,
    AUDIO_IN_A_GAIN_10_DB,
    AUDIO_IN_A_GAIN_12_DB,
    AUDIO_IN_A_GAIN_14_DB,
    AUDIO_IN_A_GAIN_16_DB,
    AUDIO_IN_A_GAIN_18_DB,
    AUDIO_IN_A_GAIN_20_DB,
    AUDIO_IN_A_GAIN_22_DB,
    AUDIO_IN_A_GAIN_24_DB, /**< 24dB */
} audio_codec0_input_again_e;

/**
 * @brief codec0/1 input channel digital gain, [-64dB, 63dB], 1dB steps.
 * @note
 *       - When SNR-Optimizer is activated, ADC digital gain range is limited to [-64dB, +35dB] range.
 */
typedef enum
{
    AUDIO_IN_D_GAIN_m64_DB = 0x40, /**< -64dB */
    AUDIO_IN_D_GAIN_m63_DB,        /**< -63dB */
    AUDIO_IN_D_GAIN_m62_DB,
    AUDIO_IN_D_GAIN_m61_DB,
    AUDIO_IN_D_GAIN_m60_DB,
    AUDIO_IN_D_GAIN_m59_DB,
    AUDIO_IN_D_GAIN_m58_DB,
    AUDIO_IN_D_GAIN_m57_DB,
    AUDIO_IN_D_GAIN_m56_DB,
    AUDIO_IN_D_GAIN_m55_DB,
    AUDIO_IN_D_GAIN_m54_DB,
    AUDIO_IN_D_GAIN_m53_DB,
    AUDIO_IN_D_GAIN_m52_DB,
    AUDIO_IN_D_GAIN_m51_DB,
    AUDIO_IN_D_GAIN_m50_DB,
    AUDIO_IN_D_GAIN_m49_DB,
    AUDIO_IN_D_GAIN_m48_DB,
    AUDIO_IN_D_GAIN_m47_DB,
    AUDIO_IN_D_GAIN_m46_DB,
    AUDIO_IN_D_GAIN_m45_DB,
    AUDIO_IN_D_GAIN_m44_DB,
    AUDIO_IN_D_GAIN_m43_DB,
    AUDIO_IN_D_GAIN_m42_DB,
    AUDIO_IN_D_GAIN_m41_DB,
    AUDIO_IN_D_GAIN_m40_DB,
    AUDIO_IN_D_GAIN_m39_DB,
    AUDIO_IN_D_GAIN_m38_DB,
    AUDIO_IN_D_GAIN_m37_DB,
    AUDIO_IN_D_GAIN_m36_DB,
    AUDIO_IN_D_GAIN_m35_DB,
    AUDIO_IN_D_GAIN_m34_DB,
    AUDIO_IN_D_GAIN_m33_DB,
    AUDIO_IN_D_GAIN_m32_DB,
    AUDIO_IN_D_GAIN_m31_DB,
    AUDIO_IN_D_GAIN_m30_DB,
    AUDIO_IN_D_GAIN_m29_DB,
    AUDIO_IN_D_GAIN_m28_DB,
    AUDIO_IN_D_GAIN_m27_DB,
    AUDIO_IN_D_GAIN_m26_DB,
    AUDIO_IN_D_GAIN_m25_DB,
    AUDIO_IN_D_GAIN_m24_DB,
    AUDIO_IN_D_GAIN_m23_DB,
    AUDIO_IN_D_GAIN_m22_DB,
    AUDIO_IN_D_GAIN_m21_DB,
    AUDIO_IN_D_GAIN_m20_DB,
    AUDIO_IN_D_GAIN_m19_DB,
    AUDIO_IN_D_GAIN_m18_DB,
    AUDIO_IN_D_GAIN_m17_DB,
    AUDIO_IN_D_GAIN_m16_DB,
    AUDIO_IN_D_GAIN_m15_DB,
    AUDIO_IN_D_GAIN_m14_DB,
    AUDIO_IN_D_GAIN_m13_DB,
    AUDIO_IN_D_GAIN_m12_DB,
    AUDIO_IN_D_GAIN_m11_DB,
    AUDIO_IN_D_GAIN_m10_DB,
    AUDIO_IN_D_GAIN_m9_DB,
    AUDIO_IN_D_GAIN_m8_DB,
    AUDIO_IN_D_GAIN_m7_DB,
    AUDIO_IN_D_GAIN_m6_DB,
    AUDIO_IN_D_GAIN_m5_DB,
    AUDIO_IN_D_GAIN_m4_DB,
    AUDIO_IN_D_GAIN_m3_DB,
    AUDIO_IN_D_GAIN_m2_DB,
    AUDIO_IN_D_GAIN_m1_DB,       /**< -1dB */

    AUDIO_IN_D_GAIN_0_DB = 0x00, /**< 0dB */
    AUDIO_IN_D_GAIN_1_DB,        /**< 1dB */
    AUDIO_IN_D_GAIN_2_DB,
    AUDIO_IN_D_GAIN_3_DB,
    AUDIO_IN_D_GAIN_4_DB,
    AUDIO_IN_D_GAIN_5_DB,
    AUDIO_IN_D_GAIN_6_DB,
    AUDIO_IN_D_GAIN_7_DB,
    AUDIO_IN_D_GAIN_8_DB,
    AUDIO_IN_D_GAIN_9_DB,
    AUDIO_IN_D_GAIN_10_DB,
    AUDIO_IN_D_GAIN_11_DB,
    AUDIO_IN_D_GAIN_12_DB,
    AUDIO_IN_D_GAIN_13_DB,
    AUDIO_IN_D_GAIN_14_DB,
    AUDIO_IN_D_GAIN_15_DB,
    AUDIO_IN_D_GAIN_16_DB,
    AUDIO_IN_D_GAIN_17_DB,
    AUDIO_IN_D_GAIN_18_DB,
    AUDIO_IN_D_GAIN_19_DB,
    AUDIO_IN_D_GAIN_20_DB,
    AUDIO_IN_D_GAIN_21_DB,
    AUDIO_IN_D_GAIN_22_DB,
    AUDIO_IN_D_GAIN_23_DB,
    AUDIO_IN_D_GAIN_24_DB,
    AUDIO_IN_D_GAIN_25_DB,
    AUDIO_IN_D_GAIN_26_DB,
    AUDIO_IN_D_GAIN_27_DB,
    AUDIO_IN_D_GAIN_28_DB,
    AUDIO_IN_D_GAIN_29_DB,
    AUDIO_IN_D_GAIN_30_DB,
    AUDIO_IN_D_GAIN_31_DB,
    AUDIO_IN_D_GAIN_32_DB,
    AUDIO_IN_D_GAIN_33_DB,
    AUDIO_IN_D_GAIN_34_DB,
    AUDIO_IN_D_GAIN_35_DB,
    AUDIO_IN_D_GAIN_36_DB,
    AUDIO_IN_D_GAIN_37_DB,
    AUDIO_IN_D_GAIN_38_DB,
    AUDIO_IN_D_GAIN_39_DB,
    AUDIO_IN_D_GAIN_40_DB,
    AUDIO_IN_D_GAIN_41_DB,
    AUDIO_IN_D_GAIN_42_DB,
    AUDIO_IN_D_GAIN_43_DB,
    AUDIO_IN_D_GAIN_44_DB,
    AUDIO_IN_D_GAIN_45_DB,
    AUDIO_IN_D_GAIN_46_DB,
    AUDIO_IN_D_GAIN_47_DB,
    AUDIO_IN_D_GAIN_48_DB,
    AUDIO_IN_D_GAIN_49_DB,
    AUDIO_IN_D_GAIN_50_DB,
    AUDIO_IN_D_GAIN_51_DB,
    AUDIO_IN_D_GAIN_52_DB,
    AUDIO_IN_D_GAIN_53_DB,
    AUDIO_IN_D_GAIN_54_DB,
    AUDIO_IN_D_GAIN_55_DB,
    AUDIO_IN_D_GAIN_56_DB,
    AUDIO_IN_D_GAIN_57_DB,
    AUDIO_IN_D_GAIN_58_DB,
    AUDIO_IN_D_GAIN_59_DB,
    AUDIO_IN_D_GAIN_60_DB,
    AUDIO_IN_D_GAIN_61_DB,
    AUDIO_IN_D_GAIN_62_DB,
    AUDIO_IN_D_GAIN_63_DB, /**< 63dB */
} audio_codec_input_dgain_e;

/**
 * @brief codec0 output channel analog gain, [-28dB, 6dB], 2dB steps.
 */
typedef enum
{
    AUDIO_OUT_A_GAIN_m28_DB, /**< -28dB */
    AUDIO_OUT_A_GAIN_m26_DB, /**< -26dB */
    AUDIO_OUT_A_GAIN_m24_DB,
    AUDIO_OUT_A_GAIN_m22_DB,
    AUDIO_OUT_A_GAIN_m20_DB,
    AUDIO_OUT_A_GAIN_m18_DB,
    AUDIO_OUT_A_GAIN_m16_DB,
    AUDIO_OUT_A_GAIN_m14_DB,
    AUDIO_OUT_A_GAIN_m12_DB,
    AUDIO_OUT_A_GAIN_m10_DB,
    AUDIO_OUT_A_GAIN_m8_DB,
    AUDIO_OUT_A_GAIN_m6_DB,
    AUDIO_OUT_A_GAIN_m4_DB,
    AUDIO_OUT_A_GAIN_m2_DB,
    AUDIO_OUT_A_GAIN_0_DB,
    AUDIO_OUT_A_GAIN_2_DB,
    AUDIO_OUT_A_GAIN_4_DB,
    AUDIO_OUT_A_GAIN_6_DB,
} audio_codec0_output_again_e;

/**
 * @brief codec0 output channel digital gain, [-64dB, 63dB], 1dB steps.
 * @note
 *       - When SNR-Optimizer is activated, DAC digital gain range is limited to [-64dB, +35dB] range.
 */
typedef enum
{
    AUDIO_OUT_D_GAIN_m64_DB = 0x40, /**< -64dB */
    AUDIO_OUT_D_GAIN_m63_DB,        /**< -63dB */
    AUDIO_OUT_D_GAIN_m62_DB,
    AUDIO_OUT_D_GAIN_m61_DB,
    AUDIO_OUT_D_GAIN_m60_DB,
    AUDIO_OUT_D_GAIN_m59_DB,
    AUDIO_OUT_D_GAIN_m58_DB,
    AUDIO_OUT_D_GAIN_m57_DB,
    AUDIO_OUT_D_GAIN_m56_DB,
    AUDIO_OUT_D_GAIN_m55_DB,
    AUDIO_OUT_D_GAIN_m54_DB,
    AUDIO_OUT_D_GAIN_m53_DB,
    AUDIO_OUT_D_GAIN_m52_DB,
    AUDIO_OUT_D_GAIN_m51_DB,
    AUDIO_OUT_D_GAIN_m50_DB,
    AUDIO_OUT_D_GAIN_m49_DB,
    AUDIO_OUT_D_GAIN_m48_DB,
    AUDIO_OUT_D_GAIN_m47_DB,
    AUDIO_OUT_D_GAIN_m46_DB,
    AUDIO_OUT_D_GAIN_m45_DB,
    AUDIO_OUT_D_GAIN_m44_DB,
    AUDIO_OUT_D_GAIN_m43_DB,
    AUDIO_OUT_D_GAIN_m42_DB,
    AUDIO_OUT_D_GAIN_m41_DB,
    AUDIO_OUT_D_GAIN_m40_DB,
    AUDIO_OUT_D_GAIN_m39_DB,
    AUDIO_OUT_D_GAIN_m38_DB,
    AUDIO_OUT_D_GAIN_m37_DB,
    AUDIO_OUT_D_GAIN_m36_DB,
    AUDIO_OUT_D_GAIN_m35_DB,
    AUDIO_OUT_D_GAIN_m34_DB,
    AUDIO_OUT_D_GAIN_m33_DB,
    AUDIO_OUT_D_GAIN_m32_DB,
    AUDIO_OUT_D_GAIN_m31_DB,
    AUDIO_OUT_D_GAIN_m30_DB,
    AUDIO_OUT_D_GAIN_m29_DB,
    AUDIO_OUT_D_GAIN_m28_DB,
    AUDIO_OUT_D_GAIN_m27_DB,
    AUDIO_OUT_D_GAIN_m26_DB,
    AUDIO_OUT_D_GAIN_m25_DB,
    AUDIO_OUT_D_GAIN_m24_DB,
    AUDIO_OUT_D_GAIN_m23_DB,
    AUDIO_OUT_D_GAIN_m22_DB,
    AUDIO_OUT_D_GAIN_m21_DB,
    AUDIO_OUT_D_GAIN_m20_DB,
    AUDIO_OUT_D_GAIN_m19_DB,
    AUDIO_OUT_D_GAIN_m18_DB,
    AUDIO_OUT_D_GAIN_m17_DB,
    AUDIO_OUT_D_GAIN_m16_DB,
    AUDIO_OUT_D_GAIN_m15_DB,
    AUDIO_OUT_D_GAIN_m14_DB,
    AUDIO_OUT_D_GAIN_m13_DB,
    AUDIO_OUT_D_GAIN_m12_DB,
    AUDIO_OUT_D_GAIN_m11_DB,
    AUDIO_OUT_D_GAIN_m10_DB,
    AUDIO_OUT_D_GAIN_m9_DB,
    AUDIO_OUT_D_GAIN_m8_DB,
    AUDIO_OUT_D_GAIN_m7_DB,
    AUDIO_OUT_D_GAIN_m6_DB,
    AUDIO_OUT_D_GAIN_m5_DB,
    AUDIO_OUT_D_GAIN_m4_DB,
    AUDIO_OUT_D_GAIN_m3_DB,
    AUDIO_OUT_D_GAIN_m2_DB,
    AUDIO_OUT_D_GAIN_m1_DB,       /**< -1dB */

    AUDIO_OUT_D_GAIN_0_DB = 0x00, /**< 0dB */
    AUDIO_OUT_D_GAIN_1_DB,        /**< 1dB */
    AUDIO_OUT_D_GAIN_2_DB,
    AUDIO_OUT_D_GAIN_3_DB,
    AUDIO_OUT_D_GAIN_4_DB,
    AUDIO_OUT_D_GAIN_5_DB,
    AUDIO_OUT_D_GAIN_6_DB,
    AUDIO_OUT_D_GAIN_7_DB,
    AUDIO_OUT_D_GAIN_8_DB,
    AUDIO_OUT_D_GAIN_9_DB,
    AUDIO_OUT_D_GAIN_10_DB,
    AUDIO_OUT_D_GAIN_11_DB,
    AUDIO_OUT_D_GAIN_12_DB,
    AUDIO_OUT_D_GAIN_13_DB,
    AUDIO_OUT_D_GAIN_14_DB,
    AUDIO_OUT_D_GAIN_15_DB,
    AUDIO_OUT_D_GAIN_16_DB,
    AUDIO_OUT_D_GAIN_17_DB,
    AUDIO_OUT_D_GAIN_18_DB,
    AUDIO_OUT_D_GAIN_19_DB,
    AUDIO_OUT_D_GAIN_20_DB,
    AUDIO_OUT_D_GAIN_21_DB,
    AUDIO_OUT_D_GAIN_22_DB,
    AUDIO_OUT_D_GAIN_23_DB,
    AUDIO_OUT_D_GAIN_24_DB,
    AUDIO_OUT_D_GAIN_25_DB,
    AUDIO_OUT_D_GAIN_26_DB,
    AUDIO_OUT_D_GAIN_27_DB,
    AUDIO_OUT_D_GAIN_28_DB,
    AUDIO_OUT_D_GAIN_29_DB,
    AUDIO_OUT_D_GAIN_30_DB,
    AUDIO_OUT_D_GAIN_31_DB,
    AUDIO_OUT_D_GAIN_32_DB,
    AUDIO_OUT_D_GAIN_33_DB,
    AUDIO_OUT_D_GAIN_34_DB,
    AUDIO_OUT_D_GAIN_35_DB,
    AUDIO_OUT_D_GAIN_36_DB,
    AUDIO_OUT_D_GAIN_37_DB,
    AUDIO_OUT_D_GAIN_38_DB,
    AUDIO_OUT_D_GAIN_39_DB,
    AUDIO_OUT_D_GAIN_40_DB,
    AUDIO_OUT_D_GAIN_41_DB,
    AUDIO_OUT_D_GAIN_42_DB,
    AUDIO_OUT_D_GAIN_43_DB,
    AUDIO_OUT_D_GAIN_44_DB,
    AUDIO_OUT_D_GAIN_45_DB,
    AUDIO_OUT_D_GAIN_46_DB,
    AUDIO_OUT_D_GAIN_47_DB,
    AUDIO_OUT_D_GAIN_48_DB,
    AUDIO_OUT_D_GAIN_49_DB,
    AUDIO_OUT_D_GAIN_50_DB,
    AUDIO_OUT_D_GAIN_51_DB,
    AUDIO_OUT_D_GAIN_52_DB,
    AUDIO_OUT_D_GAIN_53_DB,
    AUDIO_OUT_D_GAIN_54_DB,
    AUDIO_OUT_D_GAIN_55_DB,
    AUDIO_OUT_D_GAIN_56_DB,
    AUDIO_OUT_D_GAIN_57_DB,
    AUDIO_OUT_D_GAIN_58_DB,
    AUDIO_OUT_D_GAIN_59_DB,
    AUDIO_OUT_D_GAIN_60_DB,
    AUDIO_OUT_D_GAIN_61_DB,
    AUDIO_OUT_D_GAIN_62_DB,
    AUDIO_OUT_D_GAIN_63_DB, /**< 63dB */
} audio_codec0_output_dgain_e;

/**
 * @brief Audio sample rate value.
 * |                                    |                            |
 * | :---------------------------------- | :------------------------- |
 * |            <31:8>                   |         <7:0>              |
 * |         audio_control_div           |       codec_fs             |
 * | fs = MCLK / (audio_control_div + 1) |   codec_freq_sel reg value |
 *
 * @note MCLK = 11.2896MHz(for 44.1kHz) or 12.288MHz (for others).
 */
typedef enum
{
    AUDIO_16K   = 0x03 | (767 << 8),
    AUDIO_44P1K = 0x08 | (255 << 8),
    AUDIO_48K   = 0x08 | (255 << 8),
    AUDIO_96K   = 0x0a | (127 << 8),
    AUDIO_192K  = 0x0c | (63 << 8),
    AUDIO_384K  = 0x0d | (31 << 8),
    AUDIO_768K  = 0x0e | (15 << 8),
} audio_sample_rate_e;

/**
 * @brief Audio ADC/DAC Filter mode
 */
typedef enum
{
    HIGH_FILTER_MODE,
    BALANCED_FILTER_MODE,
    LOW_LATENCY_FILTER_MODE,
} audio_filter_mode_e;

/**
 * @brief Audio codec0 data format.
 *
 */
typedef enum
{
    AUDIO_CODEC0_BIT_16_DATA = 0,
    AUDIO_CODEC0_BIT_24_DATA = 3,
} audio_codec0_data_select_e;

    /**
 * @brief Audio codec1 data format.
 *
 */
typedef enum
{
    AUDIO_CODEC1_BIT_16_DATA = 0,
    AUDIO_CODEC1_BIT_24_DATA = 3,
} audio_codec1_data_select_e;

/**
 * @brief codec0 ADC power mode.
 *
 */
typedef enum
{
    AUDIO_CODEC0_ADC_NORMAL_MODE          = 0,
    AUDIO_CODEC0_ADC_LOW_POWER_MODE       = 1,
    AUDIO_CODEC0_ADC_ULTRA_LOW_POWER_MODE = 2,
} audio_codec0_adc_power_mode;

/**
 * @brief codec0 input config.
 *
 */
typedef struct
{
    audio_codec0_input_select_e input_src;
    audio_sample_rate_e         sample_rate;
    audio_codec0_data_select_e  data_format;
} audio_codec0_input_config_t;

/**
 * @brief codec1 input config.
 *
 */
typedef struct
{
    audio_codec1_input_select_e input_src;
    audio_sample_rate_e         sample_rate;
    audio_codec1_data_select_e  data_format;
} audio_codec1_input_config_t;

/**
 * @brief codec0 output config.
 *
 */
typedef struct
{
    audio_codec0_output_select_e output_dst;
    audio_sample_rate_e          sample_rate;
    audio_codec0_data_select_e   data_format;
} audio_codec0_output_config_t;

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio dma/fifo enum/struct                                          *
 *********************************************************************************************************************/
/*!
 * @name Audio dma/fifo enum/struct.
 * @{
 */

/**
 * @brief FIFO channel.
 * 
 */
typedef enum
{
    FIFO0 = 0x00,
    FIFO1,
    FIFO2,
    FIFO3,
} audio_fifo_chn_e;

/**
 * @brief FIFO TX/RX IRQ type.
 * 
 */
typedef enum
{
    AUDIO_TX_FIFO0 = BIT(0),
    AUDIO_TX_FIFO1 = BIT(1),
    AUDIO_TX_FIFO2 = BIT(2),
    AUDIO_TX_FIFO3 = BIT(3),
    AUDIO_RX_FIFO0 = BIT(4),
    AUDIO_RX_FIFO1 = BIT(5),
    AUDIO_RX_FIFO2 = BIT(6),
    AUDIO_RX_FIFO3 = BIT(7),
} audio_fifo_type_e;

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio hac enum/struct                                               *
 *********************************************************************************************************************/
/*!
 * @name Audio hac enum/struct.
 * @{
 */

/**
 * @brief HAC channel.
 * 
 */
typedef enum
{
    HAC_CHN0 = 0x00,
    HAC_CHN1,
    HAC_CHN2,
    HAC_CHN3,
} audio_hac_chn_e;

typedef enum
{
    HAC_CHN0_DATA0 = 0,
    HAC_CHN1_DATA0 = 1,
    HAC_CHN2_DATA0 = 2,
    HAC_CHN3_DATA0 = 6,
} audio_hac_chn_data_path_e;

/**
 * @brief HAC biquad coefficient select.
 * 
 */
typedef enum
{
    HAC_BIQUAD0,
    HAC_BIQUAD1,
    HAC_BIQUAD2,
    HAC_BIQUAD3,
    HAC_BIQUAD4,
    HAC_BIQUAD5,
    HAC_BIQUAD6,
    HAC_BIQUAD7,
    HAC_BIQUAD8,
    HAC_BIQUAD9,
    HAC_BIQUAD_CNT,
} audio_hac_biquad_e;

/**
 * @brief HAC data input data select.
 *
 */
typedef enum
{
    HAC_INPUT_DATA_MATRIX,
    HAC_INPUT_DATA_MCU,
} audio_hac_input_data_e;

/**
 * @brief HAC data output select.
 *
 */
typedef enum
{
    HAC_OUTPUT_DATA_MATRIX,
    HAC_OUTPUT_DATA_AHB_MST,
} audio_hac_output_data_e;

/**
 * @brief hac tdm_tx_dma_ch num
 *
 */
typedef enum
{
    HAC_TDM_TX_DMA_2CH = 0x00,
    HAC_TDM_TX_DMA_4CH = 0x01,
    HAC_TDM_TX_DMA_6CH = 0x02,
    HAC_TDM_TX_DMA_8CH = 0x03,
} audio_hac_tdm_tx_dma_ch_e;

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio i2s enum/struct                                               *
 *********************************************************************************************************************/
/*!
 * @name Audio i2s enum struct.
 * @{
 */

/**
 * @brief I2S select.
 * 
 */
typedef enum
{
    I2S0,
    I2S1,
    I2S2,
} i2s_select_e;

/**
 * @brief I2S mode.
 * 
 */
typedef enum
{
    I2S_RJ_MODE,
    I2S_LJ_MODE,
    I2S_I2S_MODE,
    I2S_DSP_MODE,
    I2S_TDM_MODE, /**< only for i2s0. */
} i2s_mode_select_e;

/**
 * @brief TDM mode.
 * @note  Only for i2s0.
 */
typedef enum
{
    I2S_TDM_MODE_A,
    I2S_TDM_MODE_B,
    I2S_TDM_MODE_C,
} i2s_tdm_mode_select_e;

/**
 * @brief TDM channel.
 * @note  Only for i2s0.
 */
typedef enum
{
    I2S_TDM_2_CHN,
    I2S_TDM_4_CHN,
    I2S_TDM_6_CHN,
    I2S_TDM_8_CHN,
} i2s_tdm_chn_e;

/**
 * @brief TDM slot width.
 * @note  Only for i2s0.
 * 
 */
typedef enum
{
    I2S_TDM_SLOT_WIDTH_16,
    I2S_TDM_SLOT_WIDTH_24,
    I2S_TDM_SLOT_WIDTH_32,
} i2s_tdm_slot_width_e;

/**
 * @brief I2S line mode.
 * 
 */
typedef enum
{
    I2S_5_LINE_MODE,     /**< BCLK, ADC_LR_CLK, DAC_LR_CLK, ADC_DATA, DAC_DATA. */
    I2S_4_LINE_DAC_MODE, /**< BCLK, DAC_LR_CLK, ADC_DATA, DAC_DATA. */
    I2S_4_LINE_ADC_MODE, /**< BCLK, ADC_LR_CLK, ADC_DATA, DAC_DATA. */
    I2S_2_LANE_TX_MODE,  /**< ADC_DATA and DAC_DATA as TX at the same time. */
    I2S_2_LANE_RX_MODE,  /**< ADC_DATA and DAC_DATA as RX at the same time. */
} i2s_io_mode_e;

/**
 * @brief I2S word length.
 * 
 */
typedef enum
{
    I2S_BIT_16_DATA,
    I2S_BIT_20_DATA,
    I2S_BIT_24_DATA,
} i2s_wl_mode_e;

/**
 * @brief I2S master/slave select.
 * 
 */
typedef enum
{
    I2S_AS_SLAVE_EN,
    I2S_AS_MASTER_EN,
} i2s_m_s_mode_e;

/**
 * @brief I2S align mode.
 * 
 */
typedef enum
{
    I2S0_I2S1_ALIGN      = BIT(0) | BIT(1),
    I2S1_I2S2_ALIGN      = BIT(1) | BIT(2),
    I2S0_I2S1_I2S2_ALIGN = BIT(0) | BIT(1) | BIT(2),
} i2s_align_mode_e;

/**
 * @brief I2S align clk.
 * 
 */
typedef enum
{
    I2S_ALIGN_SELF_CLK, /**< use self i2s clk as align clk.*/
    I2S_ALIGN_CLK,      /**< use i2s1 clk as align clk.*/
} i2s_align_clk_e;

/**
 * @brief I2S data invert select.
 * 
 */
typedef enum
{
    I2S_DATA_INVERT_DIS,
    I2S_DATA_INVERT_EN,
} i2s_data_invert_e;

/**
 * @brief I2S CLK invert select.
 * 
 */
typedef enum
{
    I2S_LR_CLK_INVERT_DIS, /**< dsp mode: dsp mode a */
    I2S_LR_CLK_INVERT_EN,  /**< dsp mode: dsp mode b */
} i2s_lr_clk_invert_e;

/**
 * @brief I2S TX channel.
 * 
 */
typedef enum
{
    I2S0_CHN0,
    I2S0_CHN1,
    I2S0_CHN2,
    I2S0_CHN3,
    I2S0_CHN4,
    I2S0_CHN5,
    I2S0_CHN6,
    I2S0_CHN7,

    I2S1_CHN0,
    I2S1_CHN1,

    I2S2_CHN0,
    I2S2_CHN1,
} audio_i2s_tx_chn_e;

/**
 * @brief I2S invert config.
 * 
 */
typedef struct
{
    unsigned char i2s_lr_clk_invert_select;
    unsigned char i2s_data_invert_select;
} i2s_invert_config_t;

/**
 * @brief I2S pin config.
 * 
 */
typedef struct
{
    gpio_func_pin_e bclk_pin;
    gpio_func_pin_e adc_lr_clk_pin;
    gpio_func_pin_e adc_dat_pin;
    gpio_func_pin_e dac_lr_clk_pin;
    gpio_func_pin_e dac_dat_pin;
} i2s_pin_config_t;

/**
 * @brief I2S align config.
 * 
 */
typedef struct
{
    unsigned int     align_th; /**< align threshold*/
    i2s_align_mode_e align_mode;
    i2s_align_clk_e  align_clk;
} i2s_align_config_t;

/**
 * @brief I2S config.
 * 
 */
typedef struct
{
    unsigned short       *sample_rate;
    i2s_pin_config_t     *pin_config;
    i2s_select_e          i2s_select;
    i2s_wl_mode_e         data_width;
    i2s_mode_select_e     i2s_mode;
    i2s_tdm_mode_select_e tdm_mode;
    i2s_tdm_slot_width_e  tdm_slot_width;
    i2s_m_s_mode_e        master_slave_mode;
    i2s_io_mode_e         io_mode;
} audio_i2s_config_t;

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio matrix enum/struct                                            *
 *********************************************************************************************************************/
/*!
 * @name Audio matrix enum/struct.
 * @{
 */

/**
 * @brief fifo rx route source select.
 * 
 */
typedef enum
{
    FIFO_RX_ROUTE_I2S0_RX = 0x01,
    FIFO_RX_ROUTE_I2S1_RX,
    FIFO_RX_ROUTE_I2S2_RX,
    FIFO_RX_ROUTE_ANC0,
    FIFO_RX_ROUTE_ANC1,
    FIFO_RX_ROUTE_CODEC0_ADCA, /**< codec0 ADC_A1(amic/dmic), ADC_A2(amic/dmic). */
    FIFO_RX_ROUTE_CODEC0_ADCB, /**< codec0 ADC_B1(amic/dmic), ADC_B2(amic/dmic). */
    FIFO_RX_ROUTE_CODEC1_ADCA, /**< codec1 ADC_A1(dmic), ADC_A2(dmic). */
    FIFO_RX_ROUTE_SPDIF_RX,
    FIFO_RX_ROUTE_USB_ISO_RX,
    FIFO_RX_ROUTE_HAC_DATA0,
    FIFO_RX_ROUTE_HAC_DATA1,
    FIFO_RX_ROUTE_HAC_TDM0_DATA2,
    FIFO_RX_ROUTE_HAC_TDM1_DATA3,
    FIFO_RX_ROUTE_HAC_TDM01_DATA23,
} audio_matrix_fifo_rx_route_e;

/**
 * @brief fifo rx route data format select.
 * 
 */
typedef enum
{
    /* fifo rx route i2s data format */
    FIFO_RX_I2S_RX_CHN01_20_OR_24 = 0x00, /**< fifo rx route from i2s data format. */
    FIFO_RX_I2S_RX_CHN01_16,
    FIFO_RX_I2S_RX_CHN0_20_OR_24,
    FIFO_RX_I2S_RX_CHN0_16,
    FIFO_RX_I2S_RX_CHN1_20_OR_24,
    FIFO_RX_I2S_RX_CHN1_16,

    FIFO_RX_I2S0_TDM_20_OR_24 = 0x06,          /**< fifo rx route from i2s0 data format. */
    FIFO_RX_I2S0_TDM_16,

    FIFO_RX_I2S1_I2S01_CHN01_20_OR_24 = 0x06,  /**< fifo rx route from i2s1 data format. */
    FIFO_RX_I2S1_I2S01_CHN01_16,

    FIFO_RX_I2S2_I2S012_CHN01_20_OR_24 = 0x06, /**< fifo rx route from i2s2 data format. */
    FIFO_RX_I2S2_I2S012_CHN01_16,
    FIFO_RX_I2S2_I2S12_CHN01_20_OR_24,
    FIFO_RX_I2S2_I2S12_CHN01_16,

    /* fifo rx route anc data format */
    FIFO_RX_ANC_POST_PRE_32BIT = 0x00, /**< fifo rx route from anc0/1 data common format. */
    FIFO_RX_ANC_POST_PRE_16BIT,
    FIFO_RX_ANC_SPEAKER_OUT,

    FIFO_RX_ANC1_PORT_PRE_ANC0_PORT_PRE_32BIT, /**< fifo rx route from anc1 data format. */
    FIFO_RX_ANC1_PORT_PRE_ANC0_PORT_PRE_16BIT,
    FIFO_RX_ANC1_AND_ANC0_SPEAKER_OUT,

    /* fifo rx route adc data format */
    FIFO_RX_CODEC0_ADCA_A1_A2_32BIT = 0x00, /**< fifo rx route from adc0 data format. */
    FIFO_RX_CODEC0_ADCA_A1_A2_16BIT,
    FIFO_RX_CODEC0_ADCA_A1_32BIT,
    FIFO_RX_CODEC0_ADCA_A1_16BIT,
    FIFO_RX_CODEC0_ADCA_A2_32BIT,
    FIFO_RX_CODEC0_ADCA_A2_16BIT,

    FIFO_RX_CODEC0_ADCB_B1_B2_32BIT = 0x00, /**< fifo rx route from codec0 adcb data format. */
    FIFO_RX_CODEC0_ADCB_B1_B2_16BIT,
    FIFO_RX_CODEC0_ADCB_B1_32BIT,
    FIFO_RX_CODEC0_ADCB_B1_16BIT,
    FIFO_RX_CODEC0_ADCB_B2_32BIT,
    FIFO_RX_CODEC0_ADCB_B2_16BIT,
    FIFO_RX_CODEC0_ADCAB_A1_A2_B1_B2_32BIT,
    FIFO_RX_CODEC0_ADCAB_A1_A2_B1_B2_16BIT,

    FIFO_RX_CODEC1_ADCA_A1_A2_32BIT = 0x00, /**< fifo rx route from codec1 adca data format. */
    FIFO_RX_CODEC1_ADCA_A1_A2_16BIT,
    FIFO_RX_CODEC1_ADCA_A1_32BIT,
    FIFO_RX_CODEC1_ADCA_A1_16BIT,
    FIFO_RX_CODEC1_ADCA_A2_32BIT,
    FIFO_RX_CODEC1_ADCA_A2_16BIT,

    FIFO_RX_HAC_MONO_16BIT = 0x01, /**< fifo rx route from hac common data format. */
    FIFO_RX_HAC_MONO_20_OR_24BIT,

    FIFO_RX_HAC23_STEREO_16BIT,    /**< fifo rx route from hac2/3 data format. */
    FIFO_RX_HAC23_STEREO_20_OR_24BIT,
    FIFO_RX_HAC23_4_CHN_16BIT,
    FIFO_RX_HAC23_4_CHN_20_OR_24BIT,
    FIFO_RX_HAC23_6_CHN_16BIT,
    FIFO_RX_HAC23_6_CHN_20_OR_24BIT,
    FIFO_RX_HAC23_8_CHN_16BIT,
    FIFO_RX_HAC23_8_CHN_20_OR_24BIT,

    FIFO_RX_DATA_FORMAT_INVALID = 0xff,
} audio_matrix_fifo_rx_format_e;

/**
 * @brief i2s tx route source select.
 * 
 */
typedef enum
{
    I2S_TX_ROUTE_FIFO = 0x01, /**< I2S channel 0/1 route. */
    I2S_TX_ROUTE_ANC0_SPEAKER,
    I2S_TX_ROUTE_ANC1_SPEAKER,
    I2S_TX_ROUTE_HAC_DATA0,
    I2S_TX_ROUTE_HAC_DATA1,
    I2S_TX_ROUTE_HAC_TDM0_DATA0123,
    I2S_TX_ROUTE_HAC_TDM1_DATA0123,
    I2S_TX_ROUTE_HAC_TDM01_DATA0123,

    TDM0_TX_ROUTE_HAC_TDM0_DATA0123 = 0x02, /**< tdm channel 2-7 route. */
    TDM1_TX_ROUTE_HAC_TDM1_DATA0123,
} audio_matrix_i2s_tx_route_e;

/**
 * @brief i2s tx route data format select.
 * 
 */
typedef enum
{
    I2S_TX_FIFO0_20_OR_24_MONO = 0x00, /**< i2s common config */
    I2S_TX_FIFO1_20_OR_24_MONO,
    I2S_TX_FIFO2_20_OR_24_MONO,
    I2S_TX_FIFO3_20_OR_24_MONO,
    I2S_TX_FIFO01_20_OR_24_STEREO,
    I2S_TX_FIFO23_20_OR_24_STEREO,
    I2S_TX_FIFO0_20_OR_24_STEREO,
    I2S_TX_FIFO1_20_OR_24_STEREO,
    I2S_TX_FIFO2_20_OR_24_STEREO,
    I2S_TX_FIFO3_20_OR_24_STEREO,
    I2S_TX_FIFO0_16_MONO,
    I2S_TX_FIFO1_16_MONO,
    I2S_TX_FIFO2_16_MONO,
    I2S_TX_FIFO3_16_MONO,
    I2S_TX_FIFO01_16_STEREO,
    I2S_TX_FIFO23_16_STEREO,
    I2S_TX_FIFO0_16_STEREO,
    I2S_TX_FIFO1_16_STEREO,
    I2S_TX_FIFO2_16_STEREO,
    I2S_TX_FIFO3_16_STEREO,

    I2S0_TX_FIFO0_16_STEREO_I2S01 = 0x14, /**< for i2s0 config */
    I2S0_TX_FIFO1_16_STEREO_I2S01,
    I2S0_TX_FIFO2_16_STEREO_I2S01,
    I2S0_TX_FIFO3_16_STEREO_I2S01,
    I2S0_TX_FIFO0_20_OR_24_STEREO_I2S01,
    I2S0_TX_FIFO1_20_OR_24_STEREO_I2S01,
    I2S0_TX_FIFO2_20_OR_24_STEREO_I2S01,
    I2S0_TX_FIFO3_20_OR_24_STEREO_I2S01,
    I2S0_TX_FIFO0_20_OR_24_TDM_I2S0,
    I2S0_TX_FIFO1_20_OR_24_TDM_I2S0,
    I2S0_TX_FIFO2_20_OR_24_TDM_I2S0,
    I2S0_TX_FIFO3_20_OR_24_TDM_I2S0,
    I2S_TX_FIFO0_16_TDM_I2S0,
    I2S_TX_FIFO1_16_TDM_I2S0,
    I2S_TX_FIFO2_16_TDM_I2S0,
    I2S_TX_FIFO3_16_TDM_I2S0,
    I2S_TX_FIFO0_16_STEREO_I2S012,
    I2S_TX_FIFO0_20_OR_24_STEREO_I2S012,

    I2S1_TX_FIFO0_16_STEREO_I2S01 = 0x14, /**< for i2s1 config */
    I2S1_TX_FIFO1_16_STEREO_I2S01,
    I2S1_TX_FIFO2_16_STEREO_I2S01,
    I2S1_TX_FIFO3_16_STEREO_I2S01,
    I2S1_TX_FIFO0_20_OR_24_STEREO_I2S01,
    I2S1_TX_FIFO1_20_OR_24_STEREO_I2S01,
    I2S1_TX_FIFO2_20_OR_24_STEREO_I2S01,
    I2S1_TX_FIFO3_20_OR_24_STEREO_I2S01,
    I2S1_TX_FIFO0_16_STEREO_I2S012,
    I2S1_TX_FIFO0_20_OR_24_STEREO_I2S012,
    I2S1_TX_FIFO0_16_STEREO_I2S12,
    I2S1_TX_FIFO0_20_OR_24_STEREO_I2S12,

    I2S2_TX_FIFO0_16_STEREO_I2S012 = 0x14, /**< for i2s2 config */
    I2S2_TX_FIFO0_20_OR_24_STEREO_I2S012,
    I2S2_TX_FIFO0_16_STEREO_I2S12,
    I2S2_TX_FIFO0_20_OR_24_STEREO_I2S12,

    I2S_TX_HAC_1CH = 1, /**< for i2s tx hac common config */
    I2S_TX_HAC_2CH = 2,
    I2S_TX_HAC_TDM = 3,

    I2S0_TX_HAC_2LINE          = 4, /**< for i2s0 tx hac config */
    I2S0_TX_HAC_I2S01_SYNC     = 5,
    I2S0_TX_HAC_I2S012_SYNC    = 6,
    I2S0_TX_HAC_TDM_HAC3_FIRST = 7,

    I2S1_TX_HAC_2LINE       = 4, /**< for i2s1 tx hac config */
    I2S1_TX_HAC_I2S01_SYNC  = 5,
    I2S1_TX_HAC_I2S12_SYNC  = 6,
    I2S1_TX_HAC_I2S012_SYNC = 7,

    I2S2_TX_HAC_I2S12_SYNC  = 4,  /**< for i2s1 tx hac config */
    I2S2_TX_HAC_I2S012_SYNC = 5,

    I2S_TX_HAC_CH01_20_OR_24 = 0, /**< for i2s rx hac common config */
    I2S_TX_HAC_CH0_20_OR_24  = 2,
    I2S_TX_HAC_CH1_20_OR_24  = 4,
    I2S_TX_HAC_TDM2_FIRST    = 6,
    I2S_TX_HAC_TDM3_FIRST    = 7,

    I2S_RX_DATA_FORMAT_INVALID = 0xff,
} audio_matrix_i2s_tx_format_e;

/**
 * @brief anc_src route source select.
 * 
 */
typedef enum
{
    ANC_SRC_ROUTE_FIFO = 0x01,
    ANC_SRC_ROUTE_I2S0_RX,
    ANC_SRC_ROUTE_I2S1_RX,
    ANC_SRC_ROUTE_I2S2_RX,
    ANC_SRC_ROUTE_CODEC0_ADCA,
    ANC_SRC_ROUTE_CODEC0_ADCB,
    ANC_SRC_ROUTE_CODEC1_ADCA,
    ANC_SRC_ROUTE_HAC_DATA0,
    ANC_SRC_ROUTE_HAC_DATA1,
    ANC_SRC_ROUTE_HAC_TDM0_DATA0,
    ANC_SRC_ROUTE_HAC_TDM0_DATA1,
    ANC_SRC_ROUTE_HAC_TDM0_DATA2,
    ANC_SRC_ROUTE_HAC_TDM0_DATA3,
    ANC_SRC_ROUTE_HAC_TDM1_DATA0,
    ANC_SRC_ROUTE_HAC_TDM1_DATA1,
    ANC_SRC_ROUTE_HAC_TDM1_DATA2,
    ANC_SRC_ROUTE_HAC_TDM1_DATA3,
} audio_matrix_anc_src_route_e;

/**
 * @brief anc_src route data format select.
 * 
 */
typedef enum
{
    ANC_SRC_DATA_FIFO0, /**< anc src route from fifo data format select. */
    ANC_SRC_DATA_FIFO1,
    ANC_SRC_DATA_FIFO2,
    ANC_SRC_DATA_FIFO3,
    ANC_SRC_2FIFO0_24BIT_STEREO,
    ANC_SRC_2FIFO2_24BIT_STEREO,
    ANC_SRC_STEREO_DATA_FIFO0,
    ANC_SRC_STEREO_DATA_FIFO1,
    ANC_SRC_STEREO_DATA_FIFO2,
    ANC_SRC_STEREO_DATA_FIFO3,
    ANC_SRC_1FIFO0_16BIT_STEREO,
    ANC_SRC_1FIFO1_16BIT_STEREO,
    ANC_SRC_1FIFO2_16BIT_STEREO,
    ANC_SRC_1FIFO3_16BIT_STEREO,

    ANC_SRC_I2S_CH0_20_OR_24_BIT = 0x02, /**< anc src route from i2s data format select. */
    ANC_SRC_I2S_CH1_20_OR_24_BIT = 0x04,

    ANC_SRC_ADC_LEFT_32_BIT  = 0x02,     /**< anc src route from adc data format select. */
    ANC_SRC_ADC_RIGHT_32_BIT = 0x04,

    ANC_SRC_DATA_FORMAT_INVALID = 0xff,
} audio_matrix_anc_src_format_e;

/**
 * @brief anc_ref route source select.
 * 
 */
typedef enum
{
    ANC_REF_ROUTE_I2S0_RX = 0x01,
    ANC_REF_ROUTE_I2S1_RX,
    ANC_REF_ROUTE_I2S2_RX,
    ANC_REF_ROUTE_CODEC0_ADCA,
    ANC_REF_ROUTE_CODEC0_ADCB,
    ANC_REF_ROUTE_CODEC1_ADCA,
    ANC_REF_ROUTE_HAC_DATA0,
    ANC_REF_ROUTE_HAC_DATA1,
    ANC_REF_ROUTE_HAC_TDM0_DATA0,
    ANC_REF_ROUTE_HAC_TDM0_DATA1,
    ANC_REF_ROUTE_HAC_TDM0_DATA2,
    ANC_REF_ROUTE_HAC_TDM0_DATA3,
    ANC_REF_ROUTE_HAC_TDM1_DATA0,
    ANC_REF_ROUTE_HAC_TDM1_DATA1,
    ANC_REF_ROUTE_HAC_TDM1_DATA2,
    ANC_REF_ROUTE_HAC_TDM1_DATA3,
} audio_matrix_anc_ref_route_e;

/**
 * @brief anc_ref route data format select.
 * 
 */
typedef enum
{
    ANC_REF_I2S_CH0_20_OR_24_BIT = 0x02, /**< anc ref route from i2s data format select. */
    ANC_REF_I2S_CH1_20_OR_24_BIT = 0x04,

    ANC_REF_ADC_LEFT_32_BIT  = 0x02,     /**< anc ref route from adc data format select. */
    ANC_REF_ADC_RIGHT_32_BIT = 0x04,

    ANC_REF_DATA_FORMAT_INVALID = 0xff,
} audio_matrix_anc_ref_format_e;

/**
 * @brief anc_err route source select.
 * 
 */
typedef enum
{
    ANC_ERR_ROUTE_I2S0_RX = 0x01,
    ANC_ERR_ROUTE_I2S1_RX,
    ANC_ERR_ROUTE_I2S2_RX,
    ANC_ERR_ROUTE_CODEC0_ADCA,
    ANC_ERR_ROUTE_CODEC0_ADCB,
    ANC_ERR_ROUTE_CODEC1_ADCA,
    ANC_ERR_ROUTE_HAC_DATA0,
    ANC_ERR_ROUTE_HAC_DATA1,
    ANC_ERR_ROUTE_HAC_TDM0_DATA0,
    ANC_ERR_ROUTE_HAC_TDM0_DATA1,
    ANC_ERR_ROUTE_HAC_TDM0_DATA2,
    ANC_ERR_ROUTE_HAC_TDM0_DATA3,
    ANC_ERR_ROUTE_HAC_TDM1_DATA0,
    ANC_ERR_ROUTE_HAC_TDM1_DATA1,
    ANC_ERR_ROUTE_HAC_TDM1_DATA2,
    ANC_ERR_ROUTE_HAC_TDM1_DATA3,
} audio_matrix_anc_err_route_e;

/**
 * @brief anc_err route data format select.
 * 
 */
typedef enum
{
    ANC_ERR_I2S_CH0_20_OR_24_BIT = 0x02, /**< anc err route from i2s data format select. */
    ANC_ERR_I2S_CH1_20_OR_24_BIT = 0x04,

    ANC_ERR_ADC_LEFT_32_BIT  = 0x02,     /**< anc err route from adc data format select. */
    ANC_ERR_ADC_RIGHT_32_BIT = 0x04,

    ANC_ERR_DATA_FORMAT_INVALID = 0xff,
} audio_matrix_anc_err_format_e;

/**
 * @brief dac route source select.
 * 
 */
typedef enum
{
    DAC_ROUTE_FIFO = 0x01,
    DAC_ROUTE_ANC0_SPEAKER,
    DAC_ROUTE_ANC1_SPEAKER,
    DAC_ROUTE_HAC_DATA0,
    DAC_ROUTE_HAC_DATA1,
    DAC_ROUTE_HAC_TDM0_DATA0,
    DAC_ROUTE_HAC_TDM0_DATA1,
    DAC_ROUTE_HAC_TDM0_DATA2,
    DAC_ROUTE_HAC_TDM0_DATA3,
    DAC_ROUTE_HAC_TDM1_DATA0,
    DAC_ROUTE_HAC_TDM1_DATA1,
    DAC_ROUTE_HAC_TDM1_DATA2,
    DAC_ROUTE_HAC_TDM1_DATA3,
} audio_matrix_dac_route_e;

/**
 * @brief dac route data format select.
 * 
 */
typedef enum
{
    DAC_FIFO_MONO_24BIT_FIFO0, /**< dac route from fifo data format select. */
    DAC_FIFO_MONO_24BIT_FIFO1,
    DAC_FIFO_MONO_24BIT_FIFO2,
    DAC_FIFO_MONO_24BIT_FIFO3,
    DAC_FIFO_STEREO_24BIT_FIFO0,
    DAC_FIFO_STEREO_24BIT_FIFO1,
    DAC_FIFO_STEREO_24BIT_FIFO2,
    DAC_FIFO_STEREO_24BIT_FIFO3,
    DAC_FIFO_STEREO_24BIT_2FIFO0,
    DAC_FIFO_STEREO_24BIT_2FIFO2,
    DAC_FIFO_MONO_16BIT_FIFO0,
    DAC_FIFO_MONO_16BIT_FIFO1,
    DAC_FIFO_MONO_16BIT_FIFO2,
    DAC_FIFO_MONO_16BIT_FIFO3,
    DAC_FIFO_STEREO_16BIT_FIFO0,
    DAC_FIFO_STEREO_16BIT_FIFO1,
    DAC_FIFO_STEREO_16BIT_FIFO2,
    DAC_FIFO_STEREO_16BIT_FIFO3,
    DAC_FIFO_STEREO_16BIT_2FIFO0,
    DAC_FIFO_STEREO_16BIT_2FIFO2,

    DAC_DATA_FORMAT_INVALID = 0xff,
} audio_matrix_dac_format_e;

/**
 * @brief spdif tx route source select.
 * 
 */
typedef enum
{
    SPDIF_TX_ROUTE_FIFO0 = 0x01,
    SPDIF_TX_ROUTE_FIFO1,
    SPDIF_TX_ROUTE_FIFO2,
    SPDIF_TX_ROUTE_FIFO3,
    SPDIF_TX_ROUTE_HAC_DATA0,
    SPDIF_TX_ROUTE_HAC_DATA1,
    SPDIF_TX_ROUTE_HAC_TDM0_DATA0,
    SPDIF_TX_ROUTE_HAC_TDM0_DATA1,
    SPDIF_TX_ROUTE_HAC_TDM0_DATA2,
    SPDIF_TX_ROUTE_HAC_TDM0_DATA3,
    SPDIF_TX_ROUTE_HAC_TDM1_DATA0,
    SPDIF_TX_ROUTE_HAC_TDM1_DATA1,
    SPDIF_TX_ROUTE_HAC_TDM1_DATA2,
    SPDIF_TX_ROUTE_HAC_TDM1_DATA3,
} audio_matrix_spdif_tx_route_e;

/**
 * @brief hac route source select.
 * 
 */
typedef enum
{
    HAC_DATA_ROUTE_FIFO0 = 0x01,
    HAC_DATA_ROUTE_FIFO1,
    HAC_DATA_ROUTE_FIFO2,
    HAC_DATA_ROUTE_FIFO3,
    HAC_DATA_ROUTE_I2S0_RX,
    HAC_DATA_ROUTE_I2S1_RX,
    HAC_DATA_ROUTE_I2S2_RX,
    HAC_DATA_ROUTE_ANC0_PRE,
    HAC_DATA_ROUTE_ANC0_POST,
    HAC_DATA_ROUTE_ANC0_SPEAKER,
    HAC_DATA_ROUTE_ANC1_PRE,
    HAC_DATA_ROUTE_ANC1_POST,
    HAC_DATA_ROUTE_ANC1_SPEAKER,
    HAC_DATA_ROUTE_ADC0,
    HAC_DATA_ROUTE_ADC1,
    HAC_DATA_ROUTE_ADC2,
    HAC_DATA_ROUTE_SPDIF_RX,
    HAC_DATA_ROUTE_USB_ISO_RX,
} audio_matrix_hac_route_e;

/**
 * @brief hac route data format select.
 * 
 */
typedef enum
{
    HAC_16_BIT       = 0x01,
    HAC_20_OR_24_BIT = 0x02,                 /**< hac route from DMA common data format select. */

    HAC2_I2S_16_BIT                  = 0x03, /**< hac2 route from DMA data format select. */
    HAC2_TDM_16_BIT                  = 0x05,
    HAC2_I2S_20_OR_24_BIT            = 0x04,
    HAC2_TDM_20_OR_24_BIT_HAC3_FIRST = 0x06,
    HAC2_TDM_20_OR_24_BIT_HAC2_FIRST = 0x00,

    HAC3_I2S_16_BIT                  = 0x03, /**< hac3 route from DMA data format select. */
    HAC3_TDM_16_BIT                  = 0x05,
    HAC3_I2S_20_OR_24_BIT            = 0x04,
    HAC3_TDM_20_OR_24_BIT_HAC3_FIRST = 0x06,
    HAC3_TDM_20_OR_24_BIT_HAC2_FIRST = 0x00,

    HAC01_I2S_CH0_20_OR_24_BIT = 0x02,  /**< hac0,1 route from i2s data format select. */
    HAC01_I2S_CH1_20_OR_24_BIT = 0x04,

    HAC23_I2S_CH01_20_OR_24_BIT = 0x00, /**< hac2,3 route from i2s data format select. */
    HAC23_I2S_CH0_20_OR_24_BIT  = 0x02,
    HAC23_I2S_CH1_20_OR_24_BIT  = 0x04,
    HAC23_I2S_TDM_HAC2_FIRST    = 0x06,
    HAC23_I2S_TDM_HAC3_FIRST    = 0x07,

    HAC_ADC_LEFT_RIGHT_32_BIT = 0x00, /**< hac route from adc data format select. */
    HAC_ADC_LEFT_32_BIT       = 0x02,
    HAC_ADC_RIGHT_32_BIT      = 0x04,

    HAC_DATA_FORMAT_INVALID = 0xff,
} audio_matrix_hac_format_e;

/**
 * @brief side_tone route source select.
 * 
 */
typedef enum
{
    SIDE_TONE_ROUTE_FIFO = 0x01,
    SIDE_TONE_ROUTE_I2S0_RX,
    SIDE_TONE_ROUTE_I2S1_RX,
    SIDE_TONE_ROUTE_I2S2_RX,
    SIDE_TONE_ROUTE_ADC0,
    SIDE_TONE_ROUTE_ADC1,
    SIDE_TONE_ROUTE_ADC2,
    SIDE_TONE_ROUTE_SPDIF_RX,
    SIDE_TONE_ROUTE_HAC_DATA0,
    SIDE_TONE_ROUTE_HAC_DATA1,
    SIDE_TONE_TX_ROUTE_HAC_TDM0_DATA0,
    SIDE_TONE_TX_ROUTE_HAC_TDM0_DATA1,
    SIDE_TONE_TX_ROUTE_HAC_TDM0_DATA2,
    SIDE_TONE_TX_ROUTE_HAC_TDM0_DATA3,
    SIDE_TONE_TX_ROUTE_HAC_TDM1_DATA0,
    SIDE_TONE_TX_ROUTE_HAC_TDM1_DATA1,
    SIDE_TONE_TX_ROUTE_HAC_TDM1_DATA2,
    SIDE_TONE_TX_ROUTE_HAC_TDM1_DATA3,
} audio_matrix_side_tone_route_e;

/**
 * @brief side_tone route data format select.
 * 
 */
typedef enum
{
    SIDE_TONE_FIFO0_20_OR_24_MONO = 0x00, /**< side_tone route from fifo data format select. */
    SIDE_TONE_FIFO1_20_OR_24_MONO,
    SIDE_TONE_FIFO2_20_OR_24_MONO,
    SIDE_TONE_FIFO3_20_OR_24_MONO,
    SIDE_TONE_FIFO01_20_OR_24_STEREO,
    SIDE_TONE_FIFO23_20_OR_24_STEREO,
    SIDE_TONE_FIFO0_20_OR_24_STEREO,
    SIDE_TONE_FIFO1_20_OR_24_STEREO,
    SIDE_TONE_FIFO2_20_OR_24_STEREO,
    SIDE_TONE_FIFO3_20_OR_24_STEREO,
    SIDE_TONE_FIFO0_16_MONO,
    SIDE_TONE_FIFO1_16_MONO,
    SIDE_TONE_FIFO2_16_MONO,
    SIDE_TONE_FIFO3_16_MONO,
    SIDE_TONE_FIFO01_16_STEREO,
    SIDE_TONE_FIFO23_16_STEREO,
    SIDE_TONE_FIFO0_16_STEREO,
    SIDE_TONE_FIFO1_16_STEREO,
    SIDE_TONE_FIFO2_16_STEREO,
    SIDE_TONE_FIFO3_16_STEREO,

    SIDE_TONE_I2S_CH0_20_OR_24_BIT = 0x02, /**< side_tone route from i2s data format select. */
    SIDE_TONE_I2S_CH1_20_OR_24_BIT = 0x04,

    SIDE_TONE_ADC_LEFT_32_BIT  = 0x02,     /**< side_tone route from adc data format select. */
    SIDE_TONE_ADC_RIGHT_32_BIT = 0x04,

    SIDE_TONE_DATA_FORMAT_INVALID = 0xff,
} audio_matrix_side_tone_format_e;

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio side_tone enum/struct                                         *
 *********************************************************************************************************************/
/*!
 * @name Audio side_tone enum/struct.
 * @{
 */

/**
 * @brief side_tone channel.
 * 
 */
typedef enum
{
    SIDE_TONE_CHN0 = 0x00,
    SIDE_TONE_CHN1,
    SIDE_TONE_CHN2,
    SIDE_TONE_CHN3,
    SIDE_TONE_CHN4,
    SIDE_TONE_CHN5,
    SIDE_TONE_CHN6,
    SIDE_TONE_CHN7,
} audio_side_tone_chn_e;

/**
 * @brief side_tone channels.
 *
 */
typedef enum
{
    SIDE_TONE_CHN01 = 0x00,
    SIDE_TONE_CHN23,
    SIDE_TONE_CHN45,
    SIDE_TONE_CHN67,
} audio_side_tone_chns_e;

/**
 * @brief side_tone gain.
 */
typedef enum
{
    SIDETONE_m30_DB = 0,
    SIDETONE_m27_DB,
    SIDETONE_m24_DB,
    SIDETONE_m21_DB,
    SIDETONE_m18_DB,
    SIDETONE_m15_DB,
    SIDETONE_m12_DB,
    SIDETONE_m9_DB,
    SIDETONE_m6_DB,
    SIDETONE_m3_DB,
    SIDETONE_0DB,
    SIDETONE_3DB,
    SIDETONE_6DB,
    SIDETONE_9DB,
    SIDETONE_12DB,
    SIDETONE_15DB,
} audio_side_tone_gain_e;

/**********************************************************************************************************************
 *                                                Audio spdif enum/struct                                             *
 *********************************************************************************************************************/
/*!
 * @name Audio spdif enum/struct
 * @{
 */

/**
 * @brief spdif tx clk config.
 * 
 */
typedef enum
{
    SPDIF_TX_CLK_32K   = 18,
    SPDIF_TX_CLK_44P1K = 15,
    SPDIF_TX_CLK_48K   = 12,
    SPDIF_TX_CLK_96K   = 6,
    SPDIF_TX_CLK_192K  = 3,
} spdif_tx_clk_e;

typedef enum
{
    SPDIF_PREAMBLE_B = 0x01,
    SPDIF_PREAMBLE_M,
    SPDIF_PREAMBLE_W,
} spdif_preamble_e;

/**
 * @brief spdif pin config.
 * 
 */
typedef struct
{
    gpio_func_pin_e spdif_rx_pin;
    gpio_func_pin_e spdif_tx_pin;
} spdif_pin_config_t;

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio anc interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio anc interface.
 * @{
 */

static inline void audio_anc_clk_en(audio_anc_chn_e anc_chn)
{
    reg_audio_clk_en_1 |= (BIT(anc_chn) << 4);
}

/**
 * @brief      This function serves to set anc mode.
 *
 * @param[in]  anc_chn  - anc channel.
 * @param[in]  anc_mode - anc mode.
 * @return     none
 */
static inline void audio_anc_set_mode(audio_anc_chn_e anc_chn, audio_anc_mode_e anc_mode)
{
    reg_audio_anc_config2(anc_chn) = (reg_audio_anc_config2(anc_chn) & (~FLD_ANC_MODE_SEL)) | MASK_VAL(FLD_ANC_MODE_SEL, anc_mode);
}

/**
 * @brief      This function serves to select anc adder2 output mode.
 *
 * @param[in]  anc_chn     - anc channel.
 * @param[in]  output_mode - adder2 output mode select.
 * @note
 */
static inline void audio_anc_set_adder2_mode(audio_anc_chn_e anc_chn, audio_anc_set_adder2_mode_e output_mode)
{
    reg_audio_anc_config2(anc_chn) = (reg_audio_anc_config2(anc_chn) & (~FLD_ANC_ADD_CZ1_SEL)) | MASK_VAL(FLD_ANC_ADD_CZ1_SEL, output_mode);
}

/**
 * @brief      This function serves to select anc adder3 output mode.
 *
 * @param[in]  anc_chn     - anc channel.
 * @param[in]  output_mode - adder3 output mode select.
 * @note
 */
static inline void audio_anc_set_adder3_mode(audio_anc_chn_e anc_chn, audio_anc_set_adder3_mode_e output_mode)
{
    reg_audio_anc_config2(anc_chn) = (reg_audio_anc_config2(anc_chn) & (~FLD_ANC_ADD_WZ_SEL)) | MASK_VAL(FLD_ANC_ADD_WZ_SEL, output_mode);
}

/**
 * @brief      This function serves to select anc adder3 output data priority.
 *
 * @param[in]  anc_chn  - anc channel.
 * @param[in]  mode_sel - adder3 output data priority.
 * @note
 *        - The adder3 output is the sum of wz and resample, but the two are not consistent in terms of time, this register is used to select which timing is first.
 */
static inline void audio_anc_set_adder3_priority(audio_anc_chn_e anc_chn, audio_anc_adder3_out_pri_e mode_sel)
{
    reg_audio_anc_config(anc_chn) = (reg_audio_anc_config(anc_chn) & (~FLD_ANC_SPK_SEL)) | MASK_VAL(FLD_ANC_SPK_SEL, mode_sel);
}

/**
 * @brief      This function serves to set resample output fs.
 *
 * @param[in]  anc_chn - anc channel.
 * @param[in]  out_fs  - resample output fs.
 * @return     none
 */
static inline void audio_anc_set_resample_out_fs(audio_anc_chn_e anc_chn, audio_anc_resample_out_fs_e out_fs)
{
    reg_audio_anc_config(anc_chn) = (reg_audio_anc_config(anc_chn) & (~FLD_RE_OUTMODE_SEL)) | MASK_VAL(FLD_RE_OUTMODE_SEL, out_fs);
}

/**
 * @brief      This function serves to set resample input fs.
 *
 * @param[in]  anc_chn - anc channel.
 * @param[in]  in_fs   - resample input fs.
 * @return     none
 */
static inline void audio_anc_set_resample_in_fs(audio_anc_chn_e anc_chn, audio_anc_resample_in_fs_e in_fs)
{
    reg_audio_anc_config(anc_chn) = (reg_audio_anc_config(anc_chn) & (~FLD_RE_INMODE_SEL)) | MASK_VAL(FLD_RE_INMODE_SEL, in_fs);
}

/**
 * @brief      This function serves to set anc count mode.
 * 
 * @param[in]  anc_chn - anc channel.
 * @param[in]  in_fs   - anc input fs.
 * @param[in]  out_fs  - anc output fs.
 * @return     none
 */
void audio_anc_set_dac_cnt_mode(audio_anc_chn_e anc_chn, audio_anc_resample_in_fs_e in_fs, audio_anc_resample_out_fs_e out_fs);

/**
 * @brief      This function serves to select who decides the resample frequency of anc.
 *
 * @param[in]  anc_chn     - anc channel.
 * @param[in]  fs_decision - who decides the resample frequency of anc.
 * @param[in]  in_fs       - resample input fs.
 * @param[in]  out_fs      - resample output fs.
 */
void audio_anc_set_resample_in_out_fs(audio_anc_chn_e anc_chn, audio_anc_resample_fs_decision_e fs_decision, audio_anc_resample_in_fs_e in_fs,
                                      audio_anc_resample_out_fs_e out_fs);

/**
 * @brief      This function serves to latch ref mic gain.
 *
 * @param[in]  anc_chn        - anc channel.
 * @return     none
 */
static inline void audio_anc_latch_ref_mic_gain(audio_anc_chn_e anc_chn)
{
    BM_SET(reg_audio_anc_config1(anc_chn), FLD_ANC_REF_GAIN_SHIFT_LATCH);
}

/**
 * @brief      This function serves to set anc ref mic gain ref_mic_gain = ref_gain >> ref_gain_shift.
 * 
 * @param[in]  anc_chn        - anc channel.
 * @param[in]  ref_gain       - ref_gain value bit[0-27] valid.
 * @param[in]  ref_gain_shift - ref_gain shift bit[0-5] valid.
 * @return     none
 */
static inline void audio_anc_set_ref_mic_gain(audio_anc_chn_e anc_chn, unsigned int ref_gain, unsigned char ref_gain_shift)
{
    reg_audio_anc_ref_gain(anc_chn)       = ref_gain & FLD_ANC_REF_GAIN;
    reg_audio_anc_ref_gain_shift(anc_chn) = ref_gain_shift & FLD_ANC_REF_GAIN_SHIFT;
    audio_anc_latch_ref_mic_gain(anc_chn);
}

/**
 * @brief      This function serves to latch ref fir gain.
 *
 * @param[in]  anc_chn       - anc channel.
 * @return     none
 */
static inline void audio_anc_latch_wz_fir_gain(audio_anc_chn_e anc_chn)
{
    BM_SET(reg_audio_anc_config1(anc_chn), FLD_ANC_WZ_GAIN_SHIFT_LATCH);
}

/**
 * @brief      This function serves to set anc wz fir gain wz_fir_gain = wz_gain >> wz_gain_shift.
 * 
 * @param[in]  anc_chn       - anc channel.
 * @param[in]  wz_gain       - wz_gain value bit[0-27] valid.
 * @param[in]  wz_gain_shift - wz_gain_shift bit[0-5] valid.
 * @return     none
 */
static inline void audio_anc_set_wz_fir_gain(audio_anc_chn_e anc_chn, unsigned int wz_gain, unsigned char wz_gain_shift)
{
    reg_audio_anc_wz_gain(anc_chn)        = wz_gain & FLD_ANC_WZ_GAIN;
    reg_audio_anc_gain_mul_shift(anc_chn) = wz_gain_shift & FLD_ANC_GAIN_MUL_SHIFT;
    audio_anc_latch_wz_fir_gain(anc_chn);
}

/**
 * @brief      This function serves to set anc wz iir taps.
 *
 * @param[in]  anc_chn - anc channel.
 * @param[in]  step    - anc wz fir step.
 * @return     none
 */
static inline void audio_anc_set_wz_iir_taps(audio_anc_chn_e anc_chn, unsigned short num)
{
    reg_audio_anc_wz_iir_taps(anc_chn) = (reg_audio_anc_wz_iir_taps(anc_chn) & (~FLD_ANC_WZ_IIR_TAPS)) | MASK_VAL(FLD_ANC_WZ_IIR_TAPS, num);
}

/**
 * @brief      This function serves to set anc wz fir taps.
 * 
 * @param[in]  anc_chn - anc channel.
 * @param[in]  num     - anc wz fir step.
 * @return     none
 */
static inline void audio_anc_set_wz_fir_taps(audio_anc_chn_e anc_chn, unsigned short num)
{
    reg_audio_anc_wz_iir_taps(anc_chn) = (reg_audio_anc_wz_iir_taps(anc_chn) & (~FLD_ANC_WZ_FIR_TAPS_BIT8)) | ((num & BIT(8))>>1);
    reg_audio_anc_wz_fir_taps(anc_chn) = num & FLD_ANC_WZ_FIR_TAPS;
}

/**
 * @brief      This function servers to update anc wz biquad iir filter coefficients.
 * 
 * @param[in]  anc_chn - anc channel.
 * @param[in]  data    - wz biquad iir filter data.
 * @param[in]  taps    - wz biquad iir filter taps.
 * @return     none
 */
void audio_anc_update_wz_iir_coef(audio_anc_chn_e anc_chn, signed int data[24][5], unsigned short taps);

/**
 * @brief      This function servers to set anc wz fir filter coefficients.
 *
 * @param[in]  anc_chn  - anc channel.
 * @param[in]  data     - wz fir filter data address.
 * @param[in]  data_len - wz fir data len max length is 384(wz fir max is 384taps).
 * @note
 *             - bypass coefficients: wz[0] = 0x4000, the rest of the parameters are all set to 0
 * @return     none
 */
void audio_anc_update_wz_fir_coef(audio_anc_chn_e anc_chn, signed short *data, unsigned short data_len);

/**
 * @brief      This function serves to set anc cz iir taps.
 * 
 * @param[in]  anc_chn - anc channel.
 * @param[in]  cz      - cz select.
 * @param[in]  num     - anc cz iir taps.
 * @return     none
 */
static inline void audio_anc_set_cz_iir_taps(audio_anc_chn_e anc_chn, unsigned char cz, unsigned char num)
{
    reg_audio_anc_cz_iir_taps(anc_chn, cz) = (reg_audio_anc_cz_iir_taps(anc_chn, cz) & (~FLD_ANC_CZ_IIR_TAPS)) | MASK_VAL(FLD_ANC_CZ_IIR_TAPS, num);
}

/**
 * @brief      This function serves to set anc cz fir taps.
 *
 * @param[in]  anc_chn - anc channel.
 * @param[in]  cz      - cz select.
 * @param[in]  num     - anc cz fir taps.
 * @return     none
 */
static inline void audio_anc_set_cz_fir_taps(audio_anc_chn_e anc_chn, unsigned char cz, unsigned char num)
{
    reg_audio_anc_cz_fir_taps(anc_chn, cz) = num;
}

/**
 * @brief      This function servers to update anc cz biquad iir filter coefficients.
 * 
 * @param[in]  anc_chn - anc channel.
 * @param[in]  cz      - cz select.
 * @param[in]  czbiq   - cz biquad iir filter data address, [b0, b1, b2, a1, a2].
 * @param[in]  taps    - cz biquad taps.
 * @return     none
 */
void audio_anc_update_cz_iir_coef(audio_anc_chn_e anc_chn, unsigned char cz, signed int czbiq[24][5],unsigned char taps);

/**
 * @brief      This function servers to update anc cz fir filter coefficients.
 * 
 * @param[in]  anc_chn  - anc channel.
 * @param[in]  data     - cz fir filter data address.
 * @param[in]  data_len - cz fir data len max length is 150(cz fir max step is 150taps).
 * @return     none
 */
void audio_anc_update_cz_fir_coef(audio_anc_chn_e anc_chn, unsigned char cz, signed short *data, unsigned short data_len);

/**
 * @brief      This function servers to update anc hb1 coefficients.
 * @param[in]  anc_chn - anc channel.
 * @param[in]  data    - coefficient data address.
 * @note
 *             - anc hb1 coefficient length is 21 word, bit[0-25] valid.
 */
void audio_anc_update_hb1_coef(audio_anc_chn_e anc_chn, signed int *data);

/**
 * @brief      This function servers to update anc hb2 coefficients.
 * @param[in]  anc_chn - anc channel.
 * @param[in]  data    - coefficient data address.
 * @note
 *             - anc hb2 coefficient length is 7 word, bit[0-25] valid.
 */
void audio_anc_update_hb2_coef(audio_anc_chn_e anc_chn, signed int *data);

/**
 * @brief      This function servers to update anc hb3 coefficients.
 * @param[in]  anc_chn - anc channel.
 * @param[in]  data    - coefficient data address.
 * @note
 *             - anc hb3 coefficient length is 7 word, bit[0-25] valid.
 */
void audio_anc_update_hb3_coef(audio_anc_chn_e anc_chn, signed int *data);

/**
 * @brief      This function servers to update anc droop coefficients.
 * @param[in]  anc_chn - anc channel.
 * @param[in]  data    - coefficient data address.
 * @note
 *             - anc droop coefficient length is 5 word, bit[0-13] valid.
 */
void audio_anc_update_droop_coef(audio_anc_chn_e anc_chn, signed short *data);

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio asrc interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio asrc interface.
 * @{
 */

/**
 * @brief      This function serves to update asrc half_band1 coefficients.
 * 
 * @param[in]  asrc_chn - asrc channel.
 * @param[in]  hb1_coef - asrc half_band1 coefficients data address.
 * @return     none
 * @note
 *             - asrc half_band1 coefficient length is 32 word, bit[0-25] valid.
 */
void audio_asrc_update_hb1_coef(audio_asrc_chn_e asrc_chn, signed int *hb1_coef);

/**
 * @brief      This function serves to update asrc half_band2 coefficients.
 * 
 * @param[in]  asrc_chn - asrc channel.
 * @param[in]  hb2_coef - asrc half_band2 coefficients data address. 
 * @return     none
 * @note
 *             - asrc half_band2 coefficient length is 7 word, bit[0-25] valid.
 */
void audio_asrc_update_hb2_coef(audio_asrc_chn_e asrc_chn, signed int *hb2_coef);

/**
 * @brief      This function serves to set asrc droop step.
 *
 * @param[in]  asrc_chn - asrc channel select.
 * @param[in]  step     - asrc droop step audio_asrc_droop_step_e.
 * @return     none
 */
static inline void audio_asrc_set_droop_step(audio_asrc_chn_e asrc_chn, audio_asrc_droop_step_e step)
{
    reg_audio_asrc_drop_len_mod(asrc_chn) = (reg_audio_asrc_drop_len_mod(asrc_chn) & (~FLD_ASRC_DROP_LEN_MOD)) | step;
}

/**
 * @brief      This function serves to update asrc droop coefficients.
 *
 * @param[in]  asrc_chn  - asrc channel select.
 * @param[in]  d_coef    - asrc droop coefficients data address.
 * @param[in]  data_len  - asrc droop coefficients data length.
 * @return     none
 * @note
 *             - droop coef max length is 9.
 */
void audio_asrc_update_droop_coef(audio_asrc_chn_e asrc_chn, signed short *d_coef, unsigned char data_len);

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio clock interface                                               *
 *********************************************************************************************************************/
/*!
 * @name Audio clock interface.
 * @{
 */

/**
 * @brief      This function serves to initialize audio.
 * @param[in]  audio_pll - audio pll clock select.
 * @return     none
 * @note       - When using the audio module, this interface must be configured first, otherwise the following interfaces will not take effect.
 *             - When the sampling rate is 44.1KHz, audio_pll needs to be set to AUDIO_PLL_CLK_33P8688M or AUDIO_PLL_CLK_169P344M.
 */
void audio_init(pll_audio_clk_e clk);

/**
 * @brief      This function serves set audio clk.The audio clk is divided by pll1_clk. When the internal codec operates at a sampling rate of 44.1KHz, \n
 *             audio_clk needs to be set to 33.8688MHz, and the remaining sampling rate audio_clk needs to be set to 36.864MHz.
 * @param[in]  clk_div - pll1_clk div.
 * @return     none
 */
static inline void audio_clk_set(unsigned int clk_div)
{
    reg_audio_clk_aclk_set = (reg_audio_clk_aclk_set & (~FLD_CLK_ACLK_SET)) | (clk_div & FLD_CLK_ACLK_SET);
    BM_SET(reg_audio_clk_en_0, FLD_CLK_ACLK_EN);
}

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio codec interface                                               *
 *********************************************************************************************************************/
/*!
 * @name Audio codec0 interface.
 * @{
 */

/**
 * @brief      This function serves to power on codec0.
 * @param[in]  power_mode - codec0 power mode selection.
 * @param[in]  volt       - codec0 analog voltage selection.
 * @return     none
 */
void audio_codec0_power_on(audio_codec0_power_e power_mode, audio_codec0_volt_supply_e volt);

/**
 * @brief      This function serves to power on codec1.
 * @param[in]  power_mode - codec1 power mode selection.
 * @return     none
 */
void audio_codec1_power_on(audio_codec1_power_e power_mode);

/**
 * @brief      This function serves to power down codec0 adc.
 * @param[in]  adc - adc channel.
 * @return     none
 * @note
 *             - This interface only powers down the analog ADC.
 */
void audio_codec0_adc_power_down(audio_codec0_input_select_e adc);

/**
 * @brief      This function serves to power on codec0 output.
 * @param[in]  output - output channel.
 * @return     none
 */
void audio_codec0_output_power_down(audio_codec0_output_select_e output);

/**
 * @brief      This function serves to power down codec0.
 * @return     none
 * @note
 *             - After power down, cannot access codec0 registers.
 */
void audio_codec0_power_down(void);

/**
  * @brief      This function serves to power down codec1.
  * @return     none
  * @note
  *             - After power down, cannot access codec1 registers.
  */
void audio_codec1_power_down(void);

/**
 * @brief      This function serves to set codec0 voltage supply.
 * @param[in]  voltage - codec0 voltage supply select.
 *
 */
static inline void audio_codec0_set_vol_supply(audio_codec0_volt_supply_e voltage)
{
    reg_audio_codec0_cr_vic = (reg_audio_codec0_cr_vic & (~FLD_CODEC0_AVD_1V8)) | MASK_VAL(FLD_CODEC0_AVD_1V8, voltage);
}

/**
 * @brief      This function serves to set codec0 dmic0 clock frequency.
 * @param[in]  clk_sel - dmic0 clock frequency.
 * @note
 *             - keep to AUDIO_CODEC0_DMIC_CLK_3072KHZ when FLD_CODEC0_ADCA_POWER_MODE = 00(Normal mode) and to AUDIO_CODEC0_DMIC_CLK_1024KHZ else(Low power mode or Ultra low power mode).
 *             - dmic0_clk1 and dmic0_clk1 set in pairs.
 */
static inline void audio_codec0_set_dmic_a_clk(audio_codec_dmic_clk_sel_e clk_sel)
{
    reg_audio_codec0_cr_dmic_adca_12_rate = (reg_audio_codec0_cr_dmic_adca_12_rate & (~FLD_CODEC0_ADCA12_DMIC_RATE)) |
                                            MASK_VAL(FLD_CODEC0_ADCA12_DMIC_RATE, clk_sel);
}

/**
 * @brief      This function serves to set codec0 dmic1 clock frequency.
 * @param[in]  clk_sel - dmic1 clock frequency.
 * @note
 *             - keep to AUDIO_CODEC0_DMIC_CLK_3072KHZ when FLD_CODEC0_ADCB_POWER_MODE = 00(Normal mode) and to AUDIO_CODEC0_DMIC_CLK_1024KHZ else(Low power mode or Ultra low power mode).
 *             - dmic1_clk1 and dmic0_clk1 set in pairs.
 */
static inline void audio_codec0_set_dmic_b_clk(audio_codec_dmic_clk_sel_e clk_sel)
{
    reg_audio_codec0_cr_dmic_adcb_12_rate = (reg_audio_codec0_cr_dmic_adcb_12_rate & (~FLD_CODEC0_ADCB12_DMIC_RATE)) |
                                            MASK_VAL(FLD_CODEC0_ADCB12_DMIC_RATE, clk_sel);
}

/**
 * @brief      This function serves to enable/disable codec0 dmic clock.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: active dmic clock, 0: power-down dmic clock.
 */
void audio_codec0_dmic_clk_en(audio_codec0_input_select_e input, unsigned char enable);

/**
 * @brief      This function serves to enable/disable codec1 dmic clock.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: active dmic clock, 0: power-down dmic clock.
 */
void audio_codec1_dmic_clk_en(audio_codec1_input_select_e input, unsigned char enable);

/**
 * @brief      This function serves to enable/disable codec0 micbias output.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: enable micbias, 0: disable micbias.
 * @return     none
 * @note
 *             - bias only for amic.
 */
void audio_codec0_set_micbias(audio_codec0_input_select_e input, unsigned char enable);

/**
 * @brief      This function serves to set codec0 micbias output state when Microphone bias in power-down (reg_audio_codec0_cr_adc_mic[5], SB_MICBIAS = 1).
 * @param[in]  input  - input channel.
 * @param[in]  state - AUDIO_AMIC_BIAS_PULL_DOWN_ZERO or AUDIO_AMIC_BIAS_HIGH_IMPEDANCE.
 * @return     none
 * @note       If all the bias are connected together (BGA-packaged chip), it is recommended to configure the other unused bias to high resistance
 *             when a bias is being used to prevent current leakage. By default, the codec0 micbias has been configured to high-impedance after powered on if codec0 amic is used.
 *             - bias only for amic.
 */
void audio_codec0_set_micbias_state(audio_codec0_input_select_e input, audio_codec0_bias_output_state_e state);

/**
 * @brief      This function serves to set codec0 adc mode.
 * @param[in]  adc            - adc channel.
 * @param[in]  capacitor_mode - 1: Capacitor-coupled input, 0: Capacitor-less input (direct connection).
 * @return     none
 * @note
 *             - capacitor mode only for line_in or amic.
 */
void audio_codec0_set_adc_capacitor_mode(audio_codec0_input_select_e adc, audio_codec0_adc_capacitor_mode_e capacitor_mode);

/**
 * @brief      This function serves to set codec0 adc mode.
 * @param[in]  adc      - adc channel.
 * @param[in]  adc_mode - 1: differential input, 0: single-ended input.
 * @return     none
 * @note
 *             - adc mode only for line_in or amic.
 */
void audio_codec0_set_adc_mode(audio_codec0_input_select_e adc, audio_codec0_adc_mode_e adc_mode);

/**
 * @brief      This function serves to set codec0 adc power mode.
 * @param[in]  adc  - adc channel.
 * @param[in]  mode - adc power mode.
 * @return     none
 * @note
 *             - This interface only set line-in and amic power mode.
 */
void audio_codec0_set_adc_power_mode(audio_codec0_input_select_e adc, audio_codec0_adc_power_mode_e mode);

/**
 * @brief      This function serves to set codec0 input mute.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: soft mute active, 0: soft mute inactive.
 * @return     none
 */
void audio_codec0_set_input_mute(audio_codec0_input_select_e input, unsigned char enable);

/**
 * @brief      This function serves to set codec1 input mute.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: soft mute active, 0: soft mute inactive.
 * @return     none
 */
void audio_codec1_set_input_mute(audio_codec1_input_select_e input, unsigned char enable);

/**
 * @brief      This function serves to set codec0 output mute.
 * @param[in]  output - output channel.
 * @param[in]  enable - 1: soft mute active, 0: soft mute inactive.
 * @return     none
 */
void audio_codec0_set_output_mute(audio_codec0_output_select_e output, unsigned char enable);

/**
 * @brief      This function serves to set codec0 adc filter mode.
 * @param[in]  input - input channel.
 * @param[in]  fmode - filter mode.
 * @return     none
 */
void audio_codec0_set_input_filter_mode(audio_codec0_input_select_e input, audio_filter_mode_e fmode);

/**
 * @brief      This function serves to set codec0 input sample rate.
 * @param[in]  input - input channel.
 * @param[in]  fs    - input sample rate.
 * @return     none
 * @note
 *             - ADC_A1/2 or ADC_B1/2 sample rates are set in pairs.
 */
void audio_codec0_set_input_fs(audio_codec0_input_select_e input, audio_sample_rate_e fs);

/**
 * @brief      This function serves to set codec1 input sample rate.
 * @param[in]  input - input channel.
 * @param[in]  fs    - input sample rate.
 * @return     none
 * @note
 *             - ADC_A1/2 sample rates are set in pairs.
 */
void audio_codec1_set_input_fs(audio_sample_rate_e fs);

/**
 * @brief      This function serves to set codec0 dac filter mode.
 * @param[in]  fmode - filter mode.
 * @return     none
 */
void audio_codec0_set_output_filter_mode(audio_filter_mode_e fmode);

/**
 * @brief      This function serves to set codec0 output sample rate.
 * @param[in]  fs - output sample rate.
 * @return     none
 * @note
 *             - DAC_A1/2 sample rates are set in pairs.
 */
void audio_codec0_set_output_fs(audio_sample_rate_e fs);

/**
 * @brief      This function serves to set codec0 input data world length.
 * @param[in]  input - input channel.
 * @param[in]  wl    - world length.
 * @return     none
 * @note
 *             - ADC_A1/2 or ADC_B1/2 world length are set in pairs.
 */
void audio_codec0_set_input_wl(audio_codec0_input_select_e input, audio_codec0_data_select_e wl);

/**
 * @brief      This function serves to set codec1 input data world length.
 * @param[in]  input - input channel.
 * @param[in]  wl    - world length.
 * @return     none
 * @note
 *             - ADC_A1/2 world length are set in pairs.
 */
void audio_codec1_set_input_wl(audio_codec1_data_select_e wl);

/**
 * @brief      This function serves to set codec0 output data world length.
 * @param[in]  output - output channel.
 * @param[in]  wl     - world length.
 * @return     none
 */
void audio_codec0_set_output_wl(audio_codec0_output_select_e output, audio_codec0_data_select_e wl);

/**
 * @brief      This function serves to set codec0 input analog gain.
 * @param[in]  input - input channel.
 * @param[in]  gain  - input analog gain.
 * @return     none
 * @note
 *             - input analog gain only for line_in or amic.
 */
void audio_codec0_set_input_again(audio_codec0_input_select_e input, audio_codec0_input_again_e gain);

/**
 * @brief      This function serves to set codec0 input digital gain.
 * @param[in]  input - input channel.
 * @param[in]  gain  - input digital gain.
 * @return     none
 */
void audio_codec0_set_input_dgain(audio_codec0_input_select_e input, audio_codec_input_dgain_e gain);

/**
 * @brief      This function serves to set codec1 input digital gain.
 * @param[in]  input - input channel.
 * @param[in]  gain  - input digital gain.
 * @return     none
 */
void audio_codec1_set_input_dgain(audio_codec1_input_select_e input, audio_codec_input_dgain_e gain);

/**
 * @brief      This function serves to set codec0 output analog gain.
 * @param[in]  output - output channel.
 * @param[in]  gain   - output analog gain.
 * @return     none
 */
void audio_codec0_set_output_again(audio_codec0_output_select_e output, audio_codec0_output_again_e gain);

/**
 * @brief      This function serves to set codec0 output digital gain.
 * @param[in]  output - output channel.
 * @param[in]  gain   - output digital gain.
 * @return     none
 */
void audio_codec0_set_output_dgain(audio_codec0_output_select_e output, audio_codec0_output_dgain_e gain);

/**
 * @brief      This function serves to enable/disable codec0 input SNR optimisation.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: adc SNR optimisation active, 0:adc SNR optimisation inactive.
 * @return     none
 * @note
 *             - ADC_A1/2 or ADC_B1/2 SNR optimisation are set in pairs.
 */
void audio_codec0_set_input_snr_opt(audio_codec0_input_select_e input, unsigned char enable);

/**
 * @brief      This function serves to enable/disable codec0 output SNR optimisation.
 * @param[in]  enable - 1: dac SNR optimisation active, 0:dac SNR optimisation inactive.
 * @return     none
 * @note
 *             - DAC_A1/2 SNR optimisation are set in pairs.
 */
static inline void audio_codec0_set_output_snr_opt(unsigned char enable)
{
    reg_audio_codec_dac_cr_daca12_agc = (reg_audio_codec_dac_cr_daca12_agc & (~FLD_CODEC_DAC_DACA12_SNR_OPT_EN)) |
                                        MASK_VAL(FLD_CODEC_DAC_DACA12_SNR_OPT_EN, enable);
}

/**
 * @brief      This function serves to enable/disable codec0 input.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: enable, 0: disable.
 * @return     none
 * @note
 *             - Must be disable when switching the input sample rate.
 *             - ADC_A1/2 or ADC_B1/2 are set in pairs.
 */
void audio_codec0_input_en(audio_codec0_input_select_e input, unsigned char enable);

/**
 * @brief      This function serves to enable/disable codec1 input.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: enable, 0: disable.
 * @return     none
 * @note
 *             - Must be disable when switching the input sample rate.
 *             - ADC_A1/2 are set in pairs.
 */
void audio_codec1_input_en(unsigned char enable);

/**
 * @brief      This function serves to enable/disable codec0 output.
 * @param[in]  enable - 1: enable, 0: disable.
 * @return     none
 * @note
 *             - Must be disable when switching the output sample rate.
 *             - DAC_A1/2 are set in pairs.
 */
static inline void audio_codec0_output_en(unsigned char enable)
{
    reg_audio_codec_ctrl = (reg_audio_codec_ctrl & (~FLD_CODEC_CTRL_DAC_MST_EN)) | MASK_VAL(FLD_CODEC_CTRL_DAC_MST_EN, enable);
}

/**
 * @brief      This function serves to enable/disable codec0 input HPF(High Pass Filter).
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: adc High Pass Filter active, 0:adc High Pass Filter inactive.
 * @return     none
 * @note
 *             - ADC_A1/2 or ADC_B1/2 High Pass Filter are set in pairs.
 */
void audio_codec0_input_hpf_en(audio_codec0_input_select_e input, unsigned char enable);

/**
 * @brief      This function serves to enable/disable codec1 input HPF(High Pass Filter).
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: adc High Pass Filter active, 0:adc High Pass Filter inactive.
 * @return     none
 * @note
 *             - ADC_A1/2 High Pass Filter are set in pairs.
 */
void audio_codec1_input_hpf_en(unsigned char enable);

/**
 * @brief      This function serves to init codec0 input.
 * @param[in]  input_config - codec0 input config.
 * @return     none
 */
void audio_codec0_input_init(audio_codec0_input_config_t *input_config);

/**
 * @brief      This function serves to init codec1 input.
 * @param[in]  input_config - codec0 input config.
 * @return     none
 */
void audio_codec1_input_init(audio_codec1_input_config_t *input_config);

/**
 * @brief      This function serves to init codec0 output config.
 * @param[in]  output_config - codec0 output config.
 * @return     none
 */
void audio_codec0_output_init(audio_codec0_output_config_t *output_config);

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio dma/fifo interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio ama/fifo interface.
 * @{
 */

/**
 * @brief      This function serves to tx fifo dma trigger number.
 * @param[in]  tx_fifo_chn - the fifo channel.
 * @param[in]  number      - the number of dma trigger, the unit is word.
 * @return     none
 */
static inline void audio_set_fifo_tx_trig_num(audio_fifo_chn_e tx_fifo_chn, unsigned char number)
{
    reg_audio_dma_tx_fifo_trig_num(tx_fifo_chn) = (reg_audio_dma_tx_fifo_trig_num(tx_fifo_chn) & (~FLD_TX_FIFO_TRIG_NUM)) |
                                                  (number & FLD_TX_FIFO_TRIG_NUM);
}

/**
 * @brief      This function serves to rx fifo dma trigger number.
 * @param[in]  rx_fifo_chn - the fifo channel.
 * @param[in]  number      - the number of dma trigger, the unit is word.
 * @return     none
 */
static inline void audio_set_fifo_rx_trig_num(audio_fifo_chn_e rx_fifo_chn, unsigned char number)
{
    reg_audio_dma_rx_fifo_trig_num(rx_fifo_chn) = (reg_audio_dma_rx_fifo_trig_num(rx_fifo_chn) & (~FLD_RX_FIFO_TRIG_NUM)) |
                                                  (number & FLD_RX_FIFO_TRIG_NUM);
}

/**
 * @brief      This function serves to clear fifo data.
 * @param[in]  fifo_chn   - fifo channel
 * @param[in]  clear_flag
 *                        - 1: audio fifo cnt clear
 *                        - 0: audio fifo cnt clear release.
 * @return     none.
 */
static inline void audio_clear_fifo(audio_fifo_type_e fifo_chn, char clear_flag)
{
    reg_audio_dma_fifo_clr = reg_audio_dma_fifo_clr | MASK_VAL(fifo_chn, clear_flag);
}

/**
 * @brief      This function serves to enable read fifo ptr.
 * @param[in]  fifo_chn - fifo channel
 * @return     none
 */
static inline void audio_fifo_ptr_en(audio_fifo_type_e fifo_chn)
{
    BM_SET(reg_audio_dma_ptr_en, fifo_chn);
}

/**
 * @brief      This function serves to disable read fifo ptr.
 * @param[in]  fifo_chn - fifo channel
 * @return     none
 */
static inline void audio_fifo_ptr_dis(audio_fifo_type_e fifo_chn)
{
    BM_CLR(reg_audio_dma_ptr_en, fifo_chn);
}

/**
 * @brief      This function servers to get audio fifo irq status.
 * @param[in]  fifo_type - the audio fifo type.
 * @return     irq status of audio fifo.
 */
static inline unsigned char audio_get_fifo_irq_status(audio_fifo_type_e fifo_type)
{
    return reg_audio_dma_irq_st & fifo_type;
}

/**
 * @brief      This function servers to clear audio fifo irq status.
 * @param[in]  fifo_type - the audio fifo type.
 * @return     none
 */
static inline void audio_clr_fifo_irq_status(audio_fifo_type_e fifo_type)
{
    reg_audio_dma_irq_st = fifo_type;
}

/**
 * @brief      This function serves to enable audio fifo irq.
 * @param[in]  fifo_chn - audio fifo channel.
 * @return     none
 */
static inline void audio_fifo_irq_en(audio_fifo_type_e fifo_chn)
{
    BM_SET(reg_audio_dma_irq_en, fifo_chn);
}

/**
 * @brief      This function serves to disable audio fifo irq.
 * @param[in]  fifo_chn - audio fifo channel.
 * @return     none
 */
static inline void audio_fifo_irq_dis(audio_fifo_type_e fifo_chn)
{
    BM_CLR(reg_audio_dma_irq_en, fifo_chn);
}

/**
 * @brief      This function serves to get tx read pointer.
 * @param[in]  tx_fifo_chn - select fifo channel
 * @return     the result of tx read pointer.
 */
static inline unsigned short audio_get_tx_rptr(audio_fifo_chn_e tx_fifo_chn)
{
    return reg_audio_dma_tx_rptr(tx_fifo_chn);
}

/**
 * @brief      This function serves to set tx read pointer.
 * @param[in]  tx_fifo_chn - select fifo channel
 * @return     the result of tx read pointer.
 */
static inline void audio_set_tx_rptr(audio_fifo_chn_e tx_fifo_chn, unsigned short value)
{
    reg_audio_dma_tx_rptr(tx_fifo_chn) = value;
}

/**
 * @brief      This function serves to get rx write pointer.
 * @param[in]  rx_fifo_chn - select fifo channel
 * @return     the result of rx write pointer.
 */
static inline unsigned short audio_get_rx_wptr(audio_fifo_chn_e rx_fifo_chn)
{
    return reg_audio_dma_rx_wptr(rx_fifo_chn);
}

/**
 * @brief      This function serves to set rx write pointer.
 * @param[in]  rx_fifo_chn - select fifo channel
 * @return     the result of rx write pointer.
 */
static inline void audio_set_rx_wptr(audio_fifo_chn_e rx_fifo_chn, unsigned short value)
{
    reg_audio_dma_rx_wptr(rx_fifo_chn) = value;
}

/**
 * @brief      This function serves to set tx buff length.
 * @param[in]  tx_fifo_chn - the fifo channel.
 * @param[in]  len         - the length of tx buff, the unit is byte.
 * @return     none
 */
static inline void audio_set_tx_buff_len(audio_fifo_chn_e tx_fifo_chn, unsigned short len)
{
    reg_audio_dma_tx_max(tx_fifo_chn) = (((len) >> 2) - 1);
}

/**
 * @brief      This function serves to set rx buff length.
 * @param[in]  rx_fifo_chn - the fifo channel.
 * @param[in]  len         - the length of rx buff, the unit is byte.
 * @return     none
 */
static inline void audio_set_rx_buff_len(audio_fifo_chn_e rx_fifo_chn, unsigned short len)
{
    reg_audio_dma_rx_max(rx_fifo_chn) = ((len) >> 2) - 1;
}

/**
 * @brief      This function serves to set tx buff threshold.
 * @param[in]  tx_fifo_chn - the fifo channel.
 * @param[in]  threshold   - the threshold of tx buff, the unit is byte.
 * @return     none
 */
static inline void audio_set_tx_buff_thres(audio_fifo_chn_e tx_fifo_chn, unsigned short threshold)
{
    reg_audio_dma_tx_th(tx_fifo_chn) = ((threshold) >> 2) - 1;
}

/**
 * @brief      This function serves to set rx buff threshold.
 * @param[in]  rx_fifo_chn - the fifo channel.
 * @param[in]  threshold   - the threshold of rx buff, the unit is byte.
 * @return     none
 */
static inline void audio_set_rx_buff_thres(audio_fifo_chn_e rx_fifo_chn, unsigned short threshold)
{
    reg_audio_dma_rx_th(rx_fifo_chn) = ((threshold) >> 2) - 1;
}

/**
 *  @brief      This function serves to get dma tx buff pointer.
 *  @param[in]  chn - dma channel
 *  @return     the result of tx read pointer.
 */
static inline unsigned int audio_get_tx_dma_rptr(dma_chn_e chn)
{
    return reg_dma_src_addr(chn);
}

/**
 *  @brief      This function serves to get dma rx buff pointer.
 *  @param[in]  chn - dma channel
 *  @return     the result of rx write pointer.
 */
static inline unsigned int audio_get_rx_dma_wptr(dma_chn_e chn)
{
    return reg_dma_dst_addr(chn);
}

/**
 * @brief      This function serves to enable rx_dma channel.
 * @param[in]  chn   - dma channel.
 * @return     none
 */
static inline void audio_rx_dma_en(dma_chn_e chn)
{
    dma_chn_en(chn);
}

/**
  * @brief      This function serves to disable rx_dma channel.
  * @param[in]  chn   - dma channel.
  * @return     none
  */
static inline void audio_rx_dma_dis(dma_chn_e chn)
{
    dma_chn_dis(chn);
}

/**
 * @brief      This function serves to enable tx_dma channel.
 * @param[in]  chn   - dma channel.
 * @return     none
 */
static inline void audio_tx_dma_en(dma_chn_e chn)
{
    dma_chn_en(chn);
}

/**
 * @brief      This function serves to disable dis_dma channel.
 * @param[in]  chn   - dma channel.
 * @return     none
 */
static inline void audio_tx_dma_dis(dma_chn_e chn)
{
    dma_chn_dis(chn);
}

/**
 * @brief      This function serves to config rx_dma channel.
 * @param[in]  chn          - dma channel.
 * @param[in]  dst_addr     - Pointer to data buffer, it must be 4-bytes aligned address.
 *                           and the actual buffer size defined by the user needs to be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in]  data_len     - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @param[in]  head_of_list - the head address of dma llp.
 * @return     none
 */
void audio_rx_dma_config(dma_chn_e chn, unsigned short *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list);

/**
 * @brief      This function serves to set rx dma chain transfer.
 * @param[in]  config_addr - the head of list of llp_pointer.
 * @param[in]  llpointer   - the next element of llp_pointer.
 * @param[in]  dst_addr    - Pointer to data buffer, it must be 4-bytes aligned address and the actual buffer size defined by the user needs to \n
 *                           be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in]  data_len    - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @return     none
 */
void audio_rx_dma_add_list_element(dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned short *dst_addr, unsigned int data_len);

/**
 * @brief      This function serves to set audio rx dma chain transfer.
 * @param[in]  rx_fifo_chn - rx fifo select.
 * @param[in]  chn         - dma channel.
 * @param[in]  in_buff     - Pointer to data buffer, it must be 4-bytes aligned address and the actual buffer size defined by the user needs to \n
 *                           be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in]  buff_size   - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @return     none
 */
void audio_rx_dma_chain_init(audio_fifo_chn_e rx_fifo_chn, dma_chn_e chn, unsigned short *in_buff, unsigned int buff_size);

/**
 * @brief      This function serves to config  tx_dma channel.
 * @param[in]  chn          - dma channel.
 * @param[in]  src_addr     - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in]  data_len     - Length of DMA in bytes, range from 1 to 0x10000.
 * @param[in]  head_of_list - the head address of dma llp.
 * @return     none
 */
void audio_tx_dma_config(dma_chn_e chn, unsigned short *src_addr, unsigned int data_len, dma_chain_config_t *head_of_list);

/**
 * @brief      This function serves to set tx dma chain transfer.
 * @param[in]  config_addr - the head of list of llp_pointer.
 * @param[in]  llpointer   - the next element of llp_pointer.
 * @param[in]  src_addr    - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in]  data_len    - Length of DMA in bytes, range from 1 to 0x10000.
 * @return     none
 */
void audio_tx_dma_add_list_element(dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned short *src_addr, unsigned int data_len);

/**
 * @brief      This function serves to initialize audio tx dma chain transfer.
 * @param[in]  tx_fifo_chn - tx fifo select.
 * @param[in]  chn         - dma channel.
 * @param[in]  out_buff    - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in]  buff_size   - Length of DMA in bytes, range from 1 to 0x10000.
 * @return     none
 */
void audio_tx_dma_chain_init(audio_fifo_chn_e tx_fifo_chn, dma_chn_e chn, unsigned short *out_buff, unsigned int buff_size);

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio hac interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio hac interface.
 * @{
 */
/**
 * @brief      This function servers to enable/disable hac clock.
 *
 * @param[in]  hac_chn    - hac channel.
 * @return     none
 */
static inline void audio_hac_clk_en(audio_hac_chn_e hac_chn)
{
    reg_audio_clk_en_1 |= BIT(hac_chn);
}

/**
 * @brief      This function servers to enable/disable hac reset.
 *
 * @param[in]  hac_chn    - hac channel.
 * @param[in]  reset_flag
 *                        - 1: hac reset
 *                        - 0: release reset hac.
 * @return     none
 */
static inline void audio_hac_asrc_ch_en(audio_hac_chn_e hac_chn, char reset_flag)
{
    reg_audio_hac_asrc_ch_en = (reg_audio_hac_asrc_ch_en & (~BIT(hac_chn))) | MASK_VAL(BIT(hac_chn), reset_flag);
}

/**
 * @brief      This function servers to select hac data src.
 *
 * @param[in]  hac_chn - hac channel.
 * @param[in]  src     - hac data source.
 * @return     none
 */
static inline void audio_hac_set_data_src(audio_hac_chn_e hac_chn, audio_hac_input_data_e src)
{
    reg_audio_hac_mux_sel = (reg_audio_hac_mux_sel & (~BIT(hac_chn))) | MASK_VAL(BIT(hac_chn), src);
}

/**
 * @brief      This function servers to select hac data dst.
 *
 * @param[in]  hac_chn - hac channel.
 * @param[in]  dst     - hac data dst.
 * @return     none
 */
static inline void audio_hac_set_data_dst(audio_hac_chn_e hac_chn, audio_hac_output_data_e dst)
{
    reg_audio_hac_hmst_sel = (reg_audio_hac_hmst_sel & (~BIT(hac_chn))) | MASK_VAL(BIT(hac_chn), dst);
}

/**
 * @brief      This function servers to set hac out data address when out type is HAC_OUTPUT_DATA_AHB_MST.
 *
 * @param[in]  hac_chn - hac channel.
 * @param[in]  address - out data address.
 * @return     none
 */
static inline void audio_hac_set_out_data_addr(audio_hac_chn_e hac_chn, unsigned int address)
{
    reg_audio_hac_haddr(hac_chn) = address;
    reg_audio_hac_haddr_set      = (reg_audio_hac_haddr_set & (~BIT(hac_chn))) | MASK_VAL(BIT(hac_chn), 1);
}

/**
 * @brief      This function servers to set MCU write fifo rate, when the input source is from AHB_SLV.
 *
 * @param[in]  hac_chn  - hac channel.
 * @param[in]  rate_div - MCU write fifo rate, rate = audio_clk / rate_div. for example, audio_clk = 36.864MHz, HAC sample = 48KHz, rate_div = 36.864MHz / 48KHz = 768.
 * @return     none
 * @note
 *             - If the data coming from matrix itself has a fixed sample rate, then there is no need for the HAC to control the input sample rate, then you need to write 0 to the above registers.
 */
static inline void audio_hac_set_in_data_rate(audio_hac_chn_e hac_chn, unsigned int rate_div)
{
    reg_audio_hac_rd_num(hac_chn) = (rate_div - 1) & FLD_HAC_RD_NUM;
}

/**
 * @brief      This function servers to set MCU write fifo rate, when the input source is from AHB_SLV.
 *
 * @param[in]  hac_chn  - hac channel.
 * @param[in]  rate_div - MCU write fifo rate.
 * @return     none
 * @note
 *             - Only used in 6 channel.
 */
static inline void audio_hac_set_in_data_rate_6ch(audio_hac_chn_e hac_chn, unsigned int rate_div)
{
    reg_audio_hac_rd_1_num(hac_chn) = (rate_div - 1) & FLD_HAC_RD_1_NUM;
}

/**
 * @brief      This function servers to set hac tdm num.
 *
 * @param[in]  hac_chn  - hac channel (only hac2 and hac3 need to set this register).
 * @param[in]  num - tdm number.
 * @return     none
 * @note
 */
static inline void audio_hac_set_tdm_num(audio_hac_chn_e hac_chn, unsigned char num)
{
    unsigned char mask    = FLD_HAC_TDM0_NUM << (4 * (hac_chn - 2));
    reg_audio_hac_tdm_num = (reg_audio_hac_tdm_num & ~mask) | MASK_VAL(mask, num);
}

/**
 * @brief      This function servers to set hac 8 channel fist.
 *
 * @param[in]  hac_chn_first  - hac channel.
 * @return     none
 * @note
 */
static inline void audio_hac_asrc_8ch_first_en(audio_hac_chn_e hac_chn_first)
{
    if (hac_chn_first == HAC_CHN2) {
        reg_audio_matrix_fifo_rx_hac23_sel = reg_audio_matrix_fifo_rx_hac23_sel & (~FLD_MATRIX_FIFO_RX_HAC3_SEL);
    } else if (hac_chn_first == HAC_CHN3) {
        reg_audio_matrix_fifo_rx_hac23_sel = reg_audio_matrix_fifo_rx_hac23_sel & (~FLD_MATRIX_FIFO_RX_HAC2_SEL);
    }
}

/**
 * @brief      This function servers to set hac 6 channel fist.
 *
 * @param[in]  hac_chn_first  - hac channel.
 * @return     none
 * @note
 */
static inline void audio_hac_asrc_6ch_first_en(audio_hac_chn_e hac_chn_first)
{
    if (hac_chn_first == HAC_CHN2) {
        reg_audio_hac_asrc_6ch_en          = FLD_HAC_ASRC_6CH_HAC2_FIRST_EN;
        reg_audio_matrix_fifo_rx_hac23_sel = reg_audio_matrix_fifo_rx_hac23_sel & (~FLD_MATRIX_FIFO_RX_HAC3_SEL);
    } else if (hac_chn_first == HAC_CHN3) {
        reg_audio_hac_asrc_6ch_en          = FLD_HAC_ASRC_6CH_HAC3_FIRST_EN;
        reg_audio_matrix_fifo_rx_hac23_sel = reg_audio_matrix_fifo_rx_hac23_sel & (~FLD_MATRIX_FIFO_RX_HAC2_SEL);
    }
}

/**
 * @brief      This function servers to set hac out data rate, when the output is matrix.
 *
 * @param[in]  hac_chn  - hac channel.
 * @param[in]  rate_div - hac out data rate, rate = audio_clk / rate_div. for example, audio_clk = 36.864MHz, HAC sample = 384KHz, rate_div = 36.864MHz / 384KHz = 96.
 * @return     none
 * @note
 *             - If the HAC data is output via AHB_MST, then there is no need to match this register, when txfifo will read away the output as soon as the AHB_MST is not empty.
 */
static inline void audio_hac_set_out_data_rate(audio_hac_chn_e hac_chn, unsigned int rate_div)
{
    reg_audio_hac_txfifo_rd_num(hac_chn) = (rate_div - 1) & FLD_HAC_TXFIFO_RD_NUM;
}

/**
 * @brief      This function servers to clear hac fifo cnt.
 *
 * @param[in]  hac_chn    - hac channel.
 * @param[in]  reset_flag
 *                        - 1: hac fifo cnt clear.
 *                        - 0: hac fifo cnt clear release.
 * @return     none
 */
static inline void audio_hac_clear_fifo_cnt(audio_hac_chn_e hac_chn, char reset_flag)
{
    reg_audio_hac_fifo_cnt_clr = (reg_audio_hac_fifo_cnt_clr & (~BIT(hac_chn))) | MASK_VAL(BIT(hac_chn), reset_flag);
}

/**
 * @brief      This function servers to enable eq config.
 *
 * @param[in]  hac_chn - hac channel.
 * @return     none
 */
static inline void audio_hac_eq_config_en(audio_hac_chn_e hac_chn)
{
    BM_SET(reg_audio_hac_eq_config_en, BIT(hac_chn));
}

/**
 * @brief      This function servers to config eq bypass.
 *
 * @param[in]  hac_chn - hac channel.
 * @return     none
 */
void audio_hac_bypass_eq(audio_hac_chn_e hac_chn);

/**
 * @brief      This function servers to enable/disable asrc bypass.
 *
 * @param[in]  asrc_chn - asrc channel select.
 * @param[in]  enable   - 1: asrc by pass, 0: asrc no bypass.
 * @return     none
 */
static inline void audio_hac_bypass_asrc(audio_hac_chn_e asrc_chn, unsigned char enable)
{
    enable ? BM_SET(reg_audio_hac_bypass_asrc, BIT(asrc_chn)) : BM_CLR(reg_audio_hac_bypass_asrc, BIT(asrc_chn));
}

/**
 * @brief      This function serves to select hac tdm tx fifo channal numbers.
 *
 * @param[in]  fifo ch - tdm used fifo.
 * @param[in]  chn   - tdm channal numbers
 * @return     none
 */
static inline void audio_hac_set_tdm_tx_dma_ch(audio_fifo_chn_e fifo_ch, audio_hac_tdm_tx_dma_ch_e chn)
{
    reg_audio_hac_tdm_tx_dma_ch_num = ((reg_audio_hac_tdm_tx_dma_ch_num & (~(0x03 << ((fifo_ch) * 2)))) | ((chn) << ((fifo_ch) * 2)));
}

/**
 * @brief      This function servers to update hac biquad filter coefficients.
 *
 * @param[in]  hac_chn - hac channel.
 * @param[in]  biquad  - biquad step audio_hac_biquad_e.
 * @param[in]  data    - biquad filter data address, [b0, b1, b2, a1, a2].
 * @return     none
 */
void audio_hac_update_biquad_coef(audio_hac_chn_e hac_chn, audio_hac_biquad_e biquad, signed int *data);

/**
 * @brief      This function serves to get hac tx fifo cnt.
 *
 * @param[in]  fifo_chn - hac channel.
 * @return     hac tx fifo cnt.
 */
unsigned int audio_hac_get_txfifo_cnt(audio_fifo_chn_e hac_chn);

/**
 * @brief      This function servers to select hac's asrc input and output fs.
 *
 * @param[in]  hac_chn   - hac channel.
 * @param[in]  fs_in     - input fs.
 * @param[in]  fs_out    - output fs.
 * @param[in]  ppm       - ppm value.
 * @return     none
 * @note       support fs in and out:
 *              - IN_16K_OUT_16K
 *              - IN_32K_OUT_16K
 *              - IN_44P1K_OUT_16K
 *              - IN_48K_OUT_16K
 *              - IN_96K_OUT_16K
 *              - IN_16K_OUT_32K
 *              - IN_32K_OUT_32K
 *              - IN_44P1K_OUT_32K
 *              - IN_48K_OUT_32K
 *              - IN_96K_OUT_32K
 *              - IN_16K_OUT_44P1K
 *              - IN_32K_OUT_44P1K
 *              - IN_44P1K_OUT_44P1
 *              - IN_48K_OUT_44P1K
 *              - IN_96K_OUT_44P1K
 *              - IN_16K_OUT_48K
 *              - IN_32K_OUT_48K
 *              - IN_44P1K_OUT_48K
 *              - IN_48K_OUT_48K
 *              - IN_96K_OUT_48K
 *              - IN_16K_OUT_96K
 *              - IN_32K_OUT_96K
 *              - IN_44P1K_OUT_96K
 *              - IN_48K_OUT_96K
 *              - IN_96K_OUT_96K
 *              - IN_768K_OUT_96K
 *              - IN_192K_OUT_192K
 *              - IN_384K_OUT_384K
 *              - IN_768K_OUT_768K
 * 
 */
void audio_hac_asrc_fs_in_out(audio_hac_chn_e hac_chn, int fs_in, int fs_out, int ppm, int tdm_chn);

/**
 * @brief      This function servers to set frac adc data.
 *
 * @param[in]  hac_chn  - hac channel.
 * @param[in]  frac_adc - frac adc value.
 * @return     none
 */
static inline void audio_hac_set_frac_adc(audio_hac_chn_e hac_chn, int frac_adc)
{
    reg_audio_hac_frac_adc(hac_chn) = frac_adc & FLD_HAC_FRAC_ADC;
}

/**
 * @brief      This function servers to set den rate data.
 *
 * @param[in]  hac_chn  - hac channel.
 * @param[in]  den_rate - den rate value.
 * @return     none
 */
static inline void audio_hac_set_den_rate(audio_hac_chn_e hac_chn, int den_rate)
{
    reg_audio_hac_den_rate(hac_chn) = den_rate & FLD_HAC_DEN_RATE;
}

/**
 * @brief      This function servers to set int adv data.
 *
 * @param[in]  hac_chn  - hac channel.
 * @param[in]  int_adv  - int adv value.
 * @return     none
 */
static inline void audio_hac_set_int_adv(audio_hac_chn_e hac_chn, char int_adv)
{
    reg_audio_hac_int_adv(hac_chn) = int_adv & FLD_HAC_INT_ADV;
}

/**
 * @brief      This function servers to set lag_int config done.
 *
 * @param[in]  hac_chn  - hac channel.
 * @return     none
 * @note
 *             - After configuring the correlation coefficients, set the bit of the corresponding channel to 1, so that the coefficients will be uniformly latched into lag_int.
 */
static inline void audio_hac_lag_int_config_done(audio_hac_chn_e hac_chn)
{
    BM_SET(reg_audio_hac_asrc_lag_update, BIT(hac_chn));
}

/**
 * @brief      This function servers to set hac interval.
 *
 * @param[in]  hac_chn - hac channel.
 * @param[in]  in_fs   - sample rate.
 * @param[in]  tdm_chn - tdm channel.
 * @return     none
 */
void audio_hac_set_interval(audio_hac_chn_e hac_chn, int in_fs, int tdm_chn);

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio I2S interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio I2S interface.
 * @{
 */

/**
 * @brief      This function serves to set i2s clock.
 * @param[in]  i2s_select      - i2s channel select.
 * @param[in]  div_numerator   - the dividing factor of div_numerator bit[0-14] valid.
 * @param[in]  div_denominator - the dividing factor of div_denominator bit[0-15] valid.
 * @return     none
 */
static inline void audio_i2s_set_clk(i2s_select_e i2s_select, unsigned short div_numerator, unsigned short div_denominator)
{
    reg_audio_clk_i2s_step(i2s_select) = div_numerator & FLD_CLK_I2S_STEP;
    reg_audio_clk_i2s_mod(i2s_select)  = div_denominator;
}

/**
 * @brief      This function serves to set the bclk divider.
 * @param[in]  i2s_select - i2s channel select.
 * @param[in]  div        - bclk = i2s_clk / (div * 2), if div = 0, i2s_clk = bclk.
 * @return     none
 */
static inline void audio_i2s_set_bclk(i2s_select_e i2s_select, unsigned char div)
{
    reg_audio_i2s_pcm_clk_num(i2s_select) = div;
}

/**
 * @brief      This function serves to set the i2s lrclk divider.
 * @param[in]  i2s_select - i2s channel select.
 * @param[in]  adc_div    - adc_lrclk = bclk / (adc_div).
 * @param[in]  dac_div    - dac_lrclk = bclk / (dac_div).
 * @return     none
 */
static inline void audio_i2s_set_lrclk(i2s_select_e i2s_select, unsigned short adc_div, unsigned short dac_div)
{
    reg_audio_i2s_int_pcm_num(i2s_select) = (adc_div - 1) & FLD_I2S_INT_PCM_NUM;
    reg_audio_i2s_dec_pcm_num(i2s_select) = (dac_div - 1) & FLD_I2S_DEC_PCM_NUM;
}

/**
 * @brief      This function serves to enable bclk and lr_clk.
 * @param[in]  i2s_select - i2s channel select
 * @return     none
 */
static inline void audio_i2s_clk_en(i2s_select_e i2s_select)
{
    BM_SET(reg_audio_i2s_cfg1(i2s_select), FLD_I2S_CLK_EN);
}

/**
 * @brief      This function serves to disable bclk and lr_clk
 * @param[in]  i2s_select - i2s channel select
 * @return     none
 */
static inline void audio_i2s_clk_dis(i2s_select_e i2s_select)
{
    BM_CLR(reg_audio_i2s_cfg1(i2s_select), FLD_I2S_CLK_EN);
}

/**
 * @brief      This function serves to set i2s schedule target value.
 * 
 * @param[in]  i2s_select   - i2s channel.
 * @param[in]  target_value - target value.
 * @return none
 */
static inline void audio_i2s_set_target_value(i2s_select_e i2s_select, unsigned int target_value)
{
    reg_audio_i2s_stimer_target(i2s_select) = target_value;
}

/**
 * @brief      This function serves to enable the i2s schedule.

 * @return    none
 */
static inline void audio_i2s_schedule_en(i2s_select_e i2s_select)
{
    BM_SET(reg_audio_i2s_route(i2s_select), FLD_I2S_SCHEDULE_EN);
}

/**
 * @brief      This function serves to disable the i2s schedule.
 * @return    none
 */
static inline void audio_i2s_schedule_dis(i2s_select_e i2s_select)
{
    BM_CLR(reg_audio_i2s_route(i2s_select), FLD_I2S_SCHEDULE_EN);
}

/**
 * @brief      This function serves to enable the i2s align function.
 * @return    none
 */
static inline void audio_i2s_align_en(void)
{
    BM_SET(reg_audio_i2s0_align_cfg, FLD_I2S_ALIGN_EN);
}

/**
 * @brief      This function serves to disable the i2s align function.
 * @return    none
 */
static inline void audio_i2s_align_dis(void)
{
    BM_CLR(reg_audio_i2s0_align_cfg, FLD_I2S_ALIGN_EN);
}

/**
 * @brief      This function serves to config i2s align mode.
 * 
 * @param[in]  align_config - i2s align config.
 * @return     none
 */
static inline void audio_i2s_align_config(i2s_align_config_t *align_config)
{
    reg_audio_i2s0_timer_th  = align_config->align_th;
    reg_audio_i2s0_align_cfg = MASK_VAL(FLD_I2S_ALIGN_EN, 1, FLD_I2S_ALIGN_CTRL, align_config->align_mode, FLD_I2S_ALIGN_MASK, 0, FLD_I2S_CLK_SEL, align_config->align_clk);
}

/**
 * @brief      This function serves to initialize configuration i2s.
 * @param[in]  i2s_config - the relevant configuration struct pointer @see audio_i2s_config_t.
 * @return     none
 */
void audio_i2s_config_init(audio_i2s_config_t *i2s_config);

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio matrix interface                                              *
 *********************************************************************************************************************/
/*!
 * @name Audio matrix interface.
 * @{
 */

/**
 * @brief      This function serves to select fifo rx route source and data format.
 *
 * @param[in]  fifo_num    - fifo channel.
 * @param[in]  route_from  - fifo rx route from.
 * @param[in]  data_format - fifo rx data format (route from i2s/anc/adc/hac valid), others select FIFO_RX_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_rx_fifo_route(audio_fifo_chn_e fifo_num, audio_matrix_fifo_rx_route_e route_from, audio_matrix_fifo_rx_format_e data_format);

/**
 * @brief   This function serves to select i2s tx route source and data format.
 *
 * @param[in]  i2s_tx_chn  - i2s tx channel.
 * @param[in]  route_from  - i2s tx route from.
 * @param[in]  data_format - i2s tx data format(route from fifo/hac2/hac3 valid), others select I2S_TX_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_i2s_tx_route(audio_i2s_tx_chn_e i2s_tx_chn, audio_matrix_i2s_tx_route_e route_from, audio_matrix_i2s_tx_format_e data_format);

/**
 * @brief      This function serves to select anc_src route source and format.
 *
 * @param[in]  anc_chn     - anc channel.
 * @param[in]  route_from  - anc_src route from.
 * @param[in]  data_format - anc src data format(route from fifo/i2s/adc valid), others select ANC_SRC_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_anc_src_route(audio_anc_chn_e anc_chn, audio_matrix_anc_src_route_e route_from, audio_matrix_anc_src_format_e data_format);

/**
 * @brief      This function serves to select anc_src route source and format.
 *
 * @param[in]  anc_chn     - anc channel.
 * @param[in]  route_from  - anc_ref route from.
 * @param[in]  data_format - anc ref data format(route from i2s/adc valid), others select ANC_REF_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_anc_ref_route(audio_anc_chn_e anc_chn, audio_matrix_anc_ref_route_e route_from, audio_matrix_anc_ref_format_e data_format);

/**
 * @brief      This function serves to select anc_err route source and data format.
 *
 * @param[in]  anc_chn     - anc channel.
 * @param[in]  route_from  - anc_err route from.
 * @param[in]  data_format - anc err data format(route from i2s/adc valid), others select ANC_ERR_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_anc_err_route(audio_anc_chn_e anc_chn, audio_matrix_anc_err_route_e route_from, audio_matrix_anc_err_format_e data_format);

/**
 * @brief      This function serves to select dac route source and data format.
 *
 * @param[in]  dac_chn     - dac channel.
 * @param[in]  route_from  - dac route from.
 * @param[in]  data_format - dac data format(route from fifo valid), others select DAC_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_dac_route(audio_codec0_output_select_e dac_chn, audio_matrix_dac_route_e route_from, audio_matrix_dac_format_e data_format);

/**
 * @brief      This function serves to select spdif tx route source.
 *
 * @param[in]  route_from - spdif tx route from.
 * @return     none
 */
static inline void audio_matrix_set_spdif_tx_route(audio_matrix_spdif_tx_route_e route_from)
{
    reg_audio_matrix_tx_sel = (reg_audio_matrix_tx_sel & (~FLD_MATRIX_SPDIF_TX_SEL)) | route_from;
}

/**
 * @brief      This function serves to select hac route source and data format.
 *
 * @param[in]  hac_chn     - hac channel.
 * @param[in]  route_from  - hac route from.
 * @param[in]  data_format - hac data format(route from i2s/adc valid), others select HAC_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_hac_route(audio_hac_chn_e hac_chn, audio_matrix_hac_route_e route_from, audio_matrix_hac_format_e data_format);

/**
 * @brief      This function serves to select side_tone route source and data format.
 *
 * @param[in]  sd_chn      - side_tone channel.
 * @param[in]  route_from  - side_tone route from.
 * @param[in]  data_format - side tone data format(route from fifo/i2s/adc valid), others select SIDE_TONE_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_side_tone_route(audio_side_tone_chn_e sd_chn, audio_matrix_side_tone_route_e route_from,
                                   audio_matrix_side_tone_format_e data_format);
/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio pin interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio pin interface.
 * @{
 */

/**
 * @brief      This function configures codec0 stream0 dmic pin.
 * @param[in]  dmic0_data - the data of dmic pin
 * @param[in]  dmic0_clk1 - the clk1 of dmic pin
 * @param[in]  dmic0_clk2 - the clk2 of dmic pin,if need not set clk2, please set GPIO_NONE_PIN.
 * @return     none
 */
void audio_codec0_set_dmic_a_pin(gpio_func_pin_e dmic0_data, gpio_func_pin_e dmic0_clk1, gpio_func_pin_e dmic0_clk2);

/**
 * @brief      This function configures codec0 stream1 dmic pin.
 * @param[in]  dmic1_data - the data of dmic pin.
 * @param[in]  dmic1_clk1 - the clk1 of dmic pin.
 * @param[in]  dmic1_clk2 - the clk2 of dmic pin, if need not set clk2,please set GPIO_NONE_PIN.
 * @return     none
 */
void audio_codec0_set_dmic_b_pin(gpio_func_pin_e dmic1_data, gpio_func_pin_e dmic1_clk1, gpio_func_pin_e dmic1_clk2);

/**
  * @brief      This function configures codec1 stream0 dmic pin.
  * @param[in]  dmic2_data - the data of dmic pin.
  * @param[in]  dmic2_clk1 - the clk1 of dmic pin.
  * @param[in]  dmic2_clk2 - the clk2 of dmic pin, if need not set clk2,please set GPIO_NONE_PIN.
  * @return     none
  */
void audio_codec1_set_dmic_a_pin(gpio_func_pin_e dmic2_data, gpio_func_pin_e dmic2_clk1, gpio_func_pin_e dmic2_clk2);

/**
 * @brief      This function serves to configure i2s pin.
 * @param[in]  i2s_select - channel select.
 * @param[in]  config     - i2s config pin struct.
 * @return     none
 */
void audio_i2s_set_pin(i2s_select_e i2s_select, i2s_pin_config_t *config);

/**
 * @brief      This function serves to configure spdif pin.
 * @param[in]  config     - spdif config pin struct.
 * @return     none
 */
void audio_spdif_set_pin(spdif_pin_config_t *config);

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio side_tone interface                                           *
 *********************************************************************************************************************/
/*!
 * @name Audio side_tone interface.
 * @{
 */

/**
 * @brief      This function servers to enable side tone.
 *
 * @param[in]  sdtn_chn    - side tone channel.
 * @return     none
 */
static inline void audio_side_tone_ch_en(audio_side_tone_chn_e sdtn_chn)
{
    reg_audio_matrix_sdtn_en |= BIT(sdtn_chn);
}

/**
 * @brief      This function servers to disable side tone.
 *
 * @param[in]  sdtn_chn    - side tone channel.
 * @return     none
 */
static inline void audio_side_tone_ch_dis(audio_side_tone_chn_e sdtn_chn)
{
    reg_audio_matrix_sdtn_en &= ~BIT(sdtn_chn);
}

/**
 * @brief      This function servers to enable side tone dma mode.
 *
 * @param[in]  sdtn_chns    - side tone channels.
 * @return     none
 */
static inline void audio_side_tone_dma_en(audio_side_tone_chns_e sdtn_chns)
{
    reg_audio_matrix_sdtn_dma_mode |= BIT(sdtn_chns);
}

/**
 * @brief      This function servers to enable side tone dma mode.
 *
 * @param[in]  sdtn_chns    - side tone channels.
 * @param[in]  audio_clk    - audio clk, eg.
 * @param[in]  fs           - sample rate.
 * @param[in]  val          - stereo 2; mono 1.
 * @return     none
 */
static inline void audio_side_tone_set_req_count(audio_side_tone_chns_e sdtn_chns, unsigned int audio_clk, unsigned fs, unsigned char val)
{
    reg_audio_matrix_sdtn_req_count(sdtn_chns) = audio_clk / fs - val;
}

/**
 * @brief      This function serves to enable anc.
 *
 * @param[in]  sdtn_ch - sidetone channel.
 * @param[in]  gain    - adc gain.
 * @return     none
 */
static inline void audio_side_tone_set_adc_gain(audio_side_tone_chn_e sdtn_ch, audio_side_tone_gain_e gain)
{
    unsigned char chn_mask = (sdtn_ch % 2) ? FLD_MATRIX_SDTN_ODD_ADC : FLD_MATRIX_SDTN_EVEN_ADC;

    reg_audio_matrix_sdtn_gain_adc(sdtn_ch) = (reg_audio_matrix_sdtn_gain_adc(sdtn_ch) & (~chn_mask)) | MASK_VAL(chn_mask, gain);
}

/**
 * @brief      This function serves to enable anc.
 *
 * @param[in]  sdtn_ch - anc channel.
 * @param[in]  gain    - dac gain.
 * @return     none
 */
static inline void audio_side_tone_set_dac_gain(audio_side_tone_chn_e sdtn_ch, audio_side_tone_gain_e gain)
{
    unsigned char chn_mask = (sdtn_ch % 2) ? FLD_MATRIX_SDTN_ODD_DAC : FLD_MATRIX_SDTN_EVEN_DAC;

    reg_audio_matrix_sdtn_gain_dac(sdtn_ch) = (reg_audio_matrix_sdtn_gain_dac(sdtn_ch) & (~chn_mask)) | MASK_VAL(chn_mask, gain);
}

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio spdif interface                                               *
 *********************************************************************************************************************/
/*!
 * @name Audio spdif interface.
 * @{
 */

/*!
 * @name Audio spdif interface
 * @{
 */
/**
 * @brief      This function serves to set spdif rx fs.
 * @param[in]  clk - audio pll clock
 * @return     none
 * @note       only support PLL_AUDIO_CLK_169P344M and PLL_AUDIO_CLK_147P456M
 */
void audio_spdif_set_rx_fs(pll_audio_clk_e clk);

/**
 * @brief      This function serves to enable spdif tx encode.
 * @param      none
 * @return     none
 */
static inline void audio_spdif_tx_encode_en(void)
{
    BM_SET(reg_audio_spdif_config, FLD_SPDIF_ENCODER_EN);
}

/**
 * @brief      This function serves to enable spdif rx decode.
 * @param      none
 * @return     none
 */
static inline void audio_spdif_rx_decode_en(void)
{
    BM_SET(reg_audio_spdif_config, FLD_SPDIF_DECODER_EN);
}

/**
 * @brief      This function serves to set spdif rx parity.
 * @param[in]  val - 0:parity by hardware; 1:parity by software
 * @return     none
 */
static inline void audio_spdif_set_rx_parity(unsigned char val)
{
    if (val) {
        BM_SET(reg_audio_spdif_config, FLD_SPDIF_PARITY_SELECT);
    } else {
        BM_CLR(reg_audio_spdif_config, FLD_SPDIF_PARITY_SELECT);
    }
}

/**
 * @brief      This function serves to enable spdif debug clk.
 * @param      none
 * @return     none
 */
static inline void audio_spdif_dbg_clk_en(void)
{
    BM_SET(reg_audio_spdif_config, FLD_SPDIF_DBG_CLK_EN);
}

/**
 * @brief      This function serves to set spdif RX format.
 * @param[in]  val - 0:low 28 bits for data, high 4bits for Pre_code. 1:high 28 bits for data, low 4bits for Pre_code.
 * @return     none
 */
static inline void audio_spdif_set_rx_fmt(unsigned char val)
{
    if (val) {
        BM_SET(reg_audio_spdif_config, FLD_SPDIF_RX_FMT);
    } else {
        BM_CLR(reg_audio_spdif_config, FLD_SPDIF_RX_FMT);
    }
}

/**
 * @brief      This function serves to set spdif tx format.
 * @param[in]  val - 0:low 28 bits for data, high 4bits for Pre_code. 1 :high 28 bits for data, low 4bits for Pre_code.
 * @return     none
 */
static inline void audio_spdif_set_tx_fmt(unsigned char val)
{
    if (val) {
        BM_SET(reg_audio_spdif_config, FLD_SPDIF_TX_FMT);
    } else {
        BM_CLR(reg_audio_spdif_config, FLD_SPDIF_TX_FMT);
    }
}

/**
 * @brief      This function serves to set hardware auto generates the preamble
 * @param[in]  val - 1'b0:preamble by software;1'b1:preamble by hardware
 * @return     none
 */
static inline void audio_spdif_set_auto_preamble(unsigned char val)
{
    if (val) {
        BM_SET(reg_audio_spdif_config, FLD_SPDIF_PREAMBLE_SELECT);
    } else {
        BM_CLR(reg_audio_spdif_config, FLD_SPDIF_PREAMBLE_SELECT);
    }
}

/**
 * @brief      This function serves to The hardware auto generates the preamble
 * @param      none
 * @return     none
 */
static inline void audio_spdif_soft_preamble(void)
{
    BM_CLR(reg_audio_spdif_config, FLD_SPDIF_PREAMBLE_SELECT);
}

/**
 * @brief      This function serves to set spdif tx clock.
 * @param[in]  tx_clk - spdif_tx_clk_e
 * @return     none
 */
static inline void audio_spdif_set_tx_clk(spdif_tx_clk_e tx_clk)
{
    BM_SET(reg_audio_clk_en_0, FLD_CLK_SPDIF_EN);
    reg_audio_spdif_tx_div = tx_clk;
}

/**
 * @}
 */

 #endif /* _AUDIO_H_ */
