/********************************************************************************************************
 * @file    tlkdbg_async_stream.h
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
 * @brief       Initialize asynchronous debug stream modules
 * @return      none.
 */
void tlkdbg_async_stream_init(void);

/**
 * @brief       Reset asynchronous debug stream modules
 * @return      none.
 */
void tlkdbg_async_stream_reset(void);

/**
 * @brief       Write data to asynchronous debug streams
 * @param[in]   type    - Stream type
 * @param[in]   pData   - Pointer to data
 * @param[in]   dataLen - Data length
 * @return      none.
 */
void tlkdbg_async_stream_write(uint8_t type, void *pData, uint16_t dataLen);

/**
 * @brief       Process asynchronous debug stream data
 * @return      none.
 */
void tlkdbg_async_stream_handler(void);

/**
 * @brief       Check if asynchronous debug stream is busy
 * @return      true if busy, false otherwise
 */
bool tlkdbg_async_stream_is_busy(void);

/**
 * @brief       Handle asynchronous debug stream download
 * @return      none.
 */
void tlkdbg_async_stream_download_handler(void);