/********************************************************************************************************
 * @file    TL751X_C1T368A20_V1_0.h
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
#ifndef VENDOR_COMMON_BOARDS_TL751X_EVK_C1T368A20_V1_0_H_
#define VENDOR_COMMON_BOARDS_TL751X_EVK_C1T368A20_V1_0_H_


#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A20_V1_0)

#define WAKEUP_PAD GPIO_PB2
/**
 *  @brief  LED Configuration
 */
#if TLK_DEV_LED_ENABLE
/**
     *  @brief  Definition gpio for led
     */
#define GPIO_LED_BLUE        GPIO_PB3
#define GPIO_LED_BLUE_PWM_ID PWM0_ID
#define GPIO_LED_RED         GPIO_PB4
#define GPIO_LED_RED_PWM_ID  PWM1_ID

#define GPIO_LED_NUMS        2
#define LED_ON_LEVEL         1 //gpio output high voltage to turn on led

#endif


#if (TLK_DEV_KEY_ENABLE)
#define TLKDRV_KEY_MAX_NUMB 4

#define KEY1_ID             TLKDRV_KEY_DID_KEY1 // SW24
#define KEY1_GPIO_IN        GPIO_PA6
#define KEY1_GPIO_OUT       0

#define KEY2_ID             TLKDRV_KEY_DID_KEY2 // SW23
#define KEY2_GPIO_IN        GPIO_PA5
#define KEY2_GPIO_OUT       0

#define KEY3_ID             TLKDRV_KEY_DID_KEY3 // SW20
#define KEY3_GPIO_IN        GPIO_PB1
#define KEY3_GPIO_OUT       0

#define KEY4_ID             TLKDRV_KEY_DID_KEY4 //SW21
#define KEY4_GPIO_IN        GPIO_PB2
#define KEY4_GPIO_OUT       0

#else

#define TLKDRV_KEY_MAX_NUMB 0

#endif // #if (TLK_DEV_KEY_ENABLE)
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

#define TLKDRV_CODEC_MIC_FIFO FIFO0
#define TLKDRV_CODEC_SPK_FIFO FIFO0

#if TLKALG_ANC_ENABLE
#define TLKDRV_ANC0_MIC_FIFO FIFO1
#define TLKDRV_ANC0_SPK_FIFO FIFO1
#endif

#define AUDIO_HD_HAC_DISABLE     0
#define AUDIO_HD_HAC_AND_CODEC   1
#define AUDIO_HD_HAC_INDEPENDENT 2
#define AUDIO_HD_HAC_ANC_MODE    3

#ifndef AUDIO_HD_HAC_MODE
#define AUDIO_HD_HAC_MODE (AUDIO_HD_HAC_INDEPENDENT)
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
#define TLKDRV_ICODEC_DMIC_CLK0_PIN  GPIO_PI5
#define TLKDRV_ICODEC_DMIC_CLK1_PIN  GPIO_NONE_PIN
#define TLKDRV_ICODEC_DMIC_DATA_PIN  GPIO_PI2

#define TLKDRV_ICODEC_DMIC1_CLK0_PIN GPIO_PI4
#define TLKDRV_ICODEC_DMIC1_CLK1_PIN GPIO_NONE_PIN
#define TLKDRV_ICODEC_DMIC1_DATA_PIN GPIO_PI3

#define TLKDRV_ICODEC_DMIC2_CLK0_PIN GPIO_NONE_PIN
#define TLKDRV_ICODEC_DMIC2_CLK1_PIN GPIO_NONE_PIN
#define TLKDRV_ICODEC_DMIC2_DATA_PIN GPIO_NONE_PIN
#else
#define TLKDRV_ICODEC_DMIC_CLK0_PIN GPIO_PI5
#define TLKDRV_ICODEC_DMIC_CLK1_PIN 0
#define TLKDRV_ICODEC_DMIC_DATA_PIN GPIO_PI2
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

#if (TLK_DEV_SERIAL_ENABLE)
//UART0
#define TLKDEV_SERIAL0_TX_PIN GPIO_PF4
#define TLKDEV_SERIAL0_RX_PIN GPIO_PF5
//UART1
#define TLKDEV_SERIAL1_TX_PIN GPIO_PC6
#define TLKDEV_SERIAL1_RX_PIN GPIO_PB7
#endif //TLK_DEV_SERIAL_ENABLE


/**
 *  @brief  GPIO definition for debug_io
 */

/*
 * GPIO Usage Overview:
 *
 * NOTE: X means gpio not pinned,  NUS means gpio not used,BTC BT controller used
 *
* Port         0                1                2                3                4                5                6                7
 * ------------------------------------------------------------------------------------------------------------------------------------------------
 * PA    GPIO_DEBUG        GPIO_DEBUG         GPIO_DEBUG    JTAG_DSP_TDI/BLED   JTAG_DSP_TCK/GLED   JTAG_DSP_MS/SW23    JTAG_DSP_TDO/SW24    SWS
 *
 * PB    GPIO_DEBUG          SW20             SW21             LED                  LED        GPIO_DEBUG(PB5E)       GPIO_DEBUG         UART_RX
 *
 * PC     GPIO_DEBUG      GPIO_DEBUG            X                 X                   X                X           UART_TX               X
 *
 * PD     X                 X                   X                   X                 X               X             GPIO_DEBUG          X
 *
 * PE       X                   X                X                   X               X                  X                   X            X
 *
 * PF        DM0                DP0              DM1             DP1           GPIO_DEBUG        GPIO_DEBUG         X                     X
 *
 * PG      DMIC0_DATA       DMIC1_CLK0       DMIC1_CLK1        DMIC1_DATA      DMIC2_CLK0        DMIC2_CLK1         DMIC2_DATA           X
 *
 * PH        X                  X                  X                 X              X                X                 X                X
 *
 * PI        X                  X                  X                 X              X                 X                X                X
 *
 * PJ        X                  X                  X                 X              X               X                  X                X
 *
 */
#if (DEBUG_COMMON_GPIO_ENABLE || DEBUG_ZHAOWEI_GPIO_ENABLE || DEBUG_JUNWEI_GPIO_ENABLE || DEBUG_ZEWEN_GPIO_ENABLE || DEBUG_MINGQIAN_GPIO_ENABLE || DEBUG_TIANXIANG_GPIO_ENABLE || \
     DEBUG_ZHISHENG_GPIO_ENABLE || DEBUG_GPIO_ENABLE || DEBUG_SIHUI_GPIO_ENABLE || DEBUG_SHUYOU_GPIO_ENABLE || DEBUG_SIMULATE_GPIO_ENABLE || DEBUG_YATING_GPIO_ENABLE)
#define GPIO_CHN0  GPIO_PA0 // Maybe conflict with bt/le dbg port -> TX_EN       	dbgport close for default
#define GPIO_CHN1  GPIO_PA1 // Maybe conflict with bt/le dbg port -> RX_EN /TX_ON    dbgport close for default
#define GPIO_CHN2  GPIO_PB6
#define GPIO_CHN3  GPIO_PB5 // switch key to PB5E
#define GPIO_CHN4  GPIO_PA2 // Maybe conflict with le dbg port -> RX_EN			    dbgport close for default
#define GPIO_CHN5  GPIO_PC0 //
#define GPIO_CHN6  GPIO_PC1
#define GPIO_CHN7  GPIO_PB0

#define GPIO_CHN8  GPIO_PF4
#define GPIO_CHN9  GPIO_PF5

#define GPIO_CHN10 GPIO_PA3 //
// #define GPIO_CHN11        GPIO_PA4
// #define GPIO_CHN12        GPIO_PD5
//#define GPIO_CHN13        GPIO_PD1
//#define GPIO_CHN14        GPIO_PD0
//#define GPIO_CHN15        GPIO_PA6

#define PA0_OUTPUT_ENABLE 1
#define PA1_OUTPUT_ENABLE 1
#define PB6_OUTPUT_ENABLE 1
#define PB5_OUTPUT_ENABLE 1
#define PA2_OUTPUT_ENABLE 1
#define PC0_OUTPUT_ENABLE 1
#define PC1_OUTPUT_ENABLE 1
#define PB0_OUTPUT_ENABLE 1

#define PF4_OUTPUT_ENABLE 1
#define PF5_OUTPUT_ENABLE 1
#define PA3_OUTPUT_ENABLE 1
// #define PA4_OUTPUT_ENABLE 1


#define PA0_INPUT_ENABLE 0
#define PA1_INPUT_ENABLE 0
#define PB6_INPUT_ENABLE 0
#define PB5_INPUT_ENABLE 0
#define PA2_INPUT_ENABLE 0
#define PC0_INPUT_ENABLE 0
#define PC1_INPUT_ENABLE 0
#define PB0_INPUT_ENABLE 0

#define PF4_INPUT_ENABLE 0
#define PF5_INPUT_ENABLE 0
#define PA3_INPUT_ENABLE 0
// #define PA4_INPUT_ENABLE  0


#define PA0_FUNC AS_GPIO
#define PA1_FUNC AS_GPIO
#define PB6_FUNC AS_GPIO
#define PB5_FUNC AS_GPIO
#define PA2_FUNC AS_GPIO
#define PC0_FUNC AS_GPIO
#define PC1_FUNC AS_GPIO
#define PB0_FUNC AS_GPIO

#define PF4_FUNC AS_GPIO
#define PF5_FUNC AS_GPIO
#define PA3_FUNC AS_GPIO
// #define PA4_FUNC          AS_GPIO

#endif //end of DEBUG_GPIO_ENABLE

#endif

#endif /* VENDOR_COMMON_BOARDS_TL751X_EVK_C1T368A20_V1_0_H_ */
