/********************************************************************************************************
 * @file    tlkapp_audioCtrl.h
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
#ifndef TLKAPP_AUDIO_CTRL_H
#define TLKAPP_AUDIO_CTRL_H
#include "common/types.h"

/**
 * @brief       This function starts playing audio.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_startPlay(void);

/**
 * @brief       This function closes current playing audio.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_closePlay(void);
/**
 * @brief       This function pauses or resumes audio playback.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_PlayPause(void);


/**
 * @brief       This function plays the next audio track.
 * @param[in]   none.
 * @return      true if success, false if failure.
 */
bool tlkapp_audio_playNext(void);

/**
 * @brief       This function plays the previous audio track.
 * @param[in]   none.
 * @return      true if success, false if failure.
 */
bool tlkapp_audio_playPrev(void);

/**
 * @brief       This function starts playing a tone.
 * @param[in]   fileIndex - the tone file index.
 * @param[in]   isActive  - whether to play immediately.
 * @return      true if success, false if failure.
 */
bool tlkapp_audio_startTone(uint8_t fileIndex, uint8_t isActive);

/**
 * @brief       This function stops playing a tone.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_audio_stopTone(void);

/**
 * @brief       This function controls audio volume.
 * @param[in]   isInc - whether to increase volume (true) or decrease (false).
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_volumeCtrl(uint8_t isInc);

/**
 * @brief       This function controls audio calls.
 * @param[in]   opcode - the operation code.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_callCtrl(uint8_t opcode);

#endif // TLKAPP_AUDIO_CTRL_H
