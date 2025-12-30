/********************************************************************************************************
 * @file    tlkapp_irq.c
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
#include "stack/btble.h"
#include "tlkmw/tlkmw.h"
#include "tlklib/usb/tlkusb.h"


#define TLKAPP_IRQ_CONTROLLER_EN (!MCU_DUAL_CORE_ENABLE || MCU_CORE_N22)

#if TLK_CFG_RTOS_ENABLE
extern void entry_irq_all(func_isr_t func, unsigned int src);
#define PLIC_ISR_REGISTER_OS(isr, irq_num)                                                                                                                 \
    __attribute__((section(".ram_code"))) __attribute__((noinline)) __attribute__((naked)) void ISR_ENTRY_NAME(irq_num)(void) __attribute__((aligned(4))); \
    void                                                                                        ISR_ENTRY_NAME(irq_num)(void)                              \
    {                                                                                                                                                      \
        __asm__("c.addi sp,-16");                                                                                                                          \
        __asm__("c.swsp ra,0(sp)");                                                                                                                        \
        __asm__("c.swsp a0,4(sp)");                                                                                                                        \
        __asm__("c.swsp a1,8(sp)");                                                                                                                        \
        entry_irq_all(isr, irq_num);                                                                                                                       \
        __asm__("c.lwsp ra,0(sp)");                                                                                                                        \
        __asm__("c.lwsp a0,4(sp)");                                                                                                                        \
        __asm__("c.lwsp a1,8(sp)");                                                                                                                        \
        __asm__("c.addi sp,16");                                                                                                                           \
        __asm__("mret");                                                                                                                                   \
    }
#else
#define PLIC_ISR_REGISTER_OS(isr, irq_num) PLIC_ISR_REGISTER(isr, irq_num)

struct TRAP_VAL
{
    uint32_t mcause;
    uint32_t mtval;
    uint32_t mepc;
    uint32_t mstatus;
    uint32_t mdcause;
    uint32_t ra;
};

volatile struct TRAP_VAL trap_value;
volatile uint32_t        AAAA_trap = 0;

static inline void tlkapp_trap_handler(uint32_t mtval, uint32_t mepc, uint32_t mstatus, uint32_t mcause, uint32_t mdcause, uint32_t ra)
{
    trap_value.mcause  = mcause;
    trap_value.mtval   = mtval;
    trap_value.mepc    = mepc;
    trap_value.mstatus = mstatus;
    trap_value.mdcause = mdcause;
    trap_value.ra      = ra;

    AAAA_trap++;
    tlkapi_printf(1, "*** app_trap_handler *** %d %d %x %d", trap_value.mcause, trap_value.mtval, trap_value.mepc, trap_value.ra);
    tlkapi_printf(1, "*** error code mark *** FFFFFFFF");

    core_interrupt_disable();
    while (1) {
        tlk_udb_usb_handle_irq();
    }
}

_attribute_ram_code_sec_noinline_ void trap_entry(void) __attribute__((interrupt("machine"), aligned(64)));

_attribute_ram_code_sec_noinline_ void trap_entry(void)
{
    register uint32_t ra asm("x1");

    uint32_t mtval, mepc, mstatus, mcause, mdcause;
    mtval   = read_csr(NDS_MTVAL);
    mepc    = read_csr(NDS_MEPC);
    mstatus = read_csr(NDS_MSTATUS);
    mcause  = read_csr(NDS_MCAUSE);
    mdcause = read_csr(NDS_MDCAUSE);
    tlkapp_trap_handler(mtval, mepc, mstatus, mcause, mdcause, ra);
}

#endif

/*************************************************************************************************
 *       ██████╗ ██████╗ ███╗   ██╗████████╗██████╗  ██████╗ ██╗     ██╗     ███████╗██████╗ 
 *      ██╔════╝██╔═══██╗████╗  ██║╚══██╔══╝██╔══██╗██╔═══██╗██║     ██║     ██╔════╝██╔══██╗
 *      ██║     ██║   ██║██╔██╗ ██║   ██║   ██████╔╝██║   ██║██║     ██║     █████╗  ██████╔╝
 *      ██║     ██║   ██║██║╚██╗██║   ██║   ██╔══██╗██║   ██║██║     ██║     ██╔══╝  ██╔══██╗
 *      ╚██████╗╚██████╔╝██║ ╚████║   ██║   ██║  ██║╚██████╔╝███████╗███████╗███████╗██║  ██║
 *       ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝
 *************************************************************************************************/

#if (TLKAPP_IRQ_CONTROLLER_EN && !MCU_CORE_TL752X_TEMP)
/**
 * @brief		System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_retention_code_ void stimer_irq_handler(void)
{
    tlksdk_irq_handler(IRQ_SYSTIMER);
}
PLIC_ISR_REGISTER_OS(stimer_irq_handler, IRQ_SYSTIMER)

/**
 * @brief		BLE RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_retention_code_ void ble_rf_irq_handler(void)
{
    tlksdk_irq_handler(IRQ_ZB_RT);
}
PLIC_ISR_REGISTER_OS(ble_rf_irq_handler, IRQ_ZB_RT)

#if (MCU_CORE_TYPE != MCU_CORE_TL721X && MCU_CORE_TYPE != MCU_CORE_TL322X)
/**
 * @brief		BT RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_retention_code_ void bt_rf_irq_handler(void)
{
    tlksdk_irq_handler(IRQ_ZB_BT);
}
PLIC_ISR_REGISTER_OS(bt_rf_irq_handler, IRQ_ZB_BT)
#endif

#endif

/******************************************************************************
 *     ████████╗██╗███╗   ███╗███████╗██████╗ 
 *     ╚══██╔══╝██║████╗ ████║██╔════╝██╔══██╗
 *        ██║   ██║██╔████╔██║█████╗  ██████╔╝
 *        ██║   ██║██║╚██╔╝██║██╔══╝  ██╔══██╗
 *        ██║   ██║██║ ╚═╝ ██║███████╗██║  ██║
 *        ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝ 
 ******************************************************************************/

/**
 * @brief		Timer0 irq handler for audio.
 * @param[in]	none
 * @return      none
 */
__attribute__((weak)) _attribute_ram_code_sec_ void tlk_timer0_irq_handler(void)
{
    tlkmdi_audio_timer_irq_handler();
}
#if (MCU_CORE_TL752X_TEMP)
PLIC_ISR_REGISTER_OS(tlk_timer0_irq_handler, IRQ_TIMER0_0)
#else
PLIC_ISR_REGISTER_OS(tlk_timer0_irq_handler, IRQ_TIMER0)
#endif
/**
* @brief		Timer1 irq handler for os timer in baremetal.
* @param[in]	none
* @return       none
*/
__attribute__((weak)) void tlk_timer1_irq_handler(void)
{
    tlkos_timer_irq_handler();
}
#if (MCU_CORE_TL752X_TEMP)
PLIC_ISR_REGISTER_OS(tlk_timer1_irq_handler, IRQ_TIMER0_1)
#else
PLIC_ISR_REGISTER_OS(tlk_timer1_irq_handler, IRQ_TIMER1)
#endif

/******************************************************************************
 *     ███╗   ███╗ █████╗ ██╗██╗     ██████╗  ██████╗ ██╗  ██╗
 *     ████╗ ████║██╔══██╗██║██║     ██╔══██╗██╔═══██╗╚██╗██╔╝
 *     ██╔████╔██║███████║██║██║     ██████╔╝██║   ██║ ╚███╔╝ 
 *     ██║╚██╔╝██║██╔══██║██║██║     ██╔══██╗██║   ██║ ██╔██╗ 
 *     ██║ ╚═╝ ██║██║  ██║██║███████╗██████╔╝╚██████╔╝██╔╝ ██╗
 *     ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚══════╝╚═════╝  ╚═════╝ ╚═╝  ╚═╝
 ******************************************************************************/

/**
* @brief       Mailbox irq in main core from controller core.
* @param[in]   none
* @return      none
*/
#if MCU_DUAL_CORE_ENABLE && !defined(MCU_CORE_N22)
void tlk_main_core_ipc_irq_handler(void);

_attribute_ram_code_sec_ void tlk_main_core_mailbox_irq_handler(void)
{
    tlk_main_core_ipc_irq_handler();
#if TLK_MW_DSP_COMM_ENABLE && MCU_CORE_TL752X_TEMP
    d25f_ipc_message_irq_handler();
#endif
}
#if (!MCU_CORE_TL752X_TEMP)
PLIC_ISR_REGISTER_OS(tlk_main_core_mailbox_irq_handler, IRQ_MAILBOX_N22_TO_D25)
#else
PLIC_ISR_REGISTER_OS(tlk_main_core_mailbox_irq_handler, IRQ_IPCM)
#endif
#endif

/**
* @brief       Mailbox irq in main core from dsp core.
* @param[in]   none
* @return      none
*/
#if TLK_MW_DSP_COMM_ENABLE && !MCU_CORE_TL752X_TEMP
void d25f_ipc_message_irq_handler(void);
PLIC_ISR_REGISTER_OS(d25f_ipc_message_irq_handler, IRQ_MAILBOX_DSP_TO_D25)
#endif

/******************************************************************************
 *     ██╗   ██╗ █████╗ ██████╗ ████████╗
 *     ██║   ██║██╔══██╗██╔══██╗╚══██╔══╝
 *     ██║   ██║███████║██████╔╝   ██║   
 *     ██║   ██║██╔══██║██╔══██╗   ██║   
 *     ╚██████╔╝██║  ██║██║  ██║   ██║   
 *      ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   
 ******************************************************************************/

/**
 * @brief		Uart0 interrupt handler.
 * @param[in]	none
 * @return      none
 */
__attribute__((weak)) _attribute_ram_code_sec_ void tlk_uart0_irq_handler(void)
{
#if TLK_DEV_SERIAL_ENABLE
    tlkdrv_uart_irqHandler(0);
#endif
}
PLIC_ISR_REGISTER_OS(tlk_uart0_irq_handler, IRQ_UART0)

/**
 * @brief		Uart1 interrupt handler.
 * @param[in]	none
 * @return      none
 */
__attribute__((weak)) _attribute_ram_code_sec_ void tlk_uart1_irq_handler(void)
{
#if TLK_DEV_SERIAL_ENABLE
    tlkdrv_uart_irqHandler(1);
#endif
}
PLIC_ISR_REGISTER_OS(tlk_uart1_irq_handler, IRQ_UART1)


/******************************************************************************
 *      ██╗   ██╗███████╗██████╗ 
 *      ██║   ██║██╔════╝██╔══██╗
 *      ██║   ██║███████╗██████╔╝
 *      ██║   ██║╚════██║██╔══██╗
 *      ╚██████╔╝███████║██████╔╝
 *       ╚═════╝ ╚══════╝╚═════╝ 
 ******************************************************************************/

/**
 * @brief       Usb ctrl ep interrupt handler.
 * @param[in]   none
 * @return      none
 */
#if MCU_CORE_TYPE != MCU_CORE_TL322X && MCU_CORE_TYPE != MCU_CORE_TL752X
__attribute__((weak)) _attribute_ram_code_sec_ void tlk_usb_ctrl_ep_irq_handler(void)
{
#if (TLK_CFG_USB_ENABLE)
    tlkusb_ctrl_ep_irq_handler(0);
#endif
}
PLIC_ISR_REGISTER_OS(tlk_usb_ctrl_ep_irq_handler, IRQ_USB_CTRL_EP_SETUP)
PLIC_ISR_REGISTER_OS(tlk_usb_ctrl_ep_irq_handler, IRQ_USB_CTRL_EP_DATA)
PLIC_ISR_REGISTER_OS(tlk_usb_ctrl_ep_irq_handler, IRQ_USB_CTRL_EP_STATUS)
PLIC_ISR_REGISTER_OS(tlk_usb_ctrl_ep_irq_handler, IRQ_USB_RESET)
PLIC_ISR_REGISTER_OS(tlk_usb_ctrl_ep_irq_handler, IRQ_USB_CTRL_EP_SETINF)

/**
 * @brief       Usb ep interrupt handler.
 * @param[in]   none
 * @return      none
 */
__attribute__((weak)) _attribute_ram_code_sec_ void tlk_usb_ep_irq_handler(void)
{
#if (TLK_CFG_USB_ENABLE)
    tlkusb_irqHandler(0);
#endif
}
PLIC_ISR_REGISTER_OS(tlk_usb_ep_irq_handler, IRQ_USB_ENDPOINT)
#endif

#if (TLK_CFG_USB_NUMB > 1)
/**
 * @brief       Usb1 ctrl ep interrupt handler.
 * @param[in]   none
 * @return      none
 */
__attribute__((weak)) _attribute_ram_code_sec_ void tlk_usb1_ctrl_ep_irq_handler(void)
{
#if (TLK_CFG_USB_ENABLE)
    tlkusb_ctrl_ep_irq_handler(1);
#endif
}
#if MCU_CORE_TYPE != MCU_CORE_TL752X
PLIC_ISR_REGISTER_OS(tlk_usb1_ctrl_ep_irq_handler, IRQ_USB1_CTRL_EP_SETUP)
PLIC_ISR_REGISTER_OS(tlk_usb1_ctrl_ep_irq_handler, IRQ_USB1_CTRL_EP_DATA)
PLIC_ISR_REGISTER_OS(tlk_usb1_ctrl_ep_irq_handler, IRQ_USB1_CTRL_EP_STATUS)
PLIC_ISR_REGISTER_OS(tlk_usb1_ctrl_ep_irq_handler, IRQ_USB1_RESET)
PLIC_ISR_REGISTER_OS(tlk_usb1_ctrl_ep_irq_handler, IRQ_USB1_CTRL_EP_SETINF)

#else
PLIC_ISR_REGISTER_OS(tlk_usb1_ctrl_ep_irq_handler, IRQ_USB1_EP0_SETUP)
PLIC_ISR_REGISTER_OS(tlk_usb1_ctrl_ep_irq_handler, IRQ_USB1_EP0_DATA)
PLIC_ISR_REGISTER_OS(tlk_usb1_ctrl_ep_irq_handler, IRQ_USB1_EP0_STATUS)
PLIC_ISR_REGISTER_OS(tlk_usb1_ctrl_ep_irq_handler, IRQ_USB1_RESET_250US)
PLIC_ISR_REGISTER_OS(tlk_usb1_ctrl_ep_irq_handler, IRQ_USB1_EP0_SETINF_SETADDR)

#endif

/**
 * @brief       Usb1 ep interrupt handler.
 * @param[in]   none
 * @return      none
 */
__attribute__((weak)) _attribute_ram_code_sec_ void tlk_usb1_ep_irq_handler(void)
{
#if (TLK_CFG_USB_ENABLE)
    tlkusb_irqHandler(1);
#endif
}
#if MCU_CORE_TYPE != MCU_CORE_TL752X
PLIC_ISR_REGISTER_OS(tlk_usb1_ep_irq_handler, IRQ_USB1_ENDPOINT)

#else
PLIC_ISR_REGISTER_OS(tlk_usb1_ep_irq_handler, IRQ_USB1_EPN_DATA)
#endif

#if MCU_CORE_TYPE == MCU_CORE_TL322X
void usb0_irq_handler(void)
{
    tlkhal_usb_irq_handler(0);
}
PLIC_ISR_REGISTER_OS(usb0_irq_handler, IRQ_USB0)
#endif
#endif

/******************************************************************************
 *      █████╗ ██╗   ██╗██████╗ ██╗ ██████╗ 
 *     ██╔══██╗██║   ██║██╔══██╗██║██╔═══██╗
 *     ███████║██║   ██║██║  ██║██║██║   ██║
 *     ██╔══██║██║   ██║██║  ██║██║██║   ██║
 *     ██║  ██║╚██████╔╝██████╔╝██║╚██████╔╝
 *     ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚═╝ ╚═════╝ 
 ******************************************************************************/

/**
 * @brief       Audio fifo interrupt handler.
 * @param[in]   none
 * @return      none
 */
__attribute__((weak)) _attribute_ram_code_sec_ void tlk_audio_fifo_irq_handler(void)
{
#if (TLKMW_FIFO_IRQ_EN)
    tlkmdi_audio_fifo_irq_handler();
#endif
}
#if (!MCU_CORE_TL752X_TEMP)
PLIC_ISR_REGISTER_OS(tlk_audio_fifo_irq_handler, IRQ_DFIFO)
#endif

/******************************************************************************
 *      ██████╗ ██████╗ ██╗  ██████╗
 *     ██╔════╝ ██╔══██╗██║ ██╔═══██╗
 *     ██║  ███╗██████╔╝██║ ██║   ██║
 *     ██║   ██║██╔═══╝ ██║ ██║   ██║
 *     ╚██████╔╝██║     ██║ ╚██████╔╝
 *      ╚═════╝ ╚═╝     ╚═╝  ╚═════╝ 
 ******************************************************************************/

/**
 * @brief		Gpio interrupt handler.
 * @param[in]	none
 * @return      none
 */
#if MCU_CORE_TYPE == MCU_CORE_TL751X

#define GPIO_IRQ_HANDLER(N)                                    \
    __attribute__((weak)) void tlk_gpio_irq##N##_handler(void) \
    {                                                          \
        tlkhal_gpio_irqHandler(N);                             \
    }                                                          \
    PLIC_ISR_REGISTER_OS(tlk_gpio_irq##N##_handler, IRQ_GPIO_IRQ##N)

GPIO_IRQ_HANDLER(0)
GPIO_IRQ_HANDLER(1)
GPIO_IRQ_HANDLER(2)
GPIO_IRQ_HANDLER(3)
GPIO_IRQ_HANDLER(4)
GPIO_IRQ_HANDLER(5)
GPIO_IRQ_HANDLER(6)
GPIO_IRQ_HANDLER(7)

#elif MCU_CORE_TYPE == MCU_CORE_TL721X

__attribute__((weak)) void tlk_gpio_irq_handler(void)
{
    tlkhal_gpio_irqHandler(0xFF);
}
PLIC_ISR_REGISTER_OS(tlk_gpio_irq_handler, IRQ_GPIO)
#elif MCU_CORE_TL752X_TEMP

__attribute__((weak)) void tlk_gpio_irq_handler(void)
{
    tlkhal_gpio_irqHandler(0xFF);
}
PLIC_ISR_REGISTER_OS(tlk_gpio_irq_handler, GPIO_IRQn)
#endif

/******************************************************************************
 *      ███████╗██████╗ ██╗
 *      ██╔════╝██╔══██╗██║
 *      ███████╗██████╔╝██║
 *      ╚════██║██╔═══╝ ██║
 *      ███████║██║     ██║
 *      ╚══════╝╚═╝     ╚═╝
 ******************************************************************************/

/**
* @brief		Lspi irq handler.
* @param[in]	none
* @return       none
*/
#if MCU_CORE_TYPE != MCU_CORE_B91
__attribute__((weak)) void tlk_lspi_irq_handler(void)
{
#if TLK_DRV_SPI_ENABLE
    extern void lspi_irq_handler(void);
    lspi_irq_handler();
#endif
}
#if (!MCU_CORE_TL752X_TEMP)
PLIC_ISR_REGISTER_OS(tlk_lspi_irq_handler, IRQ_LSPI);
#endif
#endif

#if TLK_CFG_FS_ENABLE
#if MCU_CORE_TYPE == MCU_CORE_TL751X
void sdio_irq_handler(void);
PLIC_ISR_REGISTER_OS(sdio_irq_handler, IRQ_SDIO)

#elif MCU_CORE_TYPE == MCU_CORE_TL721X
void gspi_irq_handler(void);
PLIC_ISR_REGISTER_OS(gspi_irq_handler, IRQ_GSPI)
#endif
#endif

/******************************************************************************
 *      ██████╗ ███╗   ███╗ █████╗ 
 *      ██╔══██╗████╗ ████║██╔══██╗
 *      ██║  ██║██╔████╔██║███████║
 *      ██║  ██║██║╚██╔╝██║██╔══██║
 *      ██████╔╝██║ ╚═╝ ██║██║  ██║
 *      ╚═════╝ ╚═╝     ╚═╝╚═╝  ╚═╝
 ******************************************************************************/

/**
 * @brief		DMA interrupt handler.
 * @param[in]	none
 * @return      none
 */
__attribute__((weak)) _attribute_ram_code_sec_ void tlk_dma_irq_handler(void)
{
#if TLK_DEV_SERIAL_ENABLE
    tlkdrv_serial_dmaIrqHandler();
#endif
#if TLK_DRV_SPI_ENABLE
    tlkdrv_spi_dmaIrqHandler();
#endif
}
#if (!MCU_CORE_TL752X_TEMP)
PLIC_ISR_REGISTER_OS(tlk_dma_irq_handler, IRQ_DMA)
#else
PLIC_ISR_REGISTER_OS(tlk_dma_irq_handler, IRQ_CPU_DMA)
#endif

#if (MCU_CORE_TL752X_TEMP)
void CPU2_DMA_IRQHandler(void)
{
#if TLKDRV_CODEC_ICODEC_ENABLE
    drv_dmac_irq_handle(&audio_adc_dmac_handle);
    drv_dmac_irq_handle(&audio_dac_dmac_handle);
#endif
}
PLIC_ISR_REGISTER_OS(CPU2_DMA_IRQHandler, IRQ_CPU2_DMA)
#endif
