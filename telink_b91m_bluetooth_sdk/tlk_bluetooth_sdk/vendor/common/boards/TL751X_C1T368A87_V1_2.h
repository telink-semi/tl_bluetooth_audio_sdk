/********************************************************************************************************
 * @file    TL751X_C1T368A87_V1_2.h
 *
 * @brief   This is the header file for TLSR/TL
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
#pragma once


#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_2)

#define WAKEUP_PAD GPIO_PB3
/**
 *  @brief  LED Configuration
 */
#if TLK_DEV_LED_ENABLE
/**
     *  @brief  Definition gpio for led
     */
#define GPIO_LED_BLUE         GPIO_PC0
#define GPIO_LED_BLUE_PWM_ID  PWM0_ID
#define GPIO_LED_RED          GPIO_PC1
#define GPIO_LED_RED_PWM_ID   PWM1_ID

#define GPIO_LED_GREEN        GPIO_PC6
#define GPIO_LED_GREEN_PWM_ID PWM2_ID

#define GPIO_LED_NUMS         2
#define LED_ON_LEVEL          1 //gpio output high voltage to turn on led

#endif


#if (TLK_DEV_KEY_ENABLE)
#define TLKDRV_KEY_MAX_NUMB 2

#define KEY1_ID             TLKDRV_KEY_DID_KEY1
#define KEY1_GPIO_IN        GPIO_PB3
#define KEY1_GPIO_OUT       0

#define KEY2_ID             TLKDRV_KEY_DID_KEY2
#define KEY2_GPIO_IN        GPIO_PB1
#define KEY2_GPIO_OUT       0
#else

#define TLKDRV_KEY_MAX_NUMB 0

#endif // #if (TLK_DEV_KEY_ENABLE)

#define ANT_VDD_GPIO GPIO_PC6
#define ANT_VC_GPIO  GPIO_PF5
/**
 *  @brief  GPIO definition for JTAG
 */
#if (JTAG_DEBUG_ENABLE)
//JTAG will cost some power
#define PE4_FUNC            AS_GPIO
#define PE5_FUNC            AS_GPIO
#define PE6_FUNC            AS_GPIO
#define PE7_FUNC            AS_GPIO

#define PE4_INPUT_ENABLE    0
#define PE5_INPUT_ENABLE    0
#define PE6_INPUT_ENABLE    0
#define PE7_INPUT_ENABLE    0

#define PULL_WAKEUP_SRC_PE4 0
#define PULL_WAKEUP_SRC_PE5 0
#define PULL_WAKEUP_SRC_PE6 0
#define PULL_WAKEUP_SRC_PE7 0

#endif

/**
 * @brief GPIO definition for SPI and I2S
 */
#if TLK_CFG_HRA_ENABLE
#define HRA_I2S_BCLK_PIN      GPIO_FC_PC1
#define HRA_I2S_ADC_LRCLK_PIN GPIO_FC_PC2
#define HRA_I2S_ADC_DAT_PIN   GPIO_FC_PC3
#define HRA_I2S_DAC_LRCLK_PIN GPIO_FC_PC4
#define HRA_I2S_DAC_DAT_PIN   GPIO_FC_PC5

#define HRA_SPI_CS_PIN        GPIO_FC_PA0
#define HRA_SPI_CLK_PIN       GPIO_FC_PA1
#define HRA_SPI_MOSI_PIN      GPIO_FC_PA2
#define HRA_SPI_MISO_PIN      GPIO_FC_PB0

#define HRA_DMIC_CLK0_PIN     GPIO_PG1
#define HRA_DMIC_CLK1_PIN     GPIO_PG2
#define HRA_DMIC_DATA_PIN     GPIO_PG3

#define GPIO_CHANNEL0         GPIO_PI5
#define GPIO_CHANNEL1         GPIO_PF4
#define GPIO_CHANNEL2         GPIO_PE2
#define GPIO_CHANNEL3         GPIO_PE1
#define GPIO_CHANNEL4         GPIO_PE0
#define GPIO_CHANNEL5         GPIO_PD6
#define GPIO_CHANNEL6         GPIO_PD5
#define GPIO_CHANNEL7         GPIO_PD1
#endif

/**
 * @brief AUDIO(Codec) Mode Configuration
 */
#if (TLK_DEV_CODEC_ENABLE)

#if (TLKALG_BBF_ENABLE)
#define TLKDRV_CODEC_MIC_FIFO  FIFO0
#define TLKDRV_CODEC_MIC1_FIFO FIFO1
#define TLKDRV_CODEC_SPK_FIFO  FIFO0
#else
#define TLKDRV_CODEC_MIC_FIFO FIFO0
#define TLKDRV_CODEC_SPK_FIFO FIFO0
#endif
#if TLKALG_ANC_ENABLE
#define TLKDRV_ANC0_MIC_FIFO FIFO1
#define TLKDRV_ANC0_SPK_FIFO FIFO1
#endif

#define AUDIO_HD_HAC_DISABLE     0
#define AUDIO_HD_HAC_AND_CODEC   1
#define AUDIO_HD_HAC_INDEPENDENT 2
#define AUDIO_HD_HAC_ANC_MODE    3

#if 1 //(TLKMW_INTERPHONE_EN)
#define AUDIO_HD_HAC_MODE (AUDIO_HD_HAC_INDEPENDENT)
#else
#define AUDIO_HD_HAC_MODE (AUDIO_HD_HAC_DISABLE)
#endif

#if (AUDIO_HD_HAC_MODE == AUDIO_HD_HAC_DISABLE)
#define AUDIO_HD_HAC_EN 0
#else
#define AUDIO_HD_HAC_EN          1

#define AUDIO_HD_HAC_SPK_EQ_EN   0
#define AUDIO_HD_HAC_MIC_EQ_EN   0
#define AUDIO_HD_HAC_SPK_ASRC_EN 1
#define AUDIO_HD_HAC_MIC_ASRC_EN 0

#if (AUDIO_HD_HAC_MODE == AUDIO_HD_HAC_INDEPENDENT)


#elif (AUDIO_HD_HAC_MODE == AUDIO_HD_HAC_AND_CODEC)

#if (AUDIO_HD_HAC_MIC_ASRC_EN)
#define AUDIO_HAC_ADC_L_CH        HAC_CHN2
#define AUDIO_HAC_ADC_R_CH        HAC_CHN3
#define TLKDRV_AUDIO_MIC_LCH_FIFO FIFO2
#define TLKDRV_AUDIO_MIC_RCH_FIFO FIFO3
#endif
#elif (AUDIO_HD_HAC_MODE == AUDIO_HD_HAC_ANC_MODE)
#endif

#endif

#if (TLKDRV_CODEC_ICODEC_ENABLE)
#define TLKDRV_ICODEC_POWER_PIN 0
#if (TLKALG_BBF_ENABLE)
#define TLKDRV_ICODEC_DMIC_CLK0_PIN  GPIO_PI2
#define TLKDRV_ICODEC_DMIC_CLK1_PIN  GPIO_NONE_PIN
#define TLKDRV_ICODEC_DMIC_DATA_PIN  GPIO_PI3

#define TLKDRV_ICODEC_DMIC1_CLK0_PIN GPIO_PI4
#define TLKDRV_ICODEC_DMIC1_CLK1_PIN GPIO_NONE_PIN
#define TLKDRV_ICODEC_DMIC1_DATA_PIN GPIO_PI5

#define TLKDRV_ICODEC_DMIC2_CLK0_PIN GPIO_PB4
#define TLKDRV_ICODEC_DMIC2_CLK1_PIN GPIO_NONE_PIN
#define TLKDRV_ICODEC_DMIC2_DATA_PIN GPIO_PB6

#else
#define TLKDRV_ICODEC_DMIC_CLK0_PIN GPIO_PI4
#define TLKDRV_ICODEC_DMIC_CLK1_PIN GPIO_NONE_PIN
#define TLKDRV_ICODEC_DMIC_DATA_PIN GPIO_PI5
#endif

#elif (TLKDRV_CODEC_I2S_MASTER_ENABLE)
#define TLKDRV_I2S_MST_CHN         0
#define TLKDRV_I2S_MST_BCLIK_PIN   0
#define TLKDRV_I2S_MST_DAC_CLK_PIN 0
#define TLKDRV_I2S_MST_DAC_DAT_PIN 0
#define TLKDRV_I2S_MST_ADC_CLK_PIN 0
#define TLKDRV_I2S_MST_ADC_DAT_PIN 0
#elif (TLKDRV_CODEC_I2S_SLAVE_ENABLE)
#define TLKDRV_IISSLV_CHN         0
#define TLKDRV_IISSLV_BCLIK_PIN   0
#define TLKDRV_IISSLV_DAC_CLK_PIN 0
#define TLKDRV_IISSLV_DAC_DAT_PIN 0
#define TLKDRV_IISSLV_ADC_CLK_PIN 0
#define TLKDRV_IISSLV_ADC_DAT_PIN 0
#endif /* TLKDRV_CODEC_ICODEC_ENABLE */
#endif /* TLK_DEV_CODEC_ENABLE */

#endif