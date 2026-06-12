/********************************************************************************************************
 * @file    tlkspto_send.h
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
#ifndef TLKSPTO_SEND_H
#define TLKSPTO_SEND_H

/**
 * @brief       Get buffer size.
 * @param[in]   none.
 * @return      Buffer size.
 */
int tlkspto_send_getBufferSize(void);

/**
 * @brief       Register send callback function.
 * @param[in]   sendCB - Send callback function.
 * @return      none.
 */
void tlkspto_send_regCB(TlkCpcSendCB sendCB);

/**
 * @brief       Send command.
 * @param[in]   mtype - Message type.
 * @param[in]   msgID - Message ID.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_send_cmd(uint8_t mtype, uint16_t msgID, uint8_t *pData, uint8_t dataLen);

/**
 * @brief       Send response.
 * @param[in]   mtype - Message type.
 * @param[in]   msgID - Message ID.
 * @param[in]   status - Status.
 * @param[in]   reason - Reason.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_send_rsp(uint8_t mtype, uint16_t msgID, uint8_t status, uint8_t reason, uint8_t *pData, uint8_t dataLen);

/**
 * @brief       Send event.
 * @param[in]   mtype - Message type.
 * @param[in]   msgID - Message ID.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_send_evt(uint8_t mtype, uint16_t msgID, uint8_t *pData, uint8_t dataLen);

/**
 * @brief       Send data.
 * @param[in]   datID - Data ID.
 * @param[in]   numb - Number.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_send_dat(uint8_t datID, uint32_t numb, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Send data with specified packet type.
 * @param[in]   pktType - Packet type.
 * @param[in]   pHead - Head pointer.
 * @param[in]   headLen - Head length.
 * @param[in]   pBody - Body pointer.
 * @param[in]   bodyLen - Body length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_send(uint8_t pktType, uint8_t *pHead, uint16_t headLen, uint8_t *pBody, uint16_t bodyLen);


#endif // TLKSPTO_SEND_H
