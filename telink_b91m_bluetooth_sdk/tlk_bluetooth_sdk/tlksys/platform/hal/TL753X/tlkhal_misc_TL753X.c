/********************************************************************************************************
 * @file    tlkhal_misc_TL753X.c
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
#include "../../api/tlkhal_api.h"
#include "drivers.h"
#if MCU_CORE_TYPE == MCU_CORE_TL753X

#define TLKHAL_MAC_COMPANY_ID 0x112233

void tlkhal_get_bluetooth_mac(uint8_t *mac)
{
    //temp code,wait chip backup
    uint8_t mac_flash[6];

    uint32_t rnd = trng_rand();
    mac_flash[0] = (rnd >> 0) & 0XFF;
    mac_flash[1] = (rnd >> 8) & 0XFF;
    mac_flash[2] = (rnd >> 16) & 0XFF;
    mac_flash[3] = (TLKHAL_MAC_COMPANY_ID >> 0) & 0XFF;
    mac_flash[4] = (TLKHAL_MAC_COMPANY_ID >> 8) & 0XFF;
    mac_flash[5] = (TLKHAL_MAC_COMPANY_ID >> 16) & 0XFF;

    memcpy(mac, mac_flash, sizeof(mac_flash));
}

#endif
