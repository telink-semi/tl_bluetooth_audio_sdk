/********************************************************************************************************
 * @file    svc_tbs.c
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

#ifndef LE_AUDIO_TBS_BEARER_SIGNAL_STRENGTH
#define LE_AUDIO_TBS_BEARER_SIGNAL_STRENGTH 1
#endif

#ifndef LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI
#define LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI 1
#endif

#ifndef LE_AUDIO_TBS_CALL_FRIENDLY_NAME
#define LE_AUDIO_TBS_CALL_FRIENDLY_NAME 1
#endif

#ifndef LE_AUDIO_TBS_BEARER_PROVIDER_NAME_MAX_SIZE
#define LE_AUDIO_TBS_BEARER_PROVIDER_NAME_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_TBS_BEARER_UCI_MAX_SIZE
#define LE_AUDIO_TBS_BEARER_UCI_MAX_SIZE 16
#endif

#ifndef LE_AUDIO_TBS_BEARER_URI_SCHEMES_SUPPORTED_LIST_MAX_SIZE
#define LE_AUDIO_TBS_BEARER_URI_SCHEMES_SUPPORTED_LIST_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI_MAX_SIZE
#define LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_TBS_INCOMING_CALL_MAX_SIZE
#define LE_AUDIO_TBS_INCOMING_CALL_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_TBS_FRIENDLY_NAME_MAX_SIZE
#define LE_AUDIO_TBS_FRIENDLY_NAME_MAX_SIZE 64
#endif

#ifndef LE_AUDIO_TBS_BEARER_LIST_CURRENT_CALLS_MAX_SIZE
#define LE_AUDIO_TBS_BEARER_LIST_CURRENT_CALLS_MAX_SIZE 256
#endif

#ifndef LE_AUDIO_TBS_CALL_STATE_MAX_SIZE
#define LE_AUDIO_TBS_CALL_STATE_MAX_SIZE 64
#endif

#define TBS_START_HDL SERVICE_TELEPHONE_BEARER_HDL

_attribute_ble_data_retention_ static uint8_t  tbsBearerProviderNameValue[LE_AUDIO_TBS_BEARER_PROVIDER_NAME_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t tbsBearerProviderNameValueLen;
const uint16_t                                 tbsBearerProviderNameMaxSize = sizeof(tbsBearerProviderNameValue);

_attribute_ble_data_retention_ static uint8_t  tbsBearerUCIValue[LE_AUDIO_TBS_BEARER_UCI_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t tbsBearerUCIValueLen;
const uint16_t                                 tbsBearerUCIMaxSize = sizeof(tbsBearerUCIValue);

_attribute_ble_data_retention_ static uint8_t tbsBearerTechnologyValue    = 0x01;
static const uint16_t                         tbsBearerTechnologyValueLen = sizeof(tbsBearerTechnologyValue);

_attribute_ble_data_retention_ static uint8_t  tbsBearerURISchemesSupportedListValue[LE_AUDIO_TBS_BEARER_URI_SCHEMES_SUPPORTED_LIST_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t tbsBearerURISchemesSupportedListValueLen;
const uint16_t                                 tbsBearerURISchemesSupportedListMaxSize = sizeof(tbsBearerURISchemesSupportedListValue);

#if LE_AUDIO_TBS_BEARER_SIGNAL_STRENGTH
_attribute_ble_data_retention_ static uint8_t tbsBearerSignalStrengthValue;
static const uint16_t                         tbsBearerSignalStrengthValueLen = sizeof(tbsBearerSignalStrengthValue);

_attribute_ble_data_retention_ static uint8_t tbsBearerSignalStrengthReportingIntervalValue;
static const uint16_t                         tbsBearerSignalStrengthReportingIntervalValueLen = sizeof(tbsBearerSignalStrengthReportingIntervalValue);
#endif

_attribute_ble_data_retention_ static uint8_t  tbsBearerListCurrentCallsValue[LE_AUDIO_TBS_BEARER_LIST_CURRENT_CALLS_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t tbsBearerListCurrentCallsValueLen;
const uint16_t                                 tbsBearerListCurrentCallsMaxSize = sizeof(tbsBearerListCurrentCallsValue);

_attribute_ble_data_retention_ static uint8_t tbsCCIDValue;
static const uint16_t                         tbsCCIDValueLen = sizeof(tbsCCIDValue);

_attribute_ble_data_retention_ static uint8_t tbsStatusFlagsValue[2];
static const uint16_t                         tbsStatusFlagsValueLen = sizeof(tbsStatusFlagsValue);

const uint16_t tbsIncomingCallTargetBearerURIMaxSize = LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI_MAX_SIZE;
#if LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI
_attribute_ble_data_retention_ static uint8_t  tbsIncomingCallTargetBearerURIValue[LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t tbsIncomingCallTargetBearerURIValueLen;
#endif

_attribute_ble_data_retention_ static uint8_t  tbsCallStateValue[LE_AUDIO_TBS_CALL_STATE_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t tbsCallStateValueLen;
const uint16_t                                 tbsCallStateMaxSize = sizeof(tbsCallStateValue);

_attribute_ble_data_retention_ static uint8_t tbsCallControlPointValue[3];
static const uint16_t                         tbsCallControlPointValueLen = sizeof(tbsCallControlPointValue);

_attribute_ble_data_retention_ static uint16_t tbsCallControlPointOptionalOpcodesValue;
static const uint16_t                          tbsCallControlPointOptionalOpcodesValueLen = sizeof(tbsCallControlPointOptionalOpcodesValue);

const uint16_t                                 tbsIncomingCallMaxSize = LE_AUDIO_TBS_INCOMING_CALL_MAX_SIZE;
_attribute_ble_data_retention_ static uint8_t  tbsIncomingCallValue[LE_AUDIO_TBS_INCOMING_CALL_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t tbsIncomingCallValueLen;

const uint16_t tbsFriendlyNameMaxSize = LE_AUDIO_TBS_FRIENDLY_NAME_MAX_SIZE;
#if LE_AUDIO_TBS_CALL_FRIENDLY_NAME
_attribute_ble_data_retention_ static uint8_t  tbsCallFriendlyNameValue[LE_AUDIO_TBS_FRIENDLY_NAME_MAX_SIZE];
_attribute_ble_data_retention_ static uint16_t tbsCallFriendlyNameValueLen;
#endif

/*
 * @brief the structure for default TBS service List.
 */
static const struct atts_attribute tbsList[] = {
    ATTS_PRIMARY_SERVICE(serviceTelephoneBearerUuid),

    //Bearer Provider Name
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicBearerProviderNameUuid, tbsBearerProviderNameValue),
    ATTS_COMMON_CCC_DEFINE,

    //Bearer Uniform Caller Identifier(UCI)
    ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(charPropRead, characteristicBearerUciUuid, tbsBearerUCIValue),

    //Bearer Technology
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropReadNotify, characteristicBearerTechnologyUuid, tbsBearerTechnologyValue),
    ATTS_COMMON_CCC_DEFINE,

    //Bearer URI Schemes Supported List
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropRead, characteristicBearerUriSchemesSuppListUuid, tbsBearerURISchemesSupportedListValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_TBS_BEARER_SIGNAL_STRENGTH
    //Bearer Signal Strength
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropReadNotify, characteristicBearerSsUuid, tbsBearerSignalStrengthValue),
    ATTS_COMMON_CCC_DEFINE,

    //Bearer Signal Strength Reporting Interval
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_WCB(charPropReadWriteWriteWithout, characteristicBearerSsReportingIntervalUuid, tbsBearerSignalStrengthReportingIntervalValue),
#endif

    //Bearer List Current Calls
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicBearerListCurrentCallsUuid, tbsBearerListCurrentCallsValue),
    ATTS_COMMON_CCC_DEFINE,

    //Content Control ID (CCID)
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropRead, characteristicContentControlIdUuid, tbsCCIDValue),

    //Status Flags
    ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(charPropReadNotify, characteristicStatusFlagsUuid, tbsStatusFlagsValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_TBS_INCOMING_CALL_TARGET_BEARER_URI
    //Incoming Call Target Bearer URI
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicIncomingCallTargetBearerUriUuid, tbsIncomingCallTargetBearerURIValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

    //Call State
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicCallStateUuid, tbsCallStateValue),
    ATTS_COMMON_CCC_DEFINE,

    //Call Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_POINT_CB(charPropWriteWriteWithoutNotify, characteristicCallCtrlPointUuid, tbsCallControlPointValue),
    ATTS_COMMON_CCC_DEFINE,

    //Call Control Point Optional Opcodes
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropRead, characteristicCallCtrlPointOptionalOpcodesUuid, tbsCallControlPointOptionalOpcodesValue),

    //Termination Reason
    ATTS_CHAR_UUID_NOTIF_ONLY(characteristicTerminationReasonUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Incoming Call
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicIncomingCallUuid, tbsIncomingCallValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_TBS_CALL_FRIENDLY_NAME
    //Call Friendly Name
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicCallFriendlyNameUuid, tbsCallFriendlyNameValue),
    ATTS_COMMON_CCC_DEFINE,
#endif
};

/*
 * @brief the structure for default TBS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcTbsGroup = {
    NULL, tbsList, NULL, NULL, TBS_START_HDL, 0,
};

/**
 * @brief      for user add default TBS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addTbsGroup(void)
{
    svcTbsGroup.endHandle = svcTbsGroup.startHandle + ARRAY_SIZE(tbsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcTbsGroup);
}

/**
 * @brief      for user remove default TBS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeTbsGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(TBS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in TBS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_tbsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcTbsGroup.readCallback  = readCback;
    svcTbsGroup.writeCallback = writeCback;
}
