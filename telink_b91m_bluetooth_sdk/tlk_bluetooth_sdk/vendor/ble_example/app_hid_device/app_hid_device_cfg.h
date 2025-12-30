/********************************************************************************************************
 * @file    app_hid_device_cfg.h
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

#define APP_HID_DEVICE_MODE_KEYBOARD        0x01
#define APP_HID_DEVICE_MODE_MOUSE           0x02

#define APP_HID_DEVICE_SELECT_MODE          APP_HID_DEVICE_MODE_KEYBOARD

#if (APP_HID_DEVICE_SELECT_MODE == APP_HID_DEVICE_MODE_KEYBOARD)
#include "stack/ble/host_v1/services/svc_hid/hid_demo/keyboard_cfg.h"
#elif (APP_HID_DEVICE_SELECT_MODE == APP_HID_DEVICE_MODE_MOUSE)
#include "stack/ble/host_v1/services/svc_hid/hid_demo/mouse_cfg.h"
#else

#endif

// PnP ID Information.
#define DIS_PNP_ID_VENDOR_ID_SOURCE   0x02
#define DIS_PNP_ID_VENDOR_ID          0x248A
#define DIS_PNP_ID_PRODUCT_ID         0x6666
#define DIS_PNP_ID_PRODUCT_VERSION    0x0002
