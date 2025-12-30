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

#define TLK_OTA_V2_START_HDL SERVICE_TLK_OTA2_HDL

static int blc_svc_tlkOtaV2WriteValueCallback(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len);

static const uint8_t serviceTlkOtaV2Uuid[16] = {TELINK_OTA_V2_UUID_SERVICE};

static const uint8_t tlkSppOtaV2CharacteristicUuid[16] = {TELINK_SPP_DATA_OTA_V2};

int blc_svc_tlkOtaV2_sendData(uint32_t taskID, uint8_t *data, uint16_t len, void *UserArg);

/*
 * @brief the structure for Telink SPP service List.
 */
static const struct atts_attribute tlkOtaV2List[] = {
    ATTS_PRIMARY_SERVICE_128(serviceTlkOtaV2Uuid),

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropWriteWriteWithoutNotify),
    {
        ATT_PERMISSIONS_WRITE,
        ATT_128_UUID_LEN,
        (uint8_t *)(size_t)&tlkSppOtaV2CharacteristicUuid[0],
        NULL,
        0,
        NULL,
        ATTS_SET_WRITE_CALLBACK,
    },
    ATTS_COMMON_CCC_DEFINE,
};

/*
 * @brief the structure for Telink SPP service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcTlkOtaV2Group = {NULL, tlkOtaV2List, NULL, blc_svc_tlkOtaV2WriteValueCallback, TLK_OTA_V2_START_HDL, 0};

/**
 * @brief      for user add default OTA V2(Telink) service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addOtaV2Group(void)
{
    tlkmw_ota_register_chn_send_interface(TLKMW_OTA_TRANS_CHN_BLE_GENERAL_MODE, blc_svc_tlkOtaV2_sendData);

    svcTlkOtaV2Group.endHandle = svcTlkOtaV2Group.startHandle + ARRAY_SIZE(tlkOtaV2List) - 1;
    blc_gatts_addAttributeServiceGroup(&svcTlkOtaV2Group);
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

// TODO: yating, application ota start/failure, need to handle here
// ble_host_gap_acl_peripheral_disable_latency or ble_host_gap_acl_peripheral_enable_latency
static bool ble_ota_flag = false;
#include "stack/ble/host_v1/gap/acl/inc/ble_gap_acl_peripheral.h"

static int blc_svc_tlkOtaV2WriteValueCallback(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len)
{
    (void)opcode;
    (void)attr_handle;

    (void)conn_handle;
    (void)value;
    (void)value_len;

    // TODO: yating
    if (ble_ota_flag == false) {
        ble_ota_flag = true;
        //TEMP CODE TODO: YA TING
        tlksys_pm_setChn(TLKSYS_PM_CHN_OTA, 0, 1);
#if PROJ_RECORDING_CARD && MCU_CORE_TYPE == MCU_CORE_TL721X && TLK_CFG_RTOS_ENABLE
        void tlkmdi_ble_set_thread_loop_once_period(uint32_t ms);
        tlkmdi_ble_set_thread_loop_once_period(5);
#endif
        ble_host_gap_acl_peripheral_disable_latency();
    }

    tlkmw_userctrl_pushDataToTask(conn_handle, value, value_len);

    return ATT_SUCCESS;
}

int blc_svc_tlkOtaV2_sendData(uint32_t conn_handle, uint8_t *data, uint16_t len, void *UserArg)
{
    (void)UserArg;
    return ble_gatts_notify(conn_handle & 0xFFFF, TLK_OTA_V2_START_HDL + 2, data, len);
}
