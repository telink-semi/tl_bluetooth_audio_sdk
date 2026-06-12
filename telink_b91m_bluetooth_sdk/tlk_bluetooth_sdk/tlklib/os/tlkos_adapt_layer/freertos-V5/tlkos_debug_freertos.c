/********************************************************************************************************
 * @file    tlkos_debug_freertos.c
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
#include "tlkapi/tlkapi.h"
#include "tlklib/os/tlkos_config.h"
#include <stdio.h>

#if TLKOS_CFG_FREERTOS_ENABLE && TLKOS_CFG_DEBUG_ENABLE
#include "tlklib/os/tlkos_api/tlkos_define.h"
#include "tlklib/os/3rd-party/freertos-V5/include/FreeRTOS.h"
#include "tlklib/os/3rd-party/freertos-V5/include/task.h"

/* Telink custom function to get system state for debug purposes */
UBaseType_t uxTaskGetSystemState_tlk_debug(TaskStatus_t *const pxTaskStatusArray, const UBaseType_t uxArraySize, uint32_t *const pulTotalRunTime);

#define TLKOS_DEBUG_TASK_MAX_NUM 10

typedef enum
{
    TLKOS_DEBUG_FREERTOS_INFO_SM_NONE = 0,
    TLKOS_DEBUG_FREERTOS_INFO_SM_TITLE,
    TLKOS_DEBUG_FREERTOS_INFO_SM_TASK_INF0,
    TLKOS_DEBUG_FREERTOS_INFO_SM_END,
} TLKOS_DEBUG_FREERTOS_INFO_SM_ENUM;

static uint32_t     tlkos_debug_info_sm        = TLKOS_DEBUG_FREERTOS_INFO_SM_NONE;
static UBaseType_t  tlkos_debug_info_taskIndex = 0;
static char         tlkos_debug_info_logBuf[200];
static TaskStatus_t pxTaskStatusArray[TLKOS_DEBUG_TASK_MAX_NUM];

/**
 * @brief       Get FreeRTOS core information
 * @return      Pointer to string containing core information.
 * @note        This function returns system state information including task details.
 */
static const char *tlkos_debug_freeRTOSCoreInfo(void)
{
    if (tlkos_debug_info_sm >= TLKOS_DEBUG_FREERTOS_INFO_SM_END) {
        return NULL;
    } else if (tlkos_debug_info_sm == TLKOS_DEBUG_FREERTOS_INFO_SM_NONE) {
        extern uint32_t AAA_OS_mcause;
        extern uint32_t AAA_OS_mdcause;
        extern uint32_t AAA_OS_mepc;
        extern uint32_t AAA_OS_mtval;
        ;
        sprintf(tlkos_debug_info_logBuf, "mcause %d md %d mepc %x mtval %d", (unsigned int)AAA_OS_mcause, (unsigned int)AAA_OS_mdcause, (unsigned int)AAA_OS_mepc,
                (unsigned int)AAA_OS_mtval);
        tlkos_debug_info_sm = TLKOS_DEBUG_FREERTOS_INFO_SM_TITLE;

    } else if (tlkos_debug_info_sm == TLKOS_DEBUG_FREERTOS_INFO_SM_TITLE) {
        sprintf(tlkos_debug_info_logBuf, "TaskName\tWaterMark\tTask State\tStackBase\n");
        tlkos_debug_info_sm = TLKOS_DEBUG_FREERTOS_INFO_SM_TASK_INF0;
        uxTaskGetSystemState_tlk_debug(pxTaskStatusArray, TLKOS_DEBUG_TASK_MAX_NUM, NULL);

    } else if (tlkos_debug_info_sm == TLKOS_DEBUG_FREERTOS_INFO_SM_TASK_INF0) {
        if (uxTaskGetNumberOfTasks() <= tlkos_debug_info_taskIndex) {
            tlkos_debug_info_sm = TLKOS_DEBUG_FREERTOS_INFO_SM_END;
            return NULL;
        }
        TaskStatus_t *xTaskStatus = &pxTaskStatusArray[tlkos_debug_info_taskIndex];
        sprintf(tlkos_debug_info_logBuf, "%s\t\t%d\t\t%d\t\t%x\n", xTaskStatus->pcTaskName, (int)xTaskStatus->usStackHighWaterMark, (int)xTaskStatus->eCurrentState,
                (int)xTaskStatus->pxStackBase);
        tlkos_debug_info_taskIndex++;
    }
    return (const char *)tlkos_debug_info_logBuf;
}

/**
 * @brief       Get core information for FreeRTOS implementation
 * @return      Pointer to string containing core information.
 */
const char *tlkos_debug_getCoreInfo(void)
{
    return tlkos_debug_freeRTOSCoreInfo();
}

/**
 * @brief       Hook function for handling stack overflow in FreeRTOS
 * @param[in]   xTask       - Handle of the task that has exceeded its stack
 * @param[in]   pcTaskName  - Name of the task that has exceeded its stack
 * @return      none.
 * @note        This function is automatically called by FreeRTOS when a stack overflow is detected.
 */
_attribute_ram_code_sec_ void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    TlkOsCrashInfo_t info = {
        .reason     = TLKOS_CRASH_REASON_TASK_STACK_OVERFLOW,
        .detailInfo = pcTaskName,
    };
    tlkos_crash(&info);
}

/**
 * @brief       Hook function for handling malloc failure in FreeRTOS
 * @return      none.
 * @note        This function is automatically called by FreeRTOS when a memory allocation fails.
 */
_attribute_ram_code_sec_ void vApplicationMallocFailedHook(void)
{
    static const TlkOsCrashInfo_t info = {
        .reason     = TLKOS_CRASH_REASON_CORE_MALLOC_FAIL,
        .detailInfo = "CoreMallocFailed",
    };
    tlkos_crash(&info);
}

#if TLKOS_CFG_DEBUG_CPU_USAGE
static uint32_t tlkosCpuUsage     = 0;
static uint32_t tlkosIdleTimeTick = 0;

/**
 * @brief       Set idle time for CPU usage calculation
 * @param[in]   isEnter         - Flag indicating whether entering or exiting idle state
 * @param[in]   totalTimeMs     - Total time in milliseconds for CPU usage calculation
 * @return      none.
 * @note        This function tracks idle time to calculate CPU usage in FreeRTOS.
 */
_attribute_ram_code_sec_ void tlkos_debug_setIdleTimeUs(uint8_t isEnter, uint32_t totalTimeMs)
{
    static uint32_t lastTime = 0;
    unsigned int    r        = core_interrupt_disable();
    if (totalTimeMs == 0) {
        if (isEnter) {
            lastTime = clock_time() | 1;
        } else {
            tlkosIdleTimeTick += clock_time() - lastTime;
            lastTime = 0;
        }
    } else {
        if (lastTime != 0) {
            tlkosIdleTimeTick += clock_time() - lastTime;
            lastTime = clock_time() | 1;
        }
        uint32_t usedTimeUs = totalTimeMs * 1000 - tlkosIdleTimeTick / SYSTEM_TIMER_TICK_1US;
        tlkosCpuUsage       = usedTimeUs / totalTimeMs;
        tlkosIdleTimeTick   = 0;
    }
    core_restore_interrupt(r);
}

/**
 * @brief       Get CPU usage in FreeRTOS
 * @return      Current CPU usage value.
 */
uint32_t tlkos_debug_getCpuUsage(void)
{
    return tlkosCpuUsage;
}
#else
/**
 * @brief       Set idle time for CPU usage calculation (dummy implementation)
 * @param[in]   isEnter     - Flag indicating whether entering or exiting idle state
 * @param[in]   totalTime   - Total time parameter (unused in dummy implementation)
 * @return      none.
 */
_attribute_ram_code_sec_ void tlkos_debug_setIdleTimeUs(uint8_t isEnter, uint32_t totalTime)
{
    (void)isEnter;
    (void)totalTime;
}

/**
 * @brief       Get CPU usage (dummy implementation)
 * @return      0XFFFFFFFF - CPU usage information is not available in this implementation.
 */
uint32_t tlkos_debug_getCpuUsage(void)
{
    return 0XFFFFFFFF;
}
#endif

#endif //TLKOS_CFG_FREERTOS_ENABLE && TLKOS_CFG_DEBUG_ENABLE