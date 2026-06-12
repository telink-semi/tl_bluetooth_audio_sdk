/********************************************************************************************************
 * @file    tlkalg_vad_interface.c
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
#include "tlkalg_vad_interface.h"

#if (TLKALG_VAD_ENABLE)

void *g_vad_buf_ptr = NULL;

/**
 * @brief   Get the size required for VAD buffer.
 * @param[in]   channel - the channel number (unused in current implementation).
 * @return      The size of the VAD buffer.
 */
uint16_t tlkalg_vad_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_vad_get_size();
    size     = (size + 3) / 4 * 4;

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "vad size %d", size);
    return size;
}

/**
 * @brief   Initialize the VAD module.
 * @param[in]   p_buff - pointer to the buffer used by VAD.
 * @param[in]   channel - the channel number (unused in current implementation).
 * @return      Return value from tlka_vad_init.
 */
int8_t tlkalg_vad_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;

    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_vad_init error, pointer null");
        return 0;
    }

    g_vad_buf_ptr = (void *)p_buff;
    int freq      = 16000 / 1000;

    int ret                                               = tlka_vad_init(g_vad_buf_ptr, freq, 1);
    ((tlka_vad_param *)g_vad_buf_ptr)->pwr_diffthd_linear = 4;
    ((tlka_vad_param *)g_vad_buf_ptr)->pwr_shortterm_thd  = 50000;

    return ret;
}

/**
 * @brief   Deinitialize the VAD module.
 * @return      Always returns 0.
 */
int8_t tlkalg_vad_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_vad_deinit");
    g_vad_buf_ptr = NULL;

    return 0;
}

/**
 * @brief   Process a frame of audio data using VAD.
 * @param[in]   ps - pointer to the source audio data.
 * @param[out]  pd - pointer to the destination buffer (unused in current implementation).
 * @param[in]   len - length of the audio data (unused in current implementation).
 * @param[in]   width - width of the audio data (unused in current implementation).
 * @param[in]   channel - the channel number (unused in current implementation).
 * @return      Return value from tlka_vad_process_frame.
 */
int tlkalg_vad_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)pd;
    (void)len;
    (void)width;
    (void)channel;
    if (ps == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_vad_PS null");
        return 0;
    }

    if (g_vad_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_vad_struct null");
        return 0;
    }

    int vad_en = 1;

    int ret = tlka_vad_process_frame(g_vad_buf_ptr, (short *)ps, vad_en);


    return ret;
}

#endif
