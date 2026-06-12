/********************************************************************************************************
 * @file    tlkmw_dualcore_boot.h
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

typedef struct
{
    uint32_t iram_src_addr;
    uint32_t iram_dst_addr;
    uint32_t iram_size;
    uint32_t dram_src_addr;
    uint32_t dram_dst_addr;
    uint32_t dram_size;
    uint32_t no_cache_bit;
    uint8_t  is_iram_no_used;
    uint8_t  is_dram_no_used;
} tlkmw_dualcore_boot_cfg_t;

/**
 * @brief       Boot the dual core system with the provided configuration.
 * @param[in]   cfg   Pointer to the dual core boot configuration structure.
 * @return      None
 * @note        This function is responsible for initializing and starting
 *              the secondary core using the given configuration parameters.
 */
void tlkmw_dualcore_boot(tlkmw_dualcore_boot_cfg_t *cfg);
