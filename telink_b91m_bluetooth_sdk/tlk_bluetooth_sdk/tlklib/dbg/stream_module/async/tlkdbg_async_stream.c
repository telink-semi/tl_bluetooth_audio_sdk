/********************************************************************************************************
 * @file    tlkdbg_async_stream.c
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
#include "private/tlkdbg_async_stream_inner.h"
#include "../../tlkdbg_cfg.h"
#if (TLK_DEBUG_ENABLE && TLK_DEBUG_ASYNC_LOG_ENABLE)

static tlkdbg_async_stream_item_t *s_tlkdbg_astrm_head       = NULL;
static tlkdbg_async_stream_item_t *s_tlkdbg_astrm_tail       = NULL;
static uint32_t                    s_tlkdbg_astrm_first_tick = 0;

const tlkdbg_async_stream_t s_tlkdbg_async_stream_tab[] = {
#if TLKDBG_CFG_UDB_LOG_ENABLE
    {
        .drop        = tlkdbg_usb_drop,
        .send        = tlkdbg_usb_send,
        .timeoutms   = 5,
        .initDelayMs = 2500,
    },
#endif
#if TLKDBG_CFG_HPU_LOG_ENABLE
    {
        .drop        = tlkdbg_hpu_drop,
        .send        = tlkdbg_hpu_send,
        .timeoutms   = 10,
        .initDelayMs = 10,
    },
#endif
#if TLKDBG_CFG_HWU_LOG_ENABLE
    {
        .init        = tlkdbg_hwu_init,
        .drop        = tlkdbg_hwu_drop,
        .send        = tlkdbg_hwu_send,
        .timeoutms   = 10,
        .initDelayMs = 10,
    },
#endif
};

#define TLKDBG_ASYNC_STREAM_NUM         (sizeof(s_tlkdbg_async_stream_tab) / sizeof(tlkdbg_async_stream_t))
#define TLKDBG_ASYNC_STREAM_ALL_OK_MASK ((1 << TLKDBG_ASYNC_STREAM_NUM) - 1)

/**
 * @brief       Initialize asynchronous debug stream modules
 * @return      none.
 * @note        Calls initialization functions for each enabled stream type
 */
void tlkdbg_async_stream_init(void)
{
    for (uint8_t i = 0; i < TLKDBG_ASYNC_STREAM_NUM; i++) {
        if (s_tlkdbg_async_stream_tab[i].init == NULL) {
            continue;
        }
        s_tlkdbg_async_stream_tab[i].init();
    }
}

/**
 * @brief       Reset asynchronous debug stream modules
 * @return      none.
 * @note        Frees all pending items in the stream queue
 */
void tlkdbg_async_stream_reset(void)
{
    tlkdbg_async_stream_item_t *item = NULL;

    while (s_tlkdbg_astrm_head != NULL) {
        item                = s_tlkdbg_astrm_head;
        s_tlkdbg_astrm_head = item->next;
        tlkdbg_async_stream_free(item);
    }
    s_tlkdbg_astrm_head = NULL;
    s_tlkdbg_astrm_tail = NULL;
}

/**
 * @brief       Write data to asynchronous debug streams
 * @param[in]   type    - Stream type
 * @param[in]   pData   - Pointer to data
 * @param[in]   dataLen - Data length
 * @return      none.
 * @note        Allocates memory for the item and adds it to the queue
 */
_attribute_ram_code_sec_ void tlkdbg_async_stream_write(uint8_t type, void *pData, uint16_t dataLen)
{
    tlkdbg_async_stream_item_t *item = tlkdbg_async_stream_malloc(sizeof(tlkdbg_async_stream_item_t) + dataLen);
    if (item == NULL) {
        return;
    }
    item->tick    = 0;
    item->dataLen = dataLen;
    item->type    = type;
    item->okMask  = 0;
    item->next    = NULL;
    tmemcpy(item->data, pData, dataLen);

    uint32_t r = core_interrupt_disable();
    if (s_tlkdbg_astrm_tail == NULL) {
        s_tlkdbg_astrm_head = item;
        s_tlkdbg_astrm_tail = item;
    } else {
        s_tlkdbg_astrm_tail->next = item;
        s_tlkdbg_astrm_tail       = item;
    }
    core_restore_interrupt(r);
}

/**
 * @brief       Process asynchronous debug stream data
 * @return      none.
 * @note        Processes items in the queue, sending them through all enabled streams
 */
void tlkdbg_async_stream_handler(void)
{
    //log wait 1s for log module ready
    if (s_tlkdbg_astrm_first_tick != 0XFFFFFFFE) {
        if (s_tlkdbg_astrm_first_tick == 0) {
            s_tlkdbg_astrm_first_tick = clock_time() | 1;
            delay_us(1); // |1 need delay which may cause timeout check error.
        }
        uint32_t first_delay_ms = 0;
        for (uint8_t i = 0; i < TLKDBG_ASYNC_STREAM_NUM; i++) {
            first_delay_ms = max(first_delay_ms, s_tlkdbg_async_stream_tab[i].initDelayMs);
        }
        if (!clock_time_exceed(s_tlkdbg_astrm_first_tick, first_delay_ms * 1000)) {
            return;
        }
        s_tlkdbg_astrm_first_tick = 0XFFFFFFFE;
    }
    if (s_tlkdbg_astrm_head == NULL) {
        return;
    }
    tlkdbg_async_stream_item_t *item = s_tlkdbg_astrm_head;
    if (item->tick == 0) {
        item->tick = clock_time() | 1;
    }

    for (uint8_t i = 0; i < TLKDBG_ASYNC_STREAM_NUM; i++) {
        if (item->okMask & (1 << i)) {
            continue;
        }
        int32_t ret = s_tlkdbg_async_stream_tab[i].send(item);
        if (ret == TLK_ENONE || clock_time_exceed(item->tick, s_tlkdbg_async_stream_tab[i].timeoutms * 1000)) {
            item->okMask |= (1 << i);
        }
    }

    if (item->okMask != TLKDBG_ASYNC_STREAM_ALL_OK_MASK) {
        return; //wait all stream output send ok if have not timeout
    }
    uint32_t r          = core_interrupt_disable();
    s_tlkdbg_astrm_head = item->next;
    if (s_tlkdbg_astrm_head == NULL) {
        s_tlkdbg_astrm_tail = NULL;
    }
    core_restore_interrupt(r);
    tlkdbg_async_stream_free(item);
    for (uint8_t i = 0; i < TLKDBG_ASYNC_STREAM_NUM; i++) {
        s_tlkdbg_async_stream_tab[i].drop();
    }
}

/**
 * @brief       Check if asynchronous debug stream is busy
 * @return      true if busy, false otherwise
 * @note        Checks if there are items in the queue
 */
bool tlkdbg_async_stream_is_busy(void)
{
    return s_tlkdbg_astrm_head != NULL;
}

/**
 * @brief       Handle asynchronous debug stream download
 * @return      none.
 * @note        Processes items in the queue specifically for USB download
 */
_attribute_ram_code_sec_ void tlkdbg_async_stream_download_handler(void)
{
#if TLKDBG_CFG_UDB_LOG_ENABLE
    if (s_tlkdbg_astrm_head == NULL) {
        return;
    }
    tlkdbg_async_stream_item_t *item = s_tlkdbg_astrm_head;
    int                         ret  = tlkdbg_usb_send(item);

    if (ret != TLK_ENONE) {
        return;
    }
    s_tlkdbg_astrm_head = item->next;
    if (s_tlkdbg_astrm_head == NULL) {
        s_tlkdbg_astrm_tail = NULL;
    }
    tlkdbg_async_stream_free(item);
    tlkdbg_usb_drop();
#endif
}

#endif