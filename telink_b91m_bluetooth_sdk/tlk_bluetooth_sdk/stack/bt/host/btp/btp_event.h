/********************************************************************************************************
 * @file    btp_event.h
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
#ifndef BTP_EVENT_H
#define BTP_EVENT_H


typedef int (*btp_event_func)(uint8_t *pData, uint16_t dataLen);


#define BTP_EVT_REGISTER(evtID, func)                  \
    int evtID##_FUNC(uint8_t *pData, uint16_t dataLen) \
    {                                                  \
        return func(pData, dataLen);                   \
    }

typedef enum
{
    BTP_EVTID_NONE = 0,

    // Profile Event
    BTP_EVTID_PROFILE_REQUEST,
    BTP_EVTID_PROFILE_CONNECT,
    BTP_EVTID_PROFILE_DISCONN,

    BTP_EVTID_PROFILE_SERVICE, // L2cap Service
    BTP_EVTID_PROFILE_CHANNEL, // Rfcomm Channel

    // SDP
    BTP_EVTID_SDP_COMPLETE,
    BTP_EVTID_SDP_SEARCH_ADD_PROTOCOL,
    // HFP
    BTP_EVTID_HFPHF_CODEC_CHANGED,
    BTP_EVTID_HFPAG_CODEC_CHANGED,
    BTP_EVTID_HFPHF_STATUS_CHANGED,
    BTP_EVTID_HFPHF_STATUS_INQUIRY,
    BTP_EVTID_HFPHF_CALL_STATUS_CHANGED,
    BTP_EVTID_HFPHF_SIGNAL_CHANGED,
    BTP_EVTID_HFPHF_NUMBER_INQUIRY,
    BTP_EVTID_HFPHF_VOLUME_CHANGED,
    BTP_EVTID_HFPAG_STATUS_CHANGED,
    BTP_EVTID_HFPAG_VOLUME_CHANGED,
    // A2DP
    BTP_EVTID_A2DPSRC_CODEC_CHANGED,
    BTP_EVTID_A2DPSNK_CODEC_CHANGED,
    BTP_EVTID_A2DPSRC_STATUS_CHANGED,
    BTP_EVTID_A2DPSNK_STATUS_CHANGED,
    BTP_EVTID_A2DPSRC_RECONFIG_COMPLETE,

    // AVRCP
    BTP_EVTID_AVRCP_KEY_CHANGED,
    BTP_EVTID_AVRCP_VOLUME_CHANGED,
    BTP_EVTID_AVRCP_STATUS_CHANGED,
    BTP_EVTID_AVRCP_PEER_EVT_MASK,
    BTP_EVTID_AVRCP_TRACK_CHANGED,
    BTP_EVTID_AVRCP_BATTERY_STATUS_CHANGED,
    BTP_EVTID_AVRCP_PLAYBACK_POS_CHANGED,
    BTP_EVTID_AVRCP_PLAYER_APP_SETTING_CHANGED,
    BTP_EVTID_AVRCP_PLAYER_ADDRESSED_CHANGED,
    BTP_EVTID_AVRCP_NOW_PLAYER_CHANGED,
    BTP_EVTID_AVRCP_OTHER_EVENT_CHANGED,

    // COVER ART
    BTP_EVTID_COVER_ART_GET_IMAGE,

    BTP_EVTID_MAX,
} BTP_EVTID_ENUM;

typedef struct
{
    uint8_t  ptype; // Profile Type, Refer to BTP_PTYPE_ENUM.
    uint16_t chnID;
    uint16_t handle;
} btp_requestEvt_t;

typedef struct
{
    uint8_t  status;
    uint8_t  ptype; // Profile Type, Refer to BTP_PTYPE_ENUM.
    uint8_t  usrID;
    uint16_t chnID;
    uint16_t handle;
} btp_connectEvt_t;

typedef struct
{
    uint8_t  ptype; // Profile Type, Refer to BTP_PTYPE_ENUM.
    uint8_t  usrID;
    uint16_t handle;
} btp_disconnEvt_t;

typedef struct
{
    uint16_t handle;
    uint16_t service;
    uint16_t psmID;
} btp_serviceEvt_t;

typedef struct
{
    uint16_t handle;
    uint16_t service; // Refer to .
    uint8_t  channel;
} btp_channelEvt_t;

typedef struct
{
    uint16_t handle;
} btp_sdpCompleteEvt_t;

typedef struct
{
    uint16_t handle;
    uint16_t search_uuid;
    uint16_t psm;
    uint16_t psm_base_uuid;
} btp_sdpSearchAddProtEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  codec;
} btp_hfpCodecChangedEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  volume;
    uint8_t  volType; // 1-SPK, 2-MIC
} btp_hfpVolumeChangedEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  status;
    uint8_t  callDir;
    uint8_t  numbLen;
    uint8_t *pNumber;
} btp_hfpStatusChangedEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  callState; // Inquiry State: 0-Start, 1-continue, 2-Stop
    uint16_t callSetup;
} btp_hfpStatusInquiryEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  status;
    uint8_t  type; // refer to BTP_HFP_CALL_STATUS_TYPE
} btp_hfpCallStatusChangedEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  value;
    uint8_t  idx; // refer to BTP_HFP_CALL_STATUS_TYPE
} btp_hfpSignalChangedEvt_t;

typedef struct
{
    uint8_t  index;
    uint8_t  dir;
    uint8_t  status;
    uint8_t  mode;
    uint8_t  mpty;
    uint8_t  type[4];
    uint8_t  numbLen;
    uint8_t *pNumber;
} btp_hfp_clcc_evt_t;

typedef struct
{
    uint8_t             state; // Inquiry State: 0-Start, 1-continue, 2-Stop
    uint8_t             resv;
    uint16_t            handle;
    btp_hfp_clcc_evt_t *clcc_info;
} btp_hfpNumberInquiryEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  status; // 0-Closed, 1-Opened, 2-Paused, 3-Stream, Refer to BTP_A2DP_STATUS_ENUM.
    uint16_t mtuSize;
} btp_a2dpStatusChangeEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  chnMode;   // Refer to BTP_A2DP_CHN_MODE_ENUM.
    uint8_t  codecType; // Refer to BTP_A2DP_CODEC_ENUM.
    uint16_t frequence;
    uint8_t  aacObjType; // Refer to .
    uint32_t aacBitRate; // Refer to BTP_A2DP_FREQUENCY_ENUM.
} btp_a2dpCodecChangeEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  status; // 0-success, others-failure
} btp_a2dpReconfigCompleteEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  keyID;
    uint8_t  isPress;
} btp_avrcpKeyChangeEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  volume;
    uint8_t  isControl;
} btp_avrcpVolumeChangeEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  isNoty;
    uint8_t  status; // 0-Stopped, 1-PLAYING, 2-PAUSED, 3-FWD_SEEK, 4-REV_SEEK, Refer to BTP_AVRCP_PLAY_STATE_ENUM.
} btp_avrcpStatusChangeEvt_t;

typedef struct
{
    uint16_t handle;
    uint32_t evtMask; // Refer BTP_AVRCP_EVTMSK_ENUM
} btp_avrcpPeerEvtMaskEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t *pTrackID;
} btp_avrcpTrackChangeEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  status; // Refer to BTP_AVRCP_BATTERY_STATUS_ENUM.
} btp_avrcpBatteryStatusChangeEvt_t;

typedef struct
{
    uint16_t handle;
    uint32_t playPos; // Unit: ms
} btp_avrcpPlaybackPosChangeEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t *pParam;
    uint16_t paramLen;
} btp_avrcpPlayerAppSettingChangeEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t *pParam;
    uint16_t paramLen;
} btp_avrcpPlayerAddressedChangeEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t  eventID;
    uint8_t *pParam;
    uint16_t paramLen;
} btp_avrcpOtherEventChangeEvt_t;

typedef struct
{
    uint8_t  isFirst;
    uint8_t  status;     // 0-data, 1-abort
    uint8_t  obex_state; // refer to 'BTP_OBEX_RSP_ENUM'
    uint8_t  rsv;
    uint16_t handle;
    uint16_t bodyLen;
    uint32_t totalLen;
    uint8_t *pBody;
} btp_coverArtGetImageEvt_t;

int btp_send_event(uint16_t evtID, uint8_t *pData, uint16_t dataLen);

int btp_send_requestEvt(uint8_t ptype, uint16_t aclHandle, uint16_t chnID);
int btp_send_connectEvt(uint8_t status, uint8_t ptype, uint8_t usrID, uint16_t aclHandle, uint16_t chnID);
int btp_send_disconnEvt(uint8_t ptype, uint8_t usrID, uint16_t aclHandle);

int btp_send_serviceEvt(uint16_t aclHandle, uint16_t service, uint16_t psmID);
int btp_send_channelEvt(uint16_t aclHandle, uint16_t service, uint8_t channel);
int btp_send_sdpCompleteEvt(uint16_t aclHandle);
int btp_send_sdpSearchAddProtocolEvt(uint16_t aclHandle, uint16_t search_uuid, uint16_t psm, uint16_t base_uuid);


int btp_send_hfphfCodecChangedEvt(uint16_t aclHandle, uint8_t codec);
int btp_send_hfpagCodecChangedEvt(uint16_t aclHandle, uint8_t codec);
int btp_send_hfphfVolumeChangedEvt(uint16_t aclHandle, uint8_t type, uint8_t volume);
int btp_send_hfpagVolumeChangedEvt(uint16_t aclHandle, uint8_t type, uint8_t volume);
int btp_send_hfphfCallStatusChangedEvt(uint16_t aclHandle, uint8_t status, uint8_t status_type);
int btp_send_hfphfSignalChangedEvt(uint16_t aclHandle, uint8_t idx, uint8_t value);
int btp_send_hfphfStatusChangedEvt(uint16_t aclHandle, uint8_t status, uint8_t callDir, uint8_t *pNumber, uint8_t numbLen);
int btp_send_hfpagStatusChangedEvt(uint16_t aclHandle, uint8_t status, uint8_t callDir, uint8_t *pNumber, uint8_t numbLen);
int btp_send_hfphfStatusInquiryEvt(uint16_t aclHandle, uint8_t state, uint8_t setup);
int btp_send_hfphfNumberInquiryEvt(uint16_t aclHandle, uint8_t state, btp_hfp_clcc_evt_t *clcc_info);


int btp_send_a2dpSrcCodecChangedEvt(uint16_t aclHandle, uint8_t chnMode, uint8_t codecType, uint32_t frequence, uint32_t bitRate, uint8_t objType);
int btp_send_a2dpSnkCodecChangedEvt(uint16_t aclHandle, uint8_t chnMode, uint8_t codecType, uint32_t frequence, uint32_t bitRate, uint8_t objType);
int btp_send_a2dpSrcStatusChangedEvt(uint16_t aclHandle, uint8_t status, uint16_t mtuSize);
int btp_send_a2dpSnkStatusChangedEvt(uint16_t aclHandle, uint8_t status, uint16_t mtuSize);
int btp_send_a2dpSrcReconfigCompleteEvt(uint16_t aclHandle, uint8_t status);

/******************************************************************************
 * Function: btp_send_avrcpKeyChangedEvt
 * Descript: Send events to the user layer.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @keyID--Refer to BTP_AVRCP_KEYID_ENUM.
 *     @isPress--True,this is one pressed key; False-this is one released key.
 * Return:Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_send_avrcpKeyChangedEvt(uint16_t aclHandle, uint8_t keyID, uint8_t isPress);
/******************************************************************************
 * Function: btp_send_avrcpVolumeChangedEvt
 * Descript: Send events to the user layer.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @volume--0x00~0x7F.
 * Return:Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_send_avrcpVolumeChangedEvt(uint16_t aclHandle, uint8_t volume, uint8_t isNotify);
/******************************************************************************
 * Function: btp_send_avrcpStatusChangedEvt
 * Descript: Send events to the user layer.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @status--Refer to BTP_AVRCP_PLAY_STATE_ENUM.
 *     @isNoty--True,report by notify; False-report by response.
 * Return:Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_send_avrcpStatusChangedEvt(uint16_t aclHandle, uint8_t status, uint8_t isNoty);
int btp_send_avrcpPeerEvtMaskEvt(uint16_t aclHandle, uint32_t evtMask);
int btp_send_avrcpTrackChangedEvt(uint16_t aclHandle, uint8_t *pTrackID);
/******************************************************************************
 * Function: btp_send_avrcpBatteryStatusChangedEvt
 * Descript: Send events to the user layer.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @status--Refer to BTP_AVRCP_BATTERY_STATUS_ENUM.
 * Return:Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_send_avrcpBatteryStatusChangedEvt(uint16_t aclHandle, uint8_t status);
/******************************************************************************
 * Function: btp_send_avrcpPlaybackPosChangedEvt
 * Descript: Send events to the user layer.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @playPos--The offset of the current playback position. Unit:ms.
 * Return:Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_send_avrcpPlaybackPosChangedEvt(uint16_t aclHandle, uint32_t playPos);
int btp_send_avrcpPlayerAppSettingChangedEvt(uint16_t aclHandle, uint8_t *pParam, uint16_t paramLen);
int btp_send_avrcpPlayerAddressedChangedEvt(uint16_t aclHandle, uint8_t *pParam, uint16_t paramLen);
int btp_send_avrcpOtherEventChangedEvt(uint16_t aclHandle, uint8_t evtID, uint8_t *pParam, uint16_t paramLen);
int btp_send_coverArtGetImageEvt(uint16_t aclHandle, uint8_t isFirst, uint8_t status, uint8_t obex_state, uint32_t totalLen, uint8_t *pParam, uint16_t paramLen);


#endif // BTP_EVENT_H
