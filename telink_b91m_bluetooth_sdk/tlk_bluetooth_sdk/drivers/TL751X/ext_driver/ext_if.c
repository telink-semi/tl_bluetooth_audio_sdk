/********************************************************************************************************
 * @file    ext_if.c
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
#include "ext_if.h"
#include "../driver.h"
#include "stack/bt/common/rwip_config.h"
#include "core/mcu_type.h"
#if H4TL_SUPPORT

#include "stack/bt/common/rwip.h"
/**
 * @brief  UART Select interface
 */
#ifndef EXT_HCI_UART_CHANNEL
#define EXT_HCI_UART_CHANNEL UART1
#endif

#ifndef EXT_HCI_UART_IRQ
#define EXT_HCI_UART_IRQ IRQ_UART1
#endif

#ifndef EXT_HCI_UART_DMA_CHN_RX
#define EXT_HCI_UART_DMA_CHN_RX DMA2 //uart dma
#endif

#ifndef EXT_HCI_UART_DMA_CHN_TX
#define EXT_HCI_UART_DMA_CHN_TX DMA3
#endif

/*
 * DEFINES
 *****************************************************************************************
 */

/// Max baudrate supported by this UART (in bps)
#define UART_BAUD_MAX 3500000
/// Min baudrate supported by this UART (in bps)
#define UART_BAUD_MIN 9600

/// Duration of 1 byte transfer over UART (10 bits) in us (for 921600 default baudrate)
#define UART_CHAR_DURATION 11

/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */

///UART Character format
enum UART_CHARFORMAT
{
    UART_CHARFORMAT_8 = 0,
    UART_CHARFORMAT_7 = 1
};

///UART Stop bit
enum UART_STOPBITS
{
    UART_STOPBITS_1 = 0,
    UART_STOPBITS_2 = 1 /* Note: The number of stop bits is 1.5 if a character format
                                 with 5 bit is chosen*/
};

///UART Parity enable
enum UART_PARITY
{
    UART_PARITY_DISABLED = 0,
    UART_PARITY_ENABLED  = 1
};

///UART Parity type
enum UART_PARITYBIT
{
    UART_PARITYBIT_EVEN  = 0,
    UART_PARITYBIT_ODD   = 1,
    UART_PARITYBIT_SPACE = 2, // The parity bit is always 0.
    UART_PARITYBIT_MARK  = 3  // The parity bit is always 1.
};

///UART HW flow control
enum UART_HW_FLOW_CNTL
{
    UART_HW_FLOW_CNTL_DISABLED = 0,
    UART_HW_FLOW_CNTL_ENABLED  = 1
};

///UART Input clock select
enum UART_INPUT_CLK_SEL
{
    UART_INPUT_CLK_SEL_0 = 0,
    UART_INPUT_CLK_SEL_1 = 1,
    UART_INPUT_CLK_SEL_2 = 2,
    UART_INPUT_CLK_SEL_3 = 3
};

///UART Interrupt enable/disable
enum UART_INT
{
    UART_INT_DISABLE = 0,
    UART_INT_ENABLE  = 1
};

///UART Error detection
enum UART_ERROR_DETECT
{
    UART_ERROR_DETECT_DISABLED = 0,
    UART_ERROR_DETECT_ENABLED  = 1
};

/*
 * STRUCT DEFINITIONS
 *****************************************************************************************
 */
/* TX and RX channel class holding data used for asynchronous read and write data
 * transactions
 */
/// UART TX RX Channel
struct uart_txrxchannel
{
    /// call back function pointer
    void (*callback)(void *, uint32_t);
    /// Dummy data pointer returned to callback when operation is over.
    void *dummy;
};

/// UART environment structure
struct uart_env_tag
{
    /// tx channel
    struct uart_txrxchannel tx;
    /// rx channel
    struct uart_txrxchannel rx;
    /// error detect
    uint8_t errordetect;
    /// external wakeup
    bool ext_wakeup;
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
/// uart environment structure
static struct uart_env_tag uart_env;

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
  * @brief extend UART HCI Init Structure definition
  */
typedef struct
{
    unsigned int baudrate; //This member configures the baud rate for UART communication.

    unsigned int tx_Pin; //

    unsigned int rx_Pin; //

    unsigned int HwFlowCtl; //Specifies whether the hardware flow control mode is enabled or disabled

    unsigned int cts_Pin; //uart_parity_e

    unsigned int rts_Pin; //uart_parity_e

    //  CpltCallback  RxCpltCallback;         // UART Tx Complete Callback
    //  CpltCallback  TxCpltCallback;         // UART Rx Complete Callback

} ext_hci_TypeDef;

ext_hci_TypeDef uart = {
    1000000, GPIO_PC6, GPIO_PC7, 0, 0, 0,

};

/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void ext_if_uart_init(void)
{
    unsigned short div;
    unsigned char  bwpc;


    uart_reset(EXT_HCI_UART_CHANNEL);

    uart_set_pin(EXT_HCI_UART_CHANNEL, uart.tx_Pin, uart.rx_Pin);

    uart_cal_div_and_bwpc(uart.baudrate, sys_clk.pclk * 1000 * 1000, &div, &bwpc);

    uart_set_rx_timeout(EXT_HCI_UART_CHANNEL, bwpc, 12, UART_BW_MUL2, 100);

    uart_init(EXT_HCI_UART_CHANNEL, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

    uart_set_tx_dma_config(EXT_HCI_UART_CHANNEL, EXT_HCI_UART_DMA_CHN_TX);

    uart_set_rx_dma_config(EXT_HCI_UART_CHANNEL, EXT_HCI_UART_DMA_CHN_RX);

    uart_set_irq_mask(EXT_HCI_UART_CHANNEL, UART_TXDONE_MASK);

    uart_set_irq_mask(EXT_HCI_UART_CHANNEL, UART_RXDONE_MASK);

    plic_interrupt_enable(EXT_HCI_UART_IRQ);


    //	     //cts function
    //	     if((uart->HwFlowCtl != 0) && (uart->cts_Pin != 0))
    //	     {
    //	        uart_cts_config(EXT_HCI_UART_CHANNEL,uart->cts_Pin,STOP_VOLT);
    //	        uart_set_cts_en(EXT_HCI_UART_CHANNEL);
    //
    //	     }
    //	     //rts function
    //	     if((uart->HwFlowCtl != 0) && (uart->rts_Pin != 0))
    //	     {
    //	            uart_set_rts_en(EXT_HCI_UART_CHANNEL);
    //	            uart_rts_config(EXT_HCI_UART_CHANNEL,uart->rts_Pin,RTS_INVERT,UART_RTS_MODE_AUTO);
    //	            uart_rts_trig_level_auto_mode(EXT_HCI_UART_CHANNEL, RTS_THRESH);
    //	     }
    //	     uart_dma_send_flag = 1;
    //	     TxCpltCallback = uart->TxCpltCallback;
    //	     RxCpltCallback = uart->RxCpltCallback;
    //	     return EXT_UART_OK;
    // Initialize RX and TX transfer callbacks
    uart_env.rx.callback = NULL;
    uart_env.tx.callback = NULL;

#ifndef CFG_ROM
    // Enable RX flow
    uart_flow_on();
#endif //CFG_ROM
}

void uart_flow_on(void)
{
    // Configure modem (HW flow control enable)
    //    uart_force_rts_setf(0);
}

bool uart_flow_off(void)
{
    bool flow_off = true;

    //    GLOBAL_INT_DISABLE();

    //    do
    //    {
    //        // Check if sleep is allowed by Host
    //        if(uart_rx_fifo_not_empty_getf() || (uart_rx_dma_started_getf())
    //                || (uart_env.ext_wakeup && (uart_ext_wakeup_getf())))
    //        {
    //            flow_off = false;
    //            break;
    //        }
    //
    //        // First check if no transmission is ongoing
    //        if (uart_tx_dma_started_getf() || !uart_tx_fifo_empty_getf() || uart_tx_fifo_empty_isr_getf())
    //        {
    //            flow_off = false;
    //            break;
    //        }
    //
    //        // Force RTS to 'flow off' via GPIO
    //        uart_force_rts_setf(1);
    //
    ////        // Wait for 1 character duration to ensure host has not started a transmission at the
    ////        // same time
    ////        uart_cnt_start_setf(1);
    ////        while(uart_cnt_end_getf() != 1);
    //
    //        // Check if data has been received during wait time
    //        if(uart_rx_fifo_not_empty_getf() || (uart_rx_dma_started_getf())
    //                || (uart_env.ext_wakeup && (uart_ext_wakeup_getf())))
    //        {
    //            // Re-enable UART flow
    //            uart_flow_on();
    //
    //            flow_off = false;
    //        }
    //    } while(0);

    //    GLOBAL_INT_RESTORE();
    return flow_off;
}

void uart_finish_transfers(void)
{
    //    uart_force_rts_setf(1);

    // Wait TX FIFO empty
    //    while(!uart_tx_fifo_empty_getf());
}

void uart_read(uint8_t *bufptr, uint32_t size, void (*callback)(void *, uint32_t), void *dummy)
{
    // Sanity check
    ASSERT_ERR(bufptr != NULL);
    ASSERT_ERR(size != 0);
    ASSERT_ERR(callback != NULL);
    uart_env.rx.callback = callback;
    uart_env.rx.dummy    = dummy;
    core_interrupt_disable();
    uart_receive_dma(EXT_HCI_UART_CHANNEL, bufptr, size);
    core_interrupt_enable();
    //    uart_rx_ptr_setf((uint32_t) bufptr);
    //    uart_rx_size_setf(size);
    //    uart_rx_start_setf(1);
}

void uart_write(uint8_t *bufptr, uint32_t size, void (*callback)(void *, uint32_t), void *dummy)
{
    // Sanity check
    ASSERT_ERR(bufptr != NULL);
    ASSERT_ERR(size != 0);
    ASSERT_ERR(callback != NULL);
    uart_env.tx.callback = callback;
    uart_env.tx.dummy    = dummy;
    //    uart_tx_ptr_setf((uint32_t) bufptr);
    //    uart_tx_size_setf(size);
    //    uart_tx_start_setf(1);
    core_interrupt_disable();
    uart_send_dma(EXT_HCI_UART_CHANNEL, bufptr, size);
    core_interrupt_enable();
}

void uart_isr(void)
{
    void (*callback)(void *, uint32_t) = NULL;
    void *data                         = NULL;

    if (uart_get_irq_status(EXT_HCI_UART_CHANNEL, UART_TXDONE_IRQ_STATUS)) {
        uart_clr_irq_status(EXT_HCI_UART_CHANNEL, UART_TXDONE_IRQ_STATUS);

        // Retrieve callback pointer
        callback = uart_env.tx.callback;
        data     = uart_env.tx.dummy;

        if (callback != NULL) {
            // Clear callback pointer
            uart_env.tx.callback = NULL;
            uart_env.tx.dummy    = NULL;
            // Call handler
            callback(data, 0);
        } else {
            ASSERT_ERR(0);
        }
    }


    //receive
    if (uart_get_irq_status(EXT_HCI_UART_CHANNEL, UART_RXDONE_IRQ_STATUS)) {
        if ((uart_get_irq_status(EXT_HCI_UART_CHANNEL, UART_RX_ERR))) {
            uart_clr_irq_status(EXT_HCI_UART_CHANNEL, UART_RXBUF_IRQ_STATUS);
        }
        /************************get the length of receive data****************************/
        //        unsigned int rev_data_len = uart_get_dma_rev_data_len(EXT_HCI_UART_CHANNEL,EXT_HCI_UART_DMA_CHN_RX);
        /************************clr rx_irq****************************/
        uart_clr_irq_status(EXT_HCI_UART_CHANNEL, UART_RXDONE_IRQ_STATUS);
#if (MCU_CORE_TYPE == MCU_CORE_B91 || MCU_CORE_TYPE == MCU_CORE_B92)
        unsigned int rev_data_len = uart_get_dma_rev_data_len(EXT_HCI_UART_CHANNEL, EXT_HCI_UART_DMA_CHN_RX);
#else //TL751X driver del follow interface,use our private function
        unsigned int rev_data_len = 0;
#endif

        // Retrieve callback pointer
        callback = uart_env.rx.callback;
        data     = uart_env.rx.dummy;

        if (callback != NULL) {
            // Clear callback pointer
            uart_env.rx.callback = NULL;
            uart_env.rx.dummy    = NULL;

            // Call handler
            callback(data, rev_data_len);
        } else {
            ASSERT_ERR(0);
        }
    }


    //    while (uart_isr_stat_get())
    //    {
    //        void (*callback) (void*, uint8_t) = NULL;
    //        void* data =NULL;
    //
    //        if(uart_tx_fifo_empty_isr_getf())
    //        {
    //            uart_tx_fifo_empty_clr_setf(1);
    //        }
    //
    //        if(uart_rx_fifo_not_empty_isr_getf())
    //        {
    //            uart_rx_fifo_not_empty_clr_setf(1);
    //        }
    //
    //        if(uart_rx_dma_done_getf())
    //        {
    //            uart_rx_dma_done_clr_setf(1);
    //
    //            // Retrieve callback pointer
    //            callback = uart_env.rx.callback;
    //            data     = uart_env.rx.dummy;
    //
    //            if(callback != NULL)
    //            {
    //                // Clear callback pointer
    //                uart_env.rx.callback = NULL;
    //                uart_env.rx.dummy    = NULL;
    //
    //                // Call handler
    //                callback(data, RWIP_EIF_STATUS_OK);
    //            }
    //            else
    //            {
    //                ASSERT_ERR(0);
    //            }
    //        }
    //
    //        if(uart_tx_dma_done_getf())
    //        {
    //            uart_tx_dma_done_clr_setf(1);
    //
    //            // Retrieve callback pointer
    //            callback = uart_env.tx.callback;
    //            data     = uart_env.tx.dummy;
    //
    //            if(callback != NULL)
    //            {
    //                // Clear callback pointer
    //                uart_env.tx.callback = NULL;
    //                uart_env.tx.dummy    = NULL;
    //                // Call handler
    //                callback(data, RWIP_EIF_STATUS_OK);
    //            }
    //            else
    //            {
    //                ASSERT_ERR(0);
    //            }
    //        }
    //    }
}
/**
 * @brief		UART1 interrupt handler.
 * @param[in]	none
 * @return      none
 */
PLIC_ISR_REGISTER(uart_isr, EXT_HCI_UART_IRQ)
#endif
