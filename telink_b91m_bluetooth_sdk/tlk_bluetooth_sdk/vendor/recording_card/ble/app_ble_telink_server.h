/********************************************************************************************************
 * @file    app_ble_telink_server.h
 *
 * @brief   This is the header file for TLSR/TL
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

void app_tlk_record_init(void);
void app_tlk_record_acl_connect(uint16_t conn_handle);
void app_tlk_record_acl_disconnect(void);


int app_tlk_record_report_start_record(void);
int app_tlk_record_report_stop_record(void);

int app_tlk_record_report_delete_file_result(uint8_t result);
int app_tlk_record_report_rename_file_result(uint8_t result);
int app_tlk_record_report_file_size(uint32_t file_size);
int app_tlk_record_report_file_transport_finish(void);

int app_tlk_record_report_set_time_result(uint8_t result);
int app_tlk_record_report_current_time(uint32_t time);

int app_tlk_record_report_change_channel(void);

int app_tlk_record_report_data(uint8_t *value, uint16_t value_len);

int app_tlk_record_report_file_table(uint8_t *file_table, uint16_t file_len, gatts_notify_callback callback);
int app_tlk_record_report_file_data(uint8_t *file_data, uint16_t file_len, gatts_notify_callback callback);

int app_tlk_record_report_wifi_state(uint8_t state);

int app_tlk_record_report_wifi_name(char *wifi_name, uint16_t len);
