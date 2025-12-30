/********************************************************************************************************
 * @file    app_ssdp.c
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

#include "../app_key.h"
#include "../app_example.h"

#define BLE_CONN_HANDLE_INVALID 0xFFFF

static uint16_t s_conn_handle = BLE_CONN_HANDLE_INVALID;

/**
 *   Define the GATT service discovery information.
 */
static struct app_gatt_ssdp_info
{
    uint8_t client_support_features;
    uint8_t server_support_features;
    uint8_t database_hash[16];

    char     device_name[32];
    uint16_t device_name_len;
    uint16_t appearance;
    uint16_t peripheral_conn_param_range_min;
    uint16_t peripheral_conn_param_range_max;
    uint16_t peripheral_conn_param_latency;
    uint16_t peripheral_conn_param_timeout;
    uint8_t  central_address_resolution;
} s_app_ssdp_info;

/**
 * @brief  Callback when GATT service is found during discovery.
 *          Print GATT service information including client/server supported features and database hash.
 *
 * @param[in] conn_handle   Connection handle.
 * @param[in] count        Service count.
 * @param[in] start_handle Service start handle.
 * @param[in] end_handle   Service end handle.
 */
static void app_gatt_ssdp_gatt_found_callback(uint16_t conn_handle, uint8_t count, uint16_t start_handle, uint16_t end_handle)
{
    (void)conn_handle;
    if (count == 0x01) {
        tlk_printf("[APP] GATT Service found, start_handle = 0x%04x, end_handle = 0x%04x", start_handle, end_handle);
    } else {
        tlk_printf("[APP] GATT Service found finish. GATT Server Information:");
        tlk_printf("        GATT Client Supported Features: Robust Caching: %d, EATT:%d, Multiple Ntf:%d", s_app_ssdp_info.client_support_features & 0x01,
                   (s_app_ssdp_info.client_support_features >> 1) & 0x01, (s_app_ssdp_info.client_support_features >> 2) & 0x01);
        tlk_printf("        GATT Server Supported Features: EATT:%d", s_app_ssdp_info.server_support_features & 0x01);
        tlk_printf("        Database Hash: %s", hex_to_str(s_app_ssdp_info.database_hash, sizeof(s_app_ssdp_info.database_hash)));
    }
}

/**
 * @brief  Callback when unknown characteristic is found in GATT service.
 *
 * @param[in] conn_handle  Connection handle.
 * @param[in] uuid         Characteristic UUID.
 * @param[in] properties   Characteristic properties.
 * @param[in] value_handle Characteristic value handle.
 */
static void app_gatt_ssdp_att_found_unknown_char_callback(uint16_t conn_handle, const struct att_uuid *uuid, uint8_t properties, uint16_t value_handle)
{
    (void)conn_handle;
    tlk_printf("[APP] GATT Found unknown characteristic, uuid = %s, properties = 0x%02x, value_handle = 0x%04x", ble_att_uuid_format(uuid), properties, value_handle);
}

/**
 * @brief  Callback when Service Changed characteristic is found.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] service_count Service count.
 * @param[in] properties    Characteristic properties.
 * @param[in] value_handle  Characteristic value handle.
 */
static void app_gatt_ssdp_found_service_changed_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    (void)conn_handle;
    (void)service_count;
    tlk_printf("[APP] Service Changed found, properties = 0x%02x, value_handle = 0x%04x", properties, value_handle);
}

/**
 * @brief  Callback when Service Changed CCC subscription is completed.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] ccc_handle  CCC handle.
 * @param[in] result      Subscription result.
 */
static void app_gatt_ssdp_subscribe_service_changed_ccc_callback(uint16_t conn_handle, uint16_t ccc_handle, uint8_t result)
{
    (void)conn_handle;
    tlk_printf("[APP] Service Changed subscribe ccc, handle = 0x%04x, result = %d", ccc_handle, result);
}

static void app_gatt_ssdp_found_client_supported_features_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    (void)conn_handle;
    (void)service_count;
    tlk_printf("[APP] Client Supported Features found, properties = 0x%02x, value_handle = 0x%04x", properties, value_handle);
}

/**
 * @brief  Callback to prepare read buffer for Client Supported Features characteristic.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] value_handle  Characteristic value handle.
 * @param[out] read          Pointer to read buffer.
 * @param[out] read_len      Pointer to read length (unused).
 * @param[out] read_max_size Maximum read size.
 */
static void app_gatt_ssdp_start_read_client_supported_features_callback(uint16_t conn_handle, uint16_t value_handle, uint8_t **read, uint16_t **read_len, uint16_t *read_max_size)
{
    (void)conn_handle;
    (void)value_handle;
    (void)read_len;
    *read          = &s_app_ssdp_info.client_support_features;
    *read_max_size = sizeof(s_app_ssdp_info.client_support_features);
}

/**
 * @brief  Callback when Database Hash characteristic is found.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] service_count  Service count.
 * @param[in] properties     Characteristic properties.
 * @param[in] value_handle   Characteristic value handle.
 */
static void app_gatt_ssdp_found_database_hash_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    (void)conn_handle;
    (void)service_count;
    tlk_printf("[APP] Database Hash found, properties = 0x%02x, value_handle = 0x%04x", properties, value_handle);
}

/**
 * @brief  Callback to prepare read buffer for Database Hash characteristic.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] value_handle   Characteristic value handle.
 * @param[out] read           Pointer to read buffer.
 * @param[out] read_len       Pointer to read length (unused).
 * @param[out] read_max_size  Maximum read size.
 */
static void app_gatt_ssdp_start_read_database_hash_callback(uint16_t conn_handle, uint16_t value_handle, uint8_t **read, uint16_t **read_len, uint16_t *read_max_size)
{
    (void)conn_handle;
    (void)value_handle;
    (void)read_len;
    *read          = s_app_ssdp_info.database_hash;
    *read_max_size = sizeof(s_app_ssdp_info.database_hash);
}

/**
 * @brief  Callback when Server Supported Features characteristic is found.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] service_count  Service count.
 * @param[in] properties     Characteristic properties.
 * @param[in] value_handle   Characteristic value handle.
 */
static void app_gatt_ssdp_found_server_supported_features_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    (void)conn_handle;
    (void)service_count;
    tlk_printf("[APP] Server Supported Features found, properties = 0x%02x, value_handle = 0x%04x", properties, value_handle);
}

/**
 * @brief  Callback to prepare read buffer for Server Supported Features characteristic.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] value_handle   Characteristic value handle.
 * @param[out] read          Pointer to read buffer.
 * @param[out] read_len       Pointer to read length (unused).
 * @param[out] read_max_size  Maximum read size.
 */
static void app_gatt_ssdp_start_read_server_supported_features_callback(uint16_t conn_handle, uint16_t value_handle, uint8_t **read, uint16_t **read_len, uint16_t *read_max_size)
{
    (void)conn_handle;
    (void)value_handle;
    (void)read_len;
    *read          = &s_app_ssdp_info.server_support_features;
    *read_max_size = sizeof(s_app_ssdp_info.server_support_features);
}

static const struct gatt_ssdp_characteristic s_app_gatt_char_info[] = {{
                                                                           .subscribe_indicate      = true,
                                                                           .characteristic_uuid     = &characteristicServiceChangedAttUuid,
                                                                           .characteristic_callback = app_gatt_ssdp_found_service_changed_callback,
                                                                           .subscribe_ccc_callback  = app_gatt_ssdp_subscribe_service_changed_ccc_callback,
                                                                       },
                                                                       {
                                                                           .read_value              = true,
                                                                           .characteristic_uuid     = &characteristicClientSupportedFeaturesAttUuid,
                                                                           .characteristic_callback = app_gatt_ssdp_found_client_supported_features_callback,
                                                                           .read_value_callback     = app_gatt_ssdp_start_read_client_supported_features_callback,
                                                                       },
                                                                       {
                                                                           .read_value              = true,
                                                                           .characteristic_uuid     = &characteristicDatabaseHashAttUuid,
                                                                           .characteristic_callback = app_gatt_ssdp_found_database_hash_callback,
                                                                           .read_value_callback     = app_gatt_ssdp_start_read_database_hash_callback,
                                                                       },
                                                                       {
                                                                           .read_value              = true,
                                                                           .characteristic_uuid     = &characteristicServerSupportedFeaturesAttUuid,
                                                                           .characteristic_callback = app_gatt_ssdp_found_server_supported_features_callback,
                                                                           .read_value_callback     = app_gatt_ssdp_start_read_server_supported_features_callback,
                                                                       }};

static const struct ble_gatt_ssdp_no_include_list s_app_gatt_list = {
    .max_service_count = 1,
    .service =
        {
            .service_uuid     = &serviceGenericAttributeAttUuid,
            .service_callback = app_gatt_ssdp_gatt_found_callback,
        },
    .characteristic_table =
        {
            .characteristic_size             = ARRAY_SIZE(s_app_gatt_char_info),
            .characteristic                  = s_app_gatt_char_info,
            .unknown_characteristic_callback = app_gatt_ssdp_att_found_unknown_char_callback,
        },
};

/**
 * @brief  Callback when GAP service is found during discovery.
 *          Print GAP service information including device name, appearance, connection parameters and address resolution.
 *
 * @param[in] conn_handle   Connection handle.
 * @param[in] count         Service count.
 * @param[in] start_handle  Service start handle.
 * @param[in] end_handle    Service end handle.
 */
static void app_gatt_ssdp_gap_found_callback(uint16_t conn_handle, uint8_t count, uint16_t start_handle, uint16_t end_handle)
{
    (void)conn_handle;
    if (count == 0x01) {
        tlk_printf("[APP] GAP Service found, start_handle = 0x%04x, end_handle = 0x%04x", start_handle, end_handle);
    } else {
        tlk_printf("[APP] GAP Service found finish. GAP Server Information:");
        tlk_printf("        Device Name:[%d] %s", s_app_ssdp_info.device_name_len, s_app_ssdp_info.device_name);
        tlk_printf("        Appearance: 0x%04x", s_app_ssdp_info.appearance);
        tlk_printf("        Peripheral Preferred Connection Parameters: Range Min: 0x%04x, Range Max: 0x%04x, Latency: 0x%04x, Timeout: 0x%04x",
                   s_app_ssdp_info.peripheral_conn_param_range_min, s_app_ssdp_info.peripheral_conn_param_range_max, s_app_ssdp_info.peripheral_conn_param_latency,
                   s_app_ssdp_info.peripheral_conn_param_timeout);
        tlk_printf("        Central Address Resolution: %d", s_app_ssdp_info.central_address_resolution);
    }
}

/**
 * @brief  Callback when unknown characteristic is found in GAP service.
 *
 * @param[in] conn_handle  Connection handle.
 * @param[in] uuid         Characteristic UUID.
 * @param[in] properties   Characteristic properties.
 * @param[in] value_handle Characteristic value handle.
 */
static void app_gap_ssdp_att_found_unknown_char_callback(uint16_t conn_handle, const struct att_uuid *uuid, uint8_t properties, uint16_t value_handle)
{
    (void)conn_handle;
    tlk_printf("[APP] GAP Found unknown characteristic, uuid = %s, properties = 0x%02x, value_handle = 0x%04x", ble_att_uuid_format(uuid), properties, value_handle);
}

/**
 * @brief  Callback when Device Name characteristic is found.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] service_count  Service count.
 * @param[in] properties     Characteristic properties.
 * @param[in] value_handle   Characteristic value handle.
 */
static void app_gatt_ssdp_found_device_name_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    (void)conn_handle;
    (void)service_count;
    tlk_printf("[APP] Device Name found, properties = 0x%02x, value_handle = 0x%04x", properties, value_handle);
}

/**
 * @brief  Callback to prepare read buffer for Device Name characteristic.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] value_handle   Characteristic value handle.
 * @param[out] read          Pointer to read buffer.
 * @param[out] read_len      Pointer to read length.
 * @param[out] read_max_size Maximum read size.
 */
static void app_gatt_ssdp_start_read_device_name_callback(uint16_t conn_handle, uint16_t value_handle, uint8_t **read, uint16_t **read_len, uint16_t *read_max_size)
{
    (void)conn_handle;
    (void)value_handle;
    (void)read_len;
    *read          = (uint8_t *)&s_app_ssdp_info.device_name;
    *read_len      = &s_app_ssdp_info.device_name_len;
    *read_max_size = sizeof(s_app_ssdp_info.device_name);
}

/**
 * @brief  Callback when Appearance characteristic is found.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] service_count  Service count.
 * @param[in] properties     Characteristic properties.
 * @param[in] value_handle   Characteristic value handle.
 */
static void app_gatt_ssdp_found_appearance_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    (void)conn_handle;
    (void)service_count;
    tlk_printf("[APP] Appearance found, properties = 0x%02x, value_handle = 0x%04x", properties, value_handle);
}

/**
 * @brief  Callback to prepare read buffer for Appearance characteristic.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] value_handle   Characteristic value handle.
 * @param[out] read          Pointer to read buffer.
 * @param[out] read_len       Pointer to read length (unused).
 * @param[out] read_max_size  Maximum read size.
 */
static void app_gatt_ssdp_start_read_appearance_callback(uint16_t conn_handle, uint16_t value_handle, uint8_t **read, uint16_t **read_len, uint16_t *read_max_size)
{
    (void)conn_handle;
    (void)value_handle;
    (void)read_len;
    *read          = (uint8_t *)&s_app_ssdp_info.appearance;
    *read_max_size = sizeof(s_app_ssdp_info.appearance);
}

/**
 * @brief  Callback when Peripheral Preferred Connection Parameters characteristic is found.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] service_count  Service count.
 * @param[in] properties     Characteristic properties.
 * @param[in] value_handle   Characteristic value handle.
 */
static void app_gatt_ssdp_found_peripheral_preferred_conn_param_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    (void)conn_handle;
    (void)service_count;
    tlk_printf("[APP] Peripheral Preferred Connection Parameters found, properties = 0x%02x, value_handle = 0x%04x", properties, value_handle);
}

/**
 * @brief  Callback to prepare read buffer for Peripheral Preferred Connection Parameters characteristic.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] value_handle   Characteristic value handle.
 * @param[out] read          Pointer to read buffer.
 * @param[out] read_len       Pointer to read length (unused).
 * @param[out] read_max_size  Maximum read size.
 */
static void app_gatt_ssdp_start_read_peripheral_preferred_conn_param_callback(uint16_t conn_handle, uint16_t value_handle, uint8_t **read, uint16_t **read_len,
                                                                              uint16_t *read_max_size)
{
    (void)conn_handle;
    (void)value_handle;
    (void)read_len;
    *read          = (uint8_t *)&s_app_ssdp_info.peripheral_conn_param_range_min;
    *read_max_size = 8;
}

/**
 * @brief  Callback when Central Address Resolution characteristic is found.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] service_count  Service count.
 * @param[in] properties     Characteristic properties.
 * @param[in] value_handle   Characteristic value handle.
 */
static void app_gatt_ssdp_found_central_address_resolution_callback(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle)
{
    (void)conn_handle;
    (void)service_count;
    tlk_printf("[APP] Central Address Resolution found, properties = 0x%02x, value_handle = 0x%04x", properties, value_handle);
}

/**
 * @brief  Callback to prepare read buffer for Central Address Resolution characteristic.
 *
 * @param[in] conn_handle    Connection handle.
 * @param[in] value_handle   Characteristic value handle.
 * @param[out] read          Pointer to read buffer.
 * @param[out] read_len       Pointer to read length (unused).
 * @param[out] read_max_size  Maximum read size.
 */
static void app_gatt_ssdp_start_read_central_address_resolution_callback(uint16_t conn_handle, uint16_t value_handle, uint8_t **read, uint16_t **read_len, uint16_t *read_max_size)
{
    (void)conn_handle;
    (void)value_handle;
    (void)read_len;
    *read          = &s_app_ssdp_info.central_address_resolution;
    *read_max_size = sizeof(s_app_ssdp_info.central_address_resolution);
}

static const struct gatt_ssdp_characteristic s_app_gap_char_info[] = {{
                                                                          .read_value              = true,
                                                                          .characteristic_uuid     = &characteristicDeviceNameAttUuid,
                                                                          .characteristic_callback = app_gatt_ssdp_found_device_name_callback,
                                                                          .read_value_callback     = app_gatt_ssdp_start_read_device_name_callback,
                                                                      },
                                                                      {
                                                                          .read_value              = true,
                                                                          .characteristic_uuid     = &characteristicAppearanceAttUuid,
                                                                          .characteristic_callback = app_gatt_ssdp_found_appearance_callback,
                                                                          .read_value_callback     = app_gatt_ssdp_start_read_appearance_callback,
                                                                      },
                                                                      {
                                                                          .read_value              = true,
                                                                          .characteristic_uuid     = &characteristicPeripheralPreferredConnParamAttUuid,
                                                                          .characteristic_callback = app_gatt_ssdp_found_peripheral_preferred_conn_param_callback,
                                                                          .read_value_callback     = app_gatt_ssdp_start_read_peripheral_preferred_conn_param_callback,
                                                                      },
                                                                      {
                                                                          .read_value              = true,
                                                                          .characteristic_uuid     = &characteristicCentralAddressResolutionAttUuid,
                                                                          .characteristic_callback = app_gatt_ssdp_found_central_address_resolution_callback,
                                                                          .read_value_callback     = app_gatt_ssdp_start_read_central_address_resolution_callback,
                                                                      }};

static const struct ble_gatt_ssdp_no_include_list s_app_gap_list = {
    .max_service_count = 1,
    .service =
        {
            .service_uuid     = &serviceGenericAccessAttUuid,
            .service_callback = app_gatt_ssdp_gap_found_callback,
        },
    .characteristic_table =
        {
            .characteristic_size             = ARRAY_SIZE(s_app_gap_char_info),
            .characteristic                  = s_app_gap_char_info,
            .unknown_characteristic_callback = app_gap_ssdp_att_found_unknown_char_callback,
        },
};

/**
 * @brief  Callback when ACL connection is established.
 *          Start GATT and GAP service discovery.
 *
 * @param[in] conn  Connection information.
 */
static void app_connected_callback(struct ble_host_conn *conn);
/**
 * @brief  Callback when ACL connection is disconnected.
 *
 * @param[in] conn    Connection information.
 * @param[in] reason  Disconnection reason.
 */
static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);

static const struct ble_host_acl_conn_callbacks s_app_acl_callbacks = {
    .connected    = app_connected_callback,
    .disconnected = app_disconnected_callback,
};

static void app_connected_callback(struct ble_host_conn *conn)
{
    (void)conn;
    s_conn_handle = conn->conn_handle;

    int ret = ble_host_gatt_ssdp_start_no_include(s_conn_handle, &s_app_gatt_list);
    tlk_printf("[APP] GATT Service discovery start, ret = %d", ret);

    ret = ble_host_gatt_ssdp_start_no_include(s_conn_handle, &s_app_gap_list);
    tlk_printf("[APP] GAP Service discovery start again, ret = %d", ret);
}

static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    (void)conn;
    (void)reason;
    s_conn_handle = BLE_CONN_HANDLE_INVALID;
}

int  INIT(APP_BLE_ACL)(void);
void START(APP_BLE_ACL)(void);

/**
 * @brief  Initialize SSDP application module.
 *          Initialize ACL module and register ACL connection callbacks.
 *
 * @return  0 on success.
 */
int INIT(APP_BLE_SSDP)(void)
{
    INIT(APP_BLE_ACL)();

    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA2_USER_ID, &s_app_acl_callbacks);
    tlk_printf("[APP] SSDP Client initialized");

    return 0;
}

/**
 * @brief  Start SSDP application module.
 *          Start ACL module.
 */
void START(APP_BLE_SSDP)(void)
{
    START(APP_BLE_ACL)();
}
