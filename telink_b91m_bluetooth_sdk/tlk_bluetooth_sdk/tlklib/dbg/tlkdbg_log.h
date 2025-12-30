/********************************************************************************************************
 * @file    tlkdbg_log.h
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
 * @brief       Print formatted debug message
 * @param[in]   pSign  - Debug signature
 * @param[in]   pHead  - Log header
 * @param[in]   format - Format string
 * @param[in]   args   - Variable argument list
 * @return      none.
 */
void tlkdbg_log_print(char *pSign, char *pHead, const char *format, va_list args);

/**
 * @brief       Print array data in hexadecimal format
 * @param[in]   pSign   - Debug signature
 * @param[in]   pHead   - Log header
 * @param[in]   format  - Format string
 * @param[in]   pData   - Pointer to data array
 * @param[in]   dataLen - Data length
 * @return      none.
 */
void tlkdbg_log_array(char *pSign, char *pHead, const char *format, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Send binary data through debug interface
 * @param[in]   pStr    - String identifier
 * @param[in]   pData   - Pointer to data
 * @param[in]   dataLen - Data length
 * @return      none.
 */
void tlkdbg_log_sendData(char *pStr, uint8_t *pData, uint16_t dataLen);