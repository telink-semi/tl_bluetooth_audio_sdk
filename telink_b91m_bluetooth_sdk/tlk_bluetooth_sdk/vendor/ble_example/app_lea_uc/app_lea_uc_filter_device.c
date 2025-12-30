/********************************************************************************************************
 * @file    app_lea_uc_filter_device.c
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
#include <string.h>

#include "common/types.h"

#define APP_FILTER_DEVICE_MAX_NUMBER 32

union app_filter_device_info
{
    struct
    {
        uint8_t addr_type;
        uint8_t addr[6];
    };

    uint64_t addr_info;
};

static uint8_t                      s_device_num = 0;
static union app_filter_device_info s_device_info[APP_FILTER_DEVICE_MAX_NUMBER];

/**
 * @brief  Initialize device filter module.
 *          Clear all stored device information.
 */
void app_filter_device_init(void)
{
    s_device_num = 0;
    memset(s_device_info, 0, sizeof(s_device_info));
}

/**
 * @brief  Add device to filter list.
 *          Check if device already exists, if not add it to the list.
 *
 * @param[in] addr_type  Address type.
 * @param[in] addr       Device address, 6 bytes.
 *
 * @return  Device index if added successfully, 0xFF if failed (list full or already exists).
 */
uint8_t app_filter_device_add(uint8_t addr_type, const uint8_t *addr)
{
    if (s_device_num >= APP_FILTER_DEVICE_MAX_NUMBER) {
        return 0xFF;
    }

    union app_filter_device_info addr_info = {
        .addr_info = addr_type,
    };
    memcpy(addr_info.addr, addr, 6);

    for (size_t i = 0; i < s_device_num; i++) {
        if (s_device_info[i].addr_info == addr_info.addr_info) {
            return 0xFF;
        }
    }

    s_device_info[s_device_num++] = addr_info;

    return s_device_num - 1;
}
