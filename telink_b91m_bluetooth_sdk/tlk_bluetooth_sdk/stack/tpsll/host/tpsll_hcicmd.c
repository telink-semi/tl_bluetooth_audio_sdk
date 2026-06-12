/********************************************************************************************************
 * @file    tpsll_hcicmd.c
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
#include "tpsll_hcicmd.h"
#include "tpsll_hci.h"
#include "tlkapi/tlkapi.h"

#if (TLK_CFG_TPSLL_HCI_ENABLE)

#define TPSLL_HCICMD_DBG_FLAG 0xFFFFFFFF
#define TPSLL_HCICMD_DBG_SIGN "[tpsll_cmd]"

extern int tlktpsll_hci_sendH2cCmd(uint16_t opcode, uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: tpsll_hci_sendSetBtAddrCmd
 * Descript: Set BT MAC address.
 * Params:
 *        @isTws[IN]--TWS or Headset, 1 TWS, 0 Headset.
 *        @pBtAddr[IN]--The BT MAC.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetBtAddrCmd(uint8_t isTws, uint8_t pBtAddr[6])
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendSetBtAddrCmd");
    if (pBtAddr == NULL) {
        return -TLK_EPARAM;
    }

    buffLen           = 0;
    buffer[buffLen++] = isTws;
    tmemcpy(&buffer[buffLen], pBtAddr, 6);
    buffLen += 6;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_SET_BT_MAC_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendSetDongleMacCmd
 * Descript: Set the dongle MAC address.
 * Params: @isExisted[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetDongleMacCmd(uint8_t pDongleAddr[6])
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendSetDongleMacCmd");
    if (pDongleAddr == NULL) {
        return -TLK_EPARAM;
    }

    buffLen = 0;
    tmemcpy(buffer, pDongleAddr, 6);
    buffLen += 6;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_SET_DONGLE_MAC_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendSetGeneralMacCmd
 * Descript: Get the dongle current connection status.
 * Params: @isExisted[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetGeneralMacCmd(uint8_t pBtAddr[6])
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendSetGeneralMacCmd");
    if (pBtAddr == NULL) {
        return -TLK_EPARAM;
    }

    buffLen = 0;
    tmemcpy(buffer, pBtAddr, 6);
    buffLen += 6;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_SET_GEN_MAC_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendCreateConnectCmd
 * Descript: Create a connection with dongle.
 * Params:
 *        @setupMode[IN]--The bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendCreateConnectCmd(uint8_t setupMode)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendCreateConnectCmd");

    buffLen           = 0;
    buffer[buffLen++] = setupMode;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_CONNECT_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendDisconnCmd
 * Descript: Disconnect a connection with dongle.
 * Params:
 *        @reason[IN]--Disconnect reason, refer to "tpsll_disconnect_reason_for_host_e".
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendDisconnCmd(uint8_t reason)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendDisconnCmd reason[%d]", reason);

    buffLen           = 0;
    buffer[buffLen++] = reason;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_DISCONN_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendWriteAccessCodeAndChnIDCmd
 * Descript: Write the dongle access code and channel ID.
 * Params:
 *        @pBtAddr[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendWriteAccessCodeAndChnIDCmd(uint32_t accessCode, uint8_t chnID)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendWriteAccessCodeAndChnIDCmd");

    buffLen           = 0;
    buffer[buffLen++] = (accessCode & 0x000000FF); // accessCode
    buffer[buffLen++] = (accessCode & 0x0000FF00) >> 8;
    buffer[buffLen++] = (accessCode & 0x00FF0000) >> 16;
    buffer[buffLen++] = (accessCode & 0xFF000000) >> 24;
    buffer[buffLen++] = chnID;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_WRITE_AC_CHNID_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd
 * Descript: Write the dongle access code and channel ID.
 * Params:
 *        @pBtAddr[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd(uint32_t accessCode, uint8_t chnID)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendWriteHeadsetAccessCodeAndChnIDCmd");

    buffLen           = 0;
    buffer[buffLen++] = (accessCode & 0x000000FF); // accessCode
    buffer[buffLen++] = (accessCode & 0x0000FF00) >> 8;
    buffer[buffLen++] = (accessCode & 0x00FF0000) >> 16;
    buffer[buffLen++] = (accessCode & 0xFF000000) >> 24;
    buffer[buffLen++] = chnID;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_WRITE_AC_CHNID_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendSetHeadsetIsSingleCmd
 * Descript: Set the headset is in the single(one-ear) state. 
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetHeadsetIsSingleCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendSetHeadsetIsSingleCmd:");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_SET_SINGLE_ROLE_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadDongleConnStatusCmd
 * Descript: Get the dongle current connection status.
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadDongleConnStatusCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadDongleConnStatusCmd");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_READ_DONGLE_CONN_STATUS_CMD_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadDongleIdleStatusCmd
 * Descript: Get the dongle current connection status.
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadDongleIdleStatusCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadDongleIdleStatusCmd");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_READ_DONGLE_IDLE_STATUS_CMD_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendSetDongleMacExistedCmd
 * Descript: Get the dongle current connection status.
 * Params: @isExisted[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetDongleMacExistedCmd(uint8_t isExisted)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendSetDongleMacExistedCmd");

    buffLen           = 0;
    buffer[buffLen++] = isExisted;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_SET_DONGLE_MAC_EXISTED_CMD_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendExitDongleConnSetupCmd
 * Descript: Get the dongle current connection status.
 * Params: @isExisted[IN]--The Bt address.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendExitDongleConnSetupCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendExitDongleConnSetupCmd");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_EXIT_DONGLE_SETUP_CMD_OPCODE, NULL, 0);
}

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
int tpsll_hci_sendMicDataCmd(uint8_t format, uint8_t idx, uint8_t mic_datalen, uint8_t *mic_data)
{
    uint8_t buffLen;
    uint8_t buffer[48]; // mic pkt len <= 38

    buffLen           = 0;
    buffer[buffLen++] = format;
    buffer[buffLen++] = idx;
    buffer[buffLen++] = mic_datalen;
    tmemcpy(&buffer[buffLen], mic_data, mic_datalen);
    buffLen += mic_datalen;
    return tlktpsll_hci_sendScoData(TPSLL_HCI_SCO_MIC_DATA_OPCODE, buffer, buffLen);
    //return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_SCO_MIC_DATA_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendSetHeadsetCurModeCmd
 * Descript: Set the current headset status.
 * Params:
 *        @mode[IN]-- refer to "tph_mode_for_host_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetHeadsetCurModeCmd(uint8_t mode)
{
    uint8_t buffLen;
    uint8_t buffer[16]; // mic pkt len <= 38

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendSetHeadsetCurModeCmd:");
    if (mode > BIT(7)) {
        return -TLK_EPARAM;
    }

    buffLen           = 0;
    buffer[buffLen++] = mode;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_SET_HEADSET_MODE_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendSetHeadsetCurLEModeCmd
 * Descript: Set the current headset status.
 * Params:
 *        @mode[IN]-- refer to "tph_mode_for_host_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendSetHeadsetCurLEModeCmd(uint8_t mode)
{
    uint8_t buffLen;
    uint8_t buffer[16]; // mic pkt len <= 38

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendSetHeadsetCurLEModeCmd:");
    if (mode > BIT(7)) {
        return -TLK_EPARAM;
    }

    buffLen           = 0;
    buffer[buffLen++] = mode;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_SET_HEADSET_LE_MODE_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetCurModeCmd
 * Descript: Read the current headset status. Refer to "tph_mode_for_host_e"
 * Params: none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetCurModeCmd(void)
{
    uint8_t buffLen;
    uint8_t buffer[16]; // mic pkt len <= 38

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadHeadsetCurModeCmd:");
    buffLen   = 0;
    buffer[0] = 0;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_READ_HEADSET_MODE_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetCurLEModeCmd
 * Descript: Read the current headset status. Refer to "tph_mode_for_host_e"
 * Params: none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetCurLEModeCmd(void)
{
    uint8_t buffLen;
    uint8_t buffer[16]; // mic pkt len <= 38

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadHeadsetCurLEModeCmd:");
    buffLen   = 0;
    buffer[0] = 0;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_READ_HEADSET_LE_MODE_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendClearHeadsetCurModeCmd
 * Descript: Clear the current headset status.
 * Params:
 *        @mode[IN]-- refer to "tph_mode_for_host_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendClearHeadsetCurModeCmd(uint8_t mode)
{
    uint8_t buffLen;
    uint8_t buffer[16]; // mic pkt len <= 38

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendClearHeadsetCurModeCmd:");
    if (mode > BIT(7)) {
        return -TLK_EPARAM;
    }

    buffLen           = 0;
    buffer[buffLen++] = mode;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_CLEAR_HEADSET_MODE_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendClearHeadsetCurLEModeCmd
 * Descript: Clear the current headset status.
 * Params:
 *        @mode[IN]-- refer to "tph_mode_for_host_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendClearHeadsetCurLEModeCmd(uint8_t mode)
{
    uint8_t buffLen;
    uint8_t buffer[16]; // mic pkt len <= 38

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendClearHeadsetCurLEModeCmd:");
    if (mode > BIT(7)) {
        return -TLK_EPARAM;
    }

    buffLen           = 0;
    buffer[buffLen++] = mode;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_CLEAR_HEADSET_LE_MODE_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetIsSingleCmd
 * Descript: Query whether the headset is in the single(one-ear) state. 
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetIsSingleCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadHeadsetIsSingleCmd:");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_READ_HEADSET_SINGLE_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetCurRoleCmd
 * Descript: Query the current role of the headset (master or slave).
 *           4 is slave, 2 is master. Refer to "tph_role_e"
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetCurRoleCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadHeadsetCurRoleCmd:");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_READ_HEADSET_ROLE_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetCurBtAudioStatusCmd
 * Descript: Query the audio status in BT mode (BT Audio to Phone).
 *           4 is voice, 2 is music. refer to "tph_mode_e".
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetCurBtAudioStatusCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadHeadsetCurBtAudioStatusCmd:");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_READ_HEADSET_BT_AUDIO_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetIsLeftCmd
 * Descript: Query the current if left of the headset .
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetIsLeftCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadHeadsetIsLeftCmd:");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_READ_HEADSET_LEFT_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadHeadsetFnoCmd
 * Descript: Query the current frame number.
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadHeadsetFnoCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadHeadsetFnoCmd:");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_READ_HEADSET_FNO_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendHeadsetConnectSetupCmd
 * Descript: start tws headset connect setup
 * Params:
 *        @mode[IN]-- refer to "tpt_headset_setup_mode_for_host_e"
 *        @timeout[IN]-- timeout us
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendHeadsetConnectSetupCmd(uint8_t mode, uint32_t timeout)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendHeadsetConnectSetupCmd: %d %d", mode, timeout);
    uint8_t buffLen = 0;
    uint8_t buffer[8];
    buffer[buffLen++] = mode;
    buffer[buffLen++] = (timeout & 0x000000FF);
    buffer[buffLen++] = (timeout & 0x0000FF00) >> 8;
    buffer[buffLen++] = (timeout & 0x00FF0000) >> 16;
    buffer[buffLen++] = (timeout & 0xFF000000) >> 24;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_START_HEADSET_CONNECTION_SETUP_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendHeadsetCurIsLeftCmd
 * Descript: start tws headset connect setup
 * Params:
 *        none
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendHeadsetCurIsLeftCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendHeadsetCurIsLeftCmd:");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_HEADSET_CUR_IS_LEFT_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendWriteHeadsetPduMsgCmd
 * Descript: start tws headset connect setup
 * Params:
 *        @msgType[IN]-- refer to "tph_pdu_cmd_acl_for_host_e"
 *        @pData[IN]-- 
 *        @dataLen[IN]-- 
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendWriteHeadsetPduMsgCmd(uint8_t msgType, uint8_t *pData, uint8_t dataLen, void (*cb)(uint8_t, uint8_t, uint8_t))
{
    uint8_t  buffLen;
    uint8_t  buffer[256];
    uint32_t callback;

    buffLen = 0;
    // tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendWriteHeadsetPduMsgCmd:");

    buffer[buffLen++] = msgType;
    buffer[buffLen++] = dataLen;
    tmemcpy(&buffer[buffLen], pData, dataLen);
    buffLen += dataLen;
    callback          = (uint32_t)cb;
    buffer[buffLen++] = (callback & 0x000000FF);
    buffer[buffLen++] = (callback & 0x0000FF00) >> 8;
    buffer[buffLen++] = (callback & 0x00FF0000) >> 16;
    buffer[buffLen++] = (callback & 0xFF000000) >> 24;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_MSG_PDU_ACL_CMD, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendWriteProfileSyncInfoCmd
 * Descript: Master set connected profile info to controller
 * Params:
 *        @bitProfile[IN]-- refer to "btp_tws_sync_bit_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendWriteProfileSyncInfoCmd(uint16_t bitProfile, uint8_t isSet)
{
    uint8_t buffer[4];
    uint8_t buffLen = 0;

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendWriteProfileSyncInfoCmd: prf[0x%x], isSet[%d]", bitProfile, isSet);

    buffer[buffLen++] = bitProfile & 0xFF;
    buffer[buffLen++] = (bitProfile >> 8) & 0xFF;
    buffer[buffLen++] = isSet;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_WRITE_PROFILE_SYNC_INFO_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadProfileSyncInfoCmd
 * Descript: Master read connected profile info to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadProfileSyncInfoCmd(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadProfileSyncInfoCmd:");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_READ_PROFILE_SYNC_INFO_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendLmpDataCmd
 * Descript: Master read connected profile info to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendLmpDataCmd(uint8_t dstID, uint8_t msgID, uint8_t cmd, uint8_t *pData, uint8_t dataLen)
{
    // tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendLmpDataCmd:");

    uint8_t buffer[32];
    uint8_t buffLen = 0;

    buffer[buffLen++] = dstID;
    buffer[buffLen++] = msgID;
    buffer[buffLen++] = cmd;
    buffer[buffLen++] = dataLen;
    tmemcpy(&buffer[buffLen], pData, dataLen);
    buffLen += dataLen;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_SEND_LMP_DATA_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendAclDataCmd
 * Descript: Master read connected profile info to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendAclDataCmd(uint8_t dstID, uint8_t msgID, uint8_t cmd, uint8_t *pData, uint8_t dataLen)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendAclDataCmd:");

    uint8_t buffer[64];
    uint8_t buffLen = 0;

    buffer[buffLen++] = dstID;
    buffer[buffLen++] = msgID;
    buffer[buffLen++] = cmd;
    buffer[buffLen++] = dataLen;
    tmemcpy(&buffer[buffLen], pData, dataLen);
    buffLen += dataLen;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_SEND_ACL_DATA_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendReadTwsLinkInfoCmd
 * Descript: Master read connected profile info to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendReadTwsLinkInfoCmd(uint8_t linkType, uint16_t aclHandle)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendReadTwsLinkInfoCmd:");

    uint8_t buffer[8];
    uint8_t buffLen = 0;

    buffer[buffLen++] = linkType;
    buffer[buffLen++] = (aclHandle & 0x00FF);
    buffer[buffLen++] = (aclHandle & 0xFF00) >> 8;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_READ_LINK_INFO_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendStartHandoverCmd
 * Descript: Master send start handover command to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendStartHandoverCmd()
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendStartHandoverCmd:");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_START_HANDOVER_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendStartHandoverRequestCmd
 * Descript: Slave send start handover request to controller
 * Params: None
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendStartHandoverRequestCmd()
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendStartHandoverRequestCmd:");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_START_HANDOVER_REQ_OPCODE, NULL, 0);
}

/******************************************************************************
 * Function: tpsll_hci_sendWriteTwsHandoverInfoCmd
 * Descript: Master send write handover info to controller.
 * Params: 
 *       @pData[IN]--The handover info data.
 *       @dataLen[IN]--The dataLen.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendWriteTwsHandoverInfoCmd(uint8_t *pData, uint16_t dataLen)
{
    tlkapi_array(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_sendWriteTwsHandoverInfoCmd:", pData, dataLen);

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_WRITE_HANDOVER_INFO_OPCODE, pData, dataLen);
}

/******************************************************************************
 * Function: tpsll_hci_sendRequestSyncHostTimerCmd
 * Descript: host request controller do timer sync
 * Params: 
 *       @type[IN]--the type which want to sync.
 *       @magicWord[IN]--magic word (payload) for type.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sendRequestSyncHostTimerCmd(uint8_t type, uint16_t magicWord)
{
    const uint16_t len = sizeof(type) + sizeof(magicWord);
    uint8_t        buffer[len];
    buffer[0] = type;
    buffer[1] = (magicWord & 0x00FF);
    buffer[2] = (magicWord & 0xFF00) >> 8;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TWS_REQUEST_SYNC_HOST_TIMER, buffer, len);
}

int tpsll_hci_startTpsllTaskCmd(uint8_t isLeft, void *futureUse)
{
    (void)futureUse;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_START_TPSLL_TASK, &isLeft, 1);
}

/******************************************************************************
 * Function: tpsll_hci_setPower_Control
 * Descript: use to control local tx power or remote device power.
 * Params:
 *       @obj[IN]--power control obj, local or remove device.
 *       @pwr_opcode[IN]--power control opcode, can set increase/decrease tx power and so on.
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_setPower_Control(uint8_t obj, uint8_t pwr_opcode)
{
    uint8_t buffer[8];
    uint8_t buffLen   = 0;
    buffer[buffLen++] = obj;
    buffer[buffLen++] = pwr_opcode;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TX_POWER_CONTROL_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_setPower_index
 * Descript: use to control local tx power or remote device power.
 * Params:
 *       @pwr_opcode[IN]--power index , set power level index from host, match enum "rf_power_level_index_e"
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_setPower_Index(uint8_t power_index)
{
    uint8_t buffer[8];
    uint8_t buffLen   = 0;
    buffer[buffLen++] = 0x00; //TPH_HOST_LOCAL_POWER_CONTROL;
    buffer[buffLen++] = 0x04; //TPH_HOST_SET_POWER_INDEX;
    buffer[buffLen++] = power_index;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_TX_POWER_CONTROL_OPCODE, buffer, buffLen);
}

/******************************************************************************
 * Function: tpsll_hci_sniff_Request
 * Descript: use to enter sniff mode request.
 * Params:
 *       @obj[IN]--request_type, enter or exit
 * Return: TLK_ENONE is success, other value is failure.
 *******************************************************************************/
int tpsll_hci_sniff_Request(uint8_t request_type)
{
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_SNIFF_REQUEST_OPCODE, &request_type, 1);
}

int tpsll_hci_send_start_dongle_sco_setup(uint8_t audio_mode)
{
    uint8_t buffLen;
    uint8_t buffer[16];

    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_send_start_dongle_sco_setup, mode:[%d]", audio_mode);

    buffLen           = 0;
    buffer[buffLen++] = audio_mode;
    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_DONGLE_SCO_SETUP, buffer, buffLen);
}

int tpsll_hci_send_exit_dongle_sco(void)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_send_exit_dongle_sco");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_DONGLE_EXIT_SCO_SETUP, NULL, 0);
}

int tpsll_hci_set_dongle_ota_status(uint8_t is_ota_running)
{
    tlkapi_trace(TPSLL_HCICMD_DBG_FLAG, TPSLL_HCICMD_DBG_SIGN, "tpsll_hci_set_dongle_ota_status");

    return tlktpsll_hci_sendH2cCmd(TPSLL_HCI_DONGLE_SET_OTA_STATUS, &is_ota_running, 1);
}

#endif
