/********************************************************************************************************
 * @file    app_ble.c
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

#if TLK_STK_BLE_ENABLE

#include "stack/multiCoreComm/service/service_shareMemory.h"

#include "tlkmw/ble/le_audio/inc/lea_us_headset.h"

#define LE_HEADSET_DEVICE_NAME          "LEA-bttpsll(demo)"

#define LE_HEADSET_DEVICE_NAME_MAX_SIZE 64

static uint8_t mac_public[6];

static char device_name[LE_HEADSET_DEVICE_NAME_MAX_SIZE] = {0};

/**
 * @brief      Initialize the BLE host for the headset application
 * @param[in]  none
 * @return     none
 */
void tlkapp_host_le_init(void)
{
    uint8_t mac_random_static[6];

    blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);

    tlk_d25f_register_hci_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_BLE, ble_host_hci_rx_complete_packet);

    ble_host_v1_init();

    ble_host_v1_set_bd_addr(mac_public);

    snprintf(device_name, LE_HEADSET_DEVICE_NAME_MAX_SIZE, LE_HEADSET_DEVICE_NAME "-%02X%02X%02X%02X%02X%02X", mac_public[5], mac_public[4], mac_public[3], mac_public[2],
             mac_public[1], mac_public[0]);

    blc_svc_calculateDatabaseHash();

    tlk_printf("BLE Host Initialized, MAC: %s", addr_to_str(mac_public));
    tlk_printf("LE Device Name: %s", device_name);
}

static struct lea_us_headset_param s_le_headset_param = {
    .device_name = device_name,
    .interval    = 50,
    .volume      = 150,
};

/**
 * @brief      Start the LE audio unicast server headset
 * @param[in]  none
 * @return     none
 */
void tlkapp_host_le_start(void)
{
    lea_unicast_server_headset_initial(&s_le_headset_param);
    lea_unicast_server_headset_stop(NULL);
}

#endif // TLK_STK_BLE_ENABLE