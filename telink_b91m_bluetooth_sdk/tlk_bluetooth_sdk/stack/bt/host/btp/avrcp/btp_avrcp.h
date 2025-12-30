/********************************************************************************************************
 * @file    btp_avrcp.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef BTP_AVRCP_H
#define BTP_AVRCP_H

typedef enum
{
    // Capabilities
    BTP_AVRCP_PDUID_GET_CAP = 0x10, // AV/C STATUS
    // Player Application Settings
    BTP_AVRCP_PDUID_LIST_PAS_ATTRS         = 0x11, // ListPlayerApplicationSettingAttributes, AV/C STATUS
    BTP_AVRCP_PDUID_LIST_PAS_VALUES        = 0x12, // ListPlayerApplicationSettingValues, AV/C STATUS
    BTP_AVRCP_PDUID_GET_CUR_PAS_VALUR      = 0x13, // GetCurrentPlayerApplicationSettingValue, AV/C STATUS
    BTP_AVRCP_PDUID_SET_PAS_VALUE          = 0x14, // AV/C CONTROL, AV/C CONTROL
    BTP_AVRCP_PDUID_GET_PAS_ATTR_TEXT      = 0x15, // GetPlayerApplicationSettingAttributeText , AV/C STATUS
    BTP_AVRCP_PDUID_GET_PAS_VALUE_TEXT     = 0x16, // GetPlayerApplicationSettingValueText, AV/C STATUS
    BTP_AVRCP_PDUID_INFORM_DISPLAY_CHARSET = 0x17, // GetPlayerApplicationSettingValueText, AV/C CONTROL
    BTP_AVRCP_PDUID_INFORM_BATTERY_STATUS  = 0x18, // InfomBatteryStatusOfCT, AV/C CONTROL
    // Metadata Attributes for Current Media Item
    BTP_AVRCP_PDUID_GET_ELEMENT_ATTR = 0x20, // GetElementAttributes, AV/C STATUS
    // Notifications
    BTP_AVRCP_PDUID_GET_PLAY_STATUS  = 0x30, // AV/C STATUS
    BTP_AVRCP_PDUID_REG_NOTIFICATION = 0x31, // AV/C NOTIFY
    // Continuation
    BTP_AVRCP_PDUID_REQUEST_CONTINUE_RSP = 0x40, // RequestContinuingResponse, AV/C CONTROL
    BTP_AVRCP_PDUID_ABORT_CONTINUE_RSP   = 0x41, // AbortContinuingResponse, AV/C CONTROL
    // Absolute Volume
    BTP_AVRCP_PDUID_SET_ABSOLUTE_VOLUME = 0x50, // AV/C CONTROL
    // MediaPlayerSelection
    BTP_AVRCP_PDUID_SET_ADDRESSED_PLAYER = 0x60, // SetAddressedPlayer, AV/C CONTROL
    // Browsing
    BTP_AVRCP_PDUID_SET_BROWSED_PLAYER = 0x70, // SetBrowsedPlayer, Browsing
    BTP_AVRCP_PDUID_CHANGE_PATH        = 0x72, // ChangePath, Browsing
    BTP_AVRCP_PDUID_GET_ITEM_ATTR      = 0x73, // GetItemAttributes, Browsing
    // Search,
    BTP_AVRCP_PDUID_SEARCH           = 0x80, // Browsing
    BTP_AVRCP_PDUID_GET_FOLDER_ITEMS = 0x71, // GetFolderItems(MediaPlayerList,Filesystem,SearchResultList,NowPlayingList), Browsing
    BTP_AVRCP_PDUID_GET_TOTAL_ITEMS  = 0x75, // GetTotalNumberOfItems, Browsing
    BTP_AVRCP_PDUID_PLAY_ITEM        = 0x74, // PlayItem(Filesystem,SearchResultList,NowPlayingList), AV/C CONTROL
    BTP_AVRCP_PDUID_ADD_PLAYING      = 0x90, // AddToNowPlaying, AV/C CONTROL
    // Error Response
    BTP_AVRCP_PDUID_GENERAL_REJECT = 0xA0, // Browsing
} BTP_AVRCP_PDUID_ENUM;

/* Define PASSTHROUGH OP_ID */
typedef enum
{
    BTP_AVRCP_KEYID_SELECT              = 0x00,
    BTP_AVRCP_KEYID_UP                  = 0x01,
    BTP_AVRCP_KEYID_DOWN                = 0x02,
    BTP_AVRCP_KEYID_LEFT                = 0x03,
    BTP_AVRCP_KEYID_RIGHT               = 0x04,
    BTP_AVRCP_KEYID_RIGHT_UP            = 0x05,
    BTP_AVRCP_KEYID_RIGHT_DOWN          = 0x06,
    BTP_AVRCP_KEYID_LEFT_UP             = 0x07,
    BTP_AVRCP_KEYID_LEFT_DOWN           = 0x08,
    BTP_AVRCP_KEYID_ROOT_MENU           = 0x09,
    BTP_AVRCP_KEYID_SETUP_MENU          = 0x0A,
    BTP_AVRCP_KEYID_CONTENTS_MENU       = 0x0B,
    BTP_AVRCP_KEYID_FAVORITE_MENU       = 0x0C,
    BTP_AVRCP_KEYID_EXIT                = 0x0D,
    BTP_AVRCP_KEYID_RESERVED_1          = 0x0E,
    BTP_AVRCP_KEYID_0                   = 0x20,
    BTP_AVRCP_KEYID_1                   = 0x21,
    BTP_AVRCP_KEYID_2                   = 0x22,
    BTP_AVRCP_KEYID_3                   = 0x23,
    BTP_AVRCP_KEYID_4                   = 0x24,
    BTP_AVRCP_KEYID_5                   = 0x25,
    BTP_AVRCP_KEYID_6                   = 0x26,
    BTP_AVRCP_KEYID_7                   = 0x27,
    BTP_AVRCP_KEYID_8                   = 0x28,
    BTP_AVRCP_KEYID_9                   = 0x29,
    BTP_AVRCP_KEYID_DOT                 = 0x2A,
    BTP_AVRCP_KEYID_ENTER               = 0x2B,
    BTP_AVRCP_KEYID_CLEAR               = 0x2C,
    BTP_AVRCP_KEYID_RESERVED_2          = 0x2D,
    BTP_AVRCP_KEYID_CHANNEL_UP          = 0x30,
    BTP_AVRCP_KEYID_CHANNEL_DOWN        = 0x31,
    BTP_AVRCP_KEYID_PREVIOUS_CHANNEL    = 0x32,
    BTP_AVRCP_KEYID_SOUND_SELECT        = 0x33,
    BTP_AVRCP_KEYID_INPUT_SELECT        = 0x34,
    BTP_AVRCP_KEYID_DISPLAY_INFORMATION = 0x35,
    BTP_AVRCP_KEYID_HELP                = 0x36,
    BTP_AVRCP_KEYID_PAGE_UP             = 0x37,
    BTP_AVRCP_KEYID_PAGE_DOWN           = 0x38,
    BTP_AVRCP_KEYID_RESERVED_3          = 0x39,
    BTP_AVRCP_KEYID_SKIP                = 0x3C,
    BTP_AVRCP_KEYID_POWER               = 0x40,
    BTP_AVRCP_KEYID_VOLUME_UP           = 0x41,
    BTP_AVRCP_KEYID_VOLUME_DOWN         = 0x42,
    BTP_AVRCP_KEYID_MUTE                = 0x43,
    BTP_AVRCP_KEYID_PLAY                = 0x44,
    BTP_AVRCP_KEYID_STOP                = 0x45,
    BTP_AVRCP_KEYID_PAUSE               = 0x46,
    BTP_AVRCP_KEYID_RECORD              = 0x47,
    BTP_AVRCP_KEYID_REWIND              = 0x48,
    BTP_AVRCP_KEYID_FAST_FORWARD        = 0x49,
    BTP_AVRCP_KEYID_EJECT               = 0x4A,
    BTP_AVRCP_KEYID_FORWARD             = 0x4B,
    BTP_AVRCP_KEYID_BACKWARD            = 0x4C,
    BTP_AVRCP_KEYID_RESERVED_4          = 0x4D,
    BTP_AVRCP_KEYID_ANGLE               = 0x50,
    BTP_AVRCP_KEYID_SUBPICTURE          = 0x51,
    BTP_AVRCP_KEYID_RESERVED_5          = 0x52,
    BTP_AVRCP_KEYID_F1                  = 0x71,
    BTP_AVRCP_KEYID_F2                  = 0x72,
    BTP_AVRCP_KEYID_F3                  = 0x73,
    BTP_AVRCP_KEYID_F4                  = 0x74,
    BTP_AVRCP_KEYID_F5                  = 0x75,
    BTP_AVRCP_KEYID_RESERVED_6          = 0x76,
} BTP_AVRCP_KEYID_ENUM;

typedef enum
{
    BTP_AVRCP_PLAY_STATE_STOPPED = 0,
    BTP_AVRCP_PLAY_STATE_PLAYING,
    BTP_AVRCP_PLAY_STATE_PAUSED,
    BTP_AVRCP_PLAY_STATE_FWD_SEEK,
    BTP_AVRCP_PLAY_STATE_REV_SEEK,
} BTP_AVRCP_PLAY_STATE_ENUM;

typedef enum
{
    BTP_AVRCP_STATUS_CODE_INVALID_CMD             = 0x00,     // Invalid command, sent if TG received a PDU that it did not understand.
    BTP_AVRCP_STATUS_CODE_INVALID_PARAMETER       = 0x01,     // Invalid parameter, sent if the TG received a PDU with a parameter ID that it did not understand.
    BTP_AVRCP_STATUS_CODE_PARAMETER_CONTENT_ERROR = 0x02,     // Parameter content error. Sent if the parameter ID is understood, but content is wrong or corrupted.
    BTP_AVRCP_STATUS_CODE_INTERNAL_ERROR          = 0x03,     // Internal Error - sent if there are error conditions not covered by a more specific error code.
    BTP_AVRCP_STATUS_CODE_OPERATE_WITHOUT_ERROR   = 0x04,     // Operation completed without error. This is the status that
                                                              // should be returned if the operation was successful.
    BTP_AVRCP_STATUS_CODE_UID_CHANGE                  = 0x05, // UID Changed. The UIDs on the device have changed
    BTP_AVRCP_STATUS_CODE_RESERVED                    = 0x06,
    BTP_AVRCP_STATUS_CODE_INVALID_DIRECTION           = 0x07, // Invalid Direction. The Direction parameter is invalid
    BTP_AVRCP_STATUS_CODE_NOT_A_DIRECTORY             = 0x08, // Not a Directory. The UID provided does not refer to a folder item
    BTP_AVRCP_STATUS_CODE_DOES_NOT_EXIST              = 0x09, // Does Not Exist. The UID provided does not refer to any currently valid item
    BTP_AVRCP_STATUS_CODE_INVALID_SCOPE               = 0x0A, // Invalid Scope. The scope parameter is invalid
    BTP_AVRCP_STATUS_CODE_RANGE_OUT_OF_BOUNDS         = 0x0B, // Range Out of Bounds. The start of range provided is not valid
    BTP_AVRCP_STATUS_CODE_FOLDER_ITEM_IS_NOT_PLAYABLE = 0x0C, // Folder Item is not playable. The UID provided refers to a folder item which cannot be handled by this
                                                              // media player
    BTP_AVRCP_STATUS_CODE_MEDIA_IN_USE          = 0x0D,       // Media in Use. The media is not able to be used for this operation at this time
    BTP_AVRCP_STATUS_CODE_NOW_PLAYING_LIST_FULL = 0x0E,       // Now Playing List Full. No more items can be added to the Now Playing List
    BTP_AVRCP_STATUS_CODE_SEARCH_NOT_SUPPORTED  = 0x0F,       // Search Not Supported. The Browsed Media Player does not support search
    BTP_AVRCP_STATUS_CODE_SEARCH_IN_PROGRESS    = 0x10,       // Search in Progress. A search operation is already in progress
    BTP_AVRCP_STATUS_CODE_INVALID_PLAYER_ID     = 0x11,       // Invalid Player Id. The specified Player Id does not refer to a valid player
    BTP_AVRCP_STATUS_CODE_PLAY_NOT_BROWSABLE    = 0x12,       // Player Not Browsable. The Player Id supplied refers to a Media Player which does not support browsing.
    BTP_AVRCP_STATUS_CODE_PLAY_NOT_ADDRESSED    = 0x13,       // Player Not Addressed. The Player Id supplied refers to a player which is not currently addressed, and
                                                              // the command is not able to be performed if the player is not set as addressed.
    BTP_AVRCP_STATUS_CODE_NO_VALID_SEARCHRESULTS = 0x14,      // No valid Search Results. The Search result list does not contain valid entries, e.g. after being
                                                              // invalidated due to change of browsed player
    BTP_AVRCP_STATUS_CODE_NO_AVAILABLE_PLAYERS     = 0x15,    // No available players
    BTP_AVRCP_STATUS_CODE_ADDRESSED_PLAYER_CHANGED = 0x16,    // Addressed Player Changed
} BTP_AVRCP_STATUS_CODE_ENUM;

typedef enum
{
    BTP_AVRCP_ATTRID_NOT_USED = 0x00,
    BTP_AVRCP_ATTRID_TITLE    = 0x01,               // Text field representing the title, song name or content  description coded per
                                                    // specified character set.
    BTP_AVRCP_ATTRID_ARTIST_NAME            = 0x02, // Text field representing artist(s), performer(s) or group coded per specified character set.
    BTP_AVRCP_ATTRID_ALBUM_NAME             = 0x03, // Text field representing the title of the recording (source) from which the audio in the file is taken.
    BTP_AVRCP_ATTRID_TRACK_NUMBER           = 0x04, // Numeric ASCII string containing the order number of the audio-file on its original recording.
    BTP_AVRCP_ATTRID_TOTAL_NUMBER_OF_TRACKS = 0x05, // Numeric ASCII string containing the total number of tracks or elements on the original recording.
    BTP_AVRCP_ATTRID_GENRE                  = 0x06, // Text field representing the category of the composition characterized by a particular style.
    BTP_AVRCP_ATTRID_PLAYING_TIME           = 0x07, // Numeric ASCII string containing the length of the audio file in milliseconds. (E.g. 02:30 = 150000)
    BTP_AVRCP_ATTRID_DEFAULT_COVER_ART      = 0x08, // BIP Image Handle
} BTP_AVRCP_ATTRID_ENUM;

typedef enum
{
    BTP_AVRCP_PLAYER_APP_SET_ATTR_ILLEGAL                 = 0x00, //
    BTP_AVRCP_PLAYER_APP_SET_ATTR_EQUALIZER_ON_OFF_STATUS = 0x01,
    BTP_AVRCP_PLAYER_APP_SET_ATTR_REPEAT_MODE_STATUS      = 0x02,
    BTP_AVRCP_PLAYER_APP_SET_ATTR_SHUFFLE_ON_OFF_STATUS   = 0x03,
    BTP_AVRCP_PLAYER_APP_SET_ATTR_SCAN_ON_OFF_STATUS      = 0x04,
} BTP_AVRCP_PLAYER_APP_SET_ATTR_ENUM;

typedef enum
{
    BTP_AVRCP_EVTID_PLAYBACK_STATUS_CHANGED     = 0x01,
    BTP_AVRCP_EVTID_TRACK_CHANGED               = 0x02,
    BTP_AVRCP_EVTID_TRACK_REACHED_END           = 0x03,
    BTP_AVRCP_EVTID_TRACK_REACHED_START         = 0x04,
    BTP_AVRCP_EVTID_PLAYBACK_POS_CHANGED        = 0x05,
    BTP_AVRCP_EVTID_BATT_STATUS_CHANGED         = 0x06,
    BTP_AVRCP_EVTID_SYSTEM_STATUS_CHANGED       = 0x07,
    BTP_AVRCP_EVTID_PLAYER_APP_SETTING_CHANGED  = 0x08,
    BTP_AVRCP_EVTID_NOW_PLAYING_CONTENT_CHANGED = 0x09,
    BTP_AVRCP_EVTID_AVAILABLE_PLAYERS_CHANGED   = 0x0A,
    BTP_AVRCP_EVTID_ADDRESSED_PLAYER_CHANGED    = 0x0B,
    BTP_AVRCP_EVTID_UIDS_CHANGED                = 0x0C,
    BTP_AVRCP_EVTID_VOLUME_CHANGED              = 0x0D,
} BTP_AVRCP_EVTID_ENUM;

typedef enum
{
    BTP_AVRCP_EVTMSK_PLAYBACK_STATUS_CHANGED     = 1 << (BTP_AVRCP_EVTID_PLAYBACK_STATUS_CHANGED - 1),
    BTP_AVRCP_EVTMSK_TRACK_CHANGED               = 1 << (BTP_AVRCP_EVTID_TRACK_CHANGED - 1),
    BTP_AVRCP_EVTMSK_TRACK_REACHED_END           = 1 << (BTP_AVRCP_EVTID_TRACK_REACHED_END - 1),
    BTP_AVRCP_EVTMSK_TRACK_REACHED_START         = 1 << (BTP_AVRCP_EVTID_TRACK_REACHED_START - 1),
    BTP_AVRCP_EVTMSK_PLAYBACK_POS_CHANGED        = 1 << (BTP_AVRCP_EVTID_PLAYBACK_POS_CHANGED - 1),
    BTP_AVRCP_EVTMSK_BATT_STATUS_CHANGED         = 1 << (BTP_AVRCP_EVTID_BATT_STATUS_CHANGED - 1),
    BTP_AVRCP_EVTMSK_SYSTEM_STATUS_CHANGED       = 1 << (BTP_AVRCP_EVTID_SYSTEM_STATUS_CHANGED - 1),
    BTP_AVRCP_EVTMSK_PLAYER_APP_SETTING_CHANGED  = 1 << (BTP_AVRCP_EVTID_PLAYER_APP_SETTING_CHANGED - 1),
    BTP_AVRCP_EVTMSK_NOW_PLAYING_CONTENT_CHANGED = 1 << (BTP_AVRCP_EVTID_NOW_PLAYING_CONTENT_CHANGED - 1),
    BTP_AVRCP_EVTMSK_ADDRESSED_PLAYER_CHANGED    = 1 << (BTP_AVRCP_EVTID_ADDRESSED_PLAYER_CHANGED - 1),
    BTP_AVRCP_EVTMSK_AVAILABLE_PLAYERS_CHANGED   = 1 << (BTP_AVRCP_EVTID_AVAILABLE_PLAYERS_CHANGED - 1),
    BTP_AVRCP_EVTMSK_UIDS_CHANGED                = 1 << (BTP_AVRCP_EVTID_UIDS_CHANGED - 1),
    BTP_AVRCP_EVTMSK_VOLUME_CHANGED              = 1 << (BTP_AVRCP_EVTID_VOLUME_CHANGED - 1),
    BTP_AVRCP_EVTMSK_DEFAULT                     = BTP_AVRCP_EVTMSK_PLAYBACK_STATUS_CHANGED | BTP_AVRCP_EVTMSK_VOLUME_CHANGED | BTP_AVRCP_EVTMSK_BATT_STATUS_CHANGED,
    BTP_AVRCP_EVTMSK_ALL                         = 0x1FFF,
} BTP_AVRCP_EVTMSK_ENUM;

typedef enum
{
    BTP_AVRCP_BATTERY_STATUS_NORMAL      = 0x00, // Battery operation is in normal state
    BTP_AVRCP_BATTERY_STATUS_WARNING     = 0x01, // unable to operate soon. Specified when battery going down.
    BTP_AVRCP_BATTERY_STATUS_CRITICAL    = 0x02, // cannot operate any more. Specified when battery going down.
    BTP_AVRCP_BATTERY_STATUS_EXTERNAL    = 0x03, // Connecting to external power supply
    BTP_AVRCP_BATTERY_STATUS_FULL_CHARGE = 0x04, // when the device is completely charged.
} BTP_AVRCP_BATTERY_STATUS_ENUM;

/******************************************************************************
 * typedef: BtpAvrcpKeyChangeCallback
 * Descript: The type of callback function that receives keystroke completion.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @keyID--Refer BTP_AVRCP_KEYID_ENUM.
 *     @isPress--True, this key is pressed; Flas, this key is released.
 * Return: None
 *******************************************************************************/
typedef void (*BtpAvrcpKeyChangeCallback)(uint16_t aclHandle, uint8_t keyID, uint8_t isPress);
/******************************************************************************
 * typedef: BtpAvrcpVolumeChangeCallback
 * Descript: Type of callback function for volume changed.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @volume--0x00~0x7F.
 * Return: None
 *******************************************************************************/
// typedef void (*BtpAvrcpVolumeChangeCallback)(uint16_t aclHandle, uint8_t volume);
/******************************************************************************
 * typedef: BtpAvrcpInsCompleteCB
 * Descript: The type of the callback function that the instruction completed.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @pduID--The ID of Protocol Data Unit. Refer BTP_AVRCP_PDUID_ENUM.
 *     @status--Refer BTP_AVRCP_STATUS_CODE_ENUM.
 *         BTP_AVRCP_STATUS_CODE_OPERATE_WITHOUT_ERROR is success.
 *     @pParam--The callback param to user.
 *     @paramLen--The length of param.
 * Return: None
 *******************************************************************************/
typedef void (*BtpAvrcpInsCompleteCB)(uint16_t aclHandle, uint8_t pduID, uint8_t status, void *pParam, uint16_t paramLen);

typedef struct
{
    uint32_t songLength;   // The total length of the playing song in milliseconds.
    uint32_t songPosition; // The current position of the playing in milliseconds elapsed.
    uint8_t  playStatus;   // Current Status of playing. Refer BTP_AVRCP_PLAY_STATE_ENUM.
} btp_avrcp_getPlayStatusRsp_t;

typedef struct
{
    uint8_t attNumb;
    uint8_t attID[16]; // Refer BTP_AVRCP_PLAYER_ATTID_ENUM
} btp_avrcp_listPasAttrRsp_t;

typedef struct
{
    uint8_t valNumb;
    uint8_t value[16];
} btp_avrcp_listPasValueRsp_t;

typedef struct
{
    uint8_t number;
    uint8_t attID[16]; // Refer BTP_AVRCP_PLAYER_ATTID_ENUM
    uint8_t value[16];
} btp_avrcp_getCurlistPasValueRsp_t;

typedef struct
{
    uint8_t  number;
    uint8_t  attID[16];   // Refer BTP_AVRCP_PLAYER_ATTID_ENUM
    uint16_t charSet[16]; // Refer BTP_BROWSING_CHAR_SET_ENUM
    uint8_t  textLen[16];
    uint8_t *pTxtDat[16];
} btp_avrcp_getPasAttrTextRsp_t;

typedef struct
{
    uint8_t  number;
    uint8_t  attID[16];   // Refer BTP_AVRCP_PLAYER_ATTID_ENUM
    uint16_t charSet[16]; // Refer BTP_BROWSING_CHAR_SET_ENUM
    uint8_t  textLen[16];
    uint8_t *pTxtDat[16];
} btp_avrcp_getPasValueTextRsp_t;

typedef struct
{
    uint8_t  number;
    uint32_t attID[16];   // Refer BTP_AVRCP_PLAYER_ATTID_ENUM
    uint16_t charSet[16]; // Refer BTP_BROWSING_CHAR_SET_ENUM
    uint16_t length[16];
    uint8_t *pValue[16];
} btp_avrcp_getElementAttrRsp_t;

/******************************************************************************
 * Function: AVRCP initial interface
 * Descript: Defines trigger the initial flow of AVRCP sink and AVRCP source
 * Params:
 * 		@count[IN]--the AVRCP number, refer to TLK_BT_AVRCP_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value(-TLK_ENOMEM) is returned means the initial process fail.
 *******************************************************************************/
int btp_avrcp_init(uint8_t count);

/******************************************************************************
 * Function: AVRCP deinitial interface
 * Descript: Defines trigger the initial flow of AVRCP sink and AVRCP source
 * Params:
 * 		@count[IN]--the AVRCP number, refer to TLK_BT_AVRCP_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int btp_avrcp_deinit(void);

/******************************************************************************
 * Function: btp_avrcp_getMemLen
 * Descript: AVRCP get the size of the required number of nodes interface.
 * Params:
 * 		@count[IN]--the AVRCP number, refer to TLK_BT_AVRCP_MAX_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int btp_avrcp_getMemLen(uint8_t count);

/******************************************************************************
 * Function: btp_avrcp_connect
 * Descript: Defines trigger the avrcp connect cmd to peer avrcp entity and setup
 *           a connection with devices.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @usrId--The service user id. The field is not used.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_connect(uint16_t aclHandle, uint8_t usrID);

/******************************************************************************
 * Function: btp_avrcp_disconn
 * Descript: Defines trigger the avrcp disconnect cmd to peer avrcp entity and
 *           tear up a connection which specify by aclhandle and usrid.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @usrId--The service user id.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_disconn(uint16_t aclHandle, uint8_t usrID);

/******************************************************************************
 * Function: btp_avrcp_destroy
 * Descript: Defines trigger to release the resource which allocated.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 * Return: None.
 *******************************************************************************/
void btp_avrcp_destroy(uint16_t aclHandle);


/******************************************************************************
 * Function: btp_avrcp_remoteIsPlaying
 * Descript: Defines trigger the avrcp cmd check the music status of peer avrcp entity.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 * Return: true(success)/false(fail)
 *******************************************************************************/
bool btp_avrcp_remoteIsPlaying(uint16_t aclHandle);

/******************************************************************************
 * Function: btp_avrcp_isSupportSetVolume
 * Descript: Defines trigger query the peer avrcp whether support set volume or not.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 * Return: true(success)/false(fail)
 *******************************************************************************/
bool btp_avrcp_isSupportSetVolume(uint16_t aclHandle);

/******************************************************************************
 * Function: btp_avrcp_setVolume
 * Descript: Set volume, calling this interface will trigger set volume (CT),
 *           or send Notify(TG).
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @volume--The volume value.
 *     @isSrc--True-this is audio source, false-This is audio sink.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_setVolume(uint16_t aclHandle, uint8_t volume, bool isSrc);

/******************************************************************************
 * Function: btp_avrcp_setDefaultVolume
 * Descript: Set the system default volume.
 * Params:
 *     @volume--The volume value. 0x00~0x7F
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_setDefaultVolume(uint8_t *pAddr, uint8_t volume);

/******************************************************************************
 * Function: btp_avrcp_notifyBatteryStatus
 * Descript: Notify the status of battery to CT.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @batStatus--The status of battery. Refer BTP_AVRCP_BATTERY_STATUS_ENUM.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_notifyBatteryStatus(uint16_t aclHandle, uint8_t batStatus);

/******************************************************************************
 * Function: btp_avrcp_setDefaultBatteryStatus
 * Descript: Set the default battery status.
 * Params:
 *     @batStatus--The status of battery. Refer BTP_AVRCP_BATTERY_STATUS_ENUM.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_setDefaultBatteryStatus(uint8_t batStatus);

/******************************************************************************
 * Function: btp_avrcp_setTrackValue
 * Descript: Set the value of play track ID.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @pTrackID--Play Track ID, which bond with music info. Fixed 8 Bytes.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: TrackID need to be bond with the music information if browsing is
 *     supported. If browsing is not supported, trackID is all zeor.
 *******************************************************************************/
int btp_avrcp_setTrackValue(uint16_t aclHandle, uint8_t *pTrackID);

/******************************************************************************
 * Function: btp_avrcp_notifyStatusChange
 * Descript: Notify the status is changed to CT.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @eventID--refer BTP_AVRCP_EVTID_ENUM
 *     @pData--The data to be sent.
 *     @dataLen--The length of the data.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_notifyStatusChange(uint16_t aclHandle, uint8_t eventID, uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: btp_avrcp_sendRegEventNotify
 * Descript: Send a registration event notification message to the TG.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @eventID--refer BTP_AVRCP_EVTID_ENUM
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note:
 *     1. Registration fails for events that are not supported by the other party.
 *     2. You can use btp_avrcp_eventIsSupport to check whether the peer supports it.
 *******************************************************************************/
int btp_avrcp_sendRegEventNotify(uint16_t aclHandle, uint8_t eventID);

/******************************************************************************
 * Function: btp_avrcp_eventIsSupport
 * Descript: Check the event is supported by peer.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @eventID--refer BTP_AVRCP_EVTID_ENUM
 * Return: True means be supported by peer.
 *******************************************************************************/
bool btp_avrcp_eventIsSupport(uint16_t aclHandle, uint8_t eventID);

/******************************************************************************
 * Function: btp_avrcp_setLocalEventMask
 * Descript: Set the event mask of locally supported.
 * Params:
 *     @evtMask--Refer to BTP_AVRCP_EVTMSK_ENUM.
 * Return: None.
 *******************************************************************************/
void btp_avrcp_setLocalEventMask(uint32_t evtMask);

/******************************************************************************
 * Function: btp_avrcp_setLocalEventMask
 * Descript: Obtain a locally supported Event Mask.
 * Params: None.
 * Return: Refer BTP_AVRCP_EVTMSK_ENUM.
 *******************************************************************************/
uint btp_avrcp_getLocalEventMask(void);

/******************************************************************************
 * Function: btp_avrcp_getPeerEventMask
 * Descript: Obtain a remoter supported Event Mask.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 * Return: Refer BTP_AVRCP_EVTMSK_ENUM.
 *******************************************************************************/
uint btp_avrcp_getPeerEventMask(uint16_t aclHandle);

/******************************************************************************
 * Function: AVRCP Set music state interface
 * Descript: Defines trigger the avrcp cmd change the music of peer avrcp entity.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @playsate--The state of music to be set. Refer BTP_AVRCP_PLAY_STATE_ENUM.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_setPlayState(uint16_t aclHandle, uint8_t playState);

/******************************************************************************
 * Function: AVRCP Notify music sate Command interface
 * Descript: Defines trigger the avrcp of CT notify the music state
 *           changing to peer avrcp entity.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @playState--Refer BTP_AVRCP_PLAY_STATE_ENUM.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_notifyPlayState(uint16_t aclHandle, uint8_t playState);

/******************************************************************************
 * Function: AVRCP Send Key interface
 * Descript: Defines trigger the avrcp key press cmd to peer avrcp entity.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @keyID--The key id which reflect to a special command.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_sendKeyPress(uint16_t aclHandle, uint8_t keyID);

/******************************************************************************
 * Function: AVRCP Send Key Release interface
 * Descript: Defines trigger the avrcp key release cmd to peer avrcp entity.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @keyID--The key id which reflect to a special command.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_avrcp_sendKeyRelease(uint16_t aclHandle, uint8_t keyID);

/******************************************************************************
 * Function: btp_avrcp_regKeyChangeCB
 * Descript: Defines register a callback to listen the music player state changing
 *           which operate by avrcp entity.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @cb--The play changed callback.
 * Return: None.
 *******************************************************************************/
void btp_avrcp_regKeyChangeCB(BtpAvrcpKeyChangeCallback cb);

/******************************************************************************
 * Function: btp_avrcp_sendGetPlayStatusCmd
 * Descript: This function is used by the CT to get the status of the currently
 *           playing media at the TG.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: PAS - PlayerApplicationSetting. Refer <AVRCP_v1.6.2> P51 and P134.
 *******************************************************************************/
int btp_avrcp_sendGetPlayStatusCmd(uint16_t aclHandle);

/******************************************************************************
 * Function: btp_avrcp_sendListPasAttrCmd
 * Descript: This function request the target device to provide target supported
 *           player application setting attributes.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: PAS - PlayerApplicationSetting. Refer <AVRCP_v1.6.2> P48 and P134.
 *******************************************************************************/
int btp_avrcp_sendListPasAttrCmd(uint16_t aclHandle);

/******************************************************************************
 * Function: btp_avrcp_sendListPasValuesCmd
 * Descript: This function requests the target device to list the set of possible
 *           values for the requested player application setting attribute.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @attID--Player application setting attribute ID.
 *             Refer BTP_AVRCP_PLAYER_APP_SET_ATTR_ENUM.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: PAS - PlayerApplicationSetting. Refer <AVRCP_v1.6.2> P49 and P134.
 *******************************************************************************/
int btp_avrcp_sendListPasValuesCmd(uint16_t aclHandle, uint8_t attID);

/******************************************************************************
 * Function: btp_avrcp_sendGetPasAttrTextCmd
 * Descript: This function requests the target device to provide the current set
 *           values on the target for the provided player application setting
 *           attributes list.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @pAttID--Player application setting attribute ID for which the corresponding
 *         current set value is requested. Refer BTP_AVRCP_PLAYER_APP_SET_ATTR_ENUM.
 *     @attCnt--Number of player application setting attribute for which current
 *         set values are requested.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: PAS - PlayerApplicationSetting. Refer <AVRCP_v1.6.2> P49 and P134.
 *******************************************************************************/
int btp_avrcp_sendGetCurPasValueCmd(uint16_t aclHandle, uint8_t *pAttID, uint8_t attCnt);

/******************************************************************************
 * Function: btp_avrcp_sendGetPasAttrTextCmd
 * Descript: This function requests to set the player application setting list
 *           of player application setting values on the target device for the
 *           corresponding defined list of PlayerApplicationSettingAttributes.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @pAttID--Player application setting attribute ID for which the value
 *         needs to be set. Refer BTP_AVRCP_PLAYER_APP_SET_ATTR_ENUM.
 *     @pAttVal--Player application setting value ID for the corresponding player
 *         application setting attribute ID.
 *     @attCnt--Number of player application setting attributes for which the
 *         player application setting.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: PAS - PlayerApplicationSetting. Refer <AVRCP_v1.6.2> P50 and P134.
 *******************************************************************************/
int btp_avrcp_sendSetPasValueCmd(uint16_t aclHandle, uint8_t *pAttID, uint8_t *pAttVal, uint8_t attCnt);

/******************************************************************************
 * Function: btp_avrcp_sendGetPasAttrTextCmd
 * Descript: This function requests the target device to provide supported player
 *           application setting attribute displayable text for the provided
 *           PlayerApplicationSettingAttributeIDs.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @pAttID--Player application setting attribute ID for which the corresponding
 *         attribute displayable text is needed. Refer BTP_AVRCP_PLAYER_APP_SET_ATTR_ENUM.
 *     @attCnt--Number of player application setting attribute IDs for which
 *         corresponding string is needed.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: PAS - PlayerApplicationSetting. Refer <AVRCP_v1.6.2> P51 and P134.
 *     This command is expected to be used mainly for extended attributes for
 *     menu navigation; for defined attributes the CT provides text for the
 *     application. However, to avoid inconsistency between CT and TG provided
 *     text, the TG can choose to provide text for defined attributes as well.
 *     It is assumed that all pairs used for menu extensions are statically
 *     defined by TG.
 *******************************************************************************/
int btp_avrcp_sendGetPasAttrTextCmd(uint16_t aclHandle, uint8_t *pAttID, uint8_t attCnt);

/******************************************************************************
 * Function: btp_avrcp_sendGetPasValueTextCmd
 * Descript: This function request the target device to provide target supported
 *           player application setting value displayable text for the provided
 *           player application setting attribute values.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @pAttID--Player application setting attribute ID for which the value
 *         needs to be set. Refer BTP_AVRCP_PLAYER_APP_SET_ATTR_ENUM.
 *     @pAttVal--Player application setting value ID for the corresponding player
 *         application setting attribute ID.
 *     @attCnt--Number of player application setting attributes for which the
 *         player application setting.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: PAS - PlayerApplicationSetting. Refer <AVRCP_v1.6.2> P52 and P134.
 *******************************************************************************/
int btp_avrcp_sendGetPasValueTextCmd(uint16_t aclHandle, uint8_t *pAttID, uint8_t *pAttVal, uint8_t attCnt);

/******************************************************************************
 * Function: btp_avrcp_sendInfomDisplayableCharacterSetCmd
 * Descript: This function provides the list of character sets supported by CT
 *           to the TG. This shall allow the TG to send responses with strings
 *           in any of the character sets supported by CT.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @pCharSet--Supported Character Set.
 *     @setCnt--Number of displayable character sets provided.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: Refer <AVRCP_v1.6.2> P53 and P134
 *     After the TG has received this command, the TG may send a string in any
 *     of the character sets that are specified in this command. By default, the
 *     TG shall send strings in UTF-8 if it has not received a valid version of
 *     this command.
 *******************************************************************************/
int btp_avrcp_sendInfomDisplayableCharacterSetCmd(uint16_t aclHandle, uint16_t *pCharSet, uint8_t setCnt);

/******************************************************************************
 * Function: btp_avrcp_sendInfomBatteryStatusOfCTCmd
 * Descript: This command frame is being sent by the CT to TG whenever the CT's
 *           battery status has been changed.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @status--Refer to BTP_AVRCP_BATTERY_STATUS_ENUM.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: Refer <AVRCP_v1.6.2> P53 and P134
 *******************************************************************************/
int btp_avrcp_sendInfomBatteryStatusOfCTCmd(uint16_t aclHandle, uint8_t status);

/******************************************************************************
 * Function: btp_avrcp_sendGetElementAttrCmd
 * Descript: These function requests the TG to provide the attributes of the
 *           element specified in the parameter. This shall only be used to
 *           retrieve Metadata for the currently playing track from the Addressed
 *           Player on the Control channel when GetItemAttributes is not supported.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @identify--Unique identifier to identify an element on TG.
 *     @pAttID--Specifies the attribute ID for the attributes to be retrieved.
 *         Refer BTP_AVRCP_ATTRID_ENUM.
 *     @attCnt--Number of Attributes provided.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: Refer <AVRCP_v1.6.2> P54 and P134
 *******************************************************************************/
int btp_avrcp_sendGetElementAttrCmd(uint16_t aclHandle, uint8_t identify[8], uint32_t *pAttID, uint8_t attCnt);

/******************************************************************************
 * Function: btp_avrcp_regInsCompleteCB
 * Descript: Register the callback interface for the completion of instruction
 *           execution.
 * Params:
 *     @cb--The callback interface for the completion of instruction execution.
 * Return: None.
 *******************************************************************************/
void btp_avrcp_regInsCompleteCB(BtpAvrcpInsCompleteCB cb);


#endif /* BTP_AVRCP_H */
