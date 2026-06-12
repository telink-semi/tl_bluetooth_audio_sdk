/********************************************************************************************************
 * @file    controller_config.h
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
#ifndef VENDOR_CONTROLLER_CONTROLLER_H_
#define VENDOR_CONTROLLER_CONTROLLER_H_

#define MAILBOX                 1

#define BT                      10

#define BTBLE_TPSLL_HEADSET     21
#define BTBLETPSLL              22
#define BTTPSLL_TWS             23

#define BLE_CONTROLLER          0x8000
#define BLE_MODE_RECORDING_CARD 0x0001

#define BTBLE_HEADSET           40

#define BTBLE_A2DP_TO_BIS       41

#define BTBLE_AUDIO_SOURCE      42

#define BLETPSLL_TWS            50

#define RUIMO_BTBLE_DONGLE      55

#define BT_INTERPHONE           BTBLE_HEADSET

#define RECORDING_CARD          (BLE_CONTROLLER | BLE_MODE_RECORDING_CARD)

#define HEARING_AID             BTTPSLL_TWS
#define GAMESIR_XIAOJI          BTBLE_TPSLL_HEADSET

#define CONTROLLER_MODE         BTBLE_A2DP_TO_BIS


#define CHECK_BLE_CONTROLLER    (CONTROLLER_MODE & BLE_CONTROLLER)
#define GET_BLE_CONTROLLER_MODE (CONTROLLER_MODE & (BLE_CONTROLLER - 1))

#endif /* VENDOR_CONTROLLER_CONTROLLER_H_ */
