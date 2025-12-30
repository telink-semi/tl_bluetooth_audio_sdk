/********************************************************************************************************
 * @file    lea_codec.h
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
#ifndef LE_AUDIO_CODEC_H
#define LE_AUDIO_CODEC_H

#define LE_AUDIO_CODEC_TYPE_NONE        0x00
#define LE_AUDIO_CODEC_TYPE_CODEC       0x01
#define LE_AUDIO_CODEC_TYPE_USB_AUDIO   0x02
#define LE_AUDIO_CODEC_SAMPLE_SINE_WAVE 0x03

#ifndef LE_AUDIO_CODEC_INPUT_TYPE
#define LE_AUDIO_CODEC_INPUT_TYPE LE_AUDIO_CODEC_TYPE_NONE
#endif

#ifndef LE_AUDIO_CODEC_OUTPUT_TYPE
#define LE_AUDIO_CODEC_OUTPUT_TYPE LE_AUDIO_CODEC_TYPE_NONE
#endif

/** LE Audio Codec common functions start */

/**
 * @brief LE Audio Codec configuration structure.
 */
struct lea_codec_config
{
    bool     is_input_stream_init;
    bool     is_output_stream_init;
    uint8_t  input_sample_rate;
    uint32_t input_location;
    uint8_t  output_sample_rate;
    uint32_t output_location;
};

/**
 * @brief       Initialize LE Audio codec stream.
 * @param[in]   config    - pointer to the codec configuration structure.
 * @return      none.
 */
void lea_codec_stream_init(struct lea_codec_config *config);

/**
 * @brief       Deinitialize LE Audio codec stream.
 * @return      none.
 */
void lea_codec_stream_deinit(void);

/**
 * @brief       Set output volume.
 * @param[in]   volume    - volume value to set.
 * @return      none.
 */
void lea_codec_set_output_volume(uint8_t volume);

/**
 * @brief       Get output volume.
 * @return      current output volume value.
 */
uint8_t lea_codec_get_output_volume(void);

/**
 * @brief       Get mute buffer pointer.
 * @return      pointer to the mute buffer.
 */
int16_t *lea_codec_get_mute_buffer(void);

/**
 * @brief       Get input sampling rate.
 * @return      input sampling rate in Hz, 0 if input stream is not initialized.
 */
uint32_t lea_codec_get_input_sampling_rate(void);

/**
 * @brief       Get output sampling rate.
 * @return      output sampling rate in Hz, 0 if output stream is not initialized.
 */
uint32_t lea_codec_get_output_sampling_rate(void);

/**
 * @brief       Get input location.
 * @return      input location value, 0 if input stream is not initialized.
 */
uint32_t lea_codec_get_input_location(void);

/**
 * @brief       Get output location.
 * @return      output location value, 0 if output stream is not initialized.
 */
uint32_t lea_codec_get_output_location(void);
/** LE Audio Codec common functions end */

/**
 * @brief       Clean input buffer.
 * @return      none.
 */
void lea_codec_input_clean_buffer(void);

/**
 * @brief       Initialize both input and output stream.
 * @return      none.
 */
void lea_codec_in_output_stream_init(void);

/**
 * @brief       Deinitialize both input and output stream.
 * @return      none.
 */
void lea_codec_in_output_stream_deinit(void);

/**
 * @brief       Initialize input stream.
 * @return      none.
 */
void lea_codec_input_stream_init(void);

/**
 * @brief       Deinitialize input stream.
 * @return      none.
 */
void lea_codec_input_stream_deinit(void);

/**
 * @brief       Initialize output stream.
 * @return      none.
 */
void lea_codec_output_stream_init(void);

/**
 * @brief       Deinitialize output stream.
 * @return      none.
 */
void lea_codec_output_stream_deinit(void);

/**
 * @brief       Set output audio data.
 * @param[in]   left_data    - pointer to left channel audio data.
 * @param[in]   right_data   - pointer to right channel audio data.
 * @param[in]   sample_num   - number of samples per channel.
 * @return      none.
 */
void lea_codec_output_set_audio_data(int16_t *left_data, int16_t *right_data, uint16_t sample_num);

/**
 * @brief       Get input audio data.
 * @param[out]  left_data    - pointer to left channel audio data buffer.
 * @param[out]  right_data   - pointer to right channel audio data buffer.
 * @param[in]   sample_num   - number of samples per channel to read.
 * @return      true if data is successfully read, false otherwise.
 */
bool lea_codec_input_get_audio_data(int16_t *left_data, int16_t *right_data, uint16_t sample_num);

/**
 * @brief       Continue tone playback.
 * @return      none.
 */
void lea_us_tone_continue(void);

#endif
