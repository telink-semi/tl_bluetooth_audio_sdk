/********************************************************************************************************
 * @file    TL753X_C1T413A20_V1_0.h
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
//from TL751X_C1T368A20_V1_0.h
#ifndef TL753X_C1T413A20_V1_0_H_
#define TL753X_C1T413A20_V1_0_H_


#if (TLKHW_TYPE == TLKHW_TL753X_EVK_C1T413A20_V1_0)

#define WAKEUP_PAD GPIO_PB2
/**
 *  @brief  LED Configuration
 */
#if TLK_DEV_LED_ENABLE
/**
     *  @brief  Definition gpio for led
     */
#define GPIO_LED_BLUE        GPIO_PG5
#define GPIO_LED_BLUE_PWM_ID 0XFF //PWM0_ID
#define GPIO_LED_RED         GPIO_PG6
#define GPIO_LED_RED_PWM_ID  0XFF //PWM1_ID

#define GPIO_LED_NUMS        2
#define LED_ON_LEVEL         1 //gpio output high voltage to turn on led

#endif


#if (TLK_DEV_KEY_ENABLE)
#define TLKDRV_KEY_MAX_NUMB 2

#define KEY1_ID             TLKDRV_KEY_DID_KEY1 // SW24
#define KEY1_GPIO_IN        GPIO_PD4
#define KEY1_GPIO_OUT       0

#define KEY2_ID             TLKDRV_KEY_DID_KEY2 // SW23
#define KEY2_GPIO_IN        GPIO_PD3
#define KEY2_GPIO_OUT       0

#if (TLK_DEV_SERIAL_ENABLE)
//UART1
#define TLKDEV_SERIAL1_TX_PIN GPIO_PH0
#define TLKDEV_SERIAL1_RX_PIN GPIO_PH1
#endif //TLK_DEV_SERIAL_ENABLE

#else

#define TLKDRV_KEY_MAX_NUMB 0

#endif // #if (TLK_DEV_KEY_ENABLE)

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
 * PA      TL_LSPI_CK       TL_LSPI_DM      TL_LSPI_MOSI     TL_LSPI_MISO      TL_LSPI_IO2      TL_LSPI_IO3     TL_LSPI_IO4          SWS
 *
 * PB          X                X                X                X                X                X                X                X
 *
 * PC          X                X                X                X                X            DEBUG_GPIO       DEBUG_GPIO       TL_USB1_DM
 *
 * PD      TL_USB1_DP       DEBUG_GPIO       DEBUG_GPIO         TL_TCK           TL_TMS           TL_TDO          TL_TDI         TL_DMIC0_CLK
 *
 * PE     TL_DMIC0_DAT          X                X                X                X                X                X                X
 *
 * PF       LCM_RST         DEBUG_GPIO       DEBUG_GPIO       DEBUG_GPIO       DEBUG_GPIO           DFU         TL_DMIC1_CLK     TL_DMIC1_DAT
 *
 * PG     TL_I2S0_BCK     TL_I2S0_LR0       TL_I2S0_DAT0      TL_I2S0_LR1     TL_I2S0_DAT1        TL_LED1         TL_LED2          TL_LED3
 *
 * PH       UART_TX         UART_RX          DEBUG_GPIO       DEBUG_GPIO        DEBUG_GPIO       DEBUG_GPIO          X                X
 *
 * PI     TL_MSPImosi      TL_MSPIck         TL_MSPIio3       TL_MSPIcn        TL_MSPImiso       TL_MSPIio2      DEBUG_GPIO       DEBUG_GPIO
 * 
 * PJ      DEBUG_GPIO      DEBUG_GPIO        TL_HSPImosi      TL_HSPIio6       TL_HSPImiso       TL_HSPIio7          X                X
 *
 * PK          X                X                X                X                X                X                X                X
 * 
 */
#if (DEBUG_COMMON_GPIO_ENABLE || DEBUG_ZHAOWEI_GPIO_ENABLE || DEBUG_JUNWEI_GPIO_ENABLE || DEBUG_ZEWEN_GPIO_ENABLE || DEBUG_MINGQIAN_GPIO_ENABLE || DEBUG_TIANXIANG_GPIO_ENABLE || \
     DEBUG_ZHISHENG_GPIO_ENABLE || DEBUG_GPIO_ENABLE || DEBUG_SIHUI_GPIO_ENABLE || DEBUG_SHUYOU_GPIO_ENABLE || DEBUG_SIMULATE_GPIO_ENABLE || DEBUG_YATING_GPIO_ENABLE)
#define GPIO_CHN0         GPIO_PC5
#define GPIO_CHN1         GPIO_PC6
#define GPIO_CHN2         GPIO_PD1
#define GPIO_CHN3         GPIO_PD2
#define GPIO_CHN4         GPIO_PF1
#define GPIO_CHN5         GPIO_PF2
#define GPIO_CHN6         GPIO_PF3
#define GPIO_CHN7         GPIO_PF4

#define GPIO_CHN8         GPIO_PH2
#define GPIO_CHN9         GPIO_PH3
#define GPIO_CHN10        GPIO_PH4
#define GPIO_CHN11        GPIO_PH5
#define GPIO_CHN12        GPIO_PI6
#define GPIO_CHN13        GPIO_PI7
#define GPIO_CHN14        GPIO_PJ0
#define GPIO_CHN15        GPIO_PJ1

#define PC5_OUTPUT_ENABLE 1
#define PC6_OUTPUT_ENABLE 1
#define PD1_OUTPUT_ENABLE 1
#define PD2_OUTPUT_ENABLE 1
#define PF1_OUTPUT_ENABLE 1
#define PF2_OUTPUT_ENABLE 1
#define PF3_OUTPUT_ENABLE 1
#define PF4_OUTPUT_ENABLE 1

#define PH2_OUTPUT_ENABLE 1
#define PH3_OUTPUT_ENABLE 1
#define PH4_OUTPUT_ENABLE 1
#define PH5_OUTPUT_ENABLE 1
#define PI6_OUTPUT_ENABLE 1
#define PI7_OUTPUT_ENABLE 1
#define PJ0_OUTPUT_ENABLE 1
#define PJ1_OUTPUT_ENABLE 1


#define PC5_INPUT_ENABLE  0
#define PC6_INPUT_ENABLE  0
#define PD1_INPUT_ENABLE  0
#define PD2_INPUT_ENABLE  0
#define PF1_INPUT_ENABLE  0
#define PF2_INPUT_ENABLE  0
#define PF3_INPUT_ENABLE  0
#define PF4_INPUT_ENABLE  0

#define PH2_INPUT_ENABLE  0
#define PH3_INPUT_ENABLE  0
#define PH4_INPUT_ENABLE  0
#define PH5_INPUT_ENABLE  0
#define PI6_INPUT_ENABLE  0
#define PI7_INPUT_ENABLE  0
#define PJ0_INPUT_ENABLE  0
#define PJ1_INPUT_ENABLE  0


#define PC5_FUNC          AS_GPIO
#define PC6_FUNC          AS_GPIO
#define PD1_FUNC          AS_GPIO
#define PD2_FUNC          AS_GPIO
#define PF1_FUNC          AS_GPIO
#define PF2_FUNC          AS_GPIO
#define PF3_FUNC          AS_GPIO
#define PF4_FUNC          AS_GPIO

#define PH2_FUNC          AS_GPIO
#define PH3_FUNC          AS_GPIO
#define PH4_FUNC          AS_GPIO
#define PH5_FUNC          AS_GPIO
#define PI6_FUNC          AS_GPIO
#define PI7_FUNC          AS_GPIO
#define PJ0_FUNC          AS_GPIO
#define PJ1_FUNC          AS_GPIO

#endif //end of DEBUG_GPIO_ENABLE

#endif

#endif /* TL753X_C1T413A20_V1_0_H_ */
