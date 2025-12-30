/********************************************************************************************************
 * @file    tlkmdi_btsco.c
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
#include "tlklib/usb/uac/tlkusb_uac.h"
#if (TLKBTP_CFG_HFP_ENABLE)
#include "stack/bt/host/bth/bth_event.h"
#include "stack/bt/host/bth/bth_handle.h"
#include "stack/bt/host/bth/bth_sco.h"
#include "stack/bt/host/bth/bth.h"
#include "stack/bt/host/btp/hfp/btp_hfp.h"
#include "stack/stack.h"

#define TLKMDI_BTSCO_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_SCO << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTSCO_DBG_SIGN "[MSCO]"


extern int btp_hfp_getCodec(uint16_t aclHandle, uint8_t *pCodec);

static int                    tlkmdi_btsco_connReqEvt(uint8_t *pData, uint16_t dataLen);
static int                    tlkmdi_btsco_connectEvt(uint8_t *pData, uint16_t dataLen);
static int                    tlkmdi_btsco_disconnEvt(uint8_t *pData, uint16_t dataLen);
tlkmdi_btsco_handle_t        *tlkmdi_btsco_getIdleItem(void);
static void                   tlkmdi_btsco_resetItemByAclHandle(uint16_t aclHandle);
static tlkmdi_btsco_handle_t *tlkmdi_btsco_getOtherUsedItem(uint16_t aclHandle);

static TlkMdiBtScoConnCB   sTlkMdiBtScoConnCB = NULL;
static tlkmdi_btsco_ctrl_t sTlkMdiBtScoCtrl;

BTH_EVT_REGISTER(BTH_EVTID_SCOCONN_REQUEST, tlkmdi_btsco_connReqEvt);
BTH_EVT_REGISTER(BTH_EVTID_SCOCONN_COMPLETE, tlkmdi_btsco_connectEvt);
BTH_EVT_REGISTER(BTH_EVTID_SCODISC_COMPLETE, tlkmdi_btsco_disconnEvt);

/**
 * @brief       This function is used to send host voice state change event
 * @param[in]   aclHandle - the ACL connection handle
 * @param[in]   scoHandle - the SCO connection handle
 * @param[in]   isStart - whether the voice is started
 * @param[in]   codecType - the codec type
 * @return      none.
 */
static void tlkmdi_btsco_sendHostVoiceStateChgEvt(uint16_t aclHandle, uint16_t scoHandle, uint8_t isStart, uint8_t codecType)
{
    const uint8_t         bufferLen = sizeof(tlksys_msg_hostEvt_t) + sizeof(tlksys_msg_hostEvt_btAudStateChg_t);
    uint8_t               buffer[bufferLen];
    tlksys_msg_hostEvt_t *evt                  = (tlksys_msg_hostEvt_t *)buffer;
    evt->dataLen                               = sizeof(tlksys_msg_hostEvt_btAudStateChg_t);
    evt->hostType                              = TLKSYS_MSG_HOST_TYPE_BT;
    evt->msgID                                 = TLKSYS_MSG_BT_HOST_EVT_TYPE_AUD_STATE_CHG;
    tlksys_msg_hostEvt_btAudStateChg_t *evtDat = (tlksys_msg_hostEvt_btAudStateChg_t *)evt->data;
    evtDat->handle                             = aclHandle;
    evtDat->scoHandle                          = scoHandle;
    evtDat->state                              = isStart;
    evtDat->isMusic                            = 0;
    evtDat->codecType                          = codecType;
    tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_HOST_EVT_COME, buffer, bufferLen);
}

/**
 * @brief       This function is used to initialize the SCO module
 * @param       none.
 * @return      TLK_ENONE if success.
 */
int tlkmdi_btsco_init(void)
{
    tmemset(&sTlkMdiBtScoCtrl, 0, sizeof(tlkmdi_btsco_ctrl_t));

    return TLK_ENONE;
}

/**
 * @brief       This function is used to register SCO connection callback
 * @param[in]   connCB - the connection callback function
 * @return      none.
 */
void tlkmdi_btsco_regCB(TlkMdiBtScoConnCB connCB)
{
    sTlkMdiBtScoConnCB = connCB;
}

/**
 * @brief       This function is used to handle SCO connection request event
 * @param[in]   pData - the pointer to the event data
 * @param[in]   dataLen - the length of the event data
 * @return      0 if success, otherwise error code.
 */
static int tlkmdi_btsco_connReqEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    bth_scoConnRequestEvt_t *pEvt;
    bth_acl_handle_t        *pAclItem;

    uint8_t airMode;
    uint8_t codec = 0;

    pEvt = (bth_scoConnRequestEvt_t *)pData;
    tlkapi_array(TLKMDI_BTSCO_DBG_FLAG, TLKMDI_BTSCO_DBG_SIGN, "tlkmdi_btsco_connReqEvt:", pEvt->peerMac, 6);
    pAclItem = bth_handle_searchConnAcl(pEvt->peerMac);
    if (pAclItem == NULL) {
        tlkapi_error(TLKMDI_BTSCO_DBG_FLAG, TLKMDI_BTSCO_DBG_SIGN, "no connected ACL device");
        return -TLK_ENODEV;
    }

    btp_hfp_getCodec(pAclItem->aclHandle, &codec);

    if (codec == BTP_HFP_CODEC_TYPE_MSBC) {
        airMode = BTH_SCO_AIRMODE_TRANS;
    } else {
        airMode = BTH_SCO_AIRMODE_CVSD;
    }

    bth_handle_set_all_acl_max_slot(0x01);

    return bth_sco_setAirModeByAclHandle(pAclItem->aclHandle, airMode);
}

/**
 * @brief       This function is used to handle SCO connection complete event
 * @param[in]   pData - the pointer to the event data
 * @param[in]   dataLen - the length of the event data
 * @return      0 if success, otherwise error code.
 */
static int tlkmdi_btsco_connectEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    bth_scoConnCompleteEvt_t *pEvt;
    tlkmdi_btsco_handle_t    *pItem;

    pEvt = (bth_scoConnCompleteEvt_t *)pData;
    if (pEvt->status != 0) {
        tlkapi_error(TLKMDI_BTSCO_DBG_FLAG, TLKMDI_BTSCO_DBG_SIGN, "tlkmdi_btsco_connectEvt: failure - %d", pEvt->status);
        return TLK_ENONE;
    }
    tlkapi_trace(TLKMDI_BTSCO_DBG_FLAG, TLKMDI_BTSCO_DBG_SIGN, "tlkmdi_btsco_connectEvt: {status-%d,handle-0x%x,scoHandle-0x%x,linkType-%d}", pEvt->status, pEvt->aclHandle,
                 pEvt->scoHandle, pEvt->linkType);
    sTlkMdiBtScoCtrl.scoCnt++;
    pItem = tlkmdi_btsco_getIdleItem();
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTSCO_DBG_FLAG, TLKMDI_BTSCO_DBG_SIGN, "tlkmdi_btsco_connectEvt - no enough item:%d", pEvt->aclHandle);
        return -TLK_ENOITEM;
    }
    pItem->aclHandle = pEvt->aclHandle;
    pItem->scoHandle = pEvt->scoHandle;

#if (TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE)
    if (sTlkUsbUacEvt2StatusCB != NULL) {
        sTlkUsbUacEvt2StatusCB(TLK_UAC_VOICE_START_RSP);
    }
#endif

    bth_handle_set_all_acl_max_slot(0x01);

    if (bth_handle_getConnScoCount() < TLKMDI_BTSCO_MAX_NUM) {
        uint8_t codecType = TLKMDI_BTSCO_CODEC_ID_CVSD;
        if (!btp_hfp_getCodec(pEvt->aclHandle, &pItem->codec)) {
            codecType = pItem->codec;
        }
        tlkmdi_btsco_sendHostVoiceStateChgEvt(pItem->aclHandle, pItem->scoHandle, true, codecType);
    } else if (bth_handle_getConnScoCount() == TLKMDI_BTSCO_MAX_NUM && btp_hfp_getHfHandle() && btp_hfp_getAgHandle()) { // enable esco - only hf+ag
    }

    if (sTlkMdiBtScoConnCB != NULL) {
        sTlkMdiBtScoConnCB(pEvt->aclHandle, pEvt->scoHandle, true);
    }

    return TLK_ENONE;
}

/**
 * @brief       This function is used to handle SCO disconnection complete event
 * @param[in]   pData - the pointer to the event data
 * @param[in]   dataLen - the length of the event data
 * @return      0 if success, otherwise error code.
 */
static int tlkmdi_btsco_disconnEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;

    bth_scoDiscCompleteEvt_t *pEvt = (bth_scoDiscCompleteEvt_t *)pData;
    tlkapi_trace(TLKMDI_BTSCO_DBG_FLAG, TLKMDI_BTSCO_DBG_SIGN, "tlkmdi_btsco_disconnEvt: {status-%d,handle-%d,scoHandle-%d,linkType-%d}", pEvt->reason, pEvt->aclHandle,
                 pEvt->scoHandle, pEvt->linkType);
    sTlkMdiBtScoCtrl.scoCnt--;
    tlkmdi_btsco_resetItemByAclHandle(pEvt->aclHandle);

#if (TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE)
    if (sTlkUsbUacEvt2StatusCB != NULL) {
        sTlkUsbUacEvt2StatusCB(TLK_UAC_VOICE_STOP_RSP);
    }

    if (tlkmdi_bthfpag_get_discActive() == false) {
        tlkusb_uac_update_pending_tick();
    }
    tlkmdi_bthfpag_clear_discActive();
#endif

    if (sTlkMdiBtScoConnCB != NULL) {
        sTlkMdiBtScoConnCB(pEvt->aclHandle, pEvt->scoHandle, false);
    }

    bth_handle_set_all_acl_max_slot(0x05);

    if (sTlkMdiBtScoCtrl.scoCnt < TLKMDI_BTSCO_MAX_NUM) {
        tlkmdi_btsco_handle_t *pItem = tlkmdi_btsco_getOtherUsedItem(pEvt->aclHandle);
        if (pItem != NULL) {
            uint8_t codecType = TLKMDI_BTSCO_CODEC_ID_CVSD;
            if (!btp_hfp_getCodec(pEvt->aclHandle, &pItem->codec)) {
                codecType = pItem->codec;
            }
            tlkmdi_btsco_sendHostVoiceStateChgEvt(pItem->aclHandle, pItem->scoHandle, false, codecType);
        } else {
#if (!(TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
            tlkmdi_btsco_sendHostVoiceStateChgEvt(pEvt->aclHandle, pEvt->scoHandle, false, TLKMDI_BTSCO_CODEC_ID_CVSD);
#else
            extern void tlkmdi_u2h_voice_disable(void);
            tlkmdi_u2h_voice_disable();
//TODO: ZIYU not thread safe temp code
#endif
        }
    }

#if (TLK_BT_MULTIPNT_ENABLE)
    for (uint8_t index = 0; index < TLKMDI_HFPHF_MAX_NUMBER; index++) {
        uint16_t             newHandle = tlkmdi_bthfphf_getHandle(index);
        tlkmdi_hfphf_ctrl_t *phfItem   = tlkmdi_hfphf_getItem(newHandle);
        if (phfItem == NULL || newHandle == pEvt->aclHandle) {
            continue;
        }
        if ((phfItem->status == BTP_HFP_CALL_STATUS_ACTIVE) || (phfItem->setup_status != BTP_HFP_CALL_DIR_NONE)) {
            btp_hfphf_codecConn(newHandle);
        }
    }
#endif

    return TLK_ENONE;
}

/**
 * @brief       This function is used to get idle SCO handle item
 * @param       none.
 * @return      the pointer to the idle SCO handle item.
 */
tlkmdi_btsco_handle_t *tlkmdi_btsco_getIdleItem(void)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_BTSCO_MAX_NUM; index++) {
        if (sTlkMdiBtScoCtrl.pItem[index].aclHandle == 0 && sTlkMdiBtScoCtrl.pItem[index].scoHandle == 0) {
            break;
        }
    }
    if (index == TLKMDI_BTSCO_MAX_NUM) {
        return NULL;
    }
    return &sTlkMdiBtScoCtrl.pItem[index];
}

/**
 * @brief       This function is used to get used SCO handle item by ACL handle
 * @param[in]   aclHandle - the ACL connection handle
 * @return      the pointer to the used SCO handle item.
 */
tlkmdi_btsco_handle_t *tlkmdi_btsco_getUsedItemByAclHandle(uint16_t aclHandle)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_BTSCO_MAX_NUM; index++) {
        if (sTlkMdiBtScoCtrl.pItem[index].aclHandle != 0 && sTlkMdiBtScoCtrl.pItem[index].aclHandle == aclHandle) {
            break;
        }
    }
    if (index == TLKMDI_BTSCO_MAX_NUM) {
        return NULL;
    }
    return &sTlkMdiBtScoCtrl.pItem[index];
}

/**
 * @brief       This function is used to reset SCO handle item by ACL handle
 * @param[in]   aclHandle - the ACL connection handle
 * @return      none.
 */
static void tlkmdi_btsco_resetItemByAclHandle(uint16_t aclHandle)
{
    tlkmdi_btsco_handle_t *pItem = tlkmdi_btsco_getUsedItemByAclHandle(aclHandle);
    if (pItem != NULL) {
        memset(pItem, 0, sizeof(tlkmdi_btsco_handle_t));
    }
}

/**
 * @brief       This function is used to get another used SCO handle item
 * @param[in]   aclHandle - the ACL connection handle
 * @return      the pointer to the used SCO handle item.
 */
static tlkmdi_btsco_handle_t *tlkmdi_btsco_getOtherUsedItem(uint16_t aclHandle)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_BTSCO_MAX_NUM; index++) {
        if (sTlkMdiBtScoCtrl.pItem[index].aclHandle != 0 && sTlkMdiBtScoCtrl.pItem[index].aclHandle != aclHandle) {
            break;
        }
    }
    if (index == TLKMDI_BTSCO_MAX_NUM) {
        return NULL;
    }
    return &sTlkMdiBtScoCtrl.pItem[index];
}
#endif // #if (TLKBTP_CFG_HFP_ENABLE)