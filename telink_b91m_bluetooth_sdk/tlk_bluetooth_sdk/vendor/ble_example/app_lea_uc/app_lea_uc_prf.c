/********************************************************************************************************
 * @file    app_lea_uc_prf.c
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

#include "app_lea_uc_prf.h"

struct lea_us_adv_data_value
{
    uint8_t flags;
    bool    lea_us_flags;
    uint8_t complete_name[32];
    bool    rsi_flags;
    uint8_t rsi[6];
};

enum
{
    DISCOVERY_US_CONNECTABLE_MODE = 0,
    DISCOVERY_US_FILTER_BY_SIRK   = 1,
};

static found_connectable_us_callback s_found_callback = NULL;

static uint8_t s_discovery_mode = DISCOVERY_US_CONNECTABLE_MODE;
static uint8_t s_discovery_sirk[16];

static void app_discovery_us_gap_evt_handler(uint32_t event_id, const void *data, void *user_data);

static struct ble_host_gap_evt_subscribe_param s_app_disc_us_gap_evt_param = {
    .gap_le_scan_mask = 1,
    .handler          = app_discovery_us_gap_evt_handler,
    .user_data        = NULL,
};

static struct ble_host_gap_evt_subscribe s_app_disc_us_gap_evt = {
    .next_handler = NULL,
    .param        = &s_app_disc_us_gap_evt_param,
};

/**
 * @brief  Start discovery of connectable UC devices.
 *          Register GAP event handler and set found callback.
 *
 * @param[in] param  Discovery initialization parameters.
 */
void app_uc_prf_discovery_us(const struct discovery_us_init_param *param)
{
    if (param != NULL && param->found_callback != NULL) {
        s_found_callback = param->found_callback;
    }

    ble_host_gap_evt_add_subscriber(&s_app_disc_us_gap_evt);
}

/**
 * @brief  Set discovery filter by SIRK.
 *          Switch to filter mode and store SIRK for RSI validation.
 *
 * @param[in] srik  Set Identity Resolving Key (SIRK), 16 bytes.
 */
void app_uc_prf_set_discovery_filter(uint8_t srik[16])
{
    s_discovery_mode = DISCOVERY_US_FILTER_BY_SIRK;
    memcpy(s_discovery_sirk, srik, 16);
}

/**
 * @brief  Clear discovery filter.
 *          Switch back to connectable mode and clear SIRK.
 */
void app_uc_prf_clear_discovery_filter(void)
{
    s_discovery_mode = DISCOVERY_US_CONNECTABLE_MODE;
    memset(s_discovery_sirk, 0, 16);
}

/**
 * @brief  Check and parse advertisement data.
 *          Extract flags, service UUID, device name and RSI from advertisement data.
 *
 * @param[in] length  Data length.
 * @param[in] type    Data type.
 * @param[in] value   Data value.
 * @param[in] user    User data pointer to store parsed values.
 *
 * @return  1 to continue parsing, 0 to stop.
 */
static int app_discovery_us_check_adv_data_handle(uint8_t length, uint8_t type, const uint8_t *value, void *user)
{
    struct lea_us_adv_data_value *p_adv_data = user;

    if (type == DT_FLAGS) {
        p_adv_data->flags = value[0];
    } else if (type == DT_SERVICE_DATA_16BIT_UUID) {
        uint16_t uuid = *(const uint16_t *)value;
        if (uuid == SERVICE_UUID_AUDIO_STREAM_CONTROL) {
            p_adv_data->lea_us_flags = true;
        }
    } else if (type == DT_COMPLETE_LOCAL_NAME) {
        length = min(length, 31);
        memcpy(p_adv_data->complete_name, value, length);
    } else if (type == DT_CSIP_RSI) {
        p_adv_data->rsi_flags = true;
        memcpy(p_adv_data->rsi, value, 6);
    }

    return 1;
}

/**
 * @brief  Handle undirected scan advertisement report event.
 *          Parse advertisement data and call found callback if device matches filter criteria.
 *
 * @param[in] p_report  Scan advertisement report data.
 */
static void app_discovery_us_gap_evt_scan_report_undirected(const struct ble_host_gap_evt_scan_adv_report_undirected *p_report)
{
    if (p_report->connectable == false) {
        // ignore non-connectable devices.
        return;
    }

    struct lea_us_adv_data_value adv_data;
    memset(&adv_data, 0, sizeof(struct lea_us_adv_data_value));

    int ret = ltv_unpack(p_report->data, p_report->data_len, app_discovery_us_check_adv_data_handle, &adv_data);

    if (ret == LTV_UNPACK_SUCCESS) {
        if (s_discovery_mode == DISCOVERY_US_CONNECTABLE_MODE) {
            if ((adv_data.flags & (FLAGS_LE_GENERAL_DISCOVERABLE_MODE | FLAGS_LE_LIMITED_DISCOVERABLE_MODE)) == 0 || adv_data.lea_us_flags == false) {
                // ignore non-LEA-US or cannot discoverable devices.
                return;
            }
        } else {
            if (adv_data.rsi_flags == false) {
                return;
            }
            if (ble_csis_check_rsi_valid(adv_data.rsi, s_discovery_sirk) == false) {
                return;
            }
        }

        if (s_found_callback != NULL) {
            s_found_callback(p_report->addr_type, p_report->addr, adv_data.complete_name, p_report->rssi);
        }
    } else {
        // ignore invalid adv data.
    }
}

/**
 * @brief  GAP event handler for discovery.
 *          Process scan report events.
 *
 * @param[in] event_id   Event ID.
 * @param[in] data       Event data.
 * @param[in] user_data  User data.
 */
static void app_discovery_us_gap_evt_handler(uint32_t event_id, const void *data, void *user_data)
{
    (void)user_data;

    if (event_id == BLE_HOST_GAP_EVT_LE_SCAN_REPORT_UNDIRECTED) {
        // receive scan report undirected event
        app_discovery_us_gap_evt_scan_report_undirected(data);
    }
}

/**
 * @brief  Initialize LE Audio UC profile.
 *          Initialize CAP UC, register PACS/ASCS/CSIS control clients and initialize SMP.
 */
void app_uc_prf_initial(void)
{
    ble_capuc_init(NULL);
    ble_lea_register_PACS_control_client(NULL);
    ble_lea_register_ASCS_control_client(NULL);
    ble_lea_register_CSIS_control_client(NULL);

    ble_host_smp_initial(BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS);
    ble_host_smp_store_init(0, 4);
}
