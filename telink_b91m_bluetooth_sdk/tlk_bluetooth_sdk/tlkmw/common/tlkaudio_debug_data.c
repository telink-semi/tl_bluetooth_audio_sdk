/********************************************************************************************************
 * @file    tlkaudio_debug_data.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "math.h"
#include "tl_common.h"

#define PI 3.1415926536

volatile short sin_table[480] = {0};

signed short sin_48k_stereo[] __attribute__((aligned(4))) = {
    0,     0,     1069,  1069,  2120,  2120,  3134,  3134,  4095,  4095,  4986,  4986,  5792,  5792,  6499,  6499,  7094,  7094,  7568,  7568,  7912,  7912,  8121,  8121,
    8191,  8191,  8121,  8121,  7912,  7912,  7568,  7568,  7094,  7094,  6499,  6499,  5792,  5792,  4986,  4986,  4096,  4096,  3134,  3134,  2120,  2120,  1069,  1069,
    0,     0,     -1069, -1069, -2120, -2120, -3134, -3134, -4095, -4095, -4986, -4986, -5792, -5792, -6499, -6499, -7094, -7094, -7568, -7568, -7912, -7912, -8121, -8121,
    -8191, -8191, -8121, -8121, -7912, -7912, -7568, -7568, -7094, -7094, -6499, -6499, -5792, -5792, -4986, -4986, -4096, -4096, -3134, -3134, -2120, -2120, -1069, -1069,
};

signed short sin_48k_mono_d1[] __attribute__((aligned(4))) = {
    0, 1069,  2120,  3134,  4095,  4986,  5792,  6499,  7094,  7568,  7912,  8121,  8191,  8121,  7912,  7568,  7094,  6499,  5792,  4986,  4096,  3134,  2120,  1069,
    0, -1069, -2120, -3134, -4095, -4986, -5792, -6499, -7094, -7568, -7912, -8121, -8191, -8121, -7912, -7568, -7094, -6499, -5792, -4986, -4096, -3134, -2120, -1069,
};

signed short sin_48k_mono_d2[] __attribute__((aligned(4))) = {
    0, 1069,  2120,  3134,  4095,  4986,  5792,  6499,  7094,  7568,  7912,  8121,  8191,  8121,  7912,  7568,  7094,  6499,  5792,  4986,  4096,  3134,  2120,  1069,
    0, -1069, -2120, -3134, -4095, -4986, -5792, -6499, -7094, -7568, -7912, -8121, -8191, -8121, -7912, -7568, -7094, -6499, -5792, -4986, -4096, -3134, -2120, -1069,
};

signed int sin_48k_mono_24bit_d1[] __attribute__((aligned(4))) = {
    0,        1094933,  2171131,  3210181,  4194303,  5106660,  5931641,  6655129,  7264747,  7750062,  8102772,  8316841,  8388607,  8316841,  8102772,  7750062,
    7264746,  6655129,  5931641,  5106660,  4194303,  3210179,  2171130,  1094932,  0,        -1094933, -2171131, -3210181, -4194305, -5106661, -5931642, -6655129,
    -7264747, -7750063, -8102772, -8316841, -8388607, -8316841, -8102772, -7750062, -7264747, -6655129, -5931639, -5106660, -4194301, -3210180, -2171129, -1094933,
};

signed int sin_48k_mono_24bit_d2[] __attribute__((aligned(4))) = {
    0,        1094933,  2171131,  3210181,  4194303,  5106660,  5931641,  6655129,  7264747,  7750062,  8102772,  8316841,  8388607,  8316841,  8102772,  7750062,
    7264746,  6655129,  5931641,  5106660,  4194303,  3210179,  2171130,  1094932,  0,        -1094933, -2171131, -3210181, -4194305, -5106661, -5931642, -6655129,
    -7264747, -7750063, -8102772, -8316841, -8388607, -8316841, -8102772, -7750062, -7264747, -6655129, -5931639, -5106660, -4194301, -3210180, -2171129, -1094933,
};

int array_count = sizeof(sin_48k_mono_24bit_d2) / sizeof(sin_48k_mono_24bit_d2[0]);

signed int sin_48k_stereo_24bit[96] __attribute__((aligned(4))) = {
    0,        0,        1094933,  1094933,  2171131,  2171131,  3210181,  3210181,  4194303,  4194303,  5106660,  5106660,  5931641,  5931641,  6655129,  6655129,
    7264747,  7264747,  7750062,  7750062,  8102772,  8102772,  8316841,  8316841,  8388607,  8388607,  8316841,  8316841,  8102772,  8102772,  7750062,  7750062,
    7264746,  7264746,  6655129,  6655129,  5931641,  5931641,  5106660,  5106660,  4194303,  4194303,  3210179,  3210179,  2171130,  2171130,  1094932,  1094932,
    0,        0,        -1094933, -1094933, -2171131, -2171131, -3210181, -3210181, -4194305, -4194305, -5106661, -5106661, -5931642, -5931642, -6655129, -6655129,
    -7264747, -7264747, -7750063, -7750063, -8102772, -8102772, -8316841, -8316841, -8388607, -8388607, -8316841, -8316841, -8102772, -8102772, -7750062, -7750062,
    -7264747, -7264747, -6655129, -6655129, -5931639, -5931639, -5106660, -5106660, -4194301, -4194301, -3210180, -3210180, -2171129, -2171129, -1094933, -1094933,
};

signed short sin_16k_mono[16] = {
    0, 12539, 23169, 30272, 32767, 30272, 23169, 12539, 4, -12539, -23169, -30272, -32767, -30272, -23169, -12539,
}; //16K

signed short sin_16k_stereo[32] = {
    0, 0, 12539,  12539,  23169,  23169,  30272,  30272,  32767,  32767,  30272,  30272,  23169,  23169,  12539,  12539,
    4, 4, -12539, -12539, -23169, -23169, -30272, -30272, -32767, -32767, -30272, -30272, -23169, -23169, -12539, -12539,
}; //16K


uint16_t g_sin_rptr = 0;

/**
 * @brief       This function is used to get sine wave data for codec debugging
 * @param[out]  p_des - the destination buffer to store the sine wave data
 * @param[in]   sample_num - the number of samples to generate
 * @return      1 if success, 0 if failure.
 */
_attribute_ram_code_sec_noinline_ uint8_t tlkdrv_codec_get_sin_data(int16_t *p_des, uint16_t sample_num)
{
    uint16_t i;

    if (g_sin_rptr == 0) {
        for (i = 0; i < 480; i++) {
            sin_table[i] = (short)(16384 * sinf(2 * PI * i / 16) + 0.5);
        }
    }

    for (i = 0; i < sample_num; i++) {
        *p_des++ = sin_table[g_sin_rptr++];
    }

    if (g_sin_rptr >= 480) {
        g_sin_rptr = 0;
    }

    return 1;
}

/**
 * @brief       This function is used to get sine wave data at 44.1KHz sample rate for codec debugging
 * @param[out]  p_des - the destination buffer to store the sine wave data
 * @param[in]   sample_num - the number of samples to generate
 * @return      1 if success, 0 if failure.
 */
uint8_t tlkdrv_codec_get_sin_44p1K_data(int32_t *p_des, uint16_t sample_num)
{
    uint16_t i;

    if (g_sin_rptr == 0) {
        for (i = 0; i < sample_num; i++) {
            p_des[i] = (0x3fffff * sinf(2 * PI * i / 44.1) + 0.5);
        }
    }

    return 1;
}

/**
 * @brief       This function is used to get stereo sine wave data at 44.1KHz sample rate for codec debugging
 * @param[out]  p_des - the destination buffer to store the sine wave data
 * @param[in]   sample_num - the number of samples to generate
 * @return      1 if success, 0 if failure.
 */
uint8_t tlkdrv_codec_get_sin_44p1K_stereo_data(int32_t *p_des, uint16_t sample_num)
{
    uint16_t i;

    if (g_sin_rptr == 0) {
        for (i = 0; i < sample_num; i = i + 2) {
            p_des[i]     = (0x3fffff * sinf(2 * PI * i / 44.1) + 0.5);
            p_des[i + 1] = (0x3fffff * sinf(2 * PI * i / 44.1) + 0.5);
        }
    }

    return 1;
}
