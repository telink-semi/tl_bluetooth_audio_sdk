/********************************************************************************************************
 * @file    tlkos_msgq.h
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
 * @brief     Creates a message queue.
 * @param[out] pMsgQHandle Pointer to store the created message queue handle.
 * @param[in] msgMaxSize Maximum size of each message in the queue.
 * @param[in] qLength Maximum number of messages in the queue.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_msgq_create(TlkOsMsgQHandle_t *pMsgQHandle, uint32_t msgMaxSize, uint32_t qLength);

/**
 * @brief     Destroys a message queue.
 * @param[in] msgQHandle Handle of the message queue to be destroyed.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_msgq_destroy(TlkOsMsgQHandle_t msgQHandle);

/**
 * @brief     Sends a message to the message queue.
 * @param[in] msgQHandle Handle of the message queue.
 * @param[in] pData Pointer to the message data.
 * @param[in] dataLen Length of the message data.
 * @param[in] blockTimeMs Blocking time in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_msgq_send(TlkOsMsgQHandle_t msgQHandle, uint8_t *pData, uint32_t dataLen, uint32_t blockTimeMs);

/**
 * @brief     Waits to receive a message from the message queue.
 * @param[in] msgQHandle Handle of the message queue.
 * @param[out] pBuff Buffer to store the received message.
 * @param[out] recLen Pointer to store the length of the received message.
 * @param[in] buffLen Size of the buffer.
 * @param[in] blockTimeMs Blocking time in milliseconds.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkos_msgq_wait(TlkOsMsgQHandle_t msgQHandle, uint8_t *pBuff, uint32_t *recLen, uint32_t buffLen, uint32_t blockTimeMs);