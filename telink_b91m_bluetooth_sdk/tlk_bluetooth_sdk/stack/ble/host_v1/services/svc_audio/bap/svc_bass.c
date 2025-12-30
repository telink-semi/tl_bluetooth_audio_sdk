/********************************************************************************************************
 * @file    svc_bass.c
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

#define BASS_START_HDL SERVICE_BROADCAST_AUDIO_SCAN_HDL

#ifndef LEA_BASS_RECV_STATE_CNT
#define LEA_BASS_RECV_STATE_CNT 1
#endif

#if LEA_BASS_RECV_STATE_CNT > 0
static const uint8_t  bassRecvState    = 0x00;
static const uint16_t bassRecvStateLen = 0;
#endif

/*
 * @brief the structure for default BASS service List.
 */
static const struct atts_attribute bassList[] = {
    ATTS_PRIMARY_SERVICE(serviceBroadcastAudioScanUuid),

    //Broadcast Audio Scan Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_NULL(charPropWriteWriteWithout, characteristicBasControlPointUuid),

#if LEA_BASS_RECV_STATE_CNT > 0
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicBroadcastReceiveStateUuid, bassRecvState),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LEA_BASS_RECV_STATE_CNT > 1
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicBroadcastReceiveStateUuid, bassRecvState),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LEA_BASS_RECV_STATE_CNT > 2
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicBroadcastReceiveStateUuid, bassRecvState),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LEA_BASS_RECV_STATE_CNT > 3
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicBroadcastReceiveStateUuid, bassRecvState),
    ATTS_COMMON_CCC_DEFINE,
#endif
};

/*
 * @brief the structure for default BASS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcBassGroup = {
    NULL, bassList, NULL, NULL, BASS_START_HDL, 0,
};

/**
 * @brief      for user add default BASS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addBassGroup(void)
{
    svcBassGroup.endHandle = svcBassGroup.startHandle + ARRAY_SIZE(bassList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcBassGroup);
}

/**
 * @brief      for user remove default BASS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeBassGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(BASS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in BASS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_bassCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcBassGroup.readCallback  = readCback;
    svcBassGroup.writeCallback = writeCback;
}

#if LEA_BASS_RECV_STATE_CNT > 4 || LEA_BASS_RECV_STATE_CNT < 1
#error "LEA_BASS_RECV_STATE_CNT is too large, please check the macro definition."
#endif
