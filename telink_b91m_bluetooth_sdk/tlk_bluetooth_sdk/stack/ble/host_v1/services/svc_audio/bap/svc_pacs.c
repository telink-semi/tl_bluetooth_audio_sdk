/********************************************************************************************************
 * @file    svc_pacs.c
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

#ifndef LE_AUDIO_PACS_SINK_PAC
#define LE_AUDIO_PACS_SINK_PAC 1
#endif

#ifndef LE_AUDIO_PACS_SINK_AUDIO_LOCATIONS
#define LE_AUDIO_PACS_SINK_AUDIO_LOCATIONS 1
#endif

#ifndef LE_AUDIO_PACS_SOURCE_PAC
#define LE_AUDIO_PACS_SOURCE_PAC 1
#endif

#ifndef LE_AUDIO_PACS_SOURCE_AUDIO_LOCATIONS
#define LE_AUDIO_PACS_SOURCE_AUDIO_LOCATIONS 1
#endif

#define PACS_START_HDL SERVICE_PUBLISHED_AUDIO_CAPABILITIES_HDL

#if LE_AUDIO_PACS_SINK_PAC
static const uint8_t  defaultSinkPac[]  = {0x00};
static const uint16_t defaultSinkPacLen = sizeof(defaultSinkPac);
#endif

#if LE_AUDIO_PACS_SINK_AUDIO_LOCATIONS
static const uint32_t defaultSinkAudioLocation    = 0x00000000;
static const uint16_t defaultSinkAudioLocationLen = sizeof(defaultSinkAudioLocation);
#endif

#if LE_AUDIO_PACS_SOURCE_PAC
static const uint8_t  defaultSourcePac[]  = {0x00};
static const uint16_t defaultSourcePacLen = sizeof(defaultSourcePac);
#endif

#if LE_AUDIO_PACS_SOURCE_AUDIO_LOCATIONS
static const uint32_t defaultSourceAudioLocation    = 0x00000000;
static const uint16_t defaultSourceAudioLocationLen = sizeof(defaultSourceAudioLocation);
#endif

static const uint32_t defaultAvaAudioContext     = 0x00000000;
static const uint16_t defaultAvaAudioContextLen  = sizeof(defaultAvaAudioContext);
static const uint32_t defaultSuppAudioContext    = 0x00000000;
static const uint16_t defaultSuppAudioContextLen = sizeof(defaultSuppAudioContext);

/*
 * @brief the structure for default PACS service List.
 */
static const struct atts_attribute pacsList[] = {
    ATTS_PRIMARY_SERVICE(servicePublishedAudioCapabilitiesAttUuid),
#if LE_AUDIO_PACS_SINK_PAC
    //Sink PAC
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicSinkPacAttUuid, defaultSinkPac),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_PACS_SINK_AUDIO_LOCATIONS
    //Sink Audio Locations
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_RWCB(charPropReadWriteNotify, characteristicSinkAudioLocationsAttUuid, defaultSinkAudioLocation),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_PACS_SOURCE_PAC
    //Source PAC
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicSourcePacAttUuid, defaultSourcePac),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LE_AUDIO_PACS_SOURCE_AUDIO_LOCATIONS
    //Source Audio Locations
    ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_RWCB(charPropReadWriteNotify, characteristicSourceAudioLocationsAttUuid, defaultSourceAudioLocation),
    ATTS_COMMON_CCC_DEFINE,
#endif

    //Available Audio Contexts
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicAvailableAudioContextsAttUuid, defaultAvaAudioContext),
    ATTS_COMMON_CCC_DEFINE,

    //Supported Audio Contexts
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicSupportedAudioContextsAttUuid, defaultSuppAudioContext),
    ATTS_COMMON_CCC_DEFINE,
};

/*
 * @brief the structure for default PACS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcPacsGroup = {
    NULL, pacsList, NULL, NULL, PACS_START_HDL, 0,
};

/**
 * @brief      for user add default PACS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addPacsGroup(void)
{
    svcPacsGroup.endHandle = svcPacsGroup.startHandle + ARRAY_SIZE(pacsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcPacsGroup);
}

/**
 * @brief      for user remove default PACS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removePacsGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(PACS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in PACS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_pacsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcPacsGroup.readCallback  = readCback;
    svcPacsGroup.writeCallback = writeCback;
}

#if !(LE_AUDIO_PACS_SINK_PAC || LE_AUDIO_PACS_SOURCE_PAC)
#error "pacs:Mandatory to support at least one of the Sink PAC or Source PAC characteristic"
#endif

#if !LE_AUDIO_PACS_SINK_PAC && LE_AUDIO_PACS_SINK_AUDIO_LOCATIONS
#error "pacs:Optional to support if the Sink PAC characteristic is supported, otherwise Excluded"
#endif

#if !LE_AUDIO_PACS_SOURCE_PAC && LE_AUDIO_PACS_SOURCE_AUDIO_LOCATIONS
#error "pacs:Optional to support if the Source PAC characteristic is supported, otherwise Excluded"
#endif
