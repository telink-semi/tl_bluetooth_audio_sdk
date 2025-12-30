/********************************************************************************************************
 * @file    btp_sdpConst.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef BTP_SDP_CONST_H
#define BTP_SDP_CONST_H


#define BTP_SDP_AIRPODS_ENABLE      0

#define BTP_SDP_PROFILE_MAX_HANDLE  0x001FFFFF

#define BTP_SDP_BROWSE_GROUP_HANDLE 0x00100004


#define BTP_SDP_SPP_HANDLE          0x00100001
#define BTP_SDP_OPP_HANDLE          0x00100002
#define BTP_SDP_FTP_HANDLE          0x00100003
#define BTP_SDP_SERVICE_HANDLE      0x00100005
#define BTP_SDP_HID_DEV_HANDLE      0x00100006 //
#define BTP_SDP_SPP_USER_HANDLE     0x00100007

#define BTP_SDP_IAP_HANDLE          0x00100014

#define BTP_SDP_HFP_HEADSET_HANDLE  0x00100009
#define BTP_SDP_HFP_HF_HANDLE       0x00100010
#define BTP_SDP_HFP_AG_HANDLE       0x00100011
#define BTP_SDP_PNP_INFO_HANDLE     0x00100012
#define BTP_SDP_AVRCP_CT_HANDLE     0x00100015
#define BTP_SDP_AVRCP_TG_HANDLE     0x00100016
#define BTP_SDP_A2DP_SRC_HANDLE     0x00100017
#define BTP_SDP_A2DP_SNK_HANDLE     0x00100018
#define BTP_SDP_ATT_HANDLE          0x00100019
#define BTP_SDP_PBAP_PCE_HANDLE     0x0010001A  
#define BTP_SDP_PBAP_PSE_HANDLE     0x0010001B
#define BTP_SDP_MICS_HANDLE         0x0010101C
#define BTP_SDP_MCS_HANDLE          0x0010101D

#define BTP_SDP_CUSTP_HANDLE        0xF0101000


/*
 * macro used in sdp database
 */
#define BTP_SDP_HEADSET_VERSION    0x0100
#define BTP_SDP_OPP_PROTOCOL_VERSION    0x0100
#define BTP_SDP_FTP_PROTOCOL_VERSION    0x0100
#define BTP_SDP_DUN_PROTOCOL_VERSION    0x0100
#define BTP_SDP_A2DP_PROTOCOL_VERSION   0x0103
#define BTP_SDP_AVRCP_PROTOCOL_VERSION  0x0104
#define BTP_SDP_PNP_PROTOCOL_VERSION    0x0100

#define BTP_SDP_HFP_PROFILE_VERSION     0x0108
#define BTP_SDP_OPP_PROFILE_VERSION     0x0100
#define BTP_SDP_FTP_PROFILE_VERSION     0x0100
#define BTP_SDP_DUN_PROFILE_VERSION     0x0100
#define BTP_SDP_A2DP_PROFILE_ERSION     0x0103
#define BTP_SDP_AVRCP_PROFILE_VERSION   0x0106
#define BTP_SDP_PNP_PROFILE_VERSION     0x0100


#define BTP_SDP_SRV_MAX_NUMB       16

/*
 * Possible values for attribute-id are listed below.
 * See SDP Spec, section "Service Attribute Definitions" for more details.
 */
#define BTP_SDP_ATTR_RECORD_HANDLE               0x0000
#define BTP_SDP_ATTR_SRV_CLASS_ID_LIST           0x0001
#define BTP_SDP_ATTR_RECORD_STATE                0x0002
#define BTP_SDP_ATTR_SERVICE_ID                  0x0003
#define BTP_SDP_ATTR_PROTO_DESC_LIST             0x0004
#define BTP_SDP_ATTR_BROWSE_GROUP_LIST           0x0005
#define BTP_SDP_ATTR_LANG_BASE_ATTR_ID_LIST      0x0006
#define BTP_SDP_ATTR_SVCINFO_TTL                 0x0007
#define BTP_SDP_ATTR_SERVICE_AVAILABILITY        0x0008
#define BTP_SDP_ATTR_PROFILE_DESC_LIST           0x0009
#define BTP_SDP_ATTR_DOC_URL                     0x000a
#define BTP_SDP_ATTR_CLIENT_EXEC_URL             0x000b
#define BTP_SDP_ATTR_ICON_URL                    0x000c
#define BTP_SDP_ATTR_ADD_PROTO_DESC_LIST         0x000d
#define BTP_SDP_ATTR_SERVICE_NAME                0x0100
#define BTP_SDP_ATTR_SERVICE_DESC_LIST           0x0101

#define BTP_SDP_ATTR_GROUP_ID                    0x0200
#define BTP_SDP_ATTR_IP_SUBNET                   0x0200
#define BTP_SDP_ATTR_VERSION_NUM_LIST            0x0200
#define BTP_SDP_ATTR_SVCDB_STATE                 0x0201

#define BTP_SDP_ATTR_SERVICE_VERSION             0x0300
#define BTP_SDP_ATTR_EXTERNAL_NETWORK            0x0301
#define BTP_SDP_ATTR_SUPPORTED_DATA_STORES_LIST  0x0301
#define BTP_SDP_ATTR_FAX_CLASS1_SUPPORT          0x0302
#define BTP_SDP_ATTR_REMOTE_AUDIO_VOLUME_CONTROL 0x0302
#define BTP_SDP_ATTR_FAX_CLASS20_SUPPORT         0x0303
#define BTP_SDP_ATTR_SUPPORTED_FORMATS_LIST      0x0303
#define BTP_SDP_ATTR_FAX_CLASS2_SUPPORT          0x0304
#define BTP_SDP_ATTR_AUDIO_FEEDBACK_SUPPORT      0x0305
#define BTP_SDP_ATTR_NETWORK_ADDRESS             0x0306
#define BTP_SDP_ATTR_WAP_GATEWAY                 0x0307
#define BTP_SDP_ATTR_HOMEPAGE_URL                0x0308
#define BTP_SDP_ATTR_WAP_STACK_TYPE              0x0309
#define BTP_SDP_ATTR_SECURITY_DESC               0x030a
#define BTP_SDP_ATTR_NET_ACCESS_TYPE             0x030b
#define BTP_SDP_ATTR_MAX_NET_ACCESSRATE          0x030c
#define BTP_SDP_ATTR_IP4_SUBNET                  0x030d
#define BTP_SDP_ATTR_IP6_SUBNET                  0x030e
#define BTP_SDP_ATTR_SUPPORTED_CAPABILITIES      0x0310
#define BTP_SDP_ATTR_SUPPORTED_FEATURES          0x0311
#define BTP_SDP_ATTR_SUPPORTED_FUNCTIONS         0x0312
#define BTP_SDP_ATTR_TOTAL_IMAGING_DATA_CAPACITY 0x0313
#define BTP_SDP_ATTR_SUPPORTED_REPOSITORIES      0x0314

#define BTP_SDP_ATTR_SPECIFICATION_ID            0x0200
#define BTP_SDP_ATTR_VENDOR_ID                   0x0201
#define BTP_SDP_ATTR_PRODUCT_ID                  0x0202
#define BTP_SDP_ATTR_VERSION                     0x0203
#define BTP_SDP_ATTR_PRIMARY_RECORD              0x0204
#define BTP_SDP_ATTR_VENDOR_ID_SOURCE            0x0205

#define BTP_SDP_ATTR_HID_DEVICE_RELEASE_NUMBER   0x0200
#define BTP_SDP_ATTR_HID_PARSER_VERSION          0x0201
#define BTP_SDP_ATTR_HID_DEVICE_SUBCLASS         0x0202
#define BTP_SDP_ATTR_HID_COUNTRY_CODE            0x0203
#define BTP_SDP_ATTR_HID_VIRTUAL_CABLE           0x0204
#define BTP_SDP_ATTR_HID_RECONNECT_INITIATE      0x0205
#define BTP_SDP_ATTR_HID_DESCRIPTOR_LIST         0x0206
#define BTP_SDP_ATTR_HID_LANG_ID_BASE_LIST       0x0207
#define BTP_SDP_ATTR_HID_SDP_DISABLE             0x0208
#define BTP_SDP_ATTR_HID_BATTERY_POWER           0x0209
#define BTP_SDP_ATTR_HID_REMOTE_WAKEUP           0x020a
#define BTP_SDP_ATTR_HID_PROFILE_VERSION         0x020b
#define BTP_SDP_ATTR_HID_SUPERVISION_TIMEOUT     0x020c
#define BTP_SDP_ATTR_HID_NORMALLY_CONNECTABLE    0x020d
#define BTP_SDP_ATTR_HID_BOOT_DEVICE             0x020e

#define BTP_SDP_ATTR_PUBLIC_BROWSE_ROOT          0x1002


#define BTP_SDP_PRIMARY_LANG_BASE                0x0100

#define BTP_SDP_ATTR_SVCNAME_PRIMARY             (0x0000 + BTP_SDP_PRIMARY_LANG_BASE)
#define BTP_SDP_ATTR_SVCDESC_PRIMARY             (0x0001 + BTP_SDP_PRIMARY_LANG_BASE)
#define BTP_SDP_ATTR_PROVNAME_PRIMARY            (0x0002 + BTP_SDP_PRIMARY_LANG_BASE)


#define BTP_SDP_PROTOCOL_SDP_UUID                0x0001
#define BTP_SDP_PROTOCOL_UDP_UUID                0x0002
#define BTP_SDP_PROTOCOL_RFCOMM_UUID             0x0003
#define BTP_SDP_PROTOCOL_TCP_UUID                0x0004
#define BTP_SDP_PROTOCOL_TCS_BIN_UUID            0x0005
#define BTP_SDP_PROTOCOL_TCS_AT_UUID             0x0006
#define BTP_SDP_PROTOCOL_ATT_UUID                0x0007
#define BTP_SDP_PROTOCOL_OBEX_UUID               0x0008
#define BTP_SDP_PROTOCOL_IP_UUID                 0x0009
#define BTP_SDP_PROTOCOL_FTP_UUID                0x000a
#define BTP_SDP_PROTOCOL_HTTP_UUID               0x000c
#define BTP_SDP_PROTOCOL_WSP_UUID                0x000e
#define BTP_SDP_PROTOCOL_BNEP_UUID               0x000f
#define BTP_SDP_PROTOCOL_UPNP_UUID               0x0010
#define BTP_SDP_PROTOCOL_HIDP_UUID               0x0011
#define BTP_SDP_PROTOCOL_HCRP_CTRL_UUID          0x0012
#define BTP_SDP_PROTOCOL_HCRP_DATA_UUID          0x0014
#define BTP_SDP_PROTOCOL_HCRP_NOTE_UUID          0x0016
#define BTP_SDP_PROTOCOL_AVCTP_UUID              0x0017
#define BTP_SDP_PROTOCOL_AVDTP_UUID              0x0019
#define BTP_SDP_PROTOCOL_CMTP_UUID               0x001b
#define BTP_SDP_PROTOCOL_UDI_UUID                0x001d
#define BTP_SDP_PROTOCOL_L2CAP_UUID              0x0100
#define BTP_SDP_PROTOCOL_PnP_INFO_UUID           0x1200

#define BTP_SDP_PROTOCOL_CUSTP_UUID              0xF001


#define BTP_SDP_UUID16_HDR                       0x19
#define BTP_SDP_UUID32_HDR                       0x1a
#define BTP_SDP_UUID128_HDR                      0x1c


#define BTP_SDP_UUID16_HDR                       0x19
#define BTP_SDP_UUID32_HDR                       0x1a
#define BTP_SDP_UUID128_HDR                      0x1c

typedef enum
{
    BTP_SDP_FLAG_ATT_LIST_S   = 0x0002,
    BTP_SDP_FLAG_ATT_LIST_E   = 0x0004,
    BTP_SDP_FLAG_ATT          = 0x0008,
    BTP_SDP_FLAG_STR          = 0x0010,
    BTP_SDP_FLAG_URL          = 0x0020,
    BTP_SDP_FLAG_HID_DES      = 0x0040,
    BTP_SDP_FLAG_W            = 0x0100,
    BTP_SDP_FLAG_EXT_DATA_LEN = 0x0200,
} BTP_SDP_FLAGS_ENUM;

typedef enum
{
    BTP_SDP_DTYPE_NIL = 0,
    BTP_SDP_DTYPE_UINT,
    BTP_SDP_DTYPE_INT,
    BTP_SDP_DTYPE_UUID,
    BTP_SDP_DTYPE_STRING,
    BTP_SDP_DTYPE_BOOL,
    BTP_SDP_DTYPE_DES,
    BTP_SDP_DTYPE_DEA,
    BTP_SDP_DTYPE_URL,
    BTP_SDP_DTYPE_NULL,
} BTP_SDP_DTYPE_ENUM;

typedef enum
{
    BTP_SDP_DSIZE_8 = 0,
    BTP_SDP_DSIZE_16,
    BTP_SDP_DSIZE_32,
    BTP_SDP_DSIZE_64,
    BTP_SDP_DSIZE_128,
    BTP_SDP_DSIZE_VAR_8,
    BTP_SDP_DSIZE_VAR_16,
    BTP_SDP_DSIZE_VAR_32,
    BTP_SDP_DSIZE_NULL,
} BTP_SDP_DSIZE_ENUM;

typedef struct
{
    uint16_t  uuid;
    uint16_t  dtype; // Refer to BTP_SDP_DTYPE_ENUM
    uint16_t  dsize; // Refer to BTP_SDP_DSIZE_ENUM
    uint16_t  resv2byte;
    uint32_t  flag;
    uint32_t  value;
    uint8_t *pValue;
} btp_sdp_serviceItem_t;

typedef struct
{
    uint32_t                 handle;
    uint32_t                 count;
    btp_sdp_serviceItem_t *pItem;
} btp_sdp_serviceList_t;

void btp_sdp_set_spp_user_srv_class_id(uint8_t spp_srv_user_class_id[16]);
void btp_sdp_constInit(void);
void btp_sdp_set_server_list_by_peer_type(uint8_t peer_dtype);
void btp_sdp_set_server_mode(uint8_t mode);

#endif // BTP_SDP_CONST_H
