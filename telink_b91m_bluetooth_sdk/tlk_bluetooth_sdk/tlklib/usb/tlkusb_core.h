/********************************************************************************************************
 * @file    tlkusb_core.h
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
#ifndef TLKUSB_CORE_H
#define TLKUSB_CORE_H

#include "common/types.h"
#include "tlkusb_hal.h"

#ifndef SL_STACK_VCD_EN
    #define SL_STACK_VCD_EN 1
#endif

/**
 * @brief       This function initializes the USB core module with the specified USB ID.
 * @param[in]   index   - the USB instance index.
 * @param[in]   usbID   - the USB identifier.
 * @return      TLK_ENONE on success, otherwise is failure.
 */
int    tlkusb_core_init(uint8_t index, uint16_t usbID);

/**
 * @brief       This function handles USB core events and processes control transfers.
 * @param[in]   index   - the USB instance index.
 * @return      none.
 */
void   tlkusb_core_handler(uint8_t index);

/**
 * @brief       This function gets the current mode of the USB module.
 * @param[in]   index   - the USB instance index.
 * @return      the current mode of the USB module.
 */
uint8_t tlkusb_get_curMode(uint8_t index);


#endif // TLKUSB_CORE_H
