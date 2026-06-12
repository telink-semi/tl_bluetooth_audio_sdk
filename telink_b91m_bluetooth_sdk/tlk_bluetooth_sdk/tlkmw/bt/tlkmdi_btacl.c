/********************************************************************************************************
 * @file    tlkmdi_btacl.c
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
#include "stack/bt/host/btp/avrcp/cover_art/btp_coverArt.h"
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/bt_tws/btp_tws_master.h"

int tlkmdi_btacl_deleteProf(tlkmdi_btacl_item_t *pItem, uint8_t ptype, uint8_t usrID);

static void tlkmdi_btacl_timer(TlkApiTimerHandle_t pTimer, void *userArg);
static void tlkmdi_btacl_procs(tlkmdi_btacl_item_t *pItem);
static void tlkmdi_btacl_connProfProcs(tlkmdi_btacl_item_t *pItem);
static void tlkmdi_btacl_discProfProcs(tlkmdi_btacl_item_t *pItem);
static bool tlkmdi_btacl_profileConnDeal(tlkmdi_btacl_item_t *pItem, tlkmdi_btacl_prof_t *pProf);

static int tlkmdi_btacl_requestEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_connectEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_disconnEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_encryptEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_authenCompletedEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_getAfhCompletedEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_getRemoteNameEvt(uint8_t *pData, uint16_t dataLen);


static int tlkmdi_btacl_profileServiceEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_profileChannelEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_profileRequestEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_profileConnectEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_profileDisconnEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_searchAddProtoEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmdi_btacl_linkeyNotifyEvt(uint8_t *pData, uint16_t dataLen);
static int tlkmw_btacl_btModeChangeEvt(uint8_t *pData, uint16_t dataLen);

#define TLKMDI_BTACL_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_ACL << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTACL_DBG_SIGN "[MDI]"


static tlkmdi_btacl_ctrl_t sTlkMdiBtAclCtrl;

BTH_EVT_REGISTER(BTH_EVTID_SET_SCAN_CMD_COMPLETE, tlkmdi_btSetScan_hciCmdEvt_cb);
BTH_EVT_REGISTER(BTH_EVTID_LINKKEY_NOTIFY, tlkmdi_btacl_linkeyNotifyEvt);
BTH_EVT_REGISTER(BTH_EVTID_ACLCONN_REQUEST, tlkmdi_btacl_requestEvt);
BTH_EVT_REGISTER(BTH_EVTID_ACLCONN_COMPLETE, tlkmdi_btacl_connectEvt);
BTH_EVT_REGISTER(BTH_EVTID_ENCRYPT_COMPLETE, tlkmdi_btacl_encryptEvt);
BTH_EVT_REGISTER(BTH_EVTID_ACLDISC_COMPLETE, tlkmdi_btacl_disconnEvt);
BTH_EVT_REGISTER(BTH_EVTID_AUTHEN_COMPLETE, tlkmdi_btacl_authenCompletedEvt);
BTH_EVT_REGISTER(BTH_EVTID_ACL_GETAFH_REPORT, tlkmdi_btacl_getAfhCompletedEvt);
BTH_EVT_REGISTER(BTH_EVTID_MODE_CHANGED, tlkmw_btacl_btModeChangeEvt);
BTH_EVT_REGISTER(BTH_EVTID_ACL_GETNAME_REPORT, tlkmdi_btacl_getRemoteNameEvt);

BTP_EVT_REGISTER(BTP_EVTID_PROFILE_SERVICE, tlkmdi_btacl_profileServiceEvt);
BTP_EVT_REGISTER(BTP_EVTID_PROFILE_CHANNEL, tlkmdi_btacl_profileChannelEvt);
BTP_EVT_REGISTER(BTP_EVTID_PROFILE_REQUEST, tlkmdi_btacl_profileRequestEvt);
BTP_EVT_REGISTER(BTP_EVTID_PROFILE_CONNECT, tlkmdi_btacl_profileConnectEvt);
BTP_EVT_REGISTER(BTP_EVTID_PROFILE_DISCONN, tlkmdi_btacl_profileDisconnEvt);
BTP_EVT_REGISTER(BTP_EVTID_SDP_SEARCH_ADD_PROTOCOL, tlkmdi_btacl_searchAddProtoEvt);

__attribute__((weak)) int TLKMW_BT_CONNECT_REQUEST_FUNC(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    return TLK_ENONE;
}

__attribute__((weak)) int TLKMW_BT_CONNECT_COMPLETE_FUNC(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    return TLK_ENONE;
}

__attribute__((weak)) int TLKMW_BT_DISCONNECT_COMPLETE_FUNC(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    return TLK_ENONE;
}

__attribute__((weak)) int TLKMW_BT_ENCRYPTION_COMPLETE_FUNC(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    return TLK_ENONE;
}

__attribute__((weak)) int TLKMW_BT_PROFILE_CONNECT_FUNC(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    return TLK_ENONE;
}

__attribute__((weak)) int TLKMW_BT_PROFILE_DISCONNECT_FUNC(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    return TLK_ENONE;
}

__attribute__((weak)) int TLKMW_BT_AFH_NOTIFY_FUNC(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    return TLK_ENONE;
}

__attribute__((weak)) int TLKMW_BT_SET_LOCAL_ROLE_FUNC(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < sizeof(tlkmdi_bt_set_local_role_format)) {
        return -TLK_EPARAM;
    }
    tlkmdi_bt_set_local_role_format *event     = (tlkmdi_bt_set_local_role_format *)pData;
    uint8_t                          isRequest = *((uint8_t *)event->param);
    if (isRequest) {
        return bth_acl_setInitRole(event->pBtAddr, tlkmdi_btacl_getRole(event->devClass));
    } else {
        return bth_acl_connect(event->pBtAddr, event->devClass, tlkmdi_btacl_getRole(event->devClass), event->timeout);
    }
}

static tlkmw_bt_event_func const sTlkmwBtEventFunc[TLKMW_BT_EVENT_ID_MAX] = {
    TLKMW_BT_CONNECT_REQUEST_FUNC, TLKMW_BT_CONNECT_COMPLETE_FUNC,   TLKMW_BT_DISCONNECT_COMPLETE_FUNC, TLKMW_BT_ENCRYPTION_COMPLETE_FUNC,
    TLKMW_BT_PROFILE_CONNECT_FUNC, TLKMW_BT_PROFILE_DISCONNECT_FUNC, TLKMW_BT_AFH_NOTIFY_FUNC,          TLKMW_BT_SET_LOCAL_ROLE_FUNC,
};

int tlkmw_bt_send_event(uint8_t event_id, uint8_t *pData, uint16_t dataLen)
{
    if (event_id > TLKMW_BT_EVENT_ID_MAX || sTlkmwBtEventFunc[event_id] == NULL) {
        return -TLK_ENOSUPPORT;
    }

    return sTlkmwBtEventFunc[event_id](pData, dataLen);
}

void tlkmdi_btacl_ota_status_notifyEvt(void *pData, uint8_t dataLen)
{
    (void)pData;
    (void)dataLen;
    if (dataLen < sizeof(sTlkMwNotifyEvent_t)) {
        return;
    }
    sTlkMwNotifyEvent_t *pEvent    = (sTlkMwNotifyEvent_t *)pData;
    uint16_t             aclHandle = pEvent->taskID & 0xFFFF;
    bth_acl_leaveSniff(aclHandle, 60000);
    tlkapi_array(0xffffffff, "[OTA-STATUS]", "tlkmdi_btacl_ota_status_notifyEvt", pData, dataLen);
}

/**
 * @brief       This function initializes the ACL control block and register the callback
 * @param[in]   none.
 * @return      Return TLK_ENONE is success, other value is failure.
 */
int tlkmdi_btacl_init(void)
{
    tmemset(&sTlkMdiBtAclCtrl, 0, sizeof(tlkmdi_btacl_ctrl_t));
    tlkmw_ota_register_notify_callback(TLKSYS_TASKID_HOST, tlkmdi_btacl_ota_status_notifyEvt);

    return TLK_ENONE;
}

/**
 * @brief       This function checks the acl status is connected
 * @param[in]   handle    - The ACL handle
 * @return      Return true is active, false is de-active
 */
bool tlkmdi_btacl_isActive(uint16_t handle)
{
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL || pItem->state != TLK_STATE_CONNECT) {
        return false;
    }
    return pItem->active;
}

/**
 * @brief       This function checks rfcomm whether used or not
 * @param[in]   handle    - The ACL handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isHaveRfc(uint16_t handle)
{
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL || pItem->state != TLK_STATE_CONNECT) {
        return false;
    }
    if (pItem->hfChannel != 0 || pItem->agChannel != 0 || pItem->sppChannel != 0 || pItem->pbapChannel != 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       This function checks HFP whether used or not
 * @param[in]   handle    - The ACL handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isFindHfp(uint16_t handle)
{
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL || pItem->state != TLK_STATE_CONNECT) {
        return false;
    }
    if (pItem->hfChannel != 0 || pItem->agChannel != 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       This function checks SPP whether used or not
 * @param[in]   handle    - The ACL handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isFindSpp(uint16_t handle)
{
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL || pItem->state != TLK_STATE_CONNECT) {
        return false;
    }
    if (pItem->sppChannel != 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       This function checks Pbap whether used or not
 * @param[in]   handle    - The acl handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isFindPbap(uint16_t handle)
{
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL || pItem->state != TLK_STATE_CONNECT) {
        return false;
    }
    if (pItem->pbapChannel != 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       This function checks iap whether used or not
 * @param[in]   handle    - The acl handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isFindIap(uint16_t handle)
{
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL || pItem->state != TLK_STATE_CONNECT) {
        return false;
    }
    if (pItem->iapChannel != 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       This function checks bip whether used or not
 * @param[in]   handle    - The acl handle
 * @return      Return true is used, false is unused
 */
bool tlkmdi_btacl_isFindBip(uint16_t handle)
{
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL || pItem->state != TLK_STATE_CONNECT) {
        return false;
    }
    if (pItem->bipChannel != 0 || pItem->avrcpCoverArtPsm != 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       This function cancels the acl link setup procedure
 * @param[in]   pBtAddr    - The bt address
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_cancel(uint8_t *pBtAddr)
{
    int                  ret;
    tlkmdi_btacl_item_t *pItem;

    // Check whether a local device is being connected
    pItem = tlkmdi_btacl_searchConnItem(pBtAddr);
    if (pItem != NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_cancel: failure -- connect");
        return -TLK_EBUSY;
    }

    // Check whether there are connecting nodes on the local
    pItem = tlkmdi_btacl_searchUsedItem(pBtAddr);
    if (pItem == NULL) {
        bth_acl_connectCancel(pBtAddr);
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_cancel: failure -- no device");
        return -TLK_EQUOTA;
    }

    // Cancel an ACL connecting
    ret = bth_acl_connectCancel(pBtAddr);
    if (ret != TLK_ENONE) {
        if (ret == -TLK_EHANDLE) {
            tlkmdi_btacl_resetItem(pItem);
        }
        tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_cancel: failure -- %d", ret);
        return -TLK_EBUSY;
    }

    pItem->busys |= TLKMDI_BTACL_WAIT_CANCEL;
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &pItem->timer);
    tlkapi_array(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_cancel:", pBtAddr, 6);

    return TLK_ENONE;
}

/**
 * @brief       This function creates a connection setup procedure
 * @param[in]   pBtAddr     - The bt address
 * @param[in]   devClass    - The Device type
 * @param[in]   timeout     - The timeout value. Unit:ms, Range:3000~3600000
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_connect(uint8_t *pBtAddr, uint32_t devClass, uint32_t timeout)
{
    tlkmdi_btacl_item_t *pItem;

    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_connect: 0x%x 0x%x %d", *(uint32_t *)pBtAddr, devClass, timeout);
    // Check whether the parameters are valid
    if (pBtAddr == NULL || devClass == 0) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_connect: failure -- param");
        return -TLK_EPARAM;
    }

    // Check whether a local device is being connected/connecting
    pItem = tlkmdi_btacl_searchUsedItem(pBtAddr);
    if (pItem != NULL) {
        if (pItem->state == TLK_STATE_CONNECT || pItem->state == TLK_STATE_DISCING) {
            tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_connect: failure -- connect or discing");
            return -TLK_EEXIST;
        }
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_connect: failure -- exist");
        return -TLK_EBUSY;
    }

    // Check whether there are any free nodes on the local
    pItem = tlkmdi_btacl_getIdleItem();
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_connect: failure -- no quota");
        return -TLK_EBUSY;
    }

    if (timeout < 3000) {
        timeout = 3000;
    } else if (timeout > 3600000) {
        timeout = 3600000;
    }

    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "p->initRole: initRole-%d", tlkmdi_btacl_getRole(devClass));

    // Initiating an ACL connection
    uint8_t isRequest = false;

    tlkmdi_bt_set_local_role_format set_role_event = {
        .devClass = devClass,
        .pBtAddr  = pBtAddr,
        .timeout  = timeout,
        .param    = &isRequest,
    };

    if (tlkmw_bt_send_event(TLKMW_BT_SET_LOCAL_ROLE, (uint8_t *)&set_role_event, sizeof(tlkmdi_bt_set_local_role_format)) != TLK_ENONE) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_connect: failure");
        return -TLK_EBUSY;
    }

    // Record the initiation status and start the timeout timer
    pItem->state    = TLK_STATE_CONNING;
    pItem->stage    = TLKMDI_BTACL_CONNING_STAGE_NONE;
    pItem->timeout  = 0;
    pItem->devClass = devClass;
    tmemcpy(pItem->btaddr, pBtAddr, 6);
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &pItem->timer, TLKMDI_BTACL_TIMEOUT, false, tlkmdi_btacl_timer, pItem);
    return TLK_ENONE;
}

/**
 * @brief       This function connects the acl link and set a timeout to handle acl connect timeout
 * @param[in]   btaddr     - The BT address
 * @param[in]   timeout    - The timeout value. Unit:ms
 * @return      TLK_ENONE is success, others value is failure
 */
int tlkmdi_btacl_connectEx(uint8_t btaddr[6], uint32_t timeout)
{
    tlkmdi_btacl_item_t *pBtAcl;
    int                  ret = TLK_ENONE;
    (void)ret;
    bool    bt_recon_busy  = 0;
    uint8_t bt_scan_enable = 0;
#if (TLK_MW_BTREC_ENABLE)
    bt_recon_busy = tlkmdi_btRecon_isInBusy();
#endif
    bt_scan_enable = (tlkmdi_btGetScan_state() == TLKMDI_BTSCAN_MODE_BOTH_DISABLE) ? 0 : 1;

    if (timeout < 15000) {
        timeout = 15000;
    }
    pBtAcl = tlkmdi_btacl_searchUsedItem(btaddr);
    if (pBtAcl != NULL) {
        if (pBtAcl->state == TLK_STATE_CONNECT) {
            return TLK_ENONE;
        }
        return -TLK_EBUSY;
    }
    if (tlkmdi_btacl_getIdleCount() == 0) {
        return -TLK_EQUOTA;
    }
    if (bt_recon_busy || bt_scan_enable) {
        if (bt_scan_enable) {
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
        }
#if (TLK_MW_BTREC_ENABLE)
        if (bt_recon_busy) {
            tlkmdi_btRecon_close();
        }
#endif
    }
    uint32_t devClass;
    int      sqlRet = tlkmdi_tinySql_getPairingDeviceByAddr(btaddr, &devClass, NULL, NULL);
    if (sqlRet == TLK_ENONE) {
        return tlkmdi_btacl_connect(btaddr, devClass, timeout);
    }
#if (TLK_MW_BTINQ_ENABLE)
    tlkmdi_btinq_item_t *pBtInq;
    pBtInq = tlkmdi_btinq_getUsedItem(btaddr);
    if (pBtInq) {
        ret = tlkmdi_btacl_connect(btaddr, pBtInq->devClass, timeout);
        return ret;
    }
#endif
    return -TLK_EBUSY;
}

/**
 * @brief       This function disconnects the connection
 * @param[in]   handle     - The ACL handle
 * @param[in]   reason     - The disconnect reason
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_disconn(uint16_t handle, uint8_t reason)
{
    int                  ret;
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getUsedItem(handle);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_disconn: failure -- exist  0x%x", handle);
        return -TLK_EHANDLE;
    }

    bth_acl_leaveSniff(handle, 60000);
    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_disconn: 0x%x %d", handle, reason);

    if (pItem->state == TLK_STATE_OPENED) {
        tlkmdi_btacl_resetItem(pItem);
    } else if (pItem->state == TLK_STATE_CONNING) {
        ret = bth_acl_disconn(handle, reason);
        if (ret == TLK_ENONE || ret != -TLK_EBUSY) {
            tlkmdi_btacl_resetItem(pItem);
        } else {
            pItem->state = TLK_STATE_DISCING;
            pItem->stage = TLKMDI_BTACL_DISCING_STAGE_WAIT;
        }
    } else if (pItem->state == TLK_STATE_CONNECT) {
        btp_sdpclt_disconn(handle);
        btp_sdpsrv_disconn(handle);

        pItem->state   = TLK_STATE_DISCING;
        pItem->busys   = TLKMDI_BTACL_BUSY_DISC_PROF | TLKMDI_BTACL_BUSY_DISC_RFC;
        pItem->timeout = TLKMDI_BTACL_DISC_DEF_TIMEOUT;
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &pItem->timer);
    } else {
        pItem->state = TLK_STATE_DISCING;
    }

    if (pItem->state == TLK_STATE_DISCING) {
        return -TLK_EBUSY;
    }
    return TLK_ENONE;
}

/**
 * @brief       This function disconnects the acl link which specify by bt address
 * @param[in]   btaddr    - The Bt address
 * @return      TLK_ENONE is success, others value is failure
 */
int tlkmdi_btacl_disconnByAddr(uint8_t btaddr[6])
{
    uint8_t              invalidMac[6] = {0};
    tlkmdi_btacl_item_t *pBtAcl;

    if (tmemcmp(btaddr, invalidMac, 6) == 0) {
        pBtAcl = tlkmdi_btacl_getBusyItem();
    } else {
        pBtAcl = tlkmdi_btacl_searchUsedItem(btaddr);
    }
    if (pBtAcl == NULL) {
        return TLK_ENONE;
    }
    if (pBtAcl->handle == 0) {
        return tlkmdi_btacl_cancel(pBtAcl->btaddr);
    } else {
        return tlkmdi_btacl_disconn(pBtAcl->handle, 0x13);
    }
}

/**
 * @brief       This function adds a new profile to a acl link
 * @param[in]   handle     - The ACL handle
 * @param[in]   ptype      - The profile type
 * @param[in]   usrID      - The user id
 * @param[in]   delayMs    - The delay time
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_appendProf(uint16_t handle, uint8_t ptype, uint8_t usrID, uint16_t delayMs)
{
    uint8_t              index;
    tlkmdi_btacl_prof_t *pProf;
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL || ptype == 0) {
        return -TLK_EPARAM;
    }
    if (pItem->state == TLK_STATE_DISCING) {
        return -TLK_ESTATUS;
    }
    if ((pItem->connFlag & (1 << ptype)) != 0) {
        return -TLK_EALREADY;
    }

    for (index = 0; index < TLKMDI_BTACL_PROF_NUMB; index++) {
        pProf = &pItem->prof[index];
        if (pProf->state != TLK_STATE_CLOSED && pProf->ptype == ptype && (pProf->usrID == 0 || pProf->usrID == usrID)) {
            break;
        }
    }
    if (index != TLKMDI_BTACL_PROF_NUMB) {
        return -TLK_EREPEAT;
    }
    for (index = 0; index < TLKMDI_BTACL_PROF_NUMB; index++) {
        if (pItem->prof[index].state == TLK_STATE_CLOSED) {
            break;
        }
    }
    if (index == TLKMDI_BTACL_PROF_NUMB) {
        return -TLK_EQUOTA;
    }

    pProf        = &pItem->prof[index];
    pProf->state = TLK_STATE_OPENED;
    pProf->ptype = ptype;
    pProf->usrID = usrID;
    pProf->cwait = TLKMDI_BTACL_PROFILE_CONN_TIMEOUT;
    pProf->delay = (delayMs + TLKMDI_BTACL_TIMEOUT_MS - 1) / TLKMDI_BTACL_TIMEOUT_MS;

    pItem->busys |= TLKMDI_BTACL_BUSY_CONN_PROF;
    pItem->timeout = TLKMDI_BTACL_CONN_DEF_TIMEOUT;
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &pItem->timer);

    return TLK_ENONE;
}

/**
 * @brief       This function removes a profile from the acl link
 * @param[in]   handle    - The ACL handle
 * @param[in]   ptype     - The profile type
 * @param[in]   usrID     - The user id
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_removeProf(uint16_t handle, uint8_t ptype, uint8_t usrID)
{
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL || ptype == 0) {
        return -TLK_EPARAM;
    }
    if (pItem->state == TLK_STATE_DISCING) {
        return -TLK_ESTATUS;
    }
    return tlkmdi_btacl_deleteProf(pItem, ptype, usrID);
}

/**
 * @brief       This function deletes a profile from the acl link
 * @param[in]   pItem - a pointer to the acl link item
 * @param[in]   ptype - profile type
 * @param[in]   usrID - user ID
*/
int tlkmdi_btacl_deleteProf(tlkmdi_btacl_item_t *pItem, uint8_t ptype, uint8_t usrID)
{
    uint8_t              index;
    tlkmdi_btacl_prof_t *pProf;
    for (index = 0; index < TLKMDI_BTACL_PROF_NUMB; index++) {
        pProf = &pItem->prof[index];
        if (pProf->state != TLK_STATE_CLOSED && pProf->ptype == ptype && (pProf->usrID == 0 || pProf->usrID == usrID)) {
            break;
        }
    }
    if (index == TLKMDI_BTACL_PROF_NUMB) {
        return -TLK_ENOOBJECT;
    }
    tmemset(&pItem->prof[index], 0, sizeof(tlkmdi_btacl_prof_t));
    return TLK_ENONE;
}

/**
 * @brief       This function gets the role of acl link
 * @param[in]   devClass    - The device class
 * @return      Return TLK_ENONE is success, other value is failure
 */
int tlkmdi_btacl_getRole(uint32_t devClass)
{
    uint8_t devType = bth_devClassToDevType(devClass);
    if (devType == BTH_REMOTE_DTYPE_HEADSET) {
        return BTH_ROLE_MASTER;
    } else if ((devType == BTH_REMOTE_DTYPE_COMPUTER) || (devType == BTH_REMOTE_DTYPE_PHONE)) {
        return BTH_ROLE_SLAVE;
    } else {
        tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_getRole:other - %d", devType);
#ifdef TLKCFG_ROLE_PRECEDENCE_ROLE
        return TLKCFG_ROLE_PRECEDENCE_ROLE
#else
        return BTH_ROLE_SLAVE;
#endif
    }
}

/**
 * @brief       This function handles the ACL connection request event
 * @param[in]   pData    - the event data containing peer device information
 * @param[in]   dataLen  - the length of the event data
 * @return      TLK_ENONE means success, TLK_EBUSY means device busy, TLK_EFAIL means connection request failed
 */
static int tlkmdi_btacl_requestEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    tlkmdi_btacl_item_t     *pItem;
    bth_aclConnRequestEvt_t *pEvt;

    pEvt = (bth_aclConnRequestEvt_t *)pData;
    if (tlkmdi_btacl_searchUsedItem(pEvt->peerMac) != NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_requestEvt: failure -- busy");
        return -TLK_EBUSY;
    }

    pItem = tlkmdi_btacl_getIdleItem();
    if (pItem == NULL) {
        tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_requestEvt: failure -- no quota");
        return -TLK_EBUSY;
    }
    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_requestEvt: {devClass - 0x%x}", pEvt->devClass);

    tlkmdi_bt_conn_request_evt_format event = {
        .dev_class = pEvt->devClass,
        .pBtAddr   = pEvt->peerMac,
    };

    if (tlkmw_bt_send_event(TLKMW_BT_CONNECT_REQUEST, (uint8_t *)&event, sizeof(tlkmdi_bt_conn_request_evt_format)) != TLK_ENONE) {
        return -TLK_EFAIL;
    }

    uint8_t isRequest = true;

    tlkmdi_bt_set_local_role_format set_role_event = {
        .devClass = pEvt->devClass,
        .pBtAddr  = pEvt->peerMac,
        .param    = &isRequest,
    };

    tlkmw_bt_send_event(TLKMW_BT_SET_LOCAL_ROLE, (uint8_t *)&set_role_event, sizeof(tlkmdi_bt_set_local_role_format));

    // Record the initiation status and start the timeout timer
    pItem->state = TLK_STATE_CONNING;
    pItem->stage = TLKMDI_BTACL_CONNING_STAGE_NONE;
    pItem->attrs |= TLKMDI_BTACL_ATTR_REQUEST;
    pItem->timeout  = TLKMDI_BTACL_CONN_DEF_TIMEOUT;
    pItem->devClass = pEvt->devClass;
    //
    tmemcpy(pItem->btaddr, pEvt->peerMac, 6);
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &pItem->timer, TLKMDI_BTACL_TIMEOUT, false, tlkmdi_btacl_timer, pItem);
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &pItem->timer);
#if TLKMW_BT_1_TO_2_FORWARD_EN
    uint8_t pincode[4] = {'1', '1', '2', '3'};
    bth_acl_setPinCode(pEvt->peerMac, pincode, 4);
#endif
    return TLK_ENONE;
}

/**
 * @brief       Process ACL connection complete event
 * @param[in]   pData: Pointer to the event data
 * @param[in]   dataLen: Length of the event data
 * @return      TLK_ENONE means success, negative value means failure
 */
static int tlkmdi_btacl_connectEvt(uint8_t *pData, uint16_t dataLen)
{
    tlkmdi_btacl_item_t      *pItem;
    bth_aclConnCompleteEvt_t *pEvt;
    uint8_t                   dtype;
    uint8_t                   hfp_ChId = 0;

    pEvt = (bth_aclConnCompleteEvt_t *)pData;

    pItem = tlkmdi_btacl_searchUsedItem(pEvt->peerMac);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_connectEvt: failure -- no node");
        if (pEvt->status == TLK_ETIMEOUT) {
            tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_connectEvt: failure -- connect timeout");
        }
        if (pEvt->status == TLK_ENONE) {
            bth_acl_disconn(pEvt->handle, 0x00);
        }
        return -TLK_EFAIL;
    }

    tlkmdi_tinySql_getPairingDeviceRfcChid(pItem->btaddr, &hfp_ChId, TLKMDI_BT_RFC_CHID_HFP);
    dtype = bth_devClassToDevType(pItem->devClass);

    tlkmdi_bt_connect_evt_format event = {
        .aclHandle   = pEvt->handle,
        .dtype       = dtype,
        .hfp_channel = hfp_ChId,
        .pBtAddr     = pEvt->peerMac,
        .status      = pEvt->status,
    };

    tlkmw_bt_send_event(TLKMW_BT_CONNECT_COMPLETE, (uint8_t *)&event, sizeof(tlkmdi_bt_connect_evt_format));

    if (pEvt->status != TLK_ENONE) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_connectEvt: failure -- %d", pEvt->status);

        tlkmdi_btacl_resetItem(pItem);
        return TLK_ENONE;
    }

    if (pItem->state == TLK_STATE_DISCING) {
        bth_acl_disconn(pItem->handle, 0x00);
        return TLK_ENONE;
    }

    tlkapi_array(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_connectEvt", pData, dataLen);

    pItem->attrs |= TLKMDI_BTACL_ATTR_CONNECT;
    pItem->handle  = pEvt->handle;
    pItem->active  = pEvt->active == 0 ? 0 : 1;
    pItem->timeout = TLKMDI_BTACL_CONN_DEF_TIMEOUT;
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &pItem->timer);

    pItem->state   = TLK_STATE_CONNECT;
    pItem->timeout = TLKMDI_BTACL_CONN_DEF_TIMEOUT;

    pItem->busys &= ~TLKMDI_BTACL_WAIT_CANCEL;
    pItem->busys |= TLKMDI_BTACL_WAIT_DISC_ACL;
    pItem->idleTime = TLKMDI_BTACL_IDLE_DEF_TIMEOUT;

#if (TLKBTP_CFG_HFPAG_ENABLE)
    if (dtype == BTH_REMOTE_DTYPE_HEADSET) {
        uint8_t volume = TLKBTP_HFP_DEFAULT_VOL;

        tlkmdi_tinySql_getPairingDeviceVolume(pItem->btaddr, false, &volume, NULL);
        btp_hfpag_setSpkVolumeByHandle(pEvt->handle, volume);
    }
#endif

#if (TLKBTP_CFG_HFPHF_ENABLE)
    if (dtype != BTH_REMOTE_DTYPE_HEADSET) {
        uint8_t isIos  = 0;
        uint8_t volume = TLKBTP_HFP_DEFAULT_VOL;
        tlkmdi_tinySql_getPairingDeviceVolume(pEvt->peerMac, false, &volume, &isIos);
        btp_hfphf_setDefaultSpkVolumeByHandle(pEvt->handle, volume);
    }
#endif
    return TLK_ENONE;
}

/**
 * @brief Handle Bluetooth encryption complete event
 * @param[in] pData Pointer to the encryption complete event data
 * @param[in] dataLen Length of the event data
 * @return    TLK_ENONE if success, others if failed
 */
static int tlkmdi_btacl_encryptEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    tlkmdi_btacl_item_t      *pItem;
    bth_encryptCompleteEvt_t *pEvt;
    uint8_t                   dtype;
    uint8_t                   hfp_ChId = 0;

    pEvt = (bth_encryptCompleteEvt_t *)pData;

    pItem = tlkmdi_btacl_getUsedItem(pEvt->handle);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_encryptEvt: failure -- no node");
        bth_acl_disconn(pEvt->handle, 0x00);
        return -TLK_EFAIL;
    }

    dtype = bth_devClassToDevType(pItem->devClass);

    tlkmdi_tinySql_getPairingDeviceRfcChid(pItem->btaddr, &hfp_ChId, TLKMDI_BT_RFC_CHID_HFP);

    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_encryptEvt: status[%d], enable[%d],hfp_ChId[0x%x]dtype[0x%x]", pEvt->status, pEvt->enable, hfp_ChId,
                 dtype);

    if (hfp_ChId) {
        if (dtype == BTH_REMOTE_DTYPE_PHONE) {
            pItem->agChannel = hfp_ChId;
        } else if (dtype == BTH_REMOTE_DTYPE_HEADSET) {
            pItem->hfChannel = hfp_ChId;
        } else {
            hfp_ChId = 0;
        }
        tlkmdi_tinySql_getPairingDeviceRfcChid(pItem->btaddr, &pItem->sppChannel, TLKMDI_BT_RFC_CHID_SPP);
        tlkmdi_tinySql_getPairingDeviceRfcChid(pItem->btaddr, &pItem->iapChannel, TLKMDI_BT_RFC_CHID_IAP);
        tlkmdi_tinySql_getPairingDeviceRfcChid(pItem->btaddr, &pItem->pbapChannel, TLKMDI_BT_RFC_CHID_PBAP);
        tlkmdi_tinySql_getPairingDeviceRfcChid(pItem->btaddr, &pItem->bipChannel, TLKMDI_BT_RFC_CHID_BIP);
        tlkmdi_tinySql_getPairingDeviceRfcChid(pItem->btaddr, &pItem->avrcpCoverArtPsm, TLKMDI_BT_AVRCP_ArtPsm);
    } else if (dtype == BTH_REMOTE_DTYPE_PHONE) {
        hfp_ChId = pItem->agChannel;
    } else if (dtype == BTH_REMOTE_DTYPE_HEADSET) {
        hfp_ChId = pItem->hfChannel;
    }

    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "hfp_ChId[0x%x],sppChannel[0x%x],iapChannel[0x%x],pbapChannel[0x%x], bipChannel[0x%x], avrcpCoverArtPsm[0x%x]",
                 hfp_ChId, pItem->sppChannel, pItem->iapChannel, pItem->pbapChannel, pItem->bipChannel, pItem->avrcpCoverArtPsm);

    tlkmdi_bt_encryption_evt_format event = {
        .aclHandle   = pEvt->handle,
        .enable      = pEvt->enable,
        .dtype       = dtype,
        .hfp_channel = hfp_ChId,
        .pBtAddr     = pItem->btaddr,
        .status      = pEvt->status,
    };

    tlkmw_bt_send_event(TLKMW_BT_ENCRYPTION_COMPLETE, (uint8_t *)&event, sizeof(tlkmdi_bt_encryption_evt_format));

    if (pEvt->status != TLK_ENONE) {
        uint8_t btaddr[6];
        tmemcpy(btaddr, pItem->btaddr, 6);
        tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_encryptEvt: failure -- %d", pEvt->status);
        pItem->handle = 0; // Important
        tlkmdi_btacl_resetItem(pItem);
        bth_acl_disconn(pEvt->handle, 0x00);

#if (DEBUG_BT_SNIFF_ENABLE) //TODO: temp close sniff in tws ,will open when controller is ok.
        bth_acl_enableSniff(pEvt->handle, true);
#else
        bth_acl_enableSniff(pEvt->handle, false);
#endif
        return TLK_ENONE;
    }
    if (pItem->state == TLK_STATE_DISCING) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_encryptEvt: acl is discing");
        bth_acl_disconn(pItem->handle, 0x00);
        return TLK_ENONE;
    }

    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &pItem->timer);

#if (DEBUG_BT_SNIFF_ENABLE) //TODO: temp close sniff in tws ,will open when controller is ok.
    bth_acl_enableSniff(pEvt->handle, true);
#else
    bth_acl_enableSniff(pEvt->handle, false);
#endif
    return TLK_ENONE;
}

/**
 * @brief       Handles the disconnection event for Bluetooth ACL.
 * @param[in]   pData    - Pointer to the data buffer containing event information.
 * @param[in]   dataLen  - Length of the data buffer. Unused in this function.
 * @return      TLK_ENONE on success, -TLK_EFAIL if no matching node is found.
 */
static int tlkmdi_btacl_disconnEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    tlkmdi_btacl_item_t      *pItem;
    bth_aclDiscCompleteEvt_t *pEvt;

    pEvt  = (bth_aclDiscCompleteEvt_t *)pData;
    pItem = tlkmdi_btacl_searchUsedItem(pEvt->peerMac);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_disconnEvt: failure -- no node");
        return -TLK_EFAIL;
    }

    if (pItem->profIsConn && (pItem->connFlag & (~(BTP_PFLAG_SDP | BTP_PFLAG_RFC))) == 0 && pEvt->reason == 0x08) {
        pEvt->reason = 0x13;
    }

    uint8_t dtype;
    dtype = bth_devClassToDevType(pItem->devClass); // get peer devClass

    btp_destroy(pItem->handle);
    bth_destroy(pItem->handle);

    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_disconnEvt: success-0x%x addr-%x", pEvt->handle, pEvt->peerMac);

    pItem->handle = 0; // Important
    tlkmdi_btacl_resetItem(pItem);

    tlkmdi_bt_disconnect_evt_format event = {
        .aclHandle = pEvt->handle,
        .dtype     = dtype,
        .pBtAddr   = pEvt->peerMac,
        .reason    = pEvt->reason,
    };

    tlkmw_bt_send_event(TLKMW_BT_DISCONNECT_COMPLETE, (uint8_t *)&event, sizeof(tlkmdi_bt_disconnect_evt_format));

    tlkmw_host_linkmgr_delLink(TLKMDI_HOST_LINK_TYPE_BT, pEvt->handle);
    return TLK_ENONE;
}

/**
 * @brief       Handles the authentication completed event.
 * @param[in]   pData    - Pointer to the event data.
 * @param[in]   dataLen  - Length of the event data (unused).
 * @return      TLK_ENONE - Always returns success.
 */
static int tlkmdi_btacl_authenCompletedEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    bth_authenCompleteEvt_t *evt = (bth_authenCompleteEvt_t *)pData;
    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_authenCompletedEvt evt->status=0x%x", evt->status);
    if (evt->status == BTH_HCI_ERROR_PIN_OR_KEY_MISSING || evt->status == BTH_HCI_ERROR_AUTH_FAILURE) {
        bth_acl_handle_t *pHandle = bth_handle_getUsedAcl(evt->handle);
        tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_authenCompletedEvt pHandle=0x%x", pHandle);

        if (pHandle != NULL) {
            tlkmdi_tinySql_deletePairingDevice(pHandle->btaddr);
#if (TLK_MW_BTREC_ENABLE)
            if (tlkmdi_btRecon_isInBusy()) {
                uint8_t *pPageAddr = tlkmdi_btRecon_getPageAddr();
                tlkapi_array(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_authenCompletedEvt pPageAddr", pPageAddr, 6);
                tlkapi_array(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_authenCompletedEvt Handle->btaddr", pHandle->btaddr, 6);

                if (pPageAddr != NULL && tmemcmp(pPageAddr, pHandle->btaddr, 6) == 0) {
                    tlkmdi_btRecon_close(); /* wait next retry*/
                }
            }
#endif
        }
    }
    return TLK_ENONE;
}

/**
 * @brief       Handles the get AFH completed event.
 * @param[in]   pData    - Pointer to the event data.
 * @param[in]   dataLen  - Length of the event data (unused).
 * @return      TLK_ENONE - Always returns success.
 */
static int tlkmdi_btacl_getAfhCompletedEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    bth_aclGetAfhReportEvt_t *evt = (bth_aclGetAfhReportEvt_t *)pData;
    if (evt->status == BTH_HCI_ERROR_NONE) {
        tlkapi_array(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "AFH_CHN-MAP: ", evt->map, 10);

        tlkmdi_bt_afh_notify_evt_format event = {.aclHandle = evt->conhdl, .pData = evt->map, .dataLen = sizeof(evt->map)};

        tlkmw_bt_send_event(TLKMW_BT_AFH_NOTIFY, (uint8_t *)&event, sizeof(tlkmdi_bt_afh_notify_evt_format));
    }

    return TLK_ENONE;
}

/**
 * @brief       Handles the bluetooth mode change event.
 * @param[in]   pData    - Pointer to the event data.
 * @param[in]   dataLen  - Length of the event data (unused).
 * @return      TLK_ENONE - Always returns success.
 */
static int tlkmw_btacl_btModeChangeEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint16_t handle;
    uint8_t  status;
    uint8_t  curMode;

    handle  = pData[0] | (pData[1] << 8);
    status  = pData[2];
    curMode = pData[3];

    tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL) {
        return -TLK_EEXIST;
    }
    if (status == TLK_ENONE) {
        if (curMode == BTH_LM_ACTIVE_MODE) {
            if (pItem->audioBusy) {
                bth_acl_clsLinkPolicyBit(handle, HCI_LP_ENABLE_SNIFF_MODE_MASK);
            }
        } else if (curMode == BTH_LM_SNIFF_MODE) {
        } else {
        }
    }
    return TLK_ENONE;
}

/**
 * @brief       Handles the bluetooth profile service event.
 * @param[in]   pData    - Pointer to the event data.
 * @param[in]   dataLen  - Length of the event data (unused).
 * @return      TLK_ENONE - Always returns success.
 */
static int tlkmdi_btacl_profileServiceEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    (void)pData;
    // Not Support.
    return TLK_ENONE;
}

/**
 * @brief       Reports the bluetooth service channel searched.
 * @param[in]   pData    - Pointer to the event data.
 * @param[in]   dataLen  - Length of the event data (unused).
 * @return      TLK_ENONE - Always returns success.
 */
static int tlkmdi_btacl_profileChannelEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_channelEvt_t    *pEvt;
    tlkmdi_btacl_item_t *pItem;

    pEvt  = (btp_channelEvt_t *)pData;
    pItem = tlkmdi_btacl_getUsedItem(pEvt->handle);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileChannelEvt: error - no node");
        return TLK_ENONE;
    }

    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileChannelEvt:{handle-0x%x,service-0x%x,channel-0x%x}", pEvt->handle, pEvt->service,
                 pEvt->channel);

    if (pEvt->service == BTP_SDP_SRVCLASS_ID_HANDSFREE) {
        pItem->hfChannel = pEvt->channel;
    } else if (pEvt->service == BTP_SDP_SRVCLASS_ID_HANDSFREE_AGW) {
        pItem->agChannel = pEvt->channel;
        if (pEvt->channel != 0) {
            if (pItem->active == false) { // refer to PC_Comp
                tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pEvt->channel, TLKMDI_BT_RFC_CHID_HFP);
            }
            btp_tws_set_rfcommChnID(pEvt->channel, true); // save peer hfp channel, sync to slave-headset
        }
    } else if (pEvt->service == BTP_SDP_SRVCLASS_ID_SERIAL_PORT) {
        pItem->sppChannel = pEvt->channel;
        if (pEvt->channel != 0) {
            btp_tws_set_rfcommChnID(pEvt->channel, false); // save peer spp channel, sync to slave-headset
        }
        tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pEvt->channel, TLKMDI_BT_RFC_CHID_SPP);
    } else if (pEvt->service == BTP_SDP_SRVCLASS_ID_IAP2_TEMP) {
        pItem->iapChannel = pEvt->channel;
        tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pEvt->channel, TLKMDI_BT_RFC_CHID_IAP);
    } else if (pEvt->service == BTP_SDP_SRVCLASS_ID_PBAP_PSE) {
        pItem->pbapChannel = pEvt->channel;
        tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pEvt->channel, TLKMDI_BT_RFC_CHID_PBAP);
    } else if (pEvt->service == BTP_SDP_SRVCLASS_ID_IMAGING_RESPONDER) {
        pItem->bipChannel = pEvt->channel;
        tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pEvt->channel, TLKMDI_BT_RFC_CHID_BIP);
    }
    return TLK_ENONE;
}

/**
 * @brief       Handles the bluetooth profile request event.
 * @param[in]   pData    - Pointer to the event data.
 * @param[in]   dataLen  - Length of the event data (unused).
 * @return      TLK_ENONE - Always returns success.
 */
static int tlkmdi_btacl_profileRequestEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_requestEvt_t    *pEvt;
    tlkmdi_btacl_item_t *pItem;

    pEvt  = (btp_requestEvt_t *)pData;
    pItem = tlkmdi_btacl_getUsedItem(pEvt->handle);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileRequestEvt: error - no node");
        return -TLK_EFAIL;
    }
    if (pItem->state == TLK_STATE_DISCING) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileRequestEvt: reject - discing");
        return -TLK_EFAIL;
    }

    return TLK_ENONE;
}

/**
 * @brief       Handles the bluetooth profile connect event.
 * @param[in]   pData    - Pointer to the event data.
 * @param[in]   dataLen  - Length of the event data (unused).
 * @return      TLK_ENONE - Always returns success.
 */
static int tlkmdi_btacl_profileConnectEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_connectEvt_t    *pEvt;
    tlkmdi_btacl_item_t *pItem;
    bool                 isFirstProfile = false;

    pEvt = (btp_connectEvt_t *)pData;

    pItem = tlkmdi_btacl_getUsedItem(pEvt->handle);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileConnectEvt: error - no node");
        return TLK_ENONE;
    }
    if (pItem->state == TLK_STATE_DISCING) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileConnectEvt: reject - discing");
        btp_module_disconn(pItem->handle, pEvt->ptype, pEvt->usrID);
        return TLK_ENONE;
    }

    if (pItem->connFlag & (1 << pEvt->ptype) && pEvt->ptype == BTP_PTYPE_HFP) {
        return TLK_ENONE;
    }

    tlkmdi_btacl_deleteProf(pItem, pEvt->ptype, pEvt->usrID);
    if (pEvt->status != TLK_ENONE) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileConnectEvt: fail - ptype-%d,usrID-%d,handle-0x%x,status-%d ", pEvt->ptype, pEvt->usrID,
                     pEvt->handle, pEvt->status);
    } else {
        tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileConnectEvt: {ptype-%d,usrID-%d,handle-0x%x,status-%d} - ", pEvt->ptype, pEvt->usrID,
                     pEvt->handle, pEvt->status);
        pItem->connFlag |= (1 << pEvt->ptype);
        if (pEvt->ptype != BTP_PTYPE_SDP && pEvt->ptype != BTP_PTYPE_RFC) {
            pItem->busys &= ~TLKMDI_BTACL_WAIT_DISC_ACL;
            if (!pItem->profIsConn) {
                pItem->profIsConn = true;
                isFirstProfile    = true;
            }
        }
        if (pEvt->ptype == BTP_PTYPE_A2DP) {
#if (TLKBTP_CFG_A2DP_ENABLE)
            tlkmdi_bta2dp_connectEvt(pEvt->handle, pEvt->usrID);
#endif
        }
#if (TLKBTP_CFG_AVRCP_ENABLE)
        if (pEvt->ptype == BTP_PTYPE_AVRCP) {
            uint8_t isIos  = 0;
            uint8_t volume = TLKBTP_A2DP_DEFAULT_VOL;
            tlkmdi_tinySql_getPairingDeviceVolume(pItem->btaddr, true, &volume, &isIos);
            btp_avrcp_setDefaultVolume(pItem->btaddr, volume);
        }
#endif
#if (TLKBTP_CFG_HFPHF_ENABLE)
        if (pEvt->ptype == BTP_PTYPE_HFP && pEvt->usrID == BTP_USRID_CLIENT) {
            uint8_t isIos  = 0;
            uint8_t volume = TLKBTP_HFP_DEFAULT_VOL;

            tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "!!! set hfp default vol:0x%x,addr:0x%x,pEvt->handle:0x%x", volume, *(uint32_t *)pItem->btaddr,
                         pEvt->handle);

            tlkmdi_tinySql_getPairingDeviceVolume(pItem->btaddr, false, &volume, &isIos);
            btp_hfphf_setDefaultSpkVolumeByHandle(pEvt->handle, volume);
        }
#endif

#if (TLKBTP_CFG_HFPAG_ENABLE)
        if (pEvt->ptype == BTP_PTYPE_HFP && pEvt->usrID == BTP_USRID_SERVER) {
            uint8_t volume = TLKBTP_HFP_DEFAULT_VOL;

            tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "!!! set hfpag default vol:0x%x,addr:0x%x,pEvt->handle:0x%x", volume, *(uint32_t *)pItem->btaddr,
                         pEvt->handle);

            tlkmdi_tinySql_getPairingDeviceVolume(pItem->btaddr, false, &volume, NULL);
            btp_hfpag_setSpkVolumeByHandle(pEvt->handle, volume);
        }
#endif

#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
        if (pEvt->ptype == BTP_PTYPE_AVRCP_BROWSING) {
            btp_browsing_sendGetFolderItemsCmd(pEvt->handle, 0x00, 0, 100, 0xFF, NULL);
        }
#endif
#if (TLKBTP_CFG_AVRC_COVER_ART_ENABLE)
        if (pEvt->ptype == BTP_PTYPE_AVRCP) {
            if (pItem->avrcpCoverArtPsm != 0) {
                btp_coverArt_connect(pEvt->handle, BTP_USRID_CLIENT, pItem->avrcpCoverArtPsm, true);
            }
        }
#endif
        if (pEvt->ptype == BTP_PTYPE_ATT) {
            tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, 0x01, TLKMDI_BT_RFC_GATT_SUPPORT);
        }
    }

    tlkmdi_bt_profile_connect_evt_format event = {
        .aclHandle     = pEvt->handle,
        .status        = pEvt->status,
        .ptype         = pEvt->ptype,
        .usrID         = pEvt->usrID,
        .pBtAddr       = pItem->btaddr,
        .is_first_prof = isFirstProfile,
    };

    tlkmw_bt_send_event(TLKMW_BT_PROFILE_CONNECT, (uint8_t *)&event, sizeof(tlkmdi_bt_profile_connect_evt_format));

    if (isFirstProfile) {
        tlkmw_host_linkmgr_addLink(TLKMDI_HOST_LINK_TYPE_BT, pEvt->handle);
    }
    return TLK_ENONE;
}

/**
 * @brief       Handles the bluetooth profile disconnect event.
 * @param[in]   pData    - Pointer to the event data.
 * @param[in]   dataLen  - Length of the event data (unused).
 * @return      TLK_ENONE - Always returns success.
 */
static int tlkmdi_btacl_profileDisconnEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint16_t             connFlag;
    btp_disconnEvt_t    *pEvt;
    tlkmdi_btacl_item_t *pItem;

    pEvt  = (btp_disconnEvt_t *)pData;
    pItem = tlkmdi_btacl_getUsedItem(pEvt->handle);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileDisconnEvt: error - no node");
        return TLK_ENONE;
    }

    tlkmdi_btacl_deleteProf(pItem, pEvt->ptype, pEvt->usrID);
    pItem->connFlag &= ~(1 << pEvt->ptype);
    if (pItem->state == TLK_STATE_DISCING) {
#if (TLKBTP_CFG_RFC_ENABLE) // btble_a2dp_to_bis prj clear compile error.
        if ((pItem->connFlag & BTP_PFLAG_RFC) != 0 && (pItem->connFlag & BTP_PFLAG_RFCOMM_MASK) == 0) {
            btp_rfcomm_disconn(pItem->handle);
        }
#endif
    }

    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileDisconnEvt: {ptype-%d,usrID-%d,handle-0x%x,connFlag-0x%x}", pEvt->ptype, pEvt->usrID,
                 pEvt->handle, pItem->connFlag);

    tlkmdi_bt_profile_disconnect_evt_format event = {
        .aclHandle = pItem->handle,
        .pBtAddr   = pItem->btaddr,
        .ptype     = pEvt->ptype,
        .reason    = 0,
        .usrID     = pEvt->usrID,
    };

    tlkmw_bt_send_event(TLKMW_BT_PROFILE_DISCONNECT, (uint8_t *)&event, sizeof(tlkmdi_bt_profile_disconnect_evt_format));

    connFlag = pItem->connFlag;
    connFlag &= ~BTP_PFLAG_RFC;
    connFlag &= ~BTP_PFLAG_SDP;
    if (connFlag == 0) {
        if (pItem->state == TLK_STATE_DISCING) {
            pItem->busys |= TLKMDI_BTACL_BUSY_DISC_ACL;
        }
        pItem->busys |= TLKMDI_BTACL_WAIT_DISC_ACL;
        if (pEvt->ptype != BTP_PTYPE_SDP && pEvt->ptype != BTP_PTYPE_RFC) {
            pItem->idleTime = TLKMDI_BTACL_IDLE_DEF_TIMEOUT1;
        } else {
            pItem->idleTime = TLKMDI_BTACL_IDLE_DEF_TIMEOUT;
        }
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &pItem->timer);
    }

    return TLK_ENONE;
}

/**
 * @brief       Handles the bluetooth link key notify event.
 * @param[in]   pData    - Pointer to the event data.
 * @param[in]   dataLen  - Length of the event data (unused).
 * @return      TLK_ENONE - Always returns success.
 */
static int tlkmdi_btacl_linkeyNotifyEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    bth_linkKeyNotifyEvt_t *evt = (bth_linkKeyNotifyEvt_t *)pData;
    tlkmdi_tinySql_updatePairingDevice(evt->peerMac, &evt->devClass, evt->linkKey, NULL);

    void tlkmw_btble_key_derivation_store_link_key(uint8_t bd_addr[6], uint8_t link_key[16], uint8_t key_type);
    tlkmw_btble_key_derivation_store_link_key(evt->peerMac, evt->linkKey, evt->keyType);

    return 0;
}

/**
 * @brief       Handles the bluetooth SDP search add protocol event.
 * @param[in]   pData    - Pointer to the event data.
 * @param[in]   dataLen  - Length of the event data (unused).
 * @return      TLK_ENONE - Always returns success.
 */
static int tlkmdi_btacl_searchAddProtoEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_sdpSearchAddProtEvt_t *pEvt;
    tlkmdi_btacl_item_t       *pItem;

    pEvt  = (btp_sdpSearchAddProtEvt_t *)pData;
    pItem = tlkmdi_btacl_getUsedItem(pEvt->handle);
    if (pItem == NULL) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_searchAddProtoEvt: error - no node");
        return TLK_ENONE;
    }

    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_searchAddProtoEvt:{handle-0x%x,search-0x%x,psm-0x%x base-0x%x}", pEvt->handle, pEvt->search_uuid,
                 pEvt->psm, pEvt->psm_base_uuid);

    if (pEvt->search_uuid == BTP_SDP_SRVCLASS_ID_AV_REMOTE_TARGET) {
        if (pEvt->psm_base_uuid == 0x0008) { // BTP_SDP_PROTOCOL_OBEX_UUID
            pItem->avrcpCoverArtPsm = pEvt->psm;
            tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pItem->avrcpCoverArtPsm, TLKMDI_BT_AVRCP_ArtPsm);
#if (TLKBTP_CFG_AVRC_COVER_ART_ENABLE)
            if ((pItem->connFlag & BTP_PFLAG_AVRCP) && pItem->avrcpCoverArtPsm != 0) {
                btp_coverArt_connect(pEvt->handle, BTP_USRID_CLIENT, pItem->avrcpCoverArtPsm, true);
            }
#endif
        }
    }

    return TLK_ENONE;
}

int bth_acl_linkkey_request_for_user(uint8_t *pAddr, uint8_t *pLinkkey)
{
    return tlkmdi_tinySql_getPairingDeviceByAddr(pAddr, NULL, pLinkkey, NULL);
}

/**
 * @brief       Timer callback function for handling Bluetooth ACL operations.
 * @param[in]   pTimer Timer handle provided by the system. Used to identify and manage the timer.
 * @param[out]  userArg Pointer to user-defined data.
 * @return      None.
 */
static void tlkmdi_btacl_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    tlkmdi_btacl_item_t *pItem = (tlkmdi_btacl_item_t *)userArg;
    tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_timer: %d %x", pItem->timeout, pItem->busys);

    if (pItem->busys != 0) {
        tlkmdi_btacl_procs(pItem);
    }
    if (pItem->busys == 0 && pItem->timeout != 0) {
        pItem->timeout = 0;
    }
    if (pItem->timeout != 0 || pItem->busys != 0) {
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
    }
}

/**
 * @brief       Processes Bluetooth ACL operations.
 * @param[in]   pItem Pointer to the Bluetooth ACL item.
 * @return      None.
 */
static void tlkmdi_btacl_procs(tlkmdi_btacl_item_t *pItem)
{
    if ((pItem->busys & TLKMDI_BTACL_BUSY_CONN_PROF) != 0) {
        pItem->busys &= ~TLKMDI_BTACL_BUSY_CONN_PROF;
        tlkmdi_btacl_connProfProcs(pItem);
    } else if ((pItem->busys & TLKMDI_BTACL_BUSY_DISC_PROF) != 0) {
        pItem->busys &= ~TLKMDI_BTACL_BUSY_DISC_PROF;
        tlkmdi_btacl_discProfProcs(pItem);
    } else if ((pItem->busys & TLKMDI_BTACL_WAIT_DISC_PROF) != 0) {
        if (pItem->timeout != 0) {
            pItem->timeout--;
        }
        if (pItem->timeout == 0) {
            pItem->busys &= ~TLKMDI_BTACL_WAIT_DISC_PROF;
            if (pItem->state == TLK_STATE_DISCING) {
                pItem->busys |= TLKMDI_BTACL_BUSY_DISC_ACL;
            }
        }
    } else if ((pItem->busys & TLKMDI_BTACL_BUSY_DISC_RFC) != 0) {
#if (TLKBTP_CFG_RFC_ENABLE) // btble_a2dp_to_bis prj clear compile error.
        pItem->busys &= ~TLKMDI_BTACL_BUSY_DISC_RFC;
        if (btp_rfcomm_disconn(pItem->handle) == -TLK_EBUSY) {
            pItem->busys |= TLKMDI_BTACL_WAIT_DISC_RFC;
            pItem->timeout = TLKMDI_BTACL_PROFILE_DISC_TIMEOUT;
        } else if (pItem->state == TLK_STATE_DISCING) {
            pItem->busys |= TLKMDI_BTACL_BUSY_DISC_ACL;
        }
#endif
    } else if ((pItem->busys & TLKMDI_BTACL_WAIT_DISC_RFC) != 0) {
        if (pItem->timeout != 0) {
            pItem->timeout--;
        }
        if (pItem->timeout == 0) {
            pItem->busys &= ~TLKMDI_BTACL_WAIT_DISC_RFC;
            if (pItem->state == TLK_STATE_DISCING) {
                pItem->busys |= TLKMDI_BTACL_BUSY_DISC_ACL;
            }
        }
    }

    if ((pItem->busys & TLKMDI_BTACL_WAIT_DISC_ACL) != 0) {
        if (pItem->idleTime != 0) {
            pItem->idleTime--;
        }
        if (pItem->idleTime == 0) {
            pItem->busys &= ~TLKMDI_BTACL_WAIT_DISC_ACL;
            pItem->busys |= TLKMDI_BTACL_BUSY_DISC_ACL;
            tlkapi_trace(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_procs: idle timeout");
        }
    }
    if ((pItem->busys & TLKMDI_BTACL_BUSY_DISC_ACL) != 0) {
        pItem->busys &= ~TLKMDI_BTACL_BUSY_DISC_ACL;
        if (tlkmdi_bt_tpt_isSlave() || bth_acl_disconn(pItem->handle, 0x00) != -TLK_EBUSY) {
            btp_destroy(pItem->handle);
            bth_destroy(pItem->handle);
            tlkmdi_btacl_resetItem(pItem);
        }
    }
    if ((pItem->busys & TLKMDI_BTACL_WAIT_CANCEL) != 0) {
        if (pItem->timeout != 0) {
            pItem->timeout--;
        }
        if (pItem->timeout == 0) {
            pItem->busys &= ~TLKMDI_BTACL_WAIT_CANCEL;
            tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "TLKMDI_BTACL_WAIT_CANCEL: timeout");
            tlkmdi_btacl_resetItem(pItem);
        }
    }
}

/**
 * @brief       Processes Bluetooth ACL profile connection operations.
 * @param[in]   pItem Pointer to the Bluetooth ACL item.
 * @return      None.
 */
static void tlkmdi_btacl_connProfProcs(tlkmdi_btacl_item_t *pItem)
{
    bool    busy;
    uint8_t index;
    busy = false;
    for (index = 0; index < TLKMDI_BTACL_PROF_NUMB; index++) {
        if (tlkmdi_btacl_profileConnDeal(pItem, &pItem->prof[index]) && !busy) {
            busy = true;
        }
    }
    if (busy) {
        pItem->busys |= TLKMDI_BTACL_BUSY_CONN_PROF;
    }
}

/**
 * @brief       This function processes the disconnection of profiles for a Bluetooth ACL connection.
 * @param[in]   pItem    - Pointer to the Bluetooth ACL item containing connection information.
 * @return      none.
 */
static void tlkmdi_btacl_discProfProcs(tlkmdi_btacl_item_t *pItem)
{
    bool     busy;
    uint8_t  index;
    uint16_t connFlag;

    busy     = false;
    connFlag = pItem->connFlag;
    connFlag &= ~BTP_PFLAG_SDP;
    connFlag &= ~BTP_PFLAG_RFC;
    if (connFlag == 0) {
        return;
    }
    for (index = 0; index < 16; index++) {
        if ((connFlag & (1 << index)) != 0 && btp_module_disconn(pItem->handle, index, 0) != -TLK_EBUSY) {
            busy = true;
        }
    }
    if (busy) {
        pItem->busys |= TLKMDI_BTACL_WAIT_DISC_PROF;
        pItem->timeout = TLKMDI_BTACL_PROFILE_DISC_TIMEOUT;
    } else if (pItem->state == TLK_STATE_DISCING) {
        pItem->busys |= TLKMDI_BTACL_BUSY_DISC_ACL;
    }
}

/**
 * @brief       Handles the connection deal for a Bluetooth profile.
 * @param[in]   pItem   - Pointer to the Bluetooth ACL item structure.
 * @param[in]   pProf   - Pointer to the Bluetooth ACL profile structure.
 * @return      true if the connection process is ongoing or successful, false otherwise.
 */
static bool tlkmdi_btacl_profileConnDeal(tlkmdi_btacl_item_t *pItem, tlkmdi_btacl_prof_t *pProf)
{
    int ret;

    if (pProf->state == TLK_STATE_CLOSED) {
        return false;
    }

    if ((pItem->connFlag & (1 << pProf->ptype)) != 0) {
        tmemset(pProf, 0, sizeof(tlkmdi_btacl_prof_t));
        return false;
    }

    if (pProf->delay != 0) {
        pProf->delay--;
    }
    if (pProf->delay != 0) {
        return true;
    }

    if (pProf->cwait != 0) {
        pProf->cwait--;
    }
    if (pProf->cwait == 0) {
        tlkapi_warn(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileConnDeal: wait timeout ptype-%d usrID-%d", pProf->ptype, pProf->usrID);
        tmemset(pProf, 0, sizeof(tlkmdi_btacl_prof_t));
        return false;
    }
    if (pProf->state == TLK_STATE_CONNING) {
        return true;
    }

    ret = -TLK_EFAIL;
    if (pProf->ptype == BTP_PTYPE_RFC) {
#if (TLKBTP_CFG_RFC_ENABLE)
        ret = btp_rfcomm_connect(pItem->handle);
#endif
    } else if (pProf->ptype == BTP_PTYPE_A2DP) {
#if (TLKBTP_CFG_A2DP_ENABLE)
        ret = btp_a2dp_connect(pItem->handle, pProf->usrID);
#endif
    } else if (pProf->ptype == BTP_PTYPE_HFP) {
#if (TLKBTP_CFG_HFP_ENABLE)
        if ((pItem->connFlag & BTP_PFLAG_RFC) == 0) {
            return true;
        }
#if (!TLKBTP_CFG_HFPAG_ENABLE)
        if (pItem->agChannel != 0) {
            ret = btp_hfp_connect(pItem->handle, BTP_USRID_CLIENT, pItem->agChannel);
        } else if (pItem->hfChannel != 0) {
            ret = btp_hfp_connect(pItem->handle, BTP_USRID_SERVER, pItem->hfChannel);
        }
#else
        uint8_t dtype;
        dtype = bth_devClassToDevType(pItem->devClass); // get peer devClass
        if (dtype == BTH_REMOTE_DTYPE_HEADSET) {
            if (pItem->hfChannel != 0) {
#if (TLK_CHECK_REMOTE_DEV)
                bth_aclGetNameReportEvt_t *info = (bth_aclGetNameReportEvt_t *)tlkmdi_btacl_get_remote_dev();
                if (info->isCustomerDev) {
                    ret = btp_hfp_connect(pItem->handle, BTP_USRID_CLIENT, pItem->hfChannel);
                } else {
                    ret = btp_hfp_connect(pItem->handle, BTP_USRID_SERVER, pItem->hfChannel);
                }

#else
                ret = btp_hfp_connect(pItem->handle, BTP_USRID_SERVER, pItem->hfChannel);
#endif
            }
        } else if (dtype == BTH_REMOTE_DTYPE_COMPUTER || dtype == BTH_REMOTE_DTYPE_PHONE) {
            if (pItem->agChannel != 0) {
                ret = btp_hfp_connect(pItem->handle, BTP_USRID_CLIENT, pItem->agChannel);
            }
        } else {
            if (pItem->agChannel != 0) {
                ret = btp_hfp_connect(pItem->handle, BTP_USRID_CLIENT, pItem->agChannel);
            } else if (pItem->hfChannel != 0) {
                ret = btp_hfp_connect(pItem->handle, BTP_USRID_SERVER, pItem->hfChannel);
            }
        }
#endif // TLKBTP_CFG_HFPAG_ENABLE

#endif // TLKBTP_CFG_HFP_ENABLE
    } else if (pProf->ptype == BTP_PTYPE_IAP) {
#if (TLKBTP_CFG_IAP_ENABLE)
        if (pItem->iapChannel == 0 || (pItem->connFlag & BTP_PFLAG_RFC) == 0) {
            return true;
        }
        ret = btp_iap_connect(pItem->handle, pItem->iapChannel);
#endif
    } else if (pProf->ptype == BTP_PTYPE_SPP) {
#if (TLKBTP_CFG_SPP_ENABLE)
        if (pItem->sppChannel == 0 || (pItem->connFlag & BTP_PFLAG_RFC) == 0) {
            return true;
        }
        ret = btp_spp_connect(pItem->handle, pItem->sppChannel);
#endif
    } else if (pProf->ptype == BTP_PTYPE_HID && (pItem->devClass != BTH_REMOTE_DTYPE_HEADSET)) {
#if (TLKBTP_CFG_HID_ENABLE)
        ret = btp_hid_connect(pItem->handle, pProf->usrID);
#endif
    } else if (pProf->ptype == BTP_PTYPE_ATT) {
#if (TLKBTP_CFG_ATT_ENABLE)
        ret = btp_att_connect(pItem->handle, pProf->usrID);
#endif
    } else if (pProf->ptype == BTP_PTYPE_PBAP) {
#if (TLKBTP_CFG_PBAP_ENABLE)
        if (pItem->pbapChannel == 0 || (pItem->connFlag & BTP_PFLAG_RFC) == 0) {
            return true;
        }
        ret = btp_pbap_connect(pItem->handle, pProf->usrID, pItem->pbapChannel, false);
#endif
    } else if (pProf->ptype == BTP_PTYPE_AVRCP) {
#if (TLKBTP_CFG_AVRCP_ENABLE)
        ret = btp_avrcp_connect(pItem->handle, pProf->usrID);
#endif
    } else if (pProf->ptype == BTP_PTYPE_COVERART) {
#if (TLKBTP_CFG_COVERARTCLT_ENABLE)
        if (pItem->bipChannel != 0) {
            /*based on rfcomm, TODO:yating*/
        } else if (pItem->avrcpCoverArtPsm != 0) {
            /*based on l2cap.*/
            btp_coverArt_connect(pItem->handle, BTP_USRID_CLIENT, pItem->avrcpCoverArtPsm, true);
        }
#endif
    }

    if (ret != TLK_ENONE && ret != -TLK_EBUSY && ret != -TLK_EEXIST) {
        tlkapi_error(TLKMDI_BTACL_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tlkmdi_btacl_profileConnDeal: connect fail ptype-%d usrID-%d", pProf->ptype, pProf->usrID);
        tmemset(pProf, 0, sizeof(tlkmdi_btacl_prof_t));
        return false;
    }
    pProf->state = TLK_STATE_CONNING;
    return true;
}

/**
 * @brief       This function gets the idle acl link count
 * @param[in]   none.
 * @return      The count of idle acl link
 */
uint8_t tlkmdi_btacl_getIdleCount(void)
{
    uint8_t index;
    uint8_t count = 0;
    for (index = 0; index < TLKMDI_BTACL_ITEM_NUMB; index++) {
        if (sTlkMdiBtAclCtrl.item[index].state == TLK_STATE_CLOSED) {
            count++;
        }
    }
    return count;
}

/**
 * @brief       This function gets the used acl link count
 * @param[in]   none.
 * @return      The count of used acl link
 */
uint8_t tlkmdi_btacl_getUsedCount(void)
{
    uint8_t index;
    uint8_t count = 0;
    for (index = 0; index < TLKMDI_BTACL_ITEM_NUMB; index++) {
        if (sTlkMdiBtAclCtrl.item[index].state != TLK_STATE_CLOSED) {
            count++;
        }
    }
    return count;
}

/**
 * @brief       This function gets the connected acl link count
 * @param[in]   none.
 * @return      The count of connected acl link
 */
uint8_t tlkmdi_btacl_getConnCount(void)
{
    uint8_t index;
    uint8_t count = 0;
    for (index = 0; index < TLKMDI_BTACL_ITEM_NUMB; index++) {
        if (sTlkMdiBtAclCtrl.item[index].state == TLK_STATE_CONNECT) {
            count++;
        }
    }
    return count;
}

/**
 * @brief       This function resets the acl control block
 * @param[in]   pItem    - The acl control block
 * @return      none.
 */
void tlkmdi_btacl_resetItem(tlkmdi_btacl_item_t *pItem)
{
    uint16_t handle;
    uint8_t  btaddr[6];

    if (pItem == NULL) {
        return;
    }

    uint8_t dtype;
    dtype = bth_devClassToDevType(pItem->devClass); // get peer devClass

    handle = pItem->handle;
    tmemcpy(btaddr, pItem->btaddr, 6);
    tlksys_timer_stop(TLKSYS_TASKID_HOST, &pItem->timer);
    tmemset(pItem, 0, sizeof(tlkmdi_btacl_item_t));

    if (handle != 0) {
        tlkmdi_bt_disconnect_evt_format event = {
            .aclHandle = handle,
            .dtype     = dtype,
            .pBtAddr   = btaddr,
            .reason    = 0,
        };

        tlkmw_bt_send_event(TLKMW_BT_DISCONNECT_COMPLETE, (uint8_t *)&event, sizeof(tlkmdi_bt_disconnect_evt_format));
    }

    tlkmw_host_linkmgr_delLink(TLKMDI_HOST_LINK_TYPE_BT, handle);
}

/**
 * @brief       This function gets a idle acl control block
 * @param[in]   none.
 * @return      The idle acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_getIdleItem(void)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_BTACL_ITEM_NUMB; index++) {
        if (sTlkMdiBtAclCtrl.item[index].state == TLK_STATE_CLOSED) {
            break;
        }
    }
    if (index == TLKMDI_BTACL_ITEM_NUMB) {
        return NULL;
    }
    return &sTlkMdiBtAclCtrl.item[index];
}

/**
 * @brief       This function gets a Connecting acl control block
 * @param[in]   none.
 * @return      The connecting acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_getBusyItem(void)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_BTACL_ITEM_NUMB; index++) {
        if (sTlkMdiBtAclCtrl.item[index].state != TLK_STATE_CLOSED && sTlkMdiBtAclCtrl.item[index].state != TLK_STATE_CONNECT &&
            sTlkMdiBtAclCtrl.item[index].state != TLK_STATE_DISCING) {
            break;
        }
    }
    if (index == TLKMDI_BTACL_ITEM_NUMB) {
        return NULL;
    }
    return &sTlkMdiBtAclCtrl.item[index];
}

/**
 * @brief       This function gets a Used acl control block
 * @param[in]   handle    - The ACL handle
 * @return      The used acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_getUsedItem(uint16_t handle)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_BTACL_ITEM_NUMB; index++) {
        if (sTlkMdiBtAclCtrl.item[index].state != TLK_STATE_CLOSED && sTlkMdiBtAclCtrl.item[index].handle == handle) {
            break;
        }
    }
    if (index == TLKMDI_BTACL_ITEM_NUMB) {
        return NULL;
    }
    return &sTlkMdiBtAclCtrl.item[index];
}

/**
 * @brief       This function gets a Connected acl control block
 * @param[in]   handle    - The ACL handle
 * @return      The Connected acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_getConnItem(uint16_t handle)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_BTACL_ITEM_NUMB; index++) {
        if (sTlkMdiBtAclCtrl.item[index].state == TLK_STATE_CONNECT && sTlkMdiBtAclCtrl.item[index].handle == handle) {
            break;
        }
    }
    if (index == TLKMDI_BTACL_ITEM_NUMB) {
        return NULL;
    }
    return &sTlkMdiBtAclCtrl.item[index];
}

/**
 * @brief       This function gets a Connected acl control block
 * @param[in]   index    - The index of connected acl control block
 * @return      The Connected acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_getConnItemByIndex(uint8_t index)
{
    uint8_t i;
    // uint8_t count = 0;
    for (i = 0; i < TLKMDI_BTACL_ITEM_NUMB; i++) {
        if (sTlkMdiBtAclCtrl.item[i].state == TLK_STATE_CONNECT) {
            if (i == index) {
                break;
            }
            // count++;
        }
    }
    if (i == TLKMDI_BTACL_ITEM_NUMB) {
        return NULL;
    }
    return &sTlkMdiBtAclCtrl.item[i];
}

/**
 * @brief       This function finds a Used acl control block
 * @param[in]   pBtAddr    - The Bt address of acl
 * @return      The Used acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_searchUsedItem(uint8_t *pBtAddr)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_BTACL_ITEM_NUMB; index++) {
        if (sTlkMdiBtAclCtrl.item[index].state != TLK_STATE_CLOSED && tmemcmp(sTlkMdiBtAclCtrl.item[index].btaddr, pBtAddr, 6) == 0) {
            break;
        }
    }
    if (index == TLKMDI_BTACL_ITEM_NUMB) {
        return NULL;
    }
    return &sTlkMdiBtAclCtrl.item[index];
}

/**
 * @brief       This function finds a Connected acl control block
 * @param[in]   pBtAddr    - The Bt address of acl
 * @return      The Connected acl control block or NULL
 */
tlkmdi_btacl_item_t *tlkmdi_btacl_searchConnItem(uint8_t *pBtAddr)
{
    uint8_t index;
    for (index = 0; index < TLKMDI_BTACL_ITEM_NUMB; index++) {
        if (sTlkMdiBtAclCtrl.item[index].state == TLK_STATE_CONNECT && tmemcmp(sTlkMdiBtAclCtrl.item[index].btaddr, pBtAddr, 6) == 0) {
            break;
        }
    }
    if (index == TLKMDI_BTACL_ITEM_NUMB) {
        return NULL;
    }
    return &sTlkMdiBtAclCtrl.item[index];
}

/**
 * @brief       This function checks if profile is connected
 * @param[in]   aclHandle    - The ACL handle
 * @param[in]   ptype        - The profile type
 * @return      true if profile is connected, false otherwise
 */
bool tlkmdi_btacl_searchProfIsConn(uint16_t aclHandle, uint8_t ptype)
{
    tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getConnItem(aclHandle);
    if (pItem == NULL) {
        return false;
    }

    return pItem->connFlag & (1 << ptype);
}

/**
 * @brief       This function sets sniff busy flag
 * @param[in]   handle       - The ACL handle
 * @param[in]   audioBusy    - Audio busy flag
 * @return      none.
 */
void tlkmdi_btacl_setSniffBusy(uint16_t handle, uint8_t audioBusy)
{
    tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL) {
        return;
    }
    pItem->audioBusy = audioBusy;
    bth_acl_setSniffBusy(handle, BTH_ACL_SNIFF_BUSY_HOST);
}

/**
 * @brief       This function clears sniff busy flag
 * @param[in]   handle       - The ACL handle
 * @param[in]   audioBusy    - Audio busy flag
 * @return      none.
 */
void tlkmdi_btacl_clsSniffBusy(uint16_t handle, uint8_t audioBusy)
{
    tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL) {
        return;
    }
    pItem->audioBusy = audioBusy;
    bth_acl_setLinkPolicyBit(handle, HCI_LP_ENABLE_SNIFF_MODE_MASK);
    bth_acl_clsSniffBusy(handle, BTH_ACL_SNIFF_BUSY_HOST);
}

/**
 * @brief       This function checks if device is IOS device
 * @param[in]   handle    - The ACL handle
 * @return      true if device is IOS, false otherwise
 */
bool tlkmdi_btacl_isIOS_device(uint16_t handle)
{
    tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getConnItem(handle);
    if (pItem == NULL) {
        return false;
    }

    if (pItem->connFlag & BTP_PFLAG_HFP) {
#if (TLKBTP_CFG_HFP_ENABLE)
        return btp_hfphf_isIosDev(handle);
#endif // TLKBTP_CFG_HFP_ENABL
    } else {
        return pItem->iapChannel == 0 ? false : true;
    }
    return false;
}

/**
 * @brief     Provides a hook function to customer deal remote device info.
 * @param[in] None.
 * @returns   None
 */
__attribute__((weak)) void tlkmdi_btacl_getRemoteNameChange(uint8_t *pData)
{
    (void)pData;
}

__attribute__((weak)) void tlkmdi_btacl_set_remote_dev(uint8_t *pData, uint8_t isTrue)
{
    (void)pData;
    (void)isTrue;
}

__attribute__((weak)) void *tlkmdi_btacl_get_remote_dev(void)
{
    return NULL;
}

static int tlkmdi_btacl_getRemoteNameEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    tlkmdi_btacl_getRemoteNameChange(pData);
    return 0;
}

#else

/**
 * @brief       This function gets the used acl link count
 * @param[in]   none.
 * @return      The count of used acl link
 */
uint8_t tlkmdi_btacl_getUsedCount(void)
{
    return 0;
}

#endif // #if (TLK_STK_BT_ENABLE)
