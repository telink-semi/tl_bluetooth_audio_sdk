/********************************************************************************************************
 * @file    tlkalg_ppm_interface.h
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
#ifndef TLKALG_PPM_H
#define TLKALG_PPM_H
#include "common/types.h"
#include "tl_common.h"
#include "tlkalg/audio/audio_alg_interface.h"
#if (TLK_ALG_PPM_ENABLE && (MCU_CORE_TYPE == MCU_CORE_TL721X))
#include "tlka_ppm_multi_ch_api.h"
#else
#include "tlka_ppm_asrc_api.h"
#endif

/**
 * @brief       Get the size required for the speaker PPM buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_spk_get_size(uint8_t channel);

/**
 * @brief       Set the PPM parameter for the speaker
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_spk_param_set(uint8_t type, void *param);

/**
 * @brief       Initialize the speaker PPM module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the speaker PPM module
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_deinit(void);

/**
 * @brief       Process the speaker PPM data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_spk_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Get the size required for the microphone PPM buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_mic_get_size(uint8_t channel);

/**
 * @brief       Set the PPM parameter for the microphone
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_mic_param_set(uint8_t type, void *param);

/**
 * @brief       Initialize the microphone PPM module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_mic_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the microphone PPM module
 * @return      0 on success
 */
int8_t tlkalg_ppm_mic_deinit(void);

/**
 * @brief       Process the microphone PPM data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_mic_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

#endif
