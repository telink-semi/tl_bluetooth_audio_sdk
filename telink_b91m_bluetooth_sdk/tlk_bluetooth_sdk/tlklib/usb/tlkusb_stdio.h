/********************************************************************************************************
 * @file    tlkusb_stdio.h
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
#ifndef TLKUSB_STDIO_H
#define TLKUSB_STDIO_H


#include "tlklib/usb/tlkusb_config.h"
#include "tlklib/usb/tlkusb_define.h"
#include "tlklib/usb/tlkusb_struct.h"
#include "tlklib/usb/tlkusb_module.h"
#include "tlklib/usb/tlkusb.h"
#include "tlklib/usb/tlkusb_hal.h"
#include "drivers.h"

/**
 * @brief       This function clears the USB endpoint interrupt status in inline mode.
 * @param[in]   ep   - the endpoint whose interrupt status needs to be cleared.
 * @returns     none.
 */
static __attribute__((__always_inline__)) inline void usbhw_clr_eps_irq_inline(int ep)
{
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
    reg_usb1_ep_irq_status = ep;
#elif (MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TYPE == MCU_CORE_B91)
    (void)ep;
#else
    reg_usb_ep_irq_status = ep;
#endif
}

/**
 * @brief       This function gets the USB endpoint pointer in inline mode.
 * @param[in]   ep   - the endpoint whose pointer needs to be obtained.
 * @returns     the pointer value of the specified endpoint.
 */
static __attribute__((__always_inline__)) inline unsigned int usbhw_get_ep_ptr_inline(int ep)
{
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
    return usb1hw_get_ep_ptr(ep);
#elif (MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TYPE == MCU_CORE_B91)
    (void)ep;
    return 0;
#else
    return usbhw_get_ep_ptr(ep);
#endif
}
#endif // TLKUSB_STDIO_H
