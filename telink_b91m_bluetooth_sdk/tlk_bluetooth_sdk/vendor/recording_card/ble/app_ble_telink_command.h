/********************************************************************************************************
 * @file    app_ble_telink_command.h
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

void app_ble_telink_search_opus_table(void);

void app_ble_telink_search_wav_table(void);

void app_ble_telink_delete_file(uint8_t *value, uint8_t value_len);

void app_ble_telink_rename_file(uint8_t *old_file_name, uint8_t old_file_name_len, uint8_t *new_file_name, uint8_t new_file_name_len);

void app_ble_telink_read_file(uint8_t *value, uint8_t value_len, uint32_t file_offset);

void app_ble_telink_open_wifi(uint8_t *wifi_name, uint8_t wifi_name_len, uint8_t *wifi_password, uint8_t wifi_password_len);

void app_ble_telink_read_file_wifi(uint8_t *value, uint8_t value_len, uint32_t file_offset);

void app_ble_telink_set_time(uint32_t time);

void app_ble_telink_get_time(void);

void app_ble_telink_get_wifi_name(void);
