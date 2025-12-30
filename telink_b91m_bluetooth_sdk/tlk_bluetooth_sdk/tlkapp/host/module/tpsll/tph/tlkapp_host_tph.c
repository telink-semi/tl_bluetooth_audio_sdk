/********************************************************************************************************
 * @file    tlkapp_host_tph.c
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
#include "tlkapp_host_tph_msg.h"
#if (TLK_STK_BT_TPSLL_ENABLE)

/**
 * @brief       Callback function for TPSLL Headset state changes.
 * @param[in]   state - The new state of the TPSLL Headset.
 * @return      none.
 */
static void tlkapp_bt_tph_stateChgCB(uint8_t state)
{
    if(state == TLKMDI_TPSLL_STATE_CHANGE_CB_PAIR){
        tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_SYS, 0, TLKAPP_UI_STATE_PARING);
    }else if(state == TLKMDI_TPSLL_STATE_CHANGE_CB_CONNECT){
        // 0xFFF0 TPSLL handle
        tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_TPSLL, 0xFFF0, TLKAPP_UI_STATE_CONNECTED);

        tlkapp_audioScheduler_taskInfo_t info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC,
            .optype    = TLKAUD_TYPE_TPH_AUDIO,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_TPH_AUDIO),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };

        uint32_t taskID = 0xFFF0 + ((uint32_t)TLKAUD_TYPE_TPH_AUDIO << 16);
        tlkapp_audioScheduler_updateTaskSafe(taskID, info, 0);
    }else if(state == TLKMDI_TPSLL_STATE_CHANGE_CB_DISCONNECT){
        // 0xFFF0 TPSLL handle
        tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_TPSLL, 0xFFF0, TLKAPP_UI_STATE_IDLE);

        uint32_t taskID = 0xFFF0 + ((uint32_t)TLKAUD_TYPE_TPH_AUDIO << 16);
        tlkapp_audioScheduler_deleteTaskSafe(taskID);
    }
}

/**
 * @brief       Initializes the TPSLL Headset module.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_host_tph_init(void)
{
    tlkmdi_bt_tph_init();
    tlkmdi_bt_tph_regStateChangeCB(tlkapp_bt_tph_stateChgCB);
}

/**
 * @brief       Handles events for the TPSLL Headset module.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_host_tph_handler(void)
{
    tlkmdi_bt_tph_handler();
}

/**
 * @brief       Gets the TPSLL Headset module.
 * @param[in]   none.
 * @return      Returns a pointer to the TPSLL Headset module.
 */
TlkAppHostModule_t* tlkapp_host_tph_getModule(void)
{
    static const TlkAppHostModuleCfg_t cfgs = {
        .hostType = TLKSYS_MSG_HOST_TYPE_TPH,
        .init    = tlkapp_host_tph_init,
        .handler = tlkapp_host_tph_handler,
        .input   = tlkapp_host_tph_msgHandle,
    };
    static TlkAppHostModule_t module = {
        .cfgs = &cfgs,
    };
    return &module;
}

#endif 

