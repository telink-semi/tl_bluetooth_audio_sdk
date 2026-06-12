/********************************************************************************************************
 * @file    app_ble_hid.c
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
#include "stack/ble/service/hids.h"

#include "tlkmw/sys_dev/key/tlkdrv_key.h"

#define MKEY_VOL_UP 0x00E9
#define MKEY_VOL_DN 0x00EA

static uint16_t acl_hid_conn_handle = 0x0000;

#define HID_ATTR_HANDLE (0x880 + 21)

static void app_ble_hid_report_consume_control(uint16_t consumer_key)
{
    if (acl_hid_conn_handle) {
        ble_gatts_notify(acl_hid_conn_handle, HID_ATTR_HANDLE, (u8 *)&consumer_key, 2);
        consumer_key = 0;
        ble_gatts_notify(acl_hid_conn_handle, HID_ATTR_HANDLE, (u8 *)&consumer_key, 2);
    }
}

static void app_ble_hid_report_volume_increment(void)
{
    app_ble_hid_report_consume_control(MKEY_VOL_UP);
}

static void app_ble_hid_report_volume_decrement(void)
{
    app_ble_hid_report_consume_control(MKEY_VOL_DN);
}

static void app_ble_hid_acl_connected(struct ble_host_conn *conn)
{
    acl_hid_conn_handle = conn->conn_handle;
}

static void app_ble_hid_acl_disconnected(struct ble_host_conn *conn, uint8_t reason)
{
    (void)reason;
    if (conn->conn_handle == acl_hid_conn_handle) {
        acl_hid_conn_handle = 0;
    }
}

static const struct ble_host_acl_conn_callbacks s_app_hid_acl_conn_callbacks = {
    .connected    = app_ble_hid_acl_connected,
    .disconnected = app_ble_hid_acl_disconnected,
};

void app_ble_hid_init(void)
{
    blc_svc_addDisGroup();
    blc_svc_addScpsGroup();
    blc_svc_addHidGroup();

    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA2_USER_ID, &s_app_hid_acl_conn_callbacks);

    tlkdrv_key_registerVendorConfig1Callback(app_ble_hid_report_volume_increment);
    tlkdrv_key_registerVendorConfig2Callback(app_ble_hid_report_volume_decrement);
}
