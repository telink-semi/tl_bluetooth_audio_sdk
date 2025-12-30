/********************************************************************************************************
 * @file    tlka_hybrid_alg_api.h
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
#ifndef TLKA_HYBRID_ALG_API_H
#define TLKA_HYBRID_ALG_API_H

#include "tlka_aec_api.h"
#include "tlka_gsc_api.h"
#include "tlka_ns_api.h"

//#define EN_PRINT_DEBUG

/*! Version number to ensure header and binary are matching. */
#define W_HYBRID_ALG_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define W_HYBRID_ALG_VERSION_                         W_HYBRID_ALG_VERSION_INT(0, 2, 4)


/*********************processing chian start:**********************/
/**********************3 input , 1 output**************************/
/**********************3 modules : GSC, AEC, NS*******************/
//full processing chain is below:(see HYBRID_ALG_WORK_MODE define)
//mic0_in ---->|
//             | ----> GSC ---> |
//mic1_in ---->|			    | ----> AEC ----> NS ----> out
//aec_ref_in -----------------> |
/*********************processing chian end:**********************/

//alg modules enable/disable is controlled by alg_ctrl variable passed into func tlka_hybrid_alg_process_frame
//alg module disabled will be bypassed in the processing chain
// en_gsc = alg_ctrl & GSC_CTRL_MASK
// en_aec = alg_ctrl & AEC_CTRL_MASK
// en_ns = alg_ctrl & NS_CTRL_MASK

//module control mask definition
#define GSC_CTRL_MASK     0x04
#define AEC_CTRL_MASK     0x02
#define NS_CTRL_MASK      0x01

#define HYBRID_ALG_MICNUM 2

typedef struct
{
    /* support 7.5ms
	 * 7.5ms: 120 */
    unsigned int frame_size;

    /* only support 16K */
    unsigned int sampleRate;

    GSC_Param      *gsc_para;
    AEC_Param      *aec_para;
    W_NS_CFG_PARAM *ns_para;

} HYBRID_ALG_Param;

int tlka_hybrid_alg_get_version(void);
int tlka_hybrid_alg_get_size(void);
int tlka_hybrid_alg_init(void *st_in, HYBRID_ALG_Param *pst_para, void *ScratchBuffer, unsigned int mem_size);
int tlka_hybrid_alg_get_scratch_size(void);


/*--------------------------------------------------------------------------------------*
* name : tlka_hybrid_alg_process_frame                                                  *
* st   : input struct pointer                                                           * 
* alg_ctrl : alg control variable(2bit : gsc ; 1bit : aec, 0bit : ns)                   *
* x_in : input mic signals buffer                                                       *
* ref_in : reference signal buffer                                                      *
* x_out : output signal buffer                                                          *
*---------------------------------------------------------------------------------------*/

int tlka_hybrid_alg_process_frame(void *st_in, char alg_ctrl, short *x_in, short *ref_in, short *x_out);
#endif
#ifdef __cplusplus
}
#endif
