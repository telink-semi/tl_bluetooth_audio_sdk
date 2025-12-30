/********************************************************************************************************
 * @file    tlkmdi_audio_tpsif_key.h
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
#ifndef TLKMDI_AUDIO_TPH_INTERFACE_KEY_H
#define TLKMDI_AUDIO_TPH_INTERFACE_KEY_H

typedef enum
{
    AUD_TPSIF_KEYID_VOL_UP = 0,
    AUD_TPSIF_KEYID_VOL_DOWN,
    AUD_TPSIF_KEYID_PLAY_PAUSE,
    AUD_TPSIF_KEYID_PLAY_FORWARD,
    AUD_TPSIF_KEYID_PLAY_BACKWARD,
    AUD_TPSIF_KEYID_NUM,
} AUD_TPSIF_KEYID_ENUM;

/**
 * @brief  Trigger one TPS hid key .
 * @param[in]  handle - acl handle
 * @param[in]  key - hid key id,refer to AUD_TPSIF_KEYID_ENUM
 * @returns Returning TLK_ENONE(0x00) means the send process success.
 *          If others value is returned means the send process fail.
 */
int tlkmdi_audio_tpsif_sendKey(uint16_t handle, uint8_t key);

#endif // TLKMDI_AUDIO_TPH_INTERFACE_KEY_H
