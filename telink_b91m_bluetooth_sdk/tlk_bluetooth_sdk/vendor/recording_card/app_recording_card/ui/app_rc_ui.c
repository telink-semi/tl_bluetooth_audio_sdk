/********************************************************************************************************
 * @file    app_rc_ui.c
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
#include "key/app_rc_ui_key.h"
#include "oled/app_rc_ui_oled.h"
#include "led/app_rc_ui_led.h"
#include "app_rc_ui.h"

void app_rc_ui_init(void)
{
#if TLK_DEV_LED_ENABLE
    app_rc_ui_led_init();
#endif
    app_rc_ui_key_init();
    app_rc_ui_oled_init();
}

void app_rc_ui_update_rec_state(uint8_t enable)
{
    (void)enable;
#if TLK_DEV_LED_ENABLE
    if (enable) {
        app_rc_ui_led_state_recording();
    } else {
        app_rc_ui_led_state_no_recording();
    }
#endif
}

void app_rc_ui_update_ble_state(uint8_t state)
{
    (void)state;
#if TLK_DEV_LED_ENABLE
    if (state == RC_UI_BLE_NONE) {
        app_rc_ui_led_state_no_ble();
    } else if (state == RC_UI_BLE_BRO) {
        app_rc_ui_led_state_ble_pairing();
    } else if (state == RC_UI_BLE_CON) {
        app_rc_ui_led_state_ble_connect();
    }
#endif
}