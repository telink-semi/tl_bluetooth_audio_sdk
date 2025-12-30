/********************************************************************************************************
 * @file    tpsll_hcicmd.h
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
#ifndef TPSLL_HCICMD_H_
#define TPSLL_HCICMD_H_

#if (TLK_CFG_TPSLL_HCI_ENABLE)

enum tpsll_hci_opcode
{
    TPSLL_HCI_NO_OPERATION_CMD_OPCODE = 0x0000,

    TPSLL_HCI_SET_BT_MAC_CMD_OPCODE              = 0x0001,
    TPSLL_HCI_SET_GEN_MAC_CMD_OPCODE             = 0x0002,
    TPSLL_HCI_CONNECT_CMD_OPCODE                 = 0x0003,
    TPSLL_HCI_DISCONN_CMD_OPCODE                 = 0x0004,
    TPSLL_HCI_READ_AC_CHNID_CMD_OPCODE           = 0x0005,
    TPSLL_HCI_MSG_PDU_ACL_CMD                    = 0x0006,
    TPSLL_HCI_WRITE_AC_CHNID_CMD_OPCODE          = 0x0007,
    TPSLL_HCI_READ_DONGLE_CONN_STATUS_CMD_OPCODE = 0x0008,
    TPSLL_HCI_READ_DONGLE_IDLE_STATUS_CMD_OPCODE = 0x0009,
    TPSLL_HCI_SET_DONGLE_MAC_EXISTED_CMD_OPCODE  = 0x000A,
    TPSLL_HCI_EXIT_DONGLE_SETUP_CMD_OPCODE       = 0x000B,
    TPSLL_HCI_SCO_MIC_DATA_OPCODE                = 0x000C,
    TPSLL_HCI_SET_DONGLE_MAC_CMD_OPCODE          = 0x000D,
    TPSLL_HCI_SET_HEADSET_MODE_OPCODE            = 0x000E,
    TPSLL_HCI_CLEAR_HEADSET_MODE_OPCODE          = 0x000F,

    TPSLL_HCI_READ_HEADSET_SINGLE_OPCODE            = 0x0010,
    TPSLL_HCI_READ_HEADSET_ROLE_OPCODE              = 0x0011,
    TPSLL_HCI_READ_HEADSET_BT_AUDIO_OPCODE          = 0x0012,
    TPSLL_HCI_READ_HEADSET_LEFT_OPCODE              = 0x0013,
    TPSLL_HCI_READ_HEADSET_FNO_OPCODE               = 0x0014,
    TPSLL_HCI_READ_HEADSET_MODE_OPCODE              = 0x0015,
    TPSLL_HCI_START_HEADSET_CONNECTION_SETUP_OPCODE = 0x0016,
    TPSLL_HCI_HEADSET_CUR_IS_LEFT_OPCODE            = 0x0017,

    TPSLL_HCI_TWS_WRITE_PROFILE_SYNC_INFO_OPCODE    = 0x0018,
    TPSLL_HCI_TWS_READ_PROFILE_SYNC_INFO_OPCODE     = 0x0019,
    TPSLL_HCI_TWS_SEND_LMP_DATA_OPCODE              = 0x001A,
    TPSLL_HCI_TWS_SEND_ACL_DATA_OPCODE              = 0x001B,
    TPSLL_HCI_TWS_READ_LINK_INFO_OPCODE             = 0x001C,
    TPSLL_HCI_TWS_SET_SINGLE_ROLE_OPCODE            = 0x001D,
    TPSLL_HCI_TWS_WRITE_AC_CHNID_OPCODE             = 0x001E,
    TPSLL_HCI_TWS_START_HANDOVER_OPCODE             = 0x001F,
    TPSLL_HCI_TWS_START_HANDOVER_REQ_OPCODE         = 0x0020,
    TPSLL_HCI_TWS_WRITE_HANDOVER_INFO_OPCODE        = 0x0021,
    TPSLL_HCI_TWS_REQUEST_SYNC_HOST_TIMER           = 0x0022,

	TPSLL_HCI_START_TPSLL_TASK           		    = 0x0023,
    TPSLL_HCI_SET_HEADSET_LE_MODE_OPCODE            = 0x0024,
    TPSLL_HCI_CLEAR_HEADSET_LE_MODE_OPCODE          = 0x0025,
	TPSLL_HCI_READ_HEADSET_LE_MODE_OPCODE           = 0x0026,
    TPSLL_HCI_TX_POWER_CONTROL_OPCODE               = 0x0027,
    TPSLL_HCI_SNIFF_REQUEST_OPCODE                  = 0x0028,
    
    //for mesh_audio_dongle
	TPSLL_HCI_DONGLE_SCO_SETUP                      = 0x0029,
    TPSLL_HCI_DONGLE_EXIT_SCO_SETUP                 = 0x0030,
};

/******************************************************************************
 * Function: tpsll_hci_sendSetBtAddrCmd
 * Descript: Set BT MAC address.
 * Params:
 *        @isTws[IN]--TWS or Headset, 1 TWS, 0 Headset.
 *        @pBtAddr[IN]--The BT MAC.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetBtAddrCmd(uint8_t isTws, uint8_t pBtAddr[6]);

/******************************************************************************
 * Function: tpsll_hci_sendSetDongleMacCmd
 * Descript: Set the dongle MAC address.
 * Params: @isExisted[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetDongleMacCmd(uint8_t pDongleAddr[6]);

/******************************************************************************
 * Function: tpsll_hci_sendSetGeneralMacCmd
 * Descript: Get the dongle current connection status.
 * Params: @isExisted[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetGeneralMacCmd(uint8_t pBtAddr[6]);

/******************************************************************************
 * Function: tpsll_hci_sendCreateConnectCmd
 * Descript: Create a connection with dongle.
 * Params:
 *        @setupMode[IN]--The bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendCreateConnectCmd(uint8_t setupMode);

/******************************************************************************
 * Function: tpsll_hci_sendDisconnCmd
 * Descript: Disconnect a connection with dongle.
 * Params:
 *        @reason[IN]--Disconnect reason, refer to "tpsll_disconnect_reason_for_host_e".
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendDisconnCmd(uint8_t reason);
/******************************************************************************
 * Function: tpsll_hci_sendWriteAccessCodeAndChnIDCmd
 * Descript: Write the dongle access code and channel ID.
 * Params:
 *        @pBtAddr[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendWriteAccessCodeAndChnIDCmd(uint32_t accessCode, uint8_t chnID);

/******************************************************************************
 * Function: tpsll_hci_sendReadDongleConnStatusCmd
 * Descript: Get the dongle current connection status.
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadDongleConnStatusCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendReadDongleIdleStatusCmd
 * Descript: Get the dongle current connection status.
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadDongleIdleStatusCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendSetDongleMacExistedCmd
 * Descript: Get the dongle current connection status.
 * Params: @isExisted[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetDongleMacExistedCmd(uint8_t isExisted);

/******************************************************************************
 * Function: tpsll_hci_sendExitDongleConnSetupCmd
 * Descript: Get the dongle current connection status.
 * Params: @isExisted[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendExitDongleConnSetupCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendMicData
 * Descript: for audio path push mic data to tpsll controller
 * Params:
 *        @format[IN]--The encode format of data, LC3/SBC
 *        @idx[IN]--The mic data idx
 *        @mic_datalen[IN]--The len of mic data
 *        @mic_data[IN]--The mic data after encode.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendMicDataCmd(uint8_t format, uint8_t idx, uint8_t mic_datalen, uint8_t *mic_data);

/******************************************************************************
 * Function: tpsll_hci_sendSetHeadsetCurModeCmd
 * Descript: Set the current headset status.
 * Params:
 *        @mode[IN]-- refer to "tph_mode_for_host_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetHeadsetCurModeCmd(uint8_t mode);

/******************************************************************************
 * Function: tpsll_hci_sendSetHeadsetCurLEModeCmd
 * Descript: Set the current headset status.
 * Params:
 *        @mode[IN]-- refer to "tph_mode_for_host_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetHeadsetCurLEModeCmd(uint8_t mode);

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetCurModeCmd
 * Descript: Read the current headset status.
 * Params:
 *        @mode[IN]-- refer to "tph_mode_for_host_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetCurModeCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetCurLEModeCmd
 * Descript: Read the current headset status. Refer to "tph_mode_for_host_e"
 * Params: none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetCurLEModeCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendClearHeadsetCurModeCmd
 * Descript: Clear the current headset status.
 * Params:
 *        @mode[IN]-- refer to "tph_mode_for_host_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendClearHeadsetCurModeCmd(uint8_t mode);

/******************************************************************************
 * Function: tpsll_hci_sendClearHeadsetCurLEModeCmd
 * Descript: Clear the current headset status.
 * Params:
 *        @mode[IN]-- refer to "tph_mode_for_host_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendClearHeadsetCurLEModeCmd(uint8_t mode);

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetIsSingleCmd
 * Descript: Query whether the headset is in the single(one-ear) state.
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/

int tpsll_hci_sendReadHeadsetIsSingleCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendSetHeadsetIsSingleCmd
 * Descript: Set the headset is in the single(one-ear) state. 
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetHeadsetIsSingleCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetCurRoleCmd
 * Descript: Query the current role of the headset (master or slave).
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetCurRoleCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetCurBtAudioStatusCmd
 * Descript: Query the audio status in BT mode (BT Audio to Phone).
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetCurBtAudioStatusCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetIsLeftCmd
 * Descript: Query the current if left of the headset .
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetIsLeftCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetFnoCmd
 * Descript: Query the current frame number.
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetFnoCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendHeadsetConnectSetupCmd
 * Descript: start tws headset connect setup
 * Params:
 *        @mode[IN]-- refer to "tpt_headset_setup_mode_for_host_e"
 *        @timeout[IN]-- timeout us
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendHeadsetConnectSetupCmd(uint8_t mode, uint32_t timeout);

/******************************************************************************
 * Function: tpsll_hci_sendHeadsetCurIsLeftCmd
 * Descript: start tws headset connect setup
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendHeadsetCurIsLeftCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendWriteHeadsetPduMsgCmd
 * Descript: start tws headset connect setup
 * Params:
 *        @msgType[IN]-- refer to "tph_pdu_cmd_acl_for_host_e"
 *        @pData[IN]-- 
 *        @dataLen[IN]-- 
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendWriteHeadsetPduMsgCmd(uint8_t msgType, uint8_t *pData, uint8_t dataLen, void (*cb)(uint8_t, uint8_t, uint8_t));

/******************************************************************************
 * Function: tpsll_hci_sendWriteProfileSyncInfoCmd
 * Descript: Master set connected profile info to controller
 * Params:
 *        @bitProfile[IN]-- refer to "btp_tws_sync_bit_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendWriteProfileSyncInfoCmd(uint16_t bitProfile, uint8_t isSet);

/******************************************************************************
 * Function: tpsll_hci_sendReadProfileSyncInfoCmd
 * Descript: Master read connected profile info to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadProfileSyncInfoCmd(void);

/******************************************************************************
 * Function: tpsll_hci_sendLmpDataCmd
 * Descript: Master read connected profile info to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendLmpDataCmd(uint8_t dstID, uint8_t msgID, uint8_t cmd, uint8_t *pData, uint8_t dataLen);

/******************************************************************************
 * Function: tpsll_hci_sendAclDataCmd
 * Descript: Master read connected profile info to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendAclDataCmd(uint8_t dstID, uint8_t msgID, uint8_t cmd, uint8_t *pData, uint8_t dataLen);

/******************************************************************************
 * Function: tpsll_hci_sendReadTwsLinkInfoCmd
 * Descript: Master read connected profile info to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadTwsLinkInfoCmd(uint8_t linkType, uint16_t aclHandle);

/******************************************************************************
 * Function: tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd
 * Descript: Master read connected profile info to controller
 * Params: 
 *       @accessCode[IN]--The accesscode.
 *       @chnID[IN]--The channel id.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd(uint32_t accessCode, uint8_t chnID);

/******************************************************************************
 * Function: tpsll_hci_sendStartHandoverCmd
 * Descript: Master send start handover command to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendStartHandoverCmd();

/******************************************************************************
 * Function: tpsll_hci_sendStartHandoverRequestCmd
 * Descript: Slave send start handover request to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendStartHandoverRequestCmd();

/******************************************************************************
 * Function: tpsll_hci_sendWriteTwsHandoverInfoCmd
 * Descript: Master send write handover info to controller.
 * Params: 
 *       @pData[IN]--The handover info data.
 *       @dataLen[IN]--The dataLen.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendWriteTwsHandoverInfoCmd(uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: tpsll_hci_sendRequestSyncHostTimerCmd
 * Descript: host request controller do timer sync
 * Params: 
 *       @type[IN]--the type which want to sync.
 *       @magicWord[IN]--magic word (payload) for type.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendRequestSyncHostTimerCmd(uint8_t type, uint16_t magicWord);

int tpsll_hci_startTpsllTaskCmd(uint8_t isLeft, void *futureUse);
/******************************************************************************
 * Function: tpsll_hci_setPower_Control
 * Descript: use to control local tx power or remote device power.
 * Params:
 *       @obj[IN]--power control obj, local or remove device.
 *       @pwr_opcode[IN]--power control opcode, can set increase/decrease tx power and so on.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_setPower_Control(uint8_t obj, uint8_t pwr_opcode);

/******************************************************************************
 * Function: tpsll_hci_sniff_Request
 * Descript: use to enter sniff mode request.
 * Params:
 *       @obj[IN]--request_type, enter or exit
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sniff_Request(uint8_t request_type);

int tpsll_hci_send_start_dongle_sco_setup(uint8_t audio_mode);
int tpsll_hci_send_exit_dongle_sco(void);
#endif
#endif
