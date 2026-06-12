/********************************************************************************************************
 * @file    app_ble.c
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

#include "tlkmw/ble/le_controller/tlkmdi_ble_ctrl.h"
#include "vendor/common/ble_common/tlkble_hal_mode.h"

#include "tlkmw/sys_dev/tlkmw_sysdev.h"

#include "app_ble.h"
#include "app_ble_telink_server.h"
#include "app_ble_server.h"
#include "../app_recording_card/ui/app_rc_ui.h"

#define LE_DEVICE_NAME           "Xyris"
#define LEA_MAX_DEVICE_NAME_LEN  30

#define APP_TELINK_SERVER_ENABLE 1

static void app_connected_callback(struct ble_host_conn *conn);
static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);

struct ble_ad_data_complete_local_name
{
    struct ltv_data header; //refer to AD_TYPE_SHORTENED_LOCAL_NAME
    char            name[LEA_MAX_DEVICE_NAME_LEN];
} __attribute__((packed));

struct ble_ad_data_manufacture_specific_data
{
    struct ltv_data header;       //refer to AD_TYPE_MANUFACTURER_SPECIFIC_DATA
    uint16_t        companyID;    // company ID. fixed as 0xFFFF.
    char            sn_value[16]; //Any remainder contains manufacturer specific data.
} __attribute__((packed));

struct ble_ad_data_complete_128bit_service_uuid
{
    struct ltv_data           header; //refer to AD_TYPE_COMPLETE_LIST_128BIT_SERVICE_UUID
    struct server_uuid_128bit uuid;   //Complete list of 128-bit Service UUIDs.
} __attribute__((packed));


static const struct ad_data_flags s_adv_flags = {
    .header.length                      = 0x02,
    .header.type                        = DT_FLAGS,
    .flags.le_limited_discoverable_mode = 1,
    .flags.br_edr_not_supported         = 1,
};

static const struct ble_ad_data_manufacture_specific_data s_adv_manu_data = {
    .header.length = 19,
    .header.type   = DT_MANUFACTURER_SPECIFIC_DATA,
    .companyID     = 0xFFFF,
    .sn_value      = "2506160101300008",
};

static const struct ble_ad_data_complete_128bit_service_uuid s_adv_uuid = {
    .header.length = 17,
    .header.type   = DT_COMPLETE_LIST_128BIT_SERVICE_UUID,
    .uuid.uuid     = {APP_SERVICE_UUID},
};

static const struct ble_ad_data_complete_local_name s_adv_name = {
    .header.length = sizeof(LE_DEVICE_NAME),
    .header.type   = DT_COMPLETE_LOCAL_NAME,
    .name          = LE_DEVICE_NAME,
};

static const struct ltv_data *ad_data[] = {
    (const struct ltv_data *)&s_adv_flags, (const struct ltv_data *)&s_adv_name, (const struct ltv_data *)&s_adv_manu_data,
    NULL, // end of advertisement data
};
static const struct ltv_data *ad_data_rsp[] = {
    (const struct ltv_data *)&s_adv_uuid,
    NULL, // end of advertisement data
};

static const struct ble_host_acl_conn_callbacks s_app_acl_callbacks = {
    .connected    = app_connected_callback,
    .disconnected = app_disconnected_callback,
};

static uint16_t            s_acl_conn_handle = 0xFFFF;
static TlkApiTimerHandle_t s_acl_timer       = NULL;
static TlkApiTimerHandle_t s_acl_2M_timer    = NULL;

static app_le_operate_finish_callback s_operate_callback = NULL;

static void app_acl_update_acl_phy_update(uint16_t conn_handle, uint8_t tx_phy, uint8_t rx_phy)
{
    tlk_printf("[APP] acl phy update, conn_handle: 0x%03x, tx_phy: 0x%02x, rx_phy: 0x%02x", conn_handle, tx_phy, rx_phy);
    if (tx_phy == BLE_HOST_ACL_PHY_LE_2M && rx_phy == BLE_HOST_ACL_PHY_LE_2M) {
    } else {
        int ret = ble_host_gap_acl_phy_update(s_acl_conn_handle, BLE_HOST_ACL_PHY_LE_2M, BLE_HOST_ACL_PHY_LE_2M, app_acl_update_acl_phy_update);
        tlk_printf("[APP] acl phy update, ret: %x", ret);
    }
}

static void app_acl_features_exchange_callback(uint16_t conn_handle, uint64_t features)
{
    tlk_printf("[APP] acl features exchange, conn_handle: 0x%03x, features: 0x%016llx", conn_handle, features);
}

static void app_acl_connected_delay_callback(TlkApiTimerHandle_t handle, void *userArg)
{
    (void)userArg;
    tlksys_timer_destroy(TLKSYS_TASKID_HOST, handle);
    s_acl_timer = NULL;
    int ret     = ble_host_gap_acl_phy_update(s_acl_conn_handle, BLE_HOST_ACL_PHY_LE_2M, BLE_HOST_ACL_PHY_LE_2M, app_acl_update_acl_phy_update);
    tlk_printf("[APP] acl phy update, ret: %x", ret);
}

static void app_acl_connected_update_interval(TlkApiTimerHandle_t handle, void *userArg)
{
    (void)userArg;
    tlksys_timer_destroy(TLKSYS_TASKID_HOST, handle);
    s_acl_2M_timer                                          = NULL;
    struct ble_host_signaling_conn_param_update_param param = {
        .min_interval = 18,
        .max_interval = 25,
        .latency      = 30,
        .timeout      = 500,
    };
    int ret = ble_host_signaling_send_conn_param_update(s_acl_conn_handle, &param, NULL);
    tlk_printf("[APP] conn param update, ret: %x", ret);
}

static void app_connected_callback(struct ble_host_conn *conn)
{
    tlk_printf("[APP] connected callback, ACL handle: 0x%03x", conn->conn_handle);
    s_acl_conn_handle = conn->conn_handle;
    ble_host_gattc_send_exchange_mtu_req(conn->conn_handle, APP_BLE_MTU_SIZE, NULL);
    int ret       = ble_host_gap_acl_features_exchange(s_acl_conn_handle, app_acl_features_exchange_callback);
    int timer_ret = tlksys_timer_create(TLKSYS_TASKID_HOST, &s_acl_timer, 3 * 1000000, false, app_acl_connected_delay_callback, NULL);
    if (timer_ret == 0x00) {
        tlksys_timer_start(TLKSYS_TASKID_HOST, s_acl_timer);
    }

    timer_ret = tlksys_timer_create(TLKSYS_TASKID_HOST, &s_acl_2M_timer, 4 * 1000000, false, app_acl_connected_update_interval, NULL);
    if (timer_ret == 0x00) {
        tlksys_timer_start(TLKSYS_TASKID_HOST, s_acl_2M_timer);
    }

    tlk_printf("[APP] acl features exchange, ret: %x", ret);
#if APP_TELINK_SERVER_ENABLE
    app_tlk_record_acl_connect(s_acl_conn_handle);
#endif
    app_server_acl_connected(s_acl_conn_handle);

    app_rc_ui_update_ble_state(RC_UI_BLE_CON);
}

static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    tlk_printf("[APP] disconnected callback, ACL handle: 0x%03x, reason: 0x%02x", conn->conn_handle, reason);
    s_acl_conn_handle = 0xFFFF;
    if (s_acl_timer != NULL) {
        tlksys_timer_destroy(TLKSYS_TASKID_HOST, s_acl_timer);
        s_acl_timer = NULL;
    }

    if (s_acl_2M_timer != NULL) {
        tlksys_timer_destroy(TLKSYS_TASKID_HOST, s_acl_2M_timer);
        s_acl_2M_timer = NULL;
    }
#if APP_TELINK_SERVER_ENABLE
    app_tlk_record_acl_disconnect();
#endif
    app_server_acl_disconnected();
    if (s_operate_callback != NULL) {
        s_operate_callback();
        s_operate_callback = NULL;
    } else {
        ble_host_gap_adv_start();
        app_rc_ui_update_ble_state(RC_UI_BLE_BRO);
    }
}

static void ble_stack_init(void)
{
    uint8_t mac[6];

    tlkhal_get_bluetooth_mac(mac);

    ble_host_v1_init();
    ble_host_v1_set_bd_addr(mac);
}

void tlkapp_host_le_init(void)
{
    ble_stack_init();

    blc_svc_addCoreGroup();
    app_ble_server_init();

#if APP_TELINK_SERVER_ENABLE
    app_tlk_record_init();
#endif
    blc_svc_addOtaV2Group();
    blc_svc_calculateDatabaseHash();
    blc_svc_setPeripheralPreferredParameters(0x0006, 0x000c, 0x0019, 0x012c);

    ble_host_hci_acl_data_set_max_pending_num(0xFF);

    blc_svc_setDeviceName(LE_DEVICE_NAME);
    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_app_acl_callbacks);
    ble_host_gap_adv_set_adv_ind_param(150, ad_data, ad_data_rsp);

    ble_host_set_att_mtu(APP_BLE_MTU_SIZE);
}

void tlkapp_host_le_start(void)
{
    // default disable ble.
    ble_host_gap_adv_start();
    app_rc_ui_update_ble_state(RC_UI_BLE_BRO);
}

void app_ble_close_host(app_le_operate_finish_callback callback)
{
    if (s_acl_conn_handle == 0xFFFF) {
        ble_host_gap_adv_stop();
        if (callback != NULL) {
            callback();
        }
    } else {
        ble_host_gap_disconnect_acl(s_acl_conn_handle);
        s_operate_callback = callback;
    }
    app_rc_ui_update_ble_state(RC_UI_BLE_NONE);
}

void app_ble_open_host(void)
{
    // tlkapp_host_le_init();
    if (s_acl_conn_handle == 0xFFFF) {
        ble_host_gap_adv_start();
        app_rc_ui_update_ble_state(RC_UI_BLE_BRO);
    }
}

bool app_ble_is_connected(void)
{
    return !(s_acl_conn_handle == 0xFFFF);
}
