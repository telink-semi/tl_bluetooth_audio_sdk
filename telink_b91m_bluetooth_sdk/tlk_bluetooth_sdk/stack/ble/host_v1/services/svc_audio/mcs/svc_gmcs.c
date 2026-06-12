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

#if LE_AUDIO_GMCS_SUPPORT_OTS
extern const uint16_t otsIncludeValue[3];
#endif

/*
 * @brief the structure for default GMCS service List.
 */
static const struct atts_attribute gmcsList[] = {
    ATTS_PRIMARY_SERVICE(serviceGenericMediaControlAttUuid),

#if LE_AUDIO_GMCS_SUPPORT_OTS
    ATTS_INCLUDE_DEFINE(&otsIncludeValue),
#endif
    //Media Player Name
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicMediaPlayerNameAttUuid),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_OBJECT_ID
    //Media Player Icon Object ID
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropRead, characteristicMediaPlayerIconObjectIdAttUuid),
#endif

#if LE_AUDIO_GMCS_MEDIA_PLAYER_ICON_URL
    //Media Player Icon URL
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropRead, characteristicMediaPlayerIconUrlAttUuid),
#endif

    //Track Changed
    ATTS_CHAR_UUID_NOTIF_ONLY(characteristicTrackChangedAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Track Title
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicTrackTitleAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Track Duration
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicTrackDurationAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Track Position
    ATTS_CHAR_UUID_ENCR_RDWR_NULL(charPropReadWriteWriteWithoutNotify, characteristicTrackPositionAttUuid),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GMCS_PLAYBACK_SPEED
    //Playback Speed
    ATTS_CHAR_UUID_ENCR_RDWR_NULL(charPropReadWriteWriteWithoutNotify, characteristicPlaybackSpeedAttUuid),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_GMCS_SEEKING_SPEED
    //Seeking Speed
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicSeekingSpeedAttUuid),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_GMCS_CURRENT_TRACK_OBJECT_ID
    //Current Track Segments Object ID
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropRead, characteristicCurrentTrackSegmentsObjectIdAttUuid),

    //Current Track Object ID
    ATTS_CHAR_UUID_ENCR_RDWR_NULL(charPropReadWriteWriteWithoutNotify, characteristicCurrentTrackObjectIdAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Next Track Object ID
    ATTS_CHAR_UUID_ENCR_RDWR_NULL(charPropReadWriteWriteWithoutNotify, characteristicNextTrackObjectIdAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Parent Group Object ID
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicParentGroupObjectIdAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Current Group Object ID
    ATTS_CHAR_UUID_ENCR_RDWR_NULL(charPropReadWriteWriteWithoutNotify, characteristicCurrentGroupObjectIdAttUuid),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_GMCS_PLAYING_ORDER
    //Playing Order
    ATTS_CHAR_UUID_ENCR_RDWR_NULL(charPropReadWriteWriteWithoutNotify, characteristicPlayingOrderAttUuid),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_GMCS_PLAYING_ORDERS_SUPPORTED
    //Playing Order Supported
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropRead, characteristicPlayingOrdersSupportedAttUuid),
#endif

    //Media State
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicMediaStateAttUuid),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GMCS_MEDIA_CONTROL_POINT
    //Media Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_NULL(charPropWriteWriteWithoutNotify, characteristicMediaControlPointAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Media Control Point Supported
    ATTS_CHAR_UUID_READ_NULL(charPropReadNotify, characteristicMediaCtrlPointOpSupportedAttUuid),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_GMCS_SEARCH_RESULTS_OBJECT_ID
    //Search Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_NULL(charPropWriteWriteWithoutNotify, characteristicSearchControlPointAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Search Results Object ID
    ATTS_CHAR_UUID_READ_NULL(charPropReadNotify, characteristicSearchResultsObjectIdAttUuid),
    ATTS_COMMON_CCC_DEFINE,
#endif

    //Content Control ID(CCID)
    ATTS_CHAR_UUID_READ_NULL(charPropRead, characteristicContentControlIdAttUuid),
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
