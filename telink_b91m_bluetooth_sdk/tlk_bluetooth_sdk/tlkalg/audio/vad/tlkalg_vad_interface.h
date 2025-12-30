/********************************************************************************************************
 * @file    tlkalg_vad_interface.h
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
#ifndef _TLKALG_VAD_INTERFACE_H_
#define _TLKALG_VAD_INTERFACE_H_

#include "tlka_vad_api.h"

/**
 * @brief   Get the size required for VAD buffer.
 * @param[in]   channel - the channel number (unused in current implementation).
 * @return      The size of the VAD buffer.
 */
uint16_t tlkalg_vad_get_size(uint8_t channel);

/**
 * @brief   Initialize the VAD module.
 * @param[in]   p_buff - pointer to the buffer used by VAD.
 * @param[in]   channel - the channel number (unused in current implementation).
 * @return      Return value from tlka_vad_init.
 */
int8_t tlkalg_vad_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief   Deinitialize the VAD module.
 * @return      Always returns 0.
 */
int8_t tlkalg_vad_deinit(void);

/**
 * @brief   Process a frame of audio data using VAD.
 * @param[in]   ps - pointer to the source audio data.
 * @param[out]  pd - pointer to the destination buffer (unused in current implementation).
 * @param[in]   len - length of the audio data (unused in current implementation).
 * @param[in]   width - width of the audio data (unused in current implementation).
 * @param[in]   channel - the channel number (unused in current implementation).
 * @return      Return value from tlka_vad_process_frame.
 */
int tlkalg_vad_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

#endif
