/********************************************************************************************************
 * @file    app_lea_uc_ext_scan_ctrl.c
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
#include "driver.h"

/**
 * @brief  Initialize extended scan control.
 *          Initialize extended scan, set scan parameters and filter configuration.
 */
void app_extend_scan_ctrl_init(void)
{
    ble_host_gap_ext_scan_init();
    ble_host_gap_ext_scan_set_param(BLE_HOST_GAP_EXT_SCAN_PARAM_BASIC_UNFILTERED, BLE_HOST_GAP_PASSIVE_SCAN(100, 100));
    struct ble_host_gap_scan_filter_param filter_param = {
        .filter_non_conn_non_scan = true,
        .filter_scannable         = true,
    };
    ble_host_gap_scan_set_filter(&filter_param);
}

/**
 * @brief  Start extended scan.
 */
void app_extend_scan_ctrl_start(void)
{
    ble_host_gap_ext_scan_enable();
}

/**
 * @brief  Stop extended scan.
 */
void app_extend_scan_ctrl_stop(void)
{
    ble_host_gap_ext_scan_disable();
}
