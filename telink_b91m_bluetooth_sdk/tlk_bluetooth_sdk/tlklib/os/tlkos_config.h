/********************************************************************************************************
 * @file    tlkos_config.h
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
#include "core/mcu_type.h"
#include "common/compiler.h"
#include "vendor/common/user_config.h"

#if !TLK_CFG_RTOS_ENABLE
#define TLKOS_CFG_BAREMETAL_ENABLE    1
#define TLKOS_CFG_FREERTOS_ENABLE     0
#else
#define TLKOS_CFG_BAREMETAL_ENABLE    0
#define TLKOS_CFG_FREERTOS_ENABLE     1
#endif 

#ifndef TLKOS_CFG_USE_LOWER_RAM_SIZE
#if MCU_CORE_TYPE == MCU_CORE_TL752X
#define TLKOS_CFG_USE_LOWER_RAM_SIZE  1
#else
#define TLKOS_CFG_USE_LOWER_RAM_SIZE  0
#endif
#endif

#ifndef TLKOS_CFG_BAREMETAL_HEAP_SIZE
#define TLKOS_CFG_BAREMETAL_HEAP_SIZE (8 * 1024)
#endif

#ifndef TLKOS_CFG_OS_HEAP_SIZE
#define TLKOS_CFG_OS_HEAP_SIZE        (30 * 1024)
#endif

#ifndef TLKOS_CFG_HEART_TIMER_TICK_HZ
#define TLKOS_CFG_HEART_TIMER_TICK_HZ (32000UL) //32768(ext_clock) 32000(internal_clock)
#endif 

#ifndef TLKOS_CFG_OS_TICK_HZ
#define TLKOS_CFG_OS_TICK_HZ          (1000)     //1ms
#endif

#ifndef TLKOS_CFG_PLIC_STACK_SIZE_WORD
#define TLKOS_CFG_PLIC_STACK_SIZE_WORD    (1024 * 1)
#endif

#ifndef TLKOS_CFG_DEBUG_ENABLE
#define TLKOS_CFG_DEBUG_ENABLE             (1 && TLK_CFG_RTOS_ENABLE)
#endif

#ifndef TLKOS_CFG_DEBUG_INFO_OUT
#define TLKOS_CFG_DEBUG_INFO_OUT           ((!TLKOS_CFG_USE_LOWER_RAM_SIZE) && TLKOS_CFG_DEBUG_ENABLE)
#endif
#define TLKOS_CFG_DEBUG_IO_ENABLE          (0 && TLKOS_CFG_DEBUG_ENABLE)
#define TLKOS_CFG_DEBUG_STACK_OVERFLOW     (0 && TLKOS_CFG_DEBUG_ENABLE)
#define TLKOS_CFG_DEBUG_MALLOC_FAIL        (1 && TLKOS_CFG_DEBUG_ENABLE)
#define TLKOS_CFG_DEBUG_CPU_USAGE          (0 && TLKOS_CFG_DEBUG_ENABLE)

#define TLKOS_CFG_TICKLESS_ENABLE     (TLK_CFG_SUSPEND_ENABLE)




#define TLKOS_CFG_CHECK_OS_ENABLE_NUM ((TLKOS_CFG_BAREMETAL_ENABLE) + (TLKOS_CFG_FREERTOS_ENABLE))

#if (TLKOS_CFG_CHECK_OS_ENABLE_NUM) != 1
    #error "TLK_OS_CFG_CHECK_OS_ENABLE_NUM NOT EQUAL TO 1"
#endif


#define _attribute_os_core_code_ram_sec_ __attribute__((section(".ram_code"))) __attribute__((optimize("O2"))) 
#define _attribute_os_core_code_flash_sec_ __attribute__((optimize("O2"))) 

#if MCU_DUAL_CORE_ENABLE
#define _attribute_os_heap_sec_ __attribute__((section(".iram_data"))) 
#else
#define _attribute_os_heap_sec_ 
#endif

#define TLKOS_ASSERT(x) 
