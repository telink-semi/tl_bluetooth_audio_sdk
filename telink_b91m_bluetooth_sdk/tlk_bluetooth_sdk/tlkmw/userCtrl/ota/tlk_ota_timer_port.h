/********************************************************************************************************
 * @file    tlk_ota_timer_port.h
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
#ifndef TLK_OTA_TIMER_PORT_H
#define TLK_OTA_TIMER_PORT_H

#include <stdint.h>
#include <stdbool.h>


typedef void *tlk_ota_timer_handle_t;

typedef void (*timer_callback_t)(tlk_ota_timer_handle_t timer, void *user_data);

/**
 * @brief      Create a timer for OTA operations
 * @param[out] timer       - Pointer to timer handle to be created
 * @param[in]  timeout_ms  - Timeout value in milliseconds
 * @param[in]  callback    - Callback function to be called when timer expires
 * @param[in]  user_data   - User data to be passed to callback function
 * @return     bool - true if timer creation is successful, false otherwise
 */
bool tlk_ota_timer_porting_create(tlk_ota_timer_handle_t *timer, uint32_t timeout_ms, timer_callback_t callback, void *user_data);

/**
 * @brief      Start a timer for OTA operations
 * @param[in]  timer - Timer handle to start
 * @return     bool - true if timer start is successful, false otherwise
 */
bool tlk_ota_timer_porting_start(tlk_ota_timer_handle_t timer);

/**
 * @brief      Stop a timer for OTA operations
 * @param[in]  timer - Timer handle to stop
 * @return     bool - true if timer stop is successful, false otherwise
 */
bool tlk_ota_timer_porting_stop(tlk_ota_timer_handle_t timer);

/**
 * @brief      Reset timeout value for a timer
 * @param[in]  timer         - Timer handle to reset
 * @param[in]  new_timeout_ms - New timeout value in milliseconds
 * @return     bool - true if timer reset is successful, false otherwise
 */
bool tlk_ota_timer_porting_reset_timeout(tlk_ota_timer_handle_t timer, uint32_t new_timeout_ms);

/**
 * @brief      Destroy a timer for OTA operations
 * @param[in]  timer - Timer handle to destroy
 * @return     bool - true if timer destruction is successful, false otherwise
 */
bool tlk_ota_timer_porting_destroy(tlk_ota_timer_handle_t timer);

/**
 * @brief      Check if a timer is running
 * @param[in]  timer - Timer handle to check
 * @return     bool - true if timer is running, false otherwise
 */
bool tlk_ota_timer_porting_is_running(tlk_ota_timer_handle_t timer);

#endif // TLK_OTA_TIMER_PORT_H