/********************************************************************************************************
 * @file    svc_csis.c
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

#include "svc_csis.h"

#ifndef LE_AUDIO_CSIS_COORDINATED_SET_SIZE
#define LE_AUDIO_CSIS_COORDINATED_SET_SIZE 1
#endif

#ifndef LE_AUDIO_CSIS_SET_MEMBER_LOCK
#define LE_AUDIO_CSIS_SET_MEMBER_LOCK 1
#endif

#ifndef LE_AUDIO_CSIS_SET_MEMBER_RANK
#define LE_AUDIO_CSIS_SET_MEMBER_RANK 1
#endif

#ifndef LE_AUDIO_CSIS_COORDINATED_SET_NAME
#define LE_AUDIO_CSIS_COORDINATED_SET_NAME 1
#endif

#define CSIS_START_HDL SERVICE_COORDINATED_SET_IDENTIFICATION_HDL

static const uint8_t  csisSIRKValue[17] = {0x00};
static const uint16_t csisSIRKValueLen  = sizeof(csisSIRKValue);

#if LE_AUDIO_CSIS_COORDINATED_SET_SIZE
static const uint8_t  csisCSSizeValue    = 0x01;
static const uint16_t csisCSSizeValueLen = 1;
#endif

#if LE_AUDIO_CSIS_SET_MEMBER_LOCK
static const uint8_t  csisSetMemberLockValue    = 0x01;
static const uint16_t csisSetMemberLockValueLen = 1;
#endif

#if LE_AUDIO_CSIS_SET_MEMBER_RANK
static const uint8_t  csisSetMemberRankValue;
static const uint16_t csisSetMemberRankValueLen = 1;
#endif

#if LE_AUDIO_CSIS_COORDINATED_SET_NAME
static const uint8_t  csisCoordinatedSetNameValue[]  = "Telink CSIS";
static const uint16_t csisCoordinatedSetNameValueLen = sizeof(csisCoordinatedSetNameValue) - 1;
#endif

/*
 * @brief the structure for default CSIS service List.
 */
static const struct atts_attribute csisList[] = {
    ATTS_PRIMARY_SERVICE(serviceCoordinatedSetIdentificationAttUuid),

    //Set Identity Resolving Key
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicSetIdentityResolvingKeyAttUuid, csisSIRKValue),
    ATTS_COMMON_CCC_DEFINE,

#if LE_AUDIO_CSIS_COORDINATED_SET_SIZE
    //Coordinated Set Size
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicCoordinatedSetSizeAttUuid, csisCSSizeValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_CSIS_SET_MEMBER_LOCK
    //Set Member Lock
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_RWCB(charPropReadWriteNotify, characteristicSetMemberLockAttUuid, csisSetMemberLockValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_CSIS_SET_MEMBER_RANK
    //Set Member Rank
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropRead, characteristicSetMemberRankAttUuid, csisSetMemberRankValue),
#endif

#if LE_AUDIO_CSIS_COORDINATED_SET_NAME
    //Coordinated Set Name
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicCoordinatedSetNameAttUuid, csisCoordinatedSetNameValue),
#endif

};

/*
 * @brief the structure for default CSIS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcCsisGroup = {
    NULL, csisList, NULL, NULL, CSIS_START_HDL, 0,
};

const uint16_t csisIncludeValue[3] = {CSIS_START_HDL, CSIS_START_HDL + ARRAY_SIZE(csisList) - 1, SERVICE_UUID_COORDINATED_SET_IDENTIFICATION};

/**
 * @brief      for user add default CSIS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addCsisGroup(void)
{
    svcCsisGroup.endHandle = svcCsisGroup.startHandle + ARRAY_SIZE(csisList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcCsisGroup);
}

/**
 * @brief      for user remove default CSIS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeCsisGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(SERVICE_COORDINATED_SET_IDENTIFICATION_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in CSIS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_csisCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcCsisGroup.readCallback  = readCback;
    svcCsisGroup.writeCallback = writeCback;
}

#if LE_AUDIO_CSIS_SET_MEMBER_LOCK && (!LE_AUDIO_CSIS_SET_MEMBER_RANK)
#error "ERR:CSIS attribute table fail"
#endif
