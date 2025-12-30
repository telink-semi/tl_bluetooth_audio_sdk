/********************************************************************************************************
 * @file    tlkalg_nn_ns_vad_interface.h
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
#ifndef TLKALG_NN_NS_VAD_INTERFACE_H_
#define TLKALG_NN_NS_VAD_INTERFACE_H_

#include "tl_common.h"
#include "tlka_nn_ns_vad_api.h"

extern uint8_t *g_nn_ns_vad_buf_ptr;
extern NN_NS_PARA_STRU g_nn_ns_vad_para;

/**
 * @brief       This function calculates the required buffer size for the neural network noise suppression and voice activity detection algorithm.
 * @param[in]   channel - the audio channel, not used in this implementation.
 * @return      The size of the buffer required for the algorithm.
 */
uint16_t tlkalg_nn_ns_get_size(uint8_t channel);

/**
 * @brief       This function initializes the neural network noise suppression and voice activity detection algorithm.
 * @param[in]   p_buff  - pointer to the buffer allocated for the algorithm.
 * @param[in]   channel - the audio channel, not used in this implementation.
 * @return      The result of the initialization, 0 if successful, non-zero if an error occurred.
 */
int8_t   tlkalg_nn_ns_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function deinitializes the neural network noise suppression and voice activity detection algorithm.
 * @return      The result of the deinitialization, always 0.
 */
int8_t   tlkalg_nn_ns_deinit(void);

/**
 * @brief       This function sets parameters for the neural network noise suppression and voice activity detection algorithm.
 * @param[in]   type    - the type of parameter to set, not used in this implementation.
 * @param[in]   param   - pointer to the parameter value to set.
 * @return      The result of setting the parameter, 0 if successful, non-zero if an error occurred.
 */
uint8_t  tlkalg_nn_ns_set_param(uint8_t type, void *param);

/**
 * @brief       This function processes a frame of audio data using the neural network noise suppression and voice activity detection algorithm.
 * @param[in]   ps      - pointer to the source audio data.
 * @param[out]  pd      - pointer to the destination audio data.
 * @param[in]   len     - the length of the audio data, not used in this implementation.
 * @param[in]   width   - the width of the audio data, not used in this implementation.
 * @param[in]   channel - the audio channel, not used in this implementation.
 * @return      The result of processing the audio frame, 0 if successful, non-zero if an error occurred.
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
