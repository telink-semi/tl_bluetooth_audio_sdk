/********************************************************************************************************
 * @file    tlkapp_host_bt.c
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
#include "stack/bt/host/tlkstk_stdio.h"
#include "stack/bt/host/bt_hci.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#if (TLK_STK_BT_ENABLE)

/**
 * @brief       This function handles pairing active UI state changes.
 * @param[in]   state    - pairing state.
 * @return      none.
 */
static void tlkapp_btmgr_pairingActiveUI(uint8_t state)
{
    if(state == TLKMDI_BTPAIRING_CB_STATE_ACTIVE){
        tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_SYS, 0, TLKAPP_UI_STATE_PARING);
    }else{
        tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_SYS, 0, TLKAPP_UI_STATE_IDLE);
    }
    
}

/**
 * @brief       This function initializes the Bluetooth host module.
 * @param[in]   none    - none.
 * @return      none.
 */
static void tlkapp_host_bt_init(void)
{
	(void) tlkapp_btmgr_pairingActiveUI;
#if MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22)
    tlk_d25f_register_hci_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_BT, tlkbt_hci_recvC2HData);
#endif
    tlkmdi_bt_init();
#if (TLKBTP_CFG_IAP_ENABLE)
    tlksys_task_regEvtCB(TLKSYS_TASKID_HOST,TLKSYS_TASK_EVT_HOST_IAP2,tlkmdi_btiap_handler);
#endif
#if(TLK_MW_BTPAIRING_ENABLE)
    tlkmdi_btPairing_regCB(tlkapp_btmgr_pairingActiveUI);
#endif
    tlkapp_host_bt_taskInitCompletedHook();
}

/**
 * @brief       This function starts the Bluetooth host module.
 * @param[in]   none    - none.
 * @return      none.
 */
static void tlkapp_host_bt_start(void)
{
    tlkapp_host_bt_taskStartHook();
}

/**
 * @brief       This function gets the Bluetooth host module instance.
 * @param[in]   none    - none.
 * @return      pointer to the Bluetooth host module.
 */
TlkAppHostModule_t* tlkapp_host_bt_getModule(void)
{
    static const TlkAppHostModuleCfg_t cfgs = {
        .hostType = TLKSYS_MSG_HOST_TYPE_BT,
        .init    = tlkapp_host_bt_init,
        .start   = tlkapp_host_bt_start,
        .input   = tlkapp_btmgr_msgHandle,
    };
    static TlkAppHostModule_t module = {
        .cfgs = &cfgs,
    };
    return &module;
}

#endif // #if (TLK_STK_BT_ENABLE)

