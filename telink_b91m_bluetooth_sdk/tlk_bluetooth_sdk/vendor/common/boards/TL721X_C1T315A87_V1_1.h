/********************************************************************************************************
 * @file    TL721X_C1T315A87_V1_1.h
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

#if (TLKHW_TYPE == BOARD_721X_EVK_C1T315A87_V1_0)


#define WAKEUP_PAD GPIO_PB3
/**
 *  @brief  LED Configuration
 */
#if TLK_DEV_LED_ENABLE
/**
     *  @brief  Definition gpio for led
     */
#define GPIO_LED_BLUE        GPIO_PF5
#define GPIO_LED_BLUE_PWM_ID PWM0_ID
#define GPIO_LED_RED         GPIO_PF6
#define GPIO_LED_RED_PWM_ID  PWM1_ID

#define GPIO_LED_NUMS        2
#define LED_ON_LEVEL         1 //gpio output high voltage to turn on led

#endif


#if (TLK_DEV_KEY_ENABLE)
#define TLKDRV_KEY_MAX_NUMB 1

#define KEY1_ID             TLKDRV_KEY_DID_KEY1
#define KEY1_GPIO_IN        GPIO_PB3
#define KEY1_GPIO_OUT       0

#else

#define TLKDRV_KEY_MAX_NUMB 0

#endif // #if (TLK_DEV_KEY_ENABLE)

#define ANT_VDD_GPIO GPIO_PE0
#define ANT_VC_GPIO  GPIO_PE7

#if (TLK_DEV_SERIAL_ENABLE)

//UART1
#define TLKDEV_SERIAL1_TX_PIN GPIO_PB0
#define TLKDEV_SERIAL1_RX_PIN GPIO_PB1

//UART0
// #define TLKDEV_SERIAL0_TX_PIN  GPIO_PE6
// #define TLKDEV_SERIAL0_RX_PIN  GPIO_PE7
#endif //TLK_DEV_SERIAL_ENABLE


#ifndef JTAG_DEBUG_DISABLE
#define JTAG_DEBUG_DISABLE 1
#endif

/**
 * @brief AUDIO(Codec) Mode Configuration
 */

#if (TLKDRV_CODEC_ICODEC_ENABLE)
#define TLKDRV_CODEC_MIC_FIFO       FIFO0
#define TLKDRV_CODEC_SPK_FIFO       FIFO0

#define TLKDRV_ICODEC_POWER_PIN     GPIO_FC_PC2
#define TLKDRV_ICODEC_DMIC_CLK0_PIN GPIO_FC_PC0
#define TLKDRV_ICODEC_DMIC_CLK1_PIN GPIO_NONE_PIN
#define TLKDRV_ICODEC_DMIC_DATA_PIN GPIO_FC_PC1
#define TLKDRV_ICODEC_SDM0P_PIN     GPIO_FC_PA0
#define TLKDRV_ICODEC_SDM0N_PIN     GPIO_FC_PA1
#define TLKDRV_ICODEC_SDM1P_PIN     GPIO_FC_PF4
#define TLKDRV_ICODEC_SDM1N_PIN     GPIO_FC_PF5
#elif (TLKDRV_CODEC_I2S_MASTER_ENABLE)
#define TLKDRV_I2S_MST_CHN         I2S1

#define TLKDRV_CODEC_MIC_FIFO      FIFO1
#define TLKDRV_CODEC_SPK_FIFO      FIFO1

#define TLKDRV_I2S_MST_BCLIK_PIN   GPIO_FC_PB3
#define TLKDRV_I2S_MST_DAC_CLK_PIN GPIO_FC_PB5
#define TLKDRV_I2S_MST_DAC_DAT_PIN GPIO_FC_PB4
#define TLKDRV_I2S_MST_ADC_CLK_PIN GPIO_FC_PB5
#define TLKDRV_I2S_MST_ADC_DAT_PIN GPIO_FC_PB7
#elif (TLKDRV_CODEC_I2S_SLAVE_ENABLE)
#define TLKDRV_IISSLV_CHN         I2S1

#define TLKDRV_CODEC_MIC_FIFO     FIFO1
#define TLKDRV_CODEC_SPK_FIFO     FIFO1

#define TLKDRV_IISSLV_BCLIK_PIN   0
#define TLKDRV_IISSLV_DAC_CLK_PIN 0
#define TLKDRV_IISSLV_DAC_DAT_PIN 0
#define TLKDRV_IISSLV_ADC_CLK_PIN 0
#define TLKDRV_IISSLV_ADC_DAT_PIN 0
#else
#define TLKDRV_CODEC_MIC_FIFO FIFO0
#define TLKDRV_CODEC_SPK_FIFO FIFO0
#endif /* TLKDRV_CODEC_ICODEC_ENABLE */

#define AUDIO_CODEC_LOOPBACK 0 ///for codec test,default 0

#endif
