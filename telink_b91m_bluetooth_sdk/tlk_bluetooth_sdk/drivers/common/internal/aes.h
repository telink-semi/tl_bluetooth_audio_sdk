/********************************************************************************************************
 * @file    aes.h
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
#ifndef DRIVERS_COMMON_INTERNAL_AES_H_
#define DRIVERS_COMMON_INTERNAL_AES_H_
#include "driver.h"
#include "ext_driver/driver_ext.h"
#include "tlkalg/crypt/aes_sw/aes_sw.h"

#if (MCU_AES_USE_BT_CEVA_CORE_HARDWARE_AES)

    #if (MCU_SUPPORT_BT_CLASSIC_FUNCTION_EN)
void aes_encryption_le(u8 *key, u8 *plaintext, u8 *encrypted_data);
    #else
        #define aes_encryption_le aes_encryption_hw
    #endif

void aes_encryption_be(u8 *key, u8 *plaintext, u8 *encrypted_data);

bool aes_resolve_irk_rpa_loop(u8 *key, u8 *addr);

#endif

#endif /* DRIVERS_COMMON_INTERNAL_AES_H_ */
