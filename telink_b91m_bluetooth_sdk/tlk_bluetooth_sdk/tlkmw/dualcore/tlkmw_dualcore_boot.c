/********************************************************************************************************
 * @file    tlkmw_dualcore_boot.c
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
#include "tl_common.h"
#include "drivers.h"
#include "tlkmw_dualcore_boot.h"

#if (MCU_DUAL_CORE_ENABLE)

/**
 * @brief       Copies IRAM and DRAM data from source to destination addresses for dual-core boot configuration.
 *              This function performs memory copy operations for both IRAM and DRAM regions using memcpy.
 *              The destination addresses are adjusted with a non-cache bit mask.
 * @param[in]   cfg  Pointer to the dual-core boot configuration structure containing source/destination
 *                   addresses and sizes for IRAM and DRAM regions.
 * @return      None
 * @note        This function uses standard memcpy for memory copying. If the platform supports DMA,
 *              it can be modified to use DMA copy instead for better performance.
 */
static void tlkmw_dualcore_memcpy(tlkmw_dualcore_boot_cfg_t *cfg)
{
    // Copy IRAM data from source to destination address
    memcpy((void *)(cfg->iram_dst_addr | cfg->no_cache_bit), (void *)cfg->iram_src_addr, cfg->iram_size);

    // Copy DRAM data from source to destination address
    memcpy((void *)(cfg->dram_dst_addr | cfg->no_cache_bit), (void *)cfg->dram_src_addr, cfg->dram_size);
}

/**
 * @brief       Boot the dual core system with the provided configuration.
 * @param[in]   cfg   Pointer to the dual core boot configuration structure.
 * @return      None
 * @note        This function is responsible for initializing and starting
 *              the secondary core using the given configuration parameters.
 */
void tlkmw_dualcore_boot(tlkmw_dualcore_boot_cfg_t *cfg)
{
    tlkmw_dualcore_memcpy(cfg);
}

#else
/**
 * @brief       Boot the dual core system with the provided configuration.
 * @param[in]   cfg   Pointer to the dual core boot configuration structure.
 * @return      None
 * @note        This function is responsible for initializing and starting
 *              the secondary core using the given configuration parameters.
 */
void tlkmw_dualcore_boot(tlkmw_dualcore_boot_cfg_t *cfg)
{
    (void)cfg;
}
#endif