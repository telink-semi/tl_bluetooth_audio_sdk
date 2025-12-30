/********************************************************************************************************
 * @file    tlkspto.c
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
#include "tlkapi/tlkapi.h"
#if (TLK_CFG_UART_TOOL_ENABLE)
    #include "tlkalg/digest/crc/tlkalg_crc.h"
    #include "tlkspto.h"
    #include "tlkspto_recv.h"
    #include "tlkspto_send.h"

/**
 * @brief       Reset the Serial Protocol module.
 * @param[in]   none.
 * @return      none.
 */
void tlkspto_reset(void)
{
    tlkspto_recv_reset();
}

/**
 * @brief       Process received data.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      none.
 */
void tlkspto_recvProc(uint8_t *pData, uint16_t dataLen)
{
    tlkspto_recv_handler(pData, dataLen);
}

/**
 * @brief       Process send data.
 * @param[in]   param0 - Parameter 0.
 * @param[in]   param1 - Parameter 1.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_sendProc(uint32_t param0, uint32_t param1, uint8_t *pData, uint16_t dataLen)
{
    uint8_t ptype;

    ptype = param0 & 0xFF;
    if (ptype == TLKPRT_COMM_PTYPE_CMD || ptype == TLKPRT_COMM_PTYPE_EVT) {
        uint8_t mtype;
        uint16_t msgID;
        mtype = (param0 & 0xFF00) >> 8;
        msgID = (param0 & 0x0FFF0000) >> 16;
        if (ptype == TLKPRT_COMM_PTYPE_CMD) {
            return tlkspto_send_cmd(mtype, msgID, pData, dataLen);
        } else {
            return tlkspto_send_evt(mtype, msgID, pData, dataLen);
        }
    } else if (ptype == TLKPRT_COMM_PTYPE_RSP) {
        uint8_t mtype;
        uint16_t msgID;
        uint8_t status;
        uint8_t reason;
        mtype  = (param0 & 0xFF00) >> 8;
        msgID  = (param0 & 0x0FFF0000) >> 16;
        status = param1 & 0xFF;
        reason = (param1 & 0xFF00) >> 8;
        return tlkspto_send_rsp(mtype, msgID, status, reason, pData, dataLen);
    } else if (ptype == TLKPRT_COMM_PTYPE_DAT) {
        uint8_t datID;
        uint32_t number;
        datID  = (param0 & 0xFF00) >> 8;
        number = (param1 & 0x0FFFFF);
        return tlkspto_send_dat(datID, number, pData, dataLen);
    } else {
        return -TLK_ENOSUPPORT;
    }
}

/**
 * @brief       Get receive buffer size.
 * @param[in]   none.
 * @return      Receive buffer size.
 */
int tlkspto_recv_getBufferSize(void)
{
    return tlkspto_send_getBufferSize();
}


#endif // #if (TLK_CFG_UART_TOOL_ENABLE)
