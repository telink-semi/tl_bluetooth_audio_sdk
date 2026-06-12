/********************************************************************************************************
 * @file    tlkalg_agc_interface.h
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
#ifndef _TLKALG_AGC_INTERFACE_H_
#define _TLKALG_AGC_INTERFACE_H_

#include "tlka_agc_api.h"

#define TLKALG_AGC_16K_10MS 1
#define TLKALG_AGC_48K_5MS  2

#ifndef TLKALG_AGC_TYPE
#define TLKALG_AGC_TYPE TLKALG_AGC_16K_10MS
#endif

extern AGC_Param agc_para;

/**
 * @brief   Calculates the required size for AGC algorithm buffer and scratch space.
 * @param[in]   channel - The audio channel number.
 * @return      Total size required for AGC algorithm buffer and scratch space.
 */
uint16_t tlkalg_agc_get_size(uint8_t channel);

/**
 * @brief   Initializes the AGC algorithm with the provided buffer and parameters.
 * @param[in]   p_buff  - Pointer to the buffer for AGC algorithm.
 * @param[in]   channel - The audio channel number.
 * @return      0 if initialization fails, non-zero otherwise.
 */
int8_t tlkalg_agc_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief   Deinitializes the AGC algorithm.
 * @return      0 on successful deinitialization.
 */
int8_t tlkalg_agc_deinit(void);

/**
 * @brief   Processes a frame of audio data using the AGC algorithm.
 * @param[in]   ps  - Pointer to the source audio buffer.
 * @param[out]  pd  - Pointer to the destination audio buffer.
 * @param[in]   len - Length of the audio data.
 * @param[in]   width - Width of the audio samples (e.g., 16-bit).
 * @param[in]   channel - The audio channel number.
 * @return      0 if processing fails, non-zero otherwise.
 */
int tlkalg_agc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

#endif
