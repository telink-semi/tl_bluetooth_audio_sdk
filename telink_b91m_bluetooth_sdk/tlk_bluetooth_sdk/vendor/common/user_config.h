/********************************************************************************************************
 * @file    user_config.h
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

/******************do not modify this file start****************************/
#ifdef PROJ_BLUETOOTH_CONTROLLER
    #include "vendor/bluetooth_controller/app_config.h"
#endif
#ifdef PROJ_BTBLE_HEADSET
    #include "vendor/btble_headset/app_config.h"
#endif
#ifdef PROJ_BTBLE_AUDIO_SOURCE
    #include "vendor/btble_audio_source/app_config.h"
#endif
#ifdef PROJ_BTTPSLL_HEADSET
    #include "vendor/bttpsll_headset/app_config.h"
#endif
#ifdef PROJ_BTTPSLL_TWS
    #include "vendor/bttpsll_tws/app_config.h"
#endif
#ifdef PROJ_BTBLE_A2DP_TO_BIS
    #include <vendor/btble_a2dp_to_bis/app_config.h>
#endif
#ifdef PROJ_CONTROLLER
    #include "vendor/controller/app_config.h"
#endif
#ifdef PROJ_TPSLL_AUDIO_DONGLE
    #include "vendor/tpsll_audio_dongle/app_config.h"
#endif
#ifdef PROJ_BLE_EXAMPLE
     #include "vendor/ble_example/app_config.h"
#endif

/******************do not modify this file end****************************/

