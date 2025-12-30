/********************************************************************************************************
 * @file    tlkusb.h
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
#ifndef TLKUSB_H
#define TLKUSB_H

#include "tlkusb_hal.h"
#include "tlksys/platform/api/tlkhal_usb.h"

/**
 * @brief       This function deinitializes the USB module with the specified index.
 * @param[in]   index   - the USB port index to deinitialize.
 * @returns     none.
 */
void tlkusb_deInit(uint8_t index);

/**
 * @brief       This function initializes the USB module with the specified index and USB ID.
 * @param[in]   index   - the USB port index to initialize.
 * @param[in]   usbID   - the USB ID to set.
 * @returns     TLK_ENONE if successful, -TLK_EPARAM if the index is invalid.
 */
int  tlkusb_init(uint8_t index, uint16_t usbID);

/**
 * @brief       This function sets the USB module with the specified mode type.
 * @param[in]   index    - the USB port index.
 * @param[in]   modType  - the USB mode type to set.
 * @returns     true if the module is successfully set, false otherwise.
 */
bool tlkusb_setModule(uint8_t index, TLKHAL_USB_MODE_ENUM modType);

/**
 * @brief       This function opens the USB module with the specified mode type.
 * @param[in]   index    - the USB port index to open.
 * @param[in]   modType  - the USB mode type to set.
 * @returns     TLK_ENONE if successful, error code otherwise.
 */
int tlkusb_open(uint8_t index, TLKHAL_USB_MODE_ENUM modType);

/**
 * @brief       This function closes the USB module with the specified index.
 * @param[in]   index   - the USB port index to close.
 * @returns     TLK_ENONE if successful.
 */
int tlkusb_close(uint8_t index);

/**
 * @brief       This function handles USB events for all USB ports.
 * @param       none.
 * @returns     none.
 */
void tlkusb_handler(void);

/**
 * @brief       This function handles USB interrupt requests.
 * @param       index   - the USB port index.
 * @returns     none.
 */
void tlkusb_irqHandler(uint8_t index);


#endif // TLKUSB_H
