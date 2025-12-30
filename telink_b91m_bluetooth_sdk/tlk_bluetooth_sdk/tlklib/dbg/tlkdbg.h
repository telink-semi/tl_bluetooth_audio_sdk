/********************************************************************************************************
 * @file    tlkdbg.h
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
 * @brief       Initialize the debug module
 * @return      none.
 */
void tlkdbg_init(void);

/**
 * @brief       Handle debug events and process debug data
 * @return      none.
 */
void tlkdbg_handler(void);

/**
 * @brief       Get the next interval time for debug handler in microseconds
 * @return      Next interval time in microseconds
 */
uint32_t tlkdbg_handler_getNextIntvUs(void);

/**
 * @brief       Handle USB debug interrupt
 * @return      none.
 */
void tlk_udb_usb_handle_irq(void);

/**
 * @brief       Format string and output to debug console
 * @param[out]  pOut    - Output buffer
 * @param[in]   format  - Format string
 * @return      Formatted string length
 */
int tlkdbg_sprintf(char *pOut, const char *format, ...);

/**
 * @brief       Print debug message with specific flags and format
 * @param[in]   flags       - Debug flags
 * @param[in]   pSign       - Debug signature
 * @param[in]   flagMask    - Flag mask
 * @param[in]   logHeadStr  - Log header string
 * @param[in]   format      - Format string
 * @return      none.
 */
void tlkdbg_message(uint32_t flags, char *pSign, uint32_t flagMask, char *logHeadStr, const char *format, ...);

/**
 * @brief       Print array data in hexadecimal format
 * @param[in]   flags   - Debug flags
 * @param[in]   pSign   - Debug signature
 * @param[in]   format  - Format string
 * @param[in]   pData   - Pointer to data array
 * @param[in]   dataLen - Data length
 * @return      none.
 */
void tlkdbg_array(uint32_t flags, char *pSign, const char *format, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Send string data through debug interface
 * @param[in]   str      - String identifier
 * @param[in]   pData    - Pointer to data
 * @param[in]   data_len - Data length
 * @return      none.
 */
void tlkdbg_send_str_data(char *str, uint8_t *pData, uint32_t data_len);

/**
 * @brief       Send multiple uint8 values with string identifier
 * @param[in]   str  - String identifier
 * @param[in]   size - Number of uint8 values
 * @return      none.
 */
void tlkdbg_send_str_u8s(char *str, int size, ...);

/**
 * @brief       Send multiple uint32 values with string identifier
 * @param[in]   str  - String identifier
 * @param[in]   size - Number of uint32 values
 * @return      none.
 */
void tlkdbg_send_str_u32s(char *str, int size, ...);

/**
 * @brief       Print formatted string to debug console
 * @param[in]   format - Format string
 * @return      none.
 */
void tlk_printf(const char *format, ...);

/**
 * @brief       Send status information through debug interface
 * @param[in]   status   - Status value
 * @param[in]   buffNumb - Buffer number
 * @param[in]   pData    - Pointer to data
 * @param[in]   dataLen  - Data length
 * @return      none.
 */
void tlkdbg_send_status(uint8_t status, uint8_t buffNumb, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Reset debug stream
 * @return      none.
 */
void tlkdbg_stream_reset(void);

/**
 * @brief       Handle debug stream download
 * @return      none.
 */
void tlkdbg_stream_download_handler(void);