/********************************************************************************************************
 * @file    svc_scps.c
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

#include "../svc_gatt.h"

#include "vendor/common/user_config.h"

#define SCPS_START_HDL SERVICE_SCAN_PARAMETERS_HDL

/*
  * @brief the structure for default ScPS service List.
  */
static const struct atts_attribute scpsList[] = {
    ATTS_PRIMARY_SERVICE(serviceScanParametersAttUuid),

    //Scan Interval Window
    ATTS_CHAR_UUID_WRITE_NULL(charPropWriteWithout, characteristicScanIntervalWindowAttUuid),

#ifndef SCPS_SCAN_REFRESH_DISABLE
    //Scan Refresh
    ATTS_CHAR_UUID_NOTIF_ONLY(characteristicScanRefreshAttUuid),
    ATTS_COMMON_CCC_DEFINE,
#endif
};

/*
 * @brief the structure for default ScPS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcScpsGroup = {
    NULL, scpsList, NULL, NULL, SCPS_START_HDL, 0,
};

/**
 * @brief      for user add default ScPS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addScpsGroup(void)
{
    svcScpsGroup.endHandle = svcScpsGroup.startHandle + ARRAY_SIZE(scpsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcScpsGroup);
}

/**
 * @brief      for user remove default ScPS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeScpsGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(SCPS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in ScPS service.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_scpsCbackRegister(atts_w_cb_t writeCback)
{
    svcScpsGroup.writeCallback = writeCback;
}
