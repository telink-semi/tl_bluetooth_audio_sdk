/********************************************************************************************************
 * @file    app_lea_uc_ui.c
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

#include "tlkmw/audio/common/tlkmdi_audio_common.h"
#include "tlkmw/audio/le_audio/codec/lea_codec.h"
#include "tlkmw/audio/le_audio/le_audio_common.h"
#include "tlkmw/audio/le_audio/le_audio_main.h"

#include "app_lea_uc_prf.h"
#include "app_lea_uc_ext_scan_ctrl.h"
#include "app_lea_uc_filter_device.h"
#include "app_lea_uc_ui.h"

#define APP_UC_FUNCTION_MUSIC_PLAY  0x01
#define APP_UC_FUNCTION_MIC_ONLY    0x02
#define APP_UC_FUNCTION_VOICE_CALL  0x03

#define APP_UC_FUNCTION_SELECT_ITEM APP_UC_FUNCTION_VOICE_CALL

enum app_uc_device_state
{
    APP_UC_DEVICE_STATE_IDLE,
    APP_UC_DEVICE_STATE_DISCOVERY_US,
    APP_UC_DEVICE_STATE_CREATE_ACL_US,
    APP_UC_DEVICE_STATE_WAIT_OTHER_US,
};

struct app_uc_device_info
{
    uint8_t state;
    int8_t  min_rssi_value;
};

static void app_disc_uc_found_callback(uint8_t addr_type, const uint8_t addr[6], const uint8_t *device_name, int8_t rssi);
static void app_uc_connected_callback(struct ble_host_conn *conn);
static void app_uc_disconnected_callback(struct ble_host_conn *conn, uint8_t reason);
static void app_sdp_finish_callback(uint16_t conn_handle, struct ble_app_sdp_flags *flags);
static void app_ui_start_stream_callback(struct ble_uc_stream_config *p_stream_config, enum lea_uc_result result);

static struct app_uc_device_info s_uc_device_info = {
    .state          = APP_UC_DEVICE_STATE_IDLE,
    .min_rssi_value = -35,
};

static const struct discovery_us_init_param s_disc_uc_param = {
    .found_callback = app_disc_uc_found_callback,
};

static const struct ble_host_acl_conn_callbacks s_uc_acl_callbacks = {
    .connected    = app_uc_connected_callback,
    .disconnected = app_uc_disconnected_callback,
};

static struct app_ui_store_us_info
{
    uint8_t size;
    uint8_t curr_size;

    struct
    {
        uint16_t acl_conn_handle;
        uint16_t cis_conn_handle;
    } device_info[2];
} s_ui_store_us_info = {
    .size      = 0,
    .curr_size = 0,
    .device_info[0] =
        {
            .acl_conn_handle = 0xFFFF,
            .cis_conn_handle = 0xFFFF,
        },
    .device_info[1] =
        {
            .acl_conn_handle = 0xFFFF,
            .cis_conn_handle = 0xFFFF,
        },
};

#if APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MUSIC_PLAY
#define CODEC_CONFIG LEA_CODEC_CFG_LC3_48_2(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT)
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MIC_ONLY
#define CODEC_CONFIG LEA_CODEC_CFG_LC3_32_2(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT)
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_VOICE_CALL
#define CODEC_CONFIG LEA_CODEC_CFG_LC3_32_2(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT)
#endif

static struct ble_uc_stream_config s_ui_stream_config[2] = {
    {
        .cig_id      = 0x00,
        .ase_info[0] = {.cis_id = 0x00, .phy = ASCS_TARGET_LE_2M_PHY, .codec_id = LEA_INIT_CODEC_ID_LC3, .codec_cfg = CODEC_CONFIG},
        .ase_info[1] =
            {
                .cis_id    = 0x00,
                .phy       = ASCS_TARGET_LE_2M_PHY,
                .codec_id  = LEA_INIT_CODEC_ID_LC3,
                .codec_cfg = CODEC_CONFIG,
            },
        .callback = app_ui_start_stream_callback,
    },
    {
        .cig_id      = 0x00,
        .ase_info[0] = {.cis_id = 0x01, .phy = ASCS_TARGET_LE_2M_PHY, .codec_id = LEA_INIT_CODEC_ID_LC3, .codec_cfg = CODEC_CONFIG},
        .ase_info[1] =
            {
                .cis_id    = 0x01,
                .phy       = ASCS_TARGET_LE_2M_PHY,
                .codec_id  = LEA_INIT_CODEC_ID_LC3,
                .codec_cfg = CODEC_CONFIG,
            },
        .callback = app_ui_start_stream_callback,
    },
};

/**
 * @brief  Initialize LE Audio UC UI module.
 *          Initialize device filter, extended scan control, discovery and ACL connection callbacks.
 */
void app_lea_uc_ui_init(void)
{
    app_filter_device_init();
    app_extend_scan_ctrl_init();
    app_extend_scan_ctrl_start();
    app_uc_prf_discovery_us(&s_disc_uc_param);
    ble_host_acl_conn_register_user_data(BLE_HOST_APP_DATA1_USER_ID, &s_uc_acl_callbacks);
    app_sdp_flags_initial(app_sdp_finish_callback);
    s_uc_device_info.state = APP_UC_DEVICE_STATE_DISCOVERY_US;
    app_filter_device_init();
}

/**
 * @brief  Start LE Audio UC UI module.
 *          Configure timer, audio codec and start audio streams based on selected function mode.
 */
void app_lea_uc_ui_start(void)
{
#if (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL322X)
    timer_set_irq_mask(FLD_TMR0_MODE_IRQ);
#endif
    plic_interrupt_enable(IRQ_TIMER0);
    plic_set_priority(IRQ_TIMER0, 1);


    lea_input_config_initial();
    lea_output_config_initial();
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, le_audio_timer_irq);
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, le_audio_main_loop);

// set stream config
#if APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MUSIC_PLAY
    lea_set_input_all_location(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT);
    lea_set_input_sample_config_bap(LEA_SELECT_SAMPLING_FREQ_48000_HZ, LEA_SELECT_FRAME_DURATION_10);
    struct lea_codec_config codec_config = {
        .is_input_stream_init  = true,
        .input_sample_rate     = LEA_SELECT_SAMPLING_FREQ_48000_HZ,
        .input_location        = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
        .is_output_stream_init = false,
    };

    lea_codec_stream_init(&codec_config);
    lea_open_input(clock_time());
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MIC_ONLY
    lea_set_output_all_location(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT);
    lea_set_output_sample_config_bap(LEA_SELECT_SAMPLING_FREQ_32000_HZ, LEA_SELECT_FRAME_DURATION_10);
    struct lea_codec_config codec_config = {
        .is_input_stream_init  = false,
        .is_output_stream_init = true,
        .output_sample_rate    = LEA_SELECT_SAMPLING_FREQ_32000_HZ,
        .output_location       = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
    };
    lea_codec_stream_init(&codec_config);
    lea_open_output();
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_VOICE_CALL
    lea_set_input_all_location(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT);
    lea_set_input_sample_config_bap(LEA_SELECT_SAMPLING_FREQ_32000_HZ, LEA_SELECT_FRAME_DURATION_10);
    lea_set_output_all_location(LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT);
    lea_set_output_sample_config_bap(LEA_SELECT_SAMPLING_FREQ_32000_HZ, LEA_SELECT_FRAME_DURATION_10);
    struct lea_codec_config codec_config = {
        .is_input_stream_init  = true,
        .input_sample_rate     = LEA_SELECT_SAMPLING_FREQ_32000_HZ,
        .input_location        = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
        .is_output_stream_init = true,
        .output_sample_rate    = LEA_SELECT_SAMPLING_FREQ_32000_HZ,
        .output_location       = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
    };

    lea_codec_stream_init(&codec_config);
    lea_open_output();
    lea_open_input(clock_time());
#endif

    app_extend_scan_ctrl_start();
}

/**
 * @brief  ACL connection creation callback.
 *
 * @param[in] result  Connection creation result.
 */
static void app_create_acl_connect_callback(bool result)
{
    tlk_printf("[APP] create acl connect callback, result: %d", result);
}

/**
 * @brief  Create ACL connection to the specified address.
 *
 * @param[in] addr_type  Address type.
 * @param[in] addr       Device address.
 *
 * @return  BLE_HOST_ERR_SUCC on success, error code otherwise.
 */
static int app_create_acl_connect(uint8_t addr_type, const uint8_t addr[6])
{
    app_extend_scan_ctrl_stop();
    return ble_host_gap_acl_central_ext_create_connection(BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_BALANCED(addr_type, addr, app_create_acl_connect_callback));
}

static void app_disc_uc_found_callback(uint8_t addr_type, const uint8_t addr[6], const uint8_t *device_name, int8_t rssi)
{
    uint8_t device_index = app_filter_device_add(addr_type, addr);
    if (device_index != 0xFF) {
        tlk_printf("discovery uc found:[%d] addr_type: %d, addr: %s, RSSI: %d, device_name: %s", device_index, addr_type, addr_to_str(addr), rssi, device_name);
    }

    if (s_uc_device_info.state == APP_UC_DEVICE_STATE_DISCOVERY_US) {
        if (rssi > s_uc_device_info.min_rssi_value) {
            tlk_printf("create acl connect, addr_type: %d, addr: %s, device name:%s", addr_type, addr_to_str(addr), device_name);
            int ret = app_create_acl_connect(addr_type, addr);
            if (ret == BLE_HOST_ERR_SUCC) {
                s_uc_device_info.state = APP_UC_DEVICE_STATE_CREATE_ACL_US;
            } else {
                app_extend_scan_ctrl_start();
            }
        }
    } else if (s_uc_device_info.state == APP_UC_DEVICE_STATE_WAIT_OTHER_US) {
        tlk_printf("create acl connect, addr_type: %d, addr: %s, device name:%s", addr_type, addr_to_str(addr), device_name);
        int ret = app_create_acl_connect(addr_type, addr);
        if (ret == BLE_HOST_ERR_SUCC) {
            s_uc_device_info.state = APP_UC_DEVICE_STATE_CREATE_ACL_US;
        } else {
            app_extend_scan_ctrl_start();
        }
    }
}

static void app_uc_connected_callback(struct ble_host_conn *conn)
{
    tlk_printf("[APP] connected callback, ACL handle: 0x%03x", conn->conn_handle);
    int ret = ble_host_smp_start_pairing(conn->conn_handle);
    tlk_printf("[APP] start pairing, ret: 0x%x", ret);
    ret = ble_host_gattc_send_exchange_mtu_req(conn->conn_handle, 65, NULL);
    tlk_printf("[APP] send exchange mtu req, ret: 0x%x", ret);
}

static void app_uc_disconnected_callback(struct ble_host_conn *conn, uint8_t reason)
{
    tlk_printf("[APP] disconnected callback, ACL handle: 0x%03x, reason: 0x%02x", conn->conn_handle, reason);
    app_extend_scan_ctrl_start();
    for (int i = 0; i < s_ui_store_us_info.size; i++) {
        if (s_ui_store_us_info.device_info[i].acl_conn_handle == conn->conn_handle) {
            s_ui_store_us_info.device_info[i].acl_conn_handle = 0xFFFF;
            s_ui_store_us_info.curr_size--;
            break;
        }
    }

    s_uc_device_info.state = APP_UC_DEVICE_STATE_WAIT_OTHER_US;

    if (s_ui_store_us_info.curr_size == 0) {
        app_extend_scan_ctrl_start();
        app_uc_prf_clear_discovery_filter();
        s_uc_device_info.state = APP_UC_DEVICE_STATE_DISCOVERY_US;
        ble_host_gap_cis_remove_central(0);
    }
}

/**
 * @brief  Callback when entering stream state.
 *          Configure codec and data path based on function mode.
 *
 * @param[in] conn_handle  Connection handle.
 * @param[in] result      Stream entry result.
 */
static void app_ui_enter_stream_callback(uint16_t conn_handle, enum lea_uc_result result)
{
    if (result == LEA_UC_RESULT_SUCCESS) {
        // enable datapath.
        struct ble_uc_stream_config *p_stream_config = NULL;
        uint16_t                     cis_conn_handle = 0;
        if (s_ui_store_us_info.device_info[0].acl_conn_handle == conn_handle) {
            cis_conn_handle = s_ui_store_us_info.device_info[0].cis_conn_handle;
            p_stream_config = &s_ui_stream_config[0];
        } else {
            cis_conn_handle = s_ui_store_us_info.device_info[1].cis_conn_handle;
            p_stream_config = &s_ui_stream_config[1];
        }

#if APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MUSIC_PLAY
        struct lea_codec_spec_config_param *p_codec = &p_stream_config->ase_info[0].codec_cfg;
        struct lea_config                   config  = {
                               .blocks            = p_codec->per_sdu_frame_blocks,
                               .location          = p_codec->channel_allocation,
                               .samplingFrequency = p_codec->sampling_freq,
                               .frameDuration     = p_codec->frame_duration,
                               .frameOctets       = p_codec->per_codec_frame,
                               .iso_handle        = cis_conn_handle,
        };
        lea_set_input_config(&config);
        ble_host_gap_iso_remove_data_path_c2h(cis_conn_handle);
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MIC_ONLY
        ble_host_gap_iso_remove_data_path_c2h(cis_conn_handle);
        struct lea_codec_spec_config_param *p_output_codec = &p_stream_config->ase_info[0].codec_cfg;
        struct lea_config                   output_config  = {
                               .blocks            = p_output_codec->per_sdu_frame_blocks,
                               .location          = p_output_codec->channel_allocation,
                               .samplingFrequency = p_output_codec->sampling_freq,
                               .frameDuration     = p_output_codec->frame_duration,
                               .frameOctets       = p_output_codec->per_codec_frame,
                               .iso_handle        = cis_conn_handle,
                               .presentationDelay = 50 * 1000 + 20 * 1000,
        };
        lea_set_output_config(&output_config);
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_VOICE_CALL
        ble_host_gap_iso_remove_data_path_h2c(cis_conn_handle);
        ble_host_gap_iso_remove_data_path_c2h(cis_conn_handle);
        struct lea_codec_spec_config_param *p_codec = &p_stream_config->ase_info[0].codec_cfg;
        struct lea_config                   config  = {
                               .blocks            = p_codec->per_sdu_frame_blocks,
                               .location          = p_codec->channel_allocation,
                               .samplingFrequency = p_codec->sampling_freq,
                               .frameDuration     = p_codec->frame_duration,
                               .frameOctets       = p_codec->per_codec_frame,
                               .iso_handle        = cis_conn_handle,
        };
        lea_set_input_config(&config);
        struct lea_codec_spec_config_param *p_output_codec = &p_stream_config->ase_info[1].codec_cfg;
        struct lea_config                   output_config  = {
                               .blocks            = p_output_codec->per_sdu_frame_blocks,
                               .location          = p_output_codec->channel_allocation,
                               .samplingFrequency = p_output_codec->sampling_freq,
                               .frameDuration     = p_output_codec->frame_duration,
                               .frameOctets       = p_output_codec->per_codec_frame,
                               .iso_handle        = cis_conn_handle,
                               .presentationDelay = 50 * 1000 + 20 * 1000,
        };
        lea_set_output_config(&output_config);
#endif

    } else {
        BLE_HOST_SAL_LOG_ERROR("enter stream failed, result: %d", result);
        ble_host_gap_disconnect_acl(conn_handle);
    }
}

/**
 * @brief  CIS event callback.
 *          Handle CIS established, failed and terminated events.
 *
 * @param[in] acl_conn_handle  ACL connection handle.
 * @param[in] cis_conn_handle  CIS connection handle.
 * @param[in] event            CIS event type.
 * @param[in] event_param      Event parameter.
 */
static void app_ui_cis_event_callback(uint16_t acl_conn_handle, uint16_t cis_conn_handle, enum ble_host_gap_cis_event event, const void *event_param)
{
    (void)event_param;
    tlk_printf("cis event: %d, acl_conn_handle: 0x%03x, cis_conn_handle: 0x%03x", event, acl_conn_handle, cis_conn_handle);
    if (event == BLE_HOST_GAP_CIS_EVT_ID_ESTABLISHED) {
        uint8_t rank = 1;
        ble_csisc_get_set_member_rank(acl_conn_handle, &rank);
        struct ble_uc_stream_config *p_stream_config = &s_ui_stream_config[rank - 1];

        struct ble_uc_enter_stream enter_stream = {
            .conn_handle = acl_conn_handle,
            .ase_num     = p_stream_config->ase_num,
            .callback    = app_ui_enter_stream_callback,
        };

        for (size_t i = 0; i < p_stream_config->ase_num; i++) {
            enter_stream.ase_id[i] = p_stream_config->ase_info[i].ase_id;
        }

        int ret = ble_uc_enter_stream(&enter_stream);
        tlk_printf("enter stream, ret: %d", ret);

#if APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MUSIC_PLAY
        ble_host_gap_iso_set_data_path_h2c(cis_conn_handle);
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MIC_ONLY
        ble_host_gap_iso_set_data_path_c2h(cis_conn_handle);
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_VOICE_CALL
        ble_host_gap_iso_set_data_path_h2c(cis_conn_handle);
        ble_host_gap_iso_set_data_path_c2h(cis_conn_handle);
#endif
    } else if (event == BLE_HOST_GAP_CIS_EVT_ID_ESTABLISHED_FAILED) {
        ble_host_gap_disconnect_acl(acl_conn_handle);
    } else if (event == BLE_HOST_GAP_CIS_EVT_ID_TERMINATED) {
#if APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MUSIC_PLAY
        ble_host_gap_iso_remove_data_path_h2c(cis_conn_handle);
        lea_release_input_config(cis_conn_handle);
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MIC_ONLY
        ble_host_gap_iso_remove_data_path_c2h(cis_conn_handle);
        lea_release_output_config(cis_conn_handle);
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_VOICE_CALL
        ble_host_gap_iso_remove_data_path_h2c(cis_conn_handle);
        ble_host_gap_iso_remove_data_path_c2h(cis_conn_handle);
        lea_release_input_config(cis_conn_handle);
        lea_release_output_config(cis_conn_handle);
#endif
    }
}

/**
 * @brief  Callback when starting stream.
 *          Configure CIG parameters and create CIS connection.
 *
 * @param[in] p_stream_config  Stream configuration.
 * @param[in] result           Stream start result.
 */
static void app_ui_start_stream_callback(struct ble_uc_stream_config *p_stream_config, enum lea_uc_result result)
{
    if (result == LEA_UC_RESULT_SUCCESS) {
        struct ble_ascsc_ase_codec_param param = {0};

        for (int i = 0; i < p_stream_config->ase_num; i++) {
            ble_ascsc_get_ase_codec_param(p_stream_config->conn_handle, p_stream_config->ase_info[i].ase_id, &param);
        }

        // set cig parameter value.
        uint32_t sdu_interval = p_stream_config->ase_info[0].codec_cfg.frame_duration == LEA_SELECT_FRAME_DURATION_7_5 ? 7500 : 10000;

        struct ble_host_gap_cis_central_set_cig_params set_cig_params = {
            .cig_id         = 0,
            .worst_case_sca = BLE_HOST_GAP_CIG_SCA_PPM_101_150,
            .sequential     = false,
            .framing        = false,
            .c_to_p =
                {
                    .sdu_interval          = sdu_interval,
                    .max_transport_latency = param.c_to_p.max_transport_latency,
                },
            .p_to_c =
                {
                    .sdu_interval          = sdu_interval,
                    .max_transport_latency = param.p_to_c.max_transport_latency,
                },
            .cis_count      = s_ui_store_us_info.size,
            .event_callback = NULL,
        };

        for (int i = 0; i < set_cig_params.cis_count; i++) {
            set_cig_params.cis_param[i].cis_id = i;
#if APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MUSIC_PLAY
            // if music clear p to c value.
            set_cig_params.cis_param[i].cis_p_to_c.max_sdu = 0;
            set_cig_params.cis_param[i].cis_p_to_c.phy     = 0x02;
            set_cig_params.cis_param[i].cis_p_to_c.rtn     = 0x00;
#else
            set_cig_params.cis_param[i].cis_p_to_c.max_sdu = param.p_to_c.max_sdu;
            set_cig_params.cis_param[i].cis_p_to_c.phy     = param.p_to_c.preferred_phy;
            set_cig_params.cis_param[i].cis_p_to_c.rtn     = param.p_to_c.retransmission_number;
#endif
#if APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MIC_ONLY
            // if mic clear c to p value.
            set_cig_params.cis_param[i].cis_c_to_p.max_sdu = 0;
            set_cig_params.cis_param[i].cis_c_to_p.phy     = 0x02;
            set_cig_params.cis_param[i].cis_c_to_p.rtn     = 0x00;
#else
            set_cig_params.cis_param[i].cis_c_to_p.max_sdu = param.c_to_p.max_sdu;
            set_cig_params.cis_param[i].cis_c_to_p.phy     = param.c_to_p.preferred_phy;
            set_cig_params.cis_param[i].cis_c_to_p.rtn     = param.c_to_p.retransmission_number;
#endif
        }

        uint16_t cis_conn_handles[2] = {0xFFFF, 0xFFFF};
        int      ret                 = ble_host_gap_cis_central_set_cig_param(&set_cig_params, cis_conn_handles);
        if (ret == BLE_HOST_ERR_SUCC) {
            s_ui_store_us_info.device_info[0].cis_conn_handle = cis_conn_handles[0];
            s_ui_store_us_info.device_info[1].cis_conn_handle = cis_conn_handles[1];
        }

        uint16_t cis_conn_handle = 0xFFFF;
        if (s_ui_store_us_info.device_info[0].acl_conn_handle == p_stream_config->conn_handle) {
            cis_conn_handle = s_ui_store_us_info.device_info[0].cis_conn_handle;
        } else if (s_ui_store_us_info.device_info[1].acl_conn_handle == p_stream_config->conn_handle) {
            cis_conn_handle = s_ui_store_us_info.device_info[1].cis_conn_handle;
        }

        ret = ble_host_gap_cis_central_create_connection(p_stream_config->conn_handle, cis_conn_handle, app_ui_cis_event_callback);
        tlk_printf("create cis connection, ret = 0x%0x, ACL handle = 0x%03x, cis handle = 0x%03x", ret, p_stream_config->conn_handle, cis_conn_handle);
    } else {
        // enter stream failed, disconnect.
        ble_host_gap_disconnect_acl(p_stream_config->conn_handle);
    }
}

static void app_sdp_finish_callback(uint16_t conn_handle, struct ble_app_sdp_flags *flags)
{
    tlk_printf("finish callback is %x %s", conn_handle, hex_to_str(flags, sizeof(struct ble_app_sdp_flags)));
    if (flags->ascs_client_flags == 1) {
        uint8_t size = 1;
        ble_csisc_get_coordinated_set_size(conn_handle, &size);
        if (size == 1) {
            s_ui_store_us_info.size                           = 1;
            s_ui_store_us_info.device_info[0].acl_conn_handle = conn_handle;
            s_ui_store_us_info.curr_size                      = 1;
            tlk_printf("coordinated set size is 1");
        } else {
            s_ui_store_us_info.size = size;
            uint8_t rank            = 0;
            ble_csisc_get_set_member_rank(conn_handle, &rank);

            if (rank > 2 || rank == 0 || s_ui_store_us_info.device_info[rank - 1].acl_conn_handle != 0xFFFF) {
                ble_host_gap_disconnect_acl(conn_handle); /* < if set member rank is invalid, disconnect. */
                return;
            }

            s_ui_store_us_info.device_info[rank - 1].acl_conn_handle = conn_handle;
            s_ui_store_us_info.curr_size++;

            tlk_printf("coordinated set size is %d %d %d", s_ui_store_us_info.curr_size, s_ui_store_us_info.size, s_ui_store_us_info.size <= s_ui_store_us_info.curr_size);
            if (s_ui_store_us_info.size > s_ui_store_us_info.curr_size) {
                uint8_t sirk[16];
                ble_csisc_get_resolving_key_plain(conn_handle, sirk);
                app_uc_prf_set_discovery_filter(sirk);
                s_uc_device_info.state = APP_UC_DEVICE_STATE_WAIT_OTHER_US;
                app_extend_scan_ctrl_start();
            } else {
                app_extend_scan_ctrl_stop();
            }
            tlk_printf("coordinated set size is %d", size);
        }

        // initialize stream config.
        uint8_t rank = 1;
        ble_csisc_get_set_member_rank(conn_handle, &rank);

        struct ble_uc_stream_config *p_stream_config = &s_ui_stream_config[rank - 1];
#if APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MUSIC_PLAY
        p_stream_config->conn_handle = conn_handle;
        p_stream_config->ase_num     = 1;
        uint8_t ase_id               = 0x00;
        ble_ascsc_get_sink_ase_id(conn_handle, 1, &ase_id);
        p_stream_config->ase_info[0].ase_id  = ase_id;
        p_stream_config->ase_info[0].latency = ASCS_TARGET_LOW_LATENCY;
        uint32_t sink_locations              = 0;
        ble_pacsc_get_sink_audio_locations(conn_handle, &sink_locations);
        p_stream_config->ase_info[0].codec_cfg.channel_allocation = sink_locations;
        int ret                                                   = ble_uc_start_stream(p_stream_config);
        tlk_printf("uc start stream[0x%03x] ret is 0x%x", conn_handle, ret);
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_MIC_ONLY
        p_stream_config->conn_handle = conn_handle;
        p_stream_config->ase_num     = 1;
        uint8_t src_ase_id           = 0x00;
        ble_ascsc_get_source_ase_id(conn_handle, 1, &src_ase_id);
        p_stream_config->ase_info[0].ase_id  = src_ase_id;
        p_stream_config->ase_info[0].latency = ASCS_TARGET_HIGH_RELIABILITY;
        uint32_t source_locations            = 0;
        ble_pacsc_get_source_audio_locations(conn_handle, &source_locations);
        p_stream_config->ase_info[0].codec_cfg.channel_allocation = source_locations;
        int ret                                                   = ble_uc_start_stream(p_stream_config);
        tlk_printf("uc start stream[0x%03x] ret is 0x%x", conn_handle, ret);
#elif APP_UC_FUNCTION_SELECT_ITEM == APP_UC_FUNCTION_VOICE_CALL
        p_stream_config->conn_handle = conn_handle;
        p_stream_config->ase_num     = 2;
        uint8_t ase_id               = 0x00;
        ble_ascsc_get_sink_ase_id(conn_handle, 1, &ase_id);
        p_stream_config->ase_info[0].ase_id  = ase_id;
        p_stream_config->ase_info[0].latency = ASCS_TARGET_HIGH_RELIABILITY;
        uint32_t sink_locations              = 0;
        ble_pacsc_get_sink_audio_locations(conn_handle, &sink_locations);
        p_stream_config->ase_info[0].codec_cfg.channel_allocation = sink_locations;
        uint8_t src_ase_id                                        = 0x00;
        ble_ascsc_get_source_ase_id(conn_handle, 1, &src_ase_id);
        p_stream_config->ase_info[1].ase_id  = src_ase_id;
        p_stream_config->ase_info[1].latency = ASCS_TARGET_HIGH_RELIABILITY;
        uint32_t source_locations            = 0;
        ble_pacsc_get_source_audio_locations(conn_handle, &source_locations);
        p_stream_config->ase_info[1].codec_cfg.channel_allocation = source_locations;
        int ret                                                   = ble_uc_start_stream(p_stream_config);
        tlk_printf("uc start stream[0x%03x] ret is 0x%x", conn_handle, ret);
#endif
    }
}
