/********************************************************************************************************
 * @file    tlkdrv_dsp_nosuspend.c
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
#include "../tlkmw_audio_dsp_cfg.h"
#include "tlkmw/dualcore/tlkmw_dualcore_boot.h"
#include "tlkmw/userCtrl/tlkmw_user_common.h"

#if TLK_MW_DSP_COMM_ENABLE && !DSP_SUPPORT_SUSPEND

enum
{
    TLKDRV_DSP_STATE_NOINIT = 0,
    TLKDRV_DSP_STATE_BOOTING,
    TLKDRV_DSP_STATE_RUNNING,
    TLKDRV_DSP_STATE_PAUSING,
    TLKDRV_DSP_STATE_PAUSED,
    TLKDRV_DSP_STATE_CRASH,
};

static uint8_t       sTlkdrvDspState = TLKDRV_DSP_STATE_NOINIT;
static TlkApiTimer_t sTlkdrvDspTmr   = {0};

/**
 * @brief  boot dsp core
 * @param[in]  none
 * @returns none
 */
static inline void tlkdrv_dsp_core_boot(void)
{
    sys_dsp_init(DSP_FW_DOWNLOAD_FLASH_ADDR);

    uint32_t addr = tlkmw_getDSPStartUpAddrFromFlash();
    if (addr == 0) {
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
        addr = 0x10200000;
#else
        addr = 0x20200000;
#endif
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
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
        .no_cache_bit = 0xb0000000,
#else
        .no_cache_bit = 0x80000000,
#endif
    };
    tlkmw_dualcore_boot(&cfg);

    sys_dsp_start();
    sTlkdrvDspState = TLKDRV_DSP_STATE_BOOTING;
}

/**
 * @brief  enable dsp core irq
 * @param[in]  none
 * @returns none
 */
_attribute_ram_code_sec_ static inline void tlkdrv_dsp_core_enable_irq(void)
{
    mailbox_set_irq_mask(FLD_MAILBOX_DSP_TO_D25F_IRQ);
#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
    plic_interrupt_enable(IRQ_IPCM);
#else
    plic_interrupt_enable(IRQ_MAILBOX_DSP_TO_D25);
#endif
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
    if (sTlkdrvDspState != TLKDRV_DSP_STATE_PAUSING) {
        return;
    }
    sys_dsp_clk_dis();
    sTlkdrvDspState = TLKDRV_DSP_STATE_PAUSED;
}

/**
 * @brief  initialize dsp module
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_init(void)
{
    tlkdrv_dsp_core_boot();
    tlkdrv_dsp_core_enable_irq();
    tlksys_timer_createStatic(TLKSYS_TASKID_AUDIO, &sTlkdrvDspTmr, 300 * 1000, false, tlkdrv_dsp_timer, NULL);
}

/**
 * @brief  pause dsp module
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_pause(void)
{
    if (sTlkdrvDspState != TLKDRV_DSP_STATE_RUNNING) {
        return;
    }
    sTlkdrvDspState = TLKDRV_DSP_STATE_PAUSING;
    tlksys_timer_reStart(TLKSYS_TASKID_AUDIO, &sTlkdrvDspTmr);
}

/**
 * @brief  resume dsp module
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_resume(void)
{
    if (sTlkdrvDspState == TLKDRV_DSP_STATE_PAUSED) {
        sys_dsp_clk_en();
        sTlkdrvDspState = TLKDRV_DSP_STATE_RUNNING;
    } else if (sTlkdrvDspState == TLKDRV_DSP_STATE_PAUSING) {
        tlksys_timer_stop(TLKSYS_TASKID_AUDIO, &sTlkdrvDspTmr);
        sTlkdrvDspState = TLKDRV_DSP_STATE_RUNNING;
    }
}

/**
 * @brief  dsp boot ok callback function
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_bootOkCB(void)
{
    sTlkdrvDspState = TLKDRV_DSP_STATE_RUNNING;
    sys_dsp_clk_dis();
    sTlkdrvDspState = TLKDRV_DSP_STATE_PAUSED;
}

/**
 * @brief  check if dsp is working
 * @param[in]  none
 * @returns working status, true means working
 */
bool tlkdrv_dsp_isWorking(void)
{
    return sTlkdrvDspState == TLKDRV_DSP_STATE_RUNNING;
}
#endif
