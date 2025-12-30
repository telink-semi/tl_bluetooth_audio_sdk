/********************************************************************************************************
 * @file    tlkmdi_sidetone.h
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
#ifndef TLKMDI_SIDETONE_H_
#define TLKMDI_SIDETONE_H_

/**
 * @brief     Sets the microphone channel for sidetone processing.
 * @param[in] channel The audio channel to set, either left or right.
 * @returns   None
 */
void tlkmdi_sidetone_set_mic_chanl(uint8_t channel);

/**
 * @brief  Initializes the sidetone module.
 * @param  None
 * @return Returns TLK_ENONE on success.
 */
int tlkmdi_sidetone_init(void);

/**
 * @brief     Switches the sidetone state based on the provided status.
 * @param[in] handle The handle associated with the sidetone operation.
 * @param[in] status The desired state of the sidetone (open or closed).
 * @return    Returns true if the operation is successful.
 */
bool tlkmdi_sidetone_switch(uint16_t handle, uint8_t status);

/**
 * @brief  Checks if the sidetone module is busy.
 * @param  None
 * @return Returns true if the sidetone module is busy; otherwise, returns false.
 */
bool tlkmdi_sidetone_is_busy(void);

#endif
