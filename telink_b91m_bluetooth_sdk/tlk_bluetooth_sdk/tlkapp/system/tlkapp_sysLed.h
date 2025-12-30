/********************************************************************************************************
 * @file    tlkapp_sysLed.h
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
#include "tlkmw/sys_dev/led/tlkdrv_led.h"
typedef enum
{
    TLKAPP_LED_STATE_POWERON,
    TLKAPP_LED_STATE_POWEROFF,
    TLKAPP_LED_STATE_POWERON_IDLE,
    TLKAPP_LED_STATE_PARING,
    TLKAPP_LED_STATE_CONNECTED,
    TLKAPP_LED_STATE_LOWBATTARY,

    TLKAPP_LED_STATE_NUM,
} TLKAPP_LED_STATE;

/**
 * @brief     This function initializes the system LEDs.
 * @param[in] None.
 * @returns   None.
 */
void tlkapp_sysLed_init(void);

/**
 * @brief     This function deinitializes the system LEDs.
 * @param[in] None.
 * @returns   None.
 */
void tlkapp_sysLed_deInit(void);

/**
 * @brief     This function performs an action on the system LEDs based on the given LED state.
 * @param[in] led_fun The LED state that determines the action to be performed.
 * @returns   The result of the operation.
 */
int tlkapp_sysLed_funAction(TLKAPP_LED_STATE led_fun);

/**
 * @brief     This function maps the given LED state to a corresponding LED pattern.
 *            This function can be overridden by the customer to provide custom LED patterns.
 * @param[in] state The LED state to be mapped.
 * @returns   The LED pattern corresponding to the given state.
 * @note      This function is marked as weak, allowing customers to provide their own implementation.
 */
tlkdrv_led_pattern_e tlkapp_sysLed_stateToPatternHook(TLKAPP_LED_STATE state);




