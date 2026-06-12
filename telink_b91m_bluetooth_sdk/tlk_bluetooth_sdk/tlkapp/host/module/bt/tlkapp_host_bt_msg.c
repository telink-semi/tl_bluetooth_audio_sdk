/********************************************************************************************************
 * @file    tlkapp_host_bt_msg.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#if (TLK_STK_BT_ENABLE)
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"

#define TLKAPP_BTMGR_DBG_FLAG 0XFFFFFFFF
#define TLKAPP_BTMGR_DBG_SIGN "[BTMGR]"

/**
 * @brief       This function sends a common response message.
 * @param[in]   cmdID    - command ID.
 * @param[in]   status   - status of the response.
 * @param[in]   reason   - reason code.
 * @param[in]   pData    - pointer to the data payload.
 * @param[in]   dataLen  - length of the data payload.
 * @return      0 if success, otherwise error code.
 */
int tlkapp_btmgr_sendCommRsp(uint8_t cmdID, uint8_t status, uint8_t reason, uint8_t *pData, uint8_t dataLen)
{
    uint8_t buffer[dataLen + TLKPTI_SYS_SERIAL_HEADLEN];
    buffer[0] = TLKPRT_COMM_PTYPE_RSP;
    buffer[1] = TLKPRT_COMM_MTYPE_BT;
    buffer[2] = cmdID;
    buffer[3] = status;
    buffer[4] = reason;
    tmemcpy(buffer + TLKPTI_SYS_SERIAL_HEADLEN, pData, dataLen);
    return tlksys_sendMsg(TLKSYS_TASKID_SYSTEM, TLKSYS_SYS_MSGID_SERIAL_SEND, buffer, dataLen + TLKPTI_SYS_SERIAL_HEADLEN);
}

/**
 * @brief       This function sends a common event message.
 * @param[in]   evtID    - event ID.
 * @param[in]   pData    - pointer to the data payload.
 * @param[in]   dataLen  - length of the data payload.
 * @return      0 if success, otherwise error code.
 */
int tlkapp_btmgr_sendCommEvt(uint8_t evtID, uint8_t *pData, uint8_t dataLen)
{
    uint8_t buffer[dataLen + TLKPTI_SYS_SERIAL_HEADLEN];
    buffer[0] = TLKPRT_COMM_PTYPE_EVT;
    buffer[1] = TLKPRT_COMM_MTYPE_BT;
    buffer[2] = evtID;
    tmemcpy(buffer + TLKPTI_SYS_SERIAL_HEADLEN, pData, dataLen);
    return tlksys_sendMsg(TLKSYS_TASKID_SYSTEM, TLKSYS_SYS_MSGID_SERIAL_SEND, buffer, dataLen + TLKPTI_SYS_SERIAL_HEADLEN);
}

/**
 * @brief       This function handles setting HFP volume.
 * @param[in]   pData    - pointer to data containing volume information.
 * @param[in]   dataLen  - length of data.
 * @return      0 if success, otherwise error code.
 */
__attribute__((weak)) int tlkapp_btmgr_setHfpVolumeDeal(uint8_t *pData, uint8_t dataLen)
{
    (void)dataLen;
    uint16_t handle = ((uint16_t)pData[1] << 8) | pData[0];

    tlkmdi_btacl_item_t *item = tlkmdi_btacl_getConnItem(handle);
    if (item == NULL) {
        return -TLK_ESEEK;
    }
    uint8_t isIos  = 0;
    uint8_t volume = pData[3];
    uint8_t isSrc  = pData[2];
    tlkmdi_tinySql_getPairingDeviceVolume(item->btaddr, false, NULL, &isIos);
    tlkmdi_tinySql_setPairingDeviceVolume(item->btaddr, false, volume, isIos);

    if (!isSrc) {
#if (TLKBTP_CFG_HFPHF_ENABLE)
#if TLKSTK_BTTPSLL_TWS_ENABLE
        tlkmdi_bt_tpt_volSync(handle, false, volume);
#endif
        return btp_hfphf_setSpkVolumeByHandle(item->handle, volume);
#endif
    }
    return TLK_ENONE;
}

/**
 * @brief       This function handles setting AVRCP volume.
 * @param[in]   pData    - pointer to data containing volume information.
 * @param[in]   dataLen  - length of data.
 * @return      0 if success, otherwise error code.
 */
static int tlkapp_btmgr_setAvrcpVolumeDeal(uint8_t *pData, uint8_t dataLen)
{
#if (TLKBTP_CFG_AVRCP_ENABLE)
    (void)dataLen;
    uint16_t             handle      = ((uint16_t)pData[1] << 8) | pData[0];
    uint8_t              isSrc       = pData[2];
    uint8_t              avrcpVolume = pData[3];
    tlkmdi_btacl_item_t *item        = tlkmdi_btacl_getConnItem(handle);
    if (item == NULL) {
        return -TLK_ESEEK;
    }
#if TLKSTK_BTTPSLL_TWS_ENABLE
    tlkmdi_bt_tpt_volSync(handle, true, avrcpVolume);
#endif
    uint8_t isIos = 0;
    tlkmdi_tinySql_getPairingDeviceVolume(item->btaddr, false, NULL, &isIos);
    tlkmdi_tinySql_setPairingDeviceVolume(item->btaddr, true, avrcpVolume, isIos);
    return btp_avrcp_setVolume(handle, avrcpVolume, isSrc);
#else
    (void)dataLen;
    (void)pData;
    return 0;
#endif
}

/**
 * @brief       This function handles setting scan parameters.
 * @param[in]   pData    - pointer to scan parameters.
 * @param[in]   dataLen  - length of data.
 * @return      0 if success, otherwise error code.
 */
static int tlkapp_btmgr_setScanDeal(uint8_t *pData, uint16_t dataLen)
{
#if (TLK_MW_BTREC_ENABLE)
    uint8_t mode;
    uint8_t enPageScan;
    uint8_t enInqScan;
    if (tlkmdi_btacl_getIdleCount() == 0) {
        return TLK_ENONE;
    }

    if (pData == NULL || dataLen < 2) {
        return -TLK_EPARAM;
    }

    enInqScan  = pData[0];
    enPageScan = pData[1];

    uint8_t cur_mode = tlkmdi_btGetScan_state();
    if (cur_mode == TLKMDI_BTSCAN_MODE_BOTH_DISABLE) {
        /*Already closed, do not open again.*/
        return TLK_ENONE;
    }

    mode = (enInqScan && enPageScan) ? TLKMDI_BTSCAN_MODE_BOTH_SCAN :
           (enInqScan)               ? TLKMDI_BTSCAN_MODE_INQUIRY_SCAN :
           (enPageScan)              ? TLKMDI_BTSCAN_MODE_PAGE_SCAN :
                                       TLKMDI_BTSCAN_MODE_BOTH_DISABLE;

    tlkapi_trace(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "tlkapp_btmgr_setScanDeal: mode = %d", mode);

    tlkmdi_btSet_scan(mode, 0xFFFF);
    return TLK_ENONE;

#else
    (void)pData;
    (void)dataLen;
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       This function handles start pairing command.
 * @param[in]   none    - none.
 * @return      none.
 */
static void tlkapp_btmgr_recvStartPairCmdDeal(void)
{
#if (TLK_MW_BTPAIRING_ENABLE)
    tlkmdi_btParing_start();
#endif
}

/**
 * @brief       This function handles trigger Siri command.
 * @param[in]   none    - none.
 * @return      none.
 */
static void tlkapp_btmgr_recvTriggerSiriDeal(void)
{
#if (TLKBTP_CFG_HFPHF_ENABLE)
#if !TLKSTK_BTTPSLL_TWS_ENABLE
    if (bth_handle_getUsedScoCount() != 0) {
        return;
    }
    uint16_t handle = tlkapp_host_bt_selectHandleToTriggerSiriHook();
    tlkmdi_bthfphf_assistant(handle);
#else
    tlkmdi_bt_tpt_triggerSiri();
#endif
#endif
}

/**
 * @brief       Convert the profile type to communication type.
 * @param[in]   ptype    - The profile type.
 * @param[in]   usrID    - The user ID (Client or Server).
 * @return      The communication type (AG/HFP/A2DPSNK/A2DPSRC...).
 */
uint8_t tlkapp_btmgr_ptypeToCtype(uint8_t ptype, uint8_t usrID)
{
    uint8_t ctype = 0xFF;
    switch (ptype) {
    case BTP_PTYPE_HFP:
        if (usrID == BTP_USRID_SERVER) {
            ctype = TLKPRT_COMM_BT_CHN_HFP_AG;
        } else {
            ctype = TLKPRT_COMM_BT_CHN_HFP_HF;
        }
        break;
    case BTP_PTYPE_IAP:
        ctype = TLKPRT_COMM_BT_CHN_IAP;
        break;
    case BTP_PTYPE_SPP:
        ctype = TLKPRT_COMM_BT_CHN_SPP;
        break;
    case BTP_PTYPE_HID:
        ctype = TLKPRT_COMM_BT_CHN_HID;
        break;
    case BTP_PTYPE_ATT:
        ctype = TLKPRT_COMM_BT_CHN_ATT;
        break;
    case BTP_PTYPE_A2DP:
        if (usrID == BTP_USRID_CLIENT) {
            ctype = TLKPRT_COMM_BT_CHN_A2DP_SNK;
        } else {
            ctype = TLKPRT_COMM_BT_CHN_A2DP_SRC;
        }
        break;
    case BTP_PTYPE_AVRCP:
        ctype = TLKPRT_COMM_BT_CHN_AVRCP;
        break;
    case BTP_PTYPE_PBAP:
        ctype = TLKPRT_COMM_BT_CHN_PBAP;
        break;
    case BTP_PTYPE_AVRCP_BROWSING:
        ctype = TLKPRT_COMM_BT_CHN_AVRCP_BROWSING;
        break;
    case BTP_PTYPE_COVERART:
        ctype = TLKPRT_COMM_BT_CHN_COVER_ART;
        break;
    }
    return ctype;
}

/**
 * @brief       Send ACL connect event to app.
 * @param[in]   handle   - The ACL handle.
 * @param[in]   status   - The current status.
 * @param[in]   pBtAddr  - The BT Address.
 * @return      none.
 */
void tlkapp_btmgr_sendAclConnectEvt(uint16_t handle, uint8_t status, uint8_t *pBtAddr)
{
    uint8_t buffLen;
    uint8_t buffer[32];
    tlkapi_trace(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "SendAclConnectEvt[handle-0x%x,status-%d,btaddr-0x%04x]", handle, status, *(uint32_t *)pBtAddr);
    buffLen           = 0;
    buffer[buffLen++] = TLKPRT_COMM_BT_CHN_ACL; // Object
    buffer[buffLen++] = status;                 // Status
    buffer[buffLen++] = (handle & 0x00FF);      // Handle
    buffer[buffLen++] = (handle & 0xFF00) >> 8;
    tmemcpy(buffer + buffLen, pBtAddr, 6); // MAC
    buffLen += 6;
    tlkapp_btmgr_sendCommEvt(TLKPRT_COMM_EVTID_BT_CONNECT, buffer, buffLen);
}

/**
 * @brief       Send ACL disconnect event to app.
 * @param[in]   handle   - The ACL handle.
 * @param[in]   reason   - The disconnect reason.
 * @param[in]   pBtAddr  - The BT Address.
 * @return      none.
 */
void tlkapp_btmgr_sendAclDisconnEvt(uint16_t handle, uint8_t reason, uint8_t *pBtAddr)
{
    uint8_t buffLen;
    uint8_t buffer[32];
    tlkapi_trace(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "SendAclDisconnEvt[handle-0x%x,status-%d,btaddr-0x%04x]", handle, reason, *(uint32_t *)pBtAddr);
    buffLen           = 0;
    buffer[buffLen++] = TLKPRT_COMM_BT_CHN_ACL; // Object
    buffer[buffLen++] = reason;                 // reason
    buffer[buffLen++] = (handle & 0x00FF);      // Handle
    buffer[buffLen++] = (handle & 0xFF00) >> 8;
    tmemcpy(buffer + buffLen, pBtAddr, 6); // MAC
    buffLen += 6;
    tlkapp_btmgr_sendCommEvt(TLKPRT_COMM_EVTID_BT_DISCONN, buffer, buffLen);
}

/**
 * @brief       Send profile connect event to app.
 * @param[in]   handle   - The ACL handle.
 * @param[in]   status   - The current status.
 * @param[in]   ptype    - The profile type.
 * @param[in]   usrID    - The user ID (Client/Server).
 * @param[in]   pBtAddr  - The BT address.
 * @return      none.
 */
void tlkapp_btmgr_sendProfConnectEvt(uint16_t handle, uint8_t status, uint8_t ptype, uint8_t usrID, uint8_t *pBtAddr)
{
    uint8_t buffLen;
    uint8_t buffer[32];
    tlkapi_trace(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "SendProfConnectEvt[handle-0x%x,ptype-%d,status-%d,usrID-%d]", handle, ptype, status, usrID);
    buffLen           = 0;
    buffer[buffLen++] = tlkapp_btmgr_ptypeToCtype(ptype, usrID); // Object
    buffer[buffLen++] = status;                                  // Status
    buffer[buffLen++] = (handle & 0x00FF);                       // Handle
    buffer[buffLen++] = (handle & 0xFF00) >> 8;
    tmemcpy(buffer + buffLen, pBtAddr, 6); // MAC
    buffLen += 6;
    tlkapp_btmgr_sendCommEvt(TLKPRT_COMM_EVTID_BT_PROF_CONN, buffer, buffLen);
}

/**
 * @brief       Send profile disconnect event to app.
 * @param[in]   handle   - The ACL handle.
 * @param[in]   reason   - The disconnect reason.
 * @param[in]   ptype    - The profile type.
 * @param[in]   usrID    - The user ID (Client/Server).
 * @param[in]   pBtAddr  - The BT address.
 * @return      none.
 */
void tlkapp_btmgr_sendProfDisconnEvt(uint16_t handle, uint8_t reason, uint8_t ptype, uint8_t usrID, uint8_t *pBtAddr)
{
    uint8_t buffLen;
    uint8_t buffer[32];
    tlkapi_trace(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "SendProfDisconnEvt[handle-0x%x,ptype-%d,reason-%d,usrID-%d]", handle, ptype, reason, usrID);
    buffLen           = 0;
    buffer[buffLen++] = tlkapp_btmgr_ptypeToCtype(ptype, usrID); // Object
    buffer[buffLen++] = reason;                                  // reason
    buffer[buffLen++] = (handle & 0x00FF);                       // Handle
    buffer[buffLen++] = (handle & 0xFF00) >> 8;
    tmemcpy(buffer + buffLen, pBtAddr, 6); // MAC
    buffLen += 6;
    tlkapp_btmgr_sendCommEvt(TLKPRT_COMM_EVTID_BT_PROF_DISC, buffer, buffLen);
}

/**
 * @brief       This function handles get name command.
 * @param[in]   none    - none.
 * @return      none.
 */
static void tlkapp_btmgr_recvGetNameCmdDeal(void)
{
    uint8_t buffLen;
    uint8_t nameLen;
    uint8_t buffer[64 + 4];

    uint8_t nameBuf[64];
    tlkmdi_bt_mgr_getName(nameBuf);
    nameLen = strlen((char *)nameBuf);
    if (nameLen == 0 || nameLen > 60) {
        nameLen = 60;
    }

    buffLen           = 0;
    buffer[buffLen++] = nameLen;
    tmemcpy(buffer + buffLen, nameBuf, nameLen);
    buffLen += nameLen;
    tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_NAME, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, buffer, buffLen);
}

/**
 * @brief       This function handles get address command.
 * @param[in]   none    - none.
 * @return      none.
 */
static void tlkapp_btmgr_recvGetAddrCmdDeal(void)
{
    uint8_t buffer[6];
    tlkmdi_bt_mgr_getAddr(buffer);
    tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_ADDR, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, buffer, 6);
}

/**
 * @brief       This function handles set name command.
 * @param[in]   pData    - pointer to name data.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvSetNameCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    if (dataLen < 2 || 1 + pData[0] > dataLen) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_SET_NAME, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFORMAT, NULL, 0);
        return;
    }
    tlkmdi_bt_mgr_setName(pData + 1, pData[0]);
    tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_SET_NAME, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
}

/**
 * @brief       This function handles set address command.
 * @param[in]   pData    - pointer to address data.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvSetAddrCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    if (dataLen < 6) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_SET_ADDR, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFORMAT, NULL, 0);
        return;
    }
    tlkmdi_bt_mgr_setAddr(pData);
    tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_SET_ADDR, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
}

/**
 * @brief       This function handles get link key command.
 * @param[in]   pData    - pointer to address data.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvGetLinkKeyCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    int     ret          = TLK_ENONE;
    uint8_t pLinkKey[16] = {0};
    if (dataLen == 0) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_LINKEY, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFORMAT, NULL, 0);
        return;
    } else {
        ret = tlkmdi_tinySql_getPairingDeviceByAddr(pData, NULL, pLinkKey, NULL);
    }
    if (ret != TLK_ENONE) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_LINKEY, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFAIL, NULL, 0);
    } else {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_LINKEY, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, pLinkKey, 16);
    }
}

/**
 * @brief       This function handles get peer name command.
 * @param[in]   pData    - pointer to address data.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvGetPeerNameCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    uint8_t buffer[TINYSQL_PARING_DEVICE_NAME_LENS];
    //uint16_t buffLen;
    if (dataLen < 6) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_PEER_NAME, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFORMAT, NULL, 0);
        return;
    }
    if (tlkmdi_tinySql_getPairingDeviceByAddr(pData, NULL, NULL, buffer) == TLK_ENONE && buffer[0] != 0xff) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_PEER_NAME, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, buffer + 1, buffer[0]);
    } else {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_PEER_NAME, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFORMAT, NULL, 0);
    }
}

/**
 * @brief       This function handles get PDL command.
 * @param[in]   pData    - pointer to request data.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvGetPDLCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    uint8_t index;
    uint8_t start;
    uint8_t count;
    uint8_t devNum;
    uint8_t buffLen;
    uint8_t buffer[86];

    start = pData[0];
    count = pData[1];
    if (dataLen < 2) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_PDL, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFORMAT, NULL, 0);
        return;
    }

    devNum = tlkmdi_tinySql_getPairingDevicesCount();
    if (start >= devNum) {
        count = 0;
    } else if (start + count > devNum) {
        count = devNum - start;
    }
    if (count > 10) {
        count = 10;
    }

    buffLen           = 0;
    buffer[buffLen++] = devNum;
    buffer[buffLen++] = start;
    buffer[buffLen++] = count;
    for (index = 0; index < count; index++) {
        uint32_t devClass;
        uint8_t  devAddr[6];
        int      ret = tlkmdi_tinySql_getPairingDeviceByIndex(start + index, devAddr, &devClass, NULL, NULL);
        if (ret != TLK_ENONE) {
            break;
        }
        buffer[buffLen++] = bth_devClassToDevType(devClass);
        buffer[buffLen++] = 0;
        tmemcpy(buffer + buffLen, devAddr, 6);
        buffLen += 6;
    }
    buffer[2] = index;
    tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_PDL, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, buffer, buffLen);
}

/**
 * @brief       This function handles delete PDL command.
 * @param[in]   pData    - pointer to deletion data.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvDelPDLCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    uint8_t              index;
    uint8_t              count;
    uint8_t              devNum;
    uint8_t              bdaddr[6];
    uint8_t              buffLen;
    uint8_t              buffer[20];
    int                  pPeerItemRet;
    tlkmdi_btacl_item_t *pConnItem;

    count = pData[0];
    if (dataLen < 2) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_DEL_PDL, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFORMAT, NULL, 0);
        return;
    }

    devNum = tlkmdi_tinySql_getPairingDevicesCount();
    if (count > devNum) {
        count = devNum;
    }
    if (count > 10) {
        count = 10;
    }

    pData += 1;
    dataLen -= 1;

    buffLen           = 0;
    buffer[buffLen++] = count;
    for (index = 0; index < count; index++) {
        if (dataLen < 6) {
            buffer[buffLen++] = TLK_EPARAM;
            continue;
        }
        tmemcpy(bdaddr, pData, 6);
        pPeerItemRet = tlkmdi_tinySql_getPairingDeviceByAddr(bdaddr, NULL, NULL, NULL);
        pConnItem    = tlkmdi_btacl_searchUsedItem(bdaddr);

        if (pPeerItemRet != TLK_ENONE) {
            buffer[buffLen++] = TLK_EPARAM;
        } else if (pConnItem != NULL) {
            buffer[buffLen++] = TLK_EBUSY;
        } else {
            buffer[buffLen++] = TLK_ENONE;
            tlkmdi_tinySql_deletePairingDevice(bdaddr);
        }
        pData += 6;
        dataLen -= 6;
    }
    tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_DEL_PDL, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, buffer, buffLen);
}

/**
 * @brief       This function handles clear PDL command.
 * @param[in]   none    - none.
 * @return      none.
 */
static void tlkapp_btmgr_recvClsPDLCmdDeal(void)
{
    if (tlkmdi_btacl_getUsedCount() != 0) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_CLS_PDL, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EBUSY, NULL, 0);
    } else if (tlkmdi_tinySql_getPairingDevicesCount() == 0) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_CLS_PDL, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
    } else {
        tlkmdi_tinySql_cleanPairingDevices();
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_CLS_PDL, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
    }
}

/**
 * @brief       This function handles get CDL command.
 * @param[in]   pData    - pointer to request data.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvGetCDLCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    uint8_t              index;
    uint8_t              start;
    uint8_t              count;
    uint8_t              devNum;
    uint8_t              buffLen;
    uint8_t              buffer[86];
    tlkmdi_btacl_item_t *pItem;

    start = pData[0];
    count = pData[1];
    if (dataLen < 2) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_CDL, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFORMAT, NULL, 0);
        return;
    }

    devNum = tlkmdi_btacl_getConnCount();
    if (start >= devNum) {
        count = 0;
    } else if (start + count > devNum) {
        count = devNum - start;
    }
    if (count > 10) {
        count = 10;
    }

    buffLen           = 0;
    buffer[buffLen++] = devNum;
    buffer[buffLen++] = start;
    buffer[buffLen++] = count;
    for (index = 0; index < count; index++) {
        pItem = tlkmdi_btacl_getConnItemByIndex(start + index);
        if (pItem == NULL) {
            break;
        }
        buffer[buffLen++] = pItem->handle & 0xFF;
        buffer[buffLen++] = (pItem->handle & 0xFF00) >> 8;
        tmemcpy(buffer + buffLen, pItem->btaddr, 6);
        buffLen += 6;
    }
    buffer[2] = index;
    tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_GET_CDL, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, buffer, buffLen);
}

#if (TLK_MW_BTINQ_ENABLE)
/**
 * @brief       Callback for inquiry report.
 * @param[in]   devClass  - device class.
 * @param[in]   rssi      - RSSI value.
 * @param[in]   nameLen   - length of device name.
 * @param[in]   pBtaddr   - pointer to BT address.
 * @param[in]   pBtName   - pointer to BT name.
 * @return      0 if success, otherwise error code.
 */
static int tlkapp_btmgr_inquiryReportCB(uint32_t devClass, uint8_t rssi, uint8_t nameLen, uint8_t *pBtaddr, uint8_t *pBtName)
{
    uint8_t buffLen;
    uint8_t buffer[72];

    buffLen           = 0;
    buffer[buffLen++] = bth_devClassToDevType(devClass);
    buffer[buffLen++] = rssi;
    tmemcpy(buffer + buffLen, pBtaddr, 6);
    buffLen += 6;
    if (nameLen > 64) {
        nameLen = 64;
    }
    buffer[buffLen++] = nameLen;
    if (nameLen != 0 && pBtName != NULL) {
        tmemcpy(buffer + buffLen, pBtName, nameLen);
        buffLen += nameLen;
    }
    return tlkapp_btmgr_sendCommEvt(TLKPRT_COMM_EVTID_BT_INQUIRY, buffer, buffLen);
}

/**
 * @brief       Callback for inquiry completion.
 * @param[in]   none    - none.
 * @return      none.
 */
static void tlkapp_btmgr_inquiryCompleteCB(void)
{
    tlkmdi_btinq_regCallback(NULL, NULL);
    tlkapp_btmgr_sendCommEvt(TLKPRT_COMM_EVTID_BT_INQUIRY_COMPLETE, NULL, 0);
}

/**
 * @brief       This function handles inquiry command.
 * @param[in]   pData    - pointer to inquiry parameters.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvInquiryCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    int     ret;
    uint8_t type;
    uint8_t numb;
    uint8_t rssi;
    uint8_t reason;
    uint8_t inqWind;

    type    = pData[0];
    numb    = pData[1];
    rssi    = pData[2];
    inqWind = pData[3];

    if (dataLen < 5 || (type != 1 && type != 2 && type != 4) || inqWind > 100 || inqWind < 3) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_INQUIRY, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EPARAM, NULL, 0);
        return;
    }

    ret = tlkmdi_btinq_start(type, rssi, numb, inqWind);
    if (ret == TLK_ENONE) {
        tlkmdi_btinq_regCallback(tlkapp_btmgr_inquiryReportCB, tlkapp_btmgr_inquiryCompleteCB);
        reason = TLK_ENONE;
    } else if (ret == -TLK_EBUSY) {
        reason = TLK_EBUSY;
    } else {
        reason = TLK_EPARAM;
    }

    if (reason == TLK_ENONE) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_INQUIRY, TLKPRT_COMM_RSP_STATUE_SUCCESS, reason, NULL, 0);
    } else {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_INQUIRY, TLKPRT_COMM_RSP_STATUE_FAILURE, reason, NULL, 0);
    }
}

/**
 * @brief       This function handles cancel inquiry command.
 * @param[in]   none    - none.
 * @return      none.
 */
static void tlkapp_btmgr_recvInqCancelCmdDeal(void)
{
    uint8_t reason = TLK_ENONE;

    if (!tlkmdi_btinq_isBusy()) {
        reason = TLK_ENONE;
    } else {
        tlkmdi_btinq_close();
    }

    if (reason == TLK_ENONE) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_INQ_CANCEL, TLKPRT_COMM_RSP_STATUE_SUCCESS, reason, NULL, 0);
    } else {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_INQ_CANCEL, TLKPRT_COMM_RSP_STATUE_FAILURE, reason, NULL, 0);
    }
}

/**
 * @brief       This function starts inquiry process with specified device type.
 * @param[in]   dtype    - device type.
 * @return      none.
 */
void tlkapp_user_start_inquiry(uint8_t dtype)
{
    uint8_t pData[8] = {0};
    uint8_t dataLen  = 0;
    pData[dataLen++] = dtype;                  //device type
    pData[dataLen++] = TLKMDI_BTINQ_ITEM_NUMB; //number of devices
    pData[dataLen++] = 0xAF;                   //RSSI
    pData[dataLen++] = 0x08;                   //inquiry window
    pData[dataLen++] = 0x01;

    tlkapp_btmgr_recvInquiryCmdDeal(pData, dataLen);
}

/**
 * @brief       This function stops ongoing inquiry process.
 * @param[in]   none    - none.
 * @return      none.
 */
void tlkapp_user_stop_inquiry(void)
{
    tlkapp_btmgr_recvInqCancelCmdDeal();
}

/**
 * @brief       This function connects to inquiry device by index.
 * @param[in]   index    - device index.
 * @return      none.
 */
void tlkapp_user_connect_inquiry_device_by_index(uint8_t index)
{
    tlkmdi_btinq_item_t *pItem = tlkmdi_btinq_getItem(index);
    if (pItem != NULL) {
        tlkmdi_btacl_connectEx(pItem->btaddr, 15000);
    }
}

#endif
/**
 * @brief       This function handles connect command.
 * @param[in]   pData    - pointer to connection parameters.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvConnectCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    int      ret;
    uint8_t  object;
    uint16_t handle;
    uint8_t  btAddr[6];
    uint32_t timeout;

    if (dataLen < 6 || (pData[0] != 0 && pData[0] != 1)) {
        tlkapi_error(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "tlkapp_btmgr_recvConnectCmdDeal: Error Param");
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_CONNECT, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EPARAM, NULL, 0);
        return;
    }
    if (pData[0] == 0 && dataLen < 10) {
        tlkapi_error(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "tlkapp_btmgr_recvConnectCmdDeal: Error Length");
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_CONNECT, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EPARAM, NULL, 0);
        return;
    }

    object  = pData[1];
    timeout = (((uint16_t)pData[3] << 8) | pData[2]) * 100;
    if (pData[0] == 0) {
        tmemcpy(btAddr, pData + 4, 6);
    } else {
        handle = ((uint16_t)pData[5] << 8) | pData[4];
    }

    (void)handle;
    if (object == TLKPRT_COMM_BT_CHN_ACL) {
        if (pData[0] != 0) {
            tlkapi_error(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "tlkapp_btmgr_recvConnectCmdDeal: Only Support Method - MAC");
            tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_CONNECT, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_ENOSUPPORT, NULL, 0);
            return;
        }
        ret = tlkmdi_btacl_connectEx(btAddr, timeout);
        if (ret == TLK_ENONE) {
            tlkapi_trace(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "tlkapp_btmgr_recvConnectCmdDeal: Start Connect - none");
            tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_CONNECT, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
        } else if (ret == -TLK_EBUSY) {
            tlkapi_trace(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "tlkapp_btmgr_recvConnectCmdDeal: Start Connect - busy");
            tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_CONNECT, TLKPRT_COMM_RSP_STATUE_COMPLETE, TLK_ENONE, NULL, 0);
        } else {
            tlkapi_error(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "tlkapp_btmgr_recvConnectCmdDeal: Start Connect - fail");
            tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_CONNECT, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFAIL, NULL, 0);
        }
    } else {
        tlkapi_error(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "tlkapp_btmgr_recvConnectCmdDeal: Only Support Object-ACL");
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_CONNECT, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_ENOSUPPORT, NULL, 0);
    }
}

/**
 * @brief       This function handles disconnect command.
 * @param[in]   pData    - pointer to disconnection parameters.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvDisconnCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    int      ret;
    uint8_t  object;
    uint16_t handle    = 0;
    uint8_t  btAddr[6] = {0};

    if (dataLen < 4 || (pData[0] != 0 && pData[0] != 1)) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_DISCONN, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EPARAM, NULL, 0);
        return;
    }
    if (pData[0] == 0 && dataLen < 8) {
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_DISCONN, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EPARAM, NULL, 0);
        return;
    }

    object = pData[1];
    if (pData[0] == 0) {
        tmemcpy(btAddr, pData + 2, 6);
    } else {
        handle = ((uint16_t)pData[3] << 8) | pData[2];
    }
    if (object == TLKPRT_COMM_BT_CHN_ACL) {
        tlkapi_trace(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "tlkapp_btmgr_recvDisconnCmdDeal:object-%d,handle-0x%x", object, handle);
        if (pData[0] == 0) {
            ret = tlkmdi_btacl_disconnByAddr(btAddr);
        } else {
            ret = tlkmdi_btacl_disconn(handle, 0x13);
        }
        if (ret == TLK_ENONE) {
            tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_DISCONN, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
        } else if (ret == -TLK_EBUSY) {
            tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_DISCONN, TLKPRT_COMM_RSP_STATUE_COMPLETE, TLK_ENONE, NULL, 0);
        } else {
            tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_DISCONN, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFAIL, NULL, 0);
        }
    } else {
        tlkapi_error(TLKAPP_BTMGR_DBG_FLAG, TLKAPP_BTMGR_DBG_SIGN, "tlkapp_btmgr_recvDisconnCmdDeal: Only Support Object-ACL");
        tlkapp_btmgr_sendCommRsp(TLKSYS_BT_MSGID_DISCONN, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_ENOSUPPORT, NULL, 0);
    }
}

/**
 * @brief       This function handles close pairing command.
 * @param[in]   pData    - pointer to command data.
 * @param[in]   dataLen  - length of data.
 * @return      none.
 */
static void tlkapp_btmgr_recvClosePairCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    (void)pData;
    (void)dataLen;
#if (TLK_MW_BTPAIRING_ENABLE)
    tlkmdi_btSet_scan(0, 0);
    tlkmdi_btParing_stop();
#endif
}

/**
 * @brief       This function handles HFP send accept command.
 * @param[in]   pData    - pointer to command data.
 * @param[in]   dataLen  - length of data.
 * @return      0 if success, otherwise error code.
 */
__attribute__((weak)) int tlkapp_btmgr_recvHfsendAcceptDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
#if (TLKBTP_CFG_HFPHF_ENABLE)
    uint16_t handle = *((uint16_t *)pData); //msg p[0] p[1] is handle
#if !(TLKSTK_BTTPSLL_TWS_ENABLE)
    if (handle == 0xFFFF) { // TODO:BQ Modify:ZW Only fit signal link, multiple point have bug, random hang up one of phone.
        handle = btp_hfphf_getCurHandle();
    }
    tlkmdi_hfphf_ctrl_t *pItem = tlkmdi_hfphf_getItem(handle);
    if (pItem == NULL) {
        return -TLK_ENOITEM;
    }
#if TLK_BT_MULTIPNT_ENABLE
    uint8_t gBtpHfpNumber = btp_hfp_getsBtpHfpNumber();
    if (pItem->status == BTP_HFP_CALL_STATUS_NONE && pItem->setup_status == BTP_HFP_CALL_DIR_NONE) {
        tlk_printf("tlkapp_btmgr_recvHfsendAcceptDeal find error handle = %x", handle);
        for (int i = 0; i < gBtpHfpNumber; i++) {
            handle = btp_hfphf_getCurHandlebyIndex(i);
            pItem  = tlkmdi_hfphf_getItem(handle);
            if (pItem->status != BTP_HFP_CALL_STATUS_NONE && pItem->setup_status != BTP_HFP_CALL_DIR_NONE) {
                break;
            }
        }
    }

#endif //TLK_BT_MULTIPNT_ENABLE

    if (pItem->status == BTP_HFP_CALL_STATUS_START && pItem->setup_status == BTP_HFP_CALL_DIR_INCOMING) {
        return btp_hfphf_answer(handle);
    } else if (pItem->status == BTP_HFP_CALL_STATUS_WAITING && pItem->setup_status == BTP_HFP_CALL_DIR_INCOMING) {
        return tlkmdi_bthfphf_acceptWaitAndHoldActive(handle);
    }
    return -TLK_EFAIL;
#else
    tlkmdi_bt_tpt_callCtrl(handle, true);
    return TLK_ENONE;
#endif
#else
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       This function handles HFP send hangup command.
 * @param[in]   pData    - pointer to command data.
 * @param[in]   dataLen  - length of data.
 * @return      0 if success, otherwise error code.
 */
static int tlkapp_btmgr_recvHfSendHungupDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
#if (TLKBTP_CFG_HFPHF_ENABLE)
    uint16_t handle = *((uint16_t *)pData); //msg p[0] p[1] is handle
#if !(TLKSTK_BTTPSLL_TWS_ENABLE)
    if (handle == 0xFFFF) { // TODO:BQ Modify:ZW Only fit signal link, multiple point have bug, random hang up one of phone.
        handle = btp_hfphf_getCurHandle();
    }
    tlkmdi_hfphf_ctrl_t *pItem = tlkmdi_hfphf_getItem(handle);
    if (pItem == NULL) {
        return -TLK_ENOITEM;
    }
#if TLK_BT_MULTIPNT_ENABLE
    uint8_t gBtpHfpNumber = btp_hfp_getsBtpHfpNumber();
    if (pItem->status == BTP_HFP_CALL_STATUS_NONE && pItem->setup_status == BTP_HFP_CALL_DIR_NONE) {
        tlk_printf("tlkapp_btmgr_recvHfSendHungupDeal find error handle = %x", handle);
        for (int i = 0; i < gBtpHfpNumber; i++) {
            handle = btp_hfphf_getCurHandlebyIndex(i);
            pItem  = tlkmdi_hfphf_getItem(handle);
            if (pItem->status != BTP_HFP_CALL_STATUS_NONE && pItem->setup_status != BTP_HFP_CALL_DIR_NONE) {
                break;
            }
        }
        tlk_printf("------tlkapp_btmgr_recvHfSendHungupDeal test handle3= %x", handle);
    }
#endif //TLK_BT_MULTIPNT_ENABLE
    if (pItem->status == BTP_HFP_CALL_STATUS_START && pItem->setup_status == BTP_HFP_CALL_DIR_INCOMING) {
        return btp_hfphf_reject(handle);
    } else if (pItem->status || pItem->setup_status || pItem->held_status || bth_handle_searchConnSco(handle) != 0) {
        if (pItem->status == BTP_HFP_CALL_STATUS_WAITING && pItem->setup_status == BTP_HFP_CALL_DIR_INCOMING) {
            return tlkmdi_bthfphf_rejectWaitAndKeepActive(handle);
        } else if (pItem->status == BTP_HFP_CALL_STATUS_ACTIVE && pItem->held_status != 0) {
            return tlkmdi_bthfphf_hungupActiveAndResumeHold(handle);
        } else {
            return btp_hfphf_hungUp(handle);
        }
    }
    return -TLK_EFAIL;
#else
    tlkmdi_bt_tpt_callCtrl(handle, false);
    return TLK_ENONE;
#endif
#else
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       This function handles AVRCP send key command.
 * @param[in]   pData    - pointer to key data.
 * @param[in]   dataLen  - length of data.
 * @return      0 if success, otherwise error code.
 */
static int tlkapp_btmgr_recvAvrcpSendKeyDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t  key    = pData[0];
    uint16_t handle = ((uint16_t)pData[2] << 8) | pData[1];
#if TLKSTK_BTTPSLL_TWS_ENABLE
    tlkmdi_bt_tpt_sendAvrcpKey(handle, key);
#else
#if (TLKBTP_CFG_AVRCP_ENABLE)
    btp_avrcp_sendKeyPress(handle, key);
    btp_avrcp_sendKeyRelease(handle, key);
#else
    (void)key;
    (void)handle;
#endif
#endif
    return TLK_ENONE;
}

/**
 * @brief       This function handles Bluetooth messages.
 * @param[in]   msgID    - message ID.
 * @param[in]   pData    - pointer to message data.
 * @param[in]   dataLen  - length of message data.
 * @return      0 if success, otherwise error code.
 */
int tlkapp_btmgr_msgHandle(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    switch (msgID) {
    case TLKSYS_BT_MSGID_CONNECT:
        tlkapp_btmgr_recvConnectCmdDeal(pData, dataLen);
        break;
    case TLKSYS_BT_MSGID_DISCONN:
        tlkapp_btmgr_recvDisconnCmdDeal(pData, dataLen);
        break;
#if (TLK_MW_BTINQ_ENABLE)
    case TLKSYS_BT_MSGID_INQUIRY:
        tlkapp_btmgr_recvInquiryCmdDeal(pData, dataLen);
        break;
    case TLKSYS_BT_MSGID_INQ_CANCEL:
        tlkapp_btmgr_recvInqCancelCmdDeal();
        break;
#endif
    case TLKSYS_BT_MSGID_GET_NAME:
        tlkapp_btmgr_recvGetNameCmdDeal();
        break;
    case TLKSYS_BT_MSGID_GET_ADDR:
        tlkapp_btmgr_recvGetAddrCmdDeal();
        break;
    case TLKSYS_BT_MSGID_SET_NAME:
        tlkapp_btmgr_recvSetNameCmdDeal(pData, dataLen);
        break;
    case TLKSYS_BT_MSGID_SET_ADDR:
        tlkapp_btmgr_recvSetAddrCmdDeal(pData, dataLen);
        break;
    case TLKSYS_BT_MSGID_GET_LINKEY:
        tlkapp_btmgr_recvGetLinkKeyCmdDeal(pData, dataLen);
        break;
    case TLKSYS_BT_MSGID_GET_PEER_NAME:
        tlkapp_btmgr_recvGetPeerNameCmdDeal(pData, dataLen);
        break;
    case TLKSYS_BT_MSGID_GET_PDL:
        tlkapp_btmgr_recvGetPDLCmdDeal(pData, dataLen);
        break;
    case TLKSYS_BT_MSGID_DEL_PDL:
        tlkapp_btmgr_recvDelPDLCmdDeal(pData, dataLen);
        break;
    case TLKSYS_BT_MSGID_CLS_PDL:
        tlkapp_btmgr_recvClsPDLCmdDeal();
        break;
    case TLKSYS_BT_MSGID_GET_CDL:
        tlkapp_btmgr_recvGetCDLCmdDeal(pData, dataLen);
        break;
    case TLKSYS_BT_MSGID_START_PAIR:
        tlkapp_btmgr_recvStartPairCmdDeal();
        break;
    case TLKSYS_BT_MSGID_CLOSE_PAIR:
        tlkapp_btmgr_recvClosePairCmdDeal(pData, dataLen);
        break;
    case TLKSYS_BT_MSGID_TRIGGER_SIRI:
        tlkapp_btmgr_recvTriggerSiriDeal();
        break;
    case TLKSYS_BT_MSGID_SET_HFP_VOLUME:
        return tlkapp_btmgr_setHfpVolumeDeal(pData, dataLen);
    case TLKSYS_BT_MSGID_SET_AVRCP_VOLUME:
        return tlkapp_btmgr_setAvrcpVolumeDeal(pData, dataLen);
    case TLKSYS_BT_MSGID_SET_SCAN:
        return tlkapp_btmgr_setScanDeal(pData, dataLen);
    case TLKSYS_BT_MSGID_HF_SEND_CALL_ACCEPT:
        return tlkapp_btmgr_recvHfsendAcceptDeal(pData, dataLen);
    case TLKSYS_BT_MSGID_HF_SEND_CALL_HUNGUP:
        return tlkapp_btmgr_recvHfSendHungupDeal(pData, dataLen);
    case TLKSYS_BT_MSGID_AVRCP_SEND_KEY:
        return tlkapp_btmgr_recvAvrcpSendKeyDeal(pData, dataLen);
    default:
        return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}


#endif // #if (TLK_STK_BT_ENABLE)
