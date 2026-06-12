/********************************************************************************************************
 * @file    tlkmdi_bthfphf.c
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
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "tlkmw/tlkmw.h"
#if (TLKBTP_CFG_HFPHF_ENABLE)

#define TLKMDI_BTHFPHF_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_HFP << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTHFPHF_DBG_SIGN "[MHFP-HF]"

static tlkmdi_hfphf_ctrl_t sTlkMdiBtHfpCtrl[TLKMDI_HFPHF_MAX_NUMBER];

static int tlkmdi_hfphf_unknownCmdCB(uint16_t aclHandle, uint8_t *pCmd, uint8_t cmdLen);

BTP_EVT_REGISTER(BTP_EVTID_HFPHF_CODEC_CHANGED, tlkmdi_hfphf_codecChangedEvt);
BTP_EVT_REGISTER(BTP_EVTID_HFPHF_VOLUME_CHANGED, tlkmdi_hfphf_volumeChangedEvt);
BTP_EVT_REGISTER(BTP_EVTID_HFPHF_STATUS_CHANGED, tlkmdi_hfphf_statusChangedEvt);
BTP_EVT_REGISTER(BTP_EVTID_HFPHF_CALL_STATUS_CHANGED, tlkmdi_hfphf_callStatusChangedEvt);
BTP_EVT_REGISTER(BTP_EVTID_HFPHF_NUMBER_INQUIRY, tlkmdi_hfphf_numberInquiryEvt);

/**
 * @brief       This function initializes the HFP HF module
 * @param[in]   none
 * @return      TLK_ENONE
 */
int tlkmdi_bthfphf_init(void)
{
    btp_hfphf_regRecvCmdCB(tlkmdi_hfphf_recvCmdCB);
    btp_hfphf_regUnknownCmdCB(tlkmdi_hfphf_unknownCmdCB);

    return TLK_ENONE;
}

/**
 * @brief       This function handles HFP codec changed event
 * @param[in]   pData     - pointer to event data
 * @param[in]   dataLen   - length of event data
 * @return      TLK_ENONE if success, otherwise error code
 */
__attribute__((weak)) int tlkmdi_hfphf_codecChangedEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    tlkmdi_hfphf_ctrl_t      *pItem = NULL;
    btp_hfpCodecChangedEvt_t *pEvt;
    pEvt = (btp_hfpCodecChangedEvt_t *)pData;

    pItem = tlkmdi_hfphf_getItem(pEvt->handle);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_bthfphf_codecChangedEvt error handle");
        return -TLK_EHANDLE;
    }
    pItem->codec = pEvt->codec;

    return TLK_ENONE;
}

/**
 * @brief       This function handles HFP volume changed event
 * @param[in]   pData     - pointer to event data
 * @param[in]   dataLen   - length of event data
 * @return      TLK_ENONE if success, otherwise error code
 */
__attribute__((weak)) int tlkmdi_hfphf_volumeChangedEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_hfpVolumeChangedEvt_t *pEvt = (btp_hfpVolumeChangedEvt_t *)pData;
    if (pEvt->volType != BTP_HFP_VOLUME_TYPE_SPK) {
        return TLK_ENONE;
    }
    tlkapi_trace(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_bthfphf_volumeChangedEvt: %d", pEvt->volume);
    tlkmdi_btacl_item_t *item = tlkmdi_btacl_getConnItem(pEvt->handle);
    if (item == NULL) {
        return -TLK_ESEEK;
    }
    uint8_t isIos = 0;
    tlkmdi_tinySql_getPairingDeviceVolume(item->btaddr, false, NULL, &isIos);
    tlkmdi_tinySql_setPairingDeviceVolume(item->btaddr, false, pEvt->volume, isIos);

    const uint8_t         bufferLen = sizeof(tlksys_msg_hostEvt_t) + sizeof(tlksys_msg_hostEvt_btVolChg_t);
    uint8_t               buffer[bufferLen];
    tlksys_msg_hostEvt_t *evt             = (tlksys_msg_hostEvt_t *)buffer;
    evt->dataLen                          = sizeof(tlksys_msg_hostEvt_btVolChg_t);
    evt->hostType                         = TLKSYS_MSG_HOST_TYPE_BT;
    evt->msgID                            = TLKSYS_MSG_BT_HOST_EVT_TYPE_VOLUME_CHG;
    tlksys_msg_hostEvt_btVolChg_t *evtDat = (tlksys_msg_hostEvt_btVolChg_t *)evt->data;
    evtDat->handle                        = pEvt->handle;
    evtDat->isMusic                       = false;
    evtDat->volume                        = pEvt->volume;
    tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_HOST_EVT_COME, buffer, bufferLen);


    return TLK_ENONE;
}

static void tlkmdi_hfphf_timer(TlkApiTimerHandle_t timer, void *arg)
{
    (void)timer;
    uint16_t             handle = (uint32_t)arg;
    tlkmdi_hfphf_ctrl_t *pItem  = tlkmdi_hfphf_getItem(handle);
    if (pItem == NULL) {
        return;
    }
    if (pItem->ring_timer_cnt != 0) {
        pItem->ring_timer_cnt--;
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &pItem->timer);
        return;
    }
    tlkmdi_btacl_item_t *pItem_t;
    pItem_t = tlkmdi_btacl_getConnItem(handle);
    if (pItem_t->state == TLK_STATE_CONNECT) {
        tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_RING_PLAY, (uint8_t *)&handle, sizeof(handle));
        pItem->ring_timer_cnt = 30;
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &pItem->timer);
    } else {
        tlksys_timer_stop(TLKSYS_TASKID_HOST, &pItem->timer);
    }
}

/**
 * @brief       This function handles HFP status changed event
 * @param[in]   pData     - pointer to event data
 * @param[in]   dataLen   - length of event data
 * @return      TLK_ENONE if success, otherwise error code
 */
__attribute__((weak)) int tlkmdi_hfphf_statusChangedEvt(uint8_t *pData, uint16_t dataLen)
{
    tlkmdi_hfphf_ctrl_t       *pItem;
    btp_hfpStatusChangedEvt_t *pEvt;

    pEvt = (btp_hfpStatusChangedEvt_t *)pData;
    tlkapi_array(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "statusChangedEvt:", pData, dataLen);
    pItem = tlkmdi_hfphf_getItem(pEvt->handle);
    if (pItem == NULL) {
        pItem = tlkmdi_hfphf_getIdleItem();
    }

    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_hfphf_statusChangedEvt no idle item");
        return -TLK_ENOITEM;
    }

    pItem->handle = pEvt->handle;

    if (pEvt->status == BTP_HFP_CALL_STATUS_ALART && pEvt->callDir == BTP_HFP_CALL_DIR_INCOMING) {
        tlkapi_trace(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_hfphf_statusChangedEvt: ring coming");
    } else if (pEvt->status == BTP_HFP_CALL_STATUS_START && pEvt->callDir == BTP_HFP_CALL_DIR_INCOMING) {
        tlkapi_trace(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_hfphf_statusChangedEvt: incoming call");
        tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &pItem->timer, 100 * 1000, 0, tlkmdi_hfphf_timer, (void *)(uint32_t)pEvt->handle);
        tlksys_timer_start(TLKSYS_TASKID_HOST, &pItem->timer);
        pItem->ring_timer_cnt = 10;
    } else {
        tlksys_timer_stop(TLKSYS_TASKID_HOST, &pItem->timer);
    }


    return TLK_ENONE;
}

/**
 * @brief       This function handles HFP call status changed event
 * @param[in]   pData     - pointer to event data
 * @param[in]   dataLen   - length of event data
 * @return      TLK_ENONE if success, otherwise error code
 */
__attribute__((weak)) int tlkmdi_hfphf_callStatusChangedEvt(uint8_t *pData, uint16_t dataLen)
{
    tlkmdi_hfphf_ctrl_t           *pItem;
    btp_hfpCallStatusChangedEvt_t *pEvt;
    tlkapi_array(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "callStatusChangedEvt:", pData, dataLen);
    pEvt = (btp_hfpCallStatusChangedEvt_t *)pData;
    if (pEvt == NULL) {
        tlkapi_error(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_hfphf_callStatusChangedEvt error param");
        return -TLK_EPARAM;
    }


    pItem = tlkmdi_hfphf_getItem(pEvt->handle);
    if (pItem == NULL) {
        pItem = tlkmdi_hfphf_getIdleItem();
    }

    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_hfphf_callStatusChangedEvt no idle item");
        return -TLK_ENOITEM;
    }

    pItem->handle = pEvt->handle;
    if (pEvt->type == BTP_HFP_CALL_TYPE) {
        pItem->status = pEvt->status;
    } else if (pEvt->type == BTP_HFP_CALL_SETUP_TYPE) {
        pItem->setup_status = pEvt->status;
    } else if (pEvt->type == BTP_HFP_CALL_HELD_TYPE) {
        pItem->held_status = pEvt->status;
    }

    return TLK_ENONE;
}

/**
 * @brief       This function handles HFP number inquiry event
 * @param[in]   pData     - pointer to event data
 * @param[in]   dataLen   - length of event data
 * @return      TLK_ENONE if success, otherwise error code
 */
__attribute__((weak)) int tlkmdi_hfphf_numberInquiryEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t                    status = 0;
    tlkmdi_hfphf_ctrl_t       *pItem;
    btp_hfpNumberInquiryEvt_t *pEvt;
    pEvt = (btp_hfpNumberInquiryEvt_t *)pData;
    if (pEvt == NULL) {
        tlkapi_error(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_hfphf_numberInquiryEvt error param");
        return -TLK_EPARAM;
    }

    pItem = tlkmdi_hfphf_getItem(pEvt->handle);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_hfphf_numberInquiryEvt no item");
        return -TLK_ENOITEM;
    }

    if (pEvt->clcc_info == NULL || pEvt->clcc_info->numbLen == 0) {
        return -TLK_EPARAM;
    }

    if (pEvt->clcc_info->numbLen > TLKMDI_HFPHF_NUMBER_MAX_LEN) {
        pEvt->clcc_info->numbLen = TLKMDI_HFPHF_NUMBER_MAX_LEN;
    }

    pItem->numbLen = pEvt->clcc_info->numbLen;
    tmemcpy(pItem->number, pEvt->clcc_info->pNumber, pEvt->clcc_info->numbLen);

    // if (pEvt->clcc_info->dir == 0x30) {
    //     callDir = BTP_HFP_CALL_DIR_OUTGOING;
    // } else if (pEvt->clcc_info->dir == 0x31) {
    //     callDir = BTP_HFP_CALL_DIR_INCOMING;
    // } else {
    //     callDir = BTP_HFP_CALL_DIR_NONE;
    // }

    if (pEvt->clcc_info->status == BTP_HFP_CLCC_STATUS_DIALING || pEvt->clcc_info->status == BTP_HFP_CLCC_STATUS_INCOMING) {
        status = BTP_HFP_CALL_STATUS_START;
    } else if (pEvt->clcc_info->status == BTP_HFP_CLCC_STATUS_ALERTING) {
        status = BTP_HFP_CALL_STATUS_ALART;
    } else if (pEvt->clcc_info->status == BTP_HFP_CLCC_STATUS_ACTIVE) {
        status = BTP_HFP_CALL_STATUS_ACTIVE;
    } else if (pEvt->clcc_info->status == BTP_HFP_CLCC_STATUS_WAITING) {
        status = BTP_HFP_CALL_STATUS_WAITING;
    } else if (pEvt->clcc_info->status == BTP_HFP_CLCC_STATUS_HELD || pEvt->clcc_info->status == BTP_HFP_CLCC_STATUS_CALLHELD) {
        status = BTP_HFP_CALL_STATUS_PAUSED;
    }

    pItem->status = status;
    pItem->handle = pEvt->handle;

    tlkapi_trace(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_hfphf_numberInquiryEvt: handle-%d,status-%d,numbLen-%d, number-%s", pItem->handle, pItem->status,
                 pItem->numbLen, pItem->number);

    return TLK_ENONE;
}

/**
 * @brief       This function handles received HFP command
 * @param[in]   aclHandle   - ACL connection handle
 * @param[in]   pCmd        - pointer to command data
 * @param[in]   cmdLen      - length of command data
 * @return      TLK_EFAIL
 */
__attribute__((weak)) int tlkmdi_hfphf_recvCmdCB(uint16_t aclHandle, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)aclHandle;
    // When the HFP connection is complete the user do something.
    tlkapi_array(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_hfphf_recvCmdCB: ", pCmd, cmdLen);

    return TLK_EFAIL;
}

/**
 * @brief       This function handles unknown HFP command
 * @param[in]   aclHandle   - ACL connection handle
 * @param[in]   pCmd        - pointer to command data
 * @param[in]   cmdLen      - length of command data
 * @return      TLK_ENONE
 */
static int tlkmdi_hfphf_unknownCmdCB(uint16_t aclHandle, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)aclHandle;
    tlkapi_array(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_bthfphf_unknownCmdCB: ", pCmd, cmdLen);
    tlkapi_warn(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_bthfphf_unknownCmdCB: User-defined AT commands are not allowed to be sent during a connection");

    return TLK_ENONE;
}

/**
 * @brief       This function gets HFP control item by handle
 * @param[in]   handle   - connection handle
 * @return      pointer to HFP control item if found, NULL otherwise
 */
tlkmdi_hfphf_ctrl_t *tlkmdi_hfphf_getItem(uint16_t handle)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_HFPHF_MAX_NUMBER; index++) {
        if (sTlkMdiBtHfpCtrl[index].handle != 0 && sTlkMdiBtHfpCtrl[index].handle == handle) {
            break;
        }
    }

    if (index == TLKMDI_HFPHF_MAX_NUMBER) {
        return NULL;
    } else {
        return &sTlkMdiBtHfpCtrl[index];
    }
}

tlkmdi_hfphf_ctrl_t *tlkmdi_hfphf_getItemExt(uint16_t handle)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_HFPHF_MAX_NUMBER; index++) {
        if (sTlkMdiBtHfpCtrl[index].handle != 0 && sTlkMdiBtHfpCtrl[index].handle != handle) {
            break;
        }
    }

    if (index == TLKMDI_HFPHF_MAX_NUMBER) {
        return NULL;
    } else {
        return &sTlkMdiBtHfpCtrl[index];
    }
}

/**
 * @brief       This function gets idle HFP control item
 * @param[in]   none
 * @return      pointer to idle HFP control item if found, NULL otherwise
 */
tlkmdi_hfphf_ctrl_t *tlkmdi_hfphf_getIdleItem(void)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_HFPHF_MAX_NUMBER; index++) {
        if (sTlkMdiBtHfpCtrl[index].handle == 0) {
            break;
        }
    }

    if (index == TLKMDI_HFPHF_MAX_NUMBER) {
        return NULL;
    } else {
        return &sTlkMdiBtHfpCtrl[index];
    }
}

/**
 * @brief       This function gets call number by ACL handle
 * @param[in]   aclHandle   - ACL connection handle
 * @return      pointer to call number if found, NULL otherwise
 */
uint8_t *tlkmdi_bthfphf_getCallNumber(uint16_t aclHandle)
{
    tlkmdi_hfphf_ctrl_t *pItem;
    pItem = tlkmdi_hfphf_getItem(aclHandle);

    if (pItem == NULL) {
        return NULL;
    }
    return pItem->number;
}

/**
 * @brief       This function rejects waiting call and keeps active call
 * @param[in]   aclHandle   - ACL connection handle
 * @return      btp_hfphf_rejectWaitAndKeepActive result
 */
int tlkmdi_bthfphf_rejectWaitAndKeepActive(uint16_t aclHandle)
{
    if (aclHandle == 0) {
        return -TLK_EFAIL;
    }
    return btp_hfphf_rejectWaitAndKeepActive(aclHandle);
}

/**
 * @brief       This function accepts waiting call and holds active call
 * @param[in]   aclHandle   - ACL connection handle
 * @return      btp_hfphf_acceptWaitAndHoldActive result
 */
int tlkmdi_bthfphf_acceptWaitAndHoldActive(uint16_t aclHandle)
{
    if (aclHandle == 0) {
        return -TLK_EFAIL;
    }
    return btp_hfphf_acceptWaitAndHoldActive(aclHandle);
}

/**
 * @brief       This function hangs up active call and resumes held call
 * @param[in]   aclHandle   - ACL connection handle
 * @return      btp_hfphf_hungUpActiveAndResumeHold result
 */
int tlkmdi_bthfphf_hungupActiveAndResumeHold(uint16_t aclHandle)
{
    if (aclHandle == 0) {
        return -TLK_EFAIL;
    }
    return btp_hfphf_hungUpActiveAndResumeHold(aclHandle);
}

/**
 * @brief       This function controls assistant (Siri) function
 * @param[in]   handle   - connection handle
 * @return      TLK_ENONE if success, otherwise error code
 */
int tlkmdi_bthfphf_assistant(uint16_t handle)
{
    tlkmdi_hfphf_ctrl_t *pItem;

    if (handle == 0) {
        return -TLK_EHANDLE;
    }
    pItem = tlkmdi_hfphf_getItem(handle);
    if (pItem == NULL) {
        return -TLK_EPARAM;
    }

    if (btp_hfphf_isIosDev(handle) || btp_hfphf_isSupportVoiceRecog(handle)) {
        if (!(pItem->setup_status > BTP_HFP_CALL_DIR_NONE)) {
            return btp_hfphf_siri_ctrl(handle);
        } else {
            tlkapi_error(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_bthfphf_assistant - not support siri");
            return -TLK_ENOSUPPORT;
        }
    }

    return TLK_ENONE;
}

/**
 * @brief       This function resets HFP HF module by handle
 * @param[in]   aclHandle   - ACL connection handle
 * @return      none
 */
void tlkmdi_bthfphf_reset(uint16_t aclHandle)
{
    tlkmdi_hfphf_ctrl_t *pItem;

    pItem = tlkmdi_hfphf_getItem(aclHandle);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_bthfphf_reset - failure: not exit handle: 0x%x", aclHandle);
        return;
    }
    tlksys_timer_destroy(TLKSYS_TASKID_HOST, &pItem->timer);
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkmdi_hfphf_ctrl_t)));
    memset(&sTlkMdiBtHfpCtrl, 0, sizeof(tlkmdi_hfphf_ctrl_t));
}

/**
 * @brief       This function gets handle by index
 * @param[in]   index   - item index
 * @return      handle if success, otherwise error code
 */
uint16_t tlkmdi_bthfphf_getHandle(int index)
{
    if (index >= TLKMDI_HFPHF_MAX_NUMBER) {
        tlkapi_error(TLKMDI_BTHFPHF_DBG_FLAG, TLKMDI_BTHFPHF_DBG_SIGN, "tlkmdi_bthfp_getHandle - failure invalid index: %d", index);
        return -TLK_EPARAM;
    }
    return sTlkMdiBtHfpCtrl[index].handle;
}

#endif
