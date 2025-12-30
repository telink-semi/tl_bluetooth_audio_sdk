/********************************************************************************************************
 * @file    svc_ots.c
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

#define OTS_START_HDL SERVICE_OBJECT_TRANSFER_HDL

static const struct svc_ots_feature otsFeatureValue = {
    .OACP_feature = 0,
    .OLCP_feature = 0,
};
static const uint16_t otsFeatureValueLen = sizeof(otsFeatureValue);

static const uint8_t otsObjectNameValue[]  = "Telink-OTS-Service";
static const uint8_t otsObjectNameValueLen = sizeof(otsObjectNameValue) - 1;

static const uint16_t otsObjectTypeValue    = 0x0117; //TODO: unknown
static const uint8_t  otsObjectTypeValueLen = sizeof(otsObjectTypeValue);

static const struct svc_ots_object_size otsObjectSizeValue = {
    .currentSize   = 0,
    .allocatedSize = 100,
};
static const uint8_t otsObjectSizeValueLen = sizeof(otsObjectSizeValue);

static const struct svc_ots_universal_time otsObjectFirstCreatedValue = {
    .year   = 0x2023,
    .month  = 0x01,
    .day    = 0x00,
    .hour   = 0x00,
    .minute = 0x00,
    .second = 0x00,
};
static const uint8_t otsObjectFirstCreatedValueLen = sizeof(otsObjectFirstCreatedValue);

static const struct svc_ots_universal_time otsObjectLastModifiedValue;
static const uint8_t                       otsObjectLastModifiedValueLen = sizeof(otsObjectLastModifiedValue);

static const uint8_t otsObjectIDValue[]  = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t otsObjectIDValueLen = sizeof(otsObjectIDValue);

static const uint32_t otsObjectPropertiesValue;
static const uint8_t  otsObjectPropertiesValueLen = sizeof(otsObjectPropertiesValue);

static const uint8_t otsObjectListFilterValue;
static const uint8_t otsObjectListFilterValueLen = sizeof(otsObjectListFilterValue);

/*
 * @brief the structure for default OTS service List.
 */
static const struct atts_attribute otsList[] = {
    ATTS_SECONDARY_SERVICE(serviceObjectTransferUuid),

    //OTS-Feature
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropRead, characteristicOtsFeatureUuid, otsFeatureValue),

    //Object Name
    ATTS_CHAR_UUID_ENCR_RDWR_POINT_NOCB(charPropReadWrite, characteristicObjectNameUuid, otsObjectNameValue),

    //Object type
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropRead, characteristicObjectTypeUuid, otsObjectTypeValue),

    //Object Size
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(charPropRead, characteristicObjectSizeUuid, otsObjectSizeValue),

    //Object First-Created
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_NOCB(charPropReadWrite, characteristicObjectFirstCreatedUuid, otsObjectFirstCreatedValue),

    //Object Lats-Modified
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_NOCB(charPropReadWrite, characteristicObjectLastModifiedUuid, otsObjectLastModifiedValue),

    //Object ID
    ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(charPropRead, characteristicObjectIdUuid, otsObjectIDValue),

    //Object Properties
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_NOCB(charPropReadWrite, characteristicObjectPropertiesUuid, otsObjectPropertiesValue),

    //Object Action Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_NULL(charPropWriteIndicate, characteristicObjectActionControlPointUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Object List Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_NULL(charPropWriteIndicate, characteristicObjectListControlPointUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Object Changed
    ATTS_CHAR_UUID_INDICATE_ONLY(characteristicObjectListFilterUuid),
    ATTS_COMMON_CCC_DEFINE,

    //Object List Filter
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_NOCB(charPropReadWrite, characteristicObjectChangedUuid, otsObjectListFilterValue),
};

/*
 * @brief the structure for default OTS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcOtsGroup = {
    NULL, otsList, NULL, NULL, OTS_START_HDL, 0,
};

const uint16_t otsIncludeValue[3] = {OTS_START_HDL, OTS_START_HDL + ARRAY_SIZE(otsList) - 1, SERVICE_UUID_OBJECT_TRANSFER};

/**
 * @brief      for user add default OTS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addOtsGroup(void)
{
    svcOtsGroup.endHandle = svcOtsGroup.startHandle + ARRAY_SIZE(otsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcOtsGroup);
}

/**
 * @brief      for user remove default OTS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeOtsGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(OTS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in OTS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_otsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcOtsGroup.readCallback  = readCback;
    svcOtsGroup.writeCallback = writeCback;
}
