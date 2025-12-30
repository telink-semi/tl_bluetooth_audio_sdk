/********************************************************************************************************
 * @file    tlkusb_uacMic.h
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
#ifndef TLKUSB_AUDIO_MIC_H
#define TLKUSB_AUDIO_MIC_H

#include "common/types.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#define TLKUSB_AUDMIC_VOL_MIN  ((int16_t)0x0000)                                /* Volume Minimum Value */
#define TLKUSB_AUDMIC_VOL_MAX  ((int16_t)0x1e00)                                /* Volume Maximum Value */
#define TLKUSB_AUDMIC_VOL_RES  0x0180                                          /* Volume Resolution */
#define TLKUSB_AUDMIC_VOL_DEF  0x1800                                          /* Volume default */
#define TLKUSB_AUDMIC_VOL_STEP (TLKUSB_AUDMIC_VOL_MAX / TLKUSB_AUDMIC_VOL_RES) /*Volume step*/

/**
 * @brief     Initialize the UAC microphone module
 * @return    TLK_ENONE
 */
int tlkusb_uacmic_init(void);

/**
 * @brief     Get the enable status of UAC microphone
 * @return    true if enabled, false otherwise
 */
bool tlkusb_uacmic_getEnable(void);

/**
 * @brief     Enable or disable the UAC microphone
 * @param[in] enable Enable(true) or disable(false) the microphone
 * @return    none
 */
void tlkusb_uacmic_setEnable(bool enable);

/**
 * @brief     Set the volume level for the microphone
 * @param[in] volume Volume level to set
 * @return    none
 */
void tlkusb_uacmic_setVolume(int16_t volume);

/**
 * @brief     Mute or unmute the microphone
 * @param[in] enable Mute(true) or unmute(false)
 * @return    none
 */
void tlkusb_uacmic_enterMute(bool enable);

/**
 * @brief     Handle set interface command for the microphone
 * @param[in] type Type of the feature to set
 * @return    TLK_ENONE
 */
int tlkusb_uacmic_setInfCmdDeal(int type);

/**
 * @brief     Handle get interface command for the microphone
 * @param[in] req  Request type
 * @param[in] type Type of the feature to get
 * @return    TLK_ENONE if successful, -TLK_ENOSUPPORT for unsupported requests
 */
int tlkusb_uacmic_getInfCmdDeal(int req, int type);

/**
 * @brief     Handle set endpoint command for the microphone
 * @param[in] type Type of the endpoint control
 * @return    TLK_ENONE
 */
int tlkusb_uacmic_setEdpCmdDeal(int type);

/**
 * @brief     Fill USB microphone data endpoint with audio data
 * @param[in] tick Current system tick
 * @return    none
 */
void tlkusb_uacmic_fillData(uint32_t tick);

/**
 * @brief     Process PPM for the microphone data
 * @param[in] tick Current system tick
 * @return    none
 */
void tlkusb_uacmic_ppm_process(uint32_t tick);

/**
 * @brief     Stop the PPM processing
 * @return    none
 */
void tlkusb_uacmic_ppm_stop(void);

#endif // TLKUSB_AUDIO_MIC_H
