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
#include "../controller_config.h"
#if (CONTROLLER_MODE == BTBLE_A2DP_TO_BIS)
    // #define TLKHW_TYPE          TLKHW_TL751X_EVK_C1T360A20
    #define TLKHW_TYPE          TLKHW_TL751X_EVK_C1T368A20_V1_0

    #define ACL_CENTRAL_MAX_NUM 0 // ACL central maximum number

    #define ACL_PERIPHR_MAX_NUM 1 // ACL central maximum number

    /**********************LED DEFINE**************************/
    #define PB5_OUTPUT_ENABLE 1
    #define PB6_OUTPUT_ENABLE 1
    #define PB7_OUTPUT_ENABLE 1
    #define PC0_OUTPUT_ENABLE 1
    #define PB5_FUNC          AS_GPIO
    #define PB6_FUNC          AS_GPIO
    #define PB7_FUNC          AS_GPIO
    #define PC0_FUNC          AS_GPIO

    #define LED_GREEN         GPIO_PA4
    #define LED_BLUE          GPIO_PB6
    #define LED_RED           GPIO_PB7
    #define LED_WHITE         GPIO_PC0

	//BT Stack Configuration//
	#define MAX_NB_SYNC 1
	//BT Stack Configuration end//

	//BLE Stack Configuration//
	#define BLE_A2DP_TO_BIS_CONTROLLER_EN  1
	//BLE Stack Configuration end//

#if (CHIP_TYPE == CHIP_TYPE_TL752X)
    #define TLK_CFG_SUSPEND_ENABLE  0 //1: enable suspend + wfi module
#else
    //Power Management Configuration//
    #define TLK_CFG_SUSPEND_ENABLE  1 //1: enable suspend + wfi module
#endif
    #define TLK_SM_LOG_ENABLE     1

    #include "vendor/common/default_config.h"

#endif
