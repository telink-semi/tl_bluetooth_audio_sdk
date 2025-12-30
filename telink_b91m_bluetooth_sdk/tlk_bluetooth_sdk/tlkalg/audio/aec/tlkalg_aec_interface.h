/********************************************************************************************************
 * @file    tlkalg_aec_interface.h
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
#ifndef __TLKALG_AEC_INTERFACE_H__
#define __TLKALG_AEC_INTERFACE_H__

/**
 * @brief       This function calculates the total size required for AEC initialization.
 * @param[in]   channel - the channel number.
 * @return      The total size required for AEC initialization.
 */
uint16_t tlkalg_aec_get_size(uint8_t channel);

/**
 * @brief       This function initializes the AEC algorithm with the provided buffer.
 * @param[in]   p_buff  - the buffer to be used for AEC initialization.
 * @param[in]   channel - the channel number.
 * @return      0 on success, -1 on failure.
 */
int8_t tlkalg_aec_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function deinitializes the AEC algorithm.
 * @return      0 on success.
 */
int8_t tlkalg_aec_deinit(void);

/**
 * @brief       This function processes the audio frames using the AEC algorithm.
 * @param[in]   ps      - the source audio buffer.
 * @param[out]  pd      - the destination audio buffer.
 * @param[in]   len     - the length of the audio data.
 * @param[in]   width   - the width of the audio data.
 * @param[in]   channel - the channel number.
 * @return      1 on success, -1 on failure.
 */
int tlkalg_aec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
#endif