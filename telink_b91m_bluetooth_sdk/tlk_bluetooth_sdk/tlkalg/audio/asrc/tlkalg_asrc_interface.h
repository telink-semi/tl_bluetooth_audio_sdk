/********************************************************************************************************
 * @file    tlkalg_asrc_interface.h
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
#ifndef __TLKALG_ASRC_INTERFACE_H__
#define __TLKALG_ASRC_INTERFACE_H__

#include "tl_common.h"
#include "tlkalg/audio/audio_alg_interface.h"

/**
 * @brief       This function calculates the required buffer size for ASRC 48kHz to 16kHz conversion.
 * @param[in]   channel    - the number of channels.
 * @return      The required buffer size or 0 if the channel is invalid.
 */
uint16_t tlkalg_asrc_48to16_get_size(uint8_t channel);

/**
 * @brief       This function initializes ASRC for 48kHz to 16kHz conversion.
 * @param[in]   p_buff    - the buffer pointer.
 * @param[in]   channel   - the number of channels.
 * @return      The result of the initialization, 0 on failure.
 */
int8_t   tlkalg_asrc_48to16_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function deinitializes ASRC for 48kHz to 16kHz conversion.
 * @return      Always returns 0.
 */
int8_t   tlkalg_asrc_48to16_deinit(void);

/**
 * @brief       This function processes audio data for ASRC 48kHz to 16kHz conversion.
 * @param[in]   ps        - the source buffer pointer.
 * @param[out]  pd        - the destination buffer pointer.
 * @param[in]   len       - the length of the audio data in samples.
 * @param[in]   width     - the width of each audio sample (unused).
 * @param[in]   channel   - the number of channels (unused).
 * @return      The length of the processed audio data in samples.
 */
int      tlkalg_asrc_48to16_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       This function calculates the required buffer size for ASRC 16kHz to 48kHz conversion.
 * @param[in]   channel    - the number of channels.
 * @return      The required buffer size or 0 if the channel is invalid.
 */
uint16_t tlkalg_asrc_16to48_get_size(uint8_t channel);

/**
 * @brief       This function initializes ASRC for 16kHz to 48kHz conversion.
 * @param[in]   p_buff    - the buffer pointer.
 * @param[in]   channel   - the number of channels.
 * @return      The result of the initialization, 0 on failure.
 */
int8_t   tlkalg_asrc_16to48_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function deinitializes ASRC for 16kHz to 48kHz conversion.
 * @return      Always returns 0.
 */
int8_t   tlkalg_asrc_16to48_deinit(void);

/**
 * @brief       This function processes audio data for ASRC 16kHz to 48kHz conversion.
 * @param[in]   ps        - the source buffer pointer.
 * @param[out]  pd        - the destination buffer pointer.
 * @param[in]   len       - the length of the audio data in samples.
 * @param[in]   width     - the width of each audio sample (unused).
 * @param[in]   channel   - the number of channels (unused).
 * @return      The length of the processed audio data in samples.
 */
int      tlkalg_asrc_16to48_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       This function calculates the required buffer size for ASRC 48kHz to 44.1kHz conversion.
 * @param[in]   channel    - the number of channels.
 * @return      The required buffer size or 0 if the channel is invalid.
 */
uint16_t tlkalg_asrc_48to441_get_size(uint8_t channel);

/**
 * @brief       This function initializes ASRC for 48kHz to 44.1kHz conversion.
 * @param[in]   p_buff    - the buffer pointer.
 * @param[in]   channel   - the number of channels.
 * @return      The result of the initialization, 0 on failure.
 */
int8_t   tlkalg_asrc_48to441_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function deinitializes ASRC for 48kHz to 44.1kHz conversion.
 * @return      Always returns 0.
 */
int8_t   tlkalg_asrc_48to441_deinit(void);

/**
 * @brief       This function processes audio data for ASRC 48kHz to 44.1kHz conversion.
 * @param[in]   ps        - the source buffer pointer.
 * @param[out]  pd        - the destination buffer pointer.
 * @param[in]   len       - the length of the audio data in samples.
 * @param[in]   width     - the width of each audio sample (unused).
 * @param[in]   channel   - the number of channels (unused).
 * @return      The length of the processed audio data in samples.
 */
int      tlkalg_asrc_48to441_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       This function calculates the required buffer size for ASRC 44.1kHz to 48kHz conversion.
 * @param[in]   channel    - the number of channels.
 * @return      The required buffer size or 0 if the channel is invalid.
 */
uint16_t tlkalg_asrc_441to48_get_size(uint8_t channel);

/**
 * @brief       This function initializes ASRC for 44.1kHz to 48kHz conversion.
 * @param[in]   p_buff    - the buffer pointer.
 * @param[in]   channel   - the number of channels.
 * @return      The result of the initialization, 0 on failure.
 */
int8_t   tlkalg_asrc_441to48_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function deinitializes ASRC for 44.1kHz to 48kHz conversion.
 * @return      Always returns 0.
 */
int8_t   tlkalg_asrc_441to48_deinit(void);

/**
 * @brief       This function processes audio data for ASRC 44.1kHz to 48kHz conversion.
 * @param[in]   ps        - the source buffer pointer.
 * @param[out]  pd        - the destination buffer pointer.
 * @param[in]   len       - the length of the audio data in samples.
 * @param[in]   width     - the width of each audio sample (unused).
 * @param[in]   channel   - the number of channels (unused).
 * @return      The length of the processed audio data in samples.
 */
int      tlkalg_asrc_441to48_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       This function calculates the required buffer size for ASRC 44.1kHz to 16kHz conversion.
 * @param[in]   channel    - the number of channels.
 * @return      The required buffer size or 0 if the channel is invalid.
 */
uint16_t tlkalg_asrc_441to16_get_size(uint8_t channel);

/**
 * @brief       This function initializes ASRC for 44.1kHz to 16kHz conversion.
 * @param[in]   p_buff    - the buffer pointer.
 * @param[in]   channel   - the number of channels.
 * @return      The result of the initialization, 0 on failure.
 */
int8_t   tlkalg_asrc_441to16_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function deinitializes ASRC for 44.1kHz to 16kHz conversion.
 * @return      Always returns 0.
 */
int8_t   tlkalg_asrc_441to16_deinit(void);

/**
 * @brief       This function processes audio data for ASRC 44.1kHz to 16kHz conversion.
 * @param[in]   ps        - the source buffer pointer.
 * @param[out]  pd        - the destination buffer pointer.
 * @param[in]   len       - the length of the audio data in samples.
 * @param[in]   width     - the width of each audio sample (unused).
 * @param[in]   channel   - the number of channels (unused).
 * @return      The length of the processed audio data in samples.
 */
int      tlkalg_asrc_441to16_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

#endif
