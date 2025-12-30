/********************************************************************************************************
 * @file    tlksys_port.h
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

typedef struct
{
    uint16_t isPullUp : 1;  /**< Pull-up resistor enable flag */
    uint16_t isInPut  : 1;  /**< Input/output flag (1: input, 0: output) */
    uint16_t resv     : 14; /**< Reserved field */
    uint16_t gpio;          /**< GPIO pin number */
} TlkSuspendIOCfg_t;

/**
 * @struct TlkSuspendCfg_t
 * @brief  Suspend configuration structure
 * This structure defines the overall suspend configuration including IO configurations.
 */
typedef struct
{
    TlkSuspendIOCfg_t *ioCfg;   /**< Pointer to IO configuration array */
    uint32_t           gpioNum; /**< Number of GPIO pins in the array */
} TlkSuspendCfg_t;

/**
 * @brief  Initialize port module
 * @param  None.
 * @returns  None.
 */
void tlksys_port_init(void);

/**
 * @brief  Enter wait-for-interrupt state
 * @param  None.
 * @returns  None.
 */
void tlksys_enter_wfi(void);

/**
 * @brief  Enter suspend mode
 * @param[in] timeMs : Suspend duration in milliseconds
 * @param[in] cfg : Pointer to suspend configuration
 * @returns  Result of suspend operation
 * @retval   true - Successfully entered and exited suspend mode
 * @retval   false - Failed to enter suspend mode
 */
bool tlksys_enter_suspend(uint32_t timeMs, const TlkSuspendCfg_t *cfg);
