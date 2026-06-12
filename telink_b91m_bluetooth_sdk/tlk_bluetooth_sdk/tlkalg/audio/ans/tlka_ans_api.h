/********************************************************************************************************
 * @file    tlka_ans_api.h
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
#ifndef W_NS_API_H
#define W_NS_API_H

//#include <stddef.h>

/*! Version number to ensure header and binary are matching. */
#define W_NS_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define W_NS_VERSION                          W_NS_VERSION_INT(0, 2, 0)

/* Whether to enable the GSC module */
//#define GSCON 0
//#define OUTPUTFREQ 0
//#define WEBRTC_OK (0)

typedef struct
{
    /* support 5ms/7.5ms/10ms
     * 5ms	: 50
     * 7.5ms: 75
     * 10ms	: 100 */
    unsigned int frame_size;

    /* only support 16K */
    unsigned int sampleRate;

    /* support k6dB, k12dB, k18dB, k21dB */
    unsigned int target_level;

    /* lowShelf_filter enable
     * 0: disable
     * 1: enable */
    unsigned int lowShelf_En;

    float preGain;
    float postGain;
} W_NS_CFG_PARAM;

enum target_level
{
    k6dB,
    k12dB,
    k14dB,
    k16dB,
    k18dB,
    k21dB,
    K_END
};

int tlka_w_ns_get_scratch_size();

int tlka_w_ns_get_version(void);

int tlka_w_ns_get_size();

void tlka_w_ns_init(void *sc, W_NS_CFG_PARAM w_ns_cfg_para, void *ScratchBuffer);

int tlka_w_ns_process_frame_16(void *sc, short *in_buffer, short *out_buffer);

int tlka_w_ns_process_frame_24(void *sc, int *in_buffer, int *out_buffer);

int tlka_w_ns_process_frame_float(void *sc, float *in_buffer, float *out_buffer);
#endif
