/********************************************************************************************************
 * @file    tlkalg_agc_interface.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkalg_agc_interface.h"
#include "tlkalg/audio/audio_alg_interface.h"

#if (TLKALG_AGC_ENABLE)

uint8_t *g_agc_buf_ptr     = NULL;
uint8_t *g_agc_src_buf_ptr = NULL;

AGC_Param agc_para = {
#if (TLKALG_AGC_TYPE == TLKALG_AGC_16K_10MS)
    .frame_size = 160,
    .sampleRate = 16000,
#elif (TLKALG_AGC_TYPE == TLKALG_AGC_48K_5MS)
    .frame_size = 240,
    .sampleRate = 48000,
#endif
    .target_out_level_dbfs = -12,
    .noise_level_dbfs_thld = -50,
};

/**
 * @brief   Calculates the required size for AGC algorithm buffer and scratch space.
 * @param[in]   channel - The audio channel number.
 * @return      Total size required for AGC algorithm buffer and scratch space.
 */
uint16_t tlkalg_agc_get_size(uint8_t channel)
{
    (void)channel;
    int size     = tlka_agc_alg_get_size();
    size         = (size + 3) / 4 * 4;
    int scr_size = tlka_agc_alg_get_scratch_size();
    scr_size     = (scr_size + 3) / 4 * 4;

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "agc size %d, scr size %d", size, scr_size);
    return (size + scr_size);
}

/**
 * @brief   Initializes the AGC algorithm with the provided buffer and parameters.
 * @param[in]   p_buff  - Pointer to the buffer for AGC algorithm.
 * @param[in]   channel - The audio channel number.
 * @return      0 if initialization fails, non-zero otherwise.
 */
int8_t tlkalg_agc_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_agc_init error, pointer null");
        return 0;
    }

    g_agc_buf_ptr     = p_buff;
    g_agc_src_buf_ptr = g_agc_buf_ptr + (tlka_agc_alg_get_size() + 3) / 4 * 4;

    int8_t ret = tlka_agc_alg_init(g_agc_buf_ptr, &agc_para, g_agc_src_buf_ptr, tlka_agc_alg_get_size());

    return ret;
}

/**
 * @brief   Deinitializes the AGC algorithm.
 * @return      0 on successful deinitialization.
 */
int8_t tlkalg_agc_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_agc_deinit");
    g_agc_buf_ptr     = NULL;
    g_agc_src_buf_ptr = NULL;

    return 0;
}

/**
 * @brief   Processes a frame of audio data using the AGC algorithm.
 * @param[in]   ps  - Pointer to the source audio buffer.
 * @param[out]  pd  - Pointer to the destination audio buffer.
 * @param[in]   len - Length of the audio data.
 * @param[in]   width - Width of the audio samples (e.g., 16-bit).
 * @param[in]   channel - The audio channel number.
 * @return      0 if processing fails, non-zero otherwise.
 */
int tlkalg_agc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)channel;
    if (ps == NULL || pd == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_agc_PS or PD null");
        return 0;
    }

    if (g_agc_buf_ptr == NULL || g_agc_src_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_agc_struct null");
        return 0;
    }

    if ((len % agc_para.frame_size) != 0) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "frame len error");
        return 0;
    }

    uint8_t frame_num = len / agc_para.frame_size;
    int     ret       = 0;
    if (width == ALG_WIDTH_16) {
        short *psrc = (short *)ps;
        short *pdes = (short *)pd;
        for (int i = 0; i < frame_num; i++) {
            ret = tlka_agc_alg_process_frame(g_agc_buf_ptr, psrc + (i * agc_para.frame_size), pdes + (i * agc_para.frame_size));
        }
    } else if (width == ALG_WIDTH_24) {
        int *psrc = (int *)ps;
        int *pdes = (int *)pd;
        for (int i = 0; i < frame_num; i++) {
            ret = tlka_agc_alg_process_frame_float(g_agc_buf_ptr, (float *)(psrc + (i * agc_para.frame_size)), (float *)(pdes + (i * agc_para.frame_size)));
        }
    }
    return ret;
}

#endif
