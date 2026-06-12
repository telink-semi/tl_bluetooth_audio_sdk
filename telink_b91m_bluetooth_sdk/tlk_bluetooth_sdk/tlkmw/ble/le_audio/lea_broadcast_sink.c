/********************************************************************************************************
 * @file    lea_broadcast_sink.c
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
#include <string.h>
#include <stdio.h>

#include "common/types.h"
#include "common/utility.h"
#include "common/bit.h"

#include "stack/ble/ble.h"

#include "tlkapi/tlkapi_debug.h"

#include "tlkmw/audio/le_audio/tlkmdi_lea_bmr.h"
#include "inc/lea_broadcast_sink.h"

#define BIS_SINK_APPEARANCE           GAP_APPEARANCE_GWAD

#define BIS_SINK_DEFAULT_ADV_INTERVAL 48
#define BIS_SINK_ADV_SID              6
#define BIS_SINK_DEVICE_NAME          "BIS-SINK"
#define BIS_SINK_DEVICE_NAME_MAX_SIZE 32

struct app_bis_sink_info
{
    bool                                is_connected;
    uint16_t                            conn_handle;
    lea_broadcast_sink_operate_callback operate_callback;
    uint8_t                             ext_adv_handle;
    uint8_t                             device_name_len;
    char                                device_name[BIS_SINK_DEVICE_NAME_MAX_SIZE];
};

static struct app_bis_sink_info s_app_bis_sink_info;


/*** PACS Server Parameters ***/
#define LEA_PACS_SINK_PAC_CHANNEL_1_2_LIST                                                \
    X(LC3_16_1, LEA_CHANNEL_COUNTS_1 | LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_ALL_SELECT) \
    X(LC3_16_2, LEA_CHANNEL_COUNTS_1 | LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_ALL_SELECT) \
    X(LC3_24_1, LEA_CHANNEL_COUNTS_1 | LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_ALL_SELECT) \
    X(LC3_24_2, LEA_CHANNEL_COUNTS_1 | LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_ALL_SELECT) \
    X(LC3_32_1, LEA_CHANNEL_COUNTS_1 | LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_ALL_SELECT) \
    X(LC3_32_2, LEA_CHANNEL_COUNTS_1 | LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_ALL_SELECT) \
    X(LC3_48_1, LEA_CHANNEL_COUNTS_1 | LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_ALL_SELECT) \
    X(LC3_48_2, LEA_CHANNEL_COUNTS_1 | LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_ALL_SELECT) \
    X(LC3_48_3, LEA_CHANNEL_COUNTS_1 | LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_ALL_SELECT) \
    X(LC3_48_4, LEA_CHANNEL_COUNTS_1 | LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_ALL_SELECT)

static const uint8_t s_pacs_sink_pac_channel_1_2[] = {
#define X(lc3_id, chn, contexts) lc3_id,
    COUNT_ARGS(LEA_PACS_SINK_PAC_CHANNEL_1_2_LIST) - 1,
#undef X
#define X(lc3_id, chn, contexts) LEA_PAC_PARAM(lc3_id, chn, contexts),
    LEA_PACS_SINK_PAC_CHANNEL_1_2_LIST
#undef X
};

static const struct ble_pacss_register_param s_pacss_param = {
    .sink_pac_len              = ARRAY_SIZE(s_pacs_sink_pac_channel_1_2),
    .sink_pac                  = s_pacs_sink_pac_channel_1_2,
    .sink_audio_locations      = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
    .source_audio_locations    = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
    .available_sink_contexts   = LEA_CONTEXT_TYPE_ALL_SELECT,
    .available_source_contexts = LEA_CONTEXT_TYPE_PROHIBITED,
    .supported_sink_contexts   = LEA_CONTEXT_TYPE_ALL_SELECT,
    .supported_source_contexts = LEA_CONTEXT_TYPE_PROHIBITED,
};

/**
 * @brief       ACL connection callback, stores handle and stops advertising.
 * @param[in]   conn    - ACL connection structure.
 * @return      none.
 */
static void app_bis_sink_acl_connected(struct ble_host_conn *conn)
{
    tlkapi_printf(1, "[APP][BIS] ACL connected");

    s_app_bis_sink_info.is_connected = true;
    s_app_bis_sink_info.conn_handle  = conn->conn_handle;

    ble_host_gattc_send_exchange_mtu_req(conn->conn_handle, 517, NULL);
    if (s_app_bis_sink_info.operate_callback != NULL) {
    } else {
        ble_host_gap_extend_adv_stop(s_app_bis_sink_info.ext_adv_handle);
    }
}

/**
 * @brief       ACL disconnect callback; restarts advertising or fires callback.
 * @param[in]   conn    - ACL connection pointer.
 * @param[in]   reason  - disconnect reason.
 * @return      none.
 */
static void app_bis_sink_acl_disconnected(struct ble_host_conn *conn, uint8_t reason)
{
    (void)conn;
    tlkapi_printf(1, "[APP][BIS] ACL disconnected, reason=0x%x", reason);

    s_app_bis_sink_info.is_connected = false;

    if (s_app_bis_sink_info.operate_callback != NULL) {
        ble_host_gap_extend_adv_delete_set(s_app_bis_sink_info.ext_adv_handle);
        lea_broadcast_sink_operate_callback callback = s_app_bis_sink_info.operate_callback;
        s_app_bis_sink_info.operate_callback         = NULL;
        callback();
    } else {
        ble_host_gap_extend_adv_start(s_app_bis_sink_info.ext_adv_handle);
    }
}

static const struct ble_host_acl_conn_callbacks s_app_bis_sink_acl_conn_cb = {
    .connected    = app_bis_sink_acl_connected,
    .disconnected = app_bis_sink_acl_disconnected,
};

static struct lea_bmr_stream_param s_bis_sink_stream_param;

/**
 * @brief       Handle BAP sink events to configure BIS streams and trigger BMR.
 * @param[in]   event       - sink event identifier.
 * @param[in]   event_msg   - pointer to event payload.
 * @return      none.
 */
static void app_bis_sink_event_handler(enum ble_bap_sink_event_id event, const void *event_msg)
{
    if (event == BAP_SINK_EVENT_ID_CODEC_PARAM) {
        const struct ble_bap_sink_event_codec_param *param = event_msg;
        s_bis_sink_stream_param.frequency                  = param->frequency;
        s_bis_sink_stream_param.frame_duration             = param->frame_duration;
        s_bis_sink_stream_param.frame_octets               = param->frame_octets;
        s_bis_sink_stream_param.all_location               = param->all_location;
        s_bis_sink_stream_param.presentation_delay         = param->presentation_delay;
        s_bis_sink_stream_param.num                        = param->num;

        s_bis_sink_stream_param.bmr_config[0].blocks   = param->param[0].blocks;
        s_bis_sink_stream_param.bmr_config[0].location = param->param[0].location;

        s_bis_sink_stream_param.bmr_config[1].blocks   = param->param[1].blocks;
        s_bis_sink_stream_param.bmr_config[1].location = param->param[1].location;
    } else if (event == BAP_SINK_EVENT_ID_START) {
        const struct ble_bap_sink_event_start *param     = event_msg;
        s_bis_sink_stream_param.bmr_config[0].iso_handle = param->bis_handle[0];
        s_bis_sink_stream_param.bmr_config[1].iso_handle = param->bis_handle[1];

        tlkmdi_lea_bmr_set_stream_param(&s_bis_sink_stream_param);
        tlkmdi_lea_bmr_start_stream();
    } else if (event == BAP_SINK_EVENT_ID_STOP) {
        tlkmdi_lea_bmr_release_stream_param();
        tlkmdi_lea_bmr_stop_stream();
    }
    tlk_printf("[APP][BIS] BIS Sink event: %d", event);
    tlk_printf("[APP][BIS] BIS Sink event_msg: %s", hex_to_str(event_msg, 16));
}

void lea_broadcast_sink_init_mode(void)
{
    ble_host_gap_big_init();
    ble_host_gap_ext_scan_init();
    ble_host_gap_pa_sync_init();
    ble_host_gap_big_sync_init();

    ble_lea_register_BASS_control_server(NULL);

    ble_lea_initial_bap_broadcast_sink_role(app_bis_sink_event_handler);
}

/**
 * @brief       Initialize broadcast sink stack, services, and event callbacks.
 * @param[in]   param   - sink configuration parameters.
 * @return      none.
 */
void lea_broadcast_sink_init(const struct lea_broadcast_sink_param *param)
{
    ble_host_gap_extend_adv_init();

    lea_broadcast_sink_init_mode();

    ble_host_smp_initial(BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS);
    ble_host_smp_store_init(4, 0);

    blc_svc_addCoreGroup();
    blc_svc_addDisGroup();
    ble_basic_register_BAS_control_server(NULL);
    struct ble_tmass_register_param s_tmass_param = {
        .role =
            {
                .bmr_support = 1,
            },
    };
    ble_lea_register_TMAS_control_server(&s_tmass_param);
    struct ble_vcss_register_param s_vcss_param = {
        .step   = 15,
        .volume = param->volume,
        .mute   = false,
    };
    ble_lea_register_VCS_control_server(&s_vcss_param);
    ble_lea_register_PACS_control_server(&s_pacss_param);

    blc_svc_calculateDatabaseHash();

    uint8_t *bd_addr                    = ble_host_hci_get_bd_addr();
    s_app_bis_sink_info.device_name_len = snprintf(s_app_bis_sink_info.device_name, BIS_SINK_DEVICE_NAME_MAX_SIZE,
                                                   "%s"
                                                   "-%02X%02X%02X%02X%02X%02X",
                                                   param->device_name, bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);

    blc_svc_setDeviceName(s_app_bis_sink_info.device_name);
    blc_svc_setAppearance(BIS_SINK_APPEARANCE);

    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_app_bis_sink_acl_conn_cb);
}

static const struct ad_data_flags s_adv_flags = {
    .header.length                      = 0x02,
    .header.type                        = DT_FLAGS,
    .flags.le_limited_discoverable_mode = 1,
    .flags.br_edr_not_supported         = 1,
};

static const struct ad_data_appearance s_adv_appearance = {
    .header.length = 0x03,
    .header.type   = DT_APPEARANCE,
    .appearance    = BIS_SINK_APPEARANCE,
};

struct le_sink_ad_data_complete_local_name
{
    struct ltv_data header; //refer to AD_TYPE_SHORTENED_LOCAL_NAME
    char            name[32];
} __attribute__((packed));

struct le_sink_ad_data_incomplete_list_16bit_service_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_INCOMPLETE_LIST_16BIT_SERVICE_UUID
    uint16_t        uuid[1]; //More 16-bit Service UUIDs available.
} __attribute__((packed));

struct le_sink_ad_data_broadcast_sink
{
    struct ltv_data header;    //refer to DT_SERVICE_DATA_16BIT_UUID
    uint16_t        bass_uuid; // fixed SERVICE_UUID_BROADCAST_AUDIO_SCAN.
} __attribute__((packed));

static const struct le_sink_ad_data_incomplete_list_16bit_service_uuid s_adv_service_uuid_list = {
    .header.length = 3,
    .header.type   = DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID,
    .uuid =
        {
            SERVICE_UUID_BROADCAST_AUDIO_SCAN,
        },
};

static const struct ad_data_lea_bap_announcement s_adv_bap_announcement = {
    .length                    = 9,
    .ad_type                   = DT_SERVICE_DATA,
    .uuid                      = SERVICE_UUID_AUDIO_STREAM_CONTROL,
    .type                      = LEA_BAP_ANN_TYPE_GENERAL_ANNOUNCEMENT,
    .sink_available_contexts   = LEA_CONTEXT_TYPE_ALL_SELECT,
    .source_available_contexts = LEA_CONTEXT_TYPE_ALL_SELECT,
};

static const struct le_sink_ad_data_broadcast_sink advDefBroadcastSink = {
    .header.length = sizeof(struct le_sink_ad_data_broadcast_sink) - 1,
    .header.type   = DT_SERVICE_DATA_16BIT_UUID,
    .bass_uuid     = SERVICE_UUID_BROADCAST_AUDIO_SCAN,
};

/**
 * @brief       Build advertising payload and enable extended advertising.
 * @return      none.
 */
static void lea_broadcast_sink_start_adv(void)
{
    s_app_bis_sink_info.ext_adv_handle = ble_host_gap_extend_adv_create_set();

    struct le_sink_ad_data_complete_local_name ad_complete_name;

    ad_complete_name.header.length = s_app_bis_sink_info.device_name_len + 1;
    ad_complete_name.header.type   = DT_COMPLETE_LOCAL_NAME;
    memcpy(ad_complete_name.name, s_app_bis_sink_info.device_name, s_app_bis_sink_info.device_name_len);

    const struct ltv_data *adv_data_ltvs[] = {
        (const struct ltv_data *)&s_adv_flags,
        (const struct ltv_data *)&s_adv_appearance,
        (const struct ltv_data *)&ad_complete_name,
        (const struct ltv_data *)&s_adv_service_uuid_list,
        (const struct ltv_data *)&advDefBroadcastSink,
        (const struct ltv_data *)&s_adv_bap_announcement,
        NULL, // end of advertisement data
    };

    u8      advData[255];
    int32_t adv_ext_len = ltv_pack(adv_data_ltvs, advData);

    ble_host_gap_extend_adv_set_param(s_app_bis_sink_info.ext_adv_handle,
                                      BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE(BIS_SINK_DEFAULT_ADV_INTERVAL, BIS_SINK_DEFAULT_ADV_INTERVAL, BIS_SINK_ADV_SID));

    ble_host_gap_extend_adv_set_advertising_data(s_app_bis_sink_info.ext_adv_handle, advData, adv_ext_len);

    int status = ble_host_gap_extend_adv_start(s_app_bis_sink_info.ext_adv_handle);

    tlkapi_printf(1, "[APP][INI] Sink set adv enable, status=0x%x", status);
}

/**
 * @brief       Enable advertising so broadcasters can initiate connections.
 * @return      none.
 */
void lea_broadcast_sink_start(void)
{
    lea_broadcast_sink_start_adv();
}

/**
 * @brief       Stop BIS streaming or disconnect, then invoke completion callback.
 * @param[in]   callback    - called when stop action finishes.
 * @return      none.
 */
void lea_broadcast_sink_stop(lea_broadcast_sink_operate_callback callback)
{
    if (s_app_bis_sink_info.is_connected == false) {
        ble_lea_bap_sink_close_all();
        ble_host_gap_extend_adv_stop(s_app_bis_sink_info.ext_adv_handle);
        ble_host_gap_extend_adv_delete_set(s_app_bis_sink_info.ext_adv_handle);
        callback();
    } else {
        ble_lea_bap_sink_close_all();
        ble_host_gap_disconnect_acl(s_app_bis_sink_info.conn_handle);
        s_app_bis_sink_info.operate_callback = callback;
    }
}

/**
 * @brief       Stop current BIS stream and immediately reopen advertising if idle.
 * @return      none.
 */
void lea_broadcast_sink_stop_stream_open_adv(void)
{
    if (s_app_bis_sink_info.is_connected == false) {
        ble_lea_bap_sink_close_all();
        ble_host_gap_extend_adv_start(s_app_bis_sink_info.ext_adv_handle);
    }
}
