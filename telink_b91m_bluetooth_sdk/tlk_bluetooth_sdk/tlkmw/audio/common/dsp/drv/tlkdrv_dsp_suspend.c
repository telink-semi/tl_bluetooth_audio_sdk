/********************************************************************************************************
 * @file    tlkdrv_dsp_suspend.c
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
#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "../tlkmw_audio_dsp_cfg.h"
#include "tlkmw/dualcore/tlkmw_dualcore_boot.h"
#include "tlkmw/userCtrl/tlkmw_user_common.h"

#if TLK_MW_DSP_COMM_ENABLE && DSP_SUPPORT_SUSPEND

enum
{
    TLKDRV_DSP_STATE_NOINIT = 0,
    TLKDRV_DSP_STATE_BOOTING,
    TLKDRV_DSP_STATE_RUNNING,
    TLKDRV_DSP_STATE_CRASH,
};

static uint8_t       sTlkdrvDspNowState  = TLKDRV_DSP_STATE_NOINIT;
static uint8_t       sTlkdrvDspWantState = TLKDRV_DSP_STATE_NOINIT;
static TlkApiTimer_t sTlkdrvDspTmr       = {0};
extern bool          dsp_rdy_first_flag;

/**
 * @brief  boot dsp core
 * @param[in]  none
 * @returns none
 */
static inline void tlkdrv_dsp_core_boot(void)
{
    sys_dsp_init(DSP_FW_DOWNLOAD_FLASH_ADDR);

#if DSP_USE_RAM_BOOT
    uint32_t addr = tlkmw_getDSPStartUpAddrFromFlash();
    if (addr == 0) {
        addr = 0x20200000;
    }
    // uint32_t dram_bin_begin = REG_ADDR32(addr + 0);
    uint32_t dram_bin_begin = 16;
    uint32_t dram_bin_size  = REG_ADDR32(addr + 4);

    // uint32_t iram_bin_begin = REG_ADDR32(addr + 8);
    uint32_t iram_bin_begin = 16 + dram_bin_size;
    uint32_t iram_bin_size  = REG_ADDR32(addr + 12);

    tlkmw_dualcore_boot_cfg_t cfg = {
        .iram_dst_addr = 0x2100000,
        .iram_src_addr = addr + iram_bin_begin,
        .iram_size     = iram_bin_size,

        .dram_dst_addr = 0x2000000,
        .dram_src_addr = addr + dram_bin_begin,
        .dram_size     = dram_bin_size,

        .no_cache_bit = 0x80000000,
    };
    tlkmw_dualcore_boot(&cfg);
#endif

    sys_dsp_start();
}

/**
 * @brief  enable dsp core irq
 * @param[in]  none
 * @returns none
 */
static inline void tlkdrv_dsp_core_enable_irq(void)
{
    mailbox_set_irq_mask(FLD_MAILBOX_DSP_TO_D25F_IRQ);
    plic_interrupt_enable(IRQ_MAILBOX_DSP_TO_D25);
}

/**
 * @brief  power down dsp
 * @param[in]  none
 * @returns none
 */
static inline void tlkdrv_dsp_power_down(void)
{
    reg_dsp_ctrl0 |= FLD_DSP_CTRL0_STALL;

    sys_dsp_clk_dis();
    reg_clk_en4 &= (~FLD_CLK4_DSP_EN);

    pm_set_dig_module_power_switch(FLD_PD_DSP_EN, PM_POWER_DOWN);
    dsp_rdy_first_flag = true;
}

/**
 * @brief  dsp boot protect
 * @param[in]  en - enable flag
 * @returns none
 */
static void tlkdrv_dsp_boot_protect(uint8_t en)
{
    if (en) {
        tlkmdi_tinySql_suspendSave();
        tlksys_pm_setChn(TLKSYS_PM_CHN_DSP, 0, 1);
    } else {
        tlkmdi_tinySql_resumeSave();
        tlksys_pm_setChn(TLKSYS_PM_CHN_DSP, 0, 0);
    }
}

/**
 * @brief  dsp timer callback function
 * @param[in]  handle - timer handle
 * @param[in]  userArg - user argument
 * @returns none
 */
static void tlkdrv_dsp_timer(TlkApiTimerHandle_t handle, void *userArg)
{
    (void)handle;
    (void)userArg;
    if (sTlkdrvDspNowState == TLKDRV_DSP_STATE_BOOTING) {
        tlkdrv_dsp_boot_protect(0);
        sTlkdrvDspNowState = TLKDRV_DSP_STATE_CRASH;
        tlkdrv_dsp_power_down();
        tlkapi_printf(DSP_DBG_LOG_ENABLE, "[DSP] ERROR: boot timeout!");
    }
}

/**
 * @brief  initialize dsp module
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_init(void)
{
    tlksys_timer_createStatic(TLKSYS_TASKID_AUDIO, &sTlkdrvDspTmr, 500 * 1000, false, tlkdrv_dsp_timer, NULL);
    tlkdrv_dsp_core_enable_irq();
    tlksys_pm_regChn(TLKSYS_PM_CHN_DSP);
    tlksys_pm_setChn(TLKSYS_PM_CHN_DSP, 0, 0);
}

/**
 * @brief  pause dsp module
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_pause(void)
{
    switch (sTlkdrvDspNowState) {
    case TLKDRV_DSP_STATE_RUNNING:
        tlkdrv_dsp_power_down();
        sTlkdrvDspNowState = TLKDRV_DSP_STATE_NOINIT;
        break;
    }
    sTlkdrvDspWantState = TLKDRV_DSP_STATE_NOINIT;
}

/**
 * @brief  resume dsp module
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_resume(void)
{
    switch (sTlkdrvDspNowState) {
    case TLKDRV_DSP_STATE_NOINIT:
        tlkdrv_dsp_boot_protect(1);
        tlkdrv_dsp_core_boot();
        sTlkdrvDspNowState = TLKDRV_DSP_STATE_BOOTING;
        tlksys_timer_start(TLKSYS_TASKID_AUDIO, &sTlkdrvDspTmr);
        break;
    }
    sTlkdrvDspWantState = TLKDRV_DSP_STATE_RUNNING;
}

/**
 * @brief  dsp boot ok callback function
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_bootOkCB(void)
{
    tlkdrv_dsp_boot_protect(0);
    sTlkdrvDspNowState = TLKDRV_DSP_STATE_RUNNING;
    tlksys_timer_stop(TLKSYS_TASKID_AUDIO, &sTlkdrvDspTmr);
    if (sTlkdrvDspWantState == TLKDRV_DSP_STATE_NOINIT) {
        tlkdrv_dsp_power_down();
        sTlkdrvDspNowState = TLKDRV_DSP_STATE_NOINIT;
    }
}

/**
 * @brief  check if dsp is working
 * @param[in]  none
 * @returns working status, true means working
 */
bool tlkdrv_dsp_isWorking(void)
{
    return sTlkdrvDspNowState == TLKDRV_DSP_STATE_RUNNING;
}

#endif