/********************************************************************************************************
 * @file    btp_custp.c
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
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/btp/btp_adapt.h"
#if (TLKBTP_CFG_CUSTP_ENABLE)
#include "btp_custpInner.h"
#include "btp_custp.h"


static BtpCustpRecvDataCallback sBtpCustpRecvDataCb = NULL;

static void btp_custp_timer(TlkApiTimerHandle_t pTimer, void *userArg);
static void btp_custp_procs(btp_custp_item_t *pCustp);

static int  btp_custp_eventCB(uint8_t evtID, uint16_t psmID, uint8_t *pData, uint16_t dataLen);
static void btp_custp_rdataCB(uint16_t psmID, uint16_t chnID, uint16_t handle, uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: CUSTP initial interface
 * Descript: Defines trigger the initial flow of CUSTP sink and CUSTP source
 * Params:
 * 		@count[IN]--the CUSTP number, refer to TLK_BT_CUSTP_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value(-TLK_ENOMEM) is returned means the initial process fail.
 *******************************************************************************/
int btp_custp_init(uint8_t count)
{
    btp_custp_innerInit(count);
    return bth_l2cap_regServiceCB(BTP_PSMID_PROP_SIGNAL, btp_custp_eventCB, btp_custp_rdataCB);
}

/******************************************************************************
 * Function: CUSTP deinitial interface
 * Descript: Defines trigger the initial flow of CUSTP sink and CUSTP source
 * Params:
 * 		@count[IN]--the CUSTP number, refer to TLK_BT_CUSTP_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int btp_custp_deinit(void)
{
    return btp_custp_innerDeinit();
}

/******************************************************************************
 * Function: btp_custp_getMemLen
 * Descript: CUSTP get the size of the required number of nodes interface.
 * Params:
 * 		@count[IN]--the CUSTP number, refer to TLK_BT_CUSTP_MAX_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int btp_custp_getMemLen(uint8_t count)
{
    return btp_custp_innerGetMemLen(count);
}

/******************************************************************************
 * Function: btp_custp_connect
 * Descript: Defines trigger the custp connect cmd to peer custp entity and setup
 *           a connection with devices.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @usrId--The service user id.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_custp_connect(uint16_t aclHandle, uint8_t usrID)
{
    int               ret;
    uint16_t          scid;
    btp_custp_item_t *pCustp;

    pCustp = btp_custp_getUsedNode(aclHandle);
    if (pCustp != NULL) {
        tlkapi_error(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_connect: %d failure - exist", aclHandle);
        return -TLK_EREPEAT;
    }
    if (bth_l2cap_getUsedChannelByPsm(aclHandle, BTP_PSMID_PROP_SIGNAL) != NULL) {
        tlkapi_error(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_connect: %d failure - busying", aclHandle);
        return -TLK_EBUSY;
    }

    pCustp = btp_custp_getIdleNode();
    if (pCustp == NULL) {
        tlkapi_error(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_connect: %d failure - no queta", aclHandle);
        return -TLK_EQUOTA;
    }

    if (usrID == BTP_USRID_NONE) {
        usrID = BTP_USRID_IGNORE;
    }

    tlkapi_trace(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_connect: %d", aclHandle);

    scid = 0;
    ret  = bth_signal_createConnect(aclHandle, BTP_PSMID_PROP_SIGNAL, usrID, &scid);
    if (ret != TLK_ENONE && ret != -TLK_EBUSY) {
        tlkapi_error(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_connect: failure - signal reject");
    } else {
        btp_timer_create(&pCustp->timer, BTP_TIMER_TIMEOUT, 0, btp_custp_timer, pCustp);
        pCustp->state     = TLK_STATE_CONNING;
        pCustp->chnID     = scid;
        pCustp->timeout   = gBtpConnTimeout;
        pCustp->aclHandle = aclHandle;
        btp_timer_start(&pCustp->timer);
        tlkapi_info(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_connect: start");
    }
    return ret;
}

/******************************************************************************
 * Function: btp_custp_disconn
 * Descript: Defines trigger the custp disconnect cmd to peer custp entity and
 *           tear up a connection which specify by aclhandle and usrid.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 *     @usrId--The service user id.
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int btp_custp_disconn(uint16_t aclHandle, uint8_t usrID)
{
    (void)usrID;
    int               ret;
    btp_custp_item_t *pCustp;

    pCustp = btp_custp_getUsedNode(aclHandle);
    if (pCustp == NULL) {
        bth_signal_disconnChannelByUsrID(aclHandle, BTP_PSMID_PROP_SIGNAL, 0);
        tlkapi_error(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_disconn: pCustp is null");
        return -TLK_EPARAM;
    }

    tlkapi_trace(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_disconn: usrID[%d] chnID[0x%x]", usrID, pCustp->chnID);

    if (pCustp->chnID == 0) {
        ret = TLK_ENONE;
        bth_signal_disconnChannelByUsrID(aclHandle, BTP_PSMID_PROP_SIGNAL, 0);
        btp_custp_resetNode(pCustp);
        btp_send_connectEvt(TLK_ETIMEOUT, BTP_PTYPE_CUSTP, BTP_USRID_NONE, aclHandle, 0);
    } else {
        uint16_t chnID = pCustp->chnID;
        ret            = bth_signal_disconnChannel(aclHandle, chnID);
        if (ret != -TLK_EBUSY) {
            ret = TLK_ENONE;
            btp_custp_resetNode(pCustp);
            btp_send_disconnEvt(BTP_PTYPE_CUSTP, BTP_USRID_NONE, aclHandle);
            tlkapi_info(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_disconn: success");
        } else {
            pCustp->state   = TLK_STATE_DISCING;
            pCustp->timeout = gBtpDiscTimeout;
            btp_timer_start(&pCustp->timer);
        }
    }
    return ret;
}

/******************************************************************************
 * Function: btp_custp_destroy
 * Descript: Defines trigger to release the resource which allocated.
 * Params:
 *     @aclHandle--Connection handle of the current ACL.
 * Return: null
 *******************************************************************************/
void btp_custp_destroy(uint16_t aclHandle)
{
    uint8_t           state;
    uint16_t          chnID;
    btp_custp_item_t *pCustp;

    pCustp = btp_custp_getUsedNode(aclHandle);
    if (pCustp == NULL) {
        bth_signal_destroyConnectByUsrID(aclHandle, BTP_PSMID_PROP_SIGNAL, 0);
        tlkapi_error(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_destroy: custp not exist");
        return;
    }

    tlkapi_trace(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_destroy: handle[%d] state[%d] chnID[0x%x]", aclHandle, pCustp->state, pCustp->chnID);

    state = pCustp->state;
    chnID = pCustp->chnID;
    btp_custp_resetNode(pCustp);

    if (state == TLK_STATE_CONNING) {
        btp_send_connectEvt(TLK_ETIMEOUT, BTP_PTYPE_CUSTP, BTP_USRID_NONE, aclHandle, chnID);
    } else {
        btp_send_disconnEvt(BTP_PTYPE_CUSTP, BTP_USRID_NONE, aclHandle);
    }
}

int btp_custp_sendData(uint16_t aclHandle, uint8_t *pData, uint16_t dataLen)
{
    btp_custp_item_t *pCustp;
    pCustp = btp_custp_getConnNode(aclHandle);
    if (pCustp == NULL) {
        return -TLK_EPARAM;
    }
    return bth_l2cap_sendChannelData(aclHandle, pCustp->chnID, NULL, 0, pData, dataLen);
}

void btp_custp_regRecvDataCB(BtpCustpRecvDataCallback datCallback)
{
    sBtpCustpRecvDataCb = datCallback;
}

static int btp_custp_eventCB(uint8_t evtID, uint16_t psmID, uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    (void)psmID;
    btp_custp_item_t *pCustp;

    if (evtID == BTH_L2CAP_EVTID_REQUEST) {
        bth_l2cap_requestEvt_t *pEvt = (bth_l2cap_requestEvt_t *)pData;
        tlkapi_trace(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_eventCB: request");
        pCustp = btp_custp_getUsedNode(pEvt->handle);
        if (pCustp != NULL) {
            if (pCustp->state == TLK_STATE_CONNING && pCustp->chnID != 0x00) {
                bth_signal_disconnChannel(pCustp->aclHandle, pCustp->chnID);
                pCustp->timeout = gBtpConnTimeout;
                pCustp->chnID   = pEvt->scid;
                tlkapi_trace(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, " ###########btp_custp_eventCB: request reset");
                return TLK_ENONE;
            }
            return -TLK_EBUSY;
        }
        if (btp_custp_getIdleCount() == 0) {
            return -TLK_EQUOTA;
        }
    } else if (evtID == BTH_L2CAP_EVTID_CONNECT) {
        bth_l2cap_connectEvt_t *pEvt = (bth_l2cap_connectEvt_t *)pData;

        pCustp = btp_custp_getUsedNode(pEvt->handle);
        if (pCustp == NULL) {
            pCustp = btp_custp_getIdleNode();
        }
        if (pCustp == NULL) {
            tlkapi_error(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "pCustp null err");
            bth_signal_disconnChannel(pEvt->handle, pEvt->scid);
            return -TLK_EFAIL;
        }

        tlkapi_trace(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_eventCB: connect");

        pCustp->state     = TLK_STATE_CONNECT;
        pCustp->chnID     = pEvt->scid;
        pCustp->aclHandle = pEvt->handle;
        pCustp->timeout   = gBtpConnTimeout;

        btp_timer_create(&pCustp->timer, BTP_TIMER_TIMEOUT, 0, btp_custp_timer, pCustp);
        btp_timer_start(&pCustp->timer);

        btp_send_connectEvt(TLK_ENONE, BTP_PTYPE_CUSTP, BTP_USRID_NONE, pCustp->aclHandle, pCustp->chnID);

    } else if (evtID == BTH_L2CAP_EVTID_DISCONN || evtID == BTH_L2CAP_EVTID_CLOSED) {
        uint16_t scid;
        uint16_t aclHandle = 0;
        if (evtID == BTH_L2CAP_EVTID_DISCONN) {
            tlkapi_trace(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_eventCB: disconn");
        } else {
            tlkapi_trace(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_eventCB: closed");
        }
        if (evtID == BTH_L2CAP_EVTID_CLOSED) {
            bth_l2cap_closedEvt_t *pEvt = (bth_l2cap_closedEvt_t *)pData;
            aclHandle                   = pEvt->handle;
            scid                        = pEvt->scid;
        } else {
            bth_l2cap_disconnEvt_t *pEvt = (bth_l2cap_disconnEvt_t *)pData;
            aclHandle                    = pEvt->handle;
            scid                         = pEvt->scid;
        }
        pCustp = btp_custp_getUsedNode(aclHandle);
        if (pCustp != NULL && pCustp->chnID == scid) {
            uint8_t state = pCustp->state;
            btp_custp_resetNode(pCustp);
            if (state == TLK_STATE_CONNING) {
                btp_send_connectEvt(TLK_EFAIL, BTP_PTYPE_CUSTP, BTP_USRID_NONE, aclHandle, 0);
            } else {
                btp_send_disconnEvt(BTP_PTYPE_CUSTP, BTP_USRID_NONE, aclHandle);
            }
        } else {
            tlkapi_warn(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_eventCB: disconn or closed fault ...");
        }
    }

    return TLK_ENONE;
}

static void btp_custp_rdataCB(uint16_t psmID, uint16_t chnID, uint16_t handle, uint8_t *pData, uint16_t dataLen)
{
    (void)chnID;
    (void)psmID;

    btp_custp_item_t *pCustp;

    if (pData == NULL || dataLen < 6) {
        tlkapi_error(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_rdataCB: param error {psmID %d-chnID %d-handle %d-dataLen %d}", psmID, chnID, handle, dataLen);
        return;
    }

    pCustp = btp_custp_getUsedNode(handle);
    if (pCustp == NULL) {
        tlkapi_error(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_rdataCB: no node dataLen %d", dataLen);
        return;
    }

    if (sBtpCustpRecvDataCb) {
        sBtpCustpRecvDataCb(handle, pData, dataLen);
    }
}

static void btp_custp_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;

    btp_custp_item_t *pCustp = (btp_custp_item_t *)userArg;

    if (pCustp->timeout != 0) {
        pCustp->timeout--;
    }
    if (pCustp->timeout == 0 && pCustp->state == TLK_STATE_DISCING) {
        uint16_t aclHandle = pCustp->aclHandle;
        tlkapi_trace(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_timer: DISCING - timeout");
        btp_custp_resetNode(pCustp);
        btp_send_disconnEvt(BTP_PTYPE_CUSTP, BTP_USRID_NONE, aclHandle);
    } else if (pCustp->timeout == 0 && pCustp->state == TLK_STATE_CONNING) {
        uint16_t aclHandle = pCustp->aclHandle;
        tlkapi_trace(BTP_CUSTP_DBG_FLAG, BTP_CUSTP_DBG_SIGN, "btp_custp_timer: CONNING - timeout");
        btp_custp_resetNode(pCustp);
        bth_signal_destroyConnect(aclHandle, BTP_PSMID_PROP_SIGNAL);
        btp_send_connectEvt(TLK_ETIMEOUT, BTP_PTYPE_CUSTP, BTP_USRID_NONE, aclHandle, 0);
    }
    if (pCustp->state == TLK_STATE_CLOSED) {
        return;
    }
    btp_custp_procs(pCustp);

    if (pCustp->timeout != 0 || pCustp->cmdBusy != 0 || pCustp->evtBusy != 0) {
        btp_timer_start(pTimer);
    }
}

static void btp_custp_procs(btp_custp_item_t *pCustp)
{
    (void)pCustp;

    // do something
    // evt cmd deal
}
#endif
