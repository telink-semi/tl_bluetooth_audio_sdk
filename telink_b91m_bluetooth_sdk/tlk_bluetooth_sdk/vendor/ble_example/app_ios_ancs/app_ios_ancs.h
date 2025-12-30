/********************************************************************************************************
 * @file    app_ios_ancs.h
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

// ANCS : Apple Notification Center Service

#if IS_DEMO_SELECTED(APP_BLE_IOS_ANCS)

#define BLE_IOS_ANCS_SERVICE 0xD0, 0x00, 0x2D, 0x12, 0x1E, 0x4B, 0x0F, 0xA4, 0x99, 0x4E, 0xCE, 0xB5, 0x31, 0xF4, 0x05, 0x79
#define BLE_IOS_ANCS_NOTIFICATION_SOURCE 0xBD, 0x1D, 0xA2, 0x99, 0xE6, 0x25, 0x58, 0x8c, 0xD9, 0x42, 0x01, 0x63, 0x0D, 0x12, 0xBF, 0x9F
#define BLE_IOS_ANCS_CONTROL_POINT 0xD9, 0xD9, 0xAA, 0xFD, 0xBD, 0x9B, 0x21, 0x98, 0xA8, 0x49, 0xE1, 0x45, 0xF3, 0xD8, 0xD1, 0x69
#define BLE_IOS_ANCS_DATA_SOURCE 0xFB, 0x7B, 0x7C, 0xCE, 0x6A, 0xB3, 0x44, 0xBE, 0xB5, 0x4B, 0xD6, 0x24, 0xE9, 0xC6, 0xEA, 0x22

extern const struct att_uuid ble_ios_ancs_service_uuid;
extern const struct att_uuid ble_ios_ancs_notification_source;
extern const struct att_uuid ble_ios_ancs_control_point;
extern const struct att_uuid ble_ios_ancs_data_source;

#endif
