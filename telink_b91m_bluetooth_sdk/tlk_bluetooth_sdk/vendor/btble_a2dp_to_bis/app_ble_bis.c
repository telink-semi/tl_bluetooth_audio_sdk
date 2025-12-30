/********************************************************************************************************
 * @file    app_ble_bis.c
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

#include "app_ble.h"

#include "app_ui.h"
#include "app_ble_bis.h"

#include "tlkmw/sys_dev/codec/tlkdrv_codec.h"
#include "tlkmw/audio/le_audio/le_audio_common.h"

#include "tlkmw/audio/a2dp_to_bis/tlkmdi_a2dp_to_bis.h"
#include "tlkmw/audio/a2dp_to_bis/tlkmdi_a2dp_to_bis_le_music.h"

#define EXT_ADV_SID               8

#define EXT_ADV_INTERVAL          (60) //60ms  or 120ms

#define BIS_SOURCE_COMPLETE_NAME  "A2DP to BIS Demo"

#define SOURCE_PRESENTATION_DELAY 40000 //20ms    presentation delay, unit is 1us.
#if MCU_CORE_TL752X_TEMP
#define SOURCE_TRANSMIT_LATENCY 3 // bis transmit delay, unit is sdu interval(7.5ms or 10ms).
#else
#define SOURCE_TRANSMIT_LATENCY 2 // bis transmit delay, unit is sdu interval(7.5ms or 10ms).
#endif
#define SOURCE_ISO_INTERVAL      2 // bis iso interval, unit is sdu interval(7.5ms or 10ms).
#define SOURCE_RETRANSMIT_NUMBER 3 // streaming packet retry count, usually 3 or 4, current use 3.

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

struct ble_bis_source_info
{
    bool                                is_open;
    uint8_t                             ext_adv_handle;
    uint8_t                             big_handle;
    app_ble_bis_source_operate_callback callback;
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
 * @brief       Initialize advertisement data for BLE BIS
 * @param[out]  outData - Buffer to store the advertisement data
 * @return      Length of the advertisement data
 */
static int32_t app_ble_advData_initial(void *outData)
{
    uint32_t broadcast_id = trng_rand();

    struct pbp_ext_adv_data ext_adv_data = {
        .broadcast_id   = broadcast_id,
        .complete_name  = BIS_SOURCE_COMPLETE_NAME,
        .broadcast_name = BIS_SOURCE_BROADCAST_NAME,
    };

    return bis_source_create_adv_ad_data(&ext_adv_data, outData);
}

/**
 * @brief       Start extended advertisement for BLE BIS
 * @param       none
 * @return      none
 */
static void app_ble_bis_extend_advertise_start(void)
{
    uint8_t advData[255];

    int32_t adv_ext_len = app_ble_advData_initial(advData);

    // This is flag for Telink sink to Synchronize without BASE information.
    uint8_t *advData_ptr = advData + adv_ext_len;
    uint8_t  ms_str_len  = sizeof(BIS_SOURCE_MS_STR) - 1;
    U8_TO_STREAM(advData_ptr, 0x03 + ms_str_len + 8);          // AD data length
    U8_TO_STREAM(advData_ptr, DT_MANUFACTURER_SPECIFIC_DATA);  // AD type: Manufacturer Specific Data
    U16_TO_STREAM(advData_ptr, 0x0211);                        // Company ID: Telink Semiconductor (Shanghai) Co., Ltd.
    STR_TO_STREAM(advData_ptr, BIS_SOURCE_MS_STR, ms_str_len); // Manufacturer Specific Data
    STR_TO_STREAM(advData_ptr, LC3_CONFIG, 8);
    adv_ext_len += 4 + ms_str_len + 8;

    ble_host_gap_extend_adv_set_param(s_bis_source_info.ext_adv_handle, BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN(EXT_ADV_INTERVAL, EXT_ADV_INTERVAL + 10, EXT_ADV_SID));

    ble_host_gap_extend_adv_set_advertising_data(s_bis_source_info.ext_adv_handle, advData, adv_ext_len);

    int status = ble_host_gap_extend_adv_start(s_bis_source_info.ext_adv_handle);

    tlkapi_printf(APP_LOG_EN, "[APP][INI] set adv enable, status=0x%x", status);
}

/**
 * @brief       Stop extended advertisement for BLE BIS
 * @param       none
 * @return      none
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
 * @brief       Start periodic advertisement for BLE BIS
 * @param       none
 * @return      none
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
 * @brief       Stop periodic advertisement for BLE BIS
 * @param       none
 * @return      none
 */
static void app_ble_bis_periodic_advertise_stop(void)
{
    int status = ble_host_gap_pa_disable(s_bis_source_info.ext_adv_handle);

    tlkapi_printf(APP_LOG_EN, "[APP][INI] disable pa adv, status=0x%x", status);
}

/**
 * @brief       Callback function for BIG creation
 * @param[in]   status - Status of the BIG creation
 * @param[in]   param - Parameters for the BIG creation
 * @return      none
 */
static void app_ble_big_create_callback(uint8_t status, struct ble_host_ble_host_gap_big_create_callback_param *param)
{
    if (status == BLE_SUCCESS) {
        tlkapi_printf(APP_LOG_EN, "[APP][INT] bis handle number: %d, bis handle: 0x%03x, 0x%03x", param->bis_number, param->bis_handles[0], param->bis_handles[1]);

#if (TLK_MW_LEA_A2DP_TO_BIS_ENABLE)
        struct lea_a2dp_to_bis_config bis_config = {
            .samplingFrequency = s_source_big_param.codec_config.sampling_freq,
            .frameDuration     = s_source_big_param.codec_config.frame_duration,
            .frameOctets       = s_source_big_param.codec_config.per_codec_frame,
            .iso_handle[0]     = param->bis_handles[0],
            .iso_handle[1]     = param->bis_handles[1],
        };
        lea_a2dp_to_bis_init(&bis_config);
#endif

        // blc_ll_setBisSduSplitIntoPduStrategy(BIS_SDU_2_PDU_STRATEGY1);
        // blc_ll_regBigBcstGetSduRefApCallback(big_bcst_1st_sdu_ref_ap_callback);
    }
    tlkapi_printf(APP_LOG_EN, "[APP][INI] create big broadcast callback: status=%d", status);
    s_bis_source_info.is_open = true;
    if (s_bis_source_info.callback != NULL) {
        s_bis_source_info.callback();
        s_bis_source_info.callback = NULL;
    }
}

/**
 * @brief       Start the BLE BIG (Broadcast Isochronous Group)
 * @param       none
 * @return      none
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
 * @brief       Callback function for BIG termination
 * @param[in]   big_handle - Handle of the terminated BIG
 * @return      none
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
 * @brief       Stop the BLE BIG (Broadcast Isochronous Group)
 * @param       none
 * @return      none
 */
static void app_ble_bis_big_stop(void)
{
    lea_a2dp_to_bis_deinit();
    int status = ble_host_gap_big_terminate(s_bis_source_info.big_handle, app_ble_big_terminate_callback);
    tlkapi_printf(APP_LOG_EN, "[APP][INI] terminate big broadcast, status=0x%x", status);
}

/**
 * @brief       Initialize the BLE BIS source
 * @param       none
 * @return      none
 */
void app_ble_bis_source_init(void)
{
    lea_a2dp_to_bis_le_init();
}

/**
 * @brief       Deinitialize the BLE BIS source
 * @param       none
 * @return      none
 */
void app_ble_bis_source_deinit(void)
{
    lea_a2dp_to_bis_le_deinit();
}

/**
 * @brief       Open the BLE BIS source
 * @param[in]   callback - Callback function to be called upon completion
 * @return      true if operation started successfully, false otherwise
 */
bool app_ble_bis_source_open(app_ble_bis_source_operate_callback callback)
{
    if (app_ble_bis_source_is_open() == true) {
        callback();
        return true;
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
 * @brief       Close the BLE BIS source
 * @param[in]   callback - Callback function to be called upon completion
 * @return      true if operation started successfully, false otherwise
 */
bool app_ble_bis_source_close(app_ble_bis_source_operate_callback callback)
{
    if (app_ble_bis_source_is_open() == false) {
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

/**
 * @brief       Check if the BLE BIS source is open
 * @param       none
 * @return      true if BIS source is open, false otherwise
 */
bool app_ble_bis_source_is_open(void)
{
    return s_bis_source_info.is_open;
}