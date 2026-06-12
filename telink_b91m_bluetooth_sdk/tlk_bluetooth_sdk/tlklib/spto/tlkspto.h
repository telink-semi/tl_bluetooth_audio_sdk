/********************************************************************************************************
 * @file    tlkspto.h
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
#ifndef TLKSPTO_H
#define TLKSPTO_H


#define TLKSPTO_DBG_FLAG          ((TLK_MAJOR_DBGID_MW << 24) | (TLK_MINOR_DBGID_MW_COMM << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKSPTO_DBG_SIGN          "[COMM]"


#define TLKSPTO_FRM_EXTLEN        8
#define TLKSPTO_CMDHEAD_LENS      4
#define TLKSPTO_DATHEAD_LENS      5


#define TLKSPTO_FRAME_HEAD_SIGN   0xAC
#define TLKSPTO_FRAME_TAIL_SIGN   0xCA
#define TLKSPTO_ESCAPE_CHARS      0xDC // Must not TLKSPTO1_HEAD_SIGN or TLKPRT_COMM_HEAD_TAIL
#define TLKSPTO_ESCAPE_CHARS_RAW  0x01 // eg: DC -> DC 01
#define TLKSPTO_ESCAPE_CHARS_HEAD 0x02 // eg: AC -> DC 02
#define TLKSPTO_ESCAPE_CHARS_TAIL 0x03 // eg: CA -> DC 03


// SPTO: Communication Protocol Control

typedef int (*TlkCpcSendCB)(uint8_t *pData, uint16_t dataLen);
typedef int (*TlkCpcRecvCB)(uint32_t param0, uint32_t param1, uint8_t *pData, uint16_t dataLen);
typedef int (*TlkCpcResetCB)(void);

typedef enum
{
    TLKSPTO_AUTH_STATUS_NONE    = 0,
    TLKSPTO_AUTH_STATUS_SUCCESS = 1,
    TLKSPTO_AUTH_STATUS_FAILURE = 2,
    TLKSPTO_AUTH_STATUS_RUNNING = 3,
} TLKSPTO_AUTH_STATUS_ENUM;

typedef struct
{
    uint32_t     sendLens;
    uint8_t      pBuffer[160];
    TlkCpcSendCB sendCB;
} tlkspto_send_ctrl_t;

typedef struct
{
    uint16_t     mstate;
    uint16_t     recvLens;
    uint8_t      pBuffer[432];
    TlkCpcRecvCB recvCB;
} tlkspto_recv_ctrl_t;

/**
 * @brief       Reset the Serial Protocol module.
 * @param[in]   none.
 * @return      none.
 */
void tlkspto_reset(void);

/**
 * @brief       Register receive callback function.
 * @param[in]   recvCB - Receive callback function.
 * @return      none.
 */
void tlkspto_recv_regCB(TlkCpcRecvCB recvCB);

/**
 * @brief       Get receive buffer size.
 * @param[in]   none.
 * @return      Receive buffer size.
 */
int tlkspto_recv_getBufferSize(void);

/**
 * @brief       Process received data.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      none.
 */
void tlkspto_recvProc(uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Register send callback function.
 * @param[in]   sendCB - Send callback function.
 * @return      none.
 */
void tlkspto_send_regCB(TlkCpcSendCB sendCB);

/**
 * @brief       Process send data.
 * @param[in]   param0 - Parameter 0.
 * @param[in]   param1 - Parameter 1.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_sendProc(uint32_t param0, uint32_t param1, uint8_t *pData, uint16_t dataLen);


#endif // TLKSPTO_H
