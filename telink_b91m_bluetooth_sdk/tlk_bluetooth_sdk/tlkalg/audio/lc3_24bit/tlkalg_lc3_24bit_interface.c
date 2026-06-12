/********************************************************************************************************
 * @file    tlkalg_lc3_24bit_interface.c
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
#include "tlklib/dbg/tlkdbg.h"
#include "common/types.h"
#include "lc3.h"
#include "tlkalg_lc3_24bit_interface.h"
#if (TLKALG_LC3_24BIT_ENC_ENABLE || TLKALG_LC3_24BIT_DEC_ENABLE) && (!TLK_CFG_TEMP_DRAM_OPTM_TPSLL)

#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
parameters_t lc3_enc_para = {
    .frame_us    = 10000,
    .sample_rate = 48000,
    .bitrate     = 72000,
    .bitdepth    = 24,
};

parameters_t lc3_dec_para = {
    .frame_us    = 10000,
    .sample_rate = 16000,
    .bitrate     = 24000,
    .bitdepth    = 24,
};
#else
parameters_t lc3_enc_para = {
    .frame_us    = 10000,
    .sample_rate = 16000,
    .bitrate     = 24000,
    .bitdepth    = 24,
};

parameters_t lc3_dec_para = {
    .frame_us    = 10000,
    .sample_rate = 48000,
    .bitrate     = 72000,
    .bitdepth    = 24,
};
#endif

#define LC3_MAX_FRAME_BYTES 400
uint8_t       lc3_24bit_enc_out[LC3_MAX_FRAME_BYTES];
lc3_encoder_t lc3_enc_handle[2];
lc3_decoder_t lc3_dec_handle[2];
int           pdes_stereo[480 * 2]    = {0};
int           pdes_stereo_lr[480 * 2] = {0};
int           enc_frame_size          = 0;
int           dec_frame_size          = 0;

uint8_t *g_lc3_24bit_enc_buff_left  = NULL;
uint8_t *g_lc3_24bit_enc_buff_right = NULL;
uint8_t *g_lc3_24bit_dec_buff_left  = NULL;
uint8_t *g_lc3_24bit_dec_buff_right = NULL;

int pcm_sbytes = 1;
int p_dec_data[480 * 2];

/**
 * @brief       Changes the channel number for LC3 encoding/decoding.
 * @param[in]   channel_in - the input channel number.
 * @return      The adjusted channel number or -1 if the input is invalid.
 */
static int8_t tlkalg_lc3_24bit_channel_change(uint8_t channel_in)
{
    int8_t chnl_out;
    switch (channel_in) {
    case 1:
    case 2:
        chnl_out = 1;
        break;
    case 3:
        chnl_out = 2;
        break;

    default:
        chnl_out = -1;
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "lc3 24bit error channel");
        break;
    }

    return chnl_out;
}

/**
 * @brief       Initializes the LC3 24-bit encoder.
 * @param[in]   p_buff   - pointer to the buffer for encoder data.
 * @param[in]   channel  - the number of channels.
 * @return      0 on success.
 */
int8_t tlkalg_lc3_24bit_enc_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;

    int enc_size = lc3_encoder_size(lc3_enc_para.frame_us, lc3_enc_para.sample_rate);

    g_lc3_24bit_enc_buff_left  = p_buff;
    g_lc3_24bit_enc_buff_right = p_buff + enc_size;

    enc_frame_size = lc3_frame_bytes(lc3_enc_para.frame_us, lc3_enc_para.bitrate);

    tlkapi_trace(0xFFFFFFFF, "[LC3 24BIT]", "enc_size %d, enc_frame_size: %d", enc_size, enc_frame_size);

    lc3_enc_handle[0] = lc3_setup_encoder(lc3_enc_para.frame_us, lc3_enc_para.sample_rate, 0, g_lc3_24bit_enc_buff_left);
    lc3_enc_handle[1] = lc3_setup_encoder(lc3_enc_para.frame_us, lc3_enc_para.sample_rate, 0, g_lc3_24bit_enc_buff_right);

    return 0;
}

/**
 * @brief       Initializes the LC3 24-bit decoder.
 * @param[in]   p_buff   - pointer to the buffer for decoder data.
 * @param[in]   channel  - the number of channels.
 * @return      0 on success.
 */
int8_t tlkalg_lc3_24bit_dec_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;

    int dec_size = lc3_decoder_size(lc3_dec_para.frame_us, lc3_dec_para.sample_rate);

    g_lc3_24bit_dec_buff_left  = p_buff;
    g_lc3_24bit_dec_buff_right = p_buff + dec_size;

    dec_frame_size = lc3_frame_bytes(lc3_dec_para.frame_us, lc3_dec_para.bitrate);

    tlkapi_trace(0xFFFFFFFF, "[LC3 24BIT]", "dec_size %d, dec_frame_size: %d", dec_size, dec_frame_size);

    lc3_dec_handle[0] = lc3_setup_decoder(lc3_dec_para.frame_us, lc3_dec_para.sample_rate, 0, g_lc3_24bit_dec_buff_left);
    lc3_dec_handle[1] = lc3_setup_decoder(lc3_dec_para.frame_us, lc3_dec_para.sample_rate, 0, g_lc3_24bit_dec_buff_right);

    return 0;
}

/**
 * @brief       Deinitializes the LC3 24-bit encoder.
 * @return      0 on success.
 */
int8_t tlkalg_lc3_24bit_enc_deinit(void)
{
    g_lc3_24bit_enc_buff_left  = NULL;
    g_lc3_24bit_enc_buff_right = NULL;

    return 0;
}

/**
 * @brief       Deinitializes the LC3 24-bit decoder.
 * @return      0 on success.
 */
int8_t tlkalg_lc3_24bit_dec_deinit(void)
{
    g_lc3_24bit_dec_buff_left  = NULL;
    g_lc3_24bit_dec_buff_right = NULL;

    return 0;
}

/**
 * @brief       Gets the size required for LC3 24-bit encoding.
 * @param[in]   channel - the number of channels.
 * @return      The required size for encoding or 0 if the channel is invalid.
 */
uint16_t tlkalg_lc3_24bit_enc_get_size(uint8_t channel)
{
    int    enc_size = lc3_encoder_size(lc3_enc_para.frame_us, lc3_enc_para.sample_rate);
    int8_t chnl     = tlkalg_lc3_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }
    tlkapi_trace(0xFFFFFFFF, "[LC3 24BIT]", "lc3 24bit enc size %d", chnl * enc_size);
    return (chnl * enc_size);
}

/**
 * @brief       Gets the size required for LC3 24-bit decoding.
 * @param[in]   channel - the number of channels.
 * @return      The required size for decoding or 0 if the channel is invalid.
 */
uint16_t tlkalg_lc3_24bit_dec_get_size(uint8_t channel)
{
    int    dec_size = lc3_decoder_size(lc3_dec_para.frame_us, lc3_dec_para.sample_rate);
    int8_t chnl     = tlkalg_lc3_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }
    tlkapi_trace(0xFFFFFFFF, "[LC3 24BIT]", "lc3 24bit dec size %d", chnl * dec_size);
    return (chnl * dec_size);
}

/**
 * @brief       Processes the LC3 24-bit encoding.
 * @param[in]   ps    - pointer to the source PCM data.
 * @param[out]  pd    - pointer to the destination encoded data.
 * @param[in]   len   - length of the PCM data.
 * @param[in]   width - width of the PCM data.
 * @param[in]   channel - the number of channels.
 * @return      The result of the encoding process.
 */
int tlkalg_lc3_24bit_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)len;

    int *psrc = (int *)ps;
#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
    int enc_ret = lc3_encode(lc3_enc_handle[channel], LC3_PCM_FORMAT_S24, (const void *)(psrc), 1, enc_frame_size, pd);
    return enc_ret;
#else
    int enc_ret[2] = {0};

    int8_t chnl = tlkalg_lc3_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    for (int i = 0; i < chnl; i++) {
        enc_ret[i] = lc3_encode(lc3_enc_handle[i], LC3_PCM_FORMAT_S24, (const void *)(psrc + i * pcm_sbytes), chnl, enc_frame_size, pd + i * enc_frame_size);
    }

    return enc_ret[0];
#endif
}

/**
 * @brief       Processes the LC3 24-bit decoding.
 * @param[in]   ps    - pointer to the source encoded data.
 * @param[out]  pd    - pointer to the destination PCM data.
 * @param[in]   len   - length of the encoded data.
 * @param[in]   width - width of the PCM data.
 * @param[in]   channel - the number of channels.
 * @return      The result of the decoding process.
 */
int tlkalg_lc3_24bit_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;

    int dec_ret[2] = {0};

    int8_t chnl = tlkalg_lc3_24bit_channel_change(channel & 0x0F);
    if (chnl == -1) {
        return 0;
    }

#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
    (void)len;
    for (int j = 0; j < chnl; j++) {
        dec_ret[j] = lc3_decode(lc3_dec_handle[j], (const void *)(ps + j * dec_frame_size), dec_frame_size, LC3_PCM_FORMAT_S24, pd, chnl);
    }
#else
    (void)len;
    for (int j = 0; j < chnl; j++) {
        dec_ret[j] = lc3_decode(lc3_dec_handle[j], (const void *)(ps + j * dec_frame_size), dec_frame_size, LC3_PCM_FORMAT_S24, (pdes_stereo + j * pcm_sbytes), chnl);
    }

    if (chnl == 1) {
        for (int k = 0; k < 480; k++) {
            pdes_stereo_lr[k] = pdes_stereo[k];
        }
    } else {
        for (int k = 0; k < 480; k++) {
            pdes_stereo_lr[k]       = pdes_stereo[2 * k];
            pdes_stereo_lr[480 + k] = pdes_stereo[2 * k + 1];
        }
    }

    uint8_t *pdes = (uint8_t *)pdes_stereo_lr;
    if (chnl == 1) {
        for (int k = 0; k < 480 * 4; k++) {
            pd[k] = pdes[k];
        }
    } else {
        for (int k = 0; k < 480 * 4 * 2; k++) {
            pd[k] = pdes[k];
        }
    }
#endif
    if (dec_ret[0] != 0) {
        tlkapi_trace(0xFFFFFFFF, "[LC3 24BIT]", "lc3 24bit dec_ret:  %d", dec_ret[0]);
    }

    return dec_ret[0];
}
#endif
