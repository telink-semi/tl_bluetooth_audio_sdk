/********************************************************************************************************
 * @file    ext_core.h
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
#ifndef EXT_CORE_H
#define EXT_CORE_H

#ifndef TICK_PER_US
    #define TICK_PER_US SYSTEM_TIMER_TICK_1US
#endif
#ifndef TICK_PER_HALF_US
    #define TICK_PER_HALF_US (TICK_PER_US / 2)
#endif

typedef enum
{
    SYS_CHIP_CLOCK_96M = 1,
    SYS_CHIP_CLOCK_48M = 2,
    SYS_CHIP_CLOCK_32M = 3,
    SYS_CHIP_CLOCK_24M = 4,
    SYS_CHIP_CLOCK_16M = 5,
} SYS_CHIP_CLOCK_ENUM;


void btc_context_store(void);

void btc_context_restore(void);

void tlk_bredr_clock_init(void);

_attribute_bt_ram_code_ uint32_t tlk_bredr_irq_type_get(void);

#endif //EXT_CORE_H
