/********************************************************************************************************
 * @file    app_ble_telink_server.c
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

#include "app_ble_command.h"
#include "app_ble_telink_server.h"
#include "app_ble_telink_command.h"
#include "../app_recording_card/app_recording_card_api.h"

#define APP_START_HDL 0x9010 // SERVICE_USER_128UUID_START_HDL

static int app_tlk_record_write_callback(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len);

/*
 * @brief the structure for Telink Record service List.
 */
static const struct atts_attribute tlkRecordList[] = {
    ATTS_PRIMARY_SERVICE_128(tlk_record_service_att_uuid),

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropWriteWriteWithout),
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_WRITE, tlk_record_cmd_att_uuid, ATTS_SET_WRITE_CALLBACK, 0, NULL, NULL),

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropNotify),
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_NONE, tlk_record_event_att_uuid, ATTS_SET_NONE, 0, NULL, NULL),
    ATTS_COMMON_CCC_DEFINE,

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropNotify),
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_NONE, tlk_record_data_att_uuid, ATTS_SET_NONE, 0, NULL, NULL),
    ATTS_COMMON_CCC_DEFINE,

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropNotify),
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_NONE, tlk_record_file_att_uuid, ATTS_SET_NONE, 0, NULL, NULL),
    ATTS_COMMON_CCC_DEFINE,
};

/*
 * @brief the structure for Telink Record service group.
 */
static struct atts_group svcTlkRecordGroup = {NULL, tlkRecordList, NULL, app_tlk_record_write_callback, APP_START_HDL, 0};

static uint16_t s_acl_conn_handle = 0xFFFF;

static bool s_report_recording_event = false;

static void app_tlk_record_check_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    s_report_recording_event = false;
    tlksys_timer_destroy(TLKSYS_TASKID_HOST, pTimer);
}

static void app_tlk_record_command_ack_retry_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    uint8_t value[] = {0xAA, 0x55, 0x02};
    ble_gatts_notify(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 4, value, sizeof(value));
    tlksys_timer_destroy(TLKSYS_TASKID_HOST, pTimer);
}

static void app_tlk_record_send_stop_record_cb(uint16_t conn_handle, bool success)
{
    (void)conn_handle;
    if (success || s_acl_conn_handle == 0xFFFF) {
        return;
    }
    tlk_printf("notify send fail, need retry");
    TlkApiTimerHandle_t timerHandle = 0;
    tlksys_timer_create(TLKSYS_TASKID_HOST, &timerHandle, 500000, false, app_tlk_record_command_ack_retry_timer, 0);
    tlksys_timer_start(TLKSYS_TASKID_HOST, timerHandle);
}

void app_tlk_record_init(void)
{
    svcTlkRecordGroup.endHandle = svcTlkRecordGroup.startHandle + ARRAY_SIZE(tlkRecordList) - 1;
    ble_host_add_attribute_service_group(&svcTlkRecordGroup);
    // ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA2_USER_ID, &s_app_server_acl_callbacks);
}

void app_tlk_record_acl_connect(uint16_t conn_handle)
{
    s_acl_conn_handle = conn_handle;
}

void app_tlk_record_acl_disconnect(void)
{
    s_acl_conn_handle        = 0xFFFF;
    s_report_recording_event = false;
}

static int app_tlk_record_write_callback(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len)
{
    (void)conn_handle;
    (void)opcode;
    (void)attr_handle;

    if (value[0] == 0x11) {
        if (value[1] == 0x20) {
            app_tlk_record_report_start_record();
            app_rc_api_start_record();
            s_report_recording_event = true;
        } else if (value[1] == 0x21) {
            // s_report_recording_event = false;
            TlkApiTimerHandle_t timerHandle = 0;
            tlksys_timer_create(TLKSYS_TASKID_HOST, &timerHandle, 200000, false, app_tlk_record_check_timer, 0);
            tlksys_timer_start(TLKSYS_TASKID_HOST, timerHandle);
            app_tlk_record_report_stop_record();
            app_rc_api_stop_record();
        } else if (value[1] == 0x22) {
            app_ble_telink_search_opus_table();
        } else if (value[1] == 0x23) {
            app_ble_telink_search_wav_table();
        } else if (value[1] == 0x24) {
            app_ble_telink_delete_file(value + 2, value_len - 2);
        } else if (value[1] == 0x25) {
            uint8_t  old_file_len = value[2];
            uint8_t *old_file     = value + 3;
            uint8_t  new_file_len = value[3 + old_file_len];
            uint8_t *new_file     = value + 4 + old_file_len;
            app_ble_telink_rename_file(old_file, old_file_len, new_file, new_file_len);
        } else if (value[1] == 0x26) {
            app_ble_telink_read_file(value + 2, value_len - 2, 0);
        } else if (value[1] == 0x27) {
            uint8_t  wifi_name_len = value[2];
            uint8_t *wifi_name     = value + 3;
            uint8_t  pass_word_len = value[3 + wifi_name_len];
            uint8_t *pass_word     = value + 4 + wifi_name_len;
            app_ble_telink_rename_file(wifi_name, wifi_name_len, pass_word, pass_word_len);
        } else if (value[1] == 0x28) {
            app_ble_telink_read_file_wifi(value + 2, value_len - 2, 0);
        } else if (value[1] == 0x29) {
            app_ble_telink_set_time(*(uint32_t *)(value + 2));
        } else if (value[1] == 0x2A) {
            app_ble_telink_get_time();
        } else if (value[1] == 0x2B) {
            app_tlk_record_report_change_channel();
            app_rc_api_wifi_power_on(2000);
        } else if (value[1] == 0x40) {
            uint32_t offset = (uint32_t)value[2] | ((uint32_t)value[3] << 8) | ((uint32_t)value[4] << 16) | ((uint32_t)value[5] << 24);
            app_ble_telink_read_file(value + 6, value_len - 6, offset);
        } else if (value[1] == 0x41) {
            uint32_t offset = (uint32_t)value[2] | ((uint32_t)value[3] << 8) | ((uint32_t)value[4] << 16) | ((uint32_t)value[5] << 24);
            app_ble_telink_read_file_wifi(value + 6, value_len - 6, offset);
        } else if (value[1] == 0x50) {
            app_ble_telink_get_wifi_name();
        }
    }

    return ATT_SUCCESS;
}

__attribute__((weak)) int app_tlk_record_report_event_by_other_way(uint8_t *value, uint16_t value_len)
{
    (void)value;
    (void)value_len;
    return 0;
}

int app_tlk_record_report_event(uint8_t *value, uint16_t value_len)
{
    if (s_acl_conn_handle != 0xFFFF) { //le connected
        return ble_gatts_notify(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 4, value, value_len);
    } else {
        return app_tlk_record_report_event_by_other_way(value, value_len);
    }
}

int app_tlk_record_report_start_record(void)
{
    uint8_t value[] = {0xAA, 0x55, 0x01};
    return app_tlk_record_report_event(value, sizeof(value));
}

int app_tlk_record_report_stop_record(void)
{
    uint8_t value[] = {0xAA, 0x55, 0x02};
    int     ret     = ble_gatts_notify_with_callback(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 4, value, sizeof(value), app_tlk_record_send_stop_record_cb);
    if (ret == BLE_L2CAP_ERR(BLE_L2CAP_ERR_ATT_NTF_PENDING_FULL)) {
        app_tlk_record_send_stop_record_cb(s_acl_conn_handle, false);
    }
    return ret;
}

int app_tlk_record_report_delete_file_result(uint8_t result)
{
    uint8_t value[] = {0xAA, 0x55, 0x03, result};
    return app_tlk_record_report_event(value, sizeof(value));
}

int app_tlk_record_report_rename_file_result(uint8_t result)
{
    uint8_t value[] = {0xAA, 0x55, 0x04, result};
    return app_tlk_record_report_event(value, sizeof(value));
}

int app_tlk_record_report_file_size(uint32_t file_size)
{
    uint8_t value[7] = {0xAA, 0x55, 0x05, 0x00, 0x00, 0x00, 0x00};
    value[3]         = (file_size >> 24) & 0xFF;
    value[4]         = (file_size >> 16) & 0xFF;
    value[5]         = (file_size >> 8) & 0xFF;
    value[6]         = file_size & 0xFF;
    return app_tlk_record_report_event(value, sizeof(value));
}

int app_tlk_record_report_file_transport_finish(void)
{
    uint8_t value[] = {0xAA, 0x55, 0x06};
    return app_tlk_record_report_event(value, sizeof(value));
}

int app_tlk_record_report_wifi_state(uint8_t state)
{
    uint8_t value[] = {0xAA, 0x55, 0x07, state};
    return app_tlk_record_report_event(value, sizeof(value));
}

int app_tlk_record_report_set_time_result(uint8_t result)
{
    uint8_t value[] = {0xAA, 0x55, 0x09, result};
    return app_tlk_record_report_event(value, sizeof(value));
}

int app_tlk_record_report_current_time(uint32_t time)
{
    uint8_t value[7] = {0xAA, 0x55, 0x0A, 0x00, 0x00, 0x00, 0x00};
    value[6]         = (time >> 24) & 0xFF;
    value[5]         = (time >> 16) & 0xFF;
    value[4]         = (time >> 8) & 0xFF;
    value[3]         = time & 0xFF;
    return app_tlk_record_report_event(value, sizeof(value));
}

int app_tlk_record_report_change_channel(void)
{
    uint8_t value[] = {0xAA, 0x55, 0x2B};
    return app_tlk_record_report_event(value, sizeof(value));
}

int app_tlk_record_report_wifi_name(char *wifi_name, uint16_t len)
{
    uint8_t value[len + 4];

    value[0] = 0xAA;
    value[1] = 0x55;
    value[2] = 0x50;
    value[3] = len;
    memcpy(&value[4], wifi_name, len);
    uint16_t mtu = ble_host_att_get_mtu(s_acl_conn_handle);
    if (mtu < 23) {
        return -1;
    }
    uint16_t max_pdu_len = mtu - 3;
    uint8_t  offset      = 0;
    len += 4;

    while (len > max_pdu_len) {
        uint8_t curr_len = max_pdu_len;
        ble_gatts_notify(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 4, value + offset, curr_len);
        len -= curr_len;
        offset += curr_len;
    }

    return ble_gatts_notify(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 4, value + offset, len);
}

int app_tlk_record_report_file_table(uint8_t *file_table, uint16_t file_len, gatts_notify_callback callback)
{
    uint8_t value[file_len + 3];

    value[0] = 0xAA;
    value[1] = 0x55;
    value[2] = 0x08;
    memcpy(&value[3], file_table, file_len);

    uint16_t mtu = ble_host_att_get_mtu(s_acl_conn_handle);
    if (mtu < 23) {
        if (callback) {
            callback(0, false);
        }
        return -1;
    }

    uint16_t max_pdu_len = mtu - 3;
    uint8_t  offset      = 0;
    file_len += 3;

    while (file_len > max_pdu_len) {
        uint8_t curr_len = max_pdu_len;
        ble_gatts_notify(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 4, value + offset, curr_len);
        file_len -= curr_len;
        offset += curr_len;
    }

    return ble_gatts_notify_with_callback(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 4, value + offset, file_len, callback);
}

static uint8_t  s_ble_opus_table[255];
static uint16_t s_ble_opus_table_len = 0;

int app_tlk_record_report_data(uint8_t *value, uint16_t value_len)
{
    if (!s_report_recording_event) {
        return 0;
    }
    do {
        if (value_len < 8) {
            break;
        }

        uint32_t len;
        memcpy(&len, value, 4);

        if (len + 8 < len) {
            break;
        }
        if (len + 8 > 255) {
            break;
        }

        if (s_ble_opus_table_len == 0) {
            if (value_len >= len + 8) {
                ble_gatts_notify(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 7, value, len + 8);
                value_len -= (len + 8);
                value += (len + 8);
            } else {
                if (value_len > 255) {
                    break;
                }
                memcpy(s_ble_opus_table, value, value_len);
                s_ble_opus_table_len = value_len;
                break;
            }
        } else {
            uint32_t cached_len;
            memcpy(&cached_len, s_ble_opus_table, 4);
            if (cached_len + 8 < cached_len) {
                break;
            }
            if (cached_len + 8 > 255) {
                break;
            }
            if (s_ble_opus_table_len > cached_len + 8) {
                break;
            }

            uint32_t remain_len = cached_len + 8 - s_ble_opus_table_len;
            if (remain_len > value_len) {
                break;
            }

            memcpy(s_ble_opus_table + s_ble_opus_table_len, value, remain_len);
            s_ble_opus_table_len = 0;
            value_len -= remain_len;
            value += remain_len;
            ble_gatts_notify(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 7, value, len + 8);
        }

        if (value_len == 0) {
            break;
        }
    } while (1);
    /*
    do {
        if (s_ble_opus_table_len == 0) {
            uint32_t len = *(uint32_t *) (value);
            if (value_len >= (len + 8)) {
                ble_gatts_notify(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 7, value, len + 8);
                value_len -= (len + 8);
                value += (len + 8);

            } else {
                tmemcpy(s_ble_opus_table, value, value_len);
                s_ble_opus_table_len = value_len;
                break;
            }
        } else {
        	gpio_set_high_level(GPIO_PB2);
            uint32_t len = *(uint32_t *) (s_ble_opus_table);
            uint32_t remain_len = len + 8 - s_ble_opus_table_len;
            tmemcpy(s_ble_opus_table + s_ble_opus_table_len, value, remain_len);
            s_ble_opus_table_len = 0;
            value_len -= remain_len;
            value += remain_len;
            ble_gatts_notify(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 7, s_ble_opus_table, len + 8);
            gpio_set_low_level(GPIO_PB2);
        }
    } while (1);
    */

    return 0;
}

int app_tlk_record_report_file_data(uint8_t *file_data, uint16_t file_len, gatts_notify_callback callback)
{
    uint16_t mtu = ble_host_att_get_mtu(s_acl_conn_handle);
    if (mtu < 23) {
        if (callback) {
            callback(0, false);
        }
        return -1;
    }

    uint16_t max_pdu_len = mtu - 3;
    uint8_t  offset      = 0;

    while (file_len > max_pdu_len) {
        uint8_t curr_len = max_pdu_len;
        ble_gatts_notify(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 10, file_data + offset, curr_len);
        offset += curr_len;
        file_len -= curr_len;
    }

    return ble_gatts_notify_with_callback(s_acl_conn_handle, svcTlkRecordGroup.startHandle + 10, file_data + offset, file_len, callback);
}
