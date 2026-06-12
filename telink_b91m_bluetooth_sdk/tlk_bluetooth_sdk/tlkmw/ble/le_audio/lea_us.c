/********************************************************************************************************
 * @file    lea_us.c
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

#include "common/types.h"
#include "common/utility.h"

#include "stack/ble/ble.h"

#include "inc/lea_us.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/audio/le_audio/tlkmdi_lea_bmr.h"

#define LEA_UNICAST_SERVER_VCS_DEFAULT_VOLUME_STEP 15
#define LEA_UNICAST_SERVER_MAX_DEVICE_NAME_LEN     32

static void lea_us_acl_connected_callback(struct ble_host_conn *conn);
static void lea_us_acl_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);
static void lea_us_clear_reconnect_flags(void);

struct tlk_mw_lea_us_info
{
    bool                                       is_connected;
    bool                                       close_flag;
    uint8_t                                    adv_handle;
    bool                                       had_rsi;
    bool                                       had_sink;
    uint16_t                                   appearance;
    uint16_t                                   conn_handle;
    char                                       device_name[LEA_UNICAST_SERVER_MAX_DEVICE_NAME_LEN];
    lea_unicast_server_operate_finish_callback callback;

    bool                                       is_reconn_timer_running;
    bool                                       is_reconn_timer_created;
    TlkApiTimer_t                              reconn_timer;
    lea_unicast_server_operate_finish_callback reconn_callback;
};

struct tlk_mw_lea_us_param
{
    uint8_t sid;
    uint8_t volume; /** < Initial render volume (0~255). */
    bool    is_bis_sink;
    bool    is_cis_peripheral;
    bool    is_tws;

    const char *device_name; /** < Advertising/display name. */
    uint16_t    interval;    /** < Extended advertising interval in milliseconds. */
    uint16_t    appearance;  /** < Appearance value. */

    const struct ble_pacss_register_param *p_pacss_param;
    const struct ble_tmass_register_param *p_tmass_param;

    uint8_t        ear_type;
    const uint8_t *sirk;
};

enum
{
    TLK_MW_LEA_US_HEADSET_SID = 0x01,
    TLK_MW_LEA_US_TWS_SID,
    TLK_MW_LEA_US_SPEAKER_SID,
    TLK_MW_LEA_US_AUDIO_DEVICE_SID,
    TLK_MW_LEA_US_AUDIO_DEVICE_TWS_SID,
};

struct le_headset_ad_data_complete_local_name
{
    struct ltv_data header; //refer to AD_TYPE_SHORTENED_LOCAL_NAME
    char            name[LEA_UNICAST_SERVER_MAX_DEVICE_NAME_LEN];
} __attribute__((packed));

struct le_headset_ad_data_incomplete_list_16bit_service_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_INCOMPLETE_LIST_16BIT_SERVICE_UUID
    uint16_t        uuid[7]; //More 16-bit Service UUIDs available.
} __attribute__((packed));

static struct tlk_mw_lea_us_info s_lea_unicast_server_info = {
    .is_connected = false,
    .close_flag   = false,
};

static struct ad_data_flags s_adv_flags = {
    .header.length                      = 0x02,
    .header.type                        = DT_FLAGS,
    .flags.le_limited_discoverable_mode = 1,
    .flags.br_edr_not_supported         = 1,
};

static const struct ad_data_flags s_adv_reconn_flags = {
    .header.length                      = 0x02,
    .header.type                        = DT_FLAGS,
    .flags.le_limited_discoverable_mode = 0,
    .flags.br_edr_not_supported         = 1,
};

static const struct le_headset_ad_data_incomplete_list_16bit_service_uuid s_adv_service_uuid_list = {
    .header.length = 15,
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

static const struct ad_data_lea_cap_announcement s_adv_reconn_cap_announcement = {
    .length  = 4,
    .ad_type = DT_SERVICE_DATA,
    .uuid    = SERVICE_UUID_COMMON_AUDIO,
    .type    = LEA_BAP_ANN_TYPE_TARGETED_ANNOUNCEMENT,
};

static const struct ad_data_lea_bap_announcement s_adv_reconn_bap_announcement = {
    .length                    = 9,
    .ad_type                   = DT_SERVICE_DATA,
    .uuid                      = SERVICE_UUID_AUDIO_STREAM_CONTROL,
    .type                      = LEA_BAP_ANN_TYPE_TARGETED_ANNOUNCEMENT,
    .sink_available_contexts   = LEA_CONTEXT_TYPE_ALL_SELECT,
    .source_available_contexts = LEA_CONTEXT_TYPE_ALL_SELECT,
};


static const struct ad_data_tamp_role s_adv_tamp_role = {
    .length   = 0x05,
    .type     = DT_SERVICE_DATA,
    .tamsUuid = SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO,
    .role =
        {
            .ct_support  = 1,
            .umr_support = 1,
            .bmr_support = 1,
        },
};

static const struct ble_host_acl_conn_callbacks s_app_acl_callbacks = {
    .connected    = lea_us_acl_connected_callback,
    .disconnected = lea_us_acl_disconnected_callback,
};

static struct le_headset_ad_data_complete_local_name s_adv_complete_name;

static struct ad_data_appearance s_adv_appearance;

static struct ad_data_lea_csis_rsi s_adv_csis_rsi;

static const struct ltv_data *s_tws_ad_data[] = {
    (const struct ltv_data *)&s_adv_flags,
    (const struct ltv_data *)&s_adv_appearance,
    (const struct ltv_data *)&s_adv_complete_name,
    (const struct ltv_data *)&s_adv_csis_rsi,
    (const struct ltv_data *)&s_adv_cap_announcement,
    (const struct ltv_data *)&s_adv_bap_announcement,
    (const struct ltv_data *)&s_adv_service_uuid_list,
    (const struct ltv_data *)&s_adv_tamp_role,
    NULL, // end of advertisement data
};

static const struct ltv_data *s_tws_reconn_ad_data[] = {
    (const struct ltv_data *)&s_adv_reconn_flags,
    (const struct ltv_data *)&s_adv_appearance,
    (const struct ltv_data *)&s_adv_complete_name,
    (const struct ltv_data *)&s_adv_csis_rsi,
    (const struct ltv_data *)&s_adv_reconn_cap_announcement,
    (const struct ltv_data *)&s_adv_reconn_bap_announcement,
    (const struct ltv_data *)&s_adv_service_uuid_list,
    (const struct ltv_data *)&s_adv_tamp_role,
    NULL, // end of advertisement data
};

static const struct ltv_data *s_headset_ad_data[] = {
    (const struct ltv_data *)&s_adv_flags,
    (const struct ltv_data *)&s_adv_appearance,
    (const struct ltv_data *)&s_adv_complete_name,
    (const struct ltv_data *)&s_adv_cap_announcement,
    (const struct ltv_data *)&s_adv_bap_announcement,
    (const struct ltv_data *)&s_adv_service_uuid_list,
    (const struct ltv_data *)&s_adv_tamp_role,
    NULL, // end of advertisement data
};

static const struct ltv_data *s_headset_reconn_ad_data[] = {
    (const struct ltv_data *)&s_adv_reconn_flags,
    (const struct ltv_data *)&s_adv_appearance,
    (const struct ltv_data *)&s_adv_complete_name,
    (const struct ltv_data *)&s_adv_reconn_cap_announcement,
    (const struct ltv_data *)&s_adv_reconn_bap_announcement,
    (const struct ltv_data *)&s_adv_service_uuid_list,
    (const struct ltv_data *)&s_adv_tamp_role,
    NULL, // end of advertisement data
};

static void lea_unicast_server_initialize_common(const struct tlk_mw_lea_us_param *p_param)
{
    if (p_param == NULL || p_param->device_name == NULL) {
        return;
    }

    ble_host_gap_extend_adv_init();
    ble_host_gap_cis_peripheral_init();

    // GATT & GAP group.
    blc_svc_addCoreGroup();
    blc_svc_setDeviceName(p_param->device_name);
    blc_svc_setAppearance(p_param->appearance);
    // Battery group.
    blc_svc_addBasGroup();

    // VCS server
    struct ble_vcss_register_param s_vcss_param = {
        .volume = p_param->volume,
        .step   = LEA_UNICAST_SERVER_VCS_DEFAULT_VOLUME_STEP,
    };
    ble_lea_register_VCS_control_server(&s_vcss_param);

    if (p_param->is_cis_peripheral) {
        // ASCS Server
#if TLK_MW_LEA_LOW_LATENCY_ENABLE
        ble_lea_register_ASCS_control_server(BLE_ASCSS_INITIAL_PARAMS(5, 15, 5, 5));
#else
        ble_lea_register_ASCS_control_server(BLE_ASCSS_INITIAL_PARAMS(5, 50, 40, 40));
#endif
    }

    if (p_param->p_tmass_param != NULL) {
        ble_lea_register_TMAS_control_server(p_param->p_tmass_param);
    }

    if (p_param->p_pacss_param != NULL) {
        ble_lea_register_PACS_control_server(p_param->p_pacss_param);
    }

    if (p_param->is_tws) {
        struct ble_csiss_register_param csipSetMemberParam = {
            .set_rank  = 1,
            .set_size  = 2,
            .SIRK_type = BLE_CSIS_SIRK_ENCRYPTED,
        };
        if (p_param->ear_type == TWS_LEFT_EAR_BUD) {
            csipSetMemberParam.set_rank        = 1;
            csipSetMemberParam.set_member_name = "LEA-Left-Ear-Bud";
        } else if (p_param->ear_type == TWS_RIGHT_EAR_BUD) {
            csipSetMemberParam.set_rank        = 2;
            csipSetMemberParam.set_member_name = "LEA-Right-Ear-Bud";
            s_adv_flags.flags.flags            = 0;
        }

        memcpy(csipSetMemberParam.plain_SIRK, p_param->sirk, 16);
        // CSIS Server
        ble_lea_register_CSIS_control_server(&csipSetMemberParam); //CSIP Set Member init
    }

    // todo: Initial GMCS and GTBS control parameters.
    ble_lea_register_GMCS_control_client(NULL);
    ble_lea_register_GTBS_control_client(NULL);

    blc_svc_calculateDatabaseHash();

    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_app_acl_callbacks);

    ble_host_smp_initial(BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS);
    ble_host_smp_store_init(4, 0);

    s_lea_unicast_server_info.appearance = p_param->appearance;
    strncpy(s_lea_unicast_server_info.device_name, p_param->device_name, LEA_UNICAST_SERVER_MAX_DEVICE_NAME_LEN);
    s_lea_unicast_server_info.had_rsi  = p_param->is_tws;
    s_lea_unicast_server_info.had_sink = p_param->is_bis_sink;

    s_lea_unicast_server_info.adv_handle = ble_host_gap_extend_adv_create_set();
    if (s_lea_unicast_server_info.adv_handle == BLE_HOST_GAP_EXT_ADV_INVALID_ADV_HANDLE) {
        // create extend adv set failed
        return;
    }

    ble_host_gap_extend_adv_set_param(s_lea_unicast_server_info.adv_handle, BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE(p_param->interval, p_param->interval, p_param->sid & 0x0F));

    lea_unicast_server_start_advertising();
}

/**
 * @brief       Start advertising if headset is idle.
 * @return      none.
 */
void tlkmw_lea_us_start_advertising(void)
{
    if (s_lea_unicast_server_info.is_connected == false) {
        ble_host_gap_extend_adv_start(s_lea_unicast_server_info.adv_handle);
    }
}

/**
 * @brief       ACL connect callback: cache handle and stop advertising.
 * @param[in]   conn    - connection instance.
 * @return      none.
 */
static void lea_us_acl_connected_callback(struct ble_host_conn *conn)
{
    lea_us_clear_reconnect_flags();
    tlk_printf("Unicast Server ACL Connected:0x%03x", conn->conn_handle);
    s_lea_unicast_server_info.conn_handle  = conn->conn_handle;
    s_lea_unicast_server_info.is_connected = true;
    ble_host_gap_extend_adv_stop(s_lea_unicast_server_info.adv_handle);

    ble_host_gattc_send_exchange_mtu_req(conn->conn_handle, 517, NULL);
}

/**
 * @brief       ACL disconnect callback: restart advertising or finish stop flow.
 * @param[in]   conn    - connection instance.
 * @param[in]   reason  - HCI error reason.
 * @return      none.
 */
static void lea_us_acl_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    tlk_printf("Unicast Server ACL Disconnected:0x%03x, reason:%d", conn->conn_handle, reason);
    s_lea_unicast_server_info.is_connected = false;
    if (s_lea_unicast_server_info.close_flag) {
        ble_host_gap_extend_adv_stop(s_lea_unicast_server_info.adv_handle);
        lea_unicast_server_operate_finish_callback callback = s_lea_unicast_server_info.callback;
        s_lea_unicast_server_info.callback                  = NULL;
        s_lea_unicast_server_info.close_flag                = false;
        if (callback != NULL) {
            callback();
        }
    } else {
        ble_host_gap_extend_adv_start(s_lea_unicast_server_info.adv_handle);
    }
}

/**
 * @brief       Create extended advertising set containing LEA announcements.
 * @param[in]   param   - advertising interval and device name.
 * @return      none.
 */
static void lea_us_headset_set_advertising_data(bool reconnect)
{
    int device_name_len = strlen(s_lea_unicast_server_info.device_name);
    device_name_len     = min(LEA_UNICAST_SERVER_MAX_DEVICE_NAME_LEN, device_name_len);

    s_adv_complete_name.header.length = device_name_len + 1;
    s_adv_complete_name.header.type   = DT_COMPLETE_LOCAL_NAME;
    memcpy(s_adv_complete_name.name, s_lea_unicast_server_info.device_name, device_name_len);

    s_adv_appearance.header.length = 0x03;
    s_adv_appearance.header.type   = DT_APPEARANCE;
    s_adv_appearance.appearance    = s_lea_unicast_server_info.appearance;

    s_adv_csis_rsi.header.length = 7;
    s_adv_csis_rsi.header.type   = DT_CSIP_RSI;
    ble_csiss_generate_random_rsi(s_adv_csis_rsi.rsi);

    uint8_t adv_data[255] = {0x03, DT_SERVICE_DATA_16BIT_UUID, U16_TO_BYTES(SERVICE_UUID_BROADCAST_AUDIO_SCAN)};
    int32_t adv_len       = 0;

    uint8_t *adv_data_ptr = adv_data;

    if (s_lea_unicast_server_info.had_sink) {
        adv_len      = 4;
        adv_data_ptr = adv_data + 4;
    }

    if (reconnect) {
        if (s_lea_unicast_server_info.had_rsi) {
            adv_len += ltv_pack(s_tws_reconn_ad_data, adv_data_ptr);
        } else {
            adv_len += ltv_pack(s_headset_reconn_ad_data, adv_data_ptr);
        }
    } else {
        if (s_lea_unicast_server_info.had_rsi) {
            adv_len += ltv_pack(s_tws_ad_data, adv_data_ptr);
        } else {
            adv_len += ltv_pack(s_headset_ad_data, adv_data_ptr);
        }
    }


    ble_host_gap_extend_adv_stop(s_lea_unicast_server_info.adv_handle);
    ble_host_gap_extend_adv_set_advertising_data(s_lea_unicast_server_info.adv_handle, adv_data, adv_len);
    ble_host_gap_extend_adv_start(s_lea_unicast_server_info.adv_handle);
}

static void lea_us_clear_reconnect_flags(void)
{
    if (s_lea_unicast_server_info.is_reconn_timer_running == true) {
        tlksys_timer_stop(TLKSYS_TASKID_HOST, &s_lea_unicast_server_info.reconn_timer);
        s_lea_unicast_server_info.is_reconn_timer_running = false;
    }

    if (s_lea_unicast_server_info.is_reconn_timer_created == true) {
        tlksys_timer_destroy(TLKSYS_TASKID_HOST, &s_lea_unicast_server_info.reconn_timer);
        s_lea_unicast_server_info.is_reconn_timer_created = false;
    }
}

void lea_unicast_server_start_advertising(void)
{
    if (s_lea_unicast_server_info.is_connected == false) {
        lea_us_clear_reconnect_flags();
        lea_us_headset_set_advertising_data(false);
    }
}

void lea_unicast_server_stop_task(lea_unicast_server_operate_finish_callback callback)
{
    lea_us_clear_reconnect_flags();
    if (s_lea_unicast_server_info.is_connected == false) {
        ble_host_gap_extend_adv_stop(s_lea_unicast_server_info.adv_handle);
        if (callback != NULL) {
            callback();
        }
    } else {
        ble_host_gap_disconnect_acl(s_lea_unicast_server_info.conn_handle);
        s_lea_unicast_server_info.close_flag = true;
        s_lea_unicast_server_info.callback   = callback;
    }
}

static void lea_us_reconnect_timeout(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    lea_unicast_server_start_advertising();
    if (s_lea_unicast_server_info.reconn_callback != NULL) {
        s_lea_unicast_server_info.reconn_callback();
    }
}

/**
 * @brief       Start reconnect all paired devices.
 * @param[in]   timeout     - timeout in seconds.
 * @param[in]   callback    - called when stop operation is finished.
 *
 * @return      true if operation is started, false if no paired devices.
 */
bool lea_unicast_server_reconnect_all_paired_devices(uint32_t timeout, lea_unicast_server_operate_finish_callback callback)
{
    if (s_lea_unicast_server_info.is_reconn_timer_created == false) {
        tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &s_lea_unicast_server_info.reconn_timer, timeout * 1000 * 1000, false, lea_us_reconnect_timeout, NULL);
        s_lea_unicast_server_info.is_reconn_timer_created = true;
    }

    if (s_lea_unicast_server_info.is_connected == false) {
        lea_us_headset_set_advertising_data(true);
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &s_lea_unicast_server_info.reconn_timer);
        s_lea_unicast_server_info.reconn_callback         = callback;
        s_lea_unicast_server_info.is_reconn_timer_running = true;
    } else {
        if (callback != NULL) {
            callback();
        }
    }

    return s_lea_unicast_server_info.is_connected == false;
}

/***** LE Audio Unicast Server Specific Defines. ********/
/*** PACS Server Parameters ***/
#define LEA_PACS_SINK_PAC_CHANNEL_1_LIST                                                        \
    X(LC3_16_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_16_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_24_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_24_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_32_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_32_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_3, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_4, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_5, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_6, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA)

#define LEA_PACS_SINK_PAC_CHANNEL_2_LIST                                                        \
    X(LC3_16_1, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_16_2, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_24_1, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_24_2, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_32_1, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_32_2, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_1, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_2, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_3, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_4, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_5, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA) \
    X(LC3_48_6, LEA_CHANNEL_COUNTS_2, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA)

#define LEA_PACS_SOURCE_PAC_CHANNEL_1_LIST                             \
    X(LC3_16_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL) \
    X(LC3_16_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL) \
    X(LC3_24_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL) \
    X(LC3_24_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL) \
    X(LC3_32_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL) \
    X(LC3_32_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL) \
    X(LC3_48_1, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL) \
    X(LC3_48_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL) \
    X(LC3_48_3, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL) \
    X(LC3_48_4, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL)

static const uint8_t s_pacs_sink_pac_channel_1[] = {
#define X(lc3_id, chn, contexts) lc3_id,
    COUNT_ARGS(LEA_PACS_SINK_PAC_CHANNEL_1_LIST) - 1,
#undef X
#define X(lc3_id, chn, contexts) LEA_PAC_PARAM(lc3_id, chn, contexts),
    LEA_PACS_SINK_PAC_CHANNEL_1_LIST
#undef X
};

static const uint8_t s_pacs_source_pac_channel_1[] = {
#define X(lc3_id, chn, contexts) lc3_id,
    COUNT_ARGS(LEA_PACS_SOURCE_PAC_CHANNEL_1_LIST) - 1,
#undef X
#define X(lc3_id, chn, contexts) LEA_PAC_PARAM(lc3_id, chn, contexts),
    LEA_PACS_SOURCE_PAC_CHANNEL_1_LIST
#undef X
};

static const uint8_t s_pacs_sink_pac_channel_2[] = {
#define X(lc3_id, chn, contexts) lc3_id,
    COUNT_ARGS(LEA_PACS_SINK_PAC_CHANNEL_2_LIST) - 1,
#undef X
#define X(lc3_id, chn, contexts) LEA_PAC_PARAM(lc3_id, chn, contexts),
    LEA_PACS_SINK_PAC_CHANNEL_2_LIST
#undef X
};

struct ble_pacss_register_param s_tws_pacss_param = {
    .sink_pac_len              = ARRAY_SIZE(s_pacs_sink_pac_channel_1),
    .sink_pac                  = s_pacs_sink_pac_channel_1,
    .sink_audio_locations      = LEA_LOCATION_FRONT_LEFT,
    .source_pac_len            = ARRAY_SIZE(s_pacs_source_pac_channel_1),
    .source_pac                = s_pacs_source_pac_channel_1,
    .source_audio_locations    = LEA_LOCATION_FRONT_LEFT,
    .available_sink_contexts   = LEA_CONTEXT_TYPE_ALL_SELECT,
    .available_source_contexts = LEA_CONTEXT_TYPE_ALL_SELECT,
    .supported_sink_contexts   = LEA_CONTEXT_TYPE_ALL_SELECT,
    .supported_source_contexts = LEA_CONTEXT_TYPE_ALL_SELECT,
};

static const struct ble_pacss_register_param s_headset_pacss_param = {
    .sink_pac_len              = ARRAY_SIZE(s_pacs_sink_pac_channel_2),
    .sink_pac                  = s_pacs_sink_pac_channel_2,
    .sink_audio_locations      = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
    .source_pac_len            = ARRAY_SIZE(s_pacs_source_pac_channel_1),
    .source_pac                = s_pacs_source_pac_channel_1,
    .source_audio_locations    = LEA_LOCATION_FRONT_LEFT,
    .available_sink_contexts   = LEA_CONTEXT_TYPE_ALL_SELECT,
    .available_source_contexts = LEA_CONTEXT_TYPE_ALL_SELECT,
    .supported_sink_contexts   = LEA_CONTEXT_TYPE_ALL_SELECT,
    .supported_source_contexts = LEA_CONTEXT_TYPE_ALL_SELECT,
};

static const struct ble_pacss_register_param s_speaker_pacss_param = {
    .sink_pac_len            = ARRAY_SIZE(s_pacs_sink_pac_channel_2),
    .sink_pac                = s_pacs_sink_pac_channel_2,
    .sink_audio_locations    = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
    .available_sink_contexts = LEA_CONTEXT_TYPE_ALL_SELECT,
    .supported_sink_contexts = LEA_CONTEXT_TYPE_ALL_SELECT,
};

/**
 * @brief       Initialize LEA TWS unicast server stack and start advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_unicast_server_tws_initial(const struct lea_us_tws_param *p_param)
{
    if (p_param == NULL || p_param->device_name == NULL) {
        return;
    }

    if (p_param->ear_type == TWS_LEFT_EAR_BUD) {
        s_tws_pacss_param.sink_audio_locations   = LEA_LOCATION_FRONT_LEFT;
        s_tws_pacss_param.source_audio_locations = LEA_LOCATION_FRONT_LEFT;
    } else if (p_param->ear_type == TWS_RIGHT_EAR_BUD) {
        s_tws_pacss_param.sink_audio_locations   = LEA_LOCATION_FRONT_RIGHT;
        s_tws_pacss_param.source_audio_locations = LEA_LOCATION_FRONT_RIGHT;
    }

    struct tlk_mw_lea_us_param lea_us_param = {
        .sid               = TLK_MW_LEA_US_TWS_SID,
        .volume            = p_param->volume,
        .is_bis_sink       = false,
        .is_cis_peripheral = true,
        .is_tws            = true,
        .device_name       = p_param->device_name,
        .interval          = p_param->interval,
        .appearance        = GAP_APPEARANCE_EARBUD,

        .p_pacss_param = &s_tws_pacss_param,
        .p_tmass_param = BLE_TMASS_INITIAL_PARAMS(0, 1, 0, 1, 0, 0),

        .ear_type = p_param->ear_type,
        .sirk     = p_param->sirk,
    };

    lea_unicast_server_initialize_common(&lea_us_param);
}

/**
 * @brief       Initialize headset LEA stack, services, and advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_unicast_server_headset_initial(const struct tlk_mw_lea_cap_headset_param *p_param)
{
    if (p_param == NULL || p_param->device_name == NULL) {
        return;
    }

    struct tlk_mw_lea_us_param lea_us_param = {
        .sid               = TLK_MW_LEA_US_HEADSET_SID,
        .volume            = p_param->volume,
        .is_bis_sink       = false,
        .is_cis_peripheral = true,
        .is_tws            = false,
        .device_name       = p_param->device_name,
        .interval          = p_param->interval,
        .appearance        = GAP_APPEARANCE_HEADSET,

        .p_pacss_param = &s_headset_pacss_param,
        .p_tmass_param = BLE_TMASS_INITIAL_PARAMS(0, 1, 0, 1, 0, 0),
    };

    lea_unicast_server_initialize_common(&lea_us_param);
}

/**
 * @brief       Initialize speaker LE Audio stack, services, and advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_unicast_server_speaker_initial(const struct tlk_mw_lea_cap_headset_param *p_param)
{
    if (p_param == NULL || p_param->device_name == NULL) {
        return;
    }

    struct tlk_mw_lea_us_param lea_us_param = {
        .sid               = TLK_MW_LEA_US_SPEAKER_SID,
        .volume            = p_param->volume,
        .is_bis_sink       = false,
        .is_cis_peripheral = true,
        .is_tws            = false,
        .device_name       = p_param->device_name,
        .interval          = p_param->interval,
        .appearance        = GAP_APPEARANCE_HEADSET,

        .p_pacss_param = &s_speaker_pacss_param,
        .p_tmass_param = BLE_TMASS_INITIAL_PARAMS(0, 0, 0, 1, 0, 0),
    };

    lea_unicast_server_initialize_common(&lea_us_param);
}

/**
 * @brief       Initialize generic LEA device stack, services, and advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_device_init(const struct tlk_mw_lea_cap_headset_param *p_param)
{
    if (p_param == NULL || p_param->device_name == NULL) {
        return;
    }

    void lea_broadcast_sink_init_mode(void);
    lea_broadcast_sink_init_mode();

    struct tlk_mw_lea_us_param lea_us_param = {
        .sid               = TLK_MW_LEA_US_AUDIO_DEVICE_SID,
        .volume            = p_param->volume,
        .is_bis_sink       = true,
        .is_cis_peripheral = true,
        .is_tws            = false,
        .device_name       = p_param->device_name,
        .interval          = p_param->interval,
        .appearance        = GAP_APPEARANCE_HEADSET,

        .p_pacss_param = &s_headset_pacss_param,
        .p_tmass_param = BLE_TMASS_INITIAL_PARAMS(0, 1, 0, 1, 0, 1),
    };

    lea_unicast_server_initialize_common(&lea_us_param);
}

/**
 * @brief       Initialize LEA TWS unicast server stack and start advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_device_tws_initial(const struct lea_us_tws_param *p_param)
{
    if (p_param == NULL || p_param->device_name == NULL) {
        return;
    }

    void lea_broadcast_sink_init_mode(void);
    lea_broadcast_sink_init_mode();

    if (p_param->ear_type == TWS_LEFT_EAR_BUD) {
        s_tws_pacss_param.sink_audio_locations   = LEA_LOCATION_FRONT_LEFT;
        s_tws_pacss_param.source_audio_locations = LEA_LOCATION_FRONT_LEFT;
    } else if (p_param->ear_type == TWS_RIGHT_EAR_BUD) {
        s_tws_pacss_param.sink_audio_locations   = LEA_LOCATION_FRONT_RIGHT;
        s_tws_pacss_param.source_audio_locations = LEA_LOCATION_FRONT_RIGHT;
    }

    struct tlk_mw_lea_us_param lea_us_param = {
        .sid               = TLK_MW_LEA_US_AUDIO_DEVICE_TWS_SID,
        .volume            = p_param->volume,
        .is_bis_sink       = true,
        .is_cis_peripheral = true,
        .is_tws            = true,
        .device_name       = p_param->device_name,
        .interval          = p_param->interval,
        .appearance        = GAP_APPEARANCE_HEADSET,

        .p_pacss_param = &s_tws_pacss_param,
        .p_tmass_param = BLE_TMASS_INITIAL_PARAMS(0, 1, 0, 1, 0, 1),

        .ear_type = p_param->ear_type,
        .sirk     = p_param->sirk,
    };

    lea_unicast_server_initialize_common(&lea_us_param);
}
