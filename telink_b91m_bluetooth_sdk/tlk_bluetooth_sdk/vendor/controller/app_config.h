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
#ifndef VENDOR_CONTROLLER_APP_CONFIG_H_
#define VENDOR_CONTROLLER_APP_CONFIG_H_

#include "controller_config.h"

#if (CONTROLLER_MODE == MAILBOX)
    #include "mailbox/app_config.h"
#elif (CONTROLLER_MODE == BTBLE_TPSLL_HEADSET)
    #include  "btble_tpsll_headset/app_config.h"
#elif (CONTROLLER_MODE == BTTPSLL_TWS)
    #include "bttpsll_tws/app_config.h"
#elif (CONTROLLER_MODE == BTBLETPSLL)
    #include "btbletpsll/app_config.h"
#elif (CONTROLLER_MODE == BLE_TEST)
    #include "ble_test/app_config.h"
#elif (CONTROLLER_MODE == BLE_CIS_CENTRAL)
    #include "ble_cis_central/app_config.h"
#elif (CONTROLLER_MODE == BLE_CIS_PERIPHERAL)
    #include "ble_cis_peripheral/app_config.h"
#elif (CONTROLLER_MODE == BLE_CONTROLLER)
    #include "ble_controller/app_config.h"
#elif (CONTROLLER_MODE == BT)
    #include "bt/app_config.h"
#elif (CONTROLLER_MODE == BTBLE_HEADSET)
    #include "btble_headset/app_config.h"
#elif (CONTROLLER_MODE == BTBLE_A2DP_TO_BIS)
    #include "btble_a2dp_to_bis/app_config.h"
#elif (CONTROLLER_MODE == BTBLE_AUDIO_SOURCE)
    #include "btble_audio_source/app_config.h"
#elif (CONTROLLER_MODE == BLETPSLL_TWS)
    #include "bletpsll_tws/app_config.h"
#else
    #error "must select test mode."
#endif

#endif /* VENDOR_CONTROLLER_APP_CONFIG_H_ */
