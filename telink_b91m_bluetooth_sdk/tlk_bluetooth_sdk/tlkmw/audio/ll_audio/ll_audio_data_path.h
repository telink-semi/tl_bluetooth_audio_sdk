/********************************************************************************************************
 * @file    ll_audio_data_path.h
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
#ifndef __LL_AUDIO_DATA_PATH_H__
#define __LL_AUDIO_DATA_PATH_H__

#include "common/types.h"

/**
 * @brief  async stack post received audio data to queue
 *
 * @param[in]  p_master: async data packet
 *
 * @returns none
 */
//uint8_t ll_audio_post_audio_data_to_async_queue(async_tws_master_packet_t *p_master);

/**
 * @brief  low latency decoder task get async audio data from queue
 *
 * @param[in]  wptr: async audio data wptr
 *
 * @returns audio data ptr
 */
uint8_t *ll_audio_get_async_queue_wptr(uint8_t wptr);

/**
 * @brief  low latency mic task post async audio data to fifo
 *
 * @param[in]  idx: mic id
 * @param[in]  p_enc: encoded audio data
 *
 * @returns none
 */
void ll_audio_post_audio_data_to_async_fifo(uint8_t idx, uint8_t *p_enc);

#endif
