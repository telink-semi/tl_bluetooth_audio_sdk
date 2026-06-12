/********************************************************************************************************
 * @file    tlkhal_gpio.h
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

typedef void (*TlkHalGpioIrqCB_t)(void);

typedef enum
{
    TLKHAL_GPIO_IRQ_TRIGGER_RISING_EDGE,
    TLKHAL_GPIO_IRQ_TRIGGER_FALLING_EDGE,
} TlkhalGpioIrqTriggerMode_e;

typedef enum
{
    TLKHAL_GPIO_PULL_FLOAT,
    TLKHAL_GPIO_PULL_UP_DEFAULT,
    TLKHAL_GPIO_PULL_DOWN_DEFAULT,
} TlkhalGpioPullUpDownCfg_e;

typedef struct
{
    uint8_t  pmWakeUpEn    : 4;
    uint8_t  pmWakeUpLevel : 4;
    uint8_t  pullUpDownCfg; //refer to TlkhalGpioPullUpDownCfg_e
    uint16_t gpio;
} TlkhalGpioInputCfg_t;

typedef struct
{
    uint8_t           triggerMode;
    uint8_t           resv;
    uint16_t          gpio;
    TlkHalGpioIrqCB_t cb;
} TlkhalGpioIrqChnCfg_t;

/**
 * @brief  Shut down all GPIOs
 * @param  None.
 * @returns  None.
 */
void tlkhal_gpio_allShutDown(void);

/**
 * @brief  Configure GPIO as input
 * @param[in] cfg : Pointer to GPIO input configuration
 * @returns  None.
 */
void tlkhal_gpio_setInput(const TlkhalGpioInputCfg_t *cfg);

/**
 * @brief  Configure GPIO as input
 * @param[in] cfg : Pointer to GPIO input configuration
 * @returns  None.
 */
bool tlkhal_gpio_mallocIrqChn(uint8_t *chn, const TlkhalGpioIrqChnCfg_t *cfg);

/**
 * @brief  Free a GPIO interrupt channel
 * @param[in] chn : Channel number to free
 * @returns  Free result,true - Success,false - Failure
 */
bool tlkhal_gpio_freeIrqChn(uint8_t chn);

/**
 * @brief  Start a GPIO interrupt channel
 * @param[in] chn : Channel number to start
 * @returns  Start result, true - Success,false - Failure
 */
bool tlkhal_gpio_startIrqChn(uint8_t chn);

/**
 * @brief  Stop a GPIO interrupt channel
 * @param[in] chn : Channel number to stop
 * @returns  Stop result,true - Success,false - Failure
 */
bool tlkhal_gpio_stopIrqChn(uint8_t chn);

/**
 * @brief  GPIO interrupt handler
 * @param[in] chn : Channel number to handle
 * @returns  None.
 */
void tlkhal_gpio_irqHandler(uint8_t chn);

/**
 * @brief  Set GPIO pull-up/down resistor
 * @param[in] gpio_pin : GPIO pin number
 * @param[in] res : Pull-up/down configuration
 * @returns  None.
 */
void tlkhal_gpio_set_up_down_res(uint16_t gpio_pin, TlkhalGpioPullUpDownCfg_e res);
