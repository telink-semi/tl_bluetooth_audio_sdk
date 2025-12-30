/********************************************************************************************************
 * @file    app_ble_bis.h
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
#ifndef VENDOR_APP_BLE_BIS_H_
#define VENDOR_APP_BLE_BIS_H_

#define BIS_SOURCE_BROADCAST_NAME "btble-Broadcast"
#define BIS_SOURCE_MS_STR         "A2DP Source"

/** @brief SMP pending flag */
extern int central_smp_pending;
/** @brief Pairing enable flag */
extern int central_pairing_enable;
/** @brief Unpair enable flag */
extern u16 central_unpair_enable;

/** @brief Disconnect connection handle */
extern u16 central_disconnect_connhandle;

/**
 *  @brief  Structure for BASP advertisement data
 */
typedef struct
{
    u8  announcement_type;
    u32 available_audio_context;
    u8  metadata_length;
    u8  metadata[0];
} app_basp_adv;

/**
 *  @brief  Callback function type for BIS source operations
 */
typedef void (*app_ble_bis_source_operate_callback)(void);

/**
 * @brief       Initialize the BLE BIS source
 * @param       none
 * @return      none
 */
void app_ble_bis_source_init(void);

/**
 * @brief       Deinitialize the BLE BIS source
 * @param       none
 * @return      none
 */
void app_ble_bis_source_deinit(void);

/**
 * @brief       Open the BLE BIS source
 * @param[in]   callback - Callback function to be called upon completion
 * @return      true if operation started successfully, false otherwise
 */
bool app_ble_bis_source_open(app_ble_bis_source_operate_callback callback);

/**
 * @brief       Close the BLE BIS source
 * @param[in]   callback - Callback function to be called upon completion
 * @return      true if operation started successfully, false otherwise
 */
bool app_ble_bis_source_close(app_ble_bis_source_operate_callback callback);

/**
 * @brief       Check if the BLE BIS source is open
 * @param       none
 * @return      true if BIS source is open, false otherwise
 */
bool app_ble_bis_source_is_open(void);

#endif