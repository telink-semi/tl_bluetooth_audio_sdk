/********************************************************************************************************
 * @file    tlkdbg_log.c
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
#include "tlkdbg.h"
#include "tlkdbg_cfg.h"
#include "stream_module/tlkdbg_stream.h"
#include "tlklib/utility/tlklib_vsprintf.h"

#if (TLK_DEBUG_ENABLE)

static uint16_t sTlkDbgLogSerial = 0;

static void tlkdbg_mutex_lock(void)
{
    uint32_t mie_en = read_csr(NDS_MSTATUS) & FLD_MSTATUS_MIE;
    if (mie_en == 0) {
        return;
    }
    tlksys_mutex_lock(TLKSYS_MUTEX_LOG);
}

static void tlkdbg_mutex_unlock(void)
{
    uint32_t mie_en = read_csr(NDS_MSTATUS) & FLD_MSTATUS_MIE;
    if (mie_en == 0) {
        return;
    }
    tlksys_mutex_unlock(TLKSYS_MUTEX_LOG);
}

/**
 * @brief       Wake up the print thread to handle debug logs
 * @return      none.
 * @note        Sets event for system task to process debug logs
 */
static void tlkdbg_wakeUpPrintThread(void)
{
    uint32_t mie_en = read_csr(NDS_MSTATUS) & FLD_MSTATUS_MIE;
    if (mie_en == 0) {
        return;
    }
    if (tlkos_get_irqState() == TLKOS_IRQ_STATE_IN_IRQ) {
        tlksys_task_setEvtFromIsr(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_PRINT_LOG);
    } else {
        tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_PRINT_LOG);
    }
}

/**
 * @brief       Print formatted debug message
 * @param[in]   pSign  - Debug signature
 * @param[in]   pHead  - Log header
 * @param[in]   format - Format string
 * @param[in]   args   - Variable argument list
 * @return      none.
 * @note        Formats and outputs debug messages with serial number and signature
 */
_attribute_noinline_ void tlkdbg_log_print(char *pSign, char *pHead, const char *format, va_list args)
{
    if (format == NULL) {
        return;
    }
    pSign = pSign == NULL ? "" : pSign;
    pHead = pHead == NULL ? "" : pHead;
    tlkdbg_mutex_lock();

    // need 1 more byte used to save '\0' in snprintf;
    char     dataBuffer[TLK_DEBUG_LOG_CACHE_SIZE + 1 + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE + TLK_DEBUG_LOG_CACHE_TAIL_RESERVE];
    char    *pData  = dataBuffer + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE;
    uint16_t serial = sTlkDbgLogSerial++;

    int dataLen = tlk_snprintf(pData, TLK_DEBUG_LOG_CACHE_SIZE + 1, "[%04x]%s%s", serial, pSign, pHead);
    if (dataLen >= TLK_DEBUG_LOG_CACHE_SIZE || dataLen < 0) {
        tlkdbg_mutex_unlock();
        return;
    }
    int vsLen = tlk_vsnprintf(pData + dataLen, TLK_DEBUG_LOG_CACHE_SIZE - dataLen + 1, format, args);
    if (vsLen < 0) {
        tlkdbg_mutex_unlock();
        return;
    }
    dataLen += vsLen;
    if (dataLen >= TLK_DEBUG_LOG_CACHE_SIZE) {
        dataLen = TLK_DEBUG_LOG_CACHE_SIZE;
    }

    tlkdbg_stream_write(0, dataBuffer, dataLen + TLK_DEBUG_LOG_CACHE_RESERVE_LEN);

    tlkdbg_mutex_unlock();
    tlkdbg_wakeUpPrintThread();
}

/**
 * @brief       Print array data in hexadecimal format
 * @param[in]   pSign   - Debug signature
 * @param[in]   pHead   - Log header
 * @param[in]   format  - Format string
 * @param[in]   pData   - Pointer to data array
 * @param[in]   dataLen - Data length
 * @return      none.
 * @note        Outputs array data in hexadecimal format with serial number and signature
 */
_attribute_noinline_ void tlkdbg_log_array(char *pSign, char *pHead, const char *format, uint8_t *pData, uint16_t dataLen)
{
    pSign  = pSign == NULL ? "" : pSign;
    pHead  = pHead == NULL ? "" : pHead;
    format = format == NULL ? "" : format;
    tlkdbg_mutex_lock();

    // need 1 more byte used to save '\0' in snprintf;
    char     dataBuffer[TLK_DEBUG_LOG_CACHE_SIZE + 1 + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE + TLK_DEBUG_LOG_CACHE_TAIL_RESERVE];
    char    *pBuffer = dataBuffer + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE;
    uint16_t serial  = sTlkDbgLogSerial++;

    int len = tlk_snprintf(pBuffer, TLK_DEBUG_LOG_CACHE_SIZE + 1, "[%04x]%s%s%s(%d)", serial, pSign, pHead, format, dataLen);
    if (len < 0) {
        tlkdbg_mutex_unlock();
        return;
    }

    for (uint16_t index = 0; index < dataLen; index++) {
        if (len >= TLK_DEBUG_LOG_CACHE_SIZE) {
            len = TLK_DEBUG_LOG_CACHE_SIZE;
            break;
        }
        int hexLen = tlk_snprintf(pBuffer + len, TLK_DEBUG_LOG_CACHE_SIZE + 1 - len, "%02x ", pData[index]);
        if (hexLen < 0) {
            tlkdbg_mutex_unlock();
            return;
        }
        len += hexLen;
    }

    tlkdbg_stream_write(0, dataBuffer, len + TLK_DEBUG_LOG_CACHE_RESERVE_LEN);

    tlkdbg_mutex_unlock();
    tlkdbg_wakeUpPrintThread();
}

/**
 * @brief       Send binary data through debug interface
 * @param[in]   pStr    - String identifier
 * @param[in]   pData   - Pointer to data
 * @param[in]   dataLen - Data length
 * @return      none.
 * @note        Sends raw binary data with formatting for debugging purposes
 */
_attribute_noinline_ void tlkdbg_log_sendData(char *pStr, uint8_t *pData, uint16_t dataLen)
{
    static const uint8_t scTlkDbgHexTable[] = "0123456789abcdef";
    if (tlkos_get_irqState() == TLKOS_IRQ_STATE_NOT_IN_IRQ) {
        tlkdbg_mutex_lock();
    }

    uint16_t buffLen = 0;
    uint8_t  dataBuffer[TLK_DEBUG_LOG_CACHE_SIZE + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE + TLK_DEBUG_LOG_CACHE_TAIL_RESERVE];
    uint8_t *pDataBuffer = dataBuffer + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE;

    uint16_t serial = sTlkDbgLogSerial++;
    uint16_t strLen = strlen(pStr);

    pDataBuffer[buffLen++] = '[';
    pDataBuffer[buffLen++] = scTlkDbgHexTable[serial >> 12];
    pDataBuffer[buffLen++] = scTlkDbgHexTable[(serial >> 8) & 0x0F];
    pDataBuffer[buffLen++] = scTlkDbgHexTable[(serial >> 4) & 0x0F];
    pDataBuffer[buffLen++] = scTlkDbgHexTable[serial & 0x0F];
    pDataBuffer[buffLen++] = ']';

    if (buffLen + strLen > TLK_DEBUG_LOG_CACHE_SIZE) {
        strLen = TLK_DEBUG_LOG_CACHE_SIZE - buffLen;
    }
    if (strLen != 0) {
        tmemcpy(pDataBuffer + buffLen, pStr, strLen);
        buffLen += strLen;
    }
    if (dataLen != 0 && buffLen < TLK_DEBUG_LOG_CACHE_SIZE) {
        pDataBuffer[buffLen++] = ':';
        uint16_t tempVar       = tlkapi_arrayToStr(pData, dataLen, (char *)(pDataBuffer + buffLen), TLK_DEBUG_LOG_CACHE_SIZE - buffLen, ' ');
        buffLen += tempVar;
    }

    tlkdbg_stream_write(0, dataBuffer, buffLen + TLK_DEBUG_LOG_CACHE_RESERVE_LEN);

    if (tlkos_get_irqState() == TLKOS_IRQ_STATE_NOT_IN_IRQ) {
        tlkdbg_mutex_unlock();
    }
    tlkdbg_wakeUpPrintThread();
}

#endif // #if (TLK_DEBUG_ENABLE)