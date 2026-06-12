/********************************************************************************************************
 * @file    ipc_audio.c
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
#if (TLK_MW_DSP_COMM_ENABLE)

#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkmw/audio/audio_mw_manager.h"

audio_buffer_context_t g_audio_buf_ctx[IPC_DATA_PATH_MAX];

/*
 *  | DP0 enc_audio_buff | DP0 pcm_buff | DP1 enc_audio_buff | DP1 pcm_buff |
 *
 */

/**
 * @brief       Initialize audio buffer
 * @param[in]   p_audio_buff - Pointer to audio buffer
 */
audio_ram_code void audio_buff_init(uint8_t *p_audio_buff)
{
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(IPC_DATA_PATH_0);
    uint16_t                p_buf_offset;

    // IPC data path 0 buffer
    p_audio_buf_ctx->enc_seq                   = 0;
    p_audio_buf_ctx->p_enc_audio_buff          = p_audio_buff;
    p_audio_buf_ctx->enc_audio_buff_total_len  = IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_LEN;
    p_audio_buf_ctx->enc_audio_buff_frame_size = IPC_DATA_PATH0_ENCODE_AUDIO_BUF_FRAME_SIZE;
    p_audio_buf_ctx->enc_audio_buff_num        = IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_NUM;
    p_audio_buf_ctx->enc_audio_buff_wptr       = 0;
    p_audio_buf_ctx->enc_audio_buff_rptr       = 0;
    p_audio_buf_ctx->enc_audio_data_len        = 0;

    p_buf_offset                         = IPC_DATA_PATH0_ENCODE_AUDIO_BUFFER_LEN;
    p_audio_buf_ctx->pcm_seq             = 0;
    p_audio_buf_ctx->p_pcm_buff          = p_audio_buff + p_buf_offset;
    p_audio_buf_ctx->pcm_buff_total_len  = IPC_DATA_PATH0_PCM_AUDIO_BUFFER_LEN;
    p_audio_buf_ctx->pcm_buff_frame_size = IPC_DATA_PATH0_PCM_AUDIO_BUF_FRAME_SIZE;
    p_audio_buf_ctx->pcm_buff_num        = IPC_DATA_PATH0_PCM_BUFFER_NUM;
    p_audio_buf_ctx->pcm_buff_wptr       = 0;
    p_audio_buf_ctx->pcm_buff_rptr       = 0;

    p_audio_buf_ctx->audio_data_mode    = AUDIO_BIT_24_DATA;
    p_audio_buf_ctx->audio_channel_type = AUDIO_STEREO;

    p_audio_buf_ctx = get_audio_buff_context_ptr(IPC_DATA_PATH_1);

    // IPC data path 1 buffer
    p_buf_offset                               = IPC_DATA_PATH0_LEN;
    p_audio_buf_ctx->enc_seq                   = 0;
    p_audio_buf_ctx->p_enc_audio_buff          = p_audio_buff + p_buf_offset;
    p_audio_buf_ctx->enc_audio_buff_total_len  = IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_LEN;
    p_audio_buf_ctx->enc_audio_buff_frame_size = IPC_DATA_PATH1_ENCODE_AUDIO_BUF_FRAME_SIZE;
    p_audio_buf_ctx->enc_audio_buff_num        = IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_NUM;
    p_audio_buf_ctx->enc_audio_buff_wptr       = 0;
    p_audio_buf_ctx->enc_audio_buff_rptr       = 0;
    p_audio_buf_ctx->enc_audio_data_len        = 0;

    p_buf_offset                         = IPC_DATA_PATH0_LEN + IPC_DATA_PATH1_ENCODE_AUDIO_BUFFER_LEN;
    p_audio_buf_ctx->pcm_seq             = 0;
    p_audio_buf_ctx->p_pcm_buff          = p_audio_buff + p_buf_offset;
    p_audio_buf_ctx->pcm_buff_total_len  = IPC_DATA_PATH1_PCM_AUDIO_BUFFER_LEN;
    p_audio_buf_ctx->pcm_buff_frame_size = IPC_DATA_PATH1_PCM_AUDIO_BUF_FRAME_SIZE;
    p_audio_buf_ctx->pcm_buff_num        = IPC_DATA_PATH1_PCM_BUFFER_NUM;
    p_audio_buf_ctx->pcm_buff_wptr       = 0;
    p_audio_buf_ctx->pcm_buff_rptr       = 0;
    p_audio_buf_ctx->audio_data_mode     = AUDIO_BIT_24_DATA;
    p_audio_buf_ctx->audio_channel_type  = AUDIO_STEREO;

#if 1
    p_audio_buf_ctx = get_audio_buff_context_ptr(IPC_SET_PARAM_PATH_2);

    // IPC data path 2 buffer
    p_buf_offset                               = IPC_DATA_PATH0_LEN + IPC_DATA_PATH1_LEN;
    p_audio_buf_ctx->enc_seq                   = 0;
    p_audio_buf_ctx->p_enc_audio_buff          = p_audio_buff + p_buf_offset;
    p_audio_buf_ctx->enc_audio_buff_total_len  = IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_LEN;
    p_audio_buf_ctx->enc_audio_buff_frame_size = IPC_DATA_PATH2_ENCODE_AUDIO_BUF_FRAME_SIZE;
    p_audio_buf_ctx->enc_audio_buff_num        = IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_NUM;
    p_audio_buf_ctx->enc_audio_buff_wptr       = 0;
    p_audio_buf_ctx->enc_audio_buff_rptr       = 0;
    p_audio_buf_ctx->enc_audio_data_len        = 0;

    p_buf_offset                         = IPC_DATA_PATH0_LEN + IPC_DATA_PATH1_LEN + IPC_DATA_PATH2_ENCODE_AUDIO_BUFFER_LEN;
    p_audio_buf_ctx->pcm_seq             = 0;
    p_audio_buf_ctx->p_pcm_buff          = p_audio_buff + p_buf_offset;
    p_audio_buf_ctx->pcm_buff_total_len  = IPC_DATA_PATH2_PCM_AUDIO_BUFFER_LEN;
    p_audio_buf_ctx->pcm_buff_frame_size = IPC_DATA_PATH2_PCM_AUDIO_BUF_FRAME_SIZE;
    p_audio_buf_ctx->pcm_buff_num        = IPC_DATA_PATH2_PCM_BUFFER_NUM;
    p_audio_buf_ctx->pcm_buff_wptr       = 0;
    p_audio_buf_ctx->pcm_buff_rptr       = 0;
    p_audio_buf_ctx->audio_data_mode     = AUDIO_BIT_24_DATA;
    p_audio_buf_ctx->audio_channel_type  = AUDIO_STEREO;
#endif
}

/**
 * @brief       Update audio buffer configuration
 * @param[in]   mode - Audio mode
 */
void ipc_update_audio_buff_config(uint8_t mode)
{
#if 0
    switch (mode) {
    case DSP_BT_MUSIC_AAC_MODE:
    {
        g_audio_buf_ctx.pcm_buff_frame_size = AAC_MODE_PCM_AUDIO_BUF_FRAME_SIZE;
        g_audio_buf_ctx.pcm_buff_num = PCM_AUDIO_BUFFER_LEN / g_audio_buf_ctx.pcm_buff_frame_size;
        g_audio_buf_ctx.pcm_buff_wptr = 0;
        g_audio_buf_ctx.pcm_buff_rptr = 0;
        //g_audio_buf_ctx.pcm_data_len = AAC_MODE_PCM_AUDIO_BUF_FRAME_SIZE;
        break;
    }
    case DSP_BT_MUSIC_SBC_MODE:
    {
        g_audio_buf_ctx.pcm_buff_frame_size = SBC_MODE_PCM_AUDIO_BUF_FRAME_SIZE;
        g_audio_buf_ctx.pcm_buff_num = PCM_AUDIO_BUFFER_LEN / g_audio_buf_ctx.pcm_buff_frame_size;
        g_audio_buf_ctx.pcm_buff_wptr = 0;
        g_audio_buf_ctx.pcm_buff_rptr = 0;
        //g_audio_buf_ctx.pcm_data_len = SBC_MODE_PCM_AUDIO_BUF_FRAME_SIZE;
        break;
    }
    case DSP_BT_VOICE_CVSD_MODE:
    {
        break;
    }
    case DSP_BT_VOICE_MSBC_MODE:
    {
        break;
    }
    case DSP_BT_LL_AUDIO_MODE:
    {
        break;
    }
    case DSP_BT_LL_VOICE_MODE:
    {
        break;
    }
    default:
        break;
    }
#else
    (void)mode;
#endif
}

/**
 * @brief       Get audio buffer context pointer
 * @param[in]   id - IPC data path ID
 * @return      Pointer to audio buffer context
 */
audio_ram_code audio_buffer_context_t *get_audio_buff_context_ptr(uint8_t id)
{
    return &g_audio_buf_ctx[id];
}

/**
 * @brief       Set audio data mode and channel type
 * @param[in]   mode - Audio data mode
 * @param[in]   type - Audio channel type
 * @param[in]   id - IPC data path ID
 */
audio_ram_code void d25f_set_audio_data_mode_and_channel_type(uint8_t mode, uint8_t type, uint8_t id)
{
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    p_audio_buf_ctx->audio_data_mode    = mode;
    p_audio_buf_ctx->audio_channel_type = type;
}

/**
 * @brief       Update encode buffer read pointer
 * @param[in]   id - IPC data path ID
 */
audio_ram_code void d25f_update_enc_buff_rptr(uint8_t id)
{
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    p_audio_buf_ctx->enc_audio_buff_rptr = (p_audio_buf_ctx->enc_audio_buff_rptr + 1) % p_audio_buf_ctx->enc_audio_buff_num;
}

/**
 * @brief       Update encode buffer write pointer
 * @param[in]   id - IPC data path ID
 */
audio_ram_code void d25f_update_enc_buff_wptr(uint8_t id)
{
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    p_audio_buf_ctx->enc_audio_buff_wptr = (p_audio_buf_ctx->enc_audio_buff_wptr + 1) % p_audio_buf_ctx->enc_audio_buff_num;
}

/**
 * @brief       Get number of free encode buffers
 * @param[in]   id - IPC data path ID
 * @return      Number of free encode buffers
 */
audio_ram_code int16_t d25f_get_number_of_enc_buff_free(uint8_t id)
{
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    int16_t available = p_audio_buf_ctx->enc_audio_buff_wptr - p_audio_buf_ctx->enc_audio_buff_rptr;
    if (available < 0) {
        available += p_audio_buf_ctx->enc_audio_buff_num;
    }

    return (p_audio_buf_ctx->enc_audio_buff_num - available);
}

/**
 * @brief       Get number of available PCM buffers
 * @param[in]   id - IPC data path ID
 * @return      Number of available PCM buffers
 */
audio_ram_code int16_t d25f_get_number_of_pcm_buff_available(uint8_t id)
{
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    int16_t available = p_audio_buf_ctx->pcm_buff_wptr - p_audio_buf_ctx->pcm_buff_rptr;
    if (available < 0) {
        available += p_audio_buf_ctx->pcm_buff_num;
    }

    return available;
}

/**
 * @brief       Get PCM buffer
 * @param[out]  pcm_data_len - Pointer to store PCM data length
 * @param[in]   id - IPC data path ID
 * @return      Pointer to PCM buffer
 */
audio_ram_code uint8_t *d25f_get_pcm_buff(uint16_t *pcm_data_len, uint8_t id)
{
    uint8_t                *p_data;
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);
    uint16_t                offset          = p_audio_buf_ctx->pcm_buff_wptr * p_audio_buf_ctx->pcm_buff_frame_size;

    p_data        = p_audio_buf_ctx->p_pcm_buff + offset;
    *pcm_data_len = p_audio_buf_ctx->pcm_data_len[p_audio_buf_ctx->pcm_buff_wptr];
    //    tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "output share addre: %x,%d\n", p_data,*pcm_data_len);
    return p_data;
}

/**
 * @brief       Get PCM data
 * @param[out]  p_pcm_data - Pointer to PCM data
 * @param[in]   id - IPC data path ID
 * @return      Length of PCM data
 */
audio_ram_code uint16_t d25f_get_pcm_data(uint8_t *p_pcm_data, uint8_t id)
{
    (void)p_pcm_data;
    //uint8_t *p_data;
    uint16_t                pcm_data_len;
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);
    //uint16_t offset = p_audio_buf_ctx->pcm_buff_rptr * p_audio_buf_ctx->pcm_buff_frame_size;

    if (p_audio_buf_ctx->pcm_buff_rptr == p_audio_buf_ctx->pcm_buff_wptr) {
        return 0;
    } else {
        //p_data = p_audio_buf_ctx->p_pcm_buff + offset;
        pcm_data_len = p_audio_buf_ctx->pcm_data_len[p_audio_buf_ctx->pcm_buff_rptr];

        p_audio_buf_ctx->pcm_buff_rptr++;
        if (p_audio_buf_ctx->pcm_buff_rptr == p_audio_buf_ctx->pcm_buff_num) {
            p_audio_buf_ctx->pcm_buff_rptr = 0;
        }
        //p_pcm_data = p_data;

        return pcm_data_len;
    }
}

/**
 * @brief       Get encode buffer pointer
 * @param[in]   id - IPC data path ID
 * @return      Pointer to encode buffer
 */
audio_ram_code uint8_t *d25f_get_enc_buff_ptr(uint8_t id)
{
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    uint16_t offset    = p_audio_buf_ctx->enc_audio_buff_wptr * p_audio_buf_ctx->enc_audio_buff_frame_size;
    uint8_t *p_enc_buf = p_audio_buf_ctx->p_enc_audio_buff + offset;

    return p_enc_buf;
}

/**
 * @brief       Get encode buffer by fixed write pointer
 * @param[in]   wptr - Write pointer
 * @param[in]   id - IPC data path ID
 * @return      Pointer to encode buffer
 */
audio_ram_code uint8_t *d25f_get_enc_buff_by_fixed_wptr(uint8_t wptr, uint8_t id)
{
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    uint16_t offset    = wptr * p_audio_buf_ctx->enc_audio_buff_frame_size;
    uint8_t *p_enc_buf = p_audio_buf_ctx->p_enc_audio_buff + offset;

    return p_enc_buf;
}

/**
 * @brief       Post audio data to encode buffer
 * @param[in]   p_data - Pointer to audio data
 * @param[in]   len - Length of audio data
 * @param[in]   id - IPC data path ID
 */
audio_ram_code void d25f_post_audio_data_to_enc_buff(uint8_t *p_data, uint16_t len, uint8_t id)
{
    (void)p_data;
    (void)len;
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    p_audio_buf_ctx->enc_audio_buff_wptr++;

    if (p_audio_buf_ctx->enc_audio_buff_wptr >= p_audio_buf_ctx->enc_audio_buff_num) {
        p_audio_buf_ctx->enc_audio_buff_wptr = 0;
    }
}

/**
 * @brief       Test function for posting audio data
 * @param[in]   id - IPC data path ID
 */
audio_ram_code void d25f_post_audio_data_test(uint8_t id)
{
    uint8_t audio_src[1024];
    uint8_t data[6]                         = {0, 0, 0, 0, 0, 0};
    data[0]                                 = 0; // mode
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    for (uint8_t i = 0; i < 16; i++) {
        uint8_t tmp = 0x50;
        tmp += i;
        memset(audio_src, tmp, 1024);
        data[1] = i; // wptr
        data[2] = p_audio_buf_ctx->enc_audio_buff_frame_size & 0xff;
        data[3] = (p_audio_buf_ctx->enc_audio_buff_frame_size >> 8) & 0xff;

        delay_ms(50);
        if (d25f_get_number_of_enc_buff_free(id)) {
            d25f_post_audio_data_to_enc_buff(audio_src, p_audio_buf_ctx->enc_audio_buff_frame_size, id);
            d25f_send_request_to_dsp(IPC_D25F2DSP_DATA_PROCESS, data, 6);
        } else {
            tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "enc buff is empty");
        }
    }
}

/**
 * @brief       Test function for posting audio data in timer
 * @param[in]   id - IPC data path ID
 */
// timer0
audio_ram_code void d25f_post_audio_data_test_in_timer(uint8_t id)
{
    uint8_t                 audio_src[1024];
    uint8_t                 data[6]         = {0, 0, 0, 0, 0, 0};
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);
    data[0]                                 = 0; // mode

    static uint8_t tmp = 0;
    tmp++;

    memset(audio_src, tmp, 1024);
    data[1] = p_audio_buf_ctx->enc_audio_buff_wptr; // wptr
    data[2] = p_audio_buf_ctx->enc_audio_buff_frame_size & 0xff;
    data[3] = (p_audio_buf_ctx->enc_audio_buff_frame_size >> 8) & 0xff;

    if (d25f_get_number_of_enc_buff_free(id)) {
        d25f_post_audio_data_to_enc_buff(audio_src, p_audio_buf_ctx->enc_audio_buff_frame_size, id);
        d25f_send_request_to_dsp(IPC_D25F2DSP_DATA_PROCESS, data, 6);
    } else {
        tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "available enc buff is empty");
    }
}

/**
 * @brief       Post sine wave audio data
 * @param[in]   p_data - Pointer to audio data
 * @param[in]   id - IPC data path ID
 */
audio_ram_code void d25f_post_sin_audio_data(uint8_t *p_data, uint8_t id)
{
    uint8_t                 data[6]         = {0, 0, 0, 0, 0, 0};
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(id);

    data[0] = 0; // mode

    data[1] = p_audio_buf_ctx->enc_audio_buff_wptr; // wptr
    data[2] = p_audio_buf_ctx->enc_audio_buff_frame_size & 0xff;
    data[3] = (p_audio_buf_ctx->enc_audio_buff_frame_size >> 8) & 0xff;

    if (d25f_get_number_of_enc_buff_free(id)) {
        d25f_post_audio_data_to_enc_buff(p_data, p_audio_buf_ctx->enc_audio_buff_frame_size, id);
        d25f_send_request_to_dsp(IPC_D25F2DSP_DATA_PROCESS, data, 6);
    } else {
        tlkapi_trace(DSP_DBG_FLAG, DSP_DBG_SIGN, "available enc buff is empty");
    }
}

#endif
