/********************************************************************************************************
 * @file    tlkapp_lemgrMsg.h
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
#ifndef TLKAPP_LEMGR_MSG_H
#define TLKAPP_LEMGR_MSG_H

/**
 * @brief   Send common response message.
 * @param[in]   cmdID    - Command ID.
 * @param[in]   status   - Status of the response.
 * @param[in]   reason   - Reason code.
 * @param[in]   pData    - Pointer to the data to send.
 * @param[in]   dataLen  - Length of the data.
 * @return      Status of the operation.
 */
int tlkapp_lemgr_sendCommRsp(uint8_t cmdID, uint8_t status, uint8_t reason, uint8_t *pData, uint8_t dataLen);

/**
 * @brief   Send common event message.
 * @param[in]   evtID    - Event ID.
 * @param[in]   pData    - Pointer to the data to send.
 * @param[in]   dataLen  - Length of the data.
 * @return      Status of the operation.
 */
int tlkapp_lemgr_sendCommEvt(uint8_t evtID, uint8_t *pData, uint8_t dataLen);

/**
 * @brief   Handle incoming messages based on message ID.
 * @param[in]   msgID    - Message ID.
 * @param[in]   pData    - Pointer to the message data.
 * @param[in]   dataLen  - Length of the message data.
 * @return      Status of the operation.
 */
int tlkapp_lemgr_msgHandle(uint8_t msgID, uint8_t *pData, uint16_t dataLen);

/**
 * @brief   Start scanning for devices.
 * @return  None.
 */
void tlkapp_lemgr_start_scan(void);

#endif //TLKAPP_LEMGR_MSG_H
