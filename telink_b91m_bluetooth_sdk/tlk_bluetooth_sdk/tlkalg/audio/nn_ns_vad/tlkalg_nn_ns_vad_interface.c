/********************************************************************************************************
 * @file    tlkalg_nn_ns_vad_interface.c
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
#include "../tlkalg_audio_cfg.h"


#include "tlkalg_nn_ns_vad_interface.h"

#if (TLKALG_NN_NS_VAD_ENABLE)

uint8_t        *g_nn_ns_vad_buf_ptr = NULL;
NN_NS_PARA_STRU g_nn_ns_vad_para    = {
       .n_fft          = 1024,
       .n_framesize    = 320,
       .n_target_level = target_unlimited,
       .n_vad_level    = vad_056,
};

int   nn_ns_vad_size          = 0;
int   nn_ns_vad_scratch_size  = 0;
void *g_nn_ns_vad_src_buf_ptr = NULL;

/**
 * @brief       This function calculates the required buffer size for the neural network noise suppression and voice activity detection algorithm.
 * @param[in]   channel - the audio channel, not used in this implementation.
 * @return      The size of the buffer required for the algorithm.
 */
uint16_t tlkalg_nn_ns_get_size(uint8_t channel)
{
    (void)channel;

    nn_ns_vad_size = tlka_nn_ns_get_size();
    nn_ns_vad_size = (nn_ns_vad_size + 3) / 4 * 4; //4 Byte align

    nn_ns_vad_scratch_size = (tlka_nn_ns_get_scratch_buf_size() + 3) / 4 * 4;

    //	tlkapi_trace(0xFFFFFFFF, "[TEST]", "NN_NS size %d,scratch_size= %d", nn_ns_vad_size, nn_ns_vad_scratch_size);
    return nn_ns_vad_size;
}

/**
 * @brief       This function initializes the neural network noise suppression and voice activity detection algorithm.
 * @param[in]   p_buff  - pointer to the buffer allocated for the algorithm.
 * @param[in]   channel - the audio channel, not used in this implementation.
 * @return      The result of the initialization, 0 if successful, non-zero if an error occurred.
 */
int8_t tlkalg_nn_ns_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_nn_ns_init error, pointer null");
        return 0;
    }

    g_nn_ns_vad_buf_ptr = p_buff;

    int8_t ret = tlka_nn_ns_init((NN_NS_STRU *)g_nn_ns_vad_buf_ptr, (NN_NS_PARA_STRU *)&g_nn_ns_vad_para);

    return ret;
}

/**
 * @brief       This function deinitializes the neural network noise suppression and voice activity detection algorithm.
 * @return      The result of the deinitialization, always 0.
 */
int8_t tlkalg_nn_ns_deinit(void)
{
    tlka_nn_ns_free((NN_NS_STRU *)g_nn_ns_vad_buf_ptr);
    //	g_nn_ns_vad_buf_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_ann_ns_deinit");

    return 0;
}

/**
 * @brief       This function sets parameters for the neural network noise suppression and voice activity detection algorithm.
 * @param[in]   type    - the type of parameter to set, not used in this implementation.
 * @param[in]   param   - pointer to the parameter value to set.
 * @return      The result of setting the parameter, 0 if successful, non-zero if an error occurred.
 */
uint8_t tlkalg_nn_ns_set_param(uint8_t type, void *param)
{
    (void)type;

    uint8_t result = 0;
    int    *val    = (int *)param;

    if (*val < target_end) {
        result = tlka_nn_ns_set_property((NN_NS_STRU *)g_nn_ns_vad_buf_ptr, NN_NS_SET_TARGET_LEVEL, param);
    } else {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_nn_ns_set_param error:Parameters are not supported");
    }

    return result;
}

/**
 * @brief       This function processes a frame of audio data using the neural network noise suppression and voice activity detection algorithm.
 * @param[in]   ps      - pointer to the source audio data.
 * @param[out]  pd      - pointer to the destination audio data.
 * @param[in]   len     - the length of the audio data, not used in this implementation.
 * @param[in]   width   - the width of the audio data, not used in this implementation.
 * @param[in]   channel - the audio channel, not used in this implementation.
 * @return      The result of processing the audio frame, 0 if successful, non-zero if an error occurred.
 */
int tlkalg_nn_ns_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    (void)len;
    int ret = 0xff;

    if (ps == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_nn_ns_process:PS null");
        return 0;
    }

#if (TLKALG_AUD_SCRATCH_BUF_POS == TLKALG_AUD_SCRATCH_BUF_USE_STACK)
    uint8_t srcatch_buff[nn_ns_vad_scratch_size];
    g_nn_ns_vad_src_buf_ptr = (void *)srcatch_buff;

    ret = tlka_nn_ns_process_frame((NN_NS_STRU *)g_nn_ns_vad_buf_ptr, (short *)ps, (int *)pd, g_nn_ns_vad_src_buf_ptr);
//		tlkapi_trace(0xFFFFFFFF, "[TEST]", "srcatch_buff:%x,ret:%d,VAD;%d",g_nn_ns_vad_src_buf_ptr,ret,*pd);
#elif (TLKALG_AUD_SCRATCH_BUF_POS == TLKALG_AUD_SCRATCH_BUF_USE_HEAP)
    g_nn_ns_vad_src_buf_ptr = tlkalg_malloc_func(nn_ns_vad_scratch_size);
    if (g_nn_ns_vad_src_buf_ptr != NULL) {
        ret = tlka_nn_ns_process_frame((NN_NS_STRU *)g_nn_ns_vad_buf_ptr, (short *)ps, (int *)pd, g_nn_ns_vad_src_buf_ptr);
        tlkalg_free_func(g_nn_ns_vad_src_buf_ptr);
    } else {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_nn_ns_process:struct null");
    }
#endif

    return ret;
}

#endif
