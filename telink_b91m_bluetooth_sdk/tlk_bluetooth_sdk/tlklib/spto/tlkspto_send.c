/********************************************************************************************************
 * @file    tlkspto_send.c
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
#include "tlkspto_send.h"

static int tlkspto_send_makeFrame(uint8_t pktType, uint8_t *pHead, uint16_t headLen, uint8_t *pBody, uint16_t bodyLen);


static tlkspto_send_ctrl_t spTlkSptoSendCtrl = {0};

/**
 * @brief       Register send callback function.
 * @param[in]   sendCB - Send callback function.
 * @return      none.
 */
void tlkspto_send_regCB(TlkCpcSendCB sendCB)
{
    spTlkSptoSendCtrl.sendCB = sendCB;
}

/**
 * @brief       Get buffer size.
 * @param[in]   none.
 * @return      Buffer size.
 */
int tlkspto_send_getBufferSize(void)
{
    return (sizeof(spTlkSptoSendCtrl.pBuffer) - 11 - 4);
}

/**
 * @brief       Send command.
 * @param[in]   mtype - Message type.
 * @param[in]   msgID - Message ID.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_send_cmd(uint8_t mtype, uint16_t msgID, uint8_t *pData, uint8_t dataLen)
{
    int     ret;
    uint8_t headLen;
    uint8_t head[4];

    if (spTlkSptoSendCtrl.sendCB == NULL) {
        return -TLK_ENOREADY;
    }

    headLen         = 0;
    head[headLen++] = (mtype & 0xFF);
    head[headLen++] = (msgID & 0xFF);
    head[headLen++] = (dataLen & 0xFF);
    head[headLen++] = ((dataLen & 0x0F00) >> 8) | ((msgID & 0xF00) >> 4);
    ret             = tlkspto_send_makeFrame(TLKPRT_COMM_PTYPE_CMD, head, 4, pData, dataLen);
    if (ret == TLK_ENONE) {
        ret = spTlkSptoSendCtrl.sendCB(spTlkSptoSendCtrl.pBuffer, spTlkSptoSendCtrl.sendLens);
    }
    return ret;
}

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
int tlkspto_send_rsp(uint8_t mtype, uint16_t msgID, uint8_t status, uint8_t reason, uint8_t *pData, uint8_t dataLen)
{
    int     ret;
    uint8_t headLen;
    uint8_t head[8];

    if (spTlkSptoSendCtrl.sendCB == NULL) {
        return -TLK_ENOREADY;
    }

    headLen         = 0;
    head[headLen++] = (mtype & 0xFF);
    head[headLen++] = (msgID & 0xFF);
    head[headLen++] = ((2 + dataLen) & 0xFF);
    head[headLen++] = (((2 + dataLen) & 0x0F00) >> 8) | ((msgID & 0xF00) >> 4);
    head[headLen++] = status;
    head[headLen++] = reason;
    ret             = TLK_ENONE;
    ret             = tlkspto_send_makeFrame(TLKPRT_COMM_PTYPE_RSP, head, 6, pData, dataLen);
    if (ret == TLK_ENONE) {
        ret = spTlkSptoSendCtrl.sendCB(spTlkSptoSendCtrl.pBuffer, spTlkSptoSendCtrl.sendLens);
    }
    return ret;
}

/**
 * @brief       Send event.
 * @param[in]   mtype - Message type.
 * @param[in]   msgID - Message ID.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_send_evt(uint8_t mtype, uint16_t msgID, uint8_t *pData, uint8_t dataLen)
{
    int     ret;
    uint8_t headLen;
    uint8_t head[4];

    if (spTlkSptoSendCtrl.sendCB == NULL) {
        return -TLK_ENOREADY;
    }

    headLen         = 0;
    head[headLen++] = (mtype & 0xFF);
    head[headLen++] = (msgID & 0xFF);
    head[headLen++] = (dataLen & 0xFF);
    head[headLen++] = ((dataLen & 0x0F00) >> 8) | ((msgID & 0xF00) >> 4);
    ret             = tlkspto_send_makeFrame(TLKPRT_COMM_PTYPE_EVT, head, 4, pData, dataLen);
    if (ret == TLK_ENONE) {
        ret = spTlkSptoSendCtrl.sendCB(spTlkSptoSendCtrl.pBuffer, spTlkSptoSendCtrl.sendLens);
    }
    return ret;
}

/**
 * @brief       Send data.
 * @param[in]   datID - Data ID.
 * @param[in]   numb - Number.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_send_dat(uint8_t datID, uint32_t numb, uint8_t *pData, uint16_t dataLen)
{
    int     ret;
    uint8_t head[8];

    if (spTlkSptoSendCtrl.sendCB == NULL) {
        return -TLK_ENOREADY;
    }
    if (pData == NULL || dataLen == 0 || dataLen > 256) {
        return -TLK_EPARAM;
    }

    head[0] = datID;           // DID
    head[1] = (numb & 0x00FF); // Num
    head[2] = (numb & 0xFF00) >> 8;
    head[3] = (dataLen & 0x00FF); // Lens
    head[4] = ((dataLen & 0x0F00) >> 8) | ((numb & 0x0F0000) >> 12);
    ret     = tlkspto_send_makeFrame(TLKPRT_COMM_PTYPE_DAT, head, 5, pData, dataLen);
    if (ret == TLK_ENONE) {
        ret = spTlkSptoSendCtrl.sendCB(spTlkSptoSendCtrl.pBuffer, spTlkSptoSendCtrl.sendLens);
    }

    return ret;
}

/**
 * @brief       Send data with specified packet type.
 * @param[in]   pktType - Packet type.
 * @param[in]   pHead - Head pointer.
 * @param[in]   headLen - Head length.
 * @param[in]   pBody - Body pointer.
 * @param[in]   bodyLen - Body length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkspto_send(uint8_t pktType, uint8_t *pHead, uint16_t headLen, uint8_t *pBody, uint16_t bodyLen)
{
    int ret;

    if (spTlkSptoSendCtrl.sendCB == NULL) {
        return -TLK_ENOREADY;
    }

    ret = tlkspto_send_makeFrame(pktType, pHead, headLen, pBody, bodyLen);
    if (ret == TLK_ENONE) {
        ret = spTlkSptoSendCtrl.sendCB(spTlkSptoSendCtrl.pBuffer, spTlkSptoSendCtrl.sendLens);
    }
    return ret;
}

/**
 * @brief       Make frame for sending.
 * @param[in]   pktType - Packet type.
 * @param[in]   pHead - Head pointer.
 * @param[in]   headLen - Head length.
 * @param[in]   pBody - Body pointer.
 * @param[in]   bodyLen - Body length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
static int tlkspto_send_makeFrame(uint8_t pktType, uint8_t *pHead, uint16_t headLen, uint8_t *pBody, uint16_t bodyLen)
{
    int      index;
    uint32_t count;
    uint8_t *pFrame;
    uint16_t frmLen;
    uint16_t tempVar;

    if (headLen + bodyLen == 0) {
        return -TLK_EPARAM;
    }
    if ((uint32_t)(headLen + bodyLen + 6) > sizeof(spTlkSptoSendCtrl.pBuffer)) {
        return -TLK_EOVERFLOW;
    }

    frmLen = 0;
    pFrame = spTlkSptoSendCtrl.pBuffer;

    frmLen           = 0;
    pFrame[frmLen++] = TLKSPTO_FRAME_HEAD_SIGN;
    pFrame[frmLen++] = (headLen + bodyLen + 4) & 0xFF;
    pFrame[frmLen++] = (((headLen + bodyLen + 4) & 0x0F00) >> 8) | ((pktType & 0x0F) << 4);
    if (headLen != 0) {
        tmemcpy(pFrame + frmLen, pHead, headLen);
        frmLen += headLen;
    }
    if (bodyLen != 0) {
        tmemcpy(pFrame + frmLen, pBody, bodyLen);
        frmLen += bodyLen;
    }
    tempVar          = tlkalg_crc16_calc(pFrame + 1, frmLen - 1);
    pFrame[frmLen++] = tempVar & 0xFF;
    pFrame[frmLen++] = (tempVar & 0xFF00) >> 8;

    count = 0;
    for (index = 1; index < frmLen; index++) {
        if (pFrame[index] == TLKSPTO_FRAME_HEAD_SIGN || pFrame[index] == TLKSPTO_ESCAPE_CHARS || pFrame[index] == TLKSPTO_FRAME_TAIL_SIGN) {
            count++;
        }
    }
    if (count != 0) {
        if (frmLen + count + 1 > sizeof(spTlkSptoSendCtrl.pBuffer)) {
            return -TLK_EOVERFLOW;
        }
        int offset = frmLen + count;
        for (index = frmLen - 1; index >= 1; index--) {
            if (pFrame[index] == TLKSPTO_FRAME_HEAD_SIGN) {
                pFrame[--offset] = TLKSPTO_ESCAPE_CHARS_HEAD;
                pFrame[--offset] = TLKSPTO_ESCAPE_CHARS;
            } else if (pFrame[index] == TLKSPTO_FRAME_TAIL_SIGN) {
                pFrame[--offset] = TLKSPTO_ESCAPE_CHARS_TAIL;
                pFrame[--offset] = TLKSPTO_ESCAPE_CHARS;
            } else if (pFrame[index] == TLKSPTO_ESCAPE_CHARS) {
                pFrame[--offset] = TLKSPTO_ESCAPE_CHARS_RAW;
                pFrame[--offset] = TLKSPTO_ESCAPE_CHARS;
            } else {
                pFrame[--offset] = pFrame[index];
            }
        }
        if (offset != 1) {
            return -TLK_EFAULT;
        }
        frmLen += count;
    }
    pFrame[frmLen++] = TLKSPTO_FRAME_TAIL_SIGN;

    spTlkSptoSendCtrl.sendLens = frmLen;

    return TLK_ENONE;
}


#endif // #if (TLK_CFG_UART_TOOL_ENABLE)
