/********************************************************************************************************
 * @file    app_rc_wifi_core.c
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
#include "../../../../app_recording_card/app_recording_card_api.h"
#include "../app_rc_wifi_cfg.h"
#include "app_rc_wifi_intf.h"
#include "../app_rc_wifi_api.h"
#include "stdio.h"

static TlkApiTimer_t sAppRcWifiCoreTimer = {0};

static void app_rc_wifi_switch_ble_antenna(bool isEnable)
{
    (void)isEnable;
#if TLK_CFG_RF_SHARED
    if (isEnable) {
        gpio_set_high_level(ANT_VDD_GPIO);
        gpio_set_low_level(ANT_VC_GPIO);
    } else {
        gpio_set_low_level(ANT_VDD_GPIO);
        gpio_set_low_level(ANT_VC_GPIO);
        delay_us(10);
    }
#endif
}

static void app_rc_wifi_switch_wifi_antenna(bool isEnable)
{
    (void)isEnable;
#if TLK_CFG_RF_SHARED
    if (isEnable) {
        gpio_set_high_level(ANT_VDD_GPIO);
        gpio_set_high_level(ANT_VC_GPIO);
    } else {
        gpio_set_low_level(ANT_VDD_GPIO);
        gpio_set_low_level(ANT_VC_GPIO);
        delay_us(10);
    }
#endif
}

static void app_rc_antenna_to_wifi(void)
{
    (void)app_rc_wifi_switch_ble_antenna;
    (void)app_rc_wifi_switch_wifi_antenna;
#if TLK_CFG_RF_SHARED
    tlk_printf("app_rc_antenna_to_wifi");
    app_rc_wifi_switch_ble_antenna(false);
    app_rc_wifi_switch_wifi_antenna(true);
#endif
}

static void app_rc_antenna_to_ble(void)
{
    (void)app_rc_wifi_switch_ble_antenna;
    (void)app_rc_wifi_switch_wifi_antenna;
#if TLK_CFG_RF_SHARED
    tlk_printf("app_rc_antenna_to_ble");
    app_rc_wifi_switch_wifi_antenna(false);
    app_rc_wifi_switch_ble_antenna(true);
#endif
}

void tlkmw_host_antennaInitHook(void)
{
#if TLK_CFG_RF_SHARED
    gpio_function_en(ANT_VDD_GPIO);
    gpio_output_en(ANT_VDD_GPIO);
    gpio_function_en(ANT_VC_GPIO);
    gpio_output_en(ANT_VC_GPIO);
    app_rc_antenna_to_ble();
#endif
}

static void app_rc_wifi_set_name(void)
{
    uint8_t name[256];
    int     ret = app_rc_wifi_get_name(name, sizeof(name));
    if (ret < 0) {
        return;
    }
    app_rc_wifi_spi_send_cmd(WIFI_SPI_CMD_SET_NAME, name, ret, NULL);
}

static void app_rc_wifi_core_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    app_rc_wifi_uart_open();
    app_rc_wifi_set_name();
}

void app_rc_wifi_core_init(void)
{
    (void)app_rc_antenna_to_ble;
    (void)app_rc_antenna_to_wifi;
    tlksys_pm_regChn(APP_RC_WIFI_PM_CHN);
    tlksys_pm_setChn(APP_RC_WIFI_PM_CHN, 0, 0);
    gpio_set_level(APP_WIFI_POWER_CTRL, 0);
    gpio_function_en(APP_WIFI_POWER_CTRL);
    gpio_output_en(APP_WIFI_POWER_CTRL);
    app_rc_wifi_uart_init();
}

void app_rc_wifi_core_power_on(void)
{
    tlksys_pm_setChn(APP_RC_WIFI_PM_CHN, 0, 1);
#if MCU_CORE_TYPE == MCU_CORE_TL751X
    tlksys_pm_reqHighClock(3, APP_RC_CLOCK_CNH);
#endif
    app_rc_antenna_to_wifi();
    gpio_set_level(APP_WIFI_POWER_CTRL, 1);
    tlksys_timer_createStatic(APP_RC_THREAD_ID, &sAppRcWifiCoreTimer, 7 * 1000 * 1000, false, app_rc_wifi_core_timer, 0);
    tlksys_timer_start(APP_RC_THREAD_ID, &sAppRcWifiCoreTimer);
}

void app_rc_wifi_core_power_off(void)
{
    tlksys_timer_destroy(APP_RC_THREAD_ID, &sAppRcWifiCoreTimer);
    gpio_set_level(APP_WIFI_POWER_CTRL, 0);
    app_rc_antenna_to_ble();
    app_rc_wifi_uart_close();
#if MCU_CORE_TYPE == MCU_CORE_TL751X
    tlksys_pm_cancelHighClockReq(APP_RC_CLOCK_CNH);
#endif
    tlksys_pm_setChn(APP_RC_WIFI_PM_CHN, 0, 0);
}

int app_rc_wifi_get_name(uint8_t *buffer, uint32_t bufferLen)
{
#if APP_WIFI_NAME_MAKE_BY_BLE_MAC
    uint8_t *ble_host_hci_get_host_addr(void);
    uint8_t *mac = ble_host_hci_get_host_addr();
    //use ble mac to make wifi name all different
    int ret = snprintf((char *)buffer, bufferLen, "telink_%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    tlk_printf("app_rc_wifi_get_name %d", ret);
#else
    int ret = snprintf((char *)buffer, bufferLen, "%s", "Telink_app_test");
#endif
    return ret;
}
