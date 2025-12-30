/********************************************************************************************************
 * @file    tlkalg_aec_interface.c
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
#include "tlkalg_aec_interface.h"
#include "tlka_aec_api.h"

#if TLKALG_AEC_ENABLE


#define AEC_FRAM_LEN 80

uint8_t *g_aec_buf_ptr     = NULL;
uint8_t *g_aec_src_buf_ptr = NULL;

AEC_Param aec_para = {
    .frame_size  = 80,
    .sampleRate  = 16000,
    .en_aec_post = 1,
};

/**
 * @brief       This function calculates the total size required for AEC initialization.
 * @param[in]   channel - the channel number.
 * @return      The total size required for AEC initialization.
 */
uint16_t tlkalg_aec_get_size(uint8_t channel)
{
    (void)channel;
    int size     = tlka_aec_get_size();
    size         = (size + 3) / 4 * 4;
    int scr_size = tlka_aec_get_scratch_size();
    scr_size     = (scr_size + 3) / 4 * 4;

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "aec size %d, scr size %d", size, scr_size);
    return (size + scr_size);
}

/**
 * @brief       This function initializes the AEC algorithm with the provided buffer.
 * @param[in]   p_buff  - the buffer to be used for AEC initialization.
 * @param[in]   channel - the channel number.
 * @return      0 on success, -1 on failure.
 */
int8_t tlkalg_aec_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    // int ver = tlka_aec_get_version();
    // uint16_t alg_size_aec = tlka_aec_get_size();//0x4c40 	0x5318 21272--old
    // uint16_t alg_size_aec_scr = tlka_aec_get_scratch_size();//0x1530		0x1228	4648--old
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_aec_ver %x",ver);
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_aec_size %x",alg_size_aec);
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_aec_size_scr %x",alg_size_aec_scr);
    if (p_buff == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_aec_init error pointer");
        return -1;
    }
    g_aec_buf_ptr     = p_buff;
    g_aec_src_buf_ptr = g_aec_buf_ptr + (tlka_aec_get_size() + 3) / 4 * 4;

    tlka_aec_init((void *)(g_aec_buf_ptr), &aec_para, (void *)(g_aec_src_buf_ptr));

    return 0;
}

/**
 * @brief       This function deinitializes the AEC algorithm.
 * @return      0 on success.
 */
int8_t tlkalg_aec_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_aec_deinit");
    g_aec_buf_ptr     = NULL;
    g_aec_src_buf_ptr = NULL;
    return 0;
}

/**
 * @brief       This function processes the audio frames using the AEC algorithm.
 * @param[in]   ps      - the source audio buffer.
 * @param[out]  pd      - the destination audio buffer.
 * @param[in]   len     - the length of the audio data.
 * @param[in]   width   - the width of the audio data.
 * @param[in]   channel - the channel number.
 * @return      1 on success, -1 on failure.
 */
int tlkalg_aec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    if (g_aec_buf_ptr == NULL || g_aec_src_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_aec_process error sturct pointer");
        return -1;
    }

    if (ps == NULL || pd == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_aec_process error data pointer");
        return -1;
    }

    if (len % aec_para.frame_size) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_aec_process error data len");
        return -1;
    }

    short *psrc = (short *)ps;
    short *pdes = (short *)pd;
    for (uint32_t i = 0; i < (len / aec_para.frame_size); i++) {
        tlka_aec_process_frame_16((void *)g_aec_buf_ptr, (short *)(psrc + i * aec_para.frame_size), (short *)(psrc + len + i * aec_para.frame_size),
                                  (short *)(pdes + i * aec_para.frame_size));
    }
    // tlka_aec_process_frame_16((void*)g_aec_buf_ptr, (short*)psrc, (short*)(psrc+len), (short*)pd);
    return 1;
}


#endif
