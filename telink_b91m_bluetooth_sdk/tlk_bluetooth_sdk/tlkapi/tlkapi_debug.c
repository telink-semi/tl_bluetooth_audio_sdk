/********************************************************************************************************
 * @file    tlkapi_debug.c
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
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "common/tstring.h"
#include "drivers.h"
#include "vendor/common/user_config.h"
#include "core/tlk_debug.h"
#include "tlkapi/tlkapi_common.h"
#include "tlkapi/tlkapi_qfifo.h"
#include "tlkapi/tlkapi_debug.h"
#include "tlkapi/tlkapi_string.h"
#include "tlklib/dbg/tlkdbg.h"
#include "tlklib/utility/tlklib_vsprintf.h"

/**
 * @brief       This function formats and stores output into a character array.
 * @param[out]  pOut     - the output buffer to store formatted string.
 * @param[in]   format   - the format string.
 * @param[in]   ...      - variable arguments according to the format string.
 * @returns     The number of characters that would have been written if n had been sufficiently large. 
 */
_attribute_noinline_ int tlkdbg_sprintf(char *pOut, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    return tlk_vsprintf(pOut, format, args);
}


#if !(TLK_DEBUG_ENABLE)

/**
 * @brief       This function handles debug events.
 * @param[in]   none.
 * @returns     none. 
 */
void tlkdbg_handler(void) {}

/**
 * @brief       This function outputs debug messages.
 * @param[in]   flags       - debug flags.
 * @param[in]   pSign       - sign string.
 * @param[in]   flagMask    - flag mask.
 * @param[in]   logHeadStr  - log head string.
 * @param[in]   format      - format string.
 * @param[in]   ...         - variable arguments according to the format string.
 * @returns     none. 
 */
void tlkdbg_message(uint32_t flags, char *pSign, uint32_t flagMask, char *logHeadStr, const char *format, ...)
{
    (void)flags;
    (void)pSign;
    (void)flagMask;
    (void)logHeadStr;
    (void)format;
}

/**
 * @brief       This function outputs debug arrays.
 * @param[in]   flags     - debug flags.
 * @param[in]   pSign     - sign string.
 * @param[in]   format    - format string.
 * @param[in]   pData     - pointer to the data array.
 * @param[in]   dataLen   - length of the data array.
 * @returns     none. 
 */
void tlkdbg_array(uint32_t flags, char *pSign, const char *format, uint8_t *pData, uint16_t dataLen)
{
    (void)flags;
    (void)pSign;
    (void)format;
    (void)pData;
    (void)dataLen;
}

/**
 * @brief       This function sends string data.
 * @param[in]   str       - string identifier.
 * @param[in]   pData     - pointer to the data to send.
 * @param[in]   data_len  - length of the data to send.
 * @returns     none. 
 */
_attribute_ram_code_sec_ void tlkdbg_send_str_data(char *str, uint8_t *pData, uint32_t data_len)
{
    (void)str;
    (void)pData;
    (void)data_len;
}

/**
 * @brief       This function sends string with u8 values.
 * @param[in]   str    - string identifier.
 * @param[in]   size   - number of u8 values.
 * @param[in]   ...    - variable u8 arguments.
 * @returns     none. 
 */
_attribute_ram_code_sec_ void tlkdbg_send_str_u8s(char *str, int size, ...)
{
    (void)str;
    (void)size;
}

/**
 * @brief       This function sends string with u32 values.
 * @param[in]   str    - string identifier.
 * @param[in]   size   - number of u32 values.
 * @param[in]   ...    - variable u32 arguments.
 * @returns     none. 
 */
_attribute_ram_code_sec_ void tlkdbg_send_str_u32s(char *str, int size, ...)
{
    (void)str;
    (void)size;
}

/**
 * @brief       This function prints formatted output.
 * @param[in]   format   - the format string.
 * @param[in]   ...      - variable arguments according to the format string.
 * @returns     none. 
 */
_attribute_ram_code_sec_ void tlk_printf(const char *format, ...)
{
    (void)format;
}


#endif
