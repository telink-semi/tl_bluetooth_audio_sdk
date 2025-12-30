/********************************************************************************************************
 * @file    tlka_ppm_asrc_api.h
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
#ifndef TLKA_PPM_MULTI_CH_API_H
#define TLKA_PPM_MULTI_CH_API_H

#if (TLK_ALG_PPM_ENABLE && (MCU_CORE_TYPE != MCU_CORE_TL721X))
    #include "stdio.h"

    #define PPM_ASRC_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
    #define PPM_ASRC_VERSION                          PPM_ASRC_VERSION_INT(0, 1, 1)

    #define PPM_OVERSAMPLE                            (8)
    #define PPM_BASE_FILTER_LEN                       (20)
    #define PPM_DATA_LEN_MAX                          (600) // adjust to 600 (12ms).

    #define S24_MAX                                   ((1 << 23) - 1)
    #define S24_MIN                                   (-(1 << 23))

    #define USB_OUT_SAMPLES                           (240)
    #define USB_SPEAKER_CHANNEL                       (2)

    #if __riscv
        #define REARRANGE_SINC_TABLE
    #endif

/* channel type */
typedef enum
{
    TLKA_PPM_ASRC_SINGLE = 0,
    TLKA_PPM_ASRC_STEREO

} TLKA_PPM_ASRC_CHANNEL;

typedef struct TLKA_PPM_ASRC_16_bit_PARAM
{
    unsigned int in_rate;
    unsigned int out_rate;
    unsigned int num_rate;
    unsigned int den_rate;
    int          initialised;

    int   int_advance;
    int   frac_advance;
    float cutoff;

    /* These are per-channel */
    int          last_sample;
    unsigned int samp_frac_num;

    int                   last_sample_o;
    unsigned int          samp_frac_num_o;
    TLKA_PPM_ASRC_CHANNEL channel_mode;

    int switch_flag;
    #ifdef REARRANGE_SINC_TABLE
    // short sinc_table[4*(PPM_OVERSAMPLE* PPM_BASE_FILTER_LEN+8)];
    short sinc_table[4 * (8 * 28 + 8)];
    short sinc_table2[4 * (PPM_OVERSAMPLE * 16 + 8)];
    #else
    short sinc_table[PPM_OVERSAMPLE * PPM_BASE_FILTER_LEN + 8];
    #endif
    short ppm_data_buf_left[28 + PPM_DATA_LEN_MAX];
    short ppm_data_buf_right[28 + PPM_DATA_LEN_MAX];

} tlka_ppm_asrc_16_bit_param;

int  tlka_ppm_asrc_get_version(void);
int  tlka_ppm_asrc_16_bit_get_size(void);
void tlka_ppm_asrc_16_bit_init(void *st, TLKA_PPM_ASRC_CHANNEL channel, int ppm);

int tlka_ppm_asrc_16_bit_frame_process(void *st, short *ps, int n, short *pd);


#endif
#endif
