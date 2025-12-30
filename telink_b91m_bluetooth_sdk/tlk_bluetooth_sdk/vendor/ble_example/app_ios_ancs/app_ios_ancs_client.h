/********************************************************************************************************
 * @file    app_ios_ancs_client.h
 *
 * @brief   This is the header file for TLSR/TL
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

#if IS_DEMO_SELECTED(APP_BLE_IOS_ANCS)

struct app_ios_ancs_client
{
    struct ble_gattc_ccc_message ccc_msg;
    uint16_t                     start_handle;
    uint16_t                     end_handle;

    uint16_t notification_source_handle;
    uint16_t control_point_handle;
    uint16_t data_source_handle;

    uint16_t conn_handle;

    uint8_t notification_source_properties;
    uint8_t control_point_properties;
    uint8_t data_source_properties;
};

struct app_ios_ancs_client_control
{
    struct ble_prf_client_process prf_process;
    struct app_ios_ancs_client    ancs_client;
};

#endif
