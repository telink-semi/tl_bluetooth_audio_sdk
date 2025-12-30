/********************************************************************************************************
 * @file    queue.c
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
#include "tlkapi/tlkapi.h"

#if (TLK_MW_DSP_COMM_ENABLE)

#include "queue.h"

/**
 * @brief       Initialize queue
 * @param[in]   queue - Pointer to queue structure
 * @param[in]   buffer - Buffer pointer
 * @param[in]   size - Buffer size
 */
void queue_init(Queue_t *queue, uint32_t *buffer, uint16_t size)
{
    queue->buffer = buffer;
    queue->size   = size;
    queue->write  = 0;
    queue->read   = 0;
}

/**
 * @brief       Put buffer data into queue
 * @param[in]   queue - Pointer to queue structure
 * @param[in]   buffer - Source buffer pointer
 * @param[in]   size - Data size
 * @return      Size of data put into queue
 */
_attribute_ram_code_sec_noinline_ uint16_t queue_put_buffer(Queue_t *queue, const unsigned int *buffer, uint16_t size)
{
    if (queue_get_free_size(queue) < size) {
        return 0;
    }

    uint16_t bytes_to_the_end = queue->size - queue->write;

    if (bytes_to_the_end > size) {
        for (uint16_t i = 0; i < size; i++) {
            queue->buffer[queue->write + i] = buffer[i];
        }

        queue->write += size;
    } else {
        for (uint16_t i = 0; i < bytes_to_the_end; i++) {
            queue->buffer[queue->write + i] = buffer[i];
        }

        for (uint16_t i = 0; i < size - bytes_to_the_end; i++) {
            queue->buffer[i] = buffer[bytes_to_the_end + i];
        }

        queue->write = size - bytes_to_the_end;
    }

    return size;
}

/**
 * @brief       Get buffer data from queue
 * @param[in]   queue - Pointer to queue structure
 * @param[out]  buffer - Destination buffer pointer
 * @param[in]   size - Data size
 * @return      Size of data got from queue
 */
_attribute_ram_code_sec_noinline_ uint16_t queue_get_buffer(Queue_t *queue, unsigned int *buffer, uint16_t size)
{
    if (queue_get_available_data_size(queue) < size) {
        return 0;
    }

    uint16_t bytes_to_the_end = queue->size - queue->read;

    if (bytes_to_the_end > size) {
        for (uint16_t i = 0; i < size; i++) {
            buffer[i] = queue->buffer[queue->read + i];
        }

        queue->read += size;
    } else {
        for (uint16_t i = 0; i < bytes_to_the_end; i++) {
            buffer[i] = queue->buffer[queue->read + i];
        }

        for (uint16_t i = 0; i < size - bytes_to_the_end; i++) {
            buffer[bytes_to_the_end + i] = queue->buffer[i];
        }

        queue->read = size - bytes_to_the_end;
    }

    return size;
}

/**
 * @brief       Peek buffer data from queue without removing it
 * @param[in]   queue - Pointer to queue structure
 * @param[out]  buffer - Destination buffer pointer
 * @param[in]   size - Data size
 * @return      Size of data peeked from queue
 */
uint16_t queue_peek_buffer(Queue_t *queue, uint32_t *buffer, uint16_t size)
{
    if (queue_get_available_data_size(queue) < size) {
        return 0;
    }

    uint16_t bytes_to_the_end = queue->size - queue->read;

    if (bytes_to_the_end > size) {
        for (uint16_t i = 0; i < size; i++) {
            buffer[i] = queue->buffer[queue->read + i];
        }
    } else {
        for (uint16_t i = 0; i < bytes_to_the_end; i++) {
            buffer[i] = queue->buffer[queue->read + i];
        }

        for (uint16_t i = 0; i < size - bytes_to_the_end; i++) {
            buffer[bytes_to_the_end + i] = queue->buffer[i];
        }
    }
    return size;
}

/**
 * @brief       Get free size of queue
 * @param[in]   queue - Pointer to queue structure
 * @return      Free size of queue
 */
_attribute_ram_code_sec_noinline_ uint16_t queue_get_free_size(Queue_t *queue)
{
    return (queue->read > queue->write) ? (queue->read - queue->write) : (queue->size - queue->write + queue->read);
}

/**
 * @brief       Get available data size in queue
 * @param[in]   queue - Pointer to queue structure
 * @return      Available data size in queue
 */
_attribute_ram_code_sec_noinline_ uint16_t queue_get_available_data_size(Queue_t *queue)
{
    return (queue->write >= queue->read) ? (queue->write - queue->read) : (queue->size - queue->read + queue->write);
}
#endif
