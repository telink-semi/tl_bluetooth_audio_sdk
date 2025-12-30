/********************************************************************************************************
 * @file    btp_sdpConst.c
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
#include "tl_common.h"
#include "tlkapi/tlkapi.h"

#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#if (TLKBTP_CFG_SDP_ENABLE)
#include "btp_sdpConst.h"
// #include "btp_sdpInner.h"
#include "btp_sdp.h"
//#include "../btp_config.h"
#include "stack/bt/host/btp/btp_config.h"
#if (TLKBTP_CFG_HID_ENABLE)
//#include "../hid/btp_hid.h"
//#include "../pts/btp_ptsHid.h"
//#include "../../bth/bth_hcicod.h"
#include "stack/bt/host/btp/hid/btp_hid.h"
#include "stack/bt/host/bth/bth_stdio.h"

#endif


#define BTP_SDP_VID_TELINK 0X0211


const char gcBthSdpServiceName[]  = "sdp_sv_name";
const char gcBthSdpServiceDesc[]  = "sdp_sv_desc";
const char gcBthSdpProviderName[] = "Telink";


const char gcBthSdpServiceUrl[]      = "www.telink-semi.cn";
const char gcBthPnpInfoServiceDesc[] = "PnP Information";

const char gcBthSppInfoServiceDesc[] = "Telink SPP";
const char gcBthSdpSppServiceName[]  = "Serial Port Service";

// const char gcBthSdpProviderName[] = "telink-semi.com";
const char gcBthSdpServiceAvalibility   = 0xff; // 0x55
const char gcBthSdpPnpInfoServiceDesc[] = "PnP Information";

const char gcBthSdpHfpHfServiceName[] = "HFP HF";
const char gcBthSdpHfpAgServiceName[] = "HFP AG";
const int  gcBthSdpHfpHfFeature       = 0x3F;
const int  gcBthSdpHfpAgFeature       = 0x3F;

#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
const int gcBthSdpAvrcpCtFeature = 0xC1; // Supports browsing.
const int gcBthSdpAvrcpTgFeature = 0x02;
#else
const int gcBthSdpAvrcpCtFeature = 0x01;
const int gcBthSdpAvrcpTgFeature = 0x02;
#endif
const char gcBthSdpAvrcpCtProviderName[] = "Telink";
const char gcBthSdpAvrcpTgProviderName[] = "Telink";
const char gcBthSdpAvrcpCtServiceName[]  = "AVRCP CT";
const char gcBthSdpAvrcpTgServiceName[]  = "AVRCP TG";


const int gcBthSdpA2dpSrcFeature = 1;
const int gcBthSdpA2dpSnkFeature = 1;

const char gcBthSdpA2dpSrcProviderName[] = "Telink";
const char gcBthSdpA2dpSnkProviderName[] = "Telink";
const char gcBthSdpA2dpSrcServiceName[]  = "Audio/Video Service";
const char gcBthSdpA2dpSnkServiceName[]  = "Audio/Video Service";

#if (TLKBTP_CFG_HID_ENABLE)
static const char gcBtpSdpHidServiceName[]  = "HID Gamepad";
static const char gcBtpSdpHidProviderName[] = "Microsoft Corporation";
// SubClass: Refer <HID_v1.1.1.pdf> P66
#define scBtpSdpHidSubClass 0x08 //(BTH_COD_PERIPHERAL_KEYBOARD | BTH_COD_PERIPHERAL_KEYBOARD)
#endif

#if (TLKBTP_CFG_PBAPCLT_ENABLE)
const char gcBthSdpPbapClientName[] = "Phonebook Access PCE";
#endif
#if (TLKBTP_CFG_PBAPSRV_ENABLE)
const char gcBthSdpPbapServiceName[] = "Phonebook Access PSE";
#endif

#if (TLKBTP_CFG_CUSTP_ENABLE)
const char gcBthSdpCustpServiceName[]  = "Telink_Name";
const char gcBthSdpCustpServiceDesc[]  = "Telink_Desc";
const char gcBthSdpCustpProviderName[] = "Telink_Prov";
#endif

uint8_t gBthSrvSppUserClassId[16] =
    //{0x65, 0xD6, 0x92, 0x8C, 0x9F, 0x17, 0x4F, 0xCC, 0xAA, 0x55, 0x0B, 0xF0, 0xF4, 0x5B, 0xCE, 0xE1};
    //{ 0x0C, 0xF1, 0x2D, 0x31, 0xFA, 0xC3, 0x45, 0x53, 0xBD, 0x80, 0xD6, 0x83, 0x2E, 0x7B, 0x38, 0x50 };
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const char headset_spp[] = "SPP";

#if (TLKBTP_CFG_SPP_USER_ENABLE)
const btp_sdp_serviceItem_t gcBthSdpSppUserItem[] = {
    // Service Record Handle	0x00010007
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_SPP_USER_HANDLE,
     .pValue = (unsigned char *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_128,
     .value  = 0,
     .pValue = (unsigned char *)gBthSrvSppUserClassId},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_RFCOMM_UUID,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = TLKBT_CFG_SPP_USER_RFC_CHANNEL,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Browse Group List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_UUID, .dsize = BTP_SDP_DSIZE_16, .value = 0x1002, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x656e, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (unsigned char *)0},

    // Service Availability	255
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_SERVICE_AVAILABILITY, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_8, .value = 255, .pValue = (unsigned char *)0},

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_128,
     .value  = 0,
     .pValue = (unsigned char *)gBthSrvSppUserClassId},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Service Name	"SPP"
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = BTP_SDP_ATTR_SERVICE_NAME, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)headset_spp}};
#endif //TLKBTP_CFG_SPP_USER_ENABLE

const btp_sdp_serviceItem_t gcBthSdpSppItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_RECORD_HANDLE, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_32, .value = BTP_SDP_SPP_HANDLE, .pValue = (uint8_t *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_SERIAL_PORT,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_RFCOMM_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = TLKBT_CFG_SPP_RFC_CHANNEL,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    // BluetoothProfileDescriptorList
    /* Don't modify the format easily, or it will be difficult to understand its meaning!!! */
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin profile list
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin profile list
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_SERIAL_PORT,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0x0102,
     .pValue = (uint8_t *)0}, // list item[0] VERSION = 0x0102
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x656e, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Service Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpSppServiceName},

    // Provider Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpProviderName}};

const btp_sdp_serviceItem_t gcBthSdpPnpInfoItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_PNP_INFO_HANDLE,
     .pValue = (uint8_t *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_PNP_INFO,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Browse Group List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x656e, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x6672, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE + 0x0010,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x6465, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE + 0x0020,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x6A61, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE + 0x0030,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Service Availability
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = 0,
     .pValue = (uint8_t *)&gcBthSdpServiceAvalibility},

    // Service Description List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_STR,
     .uuid   = BTP_SDP_ATTR_SERVICE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)gcBthSdpPnpInfoServiceDesc},

    // Specification ID
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_GROUP_ID,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SPECIFICATION_ID,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0103, .pValue = (uint8_t *)0},

    // Vendor ID
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 1, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_VENDOR_ID, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_GROUP_ID + 1,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0x1532, //BTP_SDP_VID_TELINK,
     .pValue = (uint8_t *)0},

    // Product ID
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 2, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_PRODUCT_ID, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 2, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0A60, .pValue = (uint8_t *)0},

    // Version
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 3, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_VERSION, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 3, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x068E, .pValue = (uint8_t *)0},

    // Primary Record
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_GROUP_ID + 4,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PRIMARY_RECORD,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 4, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = 0x01, .pValue = (uint8_t *)0},

    // Vendor ID Source
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_GROUP_ID + 5,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_VENDOR_ID_SOURCE,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 5, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0002, .pValue = (uint8_t *)0},
};

const btp_sdp_serviceItem_t gcBthSdpHfpHfItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_RECORD_HANDLE, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_32, .value = BTP_SDP_HFP_HF_HANDLE, .pValue = (uint8_t *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_HANDSFREE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_GENERIC_AUDIO,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_RFCOMM_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = TLKBT_CFG_HFPHF_RFC_CHANNEL,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Service Availability
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = 0,
     .pValue = (uint8_t *)&gcBthSdpServiceAvalibility},

    // Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_HANDSFREE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_HFP_PROFILE_VERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Service Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpHfpHfServiceName},

    // Provider Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpProviderName},

    // Additional Attribute
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0,
     .pValue = (uint8_t *)&gcBthSdpHfpHfFeature},
};

const btp_sdp_serviceItem_t gcBthSdpHfpAgItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_RECORD_HANDLE, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_32, .value = BTP_SDP_HFP_AG_HANDLE, .pValue = (uint8_t *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_HANDSFREE_AGW,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_GENERIC_AUDIO,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_RFCOMM_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = TLKBT_CFG_HFPAG_RFC_CHANNEL,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    //
    //	{BTP_SDP_FLAG_ATT, BTP_SDP_ATTR_SERVICE_AVAILABILITY,  BTP_SDP_DTYPE_UINT, BTP_SDP_DSIZE_16,
    //  BTP_SDP_ATTR_SERVICE_AVAILABILITY,  (uint8_t*)0}, //att id = 8
    //		{BTP_SDP_FLAG_ATT|BTP_SDP_FLAG_W, BTP_SDP_ATTR_SERVICE_AVAILABILITY, BTP_SDP_DTYPE_UINT, BTP_SDP_DSIZE_8, 0,
    //(uint8_t*) &gcBthSdpServiceAvalibility}, // time alive(seconds)

    // Bluetooth Profile Descriptor List

    // Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_HANDSFREE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_HFP_PROFILE_VERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Service Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpHfpAgServiceName},
    // Provider Name
    //	{BTP_SDP_FLAG_ATT, 0x0102,  BTP_SDP_DTYPE_UINT, BTP_SDP_DSIZE_16,  0x0102,  (uint8_t*)0},  // provider name
    //		{BTP_SDP_FLAG_STR, 0x0102, BTP_SDP_DTYPE_NULL, BTP_SDP_DSIZE_NULL,  0,  (uint8_t*)gcBthSdpProviderName},  //
    //  provider name string
    // Network
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_EXTERNAL_NETWORK,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_EXTERNAL_NETWORK,
     .pValue = (uint8_t *)0}, // Network
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_EXTERNAL_NETWORK,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = 1,
     .pValue = (uint8_t *)0}, // 0x01 – Ability to reject a call 0x00 – No ability to reject a call
    // SupportedFeatures
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0,
     .pValue = (uint8_t *)&gcBthSdpHfpAgFeature},
};

const btp_sdp_serviceItem_t gcBthSdpA2dpSrcItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_A2DP_SRC_HANDLE,
     .pValue = (uint8_t *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_AUDIO_SOURCE,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_AVDTP, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_AVDTP_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_A2DP_PROTOCOL_VERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_ADVANCED_AUDIO,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_A2DP_PROFILE_ERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // SupportedFeatures
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0,
     .pValue = (uint8_t *)&gcBthSdpA2dpSrcFeature},

    // Browse Group List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Provider Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpA2dpSrcProviderName},

    // Service Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpA2dpSrcServiceName},
};

const btp_sdp_serviceItem_t gcBthSdpA2dpSnkItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_A2DP_SNK_HANDLE,
     .pValue = (uint8_t *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_AUDIO_SINK,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_AVDTP, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_AVDTP_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_A2DP_PROTOCOL_VERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_ADVANCED_AUDIO,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_A2DP_PROFILE_ERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // SupportedFeatures
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0,
     .pValue = (uint8_t *)&gcBthSdpA2dpSnkFeature},

    // Browse Group List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Provider Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpA2dpSnkProviderName},

    // Service Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpA2dpSnkServiceName},
};

const btp_sdp_serviceItem_t gcBthSdpAvrcpTgItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_AVRCP_TG_HANDLE,
     .pValue = (uint8_t *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_AV_REMOTE_TARGET,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_AVCTP, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_AVCTP_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_AVRCP_PROTOCOL_VERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_AV_REMOTE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_AVRCP_PROFILE_VERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    // Additional Protocol Descriptor Lists
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_PSMID_AVCTP_BROWSING,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_AVCTP_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_AVRCP_PROTOCOL_VERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
#endif

    // SupportedFeatures
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0,
     .pValue = (uint8_t *)&gcBthSdpAvrcpTgFeature},

    // Browse Group List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Provider Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpAvrcpTgProviderName},

    // Service Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpAvrcpTgServiceName},
};

const btp_sdp_serviceItem_t gcBthSdpAvrcpCtItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_AVRCP_CT_HANDLE,
     .pValue = (uint8_t *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_AV_REMOTE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_VIDEO_CONF,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Protocol #0 PSM
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_AVCTP, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Protocol #1 Version
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_AVCTP_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_AVRCP_PROTOCOL_VERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    // Additional Protocol Descriptor Lists
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_PSMID_AVCTP_BROWSING,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_AVCTP_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_AVRCP_PROTOCOL_VERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
#endif

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Profile #0
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_AV_REMOTE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_AVRCP_PROFILE_VERSION,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // SupportedFeatures
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0,
     .pValue = (uint8_t *)&gcBthSdpAvrcpCtFeature},

    // Browse Group List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Provider Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpAvrcpCtProviderName},

    // Service Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)gcBthSdpAvrcpCtServiceName},
};

const uint8_t gcBtpSdpHidReportMap[] = {
    0x05, 0x01,       // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05,       // Usage (Game Pad)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x01,       //   Report ID (1)
    0x09, 0x30,       //   Usage (X)
    0x09, 0x31,       //   Usage (Y)
    0x09, 0x32,       //   Usage (Z)
    0x09, 0x35,       //   Usage (Rz)
    0x16, 0x01, 0x80, //   Logical Minimum (-32767)
    0x26, 0xFF, 0x7F, //   Logical Maximum (32767)
    0x75, 0x10,       //   Report Size (16)
    0x95, 0x04,       //   Report Count (4)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x39,       //   Usage (Hat switch)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x07,       //   Logical Maximum (7)
    0x35, 0x00,       //   Physical Minimum (0)
    0x46, 0x3B, 0x01, //   Physical Maximum (315)
    0x65, 0x14,       //   Unit (System: English Rotation, Length: Centimeter)
    0x75, 0x04,       //   Report Size (4)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x42,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x65, 0x00,       //   Unit (None)
    0x05, 0x09,       //   Usage Page (Button)
    0x19, 0x01,       //   Usage Minimum (0x01)
    0x29, 0x12,       //   Usage Maximum (0x12)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x12,       //   Report Count (18)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x0A,       //   Report Size (10)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x03,       //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x02,       //   Usage Page (Sim Ctrls)
    0x09, 0xC5,       //   Usage (Brake)
    0x09, 0xC4,       //   Usage (Accelerator)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x35, 0x00,       //   Physical Minimum (0)
    0x46, 0xFF, 0x00, //   Physical Maximum (255)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x02,       //   Report Count (2)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x06, 0x00, 0xFF, //   Usage Page (Vendor Defined 0xFF00)
    0x09, 0x21,       //   Usage (0x21)
    0x95, 0x02,       //   Report Count (2)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,             // End Collection
    // 103 bytes
};

const uint8_t gcBtpSdpHidReportMap01ok[] = {
    0x05, 0x01,       // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05,       // Usage (Game Pad)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x01,       //   Report ID (1)
    0x09, 0x30,       //   Usage (X)
    0x09, 0x31,       //   Usage (Y)
    0x09, 0x32,       //   Usage (Z)
    0x09, 0x35,       //   Usage (Rz)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x04,       //   Report Count (4)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x39,       //   Usage (Hat switch)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x07,       //   Logical Maximum (7)
    0x75, 0x04,       //   Report Size (4)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x42,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x05, 0x09,       //   Usage Page (Button)
    0x19, 0x01,       //   Usage Minimum (0x01)
    0x29, 0x0E,       //   Usage Maximum (0x0E)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x0E,       //   Report Count (14)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x06,       //   Report Size (6)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x01,       //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,       //   Usage Page (Generic Desktop Ctrls)
    0x09, 0x33,       //   Usage (Rx)
    0x09, 0x34,       //   Usage (Ry)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x02,       //   Report Count (2)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)


    0x06, 0x04, 0xFF, //   Usage Page (Vendor Defined 0xFF04)
    0x85, 0x02,       //   Report ID (2)
    0x09, 0x24,       //   Usage (0x24)
    0x95, 0x24,       //   Report Count (36)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA3,       //   Report ID (-93)
    0x09, 0x25,       //   Usage (0x25)
    0x95, 0x30,       //   Report Count (48)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x05,       //   Report ID (5)
    0x09, 0x26,       //   Usage (0x26)
    0x95, 0x28,       //   Report Count (40)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x06,       //   Report ID (6)
    0x09, 0x27,       //   Usage (0x27)
    0x95, 0x34,       //   Report Count (52)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x07,       //   Report ID (7)
    0x09, 0x28,       //   Usage (0x28)
    0x95, 0x30,       //   Report Count (48)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x08,       //   Report ID (8)
    0x09, 0x29,       //   Usage (0x29)
    0x95, 0x2F,       //   Report Count (47)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x09,       //   Report ID (9)
    0x09, 0x2A,       //   Usage (0x2A)
    0x95, 0x13,       //   Report Count (19)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x06, 0x03, 0xFF, //   Usage Page (Vendor Defined 0xFF03)
    0x85, 0x03,       //   Report ID (3)
    0x09, 0x21,       //   Usage (0x21)
    0x95, 0x26,       //   Report Count (38)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x04,       //   Report ID (4)
    0x09, 0x22,       //   Usage (0x22)
    0x95, 0x2E,       //   Report Count (46)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xF0,       //   Report ID (-16)
    0x09, 0x47,       //   Usage (0x47)
    0x95, 0x3F,       //   Report Count (63)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xF1,       //   Report ID (-15)
    0x09, 0x48,       //   Usage (0x48)
    0x95, 0x3F,       //   Report Count (63)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xF2,       //   Report ID (-14)
    0x09, 0x49,       //   Usage (0x49)
    0x95, 0x0F,       //   Report Count (15)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x06, 0x00, 0xFF, //   Usage Page (Vendor Defined 0xFF00)
    0x85, 0x11,       //   Report ID (17)
    0x09, 0x20,       //   Usage (0x20)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x4D,       //   Report Count (77)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x21,       //   Usage (0x21)
    0x91, 0x02,       //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x12,       //   Report ID (18)
    0x09, 0x22,       //   Usage (0x22)
    0x95, 0x8D,       //   Report Count (-115)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x23,       //   Usage (0x23)
    0x91, 0x02,       //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x13,       //   Report ID (19)
    0x09, 0x24,       //   Usage (0x24)
    0x95, 0xCD,       //   Report Count (-51)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x25,       //   Usage (0x25)
    0x91, 0x02,       //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x14,       //   Report ID (20)
    0x09, 0x26,       //   Usage (0x26)
    0x96, 0x0D, 0x01, //   Report Count (269)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x27,       //   Usage (0x27)
    0x91, 0x02,       //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x15,       //   Report ID (21)
    0x09, 0x28,       //   Usage (0x28)
    0x96, 0x4D, 0x01, //   Report Count (333)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x29,       //   Usage (0x29)
    0x91, 0x02,       //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x16,       //   Report ID (22)
    0x09, 0x2A,       //   Usage (0x2A)
    0x96, 0x8D, 0x01, //   Report Count (397)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x2B,       //   Usage (0x2B)
    0x91, 0x02,       //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

    0xC0, // End Collection

    // 442 bytes
};

#if (TLKBTP_CFG_HID_ENABLE)
const btp_sdp_serviceItem_t gcBtpSdpHidItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_HID_DEV_HANDLE,
     .pValue = (unsigned char *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_HID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_HIDC, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_HIDP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x656e, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (unsigned char *)0},

    // Additional Protocol Descriptor Lists
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_PSMID_HIDI,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_HIDP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Service Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)gcBtpSdpHidServiceName},

    // Provider Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)gcBtpSdpHidProviderName},

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_HID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0101, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x656e, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (unsigned char *)0},

    // HID Parser Version
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SVCDB_STATE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SVCDB_STATE,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_SVCDB_STATE, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0111, .pValue = (unsigned char *)0},

    // HID Device Subclass
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0202, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_DEVICE_SUBCLASS, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0202, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_8, .value = scBtpSdpHidSubClass, .pValue = (unsigned char *)0},

    // HIDCountryCode
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0203, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_COUNTRY_CODE, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0203, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_8, .value = 0x21, .pValue = (unsigned char *)0},

    // HIDVirtualCable
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0204, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_VIRTUAL_CABLE, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0204, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = 0x00, .pValue = (unsigned char *)0},

    // HIDReconnectInitiate
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0205, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_RECONNECT_INITIATE, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0205, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = 0x00, .pValue = (unsigned char *)0},

    // HIDNormallyConnectable
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = 0x0205,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_HID_NORMALLY_CONNECTABLE,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0205, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = TRUE, .pValue = (unsigned char *)0},

    // HIDSDPDISABLE
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0205, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_SDP_DISABLE, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0205, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = FALSE, .pValue = (unsigned char *)0},

    // HIDDescriptorList
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0206, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_DESCRIPTOR_LIST, .pValue = (unsigned char *)0},
    {.flag  = BTP_SDP_FLAG_ATT,
     .uuid  = 0x0206,
     .dtype = BTP_SDP_DTYPE_DES,
     .dsize = sizeof(gcBtpSdpHidReportMap) > 0xFF ? BTP_SDP_DSIZE_VAR_16 : BTP_SDP_DSIZE_VAR_8,
     .value = sizeof(gcBtpSdpHidReportMap) + 6,
     // .dsize = sizeof(gcBtpSdpHidReportMap) > 0xFF ? BTP_SDP_DSIZE_VAR_16: BTP_SDP_DSIZE_VAR_8,
     // .value = sizeof(gcBtpSdpHidReportMap) + 8,
     .pValue = (unsigned char *)0},
    {.flag  = BTP_SDP_FLAG_ATT,
     .uuid  = 0x0206,
     .dtype = BTP_SDP_DTYPE_DES,
     .dsize = sizeof(gcBtpSdpHidReportMap) > 0xFF ? BTP_SDP_DSIZE_VAR_16 : BTP_SDP_DSIZE_VAR_8,
     .value = sizeof(gcBtpSdpHidReportMap) + 4,
     // .dsize = sizeof(gcBtpSdpHidReportMap) > 0xFF ? BTP_SDP_DSIZE_VAR_16: BTP_SDP_DSIZE_VAR_8,
     // .value = sizeof(gcBtpSdpHidReportMap) + 5,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0206, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_8, .value = 0x22, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_HID_DES,
     .uuid   = 0x0206,
     .dtype  = BTP_SDP_DTYPE_STRING,
     .dsize  = sizeof(gcBtpSdpHidReportMap),
     .value  = 0x00,
     .pValue = (unsigned char *)gcBtpSdpHidReportMap},
    // HIDLANGIDBaseList
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0207, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_LANG_ID_BASE_LIST, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0207, .dtype = BTP_SDP_DTYPE_DES, .dsize = BTP_SDP_DSIZE_VAR_8, .value = 0x08, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0207, .dtype = BTP_SDP_DTYPE_DES, .dsize = BTP_SDP_DSIZE_VAR_8, .value = 0x06, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0207, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0409, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0207, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (unsigned char *)0},

    // HIDBootDevice
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x020e, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_BOOT_DEVICE, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x020e, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = 0x00, .pValue = (unsigned char *)0},
};
#endif

const btp_sdp_serviceItem_t gcBtpServiceDiscoverItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_SERVICE_HANDLE,
     .pValue = (unsigned char *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_SDP_SERVER,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Record State
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_STATE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_STATE,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_RECORD_STATE, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_32, .value = 0x12345678, .pValue = (unsigned char *)0},

    // Service ID
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_ID,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_ID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_SERVICE_ID, .dtype = BTP_SDP_DTYPE_UUID, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SVC_USER_ID0, .pValue = (unsigned char *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_SDP, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Browse Group List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x656e, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (unsigned char *)0},

    // Service Info TTL
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SVCINFO_TTL,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SVCINFO_TTL,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_SVCINFO_TTL, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_32, .value = 0x55aa7788, .pValue = (unsigned char *)0},

    // Service Availability
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = 0,
     .pValue = (unsigned char *)&gcBthSdpServiceAvalibility},

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_SDP_SERVER,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Document URL
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_DOC_URL, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_DOC_URL, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_URL, .uuid = BTP_SDP_ATTR_DOC_URL, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)gcBthSdpServiceUrl},

    // Client Execution URL
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_CLIENT_EXEC_URL,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_CLIENT_EXEC_URL,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_URL,
     .uuid   = BTP_SDP_ATTR_CLIENT_EXEC_URL,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (unsigned char *)gcBthSdpServiceUrl},

    // Icon URL
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_ICON_URL, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_ICON_URL, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_URL, .uuid = BTP_SDP_ATTR_ICON_URL, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)gcBthSdpServiceUrl},

    // Additional Protocol Descriptor Lists
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_SDP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Primary Service Name
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SVCNAME_PRIMARY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SVCNAME_PRIMARY,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_STR,
     .uuid   = BTP_SDP_ATTR_SVCNAME_PRIMARY,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (unsigned char *)gcBthSdpServiceName},

    // Primary Service Description
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SVCDESC_PRIMARY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SVCDESC_PRIMARY,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_STR,
     .uuid   = BTP_SDP_ATTR_SVCDESC_PRIMARY,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (unsigned char *)gcBthSdpServiceDesc},

    // Primary Provider Name
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROVNAME_PRIMARY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROVNAME_PRIMARY,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_STR,
     .uuid   = BTP_SDP_ATTR_PROVNAME_PRIMARY,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (unsigned char *)gcBthSdpProviderName},

    // Version Number List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_VERSION_NUM_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_VERSION_NUM_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_VERSION_NUM_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_VERSION_NUM_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_VERSION_NUM_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Service Database State
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SVCDB_STATE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SVCDB_STATE,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_SVCDB_STATE, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_32, .value = 0xfaceface, .pValue = (unsigned char *)0},
};


const uint8_t scBtpServiceIap2UuidL[16] = {0x00, 0x00, 0x00, 0x00, 0xde, 0xCA, 0xFA, 0xDE, 0xDE, 0xCA, 0xDE, 0xAF, 0xDE, 0xCA, 0xCA, 0xFF};

/*Accessory Interface Specification R1: 23.4 P124*/
const uint8_t scBtpServiceIap2UuidR[16] = {0x00, 0x00, 0x00, 0x00, 0xde, 0xCA, 0xFA, 0xDE, 0xDE, 0xCA, 0xDE, 0xAF, 0xDE, 0xCA, 0xCA, 0xFE};
//{0xFE, 0xCA, 0xCA, 0xDE, 0xAF, 0xDE, 0xCA, 0xDE, 0xDE, 0xFA, 0xCA, 0xDE, 0x00, 0x00, 0x00, 0x00};
const uint8_t               scBtpServiceIap2ServiceName[] = "Telink iAP2 Service";
const btp_sdp_serviceItem_t gcBtpIap2ServiceItem[]        = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
            .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
            .dtype  = BTP_SDP_DTYPE_UINT,
            .dsize  = BTP_SDP_DSIZE_16,
            .value  = BTP_SDP_ATTR_RECORD_HANDLE,
            .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
            .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
            .dtype  = BTP_SDP_DTYPE_UINT,
            .dsize  = BTP_SDP_DSIZE_32,
            .value  = BTP_SDP_IAP_HANDLE,
            .pValue = (unsigned char *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
            .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
            .dtype  = BTP_SDP_DTYPE_UINT,
            .dsize  = BTP_SDP_DSIZE_16,
            .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
            .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
            .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
            .dtype  = BTP_SDP_DTYPE_UUID,
            .dsize  = BTP_SDP_DSIZE_128,
            .value  = 0,
            .pValue = (unsigned char *)&scBtpServiceIap2UuidL[0]},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
            .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
            .dtype  = BTP_SDP_DTYPE_UINT,
            .dsize  = BTP_SDP_DSIZE_16,
            .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
            .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
            .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
            .dtype  = BTP_SDP_DTYPE_UUID,
            .dsize  = BTP_SDP_DSIZE_16,
            .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
            .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
            .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
            .dtype  = BTP_SDP_DTYPE_UUID,
            .dsize  = BTP_SDP_DSIZE_16,
            .value  = BTP_SDP_PROTOCOL_RFCOMM_UUID,
            .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
            .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
            .dtype  = BTP_SDP_DTYPE_UINT,
            .dsize  = BTP_SDP_DSIZE_8,
            .value  = TLKBT_CFG_IAP_RFC_CHANNEL,
            .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
            .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
            .dtype  = BTP_SDP_DTYPE_UINT,
            .dsize  = BTP_SDP_DSIZE_16,
            .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
            .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
            .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
            .dtype  = BTP_SDP_DTYPE_UINT,
            .dsize  = BTP_SDP_DSIZE_VAR_16,
            .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
            .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x656e, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
            .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
            .dtype  = BTP_SDP_DTYPE_UINT,
            .dsize  = BTP_SDP_DSIZE_16,
            .value  = BTP_SDP_PRIMARY_LANG_BASE,
            .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
            .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
            .dtype  = BTP_SDP_DTYPE_NULL,
            .dsize  = BTP_SDP_DSIZE_NULL,
            .value  = 0,
            .pValue = (unsigned char *)0},

    // Service Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)scBtpServiceIap2ServiceName},

    // Provider Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)gcBthSdpProviderName},
};

const btp_sdp_serviceItem_t gcBtpBrowseGroupServiceItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_BROWSE_GROUP_HANDLE,
     .pValue = (unsigned char *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_PUBLIC_BROWSE_GROUP,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Provider Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0200, .dtype = BTP_SDP_DTYPE_UUID, .dsize = BTP_SDP_DSIZE_16, .value = 0x0200, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0200, .dtype = BTP_SDP_DTYPE_UUID, .dsize = BTP_SDP_DSIZE_16, .value = 0x1234, .pValue = (unsigned char *)0},
};

const btp_sdp_serviceItem_t gcBtpHeadsetServiceItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_HFP_HEADSET_HANDLE,
     .pValue = (unsigned char *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_HEADSET,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_GENERIC_AUDIO,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_RFCOMM_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_8, .value = 0x02, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_HEADSET,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Service Availability
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = 255,
     .pValue = (unsigned char *)&gcBthSdpServiceAvalibility},
};

#if (TLKBTP_CFG_PBAPCLT_ENABLE)
const btp_sdp_serviceItem_t gcBthSdpPbapClientItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0}, // handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_PBAP_PCE_HANDLE,
     .pValue = (uint8_t *)0}, // handle

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_PBAP_PCE,
     .pValue = (uint8_t *)0}, // list item[0]
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    // Protocol #0
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = L2CAP
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    // Protocol #1
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_RFCOMM_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = BNEP
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_8, .value = 0x09, .pValue = (uint8_t *)0}, // list item[1]
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    // Protocol #2
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_OBEX_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = BNEP
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Service Name
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_NAME,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_NAME,
     .pValue = (uint8_t *)0}, // service name
    {.flag   = BTP_SDP_FLAG_STR,
     .uuid   = BTP_SDP_ATTR_SERVICE_NAME,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)gcBthSdpPbapClientName}, // service name string

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_PBAP,
     .pValue = (uint8_t *)0}, // list item[0] UUID = L2CAP
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0x0102,
     .pValue = (uint8_t *)0}, // list item[0] VERSION = 0x0100
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
};
#endif

#if (TLKBTP_CFG_PBAPSRV_ENABLE)
const btp_sdp_serviceItem_t gcBthSdpPbapServiceItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0}, // handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_PBAP_PSE_HANDLE,
     .pValue = (uint8_t *)0}, // handle

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_PBAP_PSE,
     .pValue = (uint8_t *)0}, // list item[0]
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    // Protocol #0
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = L2CAP
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    // Protocol #1
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_RFCOMM_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = BNEP
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = 0x02,
     .pValue = (uint8_t *)0}, // list item[1] VERSION = 0x0100
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    // Protocol #2
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_OBEX_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = BNEP
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Service Name
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_NAME,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_NAME,
     .pValue = (uint8_t *)0}, // service name
    {.flag   = BTP_SDP_FLAG_STR,
     .uuid   = BTP_SDP_ATTR_SERVICE_NAME,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)gcBthSdpPbapServiceName}, // service name string

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_PBAP_PSE,
     .pValue = (uint8_t *)0}, // list item[0] UUID = L2CAP
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0x0102,
     .pValue = (uint8_t *)0}, // list item[0] VERSION = 0x0100
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Supported Repositories
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_REPOSITORIES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SUPPORTED_REPOSITORIES,
     .pValue = (uint8_t *)0}, // provider name
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_REPOSITORIES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = 0x03,
     .pValue = (uint8_t *)0}, // provider name string

    // PbapSupportedFeatures
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .pValue = (uint8_t *)0}, // provider name
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SUPPORTED_FEATURES,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = 0x03,
     .pValue = (uint8_t *)0}, // provider name string

};
#endif

const btp_sdp_serviceItem_t gcBtpSdpAttItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_ATT_HANDLE,
     .pValue = (unsigned char *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_GATT_SERVICE,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_ATT, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_ATT_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 11, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Browse Group List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
};

#if (TLKBTP_CFG_CUSTP_ENABLE)
const btp_sdp_serviceItem_t gcBthSdpCustpItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0}, // handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_CUSTP_HANDLE,
     .pValue = (uint8_t *)0}, // handle

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_CUSTP,
     .pValue = (uint8_t *)0}, // list item[0]
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = L2CAP
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_PSMID_PROP_SIGNAL,
     .pValue = (uint8_t *)0}, // list item[0] UUID = BNEP
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_CUSTP_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = BNEP
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0x0100,
     .pValue = (uint8_t *)0}, // list item[1] VERSION = 0x0100
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (uint8_t *)0}, // att id =5
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0x656e,
     .pValue = (uint8_t *)0}, // language ID 'e' 'en'
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (uint8_t *)0}, // UTF8
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE,
     .pValue = (uint8_t *)0}, // Attr base
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (uint8_t *)0}, // list end

    // Service Availability
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_SERVICE_AVAILABILITY, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0, .pValue = (uint8_t *)0}, // O

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_CUSTP,
     .pValue = (uint8_t *)0}, // list item[0] UUID = L2CAP
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0x0100,
     .pValue = (uint8_t *)0}, // list item[0] VERSION = 0x0100
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Service Name
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_NAME,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_NAME,
     .pValue = (uint8_t *)0}, // service name
    {.flag   = BTP_SDP_FLAG_STR,
     .uuid   = BTP_SDP_ATTR_SERVICE_NAME,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)gcBthSdpCustpServiceName}, // service name string

    // Provider Name
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SVCDESC_PRIMARY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SVCDESC_PRIMARY,
     .pValue = (uint8_t *)0}, // provider name
    {.flag   = BTP_SDP_FLAG_STR,
     .uuid   = BTP_SDP_ATTR_SVCDESC_PRIMARY,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)gcBthSdpCustpServiceDesc}, // provider name string

    // Security Description
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SECURITY_DESC,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SECURITY_DESC,
     .pValue = (uint8_t *)0}, // Security Description
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SECURITY_DESC,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0x0000,
     .pValue = (uint8_t *)0} // Security Description
};
#endif

#if (TLKBTBLE_CFG_MICS_ENABLE)
const btp_sdp_serviceItem_t gcBthSdpMicsItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0}, // handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_MICS_HANDLE,
     .pValue = (uint8_t *)0}, // handle

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_MICS_SERVICE,
     .pValue = (uint8_t *)0}, // list item[0]
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    // Protocol #0
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = L2CAP
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_ATT, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    // Protocol #1
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_ATT_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // BrowseGroupList
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (uint8_t *)0}, // att id =5
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT,
     .pValue = (uint8_t *)0}, // PublicBrowseRoot
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
};
#endif // #if (TLKBTBLE_CFG_MICS_ENABLE)

#if (TLKBTBLE_CFG_MCS_ENABLE)
const btp_sdp_serviceItem_t gcBthSdpMicsItem[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0}, // handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_MCS_HANDLE,
     .pValue = (uint8_t *)0}, // handle

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin: sv class
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_GEN_MCS_SERVICE,
     .pValue = (uint8_t *)0}, // list item[0]
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_MCS_SERVICE,
     .pValue = (uint8_t *)0}, // list item[0]
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    // Protocol #0
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = L2CAP
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_ATT, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    // Protocol #1
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_ATT_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end

    // Additional Protocol Descriptor List C.2
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .pValue = (uint8_t *)0},
    //Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list
    // Protocol #0
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (uint8_t *)0}, // list item[0] UUID = L2CAP
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_EATT, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    // Protocol #1
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_ATT_UUID,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin protocol list

    // BrowseGroupList
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (uint8_t *)0}, // att id =5
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list begin
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT,
     .pValue = (uint8_t *)0}, // PublicBrowseRoot
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)0}, // list end
};
#endif // #if (TLKBTBLE_CFG_MCS_ENABLE)

const btp_sdp_serviceList_t scBthSdpServiceServerList[BTP_SDP_SRV_MAX_NUMB] = {
#if (TLKBTP_CFG_HID_ENABLE)
    //  Note:If the gcBthSdpPnpInfoItem open will cause the snk's audio path is the mobile phone,not the device.
    {BTP_SDP_PNP_INFO_HANDLE, sizeof(gcBthSdpPnpInfoItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpPnpInfoItem},

    //{BTP_SDP_PNP_INFO_HANDLE, sizeof(gcBthSdpPnpInfoItem)/sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t
    //*)gcBthSdpPnpInfoItem},
    {BTP_SDP_HID_DEV_HANDLE, sizeof(gcBtpSdpHidItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpSdpHidItem},
#endif

#if (TLKBTP_CFG_SPP_ENABLE)
    {BTP_SDP_SPP_HANDLE, sizeof(gcBthSdpSppItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpSppItem},
#if (TLKBTP_CFG_SPP_USER_ENABLE)
    {BTP_SDP_SPP_USER_HANDLE, sizeof(gcBthSdpSppUserItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpSppUserItem},
#endif
#endif

#if (TLKBTP_CFG_HFPAG_ENABLE)
    {BTP_SDP_HFP_AG_HANDLE, sizeof(gcBthSdpHfpAgItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpHfpAgItem},
#endif

#if (TLKBTP_CFG_A2DPSRC_ENABLE)
    {BTP_SDP_A2DP_SRC_HANDLE, sizeof(gcBthSdpA2dpSrcItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpA2dpSrcItem},
#endif

#if (TLKBTP_CFG_AVRCP_ENABLE)
    {BTP_SDP_AVRCP_CT_HANDLE, sizeof(gcBthSdpAvrcpCtItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpAvrcpCtItem},
#endif

    {0xffffffff, 0, (btp_sdp_serviceItem_t *)0}};

const btp_sdp_serviceList_t scBthSdpServiceClientList[BTP_SDP_SRV_MAX_NUMB] = {
#if (TLKBTP_CFG_HID_ENABLE)
    //  Note:If the gcBthSdpPnpInfoItem open will cause the snk's audio path is the mobile phone,not the device.
    {BTP_SDP_PNP_INFO_HANDLE, sizeof(gcBthSdpPnpInfoItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpPnpInfoItem},

    //{BTP_SDP_PNP_INFO_HANDLE, sizeof(gcBthSdpPnpInfoItem)/sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t
    //*)gcBthSdpPnpInfoItem},
    {BTP_SDP_HID_DEV_HANDLE, sizeof(gcBtpSdpHidItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpSdpHidItem},
#endif

#if (TLKBTP_CFG_SPP_ENABLE)
    {BTP_SDP_SPP_HANDLE, sizeof(gcBthSdpSppItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpSppItem},
#if (TLKBTP_CFG_SPP_USER_ENABLE)
    {BTP_SDP_SPP_USER_HANDLE, sizeof(gcBthSdpSppUserItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpSppUserItem},
#endif
#endif

#if (TLKBTP_CFG_HFPHF_ENABLE)
    {BTP_SDP_HFP_HF_HANDLE, sizeof(gcBthSdpHfpHfItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpHfpHfItem},
#endif

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
    {BTP_SDP_A2DP_SNK_HANDLE, sizeof(gcBthSdpA2dpSnkItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpA2dpSnkItem},
#endif

#if (TLKBTP_CFG_AVRCP_ENABLE)
    {BTP_SDP_AVRCP_TG_HANDLE, sizeof(gcBthSdpAvrcpTgItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpAvrcpTgItem},
#endif

#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    {BTP_SDP_BROWSE_GROUP_HANDLE, sizeof(gcBtpBrowseGroupServiceItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpBrowseGroupServiceItem},
#endif

#if (TLKBTP_CFG_IAP_ENABLE)
    {BTP_SDP_IAP_HANDLE, sizeof(gcBtpIap2ServiceItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpIap2ServiceItem},
#endif

#if (TLKBTP_CFG_ATTSRV_ENABLE)
    {BTP_SDP_ATT_HANDLE, sizeof(gcBtpSdpAttItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpSdpAttItem},
#endif

    {0xffffffff, 0, (btp_sdp_serviceItem_t *)0}};


// Note: BTP_SDP_BUFFER_SIZE.
const btp_sdp_serviceList_t scBthSdpServiceList[BTP_SDP_SRV_MAX_NUMB] = {
#if (TLKBTP_CFG_HID_ENABLE)
    //  Note:If the gcBthSdpPnpInfoItem open will cause the snk's audio path is the mobile phone,not the device.
    {BTP_SDP_PNP_INFO_HANDLE, sizeof(gcBthSdpPnpInfoItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpPnpInfoItem},

    //{BTP_SDP_PNP_INFO_HANDLE, sizeof(gcBthSdpPnpInfoItem)/sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t
    //*)gcBthSdpPnpInfoItem},
    {BTP_SDP_HID_DEV_HANDLE, sizeof(gcBtpSdpHidItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpSdpHidItem},
#endif

#if (TLKBTP_CFG_SPP_ENABLE)
    {BTP_SDP_SPP_HANDLE, sizeof(gcBthSdpSppItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpSppItem},
#if (TLKBTP_CFG_SPP_USER_ENABLE)
    {BTP_SDP_SPP_USER_HANDLE, sizeof(gcBthSdpSppUserItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpSppUserItem},
#endif
#endif

#if (TLKBTP_CFG_HFPHF_ENABLE)
    {BTP_SDP_HFP_HF_HANDLE, sizeof(gcBthSdpHfpHfItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpHfpHfItem},
#endif
#if (TLKBTP_CFG_HFPAG_ENABLE)
    {BTP_SDP_HFP_AG_HANDLE, sizeof(gcBthSdpHfpAgItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpHfpAgItem},
#endif

#if (TLKBTP_CFG_A2DPSRC_ENABLE)
    {BTP_SDP_A2DP_SRC_HANDLE, sizeof(gcBthSdpA2dpSrcItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpA2dpSrcItem},
#endif
#if (TLKBTP_CFG_A2DPSNK_ENABLE)
    {BTP_SDP_A2DP_SNK_HANDLE, sizeof(gcBthSdpA2dpSnkItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpA2dpSnkItem},
#endif

#if (TLKBTP_CFG_AVRCP_ENABLE)
    {BTP_SDP_AVRCP_TG_HANDLE, sizeof(gcBthSdpAvrcpTgItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpAvrcpTgItem},
    {BTP_SDP_AVRCP_CT_HANDLE, sizeof(gcBthSdpAvrcpCtItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpAvrcpCtItem},
#endif
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    {BTP_SDP_BROWSE_GROUP_HANDLE, sizeof(gcBtpBrowseGroupServiceItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpBrowseGroupServiceItem},
#endif

#if (TLKBTP_CFG_IAP_ENABLE)
    {BTP_SDP_IAP_HANDLE, sizeof(gcBtpIap2ServiceItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpIap2ServiceItem},
#endif

#if (TLKBTP_CFG_ATTSRV_ENABLE)
    {BTP_SDP_ATT_HANDLE, sizeof(gcBtpSdpAttItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpSdpAttItem},
#endif

#if (TLKBTP_CFG_CUSTP_ENABLE)
    {BTP_SDP_CUSTP_HANDLE, sizeof(gcBthSdpCustpItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpCustpItem},
#endif

    {0xffffffff, 0, (btp_sdp_serviceItem_t *)0}};

extern void btp_sdpsrv_setList(const btp_sdp_serviceList_t *cpList);
extern void btp_sdp_setCacheBuffer(uint8_t *pBuffer, uint16_t buffLen);

void btp_sdp_set_spp_user_srv_class_id(uint8_t spp_srv_user_class_id[16])
{
    tmemcpy(gBthSrvSppUserClassId, spp_srv_user_class_id, 16);
}

void btp_sdp_constInit(void)
{
    btp_sdpsrv_setList(scBthSdpServiceClientList);
}

void btp_sdp_set_server_list_by_peer_type(uint8_t peer_dtype)
{
    (void)peer_dtype;
    btp_sdpsrv_setList(scBthSdpServiceList);
}

#if (TLK_CFG_HID_XINPUT_MODE_ENABLE)

#define BTP_SDP_VID_XINPUT 0x045E
static const char gcBtpSdpHidServiceName_xinput[]  = "Xbox Bluttooth Gamepad";
static const char gcBtpSdpHidProviderName_xinput[] = "Microsoft Corporation";

uint8_t       primary_record[1]             = {0x01};
const uint8_t gcBtpSdpHidReportMap_xinput[] = {
    0x05,
    0x01, // Usage Page (Generic Desktop)
    0x09,
    0x05, // Usage (Game Pad)
    0xA1,
    0x01, // Collection (Application)

    // 报告ID（XInput 固定为1）
    0x85,
    0x01, //   Report ID (1)

    // 左摇杆（X/Y轴）
    0x09,
    0x01, //   Usage (Pointer)
    0xA1,
    0x00, //   Collection (Physical)
    0x09,
    0x30, //     Usage (X)
    0x09,
    0x31, //     Usage (Y)
    0x15,
    0x00, //     Logical Minimum (0)
    0x27,
    0xFF,
    0xFF,
    0x00,
    0x00, // Logical Maximum (65535)
    0x95,
    0x02, //     Report Count (2)
    0x75,
    0x10, //     Report Size (16 bits)
    0x81,
    0x02, //     Input (Data,Var,Abs)
    0xC0, //   End Collection

    // 右摇杆（Rx/Ry轴）
    0x09,
    0x01, //   Usage (Pointer)
    0xA1,
    0x00, //   Collection (Physical)
    0x09,
    0x33, //     Usage (Rx)
    0x09,
    0x34, //     Usage (Ry)
    0x15,
    0x00, //     Logical Minimum (0)
    0x27,
    0xFF,
    0xFF,
    0x00,
    0x00, // Logical Maximum (65535)
    0x95,
    0x02, //     Report Count (2)
    0x75,
    0x10, //     Report Size (16 bits)
    0x81,
    0x02, //     Input (Data,Var,Abs)
    0xC0, //   End Collection

    // 扳机键（Z轴）
    0x05,
    0x01, //   Usage Page (Generic Desktop)
    0x09,
    0x32, //   Usage (Z)
    0x15,
    0x00, //     Logical Minimum (0)
    0x26,
    0xFF,
    0x03, //     Logical Maximum (1023)
    0x95,
    0x01, //     Report Count (1)
    0x75,
    0x0A, //     Report Size (10 bits)
    0x81,
    0x02, //     Input (Data,Var,Abs)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x00, //     Logical Maximum (0)
    0x75,
    0x06, //     Report Size (6 bits)
    0x95,
    0x01, //     Report Count (1)
    0x81,
    0x03, //     Input (Const,Var,Abs)

    // 右扳机键（Rz轴）
    0x05,
    0x01, //   Usage Page (Generic Desktop)
    0x09,
    0x35, //   Usage (Rz)
    0x15,
    0x00, //     Logical Minimum (0)
    0x26,
    0xFF,
    0x03, //     Logical Maximum (1023)
    0x95,
    0x01, //     Report Count (1)
    0x75,
    0x0A, //     Report Size (10 bits)
    0x81,
    0x02, //     Input (Data,Var,Abs)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x00, //     Logical Maximum (0)
    0x75,
    0x06, //     Report Size (6 bits)
    0x95,
    0x01, //     Report Count (1)
    0x81,
    0x03, //     Input (Const,Var,Abs)

    // 方向键（Hat Switch）
    0x05,
    0x01, //   Usage Page (Generic Desktop)
    0x09,
    0x39, //   Usage (Hat Switch)
    0x15,
    0x01, //     Logical Minimum (1)
    0x25,
    0x08, //     Logical Maximum (8)
    0x35,
    0x00, //     Physical Minimum (0)
    0x46,
    0x3B,
    0x01, //     Physical Maximum (315)
    0x66,
    0x14,
    0x00, //     Unit (Degrees)
    0x75,
    0x04, //     Report Size (4 bits)
    0x95,
    0x01, //     Report Count (1)
    0x81,
    0x42, //     Input (Data,Var,Abs,Null)
    0x75,
    0x04, //     Report Size (4 bits)
    0x95,
    0x01, //     Report Count (1)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x00, //     Logical Maximum (0)
    0x35,
    0x00, //     Physical Minimum (0)
    0x45,
    0x00, //     Physical Maximum (0)
    0x65,
    0x00, //     Unit (None)
    0x81,
    0x03, //     Input (Const,Var,Abs)

    // 按钮（A/B/X/Y等）
    0x05,
    0x09, //   Usage Page (Button)
    0x19,
    0x01, //     Usage Minimum (Button 1)
    0x29,
    0x0A, //     Usage Maximum (Button 10)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x01, //     Logical Maximum (1)
    0x75,
    0x01, //     Report Size (1 bit)
    0x95,
    0x0A, //     Report Count (10)
    0x81,
    0x02, //     Input (Data,Var,Abs)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x00, //     Logical Maximum (0)
    0x75,
    0x06, //     Report Size (6 bits)
    0x95,
    0x01, //     Report Count (1)
    0x81,
    0x03, //     Input (Const,Var,Abs)

    // 系统按钮（Guide键）
    0x05,
    0x01, //   Usage Page (Generic Desktop)
    0x09,
    0x80, //   Usage (System Control)
    0x85,
    0x02, //   Report ID (2)
    0xA1,
    0x00, //   Collection (Physical)
    0x09,
    0x85, //     Usage (System Menu)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x01, //     Logical Maximum (1)
    0x95,
    0x01, //     Report Count (1)
    0x75,
    0x01, //     Report Size (1 bit)
    0x81,
    0x02, //     Input (Data,Var,Abs)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x00, //     Logical Maximum (0)
    0x75,
    0x07, //     Report Size (7 bits)
    0x95,
    0x01, //     Report Count (1)
    0x81,
    0x03, //     Input (Const,Var,Abs)
    0xC0, //   End Collection

    // 输出报告（震动反馈）
    0x05,
    0x0F, //   Usage Page (Physical Interface)
    0x09,
    0x21, //   Usage (Battery Strength)
    0x85,
    0x03, //   Report ID (3)
    0xA1,
    0x02, //   Collection (Logical)
    0x09,
    0x97, //     Usage (Vibrate)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x01, //     Logical Maximum (1)
    0x75,
    0x04, //     Report Size (4 bits)
    0x95,
    0x01, //     Report Count (1)
    0x91,
    0x02, //     Output (Data,Var,Abs)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x00, //     Logical Maximum (0)
    0x75,
    0x04, //     Report Size (4 bits)
    0x95,
    0x01, //     Report Count (1)
    0x91,
    0x03, //     Output (Const,Var,Abs)
    0x09,
    0x70, //     Usage (Amplitude)
    0x15,
    0x00, //     Logical Minimum (0)
    0x25,
    0x64, //     Logical Maximum (100)
    0x75,
    0x08, //     Report Size (8 bits)
    0x95,
    0x04, //     Report Count (4)
    0x91,
    0x02, //     Output (Data,Var,Abs)
    0x09,
    0x50, //     Usage (Frequency)
    0x66,
    0x01,
    0x10, //     Unit (Hz)
    0x55,
    0x0E, //     Unit Exponent (-2)
    0x15,
    0x00, //     Logical Minimum (0)
    0x26,
    0xFF,
    0x00, //     Logical Maximum (255)
    0x75,
    0x08, //     Report Size (8 bits)
    0x95,
    0x01, //     Report Count (1)
    0x91,
    0x02, //     Output (Data,Var,Abs)
    0x09,
    0xA7, //     Usage (Duration)
    0x15,
    0x00, //     Logical Minimum (0)
    0x26,
    0xFF,
    0x00, //     Logical Maximum (255)
    0x75,
    0x08, //     Report Size (8 bits)
    0x95,
    0x01, //     Report Count (1)
    0x91,
    0x02, //     Output (Data,Var,Abs)
    0x65,
    0x00, //     Unit (None)
    0x55,
    0x00, //     Unit Exponent (0)
    0x09,
    0x7C, //     Usage (Custom Force)
    0x15,
    0x00, //     Logical Minimum (0)
    0x26,
    0xFF,
    0x00, //     Logical Maximum (255)
    0x75,
    0x08, //     Report Size (8 bits)
    0x95,
    0x01, //     Report Count (1)
    0x91,
    0x02, //     Output (Data,Var,Abs)
    0xC0, //   End Collection

    // 其他功能
    0x85,
    0x04, //   Report ID (4)
    0x05,
    0x06, //   Usage Page (Generic Device Controls)
    0x09,
    0x20, //   Usage (Battery Strength)
    0x15,
    0x00, //     Logical Minimum (0)
    0x26,
    0xFF,
    0x00, //     Logical Maximum (255)
    0x75,
    0x08, //     Report Size (8 bits)
    0x95,
    0x01, //     Report Count (1)
    0x81,
    0x02, //     Input (Data,Var,Abs)
    0xC0,

};


const btp_sdp_serviceItem_t gcBtpSdpHidItem_xinput[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_HID_DEV_HANDLE,
     .pValue = (unsigned char *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_HID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    // Protocol Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_PSMID_HIDC, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_HIDP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x656e, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (unsigned char *)0},

    // Additional Protocol Descriptor Lists
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_L2CAP_UUID,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_PSMID_HIDI,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PROTOCOL_HIDP_UUID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_ADD_PROTO_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Service Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0100, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)gcBtpSdpHidServiceName_xinput},

    // Provider Name
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_STR, .uuid = 0x0102, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)gcBtpSdpHidProviderName_xinput},

    // Bluetooth Profile Descriptor List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_PROFILE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_HID,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0101, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_PROFILE_DESC_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (unsigned char *)0},

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x656e, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_E,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (unsigned char *)0},

    // HID Parser Version
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SVCDB_STATE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SVCDB_STATE,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_SVCDB_STATE, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0111, .pValue = (unsigned char *)0},

    // HID Device Subclass
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0202, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_DEVICE_SUBCLASS, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0202, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_8, .value = scBtpSdpHidSubClass, .pValue = (unsigned char *)0},

    // HIDCountryCode
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0203, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_COUNTRY_CODE, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0203, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_8, .value = 0x21, .pValue = (unsigned char *)0},

    // HIDVirtualCable
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0204, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_VIRTUAL_CABLE, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0204, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = 0x01, .pValue = (unsigned char *)0},

    // HIDReconnectInitiate
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0205, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_RECONNECT_INITIATE, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0205, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = 0x01, .pValue = (unsigned char *)0},

    // HIDNormallyConnectable
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = 0x0205,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_HID_NORMALLY_CONNECTABLE,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0205, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = TRUE, .pValue = (unsigned char *)0},

    // HIDSDPDISABLE
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0205, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_SDP_DISABLE, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0205, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = FALSE, .pValue = (unsigned char *)0},

    // HIDDescriptorList
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0206, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_DESCRIPTOR_LIST, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = 0x0206,
     .dtype  = BTP_SDP_DTYPE_DES,
     .dsize  = sizeof(gcBtpSdpHidReportMap_xinput) > 0xFF ? BTP_SDP_DSIZE_VAR_16 : BTP_SDP_DSIZE_VAR_8,
     .value  = sizeof(gcBtpSdpHidReportMap_xinput) + 8,
     .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = 0x0206,
     .dtype  = BTP_SDP_DTYPE_DES,
     .dsize  = sizeof(gcBtpSdpHidReportMap_xinput) > 0xFF ? BTP_SDP_DSIZE_VAR_16 : BTP_SDP_DSIZE_VAR_8,
     .value  = sizeof(gcBtpSdpHidReportMap_xinput) + 5,
     .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0206, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_8, .value = 0x22, .pValue = (unsigned char *)0},
    {.flag   = BTP_SDP_FLAG_HID_DES,
     .uuid   = 0x0206,
     .dtype  = BTP_SDP_DTYPE_STRING,
     .dsize  = sizeof(gcBtpSdpHidReportMap_xinput),
     .value  = 0x00,
     .pValue = (unsigned char *)gcBtpSdpHidReportMap_xinput},
    // HIDLANGIDBaseList
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0207, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_LANG_ID_BASE_LIST, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0207, .dtype = BTP_SDP_DTYPE_DES, .dsize = BTP_SDP_DSIZE_VAR_8, .value = 0x08, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0207, .dtype = BTP_SDP_DTYPE_DES, .dsize = BTP_SDP_DSIZE_VAR_8, .value = 0x06, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0207, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0409, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x0207, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0100, .pValue = (unsigned char *)0},

    // HIDBootDevice
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x020e, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_HID_BOOT_DEVICE, .pValue = (unsigned char *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0x020e, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = 0x00, .pValue = (unsigned char *)0},
};

const btp_sdp_serviceItem_t gcBthSdpPnpInfoItemXinput[] = {
    // Service Record Handle
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_HANDLE,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_HANDLE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_32,
     .value  = BTP_SDP_PNP_INFO_HANDLE,
     .pValue = (uint8_t *)0},

    // Service Class ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SRV_CLASS_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_SRVCLASS_ID_PNP_INFO,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_SRV_CLASS_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Record State
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_RECORD_STATE,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_RECORD_STATE,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_RECORD_STATE, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_32, .value = 0x00000304, .pValue = (uint8_t *)0},

    // Browse Group List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_S, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_BROWSE_GROUP_LIST,
     .dtype  = BTP_SDP_DTYPE_UUID,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_BROWSE_GROUP_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Language Base Attribute ID List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT_LIST_S,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_VAR_16,
     .value  = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x656e, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x6672, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE + 0x0010,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x6465, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE + 0x0020,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x6A61, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 106, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_PRIMARY_LANG_BASE + 0x0030,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT_LIST_E, .uuid = BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST, .dtype = BTP_SDP_DTYPE_NULL, .dsize = BTP_SDP_DSIZE_NULL, .value = 0, .pValue = (uint8_t *)0},

    // Service Availability
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT | BTP_SDP_FLAG_W,
     .uuid   = BTP_SDP_ATTR_SERVICE_AVAILABILITY,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_8,
     .value  = 0,
     .pValue = (uint8_t *)&gcBthSdpServiceAvalibility},

    // Service Description List
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_SERVICE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SERVICE_DESC_LIST,
     .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_STR,
     .uuid   = BTP_SDP_ATTR_SERVICE_DESC_LIST,
     .dtype  = BTP_SDP_DTYPE_NULL,
     .dsize  = BTP_SDP_DSIZE_NULL,
     .value  = 0,
     .pValue = (uint8_t *)gcBthSdpPnpInfoServiceDesc},

    // Group ID
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_GROUP_ID,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_SPECIFICATION_ID,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0102, .pValue = (uint8_t *)0},

    // Vendor ID
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 1, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_VENDOR_ID, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 1, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_VID_XINPUT, .pValue = (uint8_t *)0},

    // Product ID
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 2, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_PRODUCT_ID, .pValue = (uint8_t *)0},
#if BTP_SDP_AIRPODS_ENABLE
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 2, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x2002, .pValue = (uint8_t *)0},
#else
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 2, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x02E0, .pValue = (uint8_t *)0},
#endif

    // Version
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 3, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = BTP_SDP_ATTR_VERSION, .pValue = (uint8_t *)0},
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_GROUP_ID + 3,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = 0x0903, //0x0509
     .pValue = (uint8_t *)0},

    // Primary Record
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_GROUP_ID + 4,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_PRIMARY_RECORD,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 4, .dtype = BTP_SDP_DTYPE_BOOL, .dsize = BTP_SDP_DSIZE_8, .value = 0x01, .pValue = (uint8_t *)primary_record},

    // Vendor ID Source
    {.flag   = BTP_SDP_FLAG_ATT,
     .uuid   = BTP_SDP_ATTR_GROUP_ID + 5,
     .dtype  = BTP_SDP_DTYPE_UINT,
     .dsize  = BTP_SDP_DSIZE_16,
     .value  = BTP_SDP_ATTR_VENDOR_ID_SOURCE,
     .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = BTP_SDP_ATTR_GROUP_ID + 5, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0002, .pValue = (uint8_t *)0},

    // Custom Attributes
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0xa000, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0xa000, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0xa000, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_32, .value = 0x080040ff, .pValue = (uint8_t *)0},

    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0xafff, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0xafff, .pValue = (uint8_t *)0},
    {.flag = BTP_SDP_FLAG_ATT, .uuid = 0xafff, .dtype = BTP_SDP_DTYPE_UINT, .dsize = BTP_SDP_DSIZE_16, .value = 0x0001, .pValue = (uint8_t *)0},
};

const btp_sdp_serviceList_t scBthSdpServiceList_xinput[BTP_SDP_SRV_MAX_NUMB] = {
#if (TLKBTP_CFG_HID_ENABLE)
    //  Note:If the gcBthSdpPnpInfoItem open will cause the snk's audio path is the mobile phone,not the device.
    {BTP_SDP_PNP_INFO_HANDLE, sizeof(gcBthSdpPnpInfoItemXinput) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpPnpInfoItemXinput},

    {BTP_SDP_HID_DEV_HANDLE, sizeof(gcBtpSdpHidItem_xinput) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpSdpHidItem_xinput},
#endif

#if (TLKBTP_CFG_SPP_ENABLE)
    {BTP_SDP_SPP_HANDLE, sizeof(gcBthSdpSppItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpSppItem},
#if (TLKBTP_CFG_SPP_USER_ENABLE)
    {BTP_SDP_SPP_USER_HANDLE, sizeof(gcBthSdpSppUserItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpSppUserItem},
#endif
#endif

#if (TLKBTP_CFG_HFPHF_ENABLE)
    {BTP_SDP_HFP_HF_HANDLE, sizeof(gcBthSdpHfpHfItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpHfpHfItem},
#endif
#if (TLKBTP_CFG_HFPAG_ENABLE)
    {BTP_SDP_HFP_AG_HANDLE, sizeof(gcBthSdpHfpAgItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpHfpAgItem},
#endif

#if (TLKBTP_CFG_A2DPSRC_ENABLE)
    {BTP_SDP_A2DP_SRC_HANDLE, sizeof(gcBthSdpA2dpSrcItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpA2dpSrcItem},
#endif
#if (TLKBTP_CFG_A2DPSNK_ENABLE)
    {BTP_SDP_A2DP_SNK_HANDLE, sizeof(gcBthSdpA2dpSnkItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpA2dpSnkItem},
#endif

#if (TLKBTP_CFG_AVRCP_ENABLE)
    {BTP_SDP_AVRCP_TG_HANDLE, sizeof(gcBthSdpAvrcpTgItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpAvrcpTgItem},
    {BTP_SDP_AVRCP_CT_HANDLE, sizeof(gcBthSdpAvrcpCtItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBthSdpAvrcpCtItem},
#endif
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    {BTP_SDP_BROWSE_GROUP_HANDLE, sizeof(gcBtpBrowseGroupServiceItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpBrowseGroupServiceItem},
#endif

#if (TLKBTP_CFG_IAP_ENABLE)
    {BTP_SDP_IAP_HANDLE, sizeof(gcBtpIap2ServiceItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpIap2ServiceItem},
#endif

#if (TLKBTP_CFG_ATTSRV_ENABLE)
    {BTP_SDP_ATT_HANDLE, sizeof(gcBtpSdpAttItem) / sizeof(btp_sdp_serviceItem_t), (btp_sdp_serviceItem_t *)gcBtpSdpAttItem},
#endif

    {0xffffffff, 0, (btp_sdp_serviceItem_t *)0}};


void btp_sdp_set_server_mode(uint8_t mode)
{
    if (mode == 0x02) { // Xinput mode
        btp_sdpsrv_setList(scBthSdpServiceList_xinput);
    } else {
        btp_sdpsrv_setList(scBthSdpServiceClientList);
    }
}
#endif


#endif
