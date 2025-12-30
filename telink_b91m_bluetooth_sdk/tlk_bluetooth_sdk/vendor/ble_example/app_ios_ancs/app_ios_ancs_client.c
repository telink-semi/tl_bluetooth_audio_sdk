/********************************************************************************************************
 * @file    app_ios_ancs_client.c
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
#include "app_ios_ancs_client.h"

#if IS_DEMO_SELECTED(APP_BLE_IOS_ANCS)

#define SERVICE_ID_ANCS         (PRF_SERVICE_ID_USER_START + 0x01)

#define BLE_CONN_HANDLE_INVALID 0xFFFF

static void                                       ble_ancsc_connect(uint16_t conn_handle, enum prf_acl_state_change state);
static void                                       ble_ancsc_discovery(uint16_t conn_handle, enum prf_disc_type type);
static const struct ble_gatt_ssdp_no_include_list s_disc_ancs;

static const struct ble_prf_client_param s_ancs_client_params = {
    .used_acl_role = PRF_USED_ACL_ROLE_CONNECT,
    .service_id    = SERVICE_ID_ANCS,
    .init          = NULL,
    .connect       = ble_ancsc_connect,
    .discovery     = ble_ancsc_discovery,
};

static struct app_ios_ancs_client_control s_ancs_client_ctrl = {
    .prf_process =
        {
            .client_param = &s_ancs_client_params,
        },
};

/**
 * @brief  Initialize ANCS client module.
 *          Register ANCS client profile with the BLE profile framework.
 */
void app_ios_ancs_client_init(void)
{
    ble_prf_register_client_module(&s_ancs_client_ctrl.prf_process, NULL);
}

/**
 * @brief  Handle ANCS client ACL connection state change.
 *          Update connection handle based on connection state.
 *
 * @param[in] conn_handle  Connection handle.
 * @param[in] state        ACL state change type.
 */
static void ble_ancsc_connect(uint16_t conn_handle, enum prf_acl_state_change state)
{
    memset(&s_ancs_client_ctrl.ancs_client, 0, sizeof(s_ancs_client_ctrl.ancs_client));
    if (state == PRF_ACL_CONNECT) {
        s_ancs_client_ctrl.ancs_client.conn_handle = conn_handle;
    } else if (state == PRF_ACL_DISCONNECT) {
        s_ancs_client_ctrl.ancs_client.conn_handle = BLE_CONN_HANDLE_INVALID;
    }
}

/**
 * @brief  Start ANCS service discovery.
 *          Begin discovering ANCS service and characteristics on peer device.
 *
 * @param[in] conn_handle  Connection handle.
 * @param[in] type         Discovery type.
 */
static void ble_ancsc_discovery(uint16_t conn_handle, enum prf_disc_type type)
{
    (void)type;
    ble_host_gatt_ssdp_start_no_include(conn_handle, &s_disc_ancs);
}

/**
 * @brief  Callback when ANCS service is found during discovery.
 *
 * @param[in] conn_handle   Connection handle.
 * @param[in] count         Number of services found.
 * @param[in] start_handle  Service start handle.
 * @param[in] end_handle    Service end handle.
 */
static void app_ios_ancs_client_found_service_callback(uint16_t conn_handle, uint8_t count, uint16_t start_handle, uint16_t end_handle)
{
    tlk_printf("Service Callback, conn_handle: 0x%03x, count: %d, start:0x%04x, end:0x%04x", conn_handle, count, start_handle, end_handle);
}

/**
 * @brief  Callback when Notification Source characteristic is found.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] service_count  Service count.
 * @param[in] properties     Characteristic properties.
 * @param[in] value_handle   Characteristic value handle.
 */
static void app_ios_ancs_client_notification_source_found_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    tlk_printf("Notification Source Found, conn_handle: 0x%03x, service_count: %d, properties: 0x%02x, value_handle: 0x%04x", conn_handle, service_count, properties, value_handle);
}

/**
 * @brief  Callback when Control Point characteristic is found.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] service_count  Service count.
 * @param[in] properties     Characteristic properties.
 * @param[in] value_handle   Characteristic value handle.
 */
static void app_ios_ancs_client_control_point_found_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    tlk_printf("Control Point Found, conn_handle: 0x%03x, service_count: %d, properties: 0x%02x, value_handle: 0x%04x", conn_handle, service_count, properties, value_handle);
}

/**
 * @brief  Callback when Data Source characteristic is found.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] service_count   Service count.
 * @param[in] properties     Characteristic properties.
 * @param[in] value_handle   Characteristic value handle.
 */
static void app_ios_ancs_client_data_source_found_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    tlk_printf("Data Source Found, conn_handle: 0x%03x, service_count: %d, properties: 0x%02x, value_handle: 0x%04x", conn_handle, service_count, properties, value_handle);
}

static const struct gatt_ssdp_characteristic s_disc_ancs_char[] = {
    {
        .subscribe_notify        = true,
        .characteristic_uuid     = &ble_ios_ancs_notification_source,
        .characteristic_callback = app_ios_ancs_client_notification_source_found_callback,
    },
    {
        .characteristic_uuid     = &ble_ios_ancs_control_point,
        .characteristic_callback = app_ios_ancs_client_control_point_found_callback,
    },
    {
        .subscribe_notify        = true,
        .characteristic_uuid     = &ble_ios_ancs_data_source,
        .characteristic_callback = app_ios_ancs_client_data_source_found_callback,
    },
};

static const struct ble_gatt_ssdp_no_include_list s_disc_ancs = {.max_service_count = 1,
                                                                 .service =
                                                                     {
                                                                         .service_uuid     = &ble_ios_ancs_service_uuid,
                                                                         .service_callback = app_ios_ancs_client_found_service_callback,
                                                                     },
                                                                 .characteristic_table = {
                                                                     .characteristic_size = ARRAY_SIZE(s_disc_ancs_char),
                                                                     .characteristic      = s_disc_ancs_char,
                                                                 }};

#endif
