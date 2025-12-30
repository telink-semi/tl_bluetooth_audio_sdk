/********************************************************************************************************
 * @file    tlkspto_recv.h
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
#ifndef TLKSPTO_RECV_H
#define TLKSPTO_RECV_H

typedef enum
{
    TLKSPTO_RECV_MSTATE_HEAD = 0,
    TLKSPTO_RECV_MSTATE_BODY,
    TLKSPTO_RECV_MSTATE_READY,
} TLKSPTO_RECV_MSTATE_ENUM;

/**
 * @brief       Reset the receive module.
 * @param[in]   none.
 * @return      none.
 */
void tlkspto_recv_reset(void);

/**
 * @brief       Register receive callback function.
 * @param[in]   recvCB - Receive callback function.
 * @return      none.
 */
void tlkspto_recv_regCB(TlkCpcRecvCB recvCB);

/**
 * @brief       Set receive buffer.
 * @param[in]   pBuffer - Buffer pointer.
 * @param[in]   buffLen - Buffer length.
 * @return      none.
 */
void tlkspto_recv_setBuffer(uint8_t *pBuffer, uint16_t buffLen);

/**
 * @brief       Handle received data.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      none.
 */
void tlkspto_recv_handler(uint8_t *pData, uint16_t dataLen);


#endif // TLKSPTO_RECV_H
