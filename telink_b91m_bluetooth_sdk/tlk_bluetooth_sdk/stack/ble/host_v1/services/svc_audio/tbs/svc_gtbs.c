/********************************************************************************************************
 * @file    svc_gtbs.c
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

#ifndef LE_AUDIO_GTBS_BEARER_SIGNAL_STRENGTH
#define LE_AUDIO_GTBS_BEARER_SIGNAL_STRENGTH 1
#endif

#ifndef LE_AUDIO_GTBS_INCOMING_CALL_TARGET_BEARER_URI
#define LE_AUDIO_GTBS_INCOMING_CALL_TARGET_BEARER_URI 1
#endif

#ifndef LE_AUDIO_GTBS_CALL_FRIENDLY_NAME
#define LE_AUDIO_GTBS_CALL_FRIENDLY_NAME 1
#endif

#ifndef LE_AUDIO_GTBS_BEARER_PROVIDER_NAME_MAX_SIZE
#define LE_AUDIO_GTBS_BEARER_PROVIDER_NAME_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_GTBS_BEARER_UCI_MAX_SIZE
#define LE_AUDIO_GTBS_BEARER_UCI_MAX_SIZE 16
#endif

#ifndef LE_AUDIO_GTBS_BEARER_URI_SCHEMES_SUPPORTED_LIST_MAX_SIZE
#define LE_AUDIO_GTBS_BEARER_URI_SCHEMES_SUPPORTED_LIST_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_GTBS_INCOMING_CALL_TARGET_BEARER_URI_MAX_SIZE
#define LE_AUDIO_GTBS_INCOMING_CALL_TARGET_BEARER_URI_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_GTBS_INCOMING_CALL_MAX_SIZE
#define LE_AUDIO_GTBS_INCOMING_CALL_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_GTBS_FRIENDLY_NAME_MAX_SIZE
#define LE_AUDIO_GTBS_FRIENDLY_NAME_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_GTBS_BEARER_LIST_CURRENT_CALLS_MAX_SIZE
#define LE_AUDIO_GTBS_BEARER_LIST_CURRENT_CALLS_MAX_SIZE 256
#endif

#ifndef LE_AUDIO_GTBS_CALL_STATE_MAX_SIZE
#define LE_AUDIO_GTBS_CALL_STATE_MAX_SIZE 64
#endif

#define GTBS_START_HDL SERVICE_GENERIC_TELEPHONE_BEARER_HDL

_attribute_ble_data_retention_ static uint8_t  gtbsBearerProviderNameValue[LE_AUDIO_GTBS_BEARER_PROVIDER_NAME_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t gtbsBearerProviderNameValueLen;
const uint16_t                                 gtbsBearerProviderNameMaxSize = sizeof(gtbsBearerProviderNameValue);

_attribute_ble_data_retention_ static uint8_t  gtbsBearerUCIValue[LE_AUDIO_GTBS_BEARER_UCI_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t gtbsBearerUCIValueLen;
const uint16_t                                 gtbsBearerUCIMaxSize = sizeof(gtbsBearerUCIValue);

_attribute_ble_data_retention_ static uint8_t gtbsBearerTechnologyValue    = 0x01;
static const uint16_t                         gtbsBearerTechnologyValueLen = sizeof(gtbsBearerTechnologyValue);

_attribute_ble_data_retention_ static uint8_t  gtbsBearerURISchemesSupportedListValue[LE_AUDIO_GTBS_BEARER_URI_SCHEMES_SUPPORTED_LIST_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t gtbsBearerURISchemesSupportedListValueLen;
const uint16_t                                 gtbsBearerURISchemesSupportedListMaxSize = sizeof(gtbsBearerURISchemesSupportedListValue);

#if LE_AUDIO_GTBS_BEARER_SIGNAL_STRENGTH
_attribute_ble_data_retention_ static uint8_t gtbsBearerSignalStrengthValue;
static const uint16_t                         gtbsBearerSignalStrengthValueLen = sizeof(gtbsBearerSignalStrengthValue);

_attribute_ble_data_retention_ static uint8_t gtbsBearerSignalStrengthReportingIntervalValue;
static const uint16_t                         gtbsBearerSignalStrengthReportingIntervalValueLen = sizeof(gtbsBearerSignalStrengthReportingIntervalValue);
#endif

_attribute_ble_data_retention_ static uint8_t  gtbsBearerListCurrentCallsValue[LE_AUDIO_GTBS_BEARER_LIST_CURRENT_CALLS_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t gtbsBearerListCurrentCallsValueLen;
const uint16_t                                 gtbsBearerListCurrentCallsMaxSize = sizeof(gtbsBearerListCurrentCallsValue);

_attribute_ble_data_retention_ static uint8_t gtbsCCIDValue;
static const uint16_t                         gtbsCCIDValueLen = sizeof(gtbsCCIDValue);

_attribute_ble_data_retention_ static uint8_t gtbsStatusFlagsValue[2];
static const uint16_t                         gtbsStatusFlagsValueLen                = sizeof(gtbsStatusFlagsValue);
const uint16_t                                gtbsIncomingCallTargetBearerURIMaxSize = LE_AUDIO_GTBS_INCOMING_CALL_TARGET_BEARER_URI_MAX_SIZE;

#if LE_AUDIO_GTBS_INCOMING_CALL_TARGET_BEARER_URI
_attribute_ble_data_retention_ static uint8_t  gtbsIncomingCallTargetBearerURIValue[LE_AUDIO_GTBS_INCOMING_CALL_TARGET_BEARER_URI_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t gtbsIncomingCallTargetBearerURIValueLen;
#endif

_attribute_ble_data_retention_ static uint8_t  gtbsCallStateValue[LE_AUDIO_GTBS_CALL_STATE_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t gtbsCallStateValueLen;
const uint16_t                                 gtbsCallStateMaxSize = sizeof(gtbsCallStateValue);

_attribute_ble_data_retention_ static uint8_t gtbsCallControlPointValue[3];
static const uint16_t                         gtbsCallControlPointValueLen = sizeof(gtbsCallControlPointValue);

_attribute_ble_data_retention_ static uint16_t gtbsCallControlPointOptionalOpcodesValue;
static const uint16_t                          gtbsCallControlPointOptionalOpcodesValueLen = sizeof(gtbsCallControlPointOptionalOpcodesValue);

const uint16_t                                 gtbsIncomingCallMaxSize = LE_AUDIO_GTBS_INCOMING_CALL_MAX_SIZE;
_attribute_ble_data_retention_ static uint8_t  gtbsIncomingCallValue[LE_AUDIO_GTBS_INCOMING_CALL_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t gtbsIncomingCallValueLen;

const uint16_t gtbsFriendlyNameMaxSize = LE_AUDIO_GTBS_FRIENDLY_NAME_MAX_SIZE;
#if LE_AUDIO_GTBS_CALL_FRIENDLY_NAME
_attribute_ble_data_retention_ static uint8_t  gtbsCallFriendlyNameValue[LE_AUDIO_GTBS_FRIENDLY_NAME_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t gtbsCallFriendlyNameValueLen;
#endif

/*
 * @brief the structure for default GTBS service List.
 */
static const struct atts_attribute gtbsList[] = {
    ATTS_PRIMARY_SERVICE(serviceGenericTelephoneBearerUuid),

    //Bearer Provider Name
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicBearerProviderNameUuid, gtbsBearerProviderNameValue),
    ATTS_COMMON_CCC_DEFINE,

    //Bearer Uniform Caller Identifier(UCI)
    ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(charPropRead, characteristicBearerUciUuid, gtbsBearerUCIValue),

    //Bearer Technology
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropReadNotify, characteristicBearerTechnologyUuid, gtbsBearerTechnologyValue),
    ATTS_COMMON_CCC_DEFINE,

    //Bearer URI Schemes Supported List
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropRead, characteristicBearerUriSchemesSuppListUuid, gtbsBearerURISchemesSupportedListValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GTBS_BEARER_SIGNAL_STRENGTH
    //Bearer Signal Strength
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropReadNotify, characteristicBearerSsUuid, gtbsBearerSignalStrengthValue),
    ATTS_COMMON_CCC_DEFINE,

    //Bearer Signal Strength Reporting Interval
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_WCB(charPropReadWriteWriteWithout, characteristicBearerSsReportingIntervalUuid, gtbsBearerSignalStrengthReportingIntervalValue),
#endif

    //Bearer List Current Calls
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicBearerListCurrentCallsUuid, gtbsBearerListCurrentCallsValue),
    ATTS_COMMON_CCC_DEFINE,

    //Content Control ID (CCID)
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropRead, characteristicContentControlIdUuid, gtbsCCIDValue),

    //Status Flags
    ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(charPropReadNotify, characteristicStatusFlagsUuid, gtbsStatusFlagsValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GTBS_INCOMING_CALL_TARGET_BEARER_URI
    //Incoming Call Target Bearer URI
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicIncomingCallTargetBearerUriUuid, gtbsIncomingCallTargetBearerURIValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

    //Call State
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicCallStateUuid, gtbsCallStateValue),
    ATTS_COMMON_CCC_DEFINE,

    //Call Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_POINT_CB(charPropWriteWriteWithoutNotify, characteristicCallCtrlPointUuid, gtbsCallControlPointValue),
    ATTS_COMMON_CCC_DEFINE,

    //Call Control Point Optional Opcodes
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropRead, characteristicCallCtrlPointOptionalOpcodesUuid, gtbsCallControlPointOptionalOpcodesValue),

    //Termination Reason
    ATTS_CHAR_UUID_NOTIF_ONLY(characteristicTerminationReasonUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Incoming Call
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicIncomingCallUuid, gtbsIncomingCallValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GTBS_CALL_FRIENDLY_NAME
    //Call Friendly Name
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicCallFriendlyNameUuid, gtbsCallFriendlyNameValue),
    ATTS_COMMON_CCC_DEFINE,
#endif
};

/*
 * @brief the structure for default GTBS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcGtbsGroup = {
    NULL, gtbsList, NULL, NULL, GTBS_START_HDL, 0,
};

/**
 * @brief      for user add default GTBS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addGtbsGroup(void)
{
    svcGtbsGroup.endHandle = svcGtbsGroup.startHandle + ARRAY_SIZE(gtbsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcGtbsGroup);
}

/**
 * @brief      for user remove default GTBS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeGtbsGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(GTBS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in GTBS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_gtbsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcGtbsGroup.readCallback  = readCback;
    svcGtbsGroup.writeCallback = writeCback;
}
