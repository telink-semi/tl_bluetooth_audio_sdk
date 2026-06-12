/********************************************************************************************************
 * @file    lea_codec_sine.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include <string.h>

#include "common/types.h"

#include "tlkmw/audio/tlkmw_audio.h"

#include "tlkmw/common/tlkaudio_debug_data.h"

#include "lea_codec.h"

#if (TLK_MW_LE_AUDIO_ENABLE && LE_AUDIO_CODEC_INPUT_TYPE == LE_AUDIO_CODEC_SAMPLE_SINE_WAVE)

/**
 * @brief       Initialize input stream.
 * @return      none.
 */
void lea_codec_input_stream_init(void) {}

/**
 * @brief       Deinitialize input stream.
 * @return      none.
 */
void lea_codec_input_stream_deinit(void) {}

/**
 * @brief       Get input audio data (sine wave).
 * @param[out]  left_data    - pointer to left channel audio data buffer.
 * @param[out]  right_data   - pointer to right channel audio data buffer.
 * @param[in]   sample_num   - number of samples per channel to read.
 * @return      true if data is successfully read, false otherwise.
 */
bool lea_codec_input_get_audio_data(int16_t *left_data, int16_t *right_data, uint16_t sample_num)
{
    tlkdrv_codec_get_sin_data(left_data, sample_num);
    memcpy(right_data, left_data, sample_num * sizeof(int16_t));
    return true;
}
#endif
