/********************************************************************************************************
 * @file    tlkhal_gpio_B91.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "../../api/tlkhal_api.h"
#include "drivers.h"
#if MCU_CORE_TYPE == CHIP_TYPE_B91
/**
 * @brief  Shut down all GPIOs
 * @param  None.
 * @returns  None.
 */
void tlkhal_gpio_allShutDown(void)
{
    gpio_shutdown(GPIO_ALL);
}

/**
 * @brief  Set GPIO pull-up/down resistor
 * @param[in] gpio_pin : GPIO pin number
 * @param[in] res : Pull-up/down configuration
 * @returns  None.
 */
void tlkhal_gpio_set_up_down_res(uint16_t gpio_pin, TlkhalGpioPullUpDownCfg_e res)
{
    gpio_set_up_down_res(gpio_pin, (gpio_pull_type_e)res);
}

/**
 * @brief  Configure GPIO as input
 * @param[in] cfg : Pointer to GPIO input configuration
 * @returns  None.
 */
void tlkhal_gpio_setInput(const TlkhalGpioInputCfg_t *cfg)
{
    gpio_function_en(cfg->gpio);
    gpio_output_dis(cfg->gpio);
    gpio_input_en(cfg->gpio);
    switch (cfg->pullUpDownCfg) {
    case TLKHAL_GPIO_PULL_FLOAT:
    {
        gpio_set_up_down_res(cfg->gpio, GPIO_PIN_UP_DOWN_FLOAT);
    } break;
    case TLKHAL_GPIO_PULL_UP_DEFAULT:
    {
        gpio_set_up_down_res(cfg->gpio, GPIO_PIN_PULLUP_1M);
    } break;
    case TLKHAL_GPIO_PULL_DOWN_DEFAULT:
    {
        gpio_set_up_down_res(cfg->gpio, GPIO_PIN_PULLDOWN_100K);
    } break;
    }
}

/**
 * @brief  Configure GPIO as input
 * @param[in] cfg : Pointer to GPIO input configuration
 * @returns  None.
 */
bool tlkhal_gpio_mallocIrqChn(uint8_t *chn, const TlkhalGpioIrqChnCfg_t *cfg)
{
    (void)chn;
    (void)cfg;
    return false;
}

/**
 * @brief  Free a GPIO interrupt channel
 * @param[in] chn : Channel number to free
 * @returns  Free result,true - Success,false - Failure
 */
bool tlkhal_gpio_freeIrqChn(uint8_t chn)
{
    (void)chn;
    return false;
}

/**
 * @brief  Start a GPIO interrupt channel
 * @param[in] chn : Channel number to start
 * @returns  Start result, true - Success,false - Failure
 */
bool tlkhal_gpio_startIrqChn(uint8_t chn)
{
    (void)chn;
    return false;
}

/**
 * @brief  Stop a GPIO interrupt channel
 * @param[in] chn : Channel number to stop
 * @returns  Stop result,true - Success,false - Failure
 */
bool tlkhal_gpio_stopIrqChn(uint8_t chn)
{
    (void)chn;
    return false;
}

/**
 * @brief  GPIO interrupt handler
 * @param[in] chn : Channel number to handle
 * @returns  None.
 */
void tlkhal_gpio_irqHandler(uint8_t chn)
{
    (void)chn;
}

#endif
