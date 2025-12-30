/********************************************************************************************************
 * @file    tlka_gsc_api.h
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
#ifndef TLKA_GSC_API_H
#define TLKA_GSC_API_H

#define GSC_INOUT_FREQ 1

#if GSC_INOUT_FREQ

#else
    // typedef float spx_word16_t;
    // typedef float spx_word32_t;

    // typedef struct complexF_
    // {
    // 	spx_word16_t real;
    // 	spx_word16_t imag;
    // }complexF;

    // typedef struct complexFloat_
    // {
    // 	float real;
    // 	float imag;
    // }complexFloat;

#endif


typedef struct
{
    /* support 5ms/7.5ms
	 * 5ms	: 80
	 * 7.5ms: 120 */
    unsigned int frame_size;

    /* only support 16K */
    unsigned int sampleRate;

    /* only support 0 or 1
	 * 0: perform no operation
	 * 1: the main and vice-mics are in reverse order, exchanged */
    unsigned int exchange_mic;

} GSC_Param;

//int tlka_gsc_get_version(void);
int tlka_gsc_get_size(void);
int tlka_gsc_init(void *st, GSC_Param gsctate);


/*------------------------------------------------------*
* name: tlka_gsc_process_frame                          *
* st   : input struct pointer                           *
* x_in : input mic spec buffer                        *
* x_out : output spec buffer                          *
*-------------------------------------------------------*/
#if GSC_INOUT_FREQ
int tlka_gsc_process_frame(void *st, float *x_in, float *x_out);
#else
int tlka_gsc_process_frame(gscState *st, short *x_in, short *ref_in, float *x_out, int was_speech);
#endif

#endif
#ifdef __cplusplus
}
#endif
