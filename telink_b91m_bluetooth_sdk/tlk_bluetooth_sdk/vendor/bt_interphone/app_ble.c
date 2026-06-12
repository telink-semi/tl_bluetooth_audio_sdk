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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app_ble_headset.h"
#include "app_ble_hid.h"

#include "tlkmw/ble/le_controller/tlkmdi_ble_ctrl.h"

void tlkapp_host_le_init(void)
{
    //////////////////////////// BLE stack Initialization  Begin //////////////////////////////////

    u8 mac_public[6];
    tlkhal_get_bluetooth_mac(mac_public);


    ble_host_v1_init();
    ble_host_v1_set_bd_addr(mac_public);
    blc_svc_addOtaV2Group();
    // Legacy Advertising
    extern int  app_ble_acl_peripheral_init(void);
    extern void app_ble_acl_peripheral_start(void);
    app_ble_acl_peripheral_init();
    app_ble_acl_peripheral_start();
    // (Extended Advertising)
    // app_ble_headset_init(mac_public);
    app_ble_hid_init();
    blc_svc_calculateDatabaseHash();
}

void tlkapp_host_le_start(void) {}
