/********************************************************************************************************
 * @file    tlkdrv_serial.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkdrv_serial.h"
#if (TLK_DEV_SERIAL_ENABLE)

typedef enum
{
    TLKDRV_SERIAL_FLAG_NONE     = 0x0000,
    TLKDRV_SERIAL_FLAG_INIT     = 0x8000,
    TLKDRV_SERIAL_FLAG_OPEN     = 0x4000,
    TLKDRV_SERIAL_FLAG_SEND     = 0x2000,
    TLKDRV_SERIAL_FLAG_RECV     = 0x1000,
    TLKDRV_SERIAL_FLAG_SLEEP    = 0x0800,
    TLKDRV_SERIAL_FLAG_TX_DMA   = 0x0001,
    TLKDRV_SERIAL_FLAG_RX_DMA   = 0x0002,
    TLKDRV_SERIAL_FLAG_RTX_MODE = 0x0004,
    TLKDRV_SERIAL_FLAG_TX_FIFO  = 0x0010,
    TLKDRV_SERIAL_FLAG_RX_FIFO  = 0x0020,
    TLKDRV_SERIAL_FLAG_TX_BUSY  = 0x0040,
    TLKDRV_SERIAL_FLAG_RX_BUSY  = 0x0080,
} TLKDRV_SERIAL_FLAGS_ENUM;

typedef struct
{
    uint8_t  port;
    uint8_t  resv;
    uint8_t  txDma;
    uint8_t  rxDma;
    uint16_t txPin;
    uint16_t rxPin;
    uint32_t baudRate;
} tlkdrv_serial_port_t;

typedef union
{
    tlkapi_fifo_t  fifo;
    tlkapi_qfifo_t qfifo;
} tlkdrv_serial_fifo_t;

typedef struct
{
    uint16_t             flags[TLKDRV_SERIAL_MAX_NUMB];
    uint16_t             sfifoM60[TLKDRV_SERIAL_MAX_NUMB];
    uint16_t             sfifoM85[TLKDRV_SERIAL_MAX_NUMB];
    tlkdrv_serial_port_t port[TLKDRV_SERIAL_MAX_NUMB];
    tlkdrv_serial_fifo_t sfifo[TLKDRV_SERIAL_MAX_NUMB];
    tlkdrv_serial_fifo_t rfifo[TLKDRV_SERIAL_MAX_NUMB];
    TlkDrvSerialRecvCB   recvCB[TLKDRV_SERIAL_MAX_NUMB];
    tlksys_devmgr_item_t devItem[TLKDRV_SERIAL_MAX_NUMB];
} tlkdrv_serial_t;

static tlkdrv_serial_t sTlkDrvSerial = {0};

#if MCU_CORE_TL752X_TEMP
#if (MCU_CORE_TYPE == CHIP_TYPE_TL752X)
typedef struct
{
    uart_handle_t uartHandle;
    dmac_handle_t rxDmaHandle;
    dmac_handle_t txDmaHandle;
} tlkdrv_serial_tl752x_t;

tlkdrv_serial_tl752x_t sTlkDrvSerialTl752xItem[TLKDRV_SERIAL_MAX_NUMB] = {0};
#endif

#endif


/**
 * @brief       This function updates the busy state of the serial port.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
static void tlkdrv_serial_updateBusyState(uint8_t port)
{
    if (sTlkDrvSerial.flags[port] & (TLKDRV_SERIAL_FLAG_RX_BUSY | TLKDRV_SERIAL_FLAG_TX_BUSY)) {
        tlksys_devmgr_setBusy(&sTlkDrvSerial.devItem[port], 1);
    } else {
        tlksys_devmgr_setBusy(&sTlkDrvSerial.devItem[port], 0);
    }
}

/**
 * @brief       This function sets the transmit DMA buffer for the serial port.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ static void tlkdrv_serial_setTxDmaBuffer(uint8_t port)
{
    uint8_t *pData;
    uint16_t dataLen = 0;
    pData            = tlkapi_qfifo_getData(&sTlkDrvSerial.sfifo[port].qfifo);
    if (pData != NULL) {
        dataLen = ((uint16_t)pData[1] << 8) | pData[0];
    }
    if (dataLen == 0) {
        sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_TX_BUSY;
        tlkdrv_serial_updateBusyState(port);
        if (pData != NULL) {
            tlkapi_qfifo_dropData(&sTlkDrvSerial.sfifo[port].qfifo);
        }
    } else {
        delay_us(10);
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_TX_BUSY;
        tlkdrv_serial_updateBusyState(port);
#if (!MCU_CORE_TL752X_TEMP)
        uart_send_dma(port, pData + 4, dataLen);
#else
        drv_uart_transmit_dma(&sTlkDrvSerialTl752xItem[port].uartHandle, (uint8_t *)((uint32_t)(pData + 4) + CPU_SYS_BUS), dataLen);
#endif
    }
}

/**
 * @brief       This function sets the receive DMA buffer for the serial port.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ static void tlkdrv_serial_setRxDmaBuffer(uint8_t port)
{
    uint32_t *pBuffer;

    pBuffer = (uint32_t *)tlkapi_qfifo_getBuff(&sTlkDrvSerial.rfifo[port].qfifo);
    if (pBuffer == NULL) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_RX_BUSY;
        tlkdrv_serial_updateBusyState(port);
    } else {
        pBuffer[0] = 0;
        sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_RX_BUSY;
        tlkdrv_serial_updateBusyState(port);
#if (!MCU_CORE_TL752X_TEMP)
        pBuffer[0] = 0;
        uart_receive_dma(port, (uint8_t *)(pBuffer + 1), tlkapi_qfifo_size(&sTlkDrvSerial.rfifo[port].qfifo) - 4);
#else
        drv_uart_receive_dma(&sTlkDrvSerialTl752xItem[port].uartHandle, (uint8_t *)((uint32_t)pBuffer + CPU_SYS_BUS), tlkapi_qfifo_size(&sTlkDrvSerial.rfifo[port].qfifo));
#endif
    }
}

/**
 * @brief       This function sets the receive buffer for the serial port.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ static void tlkdrv_serial_setRxBuffer(uint8_t port)
{
    (void)port;
#if (MCU_CORE_TL752X_TEMP)
    uint16_t idleLen;
    idleLen = tlkapi_fifo_idleLen(&sTlkDrvSerial.rfifo[port].fifo);
    if (idleLen == 0) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_RX_BUSY;
        tlkdrv_serial_updateBusyState(port);
    } else {
        sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_RX_BUSY;
        tlkdrv_serial_updateBusyState(port);
        drv_uart_receive_it(&sTlkDrvSerialTl752xItem[port].uartHandle, sTlkDrvSerial.rfifo[port].fifo.pBuffer, sTlkDrvSerial.rfifo[port].fifo.buffLen);
    }
#endif
}

/**
 * @brief       This function sends data without DMA for the serial port.
 * @param[in]   port     - Serial port number.
 * @param[in]   pData    - Data to send.
 * @param[in]   dataLen  - Length of data to send.
 * @return      0: success.
 */
static int tlkdrv_serial_sendWithoutDma(uint8_t port, uint8_t *pData, uint16_t dataLen)
{
#if (!MCU_CORE_TL752X_TEMP)
    uart_send(port, pData, dataLen);
#else
    drv_uart_transmit(&sTlkDrvSerialTl752xItem[port].uartHandle, pData, dataLen, TLKDRV_SERIAL_MAX_TIMEOUT);
#endif
    return TLK_ENONE;
}

/**
 * @brief       This function sends data with DMA for the serial port.
 * @param[in]   port     - Serial port number.
 * @param[in]   pData    - Data to send.
 * @param[in]   dataLen  - Length of data to send.
 * @return      0: success, <0: failure.
 */
_attribute_ram_code_sec_noinline_ static int tlkdrv_serial_sendWithDma(uint8_t port, uint8_t *pData, uint16_t dataLen)
{
    uint16_t count;
    uint16_t isize; // Item Size
    uint16_t tempVar;
    uint8_t *pBuffer;

    isize = tlkapi_qfifo_size(&sTlkDrvSerial.sfifo[port].qfifo) - 4;
    if (isize == 0) {
        return -TLK_EFAULT;
    }

    count = (dataLen + isize - 1) / isize;
    if (count > tlkapi_qfifo_idleNum(&sTlkDrvSerial.sfifo[port].qfifo)) {
        return -TLK_EQUOTA;
    }

    while ((count--) != 0 && dataLen != 0) {
        pBuffer = tlkapi_qfifo_getBuff(&sTlkDrvSerial.sfifo[port].qfifo);
        if (pBuffer == NULL) {
            break;
        }
        if (dataLen <= isize) {
            tempVar = dataLen;
        } else {
            tempVar = isize;
        }
        pBuffer[0] = (tempVar & 0x00FF);
        pBuffer[1] = (tempVar & 0xFF00) >> 8;
        pBuffer[2] = 0;
        pBuffer[3] = 0;
        tmemcpy(pBuffer + 4, pData, tempVar);
        pData += tempVar;
        dataLen -= tempVar;
        tlkapi_qfifo_dropBuff(&sTlkDrvSerial.sfifo[port].qfifo);
    }
    if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_TX_BUSY) == 0) {
        tlkdrv_serial_setTxDmaBuffer(port);
    }
    return TLK_ENONE;
}

/**
 * @brief       This function mounts the serial port.
 * @param[in]   port     - Serial port number.
 * @param[in]   baudRate - Baud rate.
 * @param[in]   txPin    - Transmit pin.
 * @param[in]   rxPin    - Receive pin.
 * @param[in]   txDma    - Transmit DMA.
 * @param[in]   rxDma    - Receive DMA.
 * @return      0: success.
 */
int tlkdrv_serial_mount(uint8_t port, uint32_t baudRate, uint16_t txPin, uint16_t rxPin, uint8_t txDma, uint8_t rxDma)
{
    tlkdrv_serial_port_t *pPort;

    if (port >= TLKDRV_SERIAL_MAX_NUMB || baudRate < 9600 || (txPin == 0 && rxPin == 0)) {
        return -TLK_EPARAM;
    }
    if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_OPEN) != 0) {
        return -TLK_ESTATUS;
    }

    if (txPin == 0 && txDma != 0) {
        txDma = 0;
    }
    if (rxPin == 0 && rxDma != 0) {
        rxDma = 0;
    }

    pPort           = &sTlkDrvSerial.port[port];
    pPort->port     = port;
    pPort->txPin    = txPin;
    pPort->rxPin    = rxPin;
    pPort->txDma    = txDma;
    pPort->rxDma    = rxDma;
    pPort->baudRate = baudRate;
    sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_INIT;
    if (pPort->txPin != 0) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_SEND;
    }
    if (pPort->rxPin != 0) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_RECV;
    }
    if (pPort->rxPin == pPort->txPin) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_RTX_MODE;
    }
    if (pPort->txDma != 0) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_TX_DMA;
    }
    if (pPort->rxDma != 0) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_RX_DMA;
    }
    if (pPort->txPin != 0 && pPort->txDma == 0) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_TX_FIFO;
    }
    tlksys_devmgr_mount(&sTlkDrvSerial.devItem[port], NULL);
#if (MCU_CORE_TL752X_TEMP)
    tlkhal_uart_dmaTxDoneRegister(tlkdrv_serial_irqTxDoneWithDma);
    if (rxDma != 0) {
        tlkhal_uart_dmaRxDoneRegister(tlkdrv_uart_dmaIrqHandler);
    } else {
        tlkhal_uart_noDmaRxDoneRegister(tlkdrv_serial_irqRxDoneWithoutDma);
    }
#endif
    return TLK_ENONE;
}

/**
 * @brief       This function unmounts the serial port.
 * @param[in]   port - Serial port number.
 * @return      0: success.
 */
int tlkdrv_serial_unmount(uint8_t port)
{
    if (port >= TLKDRV_SERIAL_MAX_NUMB) {
        return -TLK_EPARAM;
    }
    if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_OPEN) == 0) {
        return -TLK_ESTATUS;
    }
    tlkdrv_serial_close(port);
    sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_INIT;
    tlksys_devmgr_unmount(&sTlkDrvSerial.devItem[port]);
    return TLK_ENONE;
}

/**
 * @brief       This function opens the serial port.
 * @param[in]   port - Serial port number.
 * @return      0: success.
 */
int tlkdrv_serial_open(uint8_t port)
{
    uint16_t              flags;
    tlkdrv_serial_port_t *pPort;

    if (port >= TLKDRV_SERIAL_MAX_NUMB) {
        return -TLK_EPARAM;
    }

    pPort = &sTlkDrvSerial.port[port];
    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_INIT) == 0) {
        return -TLK_ENOREADY;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) != 0) {
        return -TLK_EREPEAT;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_SEND) != 0 && pPort->txDma != 0 && (flags & TLKDRV_SERIAL_FLAG_TX_FIFO) == 0) {
        return -TLK_ENOREADY;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_RECV) != 0 && (flags & TLKDRV_SERIAL_FLAG_RX_FIFO) == 0) {
        return -TLK_ENOREADY;
    }

    sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_TX_BUSY;
    sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_RX_BUSY;
    tlkdrv_serial_updateBusyState(port);
    tlkdrv_serial_reset(port);

    tlkhal_uart_cfg_t cfg = {
        .port     = port,
        .rxDma    = pPort->rxDma,
        .txDma    = pPort->txDma,
        .txPin    = pPort->txPin,
        .rxPin    = pPort->rxPin,
        .baudRate = pPort->baudRate,
    };
#if (MCU_CORE_TL752X_TEMP)
    cfg.pUartHandle  = &sTlkDrvSerialTl752xItem[port].uartHandle;
    cfg.pRxDmaHandle = pPort->rxDma ? &sTlkDrvSerialTl752xItem[port].rxDmaHandle : NULL;
    cfg.pTxDmaHandle = pPort->txDma ? &sTlkDrvSerialTl752xItem[port].txDmaHandle : NULL;
    if (port == 0) {
        cfg.pUartHandle->instance = UART0;
    } else if (port == 1) {
        cfg.pUartHandle->instance = UART1;
    }
#endif
    tlkhal_uart_open(&cfg);

    sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_OPEN;
    if (pPort->rxDma != 0) {
        tlkdrv_serial_setRxDmaBuffer(port);
    } else {
        tlkdrv_serial_setRxBuffer(port);
    }
    return TLK_ENONE;
}

/**
 * @brief       This function closes the serial port.
 * @param[in]   port - Serial port number.
 * @return      0: success.
 */
int tlkdrv_serial_close(uint8_t port)
{
    uint16_t              flags;
    tlkdrv_serial_port_t *pPort;

    if (port >= TLKDRV_SERIAL_MAX_NUMB) {
        return -TLK_EPARAM;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_INIT) == 0) {
        return -TLK_ENOREADY;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) == 0) {
        return TLK_ENONE;
    }

    pPort = &sTlkDrvSerial.port[port];

    tlkhal_uart_cfg_t cfg = {
        .port     = port,
        .rxDma    = pPort->rxDma,
        .txDma    = pPort->txDma,
        .txPin    = pPort->txPin,
        .rxPin    = pPort->rxPin,
        .baudRate = pPort->baudRate,
    };
#if (MCU_CORE_TL752X_TEMP)
    cfg.pUartHandle  = &sTlkDrvSerialTl752xItem[port].uartHandle;
    cfg.pRxDmaHandle = pPort->rxDma ? &sTlkDrvSerialTl752xItem[port].rxDmaHandle : NULL;
    cfg.pTxDmaHandle = pPort->txDma ? &sTlkDrvSerialTl752xItem[port].txDmaHandle : NULL;
    if (port == 0) {
        cfg.pUartHandle->instance = UART0;
    } else if (port == 1) {
        cfg.pUartHandle->instance = UART1;
    }
#endif
    tlkhal_uart_close(&cfg);


    sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_OPEN;
    sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_TX_BUSY;
    sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_RX_BUSY;
    tlkdrv_serial_updateBusyState(port);

    return TLK_ENONE;
}

/**
 * @brief       This function sets the transmit FIFO for the serial port.
 * @param[in]   port     - Serial port number.
 * @param[in]   pBuffer  - Buffer for FIFO.
 * @param[in]   buffLen  - Length of buffer.
 * @return      0: success.
 */
int tlkdrv_serial_setTxFifo(uint8_t port, uint8_t *pBuffer, uint16_t buffLen)
{
    uint16_t flags;
    if (port >= TLKDRV_SERIAL_MAX_NUMB || pBuffer == NULL || buffLen < 64) {
        return -TLK_EPARAM;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_INIT) == 0) {
        return -TLK_ENOREADY;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_SEND) == 0 || (flags & TLKDRV_SERIAL_FLAG_TX_DMA) != 0) {
        return -TLK_ENOSUPPORT;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) != 0) {
        return -TLK_ESTATUS;
    }
    sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_TX_FIFO;
    return TLK_ENONE;
}

/**
 * @brief       This function sets the receive FIFO for the serial port.
 * @param[in]   port     - Serial port number.
 * @param[in]   pBuffer  - Buffer for FIFO.
 * @param[in]   buffLen  - Length of buffer.
 * @return      0: success.
 */
int tlkdrv_serial_setRxFifo(uint8_t port, uint8_t *pBuffer, uint16_t buffLen)
{
    int            ret;
    uint16_t       flags;
    tlkapi_fifo_t *pFifo;
    if (port >= TLKDRV_SERIAL_MAX_NUMB || pBuffer == NULL || buffLen < 64) {
        return -TLK_EPARAM;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_INIT) == 0) {
        return -TLK_ENOREADY;
    }
    if (sTlkDrvSerial.port[port].rxPin == 0) {
        return -TLK_ENOREADY;
    }

    if ((flags & TLKDRV_SERIAL_FLAG_RECV) == 0 || (flags & TLKDRV_SERIAL_FLAG_RX_DMA) != 0) {
        return -TLK_ENOSUPPORT;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) != 0) {
        return -TLK_ESTATUS;
    }

    pFifo = &sTlkDrvSerial.rfifo[port].fifo;
    ret   = tlkapi_fifo_init(pFifo, TLKAPI_FIFO_CFG_NONE, pBuffer, buffLen);
    if (ret == TLK_ENONE) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_RX_FIFO;
    } else {
        sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_RX_FIFO;
    }
    return ret;
}

/**
 * @brief       This function sets the transmit QFIFO for the serial port.
 * @param[in]   port     - Serial port number.
 * @param[in]   fnumb    - Number of FIFO items.
 * @param[in]   fsize    - Size of FIFO item.
 * @param[in]   pBuffer  - Buffer for FIFO.
 * @param[in]   buffLen  - Length of buffer.
 * @return      0: success.
 */
int tlkdrv_serial_setTxQFifo(uint8_t port, uint16_t fnumb, uint16_t fsize, uint8_t *pBuffer, uint32_t buffLen)
{
    int             ret;
    uint16_t        flags;
    tlkapi_qfifo_t *pFifo;

    if (port >= TLKDRV_SERIAL_MAX_NUMB) {
        return -TLK_EPARAM;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_INIT) == 0) {
        return -TLK_ENOREADY;
    }

    if (sTlkDrvSerial.port[port].txPin == 0) {
        return -TLK_ENOREADY;
    }

    if (pBuffer == NULL || fnumb == 0 || fsize < 8) {
        return -TLK_EPARAM;
    }
    if ((((uint32_t)pBuffer) & 0x03) != 0 || (fsize & 0x03) != 0) {
        return -TLK_EALIGN;
    }

    if ((flags & TLKDRV_SERIAL_FLAG_SEND) == 0 || (flags & TLKDRV_SERIAL_FLAG_TX_DMA) == 0) {
        return -TLK_ENOSUPPORT;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) != 0) {
        return -TLK_ESTATUS;
    }

    pFifo = &sTlkDrvSerial.sfifo[port].qfifo;
    ret   = tlkapi_qfifo_init(pFifo, fnumb, fsize, pBuffer, buffLen);
    if (ret == TLK_ENONE) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_TX_FIFO;
        sTlkDrvSerial.sfifoM60[port] = fnumb * 3 / 5;
        sTlkDrvSerial.sfifoM85[port] = fnumb * 17 / 20;
    } else {
        sTlkDrvSerial.port[port].txDma = 0;
        sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_TX_FIFO;
    }
    return ret;
}

/**
 * @brief       This function sets the receive QFIFO for the serial port.
 * @param[in]   port     - Serial port number.
 * @param[in]   fnumb    - Number of FIFO items.
 * @param[in]   fsize    - Size of FIFO item.
 * @param[in]   pBuffer  - Buffer for FIFO.
 * @param[in]   buffLen  - Length of buffer.
 * @return      0: success.
 */
int tlkdrv_serial_setRxQFifo(uint8_t port, uint16_t fnumb, uint16_t fsize, uint8_t *pBuffer, uint32_t buffLen)
{
    int             ret;
    uint16_t        flags;
    tlkapi_qfifo_t *pFifo;

    if (port >= TLKDRV_SERIAL_MAX_NUMB) {
        return -TLK_EPARAM;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_INIT) == 0) {
        return -TLK_ENOREADY;
    }

    if (sTlkDrvSerial.port[port].rxPin == 0) {
        return -TLK_ENOREADY;
    }

    if (pBuffer == NULL || fnumb == 0 || fsize < 8) {
        return -TLK_EPARAM;
    }
    if ((((uint32_t)pBuffer) & 0x03) != 0 || (fsize & 0x03) != 0) {
        return -TLK_EALIGN;
    }

    if ((flags & TLKDRV_SERIAL_FLAG_RECV) == 0 || (flags & TLKDRV_SERIAL_FLAG_RX_DMA) == 0) {
        return -TLK_ENOSUPPORT;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) != 0) {
        return -TLK_ESTATUS;
    }

    pFifo = &sTlkDrvSerial.rfifo[port].qfifo;
    ret   = tlkapi_qfifo_init(pFifo, fnumb, fsize, pBuffer, buffLen);
    if (ret == TLK_ENONE) {
        sTlkDrvSerial.flags[port] |= TLKDRV_SERIAL_FLAG_RX_FIFO;
    } else {
        sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_RX_FIFO;
    }
    return ret;
}

/**
 * @brief       This function resets the serial port.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
void tlkdrv_serial_reset(uint8_t port)
{
    uint16_t flags;

    if (port >= TLKDRV_SERIAL_MAX_NUMB) {
        return;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_INIT) == 0 || (flags & TLKDRV_SERIAL_FLAG_OPEN) == 0) {
        return;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_TX_BUSY) != 0 || (flags & TLKDRV_SERIAL_FLAG_RX_BUSY) != 0) {
        return;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_SEND) != 0) {
        if ((flags & TLKDRV_SERIAL_FLAG_TX_DMA) == 0) {
            tlkapi_fifo_clear(&sTlkDrvSerial.sfifo[port].fifo);
        } else {
            dma_chn_dis(sTlkDrvSerial.port[port].txDma);
#if (!MCU_CORE_TL752X_TEMP)
            dma_clr_irq_mask(sTlkDrvSerial.port[port].txDma, TC_MASK | ABT_MASK | ERR_MASK);
#endif
            tlkapi_qfifo_clear(&sTlkDrvSerial.sfifo[port].qfifo);
        }
    }
    if ((flags & TLKDRV_SERIAL_FLAG_RECV) != 0) {
        if ((flags & TLKDRV_SERIAL_FLAG_RX_DMA) == 0) {
            tlkapi_fifo_clear(&sTlkDrvSerial.rfifo[port].fifo);
        } else {
            dma_chn_dis(sTlkDrvSerial.port[1].rxDma);
#if (!MCU_CORE_TL752X_TEMP)
            dma_clr_irq_mask(sTlkDrvSerial.port[1].rxDma, TC_MASK | ABT_MASK | ERR_MASK);
#endif
            tlkapi_qfifo_clear(&sTlkDrvSerial.rfifo[port].qfifo);
            tlkdrv_serial_setRxDmaBuffer(port);
        }
    }
}

/**
 * @brief       This function registers the receive callback for the serial port.
 * @param[in]   port - Serial port number.
 * @param[in]   cb   - Receive callback.
 * @return      none.
 */
void tlkdrv_serial_regCB(uint8_t port, TlkDrvSerialRecvCB cb)
{
    if (port >= TLKDRV_SERIAL_MAX_NUMB) {
        return;
    }
    sTlkDrvSerial.recvCB[port] = cb;
}

/**
 * @brief       This function checks if the transmit FIFO is more than 60% full.
 * @param[in]   port     - Serial port number.
 * @param[in]   dataLen  - Length of data to send.
 * @return      true: more than 60% full.
 */
bool tlkdrv_serial_sfifoIsMore60(uint8_t port, uint16_t dataLen)
{
    uint16_t flags;
    uint16_t count;
    uint16_t isize; // Item Size

    if (port >= TLKDRV_SERIAL_MAX_NUMB) {
        return true;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) == 0 || (flags & TLKDRV_SERIAL_FLAG_SEND) == 0) {
        return true;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_TX_DMA) == 0) {
        return false;
    }

    isize = tlkapi_qfifo_size(&sTlkDrvSerial.sfifo[port].qfifo) - 4;
    if (isize == 0) {
        return true;
    }
    if (dataLen == 0) {
        count = 0;
    } else if (dataLen <= isize) {
        count = 1;
    } else if (dataLen + dataLen <= isize) {
        count = 2;
    } else {
        count = (dataLen + isize - 1) / isize;
    }
    if (tlkapi_qfifo_usedNum(&sTlkDrvSerial.sfifo[port].qfifo) + count > sTlkDrvSerial.sfifoM60[port]) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       This function checks if the transmit FIFO is more than 85% full.
 * @param[in]   port     - Serial port number.
 * @param[in]   dataLen  - Length of data to send.
 * @return      true: more than 85% full.
 */
bool tlkdrv_serial_sfifoIsMore85(uint8_t port, uint16_t dataLen)
{
    uint16_t flags;
    uint16_t count;
    uint16_t isize; // Item Size

    if (port >= TLKDRV_SERIAL_MAX_NUMB) {
        return true;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) == 0 || (flags & TLKDRV_SERIAL_FLAG_SEND) == 0) {
        return true;
    }
    if ((flags & TLKDRV_SERIAL_FLAG_TX_DMA) == 0) {
        return false;
    }

    isize = tlkapi_qfifo_size(&sTlkDrvSerial.sfifo[port].qfifo) - 4;
    if (isize == 0) {
        return true;
    }
    if (dataLen == 0) {
        count = 0;
    } else if (dataLen <= isize) {
        count = 1;
    } else if (dataLen + dataLen <= isize) {
        count = 2;
    } else {
        count = (dataLen + isize - 1) / isize;
    }
    if (tlkapi_qfifo_usedNum(&sTlkDrvSerial.sfifo[port].qfifo) + count > sTlkDrvSerial.sfifoM85[port]) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief       This function handles the serial port.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
void tlkdrv_serial_handler(uint8_t port)
{
    uint16_t flags;

    if (port >= TLKDRV_SERIAL_MAX_NUMB) {
        return;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) == 0) {
        return;
    }

    if ((flags & TLKDRV_SERIAL_FLAG_SEND) != 0 && (flags & TLKDRV_SERIAL_FLAG_TX_DMA) != 0 && (flags & TLKDRV_SERIAL_FLAG_TX_BUSY) == 0) {
        tlkdrv_serial_setTxDmaBuffer(port);
    }
    if ((flags & TLKDRV_SERIAL_FLAG_RECV) != 0) {
        if ((flags & TLKDRV_SERIAL_FLAG_RX_DMA) == 0) {
            int     ret;
            uint8_t data[64];
            while (true) {
                ret = tlkapi_fifo_read(&sTlkDrvSerial.rfifo[port].fifo, data, 64);
                if (ret <= 0) {
                    break;
                }
                if (sTlkDrvSerial.recvCB[port] != NULL) {
                    sTlkDrvSerial.recvCB[port](data, ret);
                }
            }
        } else {
            uint8_t *pData;
            uint16_t dataLen;
            tlkhal_dma_clearTcIrq(sTlkDrvSerial.port[port].rxDma);

            while (!tlkapi_qfifo_isEmpty(&sTlkDrvSerial.rfifo[port].qfifo)) {
                pData = tlkapi_qfifo_getData(&sTlkDrvSerial.rfifo[port].qfifo);
#if (MCU_CORE_TYPE != CHIP_TYPE_TL752X)
                dataLen = (((uint16_t)pData[1]) << 8) | pData[0];
                pData += 4;
#else
                dataLen = TLKDRV_SERIAL_DMA_RX_SIZE;
#endif
                if (sTlkDrvSerial.recvCB[port] != NULL) {
                    sTlkDrvSerial.recvCB[port](pData, dataLen);
                }
                tlkapi_qfifo_dropData(&sTlkDrvSerial.rfifo[port].qfifo);
            }
            if ((flags & TLKDRV_SERIAL_FLAG_RX_BUSY) != 0) {
                tlkdrv_serial_setRxDmaBuffer(port);
            }
        }
    }
}

/**
 * @brief       This function reads data from the serial port.
 * @param[in]   port     - Serial port number.
 * @param[in]   pBuff    - Buffer to read data into.
 * @param[in]   buffLen  - Length of buffer.
 * @return      Number of bytes read.
 */
int tlkdrv_serial_read(uint8_t port, uint8_t *pBuff, uint16_t buffLen)
{
    uint16_t flags;

    if (port >= TLKDRV_SERIAL_MAX_NUMB || pBuff == NULL || buffLen == 0) {
        return -TLK_EPARAM;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) == 0 || (flags & TLKDRV_SERIAL_FLAG_RECV) == 0) {
        return -TLK_ENOREADY;
    }

    if ((flags & TLKDRV_SERIAL_FLAG_RX_DMA) == 0) {
        return tlkapi_fifo_read(&sTlkDrvSerial.rfifo[port].fifo, pBuff, buffLen);
    } else {
        uint8_t *pData;
        uint16_t dataLen = 0;
        pData            = tlkapi_qfifo_getData(&sTlkDrvSerial.rfifo[port].qfifo);
        if (pData != NULL) {
            dataLen = ((uint16_t)pData[1] << 8) | pData[0];
        }
        if (dataLen > buffLen) {
            return -TLK_EOVERFLOW;
        }
        if (dataLen != 0) {
            tmemcpy(pBuff, pData, dataLen);
        }
        if (pData != NULL) {
            tlkapi_qfifo_dropData(&sTlkDrvSerial.rfifo[port].qfifo);
        }
        if ((flags & TLKDRV_SERIAL_FLAG_RX_BUSY) != 0) {
            tlkdrv_serial_setRxDmaBuffer(port);
        }
        return dataLen;
    }
}

/**
 * @brief       This function sends data to the serial port.
 * @param[in]   port     - Serial port number.
 * @param[in]   pData    - Data to send.
 * @param[in]   dataLen  - Length of data to send.
 * @return      0: success.
 */
int tlkdrv_serial_send(uint8_t port, uint8_t *pData, uint16_t dataLen)
{
    int      ret;
    uint16_t flags;

    if (port >= TLKDRV_SERIAL_MAX_NUMB || pData == NULL || dataLen == 0) {
        return -TLK_EPARAM;
    }

    flags = sTlkDrvSerial.flags[port];
    if ((flags & TLKDRV_SERIAL_FLAG_OPEN) == 0 || (flags & TLKDRV_SERIAL_FLAG_SEND) == 0) {
        return -TLK_ENOREADY;
    }
    if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_SLEEP) != 0) {
        return -TLK_EBUSY;
    }

    tlksys_enter_critical();
    if ((flags & TLKDRV_SERIAL_FLAG_TX_DMA) == 0) {
        ret = tlkdrv_serial_sendWithoutDma(port, pData, dataLen);
    } else {
        if (flags & TLKDRV_SERIAL_FLAG_RTX_MODE) {
#if (!MCU_CORE_TL752X_TEMP)
            uart_rtx_pin_tx_trig(port);
#endif
        }
        ret = tlkdrv_serial_sendWithDma(port, pData, dataLen);
    }
    tlksys_leave_critical();
    return ret;
}

/**
 * @brief       This function handles the error interrupt for the serial port.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void tlkdrv_serial_irqErrDone(uint8_t port)
{
    sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_TX_BUSY;
    tlkdrv_serial_updateBusyState(port);
    if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_SEND) != 0 && (sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_TX_DMA) != 0) {
        tlkdrv_serial_setTxDmaBuffer(port);
    }
    if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_RECV) != 0 && (sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_RX_DMA) != 0) {
        tlkdrv_serial_setRxDmaBuffer(port);
    }
}

/**
 * @brief       This function handles the transmit done interrupt for the serial port with DMA.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void tlkdrv_serial_irqTxDoneWithDma(uint8_t port)
{
    if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_SEND) == 0 || (sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_TX_DMA) == 0) {
        return;
    }
    if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_TX_BUSY) != 0) {
        tlkapi_qfifo_dropData(&sTlkDrvSerial.sfifo[port].qfifo);
        sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_TX_BUSY;
        tlkdrv_serial_updateBusyState(port);
    }
    tlkdrv_serial_setTxDmaBuffer(port);
}

/**
 * @brief       This function handles the transmit done interrupt for the serial port without DMA.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void tlkdrv_serial_irqTxDoneWithoutDma(uint8_t port)
{
    if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_SEND) == 0 || (sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_TX_DMA) != 0) {
        return;
    }
    sTlkDrvSerial.flags[port] &= ~TLKDRV_SERIAL_FLAG_TX_BUSY;
    tlkdrv_serial_updateBusyState(port);
}

/**
 * @brief       This function handles the receive done interrupt for the serial port with DMA.
 * @param[in]   port     - Serial port number.
 * @param[in]   isDmaIrq - true: DMA interrupt.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void tlkdrv_serial_irqRxDoneWithDma(uint8_t port, bool isDmaIrq)
{
    uint32_t  recvLen = 0;
    uint32_t *pBuffer;
    if (!isDmaIrq) {
        if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_RECV) == 0 || (sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_RX_DMA) == 0) {
            tlkhal_uart_clrRxDoneStatus(port, NULL);
            return;
        }
    }
#if (MCU_CORE_TYPE == MCU_CORE_B92 || MCU_CORE_TYPE == MCU_CORE_B91) //?? driver api always change
    recvLen = uart_get_dma_rev_data_len(port, sTlkDrvSerial.port[port].rxDma);
#elif (MCU_CORE_TL752X_TEMP)
    recvLen = TLKDRV_SERIAL_DMA_RX_SIZE;
#else
    recvLen = ((uint32_t *)tlkapi_qfifo_getBuff(&sTlkDrvSerial.rfifo[port].qfifo))[0];
#endif
#if (MCU_CORE_TYPE == MCU_CORE_B91)          //TODO add to hal
    tlkhal_uart_clrRxDoneStatus(port, NULL); //??????????
#else
    if (!isDmaIrq) {
        tlkhal_uart_clrRxDoneStatus(port, NULL);
    }
#endif

    if (recvLen != 0) {
        pBuffer = (uint32_t *)tlkapi_qfifo_getBuff(&sTlkDrvSerial.rfifo[port].qfifo);
        if (pBuffer != NULL) {
#if (!MCU_CORE_TL752X_TEMP)
            pBuffer[0] = recvLen;
#endif
            tlkapi_qfifo_dropBuff(&sTlkDrvSerial.rfifo[port].qfifo);
        }
        tlkdrv_serial_setRxDmaBuffer(port);
    }
    tlksys_task_setEvtFromIsr(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SERIAL);
}

/**
 * @brief       This function handles the receive done interrupt for the serial port without DMA.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void tlkdrv_serial_irqRxDoneWithoutDma(uint8_t port)
{
    if ((sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_RECV) == 0 || (sTlkDrvSerial.flags[port] & TLKDRV_SERIAL_FLAG_RX_DMA) != 0) {
        return;
    }
#if (!MCU_CORE_TL752X_TEMP)
    while (uart_get_rxfifo_num(port) > 0) {
        uint8_t byte = reg_uart_data_buf(port, uart_rx_byte_index[port]);
        uart_rx_byte_index[port]++;
        uart_rx_byte_index[port] &= 0x03;
        tlkapi_fifo_write(&sTlkDrvSerial.rfifo[port].fifo, &byte, 1);
    }
#else
    uint32_t recvLen = sTlkDrvSerialTl752xItem[port].uartHandle.rx_callback_arg->rx_data_len;
    tlkdrv_serial_setRxBuffer(port);
    tlkapi_fifo_write(&sTlkDrvSerial.rfifo[port].fifo, sTlkDrvSerial.rfifo[port].fifo.pBuffer, recvLen);
    tlksys_task_setEvtFromIsr(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SERIAL);
#endif
}

/**
 * @brief       This function handles the UART interrupt.
 * @param[in]   port - Serial port number.
 * @return      none.
 */
_attribute_ram_code_sec_ void tlkdrv_uart_irqHandler(uint8_t port)
{
#if (!MCU_CORE_TL752X_TEMP)
    if (uart_get_irq_status(port, UART_TXBUF_IRQ_STATUS)) { // for no dam
        tlkdrv_serial_irqTxDoneWithoutDma(port);
    }
    if (tlkhal_uart_getTxDoneStatus(port, NULL)) { // for dma
        tlkhal_uart_clrTxDoneStatus(port, NULL);
        tlkdrv_serial_irqTxDoneWithDma(port);
    }
    if (uart_get_irq_status(port, UART_RXBUF_IRQ_STATUS)) { // for no dam
        tlkdrv_serial_irqRxDoneWithoutDma(port);
    }
    if (tlkhal_uart_getRxDoneStatus(port, NULL)) { // for dma
        tlkdrv_serial_irqRxDoneWithDma(port, false);
    }
    if (uart_get_irq_status(port, UART_RX_ERR)) {
        uart_clr_irq_status(port, UART_RXBUF_IRQ_STATUS);
        // it will clear rx_fifo and rx_err_irq ,rx_buff_irq,so it won't enter rx_buff interrupt.
        uart_reset(port);        // clear hardware pointer
        uart_clr_rx_index(port); // clear software pointer
        tlkdrv_serial_irqErrDone(port);
    }
#elif (MCU_CORE_TL752X_TEMP)
    if (port > 4) {
        return;
    }
    drv_uart_irq_handler(&sTlkDrvSerialTl752xItem[port].uartHandle);
#else
#error "Please adapt tlkdrv_uart_irqHandler()"
#endif
}

/**
 * @brief       This function handles the UART DMA interrupt.
 * @param[in]   port - Serial port number.
 * @param[in]   rxDma - Receive DMA.
 * @return      none.
 */
_attribute_ram_code_sec_ void tlkdrv_uart_dmaIrqHandler(uint8_t port, uint8_t rxDma)
{
    (void)rxDma;
    tlkdrv_serial_irqRxDoneWithDma(port, true);
}

/**
 * @brief       This function handles the UART DMA interrupt.
 * @return      none.
 */
_attribute_ram_code_sec_ void tlkdrv_serial_dmaIrqHandler(void)
{
#if (!MCU_CORE_TL752X_TEMP)
    if ((sTlkDrvSerial.flags[UART0] & TLKDRV_SERIAL_FLAG_OPEN) != 0 && sTlkDrvSerial.port[UART0].rxDma != 0) {
        if (dma_get_tc_irq_status(BIT(sTlkDrvSerial.port[UART0].rxDma))) {
            dma_clr_tc_irq_status(BIT(sTlkDrvSerial.port[UART0].rxDma));
            tlkdrv_uart_dmaIrqHandler(sTlkDrvSerial.port[UART0].port, sTlkDrvSerial.port[UART0].rxDma);
        }
    }
    if ((sTlkDrvSerial.flags[UART1] & TLKDRV_SERIAL_FLAG_OPEN) != 0 && sTlkDrvSerial.port[UART1].rxDma != 0) {
        if (dma_get_tc_irq_status(BIT(sTlkDrvSerial.port[UART1].rxDma))) {
            dma_clr_tc_irq_status(BIT(sTlkDrvSerial.port[UART1].rxDma));
            tlkdrv_uart_dmaIrqHandler(sTlkDrvSerial.port[UART1].port, sTlkDrvSerial.port[UART1].rxDma);
        }
    }
#elif (MCU_CORE_TL752X_TEMP)
    for (int iIndex = 0; iIndex < TLKDRV_SERIAL_MAX_NUMB; iIndex++) {
        if ((sTlkDrvSerial.flags[iIndex] & TLKDRV_SERIAL_FLAG_OPEN) != 0 && sTlkDrvSerial.port[iIndex].rxDma != 0) {
            drv_dmac_irq_handle(&sTlkDrvSerialTl752xItem[iIndex].rxDmaHandle);
        }
        if ((sTlkDrvSerial.flags[iIndex] & TLKDRV_SERIAL_FLAG_OPEN) != 0 && sTlkDrvSerial.port[iIndex].txDma != 0) {
            drv_dmac_irq_handle(&sTlkDrvSerialTl752xItem[iIndex].txDmaHandle);
        }
    }
#else
#error "Please adapt tlkdrv_serial_dmaIrqHandler()"
#endif
}


#endif // #if (TLK_DEV_SERIAL_ENABLE)
