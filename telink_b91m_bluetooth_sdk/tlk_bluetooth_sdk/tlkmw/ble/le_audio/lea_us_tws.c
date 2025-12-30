/********************************************************************************************************
 * @file    lea_us_tws.c
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

#include "inc/lea_us_tws.h"

#include "stack/ble/controller/ll/iso/iso.h"
#include "stack/ble/host_v1/hci/inc/ble_hci.h"
#define GAP_APPEARANCE_tws              0x0942 // tws

#define LEA_tws_MAX_DEVICE_NAME_LEN     32
#define LEA_tws_VCS_DEFAULT_VOLUME_STEP 15

static uint8_t s_lea_us_ext_adv_handle = BLE_HOST_GAP_EXT_ADV_INVALID_ADV_HANDLE;

static struct ad_data_flags s_adv_flags = {
    .header.length                      = 0x02,
    .header.type                        = DT_FLAGS,
    .flags.le_limited_discoverable_mode = 1,
    .flags.br_edr_not_supported         = 1,
};

static const struct ad_data_appearance s_adv_appearance = {
    .header.length = 0x03,
    .header.type   = DT_APPEARANCE,
    .appearance    = GAP_APPEARANCE_tws,
};

struct le_tws_ad_data_complete_local_name
{
    struct ltv_data header; //refer to AD_TYPE_SHORTENED_LOCAL_NAME
    char            name[LEA_tws_MAX_DEVICE_NAME_LEN];
} __attribute__((packed));

struct le_tws_ad_data_csis_rsi
{
    struct ltv_data header; //refer to DT_CSIP_RSI
    uint8_t         rsi[6];
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

struct le_tws_ad_data_incomplete_list_16bit_service_uuid
{
    struct ltv_data header;  //refer to AD_TYPE_INCOMPLETE_LIST_16BIT_SERVICE_UUID
    uint16_t        uuid[7]; //More 16-bit Service UUIDs available.
} __attribute__((packed));

static const struct le_tws_ad_data_incomplete_list_16bit_service_uuid s_adv_service_uuid_list = {
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

///// LE Audio Unicast Server tws Server Initial Parameters.
/*** PACS Server Parameters ***/
static const struct lea_pac_param s_sink_pac[] = {
    LEA_PAC_PARAM(LC3_16_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_32_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_48_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
    LEA_PAC_PARAM(LC3_48_4, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL | LEA_CONTEXT_TYPE_MEDIA),
};

static const struct lea_pac_param s_source_pac[] = {
    LEA_PAC_PARAM(LC3_16_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL),
    LEA_PAC_PARAM(LC3_32_2, LEA_CHANNEL_COUNTS_1, LEA_CONTEXT_TYPE_CONVERSATIONAL),
};

static struct ble_pacss_register_param s_pacss_param = {
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
                                                              }};

//CSIP Set Member parameter configure
static struct ble_csiss_register_param csipSetMemberParam = {
    .set_rank  = 1,
    .set_size  = 2,
    .SIRK_type = BLE_CSIS_SIRK_ENCRYPTED,
    // user should change SIRK.
    .plain_SIRK = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0f, 0x10},
};

/**
 * @brief       Add mandatory GAP/GATT/BAS services for the earbuds.
 * @param[in]   device_name    - name stored in GAP device name characteristic.
 * @return      none.
 */
static void lea_us_tws_add_base_services(const char *device_name)
{
    // GATT & GAP group.
    blc_svc_addCoreGroup();
    blc_svc_setDeviceName(device_name);
    blc_svc_setAppearance(GAP_APPEARANCE_tws);
    // Battery group.
    blc_svc_addBasGroup();
}

/**
 * @brief       Register LE Audio services (PACS/ASCS/VCS/TMAS/CSIS) per ear.
 * @param[in]   param   - application parameters describing ear type/volume.
 * @return      none.
 */
static void lea_us_tws_add_lea_services(const struct lea_us_tws_param *param)
{
    // TODO: Add LE Audio specific services.
    if (param == NULL) {
        return;
    }
    if (param->ear_type == TWS_LEFT_EAR_BUD) {
        csipSetMemberParam.set_rank          = 1;
        csipSetMemberParam.set_member_name   = "LEA-Left-Ear-Bud";
        s_pacss_param.sink_audio_locations   = LEA_LOCATION_FRONT_LEFT;
        s_pacss_param.source_audio_locations = LEA_LOCATION_FRONT_LEFT;
    } else if (param->ear_type == TWS_RIGHT_EAR_BUD) {
        csipSetMemberParam.set_rank          = 2;
        csipSetMemberParam.set_member_name   = "LEA-Right-Ear-Bud";
        s_pacss_param.sink_audio_locations   = LEA_LOCATION_FRONT_RIGHT;
        s_pacss_param.source_audio_locations = LEA_LOCATION_FRONT_RIGHT;
    }
    memcpy(csipSetMemberParam.plain_SIRK, param->sirk, 16);

    // PACS Server
    ble_lea_register_PACS_control_server(&s_pacss_param);
    // ASCS Server
    ble_lea_register_ASCS_control_server(&s_ascss_param);

    // VCS server
    struct ble_vcss_register_param s_vcss_param = {
        .volume = param->volume,
        .step   = LEA_tws_VCS_DEFAULT_VOLUME_STEP,
    };
    ble_lea_register_VCS_control_server(&s_vcss_param);

    // TMAP Server
    ble_lea_register_TMAS_control_server(&s_tmass_param);

    // GMCS Client.
    ble_lea_register_GMCS_control_client(NULL);

    // CSIS Server
    ble_lea_register_CSIS_control_server(&csipSetMemberParam); //CSIP Set Member init
}

/**
 * @brief       Build and start extended advertising carrying LEA CAP/BAP/CSIP data.
 * @param[in]   param   - advertising configuration including name/interval.
 * @return      none.
 */
static void lea_us_tws_set_advertising_data(const struct lea_us_tws_param *param)
{
    s_lea_us_ext_adv_handle = ble_host_gap_extend_adv_create_set();
    if (s_lea_us_ext_adv_handle == BLE_HOST_GAP_EXT_ADV_INVALID_ADV_HANDLE) {
        // create extend adv set failed
        return;
    }

    ble_host_gap_extend_adv_set_param(s_lea_us_ext_adv_handle, BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE(param->interval, param->interval, ble_host_rand() & 0x0F));

    struct le_tws_ad_data_complete_local_name ad_complete_name;

    int device_name_len            = strlen(param->device_name);
    device_name_len                = min(LEA_tws_MAX_DEVICE_NAME_LEN, device_name_len);
    ad_complete_name.header.length = device_name_len + 1;
    ad_complete_name.header.type   = DT_COMPLETE_LOCAL_NAME;
    memcpy(ad_complete_name.name, param->device_name, device_name_len);

    struct le_tws_ad_data_csis_rsi ad_data_csis_rsi = {0};
    ad_data_csis_rsi.header.length                  = 7;
    ad_data_csis_rsi.header.type                    = DT_CSIP_RSI;
    ble_csiss_generate_random_rsi(ad_data_csis_rsi.rsi);

    if (param->ear_type == TWS_LEFT_EAR_BUD) {
    } else if (param->ear_type == TWS_RIGHT_EAR_BUD) {
        s_adv_flags.flags.flags = 0;
    }
    const struct ltv_data *tws_ad_data[] = {
        (const struct ltv_data *)&s_adv_flags,
        (const struct ltv_data *)&s_adv_appearance,
        (const struct ltv_data *)&ad_complete_name,
        (const struct ltv_data *)&ad_data_csis_rsi,
        (const struct ltv_data *)&s_adv_cap_announcement,
        (const struct ltv_data *)&s_adv_bap_announcement,
        (const struct ltv_data *)&s_adv_service_uuid_list,
        (const struct ltv_data *)&s_adv_tamp_role,
        NULL, // end of advertisement data
    };

    uint8_t adv_data[255];
    int32_t adv_len = ltv_pack(tws_ad_data, adv_data);

    ble_host_gap_extend_adv_set_advertising_data(s_lea_us_ext_adv_handle, adv_data, adv_len);

    ble_host_gap_extend_adv_start(s_lea_us_ext_adv_handle);
}

static void lea_us_acl_connected_callback(struct ble_host_conn *conn);
static void lea_us_acl_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);

struct lea_us_tws_info
{
    bool                               is_connected;
    bool                               close_flag;
    uint16_t                           conn_handle;
    lea_us_tws_operate_finish_callback callback;
};

static struct lea_us_tws_info s_lea_us_tws_info = {
    .is_connected = false,
    .close_flag   = false,
};

static const struct ble_host_acl_conn_callbacks s_app_acl_callbacks = {
    .connected    = lea_us_acl_connected_callback,
    .disconnected = lea_us_acl_disconnected_callback,
};

/**
 * @brief       BLE ACL connect callback: cache handle and stop advertising.
 * @param[in]   conn    - pointer to host connection structure.
 * @return      none.
 */
static void lea_us_acl_connected_callback(struct ble_host_conn *conn)
{
    s_lea_us_tws_info.conn_handle  = conn->conn_handle;
    s_lea_us_tws_info.is_connected = true;
    ble_host_gap_extend_adv_stop(s_lea_us_ext_adv_handle);

    ble_host_gattc_send_exchange_mtu_req(conn->conn_handle, 517, NULL);
}

/**
 * @brief       BLE ACL disconnect callback: restart adv or invoke stop callback.
 * @param[in]   conn    - pointer to host connection.
 * @param[in]   reason  - HCI error code.
 * @return      none.
 */
static void lea_us_acl_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    (void)conn;
    (void)reason;
    s_lea_us_tws_info.is_connected = false;
    if (s_lea_us_tws_info.close_flag) {
        ble_host_gap_extend_adv_stop(s_lea_us_ext_adv_handle);
        lea_us_tws_operate_finish_callback callback = s_lea_us_tws_info.callback;
        s_lea_us_tws_info.callback                  = NULL;
        s_lea_us_tws_info.close_flag                = false;
        if (callback != NULL) {
            callback();
        }
    } else {
        ble_host_gap_extend_adv_start(s_lea_us_ext_adv_handle);
    }
}

/**
 * @brief       Initialize LEA TWS unicast server stack and start advertising.
 * @param[in]   param   - configuration parameters.
 * @return      none.
 */
void lea_unicast_server_tws_initial(const struct lea_us_tws_param *param)
{
    if (param == NULL || param->device_name == NULL) {
        return;
    }

    ble_host_gap_extend_adv_init();
    ble_host_gap_cis_peripheral_init();

    lea_us_tws_add_base_services(param->device_name);
    lea_us_tws_add_lea_services(param);


    ble_lea_register_GMCS_control_client(NULL);
    ble_lea_register_GTBS_control_client(NULL);

    blc_svc_calculateDatabaseHash();

    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_app_acl_callbacks);

    ble_host_smp_initial(BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS);
    ble_host_smp_store_init(4, 0);

    lea_us_tws_set_advertising_data(param);
}

/**
 * @brief       Resume advertising when no ACL connection exists.
 * @return      none.
 */
void lea_unicast_server_tws_start(void)
{
    if (s_lea_us_tws_info.is_connected == false) {
        ble_host_gap_extend_adv_start(s_lea_us_ext_adv_handle);
    }
}

/**
 * @brief       Stop advertising or disconnect and notify caller when finished.
 * @param[in]   callback    - invoked after resources are released.
 * @return      none.
 */
void lea_unicast_server_tws_stop(lea_us_tws_operate_finish_callback callback)
{
    if (s_lea_us_tws_info.is_connected == false) {
        ble_host_gap_extend_adv_stop(s_lea_us_ext_adv_handle);
        if (callback != NULL) {
            callback();
        }
    } else {
        ble_host_gap_disconnect_acl(s_lea_us_tws_info.conn_handle);
        s_lea_us_tws_info.close_flag = true;
        s_lea_us_tws_info.callback   = callback;
    }
}
