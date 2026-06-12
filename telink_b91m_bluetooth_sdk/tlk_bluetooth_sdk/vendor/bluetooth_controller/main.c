/********************************************************************************************************
 * @file    main.c
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
#include "tlkapp/tlkapp.h"


#if (BT_EMI_D25F_TEST == 1)
#define N22_FW_DOWNLOAD_FLASH_ADDR 0x10020000
#define N22_IRAM_STARTUP_ADDR      0x50000000
#define N22_DRAM_ADDR              0x50080000

unsigned int n22_ilm_bin_size  = 0;
unsigned int n22_dlm_bin_size  = 0;
unsigned int n22_dlm_lma_start = 0;
#else
/**
 * @brief       Overwrite this function to set platform(mcu) config.
 * @param[in]   none
 * @return      Platform initialization configuration structure
 */
const tlksys_hal_platform_init_cfg_t *tlksys_hal_port_getPlatformInitCfg(void)
{
    static const tlksys_hal_platform_init_cfg_t cfg = {
        .gpioCfg    = TLKSYS_HAL_INIT_GPIO_CFG_SHUTDOWN,
        .clockLevel = TLK_CFG_AUDIO_CLOCK_LEVEL,
    };
    return &cfg;
}
#endif

/**
 * @brief       This is main function
 * @param[in]   none
 * @return      Integer value indicating exit status (always 0 in this implementation)
 */
int main(void)
{
#if (BT_EMI_D25F_TEST == 1)
    sys_init();
    PLL0_BBPLL_768M_MCLK_192M_D25F_192M_N22_96M_DSP_192M_MSPI_48M
    sys_n22_init(N22_IRAM_STARTUP_ADDR);

    n22_ilm_bin_size  = REG_ADDR32(N22_FW_DOWNLOAD_FLASH_ADDR + 0x08);
    n22_dlm_bin_size  = REG_ADDR32(N22_FW_DOWNLOAD_FLASH_ADDR + 0x0c);
    n22_dlm_lma_start = REG_ADDR32(N22_FW_DOWNLOAD_FLASH_ADDR + 0x10) + N22_FW_DOWNLOAD_FLASH_ADDR;

    memcpy((unsigned int *)N22_IRAM_STARTUP_ADDR, (unsigned int *)N22_FW_DOWNLOAD_FLASH_ADDR, n22_ilm_bin_size);
    memcpy((unsigned int *)N22_DRAM_ADDR, (unsigned int *)n22_dlm_lma_start, n22_dlm_bin_size);
    sys_n22_start();
    while (1);
#else
    tlksys_init();
    tlksys_start(tlkapp_create_allTasks);

    while (1) {
#if !TLK_CFG_RTOS_ENABLE
        tlksys_handler();
#if ((MCU_CORE_TYPE != MCU_CORE_TL751X))
        void tlk_sys_main_loop(void);
        tlk_sys_main_loop();
#endif
#endif
    }
#endif
    return 0;
}
