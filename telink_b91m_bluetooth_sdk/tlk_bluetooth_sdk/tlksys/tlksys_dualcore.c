/********************************************************************************************************
 * @file    tlksys_dualcore.c
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
#include "drivers.h"
#include "stack/multiCoreComm/service/service_mailbox.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"

#if (MCU_DUAL_CORE_ENABLE && !MCU_CORE_N22)

static TlksysHandleSyncDataCB sTlksysHandleSyncDataCBs[TLKSYS_REQ_HANDLE_SYNC_DATA_MAX];

/**
 * @brief  Shared memory ready callback function
 * @param[in] data : Pointer to data
 * @returns  None.
 */
_attribute_ram_code_sec_ static void tlksys_smReadyCB(uint8_t *data)
{
    tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_GET_MAILBOX, 1);
    uint8_t type = data[0];
    if (type == TLK_SM_DATA_READY_TYPE_HCI) {
        tlksys_task_setEvtFromIsr(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_HCI);
        if (tlk_share_memory_service_hci_isOverTargetPercent(50)) {
            tlksys_task_setPriorityFromIsr(TLKSYS_TASKID_HOST, TLKSYS_TASK_AUDIO_IRQ_PRIORITY + 1);
        }
    } else if (type == TLK_SM_DATA_READY_TYPE_LOG) {
        tlksys_task_setEvtFromIsr(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_PRINT_LOG);
    }
    tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_GET_MAILBOX, 0);
}

/**
 * @brief  Handle synchronized data callback function
 * @param[in] data : Pointer to data
 * @returns  None.
 */
_attribute_ram_code_sec_ static void tlksys_handleSyncDataCB(uint8_t *data)
{
    uint8_t  type = data[0];
    uint16_t dataLen;
    uint32_t addr;
    ARRAY_TO_UINT16L(data, 1, dataLen);
    ARRAY_TO_UINT32L(data, 3, addr);
    addr |= 0x80000000; //d25f read n22 need this
    volatile uint32_t *finishFlag = (volatile uint32_t *)addr;
    if (type >= TLKSYS_REQ_HANDLE_SYNC_DATA_MAX || sTlksysHandleSyncDataCBs[type] == NULL) {
        *finishFlag = 0XFFFFFFFF;
        return;
    }
    sTlksysHandleSyncDataCBs[type]((void *)(addr + 4), dataLen);
    *finishFlag = 1;
}

/**
 * @brief  Initialize dual-core communication module
 * @param  None.
 * @returns  None.
 */
void tlksys_dualcore_init(void)
{
    tlk_mailbox_register_message_cb(TLK_MESSAGE_FROM_N22_TO_D25F_SM_DATA_READY, tlksys_smReadyCB);
    tlk_mailbox_register_message_cb(TLK_MESSAGE_FROM_N22_TO_D25F_REQ_HANDLE_SYNC_DATA, tlksys_handleSyncDataCB);
}

/**
 * @brief  Register callback for handling synchronized data
 * @param[in] type : Type of synchronized data
 * @param[in] cb : Callback function to register
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_regHandleSyncDataCb(uint8_t type, TlksysHandleSyncDataCB cb)
{
    if (type >= TLKSYS_REQ_HANDLE_SYNC_DATA_MAX) {
        return -TLK_EQUOTA;
    }
    sTlksysHandleSyncDataCBs[type] = cb;
    return TLK_ENONE;
}

#else

/**
 * @brief  Initialize dual-core communication module (dummy implementation)
 * @param  None.
 * @returns  None.
 */
void tlksys_dualcore_init(void) {}

/**
 * @brief  Register callback for handling synchronized data (dummy implementation)
 * @param[in] type : Type of synchronized data
 * @param[in] cb : Callback function to register
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_regHandleSyncDataCb(uint8_t type, TlksysHandleSyncDataCB cb)
{
    (void)type;
    (void)cb;
    return -TLK_ENOSUPPORT;
}

#endif
