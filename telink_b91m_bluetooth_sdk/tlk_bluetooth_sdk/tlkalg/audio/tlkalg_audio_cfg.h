/********************************************************************************************************
 * @file    tlkalg_audio_cfg.h
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
#pragma once

#define TLKALG_AUD_SCRATCH_BUF_INDEPENDENCE 0
#define TLKALG_AUD_SCRATCH_BUF_USE_STACK    1
#define TLKALG_AUD_SCRATCH_BUF_USE_HEAP     2

#ifndef TLKALG_AUD_SCRATCH_BUF_POS
#define TLKALG_AUD_SCRATCH_BUF_POS TLKALG_AUD_SCRATCH_BUF_INDEPENDENCE
#endif

#ifndef TLKALG_AUD_ALIGN_SIZE
#define TLKALG_AUD_ALIGN_SIZE 4
#endif

#define TLKALG_AUD_ALIGN(val) (((val) + (TLKALG_AUD_ALIGN_SIZE) - 1) / (TLKALG_AUD_ALIGN_SIZE) * (TLKALG_AUD_ALIGN_SIZE))

/**
 * @brief       Weak implementation of memory allocation function
 * @param[in]   size - Size of memory to allocate
 * @return      Pointer to allocated memory or NULL if allocation fails
 */
void *tlkalg_malloc_func(uint32_t size);

/**
 * @brief       Weak implementation of memory deallocation function
 * @param[in]   pbuff - Pointer to memory buffer to free
 * @return      None
 */
void tlkalg_free_func(void *pbuff);