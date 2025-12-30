/********************************************************************************************************
 * @file    tlkapp_audioMsg.h
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
#ifndef TLKAPP_AUDIO_MSG_H
#define TLKAPP_AUDIO_MSG_H

/**
 * @brief       This function handles audio messages.
 * @param[in]   msgID   - the message ID.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_msgHandle(uint8_t msgID, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       This function safely updates a task with extended information.
 * @param[in]   taskId        - the task ID.
 * @param[in]   info          - task information.
 * @param[in]   sameDevTaskId - same device task ID.
 * @param[in]   exInfo        - extended information.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_updateTaskExSafe(uint32_t taskId, tlkapp_audioScheduler_taskInfo_t info, uint32_t sameDevTaskId, tlkapp_audioScheduler_extraInfo_t exInfo);

/**
 * @brief       This function safely updates a task.
 * @param[in]   taskId        - the task ID.
 * @param[in]   info          - task information.
 * @param[in]   sameDevTaskId - same device task ID.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_updateTaskSafe(uint32_t taskId, tlkapp_audioScheduler_taskInfo_t info, uint32_t sameDevTaskId);

/**
 * @brief       This function safely deletes a task.
 * @param[in]   taskId - the task ID.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_deleteTaskSafe(uint32_t taskId);
#endif // TLKAPP_AUDIO_MSG_H
