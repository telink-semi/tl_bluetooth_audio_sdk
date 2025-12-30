/********************************************************************************************************
 * @file    btp_browsing.h
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
#ifndef BTP_BROWSING_H
#define BTP_BROWSING_H

typedef enum
{
    BTP_BROWSING_FEATURE_BIT_SELECT                           = 0,  // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_UP                               = 1,  // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_DOWN                             = 2,  // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_LEFT                             = 3,  // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_RIGHT                            = 4,  // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_RIGHT_UP                         = 5,  // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_RIGHT_DOWN                       = 6,  // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_LEFT_UP                          = 7,  // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_LEFT_DOWN                        = 8,  // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_ROOT_MENU                        = 9,  // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_SETUP_MENU                       = 10, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_CONTENTS_MENU                    = 11, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_FAVORITE_MENU                    = 12, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_EXIT                             = 13, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_0                                = 14, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_1                                = 15, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_2                                = 16, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_3                                = 17, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_4                                = 18, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_5                                = 19, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_6                                = 20, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_7                                = 21, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_8                                = 22, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_9                                = 23, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_DOT                              = 24, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_ENTER                            = 25, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_CLEAR                            = 26, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_CHANNEL_UP                       = 27, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_CHANNEL_DOWN                     = 28, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_PREVIOUS_CHANNEL                 = 29, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_SOUND_SELECT                     = 30, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_INPUT_SELECT                     = 31, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_DISPLAY_INFO                     = 32, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_HELP                             = 33, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_PAGE_UP                          = 34, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_PAGE_DOWN                        = 35, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_POWER                            = 36, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_VOLUME_UP                        = 37, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_VOLUME_DOWN                      = 38, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_MUTE                             = 39, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_PLAY                             = 40, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_STOP                             = 41, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_PAUSE                            = 42, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_RECORD                           = 43, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_REWIND                           = 44, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_FAST_FORWARD                     = 45, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_EJECT                            = 46, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_FORWARD                          = 47, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_BACKWARD                         = 48, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_ANGLE                            = 49, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_SUBPICTURE                       = 50, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_F1                               = 51, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_F2                               = 52, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_F3                               = 53, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_F4                               = 54, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_F5                               = 55, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_VENDOR_UNIQUE                    = 56, // This PASSTHROUGH command is supported.
    BTP_BROWSING_FEATURE_BIT_BASIC_GROUP_NAVIGATION           = 57, // This overrules the SDP entry as it is set per player7.
    BTP_BROWSING_FEATURE_BIT_ADVANCED_CONTROL_PLAYER          = 58, // This bit is set if the player supports at least AVRCP 1.4.
    BTP_BROWSING_FEATURE_BIT_BROWSING                         = 59, // This bit is set if the player supports browsing.
    BTP_BROWSING_FEATURE_BIT_SEARCHING                        = 60, // This bit is set if the player supports searching.
    BTP_BROWSING_FEATURE_BIT_ADD_TO_NOW_PLAYING               = 61, // This bit is set if the player supports the AddToNowPlaying command.
    BTP_BROWSING_FEATURE_BIT_UIDS_UNIQUE_IN_PLAYERBROWSE_TREE = 62, // This bit is set if the player is able to maintain unique UIDs across the player browse tree.
    BTP_BROWSING_FEATURE_BIT_ONLY_BROWSABLE_WHEN_ADDRESSED    = 63, // This bit is set if the player is only able to be browsed when it is set as the Addressed Player.
    BTP_BROWSING_FEATURE_BIT_ONLY_SEARCHABLE_WHEN_ADDRESSED   = 64, // This bit is set if the player is only able to be searched when it is set as the Addressed player.
    BTP_BROWSING_FEATURE_BIT_NOW_PLAYING                      = 65, // This bit is set if the player supports the NowPlaying folder. Note
                                                                    // that for all players that support browsing this bit shall be set.
    BTP_BROWSING_FEATURE_BIT_UID_PERSISTENCY = 66,                  // This bit is set if the Player is able to persist UID values between AVRCP Browse Reconnects.
    BTP_BROWSING_FEATURE_BIT_NUMBER_OF_ITEMS = 67,                  // This bit is set if the player supports the GetTotalNumberOfItems browsing command.
    BTP_BROWSING_FEATURE_BIT_COVER_ART       = 68,                  // This bit is set if the player supports Cover Art transfer.
} BTP_BROWSING_FEATURE_BIT_ENUM;

//<AVRCP_v1.6.2> P72
typedef enum
{
    BTP_BROWSING_SCOPE_MEDIA_PLAYER_LIST               = 0x00, // Media Player Item(0x01). Contains all available media players.
    BTP_BROWSING_SCOPE_MEDIA_PLAYER_VIRTUAL_FILESYSTEM = 0x01, // Folder Item(0x02) and Media Element Item(0x03). The virtual filesystem containing the media content of
                                                               // the browsed player.
    BTP_BROWSING_SCOPE_SEARCH       = 0x02,                    // Media Element Item(0x03). The results of a search operation on the browsed player
    BTP_BROWSING_SCOPE_NOW_PLAYGING = 0x03,                    // Media Element Item(0x03). The Now Playing list (or queue) of the addressed player
} BTP_BROWSING_SCOPE_ENUM;

typedef enum
{
    BTP_BROWSING_ATTID_NONE          = 0x00,
    BTP_BROWSING_ATTID_TITLE         = 0x01, // Text field representing the title, song name or content description coded per specified character set.
    BTP_BROWSING_ATTID_ARTIST_NAME   = 0x02, // Text field representing artist(s), performer(s) or group coded per specified character set.
    BTP_BROWSING_ATTID_ALBUM_NAME    = 0x03, // Text field representing the title of the recording (source) from which the audio in the file is taken.
    BTP_BROWSING_ATTID_TRACK_NUMBER  = 0x04, // Numeric ASCII string containing the order number of the audio-file on its original recording.
    BTP_BROWSING_ATTID_TOTAL_TRACKS  = 0x05, // Numeric ASCII string containing the total number of tracks or elements on the original recording.
    BTP_BROWSING_ATTID_GENRE         = 0x06, // Text field representing the category of the composition characterized by a particular style.
    BTP_BROWSING_ATTID_PLAYING_TIME  = 0x07, // Numeric ASCII string containing the length of the audio file in milliseconds.
    BTP_BROWSING_ATTID_DEF_COVER_ART = 0x08, // BIP Image Handle
} BTP_BROWSING_ATTID_ENUM;

typedef enum
{
    BTP_BROWSING_PATHDIR_FOLDER_UP   = 0x00,
    BTP_BROWSING_PATHDIR_FOLDER_DOWN = 0x01,
} BTP_BROWSING_PATHDIR_ENUM;

typedef enum
{
    BTP_BROWSING_ITEM_TYPE_MEDIA_PLAYER_ITEM  = 0x01,
    BTP_BROWSING_ITEM_TYPE_FOLDER_ITEM        = 0x02,
    BTP_BROWSING_ITEM_TYPE_MEDIA_ELEMENT_ITEM = 0x03,
    BTP_BROWSING_ITEM_TYPE_ATTR_LIST          = 0xFF, //
} BTP_BROWSING_ITEM_TYPE_ENUM;

typedef enum
{ // Bit Mask
    BTP_BROWSING_PLAYER_TYPE_AUDIO              = 0x01,
    BTP_BROWSING_PLAYER_TYPE_VIDEO              = 0x02,
    BTP_BROWSING_PLAYER_TYPE_BROADCASTING_AUDIO = 0x04,
    BTP_BROWSING_PLAYER_TYPE_BROADCASTING_VIDEO = 0x08,
} BTP_BROWSING_PLAYER_TYPE_ENUM;

typedef enum
{
    BTP_BROWSING_PLAYER_SUBTYPE_AUDIO_BOOK = 0x01,
    BTP_BROWSING_PLAYER_SUBTYPE_PDCAST     = 0x02,
} BTP_BROWSING_PLAYER_SUBTYPE_ENUM;

typedef enum
{
    BTP_BROWSING_FOLDER_TYPE_MIXED     = 0x00,
    BTP_BROWSING_FOLDER_TYPE_TITLES    = 0x01,
    BTP_BROWSING_FOLDER_TYPE_ALBUMS    = 0x02,
    BTP_BROWSING_FOLDER_TYPE_ARTISTS   = 0x03,
    BTP_BROWSING_FOLDER_TYPE_GENRES    = 0x04,
    BTP_BROWSING_FOLDER_TYPE_PLAYLISTS = 0x05,
    BTP_BROWSING_FOLDER_TYPE_YEARS     = 0x06,
} BTP_BROWSING_FOLDER_TYPE_ENUM;

typedef enum
{ // Bit Mask
    BTP_BROWSING_MEDIA_TYPE_AUDIO = 0x01,
    BTP_BROWSING_MEDIA_TYPE_VIDEO = 0x02,
} BTP_BROWSING_MEDIA_TYPE_ENUM;

typedef enum
{
    // http://www.iana.org/assignments/character-sets
    BTP_BROWSING_CHAR_SET_UTF8     = 0x006A,
    BTP_BROWSING_CHAR_SET_UTF16    = 0x03F7,
    BTP_BROWSING_CHAR_SET_UTF32    = 0x03F9,
    BTP_BROWSING_CHAR_SET_GBK      = 0x0071,
    BTP_BROWSING_CHAR_SET_GBK18030 = 0x0072,
    BTP_BROWSING_CHAR_SET_UNICODE  = 0x03E8,
} BTP_BROWSING_CHAR_SET_ENUM;

typedef enum
{
    BTP_ELEMENTS_ATTR_ID_NONE            = 0x00,
    BTP_ELEMENTS_ATTR_ID_TITLE           = 0x01,
    BTP_ELEMENTS_ATTR_ID_ARTIST_NAME     = 0x02,
    BTP_ELEMENTS_ATTR_ID_ALBUM_NAME      = 0x03,
    BTP_ELEMENTS_ATTR_ID_TRACK_NUMBER    = 0x04,
    BTP_ELEMENTS_ATTR_ID_TOTAL_NUM_TRACK = 0x05,
    BTP_ELEMENTS_ATTR_ID_GENRE           = 0x06,
    BTP_ELEMENTS_ATTR_ID_PLAYING_TIME    = 0x07,
    BTP_ELEMENTS_ATTR_ID_COVER_ART       = 0x08,
} BTP_ELEMENTS_ATTR_ID_ENUM;

typedef struct
{
    uint32_t attrID;  // Refer to BTP_BROWSING_ATTID_ENUM.
    uint16_t charSet; // Refer BTP_BROWSING_CHAR_SET_ENUM
    uint16_t valLens; // Length of the value of this attribute
    uint8_t *pValue;
} btp_browsing_attrItem_t;

typedef struct
{
    uint16_t playerID;
    uint8_t  playerType;    // Refer BTP_BROWSING_PLAYER_TYPE_ENUM
    uint32_t playerSubType; // Refer BTP_BROWSING_PLAYER_SUBTYPE_ENUM.
    uint8_t  playStatus;    // Refer BTP_AVRCP_PLAY_STATE_ENUM
    uint8_t  feature[16];   // Refer BTP_BROWSING_FEATURE_BIT_ENUM.
    uint16_t charSet;       // Refer BTP_BROWSING_CHAR_SET_ENUM.
    uint16_t nameLen;       // Length of Displayable Name in octets.
    uint8_t *pDisName;
} btp_browsing_playerItem_t;

typedef struct
{
    uint8_t  folderUID[8];
    uint8_t  folderType; // Refer BTP_BROWSING_FOLDER_TYPE_ENUM.
    uint8_t  playabe;    // 0x00-The folder cannot be played; 0x01-The folder can be played;
    uint16_t charSet;    // Refer BTP_BROWSING_CHAR_SET_ENUM.
    uint16_t nameLen;    // Length of Displayable Name in octets.
    uint8_t *pDisName;
} btp_browsing_folderItem_t;

typedef struct
{
    uint8_t                  elementUID[8];
    uint16_t                 mediaType; // Refer BTP_BROWSING_MEDIA_TYPE_ENUM.
    uint16_t                 charSet;   // Refer BTP_BROWSING_CHAR_SET_ENUM.
    uint16_t                 nameLen;   // Length of Displayable Name in octets.
    uint8_t                 *pDisName;
    uint8_t                  AttrNumb; // The number of attributes included with this media element item
    btp_browsing_attrItem_t *pAttrItem;
} btp_browsing_elementItem_t;

// ChangePathRsp
typedef struct
{
    uint32_t itemNumb;
} btp_browsing_searchRsp_t;

typedef struct
{
    uint32_t itemNumb;
} btp_browsing_changePathRsp_t;

typedef struct
{
    uint8_t itemNumb;
} btp_browsing_getItemAttrRsp_t;

typedef struct
{
    uint16_t uidCounter;
    uint32_t itemNumb;
} btp_browsing_getTotalItemRsp_t;

typedef struct
{
    uint16_t uidCounter;
    uint16_t itemNumb;
} btp_browsing_getFolderItemRsp_t;

typedef struct
{
    uint16_t uidCounter;
    uint32_t itemNumb;
    uint16_t charSet;
    uint8_t  folderDepth;
} btp_browsing_setBrowsedPlayerRsp_t;

// Status-TLK_ENONE is success.
typedef void (*BtpBrowsingItemsReportCB)(uint16_t aclHandle, uint8_t pduID, uint8_t status, uint8_t itemType, uint8_t itemNumb, void *pItemData);
// Status-Refer BTP_AVRCP_STATUS_CODE_ENUM. BTP_AVRCP_STATUS_CODE_OPERATE_WITHOUT_ERROR is success.
typedef void (*BtpBrowsingInsCompleteCB)(uint16_t aclHandle, uint8_t pduID, uint8_t status, void *pParam, uint16_t paramLen);


/******************************************************************************
 * Function: BROWSING initial interface
 * Descript: Defines trigger the initial flow of A2DP sink and BROWSING source
 * Params:
 * 		@count[IN]--the BROWSING number, refer to TLK_BT_BROWSING_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value is returned means the initial process fail.
 *******************************************************************************/
int btp_browsing_init(uint8_t count);

/******************************************************************************
 * Function: BROWSING deinitial interface
 * Descript: Defines trigger the initial flow of BROWSING sink and BROWSING source
 * Params:
 * 		@count[IN]--the BROWSING number, refer to TLK_BT_BROWSING_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int btp_browsing_deinit(uint8_t count);

/******************************************************************************
 * Function: btp_browsing_getMemLen
 * Descript: BROWSING get the size of the required number of nodes interface.
 * Params:
 * 		@count[IN]--the BROWSING number, refer to TLK_BT_BROWSING_MAX_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int btp_browsing_getMemLen(uint8_t count);

/******************************************************************************
 * Function: btp_browsing_connect
 * Descript: Defines trigger the browsing connect cmd to peer browsing entity and
 *           setup a connection with devices.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_browsing_connect(uint16_t aclHandle);

/******************************************************************************
 * Function: btp_browsing_disconn
 * Descript: Defines trigger the browsing disconnect cmd to peer browsing entity
 *           and tear up a connection which specify by aclhandle.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @usrId--The service user id.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_browsing_disconn(uint16_t aclHandle);

/******************************************************************************
 * Function: btp_browsing_destroy
 * Descript: Defines trigger to release the resource which allocated.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 * Return: None.
 *******************************************************************************/
void btp_browsing_destroy(uint16_t aclHandle);

/******************************************************************************
 * Function: btp_browsing_sendCmd
 * Descript: Send the command message from CT to TG.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @trid--The id of the instruction message. If trid is 0xFF, it will used
 *         the trid of system. And the system's trid will increment after sent.
 *     @pduID--Refer BTP_AVRCP_PDUID_ENUM (Browsing only).
 *     @pParam--The param to be sent.
 *     @paramLen--The length of the param.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_browsing_sendCmd(uint16_t aclHandle, uint8_t trid, uint8_t pduID, uint8_t *pParam, uint16_t paramLen);

/******************************************************************************
 * Function: btp_browsing_sendRsp
 * Descript: Send the response message from TG to CT.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @trid--The id of the instruction message. It must be matched up with the
 *         command to be reponsed.
 *     @pduID--Refer BTP_AVRCP_PDUID_ENUM (Browsing only).
 *     @pParam--The param to be sent.
 *     @paramLen--The length of the param.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: It should be noted that as with the commands the response can be matched
 *       up with the command it refers to by the AVCTP transaction label.
 *******************************************************************************/
int btp_browsing_sendRsp(uint16_t aclHandle, uint8_t trid, uint8_t pduID, uint8_t *pParam, uint16_t paramLen);

/******************************************************************************
 * Function: btp_browsing_sendGeneralReject
 * Descript: The General Reject response is used in situations where the received
 *           command cannot be parsed sufficiently to return a command specific
 *           response, for example, commands where the PDU Id is not recognized.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @errCode--Refer BTP_AVRCP_STATUS_CODE_ENUM.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: It should be noted that as with all other commands the response can be
 *       matched up with the command it refers to by the AVCTP transaction label.
 *******************************************************************************/
int btp_browsing_sendGeneralReject(uint16_t aclHandle, uint8_t errCode);

/******************************************************************************
 * Function: btp_browsing_setChangePathCmd
 * Descript: The function is used to navigate the virtual filesystem. This command
 *           allows the CT to navigate one level up or down in the virtual filesystem.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @uidCounter--Database aware players shall maintain a UID counter that is
 *         incremented whenever the database changes.
 *     @direct--Refer BTP_BROWSING_PATHDIR_ENUM.
 *     @pUID--Fixed 8 Bytes.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note:
 *     1.pUID(Folder UID)-This may be retrieved via a GetFolderItems command.
 *******************************************************************************/
int btp_browsing_setChangePathCmd(uint16_t aclHandle, uint16_t uidCounter, uint8_t direct, uint8_t *pUID);

/******************************************************************************
 * Function: btp_browsing_setBrowsedPlayerCmd
 * Descript: The function is used to control to which player browsing commands
 *           should be routed.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @playerID--Unique Media Player Id. You can get is by sending
 *         btp_browsing_sendGetFolderItemsCmd (
 *             scope=BTP_BROWSING_SCOPE_MEDIA_PLAYER_LIST,
 *             startItem=0, endItem=100, attCount=0xFF, pAttList=NULL
 *         ).
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note:
 *     1.It shall be sent successfully before any other commands are sent on the
 *       browsing channel except GetFolderItems in the Media Player List scope.
 *     2.If the browsed player has become unavailable, the SetBrowsedPlayer
 *       command shall be sent successfully again before further commands are
 *       sent on the browsing channel.
 *     3.Some players may support browsing only when set as the Addressed Player.
 *       This is shown in the player feature bitmask (Refer BTP_BROWSING_FEATURE_BIT_ENUM).
 *       If a SetBrowsedPlayer command is received by the TG for a Player Id
 *       which does not support browsing while not addressed it will return the
 *       PlayerNotAddressed error in the status field of the response.
 *******************************************************************************/
int btp_browsing_setBrowsedPlayerCmd(uint16_t aclHandle, uint16_t playerID);

/******************************************************************************
 * Function: btp_browsing_sendGetFolderItemsCmd
 * Descript: This function is used to retrieve a listing of the contents of a
 *           folder.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @scope--Refer BTP_BROWSING_SCOPE_ENUM.
 *     @startItem--The offset within the listing of the item, which should be
 *         the first returned item. The first element in the listing is at
 *         offset 0.
 *     @endItem--The offset within the listing of the item which should be the
 *         final returned item. If this is set to a value beyond what is
 *         available, the TG shall return items from the provided Start Item
 *         index to the index of the final item. If the End Item index is
 *         smaller than the Start Item index, the TG shall return an error.
 *         If CT requests too many items, TG can respond with a sub-set of the
 *         requested items.
 *     @attCount--0x00, All attributes are requested. There is no following
 *         Attribute List; 0xFF, No attributes are requested. There is no
 *         following Attribute List; 0x01-0xFE, The following Attribute List
 *         contains this number of attributes.
 *     @pAttList--Attributes which are requested to be returned for each item
 *         returned.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note: The CT may specify a range of entries to be returned. This means that
 *     a CT which can only display a limited number of items can obtain a
 *     listing one part at a time as the user scrolls the display. If possible,
 *     the returned list should resemble the order used on the local display on
 *     the TG, but should list all folder items before media element items to
 *     facilitate browsing on the CT.
 *******************************************************************************/
int btp_browsing_sendGetFolderItemsCmd(uint16_t aclHandle, uint8_t scope, uint32_t startItem, uint32_t endItem, uint8_t attCount, uint32_t *pAttList);

/******************************************************************************
 * Function: btp_browsing_sendGetItemAttrCmd
 * Descript:
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @scope--Refer BTP_BROWSING_SCOPE_ENUM.
 *     @pUids--Fixed 8 Bytes.
 *     @uidCounter--Database aware players shall maintain a UID counter that is
 *         incremented whenever the database changes.
 *     @attCount--0x00, All attributes are requested. There is no following
 *         Attribute List; 0xFF, No attributes are requested. There is no
 *         following Attribute List; 0x01-0xFE, The following Attribute List
 *         contains this number of attributes.
 *     @pAttList--Attributes which are requested to be returned for each item
 *         returned.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 * Note:
 *     1.pUID(Folder UID)-This may be obtained by call btp_avrcp_sendRegEventNotify
 *       (eventID=BTP_AVRCP_EVTID_TRACK_CHANGED). This will return trackID after
 *       tack status changed. TackID is used for Folder UID.
 *******************************************************************************/
int btp_browsing_sendGetItemAttrCmd(uint16_t aclHandle, uint8_t scope, uint8_t *pUids, uint8_t uidCounter, uint8_t attCount, uint32_t *pAttList);

/******************************************************************************
 * Function: btp_browsing_sendGetTotalNumbOfItemsCmd
 * Descript: This function is used to retrieve the number of items in a folder
 *           prior to calling GetFolderItems to retrieve a listing of the
 *           contents of a folder. The purpose of the command is to provide both
 *           scaling information, and total item count for devices that may not
 *           be able to store or display an entire folder content listing.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @scope--Refer BTP_BROWSING_SCOPE_ENUM.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_browsing_sendGetTotalNumbOfItemsCmd(uint16_t aclHandle, uint8_t scope);

/******************************************************************************
 * Function: btp_browsing_sendGetTotalNumbOfItemsRsp
 * Descript:
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @status--
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_browsing_sendGetTotalNumbOfItemsRsp(uint16_t aclHandle, uint8_t status, uint16_t uidCounter, uint32_t number);

/******************************************************************************
 * Function: btp_browsing_regItemsReportCB
 * Descript: Registers the callback function interface for Item change reporting.
 * Params:
 *     @cb--The callback function interface for Item change reporting.
 * Return: None.
 *******************************************************************************/
void btp_browsing_regItemsReportCB(BtpBrowsingItemsReportCB cb);

/******************************************************************************
 * Function: btp_browsing_regInsCompleteCB
 * Descript: Register the callback function interface for the completion of
 *           instruction execution.
 * Params:
 *     @cb--The callback function interface for the completion of instruction
 *         execution.
 * Return: None.
 *******************************************************************************/
void btp_browsing_regInsCompleteCB(BtpBrowsingInsCompleteCB cb);


#endif // BTP_BROWSING_H
