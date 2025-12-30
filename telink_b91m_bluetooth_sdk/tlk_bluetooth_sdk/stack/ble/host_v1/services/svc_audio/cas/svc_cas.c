/********************************************************************************************************
 * @file    svc_cas.c
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

/*
 * There are no characteristics in CAS
 * There shall be no more than one CAS instance on a server.
 * The CAS shall include no more than one instance of CSIS
 */
#define CAS_START_HDL SERVICE_COMMON_AUDIO_HDL

extern const uint16_t csisIncludeValue[3];
/*
 * @brief the structure for default CAS service List.
 */
static const struct atts_attribute casList[] = {
    ATTS_PRIMARY_SERVICE(serviceCommonAudioUuid),
    ATTS_INCLUDE_DEFINE(csisIncludeValue),
};

/*
 * @brief the structure for default CAS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcCasGroup = {
    NULL, casList, NULL, NULL, CAS_START_HDL, 0,
};

/**
 * @brief      for user add default CAS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addCasGroup(void)
{
    svcCasGroup.endHandle = svcCasGroup.startHandle + ARRAY_SIZE(casList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcCasGroup);
}

/**
 * @brief      for user remove default CAS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeCasGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(CAS_START_HDL);
}
