/********************************************************************************************************
 * @file    tlka_asrc_api.h
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
#ifndef TLKA_ASRC_API_H
#define TLKA_ASRC_API_H

#ifndef MY_RESAMPLE_RAMCODE
#define MY_RESAMPLE_RAMCODE
#endif

#define ASRC_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define ASRC_VERSION                          ASRC_VERSION_INT(0, 10, 0)

/** src **/
#define ASRC_FRAME_LEN_MAX 480 //10ms@48khz

// old algo only
#define ASRC_DATA_LEN_MAX   (ASRC_FRAME_LEN_MAX * 4)
#define ASRC_FILTER_LEN_MAX (48 * 8)

// hp algo
//#define HP_BUF_LEN (ASRC_FRAME_LEN_MAX * 4 + 90 + 20)
#define HP_BUF_LEN (ASRC_FRAME_LEN_MAX * 2 + 90 + 20)

//typedef signed int Int24;
typedef long long int int64_t;

/* resampling type */
typedef enum
{
    TLKA_ASRC_44_TO_48 = 0,
    TLKA_ASRC_48_TO_44,
    TLKA_ASRC_16_TO_48,
    TLKA_ASRC_48_TO_16,
    TLKA_ASRC_24_TO_48,
    TLKA_ASRC_48_TO_24,
    TLKA_ASRC_48_TO_32,
    TLKA_ASRC_32_TO_48,
    TLKA_ASRC_16_TO_44,
    TLKA_ASRC_44_TO_16,
    TLKA_ASRC_44_TO_24,
    TLKA_ASRC_44_TO_32,
    TLKA_ASRC_16_TO_24,
    TLKA_ASRC_24_TO_16,
    TLKA_ASRC_16_TO_96,
    TLKA_ASRC_96_TO_16,
    TLKA_ASRC_48_TO_192,
    TLKA_ASRC_192_TO_48,
    TLKA_ASRC_44_TO_192,
    TLKA_ASRC_16_TO_192,
    TLKA_ASRC_192_TO_16,
    TLKA_ASRC_8_TO_192,
    TLKA_ASRC_192_TO_8,
    TLKA_ASRC_8_TO_16,
    TLKA_ASRC_16_TO_8,
    TLKA_ASRC_SFO, //only ppm compensation
    TLKA_ASRC_FLAG_END
} TLKA_ASRC_FLAGS;

/* channel type */
typedef enum
{
    TLKA_ASRC_SINGLE = 0,
    TLKA_ASRC_STEREO

} TLKA_ASRC_CHANNEL;

typedef enum
{
    TLKA_ASRC_OK              = 0,
    TLKA_ASRC_INVALID_TAG     = -1,
    TLKA_ASRC_INVALID_CHANNEL = -2
} TLKA_ASRC_ERROR;

/* data struct */
typedef struct TLKA_ASRC_PARAM
{
    unsigned int num_rate;
    unsigned int den_rate;
    int          int_advance;
    int          frac_advance;
    float        cutoff;
    int          filter_len;
    unsigned int oversample;
    int          initialised;
    int          started;
    int          stereo;

    TLKA_ASRC_FLAGS asrc_tag;
    // TLKA_ASRC_CHANNEL channel_mode;
    int          last_sample;
    unsigned int samp_frac_num;

    // short *src_buf_left;
    short src_buf_left[ASRC_FILTER_LEN_MAX + ASRC_DATA_LEN_MAX];
    //short src_buf_right[ASRC_FILTER_LEN_MAX + ASRC_DATA_LEN_MAX];
    short *src_buf_right;
    short *sinc_table;

    // resampler_basic_func resampler_ptr;
} tlka_asrc_param;

/* data struct */
#if 0 //24-bit fast asrc disabled
typedef struct TLKA_ASRC_24_BIT_PARAM
{
    unsigned int num_rate;
    unsigned int den_rate;
    int int_advance;
    int frac_advance;
    float cutoff;
    int filter_len;
    unsigned int oversample;
    int initialised;
    int started;
    int stereo;

    TLKA_ASRC_FLAGS asrc_tag;
    // TLKA_ASRC_CHANNEL channel_mode;
    int last_sample;
    unsigned int samp_frac_num;

    // Int24 *src_buf_left;
    int src_buf_left[ASRC_FILTER_LEN_MAX + ASRC_DATA_LEN_MAX];
    //int src_buf_right[ASRC_FILTER_LEN_MAX + ASRC_DATA_LEN_MAX];
    int *src_buf_right;
    int *sinc_table;

    //int hpmode;
    //int hp_coeff_num;
    //int hp_coeff[30];
    //int *hp_buf_l;
    //int *hp_buf_r;

    // resampler_basic_func resampler_ptr;
} tlka_asrc_24_bit_param;
#endif


/* data struct */
typedef struct TLKA_ASRC_16_BIT_HP_PARAM
{
    unsigned int num_rate;
    unsigned int den_rate;

    int config_rst;
    int buffer_rst;
    int int_advance;
    int frac_advance;
    int int_osr;

    TLKA_ASRC_FLAGS asrc_tag;
    int             last_sample;
    unsigned int    samp_frac_num;

    int stereo;

    int    hp_coeff_num;
    short *hp_coeff;

    short hp_buf_l[HP_BUF_LEN];
    short hp_filt_l[HP_BUF_LEN];
    short hp_filt_l_stat[HP_BUF_LEN];
    void *hp_filter_l;

    short *hp_buf_r;
    short *hp_filt_r;
    short *hp_filt_r_stat;
    void  *hp_filter_r;

    int mode;
} tlka_asrc_16_bit_hp_param;

/* data struct */
typedef struct TLKA_ASRC_24_BIT_HP_PARAM
{
    unsigned int num_rate;
    unsigned int den_rate;

    int config_rst;
    int buffer_rst;
    int int_advance;
    int frac_advance;
    int int_osr;

    TLKA_ASRC_FLAGS asrc_tag;
    int             last_sample;
    unsigned int    samp_frac_num;

    int stereo;

    int  hp_coeff_num;
    int *hp_coeff;

    int   hp_buf_l[HP_BUF_LEN];
    int   hp_filt_l[HP_BUF_LEN];
    int   hp_filt_l_stat[HP_BUF_LEN];
    void *hp_filter_l;

    int  *hp_buf_r;
    int  *hp_filt_r;
    int  *hp_filt_r_stat;
    void *hp_filter_r;

    int mode;
} tlka_asrc_24_bit_hp_param;

/* hp mode  */
typedef enum
{
    TLKA_ASRC_FASTMODE = 0,
    TLKA_ASRC_HPMODE

} TLKA_ASRC_MODE;

int tlka_asrc_get_version(void);
/*---------------------------------------------------------------*
 * flag : flag for enumerating resampling types                  *
 * channel : defined the input channel type                      *
 *---------------------------------------------------------------*/
TLKA_ASRC_ERROR tlka_asrc_16_bit_get_size(TLKA_ASRC_FLAGS flag, TLKA_ASRC_CHANNEL channel);
int             tlka_asrc_16_bit_init(void *state, TLKA_ASRC_FLAGS flag, TLKA_ASRC_CHANNEL channel);

TLKA_ASRC_ERROR tlka_asrc_16_bit_hp_get_size(TLKA_ASRC_CHANNEL channel);
int             tlka_asrc_16_bit_hp_init(void *state, TLKA_ASRC_FLAGS flag, TLKA_ASRC_CHANNEL channel, int ppm);

TLKA_ASRC_ERROR tlka_asrc_24_bit_hp_get_size(TLKA_ASRC_CHANNEL channel);
int             tlka_asrc_24_bit_hp_init(void *state, TLKA_ASRC_FLAGS flag, TLKA_ASRC_CHANNEL channel, int ppm);

/*---------------------------------------------------------------*
 * name : tlka_asrc_process_frame                                *
 * ps : 16 bit input for resampling                              *
 * n : input frame size                                          *
 * pd : 16 bit output                                            *
 *---------------------------------------------------------------*/
int tlka_asrc_16_bit_process_frame(void *state, short *ps, int n, short *pd);
int tlka_asrc_16_bit_process_frame_hp(void *state, short *ps, int n, short *pd);
int tlka_asrc_24_bit_process_frame_hp(void *state, int *ps, int n, int *pd);

#if 0 //24-bit fast asrc disabled
int tlka_asrc_24_bit_process_frame(void *state, int *ps, int n, int *pd);
TLKA_ASRC_ERROR tlka_asrc_24_bit_get_size(TLKA_ASRC_FLAGS flag, TLKA_ASRC_CHANNEL channel);
int tlka_asrc_24_bit_init(void *state, TLKA_ASRC_FLAGS flag, TLKA_ASRC_CHANNEL channel);
#endif
#endif
