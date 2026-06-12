/********************************************************************************************************
 * @file    app_l2cap_coc.c
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
#include "../app_key.h"

int  INIT(APP_BLE_ACL)(void);
void START(APP_BLE_ACL)(void);

#define BLE_L2CAP_COC_PARAMETER_SPSM           0x0080
#define BLE_L2CAP_COC_PARAMETER_INITIAL_CREDIT 20
#define BLE_L2CAP_COC_PARAMETER_MTU            1024
#define BLE_L2CAP_COC_PARAMETER_MPS            512
#define BLE_L2CAP_COC_MAX_COUNT                64
#define BLE_L2CAP_COC_ACL_MAX_NUMBER           8

static struct app_ble_l2cap_coc_info
{
    bool     is_connected;
    uint16_t conn_handle;
    uint16_t src_cid;
    uint16_t mtu;
    uint16_t spsm;
} s_app_ble_l2cap_coc_info[BLE_L2CAP_COC_MAX_COUNT];

static struct app_ble_l2cap_coc_conn_info
{
    uint16_t conn_handle;
} s_app_ble_l2cap_coc_conn_info[BLE_L2CAP_COC_ACL_MAX_NUMBER];

static void app_ble_l2cap_coc_connected_callback(uint16_t conn_handle, const struct ble_host_coc_connected_params *params);
static void app_ble_l2cap_coc_disconnected_callback(uint16_t conn_handle, uint16_t src_cid);
static void app_ble_l2cap_coc_receive_data_callback(uint16_t conn_handle, uint16_t cid, const uint8_t *data, uint16_t len);
static void app_ble_l2cap_coc_mtu_update_callback(uint16_t conn_handle, uint16_t src_cid, uint16_t mtu);

static const struct ble_host_coc_set_parameters s_demo_coc_params = {
    .initial_credit        = BLE_L2CAP_COC_PARAMETER_INITIAL_CREDIT,
    .mtu                   = BLE_L2CAP_COC_PARAMETER_MTU,
    .mps                   = BLE_L2CAP_COC_PARAMETER_MPS,
    .connected_callback    = app_ble_l2cap_coc_connected_callback,
    .disconnected_callback = app_ble_l2cap_coc_disconnected_callback,
    .receive_data_callback = app_ble_l2cap_coc_receive_data_callback,
    .mtu_update_callback   = app_ble_l2cap_coc_mtu_update_callback,
};

static void app_ble_l2cap_coc_connected_callback(uint16_t conn_handle, const struct ble_host_coc_connected_params *params)
{
    for (int i = 0; i < BLE_L2CAP_COC_MAX_COUNT; i++) {
        if (s_app_ble_l2cap_coc_info[i].is_connected == false) {
            s_app_ble_l2cap_coc_info[i].is_connected = true;
            s_app_ble_l2cap_coc_info[i].conn_handle  = conn_handle;
            s_app_ble_l2cap_coc_info[i].src_cid      = params->src_cid;
            s_app_ble_l2cap_coc_info[i].mtu          = params->mtu;
            s_app_ble_l2cap_coc_info[i].spsm         = params->spsm;
            tlk_printf("BLE l2cap CoC connected, conn_handle: %d, src_cid: %d, mtu: %d, spsm: %d", conn_handle, params->src_cid, params->mtu, params->spsm);
            break;
        }
    }
}

static void app_ble_l2cap_coc_disconnected_callback(uint16_t conn_handle, uint16_t src_cid)
{
    for (int i = 0; i < BLE_L2CAP_COC_MAX_COUNT; i++) {
        if (s_app_ble_l2cap_coc_info[i].is_connected == true && s_app_ble_l2cap_coc_info[i].conn_handle == conn_handle && s_app_ble_l2cap_coc_info[i].src_cid == src_cid) {
            s_app_ble_l2cap_coc_info[i].is_connected = false;
            s_app_ble_l2cap_coc_info[i].conn_handle  = 0;
            s_app_ble_l2cap_coc_info[i].src_cid      = 0;
            s_app_ble_l2cap_coc_info[i].mtu          = 0;
            s_app_ble_l2cap_coc_info[i].spsm         = 0;
            tlk_printf("BLE l2cap CoC disconnected, conn_handle: %d, src_cid: %d", conn_handle, src_cid);
            break;
        }
    }
}

static void app_ble_l2cap_coc_receive_data_callback(uint16_t conn_handle, uint16_t cid, const uint8_t *data, uint16_t len)
{
    tlk_printf("BLE l2cap CoC receive data, conn_handle: %d, cid: %d, len: %d", conn_handle, cid, len);
    tlk_printf("data: %s", hex_to_str(data, min(len, 32)));
}

static void app_ble_l2cap_coc_mtu_update_callback(uint16_t conn_handle, uint16_t src_cid, uint16_t mtu)
{
    for (int i = 0; i < BLE_L2CAP_COC_MAX_COUNT; i++) {
        if (s_app_ble_l2cap_coc_info[i].is_connected == true && s_app_ble_l2cap_coc_info[i].conn_handle == conn_handle && s_app_ble_l2cap_coc_info[i].src_cid == src_cid) {
            s_app_ble_l2cap_coc_info[i].mtu = mtu;
            tlk_printf("BLE l2cap CoC mtu update, conn_handle: %d, src_cid: %d, mtu: %d", conn_handle, src_cid, mtu);
            break;
        }
    }
}

static void app_connected_callback(struct ble_host_conn *conn);
static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);

static const struct ble_host_acl_conn_callbacks s_app_acl_callbacks = {
    .connected    = app_connected_callback,
    .disconnected = app_disconnected_callback,
};

static void app_connected_callback(struct ble_host_conn *conn)
{
    uint16_t conn_handle = conn->conn_handle;
    for (int i = 0; i < BLE_L2CAP_COC_ACL_MAX_NUMBER; i++) {
        if (s_app_ble_l2cap_coc_conn_info[i].conn_handle == 0xFFFF) {
            s_app_ble_l2cap_coc_conn_info[i].conn_handle = conn_handle;
            break;
        }
    }
}

static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    (void)reason;
    uint16_t conn_handle = conn->conn_handle;
    for (int i = 0; i < BLE_L2CAP_COC_ACL_MAX_NUMBER; i++) {
        if (s_app_ble_l2cap_coc_conn_info[i].conn_handle == conn_handle) {
            s_app_ble_l2cap_coc_conn_info[i].conn_handle = 0xFFFF;
            break;
        }
    }
}

static void app_ble_l2cap_coc_operate_callback(uint16_t conn_handle, bool success)
{
    tlk_printf("BLE l2cap CoC operate callback, conn_handle: %d, success: %d", conn_handle, success);
}

static void app_ble_l2cap_create_connection(void)
{
    for (int i = 0; i < BLE_L2CAP_COC_ACL_MAX_NUMBER; i++) {
        if (s_app_ble_l2cap_coc_conn_info[i].conn_handle == 0xFFFF) {
            continue;
        }
        uint16_t conn_handle = s_app_ble_l2cap_coc_conn_info[i].conn_handle;
        int      ret         = ble_host_coc_create_connection_by_le_credit(conn_handle, BLE_L2CAP_COC_PARAMETER_SPSM, app_ble_l2cap_coc_operate_callback);
        tlk_printf("BLE l2cap CoC create connection, conn_handle: %d, ret: 0x%x", conn_handle, ret);
    }
}

static void app_ble_l2cap_coc_create_more_connection(void)
{
    for (int i = 0; i < BLE_L2CAP_COC_ACL_MAX_NUMBER; i++) {
        if (s_app_ble_l2cap_coc_conn_info[i].conn_handle == 0xFFFF) {
            continue;
        }
        uint16_t conn_handle = s_app_ble_l2cap_coc_conn_info[i].conn_handle;
        int      ret         = ble_host_coc_create_connection_by_credit(conn_handle, BLE_L2CAP_COC_PARAMETER_SPSM, 5, app_ble_l2cap_coc_operate_callback);
        tlk_printf("BLE l2cap CoC create more connection, conn_handle: %d, ret: 0x%x", conn_handle, ret);
    }
}

static void app_ble_l2cap_coc_disconnect_all(void)
{
    for (int i = 0; i < BLE_L2CAP_COC_MAX_COUNT; i++) {
        if (s_app_ble_l2cap_coc_info[i].is_connected == true) {
            uint16_t conn_handle = s_app_ble_l2cap_coc_info[i].conn_handle;
            uint16_t src_cid     = s_app_ble_l2cap_coc_info[i].src_cid;
            int      ret         = ble_host_coc_disconnect(conn_handle, src_cid);
            tlk_printf("BLE l2cap CoC disconnect, conn_handle: %d, src_cid: %d, ret: 0x%x", conn_handle, src_cid, ret);
        }
    }
}

static void app_ble_l2cap_coc_send_data_callback(uint16_t conn_handle, uint16_t src_cid, bool success, void *arg)
{
    (void)arg;
    tlk_printf("BLE l2cap CoC send data callback, conn_handle: %d, src_cid: %d, success: %d", conn_handle, src_cid, success);
}

static void app_ble_l2cap_coc_send_data(void)
{
    uint8_t data[BLE_L2CAP_COC_PARAMETER_MTU];
    for (int i = 0; i < BLE_L2CAP_COC_PARAMETER_MTU; i++) {
        data[i] = i;
    }

    for (int i = 0; i < BLE_L2CAP_COC_MAX_COUNT; i++) {
        if (s_app_ble_l2cap_coc_info[i].is_connected == true) {
            uint16_t conn_handle = s_app_ble_l2cap_coc_info[i].conn_handle;
            uint16_t src_cid     = s_app_ble_l2cap_coc_info[i].src_cid;
            uint16_t mtu         = s_app_ble_l2cap_coc_info[i].mtu;
            mtu                  = min(mtu, BLE_L2CAP_COC_PARAMETER_MTU);
            int ret              = ble_host_coc_send_data(conn_handle, src_cid, data, mtu, app_ble_l2cap_coc_send_data_callback, NULL);
            tlk_printf("BLE L2CAP CoC Send Data, conn_handle: %d, src_cid: %d, ret: 0x%x", conn_handle, src_cid, ret);
        }
    }
}

int INIT(APP_BLE_L2CAP_COC)(void)
{
    INIT(APP_BLE_ACL)();

    memset(s_app_ble_l2cap_coc_info, 0, sizeof(s_app_ble_l2cap_coc_info));
    memset(s_app_ble_l2cap_coc_conn_info, 0xFF, sizeof(s_app_ble_l2cap_coc_conn_info));
    ble_host_coc_set_all_parameters(&s_demo_coc_params);
    app_key_register_callback(2, app_ble_l2cap_create_connection);
    app_key_register_callback(3, app_ble_l2cap_coc_create_more_connection);
    app_key_register_callback(4, app_ble_l2cap_coc_disconnect_all);
    app_key_register_callback(5, app_ble_l2cap_coc_send_data);
    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA2_USER_ID, &s_app_acl_callbacks);
    tlk_printf("BLE l2cap CoC demo initialized");
    return 0;
}

void START(APP_BLE_L2CAP_COC)(void)
{
    START(APP_BLE_ACL)();
    tlk_printf("BLE l2cap CoC demo started");
}
