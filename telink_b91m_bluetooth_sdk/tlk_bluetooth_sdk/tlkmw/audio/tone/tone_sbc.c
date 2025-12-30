/********************************************************************************************************
 * @file    tone_sbc.c
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
#include "tlkmw/tlkmw.h"
#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkalg/audio/sbc/tlkalg_sbc_interface.h"
#include "tone_sbc.h"

#ifndef SBC_BNUM
#define SBC_BNUM 128
#endif
#if TONE_SBC_EN
SBC_CFG_Param g_sbc_tone_param = {
    .sbc_blocks     = 16,
    .sbc_bitpool    = 25,
    .sbc_allocation = 0,
    .sbc_samplerate = 16000,
    .sbc_channel    = 1,
    .msbc           = 0,

};
uint8_t g_tone_sbc_dec_buf[2048] = {0};

typedef struct sbc_para_cfg
{
    unsigned char *ptr;
    int            len;
    int            offset;
    signed short   predict;
    signed short   last;

    float d1;
    float d2;
    float d3;
    float d4;
    float iuk;

    signed char   idx;
    unsigned char fra;
    unsigned char id;
    signed char   idx_enc;
    signed short  predict_enc;
    signed short  vol;
    signed short  gain;
} sbc_para_cfg_t;

sbc_para_cfg_t sbc_para = {0};

int16_t pcm_remain_num;
int16_t pcm_pos_read;

signed short sbc_d[SBC_BNUM];

/**
 * @brief Initializes the SBC parameters with the provided data.
 * @param[in] ps Pointer to the source buffer.
 * @param[in] len Length of the source buffer.
 * @param[in] pre Predicted value for initialization.
 * @param[in] idx Index value for initialization.
 * @return none.
 */
void sbc_init(unsigned char *ps, int len, int pre, int idx)
{
    sbc_para.ptr         = ps;
    sbc_para.len         = len;
    sbc_para.predict     = pre;
    sbc_para.predict_enc = pre;
    sbc_para.idx         = idx;
    sbc_para.idx_enc     = idx;
    sbc_para.offset      = 0;
    sbc_para.last        = 0;
    sbc_para.fra         = 0;
    sbc_para.id          = 0;
    sbc_para.d1          = 0;
    sbc_para.d2          = 0;
    sbc_para.d3          = 0;
    sbc_para.d4          = 0;
    sbc_para.iuk         = 0;
    if (!sbc_para.gain) {
        sbc_para.gain = 1024;
    }
}

/**
 * @brief Initializes the tone SBC module.
 * @return none.
 */
void tone_sbc_init(void)
{
#if TLKALG_SBC_DEC_ENABLE
    tlka_sbc_dec_init((sbc_dec_para_t *)g_tone_sbc_dec_buf, (SBC_CFG_Param *)&g_sbc_tone_param);
#endif
}

/**
 * @brief Decodes SBC audio data.
 * @param[in] ps Pointer to the source buffer.
 * @param[in] len Length of the source buffer.
 * @param[out] pd Pointer to the destination buffer.
 * @return The length of decoded data.
 */
int tone_sbc_dec(unsigned char *ps, int len, unsigned char *pd)
{
    if (!(ps && pd)) {
        return -1;
    }

    uint32_t dlen = 0;
#if TLKALG_SBC_DEC_ENABLE
    tlka_sbc_dec_process((sbc_dec_para_t *)g_tone_sbc_dec_buf, (const uint8_t *)ps, (uint32_t)len, (uint16_t *)pd, &dlen, 0x00, 0x01);
#else
    (void)len;
#endif
    return dlen;
}

/**
 * @brief Retrieves the frame size of an SBC audio frame from the given data.
 * @param[in] data Pointer to the SBC audio data.
 * @return Frame size of the SBC audio.
 */
uint8_t get_source_sbc_framesize(uint8_t *data)
{
    uint8_t  acl_sbc_bitpool, acl_sbc_block, acl_sbc_subband;
    uint16_t acl_sbc_framesize;

    /* read bitpool from data */
    acl_sbc_bitpool = data[2];

    /* read block length from data */
    if ((data[1] & 0x30) == 0) {
        acl_sbc_block = 4;
    } else if ((data[1] & 0x30) == 0x10) {
        acl_sbc_block = 8;
    } else if ((data[1] & 0x30) == 0x20) {
        acl_sbc_block = 12;
    } else {
        acl_sbc_block = 16;
    }

    /* read subbands from data */
    if ((data[1] & 0x01) == 0) {
        acl_sbc_subband = 4;
    } else {
        acl_sbc_subband = 8;
    }

    /* read channel mode from data&calculate frame length */
    if ((data[1] & 0x0c) == 0) {
        /* mono */
        acl_sbc_framesize = 4 + acl_sbc_subband / 2 + (acl_sbc_block * acl_sbc_bitpool) / 8;
    } else if ((data[1] & 0x0c) == 4) {
        /* dual-mode */
        acl_sbc_framesize = 4 + acl_sbc_subband + (acl_sbc_block * acl_sbc_bitpool) / 4;
    } else if ((data[1] & 0x0c) == 8) {
        /* stereo */
        acl_sbc_framesize = 4 + acl_sbc_subband + (acl_sbc_block * acl_sbc_bitpool) / 8;
    } else {
        /* joint-stereo */
        acl_sbc_framesize = 4 + acl_sbc_subband + (acl_sbc_block * acl_sbc_bitpool + acl_sbc_subband) / 8;
    }

    return acl_sbc_framesize;
}

/**
 * @brief Converts 8-bit SBC samples to 16-bit PCM samples.
 * @param[in] ps Pointer to the source buffer.
 * @param[out] pd Pointer to the destination buffer.
 * @param[in] n Number of samples to convert.
 * @return Number of converted samples.
 */
_attribute_ram_code_ int sbc8to16(unsigned char *ps, signed short *pd, int n)
{
    (void)*ps;
    return sbc_pcm_sample_read(pd, n);
}

/**
 * @brief Reads PCM samples from the decoded SBC data.
 * @param[out] buf Buffer to store the PCM samples.
 * @param[in] sample_num Number of samples to read.
 * @return Number of samples read.
 */
int sbc_pcm_sample_read(int16_t *buf, int sample_num)
{
    static int16_t dec_pcm[256];
    int            offset = 0;
    int            copy_num;

    while (offset < sample_num) {
        copy_num = sample_num - offset;

        if (pcm_remain_num) {
            copy_num = copy_num > pcm_remain_num ? pcm_remain_num : copy_num;

            tmemcpy(buf + offset, dec_pcm + pcm_pos_read, sizeof(int16_t) * copy_num);
            offset += copy_num;
            pcm_remain_num -= copy_num;
            pcm_pos_read += copy_num;
        } else {
            if (g_tone_cfg.offset >= g_tone_cfg.len) {
                goto L_PLAY_DONE;
            }

            tone_sbc_dec(g_tone_cfg.buff, g_tone_cfg.frame_size, (uint8_t *)dec_pcm);

            // Reload buffer
            pcm_remain_num = 128;
            pcm_pos_read   = 0;

            g_tone_cfg.buff += g_tone_cfg.frame_size;
            g_tone_cfg.offset += g_tone_cfg.frame_size;
        }
    }


    return offset;


L_PLAY_DONE:

    g_tone_cfg.busy = 0;

    // Clear PCM buffer
    pcm_remain_num = 0;

    // Play hold tone ?
    if (g_tone_cfg.hold) {
        tone_play(g_tone_cfg.hold & 0x7f);
    }

    return offset;
}

/**
 * @brief Performs upsampling on the SBC audio data.
 * @param[in] iTErr Error correction value.
 * @param[in] di Input sample.
 * @param[out] pd Pointer to store the output sample.
 * @return Status indicating whether a new sample was generated.
 */
_attribute_ram_code_ static int tlka_sbc_upresample_fra(float iTErr, signed short di, signed short *pd)
{
    float iOneTERRBW = 1.0f;

    float iPPAlpha = (0.2f);

    float iUKs2 = (sbc_para.iuk * sbc_para.iuk);

    float iC2  = iPPAlpha * (iUKs2 - sbc_para.iuk);
    float iC1  = iPPAlpha * (-iUKs2 + sbc_para.iuk) + sbc_para.iuk;
    float iC0  = iPPAlpha * (-iUKs2 + sbc_para.iuk) - sbc_para.iuk + iOneTERRBW;
    float iC_1 = iPPAlpha * (iUKs2 - sbc_para.iuk);

    float iOut32 = iC_1 * sbc_para.d4 + iC0 * sbc_para.d3 + iC1 * sbc_para.d2 + iC2 * sbc_para.d1;

    if (iOut32 > 32767) {
        *pd = 32767;
    } else if (iOut32 < -32768) {
        *pd = -32768;
    } else {
        *pd = (int16_t)iOut32;
    }

    sbc_para.iuk += iTErr;

    if (sbc_para.iuk >= iOneTERRBW) {
        sbc_para.iuk -= iOneTERRBW;
        sbc_para.d4 = sbc_para.d3;
        sbc_para.d3 = sbc_para.d2;
        sbc_para.d2 = sbc_para.d1;
        sbc_para.d1 = di;

        return 1;
    }

    return 0;
}

/**
 * @brief SBC to pcm
 * @param[in] pointer to the sbc source buffer
 * @param[in] decoded size
 * @param[in] Conversion sampling rate
 * @returns Encoding result size
 */
_attribute_ram_code_ int sbc_get_sample(signed short *pd, int n, int sample_rate)
{
    int                       ni       = n;
    AUDIO_CODEC_DAC_TYPE_ENUM dac_type = tlkmdi_audio_get_codec_dac_type();

    if (sbc_para.len - sbc_para.offset < SBC_BNUM / 2) {
        if (sbc_para.vol < 1024) {
            while (n--) {
                if (sbc_para.vol < 1024) {
                    sbc_para.vol++;
                }
                *pd = (*pd * sbc_para.vol + 512) >> 10;
                pd++;

                if (dac_type == AUDIO_CODEC_DAC_STEREO) {
                    *pd = ((int)(*pd) * sbc_para.vol + 1) >> 10;
                    pd++;
                }
            }
            return ni;
        }
        return 0;
    }

    if (!sbc_para.offset) {
        if (sbc_para.vol > 512) {
            while (n--) {
                if (sbc_para.vol > 512) {
                    sbc_para.vol--;
                }
                *pd = ((int)(*pd) * sbc_para.vol + 512) >> 10;
                pd++;

                if (dac_type == AUDIO_CODEC_DAC_STEREO) {
                    *pd = ((int)(*pd) * sbc_para.vol + 512) >> 10;
                    pd++;
                }
            }
            return ni;
        }

        sbc_para.offset += sbc8to16(sbc_para.ptr + sbc_para.offset, sbc_d, SBC_BNUM);
        sbc_para.last = sbc_d[0];
        sbc_para.id   = 0;
        sbc_para.fra  = 0;
    }

    int nt = 0;

    //	int step = 256 * 16000 / sample_rate;

    float iErr = 16000.0f / sample_rate;

    while (n--) {
        signed short v;

        int next;
        if (sample_rate == 16000) {
            v    = sbc_d[sbc_para.id];
            next = 1;
        } else {
            next = tlka_sbc_upresample_fra(iErr, sbc_d[sbc_para.id], &v);
        }
        if (next) {
            sbc_para.id++;
            if (sbc_para.id >= SBC_BNUM) {
                int left = sbc_para.len - sbc_para.offset;
                if (left < SBC_BNUM) {
                    if (1 || left) {
                        sbc_para.offset = sbc_para.len;
                        tmemset(sbc_d, 0, SBC_BNUM * 2);
                    }
                } else {
                    sbc_para.offset += sbc8to16(sbc_para.ptr + sbc_para.offset, sbc_d, SBC_BNUM);
                }
                sbc_para.id = 0;
            }
        }

        *pd = (((int)(*pd) * (int)sbc_para.vol) + (v * sbc_para.gain) + 512) >> 10;
        pd++;

        if (dac_type == AUDIO_CODEC_DAC_STEREO) {
            *pd = (((int)(*pd) * (int)sbc_para.vol) + (v * sbc_para.gain) + 512) >> 10;
            pd++;
        }

        nt++;
    }
    return nt;
}

#endif
