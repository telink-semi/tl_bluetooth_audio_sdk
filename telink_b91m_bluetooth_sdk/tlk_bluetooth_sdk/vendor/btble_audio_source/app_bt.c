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
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#include "tlkmw/tlkmw.h"
static void app_btmgr_appendProfile(uint16_t aclHandle);
#if 0
void app_sdp_data_callback(uint16_t uuid, uint8_t *data, uint16_t dataLen, uint8_t remain_continue_bytes, uint8_t *remail_len_buf)
{
	(void ) *data;
    tlkapi_printf(APP_LOG_EN,
                  "app_btmgr_aclConnectCB: uuid :0x%x dataLen 0x%x, rcb 0x%x,rL0: 0x%x, rL1:0x%x",
				  uuid, dataLen, remain_continue_bytes, remain_continue_bytes?remail_len_buf[0]:0, remain_continue_bytes>1?remail_len_buf[1]:0);

	//tlkapi_send_string_data(APP_LOG_EN,"d:", data, dataLen>10?10:dataLen);

	//tlkapi_send_string_data(APP_LOG_EN,"cd:", remail_len_buf, remain_continue_bytes);

}
#endif
/**
 * @brief      Callback function when ACL connection is established
 * @param[in]  handle - connection handle
 * @param[in]  status - connection status
 * @param[in]  pBtAddr - bluetooth address of remote device
 * @param[in]  dtype - device type
 * @param[in]  hfp_ChId - HFP channel ID
 * @return     none
 */
static void app_btmgr_aclConnectCB(uint16_t handle, uint8_t status, uint8_t *pBtAddr, uint8_t dtype, uint8_t hfp_ChId)
{
    (void)dtype;
    tlkapi_printf(APP_LOG_EN, "app_btmgr_aclConnectCB: 0x%x %d, 0x%08x, hfp_ChId[%d]", handle, status, *(uint32_t *)pBtAddr, hfp_ChId);
    // btp_sdp_regDataCB(app_sdp_data_callback);
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
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 0);
        }

        //if (hfp_ChId == 0 && dtype == BTH_REMOTE_DTYPE_HEADSET) {
        //  btp_sdpclt_connect(handle);
        //}
    }
}

/**
 * @brief      Callback function when ACL encryption is completed
 * @param[in]  handle - connection handle
 * @param[in]  status - encryption status
 * @param[in]  pBtAddr - bluetooth address of remote device
 * @param[in]  dtype - device type
 * @param[in]  hfp_ChId - HFP channel ID
 * @return     none
 */
static void app_btmgr_aclEncryptCB(uint16_t handle, uint8_t status, uint8_t *pBtAddr, uint8_t dtype, uint8_t hfp_ChId)
{
    (void)pBtAddr;
    (void)dtype;
    tlkapi_printf(APP_LOG_EN, "app_btmgr_aclEncryptCB:hfp_ChId =0x%x", hfp_ChId);
    if (status == TLK_ENONE) {
        if (hfp_ChId == 0) {
            btp_sdpclt_connect(handle);
        } else {
            app_btmgr_appendProfile(handle);
        }
    }
}

/**
 * @brief      Callback function when ACL connection is disconnected
 * @param[in]  handle - connection handle
 * @param[in]  reason - disconnection reason
 * @param[in]  pBtAddr - bluetooth address of remote device
 * @return     none
 */
static void app_btmgr_aclDisconnCB(uint16_t handle, uint8_t reason, uint8_t *pBtAddr)
{
#if (TLK_CFG_UART_TOOL_ENABLE)
    tlkapp_btmgr_sendAclDisconnEvt(handle, reason, pBtAddr);
#endif

    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 0);

    uint32_t taskID = handle + ((uint32_t)TLKAUD_TYPE_A2DP_OUT << 16);
    tlkapp_audioScheduler_deleteTask(taskID);
    taskID = handle + ((uint32_t)TLKAUD_TYPE_U2H_VOICE << 16);
    tlkapp_audioScheduler_deleteTask(taskID);
}

/**
 * @brief      Callback function when profile connection is established
 * @param[in]  handle - connection handle
 * @param[in]  status - connection status
 * @param[in]  ptype - profile type
 * @param[in]  usrID - user ID
 * @param[in]  pBtAddr - bluetooth address of remote device
 * @param[in]  isFirstProf - flag indicating if this is the first profile connection
 * @return     none
 */
static void app_btmgr_ProfConnCB(uint16_t handle, uint8_t status, uint8_t ptype, uint8_t usrID, uint8_t *pBtAddr, uint8_t isFirstProf)
{
    bth_acl_handle_t *pHandle;
    tlkapi_printf(APP_LOG_EN, "app_btmgr_ProfConnCB:{ptype-%d,usrID-%d,handle-%d,reason-%d} ", ptype, usrID, handle, status);
#if (TLK_CFG_UART_TOOL_ENABLE)
    if (ptype != BTP_PTYPE_SDP && ptype != BTP_PTYPE_RFC) {
        tlkapp_btmgr_sendProfConnectEvt(handle, status, ptype, usrID, pBtAddr);
    }
#endif

    if (status != TLK_ENONE) {
        return;
    }
    if (ptype == BTP_PTYPE_A2DP && usrID == BTP_USRID_SERVER) {
        tlkapp_audioScheduler_taskInfo_t info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC,
            .optype    = TLKAUD_TYPE_A2DP_OUT,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_A2DP_OUT),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };
        uint32_t                          taskID     = handle + ((uint32_t)TLKAUD_TYPE_A2DP_OUT << 16);
        uint32_t                          sameTaskID = handle + ((uint32_t)TLKAUD_TYPE_U2H_VOICE << 16);
        tlkapp_audioScheduler_extraInfo_t exInfo     = {
#if (TLK_USB_UAC_ENABLE)
            .stateChangeCB = tlkapp_audioUac_taskStateChgCB,
#endif
        };
        tlkapp_audioScheduler_updateTaskEx(taskID, info, sameTaskID, exInfo);

        btp_avrcp_connect(handle, usrID);
    } else if (ptype == BTP_PTYPE_HFP && usrID == BTP_USRID_SERVER) {
        tlkapp_audioScheduler_taskInfo_t info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE,
            .optype    = TLKAUD_TYPE_U2H_VOICE,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_U2H_VOICE),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };
        uint32_t taskID     = handle + ((uint32_t)TLKAUD_TYPE_U2H_VOICE << 16);
        uint32_t sameTaskID = handle + ((uint32_t)TLKAUD_TYPE_A2DP_OUT << 16);
        tlkapp_audioScheduler_updateTask(taskID, info, sameTaskID);
        tlkapp_audioScheduler_extraInfo_t exInfo = {
#if (TLK_USB_UAC_ENABLE)
            .stateChangeCB = tlkapp_audioUac_taskStateChgCB,
#endif
        };
        tlkapp_audioScheduler_updateTaskEx(taskID, info, sameTaskID, exInfo);
    }

    if (isFirstProf) {
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
        if (pItem->active) {
            tlkmdi_tinySql_setPairingDeviceRfcChid(pItem->btaddr, pItem->hfChannel, TLKMDI_BT_RFC_CHID_HFP);
        }
    }
}

/**
 * @brief      Callback function when profile is disconnected
 * @param[in]  handle - connection handle
 * @param[in]  reason - disconnection reason
 * @param[in]  ptype - profile type
 * @param[in]  usrID - user ID
 * @param[in]  pBtAddr - bluetooth address of remote device
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

    if (ptype == BTP_PTYPE_A2DP && usrID == BTP_USRID_SERVER) {
        uint32_t taskID = handle + ((uint32_t)TLKAUD_TYPE_A2DP_OUT << 16);
        tlkapp_audioScheduler_deleteTask(taskID);
    } else if (ptype == BTP_PTYPE_HFP && usrID == BTP_USRID_SERVER) {
        uint32_t taskID = handle + ((uint32_t)TLKAUD_TYPE_U2H_VOICE << 16);
        tlkapp_audioScheduler_deleteTask(taskID);
        taskID = handle + ((uint32_t)TLKAUD_TYPE_CC_BT_VOICE << 16);
        tlkapp_audioScheduler_deleteTask(taskID);
    }

    tlkapi_printf(APP_LOG_EN, "app_btmgr_ProfDiscCB:{ptype-%d,usrID-%d,handle-%d,reason-%d} ", ptype, usrID, handle, reason);


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
    dtype   = bth_devClassToDevType(pItem->devClass);
    if (tlkmdi_btacl_isHaveRfc(aclHandle)) {
        tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_RFC, 0, delayMs);
        if (tlkmdi_btacl_isFindHfp(aclHandle)) {
            tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_HFP, BTP_USRID_NONE, delayMs + 200);
        }
        delayMs += 800;
    }

    if (dtype == BTH_REMOTE_DTYPE_HEADSET) {
        tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_A2DP, BTP_USRID_SERVER, delayMs + 200);
    }

    tlkmdi_btacl_appendProf(aclHandle, BTP_PTYPE_AVRCP, BTP_USRID_NONE, delayMs + 500);
}

/**
 * @brief      Handle ACL connection request event
 * @param[in]  devClass - device class
 * @param[in]  pBtAddr - bluetooth address of remote device
 * @return     status
 */
static int app_btacl_requestEvt(uint32_t devClass, uint8_t *pBtAddr)
{
    (void)pBtAddr;

    (void)devClass;

    return TLK_ENONE;
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
    tlkmdi_btacl_regConnectRequsetCB(app_btacl_requestEvt);
}

/**
 * @brief     Overwrite the hook function when the BT host task starts.
 *            Open bt scan when bt host task start.
 * @param[in] None.
 * @returns   None.
 */
void tlkapp_host_bt_taskStartHook(void)
{
    uint32_t devClass;
    uint8_t  devAddr[6];
    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
    int ret = tlkmdi_tinySql_getLastPairingDevice(devAddr, &devClass, NULL, NULL);
    if (ret == TLK_ENONE) {
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 120);
    } else {
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120); /* if no last device ,open both scan */
    }
}

__attribute__((noinline)) void tlkusb_debug_shell_hook(uint8_t *pData, uint16_t dataLen)
{
    tlkapi_send_string_data(APP_LOG_EN, "tlkusb_debug_shell_hook: received", pData, dataLen);
    if (pData[1] == 0x01) {
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_A2DP_OUT, 0x08);
    }
}