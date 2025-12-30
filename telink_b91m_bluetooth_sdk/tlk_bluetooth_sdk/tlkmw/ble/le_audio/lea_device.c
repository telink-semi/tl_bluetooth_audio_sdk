/********************************************************************************************************
 * @file    lea_device.c
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

#include "core/mcu_type.h"

#include "common/types.h"
#include "common/utility.h"
#include "common/bit.h"

#include "stack/ble/ble.h"

#include "tlkmw/audio/le_audio/tlkmdi_lea_bmr.h"
#include "inc/lea_device.h"

#include "stack/ble/controller/ll/iso/iso.h"
#include "stack/ble/host_v1/hci/inc/ble_hci.h"

#define GAP_APPEARANCE_HEADSET              0x0942 // Headset

#define LEA_HEADSET_MAX_DEVICE_NAME_LEN     32
#define LEA_HEADSET_VCS_DEFAULT_VOLUME_STEP 15

static uint8_t s_lea_us_ext_adv_handle = BLE_HOST_GAP_EXT_ADV_INVALID_ADV_HANDLE;

static const struct ad_data_flags s_adv_flags = {
    .header.length                      = 0x02,
    .header.type                        = DT_FLAGS,
    .flags.le_limited_discoverable_mode = 1,
    .flags.br_edr_not_supported         = 1,
};

static const struct ad_data_appearance s_adv_appearance = {
    .header.length = 0x03,
    .header.type   = DT_APPEARANCE,
    .appearance    = GAP_APPEARANCE_HEADSET,
};

struct le_device_ad_data_complete_local_name
{
    struct ltv_data header; //refer to AD_TYPE_SHORTENED_LOCAL_NAME
    char            name[LEA_HEADSET_MAX_DEVICE_NAME_LEN];
} __attribute__((packed));

static const struct ad_data_tamp_role s_adv_tamp_role = {
    .length   = 0x05,
    .type     = DT_SERVICE_DATA,
    .tamsUuid = SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO,
    .role =
        {
            .ct_support  = 1,
            .umr_support = 1,
        },
};

struct le_device_ad_data_incomplete_list_16bit_service_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_INCOMPLETE_LIST_16BIT_SERVICE_UUID
    uint16_t        uuid[8]; //More 16-bit Service UUIDs available.
} __attribute__((packed));

struct le_sink_ad_data_broadcast_sink
{
    struct ltv_data header;    //refer to DT_SERVICE_DATA_16BIT_UUID
    uint16_t        bass_uuid; // fixed SERVICE_UUID_BROADCAST_AUDIO_SCAN.
} __attribute__((packed));

static const struct le_device_ad_data_incomplete_list_16bit_service_uuid s_adv_service_uuid_list = {
    .header.length = 17,
    .header.type   = DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID,
    .uuid =
        {
            SERVICE_UUID_AUDIO_STREAM_CONTROL,
            SERVICE_UUID_PUBLISHED_AUDIO_CAPABILITIES,
            SERVICE_UUID_VOLUME_CONTROL,
            SERVICE_UUID_COMMON_AUDIO,
            SERVICE_UUID_MICROPHONE_CONTROL,
            SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO,
            SERVICE_UUID_COORDINATED_SET_IDENTIFICATION,
            SERVICE_UUID_BROADCAST_AUDIO_SCAN,
        },
};

static const struct ad_data_lea_cap_announcement s_adv_cap_announcement = {
    .length  = 4,
    .ad_type = DT_SERVICE_DATA,
    .uuid    = SERVICE_UUID_COMMON_AUDIO,
    .type    = LEA_BAP_ANN_TYPE_GENERAL_ANNOUNCEMENT,
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


///// LE Audio Unicast Server Headset Server Initial Parameters.
/*** PACS Server Parameters ***/
static const struct lea_pac_param s_sink_pac[] = {
    // LEA_PAC_PARAM(LC3_8_2, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_16_1, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_16_2, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_24_1, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_24_2, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_32_1, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_32_2, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_48_1, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_48_2, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_48_3, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_48_4, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_48_5, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_48_6, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
};

static const struct lea_pac_param s_source_pac[] = {
    // LEA_PAC_PARAM(LC3_8_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL),
    LEA_PAC_PARAM(LC3_16_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL), LEA_PAC_PARAM(LC3_16_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL),
    LEA_PAC_PARAM(LC3_24_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL), LEA_PAC_PARAM(LC3_24_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL),
    LEA_PAC_PARAM(LC3_32_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL), LEA_PAC_PARAM(LC3_32_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL),
    LEA_PAC_PARAM(LC3_48_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL), LEA_PAC_PARAM(LC3_48_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL),
    LEA_PAC_PARAM(LC3_48_3, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL), LEA_PAC_PARAM(LC3_48_4, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL),
};

static const struct ble_pacss_register_param s_pacss_param = {
    .sink_pac_num              = ARRAY_SIZE(s_sink_pac),
    .sink_pac                  = s_sink_pac,
    .sink_audio_locations      = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
    .source_pac_num            = ARRAY_SIZE(s_source_pac),
    .source_pac                = s_source_pac,
    .source_audio_locations    = LEA_LOCATION_FRONT_LEFT,
    .available_sink_contexts   = LEA_CONTEXT_TYPE_ALL_SELECT,
    .available_source_contexts = LEA_CONTEXT_TYPE_ALL_SELECT,
    .supported_sink_contexts   = LEA_CONTEXT_TYPE_ALL_SELECT,
    .supported_source_contexts = LEA_CONTEXT_TYPE_ALL_SELECT,
};

static const struct ble_ascss_register_param s_ascss_param = {
    .config_param = BLE_ASCS_SERVER_CONFIG(5, 0x32, 40, 40),
};

static const struct ble_tmass_register_param s_tmass_param = {.role = {
                                                                  .ct_support  = 1,
                                                                  .ums_support = 1,
                                                                  .bmr_support = 1,
                                                              }};

/**
 * @brief       Add GAP/GATT/BAS service groups and set device name/appearance.
 * @param[in]   device_name    - string assigned to GAP device name.
 * @return      none.
 */
static void lea_device_add_base_services(const char *device_name)
{
    // GATT & GAP group.
    blc_svc_addCoreGroup();
    blc_svc_setDeviceName(device_name);
    blc_svc_setAppearance(GAP_APPEARANCE_HEADSET);
    // Battery group.
    blc_svc_addBasGroup();
}

/**
 * @brief       Register LE Audio services (BASS/PACS/ASCS/VCS/TMAS/GMCS/GTBS).
 * @param[in]   param   - configuration containing default volume.
 * @return      none.
 */
static void lea_device_add_lea_services(const struct lea_device_param *param)
{
    // TODO: Add LE Audio specific services.

    ble_lea_register_BASS_control_server(NULL);
    // PACS Server
    ble_lea_register_PACS_control_server(&s_pacss_param);
    // ASCS Server
    ble_lea_register_ASCS_control_server(&s_ascss_param);

    // VCS server
    struct ble_vcss_register_param s_vcss_param = {
        .volume = param->volume,
        .step   = LEA_HEADSET_VCS_DEFAULT_VOLUME_STEP,
    };
    ble_lea_register_VCS_control_server(&s_vcss_param);

    // TMAP Server
    ble_lea_register_TMAS_control_server(&s_tmass_param);

    // GMCS Client.
    ble_lea_register_GMCS_control_client(NULL);
}

/**
 * @brief       Create and start extended advertising with LEA announcements.
 * @param[in]   param   - advertising configuration (interval/name).
 * @return      none.
 */
static void lea_device_set_advertising_data(const struct lea_device_param *param)
{
    s_lea_us_ext_adv_handle = ble_host_gap_extend_adv_create_set();
    if (s_lea_us_ext_adv_handle == BLE_HOST_GAP_EXT_ADV_INVALID_ADV_HANDLE) {
        // create extend adv set failed
        return;
    }

    ble_host_gap_extend_adv_set_param(s_lea_us_ext_adv_handle, BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE(param->interval, param->interval, ble_host_rand() & 0x0F));

    struct le_device_ad_data_complete_local_name ad_complete_name;

    int device_name_len            = strlen(param->device_name);
    device_name_len                = min(LEA_HEADSET_MAX_DEVICE_NAME_LEN, device_name_len);
    ad_complete_name.header.length = device_name_len + 1;
    ad_complete_name.header.type   = DT_COMPLETE_LOCAL_NAME;
    memcpy(ad_complete_name.name, param->device_name, device_name_len);

    const struct ltv_data *device_ad_data[] = {
        (const struct ltv_data *)&s_adv_flags, (const struct ltv_data *)&s_adv_appearance, (const struct ltv_data *)&ad_complete_name,
        // (const struct ltv_data *) &ad_data_csis_rsi,
        (const struct ltv_data *)&s_adv_cap_announcement, (const struct ltv_data *)&s_adv_bap_announcement, (const struct ltv_data *)&advDefBroadcastSink,
        (const struct ltv_data *)&s_adv_service_uuid_list, (const struct ltv_data *)&s_adv_tamp_role,
        NULL, // end of advertisement data
    };

    uint8_t adv_data[255];
    int32_t adv_len = ltv_pack(device_ad_data, adv_data);

    ble_host_gap_extend_adv_set_advertising_data(s_lea_us_ext_adv_handle, adv_data, adv_len);

    ble_host_gap_extend_adv_start(s_lea_us_ext_adv_handle);
}

static void lea_us_acl_connected_callback(struct ble_host_conn *conn);
static void lea_us_acl_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);

struct lea_device_info
{
    bool                               is_connected;
    bool                               close_flag;
    uint16_t                           conn_handle;
    lea_device_operate_finish_callback callback;
};

static struct lea_device_info s_lea_device_info = {
    .is_connected = false,
    .close_flag   = false,
};

static const struct ble_host_acl_conn_callbacks s_app_acl_callbacks = {
    .connected    = lea_us_acl_connected_callback,
    .disconnected = lea_us_acl_disconnected_callback,
};

/**
 * @brief       ACL connection callback: cache handle and stop advertising.
 * @param[in]   conn    - BLE connection descriptor.
 * @return      none.
 */
static void lea_us_acl_connected_callback(struct ble_host_conn *conn)
{
    s_lea_device_info.conn_handle  = conn->conn_handle;
    s_lea_device_info.is_connected = true;
    ble_host_gap_extend_adv_stop(s_lea_us_ext_adv_handle);

    ble_host_gattc_send_exchange_mtu_req(conn->conn_handle, 517, NULL);
}

/**
 * @brief       ACL disconnect callback: restart advertising or signal stop.
 * @param[in]   conn    - BLE connection descriptor.
 * @param[in]   reason  - HCI error code.
 * @return      none.
 */
static void lea_us_acl_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    (void)conn;
    (void)reason;
    s_lea_device_info.is_connected = false;
    if (s_lea_device_info.close_flag) {
        ble_host_gap_extend_adv_stop(s_lea_us_ext_adv_handle);
        lea_device_operate_finish_callback callback = s_lea_device_info.callback;
        s_lea_device_info.callback                  = NULL;
        s_lea_device_info.close_flag                = false;
        if (callback != NULL) {
            callback();
        }
    } else {
        ble_host_gap_extend_adv_start(s_lea_us_ext_adv_handle);
    }
}

static struct lea_bmr_stream_param s_bis_sink_stream_param;

/**
 * @brief       Handle BIS sink events to configure and start BMR playback.
 * @param[in]   event       - BAP sink event ID.
 * @param[in]   event_msg   - pointer to event data.
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

/**
 * @brief       Initialize generic LEA device stack, services, and advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_device_init(const struct lea_device_param *param)
{
    if (param == NULL || param->device_name == NULL) {
        return;
    }

    ble_host_gap_extend_adv_init();
    ble_host_gap_cis_peripheral_init();
    ble_host_gap_big_init();
    ble_host_gap_ext_scan_init();
    ble_host_gap_pa_sync_init();
    ble_host_gap_big_sync_init();

    lea_device_add_base_services(param->device_name);
    lea_device_add_lea_services(param);

    ble_lea_register_GMCS_control_client(NULL);
    ble_lea_register_GTBS_control_client(NULL);

    blc_svc_calculateDatabaseHash();

    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_app_acl_callbacks);

    ble_host_smp_initial(BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS);
    ble_host_smp_store_init(4, 0);

    lea_device_set_advertising_data(param);

    ble_lea_initial_bap_broadcast_sink_role(app_bis_sink_event_handler);
}

/**
 * @brief       Start advertising if device is currently idle.
 * @return      none.
 */
void lea_device_start(void)
{
    if (s_lea_device_info.is_connected == false) {
        ble_host_gap_extend_adv_start(s_lea_us_ext_adv_handle);
    }
}

/**
 * @brief       Stop advertising or disconnect then invoke completion callback.
 * @param[in]   callback    - invoked when stop operation completes.
 * @return      none.
 */
void lea_device_stop(lea_device_operate_finish_callback callback)
{
    if (s_lea_device_info.is_connected == false) {
        ble_host_gap_extend_adv_stop(s_lea_us_ext_adv_handle);
        if (callback != NULL) {
            callback();
        }
    } else {
        ble_host_gap_disconnect_acl(s_lea_device_info.conn_handle);
        s_lea_device_info.close_flag = true;
        s_lea_device_info.callback   = callback;
    }
}
