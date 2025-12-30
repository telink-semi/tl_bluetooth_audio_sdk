/********************************************************************************************************
 * @file    tlkmem1.c
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
#include "drivers.h"
#include "tlklib/mem/tlkmem1.h"


#define TLKMEM1_COMM_MIN_SIZE 512 // Bytes

#ifndef TLKMEM1_COMM_ALIGN_SIZE
#define TLKMEM1_COMM_ALIGN_SIZE 4
#endif

#define TLKMEM1_COMM_HEAD_SIZE 12
STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(TLKMEM1_COMM_HEAD_SIZE));

struct tlkmem1_unit_s
{
    struct tlkmem1_unit_s *prev;
    struct tlkmem1_unit_s *next;
    uint32_t               size : 31;
    uint32_t               used : 1;
};
typedef struct tlkmem1_unit_s tlkmem1_unit_t;

typedef struct
{
    uint32_t totalSize;
} tlkmem1_ctrl_t;

//In certain situations, it is necessary to turn off interrupts for protection
static __attribute__((__always_inline__)) inline void tlkmem1_enter_critical(void)
{
    tlksys_enter_critical();
}

static __attribute__((__always_inline__)) inline void tlkmem1_leave_critical(void)
{
    tlksys_leave_critical();
}

/**
 * @brief       Gets the memory management unit for the specified memory.
 * @param[in]   pCtrl - Memory management handle.
 * @param[in]   ptr - The first address of occupied memory.
 * @return      Memory management unit.
 */
_attribute_ram_code_sec_ static tlkmem1_unit_t *tlkmem1_get_unit(tlkmem1_ctrl_t *pCtrl, void *ptr)
{
    if (pCtrl == NULL || ptr == NULL) {
        return NULL;
    }
    tlkmem1_unit_t *pUnit = (tlkmem1_unit_t *)((uint32_t)ptr - sizeof(tlkmem1_unit_t));
    uint32_t        addr0 = (uint32_t)pUnit;
    uint32_t        addr1 = (uint32_t)pCtrl + sizeof(tlkmem1_ctrl_t);
    if (addr0 < addr1 || addr0 > addr1 + pCtrl->totalSize) {
        return NULL;
    }
    return pUnit;
}

/**
 * @brief       Initializes the memory manager.
 * @param[in]   pBuffer - Memory to be managed, must 4 byte align.
 * @param[in]   buffLen - The size of memory to be managed.
 * @return      Operating results. TLK_ENONE means success, others means failure.
 */
int tlkmem1_init(void *pBuffer, uint32_t buffLen)
{
    if (pBuffer == NULL || buffLen < TLKMEM1_COMM_MIN_SIZE || ((uint32_t)pBuffer & (TLKMEM1_COMM_ALIGN_SIZE - 1))) {
        return -TLK_EPARAM;
    }
    uint32_t        sAddr = (uint32_t)pBuffer;
    uint32_t        eAddr = ((uint32_t)pBuffer + buffLen) & (~(TLKMEM1_COMM_ALIGN_SIZE - 1));
    tlkmem1_ctrl_t *pCtrl = (tlkmem1_ctrl_t *)(sAddr);
    tmemset(pCtrl, 0, TLKMEM1_COMM_HEAD_SIZE);
    pCtrl->totalSize = (eAddr - sAddr) - sizeof(tlkmem1_ctrl_t);
    tlkmem1_clean(pCtrl);
    return TLK_ENONE;
}

/**
 * @brief       Uninitialization the memory manager.
 * @param[in]   mem - Memory pool head address.
 * @return      none.
 */
void tlkmem1_deinit(void *mem)
{
    tlkmem1_clean(mem);
}

/**
 * @brief       Reset the memory manager.
 * @param[in]   mem - Memory pool head address.
 * @return      none.
 */
void tlkmem1_clean(void *mem)
{
    if (mem == NULL) {
        return;
    }
    tlkmem1_enter_critical();
    tlkmem1_ctrl_t *pCtrl = (tlkmem1_ctrl_t *)mem;
    tlkmem1_unit_t *pUnit = (tlkmem1_unit_t *)((uint32_t)mem + sizeof(tlkmem1_ctrl_t));
    memset(pUnit, 0, pCtrl->totalSize);
    pUnit->size = pCtrl->totalSize - TLKMEM1_COMM_HEAD_SIZE;
    tlkmem1_leave_critical();
}

/**
 * @brief       Print the memory manager information.
 * @param[in]   mem - Memory pool head address.
 * @return      none.
 */
void tlkmem1_print(void *mem)
{
    uint32_t caddr = (uint32_t)mem + sizeof(tlkmem1_ctrl_t);
    uint16_t index = 0;

    if (mem == NULL) {
        return;
    }
    tlkmem1_unit_t *pUnit = (tlkmem1_unit_t *)((uint32_t)mem + sizeof(tlkmem1_ctrl_t));
    while (pUnit != NULL) {
        tlkapi_trace(0XFFFFFFFF, "[MEM]", "  index:{index-%d, addr-0x%x, prev-0x%x, next-0x%x,used:%d,size:%d}", index++, caddr, pUnit->prev, pUnit->next, pUnit->used,
                     pUnit->size);

        caddr = (uint32_t)(pUnit->next);
        pUnit = pUnit->next;
    }
}

/**
 * @brief       Allocates a segment of memory from the specified memory pool.
 * @param[in]   mem - Memory pool head address.
 * @param[in]   size - The size to allocate
 * @return      The first address of memory allocated, NULL means failure.
 * @note        Cannot be invoked across threads or processes.
 */
_attribute_ram_code_sec_ void *tlkmem1_malloc(void *mem, uint32_t size)
{
    uint8_t *pBuffer = NULL;
    if (mem == NULL || size == 0) {
        return NULL;
    }
    tlkmem1_enter_critical();
    size = ((size + (TLKMEM1_COMM_ALIGN_SIZE - 1)) & (~(TLKMEM1_COMM_ALIGN_SIZE - 1)));
    //TLKMEM1_COMM_ALIGN_SIZE = 4   4 byte algin
    tlkmem1_unit_t *pUnit = (tlkmem1_unit_t *)((uint32_t)mem + sizeof(tlkmem1_ctrl_t));
    ;
    while (pUnit != NULL) {
        if (pUnit->used == 0 && pUnit->size >= size) {
            break;
        }
        pUnit = pUnit->next;
    }
    if (pUnit != NULL) {
        if (pUnit->size >= size + TLKMEM1_COMM_HEAD_SIZE) { //have extra size to get a new unit.
            //not tail   extra do :
            tlkmem1_unit_t *pTemp = (tlkmem1_unit_t *)((uint32_t)pUnit + (size + TLKMEM1_COMM_HEAD_SIZE));
            pTemp->used           = 0;
            pTemp->size           = pUnit->size - (TLKMEM1_COMM_HEAD_SIZE + size);
            pTemp->next           = pUnit->next;
            pTemp->prev           = pUnit; //create a new unit(pTemp)
            if (pUnit->next != NULL) {
                pUnit->next->prev = pTemp;
            }
            pUnit->used = 1;
            pUnit->size = size;
            pUnit->next = pTemp;

        } else { //no extra size to get a new unit.
            pUnit->used = 1;
        }
        pBuffer = ((uint8_t *)pUnit) + TLKMEM1_COMM_HEAD_SIZE;
    }
    tlkmem1_leave_critical();
    return pBuffer;
}

/**
 * @brief       Allocates a segment of memory from the specified memory pool. And reset all variables in memory to zero.
 * @param[in]   mem - Memory pool head address.
 * @param[in]   size - The size to allocate.
 * @return      The first address of memory allocated, NULL means failure.
 * @note        Cannot be invoked across threads or processes.
 */
void *tlkmem1_calloc(void *mem, uint32_t size)
{
    uint8_t *pTemp = (uint8_t *)tlkmem1_malloc(mem, size);
    if (NULL == pTemp) {
        return NULL;
    }
    memset(pTemp, 0, size);
    return (void *)pTemp;
}

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
void *tlkmem1_realloc(void *mem, void *ptr, uint32_t size)
{
    if (mem == 0 || ptr == NULL || size == 0) {
        return NULL;
    }
    uint8_t *pTemp = (uint8_t *)tlkmem1_malloc(mem, size);
    if (pTemp != NULL) {
        tlkmem1_free(mem, ptr);
    }
    return pTemp;
}

/**
 * @brief       Return the occupied memory to the memory manager.
 * @param[in]   mem - Memory pool head address.
 * @param[in]   ptr - The first memory address that needs to be freed.
 * @return      Operating results. TLK_ENONE means success, others means failure.
 * @note        Cannot be invoked across threads or processes.
 */
_attribute_ram_code_sec_ int tlkmem1_free(void *mem, void *ptr)
{
    tlkmem1_unit_t *pUnit = tlkmem1_get_unit((tlkmem1_ctrl_t *)mem, ptr);
    if (pUnit == NULL || pUnit->used != 1) {
        return -TLK_EPARAM;
    }
    tlkmem1_enter_critical();
    pUnit->used = 0;
    // Check Next
    tlkmem1_unit_t *pNext = pUnit->next;
    if (pNext != NULL && pNext->used == 0) {
        pUnit->next = pNext->next;
        pUnit->size += TLKMEM1_COMM_HEAD_SIZE + pNext->size;
        if (pUnit->next != NULL) { //not tail
            pUnit->next->prev = pUnit;
        }
    }
    // Check Prev
    tlkmem1_unit_t *pPrev = pUnit->prev;
    if (pPrev != NULL && pPrev->used == 0) {
        pPrev->next = pUnit->next;
        pPrev->size += TLKMEM1_COMM_HEAD_SIZE + pUnit->size;
        if (pPrev->next != NULL) {
            pPrev->next->prev = pPrev;
        }
    }
    tlkmem1_leave_critical();
    return TLK_ENONE;
}
