/********************************************************************************************************
 * @file    bluetooth_controller.h
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
#include "../../stack/bt/common/ebq_config.h"
// mode define:
#define BT_CONTROLLER_ONLY_MODE   0

#define BLE_CONTROLLER_ONLY_MODE  1

#define BT_BLE_CONTROLLER_MODE    2

#define BLUETOOTH_CONTROLLER_MODE BT_BLE_CONTROLLER_MODE


#if EBQ_BT_CTRLER_ONLY_TEST_MODE
    #ifdef BLUETOOTH_CONTROLLER_MODE
        #undef BLUETOOTH_CONTROLLER_MODE
    #endif
    #define BLUETOOTH_CONTROLLER_MODE BT_CONTROLLER_ONLY_MODE
#endif

#define SDK_RELEASE 1
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
#define HCI_TR_EN   0
#else
#define HCI_TR_EN   1
#endif
