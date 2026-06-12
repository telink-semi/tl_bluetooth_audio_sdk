/********************************************************************************************************
 * @file    tlkusb_hal.h
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
#ifndef TLKUSB_HAL_H
#define TLKUSB_HAL_H

#include "common/types.h"
#if ((MCU_CORE_TYPE == MCU_CORE_TL751X) || (MCU_CORE_TYPE == MCU_CORE_TL752X) || MCU_CORE_TYPE == MCU_CORE_TL753X)
#define TLK_CFG_USB_NUMB      2
#define TLK_CFG_USB_UDB_INDEX 1
#define TLK_CFG_USB_UAC_INDEX 0
#define TLK_CFG_USB_MSC_INDEX 0
#elif (MCU_CORE_TYPE == MCU_CORE_TL322X)
#define TLK_CFG_USB_NUMB      2
#define TLK_CFG_USB_UDB_INDEX 1
#define TLK_CFG_USB_UAC_INDEX 0
#define TLK_CFG_USB_MSC_INDEX (TLK_CFG_USB_NUMB + 1) //default noused
#else
#define TLK_CFG_USB_NUMB      1
#define TLK_CFG_USB_UDB_INDEX 0
#define TLK_CFG_USB_UAC_INDEX 0
#define TLK_CFG_USB_MSC_INDEX 0
#endif

#if ((MCU_CORE_TYPE == MCU_CORE_B91) || (MCU_CORE_TYPE == MCU_CORE_B92) || (MCU_CORE_TYPE == MCU_CORE_TL721X))
#define TLK_USB0_DP_PIN GPIO_PA6
#define TLK_USB0_DM_PIN GPIO_PA5
#define TLK_USB1_DP_PIN 0 //Do not support.
#define TLK_USB1_DM_PIN 0
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define TLK_USB0_DP_PIN GPIO_PF3
#define TLK_USB0_DM_PIN GPIO_PF2
#define TLK_USB1_DP_PIN GPIO_PF1 //used for dbg
#define TLK_USB1_DM_PIN GPIO_PF0 //used for dbg
#elif (MCU_CORE_TYPE == MCU_CORE_TL752X)
#define TLK_USB0_DP_PIN 0
#define TLK_USB0_DM_PIN 0
#define TLK_USB1_DP_PIN GPIO_PC8 //PC08 //used for dbg
#define TLK_USB1_DM_PIN GPIO_PC9 //PC09 //used for dbg
#elif (MCU_CORE_TYPE == MCU_CORE_TL753X)
#define TLK_USB0_DP_PIN GPIO_PF3
#define TLK_USB0_DM_PIN GPIO_PF2
#define TLK_USB1_DP_PIN GPIO_PF1 //used for dbg
#define TLK_USB1_DM_PIN GPIO_PF0 //used for dbg
#elif (MCU_CORE_TYPE == MCU_CORE_TL322X)
#define TLK_USB0_DP_PIN GPIO_PA4 //used for audio
#define TLK_USB0_DM_PIN GPIO_PA3 //used for audio
#define TLK_USB1_DP_PIN GPIO_PA6 //used for dbg
#define TLK_USB1_DM_PIN GPIO_PA5 //used for dbg
#endif

/**
 * @brief     This function servers to init usb core.
 * @returns    none.
 */
void tlkusb_hal_core_init(uint8_t index);

/**
 * @brief     This function servers to get the irq status of control Endpoint.
 * @returns    irq status.
 */
uint32_t tlkusb_hal_get_ctrl_ep_irq(uint8_t index);

/**
 * @brief     This function servers to clear the irq status of control Endpoint.
 * @param[in] ep - selected  the Endpoint
 * @returns    none.
 */
void tlkusb_hal_clr_ctrl_ep_irq(uint8_t index, int ep);

/**
 * @brief     This function servers to reset the pointer of control Endpoint.
 * @returns    none.
 */
void tlkusb_hal_reset_ctrl_ep_ptr(uint8_t index);

/**
 * @brief     This function servers to write the data of control Endpoint.
 * @param[in] data -  the data of control Endpoint to write
 * @returns    none
 */
void tlkusb_hal_write_ctrl_ep_data(uint8_t index, uint8_t data);

/**
 * @brief     This function servers to set the value of control Endpoint.
 * @param[in] data - the value of control Endpoint
 * @returns    none.
 */
void tlkusb_hal_write_ctrl_ep_ctrl(uint8_t index, uint8_t data);

/**
 * @brief     This function servers to read the data of control Endpoint.
 * @returns    the value of control Endpoint data
 */
uint8_t tlkusb_hal_read_ctrl_ep_data(uint8_t index);

/**
 * @brief   This function serves to read two bytes data from host via the control endpoint.
 * @returns  The two bytes data read from the control endpoint.
 */
uint16_t tlkusb_hal_read_ctrl_ep_u16(uint8_t index);

/**
 * @brief      This function enables or disables the internal pull-up resistor of DP pin of USB interface
 * @param[in]  index - usb index.
 * @param[in]  en    - enables or disables the internal pull-up resistor(1: enable 0: disable)
 * @returns     none
 */
void tlkusb_hal_dp_pullup_en(uint8_t index, int en);

/**
 * @brief      This function serves to set GPIO MUX function as DP and DM pin of USB
 * @returns     none.
 */
void tlkusb_hal_gpio_enable(uint8_t index);

/**
 * @brief      This function serves to disable GPIO MUX function as DP and DM pin of USB
 * @param[in]  index - usb index.
 * @returns     none.
 */
void tlkusb_hal_gpio_disable(uint8_t index);

/**
 * @brief      This function serves to inform Endpoint is ready.
 * @returns     none.
 */
void tlkusb_hal_ep_bulkout_ready(uint8_t index, uint32_t ep);

/**
 * @brief     This function servers to determine whether Endpoint is busy.
 * @param[in] ep -  selected the Endpoint
 * @returns    1: busy; 0: not busy.
 */
uint32_t tlkusb_hal_is_ep_busy(uint8_t index, uint32_t ep);

/**
 * @brief     This function servers to reset the pointer of Endpoint.
 * @param[in] ep - select the Endpoint
 * @returns    none.
 */
void tlkusb_hal_reset_ep_ptr(uint8_t index, uint32_t ep);

/**
 * @brief     This function servers to write the data of endpoint.
 * @param[in] ep   - selected the endpoint
 * @param[in] pData - the pointer to the data for write.
 * @param[in] dataLen  - write length.
 * @returns    none
 */
void tlkusb_hal_write_ep_data(uint8_t index, uint32_t ep, uint8_t *pData, uint16_t dataLen);

/**
 * @brief     This function servers to set the specified data endpoint to ack.
 * @param[in] ep -  select the data endpoint.
 * @returns    none.
 */
void tlkusb_hal_data_ep_ack(uint8_t index, uint32_t ep);

/**
 * @brief     This function servers to get the irq status of Endpoint.
 * @returns    none.
 */
uint32_t tlkusb_hal_get_eps_irq(uint8_t index);

/**
 * @brief     This function servers to clear the irq status of Endpoint.
 * @param[in] ep - selected  the Endpoint
 * @returns    none.
 */
void tlkusb_hal_clr_eps_irq(uint8_t index, int ep);

/**
 * @brief     This function servers to get the pointer of Endpoint.
 * @param[in] ep - select the Endpoint
 * @returns    none.
 */
uint16_t tlkusb_hal_get_ep_ptr(uint8_t index, uint32_t ep);

/**
 * @brief     This function servers to write the data of endpoint.
 * @param[in] ep   - selected the endpoint
 * @param[in] data - the pointer to the data for write.
 * @param[in] len  - write length.
 * @returns    none
 */
void tlkusb_hal_read_ep_data(uint8_t index, uint32_t ep, uint8_t *buffer, uint16_t length);

/**
 * @brief     This function servers to enable usb event mode(irq).
 * @returns    none
 */
void tlkusb_hal_enable_eventMode(void);

/**
 * @brief     This function servers to disable usb event mode(irq).
 * @returns    none
 */
void tlkusb_hal_disable_eventMode(void);

/**
 * @brief     This function servers to get the USB event mode status.
 * @returns   The USB event mode status, 1 for event mode enabled, 0 for event mode disabled.
 */
uint8_t tlkusb_hal_is_eventMode(void);

/**
 * @brief       Wake up the USB thread by setting a system event
 * @param       None
 * @return      None
 */
void tlkusb_hal_wakeup_usb_thread(void);

/**
 * @brief       Wake up the USB thread by setting a system event from ISR context
 * @param       None
 * @return      None
 */
void tlkusb_hal_wakeup_usb_thread_fromIsr(void);

/**
 * @brief       Wake up the USB hs thread by setting a system event
 * @param       None
 * @return      None
 */
void tlkusb_hal_wakeup_uachs_thread(void);

/**
 * @brief       Wake up the USB hs thread by setting a system event from ISR context
 * @param       None
 * @return      None
 */
void tlkusb_hal_wakeup_uachs_thread_fromIsr(void);

/**
 * @brief       USB control endpoint interrupt handler for specified USB instance
 * @param[in]   index - USB instance index (0 or 1)
 * @return      None
 */
void tlkusb_ctrl_ep_irq_handler(uint8_t index);

/**
 * @brief       This function handles USB endpoint interrupts for the specified USB instance.
 * @param[in]   index - USB instance index (0 or 1)
 * @returns     none. 
 */
void tlkusb_eps_irq_handler(uint8_t index);

#endif // TLKUSB_HAL_H
