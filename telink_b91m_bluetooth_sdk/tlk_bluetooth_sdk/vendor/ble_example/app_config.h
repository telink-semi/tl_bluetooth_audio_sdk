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
#include "app_example.h"

#if (MCU_CORE_TYPE == CHIP_TYPE_TL721X)
#define GREATER_TX_POWER_EN 1
#endif
// os configuration//
#define TLK_CFG_RTOS_ENABLE    0

#define TLK_CFG_SUSPEND_ENABLE (0) //1: enable suspend

#define TLK_STK_BLE_ENABLE     1

#if MCU_CORE_TL752X_TEMP
#ifndef TLKMDI_AUDMEM_TOTAL_SIZE
#define TLKMDI_AUDMEM_TOTAL_SIZE (64 * 1024)
#endif
#endif

// APP System Configuration//
#define TLK_DEV_KEY_ENABLE    1
#define TLK_MW_TINYSQL_ENABLE 1

//CODEC and ALG Configuration//
#define TLK_DEV_CODEC_ENABLE   1
#define CODEC_MIC_FIFO_SAMPLES 2048

#define APP_LOG_EN             1

#if (TLK_CFG_SUSPEND_ENABLE)
#define TLK_DEBUG_ENABLE         1
#define TLK_CFG_USB_ENABLE       0
#define TLK_USB_UDB_ENABLE       0
#define TLK_DEV_LED_ENABLE       0
#define TLK_CFG_UART_TOOL_ENABLE 1 //debug use uart, disable usb
#else
#define TLK_DEBUG_ENABLE 1
#if (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL652X)
#define TLK_USB_UDB_ENABLE 0
#define TLK_CFG_USB_ENABLE 0
#else
#define TLK_USB_UDB_ENABLE 1
#define TLK_CFG_USB_ENABLE 1
#endif
#define TLK_DEV_LED_ENABLE 1
#if MCU_CORE_TYPE != MCU_CORE_B91
#define TLK_CFG_UART_TOOL_ENABLE 1
#endif
#endif


/////////////////////// Board Select Configuration //////////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_B92)
#define TLKHW_TYPE                 TLKHW_TLSR9528A_EVK_C1T266A20
#define TLKDRV_CODEC_ICODEC_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKHW_TYPE                 TLKHW_TL751X_EVK_C1T368A20_V1_0
#define TLKDRV_CODEC_ICODEC_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)

//#define TLKHW_TYPE BOARD_721X_EVK_C1T315A20
//#define TLKDRV_CODEC_ICODEC_ENABLE (1 && TLK_DEV_CODEC_ENABLE)

#define TLKHW_TYPE                     BOARD_721X_EVK_C1TXA104_V1_1
#define TLKDRV_CODEC_I2S_MASTER_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#elif (MCU_CORE_TYPE == MCU_CORE_TL322X)
#define TLKHW_TYPE                 TLKHW_TL3228_EVK_C1T371A20_V1_1
#define TLKDRV_CODEC_ICODEC_ENABLE (1 && TLK_DEV_CODEC_ENABLE)
#else
#error "config hardware !!!"
#endif

/////////////////////// *********  Below : difference between  single-core chip and  dual-core chip //////////////////////////////
#if (MCU_DUAL_CORE_ENABLE)

#define TLK_MW_DSP_COMM_ENABLE 0
#endif

#if __has_include(CFG_PATH(APP_DEMO_SELECT))
#include CFG_PATH(APP_DEMO_SELECT)
#endif

#include "vendor/common/default_config.h"
