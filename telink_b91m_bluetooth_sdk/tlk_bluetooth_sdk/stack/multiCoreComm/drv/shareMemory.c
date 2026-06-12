/********************************************************************************************************
 * @file    shareMemory.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "platform/tlkipc_hal.h"
#include "platform/tlkipc_port.h"
#include "shareMemory.h"

#if (MCU_DUAL_CORE_ENABLE)

#define SM_FIFO_DATA_HEAD_LEN (sizeof(uint32_t) + sizeof(uint32_t))

static inline void share_memory_fifo_data_copy(tlk_safeFifo_t *fifo, uint32_t pos, uint32_t type, uint8_t *data, uint32_t dataLen)
{
    uint8_t *pBuffer = fifo->pBuffer + pos;
    tlkipc_memcpy(pBuffer, &type, sizeof(type));
    pBuffer += sizeof(type);
    tlkipc_memcpy(pBuffer, &dataLen, sizeof(dataLen));
    pBuffer += sizeof(dataLen);

    uint32_t tailLen = fifo->size - pos - SM_FIFO_DATA_HEAD_LEN;
    if (tailLen >= dataLen) {
        tlkipc_memcpy(pBuffer, data, dataLen);
    } else {
        tlkipc_memcpy(pBuffer, data, tailLen);
        tlkipc_memcpy(fifo->pBuffer, data + tailLen, dataLen - tailLen);
    }
}

tlk_sm_ret_e share_memory_data_push(tlk_sm_fifo_t *smFifo, uint32_t type, uint8_t *data, uint32_t dataLen)
{
    if (smFifo == NULL || smFifo->status != TLK_SHARE_MEMORY_STATUS_READY) {
        return TLK_SHARE_MEMORY_NOT_READY;
    }
    if (dataLen > TLK_SHARE_MEMORY_MAX_PAYLOAD_LEN) {
        return TLK_SHARE_MEMORY_ERROR_PARAM;
    }
    uint32_t idleLen  = 0;
    uint32_t critical = tlkipc_enterCritical();

    uint32_t woffset = smFifo->fifo.mallocPtr;
    uint32_t roffset = smFifo->fifo.rptr;
    if (roffset > woffset) {
        idleLen = roffset - woffset;
    } else {
        idleLen = smFifo->fifo.size + roffset - woffset;
    }
    if (idleLen < dataLen + 2 * SM_FIFO_DATA_HEAD_LEN) {
        tlkipc_leaveCritical(critical);
        return TLK_SHARE_MEMORY_FULL;
    }

    if (woffset + SM_FIFO_DATA_HEAD_LEN > smFifo->fifo.size) { //if ring fifo tail len < head len,just abandon
        woffset = 0;
    }
    smFifo->fifo.mallocPtr = (woffset + dataLen + SM_FIFO_DATA_HEAD_LEN) % smFifo->fifo.size;
    smFifo->fifo.enterCnt++;
    tlkipc_leaveCritical(critical);

    share_memory_fifo_data_copy(&smFifo->fifo, woffset, type, data, dataLen);

    critical         = tlkipc_enterCritical();
    tlk_sm_ret_e ret = TLK_SHARE_MEMORY_SUCCESS_NONEED_NOTIFY;
    smFifo->fifo.enterCnt--;
    if (smFifo->fifo.enterCnt == 0) {
        smFifo->fifo.wptr = smFifo->fifo.mallocPtr;
        ret               = TLK_SHARE_MEMORY_SUCCESS_NEED_NOTIFY;
    }
    tlkipc_leaveCritical(critical);

    return ret;
}

void share_memory_data_popAll(tlk_sm_fifo_t *smFifo)
{
    if ((smFifo == NULL) || (smFifo->status != TLK_SHARE_MEMORY_STATUS_READY)) {
        return;
    }
    while (smFifo->fifo.wptr != smFifo->fifo.rptr) {
        if (smFifo->fifo.rptr + SM_FIFO_DATA_HEAD_LEN > smFifo->fifo.size) {
            smFifo->fifo.rptr = 0;
            continue;
        }
        uint8_t *pBuffer = smFifo->fifo.pBuffer + smFifo->fifo.rptr;
        uint32_t type    = 0;
        uint32_t dataLen = 0;
        tlkipc_memcpy(&type, pBuffer, sizeof(type));
        pBuffer += sizeof(type);
        tlkipc_memcpy(&dataLen, pBuffer, sizeof(dataLen));
        pBuffer += sizeof(dataLen);

        if ((type >= TLK_SHARE_MEMORY_MESSAGE_TYPE_NUM || smFifo->cbArray[type] == NULL)) {
            smFifo->fifo.rptr = (smFifo->fifo.rptr + dataLen + SM_FIFO_DATA_HEAD_LEN) % smFifo->fifo.size;
            continue;
        }

        uint32_t tailLen = smFifo->fifo.size - smFifo->fifo.rptr - SM_FIFO_DATA_HEAD_LEN;
        uint8_t  stackBuf[dataLen];
        if (tailLen >= dataLen) {
            tlkipc_memcpy(stackBuf, pBuffer, dataLen);
        } else {
            tlkipc_memcpy(stackBuf, pBuffer, tailLen);
            tlkipc_memcpy(stackBuf + tailLen, smFifo->fifo.pBuffer, dataLen - tailLen);
        }
        smFifo->fifo.rptr = (smFifo->fifo.rptr + dataLen + SM_FIFO_DATA_HEAD_LEN) % smFifo->fifo.size;
        smFifo->cbArray[type](stackBuf, dataLen);
    }
}

void share_memory_fifo_init(tlk_sm_fifo_t *smFifo, uint8_t *pBuffer, uint32_t size)
{
    tlkipc_memset(smFifo, 0, sizeof(tlk_sm_fifo_t));
    smFifo->fifo.pBuffer = pBuffer;
    smFifo->fifo.size    = size;
    smFifo->status       = TLK_SHARE_MEMORY_STATUS_HAND_SHAKING;
}

_attribute_ram_code_sec_ uint8_t share_memory_fifo_isOverTargetPercent(tlk_sm_fifo_t *smFifo, uint32_t percent)
{
    if (percent > 100) {
        return 0;
    }
    uint32_t len = 0;
    if (smFifo->fifo.wptr >= smFifo->fifo.rptr) {
        len = smFifo->fifo.wptr - smFifo->fifo.rptr;
    } else {
        len = smFifo->fifo.size + smFifo->fifo.wptr - smFifo->fifo.rptr;
    }
    return len > smFifo->fifo.size * percent / 100;
}

_always_inline void share_memory_register_fifo_receive_cb(tlk_sm_fifo_t *smFifo, tlk_sm_rx_cb_array_t cbArray)
{
    smFifo->cbArray = cbArray;
}

#endif
