/********************************************************************************************************
 * @file    tlkdrv_spi.c
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
#include "tlkmw/tlkmw.h"
#if TLK_DRV_SPI_ENABLE
#define SPI_CLK 24000000
#if (TLKHW_TYPE == BOARD_721X_EVK_C1T315A87_V1_0)
#define SPI_TX_DMA_CHN DMA6
#define SPI_RX_DMA_CHN DMA7
#else //#elif (TLKHW_TYPE ==TLKHW_TL751X_EVK_C1T368A87_V1_0)
#define SPI_TX_DMA_CHN DMA8
#define SPI_RX_DMA_CHN DMA9
#endif

volatile unsigned char end_irq_flag = 0;
volatile unsigned char rx_dma_flag  = 0;
static TlkApiTimer_t   sTlkMdiSpiTimer;

/**
 * @brief SPI FIFO union type
 */
typedef union
{
    tlkapi_fifo_t  fifo;  /**< FIFO structure */
    tlkapi_qfifo_t qfifo; /**< Queue FIFO structure */
} tlkdrv_spi_fifo_t;

/**
 * @brief SPI driver control structure
 */
typedef struct
{
    uint16_t          flags;  /**< SPI flags */
    tlkdrv_spi_fifo_t sfifo;  /**< Send FIFO */
    tlkdrv_spi_fifo_t rfifo;  /**< Receive FIFO */
    TlkDrvSpiRecvCB   recvCB; /**< Receive callback function */
} tlkdrv_spi_t;

static tlkdrv_spi_t sTlkDrvSpi = {0};
//CS:15  clk:16  mosi:17  miso:18
lspi_pin_config_t lspi_pin_config = {
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A20_V1_0)
    .spi_csn_pin      = GPIO_FC_PF5,
    .spi_clk_pin      = GPIO_FC_PB0,
    .spi_mosi_io0_pin = GPIO_FC_PC6,
    .spi_miso_io1_pin = GPIO_FC_PC0, //3line mode is required, otherwise it is NONE_PIN.
    .spi_io2_pin      = 0,           //quad  mode is required, otherwise it is NONE_PIN.
    .spi_io3_pin      = 0,           //quad  mode is required, otherwise it is NONE_PIN.
#elif (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0)
    .spi_csn_pin      = GPIO_FC_PA6,
    .spi_clk_pin      = GPIO_FC_PA5,
    .spi_mosi_io0_pin = GPIO_FC_PB0,
    .spi_miso_io1_pin = GPIO_FC_PB2, //3line mode is required, otherwise it is NONE_PIN.
    .spi_io2_pin      = 0,           //quad  mode is required, otherwise it is NONE_PIN.
    .spi_io3_pin      = 0,           //quad  mode is required, otherwise it is NONE_PIN.
#elif (TLKHW_TYPE == BOARD_721X_EVK_C1T315A87_V1_0)
    .spi_csn_pin      = LSPI_CSN_PE6_PIN,
    .spi_clk_pin      = LSPI_CLK_PE1_PIN,
    .spi_mosi_io0_pin = LSPI_MOSI_IO0_PE2_PIN,
    .spi_miso_io1_pin = LSPI_MISO_IO1_PE3_PIN, //3line mode is required, otherwise it is NONE_PIN.
    .spi_io2_pin      = 0,                     //quad  mode is required, otherwise it is NONE_PIN.
    .spi_io3_pin      = 0,                     //quad  mode is required, otherwise it is NONE_PIN.
#endif
};
//gspi_pin_config_t gspi_pin_config = {
//    .spi_csn_pin      = GPIO_FC_PA3,
//    .spi_clk_pin      = GPIO_FC_PA5,
//    .spi_mosi_io0_pin = GPIO_FC_PA4,
//    .spi_miso_io1_pin = GPIO_FC_PA6,   //3line mode is required, otherwise it is NONE_PIN.
//    .spi_io2_pin      = 0,   //quad  mode is required, otherwise it is NONE_PIN.
//    .spi_io3_pin      = 0,   //quad  mode is required, otherwise it is NONE_PIN.
//};

//#define TLKDBG_SPI_BUFFER_SIZE (1024 * 2)
//static tlkapi_fifo_t sTlkspiFifo;
//uint8_t sTlkDbgSpiBuffer[TLKDBG_SPI_BUFFER_SIZE];
#define SPI_MODULE_SEL 0 //LSPI_MODULE
//#define SPI_MODULE_SEL 1//GSPI_MODULE


#define TLKMDI_COMM_SPI_SBUFF_NUMB 8
#define TLKMDI_COMM_SPI_SBUFF_SIZE 600

__attribute__((aligned(4))) static uint8_t sTlkMdiCommSpiSendBuffer[TLKMDI_COMM_SPI_SBUFF_NUMB * (TLKMDI_COMM_SPI_SBUFF_SIZE + 4)];
__attribute__((aligned(4))) static uint8_t sTlkMdiCommSpiRevBuffer[TLKMDI_COMM_SPI_SBUFF_NUMB * (TLKMDI_COMM_SPI_SBUFF_SIZE + 4)];

/**
 * @brief Set SPI receive DMA buffer
 * @return none
 */
static void tlkdrv_spi_setRxDmaBuffer(void);

/**
 * @brief Register SPI receive callback function
 * @param[in] cb - callback function pointer
 * @return none
 */
void tlkdrv_spi_regCB(TlkDrvSpiRecvCB cb)
{
    if (cb == NULL) {
        return;
    }
    sTlkDrvSpi.recvCB = cb;
}

/**
 * @brief Set SPI transmit queue FIFO
 * @param[in] fnumb - number of FIFO items
 * @param[in] fsize - size of each FIFO item
 * @param[in] pBuffer - pointer to buffer
 * @param[in] buffLen - length of buffer
 * @return 0: success, <0: failure
 */
int tlkdrv_spi_setTxQFifo(uint16_t fnumb, uint16_t fsize, uint8_t *pBuffer, uint32_t buffLen)
{
    int             ret = 0;
    tlkapi_qfifo_t *pFifo;
    pFifo = &sTlkDrvSpi.sfifo.qfifo;
    ret   = tlkapi_qfifo_init(pFifo, fnumb, fsize, pBuffer, buffLen);
    return ret;
}

/**
 * @brief Set SPI receive queue FIFO
 * @param[in] fnumb - number of FIFO items
 * @param[in] fsize - size of each FIFO item
 * @param[in] pBuffer - pointer to buffer
 * @param[in] buffLen - length of buffer
 * @return 0: success, <0: failure
 */
int tlkdrv_spi_setRxQFifo(uint16_t fnumb, uint16_t fsize, uint8_t *pBuffer, uint32_t buffLen)
{
    int             ret = 0;
    tlkapi_qfifo_t *pFifo;
    pFifo = &sTlkDrvSpi.rfifo.qfifo;
    ret   = tlkapi_qfifo_init(pFifo, fnumb, fsize, pBuffer, buffLen);
    return ret;
}

/**
 * @brief Reinitialize SPI IO functions
 * @return none
 */
void tlkdrv_spi_io_function_reinit(void)
{
#if (SPI_MODULE_SEL)
    {
        gspi_set_pin(&gspi_pin_config);
    }
#else
    {
        lspi_set_pin(&lspi_pin_config);
    }
#endif
}

/**
 * @brief Initialize SPI driver
 * @return none
 */
void tlkdrv_spi_init(void)
{
#if (CHIP_TYPE == CHIP_TYPE_TL751X)
    spi_master_init(SPI_MODULE_SEL, SRC_CLK_XTAL_48M, SRC_CLK_XTAL_48M / SPI_CLK, SPI_MODE0);
#elif (CHIP_TYPE == CHIP_TYPE_TL721X)
    spi_master_init(SPI_MODULE_SEL, sys_clk.pll_clk * 1000000 / SPI_CLK, SPI_MODE0);
#endif
    spi_clr_irq_status(SPI_MODULE_SEL, SPI_END_INT);  //clr
    spi_set_irq_mask(SPI_MODULE_SEL, SPI_END_INT_EN); //endint_en
    spi_set_tx_dma_config(SPI_MODULE_SEL, SPI_TX_DMA_CHN);
    sTlkDrvSpi.flags |= TLKDRV_SPI_FLAG_TX_DMA;
    spi_set_master_rx_dma_config(SPI_MODULE_SEL, SPI_RX_DMA_CHN);
    sTlkDrvSpi.flags |= TLKDRV_SPI_FLAG_RX_DMA;
#if (SPI_MODULE_SEL)
    {
        gspi_set_pin(&gspi_pin_config);
        plic_interrupt_enable(IRQ_GSPI);
    }
#else
    {
        lspi_set_pin(&lspi_pin_config);
        plic_interrupt_enable(IRQ_LSPI);
    }
#endif
#if (CHIP_TYPE == CHIP_TYPE_TL751X)
    spi_master_config(SPI_MODULE_SEL, SPI_NORMAL);
#elif (CHIP_TYPE == CHIP_TYPE_TL721X)
    spi_master_config(SPI_MODULE_SEL, SPI_NORMAL);
#endif

    dma_set_irq_mask(SPI_RX_DMA_CHN, TC_MASK);
    plic_interrupt_enable(IRQ_DMA);

    tlkdrv_spi_setTxQFifo(TLKMDI_COMM_SPI_SBUFF_NUMB, TLKMDI_COMM_SPI_SBUFF_SIZE, sTlkMdiCommSpiSendBuffer, TLKMDI_COMM_SPI_SBUFF_NUMB * (TLKMDI_COMM_SPI_SBUFF_SIZE + 4));
    tlkdrv_spi_setRxQFifo(TLKMDI_COMM_SPI_SBUFF_NUMB, TLKMDI_COMM_SPI_SBUFF_SIZE, sTlkMdiCommSpiRevBuffer, 4 * (128 + 4));
}

/**
 * @brief Write data with DMA in application
 * @return none
 */
void app_spi_write_data_with_dma(void)
{
    // tlk_printf("++++++++++++++tlkdrv_spi_init");
    uint8_t send_data[256] = {0, 1, 2, 3, 4, 5, 6, 7};
    spi_master_write_dma(0, send_data, 256);
    spi_master_write_dma_plus(SPI_MODULE_SEL, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, send_data, sizeof(send_data), SPI_MODE_WR_WRITE_ONLY);
    while (spi_is_busy(0));
}

/**
 * @brief Set SPI transmit DMA buffer
 * @return none
 */
static void tlkdrv_spi_setTxDmauffer(void)
{
    uint8_t *pData;
    uint16_t dataLen = 0;
    pData            = tlkapi_qfifo_getData(&sTlkDrvSpi.sfifo.qfifo);
    if (pData != NULL) {
        dataLen = ((uint16_t)pData[1] << 8) | pData[0];
    }
    //  tlk_printf("tlkdrv_spi_setTxDmauffer-----1");
    if (dataLen == 0) {
        sTlkDrvSpi.flags &= ~TLKDRV_SPI_FLAG_TX_BUSY;
        // This data format is error
        if (pData != NULL) {
            tlkapi_qfifo_dropData(&sTlkDrvSpi.sfifo.qfifo);
        }
    } else {
        // delay_us(10);
        sTlkDrvSpi.flags |= TLKDRV_SPI_FLAG_TX_BUSY;
        spi_master_write_dma_plus(SPI_MODULE_SEL, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, pData + 4, dataLen, SPI_MODE_WR_WRITE_ONLY);
    }
}

/**
 * @brief Set SPI receive DMA buffer
 * @return none
 */
static void tlkdrv_spi_setRxDmaBuffer(void)
{
    uint32_t *pBuffer;
    pBuffer = (uint32_t *)tlkapi_qfifo_getBuff(&sTlkDrvSpi.rfifo.qfifo);
    if (pBuffer == NULL) {
        sTlkDrvSpi.flags |= TLKDRV_SPI_FLAG_RX_BUSY;
        //    tlkdrv_serial_updateBusyState(port);
    } else {
        pBuffer[0] = 0;
        sTlkDrvSpi.flags &= ~TLKDRV_SPI_FLAG_RX_BUSY;
        // tlkdrv_serial_updateBusyState(port);
        spi_master_read_dma_plus(SPI_MODULE_SEL, SPI_READ_DATA_SINGLE_CMD, (unsigned int)NULL, (unsigned char *)(pBuffer + 1), tlkapi_qfifo_size(&sTlkDrvSpi.rfifo.qfifo) - 4,
                                 SPI_MODE_RD_READ_ONLY);
    }
}

/**
 * @brief Send data with DMA
 * @param[in] pData - pointer to data to be sent
 * @param[in] dataLen - length of data to be sent
 * @param[in] ext_data - pointer to extended data
 * @param[in] ext_dataLen - length of extended data
 * @return 0: success, <0: failure
 */
int tlkdrv_spi_sendWithDma(uint8_t *pData, uint16_t dataLen, uint8_t *ext_data, uint8_t ext_dataLen)
{
    uint16_t count;
    uint16_t isize; // Item Size
    uint16_t tempVar;
    uint8_t *pBuffer;
    isize = tlkapi_qfifo_size(&sTlkDrvSpi.sfifo.qfifo) - 4;
    if (isize == 0) {
        return -TLK_EFAULT;
    }
    // tlk_printf("tlkdrv_spi_sendWithDma-----datalen = %d  ext_dataLen = %d",dataLen,ext_dataLen);
    if (ext_data != NULL && ext_dataLen != 0) {
        dataLen += ext_dataLen;
    }
    count = (dataLen + isize - 1) / isize;
    if (count > tlkapi_qfifo_idleNum(&sTlkDrvSpi.sfifo.qfifo)) {
        return -TLK_EQUOTA;
    }
    while ((count--) != 0 && dataLen != 0) {
        pBuffer = tlkapi_qfifo_getBuff(&sTlkDrvSpi.sfifo.qfifo);
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
        if (ext_data != NULL && ext_dataLen != 0) {
            // tlkapi_array(0xffffffff,"test","ext_data:",ext_data,ext_dataLen);
            tmemcpy(pBuffer + 4, ext_data, ext_dataLen);
            tmemcpy(pBuffer + 4 + ext_dataLen, pData, tempVar);
        } else {
            tmemcpy(pBuffer + 4, pData, tempVar);
        }

        pData += tempVar;
        dataLen -= tempVar;
        tlkapi_qfifo_dropBuff(&sTlkDrvSpi.sfifo.qfifo);
    }
    if ((sTlkDrvSpi.flags & TLKDRV_SPI_FLAG_TX_BUSY) == 0) {
        tlkdrv_spi_setTxDmauffer();
    }
    return TLK_ENONE;
}

/**
 * @brief Write data via SPI
 * @param[in] data - pointer to data to be written
 * @param[in] dataLen - length of data to be written
 * @return 0: success, <0: failure
 */
int tlkdrv_spi_write_data(uint8_t *data, uint16_t dataLen)
{
    uint8_t ret = TLK_ENONE;


    tlksys_enter_critical();
    uint8_t ext_data[20];
    uint8_t bufferLen     = 0;
    ext_data[bufferLen++] = 0xFF;
    ext_data[bufferLen++] = 0xFF;
    ret                   = tlkdrv_spi_sendWithDma(data, dataLen, ext_data, bufferLen);
    tlksys_leave_critical();

    return ret;
}

/**
 * @brief Write command via SPI
 * @param[in] data - pointer to command data to be written
 * @param[in] dataLen - length of command data to be written
 * @return 0: success, <0: failure
 */
int tlkdrv_spi_write_cmd(uint8_t *data, uint8_t dataLen)
{
    uint8_t ret = TLK_ENONE;

    tlksys_enter_critical();
    uint8_t ext_data[50];
    uint8_t bufferLen     = 0;
    ext_data[bufferLen++] = 0xFF;
    ext_data[bufferLen++] = 0xFF;
    ext_data[bufferLen++] = (TLKDRV_SPI_CMD_FORMAT >> 8) & 0xff; //bb
    ext_data[bufferLen++] = TLKDRV_SPI_CMD_FORMAT & 0xff;
    ext_data[bufferLen++] = dataLen;
    ret                   = tlkdrv_spi_sendWithDma(data, dataLen, ext_data, bufferLen);
    tlksys_leave_critical();
    return ret;
}

/**
 * @brief SPI interrupt handler
 * @return none
 */
#if (SPI_MODULE_SEL)
void gspi_irq_handler(void)
#else
void lspi_irq_handler(void)
#endif
{
    // tlk_printf("+++lspi_irq_handler");
    if (spi_get_irq_status(SPI_MODULE_SEL, SPI_END_INT)) {
        spi_clr_irq_status(SPI_MODULE_SEL, SPI_END_INT); //clr
        end_irq_flag = 1;
        if ((sTlkDrvSpi.flags & TLKDRV_SPI_FLAG_TX_BUSY) != 0) {
            sTlkDrvSpi.flags &= ~TLKDRV_SPI_FLAG_TX_BUSY;
            tlkapi_qfifo_dropData(&sTlkDrvSpi.sfifo.qfifo);
        }
        tlkdrv_spi_setTxDmauffer();
    }
}

// PLIC_ISR_REGISTER(lspi_irq_handler, IRQ_LSPI)

/**
 * @brief SPI send data timer callback
 * @param[in] pTimer - timer handle
 * @param[in] userArg - user argument
 * @return none
 */
void app_spi_sendDataTimer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    uint8_t data[100] = {1, 2, 3, 4, 5, 6};
    tlkdrv_spi_write_data(data, sizeof(data));
}

/**
 * @brief Start SPI data sending
 * @return 0: success, <0: failure
 */
int app_spi_satrt_send(void)
{
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &sTlkMdiSpiTimer, 5 * 1000, true, app_spi_sendDataTimer, 0);
    return tlksys_timer_start(TLKSYS_TASKID_SYSTEM, &sTlkMdiSpiTimer);
}

/**
 * @brief Stop SPI data sending
 * @return none
 */
void app_spi_satrt_send_end(void)
{
    tlksys_timer_destroy(TLKSYS_TASKID_SYSTEM, &sTlkMdiSpiTimer);
}

/**
 * @brief Handle SPI receive done with DMA
 * @return none
 */
void tlkdrv_spi_irqRxDoneWithDma(void)
{
    uint16_t  flags;
    uint32_t  recvLen = 0;
    uint32_t *pBuffer;
    flags = sTlkDrvSpi.flags;
    if ((flags & TLKDRV_SPI_FLAG_RX_DMA) == 0) {
        return;
    }
    recvLen = ((uint32_t *)tlkapi_qfifo_getBuff(&sTlkDrvSpi.rfifo.qfifo))[0];
    if (recvLen != 0) {
        pBuffer = (uint32_t *)tlkapi_qfifo_getBuff(&sTlkDrvSpi.rfifo.qfifo);
        if (pBuffer != NULL) {
            pBuffer[0] = recvLen;
            tlkapi_qfifo_dropBuff(&sTlkDrvSpi.rfifo.qfifo);
        }
        tlkdrv_spi_setRxDmaBuffer();
    }
    tlksys_task_setEvtFromIsr(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SPI_CTRL);
}

/**
 * @brief Handle SPI DMA interrupt
 * @return none
 */
void tlkdrv_spi_dmaIrqHandler(void)
{
    // tlk_printf("---tlkdrv_spi_dmaIrqHandler");
    if (dma_get_tc_irq_status(BIT(SPI_RX_DMA_CHN))) { //RX
        // tlk_printf("+++dma_irq_handler");
        dma_clr_tc_irq_status(BIT(SPI_RX_DMA_CHN));
        // rx_dma_flag = 1;
        tlkdrv_spi_irqRxDoneWithDma();
    }
}

/**
 * @brief Handle SPI operations in main loop
 * @return none
 */
void tlkdrv_spi_handler(void)
{
    uint16_t flags;
    tlk_printf("---tlkdrv_spi_handler %x", sTlkDrvSpi.flags);
    flags = sTlkDrvSpi.flags;
    if ((flags & TLKDRV_SPI_FLAG_TX_DMA) != 0 && (flags & TLKDRV_SPI_FLAG_TX_BUSY) != 0) {
        tlkdrv_spi_setTxDmauffer();
    }

    if ((flags & TLKDRV_SPI_FLAG_RX_DMA) != 0) {
        uint8_t *pData;
        uint16_t dataLen;
        tlkhal_dma_clearTcIrq(SPI_RX_DMA_CHN);

        while (!tlkapi_qfifo_isEmpty(&sTlkDrvSpi.rfifo.qfifo)) {
            pData   = tlkapi_qfifo_getData(&sTlkDrvSpi.rfifo.qfifo);
            dataLen = (((uint16_t)pData[1]) << 8) | pData[0];
            pData += 4;
            if (sTlkDrvSpi.recvCB != NULL) {
                sTlkDrvSpi.recvCB(pData, dataLen);
            }
            tlkapi_qfifo_dropData(&sTlkDrvSpi.rfifo.qfifo);
        }
        if ((flags & TLKDRV_SPI_FLAG_RX_BUSY) != 0) {
            tlkdrv_spi_setRxDmaBuffer();
        }
    }
}

/**
 * @brief Read data from SPI master
 * @param[in] pData - pointer to buffer for read data
 * @param[in] dataLen - length of data to read
 * @return none
 */
void tlkdrv_spi_master_read_data(uint8_t *pData, uint8_t dataLen)
{
    spi_master_read(SPI_MODULE_SEL, pData, dataLen);
    spi_set_transmode(SPI_MODULE_SEL, (spi_tans_mode_e)SPI_MODE_WR_WRITE_ONLY);
}

#endif //TLK_DRV_SPI_ENABLE
