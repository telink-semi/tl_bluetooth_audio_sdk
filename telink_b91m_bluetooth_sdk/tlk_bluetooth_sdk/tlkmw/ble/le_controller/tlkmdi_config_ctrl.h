/********************************************************************************************************
 * @file    tlkmdi_config_ctrl.h
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
#include "tl_common.h"

#if (BLE_CONTROLLER_INITIAL_EN)

#ifndef APP_LOG_EN
#define APP_LOG_EN 0
#endif

#if BLE_INTERNAL_EN//BLE_INTERNAL
#define TLKMDI_ACL_PERIPHR_EN              1
#define TLKMDI_LE_LEGACY_ADV_EN            1
#if MCU_CORE_TL752X_TEMP //?????
#define TLKMDI_LE_EXTENDED_ADV_EN 		   1
#define TLKMDI_LE_CIS_PERIPHR_EN           0
#define TLKMDI_LE_PERIODIC_ADV_EN          1
#define TLKMDI_ISOCHRONOUS_BROADCASTER_EN  1

#define TLKMDI_LE_EXTENDED_SCAN_EN         1
#define TLKMDI_LE_EXTENDED_INIT_EN         1
#define TLKMDI_ISOCHRONOUS_BROADCASTER_SYNC_EN 1

#define TLKMDI_ACL_CENTRAL_EN              1
#define TLKMDI_LE_LEGACY_SCAN_EN           1
#define TLKMDI_LE_LEGACY_INIT_EN           1
#define TLKMDI_PAST_EN                     0
#endif
#endif

#if BLE_A2DP_TO_BIS_CONTROLLER_EN//A2DP_TO_BIS
#define TLKMDI_ACL_PERIPHR_EN              1
#define TLKMDI_LE_LEGACY_ADV_EN            1 //TODO: test pm for ble example, remove latter
#define TLKMDI_LE_EXTENDED_ADV_EN          1
#define TLKMDI_LE_PERIODIC_ADV_EN          1
#define TLKMDI_LE_EXTENDED_SCAN_EN         1

#define TLKMDI_ISOCHRONOUS_BROADCASTER_EN  1
#define TLKMDI_ISOCHRONOUS_BROADCASTER_SYNC_EN 1
#define TLKMDI_PAST_EN                     1
#endif

#if BLE_HEADSET_CONTROLLER_EN//BTBLE_Headset
#define TLKMDI_ACL_PERIPHR_EN              1
#define TLKMDI_LE_LEGACY_ADV_EN            1
#define TLKMDI_LE_EXTENDED_ADV_EN          1
#define TLKMDI_LE_CIS_PERIPHR_EN           1
#endif

#if BLE_SPORTS_WALKIE_TALKIE_EN // sports_walkie_talkie
#define TLKMDI_ACL_PERIPHR_EN              1
#define TLKMDI_ACL_CENTRAL_EN              1
#define TLKMDI_LE_LEGACY_ADV_EN            1
#define TLKMDI_LE_LEGACY_SCAN_EN           1
#define TLKMDI_LE_LEGACY_INIT_EN           1
#endif

#if PROJ_BLE_EXAMPLE
// default ble example configuration.
#define TLKMDI_ACL_PERIPHR_EN              1
#define TLKMDI_ACL_CENTRAL_EN              1
#define TLKMDI_LE_LEGACY_ADV_EN            1
#define TLKMDI_LE_EXTENDED_ADV_EN          1
#define TLKMDI_LE_PERIODIC_ADV_EN          1
#define TLKMDI_LE_CIS_PERIPHR_EN           1
#define TLKMDI_LE_CIS_CENTRAL_EN           1

#define TLKMDI_LE_LEGACY_SCAN_EN           1
#define TLKMDI_LE_LEGACY_INIT_EN           1
#define TLKMDI_LE_EXTENDED_SCAN_EN         1
#define TLKMDI_LE_EXTENDED_INIT_EN         1

#define TLKMDI_ISOCHRONOUS_BROADCASTER_EN  1
#define TLKMDI_ISOCHRONOUS_BROADCASTER_SYNC_EN 1
#define TLKMDI_PAST_EN                     1
#endif

#if BLE_DONGLE_CONTROLLER_EN//BTBLE_Dongle
#define TLKMDI_ACL_CENTRAL_EN              1
#define TLKMDI_LE_LEGACY_INIT_EN           1
#define TLKMDI_LE_EXTENDED_SCAN_EN         1
#define TLKMDI_LE_EXTENDED_INIT_EN         1
#define TLKMDI_LE_CIS_CENTRAL_EN           1

#define TLKMW_ACL_CENTRAL_MAX_NUM 2
#define ACL_CONN_MAX_RX_OCTETS 40
#define ACL_CENTRAL_MAX_TX_OCTETS 40
#endif

#if BLE_GAME_PAD_CONTROLLER_EN // Game_Pad
#define TLKMDI_ACL_PERIPHR_EN              1
#define TLKMDI_LE_LEGACY_ADV_EN            1
#define TLKMDI_LE_CIS_PERIPHR_EN           1
#endif

#if HM_PARTY_LIGHTING_EN//HM Party lighting
#define TLKMDI_ACL_PERIPHR_EN              1
#define TLKMDI_LE_EXTENDED_ADV_EN          1
#define TLKMDI_LE_PERIODIC_ADV_EN          1

#define EXT_ADV_SETS_NUMBER 			   2
#endif

#if BLE_FEATURE_ALL_EN//ALL Feature

#define TLKMDI_LE_LEGACY_ADV_EN                 1
#define TLKMDI_ACL_PERIPHR_EN                   1
#if MCU_CORE_TYPE != MCU_CORE_TL322X

#define TLKMDI_ACL_CENTRAL_EN              		1
#define TLKMDI_LE_EXTENDED_ADV_EN          		1
#define TLKMDI_LE_CIS_PERIPHR_EN           		1
#define TLKMDI_LE_EXTENDED_SCAN_EN        		1
#define TLKMDI_LE_EXTENDED_INIT_EN         		1
#define TLKMDI_LE_LEGACY_SCAN_EN                1
#define TLKMDI_LE_LEGACY_INIT_EN                1
#define TLKMDI_LE_CIS_CENTRAL_EN           		1
#define TLKMDI_ISOCHRONOUS_BROADCASTER_EN  		1
#define TLKMDI_ISOCHRONOUS_BROADCASTER_SYNC_EN 	1
#define TLKMDI_PAST_EN                     		1
#define TLKMDI_LE_PERIODIC_ADV_EN          		1
#define TLKMDI_POWER_CONTROL			   		0
#define TLKMDI_LE_EXTENDED_SCAN_RSP_EN			1
#endif
#endif

#if RECORDING_CARD_EN

#define ACL_CONN_MAX_RX_OCTETS              64
#define ACL_PERIPHR_MAX_TX_OCTETS           64

#define TLKMW_ACL_CENTRAL_MAX_NUM			0

#define TLKMDI_ACL_PERIPHR_EN               1
#define TLKMDI_LE_LEGACY_ADV_EN             1
#define TLKMW_ACL_PERIPHR_MAX_NUM           1
#endif

//BLE Stack Configuration end//
#endif
