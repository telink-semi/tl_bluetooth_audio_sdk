/********************************************************************************************************
 * @file    tlkhal_uart_TL752X.c
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
#include "../../api/tlkhal_api.h"
#if MCU_CORE_TYPE == CHIP_TYPE_TL752X
#include "drivers.h"
#include "tlkhal_uart_TL752X.h"


static void    tlkhal_uart_RxDoneCallback(uart_handle_t *huart);
static void    tlkhal_uart_TxDoneCallback(uart_handle_t *huart);
static void    tlkhal_uart_dmaTxDoneCallback(uart_handle_t *huart);
static void    tlkhal_uart_dmaRxDoneCallback(uart_handle_t *huart);
static uint8_t tlkhal_uart_getPortIrqNum(uart_handle_t *pUartHandle);

tlkhal_uart_dmaRxDoneCB   sTlkhalDmaRxDoneCB   = NULL;
tlkhal_uart_dmaTxDoneCB   sTlkhalDmaTxDoneCB   = NULL;
tlkhal_uart_noDmaRxDoneCB sTlkhalNoDmaRxDoneCB = NULL;

/**
 * @brief  Open and configure UART
 * @param[in] pCfg : Pointer to UART configuration structure
 * @returns  None.
 */
void tlkhal_uart_open(const tlkhal_uart_cfg_t *const pCfg)
{
    uint8_t  port     = pCfg->port;
    uint8_t  rxDma    = pCfg->rxDma;
    uint8_t  txDma    = pCfg->txDma;
    uint16_t txPin    = pCfg->txPin;
    uint16_t rxPin    = pCfg->rxPin;
    uint32_t baudRate = pCfg->baudRate;

    uint8_t irqNum = tlkhal_uart_getPortIrqNum(pCfg->pUartHandle);
    plic_interrupt_disable(irqNum);

    if (txPin != rxPin) {
        uint32_t gGroup = 0;
        gGroup          = ((uint32_t)txPin & 0xFF00) >> 8;
        uart_set_pin(gGroup, GPIO_PIN_((txPin & 0x7F)), GPIO_PIN_((rxPin & 0x7F)), GPIO_ALTERNATE_FUNC_2);
    } else {
        // TODO: ZEWEN one IO achieve RTX function by software.
    }

    pCfg->pUartHandle->init.baudrate  = baudRate;
    pCfg->pUartHandle->init.hwflowctl = UART_HWCONTROL_NONE;
    pCfg->pUartHandle->init.parity    = UART_PARITY_NONE;
    pCfg->pUartHandle->init.stopbits  = UART_STOPBITS_1;
    hal_uart_init(DRV_CPR_UART_MCLK_SRC_DBLR, pCfg->pUartHandle);

    if (rxDma != 0) {
        pCfg->pRxDmaHandle->init.channel = CPU_DMAC_CH0;
        uart_set_rx_dma_config(pCfg->pRxDmaHandle, pCfg->pUartHandle);
        drv_uart_register_callback(pCfg->pUartHandle, DRV_UART_DMA_RX_CB_ID, tlkhal_uart_dmaRxDoneCallback);
    }
    if (txDma != 0) {
        pCfg->pTxDmaHandle->init.channel = CPU_DMAC_CH1;
        uart_set_tx_dma_config(pCfg->pTxDmaHandle, pCfg->pUartHandle);
        drv_uart_register_callback(pCfg->pUartHandle, DRV_UART_DMA_TX_CB_ID, tlkhal_uart_dmaTxDoneCallback);
    }

    if (txDma != 0 || rxDma != 0) { // Rx Tx DMA mode
        plic_interrupt_enable(CPU_DMA_IRQn);
    }
    if (rxPin != 0) { // Rx no DMA mode, Tx DMA mode
        plic_interrupt_enable(irqNum);
        if (rxDma == 0) {
            drv_uart_register_callback(pCfg->pUartHandle, DRV_UART_IT_RX_CB_ID, tlkhal_uart_RxDoneCallback);
        }
    }
    if (txDma == 0 && rxDma == 0) { // Tx Rx no DMA mode
        drv_uart_register_callback(pCfg->pUartHandle, DRV_UART_IT_RX_CB_ID, tlkhal_uart_RxDoneCallback);
        drv_uart_register_callback(pCfg->pUartHandle, DRV_UART_IT_TX_CB_ID, tlkhal_uart_TxDoneCallback);
    }
    tlkapi_printf(1, "tl752x_uart_info, port:%d rxDma:%d txDma:%d txPin:%d rxPin:%d baudRate:%d irqNum:%d", port, rxDma, txDma, txPin, rxPin, baudRate, irqNum);
}

/**
 * @brief  Close UART
 * @param[in] pCfg : Pointer to UART configuration structure
 * @returns  None.
 */
void tlkhal_uart_close(const tlkhal_uart_cfg_t *const pCfg)
{
    uint8_t rxDma = pCfg->rxDma;
    uint8_t txDma = pCfg->txDma;


    drv_uart_deinit(pCfg->pUartHandle);
    uint8_t irqNum = tlkhal_uart_getPortIrqNum(pCfg->pUartHandle);
    plic_interrupt_disable(irqNum);

    if (txDma != 0) {
        drv_dmac_deinit(pCfg->pTxDmaHandle);
    }
    if (rxDma != 0) {
        drv_dmac_deinit(pCfg->pRxDmaHandle);
    }
}

void tlkhal_uart_dmaRxDoneRegister(tlkhal_uart_dmaRxDoneCB cb)
{
    sTlkhalDmaRxDoneCB = cb;
}

void tlkhal_uart_dmaTxDoneRegister(tlkhal_uart_dmaTxDoneCB cb)
{
    sTlkhalDmaTxDoneCB = cb;
}

void tlkhal_uart_noDmaRxDoneRegister(tlkhal_uart_noDmaRxDoneCB cb)
{
    sTlkhalNoDmaRxDoneCB = cb;
}

uint8_t tlkhal_uart_getUartPort(uart_handle_t *huart)
{
    uint8_t port = 0xEE;
    if (huart == NULL) {
        return 0xEE;
    }

    if (huart->instance == UART0) {
        port = 0;
    } else if (huart->instance == UART1) {
        port = 1;
    } else if (huart->instance == UART2) {
        port = 2;
    } else if (huart->instance == UART3) {
        port = 3;
    } else if (huart->instance == UART4) {
        port = 4;
    } else {
        port = 0xFF;
    }
    return port;
}

/**
 * @brief  Get UART port IRQ number
 * @param[in] port : UART port number
 * @returns  UART port IRQ number
*/
static uint8_t tlkhal_uart_getPortIrqNum(uart_handle_t *pUartHandle)
{
    if (pUartHandle == NULL) {
        return 0xEE;
    }

    if (pUartHandle->instance == UART0) {
        return IRQ_UART0; // 18
    } else if (pUartHandle->instance == UART1) {
        return IRQ_UART1; // 19
    } else if (pUartHandle->instance == UART2) {
        return IRQ_UART2; // 45
    } else if (pUartHandle->instance == UART3) {
        return IRQ_UART3; // 46
    } else if (pUartHandle->instance == UART4) {
        return IRQ_UART4; // 59
    }

    return 0xFF;
}

/**
 * @brief  Clear UART RX done status
 * @param[in] port : UART port number
 * @param[in] futureUse : Reserved for future use
 * @returns  None.
 */
__attribute__((always_inline)) inline void tlkhal_uart_clrRxDoneStatus(uint8_t port, void *futureUse)
{
    (void)futureUse;
    (void)port;
}

/**
 * @brief  UART no DMA RX done callback
 * @param[in] port : UART port number
 * @returns  None.
*/
static void tlkhal_uart_RxDoneCallback(uart_handle_t *huart)
{
    uint8_t port = 0;
    if (sTlkhalNoDmaRxDoneCB != NULL) {
        port = tlkhal_uart_getUartPort(huart);
        sTlkhalNoDmaRxDoneCB(port);
    }
}

/**
 * @brief  Clear UART TX done status
 * @param[in] port : UART port number
 * @returns  None.
*/
static void tlkhal_uart_TxDoneCallback(uart_handle_t *huart)
{
    (void)huart;
}

/**
 * @brief  UART DMA TX done callback
 * @param[in] port : UART port number
 * @returns  None.
*/
static void tlkhal_uart_dmaTxDoneCallback(uart_handle_t *huart)
{
    uint8_t port = 0;
    if (sTlkhalDmaTxDoneCB != NULL) {
        port = tlkhal_uart_getUartPort(huart);
        sTlkhalDmaTxDoneCB(port);
    }
}

/**
 * @brief  UART DMA RX done callback
 * @param[in] port : UART port number
 * @returns  None.
*/
static void tlkhal_uart_dmaRxDoneCallback(uart_handle_t *huart)
{
    uint8_t port = 0;
    if (sTlkhalDmaRxDoneCB != NULL) {
        port = tlkhal_uart_getUartPort(huart);
        sTlkhalDmaRxDoneCB(port, 0);
    }
}


#endif
