/********************************************************************************************************
 * @file    app_bt_peripheral.c
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
#include "tlkapp/tlkapp.h"
#include "tlkmw/tlkmw.h"

#if (TLK_RC_CFG_BT_PERIPHERAL_STREAM)
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

    tlkmdi_bthfp_destroy(event->aclHandle);
    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 0);
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

    if (event->status != TLK_ENONE) {
        return TLK_ENONE;
    }
    if (event->ptype == BTP_PTYPE_HFP && event->usrID == BTP_USRID_SERVER) {
        tlkapp_audioScheduler_taskInfo_t info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE,
            .optype    = TLKAUD_TYPE_CC_BT_VOICE,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_CC_BT_VOICE),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };
        uint32_t taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
        tlkapp_audioScheduler_updateTaskSafe(taskID, info, 0);
    }

    if (event->is_first_prof) {
        pHandle = bth_handle_getUsedAcl(event->aclHandle);
        if (pHandle != NULL) {
            tlkmdi_tinySql_updatePairingDevice(event->pBtAddr, &pHandle->devClass, pHandle->linkKey, pHandle->devName);
        }
        tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getUsedItem(event->aclHandle);
        if (pItem == NULL) {
            return -TLK_ENOITEM;
        }
        if (pItem->active) {
            tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pItem->hfChannel, TLKMDI_BT_RFC_CHID_HFP);
        }
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

    if (event->ptype == BTP_PTYPE_HFP && event->usrID == BTP_USRID_SERVER) {
        uint32_t taskID = event->aclHandle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
        tlkapp_audioScheduler_deleteTask(taskID);
    }

    if (event->ptype == BTP_PTYPE_SDP && event->usrID == BTP_USRID_CLIENT) {
        app_btmgr_appendProfile(event->aclHandle);
    }

    return TLK_ENONE;
}

static void app_btmgr_appendProfile(uint16_t aclHandle)
{
    tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getConnItem(aclHandle);
    if (pItem == NULL) {
        return;
    }
    uint16_t delayMs = pItem->active ? 0 : 2000;
    if (tlkmdi_btacl_isHaveRfc(aclHandle)) {
        tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_RFC, 0, delayMs);
        if (tlkmdi_btacl_isFindHfp(aclHandle)) {
            tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_HFP, BTP_USRID_NONE, delayMs + 200);
        }
        delayMs += 800;
    }
}

TLKMW_BT_EVT_REGISTER(TLKMW_BT_CONNECT_COMPLETE, app_btmgr_aclConnectCB);
TLKMW_BT_EVT_REGISTER(TLKMW_BT_DISCONNECT_COMPLETE, app_btmgr_aclDisconnCB);
TLKMW_BT_EVT_REGISTER(TLKMW_BT_ENCRYPTION_COMPLETE, app_btmgr_aclEncryptCB);
TLKMW_BT_EVT_REGISTER(TLKMW_BT_PROFILE_CONNECT, app_btmgr_ProfConnCB);
TLKMW_BT_EVT_REGISTER(TLKMW_BT_PROFILE_DISCONNECT, app_btmgr_ProfDiscCB);

/**
 * @brief     Overwrite the hook function when the BT host task starts.
 *            Open bt scan when bt host task start.
 * @param[in] None.
 * @returns   None.
 */
void tlkapp_host_bt_taskStartHook(void)
{
    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
}
#endif
