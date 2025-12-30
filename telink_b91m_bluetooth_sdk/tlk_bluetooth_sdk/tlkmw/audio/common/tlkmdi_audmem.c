/********************************************************************************************************
 * @file    tlkmdi_audmem.c
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
#include "tlkmdi_audmem.h"
#include "tlklib/mem/tlkmem1.h"

#ifndef TLKMW_AUDIO_MEMPOOL_INDEPENDENT
#define TLKMW_AUDIO_MEMPOOL_INDEPENDENT 1
#endif

#ifndef TLKMW_AUDIO_MEMPOOL_RAM_SECTION 
#if (!MCU_CORE_TL752X_TEMP)
#define TLKMW_AUDIO_MEMPOOL_RAM_SECTION
#else
#define TLKMW_AUDIO_MEMPOOL_RAM_SECTION __attribute__((section(".iram_data")))
#endif
#endif

#if (TLK_MW_AUDIO_ENABLE)

#ifndef TLKMDI_AUDMEM_TOTAL_SIZE
#if MCU_CORE_TL752X_TEMP
    #define TLKMDI_AUDMEM_TOTAL_SIZE (256)
#else
    #define TLKMDI_AUDMEM_TOTAL_SIZE (64 * 1024)
#endif
#endif

TLKMW_AUDIO_MEMPOOL_RAM_SECTION
__attribute__((aligned(4))) static uint8_t sTlkMdiAudMemBuffer[TLKMDI_AUDMEM_TOTAL_SIZE] = {0};

/**
 * @brief       This function initializes the audio memory manager
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audmem_init(void)
{
#if TLKMW_AUDIO_MEMPOOL_INDEPENDENT
    tlkmem1_init(sTlkMdiAudMemBuffer, TLKMDI_AUDMEM_TOTAL_SIZE);
#endif
}

/**
 * @brief       This function allocates a segment of memory from the audio memory pool
 * @param[in]   size - The size to allocate
 * @return      The first address of memory allocated, NULL means failure
 */
void *tlkmdi_audmem_malloc(uint32_t size)
{
    return tlkmem1_malloc(sTlkMdiAudMemBuffer, size);
}

/**
 * @brief       This function allocates a segment of memory from the audio memory pool and resets all variables to zero
 * @param[in]   size - The size to allocate
 * @return      The first address of memory allocated, NULL means failure
 */
void *tlkmdi_audmem_calloc(uint32_t size)
{
    return tlkmem1_calloc(sTlkMdiAudMemBuffer, size);
}

/**
 * @brief       This function reallocates a segment of memory to store the contents of existing memory
 * @param[in]   ptr - The first memory address that needs to be reallocated
 * @param[in]   size - The size to allocate
 * @return      The first address of memory allocated, NULL means failure
 */
void *tlkmdi_audmem_realloc(void *ptr, uint32_t size)
{
    return tlkmem1_realloc(sTlkMdiAudMemBuffer, ptr, size);
}

/**
 * @brief       This function returns the occupied memory to the audio memory manager
 * @param[in]   ptr - The first memory address that needs to be freed
 * @return      None.
 */
void tlkmdi_audmem_free(void *ptr)
{
    tlkmem1_free(sTlkMdiAudMemBuffer, ptr);
}

/**
 * @brief       This function prints the audio memory manager information
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audmem_print(void)
{
    tlkmem1_print(sTlkMdiAudMemBuffer);
}

/**
 * @brief       This function is an overwrite function for allocating memory in algorithm module
 * @param[in]   size - The size to allocate
 * @return      The first address of memory allocated, NULL means failure
 */
void *tlkalg_malloc_func(uint32_t size)
{
#if TLKMW_AUDIO_MEMPOOL_INDEPENDENT
    return tlkmdi_audmem_calloc(size);
#else
    uint8_t *pTemp = (uint8_t *)tlkos_malloc(size);
    if (NULL == pTemp) {
        return NULL;
    }
    memset(pTemp, 0, size);
    return (void *)pTemp;
#endif
}

/**
 * @brief       This function is an overwrite function for freeing memory in algorithm module
 * @param[in]   pbuff - The first memory address that needs to be freed
 * @return      None.
 */
void tlkalg_free_func(void *pbuff)
{
#if TLKMW_AUDIO_MEMPOOL_INDEPENDENT
    tlkmdi_audmem_free(pbuff);
#else
    tlkos_free(pbuff);
#endif
}

#endif // #if (TLK_MW_AUDIO_ENABLE)
