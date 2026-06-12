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
#include "tlkmw/host/tlkmw_host_linkmgr.h"

#if (TLK_STK_BLE_HID_ENABLE)

#define MKEY_VOL_UP 0x00E9
#define MKEY_VOL_DN 0x00EA

static uint16_t acl_hid_conn_handle = 0x0000;

#if TLK_DEV_KEY_ENABLE

/**
 * @brief       Report consumer control data via HID notification
 * @param[in]   consumer_key - Consumer key value to report
 * @return      none
 * @note        Sends a consumer control input report and then sends a zero-value report to release the key
 */
static void app_ble_hid_report_consume_control(uint16_t consumer_key)
{
    if (acl_hid_conn_handle) {
        ble_hids_notify_input_report(acl_hid_conn_handle, BLE_HOST_HID_REPORT_ID_CONSUME_CONTROL_INPUT, (u8 *)&consumer_key, 2);
        consumer_key = 0;
        ble_hids_notify_input_report(acl_hid_conn_handle, BLE_HOST_HID_REPORT_ID_CONSUME_CONTROL_INPUT, (u8 *)&consumer_key, 2);
    }
}

/**
 * @brief       Report volume increment key press
 * @param       none
 * @return      none
 * @note        Reports the volume up multimedia key via HID
 */
static void app_ble_hid_report_volume_increment(void)
{
    app_ble_hid_report_consume_control(MKEY_VOL_UP);
}

/**
 * @brief       Report volume decrement key press
 * @param       none
 * @return      none
 * @note        Reports the volume down multimedia key via HID
 */
static void app_ble_hid_report_volume_decrement(void)
{
    app_ble_hid_report_consume_control(MKEY_VOL_DN);
}

#endif

/**
 * @brief       Handle ACL connection establishment for HID
 * @param[in]   conn - Pointer to connection information structure
 * @return      none
 * @note        Stores the connection handle and adds the link to the link manager
 */
static void app_ble_hid_acl_connected(struct ble_host_conn *conn)
{
    acl_hid_conn_handle = conn->conn_handle;
    tlkmw_host_linkmgr_addLink(TLKMDI_HOST_LINK_TYPE_BLE, conn->conn_handle);
}

/**
 * @brief       Handle ACL disconnection for HID
 * @param[in]   conn - Pointer to connection information structure
 * @param[in]   reason - Disconnection reason code
 * @return      none
 * @note        Removes the link from the link manager and clears the connection handle if it matches
 */
static void app_ble_hid_acl_disconnected(struct ble_host_conn *conn, uint8_t reason)
{
    (void)reason;
    tlkmw_host_linkmgr_delLink(TLKMDI_HOST_LINK_TYPE_BLE, conn->conn_handle);
    if (conn->conn_handle == acl_hid_conn_handle) {
        acl_hid_conn_handle = 0;
    }
}

static const struct ble_host_acl_conn_callbacks s_app_hid_acl_conn_callbacks = {
    .connected    = app_ble_hid_acl_connected,
    .disconnected = app_ble_hid_acl_disconnected,
};

/**
 * @brief       Initialize the BLE HID service
 * @param       none
 * @return      none
 * @note        This function registers various BLE services including Battery Service (BAS),
 *              Device Information Service (DIS), Scan Parameters Service (ScPS) and HID service.
 *              It also sets up connection callbacks and key reporting functionality.
 */
void app_ble_hid_init(void)
{
    ble_basic_register_BAS_control_server(BLE_BASS_INITIAL_PARAMS(100, DEVICE_NO_CHARGING));
    ble_basic_register_DIS_control_server();
    ble_basic_register_ScPS_control_server();

    extern const unsigned char  tlk_hid_keyboard_report_map[];
    extern const unsigned short tlk_hid_keyboard_report_map_len;
    ble_hid_register_HID_control_server(BLE_HIDS_INITIAL_PARAMS_DEFAULT(tlk_hid_keyboard_report_map, tlk_hid_keyboard_report_map_len));

    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA2_USER_ID, &s_app_hid_acl_conn_callbacks);

#if TLK_DEV_KEY_ENABLE
    tlkdrv_key_registerVendorConfig1Callback(app_ble_hid_report_volume_increment);
    tlkdrv_key_registerVendorConfig2Callback(app_ble_hid_report_volume_decrement);
#endif
}

#endif