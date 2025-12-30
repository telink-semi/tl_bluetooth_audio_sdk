/********************************************************************************************************
 * @file    tlkalg_lc3_plus_interface.c
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
#include "tlkalg_lc3_plus_interface.h"

#if TLKALG_LC3_PLUS_ENC_ENABLE || TLKALG_LC3_PLUS_DEC_ENABLE

#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
LC3P_ENC_CFG_Param lc3p_enc_param = {
    .samplerate   = LC3P_SR_48K,
    .hrmode       = 1,
    .channels     = LC3P_CHN_SINGLE,
    .bitrate      = LC3P_BITRATE_72k, //if stereo, bitrate need multiply 2
    .frame_dms    = LC3_FRAME_DURATION_10MS,
    .wavFormat_in = LC3_WAV_FORMAT_INT24,
    .lfe          = 0,
};

LC3P_DEC_CFG_Param lc3p_dec_param = {
    .samplerate    = LC3P_SR_16K,
    .hrmode        = 0,
    .channels      = LC3P_CHN_SINGLE,
    .plc_method    = LC3PLUS_PLCMETH_ADV_PHECU_TDC_NS,
    .frame_dms     = LC3_FRAME_DURATION_10MS,
    .wavFormat_out = LC3_WAV_FORMAT_INT24,
};
#else
#if TLKALG_ALG_LOOPBACK_TEST_ENABLE
LC3P_ENC_CFG_Param lc3p_enc_param = {
    .samplerate   = LC3P_SR_48K,
    .hrmode       = 1,
    .channels     = LC3P_CHN_SINGLE,
    .bitrate      = LC3P_BITRATE_72k, //if stereo, bitrate need multiply 2
    .frame_dms    = LC3_FRAME_DURATION_10MS,
    .wavFormat_in = LC3_WAV_FORMAT_INT24,
    .lfe          = 0,
};
#else
LC3P_ENC_CFG_Param lc3p_enc_param = {
    .samplerate   = LC3P_SR_16K,
    .hrmode       = 0,
    .channels     = LC3P_CHN_SINGLE,
    .bitrate      = LC3P_BITRATE_24k, //if stereo, bitrate need multiply 2
    .frame_dms    = LC3_FRAME_DURATION_10MS,
    .wavFormat_in = LC3_WAV_FORMAT_INT24,
    .lfe          = 0,
};
#endif
LC3P_DEC_CFG_Param lc3p_dec_param = {
    .samplerate    = LC3P_SR_48K,
    .hrmode        = 1,
    .channels      = LC3P_CHN_SINGLE,
    .plc_method    = LC3PLUS_PLCMETH_ADV_PHECU_TDC_NS,
    .frame_dms     = LC3_FRAME_DURATION_10MS,
    .wavFormat_out = LC3_WAV_FORMAT_INT24,
};
#endif

LC3P_ENC_CFG_Param *P_lc3p_enc_param = &lc3p_enc_param;
LC3P_DEC_CFG_Param *P_lc3p_dec_param = &lc3p_dec_param;

int32_t frame_plc    = 1;
int32_t nBytes       = 0;
int32_t bfi_ext      = 0;
int32_t int_data_tmp = 0;
int     nSamples_dec = 0;
int     nSamples_enc = 0;
int     real_bitrate = 0;

int decoder_size     = 0;
int encoder_size     = 0;
int scratch_size_enc = 0;
int scratch_size_dec = 0;

LC3PLUS_Enc *g_encoder[2] = {NULL, NULL};
LC3PLUS_Dec *g_decoder[2] = {NULL, NULL};
void        *enc_scratch  = NULL;
void        *dec_scratch  = NULL;

LC3PLUS_ErrorCode err_enc[2] = {LC3PLUS_OK, LC3PLUS_OK};
LC3PLUS_ErrorCode err_dec[2] = {LC3PLUS_OK, LC3PLUS_OK};
LC3PLUS_ErrorCode err[2]     = {LC3PLUS_OK, LC3PLUS_OK};

int32_t lc3plus_sample_buf[960 * 2] = {0};
int32_t lc3plus_buf_24[960 * 2]     = {0};

int32_t *input_int24_or_int32[LC3PLUS_MAX_CHANNELS];
int32_t *output_int24_or_int32[LC3PLUS_MAX_CHANNELS];

/**
 * @brief       Set LC3 Plus encoder parameters
 * @param[in]   type - Encoder type
 * @param[in]   param - Pointer to parameter structure
 * @return      Operation result, 1 for success, 0 for failure
 */
uint8_t tlkalg_lc3_plus_enc_set_param(uint8_t type, void *param)
{
    if (param == NULL) {
        //		tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "tlkalg_lc3_plus_enc_set_param");
    }
    switch (type) {
    case LC3_PLUS_TYPE_ENC_NORMAL:
    {
        lc3p_enc_param.frame_dms = LC3_FRAME_DURATION_10MS;
#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
        lc3p_enc_param.bitrate = LC3P_BITRATE_72k;
#else
#if TLKALG_ALG_LOOPBACK_TEST_ENABLE
        lc3p_enc_param.bitrate = LC3P_BITRATE_72k;
#else
        lc3p_enc_param.bitrate = LC3P_BITRATE_24k;
#endif
#endif
    } break;
    case LC3_PLUS_TYPE_ENC_ULTRA_LOW_LATENCY:
    {
        lc3p_enc_param.frame_dms = LC3_FRAME_DURATION_5MS;
#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
        lc3p_enc_param.bitrate = LC3P_BITRATE_80k;
#else
        lc3p_enc_param.bitrate = LC3P_BITRATE_32k;
#endif
    } break;
    case LC3_PLUS_TYPE_ENC_UNKNOWN:
    default:
    {
        return 0;
    } break;
    }

    tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "tlkalg_lc3_plus_enc_set_param: %d", lc3p_enc_param.frame_dms);

    return 1;
}

/**
 * @brief       Set LC3 Plus decoder parameters
 * @param[in]   type - Decoder type
 * @param[in]   param - Pointer to parameter structure
 * @return      Operation result, 1 for success, 0 for failure
 */
uint8_t tlkalg_lc3_plus_dec_set_param(uint8_t type, void *param)
{
    if (param == NULL) {}

    switch (type) {
    case LC3_PLUS_TYPE_DEC_NORMAL:
    {
        lc3p_dec_param.frame_dms = LC3_FRAME_DURATION_10MS;
    } break;
    case LC3_PLUS_TYPE_DEC_ULTRA_LOW_LATENCY:
    {
        lc3p_dec_param.frame_dms = LC3_FRAME_DURATION_5MS;
    } break;
    case LC3_PLUS_TYPE_DEC_UNKNOWN:
    default:
    {
        return 0;
    } break;
    }

    tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "tlkalg_lc3_plus_dec_set_param: %d", lc3p_dec_param.frame_dms);
    return 1;
}

/**
 * @brief       Change LC3 Plus encoder channel count
 * @param[in]   channel_in - Input channel count
 * @return      Output channel count
 */
uint8_t tlkalg_lc3_plus_enc_channel_change(uint8_t channel_in)
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
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "lc3 plus enc error channel");
        break;
    }

    return chnl_out;
}

/**
 * @brief       Change LC3 Plus decoder channel count
 * @param[in]   channel_in - Input channel count
 * @return      Output channel count
 */
uint8_t tlkalg_lc3_plus_dec_channel_change(uint8_t channel_in)
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
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "lc3 plus dec error channel");
        break;
    }

    return chnl_out;
}

/**
 * @brief       Initialize LC3 Plus encoder
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      Operation result, 0 for success
 */
int8_t tlkalg_lc3_plus_enc_init(uint8_t *p_buff, uint8_t channel)
{
    uint8_t chnl_in      = channel & 0x0F;
    uint8_t size_type_in = (channel >> 4) & 0x0F;

    uint8_t chnl_out = tlkalg_lc3_plus_enc_channel_change(chnl_in);

    if (size_type_in == 1) { //encoder buff init
        g_encoder[0] = (LC3PLUS_Enc *)p_buff;
        g_encoder[1] = (LC3PLUS_Enc *)((uint8_t *)p_buff + encoder_size);

        for (int i = 0; i < chnl_out; i++) {
            err[i] = tlka_lc3a_enc_init(g_encoder[i], P_lc3p_enc_param);
            if (err[i] != LC3PLUS_OK) {
                tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "err_enc_init[%d]: %d", i, err[i]);
            }

            nSamples_enc = tlka_lc3a_enc_get_input_samples(g_encoder[i]);
            tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "nSamples_enc: %d", nSamples_enc);

            real_bitrate = tlka_lc3a_enc_get_real_bitrate(g_encoder[i]);
            tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "real_bitrate: %d", real_bitrate);

            err[i] = tlka_lc3plus_enc_set_bitrate(g_encoder[i], lc3p_enc_param.bitrate);
            if (err[i] != LC3PLUS_OK) {
                tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "err_bitrate: %d", err[i]);
            }
        }

        for (int i = 0; i < lc3p_enc_param.channels; i++) {
            input_int24_or_int32[i] = lc3plus_buf_24 + i * nSamples_enc;
        }
    } else if (size_type_in == 2) { //scratch buff init
        enc_scratch = (void *)p_buff;
        tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "scratch buff init");
    }

    return 0;
}

/**
 * @brief       Initialize LC3 Plus decoder
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      Operation result, 0 for success
 */
int8_t tlkalg_lc3_plus_dec_init(uint8_t *p_buff, uint8_t channel)
{
    uint8_t chnl_in      = channel & 0x0F;
    uint8_t size_type_in = (channel >> 4) & 0x0F;

    uint8_t chnl_out = tlkalg_lc3_plus_enc_channel_change(chnl_in);

    if (size_type_in == 1) { //encoder buff init
        g_decoder[0] = (LC3PLUS_Dec *)p_buff;
        g_decoder[1] = (LC3PLUS_Dec *)((uint8_t *)p_buff + decoder_size);

        for (int i = 0; i < chnl_out; i++) {
            err[i] = tlka_lc3a_dec_init(g_decoder[i], P_lc3p_dec_param);
            if (err[i] != LC3PLUS_OK) {
                tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "err_dec_init[%d]: %d", i, err[i]);
            }

            nSamples_dec = tlka_lc3a_dec_get_output_samples(g_decoder[i]);
            tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "nSamples_dec: %d", nSamples_dec);
        }

        for (int i = 0; i < lc3p_dec_param.channels; i++) {
            output_int24_or_int32[i] = lc3plus_buf_24 + i * nSamples_dec;
        }
    } else if (size_type_in == 2) { //scratch buff init
        dec_scratch = (void *)p_buff;
        tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "scratch buff init");
    }

    return 0;
}

/**
 * @brief       Deinitialize LC3 Plus encoder
 * @return      Operation result, 0 for success
 */
int8_t tlkalg_lc3_plus_enc_deinit(void)
{
    g_encoder[0] = NULL;
    g_encoder[1] = NULL;
    enc_scratch  = NULL;
    tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "tlkalg_lc3_plus_enc_deinit");
    return 0;
}

/**
 * @brief       Deinitialize LC3 Plus decoder
 * @return      Operation result, 0 for success
 */
int8_t tlkalg_lc3_plus_dec_deinit(void)
{
    g_decoder[0] = NULL;
    g_decoder[1] = NULL;
    dec_scratch  = NULL;
    tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "tlkalg_lc3_plus_dec_deinit");
    return 0;
}

/**
 * @brief       Get LC3 Plus encoder size by type
 * @param[in]   size_type_in - Size type
 * @param[in]   chnl_in - Channel count
 * @return      Required size in bytes
 */
uint16_t tlkalg_lc3_plus_enc_get_size_type(uint8_t size_type_in, uint8_t chnl_in)
{
    (void)chnl_in;
    switch (size_type_in) {
    case 1:
    {
        encoder_size = tlka_lc3a_enc_get_size(lc3p_enc_param.samplerate, lc3p_enc_param.channels, lc3p_enc_param.wavFormat_in);
        encoder_size = encoder_size / 4 * 4;
        // lc3p_enc_param.channels = chnl_in;

        return encoder_size;
        break;
    }
    case 2:
    {
        scratch_size_enc = tlka_lc3a_enc_get_scratch_size(g_encoder[0]);

        return scratch_size_enc;
        break;
    }
    default:
        tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "lc3 plus enc get size error");
        break;
    }

    return 0;
}

/**
 * @brief       Get LC3 Plus decoder size by type
 * @param[in]   size_type_in - Size type
 * @param[in]   chnl_in - Channel count
 * @return      Required size in bytes
 */
uint16_t tlkalg_lc3_plus_dec_get_size_type(uint8_t size_type_in, uint8_t chnl_in)
{
    (void)chnl_in;
    switch (size_type_in) {
    case 1:
    {
        decoder_size = tlka_lc3a_dec_get_size(lc3p_dec_param.samplerate, lc3p_dec_param.channels, lc3p_dec_param.plc_method, lc3p_dec_param.frame_dms, lc3p_dec_param.hrmode);
        decoder_size = decoder_size / 4 * 4;
        // lc3p_dec_param.channels = chnl_in;
        return decoder_size;
        break;
    }
    case 2:
    {
        scratch_size_dec = tlka_lc3a_dec_get_scratch_size(g_decoder[0]);

        return scratch_size_dec;
        break;
    }
    default:
        tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "lc3 plus dec get size error");
        break;
    }

    return 0;
}

/**
 * @brief       Get LC3 Plus encoder size
 * @param[in]   channel - Channel configuration
 * @return      Required size in bytes
 */
uint16_t tlkalg_lc3_plus_enc_get_size(uint8_t channel)
{
    uint8_t chnl_in      = channel & 0x0F;
    uint8_t size_type_in = (channel >> 4) & 0x0F;

    uint8_t  chnl_out     = tlkalg_lc3_plus_enc_channel_change(chnl_in);
    uint16_t enc_out_size = tlkalg_lc3_plus_enc_get_size_type(size_type_in, chnl_out);

    return ((chnl_out == 1) ? enc_out_size : (enc_out_size * 2));
}

/**
 * @brief       Get LC3 Plus decoder size
 * @param[in]   channel - Channel configuration
 * @return      Required size in bytes
 */
uint16_t tlkalg_lc3_plus_dec_get_size(uint8_t channel)
{
    uint8_t chnl_in      = channel & 0x0F;
    uint8_t size_type_in = (channel >> 4) & 0x0F;

    uint8_t  chnl_out     = tlkalg_lc3_plus_dec_channel_change(chnl_in);
    uint16_t dec_out_size = tlkalg_lc3_plus_dec_get_size_type(size_type_in, chnl_out);

    return ((chnl_out == 1) ? dec_out_size : (dec_out_size * 2));
}

/**
 * @brief       Process LC3 Plus encoding
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Operation result, 1 for success, 0 for failure
 */
int tlkalg_lc3_plus_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    //    (void)len;

    uint8_t  chnl = 0;
    int32_t *psrc = (int32_t *)ps;

    if (nSamples_enc != len) {
        tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "lc3 plus enc datalen error! nSamples_enc:%d, len:%d", nSamples_enc, len);
    }

#if MCU_CORE_TYPE == MCU_CORE_TL721X
    if (channel == 0 || channel == 1) {
        chnl = 1;
    }

    deinterleave_int(psrc, input_int24_or_int32, nSamples_enc, chnl);
    err_enc[0] = tlka_lc3a_enc_process_frame(g_encoder[channel], (void **)input_int24_or_int32, pd, &nBytes, enc_scratch);

#else
    chnl = tlkalg_lc3_plus_enc_channel_change(channel);

    if (chnl == 1) {
        for (int k = 0; k < nSamples_enc; k++) {
            lc3plus_sample_buf[k] = psrc[k];
        }
    } else {
        for (int k = 0; k < nSamples_enc; k++) {
            lc3plus_sample_buf[k]                = psrc[2 * k];
            lc3plus_sample_buf[k + nSamples_enc] = psrc[2 * k + 1];
        }
    }

    for (int i = 0; i < chnl; i++) {
        deinterleave_int(lc3plus_sample_buf + i * nSamples_enc, input_int24_or_int32, nSamples_enc, 1);
        err_enc[i] = tlka_lc3a_enc_process_frame(g_encoder[i], (void **)input_int24_or_int32, pd + nBytes * i, &nBytes, enc_scratch);
    }
#endif

    if (err_enc[0] != LC3PLUS_OK || err_enc[1] != LC3PLUS_OK) {
        tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "err_enc[0]: %d", err_enc[0]);
        return 0;
    }

    return 1;
}

/**
 * @brief       Process LC3 Plus decoding
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Operation result, 1 for success, 0 for failure
 */
int tlkalg_lc3_plus_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    uint8_t plc_flag = (channel >> 4) & 0x0F;

    int32_t *pdes = (int32_t *)pd;

    uint8_t chnl = tlkalg_lc3_plus_dec_channel_change(channel & 0x0F);
    nBytes       = len;

    if (plc_flag == 1) {
        bfi_ext = 1;
    }

    for (int i = 0; i < chnl; i++) {
        err_dec[i] = tlka_lc3a_dec_process_frame(g_decoder[i], ps + nBytes * i, nBytes, bfi_ext, (void **)output_int24_or_int32, dec_scratch);
        interleave_int(output_int24_or_int32, lc3plus_sample_buf + nSamples_dec * i, nSamples_dec, 1);
    }

#if (MCU_CORE_TYPE == MCU_CORE_TL721X)

    int pdes_stereo_lr[160 * 2] = {0};

    if (chnl == 1) {
        for (int k = 0; k < nSamples_dec; k++) {
            pdes_stereo_lr[k] = lc3plus_sample_buf[k];
        }
    } else {
        for (int k = 0; k < nSamples_dec; k++) {
            pdes_stereo_lr[2 * k]     = lc3plus_sample_buf[k];
            pdes_stereo_lr[2 * k + 1] = lc3plus_sample_buf[k + nSamples_dec];
        }
    }

    for (int k = 0; k < nSamples_dec * chnl; k++) {
        pdes[k] = pdes_stereo_lr[k];
    }
#else
#if TLKALG_ALG_LOOPBACK_TEST_ENABLE
    if (chnl == 1) {
        for (int k = 0; k < nSamples_dec; k++) {
            pdes[k] = lc3plus_sample_buf[k];
        }
    } else {
        for (int k = 0; k < nSamples_dec; k++) {
            pdes[2 * k]     = lc3plus_sample_buf[k];
            pdes[2 * k + 1] = lc3plus_sample_buf[k + nSamples_dec];
        }
    }

#else
    for (int k = 0; k < nSamples_dec * chnl; k++) {
        pdes[k] = lc3plus_sample_buf[k];
    }
#endif

#endif

    if (bfi_ext == 1) {
        bfi_ext = 0;
    }

    if (err_dec[0] != LC3PLUS_OK) {
        tlkapi_trace(0xFFFFFFFF, "[LC3 PLUS]", "err_dec: %d", err_dec[0]);
        return 0;
    }

    return 1;
}

/**
 * @brief       Deinterleave integer data
 * @param[in]   in - Input data pointer
 * @param[out]  out - Output data pointers array
 * @param[in]   n - Number of samples
 * @param[in]   channels - Number of channels
 * @return      None
 */
void deinterleave_int(int32_t *in, int32_t **out, int32_t n, int32_t channels)
{
    int32_t ch, i;
    for (ch = 0; ch < channels; ch++) {
        for (i = 0; i < n; i++) {
            out[ch][i] = in[i * channels + ch];
        }
    }
}

/**
 * @brief       Interleave integer data
 * @param[in]   in - Input data pointers array
 * @param[out]  out - Output data pointer
 * @param[in]   n - Number of samples
 * @param[in]   channels - Number of channels
 * @return      None
 */
void interleave_int(int32_t **in, int32_t *out, int32_t n, int32_t channels)
{
    int32_t ch, i;
    for (ch = 0; ch < channels; ch++) {
        for (i = 0; i < n; i++) {
            out[i * channels + ch] = in[ch][i];
        }
    }
}

#endif
