/********************************************************************************************************
 * @file    tlkalg_2chmix.h
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
#ifndef TLKALG_2CHMIX_H
#define TLKALG_2CHMIX_H


#define TLKMDI_PLAY_2CH_LEFT  (16384) //round(sqrt(1/2)*pow2(15))
#define TLKMDI_PLAY_2CH_RIGHT (16384) //round(sqrt(1/2)*pow2(15))

/**
 * @brief   This function performs a downmix of two audio channels into one.
 * @param[in]   pLeft   - Pointer to the start of the left channel audio data.
 * @param[in]   pRight  - Pointer to the start of the right channel audio data.
 * @param[out]  pOut    - Pointer to the output buffer where the mixed audio data will be stored.
 * @param[in]   stride  - Stride between two samples in the input channels.
 * @param[in]   length  - Number of samples to process from each channel.
 * @return      None.
 */
void tlkalg_2chnmix(short *pLeft, short *pRight, short *pOut, int stride, int length);


#endif //TLKALG_2CHMIX_H
