/********************************************************************************************************
 * @file    tlkalg_hybrid_interface.c
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
#include "tlka_hybrid_alg_api.h"
#include "tlkalg_hybrid_interface.h"

#if TLK_ALG_HYBRID_ENABLE
int      hybrid_delay          = HYBRID_DELAY;
char     alg_ctrl              = AEC_CTRL_MASK | NS_CTRL_MASK;
void    *st_hybrid             = NULL;
int     *ScratchBuffer         = NULL;
uint8_t *g_hybrid_buff_ptr     = NULL;
uint8_t *g_hybrid_scr_buff_ptr = NULL;

GSC_Param gsc_para = {
    .frame_size   = 120,
    .sampleRate   = 16000,
    .exchange_mic = 0,
};

AEC_Param aec_para = {
    .frame_size  = 120,
    .sampleRate  = 16000,
    .en_aec_post = 1,
};

W_NS_CFG_PARAM ns_para = {
    .frame_size   = 120,
    .sampleRate   = 16000,
    .target_level = k21dB,
};

HYBRID_ALG_Param hybrid_para = {
    .sampleRate = 16000,
    .frame_size = 120,
    .gsc_para   = &gsc_para,
    .aec_para   = &aec_para,
    .ns_para    = &ns_para,
};

/**
 * @brief       Calculate the required buffer size for the hybrid algorithm.
 * @param[in]   channel - the channel number, not used in this function.
 * @return      The total size of the buffer required for the hybrid algorithm.
 */
uint16_t tlkalg_hybrid_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_hybrid_alg_get_size();
    size     = (size + 3) / 4 * 4;

    int scr_size = tlka_hybrid_alg_get_scratch_size();
    scr_size     = (scr_size + 3) / 4 * 4;

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "hybrid size %d %d", size, scr_size);
    return (size + scr_size);
}

/**
 * @brief       Initialize the hybrid algorithm with the given buffer.
 * @param[in]   p_buff  - pointer to the buffer to be used by the hybrid algorithm.
 * @param[in]   channel - the channel number, not used in this function.
 * @return      0 on success, non-zero on error.
 */
int8_t tlkalg_hybrid_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_hybrid_init error, pointer null");
        return 0;
    }

    g_hybrid_buff_ptr     = p_buff;
    g_hybrid_scr_buff_ptr = p_buff + ((tlka_hybrid_alg_get_size() + 3) / 4 * 4);

    int ret = tlka_hybrid_alg_init(g_hybrid_buff_ptr, &hybrid_para, g_hybrid_scr_buff_ptr, tlka_hybrid_alg_get_size());

    return ret;
}

/**
 * @brief       Deinitialize the hybrid algorithm.
 * @return      0 on success, non-zero on error.
 */
int8_t tlkalg_hybrid_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_hybrid_deinit");
    g_hybrid_buff_ptr     = NULL;
    g_hybrid_scr_buff_ptr = NULL;

    return 0;
}

/**
 * @brief       Process a frame of audio data using the hybrid algorithm.
 * @param[in]   ps      - pointer to the source audio data buffer.
 * @param[out]  pd      - pointer to the destination audio data buffer.
 * @param[in]   len     - length of the audio frame in samples.
 * @param[in]   width   - width of the audio data, not used in this function.
 * @param[in]   channel - the channel number, not used in this function.
 * @return      0 on success, non-zero on error.
 */
int tlkalg_hybrid_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    if (ps == NULL || pd == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_hybrid_PS or PD null");
        return 0;
    }

    if (g_hybrid_buff_ptr == NULL || g_hybrid_scr_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_hybrid_struct null");
        return 0;
    }

    //ps:micL+micR+spk
    tlka_hybrid_alg_process_frame((void *)g_hybrid_buff_ptr, alg_ctrl, (short *)ps, (short *)(ps + len), (short *)pd);

    return 0;
}
#endif
