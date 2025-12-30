/********************************************************************************************************
 * @file    tlka_aec_api.h
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
#ifndef TLKA_AEC_API_H
#define TLKA_AEC_API_H

//#include <stddef.h>

#define AEC_INOUT_FREQ 1

typedef struct
{
    /* support 5ms/7.5ms
	 * 5ms	: 80
	 * 7.5ms: 120 */
    unsigned int frame_size;

    /* only support 16K */
    unsigned int sampleRate;

    //set to 1 to enable aec post process
    //set to 0 to disable aec post process
    short en_aec_post;
} AEC_Param;

typedef struct
{
    int frame_cnt;
    int fft_size;
    int frame_size;
    int freq_size;


    float A;
    float alpha;
    float alpha_wt;
} stAecLog;

//int tlka_aec_get_version(void);
int tlka_aec_get_size(void);
int tlka_aec_init(void *st_in, AEC_Param *pst_aecstate, void *ScratchBuffer);

#if AEC_INOUT_FREQ

/*------------------------------------------------------*
* name: tlka_aec_process_frame                          *
* st   : input struct pointer                           *
* x_in : input mic and ref spec buffer                *
* x_out : output spec buffer                          *
*-------------------------------------------------------*/
int tlka_aec_process_frame(void *st, float *x_in, float *x_out);

#else

int tlka_aec_process_frame(void *st, short *x_in, short *ref_in, float *x_out);

#endif
#endif
#ifdef __cplusplus
}
#endif
