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
#if TLK_STK_BT_ENABLE
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth_stdio.h"

#include "stack/tpsll/tpsll.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"

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
        tpsll_hci_sendSetHeadsetCurModeCmd(TPH_HOST_MODE_BT_ACTIVE);
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

#if (TLKBTP_CFG_IAP_ENABLE)
    tlkmdi_btiap_setAclHandle(false, event->aclHandle);
#endif

    uint32_t taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_MUSIC << 16);
    tlkapp_audioScheduler_deleteTaskSafe(taskID);
    taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
    tlkapp_audioScheduler_deleteTaskSafe(taskID);
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
                if (tlkmdi_btacl_getUsedCount() == 0) {
                    return tlkmdi_btRecon_start(event->pBtAddr, devClass, TLKMDI_BTRECON_RETRY_NUM_LINK_LOSS);
                }
            } else if (sqlRet == -TLK_ENODATA) {
                tlkmdi_tinySql_deletePairingDevice(event->pBtAddr);
            }
        }
#endif
        if (tlkmdi_btacl_getUsedCount() == 0) {
            if (tlkmdi_bt_tph_dongle_isConnect()) {
                tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 120);
            } else {
                tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
            }
        } else {
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 120);
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
        tlkapp_audioScheduler_taskInfo_t info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC,
            .optype    = TLKAUD_TYPE_CC_BT_MUSIC,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_CC_BT_MUSIC),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };
        uint32_t taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_MUSIC << 16);
        tlkapp_audioScheduler_updateTaskSafe(taskID, info, 0);

        btp_avrcp_connect(event->aclHandle, event->usrID);
    } else if (event->ptype == BTP_PTYPE_HFP) {
        tlkapp_audioScheduler_taskInfo_t info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE,
            .optype    = TLKAUD_TYPE_CC_BT_VOICE,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_CC_BT_VOICE),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };
        uint32_t taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
        tlkapp_audioScheduler_updateTaskSafe(taskID, info, 0);
    }
#if (TLKBTP_CFG_PBAP_ENABLE)
    else if (ptype == BTP_PTYPE_PBAP) {
        tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getUsedItem(event->aclHandle);
        if (pItem == NULL) {
            return -TLK_ENOITEM;
        }
        tlkmdi_pbap_phoneBookStartSync(event->aclHandle, event->pBtAddr, false);
#if (TLKBTP_CFG_PBAP_ENABLE)
        tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pItem->pbapChannel, TLKMDI_BT_RFC_CHID_PBAP);
#endif // #if (TLKBTP_CFG_PBAP_ENABLE)
    }
#endif // #if (TLKBTP_CFG_PBAP_ENABLE)
    if (event->is_first_prof) {
        //btp_sdpclt_connect(event->aclHandle); // PC_Comp. Fix(temp) PC(Chrome) first pairing device will connect timeout.
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
        uint32_t taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_MUSIC << 16);
        tlkapp_audioScheduler_deleteTaskSafe(taskID);
    } else if (event->ptype == BTP_PTYPE_HFP) {
        uint32_t taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
        tlkapp_audioScheduler_deleteTaskSafe(taskID);
    }
    if (event->ptype == BTP_PTYPE_SDP && event->usrID == BTP_USRID_CLIENT) {
        app_btmgr_appendProfile(event->aclHandle);
    }
    return TLK_ENONE;
}

/**
 * @brief      Append profiles to the connection
 * @param[in]  aclHandle - ACL connection handle
 * @return     none
 */
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
    if (!pItem->active) {
        return; // If not active, do not append profile(profile connections are not initiated actively).
    }
    dtype = bth_devClassToDevType(pItem->devClass);
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
    }

    tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_AVRCP, BTP_USRID_NONE, delayMs + 500);
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
#if (MCU_CORE_TL752X_TEMP)
    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
    return; //??????????
#endif
    uint32_t devClass;
    uint8_t  devAddr[6];
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

#endif