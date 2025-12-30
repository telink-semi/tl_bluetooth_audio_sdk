/********************************************************************************************************
 * @file    svc_gmcs.c
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

#ifndef LE_AUDIO_GMCS_SUPPORT_OTS
#define LE_AUDIO_GMCS_SUPPORT_OTS 0
#endif

#ifndef LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_OBJECT_ID
#define LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_OBJECT_ID 1
#endif

#ifndef LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_URL
#define LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_URL 1
#endif

#ifndef LE_AUDIO_GMCS_PLAYBACK_SPEED
#define LE_AUDIO_GMCS_PLAYBACK_SPEED 1
#endif

#ifndef LE_AUDIO_GMCS_SEEKING_SPEED
#define LE_AUDIO_GMCS_SEEKING_SPEED 1
#endif

#ifndef LE_AUDIO_GMCS_CURRENT_TRACK_OBJECT_ID
#define LE_AUDIO_GMCS_CURRENT_TRACK_OBJECT_ID 1
#endif

#ifndef LE_AUDIO_GMCS_PLAYING_ORDER
#define LE_AUDIO_GMCS_PLAYING_ORDER 1
#endif

#ifndef LE_AUDIO_GMCS_PLAYING_ORDERS_SUPPORTED
#define LE_AUDIO_GMCS_PLAYING_ORDERS_SUPPORTED 1
#endif

#ifndef LE_AUDIO_GMCS_MEDIA_CONTROL_POINT
#define LE_AUDIO_GMCS_MEDIA_CONTROL_POINT 1
#endif

#ifndef LE_AUDIO_GMCS_SEARCH_RESULTS_OBJECT_ID
#define LE_AUDIO_GMCS_SEARCH_RESULTS_OBJECT_ID 1
#endif

#define GMCS_START_HDL SERVICE_GENERIC_MEDIA_CONTROL_HDL

static const uint8_t  gmcsMediaPlayerNameValue    = 0x00;
static const uint16_t gmcsMediaPlayerNameValueLen = 1;

#if LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_OBJECT_ID
static const uint8_t  gmcsMediaPlayerIconObjectIDValue[6];
static const uint16_t gmcsMediaPlayerIconObjectIDValueLen = 6;
#endif

#if LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_URL
static const uint8_t  gmcsMediaPlayerIconURLValue    = 0x00;
static const uint16_t gmcsMediaPlayerIconURLValueLen = 1;
#endif

static const uint8_t  gmcsTrackTitleValue    = 0x00;
static const uint16_t gmcsTrackTitleValueLen = 1;

static const uint32_t gmcsTrackDurationValue    = 0xFFFFFFFF;
static const uint16_t gmcsTrackDurationValueLen = 4;

static const uint32_t gmcsTrackPositionValue    = 0xFFFFFFFF;
static const uint16_t gmcsTrackPositionValueLen = 4;

#if LE_AUDIO_GMCS_PLAYBACK_SPEED
static const char     gmcsPlaybackSpeedValue    = 0;
static const uint16_t gmcsPlaybackSpeedValueLen = 1;
#endif

#if LE_AUDIO_GMCS_SEEKING_SPEED
static const char     gmcsSeekingSpeedValue    = 0;
static const uint16_t gmcsSeekingSpeedValueLen = 1;
#endif

#if LE_AUDIO_GMCS_CURRENT_TRACK_OBJECT_ID
static const uint8_t  gmcsCurrentTrackSegmentsObjectIDValue[6];
static const uint16_t gmcsCurrentTrackSegmentsObjectIDValueLen = 6;

static const uint8_t  gmcsCurrentTrackObjectIDValue[6];
static const uint16_t gmcsCurrentTrackObjectIDValueLen = 6;

static const uint8_t  gmcsNextTrackObjectIDValue[6];
static const uint16_t gmcsNextTrackObjectIDValueLen = 6;

static const uint8_t  gmcsParentGroupTrackObjectIDValue[6];
static const uint16_t gmcsParentGroupTrackObjectIDValueLen = 6;

static const uint8_t  gmcsCurrentGroupTrackObjectIDValue[6];
static const uint16_t gmcsCurrentGroupTrackObjectIDValueLen = 6;
#endif

#if LE_AUDIO_GMCS_PLAYING_ORDER
static const uint8_t  gmcsPlayingOrderValue    = 0x01;
static const uint16_t gmcsPlayingOrderValueLen = 1;
#endif

#if LE_AUDIO_GMCS_PLAYING_ORDERS_SUPPORTED
static const uint16_t gmcsPlayingOrderSupportedValue;
static const uint16_t gmcsPlayingOrderSupportedValueLen = 2;
#endif

static const uint8_t  gmcsMediaStateValue    = 0x00;
static const uint16_t gmcsMediaStateValueLen = 1;

#if LE_AUDIO_GMCS_MEDIA_CONTROL_POINT
static const uint8_t  gmcsMediaControlPointValue[5];
static const uint16_t gmcsMediaControlPointValueLen = 5;

static const uint32_t gmcsMediaControlPointSupportedValue;
static const uint16_t gmcsMediaControlPointSupportedValueLen = 4;
#endif

#if LE_AUDIO_GMCS_SEARCH_RESULTS_OBJECT_ID
static const uint8_t  gmcsSearchResultsObjectIDValue[6];
static const uint16_t gmcsSearchResultsObjectIDValueLen = 6;

static const uint8_t  gmcsSearchControlPointValue    = 0x00;
static const uint16_t gmcsSearchControlPointValueLen = 1;
#endif

static const uint8_t  gmcsCCIDValue    = 0x00;
static const uint16_t gmcsCCIDValueLen = sizeof(gmcsCCIDValue);

#if LE_AUDIO_GMCS_SUPPORT_OTS
extern const uint16_t otsIncludeValue[3];
#endif

/*
 * @brief the structure for default GMCS service List.
 */
static const struct atts_attribute gmcsList[] = {
    ATTS_PRIMARY_SERVICE(serviceGenericMediaControlUuid),

#if LE_AUDIO_GMCS_SUPPORT_OTS
    ATTS_INCLUDE_DEFINE(&otsIncludeValue),
#endif
    //Media Player Name
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicMediaPlayerNameUuid, gmcsMediaPlayerNameValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_OBJECT_ID
    //Media Player Icon Object ID
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropRead, characteristicMediaPlayerIconObjectIdUuid, gmcsMediaPlayerIconObjectIDValue),
#endif

#if LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_URL
    //Media Player Icon URL
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropRead, characteristicMediaPlayerIconUrlUuid, gmcsMediaPlayerIconURLValue),
#endif

    //Track Changed
    ATTS_CHAR_UUID_NOTIF_ONLY(characteristicTrackChangedUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Track Title
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicTrackTitleUuid, gmcsTrackTitleValue),
    ATTS_COMMON_CCC_DEFINE,

    //Track Duration
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicTrackDurationUuid, gmcsTrackDurationValue),
    ATTS_COMMON_CCC_DEFINE,

    //Track Position
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_RWCB(charPropReadWriteWriteWithoutNotify, characteristicTrackPositionUuid, gmcsTrackPositionValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GMCS_PLAYBACK_SPEED
    //Playback Speed
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_RWCB(charPropReadWriteWriteWithoutNotify, characteristicPlaybackSpeedUuid, gmcsPlaybackSpeedValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_GMCS_SEEKING_SPEED
    //Seeking Speed
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicSeekingSpeedUuid, gmcsSeekingSpeedValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_GMCS_CURRENT_TRACK_OBJECT_ID
    //Current Track Segments Object ID
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropRead, characteristicCurrentTrackSegmentsObjectIdUuid, gmcsCurrentTrackSegmentsObjectIDValue),

    //Current Track Object ID
    ATTS_CHAR_UUID_ENCR_RDWR_POINT_RWCB(charPropReadWriteWriteWithoutNotify, characteristicCurrentTrackObjectIdUuid, gmcsCurrentTrackObjectIDValue),
    ATTS_COMMON_CCC_DEFINE,

    //Next Track Object ID
    ATTS_CHAR_UUID_ENCR_RDWR_POINT_RWCB(charPropReadWriteWriteWithoutNotify, characteristicNextTrackObjectIdUuid, gmcsNextTrackObjectIDValue),
    ATTS_COMMON_CCC_DEFINE,

    //Parent Group Object ID
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicParentGroupObjectIdUuid, gmcsParentGroupTrackObjectIDValue),
    ATTS_COMMON_CCC_DEFINE,

    //Current Group Object ID
    ATTS_CHAR_UUID_ENCR_RDWR_POINT_RWCB(charPropReadWriteWriteWithoutNotify, characteristicCurrentGroupObjectIdUuid, gmcsCurrentGroupTrackObjectIDValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_GMCS_PLAYING_ORDER
    //Playing Order
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_RWCB(charPropReadWriteWriteWithoutNotify, characteristicPlayingOrderUuid, gmcsPlayingOrderValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_GMCS_PLAYING_ORDERS_SUPPORTED
    //Playing Order Supported
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropRead, characteristicPlayingOrdersSupportedUuid, gmcsPlayingOrderSupportedValue),
#endif

    //Media State
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicMediaStateUuid, gmcsMediaStateValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GMCS_MEDIA_CONTROL_POINT
    //Media Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_POINT_CB(charPropWriteWriteWithoutNotify, characteristicMediaControlPointUuid, gmcsMediaControlPointValue),
    ATTS_COMMON_CCC_DEFINE,

    //Media Control Point Supported
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicMediaCtrlPointOpSupportedUuid, gmcsMediaControlPointSupportedValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_GMCS_SEARCH_RESULTS_OBJECT_ID
    //Search Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_ENTITY_CB(charPropWriteWriteWithoutNotify, characteristicSearchControlPointUuid, gmcsSearchControlPointValue),
    ATTS_COMMON_CCC_DEFINE,

    //Search Results Object ID
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicSearchResultsObjectIdUuid, gmcsSearchResultsObjectIDValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

    //Content Control ID(CCID)
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropRead, characteristicContentControlIdUuid, gmcsCCIDValue),
};

/*
 * @brief the structure for default GMCS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcGmcsGroup = {
    NULL, gmcsList, NULL, NULL, GMCS_START_HDL, 0,
};

/**
 * @brief      for user add default GMCS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addGmcsGroup(void)
{
#if LE_AUDIO_GMCS_SUPPORT_OTS
    blc_svc_addOtsGroup();
#endif
    svcGmcsGroup.endHandle = svcGmcsGroup.startHandle + ARRAY_SIZE(gmcsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcGmcsGroup);
}

/**
 * @brief      for user remove default GMCS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeGmcsGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(GMCS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in GMCS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_gmcsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    blc_svc_otsCbackRegister(readCback, writeCback);
    svcGmcsGroup.readCallback  = readCback;
    svcGmcsGroup.writeCallback = writeCback;
}
