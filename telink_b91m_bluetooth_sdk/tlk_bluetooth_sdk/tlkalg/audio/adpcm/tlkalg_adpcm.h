/********************************************************************************************************
 * @file    tlkalg_adpcm.h
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
#ifndef TLKALG_ADPCM_H
#define TLKALG_ADPCM_H

#include "tlkmw/audio/audio_mw_manager.h"

#if TONE_ADPCM_EN
#define TLKALG_ADPCM_STANDARD_EN 0

#if !TLKALG_ADPCM_STANDARD_EN
/**
 * @brief Initialize the ADPCM module with given parameters
 * @param[in] ps - pointer to the ADPCM source buffer
 * @param[in] len - length of the ADPCM data
 * @param[in] pre - initial prediction value
 * @param[in] idx - initial index value
 * @return none
 */
void adpcm_init(unsigned char *ps, int len, int pre, int idx);

/**
 * @brief Set the ADPCM conversion gain
 * @param[in] gain - conversion gain
 * @return none
 */
void adpcm_set_gain(int gain);

/**
 * @brief ADPCM to pcm
 * @param[in] pd - pointer to the adpcm source buffer
 * @param[in] n - decoded size
 * @param[in] sample_rate - Conversion sampling rate
 * @return Encoding result size
 */
_attribute_ram_code_ int adpcm_get_sample(signed short *pd, int n, int sample_rate);
#else
typedef struct tlkalg_adpcm_para
{
    int   sample_rate;
    int   len;
    short pre;
    short pre_enc;
    short gain;
    char  idx;
    char  idx_enc;
} tlkalg_adpcm_para_t;

/**
 * @brief Set ADPCM parameters
 * @param[in] type - type of parameter
 * @param[in] param - pointer to the parameter structure
 * @return none
 */
void tlkalg_adpcm_set_para(uint8_t type, void *param);

/**
 * @brief Converts ADPCM data to PCM data
 * @param[in] ps - pointer to the ADPCM source buffer
 * @param[out] pd - pointer to the PCM destination buffer
 * @param[in] n - length of the ADPCM data
 * @param[in] width - width of the data
 * @param[in] channel - number of channels
 * @return length of the converted PCM data
 */
_attribute_ram_code_ int tlkalg_adpcm_dec_process(uint8_t *ps, uint8_t *pd, uint16_t n, uint8_t width, uint8_t channel);

#endif

#endif //TLKALG_ADPCM_H
#endif
