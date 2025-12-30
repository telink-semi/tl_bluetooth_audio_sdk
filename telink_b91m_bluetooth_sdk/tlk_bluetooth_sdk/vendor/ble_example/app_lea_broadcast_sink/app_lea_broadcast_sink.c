/********************************************************************************************************
 * @file    app_lea_broadcast_sink.c
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
#include "../app_example.h"
#include "tlkmw/ble/le_audio/inc/lea_broadcast_sink.h"
#include "tlkmw/ble/le_audio/inc/lea_broadcast_sync.h"
#include "tlkmw/sys_dev/key/tlkdrv_key.h"
#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "tlkmw/audio/le_audio/codec/lea_codec.h"

static void app_connected_callback(struct ble_host_conn *conn);
static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);


static const uint16_t lea_broadcast_sink_key_cfg[TLKDRV_KEY_MAX_NUMB][TLKDRV_KEY_EVTID_MAX] = {
#ifdef KEY1_ID
    [0] =
        {

            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_VENDOR_CONFIG_1,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_VENDOR_CONFIG_2,
        },
#endif
#ifdef KEY2_ID
    [1] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_MODE_VOLUME_UP,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_MODE_VOLUME_DOWN,
        },
#endif
#ifdef KEY3_ID
    [2] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_MODE_MUSIC_PLAY_PAUSE,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_MODE_MUSIC_FORWARD,
        },
#endif

};

static const uint16_t lea_broadcast_sync_key_cfg[TLKDRV_KEY_MAX_NUMB][TLKDRV_KEY_EVTID_MAX] = {
#ifdef KEY1_ID
    [0] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_VENDOR_CONFIG_1,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_VENDOR_CONFIG_2,
        },
#endif
#ifdef KEY2_ID
    [1] =
        {
            [TLKDRV_KEY_EVTID_CLICK]  = KEY_EVT_VENDOR_CONFIG_3,
            [TLKDRV_KEY_EVTID_DCLICK] = KEY_EVT_VENDOR_CONFIG_4,
        },
#endif
};

enum
{
    APP_UI_MODE_BIS_SYNC,
    APP_UI_MODE_BIS_SINK,
};

struct app_ui_info
{
    bool    op_busy;
    uint8_t mode;
    uint8_t operate;
};

static struct app_ui_info s_ui_info = {
    .op_busy = false,
    .mode    = APP_UI_MODE_BIS_SYNC,
};

static void app_ui_mode_change_to_bis_sync(void)
{
    s_ui_info.op_busy = true;
    lea_broadcast_sync_start();
    s_ui_info.op_busy = false;
    s_ui_info.mode    = APP_UI_MODE_BIS_SYNC;
}

static void app_ui_mode_change_to_bis_sink(void)
{
    s_ui_info.op_busy = true;
    lea_broadcast_sink_start();
    s_ui_info.op_busy = false;
    s_ui_info.mode    = APP_UI_MODE_BIS_SINK;
}

static void app_ui_enable_bis_sink(void)
{
    lea_broadcast_sync_stop();
    s_ui_info.op_busy = false;
    app_ui_mode_change_to_bis_sink();
}

static void app_ui_enable_bis_sync_finish(void)
{
    app_ui_mode_change_to_bis_sync();
}

static void app_ui_enable_bis_sync(void)
{
    lea_broadcast_sink_stop(app_ui_enable_bis_sync_finish);
}

static void app_ui_vendor_set_vol(int8_t vol_offset)
{
    static int16_t vol = 200;
    vol                = vol + vol_offset;
    if (vol < 0) {
        vol = 0;
    } else if (vol > 255) {
        vol = 255;
    }
    lea_codec_set_output_volume((uint8_t)vol);
}

static void app_ui_vendor_vol_down_press(void)
{
    app_ui_vendor_set_vol(-30);
}

static void app_ui_vendor_vol_up_press(void)
{
    app_ui_vendor_set_vol(30);
}

static void app_ui_mode_change_press(void)
{
    if (s_ui_info.op_busy == true) {
        tlkapi_printf(1, "mode change is busy, please wait");
        return;
    }

    tlkapi_printf(1, "current mode is %s", s_ui_info.mode == APP_UI_MODE_BIS_SYNC ? "BIS_SYNC" : "BIS_SINK");
    tlkapi_printf(1, "change to %s", s_ui_info.mode == APP_UI_MODE_BIS_SYNC ? "BIS_SINK" : "BIS_SYNC");
    s_ui_info.op_busy = true;
    if (s_ui_info.mode == APP_UI_MODE_BIS_SYNC) {
        tlkmdi_tinySql_updateKeyCofnig((keyConfigs_t *)lea_broadcast_sink_key_cfg);
        app_ui_enable_bis_sink();
    } else if (s_ui_info.mode == APP_UI_MODE_BIS_SINK) {
        tlkmdi_tinySql_updateKeyCofnig((keyConfigs_t *)lea_broadcast_sync_key_cfg);
        app_ui_enable_bis_sync();
    }
}

static void app_ui_change_state_press(void)
{
    tlk_printf("app_ui_change_state_press");
    if (s_ui_info.op_busy == true) {
        tlkapi_printf(1, "mode change is busy, please wait");
        return;
    }
    if (s_ui_info.mode == APP_UI_MODE_BIS_SINK) {
        lea_broadcast_sink_stop_stream_open_adv();
    }
}

void app_ui_init(void)
{
    app_ui_mode_change_to_bis_sync();
    tlkmdi_tinySql_updateKeyCofnig((keyConfigs_t *)lea_broadcast_sync_key_cfg);
    tlkdrv_key_registerVendorConfig1Callback(app_ui_mode_change_press);
    tlkdrv_key_registerVendorConfig2Callback(app_ui_change_state_press);
    tlkdrv_key_registerVendorConfig3Callback(app_ui_vendor_vol_up_press);
    tlkdrv_key_registerVendorConfig4Callback(app_ui_vendor_vol_down_press);
}

static const struct ble_host_acl_conn_callbacks s_app_acl_callbacks = {
    .connected    = app_connected_callback,
    .disconnected = app_disconnected_callback,
};

static void app_connected_callback(struct ble_host_conn *conn)
{
    tlk_printf("[APP] connected callback, ACL handle: 0x%03x", conn->conn_handle);
}

static void app_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    tlk_printf("[APP] disconnected callback, ACL handle: 0x%03x, reason: 0x%02x", conn->conn_handle, reason);
}

int INIT(APP_BLE_LEA_BROADCAST_SINK)(void)
{
    struct lea_broadcast_sink_param s_lea_broadcast_sink_param = {
        .device_name = "app_lea_broadcast_sink",
        .volume      = 80,
    };
    lea_broadcast_sink_init(&s_lea_broadcast_sink_param);
    app_ui_init();

    blc_svc_calculateDatabaseHash();
    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA2_USER_ID, &s_app_acl_callbacks);
    return 0;
}

void START(APP_BLE_LEA_BROADCAST_SINK)(void) {}
