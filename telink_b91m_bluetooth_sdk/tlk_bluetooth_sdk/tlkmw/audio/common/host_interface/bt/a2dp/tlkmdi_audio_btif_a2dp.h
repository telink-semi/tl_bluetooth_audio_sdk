/********************************************************************************************************
 * @file    tlkmdi_audio_btif_a2dp.h
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
#pragma once

typedef void (*TlkMdiAudBtifA2dpSrcStartResultCB)(uint16_t handle, int32_t result);

typedef void (*TlkMdiAudBtifA2dpSrcStatusChgCB)(uint16_t handle, uint8_t is_start, uint16_t mtu_size);

void tlkmdi_audio_btif_a2dp_src_start(uint16_t handle, uint32_t freq, TlkMdiAudBtifA2dpSrcStartResultCB cb);

void tlkmdi_audio_btif_a2dp_src_stop(uint16_t handle);

void tlkmdi_audio_btif_a2dp_src_register_status_change_cb(TlkMdiAudBtifA2dpSrcStatusChgCB cb);