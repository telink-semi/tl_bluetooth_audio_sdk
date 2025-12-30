/********************************************************************************************************
 * @file    bth_hcicmd.c
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
#include "stack/bt/common/co_hci.h"

#include "stack/bt/host/bth/bth_stdio.h"


#define BTH_HCICMD_DBG_FLAG ((TLK_MAJOR_DBGID_BTH << 24) | (TLK_MINOR_DBGID_BTH_CMD << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define BTH_HCICMD_DBG_SIGN NULL

extern int tlkbt_hci_sendH2cCmd(uint16_t opcode, uint8_t *pData, uint8_t dataLen);

/******************************************************************************
 * Function: bth_hci_sendResetCmd
 * Descript: Reset controller.
 * Params:

 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendResetCmd(void)
{
    return tlkbt_hci_sendH2cCmd(HCI_RESET_CMD_OPCODE, 0, 0);
}

/******************************************************************************
 * Function: bth_hci_sendInquiryCmd
 * Descript: Start to Inquiry.
 * Params:
 *        @period[IN]--The inquiry time.
 *        @numRsp[IN]--The num of response.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
// period = n*1.28(n=0x01~0x30)
int bth_hci_sendInquiryCmd(uint8_t period, uint8_t numRsp)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendInquiryCmd");

    buffLen           = 0;
    buffer[buffLen++] = 0x33; // LAP
    buffer[buffLen++] = 0x8b;
    buffer[buffLen++] = 0x9e;
    buffer[buffLen++] = period; // Inquiry_Length: Maximum amount of time specified before the Inquiry is halted.
    buffer[buffLen++] = numRsp; // Num_Responses
    return tlkbt_hci_sendH2cCmd(HCI_INQ_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendInquiryCancelCmd
 * Descript: Cancel Inquiry.
 * Params: None.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendInquiryCancelCmd(void)
{
    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendInquiryCancelCmd");
    return tlkbt_hci_sendH2cCmd(HCI_INQ_CANCEL_CMD_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: bth_hci_sendPeriodicInquiryCmd
 * Descript: Cancel Inquiry.
 * Params:
 *        @period[IN]--The inquiry time.
 *        @numRsp[IN]--The num of response.
 *        @maxPeriod[IN]--The maximum time.
 *        @minPeriod[IN]--The minimum time.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendPeriodicInquiryCmd(uint8_t period, uint8_t numRsp, uint16_t maxPeriod, uint16_t minPeriod)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendPeriodicInquiryCmd");

    buffLen           = 0;
    buffer[buffLen++] = (maxPeriod & 0x00FF); // Packet_Type
    buffer[buffLen++] = (maxPeriod & 0xFF00) >> 8;
    buffer[buffLen++] = (minPeriod & 0x00FF); // Packet_Type
    buffer[buffLen++] = (minPeriod & 0xFF00) >> 8;
    buffer[buffLen++] = 0x33; // LAP
    buffer[buffLen++] = 0x8b;
    buffer[buffLen++] = 0x9e;
    buffer[buffLen++] = period; // Inquiry_Length: Maximum amount of time specified before the Inquiry is halted.
    buffer[buffLen++] = numRsp; // Num_Responses
    return tlkbt_hci_sendH2cCmd(HCI_PER_INQ_MODE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendPeriodicInquiryCancelCmd
 * Descript: Cancel periodic Inquiry.
 * Params: None.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendPeriodicInquiryCancelCmd(void)
{
    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendPeriodicInquiryCancelCmd");
    return tlkbt_hci_sendH2cCmd(HCI_EXIT_PER_INQ_MODE_CMD_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: bth_hci_sendCreateConnectCmd
 * Descript: Create a connection.
 * Params:
 *        @mac[IN]--The bt address.
 *        @pktType[IN]--The packet type.
 *        @mode[IN]--The mode.
 *        @clkOffs[IN]--The clock offset.
 *        @allowRoleSwitch[IN]--allow role switch.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendCreateConnectCmd(uint8_t mac[6], uint16_t pktType, uint8_t mode, uint16_t clkOffs, uint8_t allowRoleSwitch)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendCreateConnectCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    buffer[buffLen++] = (pktType & 0x00FF); // Packet_Type
    buffer[buffLen++] = (pktType & 0xFF00) >> 8;
    buffer[buffLen++] = mode;               // Page_Scan_Repetition_Mode
    buffer[buffLen++] = 0x00;               // Reserved
    buffer[buffLen++] = (clkOffs & 0x00FF); // Clock_Offset
    buffer[buffLen++] = (clkOffs & 0xFF00) >> 8;
    buffer[buffLen++] = allowRoleSwitch; // Allow_Role_Switch
    return tlkbt_hci_sendH2cCmd(HCI_CREATE_CON_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendCancelConnectCmd
 * Descript: Cancel the connection.
 * Params:
 *        @mac[IN]--The bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendCancelConnectCmd(uint8_t mac[6])
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendCancelConnectCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    return tlkbt_hci_sendH2cCmd(HCI_CREATE_CON_CANCEL_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendDisconnCmd
 * Descript: Disconnect a connection.
 * Params:
 *        @handle[IN]--The connected acl handle.
 *        @reason[IN]--Disconnect reason.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendDisconnCmd(uint16_t handle, uint8_t reason)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendDisconnCmd");

    buffLen           = 0;
    buffer[buffLen++] = (handle & 0x00FF); // Connection_Handle
    buffer[buffLen++] = (handle & 0xFF00) >> 8;
    buffer[buffLen++] = reason; // Reason
    return tlkbt_hci_sendH2cCmd(HCI_DISCONNECT_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendRejectConnReqCmd
 * Descript: Reject a connection.
 * Params:
 *        @handle[IN]--The connected acl handle.
 *        @reason[IN]--Disconnect reason.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendRejectConnReqCmd(uint8_t mac[6], uint8_t reason)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendRejectConnReqCmd: reason-%d", reason);

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    buffer[buffLen++] = reason;
    return tlkbt_hci_sendH2cCmd(HCI_REJECT_CON_REQ_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendAcceptConnReqCmd
 * Descript: Accept a connection.
 * Params:
 *        @handle[IN]--The connected acl handle.
 *        @role[IN]--The role of aclHandle.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendAcceptConnReqCmd(uint8_t mac[6], uint8_t role)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendAcceptConnReqCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    buffer[buffLen++] = role; // 0=Become the Master for this connection; 1-Remain the Slave for this connection.
    return tlkbt_hci_sendH2cCmd(HCI_ACCEPT_CON_REQ_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendLinkkeyReqReplyCmd
 * Descript: Reply the link key request.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @pLinkkey[IN]--The link key.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendLinkkeyReqReplyCmd(uint8_t mac[6], uint8_t *pLinkkey)
{
    uint8_t buffLen;
    uint8_t buffer[32];

    tlkapi_array(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendLinkkeyReqReplyCmd - linkkey", pLinkkey, 16);

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    tmemcpy(&buffer[buffLen], pLinkkey, 16);
    buffLen += 16;
    return tlkbt_hci_sendH2cCmd(HCI_LK_REQ_REPLY_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendLinkkeyReqNegReplyCmd
 * Descript: Reply negative to the link key request.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendLinkkeyReqNegReplyCmd(uint8_t mac[6])
{
    uint8_t buffLen;
    uint8_t buffer[32];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendLinkkeyReqNegReplyCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    return tlkbt_hci_sendH2cCmd(HCI_LK_REQ_NEG_REPLY_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendPinCodeReqReplyCmd
 * Descript: Reply  pin code to the pin code request.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @pPinCode[IN]--The pin code.
 *        @pinLen[In]--The pin code length.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendPinCodeReqReplyCmd(uint8_t mac[6], uint8_t *pPinCode, uint8_t pinlen)
{
    uint8_t buffLen;
    uint8_t buffer[32] = {0};

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendPinCodeReqReplyCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    buffer[buffLen++] = pinlen;
    if (pinlen > 0x10) {
        pinlen = 0x10;
    }
    if (pinlen != 0) {
        tmemcpy(&buffer[buffLen], pPinCode, pinlen);
    }
    buffLen += 16;
    return tlkbt_hci_sendH2cCmd(HCI_PIN_CODE_REQ_REPLY_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendPinCodeReqNegReplyCmd
 * Descript: Reply negative to the pin code request.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendPinCodeReqNegReplyCmd(uint8_t mac[6])
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendPinCodeReqNegReplyCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    return tlkbt_hci_sendH2cCmd(HCI_PIN_CODE_REQ_NEG_REPLY_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendChangeConPktTypeeCmd
 * Descript: set packet type command.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @pkt_type[IN] -- The packet type.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendChangeConPktTypeCmd(uint16_t aclHandle, uint16_t pkt_type)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendChangeConPktTypeCmd 0x%x", pkt_type);

    buffLen           = 0;
    buffer[buffLen++] = (aclHandle & 0x00FF); // Connection_Handle
    buffer[buffLen++] = (aclHandle & 0xFF00) >> 8;
    buffer[buffLen++] = (pkt_type & 0x00FF);
    buffer[buffLen++] = (pkt_type & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_CHG_CON_PKT_TYPE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendAuthenticationReqCmd
 * Descript: start authenticate.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendAuthenticationReqCmd(uint16_t aclHandle)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendAuthenticationReqCmd");

    buffLen           = 0;
    buffer[buffLen++] = (aclHandle & 0x00FF); // Connection_Handle
    buffer[buffLen++] = (aclHandle & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_AUTH_REQ_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendSetConnectEncryptReqCmd
 * Descript: start encrypt.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @encryptEnable[IN]--is encrypt enable.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendSetConnectEncryptReqCmd(uint16_t aclHandle, uint8_t encrytEnable)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetConnectEncryptReqCmd");

    buffLen           = 0;
    buffer[buffLen++] = (aclHandle & 0x00FF); // Connection_Handle
    buffer[buffLen++] = (aclHandle & 0xFF00) >> 8;
    buffer[buffLen++] = encrytEnable; // Reason
    return tlkbt_hci_sendH2cCmd(HCI_SET_CON_ENC_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendRemoteNameReqCmd
 * Descript: Request peer device name.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @mode[IN]--The mode.
 *        @clockoffset[IN]--The clock offset.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendRemoteNameReqCmd(uint8_t mac[6], uint8_t mode, uint16_t clockOffset)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendRemoteNameReqCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    buffer[buffLen++] = mode;                   // Page_Scan_Repetition_Mode
    buffer[buffLen++] = 0x00;                   // Reserved
    buffer[buffLen++] = (clockOffset & 0x00FF); // Clock_Offset
    buffer[buffLen++] = (clockOffset & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_REM_NAME_REQ_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendRemoteNameReqCancelCmd
 * Descript: Cancel request peer device name.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendRemoteNameReqCancelCmd(uint8_t mac[6])
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendRemoteNameReqCancelCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    return tlkbt_hci_sendH2cCmd(HCI_REM_NAME_REQ_CANCEL_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendCreateSyncConnReqCmd
 * Descript: Create a sync connection.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @txBandwidth[IN]--The transfer bandwidth.
 *        @rxBandwidth[IN]--The receive bandwidth.
 *        @maxLatency[IN]--The maximum latency.
 *        @voiceSetting[IN]--The voice setting.
 *        @rtnEffort[IN]--Theretransmission effeort.
 *        @pktType[IN]--The packet type.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendCreateSyncConnReqCmd(uint16_t aclHandle, uint32_t txBandwidth, uint32_t rxBandwidth, uint16_t maxLatency, uint16_t voiceSetting, uint8_t rtnEffort,
                                     uint16_t pktType)
{
    uint8_t buffLen;
    uint8_t buffer[32];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetupSyncConnReqCmd");

    buffLen           = 0;
    buffer[buffLen++] = (aclHandle & 0x00FF); // Connection_Handle
    buffer[buffLen++] = (aclHandle & 0xFF00) >> 8;
    buffer[buffLen++] = (txBandwidth & 0x000000FF); // Transmit_Bandwidth
    buffer[buffLen++] = (txBandwidth & 0x0000FF00) >> 8;
    buffer[buffLen++] = (txBandwidth & 0x00FF0000) >> 16;
    buffer[buffLen++] = (txBandwidth & 0xFF000000) >> 24;
    buffer[buffLen++] = (rxBandwidth & 0x000000FF); // Receive_Bandwidth
    buffer[buffLen++] = (rxBandwidth & 0x0000FF00) >> 8;
    buffer[buffLen++] = (rxBandwidth & 0x00FF0000) >> 16;
    buffer[buffLen++] = (rxBandwidth & 0xFF000000) >> 24;
    buffer[buffLen++] = (maxLatency & 0x00FF); // Max_Latency
    buffer[buffLen++] = (maxLatency & 0xFF00) >> 8;
    buffer[buffLen++] = (voiceSetting & 0x00FF); // Voice_Settings
    buffer[buffLen++] = (voiceSetting & 0xFF00) >> 8;
    buffer[buffLen++] = rtnEffort;          // Retransmission_Effort
    buffer[buffLen++] = (pktType & 0x00FF); // Packet_Type
    buffer[buffLen++] = (pktType & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_SETUP_SYNC_CON_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendAcceptSyncConnReqCmd
 * Descript: Accept a sync connection.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @txBandwidth[IN]--The transfer bandwidth.
 *        @rxBandwidth[IN]--The receive bandwidth.
 *        @maxLatency[IN]--The maximum latency.
 *        @voiceSetting[IN]--The voice setting.
 *        @rtnEffort[IN]--Theretransmission effeort.
 *        @pktType[IN]--The packet type.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendAcceptSyncConnReqCmd(uint8_t mac[6], uint32_t txBandwidth, uint32_t rxBandwidth, uint16_t maxLatency, uint16_t voiceSetting, uint8_t rtnEffort, uint16_t pktType)
{
    uint8_t buffLen;
    uint8_t buffer[32];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendAcceptSyncConnReqCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    buffer[buffLen++] = (txBandwidth & 0x000000FF); // Transmit_Bandwidth
    buffer[buffLen++] = (txBandwidth & 0x0000FF00) >> 8;
    buffer[buffLen++] = (txBandwidth & 0x00FF0000) >> 16;
    buffer[buffLen++] = (txBandwidth & 0xFF000000) >> 24;
    buffer[buffLen++] = (rxBandwidth & 0x000000FF); // Receive_Bandwidth
    buffer[buffLen++] = (rxBandwidth & 0x0000FF00) >> 8;
    buffer[buffLen++] = (rxBandwidth & 0x00FF0000) >> 16;
    buffer[buffLen++] = (rxBandwidth & 0xFF000000) >> 24;
    buffer[buffLen++] = (maxLatency & 0x00FF); // Max_Latency
    buffer[buffLen++] = (maxLatency & 0xFF00) >> 8;
    buffer[buffLen++] = (voiceSetting & 0x00FF); // Voice_Settings
    buffer[buffLen++] = (voiceSetting & 0xFF00) >> 8;
    buffer[buffLen++] = rtnEffort;          // Retransmission_Effort
    buffer[buffLen++] = (pktType & 0x00FF); // Packet_Type
    buffer[buffLen++] = (pktType & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_ACCEPT_SYNC_CON_REQ_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendRejectSyncConnReqCmd
 * Descript: Reject a sync connection.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @reason[IN]--The reject reason.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendRejectSyncConnReqCmd(uint8_t mac[6], uint8_t reason)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendRejectSyncConnReq");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    buffer[buffLen++] = reason; // Retransmission_Effort
    return tlkbt_hci_sendH2cCmd(HCI_REJECT_SYNC_CON_REQ_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendIOCapReqReplyCmd
 * Descript: Reply a IO capability request.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendIOCapReqReplyCmd(uint8_t mac[6])
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendIOCapReqReplyCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    buffer[buffLen++] = 3; // IO_Capability
    buffer[buffLen++] = 0; // OOB_Data_Present
    buffer[buffLen++] = 4; // Authentication_Requirements
    return tlkbt_hci_sendH2cCmd(HCI_IO_CAP_REQ_REPLY_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendQosSetupCmd
 * Descript: The HCI_QoS_Setup command is used to specify Quality of Service
             parameters for a Connection_Handle.
 * Params:
 *        @aclHandle[IN]--The acl link handle. Refer to SPEC v5.2 P1085
 *        @unused[IN]-- The value shall be used by the Host.
 *        @serviceType[IN]--The level of service required.
 *        @tokenRate[IN]--The average data rate with which the application transmits data.
 *        @peakBandwidth[IN]--The value limits how fast packets from applications may be sent back-to-back.
 *        @latency[IN]--The maximum acceptable delay of an L2CAP packet to the air-interface.
 *        @delayVariation[IN]--The value of this field is the difference, in microseconds, between the
                               maximum and minimum possible delay of an L2CAP SDU between two L2CAP peers.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendQosSetupCmd(uint16_t aclHandle, uint8_t unused, uint8_t serviceType, uint32_t tokenRate, uint32_t peakBandwidth, uint32_t latency, uint32_t delayVariation)
{
    uint8_t buffLen;
    uint8_t buffer[32];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendQosSetupCmd");

    buffLen           = 0;
    buffer[buffLen++] = (aclHandle & 0x00FF); // Connection_Handle
    buffer[buffLen++] = (aclHandle & 0xFF00) >> 8;
    buffer[buffLen++] = unused;                   // Unused
    buffer[buffLen++] = serviceType;              // Service_Type
    buffer[buffLen++] = (tokenRate & 0x000000FF); // Token_Rate
    buffer[buffLen++] = (tokenRate & 0x0000FF00) >> 8;
    buffer[buffLen++] = (tokenRate & 0x00FF0000) >> 16;
    buffer[buffLen++] = (tokenRate & 0xFF000000) >> 24;
    buffer[buffLen++] = (peakBandwidth & 0x000000FF); // Peak_Bandwidth
    buffer[buffLen++] = (peakBandwidth & 0x0000FF00) >> 8;
    buffer[buffLen++] = (peakBandwidth & 0x00FF0000) >> 16;
    buffer[buffLen++] = (peakBandwidth & 0xFF000000) >> 24;
    buffer[buffLen++] = (latency & 0x000000FF); // Latency
    buffer[buffLen++] = (latency & 0x0000FF00) >> 8;
    buffer[buffLen++] = (latency & 0x00FF0000) >> 16;
    buffer[buffLen++] = (latency & 0xFF000000) >> 24;
    buffer[buffLen++] = (delayVariation & 0x000000FF); // Delay_Variation
    buffer[buffLen++] = (delayVariation & 0x0000FF00) >> 8;
    buffer[buffLen++] = (delayVariation & 0x00FF0000) >> 16;
    buffer[buffLen++] = (delayVariation & 0xFF000000) >> 24;

    return tlkbt_hci_sendH2cCmd(HCI_QOS_SETUP_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendUsrConfirmReqReplyCmd
 * Descript: Send a reply user confirm request.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendUsrConfirmReqReplyCmd(uint8_t mac[6])
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendUsrConfirmReqReplyCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    return tlkbt_hci_sendH2cCmd(HCI_USER_CFM_REQ_REPLY_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendUsrConfirmReqNegReplyCmd
 * Descript: Send a negative reply user confirm request.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendUsrConfirmReqNegReplyCmd(uint8_t mac[6])
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendUsrConfirmReqNegReplyCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    return tlkbt_hci_sendH2cCmd(HCI_USER_CFM_REQ_NEG_REPLY_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendSniffModeCmd
 * Descript: start enter into sniff mode request.
 * Params:
 *        @connHandle[IN]--The connection handle.
 *        @maxInterval[IN]--The maximum interval.
 *        @minInterval[IN]--The minimum interval.
 *        @attempt[IN]--The sniff attempt times.
 *        @timeout[IN]--The sniff time.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendSniffModeCmd(uint16_t connHandle, uint16_t maxInterval, uint16_t minInterval, uint16_t attempt, uint16_t timeout)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    //	tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSniffModeCmd");

    buffLen           = 0;
    buffer[buffLen++] = (connHandle & 0x00FF); // Connection_Handle
    buffer[buffLen++] = (connHandle & 0xFF00) >> 8;
    buffer[buffLen++] = (maxInterval & 0x00FF); // Sniff_Max_Interval
    buffer[buffLen++] = (maxInterval & 0xFF00) >> 8;
    buffer[buffLen++] = (minInterval & 0x00FF); // Sniff_Min_Interval
    buffer[buffLen++] = (minInterval & 0xFF00) >> 8;
    buffer[buffLen++] = (attempt & 0x00FF); // Sniff_Attempt
    buffer[buffLen++] = (attempt & 0xFF00) >> 8;
    buffer[buffLen++] = (timeout & 0x00FF); // Sniff_Timeout
    buffer[buffLen++] = (timeout & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_SNIFF_MODE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendExitSniffModeCmd
 * Descript: start exit sniff mode request.
 * Params:
 *        @connHandle[IN]--The connection handle.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendExitSniffModeCmd(uint16_t connHandle)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    //	tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendExitSniffModeCmd");

    buffLen           = 0;
    buffer[buffLen++] = (connHandle & 0x00FF); // Connection_Handle
    buffer[buffLen++] = (connHandle & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_EXIT_SNIFF_MODE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendRoleDiscoveryCmd
 * Descript: Start role discovery.
 * Params:
 *        @connHandle[IN]--The connection handle.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendRoleDiscoveryCmd(uint16_t connHandle)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendRoleDiscoveryCmd");

    buffLen           = 0;
    buffer[buffLen++] = (connHandle & 0x00FF); // Connection_Handle
    buffer[buffLen++] = (connHandle & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_ROLE_DISCOVERY_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendSwitchRoleReqCmd
 * Descript: Start role switch.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @role[IN]--The link role.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendSwitchRoleReqCmd(uint8_t mac[6], uint8_t role)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSwitchRoleReqCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    buffer[buffLen++] = role; // Role
    return tlkbt_hci_sendH2cCmd(HCI_SWITCH_ROLE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWriteLinkPolicy
 * Descript: Set the link governor policy.
 * Params:
 *        @connHandle[IN]--The acl handle.
 *        @policySettings[IN]--The link policy.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteLinkPolicy(uint16_t connHandle, uint16_t policySettings)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteLinkPolicy");

    buffLen           = 0;
    buffer[buffLen++] = (connHandle & 0x00FF); // Connection_Handle
    buffer[buffLen++] = (connHandle & 0xFF00) >> 8;
    buffer[buffLen++] = (policySettings & 0x00FF); // Link_Policy_Settings
    buffer[buffLen++] = (policySettings & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_WR_LINK_POL_STG_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendReadStoredLinkkeyCmd
 * Descript: Read the link key.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @readAll[IN]--is read all.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendReadStoredLinkkeyCmd(uint8_t mac[6], uint8_t readAll)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendReadStoredLinkkeyCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR
    buffLen += 6;
    buffer[buffLen++] = readAll; // Read_All
    return tlkbt_hci_sendH2cCmd(HCI_RD_STORED_LK_CMD_OPCODE, buffer, buffLen);
}

int bth_hci_sendWriteStoredLinkkeyCmd(uint8_t mac[6], uint8_t *pLinkkey)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteStoredLinkkeyCmd");

    buffLen           = 0;
    buffer[buffLen++] = 1;             // Num_Keys_To_Write
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR[i]
    buffLen += 6;
    tmemcpy(&buffer[buffLen], pLinkkey, 16); // Link_Key[i]
    return tlkbt_hci_sendH2cCmd(HCI_WR_STORED_LK_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendDeleteStoredLinkkeyCmd
 * Descript: delete the link key.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @deleteAll[IN]--is delete all link key.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendDeleteStoredLinkkeyCmd(uint8_t mac[6], uint8_t deleteAll)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendDeleteStoredLinkkeyCmd");

    buffLen = 0;
    tmemcpy(&buffer[buffLen], mac, 6); // BD_ADDR[i]
    buffLen += 6;
    buffer[buffLen++] = deleteAll; // Delete_All
    return tlkbt_hci_sendH2cCmd(HCI_DEL_STORED_LK_CMD_OPCODE, buffer, buffLen);
}

int bth_hci_sendWriteLocalNameCmd(uint8_t *pLocalName)
{
    uint8_t buffer[256];
    uint8_t nameLen;
    tmemset(buffer, 0, 256);
    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteLocalNameCmd");

    if (pLocalName == NULL) {
        return -TLK_EPARAM;
    }
    nameLen = strlen((char *)pLocalName);
    tmemcpy(buffer, pLocalName, nameLen);
    return tlkbt_hci_sendH2cCmd(HCI_WR_LOCAL_NAME_CMD_OPCODE, pLocalName, 249);
}

/******************************************************************************
 * Function: bth_hci_sendWritePageTimeoutCmd
 * Descript: Set a page time to listen page timeout.
 * Params:
 *        @timeout[IN]--The time value.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
/* Page_Timeout, Page Timeout measured in number of Baseband slots.
 * Interval Length = N * 0.625 ms (1 Baseband slot),Range: 0x0001 to 0xFFFF, Time Range: 0.625 ms to 40.9 s
 * Notice that these are just part of but not all Page_Timeout value */
int bth_hci_sendWritePageTimeoutCmd(uint16_t timeout)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWritePageTimeoutCmd");

    buffLen           = 0;
    buffer[buffLen++] = (timeout & 0x00FF); // Page_Timeout
    buffer[buffLen++] = (timeout & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_WR_PAGE_TO_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWriteScanEnableCmd
 * Descript: Start a page Scan.
 * Params:
 *        @enable[IN]--is page scan enable.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteScanEnableCmd(uint8_t enable)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteScanEnable: enable-%d", enable);

    buffLen           = 0;
    buffer[buffLen++] = enable; // Scan_Enable
    return tlkbt_hci_sendH2cCmd(HCI_WR_SCAN_EN_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWritePageScanActivityCmd
 * Descript: Set a page Scan parameter.
 * Params:
 *        @interval[IN]--is page scan interval.
 *        @window[IN]--The page scan window.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
// n*0x625  ms
int bth_hci_sendWritePageScanActivityCmd(uint16_t interval, uint16_t window)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWritePageScanActivityCmd");

    buffLen           = 0;
    buffer[buffLen++] = (interval & 0x00FF); //
    buffer[buffLen++] = (interval & 0xFF00) >> 8;
    buffer[buffLen++] = (window & 0x00FF); //
    buffer[buffLen++] = (window & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_WR_PAGE_SCAN_ACT_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWriteInquiryScanActivityCmd
 * Descript: Set a inquiry Scan parameter.
 * Params:
 *        @interval[IN]--is inquiry scan interval.
 *        @window[IN]--The inquiry scan window.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteInquiryScanActivityCmd(uint16_t interval, uint16_t window)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteInquiryScanActivityCmd");

    buffLen           = 0;
    buffer[buffLen++] = (interval & 0x00FF); //
    buffer[buffLen++] = (interval & 0xFF00) >> 8;
    buffer[buffLen++] = (window & 0x00FF); //
    buffer[buffLen++] = (window & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_WR_INQ_SCAN_ACT_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWriteAuthenEnableCmd
 * Descript: Start a anthentication.
 * Params:
 *        @enable[IN]--is enable authen.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteAuthenEnableCmd(uint8_t enable)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteAuthenEnableCmd");

    buffLen           = 0;
    buffer[buffLen++] = enable; // Authentication_Enable
    return tlkbt_hci_sendH2cCmd(HCI_WR_AUTH_EN_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWriteClassOfDeviceCmd
 * Descript: Set the COD.
 * Params:
 *        @devClass[IN]--the device class.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteClassOfDeviceCmd(uint32_t devClass)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteClassOfDeviceCmd");

    buffLen           = 0;
    buffer[buffLen++] = (devClass & 0x000000FF); // Class_Of_Device:
    buffer[buffLen++] = (devClass & 0x0000FF00) >> 8;
    buffer[buffLen++] = (devClass & 0x00FF0000) >> 16;
    return tlkbt_hci_sendH2cCmd(HCI_WR_CLASS_OF_DEV_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWriteLinkSupervisionTimeoutCmd
 * Descript: Set the link supervision time.
 * Params:
 *        @aclHandle[IN]--the acl handle.
 *        @timeout[IN]--The timeout.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteLinkSupervisionTimeoutCmd(uint16_t aclHandle, uint16_t timeout)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteLinkSupervisionTimeoutCmd");

    buffLen           = 0;
    buffer[buffLen++] = (aclHandle & 0x00FF); // Handle
    buffer[buffLen++] = (aclHandle & 0xFF00) >> 8;
    buffer[buffLen++] = (timeout & 0x00FF); // Link_Supervision_Timeou
    buffer[buffLen++] = (timeout & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_WR_LINK_SUPV_TO_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWriteInquiryModeCmd
 * Descript: Set the inquiry mode
 * Params:
 *        @mode[IN]--the inquiry mode.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteInquiryModeCmd(uint8_t mode)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteInquiryModeCmd");

    buffLen           = 0;
    buffer[buffLen++] = mode; // Inquiry_Mode
    return tlkbt_hci_sendH2cCmd(HCI_WR_INQ_MODE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWriteExtendedInquiryRspCmd
 * Descript: Set the EIR
 * Params:
 *        @fecReq[IN]--0x00 FEC is not required, 0x01 FEC is required.
 *        @pData[IN]--The eir data.
 *        @datalen[IN]--Eir datalen
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteExtendedInquiryRspCmd(uint8_t fecReq, uint8_t *pData, uint8_t dataLen)
{
    // Spec request 'Extended_Inquiry_Response' param 240 octets, but TLKBT_HCI_H2C_CMD_FIFO_SIZE < 240 octets,
    // so here buffer size temp set TLKBT_HCI_H2C_CMD_FIFO_SIZE(100).
    uint8_t buffer[256];
    tmemset(&buffer[0], 0, 256);
    buffer[0] = fecReq;
    tmemcpy(&buffer[1], pData, dataLen);
    tlkapi_array(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteExtendedInquiryRspCmd", buffer, 20);
    return tlkbt_hci_sendH2cCmd(HCI_WR_EXT_INQ_RSP_CMD_OPCODE, buffer, 241);
}

/******************************************************************************
 * Function: bth_hci_sendWriteSimplePairingModeCmd
 * Descript: Set SSP mode.
 * Params:
 *        @spMode[IN]--the mode.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteSimplePairingModeCmd(uint8_t spMode)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteSimplePairingModeCmd");

    buffLen           = 0;
    buffer[buffLen++] = spMode; // Simple_Pairing_Mode
    return tlkbt_hci_sendH2cCmd(HCI_WR_SP_MODE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWriteLookbackModeCmd
 * Descript: Set loopback mode.
 * Params:
 *        @mode[IN]--the mode.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteLookbackModeCmd(uint8_t mode)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteLookbackModeCmd");

    buffLen           = 0;
    buffer[buffLen++] = mode; // Loopback_Mode
    return tlkbt_hci_sendH2cCmd(HCI_WR_LOOPBACK_MODE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendWriteSimpleDebugModeCmd
 * Descript: Set debug mode.
 * Params:
 *        @debugMode[IN]--the mode.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteSimpleDebugModeCmd(uint8_t debugMode)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteSimpleDebugModeCmd");

    buffLen           = 0;
    buffer[buffLen++] = debugMode; // Simple_Pairing_Debug_Mode
    return tlkbt_hci_sendH2cCmd(HCI_WR_SP_DBG_MODE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendReadSecureConnHostSupportCmd
 * Descript: Run the HCI command to enable the low-level security connection function.
 * Params: None.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendReadSecureConnHostSupportCmd(void)
{
    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendReadSecureConnHostSupportCmd");
    return tlkbt_hci_sendH2cCmd(HCI_RD_SEC_CON_HOST_SUPP_CMD_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: bth_hci_sendWriteSecureConnHostSupportCmd
 * Descript: Run the HCI command to control whether the underlying security
 *           connection function is enabled.
 * Params:
 *     @isSupport: True if you want to enable the sercurity connection.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteSecureConnHostSupportCmd(uint8_t isSupport)
{
    uint8_t buffLen;
    uint8_t buffer[8];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendWriteSecureConnHostSupportCmd");

    buffLen = 0;
    if (isSupport) {
        buffer[buffLen++] = 1;
    } else {
        buffer[buffLen++] = 0;
    }
    return tlkbt_hci_sendH2cCmd(HCI_WR_SEC_CON_HOST_SUPP_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendReadRssiCmd
 * Descript: This command reads the Received Signal Strength Indication (RSSI)
 *           value from a Controller.
 * Params:
 *     @connHandle: The Handle for the connection for which the RSSI is to be read.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendReadRssiCmd(uint16_t connHandle)
{
    uint8_t buffLen;
    uint8_t buffer[8];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendReadRssiCmd");

    buffLen           = 0;
    buffer[buffLen++] = (connHandle & 0xFF);
    buffer[buffLen++] = (connHandle & 0xFF00) >> 8;
    return tlkbt_hci_sendH2cCmd(HCI_RD_RSSI_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendSetESCOMuteCmd
 * Descript: Set sco mute mode.
 * Params:
 *        @escoHandle[IN]--the esco handle.
 *        @micSpk[IN]--The mic speaker
 *        @muteEn[IN]--ebable mute.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendSetESCOMuteCmd(uint16_t escoHandle, uint8_t micSpk, uint8_t muteEn)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetESCOMuteCmd");

    buffLen           = 0;
    buffer[buffLen++] = (escoHandle & 0x00FF);
    buffer[buffLen++] = (escoHandle & 0xFF00) >> 8;
    buffer[buffLen++] = micSpk;
    buffer[buffLen++] = muteEn;
    return tlkbt_hci_sendH2cCmd(HCI_DBG_ESCO_MUTE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendSetBtAddrCmd
 * Descript: Set sco mute mode.
 * Params:
 *        @pBtAddr[IN]--the bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendSetBtAddrCmd(uint8_t pBtAddr[6])
{
    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetBtAddrCmd");
    if (pBtAddr == NULL) {
        return -TLK_EPARAM;
    }

    return tlkbt_hci_sendH2cCmd(HCI_DBG_SET_BT_BD_ADDR_CMD_OPCODE, pBtAddr, 6);
}

/******************************************************************************
 * Function: bth_hci_sendSetMaxSlotCmd
 * Descript: set max slot command.
 *        @conn_handle[IN]--the acl handle.
 *        @max_slot[IN]--max_slot, 0x01, 0x03, 0x05
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetMaxSlotCmd(uint16_t conn_handle, uint8_t max_slot)
{
    uint8_t buffLen;
    uint8_t buffer[8];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetMaxSlotCmd handle[%d], maxslot[%d]", conn_handle, max_slot);

    buffLen           = 0;
    buffer[buffLen++] = (conn_handle & 0x00FF);
    buffer[buffLen++] = (conn_handle & 0xFF00) >> 8;
    buffer[buffLen++] = max_slot;
    return tlkbt_hci_sendH2cCmd(HCI_SET_MAX_SLOTS_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendSetMoreDataModeCmd
 * Descript: more data mode command.
 *        @conn_handle[IN]--the acl handle.
 *        @max_slot[IN]--max_slot, 0x01, 0x03, 0x05
 *        audio_packet_num[in]  audio_packet_num: the number of audio packets expected to be received within 600ms
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetMoreDataModeCmd(uint16_t conn_handle, uint8_t more_data_mode, uint8_t audio_packet_num)
{
    uint8_t buffLen;
    uint8_t buffer[8];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetMoreDataModeCmd handle[%d], enable[%d], audio_packet_num[%d]", conn_handle, more_data_mode,
                 audio_packet_num);

    buffLen           = 0;
    buffer[buffLen++] = (conn_handle & 0x00FF);
    buffer[buffLen++] = (conn_handle & 0xFF00) >> 8;
    buffer[buffLen++] = more_data_mode;
    buffer[buffLen++] = audio_packet_num;
    return tlkbt_hci_sendH2cCmd(HCI_SET_MORE_DATA_MODE_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendSetIncPeerPwrMaxCmd
 * Descript: set increase peer power max.
 *        @conn_handle[IN]--the acl handle.
 *        @set en
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetIncPeerPwrMaxCmd(uint16_t conn_handle, uint8_t inc_peer_pwr_en)
{
    uint8_t buffLen;
    uint8_t buffer[8];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetIncPeerPwrMaxCmd");

    buffLen           = 0;
    buffer[buffLen++] = (conn_handle & 0x00FF);
    buffer[buffLen++] = (conn_handle & 0xFF00) >> 8;
    buffer[buffLen++] = inc_peer_pwr_en;
    return tlkbt_hci_sendH2cCmd(HCI_SET_INC_PEER_PWR_MAX_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendSetLinkMaxNbCmd
 * Descript: bt link max number
 *        @ link_max_nb
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetLinkMaxNbCmd(uint8_t link_max_nb)
{
    uint8_t buffLen;
    uint8_t buffer[8];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetLinkMaxNbCmd");

    buffLen           = 0;
    buffer[buffLen++] = link_max_nb;
    return tlkbt_hci_sendH2cCmd(HCI_SET_LINK_MAX_NB_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendEnhancedPowerCntlCmd
 * Descript: set enhanced power control command.
 * Params:
 *        @conn_handle[IN]--acl connection handle
 *        @option[IN]--0: decrement power one step, 1: increment power one step, 2: increase to maximum power

 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendEnhancedPowerCntlCmd(uint16_t conn_handle, uint8_t option)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "HCI_ENHANCED_POWER_CNTL_OPCODE");

    buffLen           = 0;
    buffer[buffLen++] = (conn_handle & 0x00FF);
    buffer[buffLen++] = (conn_handle & 0xFF00) >> 8;
    buffer[buffLen++] = option;

    return tlkbt_hci_sendH2cCmd(HCI_RD_ENH_TX_PWR_LVL_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: bth_hci_sendSetEvtMaskCmd
 * Descript: set event mask command.
 * Params:
 *        @mask[IN]--event mask
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetEvtMaskCmd(uint8_t *mask)
{
    tlkapi_array(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetEvtMaskCmd", mask, 8);

    return tlkbt_hci_sendH2cCmd(HCI_SET_EVT_MASK_CMD_OPCODE, mask, 8);
}

/******************************************************************************
 * Function: bth_hci_sendSetEvtMaskPage2Cmd
 * Descript: set event mask command.
 * Params:
 *        @mask[IN]--event mask
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetEvtMaskPage2Cmd(uint8_t *mask)
{
    tlkapi_array(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetEvtMaskPage2Cmd", mask, 8);

    return tlkbt_hci_sendH2cCmd(HCI_SET_EVT_MASK_PAGE_2_CMD_OPCODE, mask, 8);
}

/******************************************************************************
 * Function: bth_hci_sendReadBufferSizeCmd
 * Descript: Read buffer size command.
 * Params: none.
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendReadBufferSizeCmd(void)
{
    tlkapi_trace(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendReadBufferSizeCmd");

    return tlkbt_hci_sendH2cCmd(HCI_RD_BUFF_SIZE_CMD_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: bth_hci_sendEnDUTModeCmd
 * Descript: none
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendEnDUTModeCmd(void)
{
    return tlkbt_hci_sendH2cCmd(HCI_EN_DUT_MODE_CMD_OPCODE, 0, 0);
}

// struct hci_set_local_ver_info_cmd
// {
//     ///HCI version number
//     uint8_t hci_ver;
//     ///HCI revision number
//     uint16_t hci_rev;
//     ///LMP version
//     uint8_t lmp_ver;
//     ///manufacturer name
//     uint16_t manuf_name;
//     ///LMP Subversion
//     uint16_t lmp_subver;
// }__attribute__((packed));


/******************************************************************************
 * Function: bth_hci_sendLocalVerInfoCmd
 * Descript: Set Local Version command.
 * Params: none.
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/

int bth_hci_sendLocalVerInfoCmd(uint8_t *pData, uint8_t dataLen)
{
    //    if(dataLen <6)
    //    {
    //        tlkapi_trace(0xffffffff, "<TEST>", "bth_hci_sendLocalVerInfoCmd dataLen error");
    //    }
    //    uint8_t buffLen;
    //    uint8_t buffer[16];
    tlkapi_trace(0xffffffff, "<TEST>", "bth_hci_sendLocalVerInfoCmd");

    return tlkbt_hci_sendH2cCmd(HCI_SET_LOCAL_VER_INFO_CMD_OPCODE, pData, dataLen);
}

// hci_rd_local_ver_info_cmd_cmp_evt

/******************************************************************************
 * Function: bth_hci_sendReadLovcalVerInfoCmd
 * Descript: Read Version command.
 * Params: none.
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendReadLocalVerInfoCmd()
{
    tlkapi_trace(0xffffffff, "<TEST>", "bth_hci_sendReadRemVerInfoCmd");
    return tlkbt_hci_sendH2cCmd(HCI_RD_LOCAL_VER_INFO_CMD_OPCODE, 0, 0);
}

/******************************************************************************
 * Function: bth_hci_sendSetAfhHostChnClassiFicationCmd
 * Descript: Host to specify a channel classification based on its “local information”.
 * Params:  pAfhChn[IN]: This parameter contains 80 1-bit fields. 0: channel n is bad
            1: channel n is unknown. The most significant bit (bit 79) is reserved 
            for future use At least Nmin channels shall be marked as unknown.
            dataLen[IN]: Size 10 octets (79 bits meaningful).
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetAfhHostChnClassiFicationCmd(uint8_t *pAfhChn, uint8_t dataLen)
{
    if (dataLen < 10) {
        return -TLK_EPARAM;
    }
    tlkapi_array(BTH_HCICMD_DBG_FLAG, BTH_HCICMD_DBG_SIGN, "bth_hci_sendSetAfhHostChnClassiFicationCmd:", pAfhChn, dataLen);
    return tlkbt_hci_sendH2cCmd(HCI_SET_AFH_HOST_CH_CLASS_CMD_OPCODE, pAfhChn, dataLen);
}
