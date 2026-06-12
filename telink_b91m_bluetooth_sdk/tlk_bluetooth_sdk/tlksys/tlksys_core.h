/********************************************************************************************************
 * @file    tlksys_core.h
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
#ifndef TLKSYS_CORE_H
#define TLKSYS_CORE_H

#define TLKSYS_IRQMSG_ITEM_SIZE 16
#define TLKSYS_IRQMSG_ITEM_NUMB 16 // Must be a power of two, and less than 250
#define TLKSYS_IRQMSG_ITEM_MASK (TLKSYS_IRQMSG_ITEM_NUMB - 1)

typedef enum
{
    TLKSYS_TASKID_SYSTEM = 0,
    TLKSYS_TASKID_AUDIO,
    TLKSYS_TASKID_HOST,
#if TLKAPP_LEMGR_ENABLE
    TLKSYS_TASKID_LEMGR,
#endif
    TLKSYS_TASKID_USER,
    TLKSYS_TASKID_MAXNUM,
} TLKSYS_TASKID_ENUM;

typedef enum
{
    TLKSYS_TASK_IDLE_PRIORITY            = 0,
    TLKSYS_TASK_USER_PRIORITY            = 2,
    TLKSYS_TASK_SYSTEM_PRIORITY          = 2,
    TLKSYS_TASK_HOST_PRIORITY            = 4,
    TLKSYS_TASK_CONTROLLER_PRIORITY      = 4,
    TLKSYS_TASK_AUDIO_PRIORITY           = 6,
    TLKSYS_TASK_AUDIO_IRQ_PRIORITY       = 7,
    TLKSYS_TASK_AUDIO_HIGH_TASK_PRIORITY = 8,
} TLKSYS_TASK_PRIORITY_ENUM;

typedef enum
{
    TLKSYS_MUTEX_H2C = 0,
    TLKSYS_MUTEX_LOG,
    TLKSYS_MUTEX_UI,
    TLKSYS_MUTEX_TINYSQL,
    TLKSYS_MUTEX_USERCTRL,
    TLKSYS_MUTEX_AUDIO,
    TLKSYS_MUTEX_NUM,
} TLKSYS_MUTEX_ENUM;

typedef enum
{
    TLKSYS_REQ_HANDLE_SYNC_DATA_TWS_AUD,
    TLKSYS_REQ_HANDLE_TWS_SLAVE_AUDIO_SYNC_DATA,
    TLKSYS_REQ_HANDLE_TWS_SLAVE_SEND_SYNC_TICK,
    TLKSYS_REQ_HANDLE_SYNC_DATA_MAX,
} TLKSYS_REQ_HANDLE_SYNC_ENUM;

typedef void (*TlksysTmrSyncCB)(uint32_t syncTick);
typedef void (*TlksysHandleSyncDataCB)(void *pData, uint16_t dataLen);

/**
 * @brief  Initialize the tlksys module
 * @param  None.
 * @returns  TLK_ENONE - Success. Otherwise, error code
 */
int tlksys_init(void);

/**
 * @brief  Start the tlksys module with specified function
 * @param[in] func : Initialization function to start with
 * @returns  None.
 */
void tlksys_start(TlkOsInitFunc_t func);

/**
 * @brief  Handle tlksys module operations
 * @param  None.
 * @returns  None.
 */
void tlksys_handler(void);

/**
 * @brief  Send message to specified task
 * @param[in] taskID : Target task ID
 * @param[in] msgID : Message ID
 * @param[in] pData : Pointer to message data
 * @param[in] dataLen : Length of message data
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_sendMsg(uint16_t taskID, uint16_t msgID, void *pData, uint16_t dataLen);

/**
 * @brief  Send message from timer context to specified task
 * @param[in] taskID : Target task ID
 * @param[in] msgID : Message ID
 * @param[in] pData : Pointer to message data
 * @param[in] dataLen : Length of message data
 * @returns  TLK_ENONE - Success. Otherwise, error code.
 */
int tlksys_sendMsgFromTimer(uint16_t taskID, uint16_t msgID, uint8_t *pData, uint16_t dataLen);

/**
 * @brief  Leave critical section
 * @param  None.
 * @returns  None.
 */
void tlksys_leave_critical(void);

/**
 * @brief  Enter critical section
 * @param  None.
 * @returns  None.
 */
void tlksys_enter_critical(void);

/**
 * @brief  Lock specified mutex
 * @param[in] mutexID : Mutex ID to lock
 * @returns  None.
 */
void tlksys_mutex_lock(uint16_t mutexID);

/**
 * @brief  Unlock specified mutex
 * @param[in] mutexID : Mutex ID to unlock
 * @returns  None.
 */
void tlksys_mutex_unlock(uint16_t mutexID);

/**
 * @brief  Hook function called when initialization is finished
 * @param  None.
 * @returns  None.
 */
void tlksys_initFinishedHook(void);
#endif // TLKSYS_CORE_H
