/********************************************************************************************************
 * @file    tlkapi_timerList.h
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

typedef void *TlkApiTimerHandle_t;

typedef void (*TlkApiTimerCB_t)(TlkApiTimerHandle_t handle, void *userArg);

struct TlkApiTimer_s
{
    uint8_t               malloced;
    uint8_t               runningCB;
    uint8_t               autoReload;
    uint8_t               nowState;
    uint32_t              arrival;
    uint32_t              timeout;
    void                 *userArg;
    TlkApiTimerCB_t       timerCB;
    struct TlkApiTimer_s *pNext;
};

typedef struct TlkApiTimer_s TlkApiTimer_t;

typedef struct
{
    TlkApiTimer_t *pList;
} TlkApiTimerList_t;

/**
 * @brief           Statically create a timer (using provided buffer)
 * @param[in]       buffer - Pointer to TlkApiTimer_t structure to hold timer information
 * @param[in]       periodUs - Timer period (microseconds)
 * @param[in]       autoReload - Whether to automatically reload (non-zero for auto-reload)
 * @param[in]       CBEnter - Callback function to call when timer expires
 * @param[in]       usrArg - User argument to pass to callback function
 * @return          TLK_ENONE for success, other negative values for failure
 * @note            This function does not allocate memory, directly uses the passed buffer as timer storage space
 */
int32_t tlkapi_timer_createStatic(TlkApiTimer_t *buffer, uint32_t periodUs, uint32_t autoReload, TlkApiTimerCB_t CBEnter, void *usrArg);

/**
 * @brief           Dynamically create a timer (allocates memory internally)
 * @param[out]      timerHandle - Pointer to store the created timer handle
 * @param[in]       periodUs - Timer period (microseconds)
 * @param[in]       autoReload - Whether to automatically reload (non-zero for auto-reload)
 * @param[in]       CBEnter - Callback function to call when timer expires
 * @param[in]       usrArg - User argument to pass to callback function
 * @return          TLK_ENONE for success, other negative values for failure
 * @note            This function allocates memory internally, need to call tlkapi_timer_destroy to release
 */
int32_t tlkapi_timer_create(TlkApiTimerHandle_t *timerHandle, uint32_t periodUs, uint32_t autoReload, TlkApiTimerCB_t CBEnter, void *usrArg);

/**
 * @brief           Destroy a timer
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle to destroy
 * @return          TLK_ENONE for success, other negative values for failure
 * @note            If the timer is executing its callback function, it will be automatically destroyed after the callback finishes
 */
int32_t tlkapi_timer_destroy(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle);

/**
 * @brief           Start a timer
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle to start
 * @return          TLK_ENONE for success, other negative values for failure
 */
int32_t tlkapi_timer_start(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle);

/**
 * @brief           Restart a timer
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle to restart
 * @return          TLK_ENONE for success, other negative values for failure
 * @note            If the timer is already running, it will be stopped and restarted
 */
int32_t tlkapi_timer_reStart(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle);

/**
 * @brief           Stop a timer
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle to stop
 * @return          TLK_ENONE for success, other negative values for failure
 * @note            If the timer is executing its callback function, it will be automatically stopped after the callback finishes
 */
int32_t tlkapi_timer_stop(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle);

/**
 * @brief           Set timer period
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle to set
 * @param[in]       periodUs - New timer period (microseconds)
 * @return          TLK_ENONE for success, other negative values for failure
 * @note            If the timer is running, it will reschedule the next expiration time
 */
int32_t tlkapi_timer_setPeriod(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle, uint32_t periodUs);

/**
 * @brief           Check if timer is started
 * @param[in]       timerHandle - Timer handle to check
 * @return          true if timer is started, false otherwise
 */
bool tlkapi_timer_isStarted(TlkApiTimerHandle_t timerHandle);

/**
 * @brief           Process expired timers
 * @param[in]       list - Timer list pointer
 * @note            This function will call the callback functions of all expired timers
 */
void tlkapi_timerList_handler(TlkApiTimerList_t *list);

/**
 * @brief           Get time until next timer expires (in microseconds)
 * @param[in]       list - Timer list pointer
 * @return          Time until next timer expires (in microseconds), or TLKOS_WAIT_FOREVER if no timers
 */
uint32_t tlkapi_timerList_getNextTimeUs(TlkApiTimerList_t *list);