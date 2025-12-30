/********************************************************************************************************
 * @file    tlkstk_mem.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#include "tlklib/mem/tlkmem1.h"

#ifndef TLKSTK_BT_HOST_MEM_POOL_SIZE
#define TLKSTK_BT_HOST_MEM_POOL_SIZE             (13 * 1024)
#endif

#if (!MCU_CORE_TL752X_TEMP)
__attribute__((aligned(4))) static  uint8_t sTlkBtMemBuffer[TLKSTK_BT_HOST_MEM_POOL_SIZE] = {0};
#else
__attribute__((aligned(4))) static _attribute_iram_data_ uint8_t sTlkBtMemBuffer[TLKSTK_BT_HOST_MEM_POOL_SIZE] = {0};
#endif
int tlkstk_mem_init(void)
{
    return tlkmem1_init(sTlkBtMemBuffer, TLKSTK_BT_HOST_MEM_POOL_SIZE);
}

void *tlkstk_malloc(uint32_t size)
{
    return tlkmem1_malloc(sTlkBtMemBuffer, size);
}

void *tlkstk_calloc(uint32_t size)
{
    return tlkmem1_calloc(sTlkBtMemBuffer, size);
}

void *tlkstk_realloc(void *ptr, uint32_t size)
{
    return tlkmem1_realloc(sTlkBtMemBuffer, ptr, size);
}

void tlkstk_free(void *ptr)
{
    tlkmem1_free(sTlkBtMemBuffer, ptr);
}

void tlkstk_print(void)
{
    tlkmem1_print(sTlkBtMemBuffer);
}
