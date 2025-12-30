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
#include "tlkmw/ble/le_audio/inc/lea_unicast_client.h"
#include "tlkmw/sys_dev/key/tlkdrv_key.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_config.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_disk_userSetting.h"

/**
 * @brief      Initialize the BLE stack with MAC address and service profiles
 * @param[in]  none
 * @return     none
 */
static void ble_stack_init(void)
{
    uint8_t mac[6], mac_random[6];

    blc_initMacAddress(flash_sector_mac_address, mac, mac_random);

    ble_host_v1_init();
    ble_host_v1_set_bd_addr(mac);

    blc_svc_addCoreGroup();
    blc_svc_addGtbsGroup();
    blc_svc_addGmcsGroup();
    blc_svc_addDisGroup();
    ble_lea_register_TMAS_control_server(BLE_TMASS_INITIAL_PARAMS(1, 0, 1, 0, 0, 0));
    blc_svc_calculateDatabaseHash();

    ble_host_smp_initial(BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS);
}

/**
 * @brief      Callback function when BLE connection is established
 * @param[in]  conn - pointer to the connection information structure
 * @return     none
 */
static void app_ble_connected_callback(struct ble_host_conn *conn)
{
    tlk_printf("[APP] connected, conn_handle: 0x%03x, role: %d", conn->conn_handle, conn->role);
}

/**
 * @brief      Callback function when BLE connection is terminated
 * @param[in]  conn - pointer to the connection information structure
 * @param[in]  reason - disconnection reason code
 * @return     none
 */
static void app_ble_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    tlk_printf("[APP] disconnected, conn_handle: 0x%03x, reason: 0x%02x, role: %d", conn->conn_handle, reason, conn->role);
}

static const struct ble_host_acl_conn_callbacks s_le_acl_callbacks = {
    .connected    = app_ble_connected_callback,
    .disconnected = app_ble_disconnected_callback,
};

/**
 * @brief      Initialize the LE audio source application
 * @param[in]  none
 * @return     none
 */
void tlkapp_host_le_init(void)
{
    tlk_printf("[APP] le audio source init");
    ble_stack_init();
}

/**
 * @brief      Start the LE audio source application
 * @param[in]  none
 * @return     none
 */
void tlkapp_host_le_start(void)
{
    tlk_printf("[APP] le audio source start");
    lea_unicast_client_start();
    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_le_acl_callbacks);
}