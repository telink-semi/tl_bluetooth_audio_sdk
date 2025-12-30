/********************************************************************************************************
 * @file    tlkmdi_audmem.h
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
 * @brief       This function initializes the audio memory manager
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audmem_init(void);

/**
 * @brief       This function allocates a segment of memory from the audio memory pool
 * @param[in]   size - The size to allocate
 * @return      The first address of memory allocated, NULL means failure
 */
void *tlkmdi_audmem_malloc(uint32_t size);

/**
 * @brief       This function allocates a segment of memory from the audio memory pool and resets all variables to zero
 * @param[in]   size - The size to allocate
 * @return      The first address of memory allocated, NULL means failure
 */
void *tlkmdi_audmem_calloc(uint32_t size);

/**
 * @brief       This function reallocates a segment of memory to store the contents of existing memory
 * @param[in]   ptr - The first memory address that needs to be reallocated
 * @param[in]   size - The size to allocate
 * @return      The first address of memory allocated, NULL means failure
 */
void *tlkmdi_audmem_realloc(void *ptr, uint32_t size);

/**
 * @brief       This function returns the occupied memory to the audio memory manager
 * @param[in]   ptr - The first memory address that needs to be freed
 * @return      None.
 */
void tlkmdi_audmem_free(void *ptr);

/**
 * @brief       This function prints the audio memory manager information
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audmem_print(void);

