/********************************************************************************************************
 * @file    queue.h
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
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "common/types.h"

typedef struct
{
    uint16_t  write;
    uint16_t  read;
    uint32_t *buffer;
    uint16_t  size;
} Queue_t;

/**
 * @brief       Initialize queue
 * @param[in]   queue - Pointer to queue structure
 * @param[in]   buffer - Buffer pointer
 * @param[in]   size - Buffer size
 */
void     queue_init(Queue_t *queue, uint32_t *buffer, uint16_t size);

/**
 * @brief       Put buffer data into queue
 * @param[in]   queue - Pointer to queue structure
 * @param[in]   buffer - Source buffer pointer
 * @param[in]   size - Data size
 * @return      Size of data put into queue
 */
uint16_t queue_put_buffer(Queue_t *queue, const unsigned int *buffer, uint16_t size);

/**
 * @brief       Get buffer data from queue
 * @param[in]   queue - Pointer to queue structure
 * @param[out]  buffer - Destination buffer pointer
 * @param[in]   size - Data size
 * @return      Size of data got from queue
 */
uint16_t queue_get_buffer(Queue_t *queue, unsigned int *buffer, uint16_t size);

/**
 * @brief       Peek buffer data from queue without removing it
 * @param[in]   queue - Pointer to queue structure
 * @param[out]  buffer - Destination buffer pointer
 * @param[in]   size - Data size
 * @return      Size of data peeked from queue
 */
uint16_t queue_peek_buffer(Queue_t *queue, uint32_t *buffer, uint16_t size);

/**
 * @brief       Get free size of queue
 * @param[in]   queue - Pointer to queue structure
 * @return      Free size of queue
 */
uint16_t queue_get_free_size(Queue_t *queue);

/**
 * @brief       Get available data size in queue
 * @param[in]   queue - Pointer to queue structure
 * @return      Available data size in queue
 */
uint16_t queue_get_available_data_size(Queue_t *queue);

#endif
