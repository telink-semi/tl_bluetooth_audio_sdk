/********************************************************************************************************
 * @file    ext_uart.c
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
#include "ext_uart.h"

#include "../driver.h"

#include "common/config/user_config.h"

#include "core/mcu_type.h"


int lp_uart_init = 0; //attention: can not be retention data !!!

void uart_debug_init(void)
{
    unsigned short div;
    unsigned char  bwpc;

    uart_hw_fsm_reset(DEBUG_UART_CHANNEL);
    uart_set_pin(DEBUG_UART_CHANNEL, TLKAPI_DEBUG_UART_TX_PIN, GPIO_NONE_PIN);

    uart_cal_div_and_bwpc(TLKAPI_DEBUG_UART_BAUDRATE, sys_clk.pclk * 1000 * 1000, &div, &bwpc);
    uart_set_rx_timeout_with_exp(DEBUG_UART_CHANNEL, bwpc, 12, UART_BW_MUL2, 0);
    uart_init(DEBUG_UART_CHANNEL, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
    uart_set_tx_dma_config(DEBUG_UART_CHANNEL, TLKAPI_DEBUG_UART_TX_DMA);
    //uart_set_rx_dma_config(DEBUG_UART_CHANNEL, UART_DMA_CHANNEL_RX);
    uart_set_irq_mask(DEBUG_UART_CHANNEL, UART_TXDONE_MASK);

#ifdef MCU_CORE_N22
    clic_interrupt_vector_en(DEBUG_UART_CHANNEL_IRQ);
    clic_interrupt_enable(DEBUG_UART_CHANNEL_IRQ);
#else
    plic_interrupt_enable(DEBUG_UART_CHANNEL_IRQ);
#endif

    //core_interrupt_enable();

    lp_uart_init = 1;
}

_attribute_ram_code_ void uart_debug_prepare_dma_data(unsigned char *addr, unsigned int len)
{
    uart_send_dma(DEBUG_UART_CHANNEL, addr, len);
}
