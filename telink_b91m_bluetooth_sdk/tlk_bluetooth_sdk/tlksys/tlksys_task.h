/********************************************************************************************************
 * @file    tlksys_task.h
 *
 * @brief   This is the header file for TLSR/TL
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
#pragma once

typedef enum
{
    TLKSYS_TASK_EVT_CORE = 0,

    TLKSYS_TASK_EVT_SYS_CORE = 0,
    TLKSYS_TASK_EVT_SYS_PM,
    TLKSYS_TASK_EVT_SYS_SERIAL,
    TLKSYS_TASK_EVT_SYS_USB,
    TLKSYS_TASK_EVT_SYS_PRINT_LOG,
    TLKSYS_TASK_EVT_SYS_SQL_SAVE,
    TLKSYS_TASK_EVT_SYS_USER_CTRL,
    TLKSYS_TASK_EVT_SYS_USB_HS,
    TLKSYS_TASK_EVT_SYS_USB_SUSPEND,
    TLKSYS_TASK_EVT_SYS_USB_EXIT_SUSPEND,
    TLKSYS_TASK_EVT_SYS_APP_0,
    TLKSYS_TASK_EVT_SYS_APP_1,
    TLKSYS_TASK_EVT_SYS_APP_MAX_INDEX = 15,

    TLKSYS_TASK_EVT_HOST_CORE = 0,
    TLKSYS_TASK_EVT_HOST_HCI,
    TLKSYS_TASK_EVT_HOST_LE_EVT,
    TLKSYS_TASK_EVT_HOST_IAP2,
    TLKSYS_TASK_EVT_HOST_TPSLL,
    TLKSYS_TASK_EVT_LOW_LATENCY_STATE,
    TLKSYS_TASK_EVT_HOST_MUSIC_STATUS_NOTIFY,
    TLKSYS_TASK_EVT_HOST_APP0,
    TLKSYS_TASK_EVT_HOST_APP1,
    TLKSYS_TASK_EVT_HOST_APP2,
    TLKSYS_TASK_EVT_HOST_APP3,

    TLKSYS_TASK_EVT_AUD_CORE = 0,
    TLKSYS_TASK_EVT_AUD_MAIN,
    TLKSYS_TASK_EVT_AUD_DSP_RDY,
    TLKSYS_TASK_EVT_AUD_DSP,
    TLKSYS_TASK_EVT_AUD_DEBUG,
    TLKSYS_TASK_EVT_AUD_HO_MIC,
    TLKSYS_TASK_EVT_AUD_NUM,
} TLKSYS_TASK_EVT_ENUM;

typedef enum
{
    TLKSYS_TASK_STATE_NOINIT = 0,
    TLKSYS_TASK_STATE_RUN_IN_MAIN_THREAD,
    TLKSYS_TASK_STATE_RUN_IN_INDEPENDENCE_THREAD,
} TLKSYS_TASK_STATE_ENUM;

typedef struct
{
    uint16_t priority;
    uint16_t stackSize;
    char    *pTaskName;
    void (*Init)(void);
    void (*Start)(void);
    int (*Input)(uint16_t msgID, uint8_t *pData, uint16_t dataLen);
    void (*Handler)(void);
    void *pTaskStaticBuffer;
} tlksys_task_cfg_t;

typedef struct
{
    uint16_t                 taskID;
    uint16_t                 state;
    TlkApiTimerList_t        timerList;
    const tlksys_task_cfg_t *pCfgs;
    TlkOsTaskHandle_t        taskHandle;
} tlksys_task_t;

/**
 * @brief  Start all registered tasks
 * @param  None.
 * @returns  None.
 */
void tlksys_task_start(void);

/**
 * @brief  Handle all registered tasks
 * @param  None.
 * @returns  None.
 */
void tlksys_task_handler(void);

/**
 * @brief  Create a new task with specified configuration
 * @param[in] taskID : Task ID to create
 * @param[in] pCfgs : Pointer to task configuration structure
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_task_create(uint16_t taskID, const tlksys_task_cfg_t *pCfgs);

/**
 * @brief  Send a message to specified task
 * @param[in] taskID : Target task ID
 * @param[in] msgID : Message ID
 * @param[in] pData : Pointer to message data
 * @param[in] dataLen : Length of message data
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_task_input(uint16_t taskID, uint16_t msgID, uint8_t *pData, uint16_t dataLen);

/**
 * @brief  Get timer list of specified task
 * @param[in] taskID : Task ID to get timer list
 * @returns  Pointer to timer list of the task
 */
TlkApiTimerList_t *tlksys_task_getTimerList(uint16_t taskID);

/**
 * @brief  Register event callback for specified task
 * @param[in] taskID : Task ID to register callback
 * @param[in] index : Event index
 * @param[in] cb : Callback function to register
 * @returns  None.
 */
void tlksys_task_regEvtCB(uint16_t taskID, uint32_t index, TlkOsEventDealCB cb);

/**
 * @brief  Register event callback for specified task
 * @param[in] taskID : Task ID to register callback
 * @param[in] index : Event index
 * @param[in] cb : Callback function to register
 * @param[in] isFirstSet : Is need set event when register.
 * @returns  None.
 */
void tlksys_task_regEvtCBEx(uint16_t taskID, uint32_t index, TlkOsEventDealCB cb, uint8_t isFirstSet);

/**
 * @brief  Set event for specified task
 * @param[in] taskID : Target task ID
 * @param[in] id : Event ID to set
 * @returns  None.
 */
void tlksys_task_setEvt(uint16_t taskID, uint32_t id);

/**
 * @brief  Set event for specified task from ISR context
 * @param[in] taskID : Target task ID
 * @param[in] id : Event ID to set
 * @returns  None.
 */
void tlksys_task_setEvtFromIsr(uint16_t taskID, uint32_t id);

/**
 * @brief  Wait for event of specified task
 * @param[in] taskID : Task ID to wait for event
 * @param[in] blockTimeMs : Maximum time to wait in milliseconds
 * @returns  None.
 */
void tlksys_task_waitEvent(uint16_t taskID, uint32_t blockTimeMs);

/**
 * @brief  Set priority of specified task from ISR context
 * @param[in] taskID : Target task ID
 * @param[in] priority : New priority value
 * @returns  None.
 */
void tlksys_task_setPriorityFromIsr(uint16_t taskID, uint32_t priority);
