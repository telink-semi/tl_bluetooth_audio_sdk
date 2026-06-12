/********************************************************************************************************
 * @file    tlkusb_uacSpk.h
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
#ifndef TLKUSB_AUDIO_SPK_H
#define TLKUSB_AUDIO_SPK_H

#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
#define TLKUSB_AUDSPK_VOL_MIN              ((int16_t)0xc180)     /* Volume Minimum Value -62db  	SPEAKER_VOL_MIN == SPEAKER_VOL_MAX - SPEAKER_VOL_RES * 100 */
#define TLKUSB_AUDSPK_VOL_MAX              ((int16_t)0x0000)     /* Volume Maximum Value 0db */
#define TLKUSB_AUDSPK_VOL_RES              0x00a0                /* Volume Resolution  100 steps, each step is 0x80*/
#define TLKUSB_AUDSPK_VOL_DEF              TLKUSB_AUDSPK_VOL_MAX /* Volume default */
#define TLKUSB_AUDSPK_VOL_STEP             400

#define APP_AUDIO_ADJUST_VOLUME_SHIFT_BITS 14
#define APP_AUDIO_DEFAULT_VOL_DB           0x7fff
#define APP_AUDIO_DEFAULT_VOL_STEP         0xff
#define APP_AUDIO_SPK_MUTE_MASK            0x80
#define APP_AUDIO_SPK_VOL_GRDL_STEP        2
#define APP_AUDIO_VALID_SAMPLE_TH          5

#define APP_VOLUME_RELATIVE_VALUE_NUM      101
#define APP_VOLUME_SCALE_NUM               101
#define APP_CC_VOLUME_SCALE_NUM            101
#define APP_AUDIO_SPEAKER_VOL_LINEAR_MIN   0
#define APP_AUDIO_SPEAKER_VOL_LINEAR_MAX   16384

#define APP_HALF(n)                        ((n) >> 1)
#else
#define TLKUSB_AUDSPK_VOL_MIN       ((int16_t)0xa000) /* Volume Minimum Value */
#define TLKUSB_AUDSPK_VOL_MAX       ((int16_t)0x0300) /* Volume Maximum Value */
#define TLKUSB_AUDSPK_VOL_RES       0x0100            /* Volume Resolution */
#define APP_AUDIO_SPK_VOL_GRDL_STEP 2
#endif

/**
 * @brief     Initialize the USB audio speaker module
 * @param[in] none
 * @return    TLK_ENONE
 */
int tlkusb_uacspk_init(void);

/**
 * @brief     Get the enable status of USB audio speaker
 * @param[in] none
 * @return    true if enabled, false otherwise
 */
bool tlkusb_uacspk_getEnable(void);
bool tlkusb_uacspk1_getEnable(void);

/**
 * @brief     Enable or disable the USB audio speaker
 * @param[in] enable Enable(true) or disable(false) the speaker
 * @return    none
 */
void tlkusb_uacspk_setEnable(bool enable);
void tlkusb_uacspk1_setEnable(bool enable);

/**
 * @brief     Get the volume of USB audio speaker
 * @param[in] none
 * @return    volume value
 */
#if (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24)
int16_t tlkusb_uacspk_getVolume(void);
int16_t tlkusb_uacspk1_getVolume(void);
#else
uint tlkusb_uacspk_getVolume(void);
uint tlkusb_uacspk1_getVolume(void);
#endif

/**
 * @brief     Set the volume of USB audio speaker
 * @param[in] volume Volume to set
 * @return    none
 */
void tlkusb_uacspk_setVolume(int16_t volume);
void tlkusb_uacspk1_setVolume(int16_t volume);

/**
 * @brief     Mute or unmute the USB audio speaker
 * @param[in] enable Mute(true) or unmute(false)
 * @return    none
 */
void tlkusb_uacspk_enterMute(bool enable);
void tlkusb_uacspk1_enterMute(bool enable);

/**
 * @brief     Handle set interface command for USB audio speaker
 * @param[in] type Type of the feature to set
 * @return    TLK_ENONE if successful, -TLK_ENOSUPPORT for unsupported requests
 */
int tlkusb_uacspk_setInfCmdDeal(int type);
int tlkusb_uacspk1_setInfCmdDeal(int type);

/**
 * @brief     Handle get interface command for USB audio speaker
 * @param[in] req  Request type
 * @param[in] type Type of the feature to get
 * @return    TLK_ENONE if successful, -TLK_ENOSUPPORT for unsupported requests
 */
int tlkusb_uacspk_getInfCmdDeal(int req, int type);
int tlkusb_uacspk1_getInfCmdDeal(int req, int type);

/**
 * @brief     Handle set endpoint command for USB audio speaker
 * @param[in] type Type of the endpoint control
 * @return    TLK_ENONE
 */
int tlkusb_uacspk_setEdpCmdDeal(int type);
int tlkusb_uacspk1_setEdpCmdDeal(int type);
/**
 * @brief     Receive data for USB audio speaker
 * @param[in] tick Current system tick
 * @return    none
 */
void tlkusb_uacspk_recvData(uint32_t tick);
void tlkusb_uacspk1_recvData(uint32_t tick);

/**
 * @brief     Close the USB audio speaker
 * @param[in] none
 * @return    none
 */
void tlkusb_uacspk_close(void);

/**
 * @brief     Copy USB audio data
 * @param[in] ticks Current system ticks
 * @return    none
 */
void tlkusb_uac_copy_data(uint32_t ticks);

/**
 * @brief     Stop the PPM processing for USB audio speaker
 * @param[in] none
 * @return    none
 */
void tlkusb_uacspk_ppm_stop(void);

/**
 * @brief     Process PPM for USB audio speaker
 * @param[in] ticks Current system ticks
 * @return    none
 */
void tlkusb_ppm_process(uint32_t ticks);

#endif // TLKUSB_AUDIO_SPK_H
