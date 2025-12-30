/********************************************************************************************************
 * @file    tlkmw_user_ctrl.c
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
#include "tlkmw_user_ctrl.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "ota/tlk_ota_protocol_common.h"

#if (TLK_MW_USER_CTRL_ENABLE)

/**
 * @brief      Wake up the user control thread
 * @param[in]  none
 * @param[out] none
 * @return     none
 */
void tlkmw_user_ctrl_wakeUpThread(void)
{
    tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USER_CTRL);
}

/**
 * @brief      Initialize the user control module
 * @param[in]  none
 * @param[out] none
 * @return     none
 */
void tlkmw_user_ctrl_init()
{
    tlkmw_ota_common_init();
    tlksys_pm_regChn(TLKSYS_PM_CHN_OTA);
    tlksys_pm_setChn(TLKSYS_PM_CHN_OTA, 0, 0);
}

/**
 * @brief      Lock the user control mutex
 * @param[in]  none
 * @param[out] none
 * @return     none
 */
void tlkmw_userctrl_mutex_lock(void)
{
    tlksys_mutex_lock(TLKSYS_MUTEX_USERCTRL);
}

/**
 * @brief      Unlock the user control mutex
 * @param[in]  none
 * @param[out] none
 * @return     none
 */
void tlkmw_userctrl_mutex_unlock(void)
{
    tlksys_mutex_unlock(TLKSYS_MUTEX_USERCTRL);
}

static sTlkMwUsrCtrlTaskNode_t sTlkMwUsrCtrlTaskList[TLKMW_USER_CTRL_CHN_MAX_NUM];

/**
 * @brief      Initialize the task list
 * @param[in]  none
 * @param[out] none
 * @return     none
 */
void tlkmw_initTaskList()
{
    for (int i = 0; i < TLKMW_USER_CTRL_CHN_MAX_NUM; i++) {
        sTlkMwUsrCtrlTaskList[i].taskID      = 0;
        sTlkMwUsrCtrlTaskList[i].pBufferHead = NULL;
    }
}

/**
 * @brief      Print the task list for debugging
 * @param[in]  none
 * @param[out] none
 * @return     none
 */
void tlkmw_user_ctrl_print_task_list()
{
    for (uint8_t i = 0; i < TLKMW_USER_CTRL_CHN_MAX_NUM; i++) {
        sTlkMwUsrCtrlTaskNode_t *pTask = &sTlkMwUsrCtrlTaskList[i];

        tlk_printf("taskID[%x] pBufferHead[%x]", pTask->taskID, pTask->pBufferHead);

        sTlkMwUsrCtrlBufferNode_t *pCurrent = pTask->pBufferHead;
        while (pCurrent != NULL) {
            tlk_printf("type[%x] pBuffer[%x] buffer_size[%d]", pCurrent->type, pCurrent->pBuffer, pCurrent->buffer_size);
            tlkapi_array(0xffffffff, "[OTA]", "OTA data", pCurrent->pBuffer, pCurrent->buffer_size);
            pCurrent = pCurrent->pNext;
        }
    }
}

/**
 * @brief      Find a task node by task ID
 * @param[in]  taskID - task identifier
 * @param[out] none
 * @return     sTlkMwUsrCtrlTaskNode_t* - pointer to the task node, or NULL if not found
 */
sTlkMwUsrCtrlTaskNode_t *tlkmw_findTaskNode(uint32_t taskID)
{
    for (int i = 0; i < TLKMW_USER_CTRL_CHN_MAX_NUM; i++) {
        if (sTlkMwUsrCtrlTaskList[i].taskID == taskID) {
            return &sTlkMwUsrCtrlTaskList[i];
        }
    }
    return NULL;
}

/**
 * @brief      Find a task node by task ID and type
 * @param[in]  taskID - task identifier
 * @param[in]  type   - task type
 * @param[out] none
 * @return     sTlkMwUsrCtrlBufferNode_t* - pointer to the buffer node, or NULL if not found
 */
sTlkMwUsrCtrlBufferNode_t *tlkmw_find_taskNode_by_type(uint32_t taskID, uint8_t type)
{
    sTlkMwUsrCtrlTaskNode_t *pTask = tlkmw_findTaskNode(taskID);
    if (pTask == NULL) {
        return NULL;
    }

    sTlkMwUsrCtrlBufferNode_t *current = pTask->pBufferHead;
    while (current != NULL) {
        if (current->type == type) {
            return current;
        }
        current = current->pNext;
    }
    return NULL;
}

/**
 * @brief      Create a new task node
 * @param[in]  taskID - task identifier
 * @param[out] none
 * @return     sTlkMwUsrCtrlTaskNode_t* - pointer to the new task node, or NULL if failed
 */
sTlkMwUsrCtrlTaskNode_t *tlkmw_createTaskNode(uint32_t taskID)
{
    for (int i = 0; i < TLKMW_USER_CTRL_CHN_MAX_NUM; i++) {
        if (sTlkMwUsrCtrlTaskList[i].taskID == 0) {
            sTlkMwUsrCtrlTaskList[i].taskID      = taskID;
            sTlkMwUsrCtrlTaskList[i].pBufferHead = NULL;
            return &sTlkMwUsrCtrlTaskList[i];
        }
    }
    return NULL;
}

/**
 * @brief      Push data to a task
 * @param[in]  taskID  - task identifier
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[out] none
 * @return     int - TLK_ENONE if success, error code otherwise
 */
int tlkmw_userctrl_pushDataToTask(uint32_t taskID, uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < 5) {
        return -TLK_EPARAM;
    }

    int      ret    = TLK_ENONE;
    uint16_t offset = 0;

    tlkmw_userctrl_mutex_lock();

    while (offset < dataLen && (dataLen - offset) >= 5) {
        uint8_t  type     = pData[offset];
        uint8_t  channel  = pData[offset + 1];
        uint16_t parseLen = pData[offset + 2] | (pData[offset + 3] << 8);

        if (type < TLKMW_USER_CTRL_MODE_OTA || type > TLKMW_USER_CTRL_MODE_MAX) {
            ret = -TLK_EPARAM;
            break;
        }

        sTlkMwUsrCtrlTaskNode_t *pTask = tlkmw_findTaskNode(taskID);
        if (pTask == NULL) {
            pTask = tlkmw_createTaskNode(taskID);
            if (pTask == NULL) {
                return -TLK_ENOITEM;
            }
            pTask->taskID = taskID;
        }

        sTlkMwUsrCtrlBufferNode_t *pBufferNode = NULL;

        pBufferNode = (sTlkMwUsrCtrlBufferNode_t *)tlkos_malloc(sizeof(sTlkMwUsrCtrlBufferNode_t));
        if (pBufferNode == NULL) {
            ret = -TLK_EFAIL;
            break;
        }

        memset(pBufferNode, 0, sizeof(sTlkMwUsrCtrlBufferNode_t));
        pBufferNode->type    = type;
        pBufferNode->channel = channel;

        pBufferNode->pBuffer = (uint8_t *)tlkos_malloc(parseLen - 1);
        if (pBufferNode->pBuffer == NULL) {
            tlkos_free(pBufferNode);
            ret = -TLK_EFAIL;
            break;
        }

        memcpy(pBufferNode->pBuffer, pData + offset + 1, parseLen - 1);
        pBufferNode->buffer_size = parseLen - 1;

        if (pTask->pBufferHead == NULL) {
            pTask->pBufferHead = pBufferNode;
        } else {
            sTlkMwUsrCtrlBufferNode_t *current = pTask->pBufferHead;
            while (current->pNext != NULL) {
                current = current->pNext;
            }
            current->pNext = pBufferNode;
        }

        offset += parseLen;
    }

    tlkmw_userctrl_mutex_unlock();
    tlkmw_user_ctrl_wakeUpThread();

    return ret;
}

/**
 * @brief      Push OTA data to a task
 * @param[in]  taskID  - task identifier
 * @param[in]  channel - communication channel
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[out] none
 * @return     int - TLK_ENONE if success, error code otherwise
 */
int tlkmw_userctrl_pushOtaDataToTask(uint32_t taskID, uint8_t channel, uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen == 0) {
        return -TLK_EPARAM;
    }

    tlkmw_userctrl_mutex_lock();

    sTlkMwUsrCtrlTaskNode_t *pTask = tlkmw_findTaskNode(taskID);
    if (pTask == NULL) {
        pTask = tlkmw_createTaskNode(taskID);
        if (pTask == NULL) {
            return -TLK_ENOITEM;
        }
        pTask->taskID = taskID;
    }

    sTlkMwUsrCtrlBufferNode_t *pBufferNode = NULL;

    pBufferNode = (sTlkMwUsrCtrlBufferNode_t *)tlkos_malloc(sizeof(sTlkMwUsrCtrlBufferNode_t));
    if (pBufferNode == NULL) {
        return -TLK_EFAIL;
    }

    tmemset(pBufferNode, 0, sizeof(sTlkMwUsrCtrlBufferNode_t));
    pBufferNode->type    = TLKMW_USER_CTRL_MODE_OTA;
    pBufferNode->channel = channel;

    /*need push channel at first byte.*/
    pBufferNode->pBuffer = (uint8_t *)tlkos_malloc(dataLen + 1);
    if (pBufferNode->pBuffer == NULL) {
        tlkos_free(pBufferNode);
        return -TLK_EFAIL;
    }

    pData[0] = channel;

    tmemcpy(pBufferNode->pBuffer, pData + 1, dataLen);
    pBufferNode->buffer_size = dataLen + 1;

    if (pTask->pBufferHead == NULL) {
        pTask->pBufferHead = pBufferNode;
    } else {
        sTlkMwUsrCtrlBufferNode_t *current = pTask->pBufferHead;
        while (current->pNext != NULL) {
            current = current->pNext;
        }
        current->pNext = pBufferNode;
    }

    tlkmw_userctrl_mutex_unlock();
    tlkmw_user_ctrl_wakeUpThread();

    return TLK_ENONE;
}

/**
 * @brief      Remove a task node
 * @param[in]  taskID - task identifier
 * @param[out] none
 * @return     int - TLK_ENONE if success, error code otherwise
 */
int tlkmw_removeTaskNode(uint32_t taskID)
{
    for (int i = 0; i < TLKMW_USER_CTRL_CHN_MAX_NUM; i++) {
        if (sTlkMwUsrCtrlTaskList[i].taskID == taskID) {
            sTlkMwUsrCtrlBufferNode_t *current = sTlkMwUsrCtrlTaskList[i].pBufferHead;
            while (current != NULL) {
                sTlkMwUsrCtrlBufferNode_t *next = current->pNext;
                if (current->pBuffer != NULL) {
                    tlkos_free(current->pBuffer);
                }
                tlkos_free(current);
                current = next;
            }

            sTlkMwUsrCtrlTaskList[i].taskID      = 0;
            sTlkMwUsrCtrlTaskList[i].pBufferHead = NULL;
            return TLK_ENONE;
        }
    }
    return -TLK_ENOITEM;
}

/**
 * @brief      Handle common user control operations
 * @param[in]  none
 * @param[out] none
 * @return     none
 */
void tlkmw_user_ctrl_common_handler()
{
    tlkmw_userctrl_mutex_lock();

    for (uint8_t i = 0; i < TLKMW_USER_CTRL_CHN_MAX_NUM; i++) {
        sTlkMwUsrCtrlTaskNode_t *pTask = &sTlkMwUsrCtrlTaskList[i];
        if (pTask->taskID == 0) {
            continue;
        }

        while (pTask->pBufferHead != NULL) {
            sTlkMwUsrCtrlBufferNode_t *pCurrent = pTask->pBufferHead;
            if (pCurrent->pBuffer == NULL) {
                pTask->pBufferHead = pCurrent->pNext;
                tlkos_free(pCurrent);
                continue;
            }

            if (pCurrent->type == TLKMW_USER_CTRL_MODE_OTA) {
                /*OTA processing*/
                uint32_t ota_busy_taskid = tlkmw_ota_common_get_busy_taskid();
                if (ota_busy_taskid == 0 || ota_busy_taskid == pTask->taskID) {
                    // tlk_printf("------taskID[%x], channel[%d], data_len[%d]-----", pTask->taskID, pCurrent->channel, pCurrent->buffer_size);
                    tlkmw_ota_common_recv_data(pTask->taskID, pCurrent->channel, pCurrent->pBuffer, pCurrent->buffer_size);
                } else {
                    tlk_printf("recv new ota data, but ota is busy, drop it. new task[%x], busy task[%x]", pTask->taskID, ota_busy_taskid);
                }
            } else {
                tlk_printf("recv other data typs:[%d]", pCurrent->type);
                /*Other processing*/
            }

            pTask->pBufferHead = pCurrent->pNext;
            tlkos_free(pCurrent->pBuffer);
            tlkos_free(pCurrent);
        }

        /*All data buffer is deal done, so clean task node*/
        tlkmw_removeTaskNode(pTask->taskID);
    }

    tlkmw_userctrl_mutex_unlock();
}

#else

/**
 * @brief      Push data to a task (unsupported implementation)
 * @param[in]  taskID  - task identifier
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[out] none
 * @return     int - TLK_ENOSUPPORT error code
 */
int tlkmw_userctrl_pushDataToTask(uint32_t taskID, uint8_t *pData, uint16_t dataLen)
{
    (void)taskID;
    (void)pData;
    (void)dataLen;

    return -TLK_ENOSUPPORT;
}

/**
 * @brief      Push OTA data to a task (unsupported implementation)
 * @param[in]  taskID  - task identifier
 * @param[in]  channel - communication channel
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[out] none
 * @return     int - TLK_ENOSUPPORT error code
 */
int tlkmw_userctrl_pushOtaDataToTask(uint32_t taskID, uint8_t channel, uint8_t *pData, uint16_t dataLen)
{
    (void)taskID;
    (void)channel;
    (void)pData;
    (void)dataLen;

    return -TLK_ENOSUPPORT;
}
#endif
