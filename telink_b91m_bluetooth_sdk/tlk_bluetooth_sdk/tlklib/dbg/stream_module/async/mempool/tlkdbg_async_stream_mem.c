/********************************************************************************************************
 * @file    tlkdbg_async_stream_mem.c
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
#include "tlklib/mem/tlkmem1.h"
#include "../../../tlkdbg_cfg.h"

#if (TLK_DEBUG_ENABLE && TLK_DEBUG_ASYNC_LOG_ENABLE)

TLK_DEBUG_LOG_MEM_POOL_SECTION
__attribute__((aligned(4))) static uint8_t tlkdbg_async_stream_mempool[TLK_DEBUG_LOG_MEM_POOL_SIZE] = {0};
static uint8_t                             tlkdbg_async_stream_mempool_is_init                      = 0;

/**
 * @brief  Allocate memory from debug async stream memory pool
 * @param  size - Size of memory to allocate
 * @return Pointer to allocated memory, or NULL if allocation failed
 * @note   This function is designed to work in interrupt disabled context,
 *         it will temporarily disable interrupts during memory allocation
 *         to ensure thread safety, then restore the interrupt state
 */
_attribute_ram_code_sec_ void *tlkdbg_async_stream_malloc(uint32_t size)
{
    if (tlkdbg_async_stream_mempool_is_init == 0) {
        tlkmem1_init(tlkdbg_async_stream_mempool, TLK_DEBUG_LOG_MEM_POOL_SIZE);
        tlkdbg_async_stream_mempool_is_init = 1;
    }
    uint32_t r   = core_interrupt_disable();
    void    *ptr = tlkmem1_malloc(tlkdbg_async_stream_mempool, size);
    core_restore_interrupt(r);
    return ptr;
}

/**
 * @brief       This function frees memory from debug async stream memory pool.
 * @param[in]   ptr     - the pointer to the memory to be freed.
 * @return      none.
 * @note        This function is designed to work in interrupt disabled context,
 *              it will temporarily disable interrupts during memory freeing
 *              to ensure thread safety, then restore the interrupt state.
 */
_attribute_ram_code_sec_ void tlkdbg_async_stream_free(void *ptr)
{
    uint32_t r = core_interrupt_disable();
    tlkmem1_free(tlkdbg_async_stream_mempool, ptr);
    core_restore_interrupt(r);
}

#endif