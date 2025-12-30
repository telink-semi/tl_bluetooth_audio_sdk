/********************************************************************************************************
 * @file    tlkdbg_hpu.c
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
#include "tlkapi/tlkapi_common.h"
#include "../../../tlkdbg_cfg.h"
#include "../private/tlkdbg_async_stream_inner.h"

#if (TLKDBG_CFG_HPU_LOG_ENABLE)
// HPU - Hardware Protocol UART

static uint16_t s_tlkdbg_hpu_send_len = 0;

/**
 * @brief       Send data via Hardware Protocol UART
 * @param[in]   pData    - pointer to the data to send
 * @param[in]   dataLen  - length of data to send
 * @return      TLK_ENOSUPPORT as default (weak function)
 * @note        This is a weak function that should be overridden by actual implementation
 */
__attribute__((weak)) int32_t tlkdbg_hpulog_send(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    return -TLK_ENOSUPPORT;
}

/**
 * @brief       Send data via HPU debug interface
 * @param[in]   item  - pointer to the stream item to send
 * @return      TLK_ENONE if success, -TLK_EAGAIN if need retry
 * @note        Handles HPU data sending with newline termination
 */
int32_t tlkdbg_hpu_send(tlkdbg_async_stream_item_t *item)
{
    if (item->type != 0) {
        return TLK_ENONE;
    }
    uint16_t total_len = 1 + item->dataLen - TLK_DEBUG_LOG_CACHE_RESERVE_LEN;
    if (s_tlkdbg_hpu_send_len >= total_len) {
        return TLK_ENONE;
    }
    uint8_t *buffer                                         = item->data + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE;
    buffer[item->dataLen - TLK_DEBUG_LOG_CACHE_RESERVE_LEN] = '\n'; //tail add '\n'
    uint16_t remain_len                                     = total_len - s_tlkdbg_hpu_send_len;
    int      ret                                            = tlkdbg_hpulog_send(buffer + s_tlkdbg_hpu_send_len, remain_len);
    if (ret >= 0) {
        s_tlkdbg_hpu_send_len += ret;
    } else if (ret == -TLK_ENOSUPPORT) {
        s_tlkdbg_hpu_send_len += remain_len;
    }

    if (s_tlkdbg_hpu_send_len >= total_len) {
        return TLK_ENONE;
    }
    return -TLK_EAGAIN;
}

/**
 * @brief       Drop/reset HPU send state
 * @return      none
 * @note        Resets the HPU send length counter to zero
 */
void tlkdbg_hpu_drop(void)
{
    s_tlkdbg_hpu_send_len = 0;
}

#endif // #if (TLKDBG_CFG_HPU_LOG_ENABLE)
