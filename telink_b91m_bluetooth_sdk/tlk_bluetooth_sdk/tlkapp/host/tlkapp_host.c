/********************************************************************************************************
 * @file    tlkapp_host.c
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

#if TLKSTK_HOST_ENABLE

#ifndef TLKAPP_HOST_TASK_STASK_SIZE
#define TLKAPP_HOST_TASK_STASK_SIZE (5 * 1024)
#endif

/**
 * @brief       Handles HCI events for the host.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_host_hci_handler(void)
{
    void tlk_share_memory_service_hci_handler(void);
    if (tlkmw_host_isC2hPending()) {
        return;
    }
    tlk_share_memory_service_hci_handler();
}

/**
 * @brief       Initializes the host module.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_host_init(void)
{
    tlkmw_host_init();
    tlksys_task_regEvtCB(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_HCI, tlkapp_host_hci_handler);
#if (TLK_STK_BT_ENABLE)
    tlkapp_host_addModule(tlkapp_host_bt_getModule());
#endif
#if (TLK_STK_BLE_ENABLE)
    tlkapp_host_addModule(tlkapp_host_le_getModule());
#endif
#if (TLK_STK_BT_TPSLL_ENABLE)
    tlkapp_host_addModule(tlkapp_host_tph_getModule());
#endif
#if (TLKSTK_BTTPSLL_TWS_ENABLE)
    tlkapp_host_addModule(tlkapp_host_tpt_getModule());
#endif
#if (TLK_STK_TPD_ENABLE)
    tlkapp_host_addModule(tlkapp_host_tpd_getModule());
#endif
#if (TLK_STK_TPMD_ENABLE)
    tlkapp_host_addModule(tlkapp_host_tpmd_getModule());
#endif
}

/**
 * @brief       Starts all modules in the host.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_host_start(void)
{
    tlkapp_host_startAllModules();
}

/**
 * @brief       Handles input messages for the host.
 * @param[in]   msgID    - The message ID.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
static int tlkapp_host_input(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    if (msgID == TLKSYS_HOST_MSGID_RUN_FUNC) {
        const uint8_t headLen = sizeof(tlksys_requestRunFunc_t);
        if (dataLen == headLen) {
            tlksys_requestRunFunc_t func = *(tlksys_requestRunFunc_t *)pData;
            func();
        } else if (dataLen >= headLen) {
            tlksys_requestRunFuncWithArg_t func = *(tlksys_requestRunFuncWithArg_t *)pData;
            func(pData + headLen, dataLen - headLen);
        } else {
            return -TLK_EPARAM;
        }
        return TLK_ENONE;
    }
    return tlkapp_host_msgHandleAllModules(msgID, pData, dataLen);
}

/**
 * @brief       Handles all modules in the host.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_host_handler(void)
{
    tlkapp_host_handleAllModules();
}

/**
 * @brief       Gets the task configuration for the host task.
 * @param[in]   none.
 * @return      Returns a pointer to the task configuration.
 */
const tlksys_task_cfg_t *tlkapp_host_getTaskCfg(void)
{
    _attribute_os_heap_sec_ static uint8_t sTlkAppHostTaskBuffer[TLKAPP_HOST_TASK_STASK_SIZE];
    (void)sTlkAppHostTaskBuffer;
    static const tlksys_task_cfg_t sTlkAppHostTask = {
        .Init      = tlkapp_host_init,
        .Start     = tlkapp_host_start,
        .Input     = tlkapp_host_input,
        .Handler   = tlkapp_host_handler,
        .priority  = TLKSYS_TASK_HOST_PRIORITY,
        .stackSize = TLKAPP_HOST_TASK_STASK_SIZE,
        .pTaskName = "HOST",
#if TLK_CFG_RTOS_ENABLE
        .pTaskStaticBuffer = &sTlkAppHostTaskBuffer,
#endif
    };
    return &sTlkAppHostTask;
}

#endif
