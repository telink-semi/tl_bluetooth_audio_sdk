/********************************************************************************************************
 * @file    tlkdbg_usb.c
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
#if (TLKDBG_CFG_UDB_LOG_ENABLE)
#include "tlkapi/tlkapi_common.h"
#include "../../../tlkdbg_cfg.h"
#include "../private/tlkdbg_async_stream_inner.h"
#include "tlklib/usb/udb/tlkusb_udb.h"
#include "tlklib/usb/tlkusb_stdio.h"

static uint16_t s_tlkdbg_usb_send_len = 0;

/**
 * @brief       Send log data via USB
 * @param[in]   item       - pointer to the stream item containing log data
 * @param[in]   total_len  - total length of data to send
 * @return      TLK_ENONE if success, -TLK_EAGAIN if need retry
 * @note        Handles USB log data sending with proper packet alignment
 */
static int32_t tlkdbg_usb_send_log(tlkdbg_async_stream_item_t *item, uint16_t total_len)
{
    if (total_len > TLKDBG_USBLOG_ITEM_SIZE) {
        return TLK_ENONE;
    }
    uint8_t *buffer = item->data + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE - 5;
    if (s_tlkdbg_usb_send_len < 5) {
        uint8_t head_info[5] = {0x82, 0x08, 0x22, 0, 0};
        tmemcpy(buffer, head_info, 5); // Must use this. 5 bytes may cause unaligned access, resulting in a bus fault/crash.
    }
    uint16_t remain_len      = total_len - s_tlkdbg_usb_send_len;
    uint16_t once_send_len   = remain_len >= 64 ? 64 : remain_len;
    int      usb_send_result = TLK_ENONE;
    if ((uint32_t)(buffer + s_tlkdbg_usb_send_len) % 4 != 0) {
        __attribute__((aligned(4))) uint8_t temp_buf[64]; //must 4 byte align,otherwise usb may crash.
        tmemcpy(temp_buf, buffer + s_tlkdbg_usb_send_len, once_send_len);
        usb_send_result = tlkusb_udb_sendData(temp_buf, once_send_len);
    } else {
        usb_send_result = tlkusb_udb_sendData(buffer + s_tlkdbg_usb_send_len, once_send_len);
    }
    if (TLK_ENONE != usb_send_result) {
        return -TLK_EAGAIN;
    }
    s_tlkdbg_usb_send_len += once_send_len;
    if (s_tlkdbg_usb_send_len >= total_len && total_len % 64 != 0) {
        return TLK_ENONE;
    }
    return -TLK_EAGAIN;
}

/**
 * @brief       Send status data via USB
 * @param[in]   item       - pointer to the stream item containing status data
 * @param[in]   total_len  - total length of data to send
 * @return      TLK_ENONE if success, -TLK_EAGAIN if need retry
 * @note        Handles USB status data sending with proper packet alignment
 */
_attribute_ram_code_sec_ static int32_t tlkdbg_usb_send_status(tlkdbg_async_stream_item_t *item, uint16_t total_len)
{
    uint8_t *buffer        = item->data;
    uint16_t remain_len    = total_len - s_tlkdbg_usb_send_len;
    uint16_t once_send_len = remain_len >= 64 ? 64 : remain_len;
    int      ret           = tlkusb_udb_sendData(buffer + s_tlkdbg_usb_send_len, once_send_len);
    if (TLK_ENONE != ret) {
        return -TLK_EAGAIN;
    }
    s_tlkdbg_usb_send_len += once_send_len;
    if (s_tlkdbg_usb_send_len >= total_len && total_len % 64 != 0) {
        return TLK_ENONE;
    }
    return -TLK_EAGAIN;
}

/**
 * @brief       Send data via USB debug interface
 * @param[in]   item  - pointer to the stream item to send
 * @return      TLK_ENONE if success, -TLK_EAGAIN if need retry
 * @note        Routes data to appropriate send function based on item type
 */
_attribute_ram_code_sec_ int32_t tlkdbg_usb_send(tlkdbg_async_stream_item_t *item)
{
    uint16_t total_len = 0;
    if (item->type == 0) {
        total_len = 5 + item->dataLen - TLK_DEBUG_LOG_CACHE_RESERVE_LEN;
    } else {
        total_len = item->dataLen;
    }

    if (tlkusb_udb_isSendBusy()) {
        return -TLK_EAGAIN;
    }
    if (s_tlkdbg_usb_send_len >= total_len) {
        if (total_len % 64 != 0) {
            return TLK_ENONE;
        }
        if (TLK_ENONE == tlkusb_udb_sendData(NULL, 0)) {
            return TLK_ENONE;
        }
        return -TLK_EAGAIN;
    }
    if (item->type == 0) {
        return tlkdbg_usb_send_log(item, total_len);
    } else {
        return tlkdbg_usb_send_status(item, total_len);
    }
}

/**
 * @brief       Drop/reset USB send state
 * @return      none
 * @note        Resets the USB send length counter to zero
 */
_attribute_ram_code_sec_ void tlkdbg_usb_drop(void)
{
    s_tlkdbg_usb_send_len = 0;
}

#endif // #if (TLKDBG_CFG_UDB_LOG_ENABLE)
