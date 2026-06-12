/********************************************************************************************************
 * @file    compiler.h
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


#define _attribute_retention_code_        __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_iram_data_             __attribute__((section(".iram_data")))
#define _attribute_ram_code_sec_          __attribute__((section(".ram_code")))
#define _attribute_ram_code_sec_noinline_ __attribute__((section(".ram_code"))) __attribute__((noinline))
#if defined(MCU_CORE_TL752X_D25F) || defined(MCU_CORE_TL752X_N22)
#define _attribute_retention_ram_code_sec_noinline_ __attribute__((section(".retention_ram_code"))) __attribute__((noinline))
#define _attribute_retention_ram_code_sec_          __attribute__((section(".retention_ram_code")))
#define _attribute_retention_ram_data_sec_          __attribute__((section(".retention_data")))
#endif
#define _attribute_text_sec_                __attribute__((section(".text")))
#define _attribute_flash_code_sec_noinline_ __attribute__((section(".flash_code"))) __attribute__((noinline))
#define _attribute_aes_data_sec_            __attribute__((section(".aes_data")))
#define _attribute_data_retention_sec_      __attribute__((section(".retention_data")))

#define _attribute_aligned_(s)              __attribute__((aligned(s)))
#define _attribute_text_sec_optimize_o2_    __attribute__((section(".text"))) __attribute__((optimize("O2"))) __attribute__((noinline)) __attribute__((no_execit))

/// Pack a structure field
#define __PACKED __attribute__((__packed__))

/*******************************      BT/BLE Use     ******************************/
#ifdef PROJ_RAZER_GAME_CONTROLLER
#define _attribute_ble_data_retention_
#else
#define _attribute_ble_data_retention_ __attribute__((section(".retention_data")))
#endif

#define _attribute_bt_retention_code_ _attribute_retention_code_
#define _attribute_bt_ram_code_       _attribute_ram_code_sec_noinline_
#define _attribute_bt_data_retention_ _attribute_data_retention_sec_

#define _attribute_packed_            __attribute__((packed))
#define _attribute_session_(s)        __attribute__((section(s)))
#define _attribute_custom_code_       _attribute_session_(".custom") volatile
#define _attribute_noinline_          __attribute__((noinline))
#define _attribute_no_inline_         __attribute__((noinline))
#define _inline_                      inline
#define _attribute_data_dlm_          _attribute_session_(".dlm_data") // dlm:Data Local Memory
#define _attribute_data_no_init_      __attribute__((section(".data_no_init")))
#define _attribute_data_sec_          _attribute_session_(".data") // Force only read data to be stored in data segments to avoid compiler optimization

#define _attribute_iram_noinit_data_

#define _attribute_data_retention_           __attribute__((section(".retention_data")))
#define _attribute_ram_code_                 _attribute_ram_code_sec_noinline_
#define _attribute_ram_code_without_oninline _attribute_ram_code_sec_

/* LE PHY (2.4G Use) */
#define _attribute_le_retention_code_ _attribute_retention_code_
#define _attribute_le_ram_code_       _attribute_ram_code_sec_noinline_
#define _attribute_le_data_retention_ __attribute__((section(".retention_data")))

/// define size of an empty array (used to declare structure with an array size not defined)
#define __ARRAY_EMPTY

/// define the static keyword for this compiler
#define __STATIC       static


#define _always_inline inline __attribute__((always_inline))
/// define the force inlining attribute for this compiler
#define __INLINE static __attribute__((__always_inline__)) inline
#if defined(MCU_CORE_TL652X_CORE0) || defined(MCU_CORE_TL652X_CORE1)
#define _attribute_ram_code_sec_optimize_o2_          __attribute__((section(".ram_code"))) __attribute__((optimize("O2")))

#define _attribute_ram_code_sec_optimize_o2_noinline_ __attribute__((noinline)) __attribute__((section(".ram_code"))) __attribute__((optimize("O2")))
#else
/**
 * No_execit must be added here for the following reasons: When compiling at the optimization level of -Os, link may use
 * exec.it for functions compiled at -O2. To disable this behavior, add -mno-exit to the linking phase (see Andes
 * Programming Guide), or add _attribute_((no_execit)) to functions that don't want to use exec.it.
 */
#define _attribute_ram_code_sec_optimize_o2_          __attribute__((section(".ram_code"))) __attribute__((optimize("O2"))) __attribute__((no_execit))

#define _attribute_ram_code_sec_optimize_o2_noinline_ __attribute__((noinline)) __attribute__((section(".ram_code"))) __attribute__((optimize("O2"))) __attribute__((no_execit))
#endif
#ifndef ASSERT_ERR
#define ASSERT_ERR(x) //do{if(!(x)) __asm__("ecall");}while(0)  //avoid others test problem�� only controller group test;
#endif
