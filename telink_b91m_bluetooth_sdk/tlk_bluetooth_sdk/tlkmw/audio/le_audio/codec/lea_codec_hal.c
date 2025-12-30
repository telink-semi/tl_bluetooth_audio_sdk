/********************************************************************************************************
 * @file    lea_codec_hal.c
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
#include "common/types.h"

#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkalg/audio/asrc_24bit/tlka_asrc_api.h"

#include "lea_codec.h"
#include "lea_codec_hal.h"

#if (TLK_MW_LE_AUDIO_ENABLE)

#if (LE_AUDIO_CODEC_INPUT_TYPE == LE_AUDIO_CODEC_TYPE_CODEC)
/**
 * @brief       Read input value from codec.
 * @param[out]  p_buffer    - pointer to the buffer to store the read data.
 * @param[in]   buffer_len  - length of the buffer in bytes.
 * @return      true if data is successfully read, false otherwise.
 */
bool tlk_lea_codec_read_input_value(uint8_t *p_buffer, uint16_t buffer_len)
{
    if (tlkdrv_codec_readMicData(p_buffer, buffer_len, NULL) == false) {
        return false;
    }

#if LEA_CODEC_INPUT_MODE == LEA_CODEC_MODE_LEFT_ONLY
    int16_t *p_data = (int16_t *) p_buffer;
    for (int i = 0; i < buffer_len / 4; i++) {
        p_data[2 * i + 1] = p_data[2 * i];
    }
#endif

#if LEA_CODEC_INPUT_MODE == LEA_CODEC_MODE_RIGHT_ONLY
    int16_t *p_data = (int16_t *) p_buffer;
    for (int i = 0; i < buffer_len / 2; i++) {
        p_data[2 * i] = p_data[2 * i + 1];
    }
#endif

#if LEA_CODEC_INPUT_MODE == LEA_CODEC_MODE_STEREO

#endif

    return true;
}
#endif

#if (LE_AUDIO_CODEC_OUTPUT_TYPE == LE_AUDIO_CODEC_TYPE_CODEC)
/**
 * @brief       Write output value to codec.
 * @param[in]   p_buffer    - pointer to the buffer containing the data to write.
 * @param[in]   buffer_len  - length of the buffer in bytes.
 * @return      none.
 */
void tlk_lea_codec_write_output_value(uint8_t *p_buffer, uint16_t buffer_len)
{
    tlkdrv_codec_fillSpkBuff((uint8_t *) p_buffer, buffer_len);
}
#endif

#endif      // TLK_MW_LE_AUDIO_ENABLE
