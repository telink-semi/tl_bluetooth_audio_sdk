/********************************************************************************************************
 * @file    tlksys_port.c
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
#include "tl_common.h"
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlksys_port.h"
#if !MCU_CORE_TL752X_TEMP
#include "stack/pm/pm_sys.h"
#endif


/**
 * @brief  Enter wait-for-interrupt state
 * @param  None.
 * @returns  None.
 */
__attribute__((weak)) void tlksys_enter_wfi(void)
{
#if (MCU_CORE_TYPE != MCU_CORE_B92) && (!PROJ_INTERNAL_TEST) && (MCU_CORE_TYPE != MCU_CORE_TL322X)
    __asm__ __volatile__("wfi");
#else
    delay_us(60); //wfi wake up timer at least 50us
#endif
}

/**
 * @brief  Enter suspend mode
 * @param[in] timeMs : Suspend duration in milliseconds
 * @param[in] cfg : Pointer to suspend configuration
 * @returns  True - Successfully entered. False - Failed to enter suspend mode
 */
__attribute__((weak)) bool tlksys_enter_suspend(uint32_t timeMs, const TlkSuspendCfg_t *cfg)
{
    (void)cfg;
    (void)timeMs;
#if (TLK_CFG_SUSPEND_ENABLE && (!MCU_CORE_TL752X_TEMP))
    tlkapi_printf(0, "[PM] tlksdk_pm_enterSleep: %d Ms\r\n", timeMs);
    uint32_t nxt_task_wakeup_tick = clock_time() + timeMs * SYSTEM_TIMER_TICK_1MS;
    uint32_t reval                = tlksdk_pm_enterSleep(SUSPEND_MODE, nxt_task_wakeup_tick);
    if (reval == 0) {
        return true;
    }
#endif
    return false;
}

#if (MCU_CORE_TL752X_TEMP)
/**
 * @brief  Initialize debug GPIOs
 * @param[in] enable : Enable flag for debug GPIOs
 * @returns  None.
 */
void tlksys_debug_gpio_init(uint8_t enable)
{
    if (enable) {
        gpio_function_en(GPIO_PB7);
        gpio_output_en(GPIO_PB7);
        gpio_set_low_level(GPIO_PB7);

        gpio_function_en(GPIO_PB8);
        gpio_output_en(GPIO_PB8);
        gpio_set_low_level(GPIO_PB8);

        gpio_function_en(GPIO_PB9);
        gpio_output_en(GPIO_PB9);
        gpio_set_low_level(GPIO_PB9);

        gpio_function_en(GPIO_PB10);
        gpio_output_en(GPIO_PB10);
        gpio_set_low_level(GPIO_PB10);

        gpio_function_en(GPIO_PB11);
        gpio_output_en(GPIO_PB11);
        gpio_set_low_level(GPIO_PB11);

        gpio_function_en(GPIO_PB12);
        gpio_output_en(GPIO_PB12);
        gpio_set_low_level(GPIO_PB12);

        gpio_function_en(GPIO_PB13);
        gpio_output_en(GPIO_PB13);
        gpio_set_low_level(GPIO_PB13);
    }
}
#endif

/**
 * @brief  Initialize port module
 * @param  None.
 * @returns  None.
 */
__attribute__((weak)) void tlksys_port_init(void)
{
#if (MCU_CORE_TL752X_TEMP)
    tlksys_debug_gpio_init(true);
#endif
}
