/********************************************************************************************************
 * @file    svc_vcs.c
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

#define APP_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM 0

#define VCS_START_HDL                          SERVICE_VOLUME_CONTROL_HDL

#define VCS_VOL_STATE_FIX_LEN                  3
#define VCS_VOL_FLAGS_FIX_LEN                  1

_attribute_ble_data_retention_ static uint8_t vcsVolumeStateValue[VCS_VOL_STATE_FIX_LEN];
static const uint16_t                         vcsVolumeStateValueLen = VCS_VOL_STATE_FIX_LEN;

_attribute_ble_data_retention_ static uint8_t vcsVolumeFlagsValue;
static const uint16_t                         vcsVolumeFlagsValueLen = VCS_VOL_FLAGS_FIX_LEN;

extern const uint16_t aicsIncludeValue[APP_AUDIO_AICS_SERVER_MAX_INSTANCE_NUM][3];
extern const uint16_t vocsIncludeValue[LEA_VCP_INCLUDED_VOCS_SERVER_NUM][3];

/*
 * @brief the structure for default VCS service List.
 */
static const struct atts_attribute vcsList[] = {
    ATTS_PRIMARY_SERVICE(serviceVolumeControlUuid),

#if APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM > 0
    ATTS_INCLUDE_DEFINE(&aicsIncludeValue[0][0]),
#endif
#if APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM > 1
    ATTS_INCLUDE_DEFINE(&aicsIncludeValue[1][0]),
#endif
#if APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM > 2
    ATTS_INCLUDE_DEFINE(&aicsIncludeValue[2][0]),
#endif
#if APP_AUDIO_VCS_INCLUDE_AICS_INSTANCE_NUM > 3
    ATTS_INCLUDE_DEFINE(&aicsIncludeValue[3][0]),
#endif

#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 0
    ATTS_INCLUDE_DEFINE(&vocsIncludeValue[0][0]),
#endif
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 1
    ATTS_INCLUDE_DEFINE(&vocsIncludeValue[1][0]),
#endif
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 2
    ATTS_INCLUDE_DEFINE(&vocsIncludeValue[2][0]),
#endif
#if LEA_VCP_INCLUDED_VOCS_SERVER_NUM > 3
    ATTS_INCLUDE_DEFINE(&vocsIncludeValue[3][0]),
#endif

    //Volume State
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropReadNotify, characteristicVolumeStateUuid, vcsVolumeStateValue),
    ATTS_COMMON_CCC_DEFINE,

    //Volume Control Point
    ATTS_CHAR_UUID_ENCR_WRITE_NULL(charPropWrite, characteristicVolumeControlPointUuid),

    //Volume Flags
    ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(charPropReadNotify, characteristicVolumeFlagsUuid, vcsVolumeFlagsValue),
    ATTS_COMMON_CCC_DEFINE,
};

/*
 * @brief the structure for default VCS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcVcsGroup = {
    NULL, vcsList, NULL, NULL, VCS_START_HDL, 0,
};

/**
 * @brief      for user add default VCS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addVcsGroup(void)
{
    svcVcsGroup.endHandle = svcVcsGroup.startHandle + ARRAY_SIZE(vcsList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcVcsGroup);
}

/**
 * @brief      for user remove default VCS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeVcsGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(VCS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in VCS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_vcsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcVcsGroup.readCallback  = readCback;
    svcVcsGroup.writeCallback = writeCback;
}
