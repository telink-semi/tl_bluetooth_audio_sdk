/********************************************************************************************************
 * @file    tlkos_semphr.h
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
 * @brief     Creates a binary semaphore.
 * @param[out] semphrHandle Pointer to store the created semaphore handle.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_semphr_createBinary(TlkOsSemphrHandle_t *semphrHandle);

/**
 * @brief     Creates a counting semaphore.
 * @param[out] semphrHandle Pointer to store the created semaphore handle.
 * @param[in] maxCnt Maximum count of the semaphore.
 * @param[in] initCnt Initial count of the semaphore.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_semphr_createCounting(TlkOsSemphrHandle_t *semphrHandle, uint32_t maxCnt, uint32_t initCnt);

/**
 * @brief     Destroys a semaphore.
 * @param[in] semphrHandle Handle of the semaphore to be destroyed.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_semphr_destroy(TlkOsSemphrHandle_t semphrHandle);

/**
 * @brief     Takes a semaphore, with a blocking time specified.
 * @param[in] semphrHandle Handle of the semaphore to take.
 * @param[in] blockTimeMs Blocking time in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_semphr_take(TlkOsSemphrHandle_t semphrHandle, uint32_t blockTimeMs);

/**
 * @brief     Gives a semaphore.
 * @param[in] semphrHandle Handle of the semaphore to give.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_semphr_give(TlkOsSemphrHandle_t semphrHandle);

/**
 * @brief     Gives a semaphore from an interrupt service routine.
 * @param[in] semphrHandle Handle of the semaphore to give.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_semphr_giveFromISR(TlkOsSemphrHandle_t semphrHandle);