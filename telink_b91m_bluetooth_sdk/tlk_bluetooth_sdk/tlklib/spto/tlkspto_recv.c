/********************************************************************************************************
 * @file    tlkspto_recv.c
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


static void tlkspto_recv_normalPacketDeal(uint8_t ptype, uint8_t *pData, uint16_t dataLen);

static tlkspto_recv_ctrl_t spTlkSptoRecvCtrl = {0};

/**
 * @brief       Register receive callback function.
 * @param[in]   recvCB - Receive callback function.
 * @return      none.
 */
void tlkspto_recv_regCB(TlkCpcRecvCB recvCB)
{
    spTlkSptoRecvCtrl.recvCB = recvCB;
}

/**
 * @brief       Reset the receive module.
 * @param[in]   none.
 * @return      none.
 */
void tlkspto_recv_reset(void)
{
    spTlkSptoRecvCtrl.mstate   = TLKSPTO_RECV_MSTATE_HEAD;
    spTlkSptoRecvCtrl.recvLens = 0;
}

/**
 * @brief       Handle received data.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      none.
 */
void tlkspto_recv_handler(uint8_t *pData, uint16_t dataLen)
{
    int      indexI;
    int      indexJ;
    uint16_t tempVar0;
    uint16_t tempVar1;

    if (spTlkSptoRecvCtrl.recvCB == NULL) {
        return;
    }

    for (indexI = 0; indexI < dataLen; indexI++) {
        if (pData[indexI] == TLKSPTO_FRAME_HEAD_SIGN) {
            spTlkSptoRecvCtrl.mstate   = TLKSPTO_RECV_MSTATE_BODY;
            spTlkSptoRecvCtrl.recvLens = 0;
        } else if (pData[indexI] == TLKSPTO_FRAME_TAIL_SIGN) {
            if (spTlkSptoRecvCtrl.mstate == TLKSPTO_RECV_MSTATE_BODY && spTlkSptoRecvCtrl.recvLens >= 6) {
                spTlkSptoRecvCtrl.mstate = TLKSPTO_RECV_MSTATE_READY;
            } else {
                spTlkSptoRecvCtrl.mstate   = TLKSPTO_RECV_MSTATE_BODY;
                spTlkSptoRecvCtrl.recvLens = 0;
            }
        } else if (spTlkSptoRecvCtrl.mstate == TLKSPTO_RECV_MSTATE_BODY) {
            if (spTlkSptoRecvCtrl.recvLens < sizeof(spTlkSptoRecvCtrl.pBuffer)) {
                spTlkSptoRecvCtrl.pBuffer[spTlkSptoRecvCtrl.recvLens++] = pData[indexI];
            } else {
                spTlkSptoRecvCtrl.mstate   = TLKSPTO_RECV_MSTATE_HEAD;
                spTlkSptoRecvCtrl.recvLens = 0;
            }
        }
        if (spTlkSptoRecvCtrl.mstate != TLKSPTO_RECV_MSTATE_READY) {
            continue;
        }

        // Check whether escaping is required
        spTlkSptoRecvCtrl.mstate = TLKSPTO_RECV_MSTATE_HEAD;
        for (indexJ = 0; indexJ < spTlkSptoRecvCtrl.recvLens; indexJ++) {
            if (spTlkSptoRecvCtrl.pBuffer[indexJ] == TLKSPTO_ESCAPE_CHARS) {
                break;
            }
        }
        if (indexJ != spTlkSptoRecvCtrl.recvLens) { // Escape the data
            int offset = 0;
            for (indexJ = 0; indexJ < spTlkSptoRecvCtrl.recvLens; indexJ++) {
                if (spTlkSptoRecvCtrl.pBuffer[indexJ] != TLKSPTO_ESCAPE_CHARS) {
                    spTlkSptoRecvCtrl.pBuffer[offset++] = spTlkSptoRecvCtrl.pBuffer[indexJ];
                } else {
                    if (indexJ + 1 >= spTlkSptoRecvCtrl.recvLens) {
                        break;
                    }
                    indexJ++;
                    if (spTlkSptoRecvCtrl.pBuffer[indexJ] == TLKSPTO_ESCAPE_CHARS_RAW) {
                        spTlkSptoRecvCtrl.pBuffer[offset++] = TLKSPTO_ESCAPE_CHARS;
                    } else if (spTlkSptoRecvCtrl.pBuffer[indexJ] == TLKSPTO_ESCAPE_CHARS_HEAD) {
                        spTlkSptoRecvCtrl.pBuffer[offset++] = TLKSPTO_FRAME_HEAD_SIGN;
                    } else if (spTlkSptoRecvCtrl.pBuffer[indexJ] == TLKSPTO_ESCAPE_CHARS_TAIL) {
                        spTlkSptoRecvCtrl.pBuffer[offset++] = TLKSPTO_FRAME_TAIL_SIGN;
                    } else {
                        break;
                    }
                }
            }
            if (indexJ != spTlkSptoRecvCtrl.recvLens) {
                continue; // escaping failure
            }
            spTlkSptoRecvCtrl.recvLens = offset;
        }

        // Check CRC is correct
        tempVar0 = spTlkSptoRecvCtrl.pBuffer[spTlkSptoRecvCtrl.recvLens - 1];
        tempVar0 <<= 8;
        tempVar0 |= spTlkSptoRecvCtrl.pBuffer[spTlkSptoRecvCtrl.recvLens - 2];
        tempVar1 = tlkalg_crc16_calc(spTlkSptoRecvCtrl.pBuffer, spTlkSptoRecvCtrl.recvLens - 2);
        if (tempVar0 != tempVar1) {
            continue;
        }

        //
        tempVar0 = (spTlkSptoRecvCtrl.pBuffer[1] & 0xF0) >> 4;
        tempVar1 = ((spTlkSptoRecvCtrl.pBuffer[1] & 0x0F) << 8) | spTlkSptoRecvCtrl.pBuffer[0];
        if (tempVar1 < 4 || tempVar1 != spTlkSptoRecvCtrl.recvLens) {
            continue;
        }

        if (tempVar0 == TLKPRT_COMM_PTYPE_FCS) {
            continue;
        }

        if ((spTlkSptoRecvCtrl.pBuffer[1] & 0x08) == 0) {
            tlkspto_recv_normalPacketDeal(tempVar0, spTlkSptoRecvCtrl.pBuffer + 2, tempVar1 - 4);
        } else if (tempVar1 >= 6) {
            tlkspto_recv_normalPacketDeal(tempVar0, spTlkSptoRecvCtrl.pBuffer + 4, tempVar1 - 6);
        }
    }
}

/**
 * @brief       Deal with normal packet.
 * @param[in]   ptype - Packet type.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      none.
 */
static void tlkspto_recv_normalPacketDeal(uint8_t ptype, uint8_t *pData, uint16_t dataLen)
{
    if (ptype == TLKPRT_COMM_PTYPE_CMD || ptype == TLKPRT_COMM_PTYPE_EVT) {
        uint16_t lens;
        uint16_t msgID;
        uint8_t  mtype;
        mtype = pData[0];
        msgID = pData[1] | ((pData[3] & 0xF0) << 8);
        lens  = pData[2] | ((pData[3] & 0x0F) << 8);
        if (4 + lens > dataLen) {
            return;
        }
        spTlkSptoRecvCtrl.recvCB(((uint32_t)msgID << 16) | ((uint32_t)mtype << 8) | ptype, 0, pData + 4, lens);
    } else if (ptype == TLKPRT_COMM_PTYPE_DAT) {
        uint32_t numb;
        uint16_t lens;
        uint8_t  datID;
        datID = pData[0];
        numb  = (((uint32_t)pData[2]) << 8) | pData[1] | (((uint32_t)pData[4] & 0xF0) << 12);
        lens  = (((uint16_t)pData[4] & 0x0F) << 8) | pData[3];
        if (5 + lens > dataLen) {
            return;
        }
        spTlkSptoRecvCtrl.recvCB(((uint32_t)datID << 8) | ptype, numb, pData + 5, lens);
    }
}


#endif // #if (TLK_CFG_UART_TOOL_ENABLE)
