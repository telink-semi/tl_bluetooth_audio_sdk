/********************************************************************************************************
 * @file    app_ios_ancs.c
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

#include "../app_example.h"

#include "app_ios_ancs.h"

#if IS_DEMO_SELECTED(APP_BLE_IOS_ANCS)

UUID128BIT_DEF(ble_ios_ancs_service_uuid, BLE_IOS_ANCS_SERVICE);
UUID128BIT_DEF(ble_ios_ancs_notification_source, BLE_IOS_ANCS_NOTIFICATION_SOURCE);
UUID128BIT_DEF(ble_ios_ancs_control_point, BLE_IOS_ANCS_CONTROL_POINT);
UUID128BIT_DEF(ble_ios_ancs_data_source, BLE_IOS_ANCS_DATA_SOURCE);

#define APP_BLE_ACL_PERIPHERAL_MAX_COUNT 1
#define BLE_PERIPHERAL_DEVICE_NAME       "telink ANCS Client"
#define BLE_CONN_HANDLE_INVALID          0xFFFF

static uint16_t s_acl_conn_handle = BLE_CONN_HANDLE_INVALID;

struct acl_ad_data_incomplete_service_uuid
{
    struct ltv_data header;
    uint16_t        uuid[2];
} __attribute__((packed));

static const struct ad_data_flags s_adv_flags = {
    .header.length                      = 0x02,
    .header.type                        = DT_FLAGS,
    .flags.le_limited_discoverable_mode = 1,
    .flags.br_edr_not_supported         = 1,
};

static const struct acl_ad_data_incomplete_service_uuid s_adv_uuid = {
    .header.length = 0x05,
    .header.type   = DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID,
    .uuid          = {SERVICE_UUID_HUMAN_INTERFACE_DEVICE, SERVICE_UUID_BATTERY},
};

static const struct ad_data_complete_local_name_complete s_adv_complete_name = {
    .header.length = sizeof(BLE_PERIPHERAL_DEVICE_NAME),
    .header.type   = DT_COMPLETE_LOCAL_NAME,
    .name          = BLE_PERIPHERAL_DEVICE_NAME,
};

static const struct ltv_data *adv_data[] = {
    (const struct ltv_data *)&s_adv_flags, (const struct ltv_data *)&s_adv_complete_name, (const struct ltv_data *)&s_adv_uuid,
    NULL, // end of advertisement data
};
static const struct ltv_data *scan_rsp_data[] = {
    (const struct ltv_data *)&s_adv_complete_name,
    NULL, // end of advertisement data
};

static void app_connected_callback(struct ble_host_conn *conn);
static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);

static const struct ble_host_acl_conn_callbacks s_app_acl_callbacks = {
    .connected    = app_connected_callback,
    .disconnected = app_disconnected_callback,
};

/**
 * @brief  Callback when ANCS service discovery is completed.
 *          Check if peer device is an iOS device by verifying ANCS service presence.
 *
 * @param[in] conn_handle  Connection handle.
 * @param[in] cid          Channel ID.
 * @param[in] err          Error code.
 * @param[in] service      Discovered service information.
 *
 * @return  false to stop discovery.
 */
static bool app_discovery_ancs_callback(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_disc_services *service)
{
    (void)conn_handle;
    (void)err;
    (void)cid;
    if (service->start_handle != 0x0000) {
        tlk_printf("Peer Device ANCS Service found, start handle:0x%x, end handle:0x%x", service->start_handle, service->end_handle);
        tlk_printf("The Device is iOS Device");
    } else {
        tlk_printf("Peer Device ANCS Service not found");
        tlk_printf("The Device is not iOS Device");
    }
    return false;
}

/**
 * @brief  Callback when ACL connection is established.
 *          Start pairing and discover ANCS service on peer device.
 *
 * @param[in] conn  Connection information.
 */
static void app_connected_callback(struct ble_host_conn *conn)
{
    tlk_printf("[APP] connected callback, ACL handle: 0x%03x", conn->conn_handle);
    s_acl_conn_handle = conn->conn_handle;
    ble_host_smp_start_pairing_peripheral(conn->conn_handle);
    ble_host_gattc_discover_primary_service_by_uuid(s_acl_conn_handle, LE_L2CAP_CID_ATT, &ble_ios_ancs_service_uuid, app_discovery_ancs_callback, NULL);
}

/**
 * @brief  Callback when ACL connection is disconnected.
 *          Restart advertising to allow new connections.
 *
 * @param[in] conn    Connection information.
 * @param[in] reason  Disconnection reason.
 */
static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    tlk_printf("[APP] disconnected callback, ACL handle: 0x%03x, reason: 0x%02x", conn->conn_handle, reason);

    s_acl_conn_handle = BLE_CONN_HANDLE_INVALID;
    ble_host_gap_adv_start();
}

/**
 * @brief  Initialize ANCS client module.
 *          Register ANCS client profile with the BLE profile framework.
 */
void app_ios_ancs_client_init(void);

/**
 * @brief  Initialize iOS ANCS application module.
 *          Configure advertisement data, register services, initialize ANCS client and ACL callbacks.
 *
 * @return  0 on success.
 */
int INIT(APP_BLE_IOS_ANCS)(void)
{
    ble_host_gap_adv_set_adv_ind_param(150, adv_data, scan_rsp_data);

    blc_svc_addCoreGroup();
    blc_svc_addDisGroup();
    ble_basic_register_BAS_control_server(BLE_BASS_INITIAL_PARAMS(100, DEVICE_NO_CHARGING));
    ble_basic_register_DIS_control_server();
    blc_svc_calculateDatabaseHash();
    blc_svc_setDeviceName(BLE_PERIPHERAL_DEVICE_NAME);

    app_ios_ancs_client_init();
    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_app_acl_callbacks);
    ble_host_smp_initial(BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS);
    ble_host_smp_store_init(4, 0);
    tlk_printf("[APP] ISO ANCS Client Test initialized");
    return 0;
}

/**
 * @brief  Start iOS ANCS application module.
 *          Start BLE advertising to allow iOS devices to connect.
 */
void START(APP_BLE_IOS_ANCS)(void)
{
    ble_host_gap_adv_start();
}

#endif
