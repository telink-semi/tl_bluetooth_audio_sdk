/********************************************************************************************************
 * @file    tlk_ota_timer_porting_example.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tlkapi/tlkapi.h"
#include "tlk_ota_timer_port.h"

/**
 * @brief      Create a timer for OTA operations
 * @param[out] timer       - Pointer to timer handle to be created
 * @param[in]  timeout_ms  - Timeout value in milliseconds
 * @param[in]  callback    - Callback function to be called when timer expires
 * @param[in]  user_data   - User data to be passed to callback function
 * @return     bool - true if timer creation is successful, false otherwise
 */
bool tlk_ota_timer_porting_create(tlk_ota_timer_handle_t *timer, uint32_t timeout_ms, timer_callback_t callback, void *user_data)
{
    int ret = tlksys_timer_create(TLKSYS_TASKID_SYSTEM, timer, timeout_ms * 1000, true, callback, user_data);
    return ret == TLK_ENONE;
}

/**
 * @brief      Start a timer for OTA operations
 * @param[in]  timer - Timer handle to start
 * @return     bool - true if timer start is successful, false otherwise
 */
bool tlk_ota_timer_porting_start(tlk_ota_timer_handle_t timer)
{
    int ret = tlksys_timer_start(TLKSYS_TASKID_SYSTEM, timer);
    return ret == TLK_ENONE;
}

/**
 * @brief      Stop a timer for OTA operations
 * @param[in]  timer - Timer handle to stop
 * @return     bool - true if timer stop is successful, false otherwise
 */
bool tlk_ota_timer_porting_stop(tlk_ota_timer_handle_t timer)
{
    int ret = tlksys_timer_stop(TLKSYS_TASKID_SYSTEM, timer);
    return ret == TLK_ENONE;
}

/**
 * @brief      Reset timeout value for a timer
 * @param[in]  timer         - Timer handle to reset
 * @param[in]  new_timeout_ms - New timeout value in milliseconds
 * @return     bool - true if timer reset is successful, false otherwise
 */
bool tlk_ota_timer_porting_reset_timeout(tlk_ota_timer_handle_t timer, uint32_t new_timeout_ms)
{
    int ret = tlksys_timer_setPeriod(TLKSYS_TASKID_SYSTEM, timer, new_timeout_ms * 1000);
    return ret == TLK_ENONE;
}

/**
 * @brief      Destroy a timer for OTA operations
 * @param[in]  timer - Timer handle to destroy
 * @return     bool - true if timer destruction is successful, false otherwise
 */
bool tlk_ota_timer_porting_destroy(tlk_ota_timer_handle_t timer)
{
    int ret = tlksys_timer_destroy(TLKSYS_TASKID_SYSTEM, timer);
    return ret == TLK_ENONE;
}

/**
 * @brief      Check if a timer is running
 * @param[in]  timer - Timer handle to check
 * @return     bool - true if timer is running, false otherwise
 * @note       Currently not implemented, always returns false
 */
bool tlk_ota_timer_porting_is_running(tlk_ota_timer_handle_t timer)
{
    (void)timer;
    return false;
}