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
    if (mutexHandle == NULL) {
        return -TLK_EPARAM;
    }
    *mutexHandle = xSemaphoreCreateMutex();
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
 * @brief     Locks a mutex.
 * @param[in] mutexHandle Handle of the mutex to lock.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_mutex_lock(TlkOsMutexHandle_t mutexHandle)
{
    if (mutexHandle == NULL) {
        return -TLK_EPARAM;
    }
    BaseType_t ret = xSemaphoreTake((QueueHandle_t)(mutexHandle), portMAX_DELAY);
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
    if (recursiveMutexHandle == NULL) {
        return -TLK_EPARAM;
    }
    *recursiveMutexHandle = xSemaphoreCreateRecursiveMutex();
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
    BaseType_t ret = xSemaphoreTakeRecursive((QueueHandle_t)(recursiveMutexHandle), portMAX_DELAY);
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
