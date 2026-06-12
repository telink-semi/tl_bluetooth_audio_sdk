/********************************************************************************************************
 * @file    tlksys_task.c
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
#include "tlksys_port.h"

static tlksys_task_t         sTlkSysTaskList[TLKSYS_TASKID_MAXNUM]          = {0};
static TlkOsMsgQHandle_t     sTlkSysTaskMsgQHandles[TLKSYS_TASKID_MAXNUM]   = {0};
static TlkOsEventTabHandle_t sTlkSysTaskEvtTabHandles[TLKSYS_TASKID_MAXNUM] = {0};

/**
 * @brief  Handle task wake up event
 * @param  None.
 * @returns  None.
 */
static void tlksys_task_handleWakeUp(void)
{
    //note:need this function do nothing,just wake up thread
}

/**
 * @brief  Handle task message
 * @param[in] pTask : Pointer to task structure
 * @returns  None.
 */
static void tlksys_task_handleMsg(const tlksys_task_t *pTask)
{
    while (1) {
        uint8_t buffer[128];
        int     res = tlkos_msgq_wait(sTlkSysTaskMsgQHandles[pTask->taskID], buffer, NULL, 128, 0);
        if (res != TLK_ENONE || pTask->pCfgs->Input == NULL) {
            return;
        }
        uint16_t msgID  = ((uint16_t)buffer[1] << 8) | buffer[0];
        uint16_t msgLen = ((uint16_t)buffer[3] << 8) | buffer[2];
        uint8_t *pdata  = NULL;
        if (msgLen != 0) {
            pdata = buffer + 4;
        }
        pTask->pCfgs->Input(msgID, pdata, msgLen);
    }
}

/**
 * @brief  Convert task ID to debug IO
 * @param[in] id : Task ID
 * @returns  Debug IO ID
 */
static uint16_t tlksys_task_id2DebugIo(uint8_t id)
{
    static const uint16_t tab[TLKSYS_TASKID_MAXNUM] = {
        [TLKSYS_TASKID_SYSTEM] = TLKOS_DEBUG_IO_SYS_THREAD,
        [TLKSYS_TASKID_AUDIO]  = TLKOS_DEBUG_IO_AUD_M_THREAD,
        [TLKSYS_TASKID_HOST]   = TLKOS_DEBUG_IO_HOST_THREAD,
#if TLKAPP_LEMGR_ENABLE
        [TLKSYS_TASKID_LEMGR] = TLKOS_DEBUG_IO_NUM,
#endif
    };
    if (id >= TLKSYS_TASKID_MAXNUM) {
        return TLKOS_DEBUG_IO_NUM;
    }
    return tab[id];
}

/**
 * @brief  Template task function
 * @param[in] arg : Task argument
 * @returns  None.
 */
static void tlksys_template_task(void *arg)
{
    tlksys_task_t *task = (tlksys_task_t *)arg;
    tlkos_task_delayMs(1);
    if (task->pCfgs->Start != NULL) {
        task->pCfgs->Start();
    }
    while (1) {
        tlkos_debug_ioCtrl(tlksys_task_id2DebugIo(task->taskID), 1);
        tlksys_task_handleMsg(task);
        tlkapi_timerList_handler(&task->timerList);
        if (task->pCfgs->Handler != NULL) {
            task->pCfgs->Handler();
        }
        uint32_t blockTimeMs = tlkos_task_nextIntvUsToMs(tlkapi_timerList_getNextTimeUs(&task->timerList), 1, TLKOS_WAIT_FOREVER);
        if (tlkos_task_getPriority(NULL) != task->pCfgs->priority) {
            tlkos_task_setPriority(NULL, task->pCfgs->priority);
        }
        tlkos_debug_ioCtrl(tlksys_task_id2DebugIo(task->taskID), 0);
        tlksys_task_waitEvent(task->taskID, blockTimeMs);
    }
}

/**
 * @brief  Idle wake timer callback
 * @param[in] timerHandle : Timer handle
 * @param[in] pUsrArg : User argument
 * @returns  None.
 */
static void tlksys_task_idleWakeTimerCB(TlkOsTimerHandle_t timerHandle, void *pUsrArg)
{
    (void)timerHandle;
    (void)pUsrArg;
}

/**
 * @brief  Check if all tasks are idle
 * @param  None.
 * @returns  Result of check
 * @retval   true - All tasks are idle
 * @retval   false - At least one task is not idle
 */
static bool tlksys_task_isAllIdle(void)
{
    for (size_t index = 0; index < TLKSYS_TASKID_MAXNUM; index++) {
        tlksys_task_t *pTask = &sTlkSysTaskList[index];
        if (pTask->state != TLKSYS_TASK_STATE_RUN_IN_MAIN_THREAD) {
            continue;
        }
        uint32_t evt = 0xFFFFFFFF;
        tlkos_event_get(sTlkSysTaskEvtTabHandles[index], &evt);
        if (evt) {
            return false;
        }
    }
    return true;
}

/**
 * @brief  Enter idle state
 * @param  None.
 * @returns  None.
 */
static void tlksys_task_enterIdle(void)
{
    uint32_t wfiTimeMs = TLKOS_WAIT_FOREVER;
    for (size_t index = 0; index < TLKSYS_TASKID_MAXNUM; index++) {
        tlksys_task_t *pTask = &sTlkSysTaskList[index];
        if (pTask->state != TLKSYS_TASK_STATE_RUN_IN_MAIN_THREAD) {
            continue;
        }
        uint32_t blockTimeMs = tlkos_task_nextIntvUsToMs(tlkapi_timerList_getNextTimeUs(&pTask->timerList), 1, TLKOS_WAIT_FOREVER);
        wfiTimeMs            = min(wfiTimeMs, blockTimeMs);
    }

    wfiTimeMs = min(wfiTimeMs, tlkos_timer_getNextTaskTimeMs());
    wfiTimeMs = min(wfiTimeMs, 50);

    static TlkOsTimerHandle_t tlksys_task_idleWakeTimer = NULL;
    if (tlksys_task_idleWakeTimer == NULL) {
        tlkos_timer_create("wfi", 1, false, tlksys_task_idleWakeTimerCB, NULL, &tlksys_task_idleWakeTimer);
    }
    tlkos_timer_stop(tlksys_task_idleWakeTimer);
    tlkos_timer_setPeriod(tlksys_task_idleWakeTimer, wfiTimeMs);
    if (!tlksys_task_isAllIdle()) {
        return;
    }
    if (tlksys_pm_isAllowedEnterSuspend()) {
        bool isLeave = tlksys_enter_suspend(wfiTimeMs, NULL); //1: enter suspend, 0: not enter suspend
        if (isLeave) {
            tlksys_devMgr_leaveSuspend();
        }
    } else {
        tlkos_timer_start(tlksys_task_idleWakeTimer);
        tlksys_enter_wfi();
    }
}

/**
 * @brief  Handle idle task
 * @param  None.
 * @returns  None.
 */
static void tlksys_task_idleTask(void)
{
    if (!tlksys_task_isAllIdle()) {
        return;
    }
    tlksys_task_enterIdle();
}

/**
 * @brief  Start all registered tasks
 * @param  None.
 * @returns  None.
 */
void tlksys_task_start(void)
{
    for (size_t index = 0; index < TLKSYS_TASKID_MAXNUM; index++) {
        tlksys_task_t *pTask = &sTlkSysTaskList[index];
        if (pTask->state != TLKSYS_TASK_STATE_RUN_IN_MAIN_THREAD) {
            continue;
        }
        if (pTask->pCfgs->Start != NULL) {
            pTask->pCfgs->Start();
        }
    }
}

/**
 * @brief  Handle all registered tasks
 * @param  None.
 * @returns  None.
 */
void tlksys_task_handler(void)
{
    for (size_t index = 0; index < TLKSYS_TASKID_MAXNUM; index++) {
        tlksys_task_t *pTask = &sTlkSysTaskList[index];
        if (pTask->state != TLKSYS_TASK_STATE_RUN_IN_MAIN_THREAD) {
            continue;
        }
        tlkos_event_wait(sTlkSysTaskEvtTabHandles[index], 0);
        tlkapi_timerList_handler(&pTask->timerList);
        if (pTask->pCfgs->Handler != NULL) {
            pTask->pCfgs->Handler();
        }
    }
    tlksys_task_idleTask();
}

/**
 * @brief  Create a new task with specified configuration
 * @param[in] taskID : Task ID to create
 * @param[in] pCfgs : Pointer to task configuration structure
 * @returns  Result of task creation
 * @retval   TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_task_create(uint16_t taskID, const tlksys_task_cfg_t *pCfgs)
{
    if (pCfgs == NULL || taskID >= TLKSYS_TASKID_MAXNUM) {
        return -TLK_EPARAM;
    }
    tlksys_task_t *pTask = &sTlkSysTaskList[taskID];
    if (pTask->state != TLKSYS_TASK_STATE_NOINIT) {
        return -TLK_EREPEAT;
    }
    pTask->pCfgs              = (tlksys_task_cfg_t *)pCfgs;
    uint8_t *pStaticBuffer    = pTask->pCfgs->pTaskStaticBuffer;
    uint16_t staticBufferSize = pTask->pCfgs->stackSize;
    if (pStaticBuffer != NULL && staticBufferSize > 512) {
        //static create msgq and evtTab
        uint32_t len = tlkos_event_getStaticBufferLen(16);
        tlkos_event_createTabStatic(16, pStaticBuffer, len, &sTlkSysTaskEvtTabHandles[taskID]);
        staticBufferSize -= len;
        pStaticBuffer += len;

        len = tlkos_msgq_getStaticBufferLen(16);
        tlkos_msgq_createStatic(&sTlkSysTaskMsgQHandles[taskID], 128, 16, pStaticBuffer, len);
        staticBufferSize -= len;
        pStaticBuffer += len;
    } else {
        //dynamic create msgq and evtTab
        tlkos_event_createTab(16, &sTlkSysTaskEvtTabHandles[taskID]);
        tlkos_msgq_create(&sTlkSysTaskMsgQHandles[taskID], 128, 16);
    }

    if (pCfgs->Init != NULL) {
        pCfgs->Init();
    }

    tlkos_event_regDealCB(sTlkSysTaskEvtTabHandles[taskID], 0, tlksys_task_handleWakeUp);
    pTask->taskID = taskID;
    //following code: create new thread
    pTask->state             = TLKSYS_TASK_STATE_RUN_IN_INDEPENDENCE_THREAD;
    TlkosTaskExtCfg_t extCfg = {
        .pStaticBuffer    = pStaticBuffer,
        .staticBufferSize = staticBufferSize,
    };
    int ret = tlkos_task_create(tlksys_template_task, pCfgs->pTaskName, pCfgs->stackSize, pCfgs->priority, pTask, &extCfg, &pTask->taskHandle);
    if (ret != TLK_ENONE) {
        pTask->state = TLKSYS_TASK_STATE_RUN_IN_MAIN_THREAD;
    }
    return TLK_ENONE;
}

/**
 * @brief  Send a message to specified task
 * @param[in] taskID : Target task ID
 * @param[in] msgID : Message ID
 * @param[in] pData : Pointer to message data
 * @param[in] dataLen : Length of message data
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_task_input(uint16_t taskID, uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    if (taskID >= TLKSYS_TASKID_MAXNUM || sTlkSysTaskList[taskID].state == TLKSYS_TASK_STATE_NOINIT) {
        return -TLK_EPARAM;
    }
    if (sTlkSysTaskList[taskID].pCfgs->Input == NULL) {
        return -TLK_ENOSUPPORT;
    }
    if (sTlkSysTaskList[taskID].state != TLKSYS_TASK_STATE_RUN_IN_INDEPENDENCE_THREAD) {
        return sTlkSysTaskList[taskID].pCfgs->Input(msgID, pData, dataLen);
    }
    if (tlkos_task_getRunningTask() == sTlkSysTaskList[taskID].taskHandle) {
        return sTlkSysTaskList[taskID].pCfgs->Input(msgID, pData, dataLen);
    }
    //to other thread:
    uint8_t buffer[dataLen + 4];
    buffer[0] = (msgID & 0xFF);
    buffer[1] = (msgID & 0xFF00) >> 8;
    buffer[2] = (dataLen & 0xFF);
    buffer[3] = (dataLen & 0xFF00) >> 8;
    tmemcpy(buffer + 4, pData, dataLen);
    int res = tlkos_msgq_send(sTlkSysTaskMsgQHandles[taskID], buffer, dataLen + 4, TLKOS_WAIT_FOREVER);
    if (res == TLK_ENONE) {
        tlksys_task_setEvt(taskID, TLKSYS_TASK_EVT_CORE);
    }
    return res;
}

/**
 * @brief  Get timer list of specified task
 * @param[in] taskID : Task ID to get timer list
 * @returns  Pointer to timer list of the task
 */
_always_inline TlkApiTimerList_t *tlksys_task_getTimerList(uint16_t taskID)
{
    if (taskID >= TLKSYS_TASKID_MAXNUM) {
        return NULL;
    }
    return &sTlkSysTaskList[taskID].timerList;
}

/**
 * @brief  Register event callback for specified task
 * @param[in] taskID : Task ID to register callback
 * @param[in] index : Event index
 * @param[in] cb : Callback function to register
 * @param[in] isFirstSet : Is need set event when register.
 * @returns  None.
 */
void tlksys_task_regEvtCBEx(uint16_t taskID, uint32_t index, TlkOsEventDealCB cb, uint8_t isFirstSet)
{
    tlkos_event_regDealCB(sTlkSysTaskEvtTabHandles[taskID], index, cb);
    if (isFirstSet) {
        tlkos_event_set(sTlkSysTaskEvtTabHandles[taskID], index);
    }
}

/**
 * @brief  Register event callback for specified task
 * @param[in] taskID : Task ID to register callback
 * @param[in] index : Event index
 * @param[in] cb : Callback function to register
 * @returns  None.
 */
void tlksys_task_regEvtCB(uint16_t taskID, uint32_t index, TlkOsEventDealCB cb)
{
    tlksys_task_regEvtCBEx(taskID, index, cb, 1);
}

/**
 * @brief  Set event for specified task
 * @param[in] taskID : Target task ID
 * @param[in] id : Event ID to set
 * @returns  None.
 */
void tlksys_task_setEvt(uint16_t taskID, uint32_t id)
{
    tlkos_event_set(sTlkSysTaskEvtTabHandles[taskID], id);
}

/**
 * @brief  Set event for specified task from ISR context
 * @param[in] taskID : Target task ID
 * @param[in] id : Event ID to set
 * @returns  None.
 */
_attribute_ram_code_sec_ void tlksys_task_setEvtFromIsr(uint16_t taskID, uint32_t id)
{
    tlkos_event_setFromIsr(sTlkSysTaskEvtTabHandles[taskID], id);
}

/**
 * @brief  Wait for event of specified task
 * @param[in] taskID : Task ID to wait for event
 * @param[in] blockTimeMs : Maximum time to wait in milliseconds
 * @returns  None.
 */
void tlksys_task_waitEvent(uint16_t taskID, uint32_t blockTimeMs)
{
    tlkos_event_wait(sTlkSysTaskEvtTabHandles[taskID], blockTimeMs);
}

/**
 * @brief  Set priority of specified task from ISR context
 * @param[in] taskID : Target task ID
 * @param[in] priority : New priority value
 * @returns  None.
 */
_attribute_ram_code_sec_ void tlksys_task_setPriorityFromIsr(uint16_t taskID, uint32_t priority)
{
    if (sTlkSysTaskList[taskID].taskHandle == NULL) {
        return;
    }
    tlkos_task_setPriorityFromIsr(sTlkSysTaskList[taskID].taskHandle, priority);
}
