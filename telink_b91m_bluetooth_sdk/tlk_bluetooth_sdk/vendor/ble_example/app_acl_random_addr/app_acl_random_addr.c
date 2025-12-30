/********************************************************************************************************
 * @file    app_acl_random_addr.c
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

#define APP_ADDRESS_TYPE_STATIC_RANDOM                      0x01
#define APP_ADDRESS_TYPE_NON_RESOLVABLE_PRIVATE_ADDRESS     0x02
#define APP_ADDRESS_TYPE_RESOLVABLE_PRIVATE_ADDRESS         0x03

#define APP_ADDRESS_TYPE        APP_ADDRESS_TYPE_RESOLVABLE_PRIVATE_ADDRESS

#if (APP_ADDRESS_TYPE == APP_ADDRESS_TYPE_STATIC_RANDOM)
static const uint8_t s_random_addr[6] = { 0x7F, 0x84, 0xC3, 0xC3, 0x76, 0xD2 };
#elif (APP_ADDRESS_TYPE == APP_ADDRESS_TYPE_NON_RESOLVABLE_PRIVATE_ADDRESS)
static const uint8_t s_non_resolvable_private_addr[6] = { 0x2A, 0xEB, 0x95, 0x66, 0x28, 0X26 };
#elif (APP_ADDRESS_TYPE == APP_ADDRESS_TYPE_RESOLVABLE_PRIVATE_ADDRESS)
#endif


int INIT(APP_BLE_ACL)(void);
void START(APP_BLE_ACL)(void);

int INIT(APP_BLE_ACL_RANDOM_ADDR)(void)
{
    // must initialize SMP module before setting random address
    ble_host_smp_initial(BLE_HOST_SMP_LEGACY_JUST_WORKS_INIT_PARAMS);

#if (APP_ADDRESS_TYPE == APP_ADDRESS_TYPE_STATIC_RANDOM)
    ble_host_gap_set_set_static_addr(s_random_addr);
#elif (APP_ADDRESS_TYPE == APP_ADDRESS_TYPE_NON_RESOLVABLE_PRIVATE_ADDRESS)
    ble_host_gap_set_random_addr(s_non_resolvable_private_addr);
#elif (APP_ADDRESS_TYPE == APP_ADDRESS_TYPE_RESOLVABLE_PRIVATE_ADDRESS)
    ble_host_gap_set_resolvable_private_addr();
#endif

    INIT(APP_BLE_ACL)();
    return 0;
}

void START(APP_BLE_ACL_RANDOM_ADDR)(void)
{
    START(APP_BLE_ACL)();
}
