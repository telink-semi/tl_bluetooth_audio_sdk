/********************************************************************************************************
 * @file    ns_fft.h
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
#ifndef MODULES_AUDIO_PROCESSING_NS_NS_FFT_H_
#define MODULES_AUDIO_PROCESSING_NS_NS_FFT_H_

#include "arch.h"
#include <stddef.h>

// Transforms the signal from time to frequency domain.
COMMON_LIB_RAMCODE   void Fft(float* time_data,float* real,float* imag, int fftsize, size_t *bit_reversal_state, float *tables);
COMMON_LIB_RAMCODE void Ifft(float* real, float* imag, float* time_data, int fftsize, size_t *bit_reversal_state, float *tables);
// Transforms the signal from frequency to time domain.
COMMON_LIB_RAMCODE   void Fft_gsc(float* time_data, int fft_size, size_t *bit_reversal_state, float *tables);
COMMON_LIB_RAMCODE void Ifft_gsc(float* time_data,int fft_size, size_t *bit_reversal_state, float *tables);


#endif  // MODULES_AUDIO_PROCESSING_NS_NS_FFT_H_
