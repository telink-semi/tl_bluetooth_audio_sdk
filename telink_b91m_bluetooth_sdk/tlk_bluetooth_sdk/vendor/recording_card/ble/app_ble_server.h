/********************************************************************************************************
 * @file    app_ble_server.h
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

void app_ble_server_init(void);

void app_server_acl_connected(uint16_t conn_handle);

void app_server_acl_disconnected(void);

/**
 *   @brief  Set the battery status and level.
 *
 *   @param  status  Battery status, 0x00: no charging, 0x01: charging, 0x02: Finished charging.
 *   @param  level   Battery level, 0-100.
 */
void app_ble_set_battery_status(uint8_t status, uint8_t level);

/**
 *   @brief  Set the SD card size.
 *
 *   @param  total_size      Total size of the SD card, unit: Byte.
 *   @param  not_used_size   Not used size of the SD card, unit: Byte.
 */
void app_ble_set_sd_size(uint32_t total_size, uint32_t not_used_size);

void app_ble_set_bluetooth_version(char version[16]);

void app_ble_ack_start_send_file_ok(void);
int  app_ble_notify_value(uint8_t *value, uint16_t value_len);

int app_ble_send_file_info(char *file_name_table, uint8_t file_len, gatts_notify_callback callback);
