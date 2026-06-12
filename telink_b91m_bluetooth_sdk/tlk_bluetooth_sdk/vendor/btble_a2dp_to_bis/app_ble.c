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
#include "stack/btble.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"
#include "stack/ble/ble.h"
#include "app_ble.h"
#include "app_ble_bis.h"
#include "app_ble_bis_sync.h"
#include "app_ble_bis_sink.h"

#include "tlkmw/ble/le_controller/tlkmdi_ble_ctrl.h"

/**
 * @brief       Initialize the BLE host
 * @param       none
 * @return      none
 * @note        This function initializes the BLE stack, sets up MAC addresses,
 *              initializes BLE host modules including extended advertising,
 *              BIG, extended scanning, periodic advertising sync, BIG sync,
 *              BIS sink, and SMP security manager
 */
void tlkapp_host_le_init(void)
{
    u8 mac_public[6];

    tlkhal_get_bluetooth_mac(mac_public);

    ble_host_v1_init();
    ble_host_v1_set_bd_addr(mac_public);

    // host initial extend adv module.
    ble_host_gap_extend_adv_init();
    ble_host_gap_big_init();
    ble_host_gap_ext_scan_init();
    ble_host_gap_pa_sync_init();
    ble_host_gap_big_sync_init();
    app_ble_bis_sink_init();
    ble_host_smp_initial(BLE_HOST_SMP_INIT_PARAMS_DEFAULT);
    ble_host_smp_store_init(4, 0);
}

/**
 * @brief       Main loop for user BLE functionality
 * @param       none
 * @return      none
 */
void user_ble_main_loop(void)
{
    ble_host_sal_get_task()();
}