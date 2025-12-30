/********************************************************************************************************
 * @file    tlkalg_opus_interface.h
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
#ifndef _TLKALG_OPUS_INTERFACE_H_
#define _TLKALG_OPUS_INTERFACE_H_

#if TLKALG_OPUS_ENC_ENABLE

/**
 * @brief       Calculate the required size for OPUS encoder buffer and scratch buffer.
 * @param[in]   channel - the number of channels.
 * @return      Total size required for encoder buffer and scratch buffer.
 */
uint16_t tlkalg_opus_enc_get_size(uint8_t channel);

/**
 * @brief       Initialize the OPUS encoder with the given buffer and channel.
 * @param[in]   p_buff  - the start address of the buffer for encoder.
 * @param[in]   channel - the number of channels.
 * @return      0 on success, non-zero on failure.
 */
int8_t tlkalg_opus_enc_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the OPUS encoder.
 * @return      0 on success, non-zero on failure.
 */
int8_t tlkalg_opus_enc_deinit(void);

/**
 * @brief       Process audio data using the OPUS encoder.
 * @param[in]   ps    - the start address of the source audio data buffer.
 * @param[out]  pd    - the start address of the destination buffer for encoded data.
 * @param[in]   len   - the length of the source audio data.
 * @param[in]   width - the width of the audio data.
 * @param[in]   channel- the number of channels.
 * @return      Length of the encoded data.
 */
int tlkalg_opus_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
#endif
#if TLKALG_OPUS_DEC_ENABLE

/**
 * @brief       Calculate the required size for OPUS decoder buffer and scratch buffer.
 * @param[in]   channel - the number of channels.
 * @return      Total size required for decoder buffer and scratch buffer.
 */
uint16_t tlkalg_opus_dec_get_size(uint8_t channel);

/**
 * @brief       Initialize the OPUS decoder with the given buffer and channel.
 * @param[in]   p_buff  - the start address of the buffer for decoder.
 * @param[in]   channel - the number of channels.
 * @return      0 on success, non-zero on failure.
 */
int8_t tlkalg_opus_dec_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize the OPUS decoder.
 * @return      0 on success, non-zero on failure.
 */
int8_t tlkalg_opus_dec_deinit(void);

/**
 * @brief       Process encoded audio data using the OPUS decoder.
 * @param[in]   ps    - the start address of the source encoded audio data buffer.
 * @param[out]  pd    - the start address of the destination buffer for decoded audio data.
 * @param[in]   len   - the length of the source encoded audio data.
 * @param[in]   width - the width of the audio data.
 * @param[in]   channel- the number of channels.
 * @return      0 on success, non-zero on failure.
 */
int tlkalg_opus_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
#endif

#endif