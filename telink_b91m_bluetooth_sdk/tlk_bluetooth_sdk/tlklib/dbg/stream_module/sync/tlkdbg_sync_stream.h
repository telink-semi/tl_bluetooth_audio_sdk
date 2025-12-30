/********************************************************************************************************
 * @file    tlkdbg_sync_stream.h
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
 * @brief       Initialize synchronous debug stream module
 * @param       none.
 * @return      none.
 */
void tlkdbg_sync_stream_init(void);

/**
 * @brief       Write data to synchronous debug stream
 * @param[in]   type    - data type to write
 * @param[in]   pData   - pointer to the data to write
 * @param[in]   dataLen - length of the data to write
 * @return      none.
 */
void tlkdbg_sync_stream_write(uint8_t type, void *pData, uint16_t dataLen);
