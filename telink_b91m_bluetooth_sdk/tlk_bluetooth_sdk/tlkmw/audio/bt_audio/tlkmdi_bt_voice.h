/********************************************************************************************************
 * @file    tlkmdi_bt_voice.h
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
#ifndef TLKMDI_BT_VOICE_H
#define TLKMDI_BT_VOICE_H

/**
 * @brief  initial bt voice module
 * @param[in]  none
 * @returns initialization result, TLK_ENONE means success
 */
int  tlkmdi_bt_voice_init(void);

/**
 * @brief  disable bt voice
 * @param[in]  none
 * @returns none
 */
int  tlkmdi_bt_voice_disable(void);

/**
 * @brief  switch bt voice state
 * @param[in]  handle - connection handle
 * @param[in]  status - switch status
 * @returns operation result
 */
bool tlkmdi_bt_voice_switch(uint16_t handle, uint8_t status);

/**
 * @brief  check if bt voice is busy
 * @param[in]  none
 * @returns busy status
 */
bool tlkmdi_bt_voice_is_busy(void);

/**
 * @brief  operate bt voice
 * @param[in]  handle  - connection handle
 * @param[in]  opcode  - operation code
 * @param[in]  pdata   - operation data
 * @param[in]  dataLen - data length
 * @returns operation result
 */
bool tlkmdi_bt_voice_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief  enable bt voice
 * @param[in]  p_data - data pointer
 * @param[in]  len    - data length
 * @returns enable result
 */
int tlkmdi_bt_voice_enable(uint8_t *p_data, uint16_t len);

#endif // TLKMDI_BT_VOICE_H
