/********************************************************************************************************
 * @file    app_spp_server.c
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

#include "stack/ble/host_v1/services/inc/svc.h"

/**
 * @brief  SPP service read callback.
 *          Handle read requests for SPP characteristics.
 *
 * @param[in] conn_handle   Connection handle.
 * @param[in] opcode         ATT opcode.
 * @param[in] attr_handle    Attribute handle.
 * @param[out] out_value     Pointer to output value buffer.
 * @param[out] out_value_len Pointer to output value length.
 *
 * @return  ATT_SUCCESS on success, error code otherwise.
 */
static int app_spp_read_cb(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t **out_value, uint16_t *out_value_len);
/**
 * @brief  SPP service write callback.
 *          Handle write requests for SPP characteristics and send notifications.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] opcode      ATT opcode.
 * @param[in] attr_handle Attribute handle.
 * @param[in] value       Write value buffer.
 * @param[in] value_len   Write value length.
 *
 * @return  ATT_SUCCESS on success, error code otherwise.
 */
static int app_spp_write_cb(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len);

static const struct atts_attribute sppList[] = {
    ATTS_PRIMARY_SERVICE_128(characteristicTlkSppServiceUuid),

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropReadNotify),
    {ATT_PERMISSIONS_ENCRYPT_READ, ATT_128_UUID_LEN, (uint8_t *)&characteristicTlkSppServer2ClientUuid[0], NULL, 0, NULL, ATTS_SET_READ_CALLBACK},
    ATTS_COMMON_CCC_DEFINE,

    ATTS_CHARACTERISTIC_DECLARATIONS(charPropReadWriteWriteWithoutNotify),
    {ATT_PERMISSIONS_ENCRYPT_RDWR, ATT_128_UUID_LEN, (uint8_t *)&characteristicTlkSppClient2ServerUuid[0], NULL, 0, NULL, ATTS_SET_WRITE_CALLBACK | ATTS_SET_READ_CALLBACK},
    ATTS_COMMON_CCC_DEFINE,
};

/* GAP group structure */
_attribute_ble_data_retention_ static struct atts_group svcSppGroup = {
    NULL, sppList, app_spp_read_cb, app_spp_write_cb, SERVICE_USER_128UUID_START_HDL, 0,
};

static const char s_notify_server_to_client_str[] = "Notify Server to Client Telink SPP Service";

static const char s_notify_client_to_server_str[]  = "Notify Client to Server Telink SPP Service";
static const char s_notify_client_to_server_str2[] = "Notify2 Client to Server Telink SPP Service";

static int app_spp_read_cb(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t **out_value, uint16_t *out_value_len)
{
    tlk_printf("[APP] read callback, AC:: 0x%03x, handle: 0x%03x opcode: 0x%02x", conn_handle, attr_handle, opcode);

    if (attr_handle == SERVICE_USER_128UUID_START_HDL + 2) {
        *out_value     = (uint8_t *)"Telink SPP Service Server to Client Characteristic";
        *out_value_len = sizeof("Telink SPP Service Server to Client Characteristic") - 1;
    } else if (attr_handle == SERVICE_USER_128UUID_START_HDL + 5) {
        *out_value     = (uint8_t *)"Telink SPP Service Client to Server Characteristic";
        *out_value_len = sizeof("Telink SPP Service Client to Server Characteristic") - 1;
        ble_gatts_notify(conn_handle, SERVICE_USER_128UUID_START_HDL + 2, (const uint8_t *)s_notify_server_to_client_str, sizeof(s_notify_server_to_client_str) - 1);
    }

    return ATT_SUCCESS;
}

/**
 * @brief  SPP notification callback.
 *          Send another notification after first notification completes.
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] success     Notification success status.
 */
static void app_spp_notify_cb(uint16_t conn_handle, bool success)
{
    tlk_printf("[APP] notify callback, AC:: 0x%03x, success: %d", conn_handle, success);
    ble_gatts_notify(conn_handle, SERVICE_USER_128UUID_START_HDL + 5, (const uint8_t *)s_notify_client_to_server_str2, sizeof(s_notify_client_to_server_str2) - 1);
}

static int app_spp_write_cb(uint16_t conn_handle, uint8_t opcode, uint16_t attr_handle, uint8_t *value, uint16_t value_len)
{
    tlk_printf("[APP] write callback, AC:: 0x%03x, handle: 0x%03x opcode: 0x%02x", conn_handle, attr_handle, opcode);
    tlk_printf("value %s", hex_to_str(value, value_len));
    if (attr_handle == SERVICE_USER_128UUID_START_HDL + 5) {
        ble_gatts_notify_with_callback(conn_handle, attr_handle, (const uint8_t *)s_notify_client_to_server_str, sizeof(s_notify_client_to_server_str) - 1, app_spp_notify_cb);
    }
    return ATT_SUCCESS;
}

int  INIT(APP_BLE_ACL)(void);
void START(APP_BLE_ACL)(void);

/**
 * @brief  Initialize SPP server application module.
 *          Add SPP service group and initialize ACL module.
 *
 * @return  0 on success.
 */
int INIT(APP_BLE_SPP_SERVER)(void)
{
    svcSppGroup.endHandle = svcSppGroup.startHandle + ARRAY_SIZE(sppList) - 1;
    ble_host_add_attribute_service_group(&svcSppGroup);
    INIT(APP_BLE_ACL)();
    tlk_printf("[APP] SPP Server initialized");
    return 0;
}

/**
 * @brief  Start SPP server application module.
 *          Start ACL module.
 */
void START(APP_BLE_SPP_SERVER)(void)
{
    START(APP_BLE_ACL)();
}
