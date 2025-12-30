/********************************************************************************************************
 * @file    tlkalg_ans_interface.h
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
#ifndef __TLKALG_ANS_INTERFACE_H__
#define __TLKALG_ANS_INTERFACE_H__

#include "tlka_ans_api.h"

///Parameter variable
extern W_NS_CFG_PARAM ans_para;
extern W_NS_CFG_PARAM ans_spk_para;

/**
 * @brief   Calculates the total buffer size required for the acoustic noise suppression (ANS) algorithm.
 * @param[in]   channel - The channel number (not used in this function).
 * @return      The total buffer size required for ANS.
 */
uint16_t tlkalg_ans_get_size(uint8_t channel);

/**
 * @brief   Initializes the acoustic noise suppression (ANS) algorithm.
 * @param[in]   p_buff  - Pointer to the buffer allocated for ANS.
 * @param[in]   channel - The channel number (not used in this function).
 * @return      1 if initialization is successful, 0 otherwise.
 */
int8_t tlkalg_ans_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief   Deinitializes the acoustic noise suppression (ANS) algorithm.
 * @return      0 to indicate completion of deinitialization.
 */
int8_t tlkalg_ans_deinit(void);

/**
 * @brief   Processes a frame of audio data for acoustic noise suppression (ANS).
 * @param[in]   ps      - Pointer to the source audio buffer.
 * @param[out]  pd      - Pointer to the destination audio buffer.
 * @param[in]   len     - Length of the audio data (not used in this function).
 * @param[in]   width   - Width of the audio data (not used in this function).
 * @param[in]   channel - The channel number (not used in this function).
 * @return      The result of the processing, typically 0 for success.
 */
int tlkalg_ans_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief   Initializes the acoustic noise suppression (ANS) algorithm for speaker audio.
 * @param[in]   p_buff  - Pointer to the buffer allocated for ANS speaker audio.
 * @param[in]   channel - The channel number (not used in this function).
 * @return      1 if initialization is successful, 0 otherwise.
 */
int8_t tlkalg_ans_spk_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief   Deinitializes the acoustic noise suppression (ANS) algorithm for speaker audio.
 * @return      0 to indicate completion of deinitialization.
 */
int8_t tlkalg_ans_spk_deinit(void);

/**
 * @brief   Processes a frame of speaker audio data for acoustic noise suppression (ANS).
 * @param[in]   ps      - Pointer to the source speaker audio buffer.
 * @param[out]  pd      - Pointer to the destination speaker audio buffer.
 * @param[in]   len     - Length of the audio data (not used in this function).
 * @param[in]   width   - Width of the audio data (not used in this function).
 * @param[in]   channel - The channel number (not used in this function).
 * @return      The result of the processing, typically 0 for success.
 */
int tlkalg_ans_spk_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

#endif
