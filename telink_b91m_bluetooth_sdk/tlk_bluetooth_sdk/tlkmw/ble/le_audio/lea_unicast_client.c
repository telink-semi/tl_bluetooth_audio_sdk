/********************************************************************************************************
 * @file    lea_unicast_client.c
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
#include "tlkapp/audio/tlkapp_audioScheduler.h"
#include "tlkapp/audio/tlkapp_audioUac.h"
#include "tlkapp/lemgr/tlkapp_lemgrAcl.h"
#include "tlkapp/lemgr/tlkapp_lemgrMsg.h"
#include "tlkmw/audio/le_audio/tlkmdi_lea_uc.h"

#define MW_LEA_UC_PRINTF(fmt, ...) tlk_printf("[MW_LEA_UC] " fmt, ##__VA_ARGS__)

// #define MW_LEA_UC_PRINTF(...)
typedef enum
{
    STOP_MODE,
    PAIRING_MODE = 0,
    PASSIVE_MODE,
} lea_uc_mode_t;

struct
{
    bool          is_running;
    bool          sirk_flag;
    lea_uc_mode_t mode;
    uint8_t       addr_type;
    uint8_t       addr[6];
    uint8_t       sirk[16];
} audio_device_context = {0};

/**
 * @brief       Parse LE advertisements and extract LEA/CISIS related data.
 * @param[in]   length  - AD structure length.
 * @param[in]   type    - AD type identifier.
 * @param[in]   value   - AD payload pointer.
 * @param[out]  user    - decoder context storing parsed values.
 * @return      1 to continue parsing.
 */
static int lea_unicast_client_check_adv_data_handle(uint8_t length, uint8_t type, const uint8_t *value, void *user)
{
    struct cap_device_adv_data_value *p_adv_data = user;

    if (type == DT_FLAGS) {
        p_adv_data->flags = value[0];
    } else if (type == DT_SERVICE_DATA_16BIT_UUID) {
        uint16_t uuid = *(const uint16_t *)value;
        if (uuid == SERVICE_UUID_AUDIO_STREAM_CONTROL) {
            p_adv_data->lea_audio_flags = true;
        }
    } else if (type == DT_COMPLETE_LOCAL_NAME) {
        p_adv_data->complete_name_len = length - 1;
        length                        = min(length, sizeof(p_adv_data->complete_name));
        memcpy(p_adv_data->complete_name, value, length);
    } else if (type == DT_CSIP_RSI) {
        p_adv_data->rsi_flags = true;
        memcpy(p_adv_data->rsi, value, 6);
    }

    return 1;
}

/**
 * @brief       Notified when ACL central connection creation completes.
 * @param[in]   result  - true when connection succeeds.
 * @return      none.
 */
static void lea_unicast_client_acl_central_create_callback(bool result)
{
    if (result) {
        MW_LEA_UC_PRINTF("create connection success");
    } else { //timeout clean
        audio_device_context.is_running = false;
        MW_LEA_UC_PRINTF("create connection fail");
    }
}

/**
 * @brief       Process extended scan data, filter LEA devices, and trigger connects.
 * @param[in]   addr_type   - advertiser address type.
 * @param[in]   addr        - advertiser address.
 * @param[in]   data        - extended AD payload.
 * @param[in]   data_len    - payload length.
 * @return      none.
 */
static void lea_unicast_client_ext_scan_handler(uint8_t addr_type, uint8_t *addr, const uint8_t *data, uint8_t data_len)
{
    struct cap_device_adv_data_value adv_data = {0};
    const int                        ret      = ltv_unpack(data, data_len, lea_unicast_client_check_adv_data_handle, &adv_data);

    if (ret != LTV_UNPACK_SUCCESS) {
        return;
    }

    if (adv_data.lea_audio_flags && adv_data.flags & (FLAGS_LE_LIMITED_DISCOVERABLE_MODE | FLAGS_LE_GENERAL_DISCOVERABLE_MODE)) {
        if (!cap_device_insert_adv(addr_type, addr, &adv_data)) {
            tlkapp_lemgr_sendExtScanDataEvt(addr_type, addr, (uint8_t *)adv_data.complete_name, adv_data.complete_name_len);
        }
    }

    if (audio_device_context.is_running) {
        return;
    }

    struct ble_host_smp_store_key *p_store_key   = ble_host_smp_store_get_pairing_info(addr_type, addr);
    uint32_t                       pairing_index = p_store_key != NULL ? p_store_key->pairing_index : 0;
    if (pairing_index != 0) {
        // check if the device is already paired.
        if (ble_host_acl_conn_check_pairing_index(pairing_index)) {
            // already connected. ignore.
            return;
        }
    }

    if (pairing_index > 0 && tlkapp_lemgr_GetAutoRec()) {
        audio_device_context.is_running = true;
        audio_device_context.mode       = PAIRING_MODE;
        // audio_device_context.addr_type = addr_type;
        // memcpy(audio_device_context.addr, addr, 6);
        MW_LEA_UC_PRINTF("using bond info connect");
        if (ble_host_gap_acl_central_ext_create_connection(
                BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_BALANCED(addr_type, addr, lea_unicast_client_acl_central_create_callback))) {
            audio_device_context.is_running = false;
        }
    } else if (audio_device_context.sirk_flag) {
        if (adv_data.rsi_flags) {
            if (ble_csis_check_rsi_valid(adv_data.rsi, audio_device_context.sirk)) {
                MW_LEA_UC_PRINTF("using sirk connect");
                audio_device_context.is_running = true;
                if (ble_host_gap_acl_central_ext_create_connection(
                        BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_BALANCED(addr_type, addr, lea_unicast_client_acl_central_create_callback))) {
                    audio_device_context.is_running = false;
                }
            }
        }
    }
}

/**
 * @brief       Lemgr callback adapter for directed advertising reports.
 * @param[in]   p_adv_report    - directed advertising report.
 * @return      none.
 */
void tlkapp_lemgr_ext_scan_directed_report_handler(const struct ble_host_gap_evt_scan_adv_report_directed *p_adv_report)
{
    lea_unicast_client_ext_scan_handler(p_adv_report->addr_type, (uint8_t *)(size_t)p_adv_report->addr, p_adv_report->data, p_adv_report->data_len);
}

/**
 * @brief       Lemgr callback adapter for undirected advertising reports.
 * @param[in]   p_adv_report    - undirected advertising report.
 * @return      none.
 */
void tlkapp_lemgr_ext_scan_undirected_report_handler(const struct ble_host_gap_evt_scan_adv_report_undirected *p_adv_report)
{
    lea_unicast_client_ext_scan_handler(p_adv_report->addr_type, (uint8_t *)(size_t)p_adv_report->addr, p_adv_report->data, p_adv_report->data_len);
}

/**
 * @brief       Receive SDP discovery events and configure codecs/tasks accordingly.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   event_id    - SDP event identifier.
 * @param[in]   event_msg   - event payload.
 * @return      none.
 */
static void lea_unicast_client_sdp_flags_event_handler(uint16_t conn_handle, enum prf_common_event_id event_id, const void *event_msg)
{
    struct ble_host_conn *conn = ble_host_conn_find_by_conn_handle(conn_handle);

    cap_device_profile_discover(conn_handle, conn->peer_ota_addr.type, conn->peer_ota_addr.val, event_id, event_msg);

    switch (event_id) {
    case PRF_EVT_ID_SDP_FOUND:
    {
        struct ble_prf_sdp_info *p_sdp_info = (struct ble_prf_sdp_info *)event_msg;
        MW_LEA_UC_PRINTF("service id %d,service name %s", p_sdp_info->service_id, p_sdp_info->service_name);
    } break;
    case PRF_EVT_ID_SDP_FINISH:
    {
        ble_vcsc_write_set_absolute_volume(conn_handle, 100);
        audio_device_context.mode = PASSIVE_MODE;
        MW_LEA_UC_PRINTF("sdp discover finished");
        audio_device_context.is_running = false;
        if (cap_device_get_remain_device_number(conn_handle)) {
            uint8_t *sirk = cap_device_get_sirk(conn_handle);
            if (sirk) {
                audio_device_context.sirk_flag = true;
                memcpy(audio_device_context.sirk, sirk, 16);
                MW_LEA_UC_PRINTF("found sirk");
                break;
            }
        }
        MW_LEA_UC_PRINTF("not found sirk");
        audio_device_context.sirk_flag = false;
        memset(audio_device_context.sirk, 0, 16);

    } break;
    default:
    {
    } break;
    }
}

/**
 * @brief       CAP device state callback, create/delete scheduler tasks per stream.
 * @param[in]   device_id   - CAP device identifier.
 * @param[in]   conn_state  - CAP device state transition.
 * @return      none.
 */
void lea_unicast_client_cap_device_state_cb(uint8_t device_id, uint8_t conn_state)
{
    MW_LEA_UC_PRINTF("audio task for device_id %d, now %s task", device_id, conn_state == CAP_DEVICE_STATE_CONNECTED ? "create " : "delete");
    if (conn_state == CAP_DEVICE_STATE_CONNECTED) {
        uint32_t                         same_task_id   = 0;
        tlkapp_audioScheduler_taskInfo_t lea_music_info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC,
            .optype    = TLKAUD_TYPE_LEA_UC_MUSIC,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_LEA_UC_MUSIC),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };
        uint32_t music_task_id                   = device_id + ((uint32_t)TLKAUD_TYPE_LEA_UC_MUSIC << 16);
        same_task_id                             = device_id + ((uint32_t)TLKAUD_TYPE_LEA_UC_VOICE << 16);
        tlkapp_audioScheduler_extraInfo_t exInfo = {
#if (TLK_USB_UAC_ENABLE || TLK_USB_UAC_HS_ENABLE)
            .stateChangeCB = tlkapp_audioUac_taskStateChgCB,
#endif
        };
        tlkapp_audioScheduler_updateTaskEx(music_task_id, lea_music_info, same_task_id, exInfo);

        tlkapp_audioScheduler_taskInfo_t lea_voice_info = {
            .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE,
            .optype    = TLKAUD_TYPE_LEA_UC_VOICE,
            .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_LEA_UC_VOICE),
            .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
        };
        uint32_t voice_task_id = device_id + ((uint32_t)TLKAUD_TYPE_LEA_UC_VOICE << 16);
        same_task_id           = device_id + ((uint32_t)TLKAUD_TYPE_LEA_UC_MUSIC << 16);
        tlkapp_audioScheduler_updateTaskEx(voice_task_id, lea_voice_info, same_task_id, exInfo);
    } else if (conn_state == CAP_DEVICE_STATE_DISCONNECTED) {
        uint32_t taskID = device_id + ((uint32_t)TLKAUD_TYPE_LEA_UC_MUSIC << 16);
        tlkapp_audioScheduler_deleteTask(taskID);
        taskID = device_id + ((uint32_t)TLKAUD_TYPE_LEA_UC_VOICE << 16);
        tlkapp_audioScheduler_deleteTask(taskID);
    }
}

/**
 * @brief       Initialize LEA unicast client GATT profiles and CAP state machine.
 * @return      none.
 */
void lea_unicast_client_profile_init(void)
{
    struct ble_tmass_register_param tmas_param = {.role = {
                                                      .cg_support  = 1,
                                                      .ums_support = 1,
                                                  }};
    //init profile server
    blc_svc_addCoreGroup();
    blc_svc_addDisGroup();
    blc_svc_addGtbsGroup();
    // blc_audio_registerMediaControlServer(&mcpsParam);
    ble_lea_register_TMAS_control_server(&tmas_param);
    blc_svc_calculateDatabaseHash();

    //init profile client
    ble_lea_register_VCS_control_client(NULL);
    ble_lea_register_PACS_control_client(NULL);
    ble_lea_register_ASCS_control_client(NULL);
    ble_lea_register_CSIS_control_client(NULL);
    ble_prf_register_event_handler(lea_unicast_client_sdp_flags_event_handler);

    ble_capuc_init(lea_unicast_client_cap_device_state_cb);
}

/**
 * @brief       ACL connected callback: start pairing/MTU exchange and notify UI.
 * @param[in]   conn    - connection descriptor.
 * @return      none.
 */
static void lea_unicast_client_connected_callback(struct ble_host_conn *conn)
{
    int ret = ble_host_smp_start_pairing(conn->conn_handle);
    MW_LEA_UC_PRINTF("start pairing, ret: 0x%x", ret);
    ret = ble_host_gattc_send_exchange_mtu_req(conn->conn_handle, 65, NULL);
    MW_LEA_UC_PRINTF("send exchange mtu req, ret: 0x%x", ret);

    tlkapp_lemgr_sendAclConnectEvt(conn->conn_handle, 0, conn->role, conn->peer_ota_addr.type, conn->peer_ota_addr.val);
}

/**
 * @brief       ACL disconnected callback: restart scanning and reset context.
 * @param[in]   conn    - connection descriptor.
 * @param[in]   reason  - disconnect reason.
 * @return      none.
 */
static void lea_unicast_client_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    uint8_t set_id = cap_device_get_device_set_id(conn->conn_handle);

    MW_LEA_UC_PRINTF("disconnected, conn_handle: 0x%03x, reason: 0x%02x, role: %d, set_id: %d", conn->conn_handle, reason, conn->role, set_id);

    tlkapp_lemgr_sendAclDisconnEvt(conn->conn_handle, reason, conn->peer_ota_addr.val);

    tlkapp_lemgr_start_scan();
    audio_device_context.is_running = false;

    if (1 == cap_device_get_remain_device_number(conn->conn_handle)) {
        audio_device_context.sirk_flag = false;
        memset(audio_device_context.sirk, 0, 16);
    }
}
static const struct ble_host_acl_conn_callbacks s_le_acl_callbacks = {
    .connected    = lea_unicast_client_connected_callback,
    .disconnected = lea_unicast_client_disconnected_callback,
};

/**
 * @brief       Initialize GAP filtering, scanning, and connection callbacks.
 * @return      none.
 */
void lea_unicast_client_gap_init(void)
{
    ble_host_smp_store_init(0, 4);
    ble_host_gap_filter_init();
    ble_host_gap_ext_scan_init();
    ble_host_gap_cis_central_init();

    struct ble_host_gap_scan_filter_param filter_param = {
        .filter_non_conn_non_scan = true,
        .filter_scannable         = true,
    };
    ble_host_gap_scan_set_filter(&filter_param);
    ble_host_gap_ext_scan_set_param(BLE_HOST_GAP_EXT_SCAN_PARAM_BASIC_UNFILTERED, BLE_HOST_GAP_PASSIVE_SCAN(100, 100));

    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA2_USER_ID, &s_le_acl_callbacks);
}

static struct prefer_codec_cfg prefer_codec_cfg = {
    .music_codec_number = 1,
    .voice_codec_number = 1,
    .music_codec_cfg =
        {
            {LEA_CAPUC_LC3_48_4_STEREO_BALANCED},
        },
    .voice_codec_cfg =
        {
            {LEA_CAPUC_LC3_32_2_STEREO_BALANCED, LEA_CAPUC_LC3_32_2_STEREO_BALANCED},
        },
};

/**
 * @brief       Entry point to bring up LEA unicast client profile and start scanning.
 * @return      none.
 */
void lea_unicast_client_start(void)
{
    MW_LEA_UC_PRINTF("lea unicast client start");
    lea_unicast_client_profile_init();
    lea_unicast_client_gap_init();

    tlkmdi_lea_uc_set_prefer_codec_cfg(&prefer_codec_cfg);
    audio_device_context.mode = PASSIVE_MODE;

    tlkapp_lemgr_msgHandle(TLKSYS_LE_MSGID_START_EXT_SCAN, NULL, 0);
}
