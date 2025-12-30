/********************************************************************************************************
 * @file    tlka_agc_api.h
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
#ifndef TLKA_AGC_API_H
#define TLKA_AGC_API_H

//#define EN_PRINT_DEBUG

/*! Version number to ensure header and binary are matching. */
#define W_HYBRID_ALG_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define W_HYBRID_ALG_VERSION_                         W_HYBRID_ALG_VERSION_INT(0, 3, 2)

typedef struct
{
    /* support 7.5ms/10ms/15ms @ 16k hz
	  7.5ms: 120
	  10ms: 160
	  15ms: 240 */
    unsigned int frame_size;

    /*support 16K/48K */
    unsigned int sampleRate;

    /*target agc output signal level in DBFS*/
    int target_out_level_dbfs;

    /*noise signal level threshold in DBFS(signal below this will not be amplified)*/
    int noise_level_dbfs_thld;
} AGC_Param;

int tlka_agc_alg_get_version(void);
int tlka_agc_alg_get_size();
int tlka_agc_alg_init(void *p_in, AGC_Param *pst_para, void *pScratchBuffer, unsigned int mem_size);
int tlka_agc_alg_get_scratch_size(void);


/*--------------------------------------------------------------------------------------*
* name : tlka_agc_alg_process_frame                                                  	*
* st   : input struct pointer                                                           *
* x_in : input mic signals buffer(16 bit)                                               *                                                    *
* x_out : output signal buffer(16 bit)                                                  *
*---------------------------------------------------------------------------------------*/
int tlka_agc_alg_process_frame(void *st_in, short *x_in, short *x_out);

/*--------------------------------------------------------------------------------------*
* name : tlka_agc_alg_process_frame                                                  	*
* st   : input struct pointer                                                           *
* x_in : input mic signals buffer(float)                                               *                                                    *
* x_out : output signal buffer(float)                                                  *
*---------------------------------------------------------------------------------------*/
int tlka_agc_alg_process_frame_float(void *st_in, float *x_in, float *x_out);

#endif
#ifdef __cplusplus
}

#endif
