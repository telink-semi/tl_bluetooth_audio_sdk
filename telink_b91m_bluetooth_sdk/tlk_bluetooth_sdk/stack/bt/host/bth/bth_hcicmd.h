/********************************************************************************************************
 * @file    bth_hcicmd.h
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
#ifndef BTH_HCICMD_H
#define BTH_HCICMD_H

typedef enum
{
    // Link Control Commands
    BTH_HCI_OPCODE_INQUIRY_CMD                    = 0x0401,
    BTH_HCI_OPCODE_INQUIRY_CANCEL_CMD             = 0x0402,
    BTH_HCI_OPCODE_PERIODIC_INQUIRY_MODE_CMD      = 0x0403,
    BTH_HCI_OPCODE_EXIT_PERIODIC_INQUIRY_MODE_CMD = 0x0404,
    BTH_HCI_OPCODE_CREATE_CONNECT_CMD             = 0x0405,
    BTH_HCI_OPCODE_DISCONN_CMD                    = 0x0406,
    BTH_HCI_OPCODE_ADD_SYNC_CON_CMD               = 0x0407,
    BTH_HCI_OPCODE_CREATE_CON_CANCEL_CMD          = 0x0408,
    BTH_HCI_OPCODE_ACCEPT_CON_REQ_CMD             = 0x0409,
    BTH_HCI_OPCODE_REJECT_CON_REQ_CMD             = 0x040A,
    BTH_HCI_OPCODE_LK_REQ_REPLY_CMD               = 0x040B,
    BTH_HCI_OPCODE_LK_REQ_NEG_REPLY_CMD           = 0x040C,
    BTH_HCI_OPCODE_PIN_CODE_REQ_REPLY_CMD         = 0x040D,
    BTH_HCI_OPCODE_PIN_CODE_REQ_NEG_REPLY_CMD     = 0x040E,
    BTH_HCI_OPCODE_CHG_CON_PKT_TYPE_CMD           = 0x040F,
    BTH_HCI_OPCODE_AUTH_REQ_CMD                   = 0x0411,
    BTH_HCI_OPCODE_SET_CON_ENC_CMD                = 0x0413,
    BTH_HCI_OPCODE_CHG_CON_LK_CMD                 = 0x0415,
    BTH_HCI_OPCODE_MASTER_LK_CMD                  = 0x0417,
    BTH_HCI_OPCODE_REM_NAME_REQ_CMD               = 0x0419,
    BTH_HCI_OPCODE_REM_NAME_REQ_CANCEL_CMD        = 0x041A,
    BTH_HCI_OPCODE_RD_REM_SUPP_FEATS_CMD          = 0x041B,
    BTH_HCI_OPCODE_RD_REM_EXT_FEATS_CMD           = 0x041C,
    BTH_HCI_OPCODE_RD_REM_VER_INFO_CMD            = 0x041D,
    BTH_HCI_OPCODE_RD_CLK_OFF_CMD                 = 0x041F,
    BTH_HCI_OPCODE_RD_LMP_HDL_CMD                 = 0x0420,
    BTH_HCI_OPCODE_SETUP_SYNC_CON_CMD             = 0x0428,
    BTH_HCI_OPCODE_ACCEPT_SYNC_CON_REQ_CMD        = 0x0429,
    BTH_HCI_OPCODE_REJECT_SYNC_CON_REQ_CMD        = 0x042A,
    BTH_HCI_OPCODE_IO_CAP_REQ_REPLY_CMD           = 0x042B,
    BTH_HCI_OPCODE_USER_CFM_REQ_REPLY_CMD         = 0x042C,
    BTH_HCI_OPCODE_USER_CFM_REQ_NEG_REPLY_CMD     = 0x042D,
    BTH_HCI_OPCODE_USER_PASSKEY_REQ_REPLY_CMD     = 0x042E,
    BTH_HCI_OPCODE_USER_PASSKEY_REQ_NEG_REPLY_CMD = 0x042F,
    BTH_HCI_OPCODE_REM_OOB_DATA_REQ_REPLY_CMD     = 0x0430,
    BTH_HCI_OPCODE_REM_OOB_DATA_REQ_NEG_REPLY_CMD = 0x0433,
    BTH_HCI_OPCODE_IO_CAP_REQ_NEG_REPLY_CMD       = 0x0434,
    BTH_HCI_OPCODE_ENH_SETUP_SYNC_CON_CMD         = 0x043D,
    BTH_HCI_OPCODE_ENH_ACCEPT_SYNC_CON_CMD        = 0x043E,
} BTH_HCI_CMD_ENUM;

/******************************************************************************
 * Function: bth_hci_sendResetCmd
 * Descript: Reset controller.
 * Params:

 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendResetCmd(void);


/******************************************************************************
 * Function: bth_hci_sendInquiryCmd
 * Descript: Start to Inquiry.
 * Params:
 *        @period[IN]--The inquiry time.
 *        @numRsp[IN]--The num of response.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendInquiryCmd(uint8_t period, uint8_t numRsp);

/******************************************************************************
 * Function: bth_hci_sendInquiryCancelCmd
 * Descript: Cancel Inquiry.
 * Params: None.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendInquiryCancelCmd(void);

/******************************************************************************
 * Function: bth_hci_sendInquiryCancelCmd
 * Descript: Cancel Inquiry.
 * Params:
 *        @period[IN]--The inquiry time.
 *        @numRsp[IN]--The num of response.
 *        @maxPeriod[IN]--The maximum time.
 *        @minPeriod[IN]--The minimum time.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendPeriodicInquiryCmd(uint8_t period, uint8_t numRsp, uint16_t maxPeriod, uint16_t minPeriod);

/******************************************************************************
 * Function: bth_hci_sendPeriodicInquiryCancelCmd
 * Descript: Cancel periodic Inquiry.
 * Params: None.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendPeriodicInquiryCancelCmd(void);

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
int bth_hci_sendCreateConnectCmd(uint8_t mac[6], uint16_t pktType, uint8_t mode, uint16_t clkOffs, uint8_t allowRoleSwitch);

/******************************************************************************
 * Function: bth_hci_sendCancelConnectCmd
 * Descript: Cancel the connection.
 * Params:
 *        @mac[IN]--The bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendCancelConnectCmd(uint8_t mac[6]);

/******************************************************************************
 * Function: bth_hci_sendDisconnCmd
 * Descript: Disconnect a connection.
 * Params:
 *        @handle[IN]--The connected acl handle.
 *        @reason[IN]--Disconnect reason.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendDisconnCmd(uint16_t handle, uint8_t reason);

/******************************************************************************
 * Function: bth_hci_sendRejectConnReqCmd
 * Descript: Reject a connection.
 * Params:
 *        @handle[IN]--The connected acl handle.
 *        @reason[IN]--Disconnect reason.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendRejectConnReqCmd(uint8_t mac[6], uint8_t reason);

/******************************************************************************
 * Function: bth_hci_sendAcceptConnReqCmd
 * Descript: Accept a connection.
 * Params:
 *        @handle[IN]--The connected acl handle.
 *        @role[IN]--The role of aclHandle.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendAcceptConnReqCmd(uint8_t mac[6], uint8_t role);

/******************************************************************************
 * Function: bth_hci_sendLinkkeyReqReplyCmd
 * Descript: Reply the link key request.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @pLinkkey[IN]--The link key.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendLinkkeyReqReplyCmd(uint8_t mac[6], uint8_t *pLinkkey);

/******************************************************************************
 * Function: bth_hci_sendLinkkeyReqNegReplyCmd
 * Descript: Reply negative to the link key request.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendLinkkeyReqNegReplyCmd(uint8_t mac[6]);

/******************************************************************************
 * Function: bth_hci_sendPinCodeReqReplyCmd
 * Descript: Start pin code request.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @pPinCode[IN]--The pin code.
 *        @pinLen[In]--The pin code length.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendPinCodeReqReplyCmd(uint8_t mac[6], uint8_t *pPinCode, uint8_t pinlen);

/******************************************************************************
 * Function: bth_hci_sendPinCodeReqNegReplyCmd
 * Descript: Reply negative to the pin code request.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendPinCodeReqNegReplyCmd(uint8_t mac[6]);
/******************************************************************************
 * Function: bth_hci_sendChangeConPktTypeCmd
 * Descript: set packet type command.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @pkt_type[IN] -- The packet type.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendChangeConPktTypeCmd(uint16_t aclHandle, uint16_t pkt_type);

/******************************************************************************
 * Function: bth_hci_sendAuthenticationReqCmd
 * Descript: start authenticate.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendAuthenticationReqCmd(uint16_t aclHandle);

/******************************************************************************
 * Function: bth_hci_sendSetConnectEncryptReqCmd
 * Descript: start encrypt.
 * Params:
 *        @aclHandle[IN]--The acl link handle.
 *        @encryptEnable[IN]--is encrypt enable.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendSetConnectEncryptReqCmd(uint16_t aclHandle, uint8_t encrytEnable);

/******************************************************************************
 * Function: bth_hci_sendRemoteNameReqCmd
 * Descript: Request peer device name.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @mode[IN]--The mode.
 *        @clockoffset[IN]--The clock offset.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendRemoteNameReqCmd(uint8_t mac[6], uint8_t mode, uint16_t clockOffset);

/******************************************************************************
 * Function: bth_hci_sendRemoteNameReqCancelCmd
 * Descript: Cancel request peer device name.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendRemoteNameReqCancelCmd(uint8_t mac[6]);

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
                                     uint16_t pktType);

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
int bth_hci_sendAcceptSyncConnReqCmd(uint8_t mac[6], uint32_t txBandwidth, uint32_t rxBandwidth, uint16_t maxLatency, uint16_t voiceSetting, uint8_t rtnEffort, uint16_t pktType);

/******************************************************************************
 * Function: bth_hci_sendRejectSyncConnReqCmd
 * Descript: Reject a sync connection.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @reason[IN]--The reject reason.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendRejectSyncConnReqCmd(uint8_t mac[6], uint8_t reason);

/******************************************************************************
 * Function: bth_hci_sendIOCapReqReplyCmd
 * Descript: Reply a IO capability request.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendIOCapReqReplyCmd(uint8_t mac[6]);

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
int bth_hci_sendQosSetupCmd(uint16_t aclHandle, uint8_t unused, uint8_t serviceType, uint32_t tokenRate, uint32_t peakBandwidth, uint32_t latency, uint32_t delayVariation);

/******************************************************************************
 * Function: bth_hci_sendUsrConfirmReqReplyCmd
 * Descript: Send a reply user confirm request.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendUsrConfirmReqReplyCmd(uint8_t mac[6]);

/******************************************************************************
 * Function: bth_hci_sendUsrConfirmReqNegReplyCmd
 * Descript: Send a negative reply user confirm request.
 * Params:
 *        @mac[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendUsrConfirmReqNegReplyCmd(uint8_t mac[6]);

/******************************************************************************
 * Function: bth_hci_sendSniffModeCmd
 * Descript: start enter into sniff mode request.
 * Params:
 *        @connHandle[IN]--The connection handle.
 *        @maxInterval[IN]--The maximum interval.
 *        @minInterval[IN]--The minimum interval.
 *        @attempt[IN]--The sniff attempt times.
 *        @time[IN]--The sniff time.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendSniffModeCmd(uint16_t connHandle, uint16_t maxInterval, uint16_t minInterval, uint16_t attempt, uint16_t timeout);

/******************************************************************************
 * Function: bth_hci_sendExitSniffModeCmd
 * Descript: start exit sniff mode request.
 * Params:
 *        @connHandle[IN]--The connection handle.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendExitSniffModeCmd(uint16_t connHandle);

/******************************************************************************
 * Function: bth_hci_sendRoleDiscoveryCmd
 * Descript: Start role discovery.
 * Params:
 *        @connHandle[IN]--The connection handle.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendRoleDiscoveryCmd(uint16_t connHandle);

/******************************************************************************
 * Function: bth_hci_sendSwitchRoleReqCmd
 * Descript: Start role switch.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @role[IN]--The link role.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendSwitchRoleReqCmd(uint8_t mac[6], uint8_t role);

int bth_hci_sendWriteDefaultLinkPolicy(uint16_t default_policy_settings);

/******************************************************************************
 * Function: bth_hci_sendWriteLinkPolicy
 * Descript: Set the link governor policy.
 * Params:
 *        @connHandle[IN]--The acl handle.
 *        @policySettings[IN]--The link policy.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteLinkPolicy(uint16_t connHandle, uint16_t policySettings);


/******************************************************************************
 * Function: bth_hci_sendReadStoredLinkkeyCmd
 * Descript: Read the link key.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @readAll[IN]--is read all.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendReadStoredLinkkeyCmd(uint8_t mac[6], uint8_t readAll);

/******************************************************************************
 * Function: bth_hci_sendWriteStoredLinkkeyCmd
 * Descript: Write the link key.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @pLinkkey[IN]--The link key.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteStoredLinkkeyCmd(uint8_t mac[6], uint8_t *pLinkkey);

/******************************************************************************
 * Function: bth_hci_sendDeleteStoredLinkkeyCmd
 * Descript: delete the link key.
 * Params:
 *        @mac[IN]--The Bt address.
 *        @deleteAll[IN]--is delete all link key.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendDeleteStoredLinkkeyCmd(uint8_t mac[6], uint8_t deleteAll);

/******************************************************************************
 * Function: bth_hci_sendWriteLocalNameCmd
 * Descript: Write local name.
 * Params:
 *        @pLocalName[IN]--The Bt device name.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteLocalNameCmd(uint8_t *pLocalName);

/******************************************************************************
 * Function: bth_hci_sendWritePageTimeoutCmd
 * Descript: Set a page time to listen page timeout.
 * Params:
 *        @timeout[IN]--The time value.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWritePageTimeoutCmd(uint16_t timeout);


/******************************************************************************
 * Function: bth_hci_sendWriteScanEnableCmd
 * Descript: Start a Scan.
 * Params:
 *        @enable[IN]--is scan enable.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteScanEnableCmd(uint8_t enable);

/******************************************************************************
 * Function: bth_hci_sendWritePageScanActivityCmd
 * Descript: Set a page Scan parameter.
 * Params:
 *        @interval[IN]--is page scan interval.
 *        @window[IN]--The page scan window.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWritePageScanActivityCmd(uint16_t interval, uint16_t window);

/******************************************************************************
 * Function: bth_hci_sendWriteInquiryScanActivityCmd
 * Descript: Set a inquiry Scan parameter.
 * Params:
 *        @interval[IN]--is inquiry scan interval.
 *        @window[IN]--The inquiry scan window.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteInquiryScanActivityCmd(uint16_t interval, uint16_t window);

/******************************************************************************
 * Function: bth_hci_sendWriteAuthenEnableCmd
 * Descript: Start a anthentication.
 * Params:
 *        @enable[IN]--is enable authen.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteAuthenEnableCmd(uint8_t enable);

/******************************************************************************
 * Function: bth_hci_sendWriteClassOfDeviceCmd
 * Descript: Set the COD.
 * Params:
 *        @devClass[IN]--the device class.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteClassOfDeviceCmd(uint32_t devClass);

/******************************************************************************
 * Function: bth_hci_sendWriteLinkSupervisionTimeoutCmd
 * Descript: Set the link supervision time.
 * Params:
 *        @aclHandle[IN]--the acl handle.
 *        @timeout[IN]--The timeout.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteLinkSupervisionTimeoutCmd(uint16_t aclHandle, uint16_t timeout);

/******************************************************************************
 * Function: bth_hci_sendWriteInquiryModeCmd
 * Descript: Set the inquiry mode
 * Params:
 *        @mode[IN]--the inquiry mode.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteInquiryModeCmd(uint8_t mode);

/******************************************************************************
 * Function: bth_hci_sendWriteExtendedInquiryRspCmd
 * Descript: Set the EIR
 * Params:
 *        @fecReq[IN]--0x00 FEC is not required, 0x01 FEC is required.
 *        @pData[IN]--The eir data.
 *        @datalen[IN]--Eir datalen
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteExtendedInquiryRspCmd(uint8_t fecReq, uint8_t *pData, uint8_t dataLen);

/******************************************************************************
 * Function: bth_hci_sendWriteSimplePairingModeCmd
 * Descript: Set SSP mode.
 * Params:
 *        @spMode[IN]--the mode.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteSimplePairingModeCmd(uint8_t spMode);

/******************************************************************************
 * Function: bth_hci_sendWriteLookbackModeCmd
 * Descript: Set loopback mode.
 * Params:
 *        @mode[IN]--the mode.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteLookbackModeCmd(uint8_t mode);

/******************************************************************************
 * Function: bth_hci_sendWriteSimpleDebugModeCmd
 * Descript: Set debug mode.
 * Params:
 *        @debugMode[IN]--the mode.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteSimpleDebugModeCmd(uint8_t debugMode);

/******************************************************************************
 * Function: bth_hci_sendReadSecureConnHostSupportCmd
 * Descript: Run the HCI command to enable the low-level security connection function.
 * Params: None.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendReadSecureConnHostSupportCmd(void);

/******************************************************************************
 * Function: bth_hci_sendReadTcfInfoCmd
 * Descript: Run the HCI command to enable the low-level security connection function.
 * Params: None.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendReadTcfInfoCmd(uint16_t aclHandle);
/******************************************************************************
 * Function: bth_hci_sendWriteSecureConnHostSupportCmd
 * Descript: Run the HCI command to control whether the underlying security
 *           connection function is enabled.
 * Params:
 *     @isSupport: True if you want to enable the sercurity connection.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendWriteSecureConnHostSupportCmd(uint8_t isSupport);

int bth_hci_sendWriteLeHostSupportCmd(uint8_t isSupport);

/******************************************************************************
 * Function: bth_hci_sendReadRssiCmd
 * Descript: This command reads the Received Signal Strength Indication (RSSI)
 *           value from a Controller.
 * Params:
 *     @connHandle: The Handle for the connection for which the RSSI is to be read.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendReadRssiCmd(uint16_t connHandle);

/******************************************************************************
 * Function: bth_hci_sendSetESCOMuteCmd
 * Descript: Set sco mute mode.
 * Params:
 *        @escoHandle[IN]--the esco handle.
 *        @micSpk[IN]--The mic speaker
 *        @muteEn[IN]--ebable mute.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int bth_hci_sendSetESCOMuteCmd(uint16_t escoHandle, uint8_t micSpk, uint8_t muteEn);

/******************************************************************************
 * Function: bth_hci_sendSetBtAddrCmd
 * Descript: Set sco mute mode.
 * Params:
 *        @pBtAddr[IN]--the bt address.

 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetBtAddrCmd(uint8_t pBtAddr[6]);


/******************************************************************************
 * Function: bth_hci_sendSetMaxSlotCmd
 * Descript: set max slot command.
 *        @conn_handle[IN]--the acl handle.
 *        @max_slot[IN]--max_slot, 0x01, 0x03, 0x05
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetMaxSlotCmd(uint16_t conn_handle, uint8_t max_slot);

/******************************************************************************
 * Function: bth_hci_sendSetMoreDataModeCmd
 * Descript: set max slot command.
 *        @conn_handle[IN]--the acl handle.
 *        @more_data_mode[IN]--music on or off
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetMoreDataModeCmd(uint16_t conn_handle, uint8_t more_data_mode, uint8_t audio_packet_num);
/******************************************************************************
 * Function: bth_hci_sendSetLinkMaxNbCmd
 * Descript: bt link max number
 *        @ link_max_nb bit0-bit3: acl number, bit4-bit7: sco number
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetLinkMaxNbCmd(uint8_t link_max_nb);

/******************************************************************************
 * Function: bth_hci_sendSetLeFeatSuppCmd
 * Descript: set controller LE supported feature reporting.
 *        @ le_feat_supp[IN]--0:disable, 1:enable.
 * Return: TLK_ENONE is success, other value is failure.
********************************************************************************/
int bth_hci_sendSetLeFeatSuppCmd(uint8_t le_feat_supp);


/******************************************************************************
 * Function: bth_hci_sendSetEvtMaskCmd
 * Descript: set event mask command.
 * Params:
 *        @mask[IN]--event mask
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetEvtMaskCmd(uint8_t *mask);

/******************************************************************************
 * Function: bth_hci_sendSetEvtMaskPage2Cmd
 * Descript: set event mask command.
 * Params:
 *        @mask[IN]--event mask
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetEvtMaskPage2Cmd(uint8_t *mask);

/******************************************************************************
 * Function: bth_hci_sendSetMaxSlotCmd
 * Descript: set max slot command.
 *        @conn_handle[IN]--the acl handle.
 *        @max_slot[IN]--max_slot, 0x01, 0x03, 0x05
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetMaxSlotCmd(uint16_t conn_handle, uint8_t max_slot);

/******************************************************************************
 * Function: bth_hci_sendReadBufferSizeCmd
 * Descript: Read buffer size command.
 * Params: none.
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendReadBufferSizeCmd(void);

/******************************************************************************
 * Function: bth_hci_sendEnDUTModeCmd
 * Descript: none
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendEnDUTModeCmd(void);

/******************************************************************************
 * Function: bth_hci_sendLocalVerInfoCmd
 * Descript: Set Local Version command.
 * Params: none.
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendLocalVerInfoCmd(uint8_t *pData, uint8_t dataLen);

/******************************************************************************
 * Function: bth_hci_sendReadLovcalVerInfoCmd
 * Descript: Read Version command.
 * Params: none.
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendReadLocalVerInfoCmd();

/******************************************************************************
 * Function: bth_hci_sendSetIncPeerPwrMaxCmd
 * Descript: set increase peer power max.
 *        @conn_handle[IN]--the acl handle.
 *        @set en
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetIncPeerPwrMaxCmd(uint16_t conn_handle, uint8_t inc_peer_pwr_en);

/******************************************************************************
 * Function: bth_hci_sendSetAfhHostChnClassiFicationCmd
 * Descript: Host to specify a channel classification based on its “local information”.
 * Params:  pAfhChn[IN]: This parameter contains 80 1-bit fields. 0: channel n is bad
            1: channel n is unknown. The most significant bit (bit 79) is reserved 
            for future use At least Nmin channels shall be marked as unknown.
            dataLen[IN]: Size 10 octets (79 bits meaningful).
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendSetAfhHostChnClassiFicationCmd(uint8_t *pAfhChn, uint8_t dataLen);

/******************************************************************************
 * Function: bth_hci_sendWriteInquiryScanTypeCmd
 * Descript: send Write Inquiry Scan Type command.
 * Params: none.
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendWriteInquiryScanTypeCmd(uint8_t type);


/******************************************************************************
 * Function: int bth_hci_sendWritePageScanTypeCmd
 * Descript: send Write Page Scan Type command.
 * Params: none.
 * Return: TLK_ENONE is success, other value is failure.
*******************************************************************************/
int bth_hci_sendWritePageScanTypeCmd(uint8_t type);

#endif // BTH_HCICMD_H
