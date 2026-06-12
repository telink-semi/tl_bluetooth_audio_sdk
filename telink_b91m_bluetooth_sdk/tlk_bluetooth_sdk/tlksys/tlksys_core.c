/********************************************************************************************************
 * @file    tlksys_core.c
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
#include "tlksys_port.h"
#include "tlksys_dualcore.h"

typedef struct tlksys_irqmsg_node
{
    uint16_t                   destID;
    uint16_t                   msgID;
    uint32_t                   msgLen;
    struct tlksys_irqmsg_node *next;
    uint8_t                    data[0];
} tlksys_irqmsg_node_t;

static tlksys_irqmsg_node_t *sTlkSysIrqMsgHead = NULL;

static TlkOsMutexHandle_t sTlkMutexHandles[TLKSYS_MUTEX_NUM];

/**
 * @brief  Initialize the tlksys module
 * @param  None.
 * @returns  TLK_ENONE - Success. Otherwise, error code
 */
int tlksys_init(void)
{
    tlksys_hal_platform_init();
    tlksys_port_init();
    tlkos_init();
    tlksys_dualcore_init();
    tlksys_timer_coreInit();
    tlkos_recursiveMutex_createMultiple(sTlkMutexHandles, TLKSYS_MUTEX_NUM);
    tlksys_initFinishedHook();
    return TLK_ENONE;
}

/**
 * @brief  Start the tlksys module with specified function
 * @param[in] func : Initialization function to start with
 * @returns  None.
 */
void tlksys_start(TlkOsInitFunc_t func)
{
    tlkos_start(func);
    tlksys_task_start();
}

/**
 * @brief  Handle tlksys module operations
 * @param  None.
 * @returns  None.
 */
void tlksys_handler(void)
{
    while (sTlkSysIrqMsgHead != NULL) {
        tlksys_enter_critical();
        tlksys_irqmsg_node_t *pNode = sTlkSysIrqMsgHead;
        sTlkSysIrqMsgHead           = pNode->next;
        tlksys_leave_critical();

        tlksys_task_input(pNode->destID, pNode->msgID, pNode->data, pNode->msgLen);
        tlkos_free(pNode);
    }
    tlksys_task_handler();
}

/**
 * @brief  Send message from timer context to specified task
 * @param[in] taskID : Target task ID
 * @param[in] msgID : Message ID
 * @param[in] pData : Pointer to message data
 * @param[in] dataLen : Length of message data
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_sendMsgFromTimer(uint16_t taskID, uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
#if !TLK_CFG_RTOS_ENABLE
    uint32_t              totalSize = sizeof(tlksys_irqmsg_node_t) + dataLen;
    tlksys_irqmsg_node_t *pNode     = (tlksys_irqmsg_node_t *)tlkos_malloc(totalSize);
    if (pNode == NULL) {
        return -TLK_EQUOTA;
    }
    pNode->destID = taskID;
    pNode->msgID  = msgID;
    pNode->msgLen = dataLen;
    pNode->next   = NULL;
    tmemcpy(pNode->data, pData, dataLen);

    tlksys_enter_critical();
    if (sTlkSysIrqMsgHead == NULL) {
        sTlkSysIrqMsgHead = pNode;
    } else {
        tlksys_irqmsg_node_t *pTemp = sTlkSysIrqMsgHead;
        while (pTemp->next != NULL) {
            pTemp = pTemp->next;
        }
        pTemp->next = pNode;
    }
    tlksys_leave_critical();

    return TLK_ENONE;
#else
    return tlksys_sendMsg(taskID, msgID, pData, dataLen);
#endif
}

/**
 * @brief  Send message to specified task
 * @param[in] taskID : Target task ID
 * @param[in] msgID : Message ID
 * @param[in] pData : Pointer to message data
 * @param[in] dataLen : Length of message data
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_sendMsg(uint16_t taskID, uint16_t msgID, void *pData, uint16_t dataLen)
{
    return tlksys_task_input(taskID, msgID, pData, dataLen);
}

/**
 * @brief  Enter critical section
 * @param  None.
 * @returns  None.
 */
_always_inline void tlksys_enter_critical(void)
{
    tlkos_enter_critical();
}

/**
 * @brief  Leave critical section
 * @param  None.
 * @returns  None.
 */
_always_inline void tlksys_leave_critical(void)
{
    tlkos_leave_critical();
}

/**
 * @brief  Lock specified mutex
 * @param[in] mutexID : Mutex ID to lock
 * @returns  None.
 */
_always_inline void tlksys_mutex_lock(uint16_t mutexID)
{
    tlkos_recursiveMutex_lock(sTlkMutexHandles[mutexID]);
}

/**
 * @brief  Unlock specified mutex
 * @param[in] mutexID : Mutex ID to unlock
 * @returns  None.
 */
_always_inline void tlksys_mutex_unlock(uint16_t mutexID)
{
    tlkos_recursiveMutex_unlock(sTlkMutexHandles[mutexID]);
}

/**
 * @brief  Hook function called when initialization is finished
 * @param  None.
 * @returns  None.
 */
__attribute__((weak)) void tlksys_initFinishedHook(void) {}
