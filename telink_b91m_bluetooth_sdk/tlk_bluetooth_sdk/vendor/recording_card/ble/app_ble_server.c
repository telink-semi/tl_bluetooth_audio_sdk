/********************************************************************************************************
 * @file    app_ble_server.c
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

#include "app_ble.h"
#include "app_ble_command.h"

#define APP_START_HDL 0x9000 // SERVICE_USER_128UUID_START_HDL


static int app_ble_write_callback(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len);
static int app_ble_notify_value_small(uint8_t *value, uint16_t value_len);

UUID128BIT_DEF(serviceAppUuid, APP_SERVICE_UUID);
UUID128BIT_DEF(writeUuid, APP_WRITE_UUID);
UUID128BIT_DEF(notifyUuid, APP_NOTIFY_UUID);

/*
 * @brief the structure for Telink Application service List.
 */
static const struct atts_attribute appList[] = {
    ATTS_PRIMARY_SERVICE_128(serviceAppUuid),

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropWriteWriteWithout),
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_WRITE, writeUuid, ATTS_SET_WRITE_CALLBACK, 0, NULL, NULL),

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropNotify),
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_NONE, notifyUuid, ATTS_SET_NONE, 0, NULL, NULL),
    ATTS_COMMON_CCC_DEFINE,
};

/*
 * @brief the structure for Telink SPP service group.
 */
static struct atts_group svcAppGroup = {NULL, appList, NULL, app_ble_write_callback, APP_START_HDL, 0};

static uint16_t s_acl_conn_handle = 0xFFFF;

void app_ble_server_init(void)
{
    svcAppGroup.endHandle = svcAppGroup.startHandle + ARRAY_SIZE(appList) - 1;
    ble_host_add_attribute_service_group(&svcAppGroup);
}

void app_server_acl_connected(uint16_t conn_handle)
{
    s_acl_conn_handle = conn_handle;
}

void app_server_acl_disconnected(void)
{
    s_acl_conn_handle = 0xFFFF;
}

static struct app_ble_info
{
    uint8_t  battery_status;
    uint8_t  battery_level;
    uint8_t  write_buffer[255];
    uint8_t  buffer_cmd;
    uint8_t  buffer_len;
    uint8_t  write_buffer_offset;
    uint32_t sd_total_size;
    uint32_t sd_not_used_size;
    uint32_t ble_version;
    uint32_t wifi_version;
    char     bluetooth_version[16];
} s_app_ble_info = {
    .battery_status    = 0x01,
    .battery_level     = 0x64,
    .sd_total_size     = 64 * 1024 * 1024,
    .sd_not_used_size  = 45 * 1024 * 1024,
    .bluetooth_version = "2506160101300008",
    .ble_version       = 0x00000001,
    .wifi_version      = 0x00000001,
};

enum app_ble_command_id
{
    APP_BLE_COMMAND_GET_BATTERY_STATUS = 0x01,
    APP_BLE_COMMAND_SEARCH_FILES       = 0x05,
    APP_BLE_COMMAND_PREPARE_SEND_FILE  = 0x07,
    APP_BLE_COMMAND_START_SEND_FILE    = 0x00,
    // APP_BLE_COMMAND_START_SEND_FILE = 0x08,

    APP_BLE_COMMAND_DELETE_FILE         = 0x12,
    APP_BLE_COMMAND_GET_VERSION         = 0x17,
    APP_BLE_RESULT_PREPARE_SEND_FILE_OK = 0x2D,
    APP_BLE_COMMAND_GET_STORAGE_SIZE    = 0x2E,
    APP_BLE_COMMAND_RESPONSE_FINISH     = 0x2F,
    // APP_BLE_COMMAND_GET_BLUETOOTH_VERSION = 0x2E,
};

void app_ble_set_battery_status(uint8_t status, uint8_t level)
{
    s_app_ble_info.battery_status = status;
    s_app_ble_info.battery_level  = level;
}

int app_ble_notify_battery_status(void)
{
    uint8_t battery_status[] = {0xFF, 0x55, 0xAA, 0x02, APP_BLE_COMMAND_GET_BATTERY_STATUS, s_app_ble_info.battery_level, s_app_ble_info.battery_status,
                                0xFF, 0x55, 0xAA, 0x00, APP_BLE_COMMAND_RESPONSE_FINISH};
    int     ret              = app_ble_notify_value_small(battery_status, sizeof(battery_status));
    tlk_printf("notify battery value ret:0x%x", ret);
    return ret;
}

void app_ble_set_bluetooth_version(char version[16])
{
    memcpy(s_app_ble_info.bluetooth_version, version, 16);
}

void app_ble_set_sd_size(uint32_t total_size, uint32_t not_used_size)
{
    s_app_ble_info.sd_total_size    = total_size;
    s_app_ble_info.sd_not_used_size = not_used_size;
}

static void app_ble_get_battery_status_callback(uint8_t *value, uint8_t value_len)
{
    (void)value;
    (void)value_len;
    app_ble_notify_battery_status();
}

static void app_ble_search_files_callback(uint8_t *value, uint8_t value_len)
{
    app_ble_search_file_table(value, value_len);
}

static void app_ble_prepare_send_file_callback(uint8_t *value, uint8_t value_len)
{
    uint32_t file_size   = app_ble_send_file(value, value_len);
    uint8_t  file_info[] = {0xFF, 0x55, 0xAA, 0x04, APP_BLE_RESULT_PREPARE_SEND_FILE_OK, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x55, 0xAA, 0x00, APP_BLE_COMMAND_RESPONSE_FINISH};

    *(uint32_t *)(file_info + 5) = __builtin_bswap32(file_size);

    app_ble_notify_value_small(file_info, sizeof(file_info));
}

static void app_ble_start_send_file_callback(uint8_t *value, uint8_t value_len)
{
    (void)value;
    (void)value_len;
    app_ble_start_send_file();
}

void app_ble_ack_start_send_file_ok(void)
{
    uint8_t file_info[] = {0xFF, 0x55, 0xAA, 0x04, 0x08, 0xFF, 0x55, 0xAA, 0x00, APP_BLE_COMMAND_RESPONSE_FINISH};

    app_ble_notify_value_small(file_info, sizeof(file_info));
}

static void app_ble_delete_file_callback(uint8_t *value, uint8_t value_len)
{
    app_ble_delete_file(value, value_len);
    uint8_t delete_file[] = {0xFF, 0x55, 0xAA, 0x00, APP_BLE_COMMAND_DELETE_FILE, 0xFF, 0x55, 0xAA, 0x00, APP_BLE_COMMAND_RESPONSE_FINISH};
    app_ble_notify_value_small(delete_file, sizeof(delete_file));
}

static void app_ble_get_version_callback(uint8_t *value, uint8_t value_len)
{
    (void)value;
    (void)value_len;
    uint8_t version[] = {0xFF, 0x55, 0xAA, 0x08, APP_BLE_COMMAND_GET_VERSION,    0x00, 0x00, 0x00, 0x00, // BT version
                         0x00, 0x00, 0x00, 0x00,                                                         // WIFI version
                         0xFF, 0x55, 0xAA, 0x00, APP_BLE_COMMAND_RESPONSE_FINISH};

    *(uint32_t *)(version + 5) = __builtin_bswap32(s_app_ble_info.ble_version);
    *(uint32_t *)(version + 9) = __builtin_bswap32(s_app_ble_info.wifi_version);

    app_ble_notify_value_small(version, sizeof(version));
}

static void app_ble_get_storage_size_callback(uint8_t *value, uint8_t value_len)
{
    (void)value;
    (void)value_len;
    uint8_t storage_size[] = {0xFF, 0x55, 0xAA, 0x08, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x55, 0xAA, 0x00, APP_BLE_COMMAND_RESPONSE_FINISH};

    *(uint32_t *)(storage_size + 5) = __builtin_bswap32(s_app_ble_info.sd_total_size);
    *(uint32_t *)(storage_size + 9) = __builtin_bswap32(s_app_ble_info.sd_not_used_size);

    app_ble_notify_value_small(storage_size, sizeof(storage_size));
}

typedef void (*app_ble_command_callback)(uint8_t *value, uint8_t value_len);

static app_ble_command_callback s_command_table[] = {
    [APP_BLE_COMMAND_GET_BATTERY_STATUS] = app_ble_get_battery_status_callback,
    [APP_BLE_COMMAND_SEARCH_FILES]       = app_ble_search_files_callback,
    [APP_BLE_COMMAND_PREPARE_SEND_FILE]  = app_ble_prepare_send_file_callback,
    [APP_BLE_COMMAND_START_SEND_FILE]    = app_ble_start_send_file_callback,
    [APP_BLE_COMMAND_DELETE_FILE]        = app_ble_delete_file_callback,
    [APP_BLE_COMMAND_GET_VERSION]        = app_ble_get_version_callback,
    [APP_BLE_COMMAND_GET_STORAGE_SIZE]   = app_ble_get_storage_size_callback,

    // [APP_BLE_COMMAND_GET_BLUETOOTH_VERSION] = app_ble_get_bluetooth_version_callback,
};

static int app_ble_write_callback(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len)
{
    (void)conn_handle;
    (void)opcode;
    (void)attr_handle;

    if (s_app_ble_info.write_buffer_offset == 0) {
        if (value[0] == 0xFF && value[1] == 0x55 && value[2] == 0xAA) {
            s_app_ble_info.buffer_len          = value[3];
            s_app_ble_info.buffer_cmd          = value[4];
            s_app_ble_info.write_buffer_offset = value_len - 5;
            memcpy(s_app_ble_info.write_buffer, &value[5], s_app_ble_info.write_buffer_offset);
        }
    } else {
        memcpy(&s_app_ble_info.write_buffer[s_app_ble_info.write_buffer_offset], value, value_len);
        s_app_ble_info.write_buffer_offset += value_len;
    }

    if (s_app_ble_info.write_buffer_offset == s_app_ble_info.buffer_len) {
        uint8_t cmd                        = s_app_ble_info.buffer_cmd;
        uint8_t buffer_len                 = s_app_ble_info.write_buffer_offset;
        s_app_ble_info.write_buffer_offset = 0;
        s_app_ble_info.buffer_len          = 0;

        tlk_printf("receive command: 0x%02x", cmd);
        // handle command here.
        if (cmd < ARRAY_SIZE(s_command_table) && s_command_table[cmd] != NULL) {
            s_command_table[cmd](s_app_ble_info.write_buffer, buffer_len);
        }
    }
    return ATT_SUCCESS;
}

static int app_ble_notify_value_small(uint8_t *value, uint16_t value_len)
{
    if (s_acl_conn_handle != 0xFFFF) {
        return ble_gatts_notify(s_acl_conn_handle, svcAppGroup.startHandle + 4, value, value_len);
    }

    return -1;
}

int app_ble_notify_value(uint8_t *value, uint16_t value_len)
{
    if (s_acl_conn_handle != 0xFFFF) {
        return ble_gatts_notify(s_acl_conn_handle, svcAppGroup.startHandle + 4, value, value_len);
    }

    return -1;
}

int app_ble_send_file_info(char *file_name_table, uint8_t file_len, gatts_notify_callback callback)
{
    uint8_t file_info[file_len + 5];
    file_info[0] = 0xFF;
    file_info[1] = 0x55;
    file_info[2] = 0xAA;
    file_info[3] = file_len;
    file_info[4] = APP_BLE_COMMAND_SEARCH_FILES;
    memcpy(&file_info[5], file_name_table, file_len);

    uint16_t mtu = ble_host_att_get_mtu(s_acl_conn_handle);
    if (mtu < 23) {
        return -1;
    }

    uint16_t max_pdu_len = mtu - 3;
    uint8_t  offset      = 0;
    file_len += 5;

    while (file_len > max_pdu_len) {
        uint8_t curr_len = min(file_len, max_pdu_len);
        ble_gatts_notify(s_acl_conn_handle, svcAppGroup.startHandle + 4, file_info + offset, curr_len);
        file_len -= max_pdu_len;
    }

    return ble_gatts_notify_with_callback(s_acl_conn_handle, svcAppGroup.startHandle + 4, file_info + offset, file_len, callback);
}
