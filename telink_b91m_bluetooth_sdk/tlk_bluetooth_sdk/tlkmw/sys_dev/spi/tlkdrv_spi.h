/********************************************************************************************************
 * @file    tlkdrv_spi.h
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
#include "core/mcu_type.h"
// #define TLKDRV_SPI_CMD_FORMAT 0XCCBB 
// #define TLKDRV_SPI_RSP_FORMAT 0XCBCB 

/**
 * @brief SPI command format identifier
 */
#define TLKDRV_SPI_CMD_FORMAT 0XABA5 

/**
 * @brief SPI response format identifier
 */
#define TLKDRV_SPI_RSP_FORMAT 0XBA5A 

/**
 * @brief SPI flags enumeration
 */
typedef enum
{
    TLKDRV_SPI_FLAG_NONE     = 0x0000,  /**< No flag set */
    TLKDRV_SPI_FLAG_INIT     = 0x8000,  /**< SPI initialized */
    TLKDRV_SPI_FLAG_OPEN     = 0x4000,  /**< SPI open */
    TLKDRV_SPI_FLAG_SEND     = 0x2000,  /**< SPI sending data */
    TLKDRV_SPI_FLAG_RECV     = 0x1000,  /**< SPI receiving data */
    TLKDRV_SPI_FLAG_SLEEP    = 0x0800,  /**< SPI in sleep mode */
    TLKDRV_SPI_FLAG_TX_DMA   = 0x0001,  /**< SPI TX DMA enabled */
    TLKDRV_SPI_FLAG_RX_DMA   = 0x0002,  /**< SPI RX DMA enabled */
    TLKDRV_SPI_FLAG_RTX_MODE = 0x0004,  /**< SPI RTX mode enabled */
    TLKDRV_SPI_FLAG_TX_FIFO  = 0x0010,  /**< SPI TX FIFO enabled */
    TLKDRV_SPI_FLAG_RX_FIFO  = 0x0020,  /**< SPI RX FIFO enabled */
    TLKDRV_SPI_FLAG_TX_BUSY  = 0x0040,  /**< SPI TX busy */
    TLKDRV_SPI_FLAG_RX_BUSY  = 0x0080,  /**< SPI RX busy */

} TLKDRV_SERIAL_FLAGS_ENUM;

/**
 * @brief SPI receive callback function pointer
 * @param[in] pFrame - pointer to received data frame
 * @param[in] frmLen - length of received data frame
 */
typedef void (*TlkDrvSpiRecvCB)(uint8_t *pFrame, uint16_t frmLen);

/**
 * @brief Initialize SPI driver
 * @return none
 */
void tlkdrv_spi_init(void);

/**
 * @brief Handle SPI DMA interrupt
 * @return none
 */
void tlkdrv_spi_dmaIrqHandler(void);

/**
 * @brief Handle SPI operations in main loop
 * @return none
 */
void tlkdrv_spi_handler(void);

/**
 * @brief Write data via SPI
 * @param[in] data - pointer to data to be written
 * @param[in] dataLen - length of data to be written
 * @return 0: success, <0: failure
 */
int tlkdrv_spi_write_data(uint8_t * data, uint16_t dataLen);

/**
 * @brief Read data from SPI master
 * @param[in] pData - pointer to buffer for read data
 * @param[in] dataLen - length of data to read
 * @return none
 */
void tlkdrv_spi_master_read_data(uint8_t *pData ,uint8_t dataLen);

/**
 * @brief Write command via SPI
 * @param[in] data - pointer to command data to be written
 * @param[in] dataLen - length of command data to be written
 * @return 0: success, <0: failure
 */
int tlkdrv_spi_write_cmd(uint8_t * data, uint8_t dataLen);

/**
 * @brief Reinitialize SPI IO functions
 * @return none
 */
void tlkdrv_spi_io_function_reinit(void);
