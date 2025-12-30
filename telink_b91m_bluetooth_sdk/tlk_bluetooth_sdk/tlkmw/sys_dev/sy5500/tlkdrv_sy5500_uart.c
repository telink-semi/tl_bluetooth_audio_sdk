/********************************************************************************************************
 * @file    tlkdrv_sy5500_uart.c
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
#include "tlkmw/tlkmw.h"
#include "tlkapi/tlkapi.h"
#include "inner/tlkdrv_sy5500_cfg.h"
#if TLK_DEV_SY5500_ENABLE

/**
 * @brief SY5500 serial receive buffer
 * Aligned to 4-byte boundary
 */
__attribute__((aligned(4))) static uint8_t sTlkDrvSy5500SerialRxBuffer[TLKDEV_SY5500_SERIAL_RXBUFF_NUM * (TLKDEV_SY5500_SERIAL_RXBUFF_SIZE + 4)];

/**
 * @brief SY5500 serial transmit buffer
 * Aligned to 4-byte boundary
 */
__attribute__((aligned(4))) static uint8_t sTlkDrvSy5500SerialTxBuffer[TLKDEV_SY5500_SERIAL_TXBUFF_NUM * (TLKDEV_SY5500_SERIAL_TXBUFF_SIZE + 4)];

/**
 * @brief Initialize SY5500 UART interface
 * @param[in] recCB Receive callback function
 */
void tlkdrv_sy5500_uart_init(TlkDrvSerialRecvCB recCB)
{
    // Mount serial device
    tlkdrv_serial_mount(TLKDEV_SY5500_SERIAL_PORT, TLKDEV_SY5500_SERIAL_BAUDRATE, TLKDEV_SY5500_SERIAL_RTX_PIN, TLKDEV_SY5500_SERIAL_RTX_PIN, TLKDEV_SY5500_SERIAL_TX_DMA,
                        TLKDEV_SY5500_SERIAL_RX_DMA);

    // Set transmit queue FIFO
    tlkdrv_serial_setTxQFifo(TLKDEV_SY5500_SERIAL_PORT, TLKDEV_SY5500_SERIAL_TXBUFF_NUM, TLKDEV_SY5500_SERIAL_TXBUFF_SIZE + 4, sTlkDrvSy5500SerialTxBuffer,
                             TLKDEV_SY5500_SERIAL_TXBUFF_NUM * (TLKDEV_SY5500_SERIAL_TXBUFF_SIZE + 4));

    // Set receive queue FIFO
    tlkdrv_serial_setRxQFifo(TLKDEV_SY5500_SERIAL_PORT, TLKDEV_SY5500_SERIAL_RXBUFF_NUM, TLKDEV_SY5500_SERIAL_RXBUFF_SIZE + 4, sTlkDrvSy5500SerialRxBuffer,
                             TLKDEV_SY5500_SERIAL_RXBUFF_NUM * (TLKDEV_SY5500_SERIAL_RXBUFF_SIZE + 4));

    // Register receive callback function and open serial port
    tlkdrv_serial_regCB(TLKDEV_SY5500_SERIAL_PORT, recCB);
    tlkdrv_serial_open(TLKDEV_SY5500_SERIAL_PORT);
}

/**
 * @brief Send data to SY5500 via UART
 * @param[in] pData Pointer to data to be sent
 * @param[in] dataLen Length of data to be sent
 */
void tlkdrv_sy5500_sendUartData(uint8_t *pData, uint16_t dataLen)
{
    tlkdrv_serial_send(TLKDEV_SY5500_SERIAL_PORT, pData, dataLen);
}

#endif