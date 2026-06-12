/********************************************************************************************************
 * @file    tstring.h
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
#pragma GCC optimize "no-tree-loop-distribute-patterns"
#pragma once
#include "compiler.h"
#include "types.h"

/**
 * @brief      Fill memory with a constant byte value
 * @param[in]  d   - pointer to the destination memory area
 * @param[in]  c   - value to fill with
 * @param[in]  n   - number of bytes to fill
 * @return     Pointer to the destination memory area
 */
void *memset(void *d, int c, unsigned int n);

/**
 * @brief      Compare two memory areas
 * @param[in]  m1  - pointer to the first memory area
 * @param[in]  m2  - pointer to the second memory area
 * @param[in]  len - number of bytes to compare
 * @return     0 if equal, otherwise the difference between the first differing bytes
 */
int tmemcmp(const void *m1, const void *m2, u32 len);

/**
 * @brief      Compare two memory areas with 4-byte alignment
 * @param[in]  m1  - pointer to the first memory area
 * @param[in]  m2  - pointer to the second memory area
 * @param[in]  len - number of bytes to compare
 * @return     0 if equal, 1 if different
 */
int tmemcmp4(void *m1, void *m2, register unsigned int len);

/**
 * @brief      Fill memory with a constant byte value (alias for memset)
 * @param[in]  dest - pointer to the destination memory area
 * @param[in]  val  - value to fill with
 * @param[in]  len  - number of bytes to fill
 * @return     Pointer to the destination memory area
 */
void *tmemset(void *dest, int val, unsigned int len);

/**
 * @brief      Copy memory from source to destination
 * @param[out] out    - pointer to the destination memory area
 * @param[in]  in     - pointer to the source memory area
 * @param[in]  length - number of bytes to copy
 * @return     none
 */
void tmemcpy(void *out, const void *in, unsigned int length);

/**
 * @brief      Copy memory with 4-byte alignment assumption
 * @param[out] d     - pointer to the destination memory area
 * @param[in]  s     - pointer to the source memory area
 * @param[in]  length - number of bytes to copy
 * @return     none
 */
void tmemcpy4(void *d, void *s, unsigned int length);

/**
 * @brief      Alias for memset function
 */
#define tmemset memset

/**
 * @brief      Alias for tmemcmp function
 */
#define smemcmp tmemcmp

/**
 * @brief      Alias for tmemcmp4 function
 */
#define smemcmp4 tmemcmp4

/**
 * @brief      Alias for memset function
 */
#define smemset memset

/**
 * @brief      Alias for tmemcpy function
 */
#define smemcpy tmemcpy

/**
 * @brief      Alias for tmemcpy4 function
 */
#define smemcpy4 tmemcpy4

/**
 * @brief      Alias for memset function
 */
#define smemset4 memset