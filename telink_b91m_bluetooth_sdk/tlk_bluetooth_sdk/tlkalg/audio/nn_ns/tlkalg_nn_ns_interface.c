/********************************************************************************************************
 * @file    tlkalg_nn_ns_interface.c
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
#include "tlkalg_nn_ns_interface.h"

#if (TLKALG_NN_NS_ENABLE)

uint8_t        *g_nn_ns_buf_ptr = NULL;
NN_NS_PARA_STRU g_nn_ns_para    = {
       .n_fft          = 1024,
       .n_framesize    = 320,
       .n_target_level = target_unlimited,
};

/**
 * @brief       This function calculates the size required for the noise suppression algorithm buffer.
 * @param[in]   channel - The channel for which the size is being calculated.
 * @return      The size of the buffer in bytes.
 */
uint16_t tlkalg_nn_ns_get_size(uint8_t channel)
{
    (void)channel;

    uint16_t size = tlka_nn_ns_get_size();
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "NN_NS size %d", size);
    return size;
}

/**
 * @brief       This function initializes the noise suppression algorithm with a given buffer.
 * @param[in]   p_buff  - Pointer to the buffer used for the algorithm.
 * @param[in]   channel - The channel to initialize.
 * @return      The result of the initialization (0 for error, non-zero for success).
 */
int8_t tlkalg_nn_ns_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_nn_ns_init error, pointer null");
        return 0;
    }

    g_nn_ns_buf_ptr = p_buff;

    int8_t ret = tlka_nn_ns_init((NN_NS_STRU *)g_nn_ns_buf_ptr, (NN_NS_PARA_STRU *)&g_nn_ns_para);

    return ret;
}

/**
 * @brief   This function deinitializes the noise suppression algorithm and frees the associated buffer.
 * @return      The result of the deinitialization (always 0 in this implementation).
 */
int8_t tlkalg_nn_ns_deinit(void)
{
    tlka_nn_ns_free((NN_NS_STRU *)g_nn_ns_buf_ptr);
    //	g_nn_ns_buf_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_ann_ns_deinit");

    return 0;
}

/**
 * @brief   This function sets the parameters for the noise suppression algorithm.
 * @param[in]   type    - Type of the parameter to set.
 * @param[in]   param   - Pointer to the parameter value to set.
 * @return      The result of the parameter setting (0 for error, non-zero for success).
 */
uint8_t tlkalg_nn_ns_set_param(uint8_t type, void *param)
{
    (void)type;

    uint8_t result = 0;
    int    *val    = (int *)param;

    if (*val < target_end) {
        result = tlka_nn_ns_set_property((NN_NS_STRU *)g_nn_ns_buf_ptr, NN_NS_SET_TARGET_LEVEL, param);
    } else {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_nn_ns_set_param error:Parameters are not supported");
    }

    return result;
}

/**
 * @brief   This function processes a frame of audio data for noise suppression.
 * @param[in]   ps      - Pointer to the source audio data.
 * @param[in]   pd      - Pointer to the destination audio data.
 * @param[in]   len     - Length of the audio data frame.
 * @param[in]   width   - Width of the audio data.
 * @param[in]   channel - Channel number of the audio data.
 * @return      The result of the audio processing (0 for error, non-zero for success).
 */
int tlkalg_nn_ns_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    (void)len;
    (void)pd;
    if (ps == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_nn_ns_process:PS null");
        return 0;
    }

    if (g_nn_ns_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_nn_ns_process:struct null");
        return 0;
    }

    int ret = tlka_nn_ns_process_frame((NN_NS_STRU *)g_nn_ns_buf_ptr, (short *)ps);

    return ret;
}

#endif
