/********************************************************************************************************
 * @file    tlkapi_timerList.c
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
#include "drivers.h"
#include "tlkapi/tlkapi_timerList.h"
#include "tlkapi/tlkapi_common.h"

#define TLKAPI_TIMEOUT_MAX (0x03FFFFFF)
#define TLKAPI_TIMEOUT_MIN 50

enum
{
    TLKAPI_TIMER_STATE_NONE = 0,
    TLKAPI_TIMER_STATE_START,
    TLKAPI_TIMER_STATE_STOP,
    TLKAPI_TIMER_STATE_DELETE,
};

/**
 * @brief           Allocate and clear memory for timer list operations
 * @param[in]       size - Size of memory to allocate in bytes
 * @return          Pointer to allocated and zero-initialized memory, or NULL if allocation fails
 */
static void *tlkapi_timerList_malloc(uint32_t size)
{
    return tlkos_calloc(size);
}

/**
 * @brief           Free memory allocated for timer list operations
 * @param[in]       ptr - Pointer to the memory block to free
 * @return          None
 */
static void tlkapi_timerList_free(void *ptr)
{
    tlkos_free(ptr);
}

/**
 * @brief           Compare two timestamps
 * @param[in]       curTicks - Current timestamp
 * @param[in]       refTicks - Reference timestamp
 * @return          0 if equal, 1 if curTicks is later than refTicks, -1 if curTicks is earlier than refTicks
 */
static int32_t tlkapi_timerList_compare(uint32_t curTicks, uint32_t refTicks)
{
    if (curTicks == refTicks) {
        return 0;
    } else if ((curTicks - refTicks) < (uint32_t)BIT(31)) {
        return 1;
    }
    return -1;
}

/**
 * @brief           Take the first expired timer from the timer list
 * @param[in]       ctrl - Timer list control structure pointer
 * @return          Expired timer node pointer, or NULL if none
 */
static TlkApiTimer_t *tlkapi_timerList_takeFirstTimer(TlkApiTimerList_t *ctrl)
{
    if (ctrl->pList == NULL) {
        return NULL;
    }

    TlkApiTimer_t *pNode = ctrl->pList->pNext;
    if (tlkapi_timerList_compare(pNode->arrival, clock_time()) > 0) {
        return NULL;
    }

    if (pNode == ctrl->pList) {
        ctrl->pList = NULL;
    } else {
        ctrl->pList->pNext = pNode->pNext;
    }
    pNode->pNext = NULL;
    return pNode;
}

/**
 * @brief           Get the time until the next timer expires (in microseconds)
 * @param[in]       ctrl - Timer list control structure pointer
 * @return          Time until next timer expires (in microseconds), or TLKOS_WAIT_FOREVER if no timers
 */
static uint32_t tlkapi_timerList_getNextTimeUsCore(TlkApiTimerList_t *ctrl)
{
    if (ctrl->pList == NULL) {
        return TLKOS_WAIT_FOREVER;
    }
    TlkApiTimer_t *pTimer = ctrl->pList->pNext;
    uint32_t       time   = pTimer->arrival - clock_time();
    if (time >= (uint32_t)BIT(31)) {
        return 0;
    }
    return (time / SYSTEM_TIMER_TICK_1US);
}

/**
 * @brief           Remove a specified timer from the timer list
 * @param[in]       ctrl - Timer list control structure pointer
 * @param[in]       pTimer - Timer node pointer to remove
 */
static void tlkapi_timerList_remove(TlkApiTimerList_t *ctrl, TlkApiTimer_t *pTimer)
{
    if (ctrl->pList == NULL || pTimer->pNext == NULL) {
        return;
    }

    TlkApiTimer_t *pTemp = ctrl->pList;
    while (1) {
        if (pTemp->pNext == pTimer) {
            break; //find node, break;
        }
        if (pTemp->pNext == ctrl->pList) {
            return; //end of list,not found,just return;
        }
        pTemp = pTemp->pNext;
    }

    //remove node
    if (ctrl->pList != pTimer) {
        pTemp->pNext = pTimer->pNext;
    } else if (pTemp == ctrl->pList) {
        ctrl->pList = NULL;
    } else {
        pTemp->pNext = pTimer->pNext;
        ctrl->pList  = pTemp;
    }

    pTimer->pNext   = NULL;
    pTimer->arrival = 0;
}

/**
 * @brief           Insert a timer into the timer list (sorted by expiration time)
 * @param[in]       ctrl - Timer list control structure pointer
 * @param[in]       pTimer - Timer node pointer to insert
 */
static void tlkapi_timerList_insert(TlkApiTimerList_t *ctrl, TlkApiTimer_t *pTimer)
{
    if (pTimer->pNext != NULL) {
        return; //have in list
    }
    pTimer->arrival = clock_time() + pTimer->timeout;

    if (ctrl->pList == NULL) {
        pTimer->pNext = pTimer;
        ctrl->pList   = pTimer;
        return; //list is empty:ez operate
    }
    bool           valid = true;
    TlkApiTimer_t *pTemp = ctrl->pList;
    while (tlkapi_timerList_compare(pTemp->pNext->arrival, pTimer->arrival) == -1) {
        if (pTemp->pNext == ctrl->pList) {
            valid = false;
            break;
        }
        pTemp = pTemp->pNext;
    }
    if (valid) {
        pTimer->pNext = pTemp->pNext;
        pTemp->pNext  = pTimer;
    } else {
        pTimer->pNext      = ctrl->pList->pNext;
        ctrl->pList->pNext = pTimer;
        ctrl->pList        = pTimer;
    }
}

/**
 * @brief           Core function to start or restart a timer
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle
 * @param[in]       isReStart - Whether to restart the timer
 * @return          TLK_ENONE for success, other negative values for failure
 */
static int32_t tlkapi_timer_startCore(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle, bool isReStart)
{
    if (list == NULL || timerHandle == NULL) {
        return -TLK_EPARAM;
    }
    TlkApiTimer_t *pTimer = (TlkApiTimer_t *)timerHandle;
    if (pTimer->nowState == TLKAPI_TIMER_STATE_NONE) {
        return -TLK_EFAIL;
    }
    if (pTimer->runningCB) {
        if (pTimer->nowState == TLKAPI_TIMER_STATE_DELETE) {
            return -TLK_EFAIL; //have been deleted
        }
        pTimer->nowState = TLKAPI_TIMER_STATE_START;
        return TLK_ENONE; //in callback do noting,wait cb finish,will auto restart
    }

    //not in callback
    if (pTimer->nowState == TLKAPI_TIMER_STATE_START) {
        if (isReStart) {
            tlkapi_timerList_remove(list, pTimer);
            tlkapi_timerList_insert(list, pTimer);
        }
        return TLK_ENONE;
    }
    tlkapi_timerList_insert(list, pTimer);
    pTimer->nowState = TLKAPI_TIMER_STATE_START;

    return TLK_ENONE;
}

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
int32_t tlkapi_timer_createStatic(TlkApiTimer_t *buffer, uint32_t periodUs, uint32_t autoReload, TlkApiTimerCB_t CBEnter, void *usrArg)
{
    TlkApiTimer_t *pTimer = buffer;
    if (pTimer == NULL || periodUs >= TLKAPI_TIMEOUT_MAX || CBEnter == NULL || periodUs < TLKAPI_TIMEOUT_MIN) {
        return -TLK_EPARAM;
    }
    if (pTimer->nowState != TLKAPI_TIMER_STATE_NONE) {
        return -TLK_EREPEAT;
    }
    pTimer->nowState   = TLKAPI_TIMER_STATE_STOP;
    pTimer->autoReload = autoReload > 0 ? 1 : 0;
    pTimer->pNext      = NULL;
    pTimer->timeout    = periodUs * SYSTEM_TIMER_TICK_1US;
    pTimer->arrival    = 0;
    pTimer->malloced   = 0;
    pTimer->userArg    = usrArg;
    pTimer->timerCB    = CBEnter;

    return TLK_ENONE;
}

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
int32_t tlkapi_timer_create(TlkApiTimerHandle_t *timerHandle, uint32_t periodUs, uint32_t autoReload, TlkApiTimerCB_t CBEnter, void *usrArg)
{
    if (timerHandle == NULL || periodUs >= TLKAPI_TIMEOUT_MAX || CBEnter == NULL || periodUs < TLKAPI_TIMEOUT_MIN) {
        return -TLK_EPARAM;
    }
    TlkApiTimer_t *pTimer = tlkapi_timerList_malloc(sizeof(TlkApiTimer_t));
    if (pTimer == NULL) {
        return -TLK_ENOMEM;
    }
    *timerHandle = pTimer;
    tlkapi_timer_createStatic(pTimer, periodUs, autoReload, CBEnter, usrArg);
    pTimer->malloced = 1;
    return TLK_ENONE;
}

/**
 * @brief           Destroy a timer
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle to destroy
 * @return          TLK_ENONE for success, other negative values for failure
 * @note            If the timer is executing its callback function, it will be automatically destroyed after the callback finishes
 */
int32_t tlkapi_timer_destroy(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle)
{
    if (list == NULL || timerHandle == NULL) {
        return -TLK_EPARAM;
    }
    TlkApiTimer_t *pTimer = (TlkApiTimer_t *)timerHandle;
    if (pTimer->nowState == TLKAPI_TIMER_STATE_NONE) {
        return -TLK_EFAIL;
    }
    if (pTimer->runningCB) {
        pTimer->nowState = TLKAPI_TIMER_STATE_DELETE;
        return TLK_ENONE; //in callback do noting,wait cb finish,will auto delete
    }

    //not in callback
    tlkapi_timerList_remove(list, pTimer);
    if (pTimer->malloced) {
        tlkapi_timerList_free(pTimer);
    } else {
        memset(pTimer, 0, sizeof(TlkApiTimer_t));
    }
    return TLK_ENONE;
}

/**
 * @brief           Start a timer
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle to start
 * @return          TLK_ENONE for success, other negative values for failure
 */
int32_t tlkapi_timer_start(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle)
{
    return tlkapi_timer_startCore(list, timerHandle, false);
}

/**
 * @brief           Restart a timer
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle to restart
 * @return          TLK_ENONE for success, other negative values for failure
 * @note            If the timer is already running, it will be stopped and restarted
 */
int32_t tlkapi_timer_reStart(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle)
{
    return tlkapi_timer_startCore(list, timerHandle, true);
}

/**
 * @brief           Stop a timer
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle to stop
 * @return          TLK_ENONE for success, other negative values for failure
 * @note            If the timer is executing its callback function, it will be automatically stopped after the callback finishes
 */
int32_t tlkapi_timer_stop(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle)
{
    if (list == NULL || timerHandle == NULL) {
        return -TLK_EPARAM;
    }
    TlkApiTimer_t *pTimer = (TlkApiTimer_t *)timerHandle;
    if (pTimer->nowState == TLKAPI_TIMER_STATE_NONE) {
        return -TLK_EFAIL;
    }
    if (pTimer->runningCB) {
        if (pTimer->nowState == TLKAPI_TIMER_STATE_DELETE) {
            return -TLK_EFAIL; //have been deleted
        }
        pTimer->nowState = TLKAPI_TIMER_STATE_STOP;
        return TLK_ENONE; //in callback do noting,wait cb finish,will auto stop
    }

    //not in callback
    if (pTimer->nowState == TLKAPI_TIMER_STATE_STOP) {
        return TLK_ENONE;
    }
    tlkapi_timerList_remove(list, pTimer);
    pTimer->nowState = TLKAPI_TIMER_STATE_STOP;

    return TLK_ENONE;
}

/**
 * @brief           Set timer period
 * @param[in]       list - Timer list pointer
 * @param[in]       timerHandle - Timer handle to set
 * @param[in]       periodUs - New timer period (microseconds)
 * @return          TLK_ENONE for success, other negative values for failure
 * @note            If the timer is running, it will reschedule the next expiration time
 */
int32_t tlkapi_timer_setPeriod(TlkApiTimerList_t *list, TlkApiTimerHandle_t timerHandle, uint32_t periodUs)
{
    if (list == NULL || timerHandle == NULL || periodUs < TLKAPI_TIMEOUT_MIN || periodUs >= TLKAPI_TIMEOUT_MAX) {
        return -TLK_EPARAM;
    }
    TlkApiTimer_t *pTimer = (TlkApiTimer_t *)timerHandle;
    if (pTimer->nowState == TLKAPI_TIMER_STATE_NONE) {
        return -TLK_EFAIL;
    }
    if (pTimer->runningCB) {
        if (pTimer->nowState == TLKAPI_TIMER_STATE_DELETE) {
            return -TLK_EFAIL; //have been deleted
        }
        pTimer->timeout = periodUs * SYSTEM_TIMER_TICK_1US;
        return TLK_ENONE; //in callback do noting,wait cb finish,will auto start/stop.
    }

    //not in callback
    if (pTimer->nowState == TLKAPI_TIMER_STATE_STOP) {
        pTimer->timeout = periodUs * SYSTEM_TIMER_TICK_1US;
        return TLK_ENONE;
    }
    tlkapi_timerList_remove(list, pTimer);
    pTimer->timeout = periodUs * SYSTEM_TIMER_TICK_1US;
    tlkapi_timerList_insert(list, pTimer);

    return TLK_ENONE;
}

/**
 * @brief           Check if timer is started
 * @param[in]       timerHandle - Timer handle to check
 * @return          true if timer is started, false otherwise
 */
bool tlkapi_timer_isStarted(TlkApiTimerHandle_t timerHandle)
{
    TlkApiTimer_t *pTimer = (TlkApiTimer_t *)timerHandle;
    return pTimer->pNext != NULL;
}

/**
 * @brief           Process expired timers
 * @param[in]       list - Timer list pointer
 * @note            This function will call the callback functions of all expired timers
 */
void tlkapi_timerList_handler(TlkApiTimerList_t *list)
{
    if (list->pList == NULL) {
        return;
    }

    TlkApiTimer_t *pTimer = tlkapi_timerList_takeFirstTimer(list);
    while (pTimer != NULL) {
        if (pTimer->autoReload == false) {
            pTimer->nowState = TLKAPI_TIMER_STATE_STOP;
        }

        pTimer->runningCB = 1;
        pTimer->timerCB(pTimer, pTimer->userArg); //in callback may use start/stop/destroy api
        pTimer->runningCB = 0;

        if (pTimer->nowState == TLKAPI_TIMER_STATE_DELETE) {
            tlkapi_timer_destroy(list, pTimer);
        } else if (pTimer->nowState == TLKAPI_TIMER_STATE_START) {
            tlkapi_timerList_insert(list, pTimer);
        }
        pTimer = tlkapi_timerList_takeFirstTimer(list);
    }
}

/**
 * @brief           Get time until next timer expires (in microseconds)

 * @return          Time until next timer expires (in microseconds), or TLKOS_WAIT_FOREVER if no timers
 */
uint32_t tlkapi_timerList_getNextTimeUs(TlkApiTimerList_t *list)
{
    if (list == NULL) {
        return TLKOS_WAIT_FOREVER;
    }
    return tlkapi_timerList_getNextTimeUsCore(list);
}
