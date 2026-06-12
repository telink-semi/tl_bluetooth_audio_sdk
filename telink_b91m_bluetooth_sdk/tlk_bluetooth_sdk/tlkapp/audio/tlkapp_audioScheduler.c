/********************************************************************************************************
 * @file    tlkapp_audioScheduler.c
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
#if (TLK_MW_AUDIO_ENABLE)
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"

static inline __attribute__((always_inline)) uint32_t tlkapp_audioScheduler_getTimerTick(void)
{
    return stimer_get_tick();
}

static inline __attribute__((always_inline)) bool tlkapp_audioScheduler_timerExceed(uint32_t ref, uint32_t us)
{
    return clock_time_exceed(ref, us);
}

typedef struct
{
    uint32_t                     isUsedFlag; //bit x high means that mem offset x has been malloced
    tlkapp_audioScheduler_node_t nodes[TLKAPP_AUDIO_SCHEDULER_MAX_TASKNUM];
} tlkapp_audioScheduler_memPool_t;

static tlkapp_audioScheduler_memPool_t tlkapp_audioScheduler_memPool;
static tlkapp_audioScheduler_t         tlkapp_audioScheduler;

/*mempool note:
    this mempool can only malloc/free size = sizeof(tlkapp_audioScheduler_node_t)
    you can use std/tlklib's mempool malloc/free interface instead.
*/
/**
 * @brief       This function allocates memory for an audio scheduler node.
 * @param[in]   size - size of memory to allocate.
 * @return      Pointer to allocated memory, or NULL if allocation fails.
 */
static void *tlkapp_audioScheduler_malloc(uint32_t size)
{
    (void)size;
    tlkapp_audioScheduler_memPool_t *memPool = &tlkapp_audioScheduler_memPool;
    for (int i = 0; i < TLKAPP_AUDIO_SCHEDULER_MAX_TASKNUM; i++) {
        if ((memPool->isUsedFlag & (1 << i)) == 0) {
            memPool->isUsedFlag |= (1 << i);
            return &memPool->nodes[i];
        }
    }
    return NULL;
}

/**
 * @brief       This function frees memory of an audio scheduler node.
 * @param[in]   ptr - pointer to the memory to free.
 * @return      none.
 */
static void tlkapp_audioScheduler_free(void *ptr)
{
    uint32_t whichBit = ((tlkapp_audioScheduler_node_t *)ptr - &tlkapp_audioScheduler_memPool.nodes[0]);
    tlkapp_audioScheduler_memPool.isUsedFlag &= ~(1 << whichBit);
}

/**
 * @brief       This function tries to run the state change callback for a node.
 * @param[in]   node       - pointer to the audio scheduler node.
 * @param[in]   lastState  - the previous state of the node.
 * @param[in]   newState   - the new state of the node.
 * @return      none.
 */
static inline void tlkapp_audioScheduler_tryRunStateChgCB(tlkapp_audioScheduler_node_t *node, uint8_t lastState, uint8_t newState)
{
    if (node->extraInfo.stateChangeCB == NULL) {
        return;
    }
    node->extraInfo.stateChangeCB(node->taskId, lastState, newState, node->extraInfo.stateChangeCBUserArg);
}

/**
 * @brief       This function sets a new state for a node with state change callback.
 * @param[in]   node     - pointer to the audio scheduler node.
 * @param[in]   newState - the new state to set.
 * @return      none.
 */
static void tlkapp_audioScheduler_nodeSetNewStateWithChgCB(tlkapp_audioScheduler_node_t *node, uint8_t newState)
{
    uint8_t lastState = node->info.state;
    if (newState == lastState) {
        return;
    }
    node->info.state = newState;
    if (node->extraInfo.stateChangeCB == NULL) {
        return;
    }
    tlkapp_audioScheduler_tryRunStateChgCB(node, lastState, newState);
}

/**
 * @brief       This function finds a node by task ID.
 * @param[in]   taskId - task ID to search for.
 * @return      Pointer to the found node, or NULL if not found.
 */
static tlkapp_audioScheduler_node_t *tlkapp_audioScheduler_findNode(uint32_t taskId)
{
    for (int i = TLKAPP_AUDIO_SCHEDULER_PRIORITY_HIGHEST; i >= 0; i--) {
        tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler.idleTaskList[i];
        while (node != NULL) {
            if (node->taskId == taskId) {
                return node;
            }
            node = node->next;
        }
    }
    for (int i = TLKAPP_AUDIO_SCHEDULER_PRIORITY_HIGHEST; i >= 0; i--) {
        tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler.readyTaskList[i];
        while (node != NULL) {
            if (node->taskId == taskId) {
                return node;
            }
            node = node->next;
        }
    }
    tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler.runningTask;
    if (node == NULL || node->taskId == taskId) {
        return node;
    }
    return NULL;
}

/**
 * @brief       This function removes a node from its list.
 * @param[in]   node - pointer to the node to remove.
 * @return      none.
 */
static void tlkapp_audioScheduler_nodeRemoveFromList(tlkapp_audioScheduler_node_t *node)
{
    uint8_t state    = node->info.state;
    uint8_t priority = node->info.priority;
    if (node->prev == NULL) {
        if (state == TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE) {
            tlkapp_audioScheduler.idleTaskList[priority] = node->next;
        } else if (state == TLKAPP_AUDIO_SCHEDULER_TASK_STATE_PAUSED || state == TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY) {
            tlkapp_audioScheduler.readyTaskList[priority] = node->next;
        }
    } else {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    node->next = NULL;
    node->prev = NULL;
}

/**
 * @brief       This function pushes a node to the front of a list.
 * @param[in]   node - pointer to the node to push.
 * @param[in]   head - pointer to the head of the list.
 * @return      none.
 */
static void tlkapp_audioScheduler_nodePushFrontToList(tlkapp_audioScheduler_node_t *node, tlkapp_audioScheduler_node_t **head)
{
    if (*head != NULL) {
        (*head)->prev = node;
    }
    node->next = (*head);
    *head      = node;
    node->prev = NULL;
}

/**
 * @brief       This function pushes a node to the back of a list.
 * @param[in]   node - pointer to the node to push.
 * @param[in]   head - pointer to the head of the list.
 * @return      none.
 */
static void tlkapp_audioScheduler_nodePushBackToList(tlkapp_audioScheduler_node_t *node, tlkapp_audioScheduler_node_t **head)
{
    tlkapp_audioScheduler_node_t *lastNode = *head;
    if (lastNode == NULL) {
        *head      = node;
        node->prev = NULL;
    } else {
        while (lastNode->next != NULL) {
            lastNode = lastNode->next;
        }
        lastNode->next = node;
        node->prev     = lastNode;
    }
    node->next = NULL;
}

/**
 * @brief       This function adds a same device task to a node.
 * @param[in]   node        - pointer to the node.
 * @param[in]   sameDevNode - pointer to the same device node.
 * @return      none.
 */
static inline void tlkapp_audioScheduler_addNewSameDevToTask(tlkapp_audioScheduler_node_t *node, tlkapp_audioScheduler_node_t *sameDevNode)
{
    if (node->sameDevTask != NULL) {
        node->sameDevTask->sameDevTask = NULL;
    }
    node->sameDevTask = sameDevNode;
    if (node->sameDevTask != NULL) {
        node->sameDevTask->sameDevTask = node;
    }
}

/**
 * @brief       This function moves a same device task to the first position.
 * @param[in]   node - pointer to the node.
 * @return      none.
 */
static void tlkapp_audioScheduler_sameDevToFirst(tlkapp_audioScheduler_node_t *node)
{
    if (node == NULL) {
        return;
    }
    tlkapp_audioScheduler_node_t *sameDevTaskNode = node->sameDevTask;
    if (sameDevTaskNode == NULL) {
        return;
    }
    uint8_t sameDevTaskPriority = sameDevTaskNode->info.priority;
    uint8_t nodePriority        = node->info.priority;
    bool    juFlag              = false;
    if ((tlkapp_audioScheduler.cfg & TLKAPP_AUDIO_SCHEDULER_CFG_SAME_DEVICE_REFRESH_L2H) && nodePriority < sameDevTaskPriority) {
        juFlag = true;
    }
    if ((tlkapp_audioScheduler.cfg & TLKAPP_AUDIO_SCHEDULER_CFG_SAME_DEVICE_REFRESH_H2L) && nodePriority > sameDevTaskPriority) {
        juFlag = true;
    }
    if (juFlag) {
        tlkapp_audioScheduler_nodeRemoveFromList(sameDevTaskNode);
        if (sameDevTaskNode->info.state == TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE) {
            tlkapp_audioScheduler_nodeSetNewStateWithChgCB(sameDevTaskNode, TLKAPP_AUDIO_SCHEDULER_TASK_STATE_PAUSED);
        }
        tlkapp_audioScheduler_nodePushFrontToList(sameDevTaskNode, &tlkapp_audioScheduler.readyTaskList[sameDevTaskPriority]);
    }
}

/**
 * @brief       This function switches the audio type of the currently running task.
 * @param[in]   audioType - the audio type to switch to.
 * @return      true if successful, false otherwise.
 */
static inline bool tlkapp_audioScheduler_runningTaskSwitchTypeCore(uint8_t audioType)
{
    (void)audioType;
    tlkapp_audioScheduler_node_t *runningTask = tlkapp_audioScheduler.runningTask;

    uint8_t newType = TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE;

    return tlkapp_audio_modinfOperate((uint16_t)runningTask->taskId, runningTask->info.optype, &newType, 1);
}

/**
 * @brief       This function transitions a task to the running state.
 * @param[in]   node             - pointer to the node.
 * @param[in]   isAutoOpenNewTask - whether to automatically open the new task.
 * @return      none.
 */
static inline void tlkapp_audioScheduler_taskToRunning(tlkapp_audioScheduler_node_t *node, bool isAutoOpenNewTask)
{
    tlkapp_audioScheduler_node_t *nowRunningTask = tlkapp_audioScheduler.runningTask;
    if (isAutoOpenNewTask) {
        tlkapp_audioScheduler_nodeRemoveFromList(node);
        tlkapp_audioScheduler.runningTask = node;
        tlkapp_audioScheduler_nodeSetNewStateWithChgCB(node, TLKAPP_AUDIO_SCHEDULER_TASK_STATE_RUNNING);
    } else {
        tlkapp_audioScheduler_sameDevToFirst(nowRunningTask);
        tlkapp_audioScheduler.runningTask = NULL;
    }
    if (nowRunningTask != NULL) {
        tlkapp_audioScheduler_nodeSetNewStateWithChgCB(nowRunningTask, TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY);
        if (nowRunningTask->info.priority != node->info.priority) {
            tlkapp_audioScheduler_nodePushFrontToList(nowRunningTask, &tlkapp_audioScheduler.readyTaskList[nowRunningTask->info.priority]);
        } else { // roundRobin
            tlkapp_audioScheduler_nodePushBackToList(nowRunningTask, &tlkapp_audioScheduler.readyTaskList[nowRunningTask->info.priority]);
        }
    }
}

/**
 * @brief       This function checks if a task switch is busy.
 * @param[in]   node - pointer to the node.
 * @return      true if busy, false otherwise.
 */
static bool tlkapp_audioScheduler_switchBusyCheck(tlkapp_audioScheduler_node_t *node)
{
    if ((tlkapp_audioScheduler.cfg & TLKAPP_AUDIO_SCHEDULER_CFG_SWITCH_BUSY_CHECK) == 0) {
        return false; //not enable this feature
    }
    if (tlkapp_audioScheduler.busyTask != NULL) {
        return true;
    }
    if (tlkapp_audioScheduler.runningTask == NULL) {
        return false;
    }
    if (tlkapp_audioScheduler.checkBusyCB && tlkapp_audioScheduler.checkBusyCB() != false) {
        tlkapp_audioScheduler.busyTask = tlkapp_audioScheduler_malloc(sizeof(tlkapp_audioScheduler_node_t));
        if (tlkapp_audioScheduler.busyTask != NULL) {
            tlkapp_audioScheduler.busyTimer = tlkapp_audioScheduler_getTimerTick();
            tmemcpy(tlkapp_audioScheduler.busyTask, node, sizeof(tlkapp_audioScheduler_node_t));
            tlksys_timer_reStart(TLKSYS_TASKID_AUDIO, &tlkapp_audioScheduler.timer);
            return true;
        }
    }
    return false;
}

/**
 * @brief       This function handles leaving the running state for a node.
 * @param[in]   node - pointer to the node.
 * @return      true if busy, false otherwise.
 */
static inline bool tlkapp_audioScheduler_nodeLeaveRunnningCB(tlkapp_audioScheduler_node_t *node)
{
    bool isBusy = tlkapp_audioScheduler_switchBusyCheck(node);
    if (isBusy) {
        return true;
    }
    tlkapp_audio_modinfSwitch(node->info.optype, (uint16_t)node->taskId, TLK_STATE_CLOSED);
    return false;
}

/**
 * @brief       This function handles entering the running state for a node.
 * @param[in]   node - pointer to the node.
 * @return      true if successful, false otherwise.
 */
static bool tlkapp_audioScheduler_nodeEnterRunnningCB(tlkapp_audioScheduler_node_t *node)
{
    bool isSucc = tlkapp_audio_modinfSwitch(node->info.optype, (uint16_t)node->taskId, TLK_STATE_OPENED);
    if (isSucc) {
        tlkapp_audio_startHandler();
    } else {
        tlkapp_audio_closeHandler();
        tlkapi_printf(TLKAPP_AUDIO_SCHEDULER_DEBUG_ENABLE, "[ASCH]switch open fail %x", node->taskId);
        tlkapp_audio_modinfSwitch(node->info.optype, (uint16_t)node->taskId, TLK_STATE_CRASH);
    }
    return isSucc;
}

/**
 * @brief       This function is the core scheduling callback.
 * @param[in]   node             - pointer to the node.
 * @param[in]   isAutoOpenNewTask - whether to automatically open the new task.
 * @return      true if successful, false otherwise.
 */
static inline bool tlkapp_audioScheduler_coreSchCB(tlkapp_audioScheduler_node_t *node, bool isAutoOpenNewTask)
{
    tlkapp_audioScheduler_node_t *nowRunningTask = tlkapp_audioScheduler.runningTask;
    if (nowRunningTask != NULL) {
        tlkapp_audio_modinfSwitch(nowRunningTask->info.optype, (uint16_t)nowRunningTask->taskId, TLK_STATE_PAUSED);
    }
    if (isAutoOpenNewTask == false) {
        tlkapp_audio_closeHandler();
        return true;
    }
    return tlkapp_audioScheduler_nodeEnterRunnningCB(node);
}

/**
 * @brief       This function checks the state for core scheduling.
 * @param[in]   node    - pointer to the node.
 * @param[in]   isCheck - whether to perform the check.
 * @return      true if check passes, false otherwise.
 */
static inline bool tlkapp_audioScheduler_coreSchCheckState(tlkapp_audioScheduler_node_t *node, bool isCheck)
{
    if (isCheck == false) {
        return true;
    }
    return node->info.state == TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY;
}

/**
 * @brief       This function performs core scheduling.
 * @param[in]   isRoundRobin    - whether to perform round robin scheduling.
 * @param[in]   isAutoOpenNewTask - whether to automatically open the new task.
 * @return      none.
 */
static void tlkapp_audioScheduler_coreSch(bool isRoundRobin, bool isAutoOpenNewTask)
{
    tlkapp_audioScheduler_node_t *nowRunningTask = tlkapp_audioScheduler.runningTask;
    tlkapp_audioScheduler_node_t *node           = NULL;
    uint8_t                       pThreshold     = 0;
    tlkapi_printf(TLKAPP_AUDIO_SCHEDULER_DEBUG_ENABLE, "[ASCH]tlkapp_audioScheduler_coreSch");
    tlkapp_audioScheduler_debug();
    if (nowRunningTask != NULL) {
        if (isRoundRobin == false) {
            pThreshold = nowRunningTask->info.priority + 1;
        } else {
            pThreshold = nowRunningTask->info.priority;
        }
    }
    for (int i = TLKAPP_AUDIO_SCHEDULER_PRIORITY_HIGHEST; i >= pThreshold; i--) {
        node         = tlkapp_audioScheduler.readyTaskList[i];
        bool ischeck = true;
        if (nowRunningTask != NULL && i == nowRunningTask->info.priority && (TLKAPP_AUDIO_SCHEDULER_CFG_SCH_RESUME_AUTO2READY & tlkapp_audioScheduler.cfg)) {
            ischeck = false;
        }
        while (node != NULL) {
            if (tlkapp_audioScheduler_coreSchCheckState(node, ischeck)) {
                bool isBusy = tlkapp_audioScheduler_switchBusyCheck(nowRunningTask);
                if (isBusy) { //busy means can't remove the task now,so need wait for some time
                    return;
                }
                bool isSucc = tlkapp_audioScheduler_coreSchCB(node, isAutoOpenNewTask);
                if (isSucc) {
                    tlkapp_audioScheduler_taskToRunning(node, isAutoOpenNewTask);
                    return;
                }
                tlkapp_audioScheduler_node_t *temp = node;
                node                               = node->next;
                tlkapp_audioScheduler_nodeRemoveFromList(temp);
                tlkapp_audioScheduler_free(temp);
                continue;
            }
            node = node->next;
        }
    }
    if (nowRunningTask == NULL) {
        tlkapp_audio_closeHandler();
    }
}

/**
 * @brief       This function transitions a node state to NOINIT.
 * @param[in]   node             - pointer to the node.
 * @param[in]   info             - task information.
 * @param[in]   isNeedSch        - pointer to flag indicating if scheduling is needed.
 * @param[in]   isAutoOpenNewTask - pointer to flag indicating if auto open new task.
 * @return      0 if success, otherwise fail.
 */
static inline int tlkapp_audioScheduler_nodeStateToNoinit(tlkapp_audioScheduler_node_t *node, tlkapp_audioScheduler_taskInfo_t info, bool *isNeedSch, bool *isAutoOpenNewTask)
{
    (void)isAutoOpenNewTask;
    switch (node->info.state) //past state
    {
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE:
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_PAUSED:
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY:
    {
        tlkapp_audioScheduler_addNewSameDevToTask(node, NULL);
        tlkapp_audioScheduler_nodeRemoveFromList(node);
    } break;
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_RUNNING:
    {
        tlkapp_audioScheduler_sameDevToFirst(node);
        tlkapp_audioScheduler_addNewSameDevToTask(node, NULL);
        bool isbusy                       = tlkapp_audioScheduler_nodeLeaveRunnningCB(node);
        tlkapp_audioScheduler.runningTask = NULL;
        if (isbusy == false) {
            *isNeedSch = true;
            if (TLKAPP_AUDIO_SCHEDULER_CFG_NOT_AUTO_START_TASK & tlkapp_audioScheduler.cfg) {
                *isAutoOpenNewTask = false;
            }
        }
    } break;
    default:
        return -TLK_EPARAM;
    }
    node->info = info;
    return TLK_ENONE;
}

/**
 * @brief       This function transitions a node state to IDLE.
 * @param[in]   node             - pointer to the node.
 * @param[in]   info             - task information.
 * @param[in]   isNeedSch        - pointer to flag indicating if scheduling is needed.
 * @param[in]   isAutoOpenNewTask - pointer to flag indicating if auto open new task.
 * @return      0 if success, otherwise fail.
 */
static inline int tlkapp_audioScheduler_nodeStateToIdle(tlkapp_audioScheduler_node_t *node, tlkapp_audioScheduler_taskInfo_t info, bool *isNeedSch, bool *isAutoOpenNewTask)
{
    switch (node->info.state) //past state
    {
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_NOINIT:
    {
        if (TLKAPP_AUDIO_SCHEDULER_CFG_IDLE_PREEMPTIVE_RUNNING & tlkapp_audioScheduler.cfg) {
            if (tlkapp_audioScheduler.runningTask != NULL && info.audioType == tlkapp_audioScheduler.runningTask->info.audioType &&
                info.priority > tlkapp_audioScheduler.runningTask->info.priority) {
                tlkapp_audioScheduler_nodePushFrontToList(node, &tlkapp_audioScheduler.readyTaskList[info.priority]);
                info.state = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY;
                *isNeedSch = true;
                if (TLKAPP_AUDIO_SCHEDULER_CFG_NOT_AUTO_START_TASK & tlkapp_audioScheduler.cfg) {
                    *isAutoOpenNewTask = false;
                }
                break;
            }
        }
        tlkapp_audioScheduler_nodePushFrontToList(node, &tlkapp_audioScheduler.idleTaskList[info.priority]);
    } break;
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE:
    {
        if (node->info.priority != info.priority) {
            tlkapp_audioScheduler_nodeRemoveFromList(node);
            tlkapp_audioScheduler_nodePushFrontToList(node, &tlkapp_audioScheduler.idleTaskList[info.priority]);
        }
    } break;
    default:
        return -TLK_EPARAM;
    }
    node->info = info;
    return TLK_ENONE;
}

/**
 * @brief       This function transitions a node state to PAUSED.
 * @param[in]   node             - pointer to the node.
 * @param[in]   info             - task information.
 * @param[in]   isNeedSch        - pointer to flag indicating if scheduling is needed.
 * @param[in]   isAutoOpenNewTask - pointer to flag indicating if auto open new task.
 * @return      0 if success, otherwise fail.
 */
static inline int tlkapp_audioScheduler_nodeStateToPaused(tlkapp_audioScheduler_node_t *node, tlkapp_audioScheduler_taskInfo_t info, bool *isNeedSch, bool *isAutoOpenNewTask)
{
    switch (node->info.state) //past state
    {
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_NOINIT:
    {
        tlkapp_audioScheduler_nodePushFrontToList(node, &tlkapp_audioScheduler.readyTaskList[info.priority]);
    } break;
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE:
    {
        tlkapp_audioScheduler_nodeRemoveFromList(node);
        tlkapp_audioScheduler_nodePushFrontToList(node, &tlkapp_audioScheduler.readyTaskList[info.priority]);
    } break;
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_PAUSED:
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY:
    {
        if (node->info.priority != info.priority) {
            tlkapp_audioScheduler_nodeRemoveFromList(node);
            tlkapp_audioScheduler_nodePushFrontToList(node, &tlkapp_audioScheduler.readyTaskList[info.priority]);
        }
    } break;
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_RUNNING:
    {
        tlkapp_audioScheduler_sameDevToFirst(node);
        tlkapp_audioScheduler_nodePushFrontToList(node, &tlkapp_audioScheduler.readyTaskList[info.priority]);
        bool isbusy                       = tlkapp_audioScheduler_nodeLeaveRunnningCB(node);
        tlkapp_audioScheduler.runningTask = NULL;
        if (isbusy == false) {
            *isNeedSch = true;
            if (TLKAPP_AUDIO_SCHEDULER_CFG_NOT_AUTO_START_TASK & tlkapp_audioScheduler.cfg) {
                *isAutoOpenNewTask = false;
            }
        }
    } break;
    default:
        return -TLK_EPARAM;
    }
    node->info = info;
    return TLK_ENONE;
}

/**
 * @brief       This function transitions a node state to READY.
 * @param[in]   node             - pointer to the node.
 * @param[in]   info             - task information.
 * @param[in]   isNeedSch        - pointer to flag indicating if scheduling is needed.
 * @param[in]   isAutoOpenNewTask - pointer to flag indicating if auto open new task.
 * @return      0 if success, otherwise fail.
 */
static inline int tlkapp_audioScheduler_nodeStateToReady(tlkapp_audioScheduler_node_t *node, tlkapp_audioScheduler_taskInfo_t info, bool *isNeedSch, bool *isAutoOpenNewTask)
{
    (void)isAutoOpenNewTask;
    switch (node->info.state) //past state
    {
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_NOINIT:
    {
        tlkapp_audioScheduler_nodePushFrontToList(node, &tlkapp_audioScheduler.readyTaskList[info.priority]);
        *isNeedSch = true;
    } break;
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE:
    {
        tlkapp_audioScheduler_nodeRemoveFromList(node);
        tlkapp_audioScheduler_nodePushFrontToList(node, &tlkapp_audioScheduler.readyTaskList[info.priority]);
        *isNeedSch = true;
    } break;
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_PAUSED:
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY:
    {
        if (node->info.priority != info.priority) {
            tlkapp_audioScheduler_nodeRemoveFromList(node);
            tlkapp_audioScheduler_nodePushFrontToList(node, &tlkapp_audioScheduler.readyTaskList[info.priority]);
        }
        *isNeedSch = true;
    } break;
    default:
        return -TLK_EPARAM;
    }

    node->info = info;
    return TLK_ENONE;
}

/**
 * @brief       This function updates a node.
 * @param[in]   node       - pointer to the node.
 * @param[in]   info       - task information.
 * @param[in]   isAutoSch  - whether to automatically schedule.
 * @param[in]   isNeedSch  - pointer to flag indicating if scheduling is needed.
 * @return      0 if success, otherwise fail.
 */
static int tlkapp_audioScheduler_updateNode(tlkapp_audioScheduler_node_t *node, tlkapp_audioScheduler_taskInfo_t info, bool isAutoSch, bool *isNeedSch)
{
    (void)isAutoSch;
    bool    isNeedSchRes      = false;
    bool    isAutoOpenNewTask = true;
    int     res               = TLK_ENONE;
    uint8_t lastState         = node->info.state;
    //state machine
    switch (info.state) //new state
    {
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_NOINIT:
        res = tlkapp_audioScheduler_nodeStateToNoinit(node, info, &isNeedSchRes, &isAutoOpenNewTask);
        break;
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE:
        res = tlkapp_audioScheduler_nodeStateToIdle(node, info, &isNeedSchRes, &isAutoOpenNewTask);
        break;
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_PAUSED:
        res = tlkapp_audioScheduler_nodeStateToPaused(node, info, &isNeedSchRes, &isAutoOpenNewTask);
        break;
    case TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY:
        res = tlkapp_audioScheduler_nodeStateToReady(node, info, &isNeedSchRes, &isAutoOpenNewTask);
        break;
    default:
        res = -TLK_EPARAM;
        break;
    }
    if (res != TLK_ENONE) {
        return res;
    }
    //state machine change ok,do following:

    //1: do state change callback
    if (lastState != node->info.state) {
        tlkapp_audioScheduler_tryRunStateChgCB(node, lastState, node->info.state);
    }
    if (node->info.state == TLKAPP_AUDIO_SCHEDULER_TASK_STATE_NOINIT) {
        tlkapp_audioScheduler_free(node);
    }
    //2: then do sch
    if (isAutoSch) {
        if (isNeedSchRes) {
            tlkapp_audioScheduler_coreSch(false, isAutoOpenNewTask);
        }
    } else {
        if (isNeedSch != NULL) {
            *isNeedSch = isNeedSchRes;
        }
    }

    return TLK_ENONE;
}

/**
 * @brief       This function gets the paused task that needs scheduling.
 * @param[in]   audioType - the audio type.
 * @return      Pointer to the node that needs scheduling, or NULL if none.
 */
static tlkapp_audioScheduler_node_t *tlkapp_audioScheduler_getNeedSchPausedTask(uint8_t audioType)
{
    tlkapp_audioScheduler_node_t *node;
    // if(tlkapp_audioScheduler.runningTask != NULL){
    //     return NULL;
    // }
    for (int i = TLKAPP_AUDIO_SCHEDULER_PRIORITY_HIGHEST; i >= 0; i--) {
        node = tlkapp_audioScheduler.readyTaskList[i];
        while (node != NULL) {
            if ((node->info.audioType & audioType) != TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_NONE) {
                return node;
            }
            node = node->next;
        }
        if (!(tlkapp_audioScheduler.cfg & TLKAPP_AUDIO_SCHEDULER_CFG_SCH_IDLE_AUTO2RESUME)) {
            continue;
        }
        node = tlkapp_audioScheduler.idleTaskList[i];
        while (node != NULL) {
            if ((node->info.audioType & audioType) != TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_NONE) {
                return node;
            }
            node = node->next;
        }
    }
    return NULL;
}

/**
 * @brief       This function performs round robin scheduling.
 * @param[in]   isAutoSch - whether to automatically schedule.
 * @return      Pointer to the running node, or NULL if none.
 */
static tlkapp_audioScheduler_node_t *tlkapp_audioScheduler_coreRoundRobin(bool isAutoSch)
{
    if (tlkapp_audioScheduler.runningTask == NULL) {
        return NULL;
    }
    if (isAutoSch == true) {
        tlkapp_audioScheduler_coreSch(true, isAutoSch);
        return tlkapp_audioScheduler.runningTask;
    }
    //isAutoSch == false
    if (tlkapp_audioScheduler.cfg & TLKAPP_AUDIO_SCHEDULER_CFG_SCH_IDLE_AUTO2RESUME) {
        uint8_t                       priority = tlkapp_audioScheduler.runningTask->info.priority;
        tlkapp_audioScheduler_node_t *node     = tlkapp_audioScheduler.idleTaskList[priority];
        while (node != NULL) {
            tlkapp_audioScheduler_nodeRemoveFromList(node);
            tlkapp_audioScheduler_nodeSetNewStateWithChgCB(node, TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY);
            tlkapp_audioScheduler_nodePushBackToList(node, &tlkapp_audioScheduler.readyTaskList[priority]);
            node = tlkapp_audioScheduler.idleTaskList[priority];
        }
    }
    tlkapp_audioScheduler_coreSch(true, isAutoSch);
    return tlkapp_audioScheduler.runningTask;
}

/**
 * @brief       This function updates a task core.
 * @param[in]   taskId        - task ID.
 * @param[in]   info          - task information.
 * @param[in]   sameDevTaskId - same device task ID.
 * @param[in]   exInfo        - extra information.
 * @return      0 if success, otherwise fail.
 */
static int tlkapp_audioScheduler_updateTaskCore(uint32_t taskId, tlkapp_audioScheduler_taskInfo_t info, uint32_t sameDevTaskId, tlkapp_audioScheduler_extraInfo_t exInfo)
{
    if (info.priority > TLKAPP_AUDIO_SCHEDULER_PRIORITY_HIGHEST) {
        return -TLK_EPARAM;
    }
    if (info.state > TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY) {
        return -TLK_EPARAM;
    }
    tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler_findNode(taskId);
    if (node == NULL) { //insert node;
        if (info.state == TLKAPP_AUDIO_SCHEDULER_TASK_STATE_NOINIT) {
            return -TLK_EPARAM;
        }
        tlkapi_printf(TLKAPP_AUDIO_SCHEDULER_DEBUG_ENABLE, "[ASCH] malloc one");
        node = tlkapp_audioScheduler_malloc(sizeof(tlkapp_audioScheduler_node_t));
        if (node == NULL) { //malloc fail
            return -TLK_ENOMEM;
        }
        tmemset(node, 0, sizeof(tlkapp_audioScheduler_node_t));
        node->taskId = taskId;
    }
    if (sameDevTaskId != TLKAPP_AUDIO_SCHEDULER_INVALID_TASKID) {
        tlkapp_audioScheduler_node_t *sameDevTaskNode = tlkapp_audioScheduler_findNode(sameDevTaskId);
        if (sameDevTaskNode != node->sameDevTask) {
            tlkapp_audioScheduler_addNewSameDevToTask(node, sameDevTaskNode);
        }
        node->extraInfo = exInfo;
    }
    // return tlkapp_audioScheduler_updateNode(node,info,true,NULL);
    int res = tlkapp_audioScheduler_updateNode(node, info, true, NULL);
    tlkapp_audioScheduler_debug();
    return res;
}

/**
 * @brief       This function handles the timer event.
 * @param[in]   pTimer  - timer handle.
 * @param[in]   userArg - user argument.
 * @return      none.
 */
static void tlkapp_audioScheduler_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    if (tlkapp_audioScheduler.busyTask == NULL) {
        return;
    }
    tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler.busyTask;
    if (tlkapp_audioScheduler.checkBusyCB && tlkapp_audioScheduler.checkBusyCB() != false) {
        if (!tlkapp_audioScheduler_timerExceed(tlkapp_audioScheduler.busyTimer, 10 * 1000 * 1000)) {
            tlksys_timer_reStart(TLKSYS_TASKID_AUDIO, pTimer);
            return; //still busy and not exceed 5s
        }
    } //wait busy 5s
    tlkapp_audio_modinfSwitch(node->info.optype, (uint16_t)node->taskId, TLK_STATE_CLOSED);
    tlkapp_audioScheduler_free(node);
    tlkapp_audioScheduler.busyTask = NULL;
    tlkapp_audioScheduler_coreSch(false, true);
    tlkapp_audioScheduler.busyTimer = 0;
}

//api code begin:

/**
 * @brief  This function servers to initialize audio scheduler.
 * @param[in] cfg - feature settings.
 * @returns  The result of initialization.
 */
int tlkapp_audioScheduler_init(uint32_t cfg)
{
    memset(&tlkapp_audioScheduler, 0, sizeof(tlkapp_audioScheduler));
    memset(&tlkapp_audioScheduler_memPool, 0, sizeof(tlkapp_audioScheduler_memPool));
    tlkapp_audioScheduler.cfg = cfg;
    tlksys_timer_createStatic(TLKSYS_TASKID_AUDIO, &tlkapp_audioScheduler.timer, 50 * 1000, false, tlkapp_audioScheduler_timer, NULL);
    return TLK_ENONE;
}

/**
 * @brief  This function servers to get current count of audio tasks.
 * @param[in] audioType - The type of audio.
 * @returns  The current count of audio tasks.
 */
uint8_t tlkapp_audioScheduler_getTaskNums(uint8_t audioType)
{
    tlkapp_audioScheduler_memPool_t *memPool = &tlkapp_audioScheduler_memPool;
    uint8_t                          cnt     = 0;
    for (int i = 0; i < TLKAPP_AUDIO_SCHEDULER_MAX_TASKNUM; i++) {
        if ((memPool->isUsedFlag & (1 << i)) == 0) {
            continue;
        }
        tlkapp_audioScheduler_node_t *node = &memPool->nodes[i];
        if (node->info.audioType & audioType) {
            cnt++;
        }
    }
    return cnt;
}

/**
 * @brief  This function servers to get now running task in audio scheduler.
 * @returns  The pointer to the node of running task , NULL means no running task now.
 */
const tlkapp_audioScheduler_node_t *tlkapp_audioScheduler_getRunningTask(void)
{
    if (tlkapp_audioScheduler.busyTask) {
        return tlkapp_audioScheduler.busyTask;
    }
    return tlkapp_audioScheduler.runningTask;
}

/**
 * @brief  This function servers to get now running task's optype.
 * @returns  optype.
 */
uint8_t tlkapp_audioScheduler_getCurOptype(void)
{
    if (tlkapp_audioScheduler.runningTask == NULL) {
        return TLKAUD_TYPE_MAX;
    }
    return tlkapp_audioScheduler.runningTask->info.optype;
}

/**
 * @brief  This function servers to get task info by taskid.
 * @param[in] taskId - task ID.
 * @returns  The pointer to the node of task info, NULL means can not find this task ID in list.
 */
const tlkapp_audioScheduler_taskInfo_t *tlkapp_audioScheduler_getTaskInfo(uint32_t taskId)
{
    tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler_findNode(taskId);
    if (node == NULL) {
        return NULL;
    }
    return &node->info;
}

/**
 * @brief  This function servers to get the audio tasks that need to be scheduled for execution.
 * @param[in] isAutoSch - whether need automatic scheduling.
 * @param[in] audioType - the type of audio ,TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC/TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE.
 * @returns  The pointer to the node of task that need to be scheduled for execution, NULL no task need to be scheduled.
 */
const tlkapp_audioScheduler_node_t *tlkapp_audioScheduler_SchPausedTask(bool isAutoSch, uint8_t audioType)
{
    if (audioType != TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE && audioType != TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC) {
        return NULL; //error parmater
    }
    tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler_getNeedSchPausedTask(audioType);
    if (node == NULL) {
        return NULL;
    }
    if (isAutoSch) {
        tlkapp_audioScheduler_taskInfo_t info = {
            .audioType = node->info.audioType,
            .optype    = node->info.optype,
            .priority  = node->info.priority,
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY,
        };
        tlkapp_audioScheduler_updateNode(node, info, true, NULL);
        if (tlkapp_audioScheduler.runningTask == NULL) {
            return NULL;
        }
        tlkapp_audioScheduler_runningTaskSwitchTypeCore(audioType);
        return tlkapp_audioScheduler.runningTask;
    }
    return node;
}

/**
 * @brief  This function servers to update/new a audio task.
 * @param[in] taskId          - task ID.
 * @param[in] info            - the info of task.
 * @param[in] sameDevTaskId   - the id of same device's task.
 * @returns  The result of update.
 */
int tlkapp_audioScheduler_updateTask(uint32_t taskId, tlkapp_audioScheduler_taskInfo_t info, uint32_t sameDevTaskId)
{
    tlkapp_audioScheduler_extraInfo_t exInfo = {0};
    return tlkapp_audioScheduler_updateTaskEx(taskId, info, sameDevTaskId, exInfo);
}

/**
 * @brief  This function servers to update/new a audio task with exInfo.
 * @param[in] taskId         - task ID.
 * @param[in] info           - the info of task.
 * @param[in] sameDevTaskId  - the id of same device's task.
 * @param[in] exInfo         - extra info.
 * @returns  The result of update.
 */
int tlkapp_audioScheduler_updateTaskEx(uint32_t taskId, tlkapp_audioScheduler_taskInfo_t info, uint32_t sameDevTaskId, tlkapp_audioScheduler_extraInfo_t exInfo)
{
    return tlkapp_audioScheduler_updateTaskCore(taskId, info, sameDevTaskId, exInfo);
}

/**
 * @brief  This function servers to pause a audio task.
 * @param[in] taskId - task ID.
 * @returns  The result of pause.
 */
int tlkapp_audioScheduler_pauseTask(uint32_t taskId)
{
    tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler_findNode(taskId);
    if (node == NULL) {
        return -TLK_ESEEK;
    }
    tlkapp_audioScheduler_taskInfo_t info = {
        .audioType = node->info.audioType,
        .optype    = node->info.optype,
        .priority  = node->info.priority,
        .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_PAUSED,
    };
    return tlkapp_audioScheduler_updateNode(node, info, true, NULL);
}

/**
 * @brief  This function servers to resume a audio task.
 * @param[in] taskId - task ID.
 * @returns  The result of resume.
 */
int tlkapp_audioScheduler_resumeTask(uint32_t taskId)
{
    tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler_findNode(taskId);
    if (node == NULL) {
        return -TLK_ESEEK;
    }
    tlkapp_audioScheduler_taskInfo_t info = {
        .audioType = node->info.audioType,
        .optype    = node->info.optype,
        .priority  = node->info.priority,
        .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY,
    };
    return tlkapp_audioScheduler_updateNode(node, info, true, NULL);
}

/**
 * @brief  This function servers to delete a audio task.
 * @param[in] taskId - task ID.
 * @returns  The result of delete.
 */
int tlkapp_audioScheduler_deleteTask(uint32_t taskId)
{
    tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler_findNode(taskId);
    if (node == NULL) {
        return -TLK_ESEEK;
    }
    tlkapp_audioScheduler_taskInfo_t info = {
        .audioType = node->info.audioType,
        .optype    = node->info.optype,
        .priority  = node->info.priority,
        .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_NOINIT,
    };
    return tlkapp_audioScheduler_updateNode(node, info, true, NULL);
}

/**
 * @brief  This function servers to round robin tasks.
 * @returns  The pointer to the node of task that need to be scheduled for execution, NULL no task need to be scheduled.
 */
const tlkapp_audioScheduler_node_t *tlkapp_audioScheduler_roundRobin(void)
{
    if (TLKAPP_AUDIO_SCHEDULER_CFG_NOT_AUTO_START_TASK & tlkapp_audioScheduler.cfg) {
        return tlkapp_audioScheduler_coreRoundRobin(false);
    }
    return tlkapp_audioScheduler_coreRoundRobin(true);
}

/**
 * @brief  This function servers to change now audio task's audiotype.
 * @param[in] audioType - the type of audio ,TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC/TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE.
 * @returns The result of switch
 */
int tlkapp_audioScheduler_runningTaskSwitchType(uint8_t audioType)
{
    tlkapp_audioScheduler_node_t *runningTask = tlkapp_audioScheduler.runningTask;
    if (runningTask == NULL) {
        return -TLK_ENOSUPPORT;
    }
    if (audioType != TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE && audioType != TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC) {
        return -TLK_EPARAM;
    }
    if ((runningTask->info.audioType & audioType) == 0) {
        return -TLK_ENOSUPPORT;
    }
    bool result = tlkapp_audioScheduler_runningTaskSwitchTypeCore(audioType);
    if (result == false) {
        return -TLK_EFAIL;
    }
    return TLK_ENONE;
}

/**
 * @brief  This function servers to get default priority of a audio optype.
 * @param[in] optype - audio optype.
 * @returns  The default priority of a audio optype.
 */
uint8_t tlkapp_audioScheduler_getDefaultPriority(uint8_t optype)
{
    static const uint8_t tlkapp_audioScheduler_priorityTab[TLKAUD_TYPE_MAX] = {
        [TLKAUD_TYPE_TONE]             = 7,
        [TLKAUD_TYPE_CC_BT_VOICE]      = 5,
        [TLKAUD_TYPE_CC_BT_MUSIC]      = 2,
        [TLKAUD_TYPE_TPH_AUDIO]        = 1,
        [TLKAUD_TYPE_A2DP_OUT]         = 2,
        [TLKAUD_TYPE_LEA_UC_MUSIC]     = 2,
        [TLKAUD_TYPE_LEA_UC_VOICE]     = 5,
        [TLKAUD_TYPE_LEA_US_MUSIC]     = 2,
        [TLKAUD_TYPE_LEA_US_VOICE]     = 5,
        [TLKAUD_TYPE_LEA_BMS]          = 2,
        [TLKAUD_TYPE_LEA_BMR]          = 2,
        [TLKAUD_TYPE_U2H_VOICE]        = 5,
        [TLKAUD_TYPE_A2DP_TO_BIS]      = 2,
        [TLKAUD_TYPE_SIDETONE]         = 4,
        [TLKAUD_TYPE_INTRTPHONE]       = 3,
        [TLKAUD_TYPE_HRA]              = 1,
        [TLKAUD_TYPE_UAC_AUD]          = 1,
        [TLKAUD_TYPE_TPD_AUDIO]        = 1,
        [TLKAUD_TYPE_ANC]              = 6,
        [TLKAUD_TYPE_UAC_LOCAL_AUDIO]  = 1,
        [TLKAUD_TYPE_BT_VOICE_FORWARD] = 6,
        [TLKAUD_TYPE_BT_MUSIC_FORWARD] = 3,
    };
    if (optype >= TLKAUD_TYPE_MAX) {
        return 0;
    }
    return tlkapp_audioScheduler_priorityTab[optype];
}

/**
 * @brief  This function servers to register a callback to check whether task switch is busy.
 * @param[in] cb - busy checK callback.
 * @returns  The result of register.
 */
int tlkapp_audioScheduler_regSwitchCheckBusyCB(tlkapp_audioScheduler_checkBusyCB cb)
{
    if ((tlkapp_audioScheduler.cfg & TLKAPP_AUDIO_SCHEDULER_CFG_SWITCH_BUSY_CHECK) == 0) {
        return -TLK_ENOSUPPORT;
    }
    tlkapp_audioScheduler.checkBusyCB = cb;
    return TLK_ENONE;
}

/**
 * @brief  This function servers for debug.
 * @returns  None.
 */
void tlkapp_audioScheduler_debug(void)
{
#if TLKAPP_AUDIO_SCHEDULER_DEBUG_ENABLE
    for (int i = TLKAPP_AUDIO_SCHEDULER_PRIORITY_HIGHEST; i >= 0; i--) {
        tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler.idleTaskList[i];
        if (node == NULL) {
            continue;
        }
        tlkapi_printf(TLKAPP_AUDIO_SCHEDULER_DEBUG_ENABLE, "[ASCH]pro:%d idlelist:", i);
        while (node != NULL) {
            tlkapi_printf(TLKAPP_AUDIO_SCHEDULER_DEBUG_ENABLE, "[ASCH]task[%x], state[%d]", node->taskId, node->info.state);
            node = node->next;
        }
    }
    for (int i = TLKAPP_AUDIO_SCHEDULER_PRIORITY_HIGHEST; i >= 0; i--) {
        tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler.readyTaskList[i];
        if (node == NULL) {
            continue;
        }
        tlkapi_printf(TLKAPP_AUDIO_SCHEDULER_DEBUG_ENABLE, "[ASCH]pro:%d rdylist:", i);
        while (node != NULL) {
            tlkapi_printf(TLKAPP_AUDIO_SCHEDULER_DEBUG_ENABLE, "[ASCH]task[%x], state[%d]", node->taskId, node->info.state);
            node = node->next;
        }
    }
    tlkapp_audioScheduler_node_t *node = tlkapp_audioScheduler.runningTask;
    if (node == NULL) {
        return;
    }
    tlkapi_printf(TLKAPP_AUDIO_SCHEDULER_DEBUG_ENABLE, "[ASCH]nowRunningTaskId[%x], state[%d]", node->taskId, node->info.state);
#endif
}

/**
 * @brief  This function servers to initialize audio scheduler by default feature settings.
 * @returns  None.
 */
void tlkapp_audioScheduler_featureInit(void)
{
#ifdef PROJ_BTBLE_AUDIO_SOURCE
    tlkapp_audioScheduler_init(TLKAPP_AUDIO_SCHEDULER_CFG_SCH_DONGLE_DEFAULT);
#else
    tlkapp_audioScheduler_init(TLKAPP_AUDIO_SCHEDULER_CFG_SCH_HEADSET_DEFAULT);
#if TLK_CFG_TONE_ENABLE
    tlkapp_audioScheduler_regSwitchCheckBusyCB(tlkmdi_tone_is_busy);
#endif
#endif
}

#endif // #if (TLK_MW_AUDIO_ENABLE)
