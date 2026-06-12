/********************************************************************************************************
 * @file    tlkmw_user_ctrl.h
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
#ifndef TLKMW_USER_CTRL_H
#define TLKMW_USER_CTRL_H

#define TLKMW_USER_CTRL_HEADER      5 //type(1Octet) + channel(1Octet) + dataLen(2Octets) + opcode(1Octet)
#define TLKMW_USER_CTRL_CHN_MAX_NUM 4

enum
{
    TLKMW_USER_CTRL_MODE_NONE = 0,
    TLKMW_USER_CTRL_MODE_OTA,
    TLKMW_USER_CTRL_MODE_AUDIO,
    TLKMW_USER_CTRL_MODE_KEY,
    TLKMW_USER_CTRL_MODE_MAX,
};

typedef struct sTlkMwUsrCtrlBufferNode
{
    uint8_t                         type;
    uint8_t                         channel;
    uint16_t                        buffer_size;
    uint16_t                        remain_size;
    uint16_t                        resv;
    uint8_t                        *pBuffer;
    struct sTlkMwUsrCtrlBufferNode *pNext;
    struct sTlkMwUsrCtrlBufferNode *pPrev;
} sTlkMwUsrCtrlBufferNode_t;

typedef struct
{
    uint32_t                   taskID;
    sTlkMwUsrCtrlBufferNode_t *pBufferHead;
} sTlkMwUsrCtrlTaskNode_t;

/**
 * @brief      Initialize the user control module
 * @param      none
 * @return     none
 */
void tlkmw_user_ctrl_init();

/**
 * @brief      Lock the user control mutex
 * @param      none
 * @return     none
 */
void tlkmw_userctrl_mutex_lock(void);

/**
 * @brief      Unlock the user control mutex
 * @param      none
 * @return     none
 */
void tlkmw_userctrl_mutex_unlock(void);

/**
 * @brief      Push data to a task
 * @param[in]  taskID  - task identifier
 * @param[in]  designate_chn -  designated channel
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @return     int - TLK_ENONE if success, error code otherwise
 * @note       This function is used to push ota data to ota module.
 * 			   If designate_chn is 0, means use the channel parsed from data,otherwise, use the designated channel.
 */
int tlkmw_userctrl_pushDataToTask(uint32_t taskID, uint8_t designate_chn, uint8_t *pData, uint16_t dataLen);

/**
 * @brief      Push OTA data to a task
 * @param[in]  taskID  - task identifier
 * @param[in]  channel - communication channel
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @return     int - TLK_ENONE if success, error code otherwise
 */
int tlkmw_userctrl_pushOtaDataToTask(uint32_t taskID, uint8_t channel, uint8_t *pData, uint16_t dataLen);

/**
 * @brief      Handle common user control operations
 * @param      none
 * @return     none
 */
void tlkmw_user_ctrl_common_handler();

#endif // #ifndef TLKMW_USER_CTRL_H
