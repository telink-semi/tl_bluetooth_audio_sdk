/********************************************************************************************************
 * @file    lea_broadcast_sync.c
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
#include "stack/ble/ble_common.h"
#include "stack/ble/ble_ad_type.h"
#include "stack/ble/host_v1/inc/ble_host.h"
#include "stack/ble/host_v1/gap/adv/inc/ble_gap_ext_adv.h"
#include "stack/ble/host_v1/gap/acl/inc/ble_gap_acl.h"
#include "stack/ble/host_v1/gap/iso/inc/ble_gap_cis_peripheral.h"
#include "stack/ble/host_v1/misc/inc/ble_rand.h"

#include "stack/ble/host_v1/l2cap/att/inc/ble_att_service.h"
#include "stack/ble/host_v1/l2cap/att/inc/uuid16bit.h"
#include "stack/ble/host_v1/l2cap/smp/inc/ble_smp.h"

#include "stack/ble/host_v1/services/svc_gatt/dis/svc_dis.h"

#include "stack/ble/host_v1/profile/le_audio/common/inc/generic_audio.h"
#include "stack/ble/host_v1/profile/le_audio/render_cap/vcs/inc/vcs.h"
#include "stack/ble/host_v1/profile/le_audio/render_cap/vcs/inc/vcss.h"
#include "stack/ble/host_v1/profile/le_audio/stream/pacs/inc/pacss.h"
#include "stack/ble/host_v1/profile/le_audio/stream/ascs/inc/ascs.h"
#include "stack/ble/host_v1/profile/le_audio/stream/ascs/inc/ascss.h"
#include "stack/ble/host_v1/profile/le_audio/stream/inc/bap.h"
#include "stack/ble/host_v1/profile/le_audio/stream/inc/bap_us.h"
#include "stack/ble/host_v1/profile/le_audio/trans_coord/cap/inc/cap.h"
#include "stack/ble/host_v1/profile/le_audio/content/gmcs/inc/gmcsc.h"
#include "stack/ble/host_v1/profile/le_audio/content/gtbs/inc/gtbsc.h"
#include "stack/ble/host_v1/profile/le_audio/user_case/tmas/inc/tmas.h"
#include "stack/ble/host_v1/profile/le_audio/user_case/tmas/inc/tmass.h"
#include "stack/ble/host_v1/profile/le_audio/stream/inc/bap_sink.h"

#include "stack/ble/host_v1/l2cap/att/inc/ble_att.h"
#include "stack/ble/host_v1/gap/iso/inc/ble_gap_big.h"
#include "stack/ble/host_v1/gap/pa/inc/ble_gap_pa_sync.h"
#include "stack/ble/host_v1/gap/scan/inc/ble_gap_scan.h"
#include "stack/ble/host_v1/gap/scan/inc/ble_gap_ext_scan.h"
#include "stack/ble/host_v1/gap/iso/inc/ble_gap_big_sync.h"
#include "stack/ble/host_v1/profile/basic_profile/bas/inc/bass.h"
#include "stack/ble/host_v1/profile/le_audio/stream/bass/inc/basss.h"

#include "tlkapi/tlkapi_debug.h"
#include "stack/ble/controller/ll/iso/iso.h"
#include "inc/lea_cfg.h"
#include "inc/lea_broadcast_sync.h"


#include "tlkmw/audio/le_audio/le_audio_common.h"
#include "tlkmw/audio/le_audio/tlkmdi_lea_bmr.h"
#include "stack/ble/host_v1/misc/inc/ble_misc.h"
#include "stack/ble/host_v1/gap/evt_dispatch/inc/ble_gap_evt_dispatch.h"

#define GAP_APPEARANCE_HEADSET       0x0942 // Headset
#define FILTER_SOURCE_BROADCAST_NAME "tlk_lea_broadcast_source"

enum ble_bis_sync_state
{
    BIS_SYNC_STATE_IDLE,
    BIS_SYNC_STATE_SYNC_PA,
    BIS_SYNC_STATE_SYNCED_PA,
    BIS_SYNC_STATE_SYNC_BIG,
    BIS_SYNC_STATE_SYNCED_BIG,
};

enum bis_sync_pa_state
{
    APP_PA_SYNC_STATE_IDLE,
    APP_PA_SYNC_STATE_SYNCING,
    APP_PA_SYNC_STATE_SYNCED,
    APP_PA_SYNC_STATE_LOST,
};

enum ble_bis_sync_bis_state
{
    APP_BIG_SYNC_STATE_IDLE,
    APP_BIG_SYNC_STATE_SYNCING,
    APP_BIG_SYNC_STATE_SYNCED,
    APP_BIG_SYNC_STATE_LOST,
};

struct ble_bis_sync_param
{
    uint8_t  state;
    uint8_t  pa_sync_state;
    uint16_t pa_sync_handle;
    uint8_t  big_sync_handle;
    uint8_t  big_sync_state;
};

static struct ble_bis_sync_param s_bis_sync_param = {
    .state          = BIS_SYNC_STATE_IDLE,
    .pa_sync_state  = APP_PA_SYNC_STATE_IDLE,
    .pa_sync_handle = 0,
    .big_sync_state = APP_BIG_SYNC_STATE_IDLE,
};

static char s_lc3_config[8] = {0};

static const struct ble_host_gap_pa_sync_callbacks s_periodic_adv_sync;
static void                                        ble_big_sync_create(void);

/**
 * @brief       Initialize broadcast sync state (placeholder for now).
 * @return      none.
 */
void lea_broadcast_sync_init(void) {}

/*************** extend advertising report device manager ***************/

struct ble_sink_ext_adv_info
{
    uint8_t broadcast_name_len;
    uint8_t broadcast_name[32];
    bool    vendor_flag;
    char    lc3_config[8];
};

/**
 * @brief       Parse extended advertising data to extract broadcast info.
 * @param[in]   length  - AD structure length.
 * @param[in]   type    - AD structure type.
 * @param[in]   value   - AD payload pointer.
 * @param[out]  user    - sink info accumulator.
 * @return      1 to continue parsing.
 */
static int ble_sink_get_ext_adv_info(uint8_t length, uint8_t type, const uint8_t *value, void *user)
{
    struct ble_sink_ext_adv_info *p_info = user;
    if (type == DT_BROADCAST_NAME) {
        p_info->broadcast_name_len = min(length - 1, 32);
        memcpy(p_info->broadcast_name, value, p_info->broadcast_name_len);
    } else if (type == DT_MANUFACTURER_SPECIFIC_DATA) {
        uint16_t company_id;
        STREAM_TO_U16(company_id, value);
        if (company_id == 0x211) {
            p_info->vendor_flag = true;
        }
    }

    return 1;
}

/**
 * @brief       Handle extended scan events to start PA sync with target source.
 * @param[in]   event_id    - scan event type.
 * @param[in]   data        - event payload pointer.
 * @param[in]   user_data   - unused.
 * @return      none.
 */
static void ble_sink_ext_scan_evt_handler(uint32_t event_id, const void *data, void *user_data)
{
    (void)user_data;
    if (event_id == BLE_HOST_GAP_EVT_LE_SCAN_REPORT_UNDIRECTED) {
        const struct ble_host_gap_evt_scan_adv_report_undirected *adv_report = data;

        if (s_bis_sync_param.pa_sync_state != APP_PA_SYNC_STATE_IDLE) {
            return;
        }

        if (adv_report->pa_interval == 0x0000) {
            // BIG must advertising in extended advertising and had PA interval,
            return;
        }

        struct ble_sink_ext_adv_info s_ext_adv_info = {
            .broadcast_name_len = 0,
            .broadcast_name[0]  = '\0',
            .vendor_flag        = 0,
        };

        int adv_pack = ltv_unpack(adv_report->data, adv_report->data_len, ble_sink_get_ext_adv_info, &s_ext_adv_info);

        if (adv_pack != LTV_UNPACK_SUCCESS || s_ext_adv_info.vendor_flag != true) {
            return;
        }

        if (memcmp(s_ext_adv_info.broadcast_name, FILTER_SOURCE_BROADCAST_NAME, sizeof(FILTER_SOURCE_BROADCAST_NAME)) != 0) {
            return;
        }

        tlkapi_printf(APP_LOG_EN, "[APP]Scanned extended advertising report, start to sync to PA");
        // maybe add timer to create PA sync failed.
        int ret;

        ret = ble_host_gap_pa_sync_create(adv_report->sid, adv_report->addr_type, adv_report->addr, 5000, &s_periodic_adv_sync);

        if (ret == BLE_HOST_ERR_SUCC) {
            s_bis_sync_param.pa_sync_state = APP_PA_SYNC_STATE_SYNCING;
            memcpy(s_lc3_config, s_ext_adv_info.lc3_config, 8);
        }
    }
}

static const struct ble_host_gap_evt_subscribe_param s_sink_ext_scan_evt_param = {
    .gap_le_scan_mask = 1,
    .handler          = ble_sink_ext_scan_evt_handler,
};

static struct ble_host_gap_evt_subscribe s_sink_ext_scan_evt = {
    .next_handler = NULL,
    .param        = &s_sink_ext_scan_evt_param,
};

/*************** extend advertising report device manager ending ***************/


/*************** periodic advertising sync ***************/
/**
 * @brief       Called when periodic advertising sync succeeds; stops scanning.
 * @param[in]   status  - BLE status code.
 * @param[in]   param   - PA sync handle info.
 * @return      none.
 */
static void ble_periodic_adv_sync_established_callback(uint8_t status, struct ble_host_gap_pa_sync_param *param)
{
    (void)param;
    if (status == BLE_SUCCESS) {
        ble_host_gap_evt_remove_subscriber(&s_sink_ext_scan_evt);
        ble_host_gap_ext_scan_disable();
        s_bis_sync_param.pa_sync_handle = param->sync_handle;
        s_bis_sync_param.pa_sync_state  = APP_PA_SYNC_STATE_SYNCED;
        ble_big_sync_create();
    } else {
        s_bis_sync_param.pa_sync_state = APP_PA_SYNC_STATE_IDLE;
    }
    tlkapi_printf(APP_LOG_EN, "[APP]Periodic Advertising Sync established, status is %02x", status);
}

/**
 * @brief       Called when periodic advertising sync is lost.
 * @param[in]   sync_handle - handle of the lost PA sync.
 * @return      none.
 */
static void ble_periodic_adv_sync_lost_callback(uint16_t sync_handle)
{
    (void)sync_handle;
    if (s_bis_sync_param.pa_sync_state == APP_PA_SYNC_STATE_SYNCED) {
        s_bis_sync_param.pa_sync_state = APP_PA_SYNC_STATE_LOST;
    }
    tlkapi_printf(APP_LOG_EN, "[APP][ERR]Periodic Advertising Sync lost");
}

static const struct ble_host_gap_pa_sync_callbacks s_periodic_adv_sync = {
    .established_cb = ble_periodic_adv_sync_established_callback,
    .lost_cb        = ble_periodic_adv_sync_lost_callback,
    .report_adv_cb  = NULL,
    .past_cb        = NULL,
};

/*************** periodic advertising sync ending ***************/

/*************** BIG state machine ***************/

static struct lea_bmr_stream_param s_bis_sync_stream = {
    .frequency          = LEA_SELECT_SAMPLING_FREQ_48000_HZ,
    .frame_duration     = LEA_SELECT_FRAME_DURATION_10,
    .frame_octets       = 120,
    .all_location       = LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT,
    .presentation_delay = 15000, // default use 15ms
    .num                = 2,
    .bmr_config[0] =
        {
            .blocks   = 1,
            .location = LEA_LOCATION_FRONT_LEFT,
        },
    .bmr_config[1] =
        {
            .blocks   = 1,
            .location = LEA_LOCATION_FRONT_RIGHT,
        },
};

/**
 * @brief       BIG sync established callback configures BIS stream parameters.
 * @param[in]   status  - BLE status result.
 * @param[in]   param   - BIG sync parameter pointer.
 * @return      none.
 */
static void ble_big_sync_established_callback(uint8_t status, struct ble_host_gap_big_sync_param *param)
{
    (void)param;
    tlkapi_printf(APP_LOG_EN, "BIG sync established, status is 0x%02x", status);
    if (status == BLE_SUCCESS) {
        s_bis_sync_param.big_sync_state = APP_BIG_SYNC_STATE_SYNCED;

        if (memcmp(s_lc3_config, "LC3_48_4", 8) == 0) {
            s_bis_sync_stream.frequency      = LEA_SELECT_SAMPLING_FREQ_48000_HZ;
            s_bis_sync_stream.frame_duration = LEA_SELECT_FRAME_DURATION_10;
            s_bis_sync_stream.frame_octets   = 120;
        } else if (memcmp(s_lc3_config, "LC3_48_2", 8) == 0) {
            s_bis_sync_stream.frequency      = LEA_SELECT_SAMPLING_FREQ_48000_HZ;
            s_bis_sync_stream.frame_duration = LEA_SELECT_FRAME_DURATION_10;
            s_bis_sync_stream.frame_octets   = 100;
        } else if (memcmp(s_lc3_config, "LC3_32_2", 8) == 0) {
            s_bis_sync_stream.frequency      = LEA_SELECT_SAMPLING_FREQ_32000_HZ;
            s_bis_sync_stream.frame_duration = LEA_SELECT_FRAME_DURATION_10;
            s_bis_sync_stream.frame_octets   = 80;
        } else if (memcmp(s_lc3_config, "LC3_24_2", 8) == 0) {
            s_bis_sync_stream.frequency      = LEA_SELECT_SAMPLING_FREQ_24000_HZ;
            s_bis_sync_stream.frame_duration = LEA_SELECT_FRAME_DURATION_10;
            s_bis_sync_stream.frame_octets   = 60;
        } else if (memcmp(s_lc3_config, "LC3_16_2", 8) == 0) {
            s_bis_sync_stream.frequency      = LEA_SELECT_SAMPLING_FREQ_16000_HZ;
            s_bis_sync_stream.frame_duration = LEA_SELECT_FRAME_DURATION_10;
            s_bis_sync_stream.frame_octets   = 40;
        }
        tlk_printf("LC3 config: %s\n", s_lc3_config);
        tlk_printf("freq is %d %d %d", s_bis_sync_stream.frame_duration, s_bis_sync_stream.frequency, s_bis_sync_stream.frame_octets);
        s_bis_sync_stream.bmr_config[0].iso_handle = param->iso_handles[0];
        s_bis_sync_stream.bmr_config[1].iso_handle = param->iso_handles[1];
        tlkmdi_lea_bmr_set_stream_param(&s_bis_sync_stream);
        tlkmdi_lea_bmr_start_stream();
    } else {
        s_bis_sync_param.big_sync_state = APP_BIG_SYNC_STATE_IDLE;
    }
}

/**
 * @brief       BIG sync lost callback stops BMR playback and updates state.
 * @param[in]   big_handle  - BIG handle.
 * @param[in]   reason      - reason code.
 * @return      none.
 */
static void ble_big_sync_lost_callback(uint8_t big_handle, uint8_t reason)
{
    tlkapi_printf(APP_LOG_EN, "big lost, handle: 0x%02x, reason: 0x%02x", big_handle, reason);

    tlkmdi_lea_bmr_release_stream_param();
    tlkmdi_lea_bmr_stop_stream();
    s_bis_sync_param.big_sync_state = APP_BIG_SYNC_STATE_LOST;
}

static const struct ble_host_gap_big_sync_callbacks s_big_sync = {
    .established_cb = ble_big_sync_established_callback,
    .lost_cb        = ble_big_sync_lost_callback,
};

/**
 * @brief       Request BIG synchronization based on current PA sync handle.
 * @return      none.
 */
static void ble_big_sync_create(void)
{
    if (s_bis_sync_param.big_sync_state == APP_BIG_SYNC_STATE_IDLE) {
        s_bis_sync_param.big_sync_state                           = APP_BIG_SYNC_STATE_SYNCING;
        struct ble_host_gap_big_sync_create_param big_sync_create = {
            .big_handle  = s_bis_sync_param.big_sync_handle,
            .sync_handle = s_bis_sync_param.pa_sync_handle,
            .enc         = 0,
            .num_bis     = 2,
            .bis_index   = {1, 2},
        };
#ifdef BROADCAST_CODE
        big_sync_create.enc = 1;
        memcpy(big_sync_create.broadcast_code, BROADCAST_CODE, sizeof(BROADCAST_CODE) - 1);
#endif
        ble_host_gap_big_sync_create(&big_sync_create, &s_big_sync);
    }
}

/*************** BIG state machine ending ***************/

/**
 * @brief       Start scanning for broadcast sources and attempt PA/BIG sync.
 * @return      none.
 */
void lea_broadcast_sync_start(void)
{
    tlkapi_printf(APP_LOG_EN, "%s", __func__);

    s_bis_sync_param.big_sync_handle = ble_host_gap_big_sync_create_big_handle();

    ble_host_gap_evt_add_subscriber(&s_sink_ext_scan_evt);
    ble_host_gap_ext_scan_set_param(BLE_HOST_GAP_EXT_SCAN_PARAM_BASIC_UNFILTERED, BLE_HOST_GAP_PASSIVE_SCAN(100, 100));
    ble_host_gap_ext_scan_enable();
}

/**
 * @brief       Stop scanning/syncing and release all broadcast sink resources.
 * @return      none.
 */
void lea_broadcast_sync_stop(void)
{
    tlkapi_printf(APP_LOG_EN, "%s", __func__);
    ble_host_gap_ext_scan_disable();
    ble_host_gap_evt_remove_subscriber(&s_sink_ext_scan_evt);
    if (s_bis_sync_param.pa_sync_state == APP_PA_SYNC_STATE_SYNCED) {
        ble_host_gap_pa_sync_terminate(s_bis_sync_param.pa_sync_handle);

    } else if (s_bis_sync_param.pa_sync_state == APP_PA_SYNC_STATE_SYNCING) {
        ble_host_gap_pa_sync_cancel();
    }
    s_bis_sync_param.pa_sync_state = APP_PA_SYNC_STATE_IDLE;

    if (s_bis_sync_param.big_sync_state == APP_BIG_SYNC_STATE_SYNCED) {
        ble_host_gap_big_sync_terminate(s_bis_sync_param.big_sync_handle);
    }
    ble_host_gap_big_sync_delete_big_handle(s_bis_sync_param.big_sync_handle);
    s_bis_sync_param.big_sync_state = APP_BIG_SYNC_STATE_IDLE;

    tlkmdi_lea_bmr_release_stream_param();
    tlkmdi_lea_bmr_stop_stream();
}
