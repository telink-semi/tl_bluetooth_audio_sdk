/********************************************************************************************************
 * @file    tlkalg_lc3_interface.h
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
enum lc3_set_type
{
    LC3_TYPE_SET_PARAM,
    LC3_TYPE_SET_PARAM_BAP,
    LC3_TYPE_SET_BEC_DETECT,
};

struct lc3_initial_param
{
    uint8_t  channelIndex;
    uint32_t sampleRate; /** < original audio sampling frequency. 8000 mean 8kHz */
    uint32_t bitRate;    /** < compressed audio bit rate. */
    uint32_t cycle;      /** audio compression cycle. 1 mean 7.5ms, 0 mean 10ms. */
};

struct lc3_initial_param_bap
{
    uint8_t  channelIndex;
    uint32_t samplingFreq;  /** < Sampling_Frequency. */
    uint32_t perCodecFrame; /** < Octets_Per_Codec_Frame. */
    uint32_t frameDuration; /** Frame_Duration */
};

struct lc3_dec_set_bec_detect
{
    uint8_t      channelIndex;
    unsigned int detect;
};

/**
 * @brief       This function sets the algorithm parameter settings based on the type.
 * @param[in]   type    - the type of parameter to set.
 * @param[in]   param   - the parameter value.
 * @return      The result of setting the parameter.
 */
uint8_t tlkalg_lc3_enc_set_param(uint8_t type, void *param);

/**
 * @brief       Gets the memory size used by the LC3 encoder.
 * @param[in]   channel - Number of channels.
 * @return      Memory size required for the encoder.
 */
uint16_t tlkalg_lc3_enc_get_size(uint8_t channel);

/**
 * @brief       Initializes the LC3 encoder.
 * @param[in]   p_buff  - Pointer to the buffer for the encoder.
 * @param[in]   channel - Number of channels (mono or stereo).
 * @return      0 on success.
 */
int8_t tlkalg_lc3_enc_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitializes the LC3 encoder.
 * @return      0 on success.
 */
int8_t tlkalg_lc3_enc_deinit(void);

/**
 * @brief       Processes the audio encoding.
 * @param[in]   ps    - Pointer to the input audio samples.
 * @param[out]  pd    - Pointer to the output encoded data.
 * @param[in]   len   - Length of the input audio samples.
 * @param[in]   width - Width of the audio samples.
 * @param[in]   channel - Channel index (mono or stereo).
 * @return      0 on success, -1 on failure.
 */
int tlkalg_lc3_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Set algorithm parameter settings for LC3 decoder.
 * @param[in]   type    - the type of parameter to set.
 * @param[in]   param   - the parameter value to set.
 * @return      0 on success, non-zero on failure.
 */
uint8_t tlkalg_lc3_dec_set_param(uint8_t type, void *param);

/**
 * @brief       Gets the memory size used by the LC3 decoder.
 * @param[in]   channel - Number of channels.
 * @return      Memory size required for the decoder.
 */
uint16_t tlkalg_lc3_dec_get_size(uint8_t channel);

/**
 * @brief       Initializes the LC3 decoder.
 * @param[in]   p_buff  - Pointer to the buffer for the decoder.
 * @param[in]   channel - Number of channels (mono or stereo).
 * @return      0 on success.
 */
int8_t tlkalg_lc3_dec_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitializes the LC3 decoder.
 * @return      0 on success.
 */
int8_t tlkalg_lc3_dec_deinit(void);

/**
 * @brief       Processes the audio decoding.
 * @param[in]   ps    - Pointer to the input encoded data.
 * @param[out]  pd    - Pointer to the output audio samples.
 * @param[in]   len   - Length of the input encoded data.
 * @param[in]   width - Width of the audio samples.
 * @param[in]   channel - Channel index (mono or stereo).
 * @return      0 on success, -1 on failure.
 */
int tlkalg_lc3_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
