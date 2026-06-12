/********************************************************************************************************
 * @file    app_scan.c
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
#include "stack/ble/ble.h"

#include "../app_example.h"

#define APP_BLE_SCAN_MODE_LEGACY       1
#define APP_BLE_SCAN_MODE_EXTENDED     2

#define APP_BLE_SCAN_FUNC_ACTIVE_SCAN  1
#define APP_BLE_SCAN_FUNC_PASSIVE_SCAN 2


#define APP_BLE_SCAN_MODE_DEFAULT      APP_BLE_SCAN_MODE_LEGACY
#define APP_BLE_SCAN_FUNC_DEFAULT      APP_BLE_SCAN_FUNC_PASSIVE_SCAN


static void app_scan_report_evt_handler(uint32_t event_id, const void *data, void *user_data);

static const struct ble_host_gap_evt_subscribe_param s_scan_evt_param = {
    .gap_le_scan_mask = 1,
    .handler          = app_scan_report_evt_handler,
};

static struct ble_host_gap_evt_subscribe s_scan_evt = {
    .next_handler = NULL,
    .param        = &s_scan_evt_param,
};

int INIT(APP_BLE_SCAN)(void)
{
#if (APP_BLE_SCAN_MODE_DEFAULT == APP_BLE_SCAN_MODE_LEGACY)
    ble_host_gap_scan_init();

#if (APP_BLE_SCAN_FUNC_DEFAULT == APP_BLE_SCAN_FUNC_ACTIVE_SCAN)
    ble_host_gap_scan_set_param(BLE_HOST_GAP_SCAN_PARAM_BASIC_UNFILTERED, BLE_HOST_GAP_ACTIVE_SCAN(50, 50));
#elif (APP_BLE_SCAN_FUNC_DEFAULT == APP_BLE_SCAN_FUNC_PASSIVE_SCAN)
    ble_host_gap_scan_set_param(BLE_HOST_GAP_SCAN_PARAM_BASIC_UNFILTERED, BLE_HOST_GAP_PASSIVE_SCAN(50, 50));
#endif

#elif (APP_BLE_SCAN_MODE_DEFAULT == APP_BLE_SCAN_MODE_EXTENDED)
    ble_host_gap_ext_scan_init();

#if (APP_BLE_SCAN_FUNC_DEFAULT == APP_BLE_SCAN_FUNC_ACTIVE_SCAN)
    ble_host_gap_ext_scan_set_param(BLE_HOST_GAP_EXT_SCAN_PARAM_BASIC_UNFILTERED, BLE_HOST_GAP_ACTIVE_SCAN(50, 50));
#elif (APP_BLE_SCAN_FUNC_DEFAULT == APP_BLE_SCAN_FUNC_PASSIVE_SCAN)
    ble_host_gap_ext_scan_set_param(BLE_HOST_GAP_EXT_SCAN_PARAM_BASIC_UNFILTERED, BLE_HOST_GAP_PASSIVE_SCAN(50, 50));
#endif

#endif

    // ble_host_gap_scan_set_filter(BLE_HOST_GAP_SCAN_FILTER_ALL_NON_CONN);
    ble_host_gap_evt_add_subscriber(&s_scan_evt);
    return 0;
}

void START(APP_BLE_SCAN)(void)
{
#if (APP_BLE_SCAN_MODE_DEFAULT == APP_BLE_SCAN_MODE_LEGACY)
    int ret = ble_host_gap_scan_enable();
#elif (APP_BLE_SCAN_MODE_DEFAULT == APP_BLE_SCAN_MODE_EXTENDED)
    int ret = ble_host_gap_ext_scan_enable();
#endif
    tlk_printf("[APP] enable scan ret = 0x%x", ret);
}

static void app_scan_report_evt_handler(uint32_t event_id, const void *data, void *user_data)
{
    (void)user_data;
    if (event_id == BLE_HOST_GAP_EVT_LE_SCAN_REPORT_UNDIRECTED) {
        const struct ble_host_gap_evt_scan_adv_report_undirected *report = data;
        tlk_printf("[APP] scan report (undirected), MAC is %s", addr_to_str(report->addr));
    } else if (event_id == BLE_HOST_GAP_EVT_LE_SCAN_REPORT_DIRECTED) {
    }
}
