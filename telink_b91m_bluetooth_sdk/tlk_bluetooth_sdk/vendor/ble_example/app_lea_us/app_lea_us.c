/********************************************************************************************************
 * @file    app_lea_us.c
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
#include "stack/ble/ble.h"
#include "driver.h"
#include "tlkmw/ble/le_audio/inc/lea_us_headset.h"
#include "tlkmw/ble/le_audio/inc/lea_us_tws.h"
#include "../app_example.h"
#define LEA_US_HEADSET     0
#define LEA_US_TWS         1
#define LEA_US_DEVICE_TYPE LEA_US_HEADSET

static void app_connected_callback(struct ble_host_conn *conn);
static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);

static const struct ble_host_acl_conn_callbacks s_app_acl_callbacks = {
    .connected    = app_connected_callback,
    .disconnected = app_disconnected_callback,
};

/**
 * @brief  Callback when ACL connection is established.
 *
 * @param[in] conn  Connection information.
 */
static void app_connected_callback(struct ble_host_conn *conn)
{
    tlk_printf("[APP] connected callback, ACL handle: 0x%03x", conn->conn_handle);
}

/**
 * @brief  Callback when ACL connection is disconnected.
 *
 * @param[in] conn    Connection information.
 * @param[in] reason  Disconnection reason.
 */
static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    tlk_printf("[APP] disconnected callback, ACL handle: 0x%03x, reason: 0x%02x", conn->conn_handle, reason);
}

#if (LEA_US_DEVICE_TYPE == LEA_US_TWS)
/**
 * @brief  Get TWS device type (left or right earbud).
 *          Read device type from flash, default to left earbud if not set.
 *
 * @return  Device type: TWS_LEFT_EAR_BUD or TWS_RIGHT_EAR_BUD.
 */
static uint8_t app_lea_us_get_device_type(void)
{
    static uint8_t s_device_type = 0;
    if (s_device_type) {
        tlk_printf("[APP] Device type is already set: %d", s_device_type);
        return s_device_type;
    }
    uint32_t read_type_addr = TLK_CFG_FLASH_USER_USBID_ADDR + flash_full_size - 0x100000;
    flash_read_page(read_type_addr, 1, (uint8_t *)&s_device_type);
    if (s_device_type != TWS_LEFT_EAR_BUD && s_device_type != TWS_RIGHT_EAR_BUD) {
        s_device_type = TWS_LEFT_EAR_BUD;
        tlk_printf("[APP] Device type is not set, default to TWS_LEFT_EAR_BUD");
    }
    return s_device_type;
}

/**
 * @brief  Get device SIRK (Set Identity Resolving Key) from flash.
 *          Read SIRK from flash, use default value if not set.
 *
 * @param[out] sirk  Buffer to store SIRK, 16 bytes.
 */
static void app_ble_get_device_sirk(uint8_t *sirk)
{
    uint8_t  s_sirk[16]     = {0};
    uint32_t read_sirk_addr = TLK_CFG_FLASH_LE_TWS_SIRK_ADDR + flash_full_size - 0x100000;
    flash_read_page(read_sirk_addr, 16, s_sirk);
    int i = 0;
    for (; i < 16; i++) {
        if (s_sirk[i] != 0xFF) {
            break;
        }
    }
    if (i != 16) {
        memcpy(sirk, s_sirk, 16);
    } else {
        tlk_printf("[APP] SIRK is not set, please set SIRK in flash address 0x%x", read_sirk_addr);
        memset(sirk, 0x11, 16);
    }
}
#endif

/**
 * @brief  Initialize LE Audio US application module.
 *          Initialize TWS or headset based on device type configuration.
 *
 * @return  0 on success.
 */
int INIT(APP_BLE_LEA_US)(void)
{
#if (LEA_US_DEVICE_TYPE == LEA_US_TWS)
    static struct lea_us_tws_param s_le_tws_param = {
        .device_name = "app_lea_us_tws",
        .interval    = 50,
        .volume      = 150,
    };
    s_le_tws_param.ear_type = app_lea_us_get_device_type();
    app_ble_get_device_sirk(s_le_tws_param.sirk);
    lea_unicast_server_tws_initial(&s_le_tws_param);
#elif (LEA_US_DEVICE_TYPE == LEA_US_HEADSET)
    static struct lea_us_headset_param s_le_headset_param = {
        .device_name = "app_lea_us_headset",
        .interval    = 50,
        .volume      = 150,
    };
    lea_unicast_server_headset_initial(&s_le_headset_param);
#endif
    blc_svc_calculateDatabaseHash();
    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA2_USER_ID, &s_app_acl_callbacks);
    return 0;
}

/**
 * @brief  Start LE Audio US application module.
 */
void START(APP_BLE_LEA_US)(void) {}
