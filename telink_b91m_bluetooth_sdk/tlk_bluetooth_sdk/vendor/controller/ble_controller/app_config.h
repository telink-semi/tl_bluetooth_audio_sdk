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

#include "core/mcu_type.h"
#include "../controller_config.h"
#if (CONTROLLER_MODE == BLE_CONTROLLER)

#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLKHW_TYPE        TLKHW_TL751X_EVK_C1T368A20_V1_0
#else
#error "config hardware !!!"
#endif

    /**********************LED DEFINE**************************/
    #define PB5_OUTPUT_ENABLE             1
    #define PB6_OUTPUT_ENABLE             1
    #define PB7_OUTPUT_ENABLE             1
    #define PC0_OUTPUT_ENABLE             1
    #define PB5_FUNC                      AS_GPIO
    #define PB6_FUNC                      AS_GPIO
    #define PB7_FUNC                      AS_GPIO
    #define PC0_FUNC                      AS_GPIO

    #define LED_GREEN                     GPIO_PB5
    #define LED_BLUE                      GPIO_PB6
    #define LED_RED                       GPIO_PB7
    #define LED_WHITE                     GPIO_PC0

    //Power Management Configuration//
    #define TLK_CFG_SUSPEND_ENABLE  0 //1: enable suspend + wfi module


    #define APP_LOG_EN                    1
    #define APP_HOST_EVT_LOG_EN           0
    #define APP_PAIR_LOG_EN               0
    #define APP_KEY_LOG_EN                0
    #define APP_CIS_LOG_EN                0
    #define TLK_SM_LOG_ENABLE     1

	#define BLE_FEATURE_ALL_EN 1
	#define ACL_CONN_MAX_RX_OCTETS 40
	#define ACL_CENTRAL_MAX_TX_OCTETS 40
//LE Feature Configuration

    #include "vendor/common/default_config.h"

#endif
