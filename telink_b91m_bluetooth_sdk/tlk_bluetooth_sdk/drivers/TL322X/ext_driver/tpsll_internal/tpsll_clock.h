/********************************************************************************************************
 * @file    tpsll_clock.h
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
#ifndef DRIVERS_TL322X_TPSLL_EXT_DRIVER_TPSLL_CLOCK_H_
#define DRIVERS_TL322X_TPSLL_EXT_DRIVER_TPSLL_CLOCK_H_
#include "drivers.h"

#ifndef TICK_PER_US
    #define TICK_PER_US 24
#endif
#ifndef TICK_PER_HALF_US
    #define TICK_PER_HALF_US (TICK_PER_US / 2)
#endif

#define N22_CLK_MHZ         (48)

#define reg_system_tick_irq reg_system_irq_level

extern unsigned int get_u32_safe(unsigned int v);

static inline int clock_time_exceed_safe(unsigned int t, unsigned int us)
{
    unsigned int ts = get_u32_safe(t);
    return ts && ((unsigned int)(stimer_get_tick() - ts) > us * SYSTEM_TIMER_TICK_1US);
}

#endif /* DRIVERS_TL751X_TPSLL_EXT_DRIVER_TPSLL_CLOCK_H_ */
