/********************************************************************************************************
 * @file    tlkalg_bbf_interface.h
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

#ifndef TLKALG_AUDIO_BBF_TLKALG_BBF_INTERFACE_H_
#define TLKALG_AUDIO_BBF_TLKALG_BBF_INTERFACE_H_

#include "tlka_bbf_alg_api.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern volatile uint8_t bbf_pypass_en;

uint16_t tlkalg_bbf_get_size(uint8_t channel);
int8_t   tlkalg_bbf_init(uint8_t *pbuff, uint8_t channel);
int8_t   tlkalg_bbf_deinit(void);
int      tlkalg_bbf_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);


#endif /* TLKALG_AUDIO_BBF_TLKALG_BBF_INTERFACE_H_ */
