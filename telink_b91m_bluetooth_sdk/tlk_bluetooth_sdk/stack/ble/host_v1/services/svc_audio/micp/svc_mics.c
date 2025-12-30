/********************************************************************************************************
 * @file    svc_mics.c
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

#define APP_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM (0)

#define MICS_START_HDL                         SERVICE_MICROPHONE_CONTROL_HDL

_attribute_ble_data_retention_ static uint8_t micsMuteValue    = 0x00;
static const uint16_t                         micsMuteValueLen = 1;

extern const uint16_t aicsIncludeValue[APP_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM][3];

/*
 * @brief the structure for default MICS service List.
 */
static const struct atts_attribute micsList[] = {
    ATTS_PRIMARY_SERVICE(serviceMicrophoneControlUuid),

#if APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM > 0
    ATTS_INCLUDE_DEFINE(&aicsIncludeValue[0][0]),
#endif
#if APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM > 1
    ATTS_INCLUDE_DEFINE(&aicsIncludeValue[1][0]),
#endif
#if APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM > 2
    ATTS_INCLUDE_DEFINE(&aicsIncludeValue[2][0]),
#endif
#if APP_AUDIO_MICS_INCLUDE_AICS_INSTANCE_NUM > 3
    ATTS_INCLUDE_DEFINE(&aicsIncludeValue[3][0]),
#endif

    //Mute
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_WCB(charPropReadWriteNotify, characteristicMuteUuid, micsMuteValue),
    ATTS_COMMON_CCC_DEFINE,
};

/*
 * @brief the structure for default MICS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcMicsGroup = {
    NULL, micsList, NULL, NULL, MICS_START_HDL, 0,
};

/**
 * @brief      for user add default MICS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */

void blc_svc_addMicsGroup(void)
{
    svcMicsGroup.endHandle = svcMicsGroup.startHandle + ARRAY_SIZE(micsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcMicsGroup);
}

/**
 * @brief      for user remove default MICS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeMicsGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(MICS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in MICS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_micsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcMicsGroup.readCallback  = readCback;
    svcMicsGroup.writeCallback = writeCback;
}
