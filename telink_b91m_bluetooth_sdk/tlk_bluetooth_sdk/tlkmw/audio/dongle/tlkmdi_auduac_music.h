/********************************************************************************************************
 * @file    tlkmdi_auduac_music.h
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
#ifndef TLKMDI_AUDUAC_MUSIC_H_
#define TLKMDI_AUDUAC_MUSIC_H_

/**
 * @brief  Initializes the audio UAC music module.
 * @param  None.
 * @return int - Returns 0 if initialization is successful; otherwise an error code.
 */
int  tlkmdi_auduac_music_init(void);

/**
 * @brief  Checks if the UAC music module is busy.
 * @param  None.
 * @return bool - Returns true if the module is busy; otherwise false.
 */
bool tlkmdi_auduac_music_isBusy(void);

/**
 * @brief     Switches the state of the UAC music module.
 * @param[in] handle - The connection handle.
 * @param[in] status - The target status (e.g., TLK_STATE_OPENED or TLK_STATE_CLOSED).
 * @return    bool - Returns true if the switch is successful; otherwise false.
 */
bool tlkmdi_auduac_music_switch(uint16_t handle, uint8_t status);

/**
 * @brief     Operates the UAC music module based on the provided opcode.
 * @param[in] handle - The connection handle.
 * @param[in] opcode - The operation code.
 * @param[in] pData  - Pointer to the data buffer.
 * @param[in] dataLen - Length of the data.
 * @return    bool - Returns true if the operation is successful; otherwise false.
 */
bool tlkmdi_auduac_music_operate(uint16_t handle, uint8_t opcode, uint8_t *pData, uint16_t dataLen);
#endif //TLKMDI_AUDUAC_MUSIC_H_
