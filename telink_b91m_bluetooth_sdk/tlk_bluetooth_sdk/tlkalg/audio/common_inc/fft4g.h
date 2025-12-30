/********************************************************************************************************
 * @file    fft4g.h
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
#ifndef COMMON_AUDIO_THIRD_PARTY_OOURA_FFT_SIZE_256_FFT4G_H_
#define COMMON_AUDIO_THIRD_PARTY_OOURA_FFT_SIZE_256_FFT4G_H_

#include "arch.h"
#include <stddef.h>

// Refer to fft4g.c for documentation.
COMMON_LIB_RAMCODE    void WebRtc_rdft(size_t n, int isgn, float *a, size_t *ip, float *w);


#endif  // COMMON_AUDIO_THIRD_PARTY_OOURA_FFT_SIZE_256_FFT4G_H_
