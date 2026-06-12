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
#define ASRC_VERSION                          ASRC_VERSION_INT(0, 5, 2)

/** src **/
#define ASRC_DATA_LEN_MAX   (1440)
#define ASRC_FILTER_LEN_MAX (48 * 8)

// #define FRAME_SZ  (75)   // FRAME_SZ = ceil(sample_rate / frame_len);

/* resampling type */
typedef enum
{
    TLKA_ASRC_44_TO_48 = 0,
    TLKA_ASRC_48_TO_44,
    TLKA_ASRC_16_TO_48,
    TLKA_ASRC_48_TO_16,
    TLKA_ASRC_24_TO_48,
    TLKA_ASRC_48_TO_24,
    TLKA_ASRC_32_TO_48,
    TLKA_ASRC_48_TO_32,
    TLKA_ASRC_16_TO_44,
    TLKA_ASRC_44_TO_16,
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
    short  src_buf_left[ASRC_FILTER_LEN_MAX + ASRC_DATA_LEN_MAX];
    short *src_buf_right;
    short *sinc_table;

    // resampler_basic_func resampler_ptr;
} tlka_asrc_param;

/*---------------------------------------------------------------*
 * flag : flag for enumerating resampling types                  *
 * channel : defined the input channel type                      *
 *---------------------------------------------------------------*/
int             tlka_asrc_init(void *state, TLKA_ASRC_FLAGS flag, TLKA_ASRC_CHANNEL channel);
TLKA_ASRC_ERROR tlka_asrc_get_size(TLKA_ASRC_FLAGS flag, TLKA_ASRC_CHANNEL channel);
int             tlka_asrc_get_version(void);

/*---------------------------------------------------------------*
 * name : tlka_asrc_process_frame                                *
 * ps : 16 bit input for resampling                              *
 * n : input frame size                                          *
 * pd : 16 bit output                                            *
 *---------------------------------------------------------------*/
int tlka_asrc_process_frame(void *state, short *ps, int n, short *pd);

#endif
