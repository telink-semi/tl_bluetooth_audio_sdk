/********************************************************************************************************
 * @file    svc_aics.c
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

#include "vendor/common/user_config.h"
#include "../svc_audio.h"
#include "svc_aics.h"

#define AICS_START_HDL SERVICE_AUDIO_INPUT_CONTROL_HDL

#if LEA_AICS_SERVICE_COUNT > 0
static const uint8_t defaultAicsAudioInputStateValue[]       = {0x00, 0x00, 0x00, 0x00};
static const uint8_t defaultAicsGainSettingPropertiesValue[] = {0x00, 0x00, 0x00};
static const uint8_t defaultAicsAudioInputTypeValue          = 0x00;
static const uint8_t defaultAicsAudioInputStatusValue        = 0x00;
static const uint8_t defaultAicsAudioInputDescriptionValue[] = {'A', 'I', 'C', 'S'};

static const uint16_t defaultAicsAudioInputStateValueLen       = sizeof(defaultAicsAudioInputStateValue);
static const uint16_t defaultAicsGainSettingPropertiesValueLen = sizeof(defaultAicsGainSettingPropertiesValue);
static const uint16_t defaultAicsAudioInputTypeValueLen        = sizeof(defaultAicsAudioInputTypeValue);
static const uint16_t defaultAicsAudioInputStatusValueLen      = sizeof(defaultAicsAudioInputStatusValue);
static const uint16_t defaultAicsAudioInputDescriptionValueLen = sizeof(defaultAicsAudioInputDescriptionValue);

#define LEA_AUDIO_AICS_SERVICE_DEFAULT_SERVICE_LIST                                                                                                                          \
    ATTS_SECONDARY_SERVICE(serviceAudioInputControlAttUuid), /* < Audio Input State */                                                                                       \
        ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicAudioInputStateAttUuid, defaultAicsAudioInputStateValue),                                        \
        ATTS_COMMON_CCC_DEFINE,                                                                                                             /** < Gain Setting Properties */ \
        ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropRead, characteristicGainSettingsAttributeAttUuid, defaultAicsGainSettingPropertiesValue), /** < Audio Input Type */        \
        ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropRead, characteristicAudioInputTypeAttUuid, defaultAicsAudioInputTypeValue),              /* < Audio Input Status */       \
        ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicAudioInputStatusAttUuid, defaultAicsAudioInputStatusValue),                                     \
        ATTS_COMMON_CCC_DEFINE,                                                                     /* < Audio Input Control Point */                                        \
        ATTS_CHAR_UUID_ENCR_WRITE_NULL(charPropWrite, characteristicAudioInputControlPointAttUuid), /* < Audio Input Description */                                          \
        ATTS_CHAR_UUID_ENCR_RDWR_POINT_RWCB(charPropReadWriteWithoutNotify, characteristicAudioInputDescriptionAttUuid, defaultAicsAudioInputDescriptionValue),              \
        ATTS_COMMON_CCC_DEFINE

/*
 * @brief the structure for default AICS service List.
 */
static const struct atts_attribute aicsList[] = {
    LEA_AUDIO_AICS_SERVICE_DEFAULT_SERVICE_LIST,
#if LEA_AICS_SERVICE_COUNT > 1
    LEA_AUDIO_AICS_SERVICE_DEFAULT_SERVICE_LIST,
#endif
#if LEA_AICS_SERVICE_COUNT > 2
    LEA_AUDIO_AICS_SERVICE_DEFAULT_SERVICE_LIST,
#endif
#if LEA_AICS_SERVICE_COUNT > 3
    LEA_AUDIO_AICS_SERVICE_DEFAULT_SERVICE_LIST,
#endif
};

/*
 * @brief the structure for default AICS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcAicsGroup = {
    NULL, aicsList, NULL, NULL, AICS_START_HDL, 0,
};
#endif

#define AICS_SVC_HDL_COUNT   ARRAY_SIZE(aicsList) / LEA_AICS_SERVICE_COUNT //15

#define AICS_START_HANDLE(n) (AICS_START_HDL + (AICS_SVC_HDL_COUNT) * (n))
#define AICS_END_HANDLE(n)   (AICS_START_HANDLE(n) + AICS_SVC_HDL_COUNT - 1)

const uint16_t aicsIncludeValue[LEA_AICS_SERVICE_COUNT][3] = {
#if LEA_AICS_SERVICE_COUNT > 0
    {AICS_START_HANDLE(0), AICS_END_HANDLE(0), SERVICE_UUID_AUDIO_INPUT_CONTROL},
#endif
#if LEA_AICS_SERVICE_COUNT > 1
    {AICS_START_HANDLE(1), AICS_END_HANDLE(1), SERVICE_UUID_AUDIO_INPUT_CONTROL},
#endif
#if LEA_AICS_SERVICE_COUNT > 2
    {AICS_START_HANDLE(2), AICS_END_HANDLE(2), SERVICE_UUID_AUDIO_INPUT_CONTROL},
#endif
#if LEA_AICS_SERVICE_COUNT > 3
    {AICS_START_HANDLE(3), AICS_END_HANDLE(3), SERVICE_UUID_AUDIO_INPUT_CONTROL},
#endif
};


/**
 * @brief      for user add default AICS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addAicsGroup(void)
{
#if LEA_AICS_SERVICE_COUNT > 0
    svcAicsGroup.endHandle = svcAicsGroup.startHandle + ARRAY_SIZE(aicsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcAicsGroup);
#endif
}

/**
 * @brief      for user remove default AICS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeAicsGroup(void)
{
#if LEA_AICS_SERVICE_COUNT > 0
    blc_gatts_removeAttributeServiceGroup(AICS_START_HDL);
#endif
}

/**
 * @brief      for user register read or write attribute value callback function in AICS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_aicsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
#if LEA_AICS_SERVICE_COUNT > 0
    svcAicsGroup.readCallback  = readCback;
    svcAicsGroup.writeCallback = writeCback;
#else
    (void)readCback;
    (void)writeCback;
#endif
}

#if LEA_AICS_SERVICE_COUNT > 4
#error "LEA_AICS_SERVICE_COUNT should be less than or equal to 4"
#endif
