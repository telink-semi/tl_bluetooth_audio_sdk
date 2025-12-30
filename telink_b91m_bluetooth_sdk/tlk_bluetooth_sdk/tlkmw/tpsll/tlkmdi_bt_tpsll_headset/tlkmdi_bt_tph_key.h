/********************************************************************************************************
 * @file    tlkmdi_bt_tph_key.h
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
#ifndef TLKMDI_BT_TPH_KEY_H
#define TLKMDI_BT_TPH_KEY_H


/**
 * @brief       This function initializes the key handling system by creating a static timer.
 * @return      TLK_ENONE if successful, error code otherwise.
 * @note        
 */
int tlkmdi_bt_tph_key_init(void);

/**
 * @brief       key event - tpsll_audio_volume_up.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncAudioVolumeUp(void);

/**
 * @brief       key event - tpsll_audio_volume_down.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncAudioVolumeDown(void);

/**
 * @brief       key event - tpsll_music_play_pause.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncMusicPlayPause(void);

/**
 * @brief       key event - tpsll_music_backward.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncMusicBackward(void);

/**
 * @brief       key event - tpsll_music_forward.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncMusicForward(void);

/**
 * @brief       key event - tpsll_save_pair_info.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncSavePairInfo(void);

#endif
