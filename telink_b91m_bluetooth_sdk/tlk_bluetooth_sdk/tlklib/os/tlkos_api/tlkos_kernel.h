/********************************************************************************************************
 * @file    tlkos_kernel.h
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
 * @brief     Gets the current interrupt state.
 * @returns   TLKOS_IRQ_STATE_IN_IRQ if in interrupt context, TLKOS_IRQ_STATE_NOT_IN_IRQ otherwise.
 */
int tlkos_get_irqState(void);

/**
 * @brief     Gets the os current kernel state.
 * @returns   Refer to TLKOS_KERNEL_STATE_ENUM.
 */
int tlkos_get_kernelState(void);

/**
 * @brief     Enters a critical section.
 * @returns   None.
 */
void tlkos_enter_critical(void);

/**
 * @brief     Leaves a critical section.
 * @returns   None.
 */
void tlkos_leave_critical(void);

/**
 * @brief     Initializes the kernel.
 * @returns   None.
 */
void tlkos_init(void);

/**
 * @brief     Starts the kernel scheduler.
 * @param[in] initFunc init functions.
 * @returns   None.
 */
void tlkos_start(TlkOsInitFunc_t initFunc);
