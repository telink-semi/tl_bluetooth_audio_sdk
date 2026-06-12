/********************************************************************************************************
 * @file    svc_has.c
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

#ifndef LE_AUDIO_HAS_HEARING_AID_PRESET_CONTROL_POINT
#define LE_AUDIO_HAS_HEARING_AID_PRESET_CONTROL_POINT 1
#endif

#define HAS_START_HDL SERVICE_HEARING_ACCESS_HDL

/*
 * @brief the structure for default HAS service List.
 */
static const struct atts_attribute hasList[] = {
    ATTS_PRIMARY_SERVICE(serviceHearingAccessAttUuid),

    //Hearing Aid Features
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicHearingAidFeaturesAttUuid),
    ATTS_COMMON_CCC_DEFINE_CB,

#if LE_AUDIO_HAS_HEARING_AID_PRESET_CONTROL_POINT
    //Hearing Aid Preset Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_NULL(charPropWriteIndicate, characteristicHearingAidPresetControlPointAttUuid),
    ATTS_COMMON_CCC_DEFINE_CB,

    //Active Preset Index
    ATTS_CHAR_UUID_ENCR_READ_NULL(charPropReadNotify, characteristicActivePresetIndexAttUuid),
    ATTS_COMMON_CCC_DEFINE_CB,
#endif
};

/*
 * @brief the structure for default HAS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcHasGroup = {
    NULL, hasList, NULL, NULL, HAS_START_HDL, 0,
};

/**
 * @brief      for user add default HAS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addHasGroup(void)
{
    svcHasGroup.endHandle = svcHasGroup.startHandle + ARRAY_SIZE(hasList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcHasGroup);
}

/**
 * @brief      for user remove default HAS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeHasGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(HAS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in HAS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_hasCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcHasGroup.readCallback  = readCback;
    svcHasGroup.writeCallback = writeCback;
}
