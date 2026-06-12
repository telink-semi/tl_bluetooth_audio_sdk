/********************************************************************************************************
 * @file    svc_mcs.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include <stddef.h>

#include "common/types.h"
#include "common/utility.h"
#include "common/compiler.h"

#include "../../../l2cap/att/inc/ble_att_uuid.h"
#include "../../../l2cap/att/inc/ble_att_service.h"

#include "../../../l2cap/att/inc/uuid16bit.h"

#include "../../inc/svc.h"
#include "../../inc/svc_format.h"

#include "../svc_audio.h"

#include "svc_ots.h"

#ifndef LE_AUDIO_MCS_MEDIA_PLAYER_ICON_OBJECT_ID
#define LE_AUDIO_MCS_MEDIA_PLAYER_ICON_OBJECT_ID 1
#endif

#ifndef LE_AUDIO_MCS_MEDIA_PLAYER_ICON_URL
#define LE_AUDIO_MCS_MEDIA_PLAYER_ICON_URL 1
#endif

#ifndef LE_AUDIO_MCS_PLAYBACK_SPEED
#define LE_AUDIO_MCS_PLAYBACK_SPEED 1
#endif

#ifndef LE_AUDIO_MCS_SEEKING_SPEED
#define LE_AUDIO_MCS_SEEKING_SPEED 1
#endif

#ifndef LE_AUDIO_MCS_CURRENT_TRACK_OBJECT_ID
#define LE_AUDIO_MCS_CURRENT_TRACK_OBJECT_ID 1
#endif

#ifndef LE_AUDIO_MCS_PLAYING_ORDER
#define LE_AUDIO_MCS_PLAYING_ORDER 1
#endif

#ifndef LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED
#define LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED 1
#endif

#ifndef LE_AUDIO_MCS_MEDIA_CONTROL_POINT
#define LE_AUDIO_MCS_MEDIA_CONTROL_POINT 1
#endif

#ifndef LE_AUDIO_MCS_SEARCH_RESULTS_OBJECT_ID
#define LE_AUDIO_MCS_SEARCH_RESULTS_OBJECT_ID 1
#endif

#ifndef LE_AUDIO_MCS_PMEDIA_PLAYER_NAME_MAX_SIZE
#define LE_AUDIO_MCS_PMEDIA_PLAYER_NAME_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_MCS_PMEDIA_PLAYER_ICON_URL_MAX_SIZE
#define LE_AUDIO_MCS_PMEDIA_PLAYER_ICON_URL_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_MCS_TRACK_TITLE_MAX_SIZE
#define LE_AUDIO_MCS_TRACK_TITLE_MAX_SIZE 64
#endif

#define MCS_START_HDL SERVICE_MEDIA_CONTROL_HDL

_attribute_ble_data_retention_ static uint8_t  mcsMediaPlayerNameValue[LE_AUDIO_MCS_PMEDIA_PLAYER_NAME_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t mcsMediaPlayerNameValueLen;
const uint16_t                                 mcsMediaPlayerNameMaxSize = sizeof(mcsMediaPlayerNameValue);

#if LE_AUDIO_MCS_MEDIA_PLAYER_ICON_OBJECT_ID
_attribute_ble_data_retention_ static uint8_t  mcsMediaPlayerIconObjectIDValue[6];
_attribute_ble_data_retention_ static uint16_t mcsMediaPlayerIconObjectIDValueLen = 6;
#endif

const uint16_t mcsMediaPlayerIconURLMaxSize = LE_AUDIO_MCS_PMEDIA_PLAYER_ICON_URL_MAX_SIZE;
#if LE_AUDIO_MCS_MEDIA_PLAYER_ICON_URL
_attribute_ble_data_retention_ static uint8_t  mcsMediaPlayerIconURLValue[LE_AUDIO_MCS_PMEDIA_PLAYER_ICON_URL_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t mcsMediaPlayerIconURLValueLen;
#endif

_attribute_ble_data_retention_ static uint8_t  mcsTrackTitleValue[LE_AUDIO_MCS_TRACK_TITLE_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t mcsTrackTitleValueLen;
const uint16_t                                 mcsTrackTitleMaxSize = sizeof(mcsTrackTitleValue);

_attribute_ble_data_retention_ static uint32_t mcsTrackDurationValue    = 0xFFFFFFFF;
static const uint16_t                          mcsTrackDurationValueLen = 4;

_attribute_ble_data_retention_ static uint32_t mcsTrackPositionValue    = 0xFFFFFFFF;
static const uint16_t                          mcsTrackPositionValueLen = 4;

#if LE_AUDIO_MCS_PLAYBACK_SPEED
_attribute_ble_data_retention_ static char mcsPlaybackSpeedValue    = 0;
static const uint16_t                      mcsPlaybackSpeedValueLen = 1;
#endif

#if LE_AUDIO_MCS_SEEKING_SPEED
_attribute_ble_data_retention_ static char mcsSeekingSpeedValue    = 0;
static const uint16_t                      mcsSeekingSpeedValueLen = 1;
#endif

#if LE_AUDIO_MCS_CURRENT_TRACK_OBJECT_ID
_attribute_ble_data_retention_ static uint8_t  mcsCurrentTrackSegmentsObjectIDValue[6];
_attribute_ble_data_retention_ static uint16_t mcsCurrentTrackSegmentsObjectIDValueLen;

_attribute_ble_data_retention_ static uint8_t  mcsCurrentTrackObjectIDValue[6];
_attribute_ble_data_retention_ static uint16_t mcsCurrentTrackObjectIDValueLen;

_attribute_ble_data_retention_ static uint8_t  mcsNextTrackObjectIDValue[6];
_attribute_ble_data_retention_ static uint16_t mcsNextTrackObjectIDValueLen;

_attribute_ble_data_retention_ static uint8_t  mcsParentGroupTrackObjectIDValue[6];
_attribute_ble_data_retention_ static uint16_t mcsParentGroupTrackObjectIDValueLen;

_attribute_ble_data_retention_ static uint8_t  mcsCurrentGroupTrackObjectIDValue[6];
_attribute_ble_data_retention_ static uint16_t mcsCurrentGroupTrackObjectIDValueLen;
#endif

#if LE_AUDIO_MCS_PLAYING_ORDER
_attribute_ble_data_retention_ static uint8_t mcsPlayingOrderValue    = 0x01;
static const uint16_t                         mcsPlayingOrderValueLen = 1;
#endif

#if LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED
_attribute_ble_data_retention_ static uint16_t mcsPlayingOrderSupportedValue;
static const uint16_t                          mcsPlayingOrderSupportedValueLen = 2;
#endif

_attribute_ble_data_retention_ static uint8_t mcsMediaStateValue    = 0x00;
static const uint16_t                         mcsMediaStateValueLen = 1;

#if LE_AUDIO_MCS_MEDIA_CONTROL_POINT
_attribute_ble_data_retention_ static uint8_t  mcsMediaControlPointValue[5];
_attribute_ble_data_retention_ static uint16_t mcsMediaControlPointValueLen;

_attribute_ble_data_retention_ static uint32_t mcsMediaControlPointSupportedValue;
static const uint16_t                          mcsMediaControlPointSupportedValueLen = 4;
#endif

#if LE_AUDIO_MCS_SEARCH_RESULTS_OBJECT_ID
_attribute_ble_data_retention_ static uint8_t  mcsSearchResultsObjectIDValue[6];
_attribute_ble_data_retention_ static uint16_t mcsSearchResultsObjectIDValueLen;

_attribute_ble_data_retention_ static uint8_t mcsSearchControlPointValue;
static const uint16_t                         mcsSearchControlPointValueLen = sizeof(mcsSearchControlPointValue);
#endif

_attribute_ble_data_retention_ static uint8_t mcsCCIDValue;
static const uint16_t                         mcsCCIDValueLen = sizeof(mcsCCIDValue);

extern const uint16_t otsIncludeValue[3];

/*
 * @brief the structure for default MCS service List.
 */
static const struct atts_attribute mcsList[] = {
    ATTS_PRIMARY_SERVICE(serviceGenericMediaControlAttUuid),

    ATTS_INCLUDE_DEFINE(&otsIncludeValue),

    //Media Player Name
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicMediaPlayerNameAttUuid, mcsMediaPlayerNameValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_MCS_MEDIA_PLAYER_ICON_OBJECT_ID
    //Media Player Icon Object ID
    ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(charPropRead, characteristicMediaPlayerIconObjectIdAttUuid, mcsMediaPlayerIconObjectIDValue),
#endif

#if LE_AUDIO_MCS_MEDIA_PLAYER_ICON_URL
    //Media Player Icon URL
    ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(charPropRead, characteristicMediaPlayerIconUrlAttUuid, mcsMediaPlayerIconURLValue),
#endif

    //Track Changed
    ATTS_CHARACTERISTIC_DECLARATIONS(charPropNotify),
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_NONE, characteristicTrackChangedAttUuid, ATTS_SET_NONE, 0, NULL, NULL),
    ATTS_COMMON_CCC_DEFINE,

    //Track Title
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicTrackTitleAttUuid, mcsTrackTitleValue),
    ATTS_COMMON_CCC_DEFINE,

    //Track Duration
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropReadNotify, characteristicTrackDurationAttUuid, mcsTrackDurationValue),
    ATTS_COMMON_CCC_DEFINE,

    //Track Position
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_WCB(charPropReadWriteWriteWithout, characteristicTrackPositionAttUuid, mcsTrackPositionValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_MCS_PLAYBACK_SPEED
    //Playback Speed
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_WCB(charPropReadWriteWriteWithoutNotify, characteristicPlaybackSpeedAttUuid, mcsPlaybackSpeedValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_MCS_SEEKING_SPEED
    //Seeking Speed
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropReadNotify, characteristicSeekingSpeedAttUuid, mcsSeekingSpeedValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_MCS_CURRENT_TRACK_OBJECT_ID
    //Current Track Segments Object ID
    ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(charPropRead, characteristicCurrentTrackSegmentsObjectIdAttUuid, mcsCurrentTrackSegmentsObjectIDValue),

    //Current Track Object ID
    ATTS_CHAR_UUID_ENCR_RDWR_POINT_WCB(charPropReadWriteWriteWithoutNotify, characteristicCurrentTrackObjectIdAttUuid, mcsCurrentTrackObjectIDValue),
    ATTS_COMMON_CCC_DEFINE,

    //Next Track Object ID
    ATTS_CHAR_UUID_ENCR_RDWR_POINT_WCB(charPropReadWriteWriteWithoutNotify, characteristicNextTrackObjectIdAttUuid, mcsNextTrackObjectIDValue),
    ATTS_COMMON_CCC_DEFINE,

    //Parent Group Object ID
    ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(charPropReadNotify, characteristicParentGroupObjectIdAttUuid, mcsParentGroupTrackObjectIDValue),
    ATTS_COMMON_CCC_DEFINE,

    //Current Group Object ID
    ATTS_CHAR_UUID_ENCR_RDWR_POINT_WCB(charPropReadWriteWriteWithoutNotify, characteristicCurrentGroupObjectIdAttUuid, mcsCurrentGroupTrackObjectIDValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_MCS_PLAYING_ORDER
    //Playing Order
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_WCB(charPropReadWriteWriteWithoutNotify, characteristicPlayingOrderAttUuid, mcsPlayingOrderValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED
    //Playing Order Supported
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropRead, characteristicPlayingOrdersSupportedAttUuid, mcsPlayingOrderSupportedValue),
#endif

    //Media State
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_NOCB(charPropReadNotify, characteristicMediaStateAttUuid, mcsMediaStateValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_MCS_MEDIA_CONTROL_POINT
    //Media Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_POINT_CB(charPropWriteWriteWithoutNotify, characteristicMediaControlPointAttUuid, mcsMediaControlPointValue),
    ATTS_COMMON_CCC_DEFINE,

    //Media Control Point Supported
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropReadNotify, characteristicMediaCtrlPointOpSupportedAttUuid, mcsMediaControlPointSupportedValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_MCS_SEARCH_RESULTS_OBJECT_ID
    //Search Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_ENTITY_CB(charPropWriteWriteWithoutNotify, characteristicSearchControlPointAttUuid, mcsSearchControlPointValue),
    ATTS_COMMON_CCC_DEFINE,

    //Search Results Object ID
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicSearchResultsObjectIdAttUuid, mcsSearchResultsObjectIDValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

    //Content Control ID(CCID)
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropRead, characteristicContentControlIdAttUuid, mcsCCIDValue),
};

/*
 * @brief the structure for default MCS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcMcsGroup = {
    NULL, mcsList, NULL, NULL, MCS_START_HDL, 0,
};

/**
 * @brief      for user add default MCS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addMcsGroup(void)
{
    blc_svc_addOtsGroup();
    svcMcsGroup.endHandle = svcMcsGroup.startHandle + ARRAY_SIZE(mcsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcMcsGroup);
}

/**
 * @brief      for user remove default MCS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeMcsGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(MCS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in MCS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_mcsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    blc_svc_otsCbackRegister(readCback, writeCback);
    svcMcsGroup.readCallback  = readCback;
    svcMcsGroup.writeCallback = writeCback;
}
