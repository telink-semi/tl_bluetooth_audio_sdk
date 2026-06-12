/********************************************************************************************************
 * @file    tlkhal_flash_TL753X.c
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

void tlkhal_flash_init(uint8_t enable_lock)
{
    (void)enable_lock;
}

uint32_t tlkhal_flash_get_size(void)
{
    return 0x400000;
}

void tlkhal_flash_lock_ex(tlkhal_flash_lock_size_e size)
{
    (void)size;
}

void tlkhal_flash_lock(void) {}

void tlkhal_flash_unlock(void) {}

void tlkhal_flash_4line_enable(void) {}

#endif
