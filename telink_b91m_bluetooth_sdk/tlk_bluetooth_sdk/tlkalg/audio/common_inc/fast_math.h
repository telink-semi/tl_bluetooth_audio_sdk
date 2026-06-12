/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef _FAST_MATH_H_
#define _FAST_MATH_H_

#include "arch.h"
#include <math.h>
#include <stdint.h>

#define kLogOf2   0.69314718056f
#define kLog10Ofe 0.4342944819f

float LogApproximation(float x);
void  LogApproximation_array(float *x, float *y, size_t size);

float ExpApproximation(float x);
void  ExpApproximation_array(float *x, float *y, size_t size);
void  ExpApproximationSignFlip_array(float *x, float *y, size_t x_size);

static inline float SqrtFastApproximation(float x)
{
    union
    {
        int   intPart;
        float floatPart;
    } convertor;

    union
    {
        int   intPart;
        float floatPart;
    } convertor2;

    convertor.floatPart  = x;
    convertor2.floatPart = x;
    convertor.intPart    = 0x1FBCF800 + (convertor.intPart >> 1);
    convertor2.intPart   = 0x5f3759df - (convertor2.intPart >> 1);
    return 0.5f * (convertor.floatPart + (x * convertor2.floatPart));
}

static inline float FastLog2f(float in)
{
    //float FastLog2f(float in) {
    //RTC_DCHECK_GT(in, .0f);
    // Read and interpret float as uint32_t and then cast to float.
    // This is done to extract the exponent (bits 30 - 23).
    // "Right shift" of the exponent is then performed by multiplying
    // with the constant (1/2^23). Finally, we subtract a constant to
    // remove the bias (https://en.wikipedia.org/wiki/Exponent_bias).
    union
    {
        float    dummy;
        uint32_t a;
    } x = {in};

    float out = x.a;
    out *= 1.1920929e-7f; // 1/2^23
    out -= 126.942695f;   // Remove bias.
    return out;
}

#if 0
    float Pow2Approximation(float p) {
        // TODO(peah): Add fast approximate implementation.
        return powf(2.f, p);
    }
#else

static inline float Pow2Approximation(float x)
{
    //RTC_DCHECK_LT(x, -126.0f);
    //RTC_DCHECK_LT(x, 128.0f);
    if (x < -126.0) {
        return 0.0;
    }

    union
    {
        float    f;
        uint32_t d;
    } pow2_i;

    float i = floorf(x);
    float f = x - i; // [0, 1)

#if 1
    float pow2_f = 0.34363414f * f + 0.65636586f;
    pow2_f       = pow2_f * f + 1.0;

#else
    const float c0 = 1, c1 = 0.65636586f, c2 = 0.34363414f;
    float       pow2_f = c2;
    pow2_f             = pow2_f * f + c1;
    pow2_f             = pow2_f * f + c0;
#endif
    pow2_i.d = ((int)i + 127) << 23;
    return pow2_f * pow2_i.f;
}
#endif

static inline float PowApproximation(float x, float p)
{
    return Pow2Approximation(p * FastLog2f(x));
}

#endif // MODULES_AUDIO_PROCESSING_NS_FAST_MATH_H_
