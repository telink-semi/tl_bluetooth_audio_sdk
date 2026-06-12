/********************************************************************************************************
 * @file    tlkos_mutex_freertos.c
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
#include "tlklib/os/3rd-party/freertos-V5/portable/GCC/RISC-V/portmacro.h"
#include "tlklib/os/3rd-party/freertos-V5/include/semphr.h"

/**
 * @brief     Creates a mutex.
 * @param[out] mutexHandle Pointer to store the created mutex handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_mutex_create(TlkOsMutexHandle_t *mutexHandle)
{
    TLKOS_ASSERT(mutexHandle != NULL);
    *mutexHandle = xSemaphoreCreateMutex();
    return TLK_ENONE;
}

/**
 * @brief     Creates multiple mutexes in one memory block.
 * @param[out] mutexHandles Array to store created mutex handles.
 * @param[in]  count        Number of mutexes to create.
 * @returns   0 indicates success, other values indicate error codes.
 * @note      Must use tlkos_mutex_destroyMultiple to destory all.
 */
int tlkos_mutex_createMultiple(TlkOsMutexHandle_t *mutexHandles, uint32_t count)
{
    TLKOS_ASSERT(mutexHandles != NULL);
    TLKOS_ASSERT(count != 0);

    uint32_t totalSize = count * sizeof(StaticSemaphore_t);

    StaticSemaphore_t *pBuffer = (StaticSemaphore_t *)tlkos_malloc(totalSize);
    if (pBuffer == NULL) {
        return -TLK_ENOMEM;
    }

    for (uint32_t i = 0; i < count; i++) {
        mutexHandles[i] = xSemaphoreCreateMutexStatic(&pBuffer[i]);
        if (mutexHandles[i] == NULL) {
            for (uint16_t j = 0; j < i; j++) {
                vSemaphoreDelete((QueueHandle_t)mutexHandles[j]);
            }
            tlkos_free(pBuffer);
            return -TLK_EFAIL;
        }
    }

    return TLK_ENONE;
}

/**
 * @brief     Destroys a mutex.
 * @param[in] mutexHandle Handle of the mutex to be destroyed.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_mutex_destroy(TlkOsMutexHandle_t mutexHandle)
{
    if (mutexHandle == NULL) {
        return -TLK_EPARAM;
    }
    vSemaphoreDelete((mutexHandle));
    return TLK_ENONE;
}

/**
 * @brief     Destroys multiple mutexes created by createMultiple.
 * @param[in]  mutexHandles Array of mutex handles to destroy.
 * @param[in]  count        Number of mutexes to destroy.
 * @returns   0 indicates success, other values indicate error codes.
 */
int tlkos_mutex_destroyMultiple(TlkOsMutexHandle_t *mutexHandles, uint32_t count)
{
    TLKOS_ASSERT(mutexHandles != NULL);
    TLKOS_ASSERT(count != 0);

    void *pBase = (void *)mutexHandles[0];

    for (uint32_t i = 0; i < count; i++) {
        vSemaphoreDelete(mutexHandles[i]);
        mutexHandles[i] = NULL;
    }

    tlkos_free(pBase);
    return TLK_ENONE;
}

/**
 * @brief     Locks a mutex.
 * @param[in] mutexHandle Handle of the mutex to lock.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_mutex_lock(TlkOsMutexHandle_t mutexHandle)
{
    if (mutexHandle == NULL) {
        return -TLK_EPARAM;
    }
    BaseType_t waitTick = portMAX_DELAY;
    if (xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED) {
        waitTick = 0;
    }
    BaseType_t ret = xSemaphoreTake((QueueHandle_t)(mutexHandle), waitTick);
    configASSERT(ret == pdTRUE);
    return ret == pdTRUE ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief     Unlocks a mutex.
 * @param[in] mutexHandle Handle of the mutex to unlock.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_mutex_unlock(TlkOsMutexHandle_t mutexHandle)
{
    if (mutexHandle == NULL) {
        return -TLK_EPARAM;
    }
    BaseType_t ret = xSemaphoreGive((QueueHandle_t)(mutexHandle));
    return ret == pdTRUE ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief     Creates a recursive mutex.
 * @param[out] recursiveMutexHandle Pointer to store the created recursive mutex handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_recursiveMutex_create(TlkOsMutexHandle_t *recursiveMutexHandle)
{
    TLKOS_ASSERT(recursiveMutexHandle != NULL);
    *recursiveMutexHandle = xSemaphoreCreateRecursiveMutex();
    return TLK_ENONE;
}

/**
 * @brief     Creates multiple recursive mutexes in one memory block.
 * @param[out] mutexHandles Array to store created mutex handles.
 * @param[in]  count        Number of mutexes to create.
 * @returns   0 indicates success, other values indicate error codes.
 * @note      Must use tlkos_mutex_destroyMultiple to destory all.
 */
int tlkos_recursiveMutex_createMultiple(TlkOsMutexHandle_t *mutexHandles, uint32_t count)
{
    TLKOS_ASSERT(mutexHandles != NULL);
    TLKOS_ASSERT(count != 0);

    uint32_t totalSize = count * sizeof(StaticSemaphore_t);

    StaticSemaphore_t *pBuffer = (StaticSemaphore_t *)tlkos_malloc(totalSize);
    if (pBuffer == NULL) {
        return -TLK_ENOMEM;
    }

    for (uint32_t i = 0; i < count; i++) {
        mutexHandles[i] = xSemaphoreCreateRecursiveMutexStatic(&pBuffer[i]);
        if (mutexHandles[i] == NULL) {
            for (uint16_t j = 0; j < i; j++) {
                vSemaphoreDelete((QueueHandle_t)mutexHandles[j]);
            }
            tlkos_free(pBuffer);
            return -TLK_EFAIL;
        }
    }

    return TLK_ENONE;
}

/**
 * @brief     Locks a recursive mutex.
 * @param[in] recursiveMutexHandle Handle of the recursive mutex to lock.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_recursiveMutex_lock(TlkOsMutexHandle_t recursiveMutexHandle)
{
    if (recursiveMutexHandle == NULL) {
        return -TLK_EPARAM;
    }
    BaseType_t waitTick = portMAX_DELAY;
    if (xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED) {
        waitTick = 0;
    }
    BaseType_t ret = xSemaphoreTakeRecursive((QueueHandle_t)(recursiveMutexHandle), waitTick);
    configASSERT(ret == pdTRUE);
    return ret == pdTRUE ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief     Unlocks a recursive mutex.
 * @param[in] recursiveMutexHandle Handle of the recursive mutex to unlock.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_recursiveMutex_unlock(TlkOsMutexHandle_t recursiveMutexHandle)
{
    if (recursiveMutexHandle == NULL) {
        return -TLK_EPARAM;
    }
    BaseType_t ret = xSemaphoreGiveRecursive((QueueHandle_t)(recursiveMutexHandle));
    return ret == pdTRUE ? TLK_ENONE : -TLK_EFAIL;
}

#endif //TLKOS_CFG_FREERTOS_ENABLE
