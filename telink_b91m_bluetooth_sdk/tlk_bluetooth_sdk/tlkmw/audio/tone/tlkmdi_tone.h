/********************************************************************************************************
 * @file    tlkmdi_tone.h
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
#ifndef TLKMDI_TONE_H
#define TLKMDI_TONE_H

#include "tone.h"

/**
 * @brief       Initializes the tone module.
 * @return      int - Status of initialization (0 for success, non-zero for failure).
 */
int  tlkmdi_tone_init(void);

/**
 * @brief       Switches the tone state.
 * @param[in]   handle - The handle associated with the tone.
 * @param[in]   status - The desired status to switch to.
 * @return      bool - True if the switch was successful, false otherwise.
 */
bool tlkmdi_tone_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Checks if the tone module is busy.
 * @return      bool - True if the tone module is busy, false otherwise.
 */
bool tlkmdi_tone_is_busy(void);

/**
 * @brief       Starts the tone with the given parameters.
 * @param[in]   handle - The handle associated with the tone.
 * @param[in]   param - Parameters for starting the tone.
 * @return      int - Status of the operation (0 for success, non-zero for failure).
 */
int  tlkmdi_tone_start(uint16_t handle, uint32_t param);

/**
 * @brief       Performs an operation on the tone.
 * @param[in]   handle - The handle associated with the tone.
 * @param[in]   opcode - The operation code.
 * @param[in]   pdata - Pointer to the data for the operation.
 * @param[in]   dataLen - Length of the data.
 * @return      bool - True if the operation was successful, false otherwise.
 */
bool tlkmdi_tone_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

#endif // TLKMDI_TONE_H
