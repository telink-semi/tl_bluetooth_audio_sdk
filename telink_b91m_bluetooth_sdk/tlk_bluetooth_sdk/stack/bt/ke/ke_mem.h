/********************************************************************************************************
 * @file    ke_mem.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/


#ifndef _KE_MEM_H_
#define _KE_MEM_H_

//#include "rwip_config.h"     // IP configuration
#include <stdint.h>  // standard integer
#include <stdbool.h> // standard includes
#include "stack/bt/common/rwip_config.h"
/**
 ****************************************************************************************
 * @defgroup MEM Memory
 * @ingroup KERNEL
 * @brief Heap management module.
 *
 * This module implements heap management functions that allow initializing heap,
 * allocating and freeing memory.
 *
 * @{
 ****************************************************************************************
 */

// forward declarations
struct mblock_free;

/**
 ****************************************************************************************
 * @brief Heap initialization.
 *
 * This function performs the following operations:
 * - sanity checks
 * - check memory allocated is at least large enough to hold two block descriptors to hold
 * start and end
 * - initialize the first and last descriptors
 * - save heap into kernel environment variable.
 *
 * @param[in]     type      Memory type.
 * @param[in|out] heap      Heap pointer
 * @param[in]     heap_size Size of the heap
 *
 *
 ****************************************************************************************
 */
void ke_mem_init(uint8_t type, uint8_t *heap, uint16_t heap_size);

/**
 ****************************************************************************************
 * @brief Allocation of a block of memory.
 *
 * Allocates a memory block whose size is size; if no memory is available return NULL
 *
 * @param[in] size Size of the memory area that need to be allocated.
 * @param[in] type Type of memory block
 *
 * @return A pointer to the allocated memory area.
 *
 ****************************************************************************************
 */
void *ke_malloc(uint32_t size, uint8_t type);


/**
 ****************************************************************************************
 * @brief Check if it's possible to allocate a block of memory with a specific size.
 *
 * @param[in] size Size of the memory area that need to be allocated.
 * @param[in] type Type of memory block
 *
 * @return True if memory block can be allocated, False else.
 *
 ****************************************************************************************
 */
bool ke_check_malloc(uint32_t size, uint8_t type);

/**
 ****************************************************************************************
 * @brief Freeing of a block of memory.
 *
 * Free the memory area pointed by mem_ptr : mark the block as free and insert it in
 * the pool of free block.
 *
 * @param[in] mem_ptr Pointer to the memory area that need to be freed.
 *
 ****************************************************************************************
 */
void ke_free(void *mem_ptr);


/**
 ****************************************************************************************
 * @brief Check if current heap is empty or not (not used)
 *
 * @param[in] type Type of memory heap block
 *
 * @return true if heap not used, false else.
 ****************************************************************************************
 */
bool ke_mem_is_empty(uint8_t type);


/**
 ****************************************************************************************
 * @brief Check if current pointer is free or not
 *
 * @param[in] mem_ptr pointer to a memory block
 *
 * @return true if already free, false else.
 ****************************************************************************************
 */
bool ke_is_free(void *mem_ptr);

#if (KE_PROFILING)

/**
 ****************************************************************************************
 * @brief Retrieve memory usage of selected heap.
 *
 * @param[in] type Type of memory heap block
 *
 * @return current memory usage of current heap.
 ****************************************************************************************
 */
uint16_t ke_get_mem_usage(uint8_t type);


/**
 ****************************************************************************************
 * @brief Retrieve max memory usage of all heap.
 * This command also resets max measured value.
 *
 * @return max memory usage of all heap.
 ****************************************************************************************
 */
uint32_t ke_get_max_mem_usage(void);

#endif // (KE_PROFILING)

///@} MEM

#endif // _KE_MEM_H_
