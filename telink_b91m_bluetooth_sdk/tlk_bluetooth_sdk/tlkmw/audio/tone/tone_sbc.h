/********************************************************************************************************
 * @file    tone_sbc.h
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
#ifndef TLKMW_AUDIO_TONE_TONE_SBC_H_
#define TLKMW_AUDIO_TONE_TONE_SBC_H_

/**
 * @brief Initializes the SBC parameters with the provided data.
 * @param[in] ps Pointer to the source buffer.
 * @param[in] len Length of the source buffer.
 * @param[in] pre Predicted value for initialization.
 * @param[in] idx Index value for initialization.
 * @return none.
 */
void sbc_init(unsigned char *ps, int len, int pre, int idx);

/**
 * @brief Initializes the tone SBC module.
 * @return none.
 */
void tone_sbc_init(void);

/**
 * @brief Reads PCM samples from the decoded SBC data.
 * @param[out] buf Buffer to store the PCM samples.
 * @param[in] sample_num Number of samples to read.
 * @return Number of samples read.
 */
int sbc_pcm_sample_read(int16_t* buf, int sample_num);

/**
 * @brief   Convert sbc parameter from a2dp form to common form and given to struct
 * @param[in]   data: sbc data in a2dp form
 * @returns sample rate
 */
uint8_t bt_music_parse_sbc_param(uint8_t *data);

/**
 * @brief Decodes SBC audio data.
 * @param[in] ps Pointer to the source buffer.
 * @param[in] len Length of the source buffer.
 * @param[out] pd Pointer to the destination buffer.
 * @return The length of decoded data.
 */
int tone_sbc_dec (unsigned char *ps, int len, unsigned char *pd);

/**
 * @brief Converts 8-bit SBC samples to 16-bit PCM samples.
 * @param[in] ps Pointer to the source buffer.
 * @param[out] pd Pointer to the destination buffer.
 * @param[in] n Number of samples to convert.
 * @return Number of converted samples.
 */
int sbc8to16(unsigned char *ps, signed short *pd, int n);

/**
 * @brief Retrieves the frame size of an SBC audio frame from the given data.
 * @param[in] data Pointer to the SBC audio data.
 * @return Frame size of the SBC audio.
 */
uint8_t get_source_sbc_framesize(uint8_t *data);

/**
 * @brief SBC to pcm
 * @param[in] pointer to the sbc source buffer
 * @param[in] decoded size
 * @param[in] Conversion sampling rate
 * @returns Encoding result size
 */
int sbc_get_sample(signed short *pd, int n, int sample_rate);

#endif /* TLKMW_AUDIO_TONE_TONE_SBC_H_ */
