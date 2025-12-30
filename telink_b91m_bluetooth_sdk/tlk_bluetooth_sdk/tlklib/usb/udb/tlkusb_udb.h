/********************************************************************************************************
 * @file    tlkusb_udb.h
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

/**
 * @brief   Check if USB sending is busy.
 * @param   None.
 * @return  Returns true if USB sending is busy, false otherwise.
 */
bool tlkusb_udb_isSendBusy(void);

/**
 * @brief   Send data through USB.
 * @param   pData Pointer to the data to send.
 * @param   dataLen Length of the data to send.
 * @return  TLK_ENONE if successful, error code otherwise.
 */
int tlkusb_udb_sendData(uint8_t *pData, uint8_t dataLen);

/**
 * @brief   Hook function for EQ configuration.
 * @param   pData Pointer to the data.
 * @param   dataLen Length of the data.
 * @return  None.
 */
void tlkusb_eq_cfg_hook(uint8_t *pData, uint16_t dataLen);

/**
 * @brief     This function serves as a hook for the USB shell, allowing users to process USB input data.
 * @param[in] pData Pointer to the USB input data buffer.
 * @param[in] dataLen Length of the USB input data.
 * @returns   None.
 * @note      Users can overload this function to implement custom processing logic for USB input data.
 */
void tlkusb_debug_shell_hook(uint8_t *pData, uint16_t dataLen);
