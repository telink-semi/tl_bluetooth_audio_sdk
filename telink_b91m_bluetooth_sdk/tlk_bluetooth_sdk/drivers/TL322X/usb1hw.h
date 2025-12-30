/********************************************************************************************************
 * @file    usb1hw.h
 *
 * @brief   This is the header file for tl322x
 *
 * @author  Driver Group
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
#ifndef __USB1_HW_H_
#define __USB1_HW_H_

#include "lib/include/analog.h"
#include "gpio.h"
#include "lib/include/pm/pm.h"
#include "reg_include/register.h"

/**
 * @brief EDP7 and EDP8 share 1024 bytes of space; EDP1, EDP2,and EDP3 share 64 bytes of space; EDP4, EDP5,and EDP6 share 64 bytes of space.
 */
typedef enum
{
    USB1_EDP8_IN  = 8, /**< bulk in. */
    USB1_EDP1_IN  = 1, /**< bulk in. */
    USB1_EDP2_IN  = 2, /**< bulk in. */
    USB1_EDP3_IN  = 3, /**< bulk in. */
    USB1_EDP4_OUT = 4, /**< bulk out. */
    USB1_EDP5_OUT = 5, /**< bulk out. */
    USB1_EDP6_OUT = 6, /**< bulk out. */
    USB1_EDP7_IN  = 7, /**< bulk in */
} usb1_ep_index;

typedef enum
{
    USB1_IRQ_RESET_MASK   = BIT(0),
    USB1_IRQ_250US_MASK   = BIT(1),
    USB1_IRQ_SUSPEND_MASK = BIT(2),
    USB1_IRQ_SOF_MASK     = BIT(3),
} usb1_irq_mask_e;

typedef enum
{
    USB1_IRQ_RESET_STATUS    = BIT(0),
    USB1_IRQ_250US_STATUS    = BIT(1),
    USB1_IRQ_SUSPEND_STATUS  = BIT(2),
    USB1_IRQ_SOF_STATUS      = BIT(3),
    USB1_IRQ_EP_SETUP_STATUS = BIT(4),
    USB1_IRQ_EP_DATA_STATUS  = BIT(5),
    USB1_IRQ_EP_STA_STATUS   = BIT(6),
    USB1_IRQ_EP_INTF_STATUS  = BIT(7),
} usb1_irq_status_e;

/**
 * @brief     This function servers to enable endpoint 8 fifo mode.
 * @return    none.
 */
static inline void usb1hw_ep8_fifo_mode_en(void)
{
    BM_SET(reg_usb1_ep8_fifo_mode, FLD_USB1_EDP8_FIFO_MODE);
}

/**
 * @brief     This function servers to disable endpoint 8 fifo mode.
 * @return    none.
 */
static inline void usb1hw_ep8_fifo_mode_dis(void)
{
    BM_CLR(reg_usb1_ep8_fifo_mode, FLD_USB1_EDP8_FIFO_MODE);
}

/**
 * @brief     This function servers to reset the pointer of control endpoint.
 * @return    none.
 */
static inline void usb1hw_reset_ctrl_ep_ptr(void)
{
    reg_usb1_ctrl_ep_ptr = 0;
}

/**
 * @brief     This function servers to set the value of control endpoint.
 * @param[in] data - the value of control endpoint
 * @return    none.
 */
static inline void usb1hw_write_ctrl_ep_ctrl(unsigned char data)
{
    reg_usb1_ctrl_ep_ctrl = data;
}

/**
 * @brief     This function servers to read the data of control endpoint.
 * @return    the value of control endpoint data
 */
static inline unsigned char usb1hw_read_ctrl_ep_data(void)
{
    return reg_usb1_ctrl_ep_dat;
}

/**
 * @brief     This function servers to write the data of control endpoint.
 * @param[in] data -  the data of control endpoint to write
 * @return    none
 */
static inline void usb1hw_write_ctrl_ep_data(unsigned char data)
{
    reg_usb1_ctrl_ep_dat = data;
}

/**
 * @brief     This function servers to determine whether control endpoint is busy.
 * @return    1: busy; 0: not busy.
 */
static inline int usb1hw_is_ctrl_ep_busy(void)
{
    return reg_usb1_ctrl_ep_ctrl & FLD_USB1_EP_DAT_BUSY;
}

/**
 * @brief     This function servers to reset the pointer of endpoint.
 * @param[in] ep - select the endpoint
 * @return    none.
 */
static inline void usb1hw_reset_ep_ptr(usb1_ep_index ep)
{
    reg_usb1_ep_ptr(ep)  = 0;
    reg_usb1_ep_ptrh(ep) = 0;
}

/**
 * @brief     This function servers to get the pointer of endpoint.
 * @param[in] ep - select the endpoint
 * @return    none.
 */
static inline unsigned short usb1hw_get_ep_ptr(usb1_ep_index ep)
{
    return (reg_usb1_ep_ptr(ep) | ((reg_usb1_ep_ptrh(ep) & 0x07) << 8));
}

/**
 * @brief     This function servers to set the base pointer of endpoint.
 * @param[in] ep - select the endpoint.
 * @param[in] addr - endpoint address.
 * @return    none.
 */
static inline void usb1hw_set_ep_addr(usb1_ep_index ep, unsigned short addr)
{
    reg_usb1_ep_buf_addr(ep)   = addr & 0xff;
    reg_usb1_ep_buf_h_addr(ep) = (addr >> 8) & 0x07;
}

/**
 * @brief     This function servers to get the irq status of control endpoint.
 * @return    The irq status of control endpoint.
 */
static inline unsigned int usb1hw_get_ctrl_ep_irq(void)
{
    return reg_usb1_ctrl_ep_irq_sta;
}

/**
 * @brief     This function servers to clear the irq status of control endpoint.
 * @param[in] ep - the irq status of control endpoint.
 * @return    none.
 */
static inline void usb1hw_clr_ctrl_ep_irq(int ep)
{
    reg_usb1_ctrl_ep_irq_sta = ep;
}

/**
 * @brief     This function servers to set the irq mask of endpoint.
 * @return    none.
 */
static inline void usb1hw_set_eps_irq_mask(usb1_ep_irq_e mask)
{
    reg_usb1_ep_irq_mask |= mask;
}

/**
 * @brief     This function servers to clr the irq mask of endpoint.
 * @return    none.
 */
static inline void usb1hw_clr_eps_irq_mask(usb1_ep_irq_e mask)
{
    reg_usb1_ep_irq_mask &= (~mask);
}

/**
 * @brief     This function servers to get the irq status of endpoint.
 * @return    The irq status of data endpoint.
 */
static inline unsigned int usb1hw_get_eps_irq(void)
{
    return reg_usb1_ep_irq_status;
}

/**
 * @brief     This function servers to clear the irq status of endpoint.
 * @param[in] ep - the irq of data endpoint
 * @return    none.
 */
static inline void usb1hw_clr_eps_irq(int ep)
{
    reg_usb1_ep_irq_status = ep;
}

/**
 * @brief     This function servers to set usb irq mask.
 * @param[in]  mask - the irq mask of usb.
 * @return    none.
 */
static inline void usb1hw_set_irq_mask(usb1_irq_mask_e mask)
{
    reg_usb1_irq_mask |= mask;
}

/**
 * @brief     This function servers to clear usb irq mask.
 * @param[in]  mask -the irq mask of usb.
 * @return    none.
 */
static inline void usb1hw_clr_irq_mask(usb1_irq_mask_e mask)
{
    reg_usb1_irq_mask &= (~mask);
}

/**
 * @brief     This function servers to get usb irq status.
 * @param[in]  status   - the irq status of usb.
 * @retval     non-zero -  the interrupt occurred.
 * @retval     zero     -  the interrupt did not occur.
 */
static inline unsigned char usb1hw_get_irq_status(usb1_irq_status_e status)
{
    return reg_usb1_ctrl_ep_irq_sta & status;
}

/**
 * @brief     This function servers to clr usb irq status.
 * @param[in]  status -the irq status of usb.
 * @return    none.
 */
static inline void usb1hw_clr_irq_status(usb1_irq_status_e status)
{
    reg_usb1_ctrl_ep_irq_sta = status;
}

/**
 * @brief     This function servers to enable endpoint.
 * @param[in] ep - selected the endpoint
 * @return    none.
 */
static inline void usb1hw_set_eps_en(usb1_ep_en_e ep)
{
    reg_usb1_edp_en = ep;
}

/**
 * @brief     This function servers to disable endpoint.
 * @param[in] ep - selected the endpoint
 * @return    none.
 */
static inline void usb1hw_set_eps_dis(usb1_ep_en_e ep)
{
    reg_usb1_edp_en &= (~ep);
}

/**
 * @brief     This function servers to determine whether endpoint is busy.
 * @param[in] ep - selected the endpoint
 * @return    1: busy; 0: not busy.
 */
static inline unsigned int usb1hw_is_ep_busy(unsigned int ep)
{
    return reg_usb1_ep_ctrl(ep) & FLD_USB1_EP_BUSY;
}

/**
 * @brief     This function servers to set the specified data endpoint to ack.
 * @param[in] ep -  select the data endpoint.
 * @return    none.
 */
static inline void usb1hw_data_ep_ack(unsigned int ep)
{
    reg_usb1_ep_ctrl(ep) = FLD_USB1_EP_ACK;
}

/**
 * @brief     This function servers to set the specified data endpoint to stall.
 * @param[in] ep - select the data endpoint.
 * @return    none.
 */
static inline void usb1hw_data_ep_stall(unsigned int ep)
{
    reg_usb1_ep_ctrl(ep) = FLD_USB1_EP_STALL;
}

/**
 * @brief     This function servers to set the threshold of printer.
 * @param[in] th - set the threshold for printer
 * @return    none.
 */
static inline void usb1hw_set_printer_threshold(unsigned short th)
{
    reg_usb1_ep8_send_thres   = th & 0xff;
    reg_usb1_ep8_send_thres_h = (reg_usb1_ep8_send_thres_h & (~FLD_USB1_EDP8_MIN1)) | MASK_VAL(FLD_USB1_EDP8_MIN1, ((th >> 8) & FLD_USB1_EDP8_MIN1));
}

/**
 * @brief     This function servers to set IN endpoint size(wMaxPacketSize) for returning a zero-length packet
 * @param[in] size -  IN endpoint size
 * @return    none.
 */
static inline void usb1hw_set_in_ep_size(unsigned short size)
{
    reg_usb1_edps_eptr = size;
}

/**
 * @brief     This function servers to determine whether endpoint is busy.
 * @param[in] ep -  selected the endpoint
 */
static inline _Bool usb1hw_is_in_ep_full_thrd(unsigned int ep)
{
    return reg_usb1_edps_full_thrd & BIT(ep & 0x07);
}

/**
 * @brief     This function servers to set in endpoint buffer max size.
 * @param[in] max_size - in endpoint max size
 * @return    none.
 */
static inline void usb1hw_set_eps_max_size(unsigned int max_size)
{
    reg_usb1_ep_max_size = max_size >> 3;
}

/**
 * @brief      This function serves to enable USB1 power and clock.
 * @return     none.
 */
void usb1hw_init(void);

/**
 * @brief      This function servers to disable the manual interrupt.
 * @param[in]  m - the irq mode needs to set
 * @return     none
 */
void usb1hw_disable_manual_interrupt(int m);

/**
 * @brief      This function servers to enable the manual interrupt
 * @param[in]  m - the irq mode needs to set
 * @return     none
 */
void usb1hw_enable_manual_interrupt(int m);

/**
 * @brief      This function sends two bytes data to host via the control endpoint.
 * @param[in]  v - the two bytes data need to send.
 * @return     none
 */
void usb1hw_write_ctrl_ep_u16(unsigned short v);

/**
 * @brief   This function reads two bytes data from host via the control endpoint.
 * @return  the two bytes data read from the control endpoint.
 */
unsigned short usb1hw_read_ctrl_ep_u16(void);

/**
 * @brief      This function enables or disables the internal pull-up resistor of DP pin of USB interface
 * @param[in]  en - enables or disables the internal pull-up resistor(1: enable 0: disable)
 * @return     none
 */
static inline void usb1_dp_pullup_en(int en)
{
    unsigned char dat = analog_read_reg8(areg_aon_0x0b);
    if (en) {
        dat = dat & (~BIT(7));
    } else {
        dat = dat | BIT(7);
    }

    analog_write_reg8(areg_aon_0x0b, dat);
}

/**
 * @brief      This function serves to power on or down USB module.
 * @param[in]  en - 1: power on 0: power down.
 * @return     none
 */
static inline void usb1_power_on(unsigned char en)
{
    if (en) {
        analog_write_reg8(areg_aon_0x7d, analog_read_reg8(areg_aon_0x7d) & 0xfd);
    } else {
        analog_write_reg8(areg_aon_0x7d, analog_read_reg8(areg_aon_0x7d) | 0x02);
    }
}

/**
 * @brief   This function serves to set GPIO MUX function as DP and DM pin of USB1.
 * @return  none.
 * @note    PA5 and PA6 pins can only be used for GPIO and USB DP/DM, so there is no need to set pin mux.
 */
static inline void usb1_set_pin(void)
{
    reg_gpio_func_mux(GPIO_PA5) = 0x00; /* DM */
    reg_gpio_func_mux(GPIO_PA6) = 0x00; /* DP */

    gpio_set_data_strength(GPIO_PA5, DRV_STRENGTH_8MA); /* Minimum 8MA for usb to work properly. */
    gpio_set_data_strength(GPIO_PA6, DRV_STRENGTH_8MA);

    gpio_function_dis(GPIO_PA5 | GPIO_PA6);
    gpio_input_en(GPIO_PA5 | GPIO_PA6); /* DP/DM must set input enable */
    usb1_dp_pullup_en(1);
}

/**
 * @brief     This function servers to write the data of endpoint.
 * @param[in] ep   - selected the endpoint
 * @param[in] data - the pointer to the data for write.
 * @param[in] len  - write length.
 * @return    none
 */
void usb1hw_write_ep_data(usb1_ep_index ep, unsigned char *data, unsigned short len);

/**
 * @brief     This function servers to write the data of endpoint.
 * @param[in] ep   - selected the endpoint
 * @param[in] data - the pointer to the data for write.
 * @param[in] len  - write length.
 * @return    none
 */
void usb1hw_read_ep_data(usb1_ep_index ep, unsigned char *data, unsigned short len);

#endif
