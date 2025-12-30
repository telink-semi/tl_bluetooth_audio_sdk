/********************************************************************************************************
 * @file    tlkusb_uacctr.h
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
#ifndef TLKUSB_AUDIO_CTR_H
#define TLKUSB_AUDIO_CTR_H

#define TLKLIB_UAC_KEY_TIMEOUT 100000//450000 //us
#define MEDIA_REPORT_DATA_LEN  4

enum
{
    TLKUSB_UAC_KEY_STATE_RELEASED = 0,
    TLKUSB_UAC_KEY_STATE_PRESSED  = 1,
};

/**
 * @brief       This function simulates pressing the volume up key.
 * @return      none.
 */
void tlkusb_uacctrl_volume_up(void);

/**
 * @brief       This function simulates pressing the volume down key.
 * @return      none.
 */
void tlkusb_uacctrl_volume_down(void);

/**
 * @brief       This function simulates pressing the play next track key.
 * @return      none.
 */
void tlklib_uacctrl_play_next(void);

/**
 * @brief       This function simulates pressing the play previous track key.
 * @return      none.
 */
void tlklib_uacctrl_play_prev(void);

/**
 * @brief       This function simulates pressing the play/pause key.
 * @return      none.
 */
void tlklib_uacctrl_play_pause(void);

/**
 * @brief       This function accepts an incoming call.
 * @return      TLK_ENONE - success.
 */
int tlklib_uacctrl_callAccept(void);

/**
 * @brief       This function hangs up a call.
 * @return      TLK_ENONE - success.
 */
int tlklib_uacctrl_callHungUp(void);


#endif // TLKUSB_AUDIO_CTR_H
