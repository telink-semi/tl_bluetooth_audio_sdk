/********************************************************************************************************
 * @file    app_ble_bis_sink.h
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

/**
 *  @brief  Callback function type for BIS sink operations
 */
typedef void (*app_ble_bis_sink_operate_callback)(void);

/**
 * @brief       Initialize the BLE BIS sink
 * @param       none
 * @return      none
 */
void app_ble_bis_sink_init(void);

/**
 * @brief       Start the BLE BIS sink
 * @param       none
 * @return      none
 */
void app_ble_bis_sink_start(void);

/**
 * @brief       Stop the BLE BIS sink
 * @param[in]   callback - Callback function to be called upon completion
 * @return      none
 */
void app_ble_bis_sink_stop(app_ble_bis_sink_operate_callback callback);

/**
 * @brief       Stop stream and open advertisement for BLE BIS sink
 * @param       none
 * @return      none
 */
void app_ble_bis_sink_stop_stream_open_adv(void);