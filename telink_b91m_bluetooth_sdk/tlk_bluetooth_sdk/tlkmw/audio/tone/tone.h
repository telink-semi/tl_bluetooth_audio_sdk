/********************************************************************************************************
 * @file    tone.h
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
#ifndef __TONE_H__
#define __TONE_H__

#if CODEC_DAC_MONO_MODE
typedef int16_t tone_int;
#else
typedef int32_t tone_int;
#endif

typedef enum
{
    TONE_PAIRING = 0,
    TONE_CONNECTED,
    TONE_LE_CONNECTED,
    TONE_LOW_POWER,
    TONE_RING,
    TONE_POWER_OFF,
    TONE_DISCONNECTED,
    TONE_BT_AUDIO_MUSIC_MODE,
    TONE_BT_AUDIO_GAME_MODE,

    MODE_MAX_TONE
} e_type_tone_t;

enum tone_type
{
    TONE_TYPE_ADPCM = 1,
    TONE_TYPE_SBC   = 5,
    TONE_TYPE_PCM8  = 6,
};

typedef struct
{
    uint8_t busy;
    uint8_t type;
    uint8_t ready;
    uint8_t cur_id;

    uint8_t play_fifo[4];

    uint8_t play_fifo_wptr;
    uint8_t play_fifo_rptr;
    int16_t volume;

    int32_t len;
    int32_t offset;

    uint8_t *buff;
    uint8_t  frame_size;
    uint8_t  hold;
} tone_cfg_t;

extern tone_cfg_t g_tone_cfg;

/**
 * @brief  whether tone is playing
 * @param[in]  none
 * @returns g_tone_cfg.busy
 */
static inline uint8_t tone_is_playing(void)
{
    return (g_tone_cfg.busy || g_tone_cfg.play_fifo_rptr != g_tone_cfg.play_fifo_wptr);
}

/**
 * @brief      get current tone id
 * @param[in]  none
 * @returns    g_tone_cfg.cur_id
 */
static inline uint8_t tone_get_cur_id(void)
{
    return g_tone_cfg.cur_id;
}

/**
 * @brief  whether tone is ready
 * @param[in]  none
 * @returns g_tone_cfg.ready
 */
static inline uint8_t tone_data_ready(void)
{
    return g_tone_cfg.ready;
}

/**
 * @brief  tone play finished
 * @param[in]  none
 * @returns none
 */
static inline void tone_data_played(void)
{
    g_tone_cfg.ready = 0;
}

/**
 * @brief  set tone volume
 * @param[in]  v: tone volume
 * @returns tone volume
 */
static inline uint8_t tone_set_volume(int16_t v)
{
    return g_tone_cfg.volume = v;
}

/**
 * @brief  get tone data according to id
 * @param[in]  id: tone id
 * @returns none
 */
void tone_play(uint8_t id);

/**
 * @brief  get tone sample
 *
 * @param[in]  p_out: address to restore tone data
 * @param[in]  number_of_byte: data length in byte
 * @param[in]  sample_rate: sample rate
 *
 * @returns tone data length
 */
uint16_t tone_get_sample(int16_t *p_out, uint16_t number_of_byte, uint16_t sample_rate);

/**
 * @brief  mix tone with pcm data
 * @param[in]  pcm_left: left pcm data
 * @param[in]  pcm_right: right pcm data
 * @param[in]  p_tone: tone data
 * @param[in]  samples: data length in sample
 * @returns none
 */
int16_t *app_mix_tone(int16_t *pcm_left, int16_t *pcm_right, int16_t *p_tone, int16_t samples);

#endif
