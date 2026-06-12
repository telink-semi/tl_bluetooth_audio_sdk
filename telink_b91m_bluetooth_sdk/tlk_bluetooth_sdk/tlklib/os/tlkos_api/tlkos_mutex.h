/********************************************************************************************************
 * @file    tlkos_mutex.h
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
 * @brief     Creates a mutex.
 * @param[out] mutexHandle Pointer to store the created mutex handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_mutex_create(TlkOsMutexHandle_t *mutexHandle);

/**
 * @brief     Creates multiple mutexes in one memory block.
 * @param[out] mutexHandles Array to store created mutex handles.
 * @param[in]  count        Number of mutexes to create.
 * @returns   0 indicates success, other values indicate error codes.
 * @note      Must use tlkos_mutex_destroyMultiple to destory all.
 */
int tlkos_mutex_createMultiple(TlkOsMutexHandle_t *mutexHandles, uint32_t count);

/**
 * @brief     Destroys a mutex.
 * @param[in] mutexHandle Handle of the mutex to be destroyed.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_mutex_destroy(TlkOsMutexHandle_t mutexHandle);

/**
 * @brief     Destroys multiple mutexes created by createMultiple.
 * @param[in]  mutexHandles Array of mutex handles to destroy.
 * @param[in]  count        Number of mutexes to destroy.
 * @returns   0 indicates success, other values indicate error codes.
 */
int tlkos_mutex_destroyMultiple(TlkOsMutexHandle_t *mutexHandles, uint32_t count);

/**
 * @brief     Creates multiple recursive mutexes in one memory block.
 * @param[out] mutexHandles Array to store created mutex handles.
 * @param[in]  count        Number of mutexes to create.
 * @returns   0 indicates success, other values indicate error codes.
 * @note      Must use tlkos_mutex_destroyMultiple to destory all.
 */
int tlkos_recursiveMutex_createMultiple(TlkOsMutexHandle_t *mutexHandles, uint32_t count);

/**
 * @brief     Locks a mutex.
 * @param[in] mutexHandle Handle of the mutex to lock.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_mutex_lock(TlkOsMutexHandle_t mutexHandle);

/**
 * @brief     Unlocks a mutex.
 * @param[in] mutexHandle Handle of the mutex to unlock.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_mutex_unlock(TlkOsMutexHandle_t mutexHandle);

/**
 * @brief     Creates a recursive mutex.
 * @param[out] recursiveMutexHandle Pointer to store the created recursive mutex handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_recursiveMutex_create(TlkOsMutexHandle_t *recursiveMutexHandle);

/**
 * @brief     Locks a recursive mutex.
 * @param[in] recursiveMutexHandle Handle of the recursive mutex to lock.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_recursiveMutex_lock(TlkOsMutexHandle_t recursiveMutexHandle);

/**
 * @brief     Unlocks a recursive mutex.
 * @param[in] recursiveMutexHandle Handle of the recursive mutex to unlock.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_recursiveMutex_unlock(TlkOsMutexHandle_t recursiveMutexHandle);