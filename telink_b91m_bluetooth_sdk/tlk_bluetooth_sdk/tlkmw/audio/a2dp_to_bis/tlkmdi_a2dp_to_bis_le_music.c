/********************************************************************************************************
 * @file    tlkmdi_a2dp_to_bis_le_music.c
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

#include "tlkalg/audio/audio_alg_interface.h"
#include "tlkalg/audio/lc3/tlkalg_lc3_interface.h"

#include "tlkmw/audio/tlkmw_audio.h"

#include "tl_common.h"
#include "tlkapi/tlkapi.h"

#include "stack/ble/ble.h"

#include "drivers.h"

#include "tlkmdi_a2dp_to_bis_le_music.h"

#if (TLK_MW_LEA_A2DP_TO_BIS_ENABLE)

#define LE_AUDIO_MAX_PER_CODEC_FRAME 155 // LC3_48_6

struct a2dp_to_bis_lea_config
{
    uint8_t *lc3_alg_buffer;
    bool     enable;
    uint16_t frameOctets;
    uint32_t sample_count;
    uint16_t iso_handle[2];
};

static struct a2dp_to_bis_lea_config s_a2dp_to_bis_lea_config = {
    .lc3_alg_buffer = NULL,
};

/**
 * @brief       Initialize LEA A2DP to BIS module
 * @param       None
 * @return      None
 */
void lea_a2dp_to_bis_le_init(void)
{
    audio_alg_interface_t *alg_lc3 = audio_alg_get_interface_by_type(ALG_LC3_ENC);
#if (BIS_SOURCE_SIMPLE_MODE)
    s_a2dp_to_bis_lea_config.lc3_alg_buffer = (uint8_t *)tlkalg_malloc_func(alg_lc3->audio_alg_get_size(1));
    if (s_a2dp_to_bis_lea_config.lc3_alg_buffer != NULL) {
        alg_lc3->audio_alg_init(s_a2dp_to_bis_lea_config.lc3_alg_buffer, 1);
    }
#else
    s_a2dp_to_bis_lea_config.lc3_alg_buffer = (uint8_t *)tlkalg_malloc_func(alg_lc3->audio_alg_get_size(2));
    if (s_a2dp_to_bis_lea_config.lc3_alg_buffer != NULL) {
        alg_lc3->audio_alg_init(s_a2dp_to_bis_lea_config.lc3_alg_buffer, 2);
    }
#endif

    tlk_printf("LC3 Malloc: %x\n", s_a2dp_to_bis_lea_config.lc3_alg_buffer);
}

/**
 * @brief       Deinitialize LEA A2DP to BIS module
 * @param       None
 * @return      None
 */
void lea_a2dp_to_bis_le_deinit(void)
{
    if (s_a2dp_to_bis_lea_config.lc3_alg_buffer != NULL) {
        audio_alg_interface_t *alg_lc3 = audio_alg_get_interface_by_type(ALG_LC3_ENC);
        alg_lc3->audio_alg_deinit();
        tlkalg_free_func(s_a2dp_to_bis_lea_config.lc3_alg_buffer);
    }
    s_a2dp_to_bis_lea_config.lc3_alg_buffer = NULL;
}

/**
 * @brief       Calculate sample count based on frequency and duration
 * @param[in]   frequency - Sampling frequency selector
 * @param[in]   duration - Frame duration selector
 * @return      Calculated sample count, or 0 if invalid parameters
 */
static uint32_t lea_calc_sample_count(uint8_t frequency, uint8_t duration)
{
    if (duration == LEA_SELECT_FRAME_DURATION_10) {
        if (frequency == LEA_SELECT_SAMPLING_FREQ_8000_HZ) {
            return 8 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_16000_HZ) {
            return 16 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_24000_HZ) {
            return 24 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_32000_HZ) {
            return 32 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_48000_HZ) {
            return 48 * 10;
        }
    } else if (duration == LEA_SELECT_FRAME_DURATION_7_5) {
        if (frequency == LEA_SELECT_SAMPLING_FREQ_8000_HZ) {
            return 8 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_16000_HZ) {
            return 16 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_24000_HZ) {
            return 24 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_32000_HZ) {
            return 32 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_48000_HZ) {
            return 48 * 7.5;
        }
    }
    return 0;
}

/**
 * @brief       Initialize A2DP to BIS configuration
 * @param[in]   p_config - Pointer to LEA A2DP to BIS configuration structure
 * @return      None
 */
void lea_a2dp_to_bis_init(struct lea_a2dp_to_bis_config *p_config)
{
    s_a2dp_to_bis_lea_config.iso_handle[0] = p_config->iso_handle[0];
    s_a2dp_to_bis_lea_config.iso_handle[1] = p_config->iso_handle[1];
    s_a2dp_to_bis_lea_config.frameOctets   = p_config->frameOctets;
    s_a2dp_to_bis_lea_config.sample_count  = lea_calc_sample_count(p_config->samplingFrequency, p_config->frameDuration);

    audio_alg_interface_t *alg_lc3 = audio_alg_get_interface_by_type(ALG_LC3_ENC);

    struct lc3_initial_param_bap lc3Init = {
        .channelIndex  = 0,
        .samplingFreq  = p_config->samplingFrequency,
        .perCodecFrame = p_config->frameOctets,
        .frameDuration = p_config->frameDuration,
    };

    alg_lc3->audio_alg_param_set(LC3_TYPE_SET_PARAM_BAP, &lc3Init);

#if (!BIS_SOURCE_SIMPLE_MODE)
    lc3Init.channelIndex = 1;
    alg_lc3->audio_alg_param_set(LC3_TYPE_SET_PARAM_BAP, &lc3Init);
#endif
    s_a2dp_to_bis_lea_config.enable = true;
}

/**
 * @brief       Deinitialize A2DP to BIS module
 * @param       None
 * @return      None
 */
void lea_a2dp_to_bis_deinit(void)
{
    s_a2dp_to_bis_lea_config.enable = false;
}

/**
 * @brief       Process input PCM data and encode to LC3 for BIS transmission
 * @param[in]   pcm_data - Pointer to input PCM data (stereo format)
 * @param[in]   sample_num - Number of samples to process
 * @return      None
 */
void lea_a2dp_to_bis_input_data_process(int16_t *pcm_data, uint16_t sample_num)
{
    if (s_a2dp_to_bis_lea_config.enable == false) {
        return;
    }

    int16_t left_data[sample_num];
#if (!BIS_SOURCE_SIMPLE_MODE)
    int16_t right_data[sample_num];
#endif
    for (int i = 0; i < sample_num; i++) {
        left_data[i] = pcm_data[2 * i];
#if (!BIS_SOURCE_SIMPLE_MODE)
        right_data[i] = pcm_data[2 * i + 1];
#endif
    }

    uint8_t encode_data_left[LE_AUDIO_MAX_PER_CODEC_FRAME];
    uint8_t encode_data_right[LE_AUDIO_MAX_PER_CODEC_FRAME];

    audio_alg_interface_t *alg_lc3 = audio_alg_get_interface_by_type(ALG_LC3_ENC);

    alg_lc3->audio_alg_process((uint8_t *)left_data, encode_data_left, s_a2dp_to_bis_lea_config.sample_count * sizeof(int16_t), 16, 0); //LC3 default 0.


#if (BIS_SOURCE_SIMPLE_MODE)
    memcpy(encode_data_right, encode_data_left, LE_AUDIO_MAX_PER_CODEC_FRAME);
#else
    alg_lc3->audio_alg_process((uint8_t *)right_data, encode_data_right, s_a2dp_to_bis_lea_config.sample_count * sizeof(int16_t), 16, 1); //LC3 default 1.
#endif

    ble_host_send_ble_iso_data(s_a2dp_to_bis_lea_config.iso_handle[0], 0, encode_data_left, s_a2dp_to_bis_lea_config.frameOctets);
    ble_host_send_ble_iso_data(s_a2dp_to_bis_lea_config.iso_handle[1], 0, encode_data_right, s_a2dp_to_bis_lea_config.frameOctets);
}

#endif