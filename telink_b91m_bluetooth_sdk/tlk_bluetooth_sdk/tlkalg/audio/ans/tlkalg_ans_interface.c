/********************************************************************************************************
 * @file    tlkalg_ans_interface.c
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
#include "tlkalg_ans_interface.h"

#if (TLKALG_ANS_ENABLE | TLKALG_ANS_SPK_ENABLE)
uint8_t *g_ans_buf_ptr         = NULL;
uint8_t *g_ans_src_buf_ptr     = NULL;
uint8_t *g_ans_spk_buf_ptr     = NULL;
uint8_t *g_ans_spk_src_buf_ptr = NULL;

W_NS_CFG_PARAM ans_para = {
    .frame_size   = 80,
    .sampleRate   = 16000,
    .target_level = k16dB,
    .lowShelf_En  = 1,
    .preGain      = 2.0,
    .postGain     = 1.0,
};

W_NS_CFG_PARAM ans_spk_para = {
    .frame_size   = 80,
    .sampleRate   = 16000,
    .target_level = k16dB,
    .lowShelf_En  = 1,
    .preGain      = 2.0,
    .postGain     = 1.0,
};

/**
 * @brief   Calculates the total buffer size required for the acoustic noise suppression (ANS) algorithm.
 * @param[in]   channel - The channel number (not used in this function).
 * @return      The total buffer size required for ANS.
 */
uint16_t tlkalg_ans_get_size(uint8_t channel)
{
    (void)channel;
    int size     = tlka_w_ns_get_size();
    size         = (size + 3) / 4 * 4;
    int scr_size = tlka_w_ns_get_scratch_size();
    scr_size     = (scr_size + 3) / 4 * 4;

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ans size %d, scr size %d", size, scr_size);
    return (size + scr_size);
}

/**
 * @brief   Initializes the acoustic noise suppression (ANS) algorithm.
 * @param[in]   p_buff  - Pointer to the buffer allocated for ANS.
 * @param[in]   channel - The channel number (not used in this function).
 * @return      1 if initialization is successful, 0 otherwise.
 */
int8_t tlkalg_ans_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[ERR]", "tlkalg_ans_init error, pointer null");
        return 0;
    }

    g_ans_buf_ptr     = p_buff;
    g_ans_src_buf_ptr = g_ans_buf_ptr + (tlka_w_ns_get_size() + 3) / 4 * 4;

    tlka_w_ns_init((void *)g_ans_buf_ptr, ans_para, (void *)g_ans_src_buf_ptr);

    return 1;
}

/**
 * @brief   Deinitializes the acoustic noise suppression (ANS) algorithm.
 * @return      0 to indicate completion of deinitialization.
 */
int8_t tlkalg_ans_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_ans_deinit");
    g_ans_buf_ptr     = NULL;
    g_ans_src_buf_ptr = NULL;

    return 0;
}

/**
 * @brief   Processes a frame of audio data for acoustic noise suppression (ANS).
 * @param[in]   ps      - Pointer to the source audio buffer.
 * @param[out]  pd      - Pointer to the destination audio buffer.
 * @param[in]   len     - Length of the audio data (not used in this function).
 * @param[in]   width   - Width of the audio data (not used in this function).
 * @param[in]   channel - The channel number (not used in this function).
 * @return      The result of the processing, typically 0 for success.
 */
int tlkalg_ans_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    (void)len;
    if (ps == NULL || pd == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERR]", "tlkalg_ans_PS or PD null");
        return 0;
    }

    if (g_ans_buf_ptr == NULL || g_ans_src_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERR]", "tlkalg_ans_struct null");
        return 0;
    }

    // gpio_set_high_level(GPIO_CHN0);
    int ret = tlka_w_ns_process_frame_16(g_ans_buf_ptr, (short *)ps, (short *)pd);
    // gpio_set_low_level(GPIO_CHN0);

    return ret;
}

/**
 * @brief   Initializes the acoustic noise suppression (ANS) algorithm for speaker audio.
 * @param[in]   p_buff  - Pointer to the buffer allocated for ANS speaker audio.
 * @param[in]   channel - The channel number (not used in this function).
 * @return      1 if initialization is successful, 0 otherwise.
 */
int8_t tlkalg_ans_spk_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[ERR]", "tlkalg_ans_init error, pointer null");
        return 0;
    }

    g_ans_spk_buf_ptr     = p_buff;
    g_ans_spk_src_buf_ptr = g_ans_spk_buf_ptr + (tlka_w_ns_get_size() + 3) / 4 * 4;

    tlka_w_ns_init((void *)g_ans_spk_buf_ptr, ans_spk_para, (void *)g_ans_spk_src_buf_ptr);

    return 1;
}

/**
 * @brief   Deinitializes the acoustic noise suppression (ANS) algorithm for speaker audio.
 * @return      0 to indicate completion of deinitialization.
 */
int8_t tlkalg_ans_spk_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_ans_deinit");
    g_ans_spk_buf_ptr     = NULL;
    g_ans_spk_src_buf_ptr = NULL;

    return 0;
}

/**
 * @brief   Processes a frame of speaker audio data for acoustic noise suppression (ANS).
 * @param[in]   ps      - Pointer to the source speaker audio buffer.
 * @param[out]  pd      - Pointer to the destination speaker audio buffer.
 * @param[in]   len     - Length of the audio data (not used in this function).
 * @param[in]   width   - Width of the audio data (not used in this function).
 * @param[in]   channel - The channel number (not used in this function).
 * @return      The result of the processing, typically 0 for success.
 */
int tlkalg_ans_spk_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    (void)len;
    if (ps == NULL || pd == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERR]", "tlkalg_ans_PS or PD null");
        return 0;
    }

    if (g_ans_spk_buf_ptr == NULL || g_ans_spk_src_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERR]", "tlkalg_ans_struct null");
        return 0;
    }

    int ret = tlka_w_ns_process_frame_16(g_ans_spk_buf_ptr, (short *)ps, (short *)pd);

    return ret;
}

#endif
