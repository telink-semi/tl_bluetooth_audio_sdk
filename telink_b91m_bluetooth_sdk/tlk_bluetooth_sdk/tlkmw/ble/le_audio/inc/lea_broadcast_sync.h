/********************************************************************************************************
 * @file    lea_broadcast_sync.h
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

#if (SINK_VERSION == SINK_ONLY_VERSION)

#define BIS_SOURCE_BROADCAST_NAME "btble-Broadcast"
#define BIS_SOURCE_MS_STR         "A2DP Source"

typedef void (*ble_bis_source_operate_callback)(void);

typedef void (*lea_broadcast_sync_operate_callback)(void);

/**
 * @brief       Initialize resources for LE Audio broadcast synchronization.
 * @return      none.
 */
void lea_broadcast_sync_init(void);

/**
 * @brief       Start scanning, PA sync, and BIG sync procedures.
 * @return      none.
 */
void lea_broadcast_sync_start(void);

/**
 * @brief       Stop ongoing scans/syncs and release broadcast sink resources.
 * @return      none.
 */
void lea_broadcast_sync_stop(void);

#endif
