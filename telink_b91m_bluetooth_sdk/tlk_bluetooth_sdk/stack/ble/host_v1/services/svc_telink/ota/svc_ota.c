/********************************************************************************************************
 * @file    svc_ota.c
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
#include "../../../l2cap/att/inc/uuid128bit.h"

#include "../../inc/svc.h"
#include "../../inc/svc_format.h"

#include "../svc_telink.h"

#define OTA_START_HDL SERVICE_TELINK_OTA_HDL
//// OTA attribute values

static const uint8_t  otaData    = 0x00;
static const uint16_t otaDataLen = sizeof(otaData);

static const uint8_t  otaOutDesc[]  = {'O', 'T', 'A'};
static const uint16_t otaOutDescLen = sizeof(otaOutDesc);

/*
 * @brief the structure for default OTA(Telink) service List.
 */
static const struct atts_attribute otaList[] = {
    ATTS_PRIMARY_SERVICE_128(tlk_ota_service_att_uuid),

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropReadWriteWithoutNotify),
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_RDWR, tlk_ota_data_att_uuid, ATTS_SET_WRITE_CALLBACK, sizeof(otaData), &otaDataLen, &otaData),

    ATTS_COMMON_CCC_DEFINE,

    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_READ, descriptorCharacteristicUserDescriptionAttUuid, ATTS_SET_NONE, sizeof(otaOutDesc), &otaOutDescLen, otaOutDesc),
};

/*
 * @brief the structure for default OTA(Telink) service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcOtaGroup = {NULL, otaList, NULL, NULL, OTA_START_HDL, 0};

static int telinkOtaWrite(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len)
{
    (void)conn_handle;
    (void)opcode;
    (void)attr_handle;
    (void)value;
    (void)value_len;

    // uint8_t                    buff[517 + 6]; //Max MTU size
    // rf_packet_att_data_t *p = (rf_packet_att_data_t *) &buff[0];
    // p->att = opcode;
    // p->handle = attrHandle;
    // p->l2cap = valueLen + 3;
    // p->rf_len = valueLen + 7;
    // memcpy(p->dat, writeValue, valueLen);

    //	otaWrite(connHandle, p);
    return 0;
}

/**
 * @brief      for user add default OTA(Telink) service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addOtaGroup(void)
{
    //	blc_ota_initOtaServer_module();
    svcOtaGroup.endHandle     = svcOtaGroup.startHandle + ARRAY_SIZE(otaList) - 1;
    svcOtaGroup.writeCallback = telinkOtaWrite;
    blc_gatts_addAttributeServiceGroup(&svcOtaGroup);
}

/**
 * @brief      for user remove default OTA(Telink) service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeOtaGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(OTA_START_HDL);
}
