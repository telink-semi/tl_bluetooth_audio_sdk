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
#include "common/types.h"

#include "tlka_lc3_api.h"
#include "tlkalg_lc3_interface.h"
#include "tlklib/dbg/tlkdbg.h"

#if (!TLK_CFG_TEMP_DRAM_OPTM_TPSLL)
#define LC3_ENC_SIZE         tlka_lc3enc_get_size()
#define LC3_ENC_SCRATCH_SIZE tlka_lc3enc_get_scratch_size()
#define LC3_DEC_SIZE         tlka_lc3dec_get_size()
#define LC3_DEC_SCRATCH_SIZE tlka_lc3dec_get_scratch_size()

static uint8_t *s_lc3_enc_buffer      = NULL;
static uint8_t  s_lc3_enc_max_channel = 0;

static uint8_t *s_lc3_dec_buffer      = NULL;
static uint8_t  s_lc3_dec_max_channel = 0;

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
    {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, //rsvd

    {{8000, 0, 7500, 1, 60, 120}, {8000, 0, 10000, 0, 80, 160}}, //UNFRAMED,

    {{11025, 1, 7500, 1, 0, 0}, {11025, 1, 10000, 0, 0, 0}}, //FRAMED

    {{16000, 3, 7500, 1, 120, 240}, {16000, 3, 10000, 0, 160, 320}}, //UNFRAMED

    {{22050, 4, 7500, 1, 0, 0}, {22050, 4, 10000, 0, 0, 0}}, //FRAMED

    {{24000, 5, 7500, 1, 160, 230}, {24000, 5, 10000, 0, 240, 480}}, //UNFRAMED

    {{32000, 6, 7500, 1, 240, 480}, {32000, 6, 10000, 0, 320, 640}}, //UNFRAMED

    {{44100, 7, 7500, 1, 0, 0}, {44100, 7, 10000, 0, 0, 0}}, //FRAMED

    {{48000, 8, 7500, 1, 360, 720}, {48000, 8, 10000, 0, 480, 960}}, //UNFRAMED

    {{88200, 9, 7500, 1, 0, 0}, {88200, 9, 10000, 0, 0, 0}}, //FRAMED

    {{96000, 10, 7500, 1, 720, 1440}, {96000, 10, 10000, 0, 960, 1960}}, //UNFRAMED

    {{176400, 11, 7500, 1, 0, 0}, {176400, 11, 10000, 0, 0, 0}}, //FRAMED

    {{192000, 12, 7500, 1, 1440, 2880}, {192000, 12, 10000, 0, 1920, 3840}}, //UNFRAMED
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
    LC3ENC_Error ret = 0;

    LC3_CFG_Param lc3_enc_param = {
        .channels   = 1,
        .bitrate    = nBitrate,
        .frame_ms   = nMs_mode,
        .samplerate = nSamplerate,
        .pScratch   = s_lc3_enc_buffer,
        //		.plc_fadeout_in_ms = 30,
    };

    ret = tlka_lc3enc_init((LC3_ENC_STRU *)enc, &lc3_enc_param);

    tlkdbg_send_str_u32s("enc_size, scratch, encode", 4, tlka_lc3enc_get_size(), tlka_lc3enc_get_scratch_size(), ret, (u32)enc);

    if (ret != LC3ENC_OK) {
        return ret;
    }

    Word32 in_framesize;
    Word32 out_framesize;
    Word32 tns_enable = 1;

    ret = tlka_lc3enc_get_parameter((LC3_ENC_STRU *)enc, LC3_PARA_FRAMESIZE, &in_framesize);
    ret = tlka_lc3enc_get_parameter((LC3_ENC_STRU *)enc, LC3_PARA_OUTSIZE, &out_framesize);
    ret = tlka_lc3enc_set_parameter((LC3_ENC_STRU *)enc, LC3_PARA_TNS, &tns_enable);

    tlkdbg_send_str_u32s("lc3enc_get_parameter", 3, ret, in_framesize, out_framesize);

    return LC3ENC_OK;
}

/**
 * @brief       Sets the encoder parameters based on the initial parameters provided.
 * @param[in]   param - Pointer to the initial parameter structure.
 * @return      0 on success, 0xFF if channel index is out of range or buffer is not initialized.
 */
static uint8_t tlkalg_lc3_enc_set_enc_param(const struct lc3_initial_param *param)
{
    if (param->channelIndex >= s_lc3_enc_max_channel || s_lc3_enc_buffer == NULL) {
        return 0xFF;
    }

    lc3_enc_init_flag[param->channelIndex] = true;
    return tlkalg_lc3_enc_init_mode(s_lc3_enc_buffer + param->channelIndex * LC3_ENC_SIZE + LC3_ENC_SCRATCH_SIZE, param->sampleRate, param->bitRate, param->cycle);
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
    return tlkalg_lc3_enc_init_mode(s_lc3_enc_buffer + param->channelIndex * LC3_ENC_SIZE + LC3_ENC_SCRATCH_SIZE,
                                    s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].freqHz, bitRate,
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
        result = tlkalg_lc3_enc_set_enc_param(param);
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
    return LC3_ENC_SIZE * channel + +LC3_ENC_SCRATCH_SIZE;
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
    lc3_enc_init_flag[0]  = false;
    lc3_enc_init_flag[1]  = false;
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
    if (channel >= s_lc3_enc_max_channel || s_lc3_enc_buffer == NULL) {
        return -1;
    }

    if (lc3_enc_init_flag[channel] == false) {
        return -1;
    }

    return tlka_lc3enc_encode_frame((LC3_ENC_STRU *)(s_lc3_enc_buffer + channel * LC3_ENC_SIZE + LC3_ENC_SCRATCH_SIZE), (Word16 *)ps, (UWord8 *)pd);
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
    LC3DEC_Error ret = 0;

    LC3_CFG_Param lc3_dec_param = {
        .channels          = 1,
        .bitrate           = nBitrate,
        .frame_ms          = nMs_mode,
        .samplerate        = nSamplerate,
        .pScratch          = s_lc3_dec_buffer,
        .plc_fadeout_in_ms = 60,
    };

    ret = tlka_lc3dec_init((LC3_DEC_STRU *)dec, &lc3_dec_param);

    tlkdbg_send_str_u32s("dec_size, scratch, decode", 4, tlka_lc3dec_get_size(), tlka_lc3dec_get_scratch_size(), ret, (u32)dec);

    return ret;
}

/**
 * @brief       Sets the decoder parameters based on the initial parameters provided.
 * @param[in]   param - Pointer to the initial parameter structure.
 * @return      0 on success, 0xFF if channel index is out of range or buffer is not initialized.
 */
static uint8_t tlkalg_lc3_dec_set_dec_param(const struct lc3_initial_param *param)
{
    if (param->channelIndex >= s_lc3_dec_max_channel || s_lc3_dec_buffer == NULL) {
        return 0xFF;
    }

    lc3_dec_init_flag[param->channelIndex] = true;
    return tlkalg_lc3_dec_init_mode(s_lc3_dec_buffer + param->channelIndex * LC3_DEC_SIZE + LC3_DEC_SCRATCH_SIZE, param->sampleRate, param->bitRate, param->cycle);
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
    return tlkalg_lc3_dec_init_mode(s_lc3_dec_buffer + param->channelIndex * LC3_DEC_SIZE + LC3_DEC_SCRATCH_SIZE,
                                    s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].freqHz, bitRate,
                                    s_lc3ParamBap2lib[param->samplingFreq][param->frameDuration].duralc3);
}

/**
 * @brief       Sets the BEC detection parameter for the decoder.
 * @param[in]   param - Pointer to the BEC detection parameter structure.
 * @return      0 on success, 0xFF if channel index is out of range or buffer is not initialized.
 */
static uint8_t tlkalg_lc3_dec_set_bec_detect(const struct lc3_dec_set_bec_detect *param)
{
    if (param->channelIndex >= s_lc3_dec_max_channel || s_lc3_dec_buffer == NULL) {
        return 0xFF;
    }

    return tlka_lc3dec_set_parameter((LC3_DEC_STRU *)(s_lc3_dec_buffer + param->channelIndex * LC3_DEC_SIZE + LC3_DEC_SCRATCH_SIZE), LC3_PARA_BEC_DETECT, (Word32 *)&param->detect);
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
        result = tlkalg_lc3_dec_set_dec_param(param);
        break;
    case LC3_TYPE_SET_PARAM_BAP:
        result = tlkalg_lc3_dec_set_dec_param_bap(param);
        break;
    case LC3_TYPE_SET_BEC_DETECT:
        result = tlkalg_lc3_dec_set_bec_detect(param);
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
    return LC3_DEC_SIZE * channel + +LC3_DEC_SCRATCH_SIZE;
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
    lc3_dec_init_flag[0]  = false;
    lc3_dec_init_flag[1]  = false;
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

    return tlka_lc3dec_decode_frame((LC3_DEC_STRU *)(s_lc3_dec_buffer + channel * LC3_DEC_SIZE + LC3_DEC_SCRATCH_SIZE), (UWord8 *)ps, (UWord32)len, (Word16 *)pd);
}
#endif // #if (!TLK_CFG_TEMP_DRAM_OPTM_TPSLL)
