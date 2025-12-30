/********************************************************************************************************
 * @file    tlkalg_2chmix.c
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
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkalg/audio/mixer/tlkalg_2chmix.h"
 
/**
 * @brief   This function performs a downmix of two audio channels into one.
 * @param[in]   pLeft   - Pointer to the start of the left channel audio data.
 * @param[in]   pRight  - Pointer to the start of the right channel audio data.
 * @param[out]  pOut    - Pointer to the output buffer where the mixed audio data will be stored.
 * @param[in]   stride  - Stride between two samples in the input channels.
 * @param[in]   length  - Number of samples to process from each channel.
 * @return      None.
 */
void tlkalg_2chnmix(short *pLeft, short *pRight, short *pOut, int stride, int length)
{
    int i;
    int sum;
    for (i = 0; i < length; i++) {
        sum = pRight[i * stride] * TLKMDI_PLAY_2CH_RIGHT + pLeft[i * stride] * TLKMDI_PLAY_2CH_LEFT;
        sum = (sum + (1 << 14)) >> 15;
        if (sum > 32767) {
            pOut[i] = 32767;
        } else if (sum < -32768) {
            pOut[i] = -32768;
        } else {
            pOut[i] = sum;
        }
    }
}
