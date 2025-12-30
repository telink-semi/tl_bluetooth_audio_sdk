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

/*! Version number to ensure header and binary are matching. */
#define W_AEC_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define W_AEC_VERSION_                         W_AEC_VERSION_INT(0, 2, 0)

//#define AEC_INOUT_FREQ


typedef struct
{
    /* support 5ms/7.5ms frame length @ 16000 / 48000 sampling rate
	 * 5ms	: set to 80 @ 16000 or 240 @ 48000
	 * 7.5ms: set to 120 @ 16000 or 360 @ 48000*/
    unsigned int frame_size;

    /*support 16000 or 48000*/
    unsigned int sampleRate;

    //set to 1 to enable aec post process
    //set to 0 to disable aec post process
    short en_aec_post;
} AEC_Param;

int tlka_aec_get_version();
int tlka_aec_get_scratch_size();
int tlka_aec_get_size();
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

int tlka_aec_process_frame_float(void *st, float *x_in, float *ref_in, float *x_out);
int tlka_aec_process_frame_24(void *st, int *x_in, int *ref_in, int *x_out);
int tlka_aec_process_frame_16(void *st, short *x_in, short *ref_in, short *x_out);

#endif
#endif
#ifdef __cplusplus
}
#endif
