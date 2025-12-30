/********************************************************************************************************
 * @file    app_lea_uc_filter_device.h
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
/**
 * @brief  Initialize device filter module.
 *          Clear all stored device information.
 */
void app_filter_device_init(void);

/**
 * @brief  Add device to filter list.
 *          Check if device already exists, if not add it to the list.
 *
 * @param[in] addr_type  Address type.
 * @param[in] addr       Device address, 6 bytes.
 *
 * @return  Device index if added successfully, 0xFF if failed (list full or already exists).
 */
uint8_t app_filter_device_add(uint8_t addr_type, const uint8_t *addr);
