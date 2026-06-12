/********************************************************************************************************
 * @file    tlkhal_gpio_TL753X.c
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
#if MCU_CORE_TYPE == MCU_CORE_TL753X

#define GPIO_IRQ_CHN_NUM 8

typedef struct
{
    uint8_t           isUsed;
    uint8_t           triggerMode;
    uint16_t          gpio;
    TlkHalGpioIrqCB_t cb;
} IrqChnInfo_t;

static IrqChnInfo_t sTlkhalGpioIrqChnInfo[GPIO_IRQ_CHN_NUM] = {0};

static inline gpio_irq_num_e tlkhal_gpio_chn2DriverEnum(uint8_t chn)
{
    return chn;
}

static inline gpio_irq_trigger_type_e tlkhal_gpio_triggerMode2DriverEnum(uint8_t triggerMode)
{
    return triggerMode % 2;
}

static inline unsigned int tlkhal_gpio_chn2DriverIrqSrc(uint8_t chn)
{
    return chn + IRQ_GPIO_IRQ0;
}

static inline gpio_irq_e tlkhal_gpio_chn2DriverIrqEnum(uint8_t chn)
{
    return 1 << (chn);
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
 * @brief  Shut down all GPIOs
 * @param  None.
 * @returns  None.
 */
void tlkhal_gpio_allShutDown(void)
{
    usb_dp_pullup_en(0);
    usb1_dp_pullup_en(0);
    gpio_shutdown(GPIO_ALL);
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
    if (((cfg->gpio >> 8) & 0xff) >= GPIO_GROUP_H) { //PI PH not support function gpio_set_up_down_res
        switch (cfg->pullUpDownCfg) {
        case TLKHAL_GPIO_PULL_UP_DEFAULT:
        {
            gpio_set_digital_pullup(cfg->gpio);
        } break;
        case TLKHAL_GPIO_PULL_DOWN_DEFAULT:
        {
            gpio_set_digital_pulldown(cfg->gpio);
        } break;
        }
    } else {
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
    if (cfg->pmWakeUpEn) {
        uint8_t level = cfg->pmWakeUpLevel ? 1 : 0;
        pm_set_gpio_wakeup(cfg->gpio, level, 1);
    } else {
        pm_set_gpio_wakeup(cfg->gpio, 0, 0);
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
        sTlkhalGpioIrqChnInfo[chnIndex].cb          = cfg->cb;
        sTlkhalGpioIrqChnInfo[chnIndex].gpio        = cfg->gpio;
        sTlkhalGpioIrqChnInfo[chnIndex].triggerMode = cfg->triggerMode;
        if (chn != NULL) {
            *chn = chnIndex;
        }
        gpio_irq_num_e          irqNum  = tlkhal_gpio_chn2DriverEnum(chnIndex);
        gpio_irq_trigger_type_e trigger = tlkhal_gpio_triggerMode2DriverEnum(cfg->triggerMode);
        gpio_set_irq(irqNum, cfg->gpio, trigger);
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
    gpio_irq_e   irqE   = tlkhal_gpio_chn2DriverIrqEnum(chn);
    unsigned int irqSrc = tlkhal_gpio_chn2DriverIrqSrc(chn);
    gpio_set_irq_mask(irqE);
    plic_interrupt_enable(irqSrc);
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
    unsigned int irqSrc = tlkhal_gpio_chn2DriverIrqSrc(chn);
    gpio_irq_e   irqE   = tlkhal_gpio_chn2DriverIrqEnum(chn);
    gpio_clr_irq_mask(irqE);
    plic_interrupt_disable(irqSrc);
    return true;
}

/**
 * @brief  GPIO interrupt handler
 * @param[in] chn : Channel number to handle
 * @returns  None.
 */
void tlkhal_gpio_irqHandler(uint8_t chn)
{
    gpio_irq_e irqE = tlkhal_gpio_chn2DriverIrqEnum(chn);
    gpio_clr_irq_status(irqE);
    IrqChnInfo_t *pInfo = &sTlkhalGpioIrqChnInfo[chn];
    if (pInfo->isUsed == 0 || pInfo->cb == NULL) {
        return;
    }
    pInfo->cb();
}

#endif
