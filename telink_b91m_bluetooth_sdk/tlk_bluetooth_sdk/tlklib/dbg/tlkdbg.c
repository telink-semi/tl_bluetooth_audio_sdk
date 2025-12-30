/********************************************************************************************************
 * @file    tlkdbg.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#if (TLK_DEBUG_ENABLE)
#include <stdio.h>
#include <stdarg.h>
#include "tlkdbg.h"
#include "tlkdbg_log.h"
#include "stream_module/tlkdbg_stream.h"

extern char *tlk_debug_getDbgSign(unsigned int flags);
extern bool  tlk_debug_logIsEnable(unsigned int flags, unsigned int dbgFlag);

__always_inline static inline bool tlkdbg_isInCriticalSection()
{
#if (MCU_CORE_N22)
    return false;
#else
    extern volatile unsigned char g_flash_is_in_critical_sec;
    if (g_flash_is_in_critical_sec) {
        return true;
    }
    return tlkos_get_irqState() == TLKOS_IRQ_STATE_IN_IRQ;
#endif
}

static uint8_t sTlkDbgMask = TLKAPI_DBG_FLAG_ALL;

/**
 * @brief       Initialize the debug module
 * @return      none.
 * @note        Initializes the debug stream modules
 */
void tlkdbg_init(void)
{
    tlkdbg_stream_init();
}

/**
 * @brief       Handle debug events and process debug data
 * @return      none.
 * @note        Processes pending debug logs and data transfers
 */
void tlkdbg_handler(void)
{
    tlkdbg_stream_handler();
}

/**
 * @brief       Get the next interval time for debug handler in microseconds
 * @return      Next interval time in microseconds
 * @note        Returns 0 if debug stream is busy, otherwise waits forever
 */
uint32_t tlkdbg_handler_getNextIntvUs(void)
{
    if (tlkdbg_stream_is_busy()) {
        return 0;
    }
    return TLKOS_WAIT_FOREVER;
}

/**
 * @brief       Print debug message with specific flags and format
 * @param[in]   flags     - Debug flags
 * @param[in]   pSign     - Debug signature
 * @param[in]   flagMask  - Flag mask
 * @param[in]   logHeadStr - Log header string
 * @param[in]   format    - Format string
 * @return      none.
 * @note        Outputs formatted debug messages based on enabled flags
 */
_attribute_noinline_ void tlkdbg_message(uint32_t flags, char *pSign, uint32_t flagMask, char *logHeadStr, const char *format, ...)
{
    if (tlkdbg_isInCriticalSection()) {
        return;
    }
    if ((sTlkDbgMask & flagMask) == 0) {
        return;
    }
    if (!tlk_debug_logIsEnable(flags, flagMask)) {
        return;
    }
    if (pSign == NULL) {
        pSign = tlk_debug_getDbgSign(flags);
    }

    va_list args;
    va_start(args, format);
    tlkdbg_log_print(pSign, logHeadStr, format, args);
    va_end(args);
}

/**
 * @brief       Print array data in hexadecimal format
 * @param[in]   flags   - Debug flags
 * @param[in]   pSign   - Debug signature
 * @param[in]   format  - Format string
 * @param[in]   pData   - Pointer to data array
 * @param[in]   dataLen - Data length
 * @return      none.
 * @note        Outputs array data in hexadecimal format for debugging purposes
 */
_attribute_noinline_ void tlkdbg_array(uint32_t flags, char *pSign, const char *format, uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    if (tlkdbg_isInCriticalSection()) {
        return;
    }

    if ((sTlkDbgMask & TLK_DEBUG_DBG_FLAG_ARRAY) == 0) {
        return;
    }
    if (!tlk_debug_logIsEnable(flags, TLK_DEBUG_DBG_FLAG_ARRAY)) {
        return;
    }
    if (pSign == NULL) {
        pSign = tlk_debug_getDbgSign(flags);
    }


    tlkdbg_log_array(pSign, TLKAPI_ARRAY_HEAD, format, pData, dataLen);
}

/**
 * @brief       Send binary data with string identifier
 * @param[in]   str      - String identifier
 * @param[in]   pData    - Pointer to data
 * @param[in]   data_len - Data length
 * @return      none.
 * @note        Sends raw binary data for debugging purposes
 */
_attribute_ram_code_sec_noinline_ void tlkdbg_send_str_data(char *str, uint8_t *pData, uint32_t data_len)
{
    (void)str;
    (void)pData;
    (void)data_len;
    if (tlkdbg_isInCriticalSection()) {
        return;
    }
    tlkdbg_log_sendData(str, pData, data_len);
}

#define SEND_U8S_MAX_NUM 16

/**
 * @brief       Send multiple uint8 values with string identifier
 * @param[in]   str  - String identifier
 * @param[in]   size - Number of uint8 values
 * @return      none.
 * @note        Sends up to 16 uint8 values with a string identifier
 */
_attribute_ram_code_sec_noinline_ void tlkdbg_send_str_u8s(char *str, int size, ...)
{
    u8 d[SEND_U8S_MAX_NUM];
    size = min(SEND_U8S_MAX_NUM, size);
    va_list args;
    va_start(args, size);
    for (int i = 0; i < size; i++) {
        d[i] = va_arg(args, unsigned int);
    }
    tlkdbg_send_str_data(str, d, size);
    va_end(args);
}

#define SEND_U32S_MAX_NUM 8

/**
 * @brief       Send multiple uint32 values with string identifier
 * @param[in]   str  - String identifier
 * @param[in]   size - Number of uint32 values
 * @return      none.
 * @note        Sends up to 8 uint32 values with a string identifier
 */
_attribute_ram_code_sec_noinline_ void tlkdbg_send_str_u32s(char *str, int size, ...)
{
    u32 d[SEND_U32S_MAX_NUM];
    size = min(SEND_U32S_MAX_NUM, size);
    va_list args;
    va_start(args, size);
    for (int i = 0; i < size; i++) {
        d[i] = va_arg(args, unsigned int);
    }
    tlkdbg_send_str_data(str, (uint8_t *)d, size * 4);
    va_end(args);
}

/**
 * @brief       Print formatted string to debug console
 * @param[in]   format - Format string
 * @return      none.
 * @note        Simple printf-like function for debug output
 */
_attribute_ram_code_sec_noinline_ void tlk_printf(const char *format, ...)
{
    va_list args;
    if (tlkdbg_isInCriticalSection()) {
        return;
    }

    va_start(args, format);
    tlkdbg_log_print(NULL, NULL, format, args);
    va_end(args);
}

/**
 * @brief       Send status information through debug interface
 * @param[in]   status   - Status value
 * @param[in]   buffNumb - Buffer number
 * @param[in]   pData    - Pointer to data
 * @param[in]   dataLen  - Data length
 * @return      none.
 * @note        Sends status data with header information
 */
_attribute_ram_code_sec_ void tlkdbg_send_status(uint8_t status, uint8_t buffNumb, uint8_t *pData, uint16_t dataLen)
{
    uint8_t sTlkDbgUsbLogCache[262 + 2];
    if (dataLen > 262) {
        dataLen = 262;
    }
    sTlkDbgUsbLogCache[0] = status;
    sTlkDbgUsbLogCache[1] = buffNumb;
    if (dataLen != 0) {
        tmemcpy(sTlkDbgUsbLogCache + 2, pData, dataLen);
    }
    tlkdbg_stream_status_write(sTlkDbgUsbLogCache, dataLen + 2);
}

#endif // #if (TLK_DEBUG_ENABLE)