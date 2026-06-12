/********************************************************************************************************
 * @file    tlkhal_misc_TL721X.c
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
#if MCU_CORE_TYPE == MCU_CORE_TL721X

#define TLKHAL_MAC_COMPANY_ID 0x382228

void tlkhal_get_bluetooth_mac(uint8_t *mac)
{
    uint8_t  mac_flash[6];
    uint8_t  mac_cmp[6];
    uint32_t mac_save_addr = 0xFF000 + tlkhal_flash_get_size() - 0x100000;
    flash_read_page(mac_save_addr, 6, mac_flash);
    memset(mac_cmp, 0XFF, sizeof(mac_cmp));

    // Step 0: Read MAC from flash. Use it if valid data exists.
    if (memcmp(mac_flash, mac_cmp, sizeof(mac_cmp)) != 0) {
        memcpy(mac, mac_flash, sizeof(mac_flash));
        return;
    }


    // Step 1: Read MAC from OTP. Use it if programmed.
    if (get_device_mac_address(mac_flash, 6)) {
        memcpy(mac, mac_flash, sizeof(mac_flash));
        return;
    }

    // Step 2: Generate random MAC and save to flash.
    // Note: Random MAC is for internal debugging only. In mass production, OTP must be programmed with a valid MAC.
    uint32_t rnd = trng_rand();
    mac_flash[0] = (rnd >> 0) & 0XFF;
    mac_flash[1] = (rnd >> 8) & 0XFF;
    mac_flash[2] = (rnd >> 16) & 0XFF;
    mac_flash[3] = (TLKHAL_MAC_COMPANY_ID >> 0) & 0XFF;
    mac_flash[4] = (TLKHAL_MAC_COMPANY_ID >> 8) & 0XFF;
    mac_flash[5] = (TLKHAL_MAC_COMPANY_ID >> 16) & 0XFF;
    flash_write_page(mac_save_addr, 6, mac_flash);

    memcpy(mac, mac_flash, sizeof(mac_flash));
}

#endif
