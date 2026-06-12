/********************************************************************************************************
 * @file    app_ble_command.h
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

void app_ble_search_file_table(uint8_t *value, uint8_t value_len);

int app_ble_delete_file(uint8_t *value, uint8_t value_len);

/**
 *   @brief BLE want send file to peer device.
 *
 *   @param[in] value: the file name to send.
 *   @param[in] value_len: the length of file name.
 *
 *   @return the size of file data.
 */
uint32_t app_ble_send_file(uint8_t *value, uint8_t value_len);

void app_ble_start_send_file(void);
