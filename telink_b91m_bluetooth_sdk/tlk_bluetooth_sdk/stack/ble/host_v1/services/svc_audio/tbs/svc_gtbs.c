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

#define GTBS_START_HDL SERVICE_GENERIC_TELEPHONE_BEARER_HDL

/*
 * @brief the structure for default GTBS service List.
 */
static const struct atts_attribute gtbsList[] = {
    ATTS_PRIMARY_SERVICE(serviceGenericTelephoneBearerAttUuid),

    //Bearer Provider Name
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicBearerProviderNameAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Bearer Uniform Caller Identifier(UCI)
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropRead, characteristicBearerUciAttUuid),

    //Bearer Technology
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicBearerTechnologyAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Bearer URI Schemes Supported List
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropRead, characteristicBearerUriSchemesSupportedListAttUuid),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GTBS_BEARER_SIGNAL_STRENGTH
    //Bearer Signal Strength
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicBearerSsAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Bearer Signal Strength Reporting Interval
    ATTS_CHAR_UUID_ENCR_RDWR_NULL(charPropReadWriteWriteWithout, characteristicBearerSsReportingIntervalAttUuid),
#endif

    //Bearer List Current Calls
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicBearerListCurrentCallsAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Content Control ID (CCID)
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropRead, characteristicContentControlIdAttUuid),

    //Status Flags
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicStatusFlagsAttUuid),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GTBS_INCOMING_CALL_TARGET_BEARER_URI
    //Incoming Call Target Bearer URI
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicIncomingCallTargetBearerUriAttUuid),
    ATTS_COMMON_CCC_DEFINE,
#endif

    //Call State
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicCallStateAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Call Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_NULL(charPropWriteWriteWithoutNotify, characteristicCallCtrlPointAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Call Control Point Optional Opcodes
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropRead, characteristicCallCtrlPointOptionalOpcodesAttUuid),

    //Termination Reason
    ATTS_CHAR_UUID_NOTIF_ONLY(characteristicTerminationReasonAttUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Incoming Call
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicIncomingCallAttUuid),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_GTBS_CALL_FRIENDLY_NAME
    //Call Friendly Name
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicCallFriendlyNameAttUuid),
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
