/********************************************************************************************************
 * @file    tlkmdi_bt_voice_forward.h
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
#ifndef TLKMDI_BT_VOICE_FORWARD_H
#define TLKMDI_BT_VOICE_FORWARD_H

int tlkmdi_bt_voice_forward_init(void);

bool tlkmdi_bt_voice_forward_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

bool tlkmdi_bt_voice_forward_switch(uint16_t handle, uint8_t status);

bool tlkmdi_bt_voice_forward_isBusy(void);

#endif
