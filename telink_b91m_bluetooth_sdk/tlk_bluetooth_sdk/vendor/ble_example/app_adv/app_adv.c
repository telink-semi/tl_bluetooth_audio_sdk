/********************************************************************************************************
 * @file    app_adv.c
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
#include "stack/ble/ble.h"
#include "../app_example.h"

#define ADV_TYPE_ADV_IND         0
#define ADV_TYPE_ADV_SCAN_IND    1
#define ADV_TYPE_ADV_NONCONN_IND 2


#define BLE_DEVICE_NAME          "app_ble_adv"

#define APP_ADV_TYPE             ADV_TYPE_ADV_IND

static const struct ad_data_flags s_adv_flags = {
    .header.length                      = 0x02,
    .header.type                        = DT_FLAGS,
    .flags.le_limited_discoverable_mode = 1,
    .flags.br_edr_not_supported         = 1,
};

static const struct ad_data_complete_local_name_complete s_adv_complete_name = {
    .header.length = sizeof(BLE_DEVICE_NAME),
    .header.type   = DT_COMPLETE_LOCAL_NAME,
    .name          = BLE_DEVICE_NAME,
};

static const struct ltv_data *adv_data[] = {
    (const struct ltv_data *)&s_adv_flags, (const struct ltv_data *)&s_adv_complete_name,
    NULL, // end of advertisement data
};
static const struct ltv_data *scan_rsp_data[] = {
    (const struct ltv_data *)&s_adv_complete_name,
    NULL, // end of advertisement data
};

int INIT(APP_BLE_ADV)(void)
{
    uint8_t ad_data[255];
    int32_t ad_len = 0;
#if APP_ADV_TYPE == ADV_TYPE_ADV_IND
    ble_host_gap_adv_set_param(BLE_HOST_GAP_ADV_SET_ADV_IND(100, 200));
#elif APP_ADV_TYPE == ADV_TYPE_ADV_SCAN_IND
    ble_host_gap_adv_set_param(BLE_HOST_GAP_ADV_SET_ADV_SCAN_IND(100, 200));
#elif APP_ADV_TYPE == ADV_TYPE_ADV_NONCONN_IND
    ble_host_gap_adv_set_param(BLE_HOST_GAP_ADV_SET_ADV_NONCONN_IND(100, 200));
#endif

    ad_len = ltv_pack(adv_data, ad_data);
    ble_host_gap_adv_set_adv_data(ad_data, ad_len);

    ad_len = ltv_pack(scan_rsp_data, ad_data);
    ble_host_gap_adv_set_scan_rsp_data(ad_data, ad_len);

    return 0;
}

void START(APP_BLE_ADV)(void)
{
    ble_host_gap_adv_start();
}
