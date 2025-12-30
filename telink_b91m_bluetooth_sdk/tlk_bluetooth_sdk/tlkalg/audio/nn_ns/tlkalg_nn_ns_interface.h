/********************************************************************************************************
 * @file    tlkalg_nn_ns_interface.h
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
#ifndef TLKALG_NN_NS_INTERFACE_H_
#define TLKALG_NN_NS_INTERFACE_H_

#include "tl_common.h"
#include "tlka_nn_ns_api.h"

extern uint8_t *g_nn_ns_buf_ptr;
extern NN_NS_PARA_STRU g_nn_ns_para;

/**
 * @brief       This function calculates the size required for the noise suppression algorithm buffer.
 * @param[in]   channel - The channel for which the size is being calculated.
 * @return      The size of the buffer in bytes.
 */
uint16_t tlkalg_nn_ns_get_size(uint8_t channel);

/**
 * @brief       This function initializes the noise suppression algorithm with a given buffer.
 * @param[in]   p_buff  - Pointer to the buffer used for the algorithm.
 * @param[in]   channel - The channel to initialize.
 * @return      The result of the initialization (0 for error, non-zero for success).
 */
int8_t   tlkalg_nn_ns_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief   This function deinitializes the noise suppression algorithm and frees the associated buffer.
 * @return      The result of the deinitialization (always 0 in this implementation).
 */
int8_t   tlkalg_nn_ns_deinit(void);

/**
 * @brief   This function sets the parameters for the noise suppression algorithm.
 * @param[in]   type    - Type of the parameter to set.
 * @param[in]   param   - Pointer to the parameter value to set.
 * @return      The result of the parameter setting (0 for error, non-zero for success).
 */
uint8_t  tlkalg_nn_ns_set_param(uint8_t type, void *param);

/**
 * @brief   This function processes a frame of audio data for noise suppression.
 * @param[in]   ps      - Pointer to the source audio data.
 * @param[in]   pd      - Pointer to the destination audio data.
 * @param[in]   len     - Length of the audio data frame.
 * @param[in]   width   - Width of the audio data.
 * @param[in]   channel - Channel number of the audio data.
 * @return      The result of the audio processing (0 for error, non-zero for success).
 */
int      tlkalg_nn_ns_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

//int tlka_nn_ns_get_version(void);
//int tlka_nn_ns_get_size();
//int tlka_nn_ns_init(NN_NS_STRU *st, NN_NS_PARA_STRU *para);
//int tlka_nn_ns_process_frame(NN_NS_STRU *st, short *buffer);
//int tlka_nn_ns_process_frame_24(NN_NS_STRU *st, int *buffer);
//int tlka_nn_ns_free(NN_NS_STRU *st);
//int tlka_nn_ns_set_property(NN_NS_STRU *st, int request, void *val);

#endif /* TLKALG_NN_NS_INTERFACE_H_ */
