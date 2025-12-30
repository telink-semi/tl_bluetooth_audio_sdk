/********************************************************************************************************
 * @file    tlkhal_usb_TL751X.c
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

#if MCU_CORE_TYPE == MCU_CORE_TL751X
/*Support two fs usbs. USB0 is used for audio, USB1 is used for debug.*/
/**
 * @brief  Initialize USB hardware
 * @param[in] port_id : USB port ID
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_hw_init(uint8_t port_id, void *userArg)
{
    (void)port_id;
    (void)userArg;
}
/**
 * @brief  Initialize USB function
 * @param[in] port_id : USB port ID
 * @param[in] usb_mode : USB mode type
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_function_init(uint8_t port_id, TLKHAL_USB_MODE_ENUM usb_mode, void *userArg)
{
    (void)port_id;
    (void)usb_mode;
    (void)userArg;
}
/**
 * @brief  USB interrupt handler
 * @param[in] port_id : USB port ID
 * @returns  None.
 */
void tlkhal_usb_irq_handler(uint8_t port_id)
{
    (void)port_id;
}

#endif
