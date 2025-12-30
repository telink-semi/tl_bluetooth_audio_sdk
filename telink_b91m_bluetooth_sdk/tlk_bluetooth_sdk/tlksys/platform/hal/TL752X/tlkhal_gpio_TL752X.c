/********************************************************************************************************
 * @file    tlkhal_gpio_TL752X.c
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
#if MCU_CORE_TYPE == CHIP_TYPE_TL752X
#define GPIO_IRQ_CHN_NUM 6

typedef struct
{
    uint8_t           isUsed    : 4;
    uint8_t           isStarted : 4;
    uint8_t           triggerMode;
    uint16_t          gpio;
    TlkHalGpioIrqCB_t cb;
} IrqChnInfo_t;

static IrqChnInfo_t sTlkhalGpioIrqChnInfo[GPIO_IRQ_CHN_NUM] = {0};
static uint8_t      sTlkHalGpioIrqStartNum                  = 0;

static inline uint16_t tlkhal_gpio_triggerMode2DriverEnum(uint8_t triggerMode)
{
    return triggerMode % 2 + 1;
}

/**
 * @brief  Set GPIO pull-up/down resistor
 * @param[in] gpio_pin : GPIO pin number
 * @param[in] res : Pull-up/down configuration
 * @returns  None.
 */
void tlkhal_gpio_set_up_down_res(uint16_t gpio_pin, TlkhalGpioPullUpDownCfg_e res)
{
    gpio_pull_type_e type = 0;
    switch (res) {
    case TLKHAL_GPIO_PULL_UP_DEFAULT:
        type = GPIO_PIN_PULLUP;
        break;
    case TLKHAL_GPIO_PULL_DOWN_DEFAULT:
        type = GPIO_PIN_PULLDOWN;
        break;
    case TLKHAL_GPIO_PULL_FLOAT:
        type = GPIO_NOPULL;
        break;
    }
    gpio_set_up_down_res(gpio_pin, type);
}

/**
 * @brief  Shut down all GPIOs
 * @param  None.
 * @returns  None.
 */
void tlkhal_gpio_allShutDown(void)
{
    //    gpio_shutdown(GPIO_ALL);
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
        gpio_set_up_down_res(cfg->gpio, GPIO_PIN_NO_PULL);
    } break;
    case TLKHAL_GPIO_PULL_UP_DEFAULT:
    {
        gpio_set_up_down_res(cfg->gpio, GPIO_PIN_PULLUP);
    } break;
    case TLKHAL_GPIO_PULL_DOWN_DEFAULT:
    {
        gpio_set_up_down_res(cfg->gpio, GPIO_PIN_PULLDOWN);
    } break;
    }
    if (cfg->pmWakeUpEn) {
        //        uint8_t level = cfg->pmWakeUpLevel ? 1 : 0;
        //        pm_set_gpio_wakeup(cfg->gpio, level, 1);
    } else {
        //        pm_set_gpio_wakeup(cfg->gpio, 0, 0);
    }
}

/**
 * @brief  Configure GPIO as input
 * @param[in] cfg : Pointer to GPIO input configuration
 * @returns  None.
 */
bool tlkhal_gpio_mallocIrqChn(uint8_t *chn, const TlkhalGpioIrqChnCfg_t *cfg)
{
    for (uint8_t chnIndex = 0; chnIndex < GPIO_IRQ_CHN_NUM; chnIndex++) {
        if (sTlkhalGpioIrqChnInfo[chnIndex].isUsed == 1) {
            continue;
        }
        //find idle index.
        sTlkhalGpioIrqChnInfo[chnIndex].isUsed      = 1;
        sTlkhalGpioIrqChnInfo[chnIndex].isStarted   = 0;
        sTlkhalGpioIrqChnInfo[chnIndex].cb          = cfg->cb;
        sTlkhalGpioIrqChnInfo[chnIndex].gpio        = cfg->gpio;
        sTlkhalGpioIrqChnInfo[chnIndex].triggerMode = cfg->triggerMode;
        if (chn != NULL) {
            *chn = chnIndex;
        }
        return true;
    }
    return false;
}

/**
 * @brief  Free a GPIO interrupt channel
 * @param[in] chn : Channel number to free
 * @returns  Free result,true - Success,false - Failure
 */
bool tlkhal_gpio_freeIrqChn(uint8_t chn)
{
    bool res = tlkhal_gpio_stopIrqChn(chn);
    if (res == false) {
        return false;
    }
    memset(&sTlkhalGpioIrqChnInfo[chn], 0, sizeof(IrqChnInfo_t));
    return true;
}

/**
 * @brief  Start a GPIO interrupt channel
 * @param[in] chn : Channel number to start
 * @returns  Start result, true - Success,false - Failure
 */
bool tlkhal_gpio_startIrqChn(uint8_t chn)
{
    if (chn >= GPIO_IRQ_CHN_NUM || sTlkhalGpioIrqChnInfo[chn].isUsed == 0) {
        return false;
    }
    if (sTlkhalGpioIrqChnInfo[chn].isStarted == 1) {
        return true;
    }
    sTlkhalGpioIrqChnInfo[chn].isStarted = 1;
    if (sTlkHalGpioIrqStartNum == 0) {
        plic_interrupt_enable(IRQ_GPIO);
    }
    gpio_set_irq(sTlkhalGpioIrqChnInfo[chn].gpio, tlkhal_gpio_triggerMode2DriverEnum(sTlkhalGpioIrqChnInfo[chn].triggerMode));
    // gpio_set_irq(sTlkhalGpioIrqChnInfo[chn].gpio,1); //TODO  clean later
    sTlkHalGpioIrqStartNum++;
    return true;
}

/**
 * @brief  Stop a GPIO interrupt channel
 * @param[in] chn : Channel number to stop
 * @returns  Stop result,true - Success,false - Failure
 */
bool tlkhal_gpio_stopIrqChn(uint8_t chn)
{
    if (chn >= GPIO_IRQ_CHN_NUM || sTlkhalGpioIrqChnInfo[chn].isUsed == 0) {
        return false;
    }
    if (sTlkhalGpioIrqChnInfo[chn].isStarted == 0) {
        return true;
    }
    sTlkhalGpioIrqChnInfo[chn].isStarted = 0;
    sTlkHalGpioIrqStartNum--;
    gpio_irq_dis(sTlkhalGpioIrqChnInfo[chn].gpio);
    if (sTlkHalGpioIrqStartNum == 0) {
        plic_interrupt_disable(IRQ_GPIO);
    }
    return true;
}

/**
 * @brief  GPIO interrupt handler
 * @param[in] chn : Channel number to handle
 * @returns  None.
 */
void tlkhal_gpio_irqHandler(uint8_t chn)
{
    (void)chn;
    for (size_t i = 0; i < GPIO_IRQ_CHN_NUM; i++) {
        IrqChnInfo_t *pInfo = &sTlkhalGpioIrqChnInfo[i];
        if (pInfo->isStarted == 0 || pInfo->cb == NULL) {
            continue;
        }
        if (gpio_get_irq_status(pInfo->gpio)) {
            gpio_clr_irq_status(pInfo->gpio);
            pInfo->cb();
        }
    }
}
#endif
