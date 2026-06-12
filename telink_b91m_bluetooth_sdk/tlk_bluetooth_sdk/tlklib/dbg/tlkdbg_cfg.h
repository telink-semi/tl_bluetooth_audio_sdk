/********************************************************************************************************
 * @file    tlkdbg_cfg.h
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

#ifndef TLK_DEBUG_LOG_MEM_POOL_SIZE
#if MCU_DUAL_CORE_ENABLE
#define TLK_DEBUG_LOG_MEM_POOL_SIZE (4 * 1024)
#else
#define TLK_DEBUG_LOG_MEM_POOL_SIZE (2 * 1024)
#endif
#endif

#ifndef TLK_DEBUG_LOG_MEM_POOL_SECTION
#define TLK_DEBUG_LOG_MEM_POOL_SECTION _attribute_extend_low_speed_bss_sec_
#endif

#define TLK_DEBUG_LOG_CACHE_SIZE         256
#define TLK_DEBUG_LOG_CACHE_HEAD_RESERVE 12
#define TLK_DEBUG_LOG_CACHE_TAIL_RESERVE 8

#define TLK_DEBUG_LOG_CACHE_RESERVE_LEN  (TLK_DEBUG_LOG_CACHE_HEAD_RESERVE + TLK_DEBUG_LOG_CACHE_TAIL_RESERVE)

#define TLKDBG_USBLOG_ITEM_SIZE          146

#define TLK_DEBUG_ASYNC_LOG_ENABLE       (TLKDBG_CFG_HPU_LOG_ENABLE || TLKDBG_CFG_UDB_LOG_ENABLE || TLKDBG_CFG_HWU_LOG_ENABLE)

#define TLK_DEBUG_SYNC_LOG_ENABLE        (TLK_SM_LOG_ENABLE || TLKDBG_CFG_GSU_LOG_ENABLE || 0)
