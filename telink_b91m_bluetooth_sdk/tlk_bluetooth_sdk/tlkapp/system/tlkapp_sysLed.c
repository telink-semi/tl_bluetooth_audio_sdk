/********************************************************************************************************
 * @file    tlkapp_sysLed.c
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
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"

#if (TLK_DEV_LED_ENABLE)


/**
 * @brief       Executes the specified LED pattern.
 * @param[in]   pattern - The LED pattern to execute.
 * @return      none.
 */
static void tlkapp_sysLed_pattern_execute(tlkdrv_led_pattern_e pattern)
{
    tlkdrv_led_patternSelect(GPIO_LED_BLUE, TLKDRV_LED_PATTERN_OFF);
    tlkdrv_led_patternSelect(GPIO_LED_BLUE, pattern);
}

/**
 * @brief     This function initializes the system LEDs.
 * @param[in] None.
 * @returns   None.
 */
__attribute__((weak)) void tlkapp_sysLed_init(void)
{
#ifdef GPIO_LED_BLUE
    tlkdrv_led_insert(GPIO_LED_BLUE, GPIO_LED_BLUE_PWM_ID, LED_ON_LEVEL);
#endif

#ifdef GPIO_LED_RED
    tlkdrv_led_insert(GPIO_LED_RED, GPIO_LED_RED_PWM_ID, LED_ON_LEVEL);
    tlkdrv_led_follow(GPIO_LED_RED, GPIO_LED_BLUE);
#endif
}

/**
 * @brief     This function deinitializes the system LEDs.
 * @param[in] None.
 * @returns   None.
 */
__attribute__((weak)) void tlkapp_sysLed_deInit(void)
{
    tlkdrv_led_removeAll();
}

/**
 * @brief     This function performs an action on the system LEDs based on the given LED state.
 * @param[in] led_fun The LED state that determines the action to be performed.
 * @returns   The result of the operation.
 */
__attribute__((weak)) int tlkapp_sysLed_funAction(TLKAPP_LED_STATE led_fun)
{
    if (led_fun >= TLKAPP_LED_STATE_NUM) {
        return -TLK_EPARAM;
    }
    tlkdrv_led_pattern_e pattern = tlkapp_sysLed_stateToPatternHook(led_fun);
    tlkapp_sysLed_pattern_execute(pattern);
    return TLK_ENONE;
}

/**
 * @brief     This function maps the given LED state to a corresponding LED pattern.
 *            This function can be overridden by the customer to provide custom LED patterns.
 * @param[in] state The LED state to be mapped.
 * @returns   The LED pattern corresponding to the given state.
 * @note      This function is marked as weak, allowing customers to provide their own implementation.
 */
__attribute__((weak)) tlkdrv_led_pattern_e tlkapp_sysLed_stateToPatternHook(TLKAPP_LED_STATE state)
{
    static const tlkdrv_led_pattern_e sTlkapp_sysLed_defaultCfg[TLKAPP_LED_STATE_NUM] = {
        [TLKAPP_LED_STATE_POWERON] = TLKDRV_LED_PATTERN_ON_1S,         [TLKAPP_LED_STATE_POWEROFF] = TLKDRV_LED_PATTERN_ON_1S,
        [TLKAPP_LED_STATE_POWERON_IDLE] = TLKDRV_LED_PATTERN_IDLE,     [TLKAPP_LED_STATE_PARING] = TLKDRV_LED_PATTERN_FLASH_FAST,
        [TLKAPP_LED_STATE_CONNECTED] = TLKDRV_LED_PATTERN_BREATH_SLOW, [TLKAPP_LED_STATE_LOWBATTARY] = TLKDRV_LED_PATTERN_SLOW_FLASH_3_TIMES,
    };
    return sTlkapp_sysLed_defaultCfg[state];
}

#endif //TLK_DEV_LED_ENABLE
