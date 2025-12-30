/********************************************************************************************************
 * @file    app_spp_client.c
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

static uint8_t  s_read_buffer[100];
static uint16_t s_read_buffer_len = 0;

static uint16_t s_service_end_handle         = 0;
static uint16_t s_last_characteristic_handle = 0;

static struct ble_gattc_ccc_message s_spp_ccc_msg;

static uint16_t s_client_to_server_handle = 0;
static uint16_t s_server_to_client_handle = 0;

/**
 * @brief  Callback when CCC notification is received from server.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] ntf_value   Notification value.
 */
static void app_spp_client_ccc_report_callback(uint16_t conn_handle, struct ble_gattc_ccc_value *ntf_value)
{
    tlk_printf("[APP] SPP CCC report callback, ACL: 0x%03x, CID: 0x%04x, Handle: 0x%04x, Value: %s", conn_handle, ntf_value->cid, ntf_value->attr_handle, ntf_value->value);
}

/**
 * @brief  Callback when characteristic value read is completed.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] cid         Channel ID.
 * @param[in] err         Error code.
 * @param[in] param       Read characteristic value parameters.
 *
 * @return  true to continue, false to stop.
 */
static bool app_spp_client_read_characteristic_value_callback(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_read_characteristic_value *param)
{
    (void)cid;

    // tlk_printf("[APP] SPP Read characteristic value complete, ACL: 0x%03x, err: 0x%08x", conn_handle, err);
    // tlk_printf("    Handle:0x%04x, Offset:0x%04x, Length:0x%04x", param->handle, param->offset, param->length);

    if (err == GATT_REQUEST_PROCEDURE_COMPLETE) {
        s_read_buffer[s_read_buffer_len] = '\0';
        tlk_printf("[APP] Read Finished, ACL: 0x%03x, Handle:0x%x, value is %s", conn_handle, param->handle, s_read_buffer);
    }

    return true;
}

/**
 * @brief  Callback when characteristic descriptor write is completed.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] cid         Channel ID.
 * @param[in] err         Error code.
 * @param[in] user_data   User data.
 */
static void app_spp_client_write_characteristic_desc_callback(uint16_t conn_handle, uint16_t cid, uint32_t err, void *user_data)
{
    (void)cid;
    (void)user_data;
    tlk_printf("[APP] SPP Write characteristic descriptor complete, ACL: 0x%03x, err: 0x%08x", conn_handle, err);
}

/**
 * @brief  Callback when characteristic descriptor discovery is completed.
 *          Enable notification if CCC descriptor is found.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] cid         Channel ID.
 * @param[in] err         Error code.
 * @param[in] descriptor   Discovered descriptor information.
 *
 * @return  true to continue, false to stop.
 */
static bool app_spp_client_discover_characteristic_desc_callback(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_disc_characteristic_desc *descriptor)
{
    (void)cid;
    tlk_printf("[APP] SPP Discover characteristic descriptor complete, ACL: 0x%03x, err: 0x%08x", conn_handle, err);

    if (err == GATT_REQUEST_SUB_PROCEDURE_COMPLETE) {
        tlk_printf("    Descriptor Handle:0x%04x, UUID:%s", descriptor->handle, ble_att_uuid_format(&descriptor->descriptor_uuid));
        if (ble_att_uuid_cmp(&descriptor->descriptor_uuid, &descriptorClientCharacteristicConfigurationAttUuid) == 0) {
            ble_host_gattc_write_ccc_value_enable_notify(conn_handle, LE_L2CAP_CID_ATT, descriptor->handle, app_spp_client_write_characteristic_desc_callback, NULL);
        }
    }
    return true;
}

static const struct gattc_characteristic_value_write_info s_write_info = {
    .write_buffer     = s_read_buffer,
    .write_buffer_len = &s_read_buffer_len,
    .max_buffer_len   = sizeof(s_read_buffer),
    .user_data        = NULL,
    .callback         = app_spp_client_read_characteristic_value_callback,
};

/**
 * @brief  Callback when characteristic discovery is completed.
 *          Read characteristic value if readable, discover descriptors, and store handles.
 *
 * @param[in] conn_handle   Connection handle.
 * @param[in] cid           Channel ID.
 * @param[in] err           Error code.
 * @param[in] characteristic Discovered characteristic information.
 *
 * @return  true to continue, false to stop.
 */
static bool app_spp_client_discover_characteristic_callback(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_disc_characteristic *characteristic)
{
    (void)cid;

    tlk_printf("[APP] SPP Discovery characteristic complete, ACL: 0x%03x, err: 0x%08x", conn_handle, err);
    if (err == GATT_REQUEST_SUB_PROCEDURE_COMPLETE) {
        tlk_printf("[APP] Handle:0x%04x, Value Handle:0x%04x, UUID:%s", characteristic->handle, characteristic->value_handle,
                   ble_att_uuid_format(&characteristic->characteristic_uuid));
        tlk_printf("    Read:%d, WriteWithoutResp:%d, Write:%d, Notify:%d, Indicate:%d", characteristic->properties.read, characteristic->properties.writeWithoutResponse,
                   characteristic->properties.write, characteristic->properties.notify, characteristic->properties.indicate);

        if (characteristic->properties.read) {
            ble_host_gattc_read_characteristic_value_write(conn_handle, LE_L2CAP_CID_ATT, characteristic->value_handle, &s_write_info);
        }

        if (ble_att_uuid_cmp(&characteristic->characteristic_uuid, &tlk_spp_server2client_char_att_uuid) == 0) {
            s_server_to_client_handle = characteristic->value_handle;
        }

        if (ble_att_uuid_cmp(&characteristic->characteristic_uuid, &tlk_spp_client2server_char_att_uuid) == 0) {
            s_client_to_server_handle = characteristic->value_handle;
        }

        if (s_last_characteristic_handle != 0 && s_last_characteristic_handle < characteristic->handle) {
            struct gattc_disc_characteristic_desc_param desc_param = {
                .start_handle = s_last_characteristic_handle,
                .end_handle   = characteristic->handle - 1,
                .user_data    = NULL,
                .callback     = app_spp_client_discover_characteristic_desc_callback,
            };

            ble_host_gattc_discover_characteristic_desc(conn_handle, LE_L2CAP_CID_ATT, &desc_param);
        }

        s_last_characteristic_handle = characteristic->value_handle + 1;
    } else {
        if (s_last_characteristic_handle != 0 && s_last_characteristic_handle < s_service_end_handle) {
            struct gattc_disc_characteristic_desc_param desc_param = {
                .start_handle = s_last_characteristic_handle,
                .end_handle   = s_service_end_handle,
                .user_data    = NULL,
                .callback     = app_spp_client_discover_characteristic_desc_callback,
            };

            ble_host_gattc_discover_characteristic_desc(conn_handle, LE_L2CAP_CID_ATT, &desc_param);
        }
    }
    return true;
}

/**
 * @brief  Callback when service discovery is completed.
 *          Start characteristic discovery and register CCC message.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] cid         Channel ID.
 * @param[in] err         Error code.
 * @param[in] service     Discovered service information.
 *
 * @return  true to continue, false to stop.
 */
static bool app_spp_client_discover_service_callback(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_disc_services *service)
{
    (void)cid;
    tlk_printf("[APP] SPP discovery service complete, ACL: 0x%03x, err: 0x%08x", conn_handle, err);
    if (err == GATT_REQUEST_SUB_PROCEDURE_COMPLETE) {
        tlk_printf("[APP] start Handle:0x%04x, end Handle:0x%04x", service->start_handle, service->end_handle);
        s_service_end_handle                        = service->end_handle;
        s_last_characteristic_handle                = 0;
        struct gattc_disc_all_characteristics param = {
            .start_handle = service->start_handle,
            .end_handle   = service->end_handle,
            .user_data    = NULL,
            .callback     = app_spp_client_discover_characteristic_callback,
        };
        int ret = ble_host_gattc_discover_all_characteristics_of_service(conn_handle, LE_L2CAP_CID_ATT, &param);

        tlk_printf("[APP] SPP start discovery all characteristics of service, ACL: 0x%03x, ret:0x%x", conn_handle, ret);

        s_spp_ccc_msg.start_handle    = service->start_handle;
        s_spp_ccc_msg.end_handle      = service->end_handle;
        s_spp_ccc_msg.report_callback = app_spp_client_ccc_report_callback;
        ble_host_gattc_add_subscribe_ccc_message(conn_handle, &s_spp_ccc_msg);
    }
    return true;
}

/**
 * @brief  Callback when ACL connection is established.
 *          Start SPP service discovery.
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
    int ret       = ble_host_gattc_discover_primary_service_by_uuid(s_conn_handle, LE_L2CAP_CID_ATT, &tlk_spp_service_att_uuid, app_spp_client_discover_service_callback, NULL);

    tlk_printf("[APP] SPP start discovery primary service by uuid, ACL: 0x%03x, ret:0x%x", s_conn_handle, ret);
}

static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    (void)conn;
    (void)reason;
    s_conn_handle             = BLE_CONN_HANDLE_INVALID;
    s_client_to_server_handle = 0;
    s_server_to_client_handle = 0;
}

static const uint8_t s_write_test_buffer[] = "Telink SPP Client Test";

/**
 * @brief  Write characteristic value without response.
 *          Send test data to server using write without response.
 */
static void app_spp_write_without_response_client_value(void)
{
    ble_host_gattc_write_characteristic_value_without_response(s_conn_handle, LE_L2CAP_CID_ATT, s_client_to_server_handle, s_write_test_buffer, sizeof(s_write_test_buffer) - 1);
}

/**
 * @brief  Callback when characteristic value write is completed.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] cid         Channel ID.
 * @param[in] err         Error code.
 * @param[in] user_data   User data.
 */
static void app_spp_write_client_value_callback(uint16_t conn_handle, uint16_t cid, uint32_t err, void *user_data)
{
    tlk_printf("[APP] SPP Write characteristic value complete, ACL: 0x%03x, err: 0x%08x", conn_handle, err);
    (void)cid;
    (void)user_data;
}

/**
 * @brief  Write characteristic value with response.
 *          Send test data to server using write with response.
 */
static void app_spp_write_client_value(void)
{
    struct gattc_write_characteristic_value_param param = {
        .buffer    = s_write_test_buffer,
        .length    = sizeof(s_write_test_buffer) - 1,
        .handle    = s_client_to_server_handle,
        .callback  = app_spp_write_client_value_callback,
        .user_data = NULL,
    };

    ble_host_gattc_write_characteristic_value(s_conn_handle, LE_L2CAP_CID_ATT, &param);
}

int  INIT(APP_BLE_ACL)(void);
void START(APP_BLE_ACL)(void);

/**
 * @brief  Initialize SPP client application module.
 *          Initialize ACL module, register ACL callbacks and key callbacks for write operations.
 *
 * @return  0 on success.
 */
int INIT(APP_BLE_SPP_CLIENT)(void)
{
    INIT(APP_BLE_ACL)();

    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA2_USER_ID, &s_app_acl_callbacks);
    tlk_printf("[APP] SPP Client initialized");

    app_key_register_callback(2, app_spp_write_without_response_client_value);
    app_key_register_callback(3, app_spp_write_client_value);
    return 0;
}

/**
 * @brief  Start SPP client application module.
 *          Start ACL module.
 */
void START(APP_BLE_SPP_CLIENT)(void)
{
    START(APP_BLE_ACL)();
}
