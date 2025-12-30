/********************************************************************************************************
 * @file    tlkos_task.h
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

typedef struct
{
    void    *pStaticBuffer;    /**< User-supplied static buffer; NULL = dynamic create */
    uint32_t staticBufferSize; /**< Size of the buffer in bytes; ignored if pStaticBuffer is NULL */
} TlkosTaskExtCfg_t;

/**
 * @brief     Creates a new task.
 * @param[in] enterCB Function pointer to the task entry callback.
 * @param[in] pName Name of the task (for debugging purposes).
 * @param[in] stackSize Size of the task stack in bytes.
 * @param[in] priority Priority of the task.
 * @param[in] CBUsrArg User-defined argument passed to the task function.
 * @param[in] extArg Additional argument，refer to TlkosTaskExtCfg_t.
 * @param[out] taskHandle Pointer to store the created task handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      Stack size must be at least 256 bytes.
 * 
 */
int tlkos_task_create(TlkOsTaskEnterCB enterCB, const char *pName, uint32_t stackSize, uint32_t priority, void *CBUsrArg, TlkosTaskExtCfg_t *extArg, TlkOsTaskHandle_t *taskHandle);

/**
 * @brief     Destroys a task.
 * @param[in] taskHandle Handle of the task to be destroyed.
 * @returns   None.
 * @note      The task must have been previously created using tlkos_task_create.
 */
void tlkos_task_destroy(TlkOsTaskHandle_t taskHandle);

/**
 * @brief     Gets the priority of a task.
 * @param[in] taskHandle Handle of the task whose priority is to be obtained.
 * @returns   The priority value of the task.
 * @note      This function can only be called from normal context.
 */
uint32_t tlkos_task_getPriority(TlkOsTaskHandle_t taskHandle);

/**
 * @brief     Gets the priority of a task from an interrupt service routine (ISR).
 * @param[in] taskHandle Handle of the task whose priority is to be obtained.
 * @returns   The priority value of the task.
 * @note      This function can only be called from within an ISR.
 */
uint32_t tlkos_task_getPriorityFromIsr(TlkOsTaskHandle_t taskHandle);

/**
 * @brief     Sets the priority of a task.
 * @param[in] taskHandle Handle of the task whose priority is to be set.
 * @param[in] priority The new priority value to set for the task.
 * @returns   None.
 * @note      This function can only be called from normal context.
 */
void tlkos_task_setPriority(TlkOsTaskHandle_t taskHandle, uint32_t priority);

/**
 * @brief     Sets the priority of a task from an interrupt service routine (ISR).
 * @param[in] taskHandle Handle of the task whose priority is to be set.
 * @param[in] priority The new priority value to set for the task.
 * @returns   None.
 * @note      This function can only be called from within an ISR.
 */
void tlkos_task_setPriorityFromIsr(TlkOsTaskHandle_t taskHandle, uint32_t priority);

/**
 * @brief     Delays the current task for a specified number of milliseconds.
 * @param[in] delayMs Delay duration in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      This function can only be called from within a task.
 */
int tlkos_task_delayMs(uint32_t delayMs);

/**
 * @brief     Gets the handle of the currently running task.
 * @returns   Handle of the current task or NULL if not in a task context.
 * @note      This function can be called from any context.
 */
TlkOsTaskHandle_t tlkos_task_getRunningTask(void);

/**
 * @brief     Suspends a task.
 * @param[in] taskHandle Handle of the task to suspend.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      The task must be in a runnable state.
 */
int tlkos_task_suspend(TlkOsTaskHandle_t taskHandle);

/**
 * @brief     Resumes a suspended task.
 * @param[in] taskHandle Handle of the task to resume.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      The task must have been previously suspended.
 */
int tlkos_task_resume(TlkOsTaskHandle_t taskHandle);

/**
 * @brief     Suspends all tasks.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      This function can only be called from within a task.
 */
int tlkos_task_suspendAll(void);

/**
 * @brief     Resumes all suspended tasks.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note      This function can only be called from within a task.
 */
int tlkos_task_resumeAll(void);

/**

    @brief     Gets the stack water mark (minimum free space) of a task.
    @param[in] taskHandle Handle of the task to query.
    @returns   The minimum free space of the task stack in words(4 byte).
    @note      The stack water mark indicates the minimum amount of free space in the stack since the task was created. It is a key metric for evaluating whether the task stack size is sufficient and helps developers avoid stack overflow issues.
*/
uint32_t tlkos_task_getStackWaterMark(TlkOsTaskHandle_t taskHandle);

/**
 * @brief     Converts a delay interval from microseconds to milliseconds with thresholds.
 * @param[in] nextIntvUs Delay interval in microseconds.
 * @param[in] lowTHMs Lower threshold in milliseconds.
 * @param[in] highTHMs Upper threshold in milliseconds.
 * @returns   Delay interval in milliseconds, clamped between lowTHMs and highTHMs.
 * @note      This function ensures the delay is within the specified threshold range.
 */
static inline uint32_t tlkos_task_nextIntvUsToMs(uint32_t nextIntvUs, uint32_t lowTHMs, uint32_t highTHMs)
{
    uint32_t delayMs = nextIntvUs;
    if (delayMs != TLKOS_WAIT_FOREVER) {
        delayMs = delayMs / 1000;
    }
    delayMs = delayMs < lowTHMs ? lowTHMs : delayMs;
    delayMs = delayMs > highTHMs ? highTHMs : delayMs;
    return delayMs;
}
