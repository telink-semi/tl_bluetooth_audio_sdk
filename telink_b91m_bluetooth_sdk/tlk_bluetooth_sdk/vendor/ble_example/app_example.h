/********************************************************************************************************
 * @file    app_example.h
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
#pragma once

#define START(...)            EXPAND(START_(__VA_ARGS__))
#define INIT(...)             EXPAND(INIT_(__VA_ARGS__))
#define IS_DEMO_SELECTED(...) (GET_DEMO_ID(APP_DEMO_SELECT) == GET_DEMO_ID(__VA_ARGS__))

//
#define APP_BLE_NEW_DEMO app_new_demo, 1

// Simple BLE demo
#define APP_BLE_ADV                  app_adv, 100
#define APP_BLE_ACL                  app_acl, 101
#define APP_BLE_ACL_CENTRAL          app_acl_central, 102
#define APP_BLE_ACL_PERIPHERAL       app_acl_peripheral, 103
#define APP_BLE_OTA_BLE              app_ota_ble, 104
#define APP_BLE_ACL_SMP              app_acl_smp, 106
#define APP_BLE_ACL_RANDOM_ADDR      app_acl_random_addr, 107
#define APP_BLE_HID_DEVICE           app_hid_device, 108
#define APP_BLE_SCAN                 app_scan, 109
#define APP_BLE_SPP_SERVER           app_spp_server, 110
#define APP_BLE_SPP_CLIENT           app_spp_client, 111
#define APP_BLE_SSDP                 app_ssdp, 112
#define APP_BLE_IOS_ANCS             app_ios_ancs, 113
#define APP_BLE_L2CAP_COC            app_l2cap_coc, 114

#define APP_BLE_LEA_US               app_lea_us, 200
#define APP_BLE_LEA_UC               app_lea_uc, 201
#define APP_BLE_LEA_SOURCE           app_lea_source, 202
#define APP_BLE_LEA_BROADCAST_SINK   app_lea_broadcast_sink, 203
#define APP_BLE_LEA_BROADCAST_SOURCE app_lea_broadcast_source, 204
#define APP_BLE_LEA_DEVICE           app_lea_device, 205

#define APP_BLE_CIS_TO_BIS_RELAY     app_lea_cis_to_bis_relay, 210


// develop can not commit this select to gitlab.
#define APP_DEMO_SELECT APP_BLE_NEW_DEMO


#ifndef STRINGIFY //some compilers don't support this
#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x)        STRINGIFY_HELPER(x)
#endif

#define CAT(a, b)           a##b
#define EXPAND(x)           x
#define EVAL(x)             EXPAND(x)
#define TOSTRING(x)         STRINGIFY(x)
#define CFG_PATH_(x, id)    TOSTRING(EVAL(CAT(x/x, _cfg.h)))
#define CFG_PATH(...)       EXPAND(CFG_PATH_(__VA_ARGS__))
#define INIT_(x, id)        EVAL(CAT(x, _init))
#define START_(x, id)       EVAL(CAT(x, _start))
#define GET_DEMO_ID_(x, id) id
#define GET_DEMO_ID(...)    GET_DEMO_ID_(__VA_ARGS__)
