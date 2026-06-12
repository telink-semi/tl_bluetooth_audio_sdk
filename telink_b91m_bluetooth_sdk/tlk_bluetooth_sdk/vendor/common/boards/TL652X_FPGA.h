/********************************************************************************************************
 * @file    TL652X_FPGA.h
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
#ifndef VENDOR_COMMON_BOARDS_TL652X_FPGA_H_
#define VENDOR_COMMON_BOARDS_TL652X_FPGA_H_


#if (TLKHW_TYPE == TLKHW_TL652X_FPGA)

#define WAKEUP_PAD GPIO_PB2
/**
 *  @brief  LED Configuration
 */
#if TLK_DEV_LED_ENABLE
/**
     *  @brief  Definition gpio for led
     */
#define GPIO_LED_GREEN        GPIO_PD0 /*pb5 green use for monitor N22 status*/
#define GPIO_LED_RED_PWM_ID   0xff     // 0xff is no pwm founction

#define GPIO_LED_BLUE         GPIO_PD1
#define GPIO_LED_BLUE_PWM_ID  0xff

#define GPIO_LED_RED          GPIO_PD2
#define GPIO_LED_RED_PWM_ID   0xff

#define GPIO_LED_WHITE        GPIO_PD3
#define GPIO_LED_WHITE_PWM_ID 0xff

#define GPIO_LED_NUMS         3

#define PD0_FUNC              AS_GPIO
#define PD1_FUNC              AS_GPIO
#define PD2_FUNC              AS_GPIO
#define PD3_FUNC              AS_GPIO

#define PD0_OUTPUT_ENABLE     1
#define PD1_OUTPUT_ENABLE     1
#define PD2_OUTPUT_ENABLE     1
#define PD3_OUTPUT_ENABLE     1

#define LED_ON_LEVEL          1 //gpio output high voltage to turn on led

#endif


#if (TLK_DEV_KEY_ENABLE)
#define TLKDRV_KEY_MAX_NUMB 4

#if (TLKHW_MINOR_VERSION == 0x0103)
#define KEY1_ID       TLKDRV_KEY_DID_KEY1 // SW7
#define KEY1_GPIO_IN  GPIO_PC1
#define KEY1_GPIO_OUT 0

#define KEY2_ID       TLKDRV_KEY_DID_KEY2 // SW34
#define KEY2_GPIO_IN  GPIO_PB5
#define KEY2_GPIO_OUT 0

#else

#define KEY1_ID       TLKDRV_KEY_DID_KEY1 // SW7
#define KEY1_GPIO_IN  GPIO_PB0
#define KEY1_GPIO_OUT 0

#define KEY2_ID       TLKDRV_KEY_DID_KEY2 // SW34
#define KEY2_GPIO_IN  GPIO_PD2
#define KEY2_GPIO_OUT 0

#endif //    #if (TLKHW_MINOR_VERSION == 0x0103)

#define KEY3_ID       TLKDRV_KEY_DID_KEY3 // SW35
#define KEY3_GPIO_IN  GPIO_PD3
#define KEY3_GPIO_OUT 0

#define KEY4_ID       TLKDRV_KEY_DID_KEY4
#define KEY4_GPIO_IN  GPIO_PD4
#define KEY4_GPIO_OUT 0

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
#endif
#if (TLK_CFG_HRA_ENABLE || PROJ_BTTPSLL_HEADSET == 1)
#define GPIO_CHANNEL0 GPIO_PI5
#define GPIO_CHANNEL1 GPIO_PF4
#define GPIO_CHANNEL2 GPIO_PE2
#define GPIO_CHANNEL3 GPIO_PE1
#define GPIO_CHANNEL4 GPIO_PE0
#define GPIO_CHANNEL5 GPIO_PD6
#define GPIO_CHANNEL6 GPIO_PD5
#define GPIO_CHANNEL7 GPIO_PD1
#endif

/**
 * @brief AUDIO(Codec) Mode Configuration
 */
#if (TLK_DEV_CODEC_ENABLE)
#define AUDIO_HD_HAC_DISABLE     0
#define AUDIO_HD_HAC_AND_CODEC   1
#define AUDIO_HD_HAC_INDEPENDENT 2 ///TODO
#define AUDIO_HD_HAC_ANC_MODE    3

#define AUDIO_CODEC_LOOPBACK     0 ///for codec test,default 0

#define AUDIO_HD_HAC_MODE        (AUDIO_HD_HAC_DISABLE)

#if (AUDIO_HD_HAC_MODE == AUDIO_HD_HAC_AND_CODEC)
#define AUDIO_HD_HAC_EN          1
#define AUDIO_HD_HAC_SPK_EQ_EN   0
#define AUDIO_HD_HAC_MIC_EQ_EN   0
#define AUDIO_HD_HAC_SPK_ASRC_EN 1
#define AUDIO_HD_HAC_MIC_ASRC_EN 1

#if (AUDIO_HD_HAC_SPK_ASRC_EN)
#else
#define TLKDRV_CODEC_SPK_FIFO FIFO0
#endif //#if(AUDIO_HAC_ASRC_EN)

#if (AUDIO_HD_HAC_MIC_ASRC_EN)
#define AUDIO_HAC_ADC_L_CH        HAC_CHN2
#define AUDIO_HAC_ADC_R_CH        HAC_CHN3
#define TLKDRV_AUDIO_MIC_LCH_FIFO FIFO2
#define TLKDRV_AUDIO_MIC_RCH_FIFO FIFO3
#else
#define TLKDRV_CODEC_MIC_FIFO FIFO0
#endif

#elif (AUDIO_HD_HAC_MODE == AUDIO_HD_HAC_INDEPENDENT)
#define AUDIO_HD_HAC_EN       1
#define AUDIO_HD_HAC_EQ_EN    0
#define AUDIO_HD_HAC_ASRC_EN  1

#define TLKDRV_CODEC_MIC_FIFO FIFO0
#define TLKDRV_CODEC_SPK_FIFO FIFO0

#else
#define AUDIO_HD_HAC_EN       0
#define TLKDRV_CODEC_MIC_FIFO FIFO0
#define TLKDRV_CODEC_SPK_FIFO FIFO0
#endif


#if (TLKDRV_CODEC_ICODEC_ENABLE)
#define TLKDRV_ICODEC_POWER_PIN     0
#define TLKDRV_ICODEC_DMIC_CLK0_PIN GPIO_PF6
#define TLKDRV_ICODEC_DMIC_CLK1_PIN GPIO_PF7
#define TLKDRV_ICODEC_DMIC_DATA_PIN GPIO_PG0
#elif (TLKDRV_CODEC_I2S_MASTER_ENABLE)
#define TLKDRV_I2S_MST_CHN         0
#define TLKDRV_I2S_MST_BCLIK_PIN   GPIO_PF0
#define TLKDRV_I2S_MST_DAC_CLK_PIN GPIO_PE7
#define TLKDRV_I2S_MST_DAC_DAT_PIN GPIO_PF2
#define TLKDRV_I2S_MST_ADC_CLK_PIN GPIO_PF1
#define TLKDRV_I2S_MST_ADC_DAT_PIN GPIO_PE1
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
#define TLKDEV_SERIAL0_TX_PIN GPIO_PE6
#define TLKDEV_SERIAL0_RX_PIN GPIO_PE4

//UART1
#define TLKDEV_SERIAL1_TX_PIN GPIO_PC6
#define TLKDEV_SERIAL1_RX_PIN GPIO_PC7
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
#define GPIO_CHN0         GPIO_PF0
#define GPIO_CHN1         GPIO_PE0
#define GPIO_CHN2         GPIO_PD7
#define GPIO_CHN3         GPIO_PD5
#define GPIO_CHN4         GPIO_PD6
#define GPIO_CHN5         GPIO_PD4
#define GPIO_CHN6         GPIO_PD3
#define GPIO_CHN7         GPIO_PD1

#define GPIO_CHN8         GPIO_PD2
#define GPIO_CHN9         GPIO_PD0
#define GPIO_CHN10        GPIO_PB2
#define GPIO_CHN11        GPIO_PB1
#define GPIO_CHN12        GPIO_PB0
#define GPIO_CHN13        GPIO_PA6
#define GPIO_CHN14        GPIO_PA5
#define GPIO_CHN15        GPIO_PA2

#define PF0_OUTPUT_ENABLE 1
#define PE0_OUTPUT_ENABLE 1
#define PD7_OUTPUT_ENABLE 1
#define PD5_OUTPUT_ENABLE 1
#define PD6_OUTPUT_ENABLE 1
#define PD4_OUTPUT_ENABLE 1
#define PD3_OUTPUT_ENABLE 1
#define PD1_OUTPUT_ENABLE 1

#define PD2_OUTPUT_ENABLE 1
#define PD0_OUTPUT_ENABLE 1
#define PB2_OUTPUT_ENABLE 1
#define PB1_OUTPUT_ENABLE 1
#define PB0_OUTPUT_ENABLE 1
#define PA6_OUTPUT_ENABLE 1
#define PA5_OUTPUT_ENABLE 1
#define PA2_OUTPUT_ENABLE 1

#define PF0_INPUT_ENABLE  0
#define PE0_INPUT_ENABLE  0
#define PD7_INPUT_ENABLE  0
#define PD5_INPUT_ENABLE  0
#define PD6_INPUT_ENABLE  0
#define PD4_INPUT_ENABLE  0
#define PD3_INPUT_ENABLE  0
#define PD1_INPUT_ENABLE  0

#define PD2_INPUT_ENABLE  0
#define PD0_INPUT_ENABLE  0
#define PB2_INPUT_ENABLE  0
#define PB1_INPUT_ENABLE  0
#define PB0_INPUT_ENABLE  0
#define PA6_INPUT_ENABLE  0
#define PA5_INPUT_ENABLE  0
#define PA2_INPUT_ENABLE  0

#define PF0_FUNC          AS_GPIO
#define PE0_FUNC          AS_GPIO
#define PD7_FUNC          AS_GPIO
#define PD5_FUNC          AS_GPIO
#define PD6_FUNC          AS_GPIO
#define PD4_FUNC          AS_GPIO
#define PD3_FUNC          AS_GPIO
#define PD1_FUNC          AS_GPIO

#define PD2_FUNC          AS_GPIO
#define PD0_FUNC          AS_GPIO
#define PB2_FUNC          AS_GPIO
#define PB1_FUNC          AS_GPIO
#define PB0_FUNC          AS_GPIO
#define PA6_FUNC          AS_GPIO
#define PA5_FUNC          AS_GPIO
#define PA2_FUNC          AS_GPIO

#endif //end of DEBUG_GPIO_ENABLE

#endif

#endif /* VENDOR_COMMON_BOARDS_TL652X_FPGA_H_ */
