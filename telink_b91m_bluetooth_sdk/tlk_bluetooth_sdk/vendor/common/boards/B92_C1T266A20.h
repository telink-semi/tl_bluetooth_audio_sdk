/********************************************************************************************************
 * @file    B92_C1T266A20.h
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
#ifndef B92_C1T266A20_H_
#define B92_C1T266A20_H_

/*
 * GPIO Usage Overview:
 *
 * NOTE: X means gpio not pinned,  NUS means gpio not used
 * 
* Port         0                1                2                3                4                5                6                7
 * ------------------------------------------------------------------------------------------------------------------------------------------------
 * PA     BT_DEBUG/I2S     BT_DEBUG/I2S      BT_DEBUG/I2S     BT_DEBUG/I2S         I2S            USB_DM           USB_DP            SWS
 * 
 * PB      GPIO_DEBUG        GPIO_DEBUG       GPIO_DEBUG       GPIO_DEBUG       GPIO_DEBUG       GPIO_DEBUG       GPIO_DEBUG        GPIO_DEBUG
 * 
 * PC      GPIO_DEBUG        UART0_TX         UART0_RX         GPIO_DEBUG        JTAG_TDI         JTAG-TDO     TAG_TMS/UART1_TX   JTAG_TCK/UART1_RX
 * 
 * PD      LED_BLUE          LED_GREEN        KEY1/3_OUT       CODEC_CLK1       CODEC_CLK2      CODEC_DATA        KEY1/2_IN          KEY2/4_OUT
 * 
 * PE     GPIO_DEBUG        GPIO_DEBUG        GPIO_DEBUG       GPIO_DEBUG       GPIO_DEBUG      GPIO_DEBUG        LED_WHITE        LED_RED/WAKEUP
 *
 * PF          X                X                 X                X                X                X            KEY3/4_IN            NUS
 * 
 * 
 */

#if (TLKHW_TYPE == TLKHW_TLSR9528A_EVK_C1T266A20)

#if (TLK_DEV_KEY_ENABLE)
#define TLKDRV_KEY_MAX_NUMB 4

#define KEY1_ID             TLKDRV_KEY_DID_KEY1
#define KEY1_GPIO_IN        GPIO_PD6
#define KEY1_GPIO_OUT       GPIO_PD2

#define KEY2_ID             TLKDRV_KEY_DID_KEY2
#define KEY2_GPIO_IN        GPIO_PD6
#define KEY2_GPIO_OUT       GPIO_PD7

#define KEY3_ID             TLKDRV_KEY_DID_KEY3
#define KEY3_GPIO_IN        GPIO_PF6
#define KEY3_GPIO_OUT       GPIO_PD2

#define KEY4_ID             TLKDRV_KEY_DID_KEY4
#define KEY4_GPIO_IN        GPIO_PF6
#define KEY4_GPIO_OUT       GPIO_PD7

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
#define GPIO_LED_BLUE         GPIO_PD0
#define GPIO_LED_BLUE_PWM_ID  PWM2_ID

#define GPIO_LED_GREEN        GPIO_PD1
#define GPIO_LED_GREEN_PWM_ID 0xff // 0xff is no pwm founction
#define GPIO_LED_WHITE        GPIO_PE6
#define GPIO_LED_WHITE_PWM_ID 0xff // 0xff is no pwm founction
#define GPIO_LED_RED          GPIO_PE7
#define GPIO_LED_RED_PWM_ID   PWM3_ID

#define GPIO_LED_NUMS         4

#define PD0_FUNC              AS_GPIO
#define PD1_FUNC              AS_GPIO
#define PE6_FUNC              AS_GPIO
#define PE7_FUNC              AS_GPIO

#define PD0_OUTPUT_ENABLE     1
#define PD1_OUTPUT_ENABLE     1
#define PE6_OUTPUT_ENABLE     1
#define PE7_OUTPUT_ENABLE     1

#define LED_ON_LEVEL          1 //gpio output high voltage to turn on led

#endif

/**
 *  @brief  GPIO definition for JTAG
 */
#if (JTAG_DEBUG_ENABLE)
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
 *  @brief  GPIO definition for debug_io
 */
#if (DEBUG_GPIO_ENABLE || DEBUG_COMMON_GPIO_ENABLE || DEBUG_SIMULATE_GPIO_ENABLE || DEBUG_SIHUI_GPIO_ENABLE || DEBUG_QIHANG_GPIO_ENABLE || DEBUG_TIANXIANG_GPIO_ENABLE ||        \
     DEBUG_SHUYOU_GPIO_ENABLE || DEBUG_ZHISHENG_GPIO_ENABLE || DEBUG_SIYOU_GPIO_ENABLE || DEBUG_YATING_GPIO_ENABLE || DEBUG_ZEWEN_GPIO_ENABLE || DEBUG_BINGQIANG_GPIO_ENABLE ||  \
     DEBUG_ZIYU_GPIO_ENABLE || DEBUG_JUNWEI_GPIO_ENABLE || DEBUG_MINGQIAN_GPIO_ENABLE || DEBUG_ZHANGJIAN_GPIO_ENABLE || DEBUG_QIPENG_GPIO_ENABLE || DEBUG_ZHAOWEI_GPIO_ENABLE || \
     DEBUG_HAIMING_GPIO_ENABLE)

#define GPIO_CHN0         GPIO_PB0
#define GPIO_CHN1         GPIO_PB1
#define GPIO_CHN2         GPIO_PB2
#define GPIO_CHN3         GPIO_PB3
#define GPIO_CHN4         GPIO_PB4
#define GPIO_CHN5         GPIO_PB5
#define GPIO_CHN6         GPIO_PB6
#define GPIO_CHN7         GPIO_PB7

#define GPIO_CHN8         GPIO_PC0
#define GPIO_CHN9         GPIO_PC3
#define GPIO_CHN10        GPIO_PE0
#define GPIO_CHN11        GPIO_PE1
#define GPIO_CHN12        GPIO_PE2
#define GPIO_CHN13        GPIO_PE3
#define GPIO_CHN14        GPIO_PE4
#define GPIO_CHN15        GPIO_PE5


#define PB0_OUTPUT_ENABLE 1
#define PB1_OUTPUT_ENABLE 1
#define PB2_OUTPUT_ENABLE 1
#define PB3_OUTPUT_ENABLE 1
#define PB4_OUTPUT_ENABLE 1
#define PB5_OUTPUT_ENABLE 1
#define PB6_OUTPUT_ENABLE 1
#define PB7_OUTPUT_ENABLE 1

#define PC0_OUTPUT_ENABLE 1
#define PC3_OUTPUT_ENABLE 1
#define PE0_OUTPUT_ENABLE 1
#define PE1_OUTPUT_ENABLE 1
#define PE2_OUTPUT_ENABLE 1
#define PE3_OUTPUT_ENABLE 1
#define PE4_OUTPUT_ENABLE 1
#define PE5_OUTPUT_ENABLE 1

#endif //end of DEBUG_GPIO_ENABLE


#define TLKAPP_WAKEUP_PIN GPIO_PE7

#if (TLK_DEV_SERIAL_ENABLE)
//UART0
#define TLKDEV_SERIAL0_TX_PIN GPIO_PC1
#define TLKDEV_SERIAL0_RX_PIN GPIO_PC2

//UART1
#ifndef TLKDEV_SERIAL1_TX_PIN
#define TLKDEV_SERIAL1_TX_PIN GPIO_PC6
#endif
#ifndef TLKDEV_SERIAL1_RX_PIN
#define TLKDEV_SERIAL1_RX_PIN GPIO_PC7
#endif

#endif //TLK_DEV_SERIAL_ENABLE

#if (TLK_DEV_CODEC_ENABLE)
#define TLKDRV_CODEC_SPK_FIFO FIFO0
#define TLKDRV_CODEC_MIC_FIFO FIFO0
#if (TLKDRV_CODEC_ICODEC_ENABLE)
//#define TLKDRV_ICODEC_POWER_PIN 	0
#define TLKDRV_ICODEC_DMIC_DATA_PIN GPIO_PD5
#define TLKDRV_ICODEC_DMIC_CLK0_PIN GPIO_PD4
#define TLKDRV_ICODEC_DMIC_CLK1_PIN GPIO_PD3
#elif (TLKDRV_CODEC_I2S_MASTER_ENABLE)
#define TLKDRV_I2S_MST_CHN         I2S1
#define TLKDRV_I2S_MST_BCLIK_PIN   GPIO_FC_PC3
#define TLKDRV_I2S_MST_DAC_CLK_PIN GPIO_FC_PC4
#define TLKDRV_I2S_MST_DAC_DAT_PIN GPIO_FC_PC5
#define TLKDRV_I2S_MST_ADC_CLK_PIN GPIO_FC_PC6
#define TLKDRV_I2S_MST_ADC_DAT_PIN GPIO_FC_PC7
#elif (TLKDRV_CODEC_I2S_SLAVE_ENABLE)
#define TLKDRV_I2S_SLV_CHN I2S1
#ifndef TLKDRV_I2S_SLV_BCLIK_PIN
#define TLKDRV_I2S_SLV_BCLIK_PIN GPIO_FC_PC3
#endif
#ifndef TLKDRV_I2S_SLV_DAC_CLK_PIN
#define TLKDRV_I2S_SLV_DAC_CLK_PIN GPIO_FC_PC4
#endif
#ifndef TLKDRV_I2S_SLV_DAC_DAT_PIN
#define TLKDRV_I2S_SLV_DAC_DAT_PIN GPIO_FC_PC5
#endif
#ifndef TLKDRV_I2S_SLV_ADC_CLK_PIN
#define TLKDRV_I2S_SLV_ADC_CLK_PIN GPIO_FC_PC6
#endif
#ifndef TLKDRV_I2S_SLV_ADC_DAT_PIN
#define TLKDRV_I2S_SLV_ADC_DAT_PIN GPIO_FC_PC7
#endif

#endif
#endif //TLK_DEV_CODEC_ENABLE

#endif /* TLKHW_TYPE == TLKHW_TLSR9528A_EVK_C1T266A20 */

#endif /* B92_C1T266A20_H_ */
