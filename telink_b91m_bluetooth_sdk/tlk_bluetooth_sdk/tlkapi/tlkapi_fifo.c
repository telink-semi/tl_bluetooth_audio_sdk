/********************************************************************************************************
 * @file    tlkapi_fifo.c
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
#include "common/types.h"
#include "common/tstring.h"
#include "common/compiler.h"
#include "drivers.h"
#include "tlkapi/tlkapi_common.h"
#include "tlkapi/tlkapi_fifo.h"

/**
 * @brief       Enter critical section to protect FIFO operations
 * @param[in]   none.
 * @returns     interrupt status flag.
 */
static __attribute__((__always_inline__)) inline unsigned int tlkapi_fifo_enterCritical(void) //mutex
{
    return core_interrupt_disable();
}

/**
 * @brief       Leave critical section to restore interrupt state
 * @param[in]   en  - interrupt enable flag.
 * @returns     restored interrupt status.
 */
static __attribute__((__always_inline__)) inline unsigned int tlkapi_fifo_leaveCritical(unsigned int en) //unMutex
{
    return core_restore_interrupt(en);
}

/**
 * @brief       Initialize FIFO buffer structure
 * @param[in]   pFifo    - pointer to FIFO structure.
 * @param[in]   config   - FIFO configuration flags.
 * @param[in]   pBuffer  - pointer to data buffer.
 * @param[in]   buffLen  - length of data buffer.
 * @returns     TLK_ENONE if success, otherwise error code.
 */
int tlkapi_fifo_init(tlkapi_fifo_t *pFifo, uint16_t config, uint8_t *pBuffer, uint16_t buffLen)
{
    if (pBuffer == NULL || buffLen < TLKAPI_FIFO_MIN_SIZE) {
        return -TLK_EPARAM;
    }
    pFifo->config  = config;
    pFifo->woffset = 0;
    pFifo->roffset = 0;
    pFifo->buffLen = buffLen;
    pFifo->pBuffer = pBuffer;

    return TLK_ENONE;
}

/**
 * @brief       Deinitialize FIFO buffer structure
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     none.
 */
void tlkapi_fifo_deinit(tlkapi_fifo_t *pFifo)
{
    tlkapi_fifo_reset(pFifo);
}

/**
 * @brief       Reset FIFO buffer structure to initial state
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     none.
 */
void tlkapi_fifo_reset(tlkapi_fifo_t *pFifo)
{
    if (pFifo == NULL) {
        return;
    }
    pFifo->config  = 0;
    pFifo->woffset = 0;
    pFifo->roffset = 0;
    pFifo->buffLen = 0;
    pFifo->pBuffer = 0;
}

/**
 * @brief       Clear FIFO data by resetting read/write pointers
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     none.
 */
void tlkapi_fifo_clear(tlkapi_fifo_t *pFifo)
{
    if (pFifo == NULL) {
        return;
    }
    pFifo->woffset = 0;
    pFifo->roffset = 0;
}

/**
 * @brief       Check if FIFO is empty
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     true if FIFO is empty, otherwise false.
 */
bool tlkapi_fifo_isEmpty(tlkapi_fifo_t *pFifo)
{
    if (pFifo == NULL || pFifo->woffset == pFifo->roffset) {
        return true;
    }
    return false;
}

/**
 * @brief       Get idle (unused) space length in FIFO
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     idle space length in bytes.
 */
uint16_t tlkapi_fifo_idleLen(tlkapi_fifo_t *pFifo)
{
    uint16_t woffset;
    uint16_t roffset;

    if (pFifo == NULL) {
        return 0;
    }

    woffset = pFifo->woffset;
    roffset = pFifo->roffset;
    if (roffset > woffset) {
        return (roffset - woffset);
    }
    return (pFifo->buffLen + roffset - woffset);
}

/**
 * @brief       Get data length in FIFO
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     data length in bytes.
 */
uint16_t tlkapi_fifo_dataLen(tlkapi_fifo_t *pFifo)
{
    uint16_t woffset;
    uint16_t roffset;

    if (pFifo == NULL) {
        return 0;
    }

    woffset = pFifo->woffset;
    roffset = pFifo->roffset;
    if (woffset >= roffset) {
        return (woffset - roffset);
    }
    return (pFifo->buffLen + woffset - roffset);
}

/**
 * @brief       Get total buffer length of FIFO
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     total buffer length in bytes.
 */
uint16_t tlkapi_fifo_buffLen(tlkapi_fifo_t *pFifo)
{
    if (pFifo == NULL) {
        return 0;
    }
    return pFifo->buffLen;
}

/**
 * @brief       Change FIFO read position
 * @param[in]   pFifo     - pointer to FIFO structure.
 * @param[in]   readLen   - length to move read pointer.
 * @returns     TLK_ENONE if success, otherwise error code.
 */
int tlkapi_fifo_chgReadPos(tlkapi_fifo_t *pFifo, uint16_t readLen)
{
    uint16_t roffset = pFifo->roffset;
    if (readLen > pFifo->buffLen) {
        return -TLK_EFAIL;
    }
    roffset += readLen;
    if (roffset >= pFifo->buffLen) {
        roffset -= pFifo->buffLen;
    }
    pFifo->roffset = roffset;
    return TLK_ENONE;
}

/**
 * @brief       Change FIFO write position
 * @param[in]   pFifo      - pointer to FIFO structure.
 * @param[in]   writeLen   - length to move write pointer.
 * @returns     TLK_ENONE if success, otherwise error code.
 */
int tlkapi_fifo_chgWritePos(tlkapi_fifo_t *pFifo, uint16_t writeLen)
{
    unsigned int irqFlag = 0;
    if (pFifo->config & TLKAPI_FIFO_CFG_WRITE_MUTEX_OPEN) {
        irqFlag = tlkapi_fifo_enterCritical();
    }
    uint16_t woffset = pFifo->woffset;
    if (writeLen > pFifo->buffLen) {
        return -TLK_EFAIL;
    }
    woffset += writeLen;
    if (woffset >= pFifo->buffLen) {
        woffset -= pFifo->buffLen;
    }
    pFifo->woffset = woffset;
    if (pFifo->config & TLKAPI_FIFO_CFG_WRITE_MUTEX_OPEN) {
        tlkapi_fifo_leaveCritical(irqFlag);
    }
    return TLK_ENONE;
}

/**
 * @brief       Write data to FIFO
 * @param[in]   pFifo     - pointer to FIFO structure.
 * @param[in]   pData     - pointer to data to write.
 * @param[in]   dataLen   - length of data to write.
 * @returns     actual written data length.
 */
int tlkapi_fifo_write(tlkapi_fifo_t *pFifo, uint8_t *pData, uint16_t dataLen)
{
    uint16_t       unUsed;
    uint16_t       tempLen;
    uint16_t       woffset;
    uint16_t       roffset;
    unsigned int irqFlag = 0;
    if (pFifo == NULL || pFifo->buffLen == 0 || pData == NULL || dataLen == 0 || dataLen >= pFifo->buffLen) {
        return -TLK_EPARAM;
    }

    if (pFifo->config & TLKAPI_FIFO_CFG_WRITE_MUTEX_OPEN) {
        irqFlag = tlkapi_fifo_enterCritical();
    }

    woffset = pFifo->woffset;
    roffset = pFifo->roffset;
    if (roffset > woffset) {
        unUsed = roffset - woffset;
    } else {
        unUsed = pFifo->buffLen + roffset - woffset;
    }

    if (unUsed <= dataLen) {
        if (pFifo->config & TLKAPI_FIFO_CFG_COVER_OPEN) {
            roffset += dataLen - unUsed + 1;
            if (roffset >= pFifo->buffLen) {
                roffset -= pFifo->buffLen;
            }
            unUsed = dataLen + 1;
        } else if (unUsed > 1 && (pFifo->config & TLKAPI_FIFO_CFG_PARTY_OPEN)) {
            dataLen = unUsed - 1;
        } else {
            if (pFifo->config & TLKAPI_FIFO_CFG_WRITE_MUTEX_OPEN) {
                tlkapi_fifo_leaveCritical(irqFlag);
            }
            return -TLK_EFULL;
        }
    }
    uint16_t posChange = pFifo->woffset;
    posChange += dataLen;
    if (posChange >= pFifo->buffLen) {
        posChange -= pFifo->buffLen;
    }
    pFifo->woffset = posChange;
    if (pFifo->config & TLKAPI_FIFO_CFG_WRITE_MUTEX_OPEN) {
        tlkapi_fifo_leaveCritical(irqFlag);
    }
    if (woffset + dataLen <= pFifo->buffLen) {
        tempLen = dataLen;
    } else {
        tempLen = pFifo->buffLen - woffset;
    }
    tmemcpy(pFifo->pBuffer + woffset, pData, tempLen);
    if (tempLen < dataLen) {
        tmemcpy(pFifo->pBuffer, pData + tempLen, dataLen - tempLen);
    }

    return dataLen;
}

/**
 * @brief       Read data from FIFO
 * @param[in]   pFifo     - pointer to FIFO structure.
 * @param[out]  pBuff     - pointer to buffer for storing read data.
 * @param[in]   readLen   - length of data to read.
 * @returns     actual read data length.
 */
int tlkapi_fifo_read(tlkapi_fifo_t *pFifo, uint8_t *pBuff, uint16_t readLen)
{
    return tlkapi_fifo_readCommon(pFifo, pBuff, readLen, true);
}

/**
 * @brief       Read data from FIFO with option to change read offset
 * @param[in]   pFifo      - pointer to FIFO structure.
 * @param[out]  pBuff      - pointer to buffer for storing read data.
 * @param[in]   readLen    - length of data to read.
 * @param[in]   chgOffset  - whether to update read offset after reading.
 * @returns     actual read data length.
 */
int tlkapi_fifo_readCommon(tlkapi_fifo_t *pFifo, uint8_t *pBuff, uint16_t readLen, bool chgOffset)
{
    uint16_t used;
    uint16_t tempLen;
    uint16_t woffset;
    uint16_t roffset;

    if (pFifo == NULL || pFifo->buffLen == 0 || pBuff == NULL || readLen == 0) {
        return -TLK_EPARAM;
    }

    woffset = pFifo->woffset;
    roffset = pFifo->roffset;
    if (woffset >= roffset) {
        used = woffset - roffset;
    } else {
        used = pFifo->buffLen + woffset - roffset;
    }
    if (used == 0) {
        return -TLK_EEMPTY;
    }
    if (readLen > used) {
        readLen = used;
    }

    if (roffset + readLen <= pFifo->buffLen) {
        tempLen = readLen;
    } else {
        tempLen = pFifo->buffLen - roffset;
    }
    tmemcpy(pBuff, pFifo->pBuffer + roffset, tempLen);
    if (tempLen < readLen) {
        tmemcpy(pBuff + tempLen, pFifo->pBuffer, readLen - tempLen);
    }

    if (chgOffset) {
        roffset += readLen;
        if (roffset >= pFifo->buffLen) {
            roffset -= pFifo->buffLen;
        }
        pFifo->roffset = roffset;
    }

    return readLen;
}
