/********************************************************************************************************
 * @file    tone.c
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
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkalg/audio/adpcm/tlkalg_adpcm.h"
#include "drivers.h"
#include "tone_sbc.h"

#define TONE_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_SNK << 16) | TLK_DEBUG_DBG_FLAG_ALL)

#define TONE_DBG_SIGN NULL

tone_cfg_t g_tone_cfg = {.addr   = 0x20180000,
                         .volume = 512,
#if TONE_SBC_EN
                         .type = TONE_TYPE_SBC,
#else
                         .type = TONE_TYPE_ADPCM,
#endif
                         .busy  = 0,
                         .ready = 0,
                         .buff  = NULL};

/**
 * @brief  get tone data according to id
 * @param[in]  id: tone id
 * @returns none
 */
void tone_play(uint8_t id)
{
    uint8_t *p     = NULL;
    uint8_t *p_des = NULL;
    uint16_t len;

    if (g_tone_cfg.busy) {
        if ((g_tone_cfg.play_fifo_wptr + 1) % 4 != g_tone_cfg.play_fifo_rptr) {
            g_tone_cfg.play_fifo[g_tone_cfg.play_fifo_wptr] = id;
            g_tone_cfg.play_fifo_wptr                       = (g_tone_cfg.play_fifo_wptr + 1) % 4;
        } //push into fifo,wait play
        return;
    }

#ifdef SL16_tone_req_id
    log_b16(SL_AUDIO_PATH_LOG_EN, SL16_tone_req_id, id);
#endif

    g_tone_cfg.cur_id = id;
    p                 = (uint8_t *)g_tone_cfg.addr + id * 12;
    len               = p[8] + (p[9] << 8) + (p[10] << 16);

    if (p[7] == 0xff || p[11] == 0xff || p[1] > 0x1F) {
        g_tone_cfg.busy = 0;
        adpcm_init(0, 0, 0, 0);
    } else {
        p_des = (uint8_t *)g_tone_cfg.addr + (p[4] + (p[5] << 8) + (p[6] << 16) + (p[7] << 24));
        if ((g_tone_cfg.type == TONE_TYPE_SBC) && (p[0] - 1 == id) && p[2] == 0x00 && p[3] == 0x00) {
            g_tone_cfg.frame_size = get_source_sbc_framesize(p_des);
            sbc_init(p_des, len / g_tone_cfg.frame_size * 128, 0, 0);
            tlkapi_trace(TONE_DBG_FLAG, TONE_DBG_SIGN, "tone_play: %d, sbc_init\n", id);
        } else {
            adpcm_init(p_des, len, 0, 0);
            tlkapi_trace(TONE_DBG_FLAG, TONE_DBG_SIGN, "tone_play: %d, adpcm_init\n", id);
        }

        g_tone_cfg.len    = len;
        g_tone_cfg.offset = 0;
        g_tone_cfg.buff   = p_des;
        g_tone_cfg.ready  = 0;
        g_tone_cfg.busy   = 1;
    }
}

/**
 * @brief  get tone sample
 * @param[in]  p_out: address to restore tone data
 * @param[in]  number_of_byte: data length in byte
 * @param[in]  sample_rate: sample rate
 * @returns tone data length
 */
uint16_t tone_get_sample(int16_t *p_out, uint16_t number_of_byte, uint16_t sample_rate)
{
    uint16_t num   = 0;
    int16_t *p_des = p_out;

#ifdef SLEV_aud_tone_playing
    log_tick_general(SL_AUDIO_PATH_LOG_EN, SLEV_aud_tone_playing);
#endif

    if (g_tone_cfg.type == TONE_TYPE_ADPCM) {
        num = adpcm_get_sample(p_des, number_of_byte / sizeof(tone_int), sample_rate);
    } else if (g_tone_cfg.type == TONE_TYPE_SBC) {
        num = sbc_get_sample(p_des, number_of_byte / sizeof(tone_int), sample_rate);
    }

    if (num != number_of_byte / sizeof(tone_int)) {
        g_tone_cfg.busy = 0;

        if (g_tone_cfg.play_fifo_wptr != g_tone_cfg.play_fifo_rptr) {
            //fifo is not empty
            uint8_t playindex         = g_tone_cfg.play_fifo[g_tone_cfg.play_fifo_rptr];
            g_tone_cfg.play_fifo_rptr = (g_tone_cfg.play_fifo_rptr + 1) % 4;
            tone_play(playindex);
        }

        return 0;
    }

    g_tone_cfg.ready = 1;

    return number_of_byte;

#if 0
    num = 0;

    while (num < number_of_byte) {
        if (g_tone_cfg.offset >= g_tone_cfg.len) {
            g_tone_cfg.busy = 0;

            if (g_tone_cfg.hold) {
                tone_play(g_tone_cfg.hold & 0x7f);
            }

            return 0;
        }

        tlkalg_msbc_dec(g_tone_cfg.buff, 60, (uint8_t *)p_des);

        num += 240;
        g_tone_cfg.buff += 60;
        g_tone_cfg.offset += 60;

        for (int16_t i = 0; i < 120; i++) {
            *p_des = (int16_t)(*p_des) * g_tone_cfg.volume / 1024;
            p_des++;
        }
    }

    g_tone_cfg.ready = 1;
#endif

    return num;
}

/**
 * @brief  mix tone with pcm data
 * @param[in]  pcm_left: left pcm data
 * @param[in]  pcm_right: right pcm data
 * @param[in]  p_tone: tone data
 * @param[in]  samples: data length in sample
 * @returns none
 */
_attribute_ram_code_ int16_t *app_mix_tone(int16_t *pcm_left, int16_t *pcm_right, int16_t *p_tone, int16_t samples)
{
    if (tone_data_ready()) {
        for (int i = 0; i < samples; i++) {
            s16 sl = pcm_left[i];
            int v  = sl + *p_tone++;
            if (v > BIT(15)) {
                v = BIT(15) - 1;
            } else if (v < -BIT(15)) {
                v = -BIT(15);
            }
            pcm_left[i] = v;
            //            if  (pcm_left == pcm_right) {
            //                p_tone++;
            //                continue;
            //            }

            s16 sh = pcm_right[i];
            v      = sh + *p_tone++;
            if (v > BIT(15)) {
                v = BIT(15) - 1;
            } else if (v < -BIT(15)) {
                v = -BIT(15);
            }
            pcm_right[i] = v;
        }
    }

    return p_tone;
}

/**
 * @brief Retrieves the frame size of an SBC audio frame from the given data.
 * @param[in] data Pointer to the SBC audio data.
 * @return Frame size of the SBC audio.
 */
__attribute__((weak)) uint8_t get_source_sbc_framesize(uint8_t *data)
{
    (void)data;
    return 0;
}

/**
 * @brief SBC to pcm
 * @param[in] pointer to the sbc source buffer
 * @param[in] decoded size
 * @param[in] Conversion sampling rate
 * @returns Encoding result size
 */
__attribute__((weak)) int sbc_get_sample(signed short *pd, int n, int sample_rate)
{
    (void)pd;
    (void)n;
    (void)sample_rate;
    return 0;
}

/**
 * @brief Initializes the tone SBC module.
 * @return none.
 */
__attribute__((weak)) void tone_sbc_init(void) {}

/**
 * @brief Initializes the SBC parameters with the provided data.
 * @param[in] ps Pointer to the source buffer.
 * @param[in] len Length of the source buffer.
 * @param[in] pre Predicted value for initialization.
 * @param[in] idx Index value for initialization.
 * @return none.
 */
__attribute__((weak)) void sbc_init(unsigned char *ps, int len, int pre, int idx)
{
    (void)ps;
    (void)len;
    (void)pre;
    (void)idx;
}
