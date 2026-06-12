/********************************************************************************************************
 * @file    svc_ota_v2.c
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

#include "../../../l2cap/att/inc/ble_att.h"
#include "../../../l2cap/att/inc/ble_att_uuid.h"
#include "../../../l2cap/att/inc/ble_att_service.h"

#include "../../../l2cap/att/inc/uuid16bit.h"
#include "../../../l2cap/att/inc/uuid128bit.h"

#include "../../../gatt/gatts/inc/gatts_req.h"

#include "../../inc/svc.h"
#include "../../inc/svc_format.h"

#include "../svc_telink.h"
#include "tlkmw/tlkmw.h"
#include "stack/ble/host_v1/gap/acl/inc/ble_gap_acl_peripheral.h"

#define TLK_OTA_V2_START_HDL SERVICE_TLK_OTA2_HDL

static int blc_svc_tlkOtaV2WriteValueCallback(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len);

int blc_svc_tlkOtaV2_sendData(uint32_t taskID, uint8_t *data, uint16_t len, void *UserArg);

/*
 * @brief the structure for Telink SPP service List.
 */
static const struct atts_attribute tlkOtaV2List[] = {
    ATTS_PRIMARY_SERVICE_128(tlk_ota_v2_service_att_uuid),

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropWriteWriteWithoutNotify),
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_WRITE, tlk_ota_v2_data_att_uuid, ATTS_SET_WRITE_CALLBACK, 0, NULL, NULL),
    ATTS_COMMON_CCC_DEFINE,
};

/*
 * @brief the structure for Telink SPP service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcTlkOtaV2Group = {NULL, tlkOtaV2List, NULL, blc_svc_tlkOtaV2WriteValueCallback, TLK_OTA_V2_START_HDL, 0};

void blc_svc_otaStatusNotify(void *pData, uint8_t dataLen)
{
    (void)pData;
    (void)dataLen;
    if (dataLen < sizeof(sTlkMwNotifyEvent_t)) {
        return;
    }
    sTlkMwNotifyEvent_t *pEvent = (sTlkMwNotifyEvent_t *)pData;
    if (pEvent->status == TLK_FIRMWARE_OTA_ING) {
        ble_host_gap_acl_peripheral_disable_latency();
    } else if (pEvent->status == TLK_FIRMWARE_OTA_FAIL) {
        ble_host_gap_acl_peripheral_enable_latency();
    }
    tlk_printf("[OTA-STATUS] blc_svc_otaStatusNotify status[%d]", pEvent->status);
}

/**
 * @brief      for user add default OTA V2(Telink) service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addOtaV2Group(void)
{
    sTlkMwUnitIntf_t interface = {
        .channel       = TLKMW_OTA_TRANS_CHN_BLE_GENERAL_MODE,
        .send          = blc_svc_tlkOtaV2_sendData,
        .recv          = NULL,
        .get_ota_param = NULL,
    };

    tlkmw_ota_register_chn_interface(&interface);

    svcTlkOtaV2Group.endHandle = svcTlkOtaV2Group.startHandle + ARRAY_SIZE(tlkOtaV2List) - 1;
    blc_gatts_addAttributeServiceGroup(&svcTlkOtaV2Group);
    tlkmw_ota_register_notify_callback(TLKSYS_TASKID_HOST, blc_svc_otaStatusNotify);
}

/**
 * @brief      for user remove default OTA V2(Telink) service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeOtaV2Group(void)
{
    blc_gatts_removeAttributeServiceGroup(TLK_OTA_V2_START_HDL);
}

static int blc_svc_tlkOtaV2WriteValueCallback(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len)
{
    (void)opcode;
    (void)attr_handle;

    (void)conn_handle;
    (void)value;
    (void)value_len;

    tlkmw_userctrl_pushDataToTask(conn_handle, 0, value, value_len);

    return ATT_SUCCESS;
}

typedef struct
{
    uint32_t conn_handle;
    uint16_t len;
    uint16_t resv;
    uint8_t *data;
} blc_svc_ota_v2_pack_t;

void blc_svc_tlkOtaV2_sendData_inHost(void *pData, uint8_t dataLen)
{
    if (pData == NULL || dataLen < sizeof(blc_svc_ota_v2_pack_t)) {
        return;
    }

    blc_svc_ota_v2_pack_t *pack = (blc_svc_ota_v2_pack_t *)pData;

    ble_gatts_notify(pack->conn_handle & 0xFFFF, TLK_OTA_V2_START_HDL + 2, pack->data, pack->len);
}

int blc_svc_tlkOtaV2_sendData(uint32_t conn_handle, uint8_t *data, uint16_t len, void *UserArg)
{
    (void)UserArg;
    blc_svc_ota_v2_pack_t package = {.conn_handle = conn_handle, .len = len, .data = data};
    tlksys_runFuncInTaskWithArg(TLKSYS_TASKID_HOST, blc_svc_tlkOtaV2_sendData_inHost, &package, sizeof(blc_svc_ota_v2_pack_t));
    return TLK_ENONE;
}
