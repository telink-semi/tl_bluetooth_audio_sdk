/********************************************************************************************************
 * @file    tlkmdi_local_uac_audio.h
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

#ifndef TLKMDI_LOCAL_UAC_AUDIO_H_
#define TLKMDI_LOCAL_UAC_AUDIO_H_

/**
 * @brief  Initializes the audio UAC music module.
 * @param  None.
 * @return int - Returns 0 if initialization is successful; otherwise an error code.
 */
int tlkmdi_local_uac_audio_init(void);

/**
 * @brief     Switches the state of the UAC music module.
 * @param[in] handle - The connection handle.
 * @param[in] status - The target status (e.g., TLK_STATE_OPENED or TLK_STATE_CLOSED).
 * @return    bool - Returns true if the switch is successful; otherwise false.
 */
bool tlkmdi_local_uac_audio_switch(uint16_t handle, uint8_t status);

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
uint tlkmdi_local_uac_audio_intval(void);

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
int tlkmdi_local_uac_audio_start(uint16_t handle, uint32_t param);

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
int tlkmdi_local_uac_audio_close(uint16_t handle);

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
bool tlkmdi_local_uac_audio_next(void);

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
bool tlkmdi_local_uac_audio_prev(void);

/**
 * @brief  This function is used to get the interval between two audio UAC music module interrupts.
 * @param  None.
 * @return uint - Returns the interval between two audio UAC music module interrupts.
 */
bool tlkmdi_local_uac_audio_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief  Checks if the UAC music module is busy.
 * @param  None.
 * @return bool - Returns true if the module is busy; otherwise false.
 */
bool tlkmdi_local_uac_audio_isBusy(void);

/**
 * @brief  Speaker handler for UAC music module.
 * @param  None.
 * @return None.
 */
void tlkmdi_local_uac_audio_spkHandler(void);

/**
 * @brief  Microphone handler for UAC music module.
 * @param  None.
 * @return None.
 */
void tlkmdi_local_uac_audio_micHandler(void);

#endif /* TLKMDI_LOCAL_UAC_AUDIO_H_ */
