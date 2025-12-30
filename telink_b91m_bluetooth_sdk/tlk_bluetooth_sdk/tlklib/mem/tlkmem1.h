/********************************************************************************************************
 * @file    tlkmem1.h
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
 * @brief       Initializes the memory manager.
 * @param[in]   pBuffer - Memory to be managed, must 4 byte align.
 * @param[in]   buffLen - The size of memory to be managed.
 * @return      Operating results. TLK_ENONE means success, others means failure.
 */
int tlkmem1_init(void *pBuffer, uint32_t buffLen);

/**
 * @brief       Uninitialization the memory manager.
 * @param[in]   mem - Memory pool head address.
 * @return      none.
 */
void tlkmem1_deinit(void *mem);

/**
 * @brief       Reset the memory manager.
 * @param[in]   mem - Memory pool head address.
 * @return      none.
 */
void tlkmem1_clean(void *mem);

/**
 * @brief       Print the memory manager information.
 * @param[in]   mem - Memory pool head address.
 * @return      none.
 */
void tlkmem1_print(void *mem);

/**
 * @brief       Allocates a segment of memory from the specified memory pool.
 * @param[in]   mem - Memory pool head address.
 * @param[in]   size - The size to allocate
 * @return      The first address of memory allocated, NULL means failure.
 * @note        Cannot be invoked across threads or processes.
 */
void *tlkmem1_malloc(void *mem, uint32_t size);

/**
 * @brief       Allocates a segment of memory from the specified memory pool. And reset all variables in memory to zero.
 * @param[in]   mem - Memory pool head address.
 * @param[in]   size - The size to allocate.
 * @return      The first address of memory allocated, NULL means failure.
 * @note        Cannot be invoked across threads or processes.
 */
void *tlkmem1_calloc(void *mem, uint32_t size);

/**
 * @brief       A segment of memory that is used to store the contents of existing memory.
 * @param[in]   mem - Memory pool head address.
 * @param[in]   ptr - The first memory address that needs to be reallocated.
 * @param[in]   size - The size to allocate.
 * @return      The first address of memory allocated, NULL means failure.
 * @note        
 *              1. Cannot be invoked across threads or processes.
 *              2. If allocation fails, the original memory address remains unchanged.
 *              3. If the allocation is successful, the existing memory is freed.
 *              4. If the allocation is successful, the contents of existing memory are copied into the new memory.
 */
void *tlkmem1_realloc(void *mem, void *ptr, uint32_t size);

/**
 * @brief       Return the occupied memory to the memory manager.
 * @param[in]   mem - Memory pool head address.
 * @param[in]   ptr - The first memory address that needs to be freed.
 * @return      Operating results. TLK_ENONE means success, others means failure.
 * @note        Cannot be invoked across threads or processes.
 */
int tlkmem1_free(void *mem, void *ptr);


