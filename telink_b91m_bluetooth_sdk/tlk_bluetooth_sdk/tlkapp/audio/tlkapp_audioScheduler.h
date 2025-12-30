/********************************************************************************************************
 * @file    tlkapp_audioScheduler.h
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
#ifndef TLKAPP_AUDIO_SCHEDULER_H
#define TLKAPP_AUDIO_SCHEDULER_H

#ifndef TLKAPP_AUDIO_SCHEDULER_MAX_TASKNUM
#define TLKAPP_AUDIO_SCHEDULER_MAX_TASKNUM 10
#endif

#define TLKAPP_AUDIO_SCHEDULER_DEBUG_ENABLE   1

#define TLKAPP_AUDIO_SCHEDULER_INVALID_TASKID 0XFFFFFFFFU
#define TLKAPP_AUDIO_SCHEDULER_NO_CHANGED     0XFFFFFFFFU

typedef enum
{
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_LEVEL0 = 0,
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_LEVEL1,
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_LEVEL2,
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_LEVEL3,
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_LEVEL4,
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_LEVEL5,
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_LEVEL6,
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_LEVEL7,
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_NUM,
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_LOWEST  = TLKAPP_AUDIO_SCHEDULER_PRIORITY_LEVEL0,
    TLKAPP_AUDIO_SCHEDULER_PRIORITY_HIGHEST = TLKAPP_AUDIO_SCHEDULER_PRIORITY_LEVEL7,
} TLKAPP_AUDIO_SCHEDULER_PRIORITY_ENUM;

typedef enum
{
    TLKAPP_AUDIO_SCHEDULER_TASK_STATE_NOINIT = 0,
    TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
    TLKAPP_AUDIO_SCHEDULER_TASK_STATE_PAUSED,
    TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY,

    //note:following is not for user,set task to these state in any api will return -TLK_EPARAM
    TLKAPP_AUDIO_SCHEDULER_TASK_STATE_RUNNING,
    TLKAPP_AUDIO_SCHEDULER_TASK_STATE_MUTEX,
    TLKAPP_AUDIO_SCHEDULER_TASK_STATE_CRASH,
} TLKAPP_AUDIO_SCHEDULER_TASK_STATE_ENUM;

typedef enum
{
    TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_NONE            = 0,
    TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC           = (1 << 0),
    TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE           = (1 << 1),
    TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE | TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC,
} TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_ENUM;

typedef enum
{
    TLKAPP_AUDIO_SCHEDULER_CFG_SAME_DEVICE_REFRESH_L2H = (1 << 0),
    TLKAPP_AUDIO_SCHEDULER_CFG_SAME_DEVICE_REFRESH_H2L = (1 << 1),
    TLKAPP_AUDIO_SCHEDULER_CFG_SCH_RESUME_AUTO2READY   = (1 << 2),
    TLKAPP_AUDIO_SCHEDULER_CFG_SCH_IDLE_AUTO2RESUME    = (1 << 3),
    TLKAPP_AUDIO_SCHEDULER_CFG_IDLE_PREEMPTIVE_RUNNING = (1 << 4),
    TLKAPP_AUDIO_SCHEDULER_CFG_NOT_AUTO_START_TASK     = (1 << 5),
    TLKAPP_AUDIO_SCHEDULER_CFG_SWITCH_BUSY_CHECK       = (1 << 6),

    TLKAPP_AUDIO_SCHEDULER_CFG_SCH_HEADSET_DEFAULT = TLKAPP_AUDIO_SCHEDULER_CFG_SCH_IDLE_AUTO2RESUME | TLKAPP_AUDIO_SCHEDULER_CFG_SWITCH_BUSY_CHECK,

    TLKAPP_AUDIO_SCHEDULER_CFG_SCH_DONGLE_DEFAULT = TLKAPP_AUDIO_SCHEDULER_CFG_SAME_DEVICE_REFRESH_H2L | TLKAPP_AUDIO_SCHEDULER_CFG_SAME_DEVICE_REFRESH_L2H |
                                                    TLKAPP_AUDIO_SCHEDULER_CFG_SCH_IDLE_AUTO2RESUME | TLKAPP_AUDIO_SCHEDULER_CFG_SCH_RESUME_AUTO2READY |
                                                    TLKAPP_AUDIO_SCHEDULER_CFG_IDLE_PREEMPTIVE_RUNNING | TLKAPP_AUDIO_SCHEDULER_CFG_NOT_AUTO_START_TASK,

    TLKAPP_AUDIO_SCHEDULER_CFG_SCH_UAC = TLKAPP_AUDIO_SCHEDULER_CFG_SCH_IDLE_AUTO2RESUME | TLKAPP_AUDIO_SCHEDULER_CFG_NOT_AUTO_START_TASK,
} TLKAPP_AUDIO_SCHEDULER_CFG_ENUM;

typedef enum
{
    TLKAPP_AUDIO_SCHEDULER_PENDING_CHN_INEAR_DETECT = 0,
} TLKAPP_AUDIO_SCHEDULER_PENDING_CHN_ENUM;

typedef void (*tlkappAudioSchStateChangeCB)(uint32_t taskID, uint8_t oldState, uint8_t newState, void *UserArg);

//this feature no

typedef struct
{
    uint8_t optype;
    uint8_t audioType;
    uint8_t priority;
    uint8_t state;
} tlkapp_audioScheduler_taskInfo_t;

typedef struct
{
    tlkappAudioSchStateChangeCB stateChangeCB;
    void                       *stateChangeCBUserArg;
} tlkapp_audioScheduler_extraInfo_t;

struct tlkapp_audioScheduler_node_s
{
    uint32_t                             taskId;
    struct tlkapp_audioScheduler_node_s *prev;
    struct tlkapp_audioScheduler_node_s *next;
    struct tlkapp_audioScheduler_node_s *sameDevTask;
    tlkapp_audioScheduler_taskInfo_t     info;
    tlkapp_audioScheduler_extraInfo_t    extraInfo;
};

typedef struct tlkapp_audioScheduler_node_s tlkapp_audioScheduler_node_t;

typedef bool (*tlkapp_audioScheduler_checkBusyCB)(void);

typedef struct
{
    uint32_t                          busyTimer;
    uint32_t                          cfg;
    tlkapp_audioScheduler_checkBusyCB checkBusyCB;
    tlkapp_audioScheduler_node_t     *busyTask;
    tlkapp_audioScheduler_node_t     *runningTask;
    tlkapp_audioScheduler_node_t     *readyTaskList[TLKAPP_AUDIO_SCHEDULER_PRIORITY_NUM];
    tlkapp_audioScheduler_node_t     *idleTaskList[TLKAPP_AUDIO_SCHEDULER_PRIORITY_NUM];
    TlkApiTimer_t                     timer;
} tlkapp_audioScheduler_t;

/**
 * @brief       This function initializes the audio scheduler.
 * @param[in]   cfg - feature settings.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_init(uint32_t cfg);

/**
 * @brief       This function gets the current count of audio tasks.
 * @param[in]   audioType - the type of audio.
 * @return      The current count of audio tasks.
 */
uint8_t tlkapp_audioScheduler_getTaskNums(uint8_t audioType);

/**
 * @brief       This function gets the currently running task in the audio scheduler.
 * @param[in]   none.
 * @return      The pointer to the node of running task, NULL means no running task now.
 */
const tlkapp_audioScheduler_node_t *tlkapp_audioScheduler_getRunningTask(void);

/**
 * @brief       This function gets the optype of the currently running task.
 * @param[in]   none.
 * @return      The optype of the currently running task.
 */
uint8_t tlkapp_audioScheduler_getCurOptype(void);

/**
 * @brief       This function gets task information by task ID.
 * @param[in]   taskId - task ID.
 * @return      The pointer to the node of task info, NULL means cannot find this task ID in list.
 */
const tlkapp_audioScheduler_taskInfo_t *tlkapp_audioScheduler_getTaskInfo(uint32_t taskId);

/**
 * @brief       This function gets the audio tasks that need to be scheduled for execution.
 * @param[in]   isAutoSch  - whether need automatic scheduling.
 * @param[in]   audioType  - the type of audio, TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC/TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE.
 * @return      The pointer to the node of task that need to be scheduled for execution, NULL no task need to be scheduled.
 */
const tlkapp_audioScheduler_node_t *tlkapp_audioScheduler_SchPausedTask(bool isAutoSch, uint8_t audioType);

/**
 * @brief       This function updates/creates an audio task.
 * @param[in]   taskId         - task ID.
 * @param[in]   info           - the info of task.
 * @param[in]   sameDevTaskId  - the id of same device's task.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_updateTask(uint32_t taskId, tlkapp_audioScheduler_taskInfo_t info, uint32_t sameDevTaskId);

/**
 * @brief       This function updates/creates an audio task with extended information.
 * @param[in]   taskId         - task ID.
 * @param[in]   info           - the info of task.
 * @param[in]   sameDevTaskId  - the id of same device's task.
 * @param[in]   exInfo         - extra info.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_updateTaskEx(uint32_t taskId, tlkapp_audioScheduler_taskInfo_t info, uint32_t sameDevTaskId, tlkapp_audioScheduler_extraInfo_t exInfo);

/**
 * @brief       This function pauses an audio task.
 * @param[in]   taskId - task ID.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_pauseTask(uint32_t taskId);

/**
 * @brief       This function resumes an audio task.
 * @param[in]   taskId - task ID.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_resumeTask(uint32_t taskId);

/**
 * @brief       This function deletes an audio task.
 * @param[in]   taskId - task ID.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_deleteTask(uint32_t taskId);

/**
 * @brief       This function performs round robin scheduling of tasks.
 * @param[in]   none.
 * @return      The pointer to the node of task that need to be scheduled for execution, NULL no task need to be scheduled.
 */
const tlkapp_audioScheduler_node_t *tlkapp_audioScheduler_roundRobin(void);

/**
 * @brief       This function switches the audio type of the current audio task.
 * @param[in]   audioType - the type of audio, TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC/TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_runningTaskSwitchType(uint8_t audioType);

/**
 * @brief       This function gets the default priority of an audio optype.
 * @param[in]   optype - audio optype.
 * @return      The default priority of an audio optype.
 */
uint8_t tlkapp_audioScheduler_getDefaultPriority(uint8_t optype);

/**
 * @brief       This function registers a callback to check whether task switch is busy.
 * @param[in]   cb - busy check callback.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_regSwitchCheckBusyCB(tlkapp_audioScheduler_checkBusyCB cb);

/**
 * @brief       This function is used for debugging purposes.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_audioScheduler_debug(void);

/**
 * @brief       This function initializes the audio scheduler with default feature settings.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_audioScheduler_featureInit(void);

#endif // TLKAPP_AUDIO_SCHEDULER_H
