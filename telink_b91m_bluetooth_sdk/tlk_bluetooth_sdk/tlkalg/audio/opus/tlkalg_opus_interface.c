/********************************************************************************************************
 * @file    tlkalg_opus_interface.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlka_opus_api.h"
#include "tlkalg_opus_interface.h"

#if TLKALG_OPUS_ENC_ENABLE
#define OPUS_ENC_SAMPLERATE     16000
#define OPUS_ENC_CHANL          1
#ifndef OPUS_ENC_BITRATE
#define OPUS_ENC_BITRATE        32000
#endif
#define OPUS_ENC_COMPLEXITY     1
#define OPUS_ENC_FRAMESIZE      320
 
OpusEncoder *g_opus_enc_buf_ptr = NULL;
uint8_t *g_opus_enc_scr_buf_ptr = NULL;
uint16_t opus_enc_size = 0;
OPUS_CFG_EncParam opus_enc_para = {
    .channels    = OPUS_ENC_CHANL,
    .sample_rate = OPUS_ENC_SAMPLERATE,
    .bitrate_bps = OPUS_ENC_BITRATE,
    .application = OPUS_APPLICATION_AUDIO,
    .complexity  = OPUS_ENC_COMPLEXITY,
    .use_vbr     = 1,
};
 
/**
 * @brief       Calculate the required size for OPUS encoder buffer and scratch buffer.
 * @param[in]   channel - the number of channels.
 * @return      Total size required for encoder buffer and scratch buffer.
 */
uint16_t tlkalg_opus_enc_get_size(uint8_t channel)
{
	(void)channel;
    opus_enc_size = tlka_opus_encoder_get_size(OPUS_ENC_CHANL);
    opus_enc_size     = (opus_enc_size + 3) / 4 * 4;
    int scr_size = tlka_opus_enc_get_scratch_size(OPUS_ENC_CHANL, 320, 1);
    scr_size     = (scr_size + 3) / 4 * 4;
 
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "opus enc size %d, scr size %d", opus_enc_size, scr_size);
    return (opus_enc_size + scr_size);
}
 
/**
 * @brief       Initialize the OPUS encoder with the given buffer and channel.
 * @param[in]   p_buff  - the start address of the buffer for encoder.
 * @param[in]   channel - the number of channels.
 * @return      0 on success, non-zero on failure.
 */
int8_t tlkalg_opus_enc_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_opus_enc_init error, pointer null");
        return 0;
    }
 
    g_opus_enc_buf_ptr = (OpusEncoder *)p_buff;
    g_opus_enc_scr_buf_ptr = p_buff + opus_enc_size;
 
    int8_t ret = tlka_opus_encoder_init((OpusEncoder *)g_opus_enc_buf_ptr, &opus_enc_para);
 
    //set complexity
    tlka_opus_encoder_ctl(g_opus_enc_buf_ptr, OPUS_SET_COMPLEXITY_REQUEST, OPUS_ENC_COMPLEXITY);
    //set vbr
    tlka_opus_encoder_ctl((OpusEncoder *)g_opus_enc_buf_ptr, OPUS_SET_VBR_REQUEST, (1));
    tlka_opus_encoder_ctl((OpusEncoder *)g_opus_enc_buf_ptr, OPUS_SET_VBR_CONSTRAINT_REQUEST, (1));
    //set silk mode
    tlka_opus_encoder_ctl((OpusEncoder *)g_opus_enc_buf_ptr, OPUS_SET_FORCE_MODE_REQUEST, MODE_SILK_ONLY);
    return ret;
}
 
/**
 * @brief       Deinitialize the OPUS encoder.
 * @return      0 on success, non-zero on failure.
 */
int8_t tlkalg_opus_enc_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_opus_enc_deinit");
    g_opus_enc_buf_ptr = NULL;
    g_opus_enc_scr_buf_ptr = NULL;
 
    return 0;
}
 
/**
 * @brief       Process audio data using the OPUS encoder.
 * @param[in]   ps    - the start address of the source audio data buffer.
 * @param[out]  pd    - the start address of the destination buffer for encoded data.
 * @param[in]   len   - the length of the source audio data.
 * @param[in]   width - the width of the audio data.
 * @param[in]   channel- the number of channels.
 * @return      Length of the encoded data.
 */
int tlkalg_opus_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    (void)len;
    if (ps == NULL || pd == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "opus_enc PS or PD null");
        return 0;
    }
 
    if (g_opus_enc_buf_ptr == NULL || g_opus_enc_scr_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "opus_enc struct null");
        return 0;
    }
 
    int enc_len = tlka_opus_encode(g_opus_enc_buf_ptr, (const opus_int16 *)ps, OPUS_ENC_FRAMESIZE, pd+8, 1500, g_opus_enc_scr_buf_ptr);
    pd[0] = enc_len%0x100;
    pd[1] = enc_len/0x100;
    pd[2] = 0;
	pd[3] = 0;
	pd[4] = 0;
	pd[5] = 0;
	pd[6] = 0;
	pd[7] = 0;

    return enc_len;
}
#endif //TLKALG_OPUS_ENC_ENABLE
 
#if TLKALG_OPUS_DEC_ENABLE
OpusDecoder *g_opus_dec_buf_ptr = NULL;
uint8_t *g_opus_dec_scr_buf_ptr = NULL;
uint16_t opus_dec_size = 0;
OPUS_CFG_DecParam opus_dec_para = {
    .channels = 1,
    .sample_rate = 16000,
    .frame_size = 320,
};
 
/**
 * @brief       Calculate the required size for OPUS decoder buffer and scratch buffer.
 * @param[in]   channel - the number of channels.
 * @return      Total size required for decoder buffer and scratch buffer.
 */
uint16_t tlkalg_opus_dec_get_size(uint8_t channel)
{
    opus_dec_size = tlka_opus_decoder_get_size(channel);
    opus_dec_size     = (opus_dec_size + 3) / 4 * 4;
    int scr_size = tlka_opus_dec_get_scratch_size();
    scr_size     = (scr_size + 3) / 4 * 4;
 
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "opus dec size %d, scr size %d", opus_dec_size, scr_size);
    return (opus_dec_size + scr_size);
}
 
/**
 * @brief       Initialize the OPUS decoder with the given buffer and channel.
 * @param[in]   p_buff  - the start address of the buffer for decoder.
 * @param[in]   channel - the number of channels.
 * @return      0 on success, non-zero on failure.
 */
int8_t tlkalg_opus_dec_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_opus_dec_init error, pointer null");
        return 0;
    }
 
    g_opus_dec_buf_ptr = (OpusDecoder *)p_buff;
    g_opus_dec_scr_buf_ptr = p_buff + opus_dec_size;
 
    int8_t ret = tlka_opus_decoder_init(g_opus_dec_buf_ptr, &opus_dec_para);
 
    return ret;
}
 
/**
 * @brief       Deinitialize the OPUS decoder.
 * @return      0 on success, non-zero on failure.
 */
int8_t tlkalg_opus_dec_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_opus_dec_deinit");
    g_opus_dec_buf_ptr = NULL;
    g_opus_dec_scr_buf_ptr = NULL;
 
    return 0;
}
 
/**
 * @brief       Process encoded audio data using the OPUS decoder.
 * @param[in]   ps    - the start address of the source encoded audio data buffer.
 * @param[out]  pd    - the start address of the destination buffer for decoded audio data.
 * @param[in]   len   - the length of the source encoded audio data.
 * @param[in]   width - the width of the audio data.
 * @param[in]   channel- the number of channels.
 * @return      0 on success, non-zero on failure.
 */
int tlkalg_opus_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    (void)len;
    if (pd == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "opus_dec PD null");
        return 0;
    }
 
    if (g_opus_dec_buf_ptr == NULL || g_opus_dec_scr_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "opus_dec struct null");
        return 0;
    }
    if (ps == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "opus_dec PLC");
    }
    //ps==null plc running
    int ret = tlka_opus_decode((OpusDecoder *)g_opus_dec_buf_ptr, (uint8_t *)ps, len, (short *)pd, 320, 0, g_opus_dec_scr_buf_ptr);
 
    return ret;
}
#endif//TLKALG_OPUS_DEC_ENABLE
