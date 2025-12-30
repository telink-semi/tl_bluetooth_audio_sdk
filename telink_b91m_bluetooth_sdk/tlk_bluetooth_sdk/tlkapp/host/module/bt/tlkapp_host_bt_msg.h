/********************************************************************************************************
 * @file    tlkapp_host_bt_msg.h
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

/**
 * @brief       This function sends a common response message.
 * @param[in]   cmdID    - command ID.
 * @param[in]   status   - status of the response.
 * @param[in]   reason   - reason code.
 * @param[in]   pData    - pointer to the data payload.
 * @param[in]   dataLen  - length of the data payload.
 * @return      0 if success, otherwise error code.
 */
int tlkapp_btmgr_sendCommRsp(uint8_t cmdID, uint8_t status, uint8_t reason, uint8_t *pData, uint8_t dataLen);

/**
 * @brief       This function sends a common event message.
 * @param[in]   evtID    - event ID.
 * @param[in]   pData    - pointer to the data payload.
 * @param[in]   dataLen  - length of the data payload.
 * @return      0 if success, otherwise error code.
 */
int tlkapp_btmgr_sendCommEvt(uint8_t evtID, uint8_t *pData, uint8_t dataLen);

/**
 * @brief       Convert the profile type to communication type.
 * @param[in]   ptype    - The profile type.
 * @param[in]   usrID    - The user ID (Client or Server).
 * @return      The communication type (AG/HFP/A2DPSNK/A2DPSRC...).
 */
uint8_t tlkapp_btmgr_ptypeToCtype(uint8_t ptype, uint8_t usrID);

/**
 * @brief       Send ACL connect event to app.
 * @param[in]   handle   - The ACL handle.
 * @param[in]   status   - The current status.
 * @param[in]   pBtAddr  - The BT Address.
 * @return      none.
 */
void tlkapp_btmgr_sendAclConnectEvt(uint16_t handle, uint8_t status, uint8_t *pBtAddr);

/**
 * @brief       Send ACL disconnect event to app.
 * @param[in]   handle   - The ACL handle.
 * @param[in]   reason   - The disconnect reason.
 * @param[in]   pBtAddr  - The BT Address.
 * @return      none.
 */
void tlkapp_btmgr_sendAclDisconnEvt(uint16_t handle, uint8_t reason, uint8_t *pBtAddr);

/**
 * @brief       Send profile connect event to app.
 * @param[in]   handle   - The ACL handle.
 * @param[in]   status   - The current status.
 * @param[in]   ptype    - The profile type.
 * @param[in]   usrID    - The user ID (Client/Server).
 * @param[in]   pBtAddr  - The BT address.
 * @return      none.
 */
void tlkapp_btmgr_sendProfConnectEvt(uint16_t handle, uint8_t status, uint8_t ptype, uint8_t usrID, uint8_t *pBtAddr);

/**
 * @brief       Send profile disconnect event to app.
 * @param[in]   handle   - The ACL handle.
 * @param[in]   reason   - The disconnect reason.
 * @param[in]   ptype    - The profile type.
 * @param[in]   usrID    - The user ID (Client/Server).
 * @param[in]   pBtAddr  - The BT address.
 * @return      none.
 */
void tlkapp_btmgr_sendProfDisconnEvt(uint16_t handle, uint8_t reason, uint8_t ptype, uint8_t usrID, uint8_t *pBtAddr);

/**
 * @brief       This function starts inquiry process with specified device type.
 * @param[in]   dtype    - device type.
 * @return      none.
 */
void tlkapp_user_start_inquiry(uint8_t dtype);

/**
 * @brief       This function stops ongoing inquiry process.
 * @param[in]   none     - none.
 * @return      none.
 */
void tlkapp_user_stop_inquiry(void);

/**
 * @brief       This function connects to inquiry device by index.
 * @param[in]   index    - device index.
 * @return      none.
 */
void tlkapp_user_connect_inquiry_device_by_index(uint8_t index);

/**
 * @brief       This function handles Bluetooth messages.
 * @param[in]   msgID    - message ID.
 * @param[in]   pData    - pointer to message data.
 * @param[in]   dataLen  - length of message data.
 * @return      0 if success, otherwise error code.
 */
int tlkapp_btmgr_msgHandle(uint16_t msgID, uint8_t *pData, uint16_t dataLen);
