/********************************************************************************************************
 * @file    tlka_eq_api.h
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
#ifndef EQ_LIB_H_
#define EQ_LIB_H_

#include <math.h>
#ifdef __riscv
#include <nds_filtering_math.h>
#endif
#ifdef __XTENSA__
#include "NatureDSP_Signal.h"
#endif

#define EQ_RAM_CODE 1

#if (EQ_RAM_CODE)
#define _EQ_RAM_CODE_ __attribute__((section(".ram_code"))) __attribute__((noinline))
#else
#define _EQ_RAM_CODE_
#endif

#define EQ_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define EQ_VERSION                          EQ_VERSION_INT(0, 3, 0)

typedef enum
{
    EQ_FLT_PEAKING   = 0,
    EQ_FLT_LOWPASS   = 1,
    EQ_FLT_HIGHPASS  = 2,
    EQ_FLT_BANDPASS  = 3,
    EQ_FLT_NORTCH    = 4,
    EQ_FLT_LOWSHELF  = 5,
    EQ_FLT_HIGHSHELF = 6,
} e_eq_filter_type_e;

int eq_get_version(void);

/**
 * @brief  Calculate EQ coefficient according to EQ parameter in one stage filter.
 *
 * @param[in]  sample_rate	Sample rate.
 * @param[in]  type	Type of filter.
 * @param[in]  freq	Center frequence.
 * @param[in]  q	Q value.
 * @param[in]  db	Gain value.
 * @param[out] coef_out	Pointer to coefficient.
 *
 * @returns	0-success	else-filter type error
 */
signed char eq_calculate_coefficient_per_stage(unsigned int sample_rate, e_eq_filter_type_e type, unsigned int freq, float q, float db, float *coef_out);

/**
 * @brief  Process data.
 *
 * @param[in]  para	Pointer to filter parameter.
 * @param[in]  ps	Pointer to rawdata-int16.
 * @param[in]  pd	Pointer to data after processing.
 * @param[in]  nsample	Number of data need to be processed(<512 sample).
 *
 * @returns	0-success else-in parameter error
 */
#ifdef __riscv
_EQ_RAM_CODE_ signed char eq_process_int16(nds_bq_df1_f32_t *para, signed short *ps, signed short *pd, unsigned short nsample);
#endif

#ifdef __XTENSA__
signed char eq_process_int16(bqriirf_df1_handle_t para, signed short *ps, signed short *pd, unsigned short nsample);
#endif

/**
 * @brief  Process data.
 *
 * @param[in]  para	Pointer to filter parameter.
 * @param[in]  ps	Pointer to rawdata-int32.
 * @param[in]  pd	Pointer to data after processing.
 * @param[in]  nsample	Number of data need to be processed(<512 sample).
 *
 * @returns	0-success else-in parameter error
 */
#ifdef __riscv
_EQ_RAM_CODE_ signed char eq_process_int32(nds_bq_df1_f32_t *para, signed int *ps, signed int *pd, unsigned short nsample);
#endif

#ifdef __XTENSA__
signed char eq_process_int32(bqriirf_df1_handle_t para, signed int *ps, signed int *pd, unsigned short nsample);
#endif


/**
 * @brief  Process data.
 *
 * @param[in]  para	Pointer to filter parameter.
 * @param[in]  ps	Pointer to rawdata-float.
 * @param[in]  pd	Pointer to data after processing.
 * @param[in]  nsample	Number of data need to be processed(<=512 sample).
 *
 * @returns	0-success else-in parameter error
 */
#ifdef __riscv
_EQ_RAM_CODE_ signed char eq_process_float(nds_bq_df1_f32_t *para, float *ps, float *pd, unsigned short nsample);
#endif
#endif
