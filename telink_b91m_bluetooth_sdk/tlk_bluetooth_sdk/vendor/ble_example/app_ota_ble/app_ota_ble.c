/********************************************************************************************************
 * @file    app_ota_ble.c
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

int INIT(APP_BLE_ACL_PERIPHERAL)(void);
void START(APP_BLE_ACL_PERIPHERAL)(void);

int INIT(APP_BLE_OTA_BLE)(void)
{
    blc_svc_addOtaV2Group();
    INIT(APP_BLE_ACL_PERIPHERAL)();

    tlk_printf("[APP] BLE OTA service initialized");
    return 0;
}

void START(APP_BLE_OTA_BLE)(void)
{
    START(APP_BLE_ACL_PERIPHERAL)();
}