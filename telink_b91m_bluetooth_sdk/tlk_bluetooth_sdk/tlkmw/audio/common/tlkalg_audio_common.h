/********************************************************************************************************
 * @file    tlkalg_audio_common.h
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
#ifndef _TLKALG_AUDIO_COMMON_H_
#define _TLKALG_AUDIO_COMMON_H_

typedef enum
{
    TLKALG_EQ           = BIT(0),
    TLKALG_PPM_CHN0     = BIT(1),
    TLKALG_PPM_CHN1     = BIT(2),
    TLKALG_PPM_USB_CALC = BIT(3),
} tlkalg_type_e;

void tlkalg_init_common(tlkalg_type_e alg_type);
void tlkalg_deinit_common(tlkalg_type_e alg_type);
#endif