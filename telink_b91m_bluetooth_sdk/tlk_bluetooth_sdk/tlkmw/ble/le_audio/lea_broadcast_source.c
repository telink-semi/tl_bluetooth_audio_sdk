/********************************************************************************************************
 * @file    lea_broadcast_source.c
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


#include "tlkmw/sys_dev/codec/tlkdrv_codec.h"
#include "tlkmw/audio/le_audio/le_audio_common.h"

#include "tlkmw/audio/a2dp_to_bis/tlkmdi_a2dp_to_bis.h"
#include "tlkmw/audio/a2dp_to_bis/tlkmdi_a2dp_to_bis_le_music.h"

#include "inc/lea_broadcast_source.h"
#include "tlkmw/audio/le_audio/tlkmdi_lea_bms.h"

#define EXT_ADV_SID               8

#define EXT_ADV_INTERVAL          (60) //60ms  or 120ms

#define BIS_SOURCE_COMPLETE_NAME  "A2DP to BIS Demo"

#define SOURCE_PRESENTATION_DELAY 40000 //20ms    presentation delay, unit is 1us.
#define SOURCE_TRANSMIT_LATENCY   2     // bis transmit delay, unit is sdu interval(7.5ms or 10ms).
#define SOURCE_ISO_INTERVAL       2     // bis iso interval, unit is sdu interval(7.5ms or 10ms).
#define SOURCE_RETRANSMIT_NUMBER  3     // streaming packet retry count, usually 3 or 4, current use 3.

// BASE control
#if (BIS_STREAM_CONFIG == BIS_STREAM_48kHz_96kbps)
#define AUDIO_PARAM_LC3_CFG LEA_CODEC_CFG_LC3_48_4
#define LC3_CONFIG          "LC3_48_4"
#elif (BIS_STREAM_CONFIG == BIS_STREAM_48kHz_80kbps)
#define AUDIO_PARAM_LC3_CFG LEA_CODEC_CFG_LC3_48_2
#define LC3_CONFIG          "LC3_48_2"
#elif (BIS_STREAM_CONFIG == BIS_STREAM_32KHz_64Kbps)
#define AUDIO_PARAM_LC3_CFG LEA_CODEC_CFG_LC3_32_2
#define LC3_CONFIG          "LC3_32_2"
#elif (BIS_STREAM_CONFIG == BIS_STREAM_24KHz_48Kbps)
#define AUDIO_PARAM_LC3_CFG LEA_CODEC_CFG_LC3_24_2
#define LC3_CONFIG          "LC3_24_2"
#elif (BIS_STREAM_CONFIG == BIS_STREAM_16KHz_32Kbps)
#define AUDIO_PARAM_LC3_CFG LEA_CODEC_CFG_LC3_32_2
#define LC3_CONFIG          "LC3_16_2"
#else
#error "not support BIS stream config"
#endif

#define SOURCE_SDU_INTERVAL 10000 // unit is 1us, current only use 10ms sdu interval.

#define BASE_SUBGROUPS_NUM  1
#define BIG_INFO_BIS_NUM    2
#define BIS_INDEX_1_CHANNEL LEA_LOCATION_FRONT_LEFT
#define BIS_INDEX_2_CHANNEL LEA_LOCATION_FRONT_RIGHT

const char *broadcast_source_name = BIS_SOURCE_COMPLETE_NAME;

struct ble_bis_source_info
{
    bool                                  is_open;
    uint8_t                               ext_adv_handle;
    uint8_t                               big_handle;
    lea_broadcast_source_operate_callback callback;
};

static struct ble_bis_source_info s_bis_source_info = {
    .is_open  = false,
    .callback = NULL,
};

static const struct lea_BASE_BIS_param s_source_bis_param[2] = {
    {
        .bis_index                       = 0x01,
        .codec_config.channel_allocation = LEA_LOCATION_FRONT_LEFT,
    },
    {
        .bis_index                       = 0x02,
        .codec_config.channel_allocation = LEA_LOCATION_FRONT_RIGHT,
    },
};

static const struct lea_BASE_BIG_param s_source_big_param = {
    .bis_num      = 2,
    .codec_id     = LEA_INIT_CODEC_ID_LC3,
    .codec_config = AUDIO_PARAM_LC3_CFG(LEA_LOCATION_NONE),
    .bis_param    = &s_source_bis_param[0],
};

static const struct lea_broadcast_audio_announcements_param bisSource = {
    .presentation_delay = SOURCE_PRESENTATION_DELAY,
    .sub_group_num      = BASE_SUBGROUPS_NUM,
    .big_param          = &s_source_big_param,
};

/**
 * @brief       Build extended advertising payload with name and vendor info.
 * @param[out]  outData - buffer receiving serialized AD data.
 * @return      total AD length in bytes.
 */
static int32_t app_ble_advData_initial(void *outData)
{
    uint32_t broadcast_id = trng_rand();

    struct pbp_ext_adv_data ext_adv_data = {
        .broadcast_id   = broadcast_id,
        .complete_name  = broadcast_source_name,
        .broadcast_name = broadcast_source_name,
    };

    return bis_source_create_adv_ad_data(&ext_adv_data, outData);
}

/**
 * @brief       Configure and start the extended advertising set for BIS source.
 * @return      none.
 */
static void app_ble_bis_extend_advertise_start(void)
{
    uint8_t advData[255];

    int32_t adv_ext_len = app_ble_advData_initial(advData);

    // This is flag for Telink sink to Synchronize without BASE information.
    uint8_t *advData_ptr = advData + adv_ext_len;
    uint8_t  ms_str_len  = strlen(broadcast_source_name);
    U8_TO_STREAM(advData_ptr, 0x03 + ms_str_len + 8);              // AD data length
    U8_TO_STREAM(advData_ptr, DT_MANUFACTURER_SPECIFIC_DATA);      // AD type: Manufacturer Specific Data
    U16_TO_STREAM(advData_ptr, 0x0211);                            // Company ID: Telink Semiconductor (Shanghai) Co., Ltd.
    STR_TO_STREAM(advData_ptr, broadcast_source_name, ms_str_len); // Manufacturer Specific Data
    STR_TO_STREAM(advData_ptr, LC3_CONFIG, 8);
    adv_ext_len += 4 + ms_str_len + 8;

    ble_host_gap_extend_adv_set_param(s_bis_source_info.ext_adv_handle, BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN(EXT_ADV_INTERVAL, EXT_ADV_INTERVAL + 10, EXT_ADV_SID));

    ble_host_gap_extend_adv_set_advertising_data(s_bis_source_info.ext_adv_handle, advData, adv_ext_len);

    int status = ble_host_gap_extend_adv_start(s_bis_source_info.ext_adv_handle);

    tlkapi_printf(APP_LOG_EN, "[APP][INI] set adv enable, status=0x%x", status);
}

/**
 * @brief       Stop the extended advertising set used by BIS source.
 * @return      none.
 */
static void app_ble_bis_extend_advertise_stop(void)
{
    int status = ble_host_gap_extend_adv_stop(s_bis_source_info.ext_adv_handle);

    tlkapi_printf(APP_LOG_EN, "[APP][INI] stop extend adv, status=0x%x", status);

    // if (status == BLE_HOST_ERR_SUCC) {
    //     ble_host_gap_extend_adv_delete_set(s_bis_source_info.ext_adv_handle);
    // }
}

/**
 * @brief       Configure and start periodic advertising carrying BASE payload.
 * @return      none.
 */
static void app_ble_bis_periodic_advertise_start(void)
{
    uint8_t pdaAdvData[256];

    uint8_t *pdaAdvDataPtr = lea_bap_set_BASE_to_address(&bisSource, pdaAdvData);

    ble_host_gap_pa_set_param_ms(s_bis_source_info.ext_adv_handle, 120, 120);

    ble_host_gap_pa_set_adv_data(s_bis_source_info.ext_adv_handle, pdaAdvData, pdaAdvDataPtr - pdaAdvData);

    int status = ble_host_gap_pa_enable(s_bis_source_info.ext_adv_handle);

    tlkapi_printf(APP_LOG_EN, "[APP][INI] periodic adv start, status=0x%x", status);
}

/**
 * @brief       Stop periodic advertising for BIS broadcast source.
 * @return      none.
 */
static void app_ble_bis_periodic_advertise_stop(void)
{
    int status = ble_host_gap_pa_disable(s_bis_source_info.ext_adv_handle);

    tlkapi_printf(APP_LOG_EN, "[APP][INI] disable pa adv, status=0x%x", status);
}

/**
 * @brief       BIG creation callback sets codec stream parameters and notifies app.
 * @param[in]   status  - BLE status returned by controller.
 * @param[in]   param   - BIG creation information.
 * @return      none.
 */
static void app_ble_big_create_callback(uint8_t status, struct ble_host_ble_host_gap_big_create_callback_param *param)
{
    if (status == BLE_SUCCESS) {
        tlkapi_printf(APP_LOG_EN, "[APP][INT] bis handle number: %d, bis handle: 0x%03x, 0x%03x", param->bis_number, param->bis_handles[0], param->bis_handles[1]);
        struct tlkmdi_lea_bms_config bms_config = {
            .samplingFrequency = s_source_big_param.codec_config.sampling_freq,
            .frameDuration     = s_source_big_param.codec_config.frame_duration,
            .frameOctets       = s_source_big_param.codec_config.per_codec_frame,
            .iso_handle[0]     = param->bis_handles[0],
            .iso_handle[1]     = param->bis_handles[1],
        };
#if MCU_CORE_TL752X_TEMP
        (void)bms_config;
#if defined(RPOJ_BLE_INTERNAL)
        tlkmdi_lea_bms_start(&bms_config);
#endif
#else
        tlkmdi_lea_bms_start(&bms_config);
#endif
    }
    tlkapi_printf(APP_LOG_EN, "[APP][INI] create big broadcast callback: status=%d", status);
    s_bis_source_info.is_open = true;
    if (s_bis_source_info.callback != NULL) {
        s_bis_source_info.callback();
        s_bis_source_info.callback = NULL;
    }
}

/**
 * @brief       Issue command to create BIG for broadcasting LC3 stream.
 * @return      none.
 */
static void app_ble_bis_big_start(void)
{
    s_bis_source_info.big_handle                               = ble_host_gap_big_create_handle();
    struct ble_host_gap_big_create_param s_big_broadcast_param = {
        .big_handle        = s_bis_source_info.big_handle,
        .adv_handle        = s_bis_source_info.ext_adv_handle,
        .bis_number        = BIG_INFO_BIS_NUM,
        .sdu_interval      = SOURCE_SDU_INTERVAL,
        .iso_interval      = SOURCE_ISO_INTERVAL,
        .transport_latency = SOURCE_TRANSMIT_LATENCY,
        .sdu_size          = 120, // current only use 120 bytes sdu size, to test controller.
        .retry_count       = SOURCE_RETRANSMIT_NUMBER,
        .enc               = 0,
    };

    ble_sts_t status = ble_host_gap_big_create_param(&s_big_broadcast_param, app_ble_big_create_callback);
    tlkapi_printf(APP_LOG_EN, "[APP][INI] create big broadcast, status=%d", status);
}

/**
 * @brief       BIG terminate callback cleans up handles and invokes completion callback.
 * @param[in]   big_handle  - handle being terminated.
 * @return      none.
 */
static void app_ble_big_terminate_callback(uint8_t big_handle)
{
    tlkapi_printf(APP_LOG_EN, "[APP][INI] BIG terminate callback: big_handle=%d", big_handle);

    ble_host_gap_big_delete_handle(s_bis_source_info.big_handle);
    ble_host_gap_extend_adv_delete_set(s_bis_source_info.ext_adv_handle);
    s_bis_source_info.is_open = false;

    if (s_bis_source_info.callback != NULL) {
        s_bis_source_info.callback();
        s_bis_source_info.callback = NULL;
    }
}

/**
 * @brief       Request BIG termination and log result.
 * @return      none.
 */
static void app_ble_bis_big_stop(void)
{
    int status = ble_host_gap_big_terminate(s_bis_source_info.big_handle, app_ble_big_terminate_callback);
    tlkapi_printf(APP_LOG_EN, "[APP][INI] terminate big broadcast, status=0x%x", status);
}

/**
 * @brief       Initialize optional broadcast source helpers (placeholder).
 * @return      none.
 */
void lea_broadcast_source_init(void)
{
    //    lea_a2dp_to_bis_le_init();
}

/**
 * @brief       De-initialize optional broadcast source helpers (placeholder).
 * @return      none.
 */
void lea_broadcast_source_deinit(void)
{
    //    lea_a2dp_to_bis_le_deinit();
}

/**
 * @brief       Return whether broadcast source pipelines are already open.
 * @return      true if open, false otherwise.
 */
bool lea_broadcast_source_is_open(void)
{
    return s_bis_source_info.is_open;
}

/**
 * @brief       Start extended/periodic advertising and BIG creation for broadcast source.
 * @param[in]   param       - configuration structure with optional device name.
 * @param[in]   callback    - invoked once creation completes.
 * @return      true if request accepted, false otherwise.
 */
bool lea_broadcast_source_open(const struct lea_broadcast_source_param *param, lea_broadcast_source_operate_callback callback)
{
    if (lea_broadcast_source_is_open() == true) {
        callback();
        return true;
    }
    ble_host_gap_extend_adv_init();
    ble_host_gap_big_init();
    if (param && param->device_name) {
        broadcast_source_name = param->device_name;
    }
    if (s_bis_source_info.callback != NULL) {
        return false;
    }

    s_bis_source_info.callback = callback;

    s_bis_source_info.ext_adv_handle = ble_host_gap_extend_adv_create_set();

    app_ble_bis_extend_advertise_start();
    app_ble_bis_periodic_advertise_start();
    app_ble_bis_big_start();
    return true;
}

/**
 * @brief       Stop BIG, periodic advertising, and extended advertising.
 * @param[in]   callback    - invoked when closure is finished.
 * @return      true if request accepted, false otherwise.
 */
bool lea_broadcast_source_close(lea_broadcast_source_operate_callback callback)
{
    if (lea_broadcast_source_is_open() == false) {
        callback();
        return true;
    }

    if (s_bis_source_info.callback != NULL) {
        return false;
    }

    s_bis_source_info.callback = callback;

    app_ble_bis_extend_advertise_stop();
    app_ble_bis_periodic_advertise_stop();
    app_ble_bis_big_stop();
    return true;
}
