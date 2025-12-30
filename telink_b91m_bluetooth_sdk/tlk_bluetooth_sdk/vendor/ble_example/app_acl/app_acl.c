/********************************************************************************************************
 * @file    app_acl.c
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

#include "../app_key.h"
#include "../app_example.h"

#define APP_BLE_ACL_PERIPHERAL_MAX_COUNT 4
#define BLE_DEVICE_NAME                  "tlk_bluetooth_ble_acl"
#define BLE_CONN_HANDLE_INVALID          0xFFFF

#define APP_BLE_ACL_CENTRAL_MAX_COUNT    4
#define BLE_CONN_HANDLE_INVALID          0xFFFF

#define APP_BLE_ACL_CENTRAL_RSSI_FILTER  (-35)
// #define APP_BLE_ACL_CENTRAL_NAME_FILTER         "LE_LINK"       /** < Filter for complete name. */
// #define APP_BLE_ACL_CENTRAL_MAC_FILTER           {0xA4, 0xC1, 0x38}  /** < Filter for high MAC address. */


#if APP_BLE_ACL_CENTRAL_MAX_COUNT > 0
enum acl_central_state
{
    APP_CENTRAL_STATE_IDLE,
    APP_CENTRAL_STATE_SCANNING,
    APP_CENTRAL_STATE_CONNECTING,
    APP_CENTRAL_STATE_CONNECTED = APP_CENTRAL_STATE_IDLE,
};

static struct app_acl_central_info
{
    uint8_t state;
    uint8_t count;

    struct
    {
        uint16_t conn_handle;
    } central_info[APP_BLE_ACL_CENTRAL_MAX_COUNT];
} s_app_acl_central_info;

#endif

#if APP_BLE_ACL_PERIPHERAL_MAX_COUNT > 0
static struct app_acl_peripheral_info
{
    uint8_t count;

    struct
    {
        uint16_t conn_handle;
    } peripheral_info[APP_BLE_ACL_PERIPHERAL_MAX_COUNT];
} s_app_acl_peripheral_info;

static const struct ad_data_flags s_adv_flags = {
    .header.length                      = 0x02,
    .header.type                        = DT_FLAGS,
    .flags.le_limited_discoverable_mode = 1,
    .flags.br_edr_not_supported         = 1,
};

static const struct ad_data_complete_local_name_complete s_adv_complete_name = {
    .header.length = sizeof(BLE_DEVICE_NAME),
    .header.type   = DT_COMPLETE_LOCAL_NAME,
    .name          = BLE_DEVICE_NAME,
};

static const struct ltv_data *adv_data[] = {
    (const struct ltv_data *)&s_adv_flags, (const struct ltv_data *)&s_adv_complete_name,
    NULL, // end of advertisement data
};

static const struct ltv_data *scan_rsp_data[] = {
    (const struct ltv_data *)&s_adv_complete_name,
    NULL, // end of advertisement data
};

#endif

#if APP_BLE_ACL_CENTRAL_MAX_COUNT > 0
static void app_receive_adv_report_callback(uint32_t event_id, const void *data, void *user_data);
static bool app_acl_central_receive_conn_param_update_callback(uint16_t conn_handle, const struct ble_host_signaling_conn_param_update_param *param);

static const struct ble_host_gap_evt_subscribe_param s_adv_report_evt_param = {
    .gap_le_scan_mask = 1,
    .handler          = app_receive_adv_report_callback,
};

static struct ble_host_gap_evt_subscribe s_adv_report_evt = {
    .next_handler = NULL,
    .param        = &s_adv_report_evt_param,
};

static void app_acl_central_init_command(void)
{
    ble_host_gap_scan_init();

    ble_host_gap_evt_add_subscriber(&s_adv_report_evt);
    int ret = ble_host_gap_scan_set_param(BLE_HOST_GAP_SCAN_PARAM_BASIC_UNFILTERED, BLE_HOST_GAP_ACTIVE_SCAN(80, 100));
    tlk_printf("GAP set scan param ret = 0x%x", ret);
    ble_host_gap_scan_set_filter(BLE_HOST_GAP_SCAN_FILTER_ALL_NON_CONN);

    ble_host_signaling_register_receive_conn_param_update_callback(app_acl_central_receive_conn_param_update_callback);
}
#endif

static void app_connected_callback(struct ble_host_conn *conn);
static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);

static const struct ble_host_acl_conn_callbacks s_app_acl_callbacks = {
    .connected    = app_connected_callback,
    .disconnected = app_disconnected_callback,
};

static void app_connected_callback(struct ble_host_conn *conn)
{
    tlk_printf("[APP] connected callback, ACL handle: 0x%03x", conn->conn_handle);

#if APP_BLE_ACL_CENTRAL_MAX_COUNT > 0
    if (conn->role == BLE_HOST_ACL_ROLE_CENTRAL) {
        for (int i = 0; i < APP_BLE_ACL_CENTRAL_MAX_COUNT; i++) {
            if (s_app_acl_central_info.central_info[i].conn_handle == BLE_CONN_HANDLE_INVALID) {
                s_app_acl_central_info.central_info[i].conn_handle = conn->conn_handle;
                s_app_acl_central_info.count++;
                break;
            }
        }
        ble_host_smp_start_pairing_central(conn->conn_handle);
    }
#endif
#if APP_BLE_ACL_PERIPHERAL_MAX_COUNT > 0
    if (conn->role == BLE_HOST_ACL_ROLE_PERIPHERAL) {
        for (int i = 0; i < APP_BLE_ACL_PERIPHERAL_MAX_COUNT; i++) {
            if (s_app_acl_peripheral_info.peripheral_info[i].conn_handle == BLE_CONN_HANDLE_INVALID) {
                s_app_acl_peripheral_info.peripheral_info[i].conn_handle = conn->conn_handle;
                s_app_acl_peripheral_info.count++;
                break;
            }
        }
        if (s_app_acl_peripheral_info.count < APP_BLE_ACL_PERIPHERAL_MAX_COUNT) {
            ble_host_gap_adv_start();
        }
    }
#endif
}

static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    tlk_printf("[APP] disconnected callback, ACL handle: 0x%03x, reason: 0x%02x", conn->conn_handle, reason);
#if APP_BLE_ACL_CENTRAL_MAX_COUNT > 0
    if (conn->role == BLE_HOST_ACL_ROLE_CENTRAL) {
        for (int i = 0; i < APP_BLE_ACL_CENTRAL_MAX_COUNT; i++) {
            if (s_app_acl_central_info.central_info[i].conn_handle == conn->conn_handle) {
                s_app_acl_central_info.central_info[i].conn_handle = BLE_CONN_HANDLE_INVALID;
                s_app_acl_central_info.count--;
                break;
            }
        }
    }
#endif

#if APP_BLE_ACL_PERIPHERAL_MAX_COUNT > 0
    if (conn->role == BLE_HOST_ACL_ROLE_PERIPHERAL) {
        for (int i = 0; i < APP_BLE_ACL_PERIPHERAL_MAX_COUNT; i++) {
            if (s_app_acl_peripheral_info.peripheral_info[i].conn_handle == conn->conn_handle) {
                s_app_acl_peripheral_info.peripheral_info[i].conn_handle = BLE_CONN_HANDLE_INVALID;
                s_app_acl_peripheral_info.count--;
                break;
            }
        }
        ble_host_gap_adv_start();
    }
#endif
}

#if APP_BLE_ACL_CENTRAL_MAX_COUNT > 0
static void app_acl_central_start_connection_device(void)
{
    if (s_app_acl_central_info.state != APP_CENTRAL_STATE_IDLE) {
        tlk_printf("State is not idle, cannot start connection device");
        return;
    }

    if (s_app_acl_central_info.count >= APP_BLE_ACL_CENTRAL_MAX_COUNT) {
        tlk_printf("Max connection count reached, cannot start connection device");
        return;
    }

    int ret = ble_host_gap_scan_enable();
    if (ret == BLE_HOST_ERR_SUCC) {
        s_app_acl_central_info.state = APP_CENTRAL_STATE_SCANNING;
    }
    tlk_printf("GAP enable scan ret = 0x%x", ret);
}

static void app_acl_central_disconnect_all_device(void)
{
    tlk_printf("Disconnect all device");
    for (int i = 0; i < APP_BLE_ACL_CENTRAL_MAX_COUNT; i++) {
        if (s_app_acl_central_info.central_info[i].conn_handle != BLE_CONN_HANDLE_INVALID) {
            int ret = ble_host_gap_disconnect_acl(s_app_acl_central_info.central_info[i].conn_handle);
            if (ret != BLE_HOST_ERR_SUCC) {
                tlk_printf("[APP] Error disconnecting ACL handle: 0x%03x, ret = 0x%x", s_app_acl_central_info.central_info[i].conn_handle, ret);
            } else {
                s_app_acl_central_info.central_info[i].conn_handle = BLE_CONN_HANDLE_INVALID;
                s_app_acl_central_info.count--;
            }
        }
    }
}
#endif

int INIT(APP_BLE_ACL)(void)
{
#if APP_BLE_ACL_PERIPHERAL_MAX_COUNT > 0
    ble_host_gap_adv_set_adv_ind_param(150, adv_data, scan_rsp_data);
    s_app_acl_peripheral_info.count = 0;
    for (int i = 0; i < APP_BLE_ACL_PERIPHERAL_MAX_COUNT; i++) {
        s_app_acl_peripheral_info.peripheral_info[i].conn_handle = BLE_CONN_HANDLE_INVALID;
    }
#endif

#if APP_BLE_ACL_CENTRAL_MAX_COUNT > 0
    app_acl_central_init_command();

    app_key_register_callback(0, app_acl_central_start_connection_device);
    app_key_register_callback(1, app_acl_central_disconnect_all_device);

    s_app_acl_central_info.state = APP_CENTRAL_STATE_IDLE;
    s_app_acl_central_info.count = 0;
    for (int i = 0; i < APP_BLE_ACL_CENTRAL_MAX_COUNT; i++) {
        s_app_acl_central_info.central_info[i].conn_handle = BLE_CONN_HANDLE_INVALID;
    }
#endif

    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_app_acl_callbacks);

    blc_svc_addCoreGroup();
    blc_svc_addDisGroup();
    blc_svc_calculateDatabaseHash();
    blc_svc_setDeviceName(BLE_DEVICE_NAME);

    ble_host_smp_initial(BLE_HOST_SMP_LEGACY_JUST_WORKS_INIT_PARAMS);
    ble_host_smp_store_init(4, 4);
    tlk_printf("[APP] BLE ACL Peripheral(%d) and Central(%d) initialized", APP_BLE_ACL_PERIPHERAL_MAX_COUNT, APP_BLE_ACL_CENTRAL_MAX_COUNT);
    return 0;
}

void START(APP_BLE_ACL)(void)
{
#if APP_BLE_ACL_PERIPHERAL_MAX_COUNT > 0
    ble_host_gap_adv_start();
#endif
}

#if APP_BLE_ACL_CENTRAL_MAX_COUNT > 0
static void app_acl_central_create_connection_callback(bool result)
{
    tlk_printf("[APP] Create connection callback, result = %d", result);
    s_app_acl_central_info.state = APP_CENTRAL_STATE_CONNECTED;
}

static void app_acl_central_create_new_connection(const struct ble_host_gap_evt_scan_adv_report_undirected *p_adv_report)
{
    int ret = ble_host_gap_scan_disable();
    if (ret == BLE_HOST_ERR_SUCC) {
        if (!ble_host_gap_acl_central_create_connection(
                BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS_BALANCED(p_adv_report->addr_type, p_adv_report->addr, app_acl_central_create_connection_callback))) {
            s_app_acl_central_info.state = APP_CENTRAL_STATE_CONNECTING;
        } else {
            s_app_acl_central_info.state = APP_CENTRAL_STATE_IDLE;
        }

        // ble_host_gap_acl_central_create_connection(BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS_BALANCED(0x01, p_adv_report->addr, app_acl_central_create_connection_callback));
    } else {
        tlk_printf("[APP] Error disabling scan, ret = 0x%x", ret);
    }
}

struct parse_ad_data_info
{
    uint8_t  complete_name_len;
    uint8_t *complete_name;
};

static int app_parse_ad_data(uint8_t length, uint8_t type, const uint8_t *value, void *user)
{
    struct parse_ad_data_info *p_ad_data = user;

    if (type == DT_COMPLETE_LOCAL_NAME) {
        p_ad_data->complete_name_len = length - 1;
        p_ad_data->complete_name     = (uint8_t *)value;
    }

    return 1;
}

static void app_receive_undirected_adv_report(const struct ble_host_gap_evt_scan_adv_report_undirected *p_adv_report)
{
    struct parse_ad_data_info ad_data = {0};

    int res = ltv_unpack(p_adv_report->data, p_adv_report->data_len, app_parse_ad_data, &ad_data);

    if (res != LTV_UNPACK_SUCCESS) {
        // ignore this adv report
        return;
    }

#ifdef APP_BLE_ACL_CENTRAL_RSSI_FILTER
    if (p_adv_report->rssi > APP_BLE_ACL_CENTRAL_RSSI_FILTER) {
        tlk_printf("RSSI is lower than filter, ignore this adv report");
        app_acl_central_create_new_connection(p_adv_report);
        return;
    }
#endif // APP_BLE_ACL_CENTRAL_RSSI_FILTER

#ifdef APP_BLE_ACL_CENTRAL_NAME_FILTER
    if (ad_data.complete_name_len != 0) {
        if (memcmp(ad_data.complete_name, APP_BLE_ACL_CENTRAL_NAME_FILTER, sizeof(APP_BLE_ACL_CENTRAL_NAME_FILTER) - 1) == 0) {
            tlk_printf("Complete name is matched, start connection device");
            app_acl_central_create_new_connection(p_adv_report);
            return;
        }
    }
#else
    (void)ad_data;
#endif // APP_BLE_ACL_CENTRAL_NAME_FILTER

#ifdef APP_BLE_ACL_CENTRAL_MAC_FILTER
    uint8_t filter_mac[3] = APP_BLE_ACL_CENTRAL_MAC_FILTER;
    if (p_adv_report->addr[5] == filter_mac[0] && p_adv_report->addr[4] == filter_mac[1] && p_adv_report->addr[3] == filter_mac[2]) {
        tlk_printf("MAC address is matched, start connection device");
        app_acl_central_create_new_connection(p_adv_report);
        return;
    }
#endif // APP_BLE_ACL_CENTRAL_MAC_FILTER
}

static void app_receive_adv_report_callback(uint32_t event_id, const void *data, void *user_data)
{
    (void)user_data;
    if (event_id == BLE_HOST_GAP_EVT_LE_SCAN_REPORT_UNDIRECTED) {
        app_receive_undirected_adv_report(data);
    } else if (event_id == BLE_HOST_GAP_EVT_LE_SCAN_REPORT_DIRECTED) {
        tlk_printf("Directed adv report received"); // todo: handle directed adv report.
    }
}

static bool app_acl_central_receive_conn_param_update_callback(uint16_t conn_handle, const struct ble_host_signaling_conn_param_update_param *param)
{
    tlk_printf("[APP] Update, conn handle: 0x%03x, conn interval: [%.2fms, %.2fms], conn latency: %d, supervision timeout: %dms", conn_handle, param->min_interval * 1.25,
               param->max_interval * 1.25, param->latency, param->timeout * 10);

    return true;
}
#endif
