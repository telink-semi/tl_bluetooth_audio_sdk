/********************************************************************************************************
 * @file    tlkdbg_async_stream_weak.c
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

/**
 * @brief       Initialize asynchronous debug stream modules
 * @return      none.
 */
__attribute__((weak)) void tlkdbg_async_stream_init(void) {}

/**
 * @brief       Reset asynchronous debug stream modules
 * @return      none.
 */
__attribute__((weak)) void tlkdbg_async_stream_reset(void) {}

/**
 * @brief       Write data to asynchronous debug streams
 * @param[in]   type    - Stream type
 * @param[in]   pData   - Pointer to data
 * @param[in]   dataLen - Data length
 * @return      none.
 */
__attribute__((weak)) void tlkdbg_async_stream_write(uint8_t type, void *pData, uint16_t dataLen)
{
    (void)type;
    (void)pData;
    (void)dataLen;
}

/**
 * @brief       Process asynchronous debug stream data
 * @return      none.
 */
__attribute__((weak)) void tlkdbg_async_stream_handler(void) {}
