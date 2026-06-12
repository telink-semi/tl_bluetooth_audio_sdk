/********************************************************************************************************
 * @file    reg_em_base.h
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
#ifndef __REG_EM_BASE_H_
#define __REG_EM_BASE_H_

#include "driver.h"
#include "ext_bredr_reg.h"
//#include "bredr_ext.h"
#ifndef __INLINE
#define __INLINE static __attribute__((__always_inline__)) inline
#endif


#if EM_DLM_EN
#define REG_EM_BASE_ADDR ((DRAM_ADDR) | (EM_BASE_DRAM_OFFSET))
#else
#define REG_EM_BASE_ADDR ((IRAM_ADDR) | (EM_BASE_IRAM_OFFSET))
#endif
#define REG_MCU_EM_BASE_ADDR       REG_EM_BASE_ADDR


#define REG_BTCORE_SIZE            612
#define REG_BTCORE_BASE_ADDR       BTCORE_BASE_ADDR

#define REG_BTCORE_ESCO_SIZE       32

#define REG_EM_BT_CS_SIZE          100 //98
#define REG_EM_BT_CS_BASE_ADDR     REG_EM_BASE_ADDR

#define REG_EM_BT_RXDESC_SIZE      16 //14
#define REG_EM_BT_RXDESC_BASE_ADDR REG_EM_BASE_ADDR

#define REG_EM_BT_TXDESC_SIZE      12 //10
#define REG_EM_BT_TXDESC_BASE_ADDR REG_EM_BASE_ADDR

#define REG_EM_ET_SIZE             16
#define REG_EM_ET_BASE_ADDR        REG_EM_BASE_ADDR

#define REG_IPCORE_SIZE            244
#define REG_IPCORE_BASE_ADDR       0xd4160000

/// Exchange memory base address
#define EM_BASE_ADDR REG_EM_ET_BASE_ADDR


#endif // __REG_EM_BASE_H_
