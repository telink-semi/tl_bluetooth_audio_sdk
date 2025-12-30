/********************************************************************************************************
 * @file    tlkmw_host.c
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
#include "stack/multiCoreComm/comm.h"
#include "tlkmw/tlkmw.h"
#include "tlkmw_test_mode.h"
#include "tlkmw/btble/tlkmdi_btble_btsnoop.h"
#include "tlkmw/dualcore/tlkmw_dualcore_boot.h"

#if (MCU_CORE_TYPE == CHIP_TYPE_TL751X)
#define N22_IRAM_ADDR 0x50020000
#define N22_DRAM_ADDR 0x50080000
#elif (MCU_CORE_TYPE == CHIP_TYPE_TL753X)
#define N22_IRAM_ADDR 0x50000000
#define N22_DRAM_ADDR 0x51000000
#elif (MCU_CORE_TYPE == CHIP_TYPE_TL322X)
#define N22_IRAM_ADDR 0x50000000
#define N22_DRAM_ADDR 0x50080000
#elif (MCU_CORE_TYPE == CHIP_TYPE_TL752X)//TODO
#define N22_IRAM_ADDR 0x50000000
#define N22_DRAM_ADDR 0x50080000
#define D25F_NO_CACHE_RAM_BIT 0x00000000
#define MCU_FLASH_ADDR_OFFSET 0x10000000
#endif
#ifndef D25F_NO_CACHE_RAM_BIT
#define D25F_NO_CACHE_RAM_BIT 0x80000000
#endif
#ifndef MCU_FLASH_ADDR_OFFSET
#define MCU_FLASH_ADDR_OFFSET 0x20000000
#endif


/**
 * @brief       This function gets the N22 flash address.
 * @param[in]   none.
 * @return      Returns the N22 flash address.
 */
static uint32_t tlkmw_host_getN22FlashAddr(void)
{
    uint32_t N22_Addr_from_flash = 0x00;
    N22_Addr_from_flash = tlkmw_getN22StartUpAddrFromFlash();
    if(N22_Addr_from_flash){
        return N22_Addr_from_flash;
    }else{
        return MCU_FLASH_ADDR_OFFSET + 0x100000;
    }
   
}

/**
 * @brief       This function starts the N22.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_host_startN22(void)
{
    (void) tlkmw_host_getN22FlashAddr;
#if (MCU_DUAL_CORE_ENABLE)
    sys_n22_init(N22_IRAM_ADDR);

    uint32_t addr = tlkmw_host_getN22FlashAddr();
    uint32_t n22_ilm_bin_size  = REG_ADDR32(addr + 0x08);
    uint32_t n22_dlm_bin_size  = REG_ADDR32(addr + 0x0c);
    uint32_t n22_dlm_lma_start = REG_ADDR32(addr + 0x10) + addr;
    uint32_t n22_dlm_vma_start = REG_ADDR32(addr + 0x14);

    tlkmw_dualcore_boot_cfg_t cfg = {
        .iram_dst_addr = N22_IRAM_ADDR,
        .iram_src_addr = addr,
        .iram_size = n22_ilm_bin_size,

        .dram_dst_addr = N22_DRAM_ADDR | n22_dlm_vma_start,
        .dram_src_addr = n22_dlm_lma_start,
        .dram_size = n22_dlm_bin_size,

        .no_cache_bit = D25F_NO_CACHE_RAM_BIT,
    };
    tlkmw_dualcore_boot(&cfg);

    sys_n22_start();
    int ret = tlk_multi_core_communication_init();
    if(ret != TLK_ENONE){
        tlk_printf("[ERROR]boot controller core fail,need check");
        return;
    }
    tlkmw_pm_enableControllerCoreSleepCheck(1);
#endif
}

/**
 * @brief       This function closes the N22.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_host_closeN22(void)
{
#if (MCU_DUAL_CORE_ENABLE)
#if (!MCU_CORE_TL752X_TEMP)//TODO
    pm_set_dig_module_power_switch(FLD_PD_ZB_EN, PM_POWER_DOWN);
#endif
    tlkmw_pm_enableControllerCoreSleepCheck(0);
#endif
}

/**
 * @brief       This function resets the controller.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_host_resetController(void)
{
    tlkmw_host_startN22();
}


/**
 * @brief       This function initializes the host module.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_host_init(void)
{
    tlkmw_test_mode_emi_check();
    tlkmw_host_resetController();
    tlkmw_test_mode_bqb_check();
    
    bluetooth_host_snoop_initial();
}

/**
 * @brief       This function closes the controller.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_host_closeController(void)
{
    tlkmw_host_closeN22();
}


/**
 * @brief       This function checks if C2H is pending.
 * @param[in]   none.
 * @return      Returns true if C2H is pending, otherwise false.
 */
__attribute__((weak)) bool tlkmw_host_isC2hPending(void)
{
    return false;
}



