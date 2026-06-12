/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_PROCESSING_NS_NS_FFT_H_
#define MODULES_AUDIO_PROCESSING_NS_NS_FFT_H_

#include "arch.h"
#include <stddef.h>

// Transforms the signal from time to frequency domain.
COMMON_LIB_RAMCODE void Fft(float *time_data, float *real, float *imag, int fftsize, size_t *bit_reversal_state, float *tables);
COMMON_LIB_RAMCODE void Ifft(float *real, float *imag, float *time_data, int fftsize, size_t *bit_reversal_state, float *tables);
// Transforms the signal from frequency to time domain.
COMMON_LIB_RAMCODE void Fft_gsc(float *time_data, int fft_size, size_t *bit_reversal_state, float *tables);
COMMON_LIB_RAMCODE void Ifft_gsc(float *time_data, int fft_size, size_t *bit_reversal_state, float *tables);


#endif // MODULES_AUDIO_PROCESSING_NS_NS_FFT_H_
