/********************************************************************************************************
 * @file    TL3228_C1T371A20_V1_1.h
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
#ifndef VENDOR_COMMON_BOARDS_TL751X_EVK_C1T360A20_H_
#define VENDOR_COMMON_BOARDS_TL751X_EVK_C1T360A20_H_


#if (TLKHW_TYPE == TLKHW_TL3228_EVK_C1T371A20_V1_1)

#define WAKEUP_PAD 0
/**
 *  @brief  LED Configuration
 */
#if TLK_DEV_LED_ENABLE
//3322X PWM_CHN MUST ADAPT GPIO,refer to gpio.h
#define GPIO_LED_BLUE         GPIO_PC5
#define GPIO_LED_BLUE_PWM_ID  PWM2_ID
#define GPIO_LED_RED          GPIO_PC4
#define GPIO_LED_RED_PWM_ID   PWM1_ID

#define GPIO_LED_WHITE        GPIO_PC6
#define GPIO_LED_WHITE_PWM_ID PWM3_ID

#define GPIO_LED_GREEN        GPIO_PC7
#define GPIO_LED_GREEN_PWM_ID PWM0_ID

#define GPIO_LED_NUMS         4

#define LED_ON_LEVEL          1 //gpio output high voltage to turn on led

#endif


#if (TLK_DEV_KEY_ENABLE)
#define TLKDRV_KEY_MAX_NUMB 4

#define KEY1_ID             TLKDRV_KEY_DID_KEY1 // SW3
#define KEY1_GPIO_IN        GPIO_PG3
#define KEY1_GPIO_OUT       GPIO_PG1

#define KEY2_ID             TLKDRV_KEY_DID_KEY2 // SW4
#define KEY2_GPIO_IN        GPIO_PG2
#define KEY2_GPIO_OUT       GPIO_PG1


#define KEY3_ID             TLKDRV_KEY_DID_KEY3 // SW6
#define KEY3_GPIO_IN        GPIO_PG3
#define KEY3_GPIO_OUT       GPIO_PG0

#define KEY4_ID             TLKDRV_KEY_DID_KEY4 //  SW7
#define KEY4_GPIO_IN        GPIO_PG2
#define KEY4_GPIO_OUT       GPIO_PG0

#else

#define TLKDRV_KEY_MAX_NUMB 0

#endif // #if (TLK_DEV_KEY_ENABLE)

/**
 * @brief AUDIO(Codec) Mode Configuration
 */
#if (TLK_DEV_CODEC_ENABLE)

#define TLKDRV_CODEC_MIC_FIFO FIFO0
#define TLKDRV_CODEC_SPK_FIFO FIFO0


#if (TLKDRV_CODEC_ICODEC_ENABLE)
#define TLKDRV_ICODEC_POWER_PIN     0
#define TLKDRV_ICODEC_DMIC_CLK0_PIN GPIO_PE4
#define TLKDRV_ICODEC_DMIC_CLK1_PIN 0
#define TLKDRV_ICODEC_DMIC_DATA_PIN GPIO_PE5

#define TLKDRV_ICODEC_SDMP0         GPIO_FC_PF1
#define TLKDRV_ICODEC_SDMN0         GPIO_FC_PF0
#define TLKDRV_ICODEC_SDMP1         GPIO_FC_PC2
#define TLKDRV_ICODEC_SDMN1         GPIO_FC_PC3
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
#define TLKDEV_SERIAL0_TX_PIN 0
#define TLKDEV_SERIAL0_RX_PIN 0
//UART1
#define TLKDEV_SERIAL1_TX_PIN GPIO_PG6
#define TLKDEV_SERIAL1_RX_PIN GPIO_PG5
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
 * PA       BB_DEBUG         BB_DEBUG         BB_DEBUG       JTAG_DSP_TDI     JTAG_DSP_TMS     JTAG_DSP_TCK  JTAG_DSP_TDO/GPIO_DEBUG  SWS
 *
 * PB        KEY1           JTAG_D25_TCK     JTAG_D25_TMS    JTAG_D25_TDO     JTAG_D25_TDI       LED_G             LED_B            LED_R
 *
 * PC        LED_W             BTC               BTC              BTC             BTC            BTC               UART_TX          UART_RX
 *
 * PD     GPIO_DEBUG        GPIO_DEBUG          KEY2              KEY3            KEY4           GPIO_DEBUG        GPIO_DEBUG          X
 *
 * PE     GPIO_DEBUG        GPIO_DEBUG       GPIO_DEBUG         GPIO_DEBUG     GPIO_DEBUG        GPIO_DEBUG        GPIO_DEBUG       GPIO_DEBUG
 *
 * PF        DM0                DP0              DM1             DP1           GPIO_DEBUG        GPIO_DEBUG         DMIC0_CLK0       DMIC0_CLK1
 *
 * PG      DMIC0_DATA       DMIC1_CLK0       DMIC1_CLK1        DMIC1_DATA      DMIC2_CLK0        DMIC2_CLK1         DMIC2_DATA           X
 *
 * PH        X                  X                  X                 X              X                X                 X                X
 *
 * PI        X                  X                  X                 X              X            GPIO_DEBUG            X                X
 *
 * PJ        X                  X                  X                 X              X               X                  X                X
 *
 */
#if (DEBUG_COMMON_GPIO_ENABLE || DEBUG_JUNWEI_GPIO_ENABLE || DEBUG_ZEWEN_GPIO_ENABLE || DEBUG_MINGQIAN_GPIO_ENABLE || DEBUG_TIANXIANG_GPIO_ENABLE || DEBUG_ZHISHENG_GPIO_ENABLE || \
     DEBUG_GPIO_ENABLE || DEBUG_SIHUI_GPIO_ENABLE || DEBUG_SHUYOU_GPIO_ENABLE || DEBUG_SIMULATE_GPIO_ENABLE || DEBUG_YATING_GPIO_ENABLE)
#define GPIO_CHN0         GPIO_PG4
#define GPIO_CHN1         GPIO_PG5
#define GPIO_CHN2         GPIO_PG6
#define GPIO_CHN3         GPIO_PG7
#define GPIO_CHN4         GPIO_PH0
#define GPIO_CHN5         GPIO_PH1
#define GPIO_CHN6         GPIO_PH2
#define GPIO_CHN7         GPIO_PH3

#define GPIO_CHN8         GPIO_PH4
#define GPIO_CHN9         GPIO_PH5
#define GPIO_CHN10        GPIO_PH6
#define GPIO_CHN11        GPIO_PH7
#define GPIO_CHN12        GPIO_PE6
#define GPIO_CHN13        GPIO_PE7
#define GPIO_CHN14        GPIO_PE0
#define GPIO_CHN15        GPIO_PE1

#define PG4_OUTPUT_ENABLE 1
#define PG5_OUTPUT_ENABLE 1
#define PG6_OUTPUT_ENABLE 1
#define PG7_OUTPUT_ENABLE 1
#define PH0_OUTPUT_ENABLE 1
#define PH1_OUTPUT_ENABLE 1
#define PH2_OUTPUT_ENABLE 1
#define PH3_OUTPUT_ENABLE 1

#define PH4_OUTPUT_ENABLE 1
#define PH5_OUTPUT_ENABLE 1
#define PH6_OUTPUT_ENABLE 1
#define PH7_OUTPUT_ENABLE 1
#define PE6_OUTPUT_ENABLE 1
#define PE7_OUTPUT_ENABLE 1
#define PE0_OUTPUT_ENABLE 1
#define PE1_OUTPUT_ENABLE 1

#define PG4_INPUT_ENABLE  0
#define PG5_INPUT_ENABLE  0
#define PG6_INPUT_ENABLE  0
#define PG7_INPUT_ENABLE  0
#define PH0_INPUT_ENABLE  0
#define PH1_INPUT_ENABLE  0
#define PH2_INPUT_ENABLE  0
#define PH3_INPUT_ENABLE  0

#define PH4_INPUT_ENABLE  0
#define PH5_INPUT_ENABLE  0
#define PH6_INPUT_ENABLE  0
#define PH7_INPUT_ENABLE  0
#define PE6_INPUT_ENABLE  0
#define PE7_INPUT_ENABLE  0
#define PE0_INPUT_ENABLE  0
#define PE1_INPUT_ENABLE  0

#define PG4_FUNC          AS_GPIO
#define PG5_FUNC          AS_GPIO
#define PG6_FUNC          AS_GPIO
#define PG7_FUNC          AS_GPIO
#define PH0_FUNC          AS_GPIO
#define PH1_FUNC          AS_GPIO
#define PH2_FUNC          AS_GPIO
#define PH3_FUNC          AS_GPIO

#define PH4_FUNC          AS_GPIO
#define PH5_FUNC          AS_GPIO
#define PH6_FUNC          AS_GPIO
#define PH7_FUNC          AS_GPIO
#define PE6_FUNC          AS_GPIO
#define PE7_FUNC          AS_GPIO
#define PE0_FUNC          AS_GPIO
#define PE1_FUNC          AS_GPIO
#endif //end of DEBUG_GPIO_ENABLE

#endif

#endif /* VENDOR_COMMON_BOARDS_TL751X_EVK_C1T360A20_H_ */
