/********************************************************************************************************
 * @file    tlkble_hal_mode.h
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

#define LE_HOST_SEND_HCI_MODE_NONE              0  /* Host does not send HCI */
#define LE_HOST_SEND_HCI_MODE_MESSAGE           1  /* Host sends HCI via message IPC */
#define LE_HOST_SEND_HCI_MODE_VHCI              2  /* Host sends HCI via VHCI */

#define BLE_CONTROLLER_INITIAL_EN  (!MCU_DUAL_CORE_ENABLE || MCU_CORE_N22)


#ifndef LE_HOST_SEND_HCI_MODE
    #if MCU_CORE_N22
        #if MCU_CORE_TL752X_TEMP && defined(PROJ_BLE_INTERNAL)
            #define LE_HOST_SEND_HCI_MODE   LE_HOST_SEND_HCI_MODE_VHCI
        #else
            #define LE_HOST_SEND_HCI_MODE   LE_HOST_SEND_HCI_MODE_NONE
        #endif
    #else
        #if (MCU_DUAL_CORE_ENABLE)
            #define LE_HOST_SEND_HCI_MODE LE_HOST_SEND_HCI_MODE_MESSAGE
        #else
            #define LE_HOST_SEND_HCI_MODE LE_HOST_SEND_HCI_MODE_VHCI
        #endif
    #endif
#endif
