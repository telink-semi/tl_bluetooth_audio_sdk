/********************************************************************************************************
 * @file    tlkos_timer_baremetal.c
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
#include "tlkapi/tlkapi_common.h"
#include "tlkapi/tlkapi_timerList.h"
#include "tlklib/os/tlkos_config.h"

#if TLKOS_CFG_BAREMETAL_ENABLE
#include "tlklib/os/tlkos_api/tlkos_define.h"
#include "tlklib/os/tlkos.h"


static uint8_t sTlkOsTimerIsBusy  = false;
static uint8_t sTlkOsTimerIsStart = false;

static TlkApiTimerList_t sTlkOsTimerList = {0};

/**
 * @brief       Start the OS timer with specified interval in microseconds
 * @param[in]   intervalUs    - the interval time in microsecond
 * @return      none.
 */
static void tlkos_timer_start_inner(uint32_t intervalUs)
{
    if (intervalUs == TLKOS_WAIT_FOREVER) {
        return;
    }
    intervalUs = max(intervalUs, 50); //at least 50us
    tlkos_enter_critical();

    if (sTlkOsTimerIsStart) {
        timer_stop(TIMER1);
#if (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TL752X_TEMP)
        timer_clr_irq_status(FLD_TMR1_MODE_IRQ);
        timer_set_irq_mask(FLD_TMR1_MODE_IRQ);
#else
        timer_clr_irq_status(TMR_STA_TMR1);
#endif
    }
    sTlkOsTimerIsStart = true;
#if (!MCU_CORE_TL752X_TEMP)
    timer_set_init_tick(TIMER1, 0);
    timer_set_cap_tick(TIMER1, intervalUs * sys_clk.pclk);
#else
    timer_init(TIMER0);
    timer_set_cap_tick(TIMER1, intervalUs * sys_clk.pclk);
#endif
    timer_set_mode(TIMER1, TIMER_MODE_SYSCLK);
    timer_start(TIMER1);
    tlkos_leave_critical();
}

/**
 * @brief       Attempt to start the OS timer if not busy
 * @return      none.
 * @note        This function checks if the timer is busy, and if not, gets the next timer
 *              expiration time and starts the timer with that interval
 */
static inline void tlkos_timer_try_start_inner(void)
{
    if (sTlkOsTimerIsBusy) {
        return;
    }
    uint32_t interval = tlkapi_timerList_getNextTimeUs(&sTlkOsTimerList);
    tlkos_timer_start_inner(interval);
}

/**
 * @brief     This function is used as an irq function in baremetal.
 * @param[in] None.
 * @returns   None.
 */
void tlkos_timer_irq_handler(void)
{
    tlkos_enter_critical();
    sTlkOsTimerIsStart = false;
#if (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TL752X_TEMP)
    timer_clr_irq_status(FLD_TMR1_MODE_IRQ);
#else
    timer_clr_irq_status(TMR_STA_TMR1);
#endif
    tlkos_leave_critical();
    sTlkOsTimerIsBusy = true;

    tlkapi_timerList_handler(&sTlkOsTimerList);

    sTlkOsTimerIsBusy = false;
    tlkos_timer_try_start_inner();
}

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
    (void)pName;
    TlkApiTimerHandle_t handle = NULL;
    tlkos_enter_critical();
    int ret = tlkapi_timer_create(&handle, periodMs * 1000, autoReload, CBEnter, pUsrArg);
    tlkos_leave_critical();
    if (ret == TLK_ENONE && timerHandle != NULL) {
        *timerHandle = handle;
    }
    return ret;
}

/**
 * @brief     Destroys a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_destroy(TlkOsTimerHandle_t timerHandle)
{
    tlkos_enter_critical();
    int ret = tlkapi_timer_destroy(&sTlkOsTimerList, timerHandle);
    tlkos_timer_try_start_inner();
    tlkos_leave_critical();
    return ret;
}

/**
 * @brief     Starts a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_start(TlkOsTimerHandle_t timerHandle)
{
    tlkos_enter_critical();
    int ret = tlkapi_timer_start(&sTlkOsTimerList, timerHandle);
    tlkos_timer_try_start_inner();
    tlkos_leave_critical();
    return ret;
}

/**
 * @brief     Resets a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_reset(TlkOsTimerHandle_t timerHandle)
{
    return tlkos_timer_start(timerHandle);
}

/**
 * @brief     Stops a timer.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_stop(TlkOsTimerHandle_t timerHandle)
{
    tlkos_enter_critical();
    int ret = tlkapi_timer_stop(&sTlkOsTimerList, timerHandle);
    tlkos_timer_try_start_inner();
    tlkos_leave_critical();
    return ret;
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
    tlkos_enter_critical();
    int ret = tlkapi_timer_setPeriod(&sTlkOsTimerList, timerHandle, periodUs);
    tlkos_timer_try_start_inner();
    tlkos_leave_critical();
    return ret;
}

/**
 * @brief     Sets the timer period in milliseconds.
 * @param[in] timerHandle Timer handle.
 * @param[in] periodMs Timer period in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_setPeriod(TlkOsTimerHandle_t timerHandle, uint32_t periodMs)
{
    return tlkos_timer_setPeriodUs(timerHandle, periodMs * 1000);
}

/**
 * @brief     get the earliest timer task in milliseconds.
 * @returns   times, 0 means fail.
 */
uint32_t tlkos_timer_getNextTaskTimeMs(void)
{
    tlkos_enter_critical();
    uint32_t us = tlkapi_timerList_getNextTimeUs(&sTlkOsTimerList);
    tlkos_leave_critical();
    return tlkos_task_nextIntvUsToMs(us, 1, TLKOS_WAIT_FOREVER);
}

/**
 * @brief     Indicates whether the timer has ISR-related API.
 * @returns   true indicates the timer has ISR-related API, false otherwise.
 * @note      Baremetal return false,and rtos return true.
 */
bool tlkos_timer_isHaveIsrApi(void)
{
    return false;
}

/**
 * @brief     Starts the timer from an interrupt service routine.
 * @param[in] timerHandle Timer handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_timer_startFromISR(TlkOsTimerHandle_t timerHandle)
{
    (void)timerHandle;
    return -TLK_ENOSUPPORT;
}

#else //TLKOS_CFG_BAREMETAL_ENABLE
/**
 * @brief     This function is used as an irq function in baremetal.
 * @param[in] None.
 * @returns   None.
 */
void tlkos_timer_irq_handler(void) {}

#endif //TLKOS_CFG_BAREMETAL_ENABLE
