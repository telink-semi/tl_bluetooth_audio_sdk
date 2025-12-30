/********************************************************************************************************
 * @file    tlkalg_cvsd_interface.h
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
#ifndef __TLKALG_CVSD_INTERFACE_H__
#define __TLKALG_CVSD_INTERFACE_H__

#include "common/types.h"
#include "tlkalg/audio/audio_alg_interface.h"
#include "tlkalg/audio/cvsd/tlka_cvsd_api.h"

#define BT_VOICE_FLAG_PACKET_LOSS 1

/**
 * @brief   Calculate the size required for the CVSD decoder buffer.
 * @param[in]   channel - Number of audio channels.
 * @return      Size of the decoder buffer in bytes, aligned to 4 bytes.
 */
uint16_t tlkalg_cvsd_dec_get_size(uint8_t channel);

/**
 * @brief   Initialize the CVSD decoder with the provided buffer.
 * @param[in]   p_buff  - Pointer to the buffer for decoder and PLC structures.
 * @param[in]   channel - Number of audio channels.
 * @return      0 on error, non-zero on success.
 */
int8_t   tlkalg_cvsd_dec_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief   Deinitialize the CVSD decoder.
 * @return      Always returns 0.
 */
int8_t   tlkalg_cvsd_dec_deinit(void);

/**
 * @brief   Process the CVSD decoding and packet loss concealment.
 * @param[in]   ps      - Pointer to the source buffer containing CVSD encoded data.
 * @param[out]  pd      - Pointer to the destination buffer for decoded PCM data.
 * @param[in]   len     - Length of the source buffer in bytes.
 * @param[in]   width   - Width of the audio data (not used).
 * @param[in]   channel - Number of audio channels (not used).
 * @return      Always returns 120.
 */
int      tlkalg_cvsd_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief   Calculate the size required for the CVSD encoder buffer.
 * @param[in]   channel - Number of audio channels.
 * @return      Size of the encoder buffer in bytes, aligned to 4 bytes.
 */
uint16_t tlkalg_cvsd_enc_get_size(uint8_t channel);

/**
 * @brief   Initialize the CVSD encoder with the provided buffer.
 * @param[in]   p_buff  - Pointer to the buffer for encoder structure.
 * @param[in]   channel - Number of audio channels.
 * @return      0 on error, non-zero on success.
 */
int8_t   tlkalg_cvsd_enc_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief   Deinitialize the CVSD encoder.
 * @return      Always returns 0.
 */
int8_t   tlkalg_cvsd_enc_deinit(void);

/**
 * @brief   Process the CVSD encoding.
 * @param[in]   ps      - Pointer to the source buffer containing PCM data.
 * @param[out]  pd      - Pointer to the destination buffer for CVSD encoded data.
 * @param[in]   len     - Length of the source buffer in bytes.
 * @param[in]   width   - Width of the audio data (not used).
 * @param[in]   channel - Number of audio channels (not used).
 * @return      TRUE on success.
 */
int      tlkalg_cvsd_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
#endif
