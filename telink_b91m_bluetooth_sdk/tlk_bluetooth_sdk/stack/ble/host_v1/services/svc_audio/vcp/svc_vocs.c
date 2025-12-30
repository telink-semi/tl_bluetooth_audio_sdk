/********************************************************************************************************
 * @file    svc_vocs.c
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
#include "svc_vcp.h"


#define VOCS_START_HDL            SERVICE_VOCS_IN_VCS_HDL

#define VOCS_OUTPUT_DESC_MAX_SIZE 50

const uint16_t gVocsOutDescMaxSize = VOCS_OUTPUT_DESC_MAX_SIZE;

#define VOCS_VOL_OFFSET_STATE_FIX_LEN 3
#define VOCS_AUDIO_LOCATION_FIX_LEN   4

#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 0
static uint8_t vocsVolumeOffsetStateValue[LEA_VCP_INCLUDED_VOCS_SERVER_NUM][VOCS_VOL_OFFSET_STATE_FIX_LEN] = {
    {0x00, 0x00, 0x00},
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 1
    {0x00, 0x00, 0x00},
#endif
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 2
    {0x00, 0x00, 0x00},
#endif
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 3
    {0x00, 0x00, 0x00},
#endif
};
static const uint16_t vocsVolumeOffsetStateValueLen = VOCS_VOL_OFFSET_STATE_FIX_LEN;

static uint32_t vocsAudioLocationValue[LEA_VCP_INCLUDED_VOCS_SERVER_NUM] = {
    0x00,
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 1
    0x00,
#endif
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 2
    0x00,
#endif
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 3
    0x00,
#endif
};
static const uint16_t vocsAudioLocationValueLen = VOCS_AUDIO_LOCATION_FIX_LEN;

static uint8_t vocsAudioOutputDescriptionValue[LEA_VCP_INCLUDED_VOCS_SERVER_NUM][VOCS_OUTPUT_DESC_MAX_SIZE];

static uint16_t vocsAudioOutputDescriptionValueLen[LEA_VCP_INCLUDED_VOCS_SERVER_NUM];
#endif

#define ATTS_CHAR_VOL_OFFSET_STATE(value)                 \
    ATTS_CHARACTERISTIC_DECLARATIONS(charPropReadNotify), \
        ATTS_CHAR_UUID_DEFINE(ATT_PERMISSIONS_ENCRYPT_READ, characteristicVolumeOffsetStateUuid, vocsVolumeOffsetStateValueLen, VOCS_VOL_OFFSET_STATE_FIX_LEN, value, 0)

#define ATTS_CHAR_AUDIO_LOCATION(value)                                                                                                                     \
    ATTS_CHARACTERISTIC_DECLARATIONS(charPropReadWriteWithoutNotify),                                                                                       \
        ATTS_CHAR_UUID_DEFINE(ATT_PERMISSIONS_ENCRYPT_RDWR, characteristicAudioLocationUuid, vocsAudioLocationValueLen, VOCS_AUDIO_LOCATION_FIX_LEN, value, \
                              ATTS_SET_WRITE_CALLBACK | ATTS_SET_ALLOW_WRITE)

#define ATTS_CHAR_VOL_OFFSET_CTRL_POINT()                                                                                                                     \
    ATTS_CHARACTERISTIC_DECLARATIONS(charPropWrite),                                                                                                          \
    {                                                                                                                                                         \
        ATT_PERMISSIONS_ENCRYPT_WRITE, ATT_16_UUID_LEN, (uint8_t *)(size_t)characteristicVolumeOffsetControlPointUuid, NULL, 0, NULL, ATTS_SET_WRITE_CALLBACK \
    }

#define ATTS_CHAR_AUDIO_OUTPUT_DESC(value, len)                                                                                              \
    ATTS_CHARACTERISTIC_DECLARATIONS(charPropReadWriteWithoutNotify),                                                                        \
        ATTS_CHAR_UUID_DEFINE(ATT_PERMISSIONS_ENCRYPT_RDWR, characteristicAudioOutputDescriptionUuid, len, VOCS_OUTPUT_DESC_MAX_SIZE, value, \
                              ATTS_SET_WRITE_CALLBACK | ATTS_SET_ALLOW_WRITE | ATTS_SET_VARIABLE_LEN)

#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 0
/*
 * @brief the structure for default VOCS service List.
 */
static const struct atts_attribute vocsList[] = {
    ATTS_SECONDARY_SERVICE(serviceVolumeOffsetControlUuid),

    //Volume Offset State
    ATTS_CHAR_VOL_OFFSET_STATE(&vocsVolumeOffsetStateValue[0][0]),
    ATTS_COMMON_CCC_DEFINE,

    //Audio Location
    ATTS_CHAR_AUDIO_LOCATION(&vocsAudioLocationValue[0]),
    ATTS_COMMON_CCC_DEFINE,

    //Volume Offset Control Point
    ATTS_CHAR_VOL_OFFSET_CTRL_POINT(),

    //Audio output description
    ATTS_CHAR_AUDIO_OUTPUT_DESC(&vocsAudioOutputDescriptionValue[0][0], vocsAudioOutputDescriptionValueLen[0]),
    ATTS_COMMON_CCC_DEFINE,

#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 1
    ATTS_SECONDARY_SERVICE(serviceVolumeOffsetControlUuid),

    //Volume Offset State
    ATTS_CHAR_VOL_OFFSET_STATE(&vocsVolumeOffsetStateValue[1][0]),
    ATTS_COMMON_CCC_DEFINE,

    //Audio Location
    ATTS_CHAR_AUDIO_LOCATION(&vocsAudioLocationValue[1]),
    ATTS_COMMON_CCC_DEFINE,

    //Volume Offset Control Point
    ATTS_CHAR_VOL_OFFSET_CTRL_POINT(),

    //Audio output description
    ATTS_CHAR_AUDIO_OUTPUT_DESC(&vocsAudioOutputDescriptionValue[1][0], vocsAudioOutputDescriptionValueLen[1]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 2
    ATTS_SECONDARY_SERVICE(serviceVolumeOffsetControlUuid),

    //Volume Offset State
    ATTS_CHAR_VOL_OFFSET_STATE(&vocsVolumeOffsetStateValue[2][0]),
    ATTS_COMMON_CCC_DEFINE,

    //Audio Location
    ATTS_CHAR_AUDIO_LOCATION(&vocsAudioLocationValue[2]),
    ATTS_COMMON_CCC_DEFINE,

    //Volume Offset Control Point
    ATTS_CHAR_VOL_OFFSET_CTRL_POINT(),

    //Audio output description
    ATTS_CHAR_AUDIO_OUTPUT_DESC(&vocsAudioOutputDescriptionValue[2][0], vocsAudioOutputDescriptionValueLen[2]),
    ATTS_COMMON_CCC_DEFINE,
#endif

#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 3
    ATTS_SECONDARY_SERVICE(serviceVolumeOffsetControlUuid),

    //Volume Offset State
    ATTS_CHAR_VOL_OFFSET_STATE(&vocsVolumeOffsetStateValue[3][0]),
    ATTS_COMMON_CCC_DEFINE,

    //Audio Location
    ATTS_CHAR_AUDIO_LOCATION(&vocsAudioLocationValue[3]),
    ATTS_COMMON_CCC_DEFINE,

    //Volume Offset Control Point
    ATTS_CHAR_VOL_OFFSET_CTRL_POINT(),

    //Audio output description
    ATTS_CHAR_AUDIO_OUTPUT_DESC(&vocsAudioOutputDescriptionValue[3][0], vocsAudioOutputDescriptionValueLen[3]),
    ATTS_COMMON_CCC_DEFINE,
#endif
};
#endif

#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 0
/*
 * @brief the structure for default VOCS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcVocsGroup = {
    NULL, vocsList, NULL, NULL, VOCS_START_HDL, 0,
};
#endif

#define VOCS_SVC_HDL_COUNT   ARRAY_SIZE(vocsList) / LEA_VCP_INCLUDED_VOCS_SERVER_NUM //15

#define VOCS_START_HANDLE(n) (VOCS_START_HDL + (VOCS_SVC_HDL_COUNT) * (n))
#define VOCS_END_HANDLE(n)   (VOCS_START_HANDLE(n) + VOCS_SVC_HDL_COUNT - 1)

const uint16_t vocsIncludeValue[LEA_VCP_INCLUDED_VOCS_SERVER_NUM][3] = {
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 0
    {VOCS_START_HANDLE(0), VOCS_END_HANDLE(0), SERVICE_UUID_VOLUME_OFFSET_CONTROL},
#endif
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 1
    {VOCS_START_HANDLE(1), VOCS_END_HANDLE(1), SERVICE_UUID_VOLUME_OFFSET_CONTROL},
#endif
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 2
    {VOCS_START_HANDLE(2), VOCS_END_HANDLE(2), SERVICE_UUID_VOLUME_OFFSET_CONTROL},
#endif
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 3
    {VOCS_START_HANDLE(3), VOCS_END_HANDLE(3), SERVICE_UUID_VOLUME_OFFSET_CONTROL},
#endif
};

void blc_svc_addVocsGroup(void)
{
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 0
    svcVocsGroup.endHandle = svcVocsGroup.startHandle + ARRAY_SIZE(vocsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcVocsGroup);
#endif
}

void blc_svc_removeVocsGroup(void)
{
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 0
    blc_gatts_removeAttributeServiceGroup(VOCS_START_HDL);
#endif
}

void blc_svc_vocsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 0
    svcVocsGroup.readCallback  = readCback;
    svcVocsGroup.writeCallback = writeCback;
#else
    (void)readCback;
    (void)writeCback;
#endif
}

#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 4
#error "LEA_VCP_INCLUDED_VOCS_SERVER_NUM should be less than or equal to 4"
#endif
