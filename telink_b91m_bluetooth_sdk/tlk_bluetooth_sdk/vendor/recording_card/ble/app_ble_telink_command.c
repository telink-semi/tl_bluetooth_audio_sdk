/********************************************************************************************************
 * @file    app_ble_telink_command.c
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

#include "app_ble_telink_server.h"

#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "../app_recording_card/app_recording_card_api.h"
#include "../app_recording_card/data_path/app_rc_stream.h"
#include "../app_recording_card/data_path/wifi/app_rc_wifi_api.h"

void app_ble_telink_search_opus_table(void)
{
    app_rc_api_report_opus_table();
}

void app_ble_telink_search_wav_table(void)
{
    app_rc_api_report_wav_table();
}

void app_ble_telink_delete_file(uint8_t *value, uint8_t value_len)
{
    (void)value_len;
    char file_name[256] = {0};
    memcpy(file_name, value, value_len);
    tlk_printf("delete file:%s", file_name);

    app_rc_api_delete_file(file_name);
}

void app_ble_telink_rename_file(uint8_t *old_file_name, uint8_t old_file_name_len, uint8_t *new_file_name, uint8_t new_file_name_len)
{
    char file_name_old[256] = {0};
    memcpy(file_name_old, old_file_name, old_file_name_len);
    char file_name_new[256] = {0};
    memcpy(file_name_new, new_file_name, new_file_name_len);

    tlk_printf("rename file:%s to %s", file_name_old, file_name_new);
    app_rc_api_rename_file(file_name_old, file_name_new);
}

void app_ble_telink_open_wifi(uint8_t *wifi_name, uint8_t wifi_name_len, uint8_t *wifi_password, uint8_t wifi_password_len)
{
    tlk_printf("open wifi:%.*s, password:%.*s", wifi_name_len, wifi_name, wifi_password_len, wifi_password);
    app_tlk_record_report_wifi_state(0x00); //TODO: BQ
}

void app_ble_telink_read_file(uint8_t *value, uint8_t value_len, uint32_t file_offset)
{
    (void)value_len;
    char file_name[256] = {0};
    memcpy(file_name, value, value_len);
    tlk_printf("app_ble_telink_read_file value++++ = %s, value_len = %d\n", file_name, value_len);
    app_rc_api_send_opus_file((char *)file_name, file_offset);
}

void app_ble_telink_read_file_wifi(uint8_t *value, uint8_t value_len, uint32_t file_offset)
{
    char file_name[256] = {0};
    memcpy(file_name, value, value_len);
    tlk_printf("app_ble_telink_read_file_wifi value = %s, value_len = %d\n", file_name, value_len);
    app_rc_api_send_file_by_Wifi((char *)file_name, file_offset);
}

void app_ble_telink_set_time(uint32_t time)
{
    tlkdrv_rtc_setTimeStamp(time);
    app_tlk_record_report_set_time_result(0x00);
}

void app_ble_telink_get_time(void)
{
    uint32_t time = tlkdrv_rtc_getTimeStamp(0);
    app_tlk_record_report_current_time(time);
}

void app_ble_telink_get_wifi_name(void)
{
#if TLK_CFG_WIFI_LOGIC_ENABLE
    uint8_t buffer[256] = {0};
    int     len         = app_rc_wifi_get_name(buffer, sizeof(buffer));
    if (len < 0) {
        return;
    }
    app_tlk_record_report_wifi_name((char *)buffer, len);
#endif
}
