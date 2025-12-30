/********************************************************************************************************
 * @file    tlkmdi_audio_hostif_tone.h
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
#ifndef TLKMDI_AUDIO_HOST_INTERFACE_TONE_H
#define TLKMDI_AUDIO_HOST_INTERFACE_TONE_H

/**
 * @brief Callback function for tone synchronization.
 * @param[in] syncTick - The synchronization tick.
 */
typedef void (*TlkMdiAudHostifToneSyncCB)(uint32_t syncTick);

/**
 * @brief Check whether the tone playing request needs to be refused.
 * @param[in] tone_id - The ID of the tone to be played.
 * @return true - The tone playing request should be refused.
 * @return false - The tone playing request can be accepted.
 */
bool tlkmdi_audio_hostif_tone_isNeedRefuse(uint8_t tone_id);

/**
 * @brief Check whether active tone playing is supported.
 * @param[in] tone_id - The ID of the tone to be played.
 * @return true - Active tone playing is supported.
 * @return false - Active tone playing is not supported.
 */
bool tlkmdi_audio_hostif_tone_isSupportActivePlay(uint8_t tone_id);

/**
 * @brief Check whether synchronization is needed for tone playing.
 * @return true - Tone playing requires synchronization.
 * @return false - Tone playing does not require synchronization.
 */
bool tlkmdi_audio_hostif_tone_isNeedSyncPlay(void);

/**
 * @brief Request synchronized tone playing.
 * @param[in] tone_id - The ID of the tone to be played.
 * @return 0 - Success.
 * @return TLK_ENOSUPPORT - Not supported.
 */
int tlkmdi_audio_hostif_tone_requestSyncPlay(uint8_t tone_id);

/**
 * @brief Register callback function for tone synchronization.
 * @param[in] cb - Callback function for tone synchronization.
 * @return 0 - Success.
 * @return TLK_ENOSUPPORT - Not supported.
 */
int tlkmdi_audio_hostif_tone_regSyncCB(TlkMdiAudHostifToneSyncCB cb);
#endif // TLKMDI_AUDIO_HOST_INTERFACE_CORE_H
