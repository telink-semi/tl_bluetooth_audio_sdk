/********************************************************************************************************
 * @file    btp_sdp.h
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
#ifndef BTP_SDP_H
#define BTP_SDP_H

#if (TLKBTP_CFG_SDP_ENABLE)

#define BTP_SDP_SRVCLASS_ID_SDP_SERVER          0x1000
#define BTP_SDP_SRVCLASS_ID_BROWSE_GRP_DESC     0x1001
#define BTP_SDP_SRVCLASS_ID_PUBLIC_BROWSE_GROUP 0x1002
#define BTP_SDP_SRVCLASS_ID_SERIAL_PORT         0x1101 // SPP
#define BTP_SDP_SRVCLASS_ID_LAN_ACCESS          0x1102
#define BTP_SDP_SRVCLASS_ID_DIALUP_NET          0x1103
#define BTP_SDP_SRVCLASS_ID_IRMC_SYNC           0x1104
#define BTP_SDP_SRVCLASS_ID_OBEX_OBJPUSH        0x1105
#define BTP_SDP_SRVCLASS_ID_OBEX_FILETRANS      0x1106
#define BTP_SDP_SRVCLASS_ID_IRMC_SYNC_CMD       0x1107
#define BTP_SDP_SRVCLASS_ID_HEADSET             0x1108
#define BTP_SDP_SRVCLASS_ID_CORDLESS_TELEPHONY  0x1109
#define BTP_SDP_SRVCLASS_ID_AUDIO_SOURCE        0x110a
#define BTP_SDP_SRVCLASS_ID_AUDIO_SINK          0x110b
#define BTP_SDP_SRVCLASS_ID_AV_REMOTE_TARGET    0x110c
#define BTP_SDP_SRVCLASS_ID_ADVANCED_AUDIO      0x110d
#define BTP_SDP_SRVCLASS_ID_AV_REMOTE           0x110e
#define BTP_SDP_SRVCLASS_ID_VIDEO_CONF          0x110f
#define BTP_SDP_SRVCLASS_ID_INTERCOM            0x1110
#define BTP_SDP_SRVCLASS_ID_FAX                 0x1111
#define BTP_SDP_SRVCLASS_ID_HEADSET_AGW         0x1112
#define BTP_SDP_SRVCLASS_ID_WAP                 0x1113
#define BTP_SDP_SRVCLASS_ID_WAP_CLIENT          0x1114
#define BTP_SDP_SRVCLASS_ID_PANU                0x1115
#define BTP_SDP_SRVCLASS_ID_NAP                 0x1116
#define BTP_SDP_SRVCLASS_ID_GN                  0x1117
#define BTP_SDP_SRVCLASS_ID_DIRECT_PRINTING     0x1118
#define BTP_SDP_SRVCLASS_ID_REFERENCE_PRINTING  0x1119
#define BTP_SDP_SRVCLASS_ID_IMAGING             0x111a
#define BTP_SDP_SRVCLASS_ID_IMAGING_RESPONDER   0x111b
#define BTP_SDP_SRVCLASS_ID_IMAGING_ARCHIVE     0x111c
#define BTP_SDP_SRVCLASS_ID_IMAGING_REFOBJS     0x111d
#define BTP_SDP_SRVCLASS_ID_HANDSFREE           0x111e
#define BTP_SDP_SRVCLASS_ID_HANDSFREE_AGW       0x111f
#define BTP_SDP_SRVCLASS_ID_DIRECT_PRT_REFOBJS  0x1120
#define BTP_SDP_SRVCLASS_ID_REFLECTED_UI        0x1121
#define BTP_SDP_SRVCLASS_ID_BASIC_PRINTING      0x1122
#define BTP_SDP_SRVCLASS_ID_PRINTING_STATUS     0x1123
#define BTP_SDP_SRVCLASS_ID_HID                 0x1124
#define BTP_SDP_SRVCLASS_ID_HCR                 0x1125
#define BTP_SDP_SRVCLASS_ID_HCR_PRINT           0x1126
#define BTP_SDP_SRVCLASS_ID_HCR_SCAN            0x1127
#define BTP_SDP_SRVCLASS_ID_CIP                 0x1128
#define BTP_SDP_SRVCLASS_ID_VIDEO_CONF_GW       0x1129
#define BTP_SDP_SRVCLASS_ID_UDI_MT              0x112a
#define BTP_SDP_SRVCLASS_ID_UDI_TA              0x112b
#define BTP_SDP_SRVCLASS_ID_AV                  0x112c
#define BTP_SDP_SRVCLASS_ID_SAP                 0x112d
#define BTP_SDP_SRVCLASS_ID_PBAP_PCE            0x112e
#define BTP_SDP_SRVCLASS_ID_PBAP_PSE            0x112f
#define BTP_SDP_SRVCLASS_ID_PBAP                0x1130
#define BTP_SDP_SRVCLASS_ID_PNP_INFO            0x1200
#define BTP_SDP_SRVCLASS_ID_GENERIC_NETWORKING  0x1201
#define BTP_SDP_SRVCLASS_ID_GENERIC_FILETRANS   0x1202
#define BTP_SDP_SRVCLASS_ID_GENERIC_AUDIO       0x1203
#define BTP_SDP_SRVCLASS_ID_GENERIC_TELEPHONY   0x1204
#define BTP_SDP_SRVCLASS_ID_UPNP                0x1205
#define BTP_SDP_SRVCLASS_ID_UPNP_IP             0x1206
#define BTP_SDP_SRVCLASS_ID_UPNP_PAN            0x1300
#define BTP_SDP_SRVCLASS_ID_UPNP_LAP            0x1301
#define BTP_SDP_SRVCLASS_ID_UPNP_L2CAP          0x1302
#define BTP_SDP_SRVCLASS_ID_VIDEO_SOURCE        0x1303
#define BTP_SDP_SRVCLASS_ID_VIDEO_SINK          0x1304
#define BTP_SDP_SRVCLASS_ID_VIDEO_DISTRIBUTION  0x1305

// Services by UUID, Last Modified: 2024-11-20, refer to <Assigned Numbers> page 69.
#define BTP_SDP_SRVCLASS_ID_GAP_SERVICE  0x1800
#define BTP_SDP_SRVCLASS_ID_GATT_SERVICE 0x1801
/* 0x1802 ... 0x1847 */
#define BTP_SDP_SRVCLASS_ID_MCS_SERVICE     0x1848 // Media Control Service
#define BTP_SDP_SRVCLASS_ID_GEN_MCS_SERVICE 0x1849 // Generic Media Control Service
#define BTP_SDP_SRVCLASS_ID_MICS_SERVICE    0x184D // Microphone Control Service
/* 0x184E ... 0x1859 */
#define BTP_SDP_SRVCLASS_ID_IMDS_SERVICE  0x185A // Industrial Measurement Device Service
#define BTP_SDP_SRVCLASS_ID_RANGS_SERVICE 0x185B // Ranging Service


#define BTP_SDP_SRVCLASS_ID_IAP2_TEMP     0xFFAC //{0x00, 0x00, 0x00, 0x00, 0xde,  0xCA, 0xFA, 0xDE, 0xDE, 0xCA, 0xDE, 0xAF, 0xDE, 0xCA, 0xCA, 0xFF}

#define BTP_SDP_SRVCLASS_ID_CUSTP         0xF0F0


#define BTP_SVC_USER_ID0                  0xff00
#define BTP_SVC_USER_ID1                  0xff01


#define BTP_SDPSRV_RSP_ATT_LEN            700
#define BTP_SDP_BUFFER_SIZE               1500

typedef enum
{
    BTP_SDP_PDUID_ERROR_RSP       = 0x01,
    BTP_SDP_PDUID_SEARCH_REQ      = 0x02,
    BTP_SDP_PDUID_SEARCH_RSP      = 0x03,
    BTP_SDP_PDUID_ATTR_REQ        = 0x04,
    BTP_SDP_PDUID_ATTR_RSP        = 0x05,
    BTP_SDP_PDUID_SEARCH_ATTR_REQ = 0x06,
    BTP_SDP_PDUID_SEARCH_ATTR_RSP = 0x07,
} BTP_SDP_PDUID_ENUM;

typedef enum
{
    BTP_SDP_ECODE_INVALID_SDP_VERSION    = 0x0001,
    BTP_SDP_ECODE_INVALID_RECORD_HANDLE  = 0x0002,
    BTP_SDP_ECODE_INVALID_REQUEST_SYNTAX = 0x0003,
    BTP_SDP_ECODE_INVALID_PDU_SIZE       = 0x0004,
    BTP_SDP_ECODE_INVALID_CONTINUE_STATE = 0x0005,
    BTP_SDP_ECODE_INSUFFICIENT_RESOURCES = 0x0006,
} BTP_SDP_ECODE_ENUM;

typedef enum
{
    BTP_SDP_BUSY_NONE                 = 0x00,
    BTP_SDP_BUSY_SEND_ERROR_RSP       = 0x01,
    BTP_SDP_BUSY_SEND_SEARCH_REQ      = 0x02,
    BTP_SDP_BUSY_SEND_SEARCH_RSP      = 0x04,
    BTP_SDP_BUSY_SEND_ATTR_REQ        = 0x08,
    BTP_SDP_BUSY_SEND_ATTR_RSP        = 0x10,
    BTP_SDP_BUSY_SEND_SEARCH_ATTR_REQ = 0x20,
    BTP_SDP_BUSY_SEND_SEARCH_ATTR_RSP = 0x40,
} BTP_SDP_BUSYS_ENUM;

/******************************************************************************
 * Function: SDP Init interface
 * Descript: This interface be used by User to initial sdp resource.
 * Params:
 *		 @count[IN]--refer to TLK_BT_SDP_MAX_NUMB.
 *		 @sdpsrvAttLen[IN]--The size of the data space when responding to the
 *           peer device as an SDP-SRV. Refer to BTP_SDPSRV_RSP_ATT_LEN.
 *       @isEnable[IN]--true: enable the share buffer.
 *                      false: disable the share buffer.
 *       @shareBufferSize[IN]--refer to BTP_SDP_BUFFER_SIZE.
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value is returned means the initial process fail.
 * Notice:
 *     1.Whether to enable SDP shared buffer. Used for SDP service discovery
 *       to save information about discovered profiles, enabled by default (to
 *       reduce memory consumption, about 1.5k+ bytes).
 *     2.When using two SDKs to connect to each other and need to connect to HFP
 *       and HID, you need to disable the shared buffer function. The value of
 *       this parameter must be synchronized with the BTP_SDP_SHARE_BUFFER_ENABLE.
 *******************************************************************************/
int btp_sdp_init(uint8_t count, uint16_t srvRspAttLen, uint16_t cacheBufferSize, bool isShareCache);

/******************************************************************************
 * Function: SDP Deinit interface
 * Descript: This interface be used by User to deinitial sdp resource.
 * Params: pItem: refer to btp_sdp_item_t
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int btp_sdp_deinit(void);

/******************************************************************************
 * Function: btp_sdp_getMemLen
 * Descript: SDP get the size of the required number of nodes interface.
 * Params: count: refer to TLK_BT_SDP_MAX_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int btp_sdp_getMemLen(uint8_t count, uint16_t srvRspAttLen, uint16_t shareBufferSize, bool isShareCache);

/******************************************************************************
 * Function: SDP Connect interface
 * Descript: This interface be used by User to setup a sdp connection
 * Params:
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_sdp_connect(uint16_t aclHandle, uint8_t usrID);


/******************************************************************************
 * Function: SDP DisConnect interface
 * Descript: This interface be used by User to tearup a sdp connection
 * Params:
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_sdp_disconn(uint16_t aclHandle, uint8_t usrID);

/******************************************************************************
 * Function: SDP destroy interface
 * Descript: This interface be used by user to destroy sdp
 *           resource which indicate by aclHandle.
 * Params:
 *         @aclHandle[IN]--The acl link handle.
 * Return:
 *******************************************************************************/
void btp_sdp_destroy(uint16_t aclHandle);

#if (TLKBTP_CFG_SDPSRV_ENABLE)
/******************************************************************************
 * Function: SDP DisConnect interface
 * Descript: This interface be used by Server to disconnect sdp
 *           connection which indicate by aclHandle.
 * Params:
 *         @aclHandle[IN]--The acl link handle.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
extern int btp_sdpsrv_disconn(uint16_t aclHandle);
/******************************************************************************
 * Function: SDP destroy interface
 * Descript: This interface be used by Server to destroy sdp
 *           resource which indicate by aclHandle.
 * Params:
 *         @aclHandle[IN]--The acl link handle.
 * Return:
 *******************************************************************************/
extern void btp_sdpsrv_destroy(uint16_t aclHandle);
#endif

#if (TLKBTP_CFG_SDPCLT_ENABLE)
/******************************************************************************
 * Function: SDP Connect interface
 * Descript: This interface be used by Client to connect sdp
 *           connection which indicate by aclHandle.
 * Params:
 *         @aclHandle[IN]--The acl link handle.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
extern int btp_sdpclt_connect(uint16_t aclHandle);

/******************************************************************************
 * Function: SDP DisConnect interface
 * Descript: This interface be used by Client to disconnect sdp
 *           connection which indicate by aclHandle.
 * Params:
 *         @aclHandle[IN]--The acl link handle.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
extern int btp_sdpclt_disconn(uint16_t aclHandle);

/******************************************************************************
 * Function: SDP destroy interface
 * Descript: This interface be used by Client to destroy sdp
 *           resource which indicate by aclHandle.
 * Params:
 *         @aclHandle[IN]--The acl link handle.
 * Return:
 *******************************************************************************/
extern void btp_sdpclt_destroy(uint16_t aclHandle);
#endif


typedef void (*BtpSdpDataCB)(uint16_t uuid, uint8_t *pData, uint16_t dataLen, uint8_t remain_continue_bytes, uint8_t *remail_len_buf);
/******************************************************************************
 * Function: sdp Register Callback interface
 * Descript:
 * Params:
 * Return:
 *******************************************************************************/
void btp_sdp_regDataCB(BtpSdpDataCB dataCB);

#endif // #if (TLKBTP_CFG_SDP_ENABLE)

#endif // BTP_SDP_H
