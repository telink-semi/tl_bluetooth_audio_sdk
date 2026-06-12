/********************************************************************************************************
 * @file    tlkalg_lc3_interface.c
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
 ********************************************************************************************************/
#include "tl_common.h"
#include "common/types.h"
#include "tlka_lc3a_api.h"
#include "tlkalg_lc3_interface.h"
#include "tlklib/dbg/tlkdbg.h"
#include "../tlkalg_audio_cfg.h"

#ifndef LE_AUDIO_NEW_LC3_ENABLE
#define LE_AUDIO_NEW_LC3_ENABLE (0)
#endif

#if (LE_AUDIO_NEW_LC3_ENABLE)

static int s_lc3_enc_size     = 0;
static int s_lc3_nSamples_enc = 0;

static int s_lc3_dec_size      = 0;
static int s_lc3_nSamples_dec  = 0;
static int s_is_cur_frame_lost = 0;

static uint8_t *s_lc3_enc_buffer         = NULL;
static uint8_t  s_lc3_enc_max_channel    = 0;
static uint8_t *s_lc3_enc_scratch_buffer = NULL;
static int      s_lc3_enc_scratch_size   = 0;

static uint8_t *s_lc3_dec_buffer         = NULL;
static uint8_t  s_lc3_dec_max_channel    = 0;
static uint8_t *s_lc3_dec_scratch_buffer = NULL;
static int      s_lc3_dec_scratch_size   = 0;

LC3P_ENC_CFG_Param lc3_enc_param = {
    .samplerate   = 48000,
    .hrmode       = 0,
    .channels     = 1,
    .bitrate      = 48000, //if stereo, bitrate need multiply 2
    .frame_dms    = LC3_FRAME_DURATION_10MS,
    .wavFormat_in = LC3_WAV_FORMAT_INT16,
    .lfe          = 0,
};

LC3P_DEC_CFG_Param lc3_dec_param = {
    .samplerate    = 48000,
    .hrmode        = 0,
    .channels      = 1,
    .plc_method    = LC3PLUS_PLCMETH_ADV_PHECU_TDC_NS,
    .frame_dms     = LC3_FRAME_DURATION_10MS,
    .wavFormat_out = LC3_WAV_FORMAT_INT16,
};

typedef struct
{
    u32 freqHz;

    u8  freqCodec;
    u16 duraUs;
    u8  duralc3;

    u16 frameSample;
    u16 frameOctets;
} lc3ParamIndex;

//16bit each sample
static const lc3ParamIndex s_lc3ParamBap2lib[13][2] = {
    {{0, 0, 0, LC3_FRAME_DURATION_10MS, 0, 0}, {0, 0, 0, LC3_FRAME_DURATION_10MS, 0, 0}}, //rsvd

    {{8000, 0, 7500, LC3_FRAME_DURATION_7p5MS, 60, 120}, {8000, 0, 10000, LC3_FRAME_DURATION_10MS, 80, 160}}, //UNFRAMED,

    {{11025, 1, 7500, LC3_FRAME_DURATION_7p5MS, 0, 0}, {11025, 1, 10000, LC3_FRAME_DURATION_10MS, 0, 0}}, //FRAMED

    {{16000, 3, 7500, LC3_FRAME_DURATION_7p5MS, 120, 240}, {16000, 3, 10000, LC3_FRAME_DURATION_10MS, 160, 320}}, //UNFRAMED

    {{22050, 4, 7500, LC3_FRAME_DURATION_7p5MS, 0, 0}, {22050, 4, 10000, LC3_FRAME_DURATION_10MS, 0, 0}}, //FRAMED

    {{24000, 5, 7500, LC3_FRAME_DURATION_7p5MS, 160, 230}, {24000, 5, 10000, LC3_FRAME_DURATION_10MS, 240, 480}}, //UNFRAMED

    {{32000, 6, 7500, LC3_FRAME_DURATION_7p5MS, 240, 480}, {32000, 6, 10000, LC3_FRAME_DURATION_10MS, 320, 640}}, //UNFRAMED

    {{44100, 7, 7500, LC3_FRAME_DURATION_7p5MS, 0, 0}, {44100, 7, 10000, LC3_FRAME_DURATION_10MS, 0, 0}}, //FRAMED

    {{48000, 8, 7500, LC3_FRAME_DURATION_7p5MS, 360, 720}, {48000, 8, 10000, LC3_FRAME_DURATION_10MS, 480, 960}}, //UNFRAMED

    {{88200, 9, 7500, LC3_FRAME_DURATION_7p5MS, 0, 0}, {88200, 9, 10000, LC3_FRAME_DURATION_10MS, 0, 0}}, //FRAMED

    {{96000, 10, 7500, LC3_FRAME_DURATION_7p5MS, 720, 1440}, {96000, 10, 10000, LC3_FRAME_DURATION_10MS, 960, 1960}}, //UNFRAMED

    {{176400, 11, 7500, LC3_FRAME_DURATION_7p5MS, 0, 0}, {176400, 11, 10000, LC3_FRAME_DURATION_10MS, 0, 0}}, //FRAMED

    {{192000, 12, 7500, LC3_FRAME_DURATION_7p5MS, 1440, 2880}, {192000, 12, 10000, LC3_FRAME_DURATION_10MS, 1920, 3840}}, //UNFRAMED
};

static bool lc3_enc_init_flag[2] = {false, false};

/**
 * @brief       Initializes the LC3 encoder with specified parameters.
 * @param[in]   enc         - Pointer to the encoder buffer.
 * @param[in]   nSamplerate - Sampling rate of the audio.
 * @param[in]   nBitrate    - Bitrate for encoding.
 * @param[in]   nMs_mode    - Frame duration in milliseconds.
 * @return      LC3ENC_OK on success, error code otherwise.
 */
static int tlkalg_lc3_enc_init_mode(u8 *enc, u32 nSamplerate, u32 nBitrate, u16 nMs_mode)
{
    LC3PLUS_ErrorCode ret = 0;

    LC3P_ENC_CFG_Param param;
    tmemcpy(&param, &lc3_enc_param, sizeof(LC3P_ENC_CFG_Param));
    param.samplerate = nSamplerate;
    param.bitrate    = nBitrate;
    param.frame_dms  = nMs_mode;

    ret = tlka_lc3a_enc_init((LC3PLUS_Enc *)enc, &param);

    s_lc3_nSamples_enc = tlka_lc3a_enc_get_input_samples((LC3PLUS_Enc *)enc);

    ret = tlka_lc3plus_enc_set_bitrate((LC3PLUS_Enc *)enc, param.bitrate);

    if (s_lc3_enc_scratch_buffer == NULL) {
        s_lc3_enc_scratch_size   = tlka_lc3a_enc_get_scratch_size((LC3PLUS_Enc *)enc);
        s_lc3_enc_scratch_buffer = tlkalg_malloc_func(s_lc3_enc_scratch_size);
    }

    return ret;
}

/**
 * @brief       Sets the encoder parameters based on BAP initial parameters provided.
 * @param[in]   param - Pointer to the BAP initial parameter structure.
 * @return      0 on success, 0xFF if channel index is out of range or buffer is not initialized, 0xFE if invalid BAP parameters.
 */
static uint8_t tlkalg_lc3_enc_set_enc_param_bap(const struct lc3_initial_param_bap *param)
{
    if (param->channelIndex >= s_lc3_enc_max_channel || s_lc3_enc_buffer == NULL) {
        return 0xFF;
    }

    if (param->frameDuration > 2 || param->samplingFreq > 12 || param->samplingFreq == 0) {
        return 0xFE;
    }
    u32 bitRate = (param->perCodecFrame * 8 * 10 * 1000) / (param->frameDuration ? 100 : 75);

    lc3_enc_init_flag[param->channelIndex] = true;
    tlkdbg_send_str_u32s("Encode Initial index, freq, bitrate,dura", 4, param->channelIndex, s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].freqHz, bitRate,
                         s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].duralc3);
    return tlkalg_lc3_enc_init_mode(s_lc3_enc_buffer + param->channelIndex * s_lc3_enc_size, s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].freqHz, bitRate,
                                    s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].duralc3);
}

/**
 * @brief       This function sets the algorithm parameter settings based on the type.
 * @param[in]   type    - the type of parameter to set.
 * @param[in]   param   - the parameter value.
 * @return      The result of setting the parameter.
 */
uint8_t tlkalg_lc3_enc_set_param(uint8_t type, void *param)
{
    uint8_t result = 0;

    switch (type) {
    case LC3_TYPE_SET_PARAM:
        break;
    case LC3_TYPE_SET_PARAM_BAP:
        result = tlkalg_lc3_enc_set_enc_param_bap(param);
        break;
    }

    return result;
}

/**
 * @brief       Gets the memory size used by the LC3 encoder.
 * @param[in]   channel - Number of channels.
 * @return      Memory size required for the encoder.
 */
uint16_t tlkalg_lc3_enc_get_size(uint8_t channel)
{
    s_lc3_enc_size = tlka_lc3a_enc_get_size(lc3_enc_param.samplerate, lc3_enc_param.channels, lc3_enc_param.wavFormat_in);
    return s_lc3_enc_size * channel;
}

/**
 * @brief       Initializes the LC3 encoder.
 * @param[in]   p_buff  - Pointer to the buffer for the encoder.
 * @param[in]   channel - Number of channels (mono or stereo).
 * @return      0 on success.
 */
int8_t tlkalg_lc3_enc_init(uint8_t *p_buff, uint8_t channel)
{
    s_lc3_enc_max_channel = channel;
    s_lc3_enc_buffer      = p_buff;
    return 0;
}

/**
 * @brief       Deinitializes the LC3 encoder.
 * @return      0 on success.
 */
int8_t tlkalg_lc3_enc_deinit(void)
{
    s_lc3_enc_max_channel = 0;
    s_lc3_enc_buffer      = NULL;

    s_lc3_enc_size     = 0;
    s_lc3_nSamples_enc = 0;

    if (s_lc3_enc_scratch_buffer) {
        s_lc3_enc_scratch_size = 0;
        tlkalg_free_func(s_lc3_enc_scratch_buffer);
        s_lc3_enc_scratch_buffer = NULL;
    }
    lc3_enc_init_flag[0] = false;
    lc3_enc_init_flag[1] = false;
    return 0;
}

/**
 * @brief       Processes the audio encoding.
 * @param[in]   ps    - Pointer to the input audio samples.
 * @param[out]  pd    - Pointer to the output encoded data.
 * @param[in]   len   - Length of the input audio samples.
 * @param[in]   width - Width of the audio samples.
 * @param[in]   channel - Channel index (mono or stereo).
 * @return      0 on success, -1 on failure.
 */
int tlkalg_lc3_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)len;
    (void)width;
    (void)pd;
    (void)ps;
    if (channel >= s_lc3_enc_max_channel || s_lc3_enc_buffer == NULL) {
        return -1;
    }

    if (lc3_enc_init_flag[channel] == false) {
        return -1;
    }
    int nBytes = 0;

    int16_t buf_16[1 * 480];

    int16_t *input_16[] = {buf_16, buf_16 + s_lc3_nSamples_enc};

    /* copy input data into buf_24 */
    tmemcpy(buf_16, ps, sizeof(int16_t) * s_lc3_nSamples_enc);

    return tlka_lc3a_enc_process_frame((LC3PLUS_Enc *)(s_lc3_enc_buffer + (channel * s_lc3_enc_size)), (void **)input_16, pd, (int32_t *)&nBytes, (void *)s_lc3_enc_scratch_buffer);
}

static bool lc3_dec_init_flag[2] = {false, false};

/**
 * @brief       Initializes the LC3 decoder with specified parameters.
 * @param[in]   dec         - Pointer to the decoder buffer.
 * @param[in]   nSamplerate - Sampling rate of the audio.
 * @param[in]   nBitrate    - Bitrate for decoding.
 * @param[in]   nMs_mode    - Frame duration in milliseconds.
 * @return      0 on success, error code otherwise.
 */
static int tlkalg_lc3_dec_init_mode(u8 *dec, u32 nSamplerate, u32 nBitrate, u16 nMs_mode)
{
    LC3PLUS_ErrorCode ret = 0;
    (void)nBitrate;
    LC3P_DEC_CFG_Param param;
    tmemcpy(&param, &lc3_dec_param, sizeof(LC3P_DEC_CFG_Param));
    param.samplerate = nSamplerate;
    param.frame_dms  = nMs_mode;

    ret = tlka_lc3a_dec_init((LC3PLUS_Dec *)dec, &param);

    tlk_printf("tlka_lc3a_dec_init %d", ret);

    tlk_printf("s_lc3_dec_size %d", s_lc3_dec_size);

    s_lc3_nSamples_dec = tlka_lc3a_dec_get_output_samples((LC3PLUS_Dec *)dec);

    tlk_printf("tlka_lc3a_dec_get_output_samples %d", s_lc3_nSamples_dec);

    if (s_lc3_dec_scratch_buffer == NULL) {
        s_lc3_dec_scratch_size = tlka_lc3a_dec_get_scratch_size((LC3PLUS_Dec *)dec);
        tlk_printf("tlka_lc3a_dec_get_scratch_size %d", s_lc3_dec_scratch_size);
        s_lc3_dec_scratch_buffer = tlkalg_malloc_func(s_lc3_dec_scratch_size);
    }

    return ret;
}

/**
 * @brief       Sets the decoder parameters based on BAP initial parameters provided.
 * @param[in]   param - Pointer to the BAP initial parameter structure.
 * @return      0 on success, 0xFF if channel index is out of range or buffer is not initialized, 0xFE if invalid BAP parameters.
 */
static uint8_t tlkalg_lc3_dec_set_dec_param_bap(const struct lc3_initial_param_bap *param)
{
    if (param->channelIndex >= s_lc3_dec_max_channel || s_lc3_dec_buffer == NULL) {
        return 0xFF;
    }

    if (param->frameDuration > 2 || param->samplingFreq > 12 || param->samplingFreq == 0) {
        return 0xFE;
    }
    u32 bitRate = (param->perCodecFrame * 8 * 10 * 1000) / (param->frameDuration ? 100 : 75);

    lc3_dec_init_flag[param->channelIndex] = true;
    tlkdbg_send_str_u32s("Decode Initial index, freq, bitrate,dura", 4, param->channelIndex, s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].freqHz, bitRate,
                         s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].duralc3);
    return tlkalg_lc3_dec_init_mode(s_lc3_dec_buffer + (param->channelIndex * s_lc3_dec_size), s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].freqHz, bitRate,
                                    s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].duralc3);
}

/**
 * @brief       Set algorithm parameter settings for LC3 decoder.
 * @param[in]   type    - the type of parameter to set.
 * @param[in]   param   - the parameter value to set.
 * @return      0 on success, non-zero on failure.
 */
uint8_t tlkalg_lc3_dec_set_param(uint8_t type, void *param)
{
    uint8_t result = 0;

    switch (type) {
    case LC3_TYPE_SET_PARAM:
        break;
    case LC3_TYPE_SET_PARAM_BAP:
        result = tlkalg_lc3_dec_set_dec_param_bap(param);
        break;
    case LC3_TYPE_SET_BEC_DETECT:
        struct lc3_dec_set_bec_detect *detect_param = param;
        s_is_cur_frame_lost                         = detect_param->detect;
        break;
    }

    return result;
}

/**
 * @brief       Gets the memory size used by the LC3 decoder.
 * @param[in]   channel - Number of channels.
 * @return      Memory size required for the decoder.
 */
uint16_t tlkalg_lc3_dec_get_size(uint8_t channel)
{
    s_lc3_dec_size = tlka_lc3a_dec_get_size(lc3_dec_param.samplerate, lc3_dec_param.channels, lc3_dec_param.plc_method, lc3_dec_param.frame_dms, lc3_dec_param.hrmode);
    return s_lc3_dec_size * channel;
}

/**
 * @brief       Initializes the LC3 decoder.
 * @param[in]   p_buff  - Pointer to the buffer for the decoder.
 * @param[in]   channel - Number of channels (mono or stereo).
 * @return      0 on success.
 */
int8_t tlkalg_lc3_dec_init(uint8_t *p_buff, uint8_t channel)
{
    s_lc3_dec_max_channel = channel;
    s_lc3_dec_buffer      = p_buff;
    return 0;
}

/**
 * @brief       Deinitializes the LC3 decoder.
 * @return      0 on success.
 */
int8_t tlkalg_lc3_dec_deinit(void)
{
    s_lc3_dec_max_channel = 0;
    s_lc3_dec_buffer      = NULL;

    s_lc3_dec_size     = 0;
    s_lc3_nSamples_dec = 0;

    if (s_lc3_dec_scratch_buffer) {
        s_lc3_dec_scratch_size = 0;
        tlkalg_free_func(s_lc3_dec_scratch_buffer);
        s_lc3_dec_scratch_buffer = NULL;
    }
    lc3_dec_init_flag[0] = false;
    lc3_dec_init_flag[1] = false;
    return 0;
}

/**
 * @brief       Processes the audio decoding.
 * @param[in]   ps    - Pointer to the input encoded data.
 * @param[out]  pd    - Pointer to the output audio samples.
 * @param[in]   len   - Length of the input encoded data.
 * @param[in]   width - Width of the audio samples.
 * @param[in]   channel - Channel index (mono or stereo).
 * @return      0 on success, -1 on failure.
 */
int tlkalg_lc3_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    if (channel >= s_lc3_dec_max_channel || s_lc3_dec_buffer == NULL) {
        return -1;
    }

    if (lc3_dec_init_flag[channel] == false) {
        return -1;
    }
    int16_t  buf_16[1 * 480];
    int16_t *output16[] = {(int16_t *)pd, buf_16};

    return tlka_lc3a_dec_process_frame((LC3PLUS_Dec *)(s_lc3_dec_buffer + (channel * s_lc3_dec_size)), (void *)ps, (int32_t)len, (int32_t)s_is_cur_frame_lost, (void **)output16,
                                       s_lc3_dec_scratch_buffer);
}
#endif // #if (!TLK_CFG_TEMP_DRAM_OPTM_TPSLL)
