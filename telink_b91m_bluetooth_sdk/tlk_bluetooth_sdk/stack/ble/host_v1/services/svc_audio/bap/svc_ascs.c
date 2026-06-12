/********************************************************************************************************
 * @file    svc_ascs.c
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

#include "vendor/common/user_config.h"

#ifndef APP_AUDIO_ASCSS_SINK_ASE_CNT
#define APP_AUDIO_ASCSS_SINK_ASE_CNT 1
#endif

#ifndef APP_AUDIO_ASCSS_SINK_ASE_ID
#define APP_AUDIO_ASCSS_SINK_ASE_ID 1
#endif

#ifndef APP_AUDIO_ASCSS_SRC_ASE_CNT
#define APP_AUDIO_ASCSS_SRC_ASE_CNT 1
#endif

#ifndef APP_AUDIO_ASCSS_SRC_ASE_ID
#define APP_AUDIO_ASCSS_SRC_ASE_ID 0x10
#endif

#define ASCS_START_HDL SERVICE_AUDIO_STREAM_CONTROL_HDL

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 0
static const uint8_t ascsSinkASEValue[APP_AUDIO_ASCSS_SINK_ASE_CNT][2] = {
    {APP_AUDIO_ASCSS_SINK_ASE_ID + 0, 0x00}, //idle
#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 1
    {APP_AUDIO_ASCSS_SINK_ASE_ID + 1, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 2
    {APP_AUDIO_ASCSS_SINK_ASE_ID + 2, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 3
    {APP_AUDIO_ASCSS_SINK_ASE_ID + 3, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 4
    {APP_AUDIO_ASCSS_SINK_ASE_ID + 4, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 5
    {APP_AUDIO_ASCSS_SINK_ASE_ID + 5, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 6
    {APP_AUDIO_ASCSS_SINK_ASE_ID + 6, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 7
    {APP_AUDIO_ASCSS_SINK_ASE_ID + 7, 0x00}, //idle
#endif
};
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 0
static const uint8_t ascsSourceASEValue[APP_AUDIO_ASCSS_SRC_ASE_CNT][2] = {
    {APP_AUDIO_ASCSS_SRC_ASE_ID + 0, 0x00}, //idle
#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 1
    {APP_AUDIO_ASCSS_SRC_ASE_ID + 1, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 2
    {APP_AUDIO_ASCSS_SRC_ASE_ID + 2, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 3
    {APP_AUDIO_ASCSS_SRC_ASE_ID + 3, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 4
    {APP_AUDIO_ASCSS_SRC_ASE_ID + 4, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 5
    {APP_AUDIO_ASCSS_SRC_ASE_ID + 5, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 6
    {APP_AUDIO_ASCSS_SRC_ASE_ID + 6, 0x00}, //idle
#endif
#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 7
    {APP_AUDIO_ASCSS_SRC_ASE_ID + 7, 0x00}, //idle
#endif
};
#endif

#if (APP_AUDIO_ASCSS_SINK_ASE_CNT + APP_AUDIO_ASCSS_SOURCE_ASE_CNT) > 0
static const uint16_t ascsASEValueLen = 2;
#endif

#define ATTS_CHAR_ASCS_ASE_UUID(uuid, value) \
    ATTS_CHARACTERISTIC_DECLARATIONS(charPropReadNotify), ATTS_CHAR_UUID_DEFINE(ATT_PERMISSIONS_ENCRYPT_READ, uuid, ascsASEValueLen, 2, value, ATTS_SET_READ_CALLBACK)

#define ATTS_CHAR_ASCS_SINK_ASE(value) ATTS_CHAR_ASCS_ASE_UUID(characteristicSinkAseAttUuid, value)
#define ATTS_CHAR_ASCS_SRC_ASE(value)  ATTS_CHAR_ASCS_ASE_UUID(characteristicSourceAseAttUuid, value)

/*
 * @brief the structure for default ASCS service List.
 */
static const struct atts_attribute ascsList[] = {
    ATTS_PRIMARY_SERVICE(serviceAudioStreamControlAttUuid),

    //ASE Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_NULL(charPropWriteWriteWithoutNotify, characteristicAseControlPointAttUuid),
    ATTS_COMMON_CCC_DEFINE_WCB,

//Sink ASE
#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 0
    ATTS_CHAR_ASCS_SINK_ASE(&ascsSinkASEValue[0][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 1
    ATTS_CHAR_ASCS_SINK_ASE(&ascsSinkASEValue[1][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 2
    ATTS_CHAR_ASCS_SINK_ASE(&ascsSinkASEValue[2][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 3
    ATTS_CHAR_ASCS_SINK_ASE(&ascsSinkASEValue[3][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 4
    ATTS_CHAR_ASCS_SINK_ASE(&ascsSinkASEValue[4][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 5
    ATTS_CHAR_ASCS_SINK_ASE(&ascsSinkASEValue[5][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 6
    ATTS_CHAR_ASCS_SINK_ASE(&ascsSinkASEValue[6][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SINK_ASE_CNT > 7
    ATTS_CHAR_ASCS_SINK_ASE(&ascsSinkASEValue[7][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

//Source ASE
#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 0
    ATTS_CHAR_ASCS_SRC_ASE(&ascsSourceASEValue[0][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 1
    ATTS_CHAR_ASCS_SRC_ASE(&ascsSourceASEValue[1][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 2
    ATTS_CHAR_ASCS_SRC_ASE(&ascsSourceASEValue[2][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 3
    ATTS_CHAR_ASCS_SRC_ASE(&ascsSourceASEValue[3][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 4
    ATTS_CHAR_ASCS_SRC_ASE(&ascsSourceASEValue[4][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 5
    ATTS_CHAR_ASCS_SRC_ASE(&ascsSourceASEValue[5][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 6
    ATTS_CHAR_ASCS_SRC_ASE(&ascsSourceASEValue[6][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if APP_AUDIO_ASCSS_SRC_ASE_CNT > 7
    ATTS_CHAR_ASCS_SRC_ASE(&ascsSourceASEValue[7][0]),
    ATTS_COMMON_CCC_DEFINE,
#endif

};

/*
 * @brief the structure for default ASCS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcAscsGroup = {
    NULL, ascsList, NULL, NULL, ASCS_START_HDL, 0,
};

/**
 * @brief      for user add default ASCS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addAscsGroup(void)
{
    svcAscsGroup.endHandle = svcAscsGroup.startHandle + ARRAY_SIZE(ascsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcAscsGroup);
}

/**
 * @brief      for user remove default ASCS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeAscsGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(ASCS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in ASCS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_ascsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcAscsGroup.readCallback  = readCback;
    svcAscsGroup.writeCallback = writeCback;
}
