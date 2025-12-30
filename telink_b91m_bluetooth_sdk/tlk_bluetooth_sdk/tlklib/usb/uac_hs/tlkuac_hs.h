/********************************************************************************************************
 * @file    tlkuac_hs.h
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
#ifndef TLKUAC_HS_H
#define TLKUAC_HS_H
#include "../tlkusb_struct.h"

/**
 * @brief       This function handles UAC HS events and reports mic and spk status to upper layer.
 * @return      none.
 */
void tlkusb_uachs_handler(void);

/**
 * @brief       Process USB control requests based on request type
 * @param[in]   bus - USB bus number
 * @param[in]   setup - Pointer to USB control request structure
 * @param[in]   setup_stage - Current stage of setup request processing
 * @return      Returns true if request type is vendor-specific or reserved, false for standard and class requests
 *              which are handled by their respective handlers
 */
unsigned char usbd_control_request_process(const unsigned char bus, const usb_control_request_t *setup, unsigned char setup_stage);

#endif // TLKUAC_HS_H
