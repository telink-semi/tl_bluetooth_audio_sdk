/********************************************************************************************************
 * @file    tlkapi_qfifo.c
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
#include "tlkapi/tlkapi_common.h"
#include "tlkapi/tlkapi_qfifo.h"

/**
 * @brief       Initialize the QFIFO structure
 * @param[in]   pFifo     - pointer to the QFIFO structure
 * @param[in]   numb      - number of elements in the FIFO
 * @param[in]   size      - size of each element in bytes
 * @param[in]   pBuffer   - pointer to the data buffer
 * @param[in]   buffLen   - length of the data buffer in bytes
 * @returns     TLK_ENONE if successful, -TLK_EPARAM if invalid parameters
 */
int tlkapi_qfifo_init(tlkapi_qfifo_t *pFifo, uint16_t numb, uint16_t size, uint8_t *pBuffer, uint32_t buffLen)
{
    if (pFifo == NULL || pBuffer == NULL || numb == 0 || size == 0 || numb * size > buffLen) {
        return -TLK_EPARAM;
    }

    pFifo->full  = false;
    pFifo->wptr  = 0;
    pFifo->rptr  = 0;
    pFifo->size  = size;
    pFifo->numb  = numb;
    pFifo->pBuff = pBuffer;
    return TLK_ENONE;
}

/**
 * @brief       Reset the QFIFO structure to initial state
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     none
 */
void tlkapi_qfifo_reset(tlkapi_qfifo_t *pFifo)
{
    pFifo->wptr  = 0;
    pFifo->rptr  = 0;
    pFifo->full  = false;
    pFifo->size  = 0;
    pFifo->numb  = 0;
    pFifo->pBuff = NULL;
}

/**
 * @brief       Clear the QFIFO data without resetting the structure
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     none
 */
void tlkapi_qfifo_clear(tlkapi_qfifo_t *pFifo)
{
    pFifo->wptr = 0;
    pFifo->rptr = 0;
    pFifo->full = false;
}

/**
 * @brief       Drop one buffer slot in the QFIFO (advance write pointer)
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     none
 */
void tlkapi_qfifo_dropBuff(tlkapi_qfifo_t *pFifo)
{
    uint16_t wptr = pFifo->wptr + 1;
    if (pFifo->numb == 0 || pFifo->full) {
        return;
    }
    if (wptr >= pFifo->numb) {
        wptr = 0;
    }
    if (wptr == pFifo->rptr) {
        pFifo->full = true;
    }
    pFifo->wptr = wptr;
}

/**
 * @brief       Drop one data slot in the QFIFO (advance read pointer)
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     none
 */
void tlkapi_qfifo_dropData(tlkapi_qfifo_t *pFifo)
{
    uint16_t rptr = pFifo->rptr + 1;
    if (tlkapi_qfifo_isEmpty(pFifo)) {
        return;
    }
    if (rptr >= pFifo->numb) {
        rptr = 0;
    }
    if (pFifo->full) {
        pFifo->full = false;
    }
    pFifo->rptr = rptr;
}

/**
 * @brief       Advance the write pointer by one position without checking FIFO status
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     none
 */
void tlkapi_qfifo_dropBuff1(tlkapi_qfifo_t *pFifo)
{
    uint16_t wptr = pFifo->wptr + 1;
    if (wptr == pFifo->numb) {
        wptr = 0;
    }
    pFifo->wptr = wptr;
}

/**
 * @brief       Advance the read pointer by one position without checking FIFO status
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     none
 */
void tlkapi_qfifo_dropData1(tlkapi_qfifo_t *pFifo)
{
    uint16_t rptr = pFifo->rptr + 1;
    if (rptr == pFifo->numb) {
        rptr = 0;
    }
    pFifo->rptr = rptr;
}

/**
 * @brief       Get the number of used slots in the QFIFO
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     number of used slots
 */
uint16_t tlkapi_qfifo_usedNum(tlkapi_qfifo_t *pFifo)
{
    if (pFifo->full) {
        return pFifo->numb;
    }
    if (pFifo->wptr >= pFifo->rptr) {
        return (pFifo->wptr - pFifo->rptr);
    } else {
        return (pFifo->numb + pFifo->wptr - pFifo->rptr);
    }
}

/**
 * @brief       Get the number of idle slots in the QFIFO
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     number of idle slots
 */
uint16_t tlkapi_qfifo_idleNum(tlkapi_qfifo_t *pFifo)
{
    return (pFifo->numb - tlkapi_qfifo_usedNum(pFifo));
}

/**
 * @brief       Get the pointer to the current write buffer without advancing the write pointer
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     pointer to the current write buffer, or NULL if FIFO is full
 */
uint8_t *tlkapi_qfifo_getBuff(tlkapi_qfifo_t *pFifo)
{
    uint8_t *pBuffer;
    if (pFifo->full) {
        pBuffer = NULL;
    } else {
        pBuffer = (pFifo->pBuff + pFifo->wptr * pFifo->size);
    }
    return pBuffer;
}

/**
 * @brief       Get the pointer to the current read data without advancing the read pointer
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     pointer to the current read data, or NULL if FIFO is empty
 */
uint8_t *tlkapi_qfifo_getData(tlkapi_qfifo_t *pFifo)
{
    if (!pFifo->full && pFifo->rptr == pFifo->wptr) {
        return NULL;
    } else {
        return (pFifo->pBuff + pFifo->rptr * pFifo->size);
    }
}

/**
 * @brief       Get the pointer to the current write buffer and advance the write pointer
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     pointer to the current write buffer, or NULL if FIFO is full
 */
uint8_t *tlkapi_qfifo_takeBuff(tlkapi_qfifo_t *pFifo)
{
    uint16_t wptr;
    uint8_t *pBuffer;
    if (pFifo->full) {
        return NULL;
    }
    pBuffer = (pFifo->pBuff + pFifo->wptr * pFifo->size);
    wptr    = pFifo->wptr + 1;
    if (wptr >= pFifo->numb) {
        wptr = 0;
    }
    pFifo->wptr = wptr;
    return pBuffer;
}

/**
 * @brief       Get the pointer to the current read data and advance the read pointer
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     pointer to the current read data, or NULL if FIFO is empty
 */
uint8_t *tlkapi_qfifo_takeData(tlkapi_qfifo_t *pFifo)
{
    uint8_t *pBuff = tlkapi_qfifo_getData(pFifo);
    if (pBuff != NULL) {
        tlkapi_qfifo_dropData1(pFifo);
    }
    return pBuff;
}
