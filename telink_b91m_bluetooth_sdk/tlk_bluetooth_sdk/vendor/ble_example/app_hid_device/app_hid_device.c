/********************************************************************************************************
 * @file    app_hid_device.c
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

#if (APP_HID_DEVICE_SELECT_MODE == APP_HID_DEVICE_MODE_KEYBOARD)
#define APP_BLE_HID_DEVICE_NAME       "Tlk Keyboard"
#define APP_BLE_HID_DEVICE_APPEARANCE 0x03C4 /** < GAP_APPEARANCE_HID_KEYBOARD */
extern const unsigned char  tlk_hid_keyboard_report_map[];
extern const unsigned short tlk_hid_keyboard_report_map_len;

struct tlk_hid_keyboard_data
{
    union
    {
        struct
        {
            unsigned char left_control  : 1;
            unsigned char left_shift    : 1;
            unsigned char left_alt      : 1;
            unsigned char left_GUI      : 1;
            unsigned char right_control : 1;
            unsigned char right_shift   : 1;
            unsigned char right_alt     : 1;
            unsigned char right_GUI     : 1;
        };

        unsigned char modifiers;
    };

    unsigned char reserved;
    unsigned char key[6];
};

#elif (APP_HID_DEVICE_SELECT_MODE == APP_HID_DEVICE_MODE_MOUSE)
#define APP_BLE_HID_DEVICE_NAME       "Tlk Mouse"
#define APP_BLE_HID_DEVICE_APPEARANCE 0x03C2 /** < GAP_APPEARANCE_HID_MOUSE */
extern const unsigned char  tlk_hid_mouse_report_map[];
extern const unsigned short tlk_hid_mouse_report_map_len;

struct tlk_hid_mouse_data
{
    union
    {
        struct
        {
            unsigned char left   : 1;
            unsigned char right  : 1;
            unsigned char middle : 1;
            unsigned char btn4   : 1;
            unsigned char btn5   : 1;
        };

        unsigned char button;
    };

    signed char x;
    signed char y;
    signed char wheel;
};
#endif

#define BLE_CONN_HANDLE_INVALID 0xFFFF

static void app_connected_handler(struct ble_host_conn *conn);
static void app_disconnected_handler(struct ble_host_conn *conn, uint8_t reason);

struct acl_ad_data_complete_local_name
{
    struct ltv_data header;
    char            name[30];
} __attribute__((packed));

struct acl_ad_data_incomplete_service_uuid
{
    struct ltv_data header;
    uint16_t        uuid[2];
} __attribute__((packed));

static const struct ad_data_flags s_adv_flags = {
    .header.length                      = 0x02,
    .header.type                        = DT_FLAGS,
    .flags.le_limited_discoverable_mode = 1,
    .flags.br_edr_not_supported         = 1,
};

static const struct ad_data_appearance s_adv_appearance = {
    .header.length = 0x03,
    .header.type   = DT_APPEARANCE,
    .appearance    = APP_BLE_HID_DEVICE_APPEARANCE,
};

static const struct acl_ad_data_complete_local_name s_adv_complete_name = {
    .header.length = sizeof(APP_BLE_HID_DEVICE_NAME),
    .header.type   = DT_COMPLETE_LOCAL_NAME,
    .name          = APP_BLE_HID_DEVICE_NAME,
};

static const struct acl_ad_data_incomplete_service_uuid s_adv_uuid = {
    .header.length = 0x05,
    .header.type   = DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID,
    .uuid          = {SERVICE_UUID_HUMAN_INTERFACE_DEVICE, SERVICE_UUID_BATTERY},
};

static const struct ltv_data *ad_data[] = {
    (const struct ltv_data *)&s_adv_flags,
    (const struct ltv_data *)&s_adv_appearance,
    (const struct ltv_data *)&s_adv_complete_name,
    (const struct ltv_data *)&s_adv_uuid,
    NULL, // end of advertisement data
};

static const struct ltv_data *ad_data_rsp[] = {
    (const struct ltv_data *)&s_adv_complete_name,
    NULL, // end of advertisement data
};

static uint16_t s_acl_connect_handle = BLE_CONN_HANDLE_INVALID;

static const struct ble_host_acl_conn_callbacks s_conn_callbacks = {
    .connected    = app_connected_handler,
    .disconnected = app_disconnected_handler,
};

static void app_connected_handler(struct ble_host_conn *conn)
{
    (void)conn;
    s_acl_connect_handle = conn->conn_handle;
    tlk_printf("Connected");
}

static void app_disconnected_handler(struct ble_host_conn *conn, uint8_t reason)
{
    (void)conn;
    s_acl_connect_handle = BLE_CONN_HANDLE_INVALID;
    tlk_printf("Disconnected, reason: %d", reason);
    ble_host_gap_adv_set_adv_ind_param(100, ad_data, ad_data_rsp);
    ble_host_gap_adv_start();
}

static void app_hid_server_event_callback(uint16_t conn_handle, enum ble_hids_event_id event_id, const void *event_msg)
{
    switch (event_id) {
    case HIDS_EVT_ENTER_SUSPEND_STATE:
    {
        tlk_printf("Acl Connect:0x%03x, Enter Suspend State", conn_handle);
    } break;
    case HIDS_EVT_EXIT_SUSPEND_STATE:
    {
        tlk_printf("Acl Connect:0x%03x, Exit Suspend State", conn_handle);
    } break;
    case HIDS_EVT_RECV_REPORT:
    {
        const struct ble_hids_receive_report_event *p_report_evt = (const struct ble_hids_receive_report_event *)event_msg;
#if defined(APP_HID_DEVICE_SELECT_MODE) && (APP_HID_DEVICE_SELECT_MODE == APP_HID_DEVICE_MODE_KEYBOARD)
        if (p_report_evt->report_id == BLE_HOST_HID_REPORT_ID_KEYBOARD_INPUT && p_report_evt->report_type == BLE_HOST_HID_REPORT_TYPE_OUTPUT) {
            uint8_t locks = p_report_evt->value[0];
            tlk_printf("Num Lock:%d, Caps Lock:%d, Scroll Lock:%d", (locks & 0x01) == 0x01, (locks & 0x02) == 0x02, (locks & 0x04) == 0x04);
        }
#else
        (void)p_report_evt;
#endif
    } break;
    default:
        break;
    }
}

#if defined(APP_HID_DEVICE_SELECT_MODE)

#if (APP_HID_DEVICE_SELECT_MODE == APP_HID_DEVICE_MODE_KEYBOARD)
static void app_keyboard_caps_lock_key_callback(void)
{
    if (s_acl_connect_handle != BLE_CONN_HANDLE_INVALID) {
        struct tlk_hid_keyboard_data caps_lock_key = {
            .key[0] = 0x39,
        };
        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_KEYBOARD_INPUT, (const uint8_t *)&caps_lock_key, sizeof(caps_lock_key));

        caps_lock_key.key[0] = 0x00;
        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_KEYBOARD_INPUT, (const uint8_t *)&caps_lock_key, sizeof(caps_lock_key));
    }
}

static void app_keyboard_volume_increment_callback(void)
{
    if (s_acl_connect_handle != BLE_CONN_HANDLE_INVALID) {
        uint16_t volume_increment = 0x00E9;
        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_CONSUME_CONTROL_INPUT, (const uint8_t *)&volume_increment, sizeof(volume_increment));

        volume_increment = 0x0000;
        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_CONSUME_CONTROL_INPUT, (const uint8_t *)&volume_increment, sizeof(volume_increment));
    }
}

static void app_keyboard_volume_decrement_callback(void)
{
    if (s_acl_connect_handle != BLE_CONN_HANDLE_INVALID) {
        uint16_t volume_increment = 0x00EA;
        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_CONSUME_CONTROL_INPUT, (const uint8_t *)&volume_increment, sizeof(volume_increment));

        volume_increment = 0x0000;
        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_CONSUME_CONTROL_INPUT, (const uint8_t *)&volume_increment, sizeof(volume_increment));
    }
}
#endif

#if (APP_HID_DEVICE_SELECT_MODE == APP_HID_DEVICE_MODE_MOUSE)
static void app_mouse_left_click_callback(void)
{
    if (s_acl_connect_handle != BLE_CONN_HANDLE_INVALID) {
        struct tlk_hid_mouse_data left_click = {
            .left = 1,
        };

        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_MOUSE_INPUT, (const uint8_t *)&left_click, sizeof(struct tlk_hid_mouse_data));
        left_click.left = 0;
        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_MOUSE_INPUT, (const uint8_t *)&left_click, sizeof(struct tlk_hid_mouse_data));
    }
}

static void app_mouse_x_move_10_callback(void)
{
    if (s_acl_connect_handle != BLE_CONN_HANDLE_INVALID) {
        struct tlk_hid_mouse_data x_move_10 = {
            .x = 10,
        };
        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_MOUSE_INPUT, (const uint8_t *)&x_move_10, sizeof(struct tlk_hid_mouse_data));
    }
}

static void app_mouse_x_move_p10_callback(void)
{
    if (s_acl_connect_handle != BLE_CONN_HANDLE_INVALID) {
        struct tlk_hid_mouse_data x_move_p10 = {
            .x = -10,
        };
        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_MOUSE_INPUT, (const uint8_t *)&x_move_p10, sizeof(struct tlk_hid_mouse_data));
    }
}

static void app_mouse_wheel_1_callback(void)
{
    if (s_acl_connect_handle != BLE_CONN_HANDLE_INVALID) {
        struct tlk_hid_mouse_data wheel_1 = {
            .wheel = 1,
        };
        ble_hids_notify_input_report(s_acl_connect_handle, BLE_HOST_HID_REPORT_ID_MOUSE_INPUT, (const uint8_t *)&wheel_1, sizeof(struct tlk_hid_mouse_data));
    }
}

#endif

#endif

void ble_host_gap_acl_peripheral_reconnect_timeout_hook(void)
{
    tlk_printf("Reconnect timeout");
    ble_host_gap_adv_set_adv_ind_param(100, ad_data, ad_data_rsp);
    ble_host_gap_adv_start();
}

static void app_hid_device_reconnect_first_device(void)
{
    ble_host_gap_acl_peripheral_reconnect_legacy_adv(true, 0);
}

static void app_hid_device_reconnect_second_device(void)
{
    ble_host_gap_acl_peripheral_reconnect_legacy_adv(true, 1);
}

int INIT(APP_BLE_HID_DEVICE)(void)
{
    blc_svc_addCoreGroup();
    blc_svc_setDeviceName(APP_BLE_HID_DEVICE_NAME);

    ble_basic_register_BAS_control_server(BLE_BASS_INITIAL_PARAMS(100, DEVICE_NO_CHARGING));
    ble_basic_register_DIS_control_server();
    ble_basic_register_ScPS_control_server();

#if defined(APP_HID_DEVICE_SELECT_MODE)

#if (APP_HID_DEVICE_SELECT_MODE == APP_HID_DEVICE_MODE_KEYBOARD)
    ble_hid_register_HID_control_server(BLE_HIDS_INITIAL_PARAMS_DEFAULT(tlk_hid_keyboard_report_map, tlk_hid_keyboard_report_map_len));
    app_key_register_callback(0, app_keyboard_caps_lock_key_callback);
    app_key_register_callback(1, app_keyboard_volume_increment_callback);
    app_key_register_callback(2, app_keyboard_volume_decrement_callback);
#elif (APP_HID_DEVICE_SELECT_MODE == APP_HID_DEVICE_MODE_MOUSE)
    ble_hid_register_HID_control_server(BLE_HIDS_INITIAL_PARAMS_DEFAULT(tlk_hid_mouse_report_map, tlk_hid_mouse_report_map_len));
    app_key_register_callback(0, app_mouse_left_click_callback);
    app_key_register_callback(1, app_mouse_x_move_10_callback);
    app_key_register_callback(2, app_mouse_x_move_p10_callback);
    app_key_register_callback(3, app_mouse_wheel_1_callback);
#endif
#endif

    app_key_register_callback(4, app_hid_device_reconnect_first_device);
    app_key_register_callback(5, app_hid_device_reconnect_second_device);

    ble_hid_register_HID_server_event_callback(app_hid_server_event_callback);

    blc_svc_calculateDatabaseHash();

    ble_host_gap_adv_set_adv_ind_param(100, ad_data, ad_data_rsp);
    ble_host_smp_initial(BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS);
    ble_host_smp_store_init(4, 0);
    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_conn_callbacks);

    return 0;
}

void START(APP_BLE_HID_DEVICE)(void)
{
    ble_host_gap_adv_start();
}
