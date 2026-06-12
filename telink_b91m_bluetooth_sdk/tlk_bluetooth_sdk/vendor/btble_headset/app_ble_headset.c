/********************************************************************************************************
 * @file    app_ble_headset.c
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
#include <stdio.h>
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app_config.h"
#include "app_ble_headset.h"
#include "app_ble_hid.h"
#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkapp/tlkapp.h"
#include "tlkmw/ble/le_audio/inc/lea_us.h"


#define LE_HEADSET_DEVICE_NAME          "LE headset"

#define LE_HEADSET_DEVICE_NAME_MAX_SIZE 64

static char device_name[LE_HEADSET_DEVICE_NAME_MAX_SIZE] = {0};

static struct tlk_mw_lea_cap_headset_param s_le_headset_param = {
    .device_name = device_name,
    .interval    = 50,
    .volume      = 150,
};

/**
 * @brief       Initialize the BLE headset application
 * @param[in]   mac_public - Public MAC address of the device used to generate device name
 * @return      none
 * @note        This function initializes the LE Audio unicast server headset with given parameters
 */
void app_ble_headset_init(uint8_t mac_public[6])
{
    snprintf(device_name, LE_HEADSET_DEVICE_NAME_MAX_SIZE, LE_HEADSET_DEVICE_NAME "-%02X%02X%02X%02X%02X%02X", mac_public[5], mac_public[4], mac_public[3], mac_public[2],
             mac_public[1], mac_public[0]);

    lea_unicast_server_headset_initial(&s_le_headset_param);
    tlk_printf("BLE name: %s", device_name);
}