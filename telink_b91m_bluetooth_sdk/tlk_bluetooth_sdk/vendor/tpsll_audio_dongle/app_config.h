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

#define DEBUG_WFI_ENABLE				0

#define TLK_CFG_FLASH_PROT_ENABLE		0

/**
 *  @brief  SCHEDULER SIMULATE Configuration
 */
#define SCHEDULER_SIMULATE_TASK_ENABLE 	0

#if DEBUG_WFI_ENABLE
    #define TLK_DEV_KEY_ENABLE             	1
    #define TLK_DEV_LED_ENABLE             	0
    #define TLK_DEV_CODEC_ENABLE           	1
    #define TLK_MW_TINYSQL_ENABLE          	1
    #define TLK_CFG_UART_TOOL_ENABLE     	0
#else
    #define TLK_DEV_KEY_ENABLE             	1
    #define TLK_DEV_LED_ENABLE             	1
    #define TLK_DEV_CODEC_ENABLE           	1
    #define TLK_MW_TINYSQL_ENABLE          	1
    #define TLK_CFG_UART_TOOL_ENABLE     	1
#endif

#if DEBUG_WFI_ENABLE
    #define TLK_USB_UDB_ENABLE 		   	    0
#else
    #define TLK_USB_UDB_ENABLE 		   	    1
#endif

#define TLK_USB_UAC_ENABLE      		1
#define TLKUSB_AUD_MIC_RESOLUTION_BIT   24
#define TLKUSB_AUD_SPK_RESOLUTION_BIT   24
///////////////////////// Stack Configuration//////////////////////////////////////////////////

#define TLK_STK_TPD_ENABLE   			1

#define TLK_DBG_AUDIO_ENABLE  			TLK_DEBUG_ENABLE

#define VCD_DEFINE_SELECT     VCD_DEFINE_TPSLL // VCD_DEFINE_TPSLL//VCD_DEFINE_SIMULATE


#define TLK_MW_LL_DONGLE_PHONE_ENABLE 1
#define TLK_MW_LL_DONGLE_MUSIC_ENABLE 1


/* GPIO */
#if DEBUG_WFI_ENABLE
    #define DEBUG_GPIO_ENABLE 				0
    #define DEBUG_COMMON_GPIO_ENABLE		0
    #define DEBUG_MINGQIAN_GPIO_ENABLE		0
    #define DEBUG_YATING_GPIO_ENABLE		0
#else
    #define DEBUG_GPIO_ENABLE 				0
    #define DEBUG_COMMON_GPIO_ENABLE		0
    #define DEBUG_MINGQIAN_GPIO_ENABLE		0
    #define DEBUG_YATING_GPIO_ENABLE		0
#endif

#define DONGLE_AUDIO_PATH_GPIO_DEBUG    0

#define TLK_SPI_DEBUG_ENABLE            0
#define TLK_I2S_DEBUG_ENABLE            0

/* print log */
#if (!DBG_WFI_ENABLE)
#define APP_LOG_EN                     	1

#define DBG_LOG_SIMULATE_FIXED_TASK    	1
#define DBG_LOG_SIMULATE_FLEXIBLE_TASK 	1
#define DBG_LOG_SIMULATE_INSERT_TASK   	1
#endif

#define DRIVER_FAST_SETTLE_EN           (1)

//ULTRA LOW LATENCY EN
#define ULTRA_LOW_LATENCY_EN            1
#define AUDIO_PATH_24BITS_EN            1

/////////////////////// Algorithm Select Configuration //////////////////////////////
#if ULTRA_LOW_LATENCY_EN
#define TLKALG_LC3_PLUS_ENC_ENABLE              1
#define TLKALG_LC3_PLUS_DEC_ENABLE              1
#else
#define TLKALG_LC3_24BIT_ENC_ENABLE             1
#define TLKALG_LC3_24BIT_DEC_ENABLE             1
#endif

//asrc/ppm algorithm
#define TLKALG_ASRC_16TO48_24BIT_ENABLE         1
#define TLKALG_HIGH_PERFORMANCE_24BITS_EN       1
#define TLKALG_HIGH_PERFORMANCE_EN              1

#define TLKMDI_AUDMEM_TOTAL_SIZE (95 * 1024)

#include "vendor/common/default_config.h"
