/********************************************************************************************************
 * @file    app_rc_ui_led.c
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
#include "tlkmw/sys_dev/tlkmw_sysdev.h"
#include "tlkmw/host/tlkmw_host.h"
#include "tlkapp/system/tlkapp_sysLed.h"
#if TLK_DEV_LED_ENABLE
void tlkapp_sysLed_init(void) {}

int tlkapp_sysLed_funAction(TLKAPP_LED_STATE led_fun)
{
    (void)led_fun;
    return 0;
}

void tlkapp_sysLed_deInit(void)
{
    tlkdrv_led_removeAll();
}

void app_rc_ui_led_state_no_ble(void)
{
    tlkdrv_led_patternSelect(GPIO_LED_BLUE, TLKDRV_LED_PATTERN_OFF);
}

void app_rc_ui_led_state_ble_connect(void)
{
    tlkdrv_led_patternSelect(GPIO_LED_BLUE, TLKDRV_LED_PATTERN_ON);
}

void app_rc_ui_led_state_ble_pairing(void)
{
    tlkdrv_led_patternSelect(GPIO_LED_BLUE, TLKDRV_LED_PATTERN_FLASH_SLOW);
}

void app_rc_ui_led_state_no_recording(void)
{
    tlkdrv_led_patternSelect(GPIO_LED_RED, TLKDRV_LED_PATTERN_ON);
}

void app_rc_ui_led_state_recording(void)
{
    tlkdrv_led_patternSelect(GPIO_LED_RED, TLKDRV_LED_PATTERN_BREATH_SLOW);
}

void app_rc_ui_led_init(void)
{
#ifdef GPIO_LED_BLUE
    tlkdrv_led_insert(GPIO_LED_BLUE, GPIO_LED_BLUE_PWM_ID, LED_ON_LEVEL);
#endif

#ifdef GPIO_LED_RED
    tlkdrv_led_insert(GPIO_LED_RED, GPIO_LED_RED_PWM_ID, LED_ON_LEVEL);
#endif
    app_rc_ui_led_state_no_recording();
}
#endif
