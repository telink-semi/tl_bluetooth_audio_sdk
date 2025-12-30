/********************************************************************************************************
 * @file    svc_spp.c
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

#define TLK_SPP_START_HDL SERVICE_TLK_SPP_HDL

static const uint8_t serviceTlkSppUuid[16] = {TELINK_SPP_UUID_SERVICE};

static const uint8_t tlkSppServer2ClientUuid[16] = {TELINK_SPP_DATA_SERVER2CLIENT};
static const uint8_t tlkSppClient2ServerUuid[16] = {TELINK_SPP_DATA_CLIENT2SERVER};

/*
 * @brief the structure for Telink SPP service List.
 */
static const struct atts_attribute tlkSppList[] = {
    ATTS_PRIMARY_SERVICE_128(serviceTlkSppUuid),

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropNotify),
    {
        ATT_PERMISSIONS_NONE,
        ATT_128_UUID_LEN,
        (uint8_t *)(size_t)&tlkSppServer2ClientUuid[0],
        NULL,
        0,
        NULL,
        0,
    },
    ATTS_COMMON_CCC_DEFINE,

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropWriteWithout),
    {ATT_PERMISSIONS_WRITE, ATT_128_UUID_LEN, (uint8_t *)(size_t)&tlkSppClient2ServerUuid[0], NULL, 0, NULL, ATTS_SET_WRITE_CALLBACK},
};

/*
 * @brief the structure for Telink SPP service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcTlkSppGroup = {NULL, tlkSppList, NULL, NULL, TLK_SPP_START_HDL, 0};

/**
 * @brief      for user add Telink SPP service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addTlkSppGroup(void)
{
    svcTlkSppGroup.endHandle = svcTlkSppGroup.startHandle + ARRAY_SIZE(tlkSppList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcTlkSppGroup);
}

/**
 * @brief      for user remove Telink SPP service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeTlkSppGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(TLK_SPP_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in Telink SPP service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_tlkSppCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcTlkSppGroup.readCallback  = readCback;
    svcTlkSppGroup.writeCallback = writeCback;
}
