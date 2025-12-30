/********************************************************************************************************
 * @file    TL721X_C1T315A20.h
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
#ifndef VENDOR_COMMON_BOARDS_TL721X_C1T315A20_H_
#define VENDOR_COMMON_BOARDS_TL721X_C1T315A20_H_
/*
 * GPIO Usage Overview:
 *
 * NOTE: X means gpio not pinned,  NUS means gpio not used
 *
 * Port         0                1                2                3                4                5                6                7
 * ------------------------------------------------------------------------------------------------------------------------------------------------
 * PA        BBDUG            BBDUG         BBDUG/DMICDAT        DMICCK1          DMICCK2          USB_DM           USB_DP            SWS
 *
 * PB      JTAG_CFG           NUS               NUS               NUS             NUS               NUS              NUS              NUS
 *
 * PC       LED_B             LED_R            LED_G             LED_W          JTAG_TDI          JTAG-TDO          JTAG_TMSX         JTAG_TCK
 *
 * PD       AMIC              AMIC_BIAS          32k              32k            TL_Key1           TL_Key3          TL_Key2          TL_Key4
 *
 * PE      GPIO_DEBUG        GPIO_DEBUG        GPIO_DEBUG        GPIO_DEBUG      GPIO_DEBUG       GPIO_DEBUG        GPIO_DEBUG       GPIO_DEBUG
 *
 * PF      GPIO_DEBUG        GPIO_DEBUG        GPIO_DEBUG        GPIO_DEBUG      GPIO_DEBUG       GPIO_DEBUG        GPIO_DEBUG       GPIO_DEBUG
 *
 *
 */
#if (TLKHW_TYPE == BOARD_721X_EVK_C1T315A20)

/**
 *  @brief  Keyboard Configuration
 */
#if (UI_KEYBOARD_ENABLE)
#define MATRIX_ROW_PULL    PM_PIN_PULLDOWN_100K
#define MATRIX_COL_PULL    PM_PIN_PULLUP_10K

#define KB_LINE_HIGH_VALID 0 //drive pin output 0 when scan key, scan pin read 0 is valid

#define BTN_PAIR           0x01
#define BTN_UNPAIR         0x02

#define CR_VOL_UP          0xf0 ////
#define CR_VOL_DN          0xf1

/**
     *  @brief  Normal keyboard map
     */
#define KB_MAP_NORMAL           \
    {                           \
        {BTN_UNPAIR, BTN_PAIR}, \
        {CR_VOL_UP, CR_VOL_DN}, \
    }

//////////////////// KEY CONFIG (EVK board) ///////////////////////////
#define KB_DRIVE_PINS {GPIO_PD4, GPIO_PD6}
#define KB_SCAN_PINS  {GPIO_PD5, GPIO_PD7}

//scan pin as gpio
#define PD4_FUNC AS_GPIO
#define PD6_FUNC AS_GPIO

//scan  pin need 10K pullup
#define PULL_WAKEUP_SRC_PD4 MATRIX_ROW_PULL
#define PULL_WAKEUP_SRC_PD6 MATRIX_ROW_PULL

//scan pin open input to read gpio level
#define PD4_INPUT_ENABLE 1
#define PD6_INPUT_ENABLE 1

//drive pin as gpio
#define PD5_FUNC AS_GPIO
#define PD7_FUNC AS_GPIO

//drive pin need 100K pulldown
#define PULL_WAKEUP_SRC_PD5 MATRIX_COL_PULL
#define PULL_WAKEUP_SRC_PD7 MATRIX_COL_PULL

//drive pin open input to read gpio wakeup level
#define PD5_INPUT_ENABLE 1
#define PD7_INPUT_ENABLE 1

#endif


/**
 *  @brief  LED Configuration
 */
#if TLK_DEV_LED_ENABLE
/**
     *  @brief  Definition gpio for led
     */
#define GPIO_LED_BLUE         GPIO_PC0
#define GPIO_LED_BLUE_PWM_ID  0xff

#define GPIO_LED_GREEN        GPIO_PC2
#define GPIO_LED_GREEN_PWM_ID 0xff // 0xff is no pwm founction


#define GPIO_LED_WHITE        GPIO_PC3
#define GPIO_LED_WHITE_PWM_ID 0xff // 0xff is no pwm founction

#define GPIO_LED_RED          GPIO_PC1
#define GPIO_LED_RED_PWM_ID   0xff

#define GPIO_LED_NUMS         4

#define PC0_FUNC              AS_GPIO
#define PC2_FUNC              AS_GPIO
#define PC3_FUNC              AS_GPIO
#define PC1_FUNC              AS_GPIO

#define PC0_OUTPUT_ENABLE     1
#define PC2_OUTPUT_ENABLE     1
#define PC3_OUTPUT_ENABLE     1
#define PC1_OUTPUT_ENABLE     1

#define LED_ON_LEVEL          1 //gpio output high voltage to turn on led

#endif

#if (TLK_DEV_KEY_ENABLE)
#define TLKDRV_KEY_MAX_NUMB 4

#define KEY1_ID             TLKDRV_KEY_DID_KEY1 //SW4
#define KEY1_GPIO_IN        GPIO_PD4
#define KEY1_GPIO_OUT       GPIO_PD7

#define KEY2_ID             TLKDRV_KEY_DID_KEY2 //SW6
#define KEY2_GPIO_IN        GPIO_PD6
#define KEY2_GPIO_OUT       GPIO_PD7

#define KEY3_ID             TLKDRV_KEY_DID_KEY3 //SW3
#define KEY3_GPIO_IN        GPIO_PD4
#define KEY3_GPIO_OUT       GPIO_PD5

#define KEY4_ID             TLKDRV_KEY_DID_KEY4 //SW5
#define KEY4_GPIO_IN        GPIO_PD6
#define KEY4_GPIO_OUT       GPIO_PD5

#else

#define TLKDRV_KEY_MAX_NUMB 0

#endif // #if (TLK_DEV_KEY_ENABLE)

#if (TLK_DEV_SERIAL_ENABLE)

//UART1
#define TLKDEV_SERIAL1_TX_PIN GPIO_PC6
#define TLKDEV_SERIAL1_RX_PIN GPIO_PC7
#define TLKDEV_SERIAL1_TX_DMA DMA0
#define TLKDEV_SERIAL1_RX_DMA DMA1

//UART0
#define TLKDEV_SERIAL0_TX_PIN GPIO_PE6
#define TLKDEV_SERIAL0_RX_PIN GPIO_PE7
#define TLKDEV_SERIAL0_TX_DMA DMA6
#define TLKDEV_SERIAL0_RX_DMA DMA7
#endif //TLK_DEV_SERIAL_ENABLE


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

#if (TLKDRV_CODEC_ICODEC_ENABLE)
#define TLKDRV_CODEC_MIC_DMA        DMA2
#define TLKDRV_CODEC_MIC_FIFO       FIFO0
#define TLKDRV_CODEC_SPK_DMA        DMA3
#define TLKDRV_CODEC_SPK_FIFO       FIFO0

#define TLKDRV_ICODEC_POWER_PIN     0
#define TLKDRV_ICODEC_DMIC_CLK0_PIN GPIO_FC_PA3
#define TLKDRV_ICODEC_DMIC_CLK1_PIN GPIO_FC_PA4
#define TLKDRV_ICODEC_DMIC_DATA_PIN GPIO_FC_PA2
#define TLKDRV_ICODEC_SDM0P_PIN     GPIO_FC_PA0
#define TLKDRV_ICODEC_SDM0N_PIN     GPIO_FC_PA1
#define TLKDRV_ICODEC_SDM1P_PIN     GPIO_FC_PF4
#define TLKDRV_ICODEC_SDM1N_PIN     GPIO_FC_PF5
#elif (TLKDRV_CODEC_I2S_MASTER_ENABLE)
#define TLKDRV_I2S_MST_CHN         I2S1

#define TLKDRV_CODEC_MIC_DMA       DMA2
#define TLKDRV_CODEC_MIC_FIFO      FIFO1
#define TLKDRV_CODEC_SPK_DMA       DMA3
#define TLKDRV_CODEC_SPK_FIFO      FIFO1

#define TLKDRV_I2S_MST_BCLIK_PIN   GPIO_FC_PB3
#define TLKDRV_I2S_MST_DAC_CLK_PIN GPIO_FC_PB5
#define TLKDRV_I2S_MST_DAC_DAT_PIN GPIO_FC_PB4
#define TLKDRV_I2S_MST_ADC_CLK_PIN GPIO_FC_PB5
#define TLKDRV_I2S_MST_ADC_DAT_PIN GPIO_FC_PB7
#elif (TLKDRV_CODEC_I2S_SLAVE_ENABLE)
#define TLKDRV_IISSLV_CHN         I2S1

#define TLKDRV_CODEC_MIC_DMA      DMA2
#define TLKDRV_CODEC_MIC_FIFO     FIFO1
#define TLKDRV_CODEC_SPK_DMA      DMA3
#define TLKDRV_CODEC_SPK_FIFO     FIFO1

#define TLKDRV_IISSLV_BCLIK_PIN   0
#define TLKDRV_IISSLV_DAC_CLK_PIN 0
#define TLKDRV_IISSLV_DAC_DAT_PIN 0
#define TLKDRV_IISSLV_ADC_CLK_PIN 0
#define TLKDRV_IISSLV_ADC_DAT_PIN 0
#else
#define TLKDRV_CODEC_MIC_DMA  DMA2
#define TLKDRV_CODEC_MIC_FIFO FIFO0
#define TLKDRV_CODEC_SPK_DMA  DMA3
#define TLKDRV_CODEC_SPK_FIFO FIFO0
#endif /* TLKDRV_CODEC_ICODEC_ENABLE */

#define AUDIO_CODEC_LOOPBACK 0 ///for codec test,default 0

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
     * @brief USB Configuration
     */
#if 0 //(TLK_CFG_USB_ENABLE)
#define TLK_CFG_USB_NUMB       1

#define TLK_CFG_USB_UDB_INDEX  0
#define TLK_CFG_USB_UAC_INDEX  0
#define TLK_CFG_USB_MSC_INDEX  (TLK_CFG_USB_NUMB + 1) //default noused

#define TLK_USB0_DP_PIN        GPIO_PA6
#define TLK_USB0_DM_PIN        GPIO_PA5
#define TLK_USB1_DP_PIN        0 //Do not support.
#define TLK_USB1_DM_PIN        0

#define TLKUSB_UDB_EDP_DBG_IN  USB_EDP3_IN
#define TLKUSB_UDB_EDP_DBG_OUT USB_EDP5_OUT
#define TLKUSB_UDB_EDP_VCD_IN  USB_EDP8_IN
#define TLKUSB_UDB_EDP_VCD_OUT USB_EDP6_OUT

#define TLKUSB_UAC_EDP_HID     USB_EDP1_IN
#define TLKUSB_UAC_EDP_MIC     USB_EDP7_IN
#define TLKUSB_UAC_EDP_SPK     USB_EDP6_OUT
#endif

/**
 *  @brief  GPIO definition for debug_io
 */
#if (DEBUG_COMMON_GPIO_ENABLE || DEBUG_GPIO_ENABLE || DEBUG_MINGQIAN_GPIO_ENABLE || DEBUG_TIANXIANG_GPIO_ENABLE || DEBUG_SHUYOU_GPIO_ENABLE || DEBUG_YATING_GPIO_ENABLE)
#define GPIO_CHN0         GPIO_PE0
#define GPIO_CHN1         GPIO_PE1
#define GPIO_CHN2         GPIO_PE2
#define GPIO_CHN3         GPIO_PE3
#define GPIO_CHN4         GPIO_PE4
#define GPIO_CHN5         GPIO_PE5
#define GPIO_CHN6         GPIO_PE6
#define GPIO_CHN7         GPIO_PF3

#define GPIO_CHN8         GPIO_PF4
#define GPIO_CHN9         GPIO_PF6
#define GPIO_CHN10        GPIO_PF7
#define GPIO_CHN11        GPIO_PE7
#define GPIO_CHN12        GPIO_PF0
#define GPIO_CHN13        GPIO_PF1
#define GPIO_CHN14        GPIO_PF2
#define GPIO_CHN15        GPIO_PF5


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
#define PE7_OUTPUT_ENABLE 1
#define PF0_OUTPUT_ENABLE 1
#define PF1_OUTPUT_ENABLE 1
#define PF2_OUTPUT_ENABLE 1
#define PF5_OUTPUT_ENABLE 1

#endif //end of DEBUG_GPIO_ENABLE

#define TLKAPI_DEBUG_GPIO_PIN GPIO_PB7

#endif ////* VENDOR_COMMON_BOARDS_TL721X_C1T315A20_H_ */

#endif
