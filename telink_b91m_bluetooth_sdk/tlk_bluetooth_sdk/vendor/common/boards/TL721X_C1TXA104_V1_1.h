/********************************************************************************************************
 * @file    TL721X_C1TXA104_V1_1.h
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
#ifndef VENDOR_COMMON_BOARDS_TL721X_C1TXA104_V1_1_H_
#define VENDOR_COMMON_BOARDS_TL721X_C1TXA104_V1_1_H_

#if (TLKHW_TYPE == BOARD_721X_EVK_C1TXA104_V1_1)

/**
 *  @brief  Keyboard Configuration
 */
#if (TLK_DEV_KEY_ENABLE)
#define TLKDRV_KEY_MAX_NUMB 3

#define KEY1_ID             TLKDRV_KEY_DID_KEY1
#define KEY1_GPIO_IN        GPIO_PE3
#define KEY1_GPIO_OUT       0

#define KEY2_ID             TLKDRV_KEY_DID_KEY2
#define KEY2_GPIO_IN        GPIO_PE5
#define KEY2_GPIO_OUT       0

#define KEY3_ID             TLKDRV_KEY_DID_KEY3
#define KEY3_GPIO_IN        GPIO_PF4
#define KEY3_GPIO_OUT       0
#else

#define TLKDRV_KEY_MAX_NUMB 0

#endif // #if (TLK_DEV_KEY_ENABLE)

/**
 *  @brief  LED Configuration
 */
#if TLK_DEV_LED_ENABLE
/**
     *  @brief  Definition gpio for led
     */
#define GPIO_LED_BLUE         GPIO_PE2
#define GPIO_LED_BLUE_PWM_ID  0xff

#define GPIO_LED_GREEN        0
#define GPIO_LED_GREEN_PWM_ID 0xff // 0xff is no pwm founction

#define GPIO_LED_WHITE        GPIO_PE1
#define GPIO_LED_WHITE_PWM_ID 0xff // 0xff is no pwm founction

#define GPIO_LED_RED          0
#define GPIO_LED_RED_PWM_ID   0xff

#define GPIO_LED_NUMS         2

#define PE2_FUNC              AS_GPIO
#define PE1_FUNC              AS_GPIO

#define PE2_OUTPUT_ENABLE     1
#define PE1_OUTPUT_ENABLE     1

#define LED_ON_LEVEL          1 //gpio output high voltage to turn on led

#endif

#ifndef JTAG_DEBUG_DISABLE
#define JTAG_DEBUG_DISABLE 1
#endif
/**
 *  @brief  GPIO definition for JTAG
 */
#if (JTAG_DEBUG_DISABLE)
//JTAG will cost some power
#define PC4_FUNC            AS_GPIO
#define PC5_FUNC            AS_GPIO
#define PC6_FUNC            AS_GPIO
#define PC7_FUNC            AS_GPIO

#define PC4_INPUT_ENABLE    0
#define PC5_INPUT_ENABLE    0
#define PC6_INPUT_ENABLE    0
#define PC7_INPUT_ENABLE    0

#define PULL_WAKEUP_SRC_PC4 0
#define PULL_WAKEUP_SRC_PC5 0
#define PULL_WAKEUP_SRC_PC6 0
#define PULL_WAKEUP_SRC_PC7 0

#endif

/**
 * @brief AUDIO(Codec) Mode Configuration
 */
#if (TLK_DEV_CODEC_ENABLE)
#define TLKDRV_CODEC_MIC_FIFO FIFO1
#define TLKDRV_CODEC_SPK_FIFO FIFO1
#endif ///#if (TLK_DEV_CODEC_ENABLE)


#if (TLKDRV_CODEC_ICODEC_ENABLE)
#define TLKDRV_ICODEC_POWER_PIN     0
#define TLKDRV_ICODEC_DMIC_CLK0_PIN GPIO_PF5
#define TLKDRV_ICODEC_DMIC_CLK1_PIN GPIO_PF6
#define TLKDRV_ICODEC_DMIC_DATA_PIN GPIO_PF7
#elif (TLKDRV_CODEC_I2S_MASTER_ENABLE)
#define TLKDRV_I2S_MST_CHN         I2S1
#define TLKDRV_I2S_MST_BCLIK_PIN   GPIO_FC_PA3
#define TLKDRV_I2S_MST_DAC_CLK_PIN GPIO_FC_PB4
#define TLKDRV_I2S_MST_DAC_DAT_PIN GPIO_FC_PA2
#define TLKDRV_I2S_MST_ADC_CLK_PIN GPIO_FC_PB4
#define TLKDRV_I2S_MST_ADC_DAT_PIN GPIO_FC_PA4
#elif (TLKDRV_CODEC_I2S_SLAVE_ENABLE)
#define TLKDRV_IISSLV_CHN         0
#define TLKDRV_IISSLV_BCLIK_PIN   0
#define TLKDRV_IISSLV_DAC_CLK_PIN 0
#define TLKDRV_IISSLV_DAC_DAT_PIN 0
#define TLKDRV_IISSLV_ADC_CLK_PIN 0
#define TLKDRV_IISSLV_ADC_DAT_PIN 0
#endif /* TLKDRV_CODEC_ICODEC_ENABLE */

#define AUDIO_CODEC_LOOPBACK 0 ///for codec test,default 0
#if (TLK_DEV_SERIAL_ENABLE)
//UART0
#define TLKDEV_SERIAL0_TX_PIN GPIO_PF6
#define TLKDEV_SERIAL0_RX_PIN GPIO_PF5
//UART1
#define TLKDEV_SERIAL1_TX_PIN GPIO_FC_PD5
#define TLKDEV_SERIAL1_RX_PIN GPIO_FC_PD4
#endif //TLK_DEV_SERIAL_ENABLE
/**
 *  @brief  Battery_check Configuration
 */
#if (BATT_CHECK_ENABLE)
#define VBAT_CHANNEL_EN 0

#if VBAT_CHANNEL_EN
/**     The battery voltage sample range is 1.8~3.5V    **/
#else
/**     if the battery voltage > 3.6V, should take some external voltage divider    **/
#define ADC_INPUT_PIN_CHN ADC_GPIO_PB0
#endif
#endif


/**
 *  @brief  GPIO definition for debug_io
 */
#if (DEBUG_GPIO_ENABLE || DEBUG_COMMON_GPIO_ENABLE || DEBUG_TIANXIANG_GPIO_ENABLE || DEBUG_SHUYOU_GPIO_ENABLE)
#define GPIO_CHN0         GPIO_PE0
#define GPIO_CHN1         GPIO_PE1
#define GPIO_CHN2         GPIO_PE2
#define GPIO_CHN3         GPIO_PE3
#define GPIO_CHN4         GPIO_PE4
#define GPIO_CHN5         GPIO_PE5
#define GPIO_CHN6         GPIO_PE6
#define GPIO_CHN7         GPIO_PC4

#define GPIO_CHN8         GPIO_PF4
#define GPIO_CHN9         GPIO_PF6
#define GPIO_CHN10        GPIO_PF7
#define GPIO_CHN11        GPIO_PA0
#define GPIO_CHN12        GPIO_PA1
#define GPIO_CHN13        GPIO_PE7
#define GPIO_CHN14        GPIO_PF0
#define GPIO_CHN15        GPIO_PF1


#define PE0_OUTPUT_ENABLE 1
#define PE1_OUTPUT_ENABLE 1
#define PE2_OUTPUT_ENABLE 1
#define PE3_OUTPUT_ENABLE 1
#define PE4_OUTPUT_ENABLE 1
#define PE5_OUTPUT_ENABLE 1
#define PE6_OUTPUT_ENABLE 1
#define PF3_OUTPUT_ENABLE 1

#define PF4_OUTPUT_ENABLE 1
#define PF6_OUTPUT_ENABLE 1
#define PF7_OUTPUT_ENABLE 1
#define PA0_OUTPUT_ENABLE 1
#define PA1_OUTPUT_ENABLE 1
#define PE7_OUTPUT_ENABLE 1
#define PF0_OUTPUT_ENABLE 1
#define PF1_OUTPUT_ENABLE 1

#endif //end of DEBUG_GPIO_ENABLE


#endif ////* VENDOR_COMMON_BOARDS_TL721X_C1T315A20_V2_H_ */

#endif
