/********************************************************************************************************
 * @file    app_lea_source.c
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
#include "stack/ble/ble.h"
#include "tlkmw/ble/le_audio/inc/lea_unicast_client.h"

#include "../app_example.h"

static void app_ble_connected_callback(struct ble_host_conn *conn)
{
    tlk_printf("[APP] connected, conn_handle: 0x%03x, role: %d", conn->conn_handle, conn->role);
}

static void app_ble_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    tlk_printf("[APP] disconnected, conn_handle: 0x%03x, reason: 0x%02x, role: %d", conn->conn_handle, reason, conn->role);
}

static const struct ble_host_acl_conn_callbacks s_le_acl_callbacks = {
    .connected    = app_ble_connected_callback,
    .disconnected = app_ble_disconnected_callback,
};

int INIT(APP_BLE_LEA_SOURCE)(void)
{
    tlk_printf("[APP] le audio source init");
    blc_svc_addCoreGroup();
    blc_svc_addGtbsGroup();
    blc_svc_addGmcsGroup();
    blc_svc_addDisGroup();
    ble_lea_register_TMAS_control_server(BLE_TMASS_INITIAL_PARAMS(1, 0, 1, 0, 0, 0));
    blc_svc_calculateDatabaseHash();

    ble_host_smp_initial(BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS);

    return 0;
}

void START(APP_BLE_LEA_SOURCE)(void)
{
    tlk_printf("[APP] le audio source start");
    lea_unicast_client_start();
    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_le_acl_callbacks);
}
