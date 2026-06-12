/********************************************************************************************************
 * @file    tlkdbg_hwu.c
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
#include "tlkapi/tlkapi_common.h"

#if (TLKDBG_CFG_HWU_LOG_ENABLE)

#include "tlkmw/sys_dev/tlkmw_sysdev.h"
#include "../../../tlkdbg_cfg.h"
#include "../private/tlkdbg_async_stream_inner.h"

#define TLKDBG_HWULOG_UART_PORT         UART0
#define TLKDBG_HWULOG_UART_BAUDRATE     1000000

#define TLKDBG_HWU_LOG_SEND_BUFFER_SIZE 128

#define TLKDEV_SERIAL0_QFIFO_NUM        4

static uint16_t s_tlkdbg_hwu_send_len = 0;

__attribute__((aligned(4))) static uint8_t sTlkDbgHwuLogSendBuff[TLKDEV_SERIAL0_QFIFO_NUM * (TLKDBG_HWU_LOG_SEND_BUFFER_SIZE + 4)];

/**
 * @brief       Initialize HWU debug module
 * @return      none
 * @note        Mounts and opens serial port for HWU logging
 */
void tlkdbg_hwu_init(void)
{
#if (TLK_DEV_SERIAL_ENABLE)
    tlkdrv_serial_mount(TLKDBG_HWULOG_UART_PORT, TLKDBG_HWULOG_UART_BAUDRATE, TLKDEV_SERIAL0_TX_PIN, 0, tlkhal_dma_malloc_ex(1), 0);
    tlkdrv_serial_setTxQFifo(TLKDBG_HWULOG_UART_PORT, TLKDEV_SERIAL0_QFIFO_NUM, TLKDBG_HWU_LOG_SEND_BUFFER_SIZE + 4, sTlkDbgHwuLogSendBuff,
                             TLKDEV_SERIAL0_QFIFO_NUM * (TLKDBG_HWU_LOG_SEND_BUFFER_SIZE + 4));
    tlkdrv_serial_open(TLKDBG_HWULOG_UART_PORT);
#endif
}

/**
 * @brief       Send data via HWU debug interface
 * @param[in]   item  - pointer to the stream item to send
 * @return      TLK_ENONE if success, -TLK_EAGAIN if need retry
 * @note        Handles HWU data sending with CRLF termination
 */
int32_t tlkdbg_hwu_send(tlkdbg_async_stream_item_t *item)
{
    (void)item;
#if (!TLK_DEV_SERIAL_ENABLE)
    return TLK_ENONE;
#else
    if (item->type != 0) {
        return TLK_ENONE;
    }
    uint16_t total_len = 2 + item->dataLen - TLK_DEBUG_LOG_CACHE_RESERVE_LEN;
    if (s_tlkdbg_hwu_send_len >= total_len) {
        return TLK_ENONE;
    }
    uint8_t *buffer                                             = item->data + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE;
    buffer[item->dataLen - TLK_DEBUG_LOG_CACHE_RESERVE_LEN]     = '\r';
    buffer[item->dataLen - TLK_DEBUG_LOG_CACHE_RESERVE_LEN + 1] = '\n'; //tail add '\r\n'

    uint16_t remain_len    = total_len - s_tlkdbg_hwu_send_len;
    uint16_t once_send_len = min(remain_len, TLKDBG_HWU_LOG_SEND_BUFFER_SIZE);
    tlkdrv_serial_send(TLKDBG_HWULOG_UART_PORT, buffer + s_tlkdbg_hwu_send_len, once_send_len);

    s_tlkdbg_hwu_send_len += once_send_len;

    if (s_tlkdbg_hwu_send_len >= total_len) {
        return TLK_ENONE;
    }
    return -TLK_EAGAIN;
#endif
}

/**
 * @brief       Drop/reset HWU send state
 * @return      none
 * @note        Resets the HWU send length counter to zero
 */
void tlkdbg_hwu_drop(void)
{
    s_tlkdbg_hwu_send_len = 0;
}


#endif // #if (TLKDBG_CFG_HWU_LOG_ENABLE)
