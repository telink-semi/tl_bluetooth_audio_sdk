/********************************************************************************************************
 * @file    tlkusb_hal.c
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
#include "tlkapi/tlkapi.h"
#include "drivers.h"
#include "tlklib/usb/tlkusb_stdio.h"
#if (TLK_CFG_USB_ENABLE)

typedef struct
{
    uint8_t ctrlEpIrq[2];
    uint8_t epsIrq[2];
} tlkusb_reg_t;

static uint8_t      sUsbUseEventMode = 0;
static tlkusb_reg_t sTlkUsbReg;

/**
 * @brief     This function servers to init usb core.
 * @returns    none.
 */
void tlkusb_hal_core_init(uint8_t index)
{
    if (index == 0) {
#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
        usbhw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC | FLD_CTRL_EP_AUTO_CFG | FLD_CTRL_EP_AUTO_INTF);
        usbhw_set_printer_threshold(1);
        usbhw_set_ep8_fifo_mode();
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL753X)
        usbhw_enable_hw_feature(FLD_USB_AUTO_HALT_CLR | FLD_USB_AUTO_HALT_STALL);
        usbhw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC | FLD_CTRL_EP_AUTO_CFG | FLD_CTRL_EP_AUTO_INTF);
#elif (MCU_CORE_TYPE == MCU_CORE_TL322X)

#elif (MCU_CORE_TYPE == MCU_CORE_TL752X)

#else
        reg_usb_mdev &= ~BIT(3); // vendor command: bRequest[7] = 0
        usbhw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC | FLD_CTRL_EP_AUTO_INTF);
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X))
        usb1hw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC | FLD_CTRL_EP_AUTO_INTF);
#elif (MCU_CORE_TYPE == MCU_CORE_TL322X || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        usb1hw_enable_manual_interrupt(FLD_USB1_CTRL_EP_AUTO_STD | FLD_USB1_CTRL_EP_AUTO_DESC);
#endif
    }
}

/**
 * @brief     This function servers to get the irq status of control Endpoint.
 * @returns    irq status.
 */
uint32_t tlkusb_hal_get_ctrl_ep_irq(uint8_t index)
{
    if (sUsbUseEventMode == 0) {
        if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
            return usbhw_get_ctrl_ep_irq();
#endif
        } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
            return usb1hw_get_ctrl_ep_irq();
#endif
        }
    } else {
        return sTlkUsbReg.ctrlEpIrq[index];
    }
    return 0;
}

/**
 * @brief     This function servers to clear the irq status of control Endpoint.
 * @param[in] ep - selected  the Endpoint
 * @returns    none.
 */
void tlkusb_hal_clr_ctrl_ep_irq(uint8_t index, int ep)
{
    if (sUsbUseEventMode == 0) {
        if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
            usbhw_clr_ctrl_ep_irq(ep);
#endif
        } else if (index == 1) {
#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL753X || MCU_CORE_TYPE == MCU_CORE_TL322X || (MCU_CORE_TYPE == MCU_CORE_TL752X))
            usb1hw_clr_ctrl_ep_irq(ep);
#endif
        }
    } else {
        uint32_t r = core_interrupt_disable();
        sTlkUsbReg.ctrlEpIrq[index] &= (~ep);
        core_restore_interrupt(r);
    }
}

/**
 * @brief     This function servers to reset the pointer of control Endpoint.
 * @returns    none.
 */
void tlkusb_hal_reset_ctrl_ep_ptr(uint8_t index)
{
    if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
        usbhw_reset_ctrl_ep_ptr();
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        usb1hw_reset_ctrl_ep_ptr();
#endif
    }
}

/**
 * @brief     This function servers to write the data of control Endpoint.
 * @param[in] data -  the data of control Endpoint to write
 * @returns    none
 */
void tlkusb_hal_write_ctrl_ep_data(uint8_t index, uint8_t data)
{
    if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
        usbhw_write_ctrl_ep_data(data);
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        usb1hw_write_ctrl_ep_data(data);
#endif
    }
}

/**
 * @brief     This function servers to set the value of control Endpoint.
 * @param[in] data - the value of control Endpoint
 * @returns    none.
 */
void tlkusb_hal_write_ctrl_ep_ctrl(uint8_t index, uint8_t data)
{
    if (index == 0) {
#if ((MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X))
        usbhw_write_ctrl_ep_ctrl(data);
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        usb1hw_write_ctrl_ep_ctrl(data);
#endif
    }
}

/**
 * @brief     This function servers to read the data of control Endpoint.
 * @returns    the value of control Endpoint data
 */
uint8_t tlkusb_hal_read_ctrl_ep_data(uint8_t index)
{
    if (index == 0) {
#if ((MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X))
        return usbhw_read_ctrl_ep_data();
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        return usb1hw_read_ctrl_ep_data();
#endif
    }
    return 0;
}

/**
 * @brief   This function serves to read two bytes data from host via the control endpoint.
 * @returns  The two bytes data read from the control endpoint.
 */
uint16_t tlkusb_hal_read_ctrl_ep_u16(uint8_t index)
{
    if (index == 0) {
#if ((MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X))
        return usbhw_read_ctrl_ep_u16();
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        return usb1hw_read_ctrl_ep_u16();
#endif
    }
    return 0;
}

/**
 * @brief      This function enables or disables the internal pull-up resistor of DP pin of USB interface
 * @param[in]  en - enables or disables the internal pull-up resistor(1: enable 0: disable)
 * @returns     none
 */
void tlkusb_hal_dp_pullup_en(uint8_t index, int en)
{
    if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
        usb_dp_pullup_en(en);
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        usb1_dp_pullup_en(en);
#endif
    }
}

/**
 * @brief      This function serves to set GPIO MUX function as DP and DM pin of USB
 * @returns     none.
 */
void tlkusb_hal_gpio_enable(uint8_t index)
{
    if (index == 0) {
        gpio_input_en(TLK_USB0_DM_PIN);
        gpio_input_en(TLK_USB0_DP_PIN);
#if (MCU_CORE_TYPE != MCU_CORE_TL751X && MCU_CORE_TYPE != MCU_CORE_TL753X && MCU_CORE_TYPE != MCU_CORE_TL322X && MCU_CORE_TYPE != MCU_CORE_TL752X)
        usb_set_pin_en();
#endif
    } else if (index == 1) {
        gpio_input_en(TLK_USB1_DP_PIN);
        gpio_input_en(TLK_USB1_DM_PIN);
    }
}

/**
 * @brief      This function serves to disable GPIO MUX function as DP and DM pin of USB
 * @returns     none.
 */
void tlkusb_hal_gpio_disable(uint8_t index)
{
    if (index == 0) {
        gpio_input_dis(TLK_USB0_DM_PIN | TLK_USB0_DP_PIN);
        gpio_shutdown(TLK_USB0_DM_PIN | TLK_USB0_DP_PIN);
    } else if (index == 1) {
        gpio_input_dis(TLK_USB1_DM_PIN | TLK_USB1_DP_PIN);
        gpio_shutdown(TLK_USB1_DM_PIN | TLK_USB1_DP_PIN);
    }
}

/**
 * @brief      This function serves to inform Endpoint is ready.
 * @returns     none.
 */
void tlkusb_hal_ep_bulkout_ready(uint8_t index, uint32_t ep)
{
    if (index == 0) {
#if ((MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X))
        reg_usb_ep_ctrl(ep) = FLD_EP_DAT_ACK;
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X))
        reg_usb1_ep_ctrl(ep) = FLD_EP_DAT_ACK;
#elif (MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TYPE == MCU_CORE_TL752X)
        reg_usb1_ep_ctrl(ep) = FLD_USB1_EP_DAT_ACK;
#endif
    }
}

/**
 * @brief     This function servers to determine whether Endpoint is busy.
 * @param[in] ep -  selected the Endpoint
 * @returns    1: busy; 0: not busy.
 */
uint32_t tlkusb_hal_is_ep_busy(uint8_t index, uint32_t ep)
{
    if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
        return usbhw_is_ep_busy(ep);
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        return usb1hw_is_ep_busy(ep);
#endif
    }
    return 1;
}

/**
 * @brief     This function servers to reset the pointer of Endpoint.
 * @param[in] ep - select the Endpoint
 * @returns    none.
 */
void tlkusb_hal_reset_ep_ptr(uint8_t index, uint32_t ep)
{
    if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
        usbhw_reset_ep_ptr(ep);
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        usb1hw_reset_ep_ptr(ep);
#endif
    }
}

/**
 * @brief     This function servers to write the data of endpoint.
 * @param[in] ep   - selected the endpoint
 * @param[in] pData - the pointer to the data for write.
 * @param[in] dataLen  - write length.
 * @returns    none
 */
void tlkusb_hal_write_ep_data(uint8_t index, uint32_t ep, uint8_t *pData, uint16_t dataLen)
{
    if (index == 0) {
        for (uint16_t i = 0; i < dataLen; i++) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
            usbhw_write_ep_data(ep, pData[i]);
#endif
        }
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        usb1hw_write_ep_data(ep, pData, dataLen);
#endif
    }
}

/**
 * @brief     This function servers to set the specified data endpoint to ack.
 * @param[in] ep -  select the data endpoint.
 * @returns    none.
 */
void tlkusb_hal_data_ep_ack(uint8_t index, uint32_t ep)
{
    if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
        usbhw_data_ep_ack(ep);
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        usb1hw_data_ep_ack(ep);
#endif
    }
}

/**
 * @brief     This function servers to get the irq status of Endpoint.
 * @returns    none.
 */
uint32_t tlkusb_hal_get_eps_irq(uint8_t index)
{
    if (sUsbUseEventMode == 0) {
        if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
            return usbhw_get_eps_irq();
#endif
        } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
            return usb1hw_get_eps_irq();
#endif
        }
    } else {
        return sTlkUsbReg.epsIrq[index];
    }
    return 0;
}

/**
 * @brief     This function servers to clear the irq status of Endpoint.
 * @param[in] ep - selected  the Endpoint
 * @returns    none.
 */
void tlkusb_hal_clr_eps_irq(uint8_t index, int ep)
{
    if (sUsbUseEventMode == 0) {
        if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
            usbhw_clr_eps_irq(ep);
#endif
        } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
            usb1hw_clr_eps_irq(ep);
#endif
        }
    } else {
        uint32_t r = core_interrupt_disable();
        sTlkUsbReg.epsIrq[index] &= ~ep;
        core_restore_interrupt(r);
    }
}

/**
 * @brief     This function servers to get the pointer of Endpoint.
 * @param[in] ep - select the Endpoint
 * @returns    none.
 */
uint16_t tlkusb_hal_get_ep_ptr(uint8_t index, uint32_t ep)
{
    if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X) && (MCU_CORE_TYPE != MCU_CORE_TL752X)
#if (MCU_CORE_TYPE != MCU_CORE_B91)
        return usbhw_get_ep_ptr(ep);
#else
        return reg_usb_ep_ptr(ep);
#endif
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        return usb1hw_get_ep_ptr(ep);
#endif
    }
    return 0;
}

/**
 * @brief     This function servers to write the data of endpoint.
 * @param[in] ep   - selected the endpoint
 * @param[in] data - the pointer to the data for write.
 * @param[in] len  - write length.
 * @returns    none
 */
void tlkusb_hal_read_ep_data(uint8_t index, uint32_t ep, uint8_t *buffer, uint16_t length)
{
    if (index == 0) {
        for (uint8_t i = 0; i < length; i++) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X && MCU_CORE_TYPE != MCU_CORE_TL752X)
            buffer[i] = usbhw_read_ep_data(ep);
#endif
        }
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        usb1hw_read_ep_data(ep, buffer, length);
#endif
    }
}

/**
 * @brief     This function servers to enable usb event mode(irq).
 * @returns    none
 */
#if (MCU_CORE_N22 == 0)
inline void tlkusb_hal_enable_eventMode(void)
{
    if (sUsbUseEventMode == 1) {
        return;
    }
    sUsbUseEventMode = 1;
#if (MCU_CORE_TYPE == MCU_CORE_TL322X)
    plic_interrupt_enable(IRQ_USB1_CTRL_EP_SETUP);
    plic_interrupt_enable(IRQ_USB1_CTRL_EP_DATA);
    plic_interrupt_enable(IRQ_USB1_CTRL_EP_STATUS);
    plic_interrupt_enable(IRQ_USB1_RESET);
    plic_interrupt_enable(IRQ_USB1_ENDPOINT);
#elif (MCU_CORE_TYPE == MCU_CORE_TL752X)
    plic_interrupt_enable(IRQ_USB1_EP0_SETUP);
    plic_interrupt_enable(IRQ_USB1_EP0_DATA);
    plic_interrupt_enable(IRQ_USB1_EP0_STATUS);
    plic_interrupt_enable(IRQ_USB1_RESET_250US);
    plic_interrupt_enable(IRQ_USB1_EPN_DATA);
#else
    plic_interrupt_enable(IRQ_USB_CTRL_EP_SETUP);
    plic_interrupt_enable(IRQ_USB_CTRL_EP_DATA);
    plic_interrupt_enable(IRQ_USB_CTRL_EP_STATUS);
    plic_interrupt_enable(IRQ_USB_RESET);
    plic_interrupt_enable(IRQ_USB_ENDPOINT);
#endif

#if (MCU_CORE_TYPE == MCU_CORE_B92)
    usbhw_set_irq_mask(USB_IRQ_RESET_MASK);
#endif
#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL753X)
    usbhw_set_irq_mask(USB_IRQ_DATA_MASK | USB_IRQ_SETUP_MASK | USB_IRQ_STATUS_MASK | USB_IRQ_RESET_MASK);
#endif
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
    usb1hw_set_irq_mask(USB1_IRQ_RESET_MASK | USB1_IRQ_SETUP_MASK | USB1_IRQ_DATA_MASK | USB1_IRQ_STATUS_MASK);
#endif


#if (MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TYPE == MCU_CORE_TL752X)
    usb1hw_set_eps_irq_mask(FLD_USB1_EDP5_IRQ);
#else
    usbhw_set_eps_irq_mask(FLD_USB_EDP5_IRQ);
#endif

#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL753X || MCU_CORE_TYPE == MCU_CORE_TL322X)
    plic_interrupt_enable(IRQ_USB1_CTRL_EP_SETUP);
    plic_interrupt_enable(IRQ_USB1_CTRL_EP_DATA);
    plic_interrupt_enable(IRQ_USB1_CTRL_EP_STATUS);
    plic_interrupt_enable(IRQ_USB1_RESET);
    plic_interrupt_enable(IRQ_USB1_ENDPOINT);
#if (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X)
    usb1hw_set_irq_mask(FLD_USB1_IRQ_DATA_MASK | FLD_USB1_IRQ_SETUP_MASK | FLD_USB1_IRQ_STATUS_MASK | FLD_USB1_IRQ_RESET_MASK);
#else
    usb1hw_set_irq_mask(USB_IRQ_DATA_MASK | USB_IRQ_SETUP_MASK | USB_IRQ_STATUS_MASK | USB_IRQ_RESET_MASK);
#endif

    usb1hw_set_eps_irq_mask(FLD_USB1_EDP5_IRQ);
#endif
}
#else
void tlkusb_hal_enable_eventMode(void) {}
#endif


/**
 * @brief     This function servers to disable usb event mode(irq).
 * @returns    none
 */
#if (MCU_CORE_N22 == 0)
void tlkusb_hal_disable_eventMode(void)
{
    if (sUsbUseEventMode == 0) {
        return;
    }
    sUsbUseEventMode = 0;
#if (MCU_CORE_TYPE == MCU_CORE_TL322X)
    plic_interrupt_disable(IRQ_USB1_CTRL_EP_SETUP);
    plic_interrupt_disable(IRQ_USB1_CTRL_EP_DATA);
    plic_interrupt_disable(IRQ_USB1_CTRL_EP_STATUS);
    plic_interrupt_disable(IRQ_USB1_RESET);
    plic_interrupt_disable(IRQ_USB1_ENDPOINT);
#elif (MCU_CORE_TYPE == MCU_CORE_TL752X)
    plic_interrupt_disable(IRQ_USB1_EP0_SETUP);
    plic_interrupt_disable(IRQ_USB1_EP0_DATA);
    plic_interrupt_disable(IRQ_USB1_EP0_STATUS);
    plic_interrupt_disable(IRQ_USB1_RESET_250US);
    plic_interrupt_disable(IRQ_USB1_EPN_DATA);
#else
    plic_interrupt_disable(IRQ_USB_CTRL_EP_SETUP);
    plic_interrupt_disable(IRQ_USB_CTRL_EP_DATA);
    plic_interrupt_disable(IRQ_USB_CTRL_EP_STATUS);
    plic_interrupt_disable(IRQ_USB_RESET);
    plic_interrupt_disable(IRQ_USB_ENDPOINT);
#endif

#if (MCU_CORE_TYPE == MCU_CORE_B92)
    usbhw_clr_irq_mask(USB_IRQ_RESET_MASK);
#endif
#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL753X)
    usbhw_clr_irq_mask(USB_IRQ_DATA_MASK | USB_IRQ_SETUP_MASK | USB_IRQ_STATUS_MASK | USB_IRQ_RESET_MASK);
#endif

#if (MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TYPE == MCU_CORE_TL752X)
    usb1hw_clr_eps_irq_mask(FLD_USB1_EDP5_IRQ);
#else
    usbhw_clr_eps_irq_mask(FLD_USB_EDP5_IRQ);
#endif

#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL753X || MCU_CORE_TYPE == MCU_CORE_TL322X)
    plic_interrupt_disable(IRQ_USB1_CTRL_EP_SETUP);
    plic_interrupt_disable(IRQ_USB1_CTRL_EP_DATA);
    plic_interrupt_disable(IRQ_USB1_CTRL_EP_STATUS);
    plic_interrupt_disable(IRQ_USB1_RESET);
    plic_interrupt_disable(IRQ_USB1_ENDPOINT);
#if (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X)
    usb1hw_clr_irq_mask(FLD_USB1_IRQ_DATA_MASK | FLD_USB1_IRQ_SETUP_MASK | FLD_USB1_IRQ_STATUS_MASK | FLD_USB1_IRQ_RESET_MASK);
#else
    usb1hw_clr_irq_mask(USB_IRQ_DATA_MASK | USB_IRQ_SETUP_MASK | USB_IRQ_STATUS_MASK | USB_IRQ_RESET_MASK);
#endif

    usb1hw_clr_eps_irq_mask(FLD_USB1_EDP5_IRQ);
#endif
}
#else
void tlkusb_hal_disable_eventMode(void) {}
#endif

/**
 * @brief     This function servers to get the USB event mode status.
 * @returns   The USB event mode status, 1 for event mode enabled, 0 for event mode disabled.
 */
uint8_t tlkusb_hal_is_eventMode(void)
{
    return sUsbUseEventMode;
}

/**
 * @brief       Wake up the USB thread by setting a system event
 * @param       None
 * @return      None
 */
_always_inline void tlkusb_hal_wakeup_usb_thread(void)
{
    tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USB);
}

/**
 * @brief       Wake up the USB thread by setting a system event from ISR context
 * @param       None
 * @return      None
 */
_always_inline void tlkusb_hal_wakeup_usb_thread_fromIsr(void)
{
    tlksys_task_setEvtFromIsr(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USB);
}

/**
 * @brief       Wake up the USB hs thread by setting a system event
 * @param       None
 * @return      None
 */
_always_inline void tlkusb_hal_wakeup_uachs_thread(void)
{
    tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USB_HS);
}

/**
 * @brief       Wake up the USB hs thread by setting a system event from ISR context
 * @param       None
 * @return      None
 */
_always_inline void tlkusb_hal_wakeup_uachs_thread_fromIsr(void)
{
    tlksys_task_setEvtFromIsr(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USB_HS);
}

/**
 * @brief       USB control endpoint interrupt handler for specified USB instance
 * @param[in]   index - USB instance index (0 or 1)
 * @return      None
 */
inline void tlkusb_ctrl_ep_irq_handler(uint8_t index)
{
    uint32_t irq = 0;
    if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X)
#if (MCU_CORE_TYPE != CHIP_TYPE_TL752X)
        irq = usbhw_get_ctrl_ep_irq();
        usbhw_clr_ctrl_ep_irq(irq);
#endif
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        irq = usb1hw_get_ctrl_ep_irq();
        usb1hw_clr_ctrl_ep_irq(irq);
#endif
    }
    sTlkUsbReg.ctrlEpIrq[index] |= irq;
    tlkusb_hal_wakeup_usb_thread_fromIsr();
}

/**
 * @brief       This function handles USB endpoint interrupts for the specified USB instance.
 * @param[in]   index - USB instance index (0 or 1)
 * @returns     none. 
 */
inline void tlkusb_eps_irq_handler(uint8_t index)
{
    uint32_t irq = 0;
    if (index == 0) {
#if (MCU_CORE_TYPE != MCU_CORE_TL322X)
#if (MCU_CORE_TYPE != CHIP_TYPE_TL752X)
        irq = usbhw_get_eps_irq();
        usbhw_clr_eps_irq(irq);
#endif
#endif
    } else if (index == 1) {
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL753X) || (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X))
        irq = usb1hw_get_eps_irq();
        usb1hw_clr_eps_irq(irq);
#endif
    }
    sTlkUsbReg.epsIrq[index] |= irq;
    tlkusb_hal_wakeup_usb_thread_fromIsr();
}
#else
/**
 * @brief     This function servers to disable usb event mode(irq).
 * @returns    none
 */
void tlkusb_hal_disable_eventMode(void) {}

#endif // #if (TLK_CFG_USB_ENABLE)
