/********************************************************************************************************
 * @file    app_rc_ui_key_plan0.c
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
#include "../../app_recording_card_api.h"
#include "tlklib/usb/tlkusb.h"
#include "app_rc_ui_key_cfg.h"

#ifdef APP_RC_UI_KEY_PLAN0_2KEY

void app_rc_key1_short_click_evt(void)
{
    uint8_t isOpen = app_rc_api_is_recording();
    if (isOpen == 0) {
        app_rc_api_start_record();
    } else {
        app_rc_api_stop_record();
    }
}

void app_rc_key1_double_click_evt(void)
{
    uint8_t isEnable = app_rc_api_ble_is_enable();
    app_rc_api_enable_ble(!isEnable);
}

void app_rc_key1_tri_click_evt(void)
{
    app_rc_api_enable_alg(1);
}

void app_rc_key1_4click_evt(void)
{
    app_rc_api_enable_wav_save(1);
}

void app_rc_key2_tri_click_evt(void)
{
    app_rc_api_enable_alg(0);
}

void app_rc_key2_4click_evt(void)
{
    app_rc_api_enable_wav_save(0);
}

void app_rc_key2_short_click_evt(void)
{
    app_rc_api_wifi_power_on(0);
}

void app_rc_key2_double_click_evt(void)
{
    app_rc_api_wifi_power_off();
}

void app_rc_key2_long_long_press_evt(void)
{
    app_rc_api_msc_switch();
}

void app_rc_ui_key_init(void)
{
    tlkdrv_key_registerVendorConfig1Callback(app_rc_key1_short_click_evt);
    tlkdrv_key_registerVendorConfig2Callback(app_rc_key1_double_click_evt);
    tlkdrv_key_registerVendorConfig3Callback(app_rc_key1_tri_click_evt);
    tlkdrv_key_registerVendorConfig4Callback(app_rc_key1_4click_evt);
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_8, app_rc_key2_short_click_evt);
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_5, app_rc_key2_double_click_evt);
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_6, app_rc_key2_tri_click_evt);
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_7, app_rc_key2_4click_evt);
    tlkdrv_key_registerVendorConfigCallback(KEY_EVT_VENDOR_CONFIG_9, app_rc_key2_long_long_press_evt);
}

#endif
