/********************************************************************************************************
 * @file    bth_l2cap.h
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
#ifndef BTH_L2CAP_H
#define BTH_L2CAP_H


#define BTH_L2CAP_CHN_TIMEOUT      (100000) // 100ms
#define BTH_L2CAP_CHN_CONN_TIMEOUT (10000000 / BTH_L2CAP_CHN_TIMEOUT)
#define BTH_L2CAP_CHN_DISC_TIMEOUT (3000000 / BTH_L2CAP_CHN_TIMEOUT)

#define BTH_L2CAP_MTU_DEF_SIZE     672 // The default value is 672 octets. Spec5.2 P1083
#define BTH_L2CAP_MTU_MAX_SIZE     1024
#define BTH_L2CAP_MPS_MAX_SIZE     668 //

/* base frame */
#define BTH_L2CAP_SIG_CID            0x0001
#define BTH_L2CAP_CONNECTIONLESS_CID 0x0002


#define BTH_L2CAP_CFG_EFS_ENABLE     0
#define BTH_L2CAP_CFG_QOS_ENABLE     0

typedef int (*bth_l2cap_eventCallback_t)(uint8_t evtID, uint16_t psmID, uint8_t *pData, uint16_t dataLen);
typedef void (*bth_l2cap_rdataCallback_t)(uint16_t psmID, uint16_t chnID, uint16_t handle, uint8_t *pData, uint16_t dataLen);

typedef enum
{
    BTH_L2CAP_SAR_UNSEGMENTED = 0x00,
    BTH_L2CAP_SAR_START       = 0x01,
    BTH_L2CAP_SAR_END         = 0x02,
    BTH_L2CAP_SAR_CONTINUE    = 0x03,
} BTH_L2CAP_SAR_ENUM;

typedef enum
{
    BTH_L2CAP_SIG_RTNMODE_BASIC     = 0x00, // L2CAP Basic Mode
    BTH_L2CAP_SIG_RTNMODE_RTN       = 0x01, // Retransmission mode
    BTH_L2CAP_SIG_RTNMODE_FLOW      = 0x02, // Flow control mode
    BTH_L2CAP_SIG_RTNMODE_ENHANCE   = 0x03, // Enhanced Retransmission mode
    BTH_L2CAP_SIG_RTNMODE_STREAMING = 0x04, // Streaming mode
} BTH_L2CAP_SIG_RTNMODE_ENUM;

typedef enum
{
    BTH_L2CAP_SIG_FCSTYPE_NOFCS  = 0x00,
    BTH_L2CAP_SIG_FCSTYPE_16bFCS = 0x01,
} BTH_L2CAP_SIG_FCSTYPE_ENUM;

typedef enum
{
    BTH_L2CAP_OPTION_TYPE_MTU           = 0x01,
    BTH_L2CAP_OPTION_TYPE_FLUSH_TIMEOUT = 0x02,
    BTH_L2CAP_OPTION_TYPE_QOS           = 0x03,
    BTH_L2CAP_OPTION_TYPE_RTN           = 0x04,
    BTH_L2CAP_OPTION_TYPE_FCS           = 0x05,
    BTH_L2CAP_OPTION_TYPE_EXT_FLOW      = 0x06,
    BTH_L2CAP_OPTION_TYPE_EXT_WINSIZE   = 0x07,
} BTH_L2CAP_OPTION_TYPE_ENUM;

typedef enum
{
    BTH_L2CAP_SERVICE_TYPE_NO_TRAFFIC  = 0x00,
    BTH_L2CAP_SERVICE_TYPE_BEST_EFFORT = 0x01, // Default
    BTH_L2CAP_SERVICE_TYPE_GUARANTEED  = 0x02,
} BTH_L2CAP_SERVICE_TYPE_ENUM;

// EXTENDED FEATURE MASK <Core5.2.pdf>P1063
typedef enum
{
    BTH_L2CAP_EXT_FEATURE_NONE              = 0x0000,
    BTH_L2CAP_EXT_FEATURE_FLOW_CTRL_MODE    = 0x0001, // Flow control mode
    BTH_L2CAP_EXT_FEATURE_RTN_MODE          = 0x0002, // Retransmission mode
    BTH_L2CAP_EXT_FEATURE_BI_DIR_QOS        = 0x0004, // Bi-directional QoS
    BTH_L2CAP_EXT_FEATURE_ENHANCED_RTN_MODE = 0x0008, // Enhanced Retransmission Mode
    BTH_L2CAP_EXT_FEATURE_STREAMING_MODE    = 0x0010, // Streaming Mode
    BTH_L2CAP_EXT_FEATURE_FCS_OPTION        = 0x0020, // FCS Option
    BTH_L2CAP_EXT_FEATURE_EXT_FLOW_SPEC     = 0x0040, // Extended Flow Specification for BR/EDR
    BTH_L2CAP_EXT_FEATURE_FIXED_CHANNELS    = 0x0080, // Fixed Channels
    BTH_L2CAP_EXT_FEATURE_EXT_WIN_SIZE      = 0x0100, // Extended Window Size
    BTH_L2CAP_EXT_FEATURE_UNICAST_CONN_DATA = 0x0200, // Unicast Connectionless Data Reception
    BTH_L2CAP_EXT_FEATURE_ENHANCED_CREDIT   = 0x0400, // Enhanced Credit Based Flow Control Mode
    BTH_L2CAP_EXT_FEATURE_DEFAULT           = BTH_L2CAP_EXT_FEATURE_FIXED_CHANNELS,
} BTH_L2CAP_EXT_FEATURE_ENUM;

// EXTENDED CHANNEL MASK <Core5.2.pdf>P1063
typedef enum
{
    BTH_L2CAP_EXT_CHANNEL_NONE              = 0x0000,
    BTH_L2CAP_EXT_CHANNEL_SIGNAL            = 0x0002, // BIT(1)
    BTH_L2CAP_EXT_CHANNEL_CONNECTIONLESS    = 0x0004, // BIT(2)
    BTH_L2CAP_EXT_CHANNEL_AMP_MANAGER       = 0x0008, // BIT(3)
    BTH_L2CAP_EXT_CHANNEL_SERCURITY_MANAGER = 0x0080, // BIT(7)
    BTH_L2CAP_EXT_CHANNEL_DEFAULT           = BTH_L2CAP_EXT_CHANNEL_SIGNAL,
} BTH_L2CAP_EXT_CHANNEL_ENUM;

typedef enum
{
    BTP_L2CAP_INFO_TYPE_CONNLESS_MTU      = 0x01,
    BTP_L2CAP_INFO_TYPE_EXTENDED_FEATURE  = 0x02,
    BTP_L2CAP_INFO_TYPE_EXTENDED_CHANNELS = 0x03, // FIXED CHANNELS SUPPORTED   <Core5.2.pdf>P1061 and 1064
} BTH_L2CAP_INFO_TYPE_ENUM;

typedef enum
{
    BTH_L2CAP_EVTID_NONE = 0,
    BTH_L2CAP_EVTID_CLOSED,
    BTH_L2CAP_EVTID_REQUEST,
    BTH_L2CAP_EVTID_DISCONN,
    BTH_L2CAP_EVTID_CONNECT,
} BTH_L2CAP_EVTID_ENUM;

typedef enum
{
    BTP_L2CAP_OPTION_FLAG_MTU = 0x01,
    BTP_L2CAP_OPTION_FLAG_FCS = 0x02, // It is non-negotiable. The FCS option shall only be used when the mode is being,
                                      // or is already configured to Enhanced Retransmission mode or Streaming mode.
    BTP_L2CAP_OPTION_FLAG_RTN         = 0x04,
    BTP_L2CAP_OPTION_FLAG_QOS         = 0x08,
    BTP_L2CAP_OPTION_FLAG_TIMEOUT     = 0x10,
    BTP_L2CAP_OPTION_FLAG_EXT_FLOW    = 0x20,
    BTP_L2CAP_OPTION_FLAG_EXT_WINSIZE = 0x40,
    BTP_L2CAP_OPTION_FLAG_RESERVE     = 0x80,
} BTH_L2CAP_OPTION_FLAG_ENUM;

typedef enum
{
    BTP_L2CAP_CONFIG_FLAG_NONE     = 0x0000,
    BTP_L2CAP_CONFIG_FLAG_CONTINUE = 0x0001,
} BTH_L2CAP_CONFIG_FLAG_ENUM;

typedef enum
{
    BTH_L2CAP_REASON_NONE = 0,
    BTH_L2CAP_REASON_DISC_BY_USER,
    BTH_L2CAP_REASON_DISC_BY_REMOTE,
    BTH_L2CAP_REASON_DISC_BY_REJECT, // The remote does not accept the given parameters.
    BTH_L2CAP_REASON_DISC_BY_TIMEOUT,
    BTH_L2CAP_REASON_DISC_BY_ERR_PRAMA,
    BTH_L2CAP_REASON_DISC_BY_ERR_MTU,
    BTH_L2CAP_REASON_DISC_BY_ERR_STATUS,
} BTH_L2CAP_REASON_ENUM;

typedef struct
{
    uint16_t scid;
    uint16_t handle;
    uint8_t  isActive;
} bth_l2cap_closedEvt_t;

typedef struct
{
    uint16_t scid;
    uint16_t handle;
} bth_l2cap_requestEvt_t;

typedef struct
{
    uint16_t scid;
    uint16_t dcid;
    uint16_t handle;
    uint8_t  userID;
    uint16_t mtuSize;
    uint8_t  isActive; // True-The connection was initiated by us; False-The connection was initiated by peer.
} bth_l2cap_connectEvt_t;

typedef struct
{
    uint16_t scid;
    uint16_t dcid;
    uint16_t handle;
    uint8_t  userID;
    uint16_t reason;
} bth_l2cap_disconnEvt_t;

typedef struct
{
    uint16_t                  psmID;
    uint16_t                  resv0;
    bth_l2cap_eventCallback_t eventCB;
    bth_l2cap_rdataCallback_t rdataCB;
} bth_l2cap_service_t;

typedef struct
{
    uint16_t scid;
    uint16_t dcid;

    uint8_t  state; // TLK_STATE_CLOSED, TLK_STATE_OPENED, TLK_STATE_CONNING, TLK_STATE_CONNECT, TLK_STATE_DISCING
    uint8_t  busys;
    uint8_t  flags;
    uint8_t  usrID;
    uint8_t  attrs; // Refer to BTH_SIGNAL_ATTRS_ENUM.
    uint8_t  resv01;
    uint16_t resv02;

    uint16_t psmID;
    uint16_t mtuSize;
    uint16_t timeout;
    uint16_t aclHandle;

    uint8_t  rspResult;
    uint8_t  rejReason; // reject reason
    uint8_t  disReason; // disconnect reason
    uint8_t  cmdIdentify;
    uint8_t  rejIdentify;
    uint8_t  rspIdentify;
    uint16_t resv03;

    TlkApiTimer_t timer;

    // Options
    uint8_t negIsNext; //
    uint8_t cfgOption; // Refer to BTH_L2CAP_OPTION_FLAG_ENUM.
    uint8_t negOption; // negotiation option by both. Refer to BTH_L2CAP_OPTION_FLAG_ENUM.
    uint8_t curOption; //
    // Flush Timeout.
    uint16_t flushTimeout;
    // RETRANSMISSION AND FLOW CONTROL OPTION
    uint16_t rtnMps;
    uint8_t  rtnMode; //
    uint8_t  rtnTxWin;
    uint8_t  rtnMaxTx;
    uint8_t  resv04;
    uint16_t rtnTimeout;
    uint16_t monTimeout;
    // FCS
    uint8_t  fcsType;
    uint8_t  resv05;
    uint16_t resv06;
// EXTENDED FLOW SPECIFICATION OPTION
#if (BTH_L2CAP_CFG_EFS_ENABLE)
    uint8_t  efsIdentifier;     // Default: 0x01
    uint8_t  efsServiceType;    // Default: 0x01
    uint16_t efsMaxSduSize;     // Default: 0xFFFF
    uint32_t efsSduArrivalTime; // Default: 0xFFFFFFFF
    uint32_t efsAccessLatency;  // Default: 0xFFFFFFFF
    uint32_t efsFlushTimeout;   // Default: 0xFFFFFFFF
#endif
#if (BTH_L2CAP_CFG_QOS_ENABLE)
    uint8_t  qosFlag;
    uint8_t  qosServiceType; // Default: 0x01
    uint16_t resv07;
    uint32_t qosTokenRate;       //
    uint32_t qosTokenBucketSize; //
    uint32_t qosPeekBandWidth;   //
    uint32_t qosLatency;         //
    uint32_t qosDelayVariation;  //
#endif
} bth_l2cap_channel_t;

typedef struct
{
    uint16_t handle;
    uint16_t chnID;
    uint16_t sumLen;
    uint16_t curLen;
    uint8_t  buffer[BTH_L2CAP_MTU_MAX_SIZE];
} bth_l2cap_acldata_t;

typedef struct
{
    uint8_t              mpsBuffer[BTH_L2CAP_MPS_MAX_SIZE + 12]; // 10 -> 12
    bth_l2cap_service_t *service;
    bth_l2cap_channel_t *channel;
    bth_l2cap_acldata_t *aclData;
} bth_l2cap_ctrl_t;

// I-frame Standard Control Field  Core5.2:P1038
typedef union
{
    uint16_t value;

    struct
    {
        uint16_t Type   : 1; // 0:I-Frame, 1-S-Frame
        uint16_t TxSeq  : 6; // Send Sequence Number (Sequence Number Of Acknowledgement)
        uint16_t R      : 1; // Final:The F-bit is set to 1 in response to an S-frame with the P bit set to 1.
        uint16_t ReqSeq : 6; // Receive Sequence Number
        uint16_t SAR    : 2; // Segmentation and Reassembly
    } field;
} bth_l2cap_iframeStdCtrlField_t;

// I-frame Enhanced Control Field
typedef union
{
    uint16_t value;

    struct
    {
        uint16_t Type   : 1;
        uint16_t TxSeq  : 6;
        uint16_t F      : 1;
        uint16_t ReqSeq : 6;
        uint16_t SAR    : 2;
    } field;
} bth_l2cap_iframeEnhCtrlField_t;

// I-frame Extended Control Field
typedef union
{
    uint32_t value;

    struct
    {
        uint32_t Type   : 1;
        uint32_t F      : 1;
        uint32_t TxSeq  : 14;
        uint32_t SAR    : 2;
        uint32_t ReqSeq : 14;
    } field;
} bth_l2cap_iframeExtCtrlField_t;

// S-frame Standard Control Field  Core5.2:P1038
typedef union
{
    uint16_t value;

    struct
    {
        uint16_t Type : 1;   // 0:I-Frame, 1-S-Frame
        uint16_t RFU0 : 1;   //
        uint16_t S    : 2;   // Supervisory function. 00-RR,Receiver Ready; 01-REJ,Reject; 10-RNR,Receiver Not Ready;
                             // 11-SREJ,Select Reject
        uint16_t RFU1 : 3;   //
        uint16_t R    : 1;   // Retransmission Disable Bit. 0-Normal operation; 1-Receiver side requests sender to postpone
                             // retransmission of I-frames.
        uint16_t ReqSeq : 6; // Receive Sequence Number
        uint16_t RFU2   : 2; //
    } field;
} bth_l2cap_sframeStdCtrlField_t;

// S-frame Enhanced Control Field
typedef union
{
    uint16_t value;

    struct
    {
        uint16_t Type : 1;   // 0:I-Frame, 1-S-Frame
        uint16_t RFU0 : 1;   //
        uint16_t S    : 2;   // Supervisory function. 00-RR,Receiver Ready; 01-REJ,Reject; 10-RNR,Receiver Not Ready;
                             // 11-SREJ,Select Reject
        uint16_t P : 1;      // Retransmission Disable Bit. 0-Normal operation; 1-Receiver side requests sender to postpone
                             // retransmission of I-frames.
        uint16_t RFU1 : 2;   // Poll. The P-bit is set to 1 to solicit a response from the receiver. The receiver
                             // shallrespond immediately with a frame with the F-bit set to 1.
        uint16_t F      : 1; // Final. The F-bit is set to 1 in response to an S-frame with the P bit set to 1.
        uint16_t ReqSeq : 6; // Receive Sequence Number
        uint16_t RFU2   : 2; //
    } field;
} bth_l2cap_sframeEnhCtrlField_t;

// S-frame Extended Control Field
typedef union
{
    uint32_t value;

    struct
    {
        uint32_t Type   : 1;
        uint32_t F      : 1;
        uint32_t TxSeq  : 14;
        uint32_t S      : 2;
        uint32_t P      : 1;
        uint32_t ReqSeq : 13;
    } field;
} bth_l2cap_sframeExtCtrlField_t;

/******************************************************************************
 * Function: bth_l2cap_init
 * Descript: Initial the l2cap control block.
 * Params:
 * 		@psmNumb[IN]--refer to TLK_STK_BTPSM_NUMB
 * 		@chnNumb[IN]--refer to TLK_STK_BTCHN_NUMB
 * 		@aclNumb[IN]--refer to TLK_STK_BTACL_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value(-TLK_ENOMEM) is returned means the initial process fail.
 *******************************************************************************/
int bth_l2cap_init(uint8_t psmNumb, uint8_t chnNumb, uint8_t aclNumb);

/******************************************************************************
 * Function: bth_l2cap_deinit
 * Descript: Deinitial the l2cap control block.
 * Params:
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int bth_l2cap_deinit(void);

/******************************************************************************
 * Function: bth_l2cap_getMemLen
 * Descript: get the size of the required number of nodes interface.
 * Params:
 * 		@psmNumb[IN]--refer to TLK_STK_BTPSM_NUMB
 * 		@chnNumb[IN]--refer to TLK_STK_BTCHN_NUMB
 * 		@aclNumb[IN]--refer to TLK_STK_BTACL_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int bth_l2cap_getMemLen(uint8_t psmNumb, uint8_t chnNumb, uint8_t aclNumb);

/******************************************************************************
 * Function: bth_l2cap_aclDisconn
 * Descript: Send l2cap disconnect.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 * Return: TLK_ENONE is success, other value if false.
 *******************************************************************************/
int bth_l2cap_aclDisconn(uint16_t aclHandle);


/******************************************************************************
 * Function: bth_l2cap_setCurChnID
 * Descript: Initial sBthL2capCurChnID.
 * Params:
 *        @cur_cid[IN]--new set cid
 * Return: None.
 *******************************************************************************/
void bth_l2cap_setCurChnID(uint16_t cur_cid);

/******************************************************************************
 * Function: bth_l2cap_setDefMtuSize
 * Descript: Set the Mtu size.
 * Params:
 *     @size[IN]--The mtu size.
 * Return: None.
 *******************************************************************************/
void bth_l2cap_setDefMtuSize(uint16_t size);
uint bth_l2cap_getDefMtuSize(void);

void bth_l2cap_setExtFeature(uint32_t feature);
uint bth_l2cap_getExtFeature(void);
void bth_l2cap_setExtFeatureBits(uint32_t featBits);
void bth_l2cap_clsExtFeatureBits(uint32_t featBits);
bool bth_l2cap_haveExtFeatureBits(uint32_t featBits);

void bth_l2cap_setExtChannel(uint32_t channel);
uint bth_l2cap_getExtChannel(void);
void bth_l2cap_setExtChannelBits(uint32_t channel);
void bth_l2cap_clsExtChannelBits(uint32_t channel);
bool bth_l2cap_haveExtChannelBits(uint32_t channel);

uint bth_l2cap_getValidCID(void);


/******************************************************************************
 * Function: bth_l2cap_regServiceCB
 * Descript: Register the callback.
 * Params:
 *     @psmID[IN]--The psm id.
 *     @usrID[IN]--The user id.
 *     @eventCB[IN]--The l2cap event callback.
 *     @rdataCB[IN]--The read data callback.
 * Return: TLK_ENONE is success, other' value is failure.
 *******************************************************************************/
int bth_l2cap_regServiceCB(uint16_t psmID, bth_l2cap_eventCallback_t eventCB, bth_l2cap_rdataCallback_t rdataCB);

/******************************************************************************
 * Function: bth_l2cap_sendEvent
 * Descript: Set the Mtu size.
 * Params:
 *     @size[IN]--The mtu size.
 * Return: None.
 *******************************************************************************/
int bth_l2cap_sendEvent(uint8_t evtID, uint16_t psmID, uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: bth_l2cap_getIdleService
 * Descript: Get the l2cap service.
 * Params: None.
 * Return: Return L2cap service.
 *******************************************************************************/
bth_l2cap_service_t *bth_l2cap_getIdleService(void);

/******************************************************************************
 * Function: bth_l2cap_getUsedService
 * Descript: Get the Used l2cap service.
 * Params: @psmID[IN]--The psm id.
 * Return: Return L2cap service.
 *******************************************************************************/
bth_l2cap_service_t *bth_l2cap_getUsedService(uint16_t psmID);

/******************************************************************************
 * Function: bth_l2cap_getIdleChannel
 * Descript: Get the idle l2cap channel.
 * Params:
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getIdleChannel(void);

/******************************************************************************
 * Function: bth_l2cap_getInitChannel
 * Descript: Get the init l2cap channel.
 * Params:
 *     @scid[IN]--The channel id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getInitChannel(uint16_t scid);

/******************************************************************************
 * Function: bth_l2cap_getUsedChannelByScid
 * Descript: Get the Used l2cap channel by scid.
 * Params:
 *     @scid[IN]--The channel id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getUsedChannelByScid(uint16_t scid);

/******************************************************************************
 * Function: bth_l2cap_getConnChannelByScid
 * Descript: Get the Connect l2cap channel by scid.
 * Params:
 *     @scid[IN]--The channel id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getConnChannelByScid(uint16_t scid);

/******************************************************************************
 * Function: bth_l2cap_getUsedChannelByDcid
 * Descript: Get the Used l2cap channel by data channel id.
 * Params:
 *     @aclHandle[IN]--The acl link handle.
 *     @dcid[IN]--The data channel id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getUsedChannelByDcid(uint16_t aclHandle, uint16_t dcid);

/******************************************************************************
 * Function: bth_l2cap_getConnChannelByDcid
 * Descript: Get the Connect l2cap channel by data channel id.
 * Params:
 *     @aclHandle[IN]--The acl link handle.
 *     @dcid[IN]--The data channel id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getConnChannelByDcid(uint16_t aclHandle, uint16_t dcid);

/******************************************************************************
 * Function: bth_l2cap_getUsedChannelByUser
 * Descript: Get the Used l2cap channel by userid.
 * Params:
 *     @aclHandle[IN]--The acl link handle.
 *     @psmID[IN]--The psm id.
 *     @usrID[ID]--The user id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getUsedChannelByUser(uint16_t aclHandle, uint16_t psmID, uint8_t usrID);

/******************************************************************************
 * Function: bth_l2cap_getConnChannelByUser
 * Descript: Get the Connect l2cap channel by userid.
 * Params:
 *     @aclHandle[IN]--The acl link handle.
 *     @psmID[IN]--The psm id.
 *     @usrID[ID]--The user id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getConnChannelByUser(uint16_t aclHandle, uint16_t psmID, uint8_t usrID);

/******************************************************************************
 * Function: bth_l2cap_getInitChannelByPsm
 * Descript: Get the Init l2cap channel by psm.
 * Params:
 *     @aclHandle[IN]--The acl link handle.
 *     @psmID[IN]--The psm id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getInitChannelByPsm(uint16_t aclHandle, uint16_t psmID);

/******************************************************************************
 * Function: bth_l2cap_getUsedChannelByPsm
 * Descript: Get the Used l2cap channel by psm.
 * Params:
 *     @aclHandle[IN]--The acl link handle.
 *     @psmID[IN]--The psm id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getUsedChannelByPsm(uint16_t aclHandle, uint16_t psmID);

/******************************************************************************
 * Function: bth_l2cap_getUsedChannelByPsm
 * Descript: Get the Connected l2cap channel by psm.
 * Params:
 *     @aclHandle[IN]--The acl link handle.
 *     @psmID[IN]--The psm id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getConnChannelByPsm(uint16_t aclHandle, uint16_t psmID);

/******************************************************************************
 * Function: bth_l2cap_getNoConnChannelByPsm
 * Descript: Get the no Connected l2cap channel by psm.
 * Params:
 *     @aclHandle[IN]--The acl link handle.
 *     @psmID[IN]--The psm id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getNoConnChannelByPsm(uint16_t aclHandle, uint16_t psmID);

/******************************************************************************
 * Function: bth_l2cap_getNoDiscChannelByPsm
 * Descript: Get the no DisConnected l2cap channel by psm.
 * Params:
 *     @aclHandle[IN]--The acl link handle.
 *     @psmID[IN]--The psm id.
 * Return: Return L2cap Channel.
 *******************************************************************************/
bth_l2cap_channel_t *bth_l2cap_getNoDiscChannelByPsm(uint16_t aclHandle, uint16_t psmID);

/******************************************************************************
 * Function: bth_l2cap_getSendFifoNumb
 * Descript: Gets the number of TX-FIFOs that can be used.
 * Params: None.
 * Return: The number of TX-FIFOs.
 *******************************************************************************/
int bth_l2cap_getValidTxFifoNumb(void);

/******************************************************************************
 * Function: bth_l2cap_sendData
 * Descript: Send the l2cap data.
 * Params:
 *     @connHandle[IN]--The conntion handle.
 *     @pHead[IN]--The head data.
 *     @headLen[IN]--The headdata length.
 *     @pData[IN]--The payload data.
 *     @dataLen[IN]--The payload data length.
 * Return: Return TLK_ENONE is success or others value is failure.
 *******************************************************************************/
int bth_l2cap_sendData(uint16_t connHandle, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: bth_l2cap_sendChannelData
 * Descript: Send the l2cap data.
 * Params:
 *     @connHandle[IN]--The conntion handle.
 *     @scid[IN]--The channel id.
 *     @pHead[IN]--The head data.
 *     @headLen[IN]--The headdata length.
 *     @pData[IN]--The payload data.
 *     @dataLen[IN]--The payload data length.
 * Return: Return TLK_ENONE is success or others value is failure.
 *******************************************************************************/
int bth_l2cap_sendChannelData(uint16_t connHandle, uint16_t scid, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: bth_l2cap_sendChannelDataExt
 * Descript: Send the l2cap data with user extend data.
 * Params:
 *     @connHandle[IN]--The conntion handle.
 *     @scid[IN]--The channel id.
 *     @pUsrExt[IN]--The user extend data.
 *     @extLen[IN]--The data length.
 *     @pHead[IN]--The head data.
 *     @headLen[IN]--The headdata length.
 *     @pData[IN]--The payload data.
 *     @dataLen[IN]--The payload data length.
 * Return: Return TLK_ENONE is success or others value is failure.
 *******************************************************************************/
int bth_l2cap_sendChannelDataExt(uint16_t connHandle, uint16_t scid, uint8_t *pUsrExt, uint8_t extLen, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen);

int bth_l2cap_sendIFrameSFrame(uint16_t connHandle, uint16_t scid, uint32_t ctrl, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen, bool isAddFcs, bool isExtCtr);
int bth_l2cap_sendIFrame(uint16_t connHandle, uint16_t scid, uint16_t ictrl, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen, bool isAddFcs);
int bth_l2cap_sendExtIFrame(uint16_t connHandle, uint16_t scid, uint16_t ictrl, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen, bool isAddFcs);
int bth_l2cap_sendIFrameWithoutCtrFcs(uint16_t connHandle, uint16_t scid, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen);

int bth_l2cap_sendSFrame(uint16_t connHandle, uint16_t scid, uint16_t sctrl, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen, bool isAddFcs);
int bth_l2cap_sendExtSFrame(uint16_t connHandle, uint16_t scid, uint16_t sctrl, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen, bool isAddFcs);


/******************************************************************************
 * Function: bth_l2cap_recvHandler
 * Descript: Receive the l2cap data.
 * Params:
 *     @connHandle[IN]--The conntion handle.
 *     @llid[IN]--The ll id.
 *     @p[IN]--The data.
 *     @len[IN]--The data length.
 * Return: Return TLK_ENONE is success or others value is fifo blocking.
 *******************************************************************************/
int bth_l2cap_recvHandler(uint16_t handle, uint8_t *pData, uint16_t dataLen);


#endif // BTH_L2CAP_H
