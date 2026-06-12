/********************************************************************************************************
 * @file    tlkalg_cvsd_interface.c
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
//#include "stack/debug/debug_internal.h"
#include <stdlib.h>
#include "tlkalg/audio/cvsd/tlkalg_cvsd_interface.h"
#include "tlkapi/tlkapi.h"


#if TLKALG_CVSD_ENABLE || TLKALG_CVSD_ENABLE_CH2

void *scratch_enc  = NULL;
void *scratch_dec  = NULL;
int   scratch_size = 0;

/**
 * @brief   Calculate the size required for the CVSD decoder buffer.
 * @param[in]   channel - Number of audio channels.
 * @return      Size of the decoder buffer in bytes, aligned to 4 bytes.
 */
uint16_t tlkalg_cvsd_dec_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_cvsd_dec_get_size(TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS) + tlka_cvsd_g711plc_get_size(TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS);

    size = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "cvsd dec size %d", size);
    return size;
}

/**
 * @brief   Calculate the size required for the CVSD encoder buffer.
 * @param[in]   channel - Number of audio channels.
 * @return      Size of the encoder buffer in bytes, aligned to 4 bytes.
 */
uint16_t tlkalg_cvsd_enc_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_cvsd_enc_get_size(TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS);
    size     = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "cvsd enc size %d", size);
    return size;
}

#endif

#if TLKALG_CVSD_ENABLE
uint8_t *g_cvsd_dec_buf_ptr = NULL;
uint8_t *g_cvsd_enc_buf_ptr = NULL;
uint8_t *g_cvsd_plc_buf_ptr = NULL;

/**
 * @brief   Initialize the CVSD decoder with the provided buffer.
 * @param[in]   p_buff  - Pointer to the buffer for decoder and PLC structures.
 * @param[in]   channel - Number of audio channels.
 * @return      0 on error, non-zero on success.
 */
int8_t tlkalg_cvsd_dec_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_cvsd_dec_init error");
        return 0;
    }
    g_cvsd_dec_buf_ptr = p_buff;
    g_cvsd_plc_buf_ptr = p_buff + tlka_cvsd_dec_get_size(TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS);

    int ret = tlka_cvsd_dec_init(g_cvsd_dec_buf_ptr, TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS);
    tlka_cvsd_g711plc_init((TLKA_CVSD_PLC *)g_cvsd_plc_buf_ptr, TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS);

    return ret;
}

/**
 * @brief   Deinitialize the CVSD decoder.
 * @return      Always returns 0.
 */
int8_t tlkalg_cvsd_dec_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_cvsd_dec_deinit");
    g_cvsd_dec_buf_ptr = NULL;
    g_cvsd_plc_buf_ptr = NULL;
    return 0;
}

/**
 * @brief   Process the CVSD decoding and packet loss concealment.
 * @param[in]   ps      - Pointer to the source buffer containing CVSD encoded data.
 * @param[out]  pd      - Pointer to the destination buffer for decoded PCM data.
 * @param[in]   len     - Length of the source buffer in bytes.
 * @param[in]   width   - Width of the audio data (not used).
 * @param[in]   channel - Number of audio channels (not used).
 * @return      Always returns 120.
 */
int tlkalg_cvsd_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "cvsd dec input buff point null");
        return 0;
    }

    if (g_cvsd_dec_buf_ptr == NULL || g_cvsd_plc_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "cvsd dec struct point null");
        return 0;
    }

    if (ps[0] == BT_VOICE_FLAG_PACKET_LOSS) {
        cvsd_dec_mute(g_cvsd_dec_buf_ptr, (int16_t *)pd, len * 2);
        tlka_cvsd_g711plc_dofe((TLKA_CVSD_PLC *)g_cvsd_plc_buf_ptr, (int16_t *)pd);

        // tlkapi_trace(0xFFFFFFFF, "[TEST]", "cvsd dec plc");

        // DBG_COMMON_CHN8_HIGH;
        // DBG_COMMON_CHN8_LOW;
    } else {
        scratch_size = tlka_cvsd_dec_get_scratch_size();
        scratch_dec  = (void *)tlkalg_malloc_func(scratch_size);

        tlka_cvsd_dec_process(g_cvsd_dec_buf_ptr, ps + 2, len, (int16_t *)pd, scratch_dec);
        tlka_cvsd_g711plc_addtohistory((TLKA_CVSD_PLC *)g_cvsd_plc_buf_ptr, (int16_t *)pd);

        if (scratch_dec != NULL) {
            tlkalg_free_func(scratch_dec);
            scratch_dec = NULL;
        }
    }

    return 120;
}

/**
 * @brief   Initialize the CVSD encoder with the provided buffer.
 * @param[in]   p_buff  - Pointer to the buffer for encoder structure.
 * @param[in]   channel - Number of audio channels.
 * @return      0 on error, non-zero on success.
 */
int8_t tlkalg_cvsd_enc_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_cvsd_enc_init error");
        return 0;
    }
    g_cvsd_enc_buf_ptr = p_buff;

    int ret = tlka_cvsd_enc_init(g_cvsd_enc_buf_ptr, TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS);

    return ret;
}

/**
 * @brief   Deinitialize the CVSD encoder.
 * @return      Always returns 0.
 */
int8_t tlkalg_cvsd_enc_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_cvsd_enc_deinit");
    g_cvsd_enc_buf_ptr = NULL;
    return 0;
}

/**
 * @brief   Process the CVSD encoding.
 * @param[in]   ps      - Pointer to the source buffer containing PCM data.
 * @param[out]  pd      - Pointer to the destination buffer for CVSD encoded data.
 * @param[in]   len     - Length of the source buffer in bytes.
 * @param[in]   width   - Width of the audio data (not used).
 * @param[in]   channel - Number of audio channels (not used).
 * @return      TRUE on success.
 */
int tlkalg_cvsd_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "cvsd enc input buff point null");
        return 0;
    }

    if (g_cvsd_enc_buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "cvsd enc struct point null");
        return 0;
    }

    scratch_size = tlka_cvsd_enc_get_scratch_size();
    scratch_enc  = (void *)tlkalg_malloc_func(scratch_size);

    tlka_cvsd_enc_process(g_cvsd_enc_buf_ptr, (int16_t *)ps, len / 2, pd, scratch_enc);
    if (scratch_enc != NULL) {
        tlkalg_free_func(scratch_enc);
        scratch_enc = NULL;
    }

    return TRUE;
}
#endif


#if TLKALG_CVSD_ENABLE_CH2
uint8_t *g_cvsd_dec_ch2buf_ptr = NULL;
uint8_t *g_cvsd_enc_ch2buf_ptr = NULL;
uint8_t *g_cvsd_plc_ch2buf_ptr = NULL;

int8_t tlkalg_cvsd_dec_ch2_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_cvsd_dec_ch2_init error");
        return 0;
    }
    g_cvsd_dec_ch2buf_ptr = p_buff;
    g_cvsd_plc_ch2buf_ptr = p_buff + tlka_cvsd_dec_get_size(TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS);

    int ret = tlka_cvsd_dec_init(g_cvsd_dec_ch2buf_ptr, TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS);
    tlka_cvsd_g711plc_init((TLKA_CVSD_PLC *)g_cvsd_plc_ch2buf_ptr, TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS);

    return ret;
}

int8_t tlkalg_cvsd_dec_ch2_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_cvsd_dec_ch2_deinit");
    g_cvsd_dec_ch2buf_ptr = NULL;
    g_cvsd_plc_ch2buf_ptr = NULL;
    return 0;
}

int tlkalg_cvsd_dec_ch2_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "cvsd dec ch2 input buff point null");
        return 0;
    }

    if (g_cvsd_dec_ch2buf_ptr == NULL || g_cvsd_plc_ch2buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "cvsd dec ch2 struct point null");
        return 0;
    }

    if (ps[0] == BT_VOICE_FLAG_PACKET_LOSS) {
        cvsd_dec_mute(g_cvsd_dec_ch2buf_ptr, (int16_t *)pd, len * 2);
        tlka_cvsd_g711plc_dofe((TLKA_CVSD_PLC *)g_cvsd_plc_ch2buf_ptr, (int16_t *)pd);

        // tlkapi_trace(0xFFFFFFFF, "[TEST]", "cvsd dec ch2 plc");

        // DBG_COMMON_CHN8_HIGH;
        // DBG_COMMON_CHN8_LOW;
    } else {
        scratch_size = tlka_cvsd_dec_get_scratch_size();
        scratch_dec  = (void *)tlkalg_malloc_func(scratch_size);

        tlka_cvsd_dec_process(g_cvsd_dec_ch2buf_ptr, ps + 2, len, (int16_t *)pd, scratch_dec);
        tlka_cvsd_g711plc_addtohistory((TLKA_CVSD_PLC *)g_cvsd_plc_ch2buf_ptr, (int16_t *)pd);
        if (scratch_dec != NULL) {
            tlkalg_free_func(scratch_dec);
            scratch_dec = NULL;
        }
    }

    return 120;
}

int8_t tlkalg_cvsd_enc_ch2_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_cvsd_enc_ch2_init error");
        return 0;
    }
    g_cvsd_enc_ch2buf_ptr = p_buff;

    int ret = tlka_cvsd_enc_init(g_cvsd_enc_ch2buf_ptr, TLKA_CVSD_SINGLE, TLKA_CVSD_7_5MS);

    return ret;
}

int8_t tlkalg_cvsd_enc_ch2_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_cvsd_enc_ch2_deinit");
    g_cvsd_enc_ch2buf_ptr = NULL;
    return 0;
}

int tlkalg_cvsd_enc_ch2_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "cvsd enc ch2 input buff point null");
        return 0;
    }

    if (g_cvsd_enc_ch2buf_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "cvsd enc ch2 struct point null");
        return 0;
    }
    scratch_size = tlka_cvsd_enc_get_scratch_size();
    scratch_enc  = (void *)tlkalg_malloc_func(scratch_size);

    tlka_cvsd_enc_process(g_cvsd_enc_ch2buf_ptr, (int16_t *)ps, len / 2, pd, scratch_enc);
    if (scratch_enc != NULL) {
        tlkalg_free_func(scratch_enc);
        scratch_enc = NULL;
    }

    return TRUE;
}
#endif
