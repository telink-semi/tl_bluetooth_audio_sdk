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
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth_stdio.h"

#include "stack/tpsll/tpsll.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"

static void app_btmgr_appendProfile(uint16_t aclHandle);

/**
 * @brief      Callback function invoked when ACL connection is established
 * @param[in]  handle    - Connection handle
 * @param[in]  status    - Connection status
 * @param[in]  pBtAddr   - Pointer to Bluetooth address of remote device
 * @param[in]  dtype     - Device type
 * @param[in]  hfp_ChId  - HFP channel ID
 * @return     none
 */
static void app_btmgr_aclConnectCB(uint16_t handle, uint8_t status, uint8_t *pBtAddr, uint8_t dtype, uint8_t hfp_ChId)
{
    (void)dtype;
    tlkapi_printf(APP_LOG_EN, "app_btmgr_aclConnectCB: 0x%x %d, 0x%08x, hfp_ChId[%d]", handle, status, *(uint32_t *)pBtAddr, hfp_ChId);
#if (TLK_CFG_UART_TOOL_ENABLE)
    tlkapp_btmgr_sendAclConnectEvt(handle, status, pBtAddr);
#endif


    if (status == BTH_HCI_ERROR_NONE) {
#if (TLK_MW_BTPAIRING_ENABLE)
        tlkmdi_btParing_stop();
#endif
        if (tlkmdi_btacl_getIdleCount() == 0) {
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
        } else {
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
        }
        //if (hfp_ChId == 0 && dtype == BTH_REMOTE_DTYPE_HEADSET) {
        //   btp_sdpclt_connect(handle);
        //}
    }
}

/**
 * @brief      Callback function invoked when ACL encryption is completed
 * @param[in]  handle    - Connection handle
 * @param[in]  status    - Encryption status
 * @param[in]  pBtAddr   - Pointer to Bluetooth address of remote device
 * @param[in]  dtype     - Device type
 * @param[in]  hfp_ChId  - HFP channel ID
 * @return     none
 */
static void app_btmgr_aclEncryptCB(uint16_t handle, uint8_t status, uint8_t *pBtAddr, uint8_t dtype, uint8_t hfp_ChId)
{
    (void)pBtAddr;
    (void)dtype;
    tlkapi_printf(APP_LOG_EN, "app_btmgr_aclEncryptCB:hfp_ChId =0x%x", hfp_ChId);
    if (status == TLK_ENONE) {
        if (hfp_ChId == 0) {
            btp_sdpclt_connect(handle); // refer to PC_Comp in app_btmgr_ProfConnCB
        } else {
            app_btmgr_appendProfile(handle);
        }
        tpsll_hci_sendSetHeadsetCurModeCmd(TPH_HOST_MODE_BT_ACTIVE);
    }
}

/**
 * @brief      Callback function invoked when ACL connection is disconnected
 * @param[in]  handle   - Connection handle
 * @param[in]  reason   - Disconnection reason
 * @param[in]  pBtAddr  - Pointer to Bluetooth address of remote device
 * @return     none
 */
static void app_btmgr_aclDisconnCB(uint16_t handle, uint8_t reason, uint8_t *pBtAddr)
{
#if (TLK_CFG_UART_TOOL_ENABLE)
    tlkapp_btmgr_sendAclDisconnEvt(handle, reason, pBtAddr);
#endif

#if (TLKBTP_CFG_IAP_ENABLE)
    tlkmdi_btiap_setAclHandle(false, handle);
#endif

    uint32_t taskID = handle + ((uint32_t)TLKAUD_TYPE_CC_BT_MUSIC << 16);
    tlkapp_audioScheduler_deleteTaskSafe(taskID);
    taskID = handle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
    tlkapp_audioScheduler_deleteTaskSafe(taskID);
#if TLK_MW_BTREC_ENABLE
    if (tlkmdi_btRecon_isInBusy()) {
        uint8_t *pPageAddr = tlkmdi_btRecon_getPageAddr();
        if (pPageAddr != NULL && tmemcmp(pBtAddr, pPageAddr, 6) == 0) {
            return; /* wait next retry, todo modify not wait 9s again syzhou*/
        }
    }
#endif

#if (TLK_MW_BTPAIRING_ENABLE)
    if (tlkmdi_btPairing_isInProgress() == false)
#endif
    {
#if TLK_MW_BTREC_ENABLE
        if (reason == BTH_HCI_ERROR_CONN_TIMEOUT) { //Start Reconnect
            uint32_t devClass = 0;
            int      sqlRet   = tlkmdi_tinySql_getPairingDeviceByAddr(pBtAddr, &devClass, NULL, NULL);
            if (sqlRet == TLK_ENONE) {
                if (tlkmdi_btacl_getUsedCount() == 0) {
                    tlkmdi_btRecon_start(pBtAddr, devClass, TLKMDI_BTRECON_RETRY_NUM_LINK_LOSS);
                    return;
                }
            } else if (sqlRet == -TLK_ENODATA) {
                tlkmdi_tinySql_deletePairingDevice(pBtAddr);
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
}

/**
 * @brief      Callback function invoked when a profile connection is established
 * @param[in]  handle       - Connection handle
 * @param[in]  status       - Connection status
 * @param[in]  ptype        - Profile type
 * @param[in]  usrID        - User ID
 * @param[in]  pBtAddr      - Pointer to Bluetooth address of remote device
 * @param[in]  isFirstProf  - Flag indicating if this is the first profile connection
 * @return     none
 */
static void app_btmgr_ProfConnCB(uint16_t handle, uint8_t status, uint8_t ptype, uint8_t usrID, uint8_t *pBtAddr, uint8_t isFirstProf)
{
    bth_acl_handle_t *pHandle;
    tlkapi_printf(APP_LOG_EN, "app_btmgr_ProfConnCB:{ptype-%d,usrID-%d,handle-%d,status-%d, isFirstProf-%d} ", ptype, usrID, handle, status, isFirstProf);
#if (TLK_CFG_UART_TOOL_ENABLE)
    if (ptype != BTP_PTYPE_SDP && ptype != BTP_PTYPE_RFC) {
        tlkapp_btmgr_sendProfConnectEvt(handle, status, ptype, usrID, pBtAddr);
    }
#endif

    if (status != TLK_ENONE) {
        return;
    }

    if (ptype == BTP_PTYPE_A2DP) {
        tlkapp_audioScheduler_taskInfo_t info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC,
            .optype    = TLKAUD_TYPE_CC_BT_MUSIC,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_CC_BT_MUSIC),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };
        uint32_t taskID = handle + ((uint32_t)TLKAUD_TYPE_CC_BT_MUSIC << 16);
        tlkapp_audioScheduler_updateTaskSafe(taskID, info, 0);

        btp_avrcp_connect(handle, usrID);
    } else if (ptype == BTP_PTYPE_HFP) {
        tlkapp_audioScheduler_taskInfo_t info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE,
            .optype    = TLKAUD_TYPE_CC_BT_VOICE,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_CC_BT_VOICE),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };
        uint32_t taskID = handle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
        tlkapp_audioScheduler_updateTaskSafe(taskID, info, 0);
    }
#if (TLKBTP_CFG_PBAP_ENABLE)
    else if (ptype == BTP_PTYPE_PBAP) {
        tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getUsedItem(handle);
        if (pItem == NULL) {
            return;
        }
        tlkmdi_pbap_phoneBookStartSync(handle, pBtAddr, false);
#if (TLKBTP_CFG_PBAP_ENABLE)
        tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pItem->pbapChannel, TLKMDI_BT_RFC_CHID_PBAP);
#endif // #if (TLKBTP_CFG_PBAP_ENABLE)
    }
#endif // #if (TLKBTP_CFG_PBAP_ENABLE)
    if (isFirstProf) {
        //btp_sdpclt_connect(handle); // PC_Comp. Fix(temp) PC(Chrome) first pairing device will connect timeout.
        pHandle = bth_handle_getUsedAcl(handle);
        if (pHandle != NULL) {
            tlkmdi_tinySql_updatePairingDevice(pBtAddr, &pHandle->devClass, pHandle->linkKey, pHandle->devName);
        }
        //save device class and linkkey

        //save rfc channel id
        tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getUsedItem(handle);
        if (pItem == NULL) {
            return;
        }

        if (!pItem->active) {
            if (pItem->agChannel != 0) {
                tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pItem->agChannel, TLKMDI_BT_RFC_CHID_HFP);
            }
        }
        tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pItem->sppChannel, TLKMDI_BT_RFC_CHID_SPP);
    }
}

/**
 * @brief      Callback function invoked when a profile is disconnected
 * @param[in]  handle   - Connection handle
 * @param[in]  reason   - Disconnection reason
 * @param[in]  ptype    - Profile type
 * @param[in]  usrID    - User ID
 * @param[in]  pBtAddr  - Pointer to Bluetooth address of remote device
 * @return     none
 */
static void app_btmgr_ProfDiscCB(uint16_t handle, uint8_t reason, uint8_t ptype, uint8_t usrID, uint8_t *pBtAddr)
{
#if (TLK_CFG_UART_TOOL_ENABLE)
    if (ptype != BTP_PTYPE_SDP && ptype != BTP_PTYPE_RFC) {
        tlkapp_btmgr_sendProfDisconnEvt(handle, reason, ptype, usrID, pBtAddr);
    }
#else
    (void)pBtAddr;
#endif
    tlkapi_printf(APP_LOG_EN, "app_btmgr_ProfDiscCB:{ptype-%d,usrID-%d,handle-%d,reason-%d} ", ptype, usrID, handle, reason);

    if (ptype == BTP_PTYPE_A2DP) {
        uint32_t taskID = handle + ((uint32_t)TLKAUD_TYPE_CC_BT_MUSIC << 16);
        tlkapp_audioScheduler_deleteTaskSafe(taskID);
    } else if (ptype == BTP_PTYPE_HFP) {
        uint32_t taskID = handle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
        tlkapp_audioScheduler_deleteTaskSafe(taskID);
    }
    if (ptype == BTP_PTYPE_SDP && usrID == BTP_USRID_CLIENT) {
        app_btmgr_appendProfile(handle);
    }
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

/**
 * @brief     Overwrite the hook function when the BT host init complete.
 *            Reg some callbacks
 * @param[in] None.
 * @returns   None.
 */
void tlkapp_host_bt_taskInitCompletedHook(void)
{
    tlkmdi_btacl_regConnectCB(app_btmgr_aclConnectCB);
    tlkmdi_btacl_regEncryptCB(app_btmgr_aclEncryptCB);
    tlkmdi_btacl_regDisconnCB(app_btmgr_aclDisconnCB);
    tlkmdi_btacl_regProfileConnectCB(app_btmgr_ProfConnCB);
    tlkmdi_btacl_regProfileDisconnCB(app_btmgr_ProfDiscCB);
}

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