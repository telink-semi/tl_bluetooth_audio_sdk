/********************************************************************************************************
 * @file    app_bt.c
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
#include "tlkapp/tlkapp.h"
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth_stdio.h"
uint8_t tlkapp_get_remote_dev_istrue(void);
#if (TLK_CHECK_REMOTE_DEV)
#define REMOTE_DEV_NAME_MAX_LEN 248
static uint8_t                   remoteDevNameBuf[REMOTE_DEV_NAME_MAX_LEN];
static bth_aclGetNameReportEvt_t remoteDeveInfo = {0};
#endif
static void app_btmgr_appendProfile(uint16_t aclHandle);

/**
 * @brief       ACL connection callback function
 * @param[in]   pData: event data
 * @param[in]   dataLen: event data length
 * @note        This function handles ACL connection events, including stopping pairing 
 *              and managing scan modes based on connection status
 */
static int app_btmgr_aclConnectCB(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < sizeof(tlkmdi_bt_connect_evt_format)) {
        return -TLK_EPARAM;
    }

    tlkmdi_bt_connect_evt_format *event = (tlkmdi_bt_connect_evt_format *)pData;

    tlkapi_printf(APP_LOG_EN, "app_btmgr_aclConnectCB: 0x%x %d, 0x%08x, hfp_ChId[%d]", event->aclHandle, event->status, *(uint32_t *)event->pBtAddr, event->hfp_channel);
#if (TLK_CFG_UART_TOOL_ENABLE)
    tlkapp_btmgr_sendAclConnectEvt(event->aclHandle, event->status, event->pBtAddr);
#endif

    if (event->status == BTH_HCI_ERROR_NONE) {
#if (TLK_MW_BTPAIRING_ENABLE)
        tlkmdi_btParing_stop();
#endif
        if (tlkmdi_btacl_getIdleCount() == 0) {
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
        } else {
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
        }
    }

    return TLK_ENONE;
}

/**
 * @brief       ACL encryption callback function
 * @param[in]   pData - pointer to the event data
 * @param[in]   dataLen - length of the event data
 * @return      TLK_ENONE - success, otherwise failure
 * @note        This function handles ACL encryption events, connecting SDP client if encryption succeeds
 */
static int app_btmgr_aclEncryptCB(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < sizeof(tlkmdi_bt_encryption_evt_format)) {
        return -TLK_ENONE;
    }

    tlkmdi_bt_encryption_evt_format *event = (tlkmdi_bt_encryption_evt_format *)pData;

    if (event->status == TLK_ENONE) {
        if (event->hfp_channel == 0) {
            btp_sdpclt_connect(event->aclHandle); // refer to PC_Comp in app_btmgr_ProfConnCB
        } else {
            app_btmgr_appendProfile(event->aclHandle);
        }
    }
    return TLK_ENONE;
}

/**
 * @brief       ACL disconnection callback function
 * @param[in]   pData - pointer to the event data
 * @param[in]   dataLen - length of the event data
 * @return      TLK_ENONE is success, others is failure
 */
static int app_btmgr_aclDisconnCB(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < sizeof(tlkmdi_bt_disconnect_evt_format)) {
        return -TLK_EPARAM;
    }
    tlkmdi_bt_disconnect_evt_format *event = (tlkmdi_bt_disconnect_evt_format *)pData;

#if (TLK_CFG_UART_TOOL_ENABLE)
    tlkapp_btmgr_sendAclDisconnEvt(event->aclHandle, event->reason, event->pBtAddr);
#endif

#if (TLK_CHECK_REMOTE_DEV)
    bth_aclGetNameReportEvt_t *info = (bth_aclGetNameReportEvt_t *)tlkmdi_btacl_get_remote_dev();
    if (info != NULL) {
        if (info == &remoteDeveInfo) {
            if (info->handle == event->aclHandle && info->isCustomerDev) {
                memset(info, 0x00, sizeof(bth_aclGetNameReportEvt_t));
            }
        }
    }

#endif

    uint32_t taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_MUSIC << 16);
    tlkapp_audioScheduler_deleteTaskSafe(taskID);
    taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
    tlkapp_audioScheduler_deleteTaskSafe(taskID);
    tlkmdi_bta2dp_sendHostMusicStateChgEvt(event->aclHandle, TLK_STATE_CLOSED);
#if TLK_MW_BTREC_ENABLE
    if (tlkmdi_btRecon_isInBusy()) {
        uint8_t *pPageAddr = tlkmdi_btRecon_getPageAddr();
        if (pPageAddr != NULL && tmemcmp(event->pBtAddr, pPageAddr, 6) == 0) {
            return TLK_ENONE; /* wait next retry, todo modify not wait 9s again syzhou*/
        }
    }
#endif

#if (TLK_MW_BTPAIRING_ENABLE)
    if (tlkmdi_btPairing_isInProgress() == false)
#endif
    {
#if TLK_MW_BTREC_ENABLE
        if (event->reason == BTH_HCI_ERROR_CONN_TIMEOUT) { //Start Reconnect
            uint32_t devClass = 0;
            int      sqlRet   = tlkmdi_tinySql_getPairingDeviceByAddr(event->pBtAddr, &devClass, NULL, NULL);
            if (sqlRet == TLK_ENONE) {
                if (tlkmdi_btacl_getUsedCount() < TLKMDI_BTACL_ITEM_NUMB) {
                    return tlkmdi_btRecon_start(event->pBtAddr, devClass, TLKMDI_BTRECON_RETRY_NUM_LINK_LOSS);
                }
            } else if (sqlRet == -TLK_ENODATA) {
                tlkmdi_tinySql_deletePairingDevice(event->pBtAddr);
            }
        }
#endif
        if (tlkmdi_btacl_getUsedCount() == 0) {
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
        } else {
#if (!TLK_BT_MULTIPNT_ENABLE)
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 120);
#else
            if (tlkmw_audio_btif_getPlayHandle() != 0) {
                tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 120);
            } else {
                tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
            }
#endif
        }
    }
    return TLK_ENONE;
}

/**
 * @brief       Profile connection callback function
 * @param[in]   pData - pointer to the event data
 * @param[in]   dataLen - length of the event data
 * @return      TLK_ENONE is success, others is failure
 */
static int app_btmgr_ProfConnCB(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < sizeof(tlkmdi_bt_profile_connect_evt_format)) {
        return -TLK_ENONE;
    }
    tlkmdi_bt_profile_connect_evt_format *event = (tlkmdi_bt_profile_connect_evt_format *)pData;

    bth_acl_handle_t *pHandle;

#if (TLK_CFG_UART_TOOL_ENABLE)
    if (event->ptype != BTP_PTYPE_SDP && event->ptype != BTP_PTYPE_RFC) {
        tlkapp_btmgr_sendProfConnectEvt(event->aclHandle, event->status, event->ptype, event->usrID, event->pBtAddr);
    }
#endif

    if (event->status != TLK_ENONE) {
        return TLK_ENONE;
    }

    if (event->ptype == BTP_PTYPE_A2DP) {
        tlkmdi_bta2dp_sendHostMusicStateChgEvt(event->aclHandle, TLK_STATE_IDLE);
        btp_avrcp_connect(event->aclHandle, event->usrID);
    } else if (event->ptype == BTP_PTYPE_HFP) {
#if (TLK_CHECK_REMOTE_DEV)
        bth_aclGetNameReportEvt_t *info_t = (bth_aclGetNameReportEvt_t *)tlkmdi_btacl_get_remote_dev();
        if (event->aclHandle != btp_hfp_getAgHandle() && (info_t->handle != event->aclHandle))
#else
        if (event->aclHandle != btp_hfp_getAgHandle())
#endif
        {
            tlkapp_audioScheduler_taskInfo_t info = {
                .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE,
                .optype    = TLKAUD_TYPE_CC_BT_VOICE,
                .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_CC_BT_VOICE),
                .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
            };
            uint32_t taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
            tlkapp_audioScheduler_updateTaskSafe(taskID, info, 0);
        }
    }
    if (event->is_first_prof) {
        // btp_sdpclt_connect(handle); // PC_Comp. Fix(temp) PC(Chrome) first pairing device will connect timeout.
        pHandle = bth_handle_getUsedAcl(event->aclHandle);
        if (pHandle != NULL) {
            tlkmdi_tinySql_updatePairingDevice(event->pBtAddr, &pHandle->devClass, pHandle->linkKey, pHandle->devName);
        }
        //save device class and linkkey

        //save rfc channel id
        tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getUsedItem(event->aclHandle);
        if (pItem == NULL) {
            return -TLK_ENOITEM;
        }

        if (!pItem->active) {
            if (pItem->agChannel != 0) {
                tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pItem->agChannel, TLKMDI_BT_RFC_CHID_HFP);
            }
        }
        tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pItem->sppChannel, TLKMDI_BT_RFC_CHID_SPP);
    }
    return TLK_ENONE;
}

/**
 * @brief       Profile disconnection callback function
 * @param[in]   pData - pointer to the event data
 * @param[in]   dataLen - length of the event data
 * @return      TLK_ENONE is success, others is failure
 */
static int app_btmgr_ProfDiscCB(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < sizeof(tlkmdi_bt_profile_disconnect_evt_format)) {
        return -TLK_EPARAM;
    }
    tlkmdi_bt_profile_disconnect_evt_format *event = (tlkmdi_bt_profile_disconnect_evt_format *)pData;

#if (TLK_CFG_UART_TOOL_ENABLE)
    if (event->ptype != BTP_PTYPE_SDP && event->ptype != BTP_PTYPE_RFC) {
        tlkapp_btmgr_sendProfDisconnEvt(event->aclHandle, event->reason, event->ptype, event->usrID, event->pBtAddr);
    }
#endif

    if (event->ptype == BTP_PTYPE_A2DP) {
        tlkmdi_bta2dp_sendHostMusicStateChgEvt(event->aclHandle, TLK_STATE_CLOSED);
    } else if (event->ptype == BTP_PTYPE_HFP) {
        uint32_t taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
        tlkapp_audioScheduler_deleteTaskSafe(taskID);
    }
    if (event->ptype == BTP_PTYPE_SDP && event->usrID == BTP_USRID_CLIENT) {
        app_btmgr_appendProfile(event->aclHandle);
    }
    return TLK_ENONE;
}

static void app_btmgr_appendProfile(uint16_t aclHandle)
{
    uint8_t              dtype;
    uint16_t             delayMs;
    tlkmdi_btacl_item_t *pItem;

    pItem = tlkmdi_btacl_getConnItem(aclHandle);
    if (pItem == NULL) {
        return;
    }

    delayMs = pItem->active ? 0 : 2000;
    dtype   = bth_devClassToDevType(pItem->devClass);
    if (tlkmdi_btacl_isHaveRfc(aclHandle)) {
        tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_RFC, 0, delayMs);

        if (tlkmdi_btacl_isFindHfp(aclHandle)) {
            tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_HFP, BTP_USRID_NONE, delayMs + 200);
        }
        if (dtype != BTH_REMOTE_DTYPE_HEADSET && tlkmdi_btacl_isFindPbap(aclHandle)) {
            tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_PBAP, BTP_USRID_CLIENT, delayMs + 500);
        }
        delayMs += 800;
    }

    if (dtype == BTH_REMOTE_DTYPE_COMPUTER || dtype == BTH_REMOTE_DTYPE_PHONE) {
        tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_A2DP, BTP_USRID_CLIENT, delayMs);
        tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_HID, BTP_USRID_SERVER, delayMs);
    }

    tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_AVRCP, BTP_USRID_NONE, delayMs + 2000);
}

TLKMW_BT_EVT_REGISTER(TLKMW_BT_CONNECT_COMPLETE, app_btmgr_aclConnectCB);
TLKMW_BT_EVT_REGISTER(TLKMW_BT_DISCONNECT_COMPLETE, app_btmgr_aclDisconnCB);
TLKMW_BT_EVT_REGISTER(TLKMW_BT_ENCRYPTION_COMPLETE, app_btmgr_aclEncryptCB);
TLKMW_BT_EVT_REGISTER(TLKMW_BT_PROFILE_CONNECT, app_btmgr_ProfConnCB);
TLKMW_BT_EVT_REGISTER(TLKMW_BT_PROFILE_DISCONNECT, app_btmgr_ProfDiscCB);

/**
 * @brief     Overwrite the hook function when the BT host task starts.
 *            If have peer device,reconnect to it,Otherwise open bt scan.
 * @param[in] None.
 * @returns   None.
 */
void tlkapp_host_bt_taskStartHook(void)
{
    uint32_t devClass;
    uint8_t  devAddr[6];
    bth_acl_systemManageSniff(false); //close sniff auto manage.
#if TLK_CFG_BT_EX_PAIRING_MODE_ENABLE
    tlkmdi_btPairing_setMode(TLKMDI_BTPAIRING_MODE_RETAIN_LINK);
#endif
    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
    int ret = tlkmdi_tinySql_getLastPairingDevice(devAddr, &devClass, NULL, NULL);
    if (ret == TLK_ENONE) {
        if (bth_devClassToDevType(devClass) != BTH_REMOTE_DTYPE_HEADSET) {
            tlkmdi_btRecon_start(devAddr, devClass, TLKMDI_BTRECON_RETRY_NUM_POWERON);
        } else {
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 120);
        }

    } else {
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120); /* if no last device ,open both scan */
    }
}

uint16_t tlkapp_host_bt_selectHandleToTriggerSiriHook(void)
{
    return tlkmdi_interphone_linkmgr_getValidHandle();
}
#if (TLK_CHECK_REMOTE_DEV)

void tlkmdi_btacl_getRemoteNameChange(uint8_t *pData)
{
    bth_aclGetNameReportEvt_t *evt = (bth_aclGetNameReportEvt_t *)pData;
    tlk_printf("tlkmdi_btacl_getRemoteNameChange +++++++++++++handle = %x", evt->handle);
    tlkapi_array(0xffffffff, "test", "tlkmdi_btacl_getRemoteNameChangedHandle", evt->pName, evt->nameLen);
    tlk_printf("tlkmdi_btacl_getRemoteNameChange handle = %x", evt->handle);
    if (!strncmp((char *)evt->pName, TLK_APP_REMOTE_NAME_DATA, TLK_APP_REMOTE_NAME_LEN)) {
        tlk_printf("X5 is connected\r\n");
        tlkmdi_btacl_set_remote_dev(pData, 1);
    } else {
        tlk_printf("X5 is not connected\r\n");
    }
}

void tlkmdi_btacl_set_remote_dev(uint8_t *pData, uint8_t isTrue)
{
    if (pData == NULL) {
        tlk_printf("tlkmdi_btacl_set_remote_dev: pData is NULL!\r\n");
        return;
    }
    bth_aclGetNameReportEvt_t *evt = (bth_aclGetNameReportEvt_t *)pData;
    remoteDeveInfo.status          = evt->status;
    remoteDeveInfo.handle          = evt->handle;
    remoteDeveInfo.nameLen         = evt->nameLen;
    if (evt->pName != NULL && evt->nameLen > 0) {
        uint8_t len = evt->nameLen;
        if (len > REMOTE_DEV_NAME_MAX_LEN - 1) {
            len = REMOTE_DEV_NAME_MAX_LEN - 1;
        }
        memcpy(remoteDevNameBuf, evt->pName, len);
        remoteDevNameBuf[len] = 0;
    }
    remoteDeveInfo.pName = remoteDevNameBuf;
    memcpy(&remoteDeveInfo.btaddr, evt->btaddr, 6);
    tlk_printf("tlkmdi_btacl_set_remote_dev:[status:%x] [handle:%x][namelen:%x]\r\n", remoteDeveInfo.status, remoteDeveInfo.handle, remoteDeveInfo.nameLen);
    tlkapi_array(0xffffffff, "test", "tlkmdi_btacl_set_remote_dev addr:", remoteDeveInfo.btaddr, 6);
    tlk_printf("name:%s", remoteDeveInfo.pName);
    // memcpy(&remoteDeveInfo, pData, sizeof(bth_aclGetNameReportEvt_t));
    remoteDeveInfo.isCustomerDev = isTrue;
    remoteDeveInfo.resume_music  = 0;
    remoteDeveInfo.music_handle  = 0;
    tlk_printf("tlkmdi_btacl_set_remote_dev: handle = %x  isTrue = %x\r\n", remoteDeveInfo.handle, remoteDeveInfo.isCustomerDev);
}

void *tlkmdi_btacl_get_remote_dev(void)
{
    tlk_printf("tlkmdi_btacl_get_remote_dev: handle = %x  isTrue = %x\r\n", remoteDeveInfo.handle, remoteDeveInfo.isCustomerDev);

    return (void *)&remoteDeveInfo;
}

uint8_t tlkapp_get_remote_dev_istrue(void)
{
    return remoteDeveInfo.isCustomerDev;
}

#else
uint8_t tlkapp_get_remote_dev_istrue(void)
{
    return 0;
}
#endif

#if TLK_CFG_BT_EX_PAIRING_MODE_ENABLE
uint8_t tlkmdi_btPairing_retainLinkHook(uint16_t handle)
{
    btp_avrcp_sendKeyPress(handle, BTP_AVRCP_KEYID_PAUSE);
    btp_avrcp_sendKeyRelease(handle, BTP_AVRCP_KEYID_PAUSE);
    // tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_HF_SEND_CALL_HUNGUP, &handle, sizeof(handle));
    if (bth_handle_getUsedScoCount()) {
        return TLK_EBUSY;
    }
    return TLK_ENONE;
}
#endif
