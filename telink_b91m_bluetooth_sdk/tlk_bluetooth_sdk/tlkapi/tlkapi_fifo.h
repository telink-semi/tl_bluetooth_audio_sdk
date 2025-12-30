/********************************************************************************************************
 * @file    tlkapi_fifo.h
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


#define TLKAPI_FIFO_MIN_SIZE                  64

#define TLKAPI_FIFO_SET_CONFIG(pFifo, config) ((pFifo)->config = (config))

typedef enum
{
    TLKAPI_FIFO_CFG_COVER_OPEN       = (1 << 0), //Whether overwrite is allowed.
    TLKAPI_FIFO_CFG_PARTY_OPEN       = (1 << 1), //Whether partial writing is allowed.
    TLKAPI_FIFO_CFG_WRITE_MUTEX_OPEN = (1 << 2), //Whether mutex when write fifo.

    TLKAPI_FIFO_CFG_NONE     = 0,
    TLKAPI_FIFO_CFG_ALL_OPEN = TLKAPI_FIFO_CFG_COVER_OPEN | TLKAPI_FIFO_CFG_PARTY_OPEN | TLKAPI_FIFO_CFG_WRITE_MUTEX_OPEN,
} TLKAPI_FIFO_CFG_ENUM;

typedef struct
{
    uint16_t config;
    uint16_t woffset;
    uint16_t roffset;
    uint16_t buffLen;
    uint8_t *pBuffer;
} tlkapi_fifo_t;

/**
 * @brief       Initialize FIFO buffer structure
 * @param[in]   pFifo    - pointer to FIFO structure.
 * @param[in]   config   - FIFO configuration flags.
 * @param[in]   pBuffer  - pointer to data buffer.
 * @param[in]   buffLen  - length of data buffer.
 * @returns     TLK_ENONE if success, otherwise error code.
 */
int tlkapi_fifo_init(tlkapi_fifo_t *pFifo, uint16_t config, uint8_t *pBuffer, uint16_t buffLen);

/**
 * @brief       Deinitialize FIFO buffer structure
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     none.
 */
void tlkapi_fifo_deinit(tlkapi_fifo_t *pFifo);

/**
 * @brief       Reset FIFO buffer structure to initial state
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     none.
 */
void tlkapi_fifo_reset(tlkapi_fifo_t *pFifo);

/**
 * @brief       Clear FIFO data by resetting read/write pointers
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     none.
 */
void tlkapi_fifo_clear(tlkapi_fifo_t *pFifo);

/**
 * @brief       Check if FIFO is empty
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     true if FIFO is empty, otherwise false.
 */
bool tlkapi_fifo_isEmpty(tlkapi_fifo_t *pFifo);

/**
 * @brief       Get idle (unused) space length in FIFO
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     idle space length in bytes.
 */
uint16_t tlkapi_fifo_idleLen(tlkapi_fifo_t *pFifo);

/**
 * @brief       Get data length in FIFO
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     data length in bytes.
 */
uint16_t tlkapi_fifo_dataLen(tlkapi_fifo_t *pFifo);

/**
 * @brief       Get total buffer length of FIFO
 * @param[in]   pFifo  - pointer to FIFO structure.
 * @returns     total buffer length in bytes.
 */
uint16_t tlkapi_fifo_buffLen(tlkapi_fifo_t *pFifo);

/**
 * @brief       Read data from FIFO
 * @param[in]   pFifo     - pointer to FIFO structure.
 * @param[out]  pBuff     - pointer to buffer for storing read data.
 * @param[in]   readLen   - length of data to read.
 * @returns     actual read data length.
 */
int tlkapi_fifo_read(tlkapi_fifo_t *pFifo, uint8_t *pBuff, uint16_t readLen);

/**
 * @brief       Write data to FIFO
 * @param[in]   pFifo     - pointer to FIFO structure.
 * @param[in]   pData     - pointer to data to write.
 * @param[in]   dataLen   - length of data to write.
 * @returns     actual written data length.
 */
int tlkapi_fifo_write(tlkapi_fifo_t *pFifo, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Read data from FIFO with option to change read offset
 * @param[in]   pFifo      - pointer to FIFO structure.
 * @param[out]  pBuff      - pointer to buffer for storing read data.
 * @param[in]   readLen    - length of data to read.
 * @param[in]   chgOffset  - whether to update read offset after reading.
 * @returns     actual read data length.
 */
int tlkapi_fifo_readCommon(tlkapi_fifo_t *pFifo, uint8_t *pBuff, uint16_t readLen, bool chgOffset);

/**
 * @brief       Change FIFO read position
 * @param[in]   pFifo     - pointer to FIFO structure.
 * @param[in]   readLen   - length to move read pointer.
 * @returns     TLK_ENONE if success, otherwise error code.
 */
int tlkapi_fifo_chgReadPos(tlkapi_fifo_t *pFifo, uint16_t readLen);

/**
 * @brief       Change FIFO write position
 * @param[in]   pFifo      - pointer to FIFO structure.
 * @param[in]   writeLen   - length to move write pointer.
 * @returns     TLK_ENONE if success, otherwise error code.
 */
int tlkapi_fifo_chgWritePos(tlkapi_fifo_t *pFifo, uint16_t writeLen);
