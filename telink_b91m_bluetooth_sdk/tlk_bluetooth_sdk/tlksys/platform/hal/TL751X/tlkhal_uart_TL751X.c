/********************************************************************************************************
 * @file    tlkhal_uart_TL751X.c
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
#include "drivers.h"
#if MCU_CORE_TYPE == CHIP_TYPE_TL751X
/**
 * @brief  Get UART port IRQ number
 * @param[in] port : UART port number
 * @returns  UART port IRQ number
*/
static inline uint8_t tlkhal_uart_getPortIrqNum(uint8_t port)
{
    TLKHAL_ASSERT(port <= UART3);
    switch (port) {
    case UART0:
        return IRQ_UART0;
    case UART1:
        return IRQ_UART1;
    case UART2:
        return IRQ_UART2;
    case UART3:
        return IRQ_UART3;
    }
    return IRQ_UART0;
}

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

    TLKHAL_ASSERT(port <= UART3);
    TLKHAL_ASSERT(rxDma <= DMA15);
    TLKHAL_ASSERT(txDma <= DMA15);
    TLKHAL_ASSERT(baudRate <= 2 * 1000 * 1000);

    uint8_t irqNum = tlkhal_uart_getPortIrqNum(port);
    plic_interrupt_disable(irqNum);

    uart_hw_fsm_reset(port);
    if (txPin != rxPin) {
        uart_set_pin(port, txPin, rxPin);
    } else {
        uart_set_rtx_pin(port, rxPin);
        gpio_set_up_down_res(rxPin, GPIO_PIN_PULLUP_1M);
        uart_rtx_en(port);
    }

    uint16_t div  = 0;
    uint8_t  bwpc = 0;
    uart_cal_div_and_bwpc(baudRate, sys_clk.pclk * 1000 * 1000, &div, &bwpc);

    uart_set_rx_timeout_with_exp(port, bwpc, 12, UART_BW_MUL2, 0);

    uart_init(port, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);


    uart_clr_irq_status(port, UART_TXDONE_IRQ_STATUS);
    uart_clr_irq_status(port, UART_RXDONE_IRQ_STATUS);
    uart_clr_irq_status(port, UART_RXBUF_IRQ_STATUS);
    uart_clr_irq_status(port, UART_TXBUF_IRQ_STATUS);
    uart_set_irq_mask(port, UART_ERR_IRQ_MASK);

    if (rxDma == 0) {
        uart_set_irq_mask(port, UART_RX_IRQ_MASK);
    } else {
        uart_set_rx_dma_config(port, rxDma);
        dma_clr_irq_mask(rxDma, TC_MASK | ABT_MASK | ERR_MASK);
        uart_set_irq_mask(port, UART_RXDONE_MASK);
        dma_set_irq_mask(rxDma, TC_MASK);

        if (rxDma <= DMA7) {
            plic_interrupt_enable(IRQ_DMA);
        } else {
            plic_interrupt_enable(IRQ_DMA1);
        }
    }

    if (txDma != 0) {
        uart_set_tx_dma_config(port, txDma);
        dma_clr_irq_mask(txDma, TC_MASK | ABT_MASK | ERR_MASK);
        uart_set_irq_mask(port, UART_TXDONE_MASK);
    }
    uart_set_irq_mask(port, UART_TXDONE_MASK);

    if (txDma != 0 || rxDma != 0) {
        plic_interrupt_enable(irqNum);
    } else if (rxPin != 0) {
        plic_interrupt_enable(irqNum);
    }
}

/**
 * @brief  Close UART
 * @param[in] pCfg : Pointer to UART configuration structure
 * @returns  None.
 */
void tlkhal_uart_close(const tlkhal_uart_cfg_t *const pCfg)
{
    uint8_t port  = pCfg->port;
    uint8_t rxDma = pCfg->rxDma;
    uint8_t txDma = pCfg->txDma;

    TLKHAL_ASSERT(port <= UART3);
    TLKHAL_ASSERT(rxDma <= DMA15);
    TLKHAL_ASSERT(txDma <= DMA15);

    uart_reset(port);
    uint8_t irqNum = tlkhal_uart_getPortIrqNum(port);
    plic_interrupt_disable(irqNum);

    if (txDma != 0) {
        dma_chn_dis(txDma);
        dma_clr_irq_mask(txDma, TC_MASK | ABT_MASK | ERR_MASK);
    }
    if (rxDma != 0) {
        dma_chn_dis(rxDma);
        dma_clr_irq_mask(rxDma, TC_MASK | ABT_MASK | ERR_MASK);
    }
    uart_clr_irq_status(port, UART_TXDONE_IRQ_STATUS);
    uart_clr_irq_status(port, UART_RXDONE_IRQ_STATUS);
    uart_clr_irq_status(port, UART_RXBUF_IRQ_STATUS);
    uart_clr_irq_status(port, UART_TXBUF_IRQ_STATUS);
}

/**
 * @brief  Clear UART TX done status
 * @param[in] port : UART port number
 * @param[in] futureUse : Reserved for future use
 * @returns  None.
 */
__attribute__((always_inline)) inline void tlkhal_uart_clrTxDoneStatus(uint8_t port, void *futureUse)
{
    (void)futureUse;
    TLKHAL_ASSERT(port <= UART3);
    uart_clr_irq_status(port, UART_TXDONE_IRQ_STATUS);
}

/**
 * @brief  Get UART TX done status
 * @param[in] port : UART port number
 * @param[in] futureUse : Reserved for future use
 * @returns  TX done status.
 */
__attribute__((always_inline)) inline uint32_t tlkhal_uart_getTxDoneStatus(uint8_t port, void *futureUse)
{
    (void)futureUse;
    TLKHAL_ASSERT(port <= UART3);
    return uart_get_irq_status(port, UART_TXDONE_IRQ_STATUS);
}

/**
 * @brief  Get UART RX done status
 * @param[in] port : UART port number
 * @param[in] futureUse : Reserved for future use
 * @returns  RX done status.
 */
__attribute__((always_inline)) inline uint32_t tlkhal_uart_getRxDoneStatus(uint8_t port, void *futureUse)
{
    (void)futureUse;
    TLKHAL_ASSERT(port <= UART3);
    return uart_get_irq_status(port, UART_RXDONE_IRQ_STATUS);
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
    TLKHAL_ASSERT(port <= UART3);
    uart_clr_irq_status(port, UART_RXDONE_IRQ_STATUS);
}
#endif