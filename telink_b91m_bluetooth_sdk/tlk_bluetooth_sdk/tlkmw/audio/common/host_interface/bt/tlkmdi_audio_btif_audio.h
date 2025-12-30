/********************************************************************************************************
 * @file    tlkmdi_audio_btif_audio.h
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

typedef enum
{
    AUD_BTIF_AVRCP_KEYID_SELECT              = 0x00,
    AUD_BTIF_AVRCP_KEYID_UP                  = 0x01,
    AUD_BTIF_AVRCP_KEYID_DOWN                = 0x02,
    AUD_BTIF_AVRCP_KEYID_LEFT                = 0x03,
    AUD_BTIF_AVRCP_KEYID_RIGHT               = 0x04,
    AUD_BTIF_AVRCP_KEYID_RIGHT_UP            = 0x05,
    AUD_BTIF_AVRCP_KEYID_RIGHT_DOWN          = 0x06,
    AUD_BTIF_AVRCP_KEYID_LEFT_UP             = 0x07,
    AUD_BTIF_AVRCP_KEYID_LEFT_DOWN           = 0x08,
    AUD_BTIF_AVRCP_KEYID_ROOT_MENU           = 0x09,
    AUD_BTIF_AVRCP_KEYID_SETUP_MENU          = 0x0A,
    AUD_BTIF_AVRCP_KEYID_CONTENTS_MENU       = 0x0B,
    AUD_BTIF_AVRCP_KEYID_FAVORITE_MENU       = 0x0C,
    AUD_BTIF_AVRCP_KEYID_EXIT                = 0x0D,
    AUD_BTIF_AVRCP_KEYID_RESERVED_1          = 0x0E,
    AUD_BTIF_AVRCP_KEYID_0                   = 0x20,
    AUD_BTIF_AVRCP_KEYID_1                   = 0x21,
    AUD_BTIF_AVRCP_KEYID_2                   = 0x22,
    AUD_BTIF_AVRCP_KEYID_3                   = 0x23,
    AUD_BTIF_AVRCP_KEYID_4                   = 0x24,
    AUD_BTIF_AVRCP_KEYID_5                   = 0x25,
    AUD_BTIF_AVRCP_KEYID_6                   = 0x26,
    AUD_BTIF_AVRCP_KEYID_7                   = 0x27,
    AUD_BTIF_AVRCP_KEYID_8                   = 0x28,
    AUD_BTIF_AVRCP_KEYID_9                   = 0x29,
    AUD_BTIF_AVRCP_KEYID_DOT                 = 0x2A,
    AUD_BTIF_AVRCP_KEYID_ENTER               = 0x2B,
    AUD_BTIF_AVRCP_KEYID_CLEAR               = 0x2C,
    AUD_BTIF_AVRCP_KEYID_RESERVED_2          = 0x2D,
    AUD_BTIF_AVRCP_KEYID_CHANNEL_UP          = 0x30,
    AUD_BTIF_AVRCP_KEYID_CHANNEL_DOWN        = 0x31,
    AUD_BTIF_AVRCP_KEYID_PREVIOUS_CHANNEL    = 0x32,
    AUD_BTIF_AVRCP_KEYID_SOUND_SELECT        = 0x33,
    AUD_BTIF_AVRCP_KEYID_INPUT_SELECT        = 0x34,
    AUD_BTIF_AVRCP_KEYID_DISPLAY_INFORMATION = 0x35,
    AUD_BTIF_AVRCP_KEYID_HELP                = 0x36,
    AUD_BTIF_AVRCP_KEYID_PAGE_UP             = 0x37,
    AUD_BTIF_AVRCP_KEYID_PAGE_DOWN           = 0x38,
    AUD_BTIF_AVRCP_KEYID_RESERVED_3          = 0x39,
    AUD_BTIF_AVRCP_KEYID_SKIP                = 0x3C,
    AUD_BTIF_AVRCP_KEYID_POWER               = 0x40,
    AUD_BTIF_AVRCP_KEYID_VOLUME_UP           = 0x41,
    AUD_BTIF_AVRCP_KEYID_VOLUME_DOWN         = 0x42,
    AUD_BTIF_AVRCP_KEYID_MUTE                = 0x43,
    AUD_BTIF_AVRCP_KEYID_PLAY                = 0x44,
    AUD_BTIF_AVRCP_KEYID_STOP                = 0x45,
    AUD_BTIF_AVRCP_KEYID_PAUSE               = 0x46,
    AUD_BTIF_AVRCP_KEYID_RECORD              = 0x47,
    AUD_BTIF_AVRCP_KEYID_REWIND              = 0x48,
    AUD_BTIF_AVRCP_KEYID_FAST_FORWARD        = 0x49,
    AUD_BTIF_AVRCP_KEYID_EJECT               = 0x4A,
    AUD_BTIF_AVRCP_KEYID_FORWARD             = 0x4B,
    AUD_BTIF_AVRCP_KEYID_BACKWARD            = 0x4C,
    AUD_BTIF_AVRCP_KEYID_RESERVED_4          = 0x4D,
    AUD_BTIF_AVRCP_KEYID_ANGLE               = 0x50,
    AUD_BTIF_AVRCP_KEYID_SUBPICTURE          = 0x51,
    AUD_BTIF_AVRCP_KEYID_RESERVED_5          = 0x52,
    AUD_BTIF_AVRCP_KEYID_F1                  = 0x71,
    AUD_BTIF_AVRCP_KEYID_F2                  = 0x72,
    AUD_BTIF_AVRCP_KEYID_F3                  = 0x73,
    AUD_BTIF_AVRCP_KEYID_F4                  = 0x74,
    AUD_BTIF_AVRCP_KEYID_F5                  = 0x75,
    AUD_BTIF_AVRCP_KEYID_RESERVED_6          = 0x76,
} AUD_BTIF_AVRCP_KEYID_ENUM;


typedef void (*TlkMdiAudBtifMusicStateChgCB)(uint16_t handle, uint8_t state);
typedef void (*TlkMdiAudBtifVoiceStateChgCB)(uint16_t aclHandle,uint16_t scoHandle, uint8_t isStart ,uint8_t codecType);

/**
 * @brief       Trigger one avrcp key press and release
 * @param[in]   handle - ACL handle
 * @param[in]   key - AVRCP key ID, refer to AUD_BTIF_AVRCP_KEYID_ENUM
 * @return      TLK_ENONE: success; Others: failure
 */
int tlkmdi_audio_btif_avrcp_sendKey(uint16_t handle, uint8_t key);

/**
 * @brief       Set increase peer power max command
 * @param[in]   handle - ACL connection handle
 * @param[in]   enable - Enable or disable the command
 * @return      Command execution status
 */
int tlkmdi_audio_btif_setIncPeerPwrMax(uint16_t handle, uint8_t enable);

/**
 * @brief       Add music state change callback function
 * @param[in]   cb - Callback function pointer
 * @param[in]   isPushBack - Whether to add at the end of callback array
 * @return      TLK_ENONE: success; TLK_EPARAM: invalid parameter; TLK_EQUOTA: no space for new callback
 */
int tlkmdi_audio_btif_addMusicStateChgCB(TlkMdiAudBtifMusicStateChgCB cb,uint8_t isPushBack);

/**
 * @brief       Add voice state change callback function
 * @param[in]   cb - Callback function pointer
 * @return      TLK_ENONE: success; TLK_EPARAM: invalid parameter; TLK_EQUOTA: no space for new callback
 */
int tlkmdi_audio_btif_addVoiceStateChgCB(TlkMdiAudBtifVoiceStateChgCB cb);

/**
 * @brief       Set trigger mode for music state change callbacks
 * @param[in]   mode - Trigger mode (0: forward, non-zero: backward)
 * @return      None
 */
void tlkmdi_audio_btif_setMusicStateChgCBTriggerMode(uint8_t mode);

/**
 * @brief       Inform host that audio is enabled
 * @param[in]   handle - ACL connection handle
 * @param[in]   is_music - Music or voice flag
 * @return      TLK_ENONE: success; TLK_EREPEAT: repeat operation
 */
int tlkmw_audio_btif_inform_host_audio_en(uint16_t handle, uint8_t is_music);

/**
 * @brief       Inform host that audio is disabled
 * @param[in]   handle - ACL connection handle
 * @return      TLK_ENONE: success; TLK_ENOOBJECT: object not found
 */
int tlkmw_audio_btif_inform_host_audio_dis(uint16_t handle);

/**
 * @brief       Get AVDTP SEID type for music
 * @param[in]   None
 * @return      Codec type
 */
uint8_t tlkmw_audio_btif_music_get_avdtp_seid_type(void);

/**
 * @brief       Get current playing handle
 * @param[in]   None
 * @return      Current handle
 */
uint16_t tlkmw_audio_btif_getPlayHandle(void);

/**
 * @brief       Check if handle is AG
 * @param[in]   handle - ACL connection handle
 * @return      True if handle is AG, otherwise false
 */
bool tlkmw_audio_btif_handle_is_ag(uint16_t handle);

/**
 * @brief       Get is allowed slave headset create sco when hfp is not connected
 * @param[in]   handle - ACL handle
 * @return      True means allowed
 */
bool tlkmdi_audio_btif_allowedCreateScoWithoutHfp(uint16_t handle);
