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
#ifndef COMPILER_H_
#define COMPILER_H_


#define _attribute_retention_code_        __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_iram_data_             __attribute__((section(".iram_data")))
#define _attribute_ram_code_sec_          __attribute__((section(".ram_code")))
#define _attribute_ram_code_sec_noinline_ __attribute__((section(".ram_code"))) __attribute__((noinline))
#if (CHIP_TYPE == CHIP_TYPE_TL752X)
#define _attribute_retention_ram_code_sec_noinline_ _attribute_ram_code_sec_noinline_
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
#define _attribute_ble_retention_code_
#define _attribute_ble_ram_code_
#define _attribute_ble_data_retention_
#else
#define _attribute_ble_retention_code_ __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_ble_ram_code_       __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_ble_data_retention_ __attribute__((section(".retention_data")))
#endif

#define _attribute_bt_retention_code_        __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_bt_ram_code_              __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_bt_data_retention_        __attribute__((section(".retention_data")))

#define _attribute_packed_                   __attribute__((packed))
#define _attribute_session_(s)               __attribute__((section(s)))
#define _attribute_custom_code_              _attribute_session_(".custom") volatile
#define _attribute_noinline_                 __attribute__((noinline))
#define _attribute_no_inline_                __attribute__((noinline))
#define _inline_                             inline
#define _attribute_data_dlm_                 _attribute_session_(".dlm_data") // dlm:Data Local Memory
#define _attribute_data_no_init_             __attribute__((section(".data_no_init")))
#define _attribute_data_sec_                 _attribute_session_(".data") // Force only read data to be stored in data segments to avoid compiler optimization


#define _attribute_iram_data_                __attribute__((section(".iram_data")))
#define _attribute_iram_noinit_data_         //__attribute__((section(".iram_noinit_data")))
#define _attribute_iram_bss_                 //__attribute__((section(".iram_bss")))

#define _attribute_data_retention_           __attribute__((section(".retention_data")))
#define _attribute_ram_code_                 __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_ram_code_without_oninline __attribute__((section(".ram_code")))

/* LE PHY (2.4G Use) */
#define _attribute_le_retention_code_ __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_le_ram_code_       __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_le_data_retention_ __attribute__((section(".retention_data")))
/* AUD */
#define _attribute_audio_retention_code_ __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_audio_ram_code_       __attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_audio_data_retention_ __attribute__((section(".retention_data")))

#define _attribute_text_code_            __attribute__((section(".text")))
/// define size of an empty array (used to declare structure with an array size not defined)
#define __ARRAY_EMPTY

/// define the static keyword for this compiler
#define __STATIC       static


#define _always_inline inline __attribute__((always_inline))
/// define the force inlining attribute for this compiler
#define __INLINE static __attribute__((__always_inline__)) inline
/**
 * No_execit must be added here for the following reasons: When compiling at the optimization level of -Os, link may use
 * exec.it for functions compiled at -O2. To disable this behavior, add -mno-exit to the linking phase (see Andes
 * Programming Guide), or add _attribute_((no_execit)) to functions that don't want to use exec.it.
 */
#define _attribute_ram_code_sec_optimize_o2_          __attribute__((section(".ram_code"))) __attribute__((optimize("O2"))) __attribute__((no_execit))

#define _attribute_ram_code_sec_optimize_o2_noinline_ __attribute__((noinline)) __attribute__((section(".ram_code"))) __attribute__((optimize("O2"))) __attribute__((no_execit))
#ifndef ASSERT_ERR
#define ASSERT_ERR(x) //do{if(!(x)) __asm__("ecall");}while(0)  //avoid others test problem�� only controller group test;
#endif
#endif
