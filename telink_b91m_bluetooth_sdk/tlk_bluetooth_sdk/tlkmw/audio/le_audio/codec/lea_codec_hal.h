/********************************************************************************************************
 * @file    lea_codec_hal.h
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
#include "core/mcu_type.h"

#define LEA_CODEC_MODE_LEFT_ONLY        0x00
#define LEA_CODEC_MODE_RIGHT_ONLY       0x01
#define LEA_CODEC_MODE_STEREO           0x02

#if (CHIP_TYPE == CHIP_TYPE_TL721X)
#if (TLKHW_TYPE == BOARD_721X_EVK_C1TXA104_V1_1)
    #define LEA_CODEC_INPUT_MODE            LEA_CODEC_MODE_LEFT_ONLY
#else
    #define LEA_CODEC_INPUT_MODE            LEA_CODEC_MODE_STEREO
#endif
#endif

#ifndef LEA_CODEC_INPUT_MODE
#define LEA_CODEC_INPUT_MODE            LEA_CODEC_MODE_STEREO
#endif

#ifndef LEA_CODEC_OUTPUT_MODE
#define LEA_CODEC_OUTPUT_MODE           LEA_CODEC_MODE_STEREO
#endif

#define LEA_CODEC_OUTPUT_ALG_TIME_US     2000

/**
 * @brief       Read input value from codec.
 * @param[out]  p_buffer    - pointer to the buffer to store the read data.
 * @param[in]   buffer_len  - length of the buffer in bytes.
 * @return      true if data is successfully read, false otherwise.
 */
bool tlk_lea_codec_read_input_value(uint8_t *p_buffer, uint16_t buffer_len);

/**
 * @brief       Write output value to codec.
 * @param[in]   p_buffer    - pointer to the buffer containing the data to write.
 * @param[in]   buffer_len  - length of the buffer in bytes.
 * @return      none.
 */
void tlk_lea_codec_write_output_value(uint8_t *p_buffer, uint16_t buffer_len);
