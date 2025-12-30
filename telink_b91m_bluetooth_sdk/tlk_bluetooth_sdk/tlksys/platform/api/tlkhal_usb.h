/********************************************************************************************************
 * @file    tlkhal_usb.h
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
#pragma once

typedef enum
{
    TLKUSB_MODTYPE_UDB = 0,
    TLKUSB_MODTYPE_UAC,
    TLKUSB_MODTYPE_MSC,
    TLKUSB_MODTYPE_CDC,
    TLKUSB_MODTYPE_UAC_HS,
    TLKUSB_MODTYPE_MAX,
} TLKHAL_USB_MODE_ENUM;

/**
 * @brief  Initialize USB hardware
 * @param[in] port_id : USB port ID
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_hw_init(uint8_t port_id, void *userArg);

/**
 * @brief  Initialize USB function
 * @param[in] port_id : USB port ID
 * @param[in] usb_mode : USB mode type
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_function_init(uint8_t port_id, TLKHAL_USB_MODE_ENUM usb_mode, void *userArg);

/**
 * @brief  Reset USB
 * @param[in] port_id : USB port ID
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_reset(uint8_t port_id, void *userArg);

/**
 * @brief  Handle USB unplug event
 * @param[in] port_id : USB port ID
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_unpluged(uint8_t port_id, void *userArg);

/**
 * @brief  Stall USB endpoint
 * @param[in] port_id : USB port ID
 * @param[in] ep_addr : Endpoint address
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_ep_stall(uint8_t port_id, uint8_t ep_addr, void *userArg);

/**
 * @brief  Read data from USB endpoint
 * @param[in] port_id : USB port ID
 * @param[in] ep_addr : Endpoint address
 * @param[in] data : Data buffer to read into
 * @param[in] len : Length of data to read
 * @param[in] userArg : User argument
 * @returns  Result of read operation.
 */
int tlkhal_usb_read_ep_data(uint8_t port_id, uint8_t ep_addr, uint8_t *data, uint16_t len, void *userArg);

/**
 * @brief  Write data to USB endpoint
 * @param[in] port_id : USB port ID
 * @param[in] ep_addr : Endpoint address
 * @param[in] data : Data buffer to write from
 * @param[in] len : Length of data to write
 * @param[in] userArg : User argument
 * @returns  Result of write operation.
 */
int tlkhal_usb_write_ep_data(uint8_t port_id, uint8_t ep_addr, uint8_t *data, uint16_t len, void *userArg);

/**
 * @brief  USB interrupt handler
 * @param[in] port_id : USB port ID
 * @returns  None.
 */
void tlkhal_usb_irq_handler(uint8_t port_id);