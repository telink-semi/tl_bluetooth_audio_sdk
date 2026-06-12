/********************************************************************************************************
 * @file    tlkdrv_serial.h
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

#if (MCU_CORE_TL752X_TEMP)
#define TLKDRV_SERIAL_MAX_NUMB    2 // tl752x support 5 hw uart.
#define TLKDRV_SERIAL_MAX_TIMEOUT 0xFFFFFFFFU
#define TLKDRV_SERIAL_DMA_RX_SIZE 256
#else
#define TLKDRV_SERIAL_MAX_NUMB 2
#endif


typedef void (*TlkDrvSerialRecvCB)(uint8_t *pFrame, uint16_t frmLen);

/**
 * @brief       This function mounts the serial port with specified parameters.
 * @param[in]   port     - Serial port number.
 * @param[in]   baudRate - Baud rate for communication.
 * @param[in]   txPin    - Transmit pin.
 * @param[in]   rxPin    - Receive pin.
 * @param[in]   txDma    - DMA channel for transmission.
 * @param[in]   rxDma    - DMA channel for reception.
 * @return      0: success, <0: failure.
 */
int tlkdrv_serial_mount(uint8_t port, uint32_t baudRate, uint16_t txPin, uint16_t rxPin, uint8_t txDma, uint8_t rxDma);

/**
 * @brief       This function unmounts the serial port.
 * @param[in]   port - Serial port number.
 * @return      0: success, <0: failure.
 */
int tlkdrv_serial_unmount(uint8_t port);

/**
 * @brief       This function opens the serial port.
 * @param[in]   port - Serial port number.
 * @return      0: success, <0: failure.
 */
int tlkdrv_serial_open(uint8_t port);

/**
 * @brief       This function closes the serial port.
 * @param[in]   port - Serial port number.
 * @return      0: success, <0: failure.
 */
int tlkdrv_serial_close(uint8_t port);

/**
 * @brief       This function reads data from the serial port.
 * @param[in]   port    - Serial port number.
 * @param[out]  pBuff   - Buffer to store received data.
 * @param[in]   buffLen - Length of buffer.
 * @return      Number of bytes read, <0: failure.
 */
int tlkdrv_serial_read(uint8_t port, uint8_t *pBuff, uint16_t buffLen);

/**
 * @brief       This function sends data through the serial port.
 * @param[in]   port     - Serial port number.
 * @param[in]   pData    - Data to send.
 * @param[in]   dataLen  - Length of data to send.
 * @return      0: success, <0: failure.
 */
int tlkdrv_serial_send(uint8_t port, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       This function sets transmit FIFO for the serial port.
 * @param[in]   port    - Serial port number.
 * @param[in]   pBuffer - Buffer for FIFO.
 * @param[in]   buffLen - Length of buffer.
 * @return      0: success, <0: failure.
 */
int tlkdrv_serial_setTxFifo(uint8_t port, uint8_t *pBuffer, uint16_t buffLen);

/**
 * @brief       This function sets receive FIFO for the serial port.
 * @param[in]   port    - Serial port number.
 * @param[in]   pBuffer - Buffer for FIFO.
 * @param[in]   buffLen - Length of buffer.
 * @return      0: success, <0: failure.
 */
int tlkdrv_serial_setRxFifo(uint8_t port, uint8_t *pBuffer, uint16_t buffLen);

/**
 * @brief       This function sets transmit QFIFO for the serial port.
 * @param[in]   port    - Serial port number.
 * @param[in]   fnumb   - Number of FIFO items.
 * @param[in]   fsize   - Size of each FIFO item.
 * @param[in]   pBuffer - Buffer for QFIFO.
 * @param[in]   buffLen - Length of buffer.
 * @return      0: success, <0: failure.
 */
int tlkdrv_serial_setTxQFifo(uint8_t port, uint16_t fnumb, uint16_t fsize, uint8_t *pBuffer, uint32_t buffLen);

/**
 * @brief       This function sets receive QFIFO for the serial port.
 * @param[in]   port    - Serial port number.
 * @param[in]   fnumb   - Number of FIFO items.
 * @param[in]   fsize   - Size of each FIFO item.
 * @param[in]   pBuffer - Buffer for QFIFO.
 * @param[in]   buffLen - Length of buffer.
 * @return      0: success, <0: failure.
 */
int tlkdrv_serial_setRxQFifo(uint8_t port, uint16_t fnumb, uint16_t fsize, uint8_t *pBuffer, uint32_t buffLen);

/**
 * @brief       This function resets the serial port.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
void tlkdrv_serial_reset(uint8_t port);

/**
 * @brief       This function registers callback function for serial port receiving.
 * @param[in]   port - Serial port number.
 * @param[in]   cb   - Callback function.
 * @return      none.
 */
void tlkdrv_serial_regCB(uint8_t port, TlkDrvSerialRecvCB cb);

/**
 * @brief       This function handles serial port events.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
void tlkdrv_serial_handler(uint8_t port);

/**
 * @brief       This function checks if serial port send FIFO is more than 60% full.
 * @param[in]   port     - Serial port number.
 * @param[in]   dataLen  - Length of data.
 * @return      true: FIFO is more than 60% full, false: otherwise.
 */
bool tlkdrv_serial_sfifoIsMore60(uint8_t port, uint16_t dataLen);

/**
 * @brief       This function checks if serial port send FIFO is more than 85% full.
 * @param[in]   port     - Serial port number.
 * @param[in]   dataLen  - Length of data.
 * @return      true: FIFO is more than 85% full, false: otherwise.
 */
bool tlkdrv_serial_sfifoIsMore85(uint8_t port, uint16_t dataLen);

/**
 * @brief       This function handles UART interrupts.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
void tlkdrv_uart_irqHandler(uint8_t port);

/**
 * @brief       This function handles DMA interrupts for serial port.
 * @return      none.
 */
void tlkdrv_serial_dmaIrqHandler(void);

/**
 * @brief       This function handles UART DMA interrupts.
 * @param[in]   port  - Serial port number.
 * @param[in]   rxDma - DMA channel for reception.
 * @return      none.
 */
void tlkdrv_uart_dmaIrqHandler(uint8_t port, uint8_t rxDma);

/**
 * @brief       This function handles transmit DMA done interrupt.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
void tlkdrv_serial_irqTxDoneWithDma(uint8_t port);

/**
 * @brief       This function handles receive without DMA done interrupt.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
void tlkdrv_serial_irqRxDoneWithoutDma(uint8_t port);