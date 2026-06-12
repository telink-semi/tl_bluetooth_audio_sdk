/********************************************************************************************************
 * @file    tlkmdi_bt_ctrl.h
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

typedef void (*TlkMdiBtCtrlFinishedCB)(void);

/**
 * @brief Force bt to enter the idle state.
 *        This function stops bt scanning and disables Bluetooth reconnection. 
 *        Upon completion, the provided callback function will be invoked.
 *
 * @param[in] finishedCB  The callback function pointer that will be called upon completion of the operation.
 *                        - If the callback function is not needed, pass NULL.
 *                        - The callback function should be a valid function pointer to avoid undefined behavior.
 *
 * @return The process result,TLK_ENONE means success, others mean fail.
 */
int tlkmdi_bt_forceToIdle(TlkMdiBtCtrlFinishedCB finishedCB);

/**
 * @brief Get bt state is ready.
 *        Not ready means bt host has not init or is in deinit/paused/force2idle process. 
 *
 * @param[in] none.
 * 
 * @return True means ready,false means not ready.
 */
bool tlkmdi_bt_isReady(void);
