/********************************************************************************************************
 * @file    tlkos_timer_freertos.c
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
#include "tlklib/os/tlkos_config.h"

#if TLKOS_CFG_FREERTOS_ENABLE
#include "tlklib/os/tlkos_api/tlkos_define.h"
#include "tlklib/os/3rd-party/freertos-V5/include/FreeRTOS.h"
#include "tlklib/os/3rd-party/freertos-V5/include/timers.h"
#include "tlkos_freertos_inner.h"

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
int tlkos_timer_create(char *pName, uint32_t periodMs, uint32_t autoReload, TlkOsTimerEnterCB CBEnter, void *pUsrArg, TlkOsTimerHandle_t *timerHandle)
{
    if (CBEnter == NULL || periodMs == 0 || periodMs == TLKOS_WAIT_FOREVER) {
        return -TLK_EPARAM;
    }
    uint32_t      periodTick = tlkos_freertos_msToTick(periodMs);
    TimerHandle_t handle     = xTimerCreate(pName, periodTick, autoReload, pUsrArg, (TimerCallbackFunction_t)CBEnter);
    if (handle == 0) {
        return -TLK_EFAIL;
    }
    if (timerHandle != NULL) {
        *timerHandle = (TlkOsTimerHandle_t)handle;
    }
    return TLK_ENONE;
}

/**
 * @brief     Destroys a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_destroy(TlkOsTimerHandle_t timerHandle)
{
    BaseType_t ret = xTimerDelete((TimerHandle_t)timerHandle, 0);
    return ret == pdTRUE ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief     Starts a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_start(TlkOsTimerHandle_t timerHandle)
{
    BaseType_t ret = xTimerStart((TimerHandle_t)(timerHandle), 0);
    return ret == pdTRUE ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief     Resets a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_reset(TlkOsTimerHandle_t timerHandle)
{
    BaseType_t ret = xTimerReset((TimerHandle_t)(timerHandle), 0);
    return ret == pdTRUE ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief     Stops a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_stop(TlkOsTimerHandle_t timerHandle)
{
    BaseType_t ret = xTimerStop((TimerHandle_t)(timerHandle), 0);
    return ret == pdTRUE ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief     Sets the timer period in microseconds.
 * @param[in] timerHandle Timer handle.
 * @param[in] periodUs Timer period in microseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      Only support on baremetal.
 */
int tlkos_timer_setPeriodUs(TlkOsTimerHandle_t timerHandle, uint32_t periodUs)
{
    (void)timerHandle;
    (void)periodUs;
    return -TLK_ENOSUPPORT;
}

/**
 * @brief     Sets the timer period in milliseconds.
 * @param[in] timerHandle Timer handle.
 * @param[in] periodMs Timer period in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_setPeriod(TlkOsTimerHandle_t timerHandle, uint32_t periodMs)
{
    uint32_t   periodTick = tlkos_freertos_msToTick(periodMs);
    BaseType_t ret        = xTimerChangePeriod((TimerHandle_t)(timerHandle), periodTick, 0);
    return ret == pdTRUE ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief     get the earliest timer task in milliseconds.
 * @returns   times, 0 means fail.
 */
uint32_t tlkos_timer_getNextTaskTimeMs(void)
{
    return 0;
}

/**
 * @brief     Indicates whether the timer has ISR-related API.
 * @returns   true indicates the timer has ISR-related API, false otherwise.
 * @note      Baremetal return false,and rtos return true.
 */
bool tlkos_timer_isHaveIsrApi(void)
{
    return true;
}

/**
 * @brief     Starts the timer from an interrupt service routine.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
_attribute_ram_code_sec_ int tlkos_timer_startFromISR(TlkOsTimerHandle_t timerHandle)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t ret                      = xTimerStartFromISR((TimerHandle_t)(timerHandle), &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    return ret == pdTRUE ? TLK_ENONE : -TLK_EFAIL;
}


#endif //TLKOS_CFG_FREERTOS_ENABLE
