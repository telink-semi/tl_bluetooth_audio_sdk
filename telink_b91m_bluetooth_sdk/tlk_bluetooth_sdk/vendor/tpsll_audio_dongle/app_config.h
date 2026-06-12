/********************************************************************************************************
 * @file    app_config.h
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

#include "core/mcu_type.h"

/////////////////////// Board Select Configuration //////////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
#define TLKHW_TYPE BOARD_721X_EVK_C1T315A20
#else
#error "config hardware !!!"
#endif

#define DEBUG_LOW_POWER_ENABLE 0
#define TLK_CFG_SUSPEND_ENABLE 0 //1
/**
 *  @brief  SCHEDULER SIMULATE Configuration
 */
#define SCHEDULER_SIMULATE_TASK_ENABLE 0

#define TLK_DEV_KEY_ENABLE             1
#define TLK_DEV_LED_ENABLE             1
#define TLK_DEV_CODEC_ENABLE           1
#define TLK_MW_TINYSQL_ENABLE          1
#define TLK_PM_BUSY_CHN_USB            8 // TLKSYS_PM_CHN_APP0

#if (DEBUG_LOW_POWER_ENABLE || TLK_CFG_SUSPEND_ENABLE)
#define TLK_CFG_UART_TOOL_ENABLE 0
#define TLK_USB_UDB_ENABLE       0
#else
#define TLK_CFG_UART_TOOL_ENABLE 1
#define TLK_USB_UDB_ENABLE       1
#endif

// BT TPSLL OPTIMIZE LATENCY TEST//
#define BT_TPSLL_OPTIMIZE_LATENCY_TEST 1


// #define TLK_MW_USER_CTRL_ENABLE (1)
// #define TLK_MW_OTA_ENABLE       (1 && TLK_MW_USER_CTRL_ENABLE)

#define TLK_USB_UAC_ENABLE        1

#define TLKALG_UAC_PPM_MIC_ENABLE 0
#define TLKALG_UAC_PPM_SPK_ENABLE 0
#define TLKALG_UAC_PPM_ENABLE     0
#define APP_USB_ISO_IN_BUFF_SIZE  (1024 * 2)
#define APP_USB_ISO_OUT_BUFF_SIZE (1024 * 2)

#define TLKALG_PPM_MIC_ENABLE     1
#define TLKALG_PPM_SPK_ENABLE     1
#define TLKALG_PPM_CALC_BY_SAMPLE 1

//this two control 24bit or 16bit ppm in UAC
#define TLKALG_PPM_MIC_24BIT_ENABLE     0
#define TLKUAC_PPM_SPK_24BIT_ENABLE     0 //24BIT SPK

#define TLKUSB_AUD_MIC_RESOLUTION_BIT   24
#define TLKUSB_AUD_SPK_RESOLUTION_BIT   24
#define TLK_USB_UAC_DUAL_SOUNDCARD_MODE 1
#define TLK_USB_REMOTEWAKEUP_EN         1

///////////////////////// Stack Configuration//////////////////////////////////////////////////

#define TLK_STK_TPD_ENABLE            1

#define TLK_DBG_AUDIO_ENABLE          TLK_DEBUG_ENABLE

#define VCD_DEFINE_SELECT             VCD_DEFINE_TPSLL // VCD_DEFINE_TPSLL//VCD_DEFINE_SIMULATE


#define TLK_MW_LL_DONGLE_PHONE_ENABLE 1
#define TLK_MW_LL_DONGLE_MUSIC_ENABLE 1


/* GPIO */
#define DEBUG_GPIO_ENABLE            0
#define DEBUG_COMMON_GPIO_ENABLE     0
#define DEBUG_MINGQIAN_GPIO_ENABLE   0
#define DEBUG_YATING_GPIO_ENABLE     0

#define DONGLE_AUDIO_PATH_GPIO_DEBUG 1

#define TLK_SPI_DEBUG_ENABLE         0
#define TLK_I2S_DEBUG_ENABLE         0

/* print log */
#define APP_LOG_EN            1
#define DRIVER_FAST_SETTLE_EN (1)

//ULTRA LOW LATENCY EN
#define ULTRA_LOW_LATENCY_EN 1
#define AUDIO_PATH_24BITS_EN 1

/////////////////////// Algorithm Select Configuration //////////////////////////////
#if ULTRA_LOW_LATENCY_EN
#define TLKALG_LC3_PLUS_ENC_ENABLE 1
#define TLKALG_LC3_PLUS_DEC_ENABLE 1
#else
#define TLKALG_LC3_24BIT_ENC_ENABLE 1
#define TLKALG_LC3_24BIT_DEC_ENABLE 1
#endif

//asrc/ppm algorithm
#define TLKALG_ASRC_16TO48_24BIT_ENABLE   1
#define TLKALG_HIGH_PERFORMANCE_24BITS_EN 0
#define TLKALG_HIGH_PERFORMANCE_EN        0

#define TLKMDI_AUDMEM_TOTAL_SIZE          (62 * 1024)

#include "vendor/common/default_config.h"
