/********************************************************************************************************
 * @file    bth_event.h
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
#ifndef BTH_EVENT_H
#define BTH_EVENT_H


typedef int (*bth_event_func)(uint8_t *pData, uint16_t dataLen);

#define BTH_EVT_REGISTER(evtID, func)                  \
    int evtID##_FUNC(uint8_t *pData, uint16_t dataLen) \
    {                                                  \
        return func(pData, dataLen);                   \
    }

typedef enum
{
    BTH_EVTID_NONE = 0,

    // Basic Event
    BTH_EVTID_INQUIRY_RESULT,
    BTH_EVTID_INQUIRY_COMPLETE,
    BTH_EVTID_GETNAME_COMPLETE,
    BTH_EVTID_ACLCONN_REQUEST,
    BTH_EVTID_SCOCONN_REQUEST,
    BTH_EVTID_ACLCONN_COMPLETE,
    BTH_EVTID_SCOCONN_COMPLETE,
    BTH_EVTID_ACLDISC_COMPLETE,
    BTH_EVTID_SCODISC_COMPLETE,
    BTH_EVTID_SIMPLE_PAIRING_COMPLETE,
    BTH_EVTID_AUTHEN_COMPLETE,
    BTH_EVTID_ENCRYPT_COMPLETE,
    BTH_EVTID_SCOCODEC_CHANGED,
    BTH_EVTID_ROLE_CHANGED,
    BTH_EVTID_MODE_CHANGED,
    BTH_EVTID_PINCODE_REQUEST,
    BTH_EVTID_LINKKEY_REQUEST,
    BTH_EVTID_LINKKEY_NOTIFY,
    BTH_EVTID_EXT_FEATURE_CHANGED,
    BTH_EVTID_ACL_GETNAME_REPORT,
    BTH_EVTID_ACL_GETRSSI_REPORT,

    // Signal Event
    BTH_EVTID_SIGNAL_CONNREQ,
    BTH_EVTID_SIGNAL_CONNRSP,
    BTH_EVTID_SET_SCAN_CMD_COMPLETE,

    BTH_EVTID_START_INQUIRY_STATUS,
    BTH_EVTID_CANCEL_INQUIRY_COMPLETE,
    BTH_EVTID_CANCEL_GET_NAME_COMPLETE,

    BTH_EVTID_CMD_STATUS,
    BTH_EVTID_CMD_COMPLETE,
    BTH_EVTID_ACL_GETAFH_REPORT,

    BTH_EVTID_MAX,
} BTH_EVTID_ENUM;

typedef struct
{
    uint8_t  mac[6];
    uint8_t  rssi;
    uint8_t  smode;   // Scan Mode
    uint8_t  nameLen; // Name Length
    uint8_t  resv3byte[3];
    uint16_t clock;
    uint16_t eirLen;
    uint32_t dtype;
    uint8_t *pName;
    uint8_t *pEird;
} bth_inquiryResultEvt_t;

typedef struct
{
    uint8_t  status;
    uint8_t  nameLen;
    uint8_t  btaddr[6];
    uint8_t *pName;
} bth_getNameCompleteEvt_t;

typedef struct
{
    uint32_t devClass;
    uint8_t  peerMac[6];
    uint8_t  resv2byte[2];
} bth_aclConnRequestEvt_t;

typedef struct
{
    uint8_t  linkType;
    uint8_t  resv1byte;
    uint8_t  peerMac[6];
    uint32_t devClass;
} bth_scoConnRequestEvt_t;

typedef struct
{
    uint8_t  status;
    uint8_t  active; // True - The link is initiated by us.
    uint16_t handle;
    uint8_t  isEncrypt;
    uint8_t  resv1byte;
    uint8_t  peerMac[6];
} bth_aclConnCompleteEvt_t;

typedef struct
{
    uint8_t  status;
    uint8_t  linkType : 4; // Refer BTH_SCO_LINK_TYPE_ENUM.
    uint8_t  airMode  : 4; // Refer BTH_SCO_AIRMODE_ENUM.
    uint8_t  peerMac[6];
    uint16_t aclHandle;
    uint16_t scoHandle;
} bth_scoConnCompleteEvt_t;

typedef struct
{
    uint8_t  reason;
    uint8_t  resv1byte;
    uint8_t  peerMac[6];
    uint16_t handle;
    uint16_t resv2byte;
} bth_aclDiscCompleteEvt_t;

typedef struct
{
    uint8_t  reason;
    uint8_t  linkType;
    uint8_t  peerMac[6];
    uint16_t aclHandle;
    uint16_t scoHandle;
} bth_scoDiscCompleteEvt_t;

typedef struct
{
    uint8_t status;
    uint8_t resv1byte;
    uint8_t bd_addr[6];
} bth_simplePairingCompleteEvt_t;

typedef struct
{
    uint8_t  status;
    uint8_t  resv1byte;
    uint16_t handle;
} bth_authenCompleteEvt_t;

typedef struct
{
    uint8_t  status;
    uint8_t  enable;
    uint16_t handle;
} bth_encryptCompleteEvt_t;

typedef struct
{
    uint8_t codec; // 0/1-CVSD, 2-MSBC
    uint8_t resv3byte[3];
} bth_scoCodecChangedEvt_t;

typedef struct
{
    uint8_t status;  // 0x00-A role change has occurred;
    uint8_t newRole; // 0x00-Master, 0x01-Slave
    uint8_t peerMac[6];
} bth_roleChangedEvt_t;

typedef struct
{
    uint8_t  status;  // 0x00 -A Mode Change has occurred;
    uint8_t  curMode; // 0x00-Active Mode; 0x01-Hold Mode; 0x02-Sniff Mode.
    uint16_t handle;
    uint16_t interval;
    uint16_t resv2byte;
} bth_modeChangedEvt_t;

typedef struct
{
    uint8_t  peerMac[6];
    uint16_t aclHandle;
} bth_pinCodeRequestEvt_t;

typedef struct
{
    uint8_t  peerMac[6];
    uint16_t aclHandle;
} bth_linkKeyRequestEvt_t;

typedef struct
{
    uint8_t  keyType;
    uint8_t  reserve;
    uint8_t  peerMac[6];
    uint8_t  linkKey[16];
    uint32_t devClass;
} bth_linkKeyNotifyEvt_t;

typedef struct
{
    uint16_t resv2byte;
    uint16_t aclHandle;
    uint32_t peerExtFeature; // Refer BTH_L2CAP_EXT_FEATURE_ENUM
    uint32_t mineExtFeature; // Refer BTH_L2CAP_EXT_FEATURE_ENUM
} bth_extFeatureChangedEvt_t;

typedef struct
{
    uint8_t  status;
    uint8_t  nameLen;
    uint8_t  btaddr[6];
    uint8_t  isCustomerDev;
    uint16_t handle;
    uint8_t *pName;
    uint8_t  resume_music;
    uint8_t  rev2;
    uint16_t music_handle;
} bth_aclGetNameReportEvt_t;

typedef struct
{
    uint8_t  status;
    uint8_t  rssi;
    uint16_t handle;
} bth_aclGetRssiReportEvt_t;

typedef struct
{
    uint8_t  status;
    uint8_t  afh_mode;
    uint16_t conhdl;
    uint8_t  map[10];
} bth_aclGetAfhReportEvt_t;

typedef struct
{
    uint16_t handle;
    uint16_t psm;
    uint16_t dcid;
    uint16_t resv2byte;
} bth_signalConnReqEvt_t;

/******************************************************************************
 * Function: bth_send_event
 * Descript: Send the event via callback.
 * Params:
 *        @evtID[IN]--The event id.
 *        @pData[IN]--The callback data.
 *        @dataLen[IN]--The data len.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_event(uint16_t evtID, uint8_t *pData, uint16_t dataLen);


/******************************************************************************
 * Function: bth_send_aclConnRequestEvt
 * Descript: Send the acl connect request event via callback.
 * Params:
 *        @devClass[IN]--The device class.
 *        @btaddr[IN]--The bt address.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_aclConnRequestEvt(uint32_t devClass, uint8_t btaddr[6]);

/******************************************************************************
 * Function: bth_send_aclConnCompleteEvt
 * Descript: Send the ACL link connect complete event via callback.
 * Params:
 *        @handle[IN]--The acl link handle.
 *        @status[IN]--The link status.
 *        @active[IN]--The acl link is active.
 *        @isEncrypt[IN]--is encrypt.
 *        @btaddr[IN]--The device bt address.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_aclConnCompleteEvt(uint16_t handle, uint8_t status, uint8_t active, uint8_t isEncrypt, uint8_t btaddr[6]);

/******************************************************************************
 * Function: bth_send_aclDiscCompleteEvt
 * Descript: Send the ACL link disconnect complete event via callback.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @reason[IN]--The disconnect reason.
 *        @btaddr[IN]--The device bt address.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_aclDiscCompleteEvt(uint16_t handle, uint8_t reason, uint8_t btaddr[6]);

/******************************************************************************
 * Function: bth_send_scoConnRequestEvt
 * Descript: Send the Sco link connect request event via callback.
 * Params:
 *        @devClass[IN]--The device class.
 *        @linkType[IN]--The link type.
 *        @btaddr[IN]--The bt address.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_scoConnRequestEvt(uint32_t devClass, uint8_t linkType, uint8_t btaddr[6]);

/******************************************************************************
 * Function: bth_send_scoConnCompleteEvt
 * Descript: Send the SCO link connect complete event via callback.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @scoHandle[IN]--The Sco link handle.
 *        @status[IN]--The acl link status.
 *        @linkType[IN]--The link type. Refer BTH_SCO_LINK_TYPE_ENUM.
 *        @airMode[IN]--Refer BTH_SCO_AIRMODE_ENUM.
 *        @btaddr[IN]--The device bt address.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_scoConnCompleteEvt(uint16_t aclHandle, uint16_t scoHandle, uint8_t status, uint8_t linkType, uint8_t airMode, uint8_t btaddr[6]);

/******************************************************************************
 * Function: bth_send_scoDiscCompleteEvt
 * Descript: Send the SCO link disconnect complete event via callback.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @scoHandle[IN]--The Sco link handle.
 *        @reason[IN]--The sco link disconnect reason.
 *        @linkType[IN]--The link type.
 *        @btaddr[IN]--The device bt address.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_scoDiscCompleteEvt(uint16_t aclHandle, uint16_t scoHandle, uint8_t reason, uint8_t linkType, uint8_t btaddr[6]);

int bth_send_simplePairingCompleteEvt(uint8_t status, uint8_t bd_addr[6]);

/******************************************************************************
 * Function: bth_send_authenCompleteEvt
 * Descript: Send the authenticate complete event via callback.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @status[IN]--The status of authenticate.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_authenCompleteEvt(uint16_t handle, uint8_t status);

/******************************************************************************
 * Function: bth_send_encryptCompleteEvt
 * Descript: Send the encrypt complete event via callback.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @status[IN]--The status of authenticate.
 *        @enable[IN]--is encrypt enable.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_encryptCompleteEvt(uint16_t handle, uint8_t status, uint8_t enable);

/******************************************************************************
 * Function: bth_send_scoCodecChangedEvt
 * Descript: Send the SCO codec changed event via callback.
 * Params:
 *        @codec[IN]--The sco codec id.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
extern int bth_send_scoCodecChangedEvt(uint8_t codec);

/******************************************************************************
 * Function: bth_send_aclRoleChangeEvt
 * Descript: Send the Acl role changed event via callback.
 * Params:
 *        @btaddr[IN]--The peer btaddr.
 *        @status[IN]--The role change event status.
 *        @newRole[IN]--The new role.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
extern int bth_send_aclRoleChangeEvt(uint8_t btaddr[6], uint8_t status, uint8_t newRole);

/******************************************************************************
 * Function: bth_send_pinCodeRequestEvt
 * Descript: Send the Pin Code request event via callback.
 * Params:
 *        @handle[IN]--The handle of acl link.
 *        @btaddr[IN]--The bt addr.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_pinCodeRequestEvt(uint16_t handle, uint8_t btaddr[6]);

/******************************************************************************
 * Function: bth_send_linkKeyRequestEvt
 * Descript: Send the link key request event via callback.
 * Params:
 *        @handle[IN]--The handle of acl link.
 *        @btaddr[IN]--The bt addr.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_linkKeyRequestEvt(uint16_t handle, uint8_t btaddr[6]);

/******************************************************************************
 * Function: bth_send_linkKeyNotifyEvt
 * Descript: Send the link key notify event via callback.
 * Params:
 *        @keyType[IN]--The link key type.
 *        @btaddr[IN]--The bt addr.
 *        @linkKey[IN]--The link key.
 *        @devClass[IN]--The class of device.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_linkKeyNotifyEvt(uint8_t keyType, uint8_t btaddr[6], uint8_t linkKey[16], uint32_t devClass);

/******************************************************************************
 * Function: bth_send_extFeatureChangedEvt
 * Descript:
 * Params:
 *     @handle[IN]--The handle of acl link.
 *     @peerFeature[IN]--Refer BTH_L2CAP_EXT_FEATURE_ENUM.
 *     @mineFeature[IN]--Refer BTH_L2CAP_EXT_FEATURE_ENUM.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_extFeatureChangedEvt(uint16_t handle, uint32_t peerFeature, uint32_t mineFeature);


/******************************************************************************
 * Function: bth_send_aclGetNameReqEvt
 * Descript:
 * Params:
 *     @handle[IN]--The handle of acl link.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_aclGetNameReqEvt(uint16_t handle, uint8_t status, uint8_t btaddr[6], uint8_t *pName, uint8_t nameLen);

/******************************************************************************
 * Function: bth_send_aclGetRssiReqEvt
 * Descript:
 * Params:
 *     @handle[IN]--The handle of acl link.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_aclGetRssiReqEvt(uint16_t handle, uint8_t status, uint8_t rssi);

/******************************************************************************
 * Function: bth_send_aclGetAfhReqEvt
 * Descript:
 * Params:
 *     @handle[IN]--The handle of acl link.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_aclGetAfhReqEvt(uint16_t handle, uint8_t afhMode, uint8_t status, uint8_t *chnMap);

/******************************************************************************
 * Function: bth_send_set_scan_cmd_CompleteEvt
 * Descript:
 * Params:
 *     @handle[IN]--the resut of set bt scan.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_set_scan_cmd_CompleteEvt(uint8_t status);

/******************************************************************************
 * Function: bth_send_commandCompleteEvt
 * Descript:
 * Params:
 *     @handle[IN]--the resut of set bt scan.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_send_modeChangeCompleteEvt(uint16_t handle, uint8_t status, uint8_t mode);

#endif // BTH_EVENT_H
