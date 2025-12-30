/********************************************************************************************************
 * @file    plmt_reg.h
 *
 * @brief   This is the header file for tl322x
 *
 * @author  Driver Group
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
#ifndef __PLMT_REG_H_
#define __PLMT_REG_H_
#include "soc.h"

/*******************************     mtime registers:     ******************************/

#if defined(MCU_CORE_TL322X_N22)
    #define MACHINE_TIMER_BASE 0xe6000000
#else
    #define MACHINE_TIMER_BASE 0xc6000000
#endif

/**
 * @brief The compiler supports the long long (64bit) type, but since the CPU is 32bit, it is operated separately in assembly, so need to pay attention to synchronization when reading this register
 * 
 */
#define reg_mtime         (*(volatile unsigned long long *)(MACHINE_TIMER_BASE))
#define reg_mtime_low     (*(volatile unsigned long *)(MACHINE_TIMER_BASE))
#define reg_mtime_high    (*(volatile unsigned long *)(MACHINE_TIMER_BASE + 0x04))

#define reg_mtimecmp      (*(volatile unsigned long long *)(MACHINE_TIMER_BASE + 0x08))
#define reg_mtimecmp_low  (*(volatile unsigned long *)(MACHINE_TIMER_BASE + 0x08))
#define reg_mtimecmp_high (*(volatile unsigned long *)(MACHINE_TIMER_BASE + 0x0c))

#endif /* end of __PLMT_REG_H_ */
