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
#ifndef VENDOR_CONTROLLER_BTTPSLL_TEST_APP_CONFIG_H_
#define VENDOR_CONTROLLER_BTTPSLL_TEST_APP_CONFIG_H_

#include "../controller_config.h"
#include "core/mcu_type.h"
#if (CONTROLLER_MODE == BTBLE_TPSLL_HEADSET)
// #define TLKHW_TYPE        TLKHW_TL751X_EVK_C1T360A20
#if (CHIP_TYPE == CHIP_TYPE_TL751X)
#define TLKHW_TYPE TLKHW_TL751X_EVK_C1T368A20_V1_0
#elif (CHIP_TYPE == CHIP_TYPE_TL752X)
#define TLKHW_TYPE TLKHW_TL752X_96PIN
#endif
// #define     TLKHW_MINOR_VERSION       0x0103    // adapt for TLKHW_TL751X_EVK_C1T360A20 V1.3
#define VCD_DEFINE_SELECT              VCD_DEFINE_MINGQIAN
#define DEBUG_ZHAOWEI_GPIO_ENABLE_LYNX 0
/**********************LED DEFINE**************************/
#define PB5_OUTPUT_ENABLE 1
#define PB6_OUTPUT_ENABLE 1
#define PB7_OUTPUT_ENABLE 1
#define PC0_OUTPUT_ENABLE 1
#ifndef PB5_FUNC
#define PB5_FUNC AS_GPIO
#endif
#ifndef PB6_FUNC
#define PB6_FUNC AS_GPIO
#endif
#ifndef PB7_FUNC
#define PB7_FUNC AS_GPIO
#endif
#ifndef PC0_FUNC
#define PC0_FUNC AS_GPIO
#endif

#if (TLKHW_MINOR_VERSION == 0x0103)
#define LED_GREEN GPIO_PD2
#else
#define LED_GREEN GPIO_PA4
#endif
#define LED_BLUE  GPIO_PB6
#define LED_RED   GPIO_PB7
#define LED_WHITE GPIO_PC0

#if (CHIP_TYPE != CHIP_TYPE_TL752X)
#define TLK_CFG_SUSPEND_ENABLE 1 //1: enable suspend + wfi module
#endif
#define TLK_LOW_POWER_ENABLE (0)
#define DEBUG_SUSPEND_ENABLE (0)

#if (DEBUG_SUSPEND_ENABLE)
#define TLK_CFG_UART2USBVCD_ENABLE (1)
#define TLK_SM_LOG_ENABLE          (0)
#else
#define TLK_SM_LOG_ENABLE (1)
#endif


#define TPSLL_LATENCY_LED_DEBUG_EN (0)
#define APP_LOG_EN                 (0)

#define TLK_CFG_TPSLL_HCI_ENABLE   1
#define TLK_CFG_N22_VCD_ENABLE     1
#define TLK_STK_TPH_ENABLE         1

/**********************BLE FEATURE DEFINE**************************/
#define ACL_CENTRAL_MAX_NUM           4 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM           4 // ACL peripheral maximum number
#define ACL_CENTRAL_SIMPLE_SDP_ENABLE 0 //simple service discovery for ACL central

#define BLE_FEATURE_ALL_EN            1

#include "vendor/common/default_config.h"

#endif

#endif /* VENDOR_CONTROLLER_BTTPSLL_TEST_APP_CONFIG_H_ */
