/********************************************************************************************************
 * @file    tlkapi_qfifo.h
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

#define TLKAPI_QFIFO_NEED_SIZE(numb, size) ((size) * (numb))

#define tlkapi_qfifo_size(pFifo)           ((pFifo)->size)
#define tlkapi_qfifo_numb(pFifo)           ((pFifo)->numb)
#define tlkapi_qfifo_isFull(pFifo)         ((pFifo)->full || (pFifo)->numb == 0)
#define tlkapi_qfifo_isEmpty(pFifo)        (!(pFifo)->full && (pFifo)->wptr == (pFifo)->rptr)

#define tlkapi_qfifo_getFull(pFifo)        ((pFifo)->full)
#define tlkapi_qfifo_getTemp(pFifo)        ((pFifo)->temp)
#define tlkapi_qfifo_getResv(pFifo)        ((pFifo)->resv)
#define tlkapi_qfifo_getRptr(pFifo)        ((pFifo)->rptr)
#define tlkapi_qfifo_getWptr(pFifo)        ((pFifo)->wptr)
#define tlkapi_qfifo_setFull(pFifo, value) ((pFifo)->full = (value))
#define tlkapi_qfifo_setTemp(pFifo, value) ((pFifo)->temp = (value))
#define tlkapi_qfifo_setResv(pFifo, value) ((pFifo)->resv = (value))
#define tlkapi_qfifo_setRptr(pFifo, value) ((pFifo)->rptr = (value))
#define tlkapi_qfifo_setWptr(pFifo, value) ((pFifo)->wptr = (value))


#define tlkapi_qfifo_getBase(pFifo) ((pFifo)->pBuff)

typedef struct
{
    uint8_t   full;
    uint8_t   temp;
    uint16_t  resv;
    uint16_t  numb;
    uint16_t  size;
    uint16_t  wptr;
    uint16_t  rptr;
    uint8_t  *pBuff;
} tlkapi_qfifo_t;

/**
 * @brief       Initialize the QFIFO structure
 * @param[in]   pFifo     - pointer to the QFIFO structure
 * @param[in]   numb      - number of elements in the FIFO
 * @param[in]   size      - size of each element in bytes
 * @param[in]   pBuffer   - pointer to the data buffer
 * @param[in]   buffLen   - length of the data buffer in bytes
 * @returns     TLK_ENONE if successful, -TLK_EPARAM if invalid parameters
 */
int tlkapi_qfifo_init(tlkapi_qfifo_t *pFifo, uint16_t numb, uint16_t size, uint8_t *pBuffer, uint32_t buffLen);

/**
 * @brief       Reset the QFIFO structure to initial state
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     none
 */
void tlkapi_qfifo_reset(tlkapi_qfifo_t *pFifo);

/**
 * @brief       Clear the QFIFO data without resetting the structure
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     none
 */
void tlkapi_qfifo_clear(tlkapi_qfifo_t *pFifo);

/**
 * @brief       Get the number of used slots in the QFIFO
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     number of used slots
 */
uint16_t tlkapi_qfifo_usedNum(tlkapi_qfifo_t *pFifo);

/**
 * @brief       Get the number of idle slots in the QFIFO
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     number of idle slots
 */
uint16_t tlkapi_qfifo_idleNum(tlkapi_qfifo_t *pFifo);

/**
 * @brief       Get the pointer to the current write buffer without advancing the write pointer
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     pointer to the current write buffer, or NULL if FIFO is full
 */
uint8_t *tlkapi_qfifo_getBuff(tlkapi_qfifo_t *pFifo);

/**
 * @brief       Drop one buffer slot in the QFIFO (advance write pointer)
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     none
 */
void tlkapi_qfifo_dropBuff(tlkapi_qfifo_t *pFifo);

/**
 * @brief       Get the pointer to the current read data without advancing the read pointer
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     pointer to the current read data, or NULL if FIFO is empty
 */
uint8_t *tlkapi_qfifo_getData(tlkapi_qfifo_t *pFifo);

/**
 * @brief       Drop one data slot in the QFIFO (advance read pointer)
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     none
 */
void tlkapi_qfifo_dropData(tlkapi_qfifo_t *pFifo);

/**
 * @brief       Get the pointer to the current write buffer and advance the write pointer
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     pointer to the current write buffer, or NULL if FIFO is full
 */
uint8_t *tlkapi_qfifo_takeBuff(tlkapi_qfifo_t *pFifo);

/**
 * @brief       Get the pointer to the current read data and advance the read pointer
 * @param[in]   pFifo   - pointer to the QFIFO structure
 * @returns     pointer to the current read data, or NULL if FIFO is empty
 */
uint8_t *tlkapi_qfifo_takeData(tlkapi_qfifo_t *pFifo);
