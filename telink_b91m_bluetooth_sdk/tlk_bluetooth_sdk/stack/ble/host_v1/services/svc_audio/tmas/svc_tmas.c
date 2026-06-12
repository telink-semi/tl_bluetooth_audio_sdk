/********************************************************************************************************
 * @file    svc_tmas.c
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

#include "svc_tmas.h"

#define TMAS_START_HDL SERVICE_TELEPHONE_AND_MEDIA_AUDIO_HFL

_attribute_ble_data_retention_ static uint16_t tmasTmapRoleValue    = 0x0000;
static const uint16_t                          tmasTmapRoleValueLen = sizeof(tmasTmapRoleValue);

/*
 * @brief the structure for default TMAS service List.
 */
static const struct atts_attribute tmasList[] = {
    ATTS_PRIMARY_SERVICE(serviceTelephonyAndMediaAudioAttUuid),

    //TMAP Role
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropRead, characteristicTmapRoleAttUuid, tmasTmapRoleValue),
};

/*
 * @brief the structure for default TMAS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcTmasGroup = {
    NULL, tmasList, NULL, NULL, TMAS_START_HDL, 0,
};

/**
 * @brief      for user add default TMAS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addTmasGroup(void)
{
    svcTmasGroup.endHandle = svcTmasGroup.startHandle + ARRAY_SIZE(tmasList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcTmasGroup);
}

/**
 * @brief      for user remove default TMAS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeTmasGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(TMAS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in TMAS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_tmasCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcTmasGroup.readCallback  = readCback;
    svcTmasGroup.writeCallback = writeCback;
}

/**
 * @brief      for user set the TMAP Role value.
 * @param[in]  value - the value of TMAP Role.
 * @return     none.
 */
void blc_tmas_setTmapRoleValue(uint16_t value)
{
    tmasTmapRoleValue = value & (~BLC_TMAP_ROLE_RFU);
}
