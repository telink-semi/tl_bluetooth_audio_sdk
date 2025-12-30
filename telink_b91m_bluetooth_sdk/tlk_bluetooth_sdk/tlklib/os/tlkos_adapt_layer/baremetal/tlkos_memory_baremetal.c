/********************************************************************************************************
 * @file    tlkos_memory_baremetal.c
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

#if TLKOS_CFG_BAREMETAL_ENABLE
#include "tlklib/os/tlkos_api/tlkos_define.h"
#include "tlklib/mem/tlkmem1.h"

_attribute_os_heap_sec_ __attribute__((aligned(4))) static uint8_t sTlkOsBareMetalMemBuffer[TLKOS_CFG_BAREMETAL_HEAP_SIZE] = {0};
static uint8_t                                                     sTlkOsBareMetalMemIsInit                                = 0;

/**
 * @brief     Allocates a block of memory.
 * @param[in] size Number of bytes to allocate.
 * @returns   Pointer to the allocated memory, or NULL if allocation fails.
 */
void *tlkos_malloc(uint32_t size)
{
    if (sTlkOsBareMetalMemIsInit == 0) {
        tlkmem1_init(sTlkOsBareMetalMemBuffer, TLKOS_CFG_BAREMETAL_HEAP_SIZE);
        sTlkOsBareMetalMemIsInit = 1;
    }
    void *ptr = tlkmem1_malloc(sTlkOsBareMetalMemBuffer, size);
    if (ptr != NULL) {
        return ptr;
    }
    static const TlkOsCrashInfo_t info = {
        .reason     = TLKOS_CRASH_REASON_CORE_MALLOC_FAIL,
        .detailInfo = "CoreMallocFailed",
    };
    tlkos_crash(&info);
    return NULL;
}

/**
 * @brief     Allocates a block of memory and initializes it to zero.
 * @param[in] size Number of bytes to allocate.
 * @returns   Pointer to the allocated and zero-initialized memory, or NULL if allocation fails.
 */
void *tlkos_calloc(uint32_t size)
{
    void *ptr = tlkos_malloc(size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
    }
    return ptr;
}

/**
 * @brief     Frees a previously allocated block of memory.
 * @param[in] ptr Pointer to the memory block to free.
 * @returns   None.
 */
void tlkos_free(void *ptr)
{
    tlkmem1_free(sTlkOsBareMetalMemBuffer, ptr);
}


#endif //TLKOS_CFG_BAREMETAL_ENABLE
