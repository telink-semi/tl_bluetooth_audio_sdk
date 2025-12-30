/********************************************************************************************************
 * @file    tlkos_memory.h
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
 * @brief     Allocates a block of memory.
 * @param[in] size Number of bytes to allocate.
 * @returns   Pointer to the allocated memory, or NULL if allocation fails.
 */
void *tlkos_malloc(uint32_t size);

/**
 * @brief     Allocates a block of memory and initializes it to zero.
 * @param[in] size Number of bytes to allocate.
 * @returns   Pointer to the allocated and zero-initialized memory, or NULL if allocation fails.
 */
void *tlkos_calloc(uint32_t size);

/**
 * @brief     Frees a previously allocated block of memory.
 * @param[in] ptr Pointer to the memory block to free.
 * @returns   None.
 */
void tlkos_free(void *ptr);