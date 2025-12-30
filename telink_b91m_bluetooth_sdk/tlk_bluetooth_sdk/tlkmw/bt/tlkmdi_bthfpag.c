/********************************************************************************************************
 * @file    tlkmdi_bthfpag.c
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
#include "tlkmw/bt/tlkmw_bt.h"
#if (TLKBTP_CFG_HFPAG_ENABLE)
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth.h"
#include "stack/bt/host/bth/bth_sco.h"
#include "stack/bt/host/bth/bth_define.h"
#include "stack/bt/host/bth/bth_event.h"
#include "stack/bt/host/bth/bth_handle.h"


#define TLKMDI_BTAG_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_AG << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTAG_DBG_SIGN NULL

static uint16_t sTlkMdiAgTimeout = TLKMDI_BTAG_TIMEOUT_MS;
#if TLKMDI_AG_CALL_SETUP
static uint8_t             sTlkMdiHfpAgAttr;
static tlkmdi_hfpag_ctrl_t sTlkMdiHfpAgCtrl;
#endif

static uint8_t            sTlkMdiScoCodecBcsResult = FALSE;
static tlkmdi_btag_ctrl_t sTlkMdiBtAgCtrl;

static void tlkmdi_bthfpag_timer(TlkApiTimerHandle_t pTimer, void *userArg);
static int  tlkmdi_bthfpag_recvCmdCB(uint16_t aclHandle, uint8_t *pCmd, uint8_t cmdLen);
static bool tlkmdi_bthfpag_sendOkCmd(uint16_t aclHandle);
static bool tlkmdi_bthfpag_sendErrorCmd(uint16_t aclHandle);
#if TLKMDI_AG_CALL_SETUP
static void                 tlkmdi_hfpag_resetCall(tlkmdi_hfpag_unit_t *pCall);
static bool                 tlkmdi_bthfpag_sendCievCmd(uint16_t aclHandle, uint8_t indicators, uint8_t indValue);
static bool                 tlkmdi_btbthfpag_recvClccCmdDeal(uint16_t aclHandle);
static bool                 tlkmdi_bthfpag_recvAtdCmdDeal(uint16_t aclHandle, uint8_t *pNumber, uint8_t numbLen);
static bool                 tlkmdi_bthfpag_recvAtaCmdDeal(uint16_t aclHandle);
static bool                 tlkmdi_bthfpag_recvCindCmdDeal(uint16_t aclHandle);
static bool                 tlkmdi_bthfpag_recvChupCmdDeal(uint16_t aclHandle);
static bool                 tlkmdi_bthfpag_recvChldCmdDeal(uint16_t aclHandle, uint8_t *pData, uint16_t dataLen);
static void                 tlkmdi_bthfpag_pushClccCmdDeal(uint16_t aclHandle, tlkmdi_hfpag_unit_t *pCall, uint8_t index);
static uint                 tlkmdi_bthfpag_getCallUnitCount(void);
static tlkmdi_hfpag_unit_t *tlkmdi_bthfpag_getIdleCallUnit(void);
static tlkmdi_hfpag_unit_t *tlkmdi_bthfpag_getWaitCallUnit(void);
static tlkmdi_hfpag_unit_t *tlkmdi_bthfpag_getUsedCallUnit(uint8_t numbLen, uint8_t *pNumber);
static tlkmdi_hfpag_unit_t *tlkmdi_bthfpag_getActiveCallUnit(void);
static void                 tlkmdi_bthfpag_sendStatusChangedEvt(tlkmdi_hfpag_unit_t *pCall, uint8_t evtStatus);
#endif

extern void btp_hfpag_connectCompleteEvt(uint16_t aclHandle);

/**
 * @brief       Initialize the HFP AG module
 * @param[in]   none.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_init(void)
{
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(sTlkMdiBtAgCtrl)));
    memset(&sTlkMdiBtAgCtrl, 0, sizeof(sTlkMdiBtAgCtrl));
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &sTlkMdiBtAgCtrl.timer, TLKMDI_BTAG_TIMEOUT, false, tlkmdi_bthfpag_timer, NULL);
    btp_hfpag_regRecvCmdCB(tlkmdi_bthfpag_recvCmdCB);

#if (TLKMDI_AG_CALL_SETUP)
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(sTlkMdiHfpAgCtrl)));
    memset(&sTlkMdiHfpAgCtrl, 0, sizeof(sTlkMdiHfpAgCtrl));
#endif
    tlkmdi_btsco_regCB(tlkmdi_bthfag_scoActive);
    return TLK_ENONE;
}

/**
 * @brief       Callback function for SCO connection status
 * @param[in]   aclHandle   - The ACL connection handle.
 * @param[in]   scoHandle   - The SCO connection handle.
 * @param[in]   isConn      - Connection status, 1 is connected, 0 is disconnected.
 * @return      none.
 */
void tlkmdi_bthfag_scoActive(uint16_t aclHandle, uint16_t scoHandle, bool isConn)
{
    (void)scoHandle;
    if (isConn) {
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALL, TLKMDI_BTAG_CALL_IN_PROGRESS);
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLSETUP, TLKMDI_BTAG_CALLSETUP_NOT_SETUP);
    } else {
    }
}

/**
 * @brief       Create a SCO connection for HFP AG
 * @param[in]   pBtAddr     - The bluetooth device address.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_createSco(uint8_t *pBtAddr)
{
    int               ret;
    uint8_t           buffer[20];
    uint8_t           tempLen;
    bth_acl_handle_t *pAclHandle;
    bth_sco_handle_t *pScoHandle;

    pAclHandle = bth_handle_searchConnAcl(pBtAddr);
    if (pAclHandle == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_createSco: rejected - no connected ACL device");
        return -TLK_ESTATUS;
    }
    pScoHandle = bth_handle_searchConnSco(pAclHandle->aclHandle);
    if (pScoHandle != NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_createSco: failure - exist");
        return -TLK_ESTATUS;
    }
    if ((sTlkMdiBtAgCtrl.feature & BTP_HFP_HF_FEATURE_CN) == 0) { // HF CN(Codec negotiation) is not supported
        ret = bth_sco_connect(pAclHandle->aclHandle, BTH_SCO_LINK_TYPE_ESCO, BTH_SCO_AIRMODE_CVSD);
        if (ret != TLK_ENONE) {
            tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_createSco: SCO-CVSD connect failure");
            return ret;
        }
    } else { // HF CN is supported, default use MSBC
        STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(buffer)));
        memset(buffer, 0, sizeof(buffer));
        tempLen = strlen("\r\n+BVRA:1\r\n");
        tmemcpy(buffer, "\r\n+BVRA:1\r\n", tempLen);
        ret = btp_hfpag_send(pAclHandle->aclHandle, buffer, tempLen);
        if (ret != TLK_ENONE) {
            tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_createSco: send +BVRA:1 failure");
            return ret;
        }
        //If AT commands are being exchanged during HFP connection, headset might send AT+BCS after connection complete.
        if (sTlkMdiScoCodecBcsResult == FALSE) {
            tempLen = strlen("\r\n+BCS:2\r\n");
            tmemcpy(buffer, "\r\n+BCS:2\r\n", tempLen);
            ret = btp_hfpag_send(pAclHandle->aclHandle, buffer, tempLen);
            if (ret != TLK_ENONE) {
                tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_createSco: send +BCS:2 failure");
                return ret;
            }
            sTlkMdiScoCodecBcsResult = TRUE;
        }

        tmemcpy(sTlkMdiBtAgCtrl.pBtAddr, pBtAddr, sizeof(sTlkMdiBtAgCtrl.pBtAddr));
        sTlkMdiBtAgCtrl.state = TLKMDI_BTAG_STATE_CONNECTING;
        tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &sTlkMdiBtAgCtrl.timer, TLKMDI_BTAG_TIMEOUT, false, tlkmdi_bthfpag_timer, NULL);
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtAgCtrl.timer);
    }

    return TLK_ENONE;
}

/**
 * @brief       Disconnect the SCO connection for HFP AG
 * @param[in]   pBtAddr     - The bluetooth device address.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfag_disconnSco(uint8_t *pBtAddr)
{
    int               ret;
    uint8_t           buffer[20];
    uint8_t           tempLen;
    bth_acl_handle_t *pAclHandle;
    bth_sco_handle_t *pScoHandle;

    pAclHandle = bth_handle_searchConnAcl(pBtAddr);
    if (pAclHandle == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfag_disconnSco: rejected - no connected ACL device");
        return TLK_ESTATUS;
    }

    pScoHandle = bth_handle_searchConnSco(pAclHandle->aclHandle);
    if (pScoHandle == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfag_disconnSco: failure - not exist");
        return TLK_ESTATUS;
    }

    sTlkMdiScoCodecBcsResult = FALSE;
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(buffer)));
    memset(buffer, 0, sizeof(buffer));
    tempLen = strlen("\r\n+BVRA:0\r\n");
    tmemcpy(buffer, "\r\n+BVRA:0\r\n", tempLen);
    ret = btp_hfpag_send(pAclHandle->aclHandle, buffer, tempLen);
    if (ret != TLK_ENONE) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfag_disconnSco: send +BVRA:0 failure");
    }

    sTlkMdiBtAgCtrl.state = TLKMDI_BTAG_STATE_DISCONNECT;
    bth_sco_disconn(pScoHandle->scoHandle, 0x13);
    sTlkMdiHfpAgCtrl.disc_active = true;
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &sTlkMdiBtAgCtrl.timer, TLKMDI_BTAG_TIMEOUT, false, tlkmdi_bthfpag_timer, NULL);
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtAgCtrl.timer);

    return TLK_ENONE;
}

/**
 * @brief       Get the disc active flag status
 * @param[in]   none.
 * @return      true if disconnect active flag is set, false otherwise.
 *              This flag indicates if a hang up call is being initiated.
 */
bool tlkmdi_bthfpag_get_discActive()
{
    return sTlkMdiHfpAgCtrl.disc_active;
}

/**
 * @brief       Clear the disc active flag to indicate no hang up call is being initiated
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_bthfpag_clear_discActive()
{
    sTlkMdiHfpAgCtrl.disc_active = false;
}

/**
 * @brief       Reset the HFP AG module
 * @param[in]   none.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_reset(void)
{
    sTlkMdiScoCodecBcsResult = FALSE;
    tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtAgCtrl.timer);
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(sTlkMdiBtAgCtrl)));
    memset(&sTlkMdiBtAgCtrl, 0, sizeof(sTlkMdiBtAgCtrl));

    return TLK_ENONE;
}

/**
 * @brief       Send mute command to the peer device
 * @param[in]   pBtAddr     - The bluetooth device address.
 * @param[in]   micSpk      - Device type, 1 is mic, 2 is speaker.
 * @param[in]   enable      - Enable or disable mute, 1 is enable, 0 is disable.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_sendMute(uint8_t *pBtAddr, uint8_t micSpk, uint8_t enable)
{
    int               ret;
    bth_acl_handle_t *pAclHandle;
    bth_sco_handle_t *pScoHandle;

    pAclHandle = bth_handle_searchConnAcl(pBtAddr);
    if (pAclHandle == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_muteMic: rejected - no connected ACL device");
        return TLK_ESTATUS;
    }

    pScoHandle = bth_handle_searchConnSco(pAclHandle->aclHandle);
    if (pScoHandle == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_muteMic: failure - not exist");
        return TLK_ESTATUS;
    }
    ret = bth_sco_sendMute(pScoHandle->scoHandle, micSpk, enable);
    if (ret != TLK_ENONE) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_muteMic: send mute failure");
        return ret;
    }

    return TLK_ENONE;
}

/**
 * @brief       Set the microphone gain by bluetooth address
 * @param[in]   pBtAddr     - The bluetooth device address.
 * @param[in]   volume      - The volume level(0-15).
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_setVgm(uint8_t *pBtAddr, uint8_t volume)
{
    int               ret;
    uint8_t           tempLen;
    uint8_t           buffLen;
    char              buffer[20];
    bth_acl_handle_t *pHandle;

    if (volume > 15) {
        return -TLK_EPARAM;
    }
    buffLen           = 0;
    buffer[buffLen++] = '\r';
    buffer[buffLen++] = '\n';
    tempLen           = strlen("+VGM=");
    tmemcpy(buffer + buffLen, "+VGM=", tempLen);
    buffLen += tempLen;
    tempLen = (uint8_t)tlkapi_decToStr((char *)(buffer + buffLen), volume, 2, true);
    buffLen += tempLen;
    buffer[buffLen++] = '\r';
    buffer[buffLen++] = '\n';

    pHandle = bth_handle_searchConnAcl(pBtAddr);
    if (pHandle == NULL) {
        tlkapi_trace(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_setVgm: rejected - no connected ACL device ");
        return TLK_ESTATUS;
    }
    ret = btp_hfpag_send(pHandle->aclHandle, (uint8_t *)buffer, buffLen);
    if (ret != TLK_ENONE) {
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtAgCtrl.timer);
    } else {
        btp_send_hfpagVolumeChangedEvt(pHandle->aclHandle, BTP_HFP_VOLUME_TYPE_MIC, volume);
    }

    return TLK_ENONE;
}

/**
 * @brief       Set the microphone gain by ACL handle
 * @param[in]   aclHandle   - The ACL connection handle.
 * @param[in]   volume      - The volume level(0-15).
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_setVgmbyHandle(uint16_t aclHandle, uint8_t volume)
{
    int     ret;
    uint8_t tempLen;
    uint8_t buffLen;
    char    buffer[20];

    if (bth_handle_getConnAcl(aclHandle) == NULL || volume > 15) {
        return -TLK_EPARAM;
    }

    buffLen           = 0;
    buffer[buffLen++] = '\r';
    buffer[buffLen++] = '\n';
    tempLen           = strlen("+VGM=");
    tmemcpy(buffer + buffLen, "+VGM=", tempLen);
    buffLen += tempLen;
    tempLen = (uint8_t)tlkapi_decToStr((char *)(buffer + buffLen), volume, 2, true);
    buffLen += tempLen;
    buffer[buffLen++] = '\r';
    buffer[buffLen++] = '\n';

    ret = btp_hfpag_send(aclHandle, (uint8_t *)buffer, buffLen);
    if (ret != TLK_ENONE) {
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtAgCtrl.timer);
    } else {
        btp_send_hfpagVolumeChangedEvt(aclHandle, BTP_HFP_VOLUME_TYPE_MIC, volume);
    }

    return TLK_ENONE;
}

/**
 * @brief       Set the speaker gain by bluetooth address
 * @param[in]   pBtAddr     - The bluetooth device address.
 * @param[in]   volume      - The volume level(0-15).
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_setVgs(uint8_t *pBtAddr, uint8_t volume)
{
    int               ret;
    uint8_t           tempLen;
    uint8_t           buffLen;
    char              buffer[32];
    bth_acl_handle_t *pHandle;

    if (volume > 15) {
        return -TLK_EPARAM;
    }
    buffLen           = 0;
    buffer[buffLen++] = '\r';
    buffer[buffLen++] = '\n';
    tempLen           = strlen("+VGS=");
    tmemcpy(buffer + buffLen, "+VGS=", tempLen);
    buffLen += tempLen;
    tempLen = (uint8_t)tlkapi_decToStr((char *)(buffer + buffLen), volume, 2, true);
    buffLen += tempLen;
    buffer[buffLen++] = '\r';
    buffer[buffLen++] = '\n';

    pHandle = bth_handle_searchConnAcl(pBtAddr);
    if (pHandle == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_setVgs: rejected - no connected ACL device ");
        return TLK_ESTATUS;
    }
    ret = btp_hfpag_send(pHandle->aclHandle, (uint8_t *)buffer, buffLen);
    if (ret != TLK_ENONE) {
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtAgCtrl.timer);
    } else {
        btp_send_hfpagVolumeChangedEvt(pHandle->aclHandle, BTP_HFP_VOLUME_TYPE_SPK, volume);
    }

    return TLK_ENONE;
}

#if TLKMDI_AG_CALL_SETUP
/**
 * @brief       Insert a call
 * @param[in]   pNumber     - The phone number.
 * @param[in]   numbLen     - The length of phone number.
 * @param[in]   isIncoming  - Call direction, 1 is incoming, 0 is outgoing.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_insertCall(uint8_t *pNumber, uint8_t numbLen, uint8_t isIncoming)
{
    uint8_t              count;
    uint16_t             aclHandle;
    tlkmdi_hfpag_unit_t *pCall;

    if (pNumber == NULL || numbLen == 0 || numbLen > 70) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_insertCall: failure - error prarm");
        return -TLK_EPARAM;
    }

    count = tlkmdi_bthfpag_getCallUnitCount();
    if ((((btp_hfpag_getFeature() & 0x01) == 0) && count >= 1) || (((btp_hfpag_getFeature() & 0x01) != 0) && count >= 2)) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_insertCall: failure - no quota");
        return -TLK_EQUOTA;
    }
    if (count == 1) {
        if (sTlkMdiHfpAgCtrl.unit[0].status == 0) {
            pCall = &sTlkMdiHfpAgCtrl.unit[1];
        } else {
            pCall = &sTlkMdiHfpAgCtrl.unit[0];
        }
        if (sTlkMdiHfpAgCtrl.callState != TLKMDI_HFP_CALL_STATE_ACTIVE) {
            tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_insertCall: failure - exist not deal call");
            return -TLK_EBUSY;
        }
        if (isIncoming == false) {
            isIncoming = true;
        } else {
            isIncoming = false;
        }
    }
    if (count == 1 && !isIncoming) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_insertCall: failure - No dialing is allowed during the call");
        return -TLK_EFAIL;
    }

    pCall = tlkmdi_bthfpag_getUsedCallUnit(numbLen, pNumber);
    if (pCall != NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_insertCall: failure - repeat insert");
        return -TLK_EEXIST;
    }
    pCall = tlkmdi_bthfpag_getIdleCallUnit();
    if (pCall == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_insertCall: failure - system fault");
        return -TLK_EFAULT;
    }

    tlkapi_trace(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_insertCall: success");

    if (count != 0) {
        pCall->status = TLKMDI_HFP_CLCC_STATUS_WAITING;
    } else {
        if (isIncoming) {
            pCall->status = TLKMDI_HFP_CLCC_STATUS_INCOMING;
        } else {
            pCall->status = TLKMDI_HFP_CLCC_STATUS_DIALING;
        }
    }
    pCall->numbLen = numbLen;
    if (isIncoming) {
        pCall->callDir = TLKMDI_HFP_CLCC_DIR_INCOMING;
    } else {
        pCall->callDir = TLKMDI_HFP_CLCC_DIR_OUTGOING;
    }
    if (count == 0) {
        pCall->callIsHold = false;
    } else {
        pCall->callIsHold = true;
    }
    if (isIncoming) {
        pCall->callSetup = TLKMDI_HFP_CALL_SETUP_INCOMING;
    } else {
        pCall->callSetup = TLKMDI_HFP_CALL_SETUP_OUTGOING;
    }
    if (numbLen != 0) {
        tmemcpy(pCall->number, pNumber, numbLen);
    }
    aclHandle = btp_hfp_getAgHandle();
    if (pCall->status == TLKMDI_HFP_CLCC_STATUS_INCOMING) {
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLSETUP, TLKMDI_BTAG_CALLSETUP_INCOMING);
        sTlkMdiHfpAgCtrl.busys |= TLKMDI_BTAG_BUSY_RING;
        tlkmdi_bthfpag_sendStatusChangedEvt(pCall, TLKMDI_HFP_CLCC_STATUS_NONE);
    } else if (pCall->status == TLKMDI_HFP_CLCC_STATUS_DIALING) {
        pCall->status    = TLKMDI_HFP_CLCC_STATUS_ALERTING;
        pCall->callSetup = TLKMDI_HFP_CALL_SETUP_ALERTING;
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLSETUP, TLKMDI_BTAG_CALLSETUP_OUTGOING);
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLSETUP, TLKMDI_BTAG_CALLSETUP_REMOTE_ALERTED);
        sTlkMdiHfpAgCtrl.busys |= TLKMDI_BTAG_BUSY_RING;
        tlkmdi_bthfpag_sendStatusChangedEvt(pCall, TLKMDI_HFP_CLCC_STATUS_NONE);
    }

    sTlkMdiHfpAgCtrl.callState = TLKMDI_HFP_CALL_STATE_ACTIVE;
    if (count != 0) {
        pCall->status = TLKMDI_HFP_CLCC_STATUS_WAITING;
        if (pCall->status == TLKMDI_HFP_CLCC_STATUS_ACTIVE) {
            sTlkMdiHfpAgCtrl.callHold = TLKMDI_HFP_CALL_HOLD_HELD_AND_ACTIVE;
            tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLHELD, TLKMDI_BTAG_CALLHELD_ACTIVE_HELD_SWAPPED);
        } else {
            sTlkMdiHfpAgCtrl.callHold = TLKMDI_HFP_CALL_HOLD_HELD_AND_NO_ACTIVE;
            tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLHELD, TLKMDI_BTAG_CALLHELD_HOLD_NO_ACTIVE);
        }
        tlkmdi_bthfpag_sendStatusChangedEvt(pCall, TLKMDI_HFP_CLCC_STATUS_NONE);
    }

    return TLK_ENONE;
}

/**
 * @brief       Remove a call
 * @param[in]   pNumber     - The phone number.
 * @param[in]   numbLen     - The length of phone number.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_removeCall(uint8_t *pNumber, uint8_t numbLen)
{
    uint8_t              count;
    uint16_t             aclHandle;
    tlkmdi_hfpag_unit_t *pCall;
    tlkmdi_hfpag_unit_t *pTemp;

    pCall = tlkmdi_bthfpag_getUsedCallUnit(numbLen, pNumber);
    if (pCall == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_removeCall: failure - not exist");
        return -TLK_EPARAM;
    }
    sTlkMdiHfpAgCtrl.busys &= ~TLKMDI_BTAG_BUSY_RING;
    aclHandle = btp_hfp_getAgHandle();
    count     = tlkmdi_bthfpag_getCallUnitCount();
    if (pCall->status == TLKMDI_HFP_CLCC_STATUS_DIALING || pCall->status == TLKMDI_HFP_CLCC_STATUS_ALERTING || pCall->status == TLKMDI_HFP_CLCC_STATUS_INCOMING) {
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLSETUP, TLKMDI_BTAG_CALLSETUP_NOT_SETUP);
    } else if (count == 1 || pCall->status == TLKMDI_HFP_CLCC_STATUS_ACTIVE) {
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALL, TLKMDI_BTAG_CALL_NOT_PROGRESS);
    }
    tlkmdi_hfpag_resetCall(pCall);

    if (tlkmdi_bthfpag_getCallUnitCount() == 0) {
        sTlkMdiHfpAgCtrl.callHold  = 0;
        sTlkMdiHfpAgCtrl.callState = 0;
        return TLK_ENONE;
    }

    if (pCall == &sTlkMdiHfpAgCtrl.unit[0]) {
        pTemp = &sTlkMdiHfpAgCtrl.unit[1];
    } else {
        pTemp = &sTlkMdiHfpAgCtrl.unit[0];
    }
    aclHandle = btp_hfp_getAgHandle();
    if (pTemp->status == TLKMDI_HFP_CLCC_STATUS_ACTIVE) {
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLHELD, TLKMDI_BTAG_CALLHELD_NOT_HELD);
    } else {
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLHELD, TLKMDI_BTAG_CALLHELD_HOLD_NO_ACTIVE);
    }

    return TLK_ENONE;
}

/**
 * @brief       Active a call
 * @param[in]   pNumber     - The phone number.
 * @param[in]   numbLen     - The length of phone number.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_activeCall(uint8_t *pNumber, uint8_t numbLen)
{
    uint8_t              count;
    uint16_t             aclHandle;
    tlkmdi_hfpag_unit_t *pCall;
    tlkmdi_hfpag_unit_t *pTemp;

    pCall = tlkmdi_bthfpag_getUsedCallUnit(numbLen, pNumber);
    if (pCall == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_activeCall: failure - not exist");
        return -TLK_EPARAM;
    }
    if (pCall->status == TLKMDI_HFP_CLCC_STATUS_ACTIVE) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_activeCall: failure - repeat operate");
        return -TLK_EREPEAT;
    }
    sTlkMdiHfpAgCtrl.callState = TLKMDI_HFP_CALL_STATE_ACTIVE;

    count     = tlkmdi_bthfpag_getCallUnitCount();
    aclHandle = btp_hfp_getAgHandle();
    if (pCall == &sTlkMdiHfpAgCtrl.unit[0]) {
        pTemp = &sTlkMdiHfpAgCtrl.unit[1];
    } else {
        pTemp = &sTlkMdiHfpAgCtrl.unit[0];
    }
    if (pTemp->status == 0) {
        pTemp = NULL;
    }
    if (pTemp != NULL && pTemp->status == TLKMDI_HFP_CLCC_STATUS_ACTIVE) {
        if (pTemp->callDir == TLKMDI_HFP_CLCC_DIR_OUTGOING) {
            pTemp->status = TLKMDI_HFP_CLCC_STATUS_CALLHELD;
        } else {
            pTemp->status = TLKMDI_HFP_CLCC_STATUS_WAITING;
        }
        tlkmdi_bthfpag_sendStatusChangedEvt(pCall, TLKMDI_HFP_CLCC_STATUS_NONE);
    }

    if (pCall->callSetup != TLKMDI_HFP_CALL_SETUP_IDLE) {
        pCall->callSetup = TLKMDI_HFP_CALL_SETUP_IDLE;
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALL, TLKMDI_BTAG_CALL_IN_PROGRESS);
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLSETUP, TLKMDI_BTAG_CALLSETUP_NOT_SETUP);
    }

    pCall->status = TLKMDI_HFP_CLCC_STATUS_ACTIVE;

    if (count == 2) {
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLHELD, TLKMDI_BTAG_CALLHELD_ACTIVE_HELD_SWAPPED);
    }

    pCall->status = TLKMDI_HFP_CLCC_STATUS_ACTIVE;

    return TLK_ENONE;
}

/**
 * @brief       Hang up a call
 * @param[in]   none.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_hungupCall(void)
{
    uint8_t  count;
    uint16_t aclHandle;

    count = tlkmdi_bthfpag_getCallUnitCount();
    tlkapi_trace(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_hungupCall: count - %d", count);
    if (count == 0) {
        return TLK_ENONE;
    }

    aclHandle = btp_hfp_getAgHandle();
    tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALL, TLKMDI_BTAG_CALL_NOT_PROGRESS);

    if (sTlkMdiHfpAgCtrl.unit[0].status != 0) {
        tlkmdi_hfpag_resetCall(&sTlkMdiHfpAgCtrl.unit[0]);
    }
    if (sTlkMdiHfpAgCtrl.unit[1].status != 0) {
        tlkmdi_hfpag_resetCall(&sTlkMdiHfpAgCtrl.unit[1]);
    }
    tlkmdi_bthfag_disconnSco(sTlkMdiBtAgCtrl.pBtAddr);

    return TLK_ENONE;
}
#if 1
/**
 * @brief       Reject waiting call and keep active call
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_rejectWaitAndKeepActive(uint16_t aclHandle)
{
    (void)aclHandle;
    uint8_t              count;
    tlkmdi_hfpag_unit_t *pWaitCall;

    count = tlkmdi_bthfpag_getCallUnitCount();
    if (count < 2) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_rejectWaitAndKeepActive: failure - no quota");
        return -TLK_EFAIL;
    }

    pWaitCall = tlkmdi_bthfpag_getWaitCallUnit();
    if (pWaitCall == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_rejectWaitAndKeepActive: no calls on wait");
        return -TLK_EFAULT;
    }
    if (pWaitCall->callIsHold == true) {
        tlkmdi_bthfpag_removeCall(pWaitCall->number, pWaitCall->numbLen);
        tlkapi_trace(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_rejectWaitAndKeepActive: success");
    } else {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_rejectWaitAndKeepActive: no calls on hold");
        return -TLK_EFAULT;
    }

    return TLK_ENONE;
}

/**
 * @brief       Accept waiting call and hold active call
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_acceptWaitAndHoldActive(uint16_t aclHandle)
{
    uint8_t              count;
    tlkmdi_hfpag_unit_t *pWaitCall;
    tlkmdi_hfpag_unit_t *pActiveCall;

    count = tlkmdi_bthfpag_getCallUnitCount();
    if (count < 2) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_acceptWaitAndHoldActive: no available resource");
        return -TLK_EFAIL;
    }

    pWaitCall = tlkmdi_bthfpag_getWaitCallUnit();
    if (pWaitCall == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_acceptWaitAndHoldActive: no calls on wait");
        return -TLK_EFAULT;
    }
    if (pWaitCall->callIsHold == true) {
        pActiveCall = tlkmdi_bthfpag_getActiveCallUnit();
        if (pActiveCall == NULL) {
            tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_acceptWaitAndHoldActive: no calls on active");
        }
        tlkmdi_bthfpag_sendCievCmd(aclHandle, TLKMDI_BTAG_INDIC_CALLHELD, TLKMDI_BTAG_CALLHELD_ACTIVE_HELD_SWAPPED);
        /*
         * pBtAddr - the pActiveCall BtAddr
         * micSpk  - SPK
         * enable  - 1
         tlkmdi_bthfpag_sendMute(uint8_t *pBtAddr,  uint8_t micSpk, uint8_t enable);

         * pBtAddr - the pWaitCall BtAddr
         tlkmdi_bthfpag_createSco(uint8_t *pBtAddr);
        */

    } else {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_acceptWaitAndHoldActive: no calls on hold");
        return -TLK_EFAULT;
    }
    tlkapi_trace(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_acceptWaitAndHoldActive: success");

    return TLK_ENONE;
}

/**
 * @brief       Hang up active call and resume held call
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfpag_hungUpActiveAndResumeHold(uint16_t aclHandle)
{
    (void)aclHandle;
    uint8_t              count;
    tlkmdi_hfpag_unit_t *pWaitCall;
    tlkmdi_hfpag_unit_t *pActiveCall;

    count = tlkmdi_bthfpag_getCallUnitCount();
    if (count < 2) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_hungUpActiveAndResumeHold: failure - no quota");
        return -TLK_EFAIL;
    }

    pWaitCall = tlkmdi_bthfpag_getWaitCallUnit();
    if (pWaitCall == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_hungUpActiveAndResumeHold: no calls on wait");
        return -TLK_EFAULT;
    }
    if (pWaitCall->callIsHold == true) {
        pActiveCall = tlkmdi_bthfpag_getActiveCallUnit();
        if (pActiveCall == NULL) {
            tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_hungUpActiveAndResumeHold: no calls on active");
        }
        tlkmdi_bthfag_disconnSco(sTlkMdiBtAgCtrl.pBtAddr);
        tlkmdi_bthfpag_removeCall(pActiveCall->number, pActiveCall->numbLen);
    } else {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_hungUpActiveAndResumeHold: no calls on hold");
        return -TLK_EFAULT;
    }

    pWaitCall->callIsHold = false;
    pWaitCall->callSetup  = TLKMDI_HFP_CALL_SETUP_INCOMING;
    pWaitCall->callDir    = TLKMDI_HFP_CLCC_DIR_INCOMING;
    pWaitCall->status     = TLKMDI_HFP_CLCC_STATUS_ACTIVE;

    if (tlkmdi_bthfpag_createSco(sTlkMdiBtAgCtrl.pBtAddr) == TLK_ENONE) {
        tlkmdi_bthfpag_activeCall(pWaitCall->number, pWaitCall->numbLen);
        tlkapi_trace(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_hungUpActiveAndResumeHold: success");
    } else {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_hungUpActiveAndResumeHold: failure");
    }

    return TLK_ENONE;
}
#endif

/**
 * @brief       Timer callback function for HFP AG
 * @param[in]   pTimer      - The timer handle.
 * @param[in]   userArg     - User defined argument.
 * @return      none.
 */
static void tlkmdi_bthfpag_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    uint8_t           scoCodec = 0;
    int               ret;
    bth_acl_handle_t *pHandle;

    pHandle = bth_handle_searchConnAcl(sTlkMdiBtAgCtrl.pBtAddr);
    if (pHandle == NULL) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_timer: rejected - no connected ACL device");
        return;
    }
    if (sTlkMdiAgTimeout != 0) {
        sTlkMdiAgTimeout--;
    }
    if (sTlkMdiAgTimeout == 0 && sTlkMdiBtAgCtrl.state == TLKMDI_BTAG_STATE_CONNECTING) {
        tlkapi_trace(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_timer:[timeout] - CONNECTING");
        sTlkMdiAgTimeout         = TLKMDI_BTAG_TIMEOUT_MS;
        sTlkMdiScoCodecBcsResult = false;
        return;
    }
    if (sTlkMdiBtAgCtrl.state == TLKMDI_BTAG_STATE_IDLE) {
        return;
    }
    if (sTlkMdiBtAgCtrl.state == TLKMDI_BTAG_STATE_CONNECTING) {
        if (sTlkMdiScoCodecBcsResult == TRUE) {
            tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
            return;
        }
        if (!btp_hfp_getCodec(pHandle->aclHandle, &scoCodec)) {
            tlkapi_trace(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_timer: %d", sTlkMdiAgTimeout);
            if (scoCodec == 0x02) { //MSBC
                ret = bth_sco_connect(pHandle->aclHandle, BTH_SCO_LINK_TYPE_ESCO, BTH_SCO_AIRMODE_TRANS);
                if (ret != TLK_ENONE) {
                    tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_timer: SCO-MSBC connect failure");
                    return;
                }
            } else {
                ret = bth_sco_connect(pHandle->aclHandle, BTH_SCO_LINK_TYPE_ESCO, BTH_SCO_AIRMODE_CVSD);
                if (ret != TLK_ENONE) {
                    tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_timer: SCO-CVSD connect failure");
                    return;
                }
            }
            sTlkMdiBtAgCtrl.state = TLKMDI_BTAG_STATE_CONNECTED;
        }
        sTlkMdiAgTimeout = TLKMDI_BTAG_TIMEOUT_MS;
    }
    if (sTlkMdiBtAgCtrl.state == TLKMDI_BTAG_STATE_DISCONNECT) {
        sTlkMdiBtAgCtrl.state = TLKMDI_BTAG_STATE_IDLE;
    }
}

/**
 * @brief       Receive command callback function
 * @param[in]   aclHandle   - The ACL connection handle.
 * @param[in]   pCmd        - Pointer to the command data.
 * @param[in]   cmdLen      - Length of the command data.
 * @return      TLK_ENONE is success, others is failure.
 */
static int tlkmdi_bthfpag_recvCmdCB(uint16_t aclHandle, uint8_t *pCmd, uint8_t cmdLen)
{
    if (aclHandle == 0 || pCmd == NULL || cmdLen == 0) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_recvCmdCB: error param");
        return -TLK_EPARAM;
    }

    tlkapi_array(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_recvCmdCB: ", pCmd, cmdLen);
    if (cmdLen >= 5 && tmemcmp(pCmd, "AT+BCS", 6) == 0) {
        if (sTlkMdiScoCodecBcsResult == TRUE) {
            sTlkMdiScoCodecBcsResult = FALSE;
        }
        tlkmdi_bthfpag_sendOkCmd(aclHandle);
        return TLK_ENONE;
    } else if (cmdLen >= 8 && tmemcmp(pCmd, "AT+BRSF=", 8) == 0) {
        tlkapi_strToUint32((char *)(pCmd + 8), cmdLen - 8, &(sTlkMdiBtAgCtrl.feature));
        return -TLK_ESTATUS; // Just get the HF feature, then hand off to the lower layers to process the command
    }
#if (TLKMDI_AG_CALL_SETUP)
    else if (cmdLen >= 7 && tmemcmp(pCmd, "AT+BLDN", 7) == 0) {
        tlkmdi_bthfpag_sendErrorCmd(aclHandle);
        return TLK_ENONE;
    } else if (cmdLen >= 7 && tmemcmp(pCmd, "AT+CLCC", 7) == 0) {
        tlkmdi_btbthfpag_recvClccCmdDeal(aclHandle);
        return TLK_ENONE;
    } else if (cmdLen >= 8 && tmemcmp(pCmd, "AT+CMER=", 8) == 0) {
        if (cmdLen < 15) {
            tlkmdi_bthfpag_sendErrorCmd(aclHandle);
        } else {
            if (pCmd[14] == '0') {
                sTlkMdiHfpAgAttr &= ~TLKMDI_BTAG_ATTR_CIEV;
            } else {
                sTlkMdiHfpAgAttr |= TLKMDI_BTAG_ATTR_CIEV;
            }
            tlkmdi_bthfpag_sendOkCmd(aclHandle);
        }
        return TLK_ENONE;
    } else if (cmdLen >= 8 && tmemcmp(pCmd, "AT+CLIP=", 8) == 0) {
        if (cmdLen < 9) {
            tlkmdi_bthfpag_sendErrorCmd(aclHandle);
        } else {
            if (pCmd[8] == '0') {
                sTlkMdiHfpAgAttr &= ~TLKMDI_BTAG_ATTR_CLIP;
            } else {
                sTlkMdiHfpAgAttr |= TLKMDI_BTAG_ATTR_CLIP;
            }
            tlkmdi_bthfpag_sendOkCmd(aclHandle);
        }
        return TLK_ENONE;
    } else if (cmdLen >= 3 && tmemcmp(pCmd, "ATD", 3) == 0) {
        if (cmdLen < 5) {
            tlkmdi_bthfpag_sendErrorCmd(aclHandle);
        } else {
            uint8_t offset = 3;
            if (pCmd[3] == '>') {
                offset += 1;
            }
            tlkmdi_bthfpag_recvAtdCmdDeal(aclHandle, pCmd + offset, cmdLen - 1 - offset);
        }
        return TLK_ENONE;
    } else if (cmdLen >= 3 && tmemcmp(pCmd, "ATA", 3) == 0) {
        tlkmdi_bthfpag_recvAtaCmdDeal(aclHandle);
        tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_CALL_ACCEPT, NULL, 0); //contact ziyu ,have bug!
        return TLK_ENONE;
    } else if (cmdLen >= 7 && tmemcmp(pCmd, "AT+CHUP", 7) == 0) {
        tlkmdi_bthfpag_recvChupCmdDeal(aclHandle);
        tlkapi_trace(0xffffffff, "AG", "receiver AT+CHUP");
        tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_CALL_HUNGUP, NULL, 0); //contact ziyu ,have bug!

        return TLK_ENONE;
    } else if (cmdLen >= 8 && tmemcmp(pCmd, "AT+CIND?", 8) == 0) {
        if (tlkmdi_bthfpag_recvCindCmdDeal(aclHandle)) {
            tlkmdi_bthfpag_sendOkCmd(aclHandle);
        } else {
            tlkmdi_bthfpag_sendErrorCmd(aclHandle);
        }
        return TLK_ENONE;
    } else if (cmdLen >= 9 && tmemcmp(pCmd, "AT+CIND=?", 9) == 0) {
        char *pInstruct  = "\r\n+CIND: (\"service\",(0-1)),(\"call\",(0-1)),(\"callsetup\",(0-3)),(\"battchg\",(0-5)),(\"signal\",(0-5)),(\"roam\",(0-1)),(\"callheld\",(0-2))\r\n";
        uint8_t instLens = strlen(pInstruct);
        btp_hfpag_send(aclHandle, (uint8_t *)pInstruct, instLens);
        tlkmdi_bthfpag_sendOkCmd(aclHandle);
        return TLK_ENONE;
    } else if (cmdLen >= 9 && tmemcmp(pCmd, "AT+CHLD=?", 9) == 0) {
        char   *pTempStr = "\r\n+CHLD:(0,1,1x,2,2x,3)\r\n";
        uint8_t tempLen  = strlen(pTempStr);
        btp_hfpag_send(aclHandle, (uint8_t *)pTempStr, tempLen);
        tlkmdi_bthfpag_sendOkCmd(aclHandle);
        tlkmdi_bthfpag_setVgmbyHandle(aclHandle, 15);
        btp_hfpag_connectCompleteEvt(aclHandle);
        return TLK_ENONE;
    } else if (cmdLen >= 9 && tmemcmp(pCmd, "AT+CHLD=", 8) == 0) {
        if (tlkmdi_bthfpag_recvChldCmdDeal(aclHandle, pCmd, cmdLen)) {
            tlkmdi_bthfpag_sendOkCmd(aclHandle);
        } else {
            tlkmdi_bthfpag_sendErrorCmd(aclHandle);
        }
        return TLK_ENONE;
    } else {
        // AT commands not related to call are handled at a lower level
        return -TLK_EPARAM;
    }
#endif // #if (TLKMDI_AG_CALL_SETUP)
}

/**
 * @brief       Send OK response command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      true is success, false is failure.
 */
static bool tlkmdi_bthfpag_sendOkCmd(uint16_t aclHandle)
{
    char buffer[20];

    if (aclHandle == 0) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_sendOkCmd: failure - error prarm");
        return false;
    }
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(buffer)));
    memset(buffer, 0, sizeof(buffer));
    tmemcpy(buffer, "\r\nOK\r\n", strlen("\r\nOK\r\n"));
    btp_hfpag_send(aclHandle, (uint8_t *)buffer, strlen("\r\nOK\r\n"));

    return true;
}

/**
 * @brief       Send ERROR response command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      true is success, false is failure.
 */
static bool tlkmdi_bthfpag_sendErrorCmd(uint16_t aclHandle)
{
    char buffer[20];

    if (aclHandle == 0) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_sendErrorCmd: failure - error prarm");
        return false;
    }
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(buffer)));
    memset(buffer, 0, sizeof(buffer));
    tmemcpy(buffer, "\r\nERROR\r\n", strlen("\r\nERROR\r\n"));
    btp_hfpag_send(aclHandle, (uint8_t *)buffer, strlen("\r\nERROR\r\n"));

    return true;
}

/**
 * @brief       Send CIEV command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @param[in]   indicators  - Indicator index.
 * @param[in]   indValue    - Indicator value.
 * @return      true is success, false is failure.
 */
static bool tlkmdi_bthfpag_sendCievCmd(uint16_t aclHandle, uint8_t indicators, uint8_t indValue)
{
    uint8_t tempLen;
    uint8_t buffLen;
    char    buffer[20];

    tlkapi_trace(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_sendCievCmd: indicators %d indValue %d", indicators, indValue);
    if (aclHandle == 0 || indicators == 0 || indicators > 7) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_sendCievCmd: error param");
        return false;
    }
    if ((sTlkMdiHfpAgAttr & TLKMDI_BTAG_ATTR_CIEV) == 0) {
        tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "tlkmdi_bthfpag_sendCievCmd: error attribute %d", sTlkMdiHfpAgAttr);
        return false;
    }

    buffLen = 0;
    tempLen = strlen("\r\n+CIEV: ");
    tmemcpy(buffer, "\r\n+CIEV: ", tempLen);
    buffLen += tempLen;
    tempLen = (uint8_t)tlkapi_decToStr((char *)(buffer + buffLen), indicators, 1, true);
    buffLen += tempLen;
    buffer[buffLen++] = ',';
    tempLen           = (uint8_t)tlkapi_decToStr((char *)(buffer + buffLen), indValue, 1, true);
    buffLen += tempLen;
    buffer[buffLen++] = '\r';
    buffer[buffLen++] = '\n';
    btp_hfpag_send(aclHandle, (uint8_t *)buffer, buffLen);

    return true;
}

/**
 * @brief       Reset call information
 * @param[in]   pCall       - Pointer to the call unit.
 * @return      none.
 */
static void tlkmdi_hfpag_resetCall(tlkmdi_hfpag_unit_t *pCall)
{
    pCall->status = 0;
    tlkmdi_bthfpag_sendStatusChangedEvt(pCall, BTP_HFP_CALL_STATUS_CLOSE);
    pCall->numbLen   = 0;
    pCall->callDir   = 0;
    pCall->callSetup = 0;
}

// static void tlkmdi_bthfpag_sendRingProc(btp_hfp_item_t *pHfp)
//{
//	if(pHfp->timeout != 0) pHfp->timeout--;
//	if(pHfp->timeout == 0){
//		tlkmdi_hfpag_unit_t *pCall;
//		tlkapi_error(TLKMDI_BTAG_DBG_FLAG, TLKMDI_BTAG_DBG_SIGN, "btp_hfpag_procs: send ring");
//		pHfp->timeout = BTP_HFPAG_RING_TIMEOUT;
//		if(sTlkMdiHfpAgCtrl.unit[0].status == 0) pCall = &sTlkMdiHfpAgCtrl.unit[1];
//		else pCall = &sTlkMdiHfpAgCtrl.unit[0];
//		if((pHfp->attrs & BTP_HFPAG_ATTR_CIEV) != 0){
//			btp_hfpag_pushData(pHfp, "\r\nRING\r\n", strlen("\r\nRING\r\n"));
//		}
//		if((pHfp->attrs & BTP_HFPAG_ATTR_CLIP) != 0){
//			btp_hfpag_pushClipCmdDeal(pHfp, pCall);
//		}
//	}
// }

/**
 * @brief       Handle CLCC command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      true is success, false is failure.
 */
static bool tlkmdi_btbthfpag_recvClccCmdDeal(uint16_t aclHandle)
{
    tlkmdi_bthfpag_sendOkCmd(aclHandle);
    if (sTlkMdiHfpAgCtrl.unit[0].status != TLK_STATE_CLOSED) {
        tlkmdi_bthfpag_pushClccCmdDeal(aclHandle, &sTlkMdiHfpAgCtrl.unit[0], 0);
    }
    if (sTlkMdiHfpAgCtrl.unit[1].status != TLK_STATE_CLOSED) {
        tlkmdi_bthfpag_pushClccCmdDeal(aclHandle, &sTlkMdiHfpAgCtrl.unit[1], 1);
    }
    tlkmdi_bthfpag_sendOkCmd(aclHandle);

    return true;
}

/**
 * @brief       Handle ATD command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @param[in]   pNumber     - Phone number.
 * @param[in]   numbLen     - Length of phone number.
 * @return      true is success, false is failure.
 */
static bool tlkmdi_bthfpag_recvAtdCmdDeal(uint16_t aclHandle, uint8_t *pNumber, uint8_t numbLen)
{
    if (tlkmdi_bthfpag_getCallUnitCount() == TLKMDI_PHONE_SETUP_MAX_NUMB) {
        tlkmdi_bthfpag_sendErrorCmd(aclHandle);
        return false;
    }
    tlkmdi_bthfpag_sendOkCmd(aclHandle);
    tlkmdi_bthfpag_insertCall(pNumber, numbLen, false);
    return true;
}

/**
 * @brief       Handle ATA command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      true is success, false is failure.
 */
static bool tlkmdi_bthfpag_recvAtaCmdDeal(uint16_t aclHandle)
{
    tlkmdi_hfpag_unit_t *pCall;

    if (tlkmdi_bthfpag_getCallUnitCount() == 0) {
        tlkmdi_bthfpag_sendErrorCmd(aclHandle);
        return false;
    }

    if (sTlkMdiHfpAgCtrl.unit[0].status == 0) {
        pCall = &sTlkMdiHfpAgCtrl.unit[1];
    } else {
        pCall = &sTlkMdiHfpAgCtrl.unit[0];
    }
    if (pCall->status != TLKMDI_HFP_CLCC_STATUS_DIALING && pCall->status != TLKMDI_HFP_CLCC_STATUS_ALERTING && pCall->status != TLKMDI_HFP_CLCC_STATUS_INCOMING) {
        tlkmdi_bthfpag_sendErrorCmd(aclHandle);
        return false;
    }
    tlkmdi_bthfpag_sendOkCmd(aclHandle);
    tlkmdi_bthfpag_activeCall(pCall->number, pCall->numbLen);
    return true;
}

/**
 * @brief       Handle CIND command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      true is success, false is failure.
 */
static bool tlkmdi_bthfpag_recvCindCmdDeal(uint16_t aclHandle)
{
    uint8_t  tempLen;
    uint8_t  buffer[32];
    uint8_t *pCmd = buffer;

    if (tlkmdi_bthfpag_getCallUnitCount() == 0) {
        tempLen = strlen("\r\n+CIND:1,0,0,5,3,0,0\r\n");
        tmemcpy(pCmd, "\r\n+CIND:1,0,0,5,3,0,0\r\n", tempLen);
        btp_hfpag_send(aclHandle, pCmd, tempLen);
    } else {
        uint8_t buffLen = 0;
        char    sendBuff[32];
        STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(sendBuff)));
        memset(sendBuff, 0, sizeof(sendBuff));
        tempLen = strlen("\r\n+CIND:1,");
        tmemcpy(sendBuff + buffLen, "\r\n+CIND:1,", tempLen);
        buffLen += tempLen;
        sendBuff[buffLen++] = '1'; // Service
        sendBuff[buffLen++] = ',';
        if (sTlkMdiHfpAgCtrl.callState == 0) {
            sendBuff[buffLen++] = TLKMDI_HFP_CALL_STATE_HUNGUP;
        } else {
            sendBuff[buffLen++] = TLKMDI_HFP_CALL_STATE_ACTIVE;
        }
        sendBuff[buffLen++] = ',';
        if (tlkmdi_bthfpag_getCallUnitCount() != 1) {
            sendBuff[buffLen++] = TLKMDI_HFP_CALL_SETUP_IDLE;
        } else if (sTlkMdiHfpAgCtrl.callState != 0) {
            sendBuff[buffLen++] = TLKMDI_HFP_CALL_SETUP_IDLE;
        } else {
            tlkmdi_hfpag_unit_t *pCall;
            if (sTlkMdiHfpAgCtrl.unit[0].status == 0) {
                pCall = &sTlkMdiHfpAgCtrl.unit[1];
            } else {
                pCall = &sTlkMdiHfpAgCtrl.unit[0];
            }
            if (pCall->status == TLKMDI_HFP_CLCC_STATUS_INCOMING) {
                sendBuff[buffLen++] = TLKMDI_HFP_CALL_SETUP_INCOMING;
            } else if (pCall->status == TLKMDI_HFP_CLCC_STATUS_DIALING) {
                sendBuff[buffLen++] = TLKMDI_HFP_CALL_SETUP_OUTGOING;
            } else if (pCall->status == TLKMDI_HFP_CLCC_STATUS_ALERTING) {
                sendBuff[buffLen++] = TLKMDI_HFP_CALL_SETUP_ALERTING;
            } else {
                sendBuff[buffLen++] = TLKMDI_HFP_CALL_SETUP_IDLE;
            }
        }
        sendBuff[buffLen++] = ',';
        sendBuff[buffLen++] = '5'; // battchg
        sendBuff[buffLen++] = ',';
        sendBuff[buffLen++] = '3'; // signal
        sendBuff[buffLen++] = ',';
        sendBuff[buffLen++] = '0'; // roam
        sendBuff[buffLen++] = ',';
        if (sTlkMdiHfpAgCtrl.callHold == 1) {
            sendBuff[buffLen++] = '1'; // callheld=0 -  No calls held
        } else if (sTlkMdiHfpAgCtrl.callHold == 2) {
            sendBuff[buffLen++] = '2'; // callheld=1 - Call is placed on hold or active/held calls swapped
        } else {
            sendBuff[buffLen++] = '0'; // callheld=2 - Call on hold, no active call
        }
        sendBuff[buffLen++] = '\r';
        sendBuff[buffLen++] = '\n';
        btp_hfpag_send(aclHandle, (uint8_t *)sendBuff, buffLen);
    }
    return true;
}

/**
 * @brief       Handle CHUP command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @return      true is success, false is failure.
 */
static bool tlkmdi_bthfpag_recvChupCmdDeal(uint16_t aclHandle)
{
    if (tlkmdi_bthfpag_getCallUnitCount() == 0) {
        tlkmdi_bthfpag_sendErrorCmd(aclHandle);
        return false;
    }
    tlkmdi_bthfpag_sendOkCmd(aclHandle);
    tlkmdi_bthfpag_hungupCall();
    return true;
}

/**
 * @brief       Handle CHLD command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @param[in]   pData       - Command data.
 * @param[in]   dataLen     - Length of command data.
 * @return      true is success, false is failure.
 */
static bool tlkmdi_bthfpag_recvChldCmdDeal(uint16_t aclHandle, uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t              count;
    uint8_t              callHold;
    tlkmdi_hfpag_unit_t *pCall;

    callHold = pData[8];
    count    = tlkmdi_bthfpag_getCallUnitCount();
    if (count == 0) {
        return false;
    }

    if (callHold == TLKMDI_HFP_CALL_HOLD_NO_HELD) {
        if (count == 1) {
            return false;
        }
        pCall = tlkmdi_bthfpag_getWaitCallUnit();
        if (pCall == NULL) {
            return false;
        }
        tlkmdi_bthfpag_sendOkCmd(aclHandle);
        tlkmdi_bthfpag_removeCall(pCall->number, pCall->numbLen);
    } else if (callHold == TLKMDI_HFP_CALL_HOLD_HELD_AND_ACTIVE) {
        if (count == 1) {
            return false;
        }
        pCall = tlkmdi_bthfpag_getWaitCallUnit();
        if (pCall == NULL) {
            return false;
        }
        tlkmdi_bthfpag_activeCall(pCall->number, pCall->numbLen);
    } else if (callHold == TLKMDI_HFP_CALL_HOLD_HELD_AND_NO_ACTIVE) {
        pCall = tlkmdi_bthfpag_getActiveCallUnit();
        if (pCall == NULL) {
            return false;
        }
        tlkmdi_bthfpag_removeCall(pCall->number, pCall->numbLen);
    } else {
        return false;
    }

    return true;
}

/**
 * @brief       Push CLCC command
 * @param[in]   aclHandle   - The ACL connection handle.
 * @param[in]   pCall       - Pointer to the call unit.
 * @param[in]   index       - Call index.
 * @return      none.
 */
static void tlkmdi_bthfpag_pushClccCmdDeal(uint16_t aclHandle, tlkmdi_hfpag_unit_t *pCall, uint8_t index)
{
    uint8_t tempLen;
    uint8_t buffLen;
    uint8_t buffer[128];

    //<idx>,<dir>,<status>,<mode>,<mprty>[,<number>,<type>]
    buffLen           = 0;
    buffer[buffLen++] = '\r';
    buffer[buffLen++] = '\n';
    tempLen           = strlen("+CLCC: ");
    tmemcpy(buffer + buffLen, "+CLCC: ", tempLen);
    buffLen += tempLen;
    buffer[buffLen++] = 0x31 + index;
    buffer[buffLen++] = ',';
    buffer[buffLen++] = pCall->callDir;
    buffer[buffLen++] = ',';
    buffer[buffLen++] = pCall->status;
    buffer[buffLen++] = ',';
    buffer[buffLen++] = '0';
    buffer[buffLen++] = ',';
    buffer[buffLen++] = '0';
    buffer[buffLen++] = ',';
    buffer[buffLen++] = '\"';
    tmemcpy(buffer + buffLen, pCall->number, pCall->numbLen);
    buffLen += pCall->numbLen;
    buffer[buffLen++] = '\"';
    buffer[buffLen++] = ',';
    buffer[buffLen++] = '1';
    buffer[buffLen++] = '2';
    buffer[buffLen++] = '9';
    buffer[buffLen++] = ',';
    buffer[buffLen++] = '\"';
    buffer[buffLen++] = '\"';
    buffer[buffLen++] = '\r';
    buffer[buffLen++] = '\n';

    btp_hfpag_send(aclHandle, buffer, buffLen);
}

#if 0
static void tlkmdi_bthfpag_pushClipCmdDeal(uint16_t aclHandle, tlkmdi_hfpag_unit_t *pCall)
{
	uint8_t tempLen;
	uint8_t buffLen;
	uint8_t buffer[128];

	//<number>, type> [,<subaddr>,<satype> [,[<alpha>] [,<CLI validity>]]]
	buffLen = 0;
	tempLen = strlen("+CLIP: ");
	tmemcpy(buffer+buffLen, "+CLIP: ", tempLen);
	buffLen += tempLen;
	buffer[buffLen++] = '\"';
	tmemcpy(buffer+buffLen, pCall->number, pCall->numbLen);
	buffLen += pCall->numbLen;
	buffer[buffLen++] = '\"';
	buffer[buffLen++] = ',';
	buffer[buffLen++] = '1';
	buffer[buffLen++] = '2';
	buffer[buffLen++] = '9';
	buffer[buffLen++] = ',';
	buffer[buffLen++] = ',';
	buffer[buffLen++] = ',';
	buffer[buffLen++] = '\"';
	buffer[buffLen++] = '\"';
	buffer[buffLen++] = '\r';
	buffer[buffLen++] = '\n';
	btp_hfpag_send(aclHandle, buffer, buffLen);
}
#endif

/**
 * @brief       Get the count of call units
 * @param[in]   none.
 * @return      Count of call units.
 */
static uint tlkmdi_bthfpag_getCallUnitCount(void)
{
    uint8_t count = 0;
    if (sTlkMdiHfpAgCtrl.unit[0].status != 0) {
        count++;
    }
    if (sTlkMdiHfpAgCtrl.unit[1].status != 0) {
        count++;
    }
    return count;
}

/**
 * @brief       Get idle call unit
 * @param[in]   none.
 * @return      Pointer to idle call unit, NULL if not found.
 */
static tlkmdi_hfpag_unit_t *tlkmdi_bthfpag_getIdleCallUnit(void)
{
    if (sTlkMdiHfpAgCtrl.unit[0].status == 0) {
        return &sTlkMdiHfpAgCtrl.unit[0];
    }
    if (sTlkMdiHfpAgCtrl.unit[1].status == 0) {
        return &sTlkMdiHfpAgCtrl.unit[1];
    }
    return NULL;
}

/**
 * @brief       Get waiting call unit
 * @param[in]   none.
 * @return      Pointer to waiting call unit, NULL if not found.
 */
static tlkmdi_hfpag_unit_t *tlkmdi_bthfpag_getWaitCallUnit(void)
{
    if (sTlkMdiHfpAgCtrl.unit[0].status == TLKMDI_HFP_CLCC_STATUS_WAITING) {
        return &sTlkMdiHfpAgCtrl.unit[0];
    }
    if (sTlkMdiHfpAgCtrl.unit[1].status == TLKMDI_HFP_CLCC_STATUS_WAITING) {
        return &sTlkMdiHfpAgCtrl.unit[1];
    }
    return NULL;
}

/**
 * @brief       Get used call unit by phone number
 * @param[in]   numbLen     - Length of phone number.
 * @param[in]   pNumber     - Phone number.
 * @return      Pointer to used call unit, NULL if not found.
 */
static tlkmdi_hfpag_unit_t *tlkmdi_bthfpag_getUsedCallUnit(uint8_t numbLen, uint8_t *pNumber)
{
    if (numbLen == 0 || pNumber == NULL) {
        return NULL;
    }
    if (sTlkMdiHfpAgCtrl.unit[0].numbLen == numbLen && tmemcmp(sTlkMdiHfpAgCtrl.unit[0].number, pNumber, numbLen) == 0) {
        return &sTlkMdiHfpAgCtrl.unit[0];
    }
    if (sTlkMdiHfpAgCtrl.unit[1].numbLen == numbLen && tmemcmp(sTlkMdiHfpAgCtrl.unit[1].number, pNumber, numbLen) == 0) {
        return &sTlkMdiHfpAgCtrl.unit[1];
    }
    return NULL;
}

/**
 * @brief       Get active call unit
 * @param[in]   none.
 * @return      Pointer to active call unit, NULL if not found.
 */
static tlkmdi_hfpag_unit_t *tlkmdi_bthfpag_getActiveCallUnit(void)
{
    if (sTlkMdiHfpAgCtrl.unit[0].status == TLKMDI_HFP_CLCC_STATUS_ACTIVE) {
        return &sTlkMdiHfpAgCtrl.unit[0];
    }
    if (sTlkMdiHfpAgCtrl.unit[1].status == TLKMDI_HFP_CLCC_STATUS_ACTIVE) {
        return &sTlkMdiHfpAgCtrl.unit[1];
    }
    return NULL;
}

/**
 * @brief       Send call status changed event
 * @param[in]   pCall       - Pointer to the call unit.
 * @param[in]   evtStatus   - Event status.
 * @return      none.
 */
static void tlkmdi_bthfpag_sendStatusChangedEvt(tlkmdi_hfpag_unit_t *pCall, uint8_t evtStatus)
{
    uint8_t callDir;

    if (pCall->callDir == TLKMDI_HFP_CLCC_DIR_INCOMING) {
        callDir = BTP_HFP_CALL_DIR_INCOMING;
    } else {
        callDir = BTP_HFP_CALL_DIR_OUTGOING;
    }
    if (evtStatus != BTP_HFP_CALL_DIR_NONE) {
    } else if (pCall->status == TLKMDI_HFP_CLCC_STATUS_WAITING) {
        evtStatus = BTP_HFP_CALL_STATUS_WAITING;
    } else if (pCall->status == TLKMDI_HFP_CLCC_STATUS_INCOMING || pCall->status == TLKMDI_HFP_CLCC_STATUS_DIALING || pCall->status == TLKMDI_HFP_CLCC_STATUS_ALERTING) {
        evtStatus = BTP_HFP_CALL_STATUS_START;
    } else if (pCall->status == TLKMDI_HFP_CLCC_STATUS_HELD || pCall->status == TLKMDI_HFP_CLCC_STATUS_CALLHELD) {
        evtStatus = BTP_HFP_CALL_STATUS_PAUSED;
    } else if (pCall->status == TLKMDI_HFP_CLCC_STATUS_ACTIVE) {
        evtStatus = BTP_HFP_CALL_STATUS_ACTIVE;
    } else {
        evtStatus = BTP_HFP_CALL_STATUS_CLOSE;
    }
    btp_send_hfpagStatusChangedEvt(0, evtStatus, callDir, pCall->number, pCall->numbLen);
}


#endif // #if TLKMDI_AG_CALL_SETUP


#endif
