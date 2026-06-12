/********************************************************************************************************
 * @file    tlkdbg_async_stream_inner.h
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

struct tlkdbg_async_stream_item_s
{
    uint8_t                            type;
    uint8_t                            okMask;
    uint16_t                           dataLen;
    uint32_t                           tick;
    struct tlkdbg_async_stream_item_s *next;
    uint8_t                            data[0];
};

typedef struct tlkdbg_async_stream_item_s tlkdbg_async_stream_item_t;

typedef struct
{
    void (*init)(void);
    int32_t (*send)(tlkdbg_async_stream_item_t *item);
    void (*drop)(void);
    uint32_t timeoutms;
    uint32_t initDelayMs;
} tlkdbg_async_stream_t;

/**
 * @brief  Allocate memory from debug async stream memory pool
 * @param  size - Size of memory to allocate
 * @return Pointer to allocated memory, or NULL if allocation failed
 * @note   This function is designed to work in interrupt disabled context,
 *         it will temporarily disable interrupts during memory allocation
 *         to ensure thread safety, then restore the interrupt state
 */
void *tlkdbg_async_stream_malloc(uint32_t size);

/**
 * @brief       This function frees memory from debug async stream memory pool.
 * @param[in]   ptr     - the pointer to the memory to be freed.
 * @return      none.
 * @note        This function is designed to work in interrupt disabled context,
 *              it will temporarily disable interrupts during memory freeing
 *              to ensure thread safety, then restore the interrupt state.
 */
void tlkdbg_async_stream_free(void *ptr);

/**
 * @brief       Send data via USB debug interface
 * @param[in]   item  - pointer to the stream item to send
 * @return      TLK_ENONE if success, -TLK_EAGAIN if need retry
 * @note        Routes data to appropriate send function based on item type
 */
int32_t tlkdbg_usb_send(tlkdbg_async_stream_item_t *item);

/**
 * @brief       Drop/reset USB send state
 * @return      none
 * @note        Resets the USB send length counter to zero
 */
void tlkdbg_usb_drop(void);

/**
 * @brief       Send data via HPU debug interface
 * @param[in]   item  - pointer to the stream item to send
 * @return      TLK_ENONE if success, -TLK_EAGAIN if need retry
 * @note        Handles HPU data sending with newline termination
 */
int32_t tlkdbg_hpu_send(tlkdbg_async_stream_item_t *item);

/**
 * @brief       Drop/reset HPU send state
 * @return      none
 * @note        Resets the HPU send length counter to zero
 */
void tlkdbg_hpu_drop(void);

/**
 * @brief       Initialize HWU debug module
 * @return      none
 * @note        Mounts and opens serial port for HWU logging
 */
void tlkdbg_hwu_init(void);

/**
 * @brief       Send data via HWU debug interface
 * @param[in]   item  - pointer to the stream item to send
 * @return      TLK_ENONE if success, -TLK_EAGAIN if need retry
 * @note        Handles HWU data sending with CRLF termination
 */
int32_t tlkdbg_hwu_send(tlkdbg_async_stream_item_t *item);

/**
 * @brief       Drop/reset HWU send state
 * @return      none
 * @note        Resets the HWU send length counter to zero
 */
void tlkdbg_hwu_drop(void);
