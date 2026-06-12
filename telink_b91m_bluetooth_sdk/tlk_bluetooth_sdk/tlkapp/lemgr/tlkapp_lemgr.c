/********************************************************************************************************
 * @file    tlkapp_lemgr.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#if (TLKAPP_LEMGR_ENABLE)
#include "tlkapp/tlkapp.h"
#include "stack/ble/ble.h"

/**
 * @brief   Initialize the LE manager module.
 * @return  None.
 */
static void tlkapp_lemgr_init(void)
{
#if (TLKAPP_LEMGR_EXT_SCAN_ENABLE)
    ble_host_gap_ext_scan_init();
#endif
}

/**
 * @brief   Input handler for LE manager messages.
 * @param[in]   msgID    - Message ID.
 * @param[in]   pData    - Pointer to the message data.
 * @param[in]   dataLen  - Length of the message data.
 * @return      Status of the operation. TLK_ENOSUPPORT if UART tool is not enabled, otherwise return from tlkapp_lemgr_msgHandle.
 */
static int tlkapp_lemgr_input(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    (void)msgID;
    (void)pData;
    (void)dataLen;
#if (TLK_CFG_UART_TOOL_ENABLE)
    return tlkapp_lemgr_msgHandle(msgID, pData, dataLen);
#else
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief   Get the task configuration for LE manager.
 * @return  Pointer to the task configuration structure.
 */
const tlksys_task_cfg_t *tlkapp_lemgr_getTaskCfg(void)
{
    static const tlksys_task_cfg_t sTlkAppLemgrTask = {
        .Init  = tlkapp_lemgr_init,
        .Input = tlkapp_lemgr_input,
    };
    return &sTlkAppLemgrTask;
}

#endif
