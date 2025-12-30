/********************************************************************************************************
 * @file    tlkalg_lc3_24bit_interface.h
 *
 * @brief   This is the header file for TLSR/TL
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
#pragma once
#include "tlkalg/audio/audio_alg_interface.h"
#if TLKALG_LC3_24BIT_ENC_ENABLE || TLKALG_LC3_24BIT_DEC_ENABLE

    #include "lc3.h"

    #define LC3_ENC_BUFFER_SIZE 5404
    #define LC3_DEC_BUFFER_SIZE 9072

struct lc3_encoder;
struct lc3_decoder;

//enum lc3_pcm_format {
//    LC3_PCM_FORMAT_S16,
//    LC3_PCM_FORMAT_S24,
//    LC3_PCM_FORMAT_S24_3LE,
//    LC3_PCM_FORMAT_FLOAT,
//};

typedef struct LC3_ENC_PARA_INIT
{
    unsigned int   frame_dt_us;
    unsigned int   samplerate_hz;
    unsigned int   bitrate;
    unsigned short enc_bytes;
    unsigned short num_samples;
    unsigned int   enc_size;

    void *lc3_encoder_mem;
} lc3_enc_para_init_t;

typedef struct parameters
{
    float frame_us;
    int   sample_rate;
    int   bitrate;
    int   bitdepth;
} parameters_t;

extern lc3_enc_para_init_t g_lc3_enc_para;

typedef struct LC3_DEC_PARA_INIT
{
    unsigned int frame_dt_us;
    unsigned int samplerate_hz;
    unsigned int bitrate;
    unsigned int dec_size;
    void        *lc3_decoder_mem;
} lc3_dec_para_init_t;

extern lc3_dec_para_init_t g_lc3_dec_para;

typedef struct lc3_encoder *lc3_encoder_t;
typedef struct lc3_decoder *lc3_decoder_t;

extern unsigned            lc3_encoder_size(int dt_us, int sr_hz);
extern int                 lc3_frame_bytes(int dt_us, int bitrate);
extern int                 lc3_frame_samples(int dt_us, int sr_hz);
extern struct lc3_encoder *lc3_setup_encoder(int dt_us, int sr_hz, int sr_pcm_hz, void *mem);

extern unsigned            lc3_decoder_size(int dt_us, int sr_hz);
extern struct lc3_decoder *lc3_setup_decoder(int dt_us, int sr_hz, int sr_pcm_hz, void *mem);

extern int lc3_encode(struct lc3_encoder *encoder, enum lc3_pcm_format fmt, const void *pcm, int stride, int nbytes, void *out);
extern int lc3_decode(struct lc3_decoder *decoder, const void *in, int nbytes, enum lc3_pcm_format fmt, void *pcm, int stride);


void lc3_deinit(void);
int  lc3_encode24(int *pcm_in, int num_samples, unsigned char *enc_data);
int  lc3_decode24(const unsigned char *enc_data, int num_bytes, int *pcm_out);
void lc3_enc_set_bitrate(int bitrate);

/**
 * @brief       Initializes the LC3 24-bit encoder.
 * @param[in]   p_buff   - pointer to the buffer for encoder data.
 * @param[in]   channel  - the number of channels.
 * @return      0 on success.
 */
int8_t   tlkalg_lc3_24bit_enc_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Initializes the LC3 24-bit decoder.
 * @param[in]   p_buff   - pointer to the buffer for decoder data.
 * @param[in]   channel  - the number of channels.
 * @return      0 on success.
 */
int8_t   tlkalg_lc3_24bit_dec_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitializes the LC3 24-bit encoder.
 * @return      0 on success.
 */
int8_t   tlkalg_lc3_24bit_enc_deinit(void);

/**
 * @brief       Deinitializes the LC3 24-bit decoder.
 * @return      0 on success.
 */
int8_t   tlkalg_lc3_24bit_dec_deinit(void);

/**
 * @brief       Gets the size required for LC3 24-bit encoding.
 * @param[in]   channel - the number of channels.
 * @return      The required size for encoding or 0 if the channel is invalid.
 */
uint16_t tlkalg_lc3_24bit_enc_get_size(uint8_t channel);

/**
 * @brief       Gets the size required for LC3 24-bit decoding.
 * @param[in]   channel - the number of channels.
 * @return      The required size for decoding or 0 if the channel is invalid.
 */
uint16_t tlkalg_lc3_24bit_dec_get_size(uint8_t channel);

/**
 * @brief       Processes the LC3 24-bit encoding.
 * @param[in]   ps    - pointer to the source PCM data.
 * @param[out]  pd    - pointer to the destination encoded data.
 * @param[in]   len   - length of the PCM data.
 * @param[in]   width - width of the PCM data.
 * @param[in]   channel - the number of channels.
 * @return      The result of the encoding process.
 */
int      tlkalg_lc3_24bit_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       Processes the LC3 24-bit decoding.
 * @param[in]   ps    - pointer to the source encoded data.
 * @param[out]  pd    - pointer to the destination PCM data.
 * @param[in]   len   - length of the encoded data.
 * @param[in]   width - width of the PCM data.
 * @param[in]   channel - the number of channels.
 * @return      The result of the decoding process.
 */
int      tlkalg_lc3_24bit_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

#endif
