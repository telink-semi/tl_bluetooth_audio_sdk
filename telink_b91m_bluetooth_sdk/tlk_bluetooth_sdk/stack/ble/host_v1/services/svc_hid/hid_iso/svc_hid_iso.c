/********************************************************************************************************
 * @file    svc_hid_iso.c
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

#include "../svc_hid.h"

#define ULL_HID_START_HDL SERVICE_ULTRA_LOW_LATENCY_HID_HDL


_attribute_ble_data_retention_ static uint8_t  ullhidProperties[1];
_attribute_ble_data_retention_ static uint16_t ullhidPropertiesLen = 0;

/*
 * @brief the structure for default ULL-HID service List.
 */
static const struct atts_attribute ullhidList[] = {
    ATTS_PRIMARY_SERVICE(serviceUllhidUuid),

    //ULL HID Properties
    ATTS_CHAR_UUID_READ_POINT_CB(charPropRead, characteristicUllHidPropertiesUuid, ullhidProperties),

    //LE HID Operation
    ATTS_CHAR_UUID_WRITE_NULL(charPropWriteIndicate, characteristicLeHidOperationModeUuid),
    ATTS_COMMON_CCC_DEFINE_CB,
};

/*
 * @brief the structure for default ULL-HID service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcUllhidGroup = {
    NULL, ullhidList, NULL, NULL, ULL_HID_START_HDL, 0,
};

/**
 * @brief      for user add default ULL-HID service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addUllhidGroup(void)
{
    svcUllhidGroup.endHandle = svcUllhidGroup.startHandle + ARRAY_SIZE(ullhidList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcUllhidGroup);
}

/**
 * @brief      for user remove default ULL-HID service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeUllhidGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(ULL_HID_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in ULL-HID service.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_ullhidCbackRegister(atts_w_cb_t writeCback, atts_r_cb_t readCback)
{
    svcUllhidGroup.writeCallback = writeCback;
    svcUllhidGroup.readCallback  = readCback;
}
