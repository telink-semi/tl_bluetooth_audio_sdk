/********************************************************************************************************
 * @file    tlkos_timer.h
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
 * @brief     Creates a timer.
 * @param[in] pName Name of the timer.
 * @param[in] periodMs Timer period in milliseconds.
 * @param[in] autoReload Whether to auto-reload, 1 means yes, 0 means no.
 * @param[in] CBEnter Timeout callback function of the timer.
 * @param[in] pUsrArg User parameters passed to the callback function.
 * @param[out] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_create(char *pName, uint32_t periodMs, uint32_t autoReload, TlkOsTimerEnterCB CBEnter, void *pUsrArg, TlkOsTimerHandle_t *timerHandle);

/**
 * @brief     Destroys a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_destroy(TlkOsTimerHandle_t timerHandle);

/**
 * @brief     Starts a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_start(TlkOsTimerHandle_t timerHandle);

/**
 * @brief     Resets a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_reset(TlkOsTimerHandle_t timerHandle);

/**
 * @brief     Stops a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_stop(TlkOsTimerHandle_t timerHandle);

/**
 * @brief     Sets the timer period in microseconds.
 * @param[in] timerHandle Timer handle.
 * @param[in] periodUs Timer period in microseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      Only support on baremetal.
 */
int tlkos_timer_setPeriodUs(TlkOsTimerHandle_t timerHandle, uint32_t periodUs);

/**
 * @brief     Sets the timer period in milliseconds.
 * @param[in] timerHandle Timer handle.
 * @param[in] periodMs Timer period in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_setPeriod(TlkOsTimerHandle_t timerHandle, uint32_t periodMs);

/**
 * @brief     get the earliest timer task in milliseconds.
 * @returns   times, 0 means fail.
 */
uint32_t tlkos_timer_getNextTaskTimeMs(void);

/**
 * @brief     Indicates whether the timer has ISR-related API.
 * @returns   true indicates the timer has ISR-related API, false otherwise.
 * @note      Baremetal return false,and rtos return true.
 */
bool tlkos_timer_isHaveIsrApi(void);

/**
 * @brief     Starts the timer from an interrupt service routine.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_startFromISR(TlkOsTimerHandle_t timerHandle);

/**
 * @brief     This function is used as an irq function in baremetal.
 * @param[in] None.
 * @returns   None.
 */
void tlkos_timer_irq_handler(void);