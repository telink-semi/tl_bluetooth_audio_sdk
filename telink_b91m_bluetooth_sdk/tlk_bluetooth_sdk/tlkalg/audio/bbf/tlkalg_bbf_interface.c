/********************************************************************************************************
 * @file    tlkalg_bbf_interface.c
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
#include "tlkalg_bbf_interface.h"
#include "../tlkalg_audio_cfg.h"

//#undef TLKALG_AUD_SCRATCH_BUF_POS
//#define TLKALG_AUD_SCRATCH_BUF_POS TLKALG_AUD_SCRATCH_BUF_INDEPENDENCE

#if (TLKALG_BBF_PATH_SEL && TLKALG_BBF_ENABLE)
BBF_ALG_Param bbf_para = {
    .frame_size  = 80,
    .sample_rate = 16000,
#if (TLKALG_BBF_ENABLE == TLKALG_BBF_2CH_EN)
    .mic_num = 2,
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_4CH_EN)
    .mic_num = 4,
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN)
    .mic_num = 6,
#endif
    .ref_mic_idx = 0,
};

int bbf_ver;
int bbf_mem_size;
int bbf_scratch_size;

uint8_t *g_bbf_buf_ptr     = NULL;
uint8_t *g_bbf_src_buf_ptr = NULL;

uint16_t tlkalg_bbf_get_size(uint8_t channel)
{
    (void)channel;

    bbf_mem_size     = tlka_bbf_alg_get_size(&bbf_para);
    bbf_mem_size     = (bbf_mem_size + 3) / 4 * 4;
    bbf_scratch_size = tlka_bbf_alg_get_scratch_size();
    bbf_scratch_size = (bbf_scratch_size + 3) / 4 * 4;

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "bbf size %d, scr size %d", bbf_mem_size, bbf_scratch_size);
    return (bbf_mem_size + bbf_scratch_size);
}

int8_t tlkalg_bbf_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    (void)p_buff;

    if (g_bbf_buf_ptr != NULL) {
        return -1;
    }

    int ret = 0;

    bbf_ver = tlka_bbf_alg_get_version();

    bbf_mem_size     = tlka_bbf_alg_get_size(&bbf_para);
    bbf_mem_size     = (bbf_mem_size + 3) / 4 * 4;
    bbf_scratch_size = tlka_bbf_alg_get_scratch_size();
    bbf_scratch_size = (bbf_scratch_size + 3) / 4 * 4;
    g_bbf_buf_ptr    = tlkalg_malloc_func(bbf_mem_size);
    if (g_bbf_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "bbf buff is NULL;ERR1");
        return -1;
    }

#if TLKALG_AUD_SCRATCH_BUF_POS == TLKALG_AUD_SCRATCH_BUF_INDEPENDENCE
    g_bbf_src_buf_ptr = tlkalg_malloc_func(bbf_scratch_size);
    if (g_bbf_src_buf_ptr == NULL) {
        tlkalg_free_func(g_bbf_buf_ptr);
        g_bbf_buf_ptr = NULL;
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "bbf buff is NULL;ERR2");
        return -1;
    }
    ret = tlka_bbf_alg_init((void *)(g_bbf_buf_ptr), &bbf_para, bbf_mem_size);
#else
    ret = tlka_bbf_alg_init((void *)(g_bbf_buf_ptr), &bbf_para, bbf_mem_size);
#endif

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "bbf size %d, scr size %d,ret %d", bbf_mem_size, bbf_scratch_size, ret);
    return ret;
}

int8_t tlkalg_bbf_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_bbf_deinit");
    if (g_bbf_buf_ptr != NULL) {
        tlkalg_free_func(g_bbf_buf_ptr);
        g_bbf_buf_ptr = NULL;
    }

#if TLKALG_AUD_SCRATCH_BUF_POS == TLKALG_AUD_SCRATCH_BUF_INDEPENDENCE
    if (g_bbf_src_buf_ptr != NULL) {
        tlkalg_free_func(g_bbf_src_buf_ptr);
        g_bbf_src_buf_ptr = NULL;
    }
#endif

    return 0;
}

volatile uint8_t bbf_func_en = 1;

//len = 320: 80samples*2Byte*4chn
int tlkalg_bbf_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    (void)len;

    if (ps == NULL || pd == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "data addr is NULL;ERR3");
        return 0;
    }

    if (g_bbf_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "BBF struct is NULL;ERR4");
        return 0;
    }

    int ret = 0;

    if (bbf_func_en) {
#if (TLKALG_AUD_SCRATCH_BUF_POS == TLKALG_AUD_SCRATCH_BUF_USE_STACK)
        uint8_t ScratchBuffer[bbf_scratch_size];
        g_bbf_src_buf_ptr = ScratchBuffer;
#elif (TLKALG_AUD_SCRATCH_BUF_POS == TLKALG_AUD_SCRATCH_BUF_USE_HEAP)
        g_bbf_src_buf_ptr = tlkalg_malloc_func(bbf_scratch_size);
#endif

        ret = tlka_bbf_alg_process_frame_16(g_bbf_buf_ptr, g_bbf_src_buf_ptr, (short *)ps, (short *)pd);

#if (TLKALG_AUD_SCRATCH_BUF_POS == TLKALG_AUD_SCRATCH_BUF_USE_HEAP)
        tlkalg_free_func(scr_buf);
#endif

    } else {
        memcpy(pd, ps, 160);
    }
    return ret;
}
#endif
