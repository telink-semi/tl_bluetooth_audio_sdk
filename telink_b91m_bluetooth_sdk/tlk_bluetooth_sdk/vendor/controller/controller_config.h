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

#define MAILBOX                   1

#define BT	                      10

#define BTBLE_TPSLL_HEADSET       21
#define BTBLETPSLL		          22
#define BTTPSLL_TWS               23

#define BLE_CONTROLLER            33

#define BTBLE_HEADSET             40

#define BTBLE_A2DP_TO_BIS         41

#define BTBLE_AUDIO_SOURCE     	      42

#define BLETPSLL_TWS              50

#define BT_INTERPHONE             BTBLE_HEADSET

#define RECARDING_CARD            BLE_CONTROLLER

#define HEARING_AID               BTTPSLL_TWS

#define CONTROLLER_MODE           BTBLE_HEADSET

#endif                                /* VENDOR_CONTROLLER_CONTROLLER_H_ */
