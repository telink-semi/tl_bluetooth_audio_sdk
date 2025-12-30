/********************************************************************************************************
 * @file    tlksys_timer.h
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

#include "tlkapi/tlkapi_timerList.h"

/**
 * @brief  Initialize timer core module
 * @param  None.
 * @returns  None.
 */
void tlksys_timer_coreInit(void);

/**
 * @brief  Create a static timer for specified task
 * @param[in] taskID : Task ID to associate with the timer
 * @param[in] pTimer : Pointer to timer structure
 * @param[in] periodUs : Timer period in microseconds
 * @param[in] autoReload : Auto-reload flag (non-zero for auto-reload)
 * @param[in] timerCB : Timer callback function
 * @param[in] userArg : User argument for callback
 * @returns  Result of timer creation
 */
int tlksys_timer_createStatic(uint16_t taskID, TlkApiTimer_t *pTimer, uint32_t periodUs, uint32_t autoReload, TlkApiTimerCB_t timerCB, void* userArg);

/**
 * @brief  Create a dynamic timer for specified task
 * @param[in] taskID : Task ID to associate with the timer
 * @param[in] pTimer : Pointer to timer handle
 * @param[in] periodUs : Timer period in microseconds
 * @param[in] autoReload : Auto-reload flag (non-zero for auto-reload)
 * @param[in] timerCB : Timer callback function
 * @param[in] userArg : User argument for callback
 * @returns  Result of timer creation
 */
int tlksys_timer_create(uint16_t taskID, TlkApiTimerHandle_t *pTimer, uint32_t periodUs, uint32_t autoReload, TlkApiTimerCB_t timerCB, void* userArg);

/**
 * @brief  Destroy specified timer
 * @param[in] taskID : Task ID associated with the timer
 * @param[in] pTimer : Timer handle to destroy
 * @returns  Result of timer destruction
 */
int tlksys_timer_destroy(uint16_t taskID, TlkApiTimerHandle_t pTimer);

/**
 * @brief  Start specified timer
 * @param[in] taskID : Task ID associated with the timer
 * @param[in] pTimer : Timer handle to start
 * @returns  Result of timer start operation
 */
int tlksys_timer_start(uint16_t taskID, TlkApiTimerHandle_t pTimer);

/**
 * @brief  Restart specified timer
 * @param[in] taskID : Task ID associated with the timer
 * @param[in] pTimer : Timer handle to restart
 * @returns  Result of timer restart operation
 */
int tlksys_timer_reStart(uint16_t taskID, TlkApiTimerHandle_t pTimer);

/**
 * @brief  Stop specified timer
 * @param[in] taskID : Task ID associated with the timer
 * @param[in] pTimer : Timer handle to stop
 * @returns  Result of timer stop operation
 */
int tlksys_timer_stop(uint16_t taskID, TlkApiTimerHandle_t pTimer);

/**
 * @brief  Set period for specified timer
 * @param[in] taskID : Task ID associated with the timer
 * @param[in] pTimer : Timer handle to modify
 * @param[in] periodUs : New timer period in microseconds
 * @returns  Result of timer period setting
 */
int tlksys_timer_setPeriod(uint16_t taskID, TlkApiTimerHandle_t pTimer, uint32_t periodUs);
