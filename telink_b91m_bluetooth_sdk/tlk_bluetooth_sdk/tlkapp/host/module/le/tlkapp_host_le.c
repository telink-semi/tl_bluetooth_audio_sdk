/********************************************************************************************************
 * @file    tlkapp_host_le.c
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
#include "stack/ble/host_v1/inc/ble_host_sal.h"

#include "stack/multiCoreComm/service/service_shareMemory.h"

#if TLK_STK_BLE_ENABLE

/**
 * @brief       Performs internal initialization for the LE module.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_host_le_innerInit(void)
{
#if MCU_DUAL_CORE_ENABLE
    extern void ble_host_hci_rx_complete_packet(uint8_t * data, uint32_t len);
    tlk_d25f_register_hci_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_BLE, ble_host_hci_rx_complete_packet);
#endif
    tlksys_task_regEvtCB(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_LE_EVT, ble_host_sal_get_task());
    tlkapp_host_le_init();
}

/**
 * @brief       Starts the LE module internally.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_host_le_innerStart(void)
{
    tlkapp_host_le_start();
}

/**
 * @brief       Gets the LE module.
 * @param[in]   none.
 * @return      Returns a pointer to the LE module.
 */
TlkAppHostModule_t *tlkapp_host_le_getModule(void)
{
    static const TlkAppHostModuleCfg_t cfgs = {
        .hostType = TLKSYS_MSG_HOST_TYPE_LE,
        .init     = tlkapp_host_le_innerInit,
        .start    = tlkapp_host_le_innerStart,
    };
    static TlkAppHostModule_t module = {
        .cfgs = &cfgs,
    };
    return &module;
}

/**
 * @brief     Provides a weak function for le host init in app.
 * @param[in] None.
 * @returns   None.
 */
__attribute__((weak)) void tlkapp_host_le_init(void) {}

/**
 * @brief     Provides a weak function for le host start in app.
 * @param[in] None.
 * @returns   None.
 */
__attribute__((weak)) void tlkapp_host_le_start(void) {}

#endif
