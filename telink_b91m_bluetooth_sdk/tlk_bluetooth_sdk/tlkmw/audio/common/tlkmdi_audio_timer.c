/********************************************************************************************************
 * @file    tlkmdi_audio_timer.c
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
#include "tlkapi/tlkapi.h"
#if (TLK_MW_AUDIO_ENABLE)

#if (MCU_CORE_TYPE == MCU_CORE_TL721X||MCU_CORE_TYPE == MCU_CORE_TL322X ||MCU_CORE_TYPE == MCU_CORE_TL752X)
#define TMR_IRQ_BIT FLD_TMR0_MODE_IRQ
#else
#define TMR_IRQ_BIT TMR_STA_TMR0
#endif

/**
 * @brief       This function is used to start audio timer
 * @param[in]   None.
 * @return      None.
 */
_attribute_ram_code_sec_ void tlkmdi_audio_start_timer(void)
{
    tlkhal_timer_cfg_t pCfg = {
        .chn = TIMER0,
    };
    tlkhal_timer_start(&pCfg);
}

/**
 * @brief       This function is used to stop audio timer
 * @param[in]   None.
 * @return      None.
 */
_attribute_ram_code_sec_ void tlkmdi_audio_stop_timer(void)
{
    tlkhal_timer_cfg_t pCfg = {
        .chn = TIMER0,
    };
    tlkhal_timer_stop(&pCfg);
    timer_clr_irq_status(TMR_IRQ_BIT);
}

/**
 * @brief       This function is used to set audio timer capture tick
 * @param[in]   cap_tick - Capture tick value
 * @return      None.
 */
_attribute_ram_code_sec_ void tlkmdi_audio_set_timer(uint32_t cap_tick)
{
#if(MCU_CORE_TL752X_TEMP)
//    timer_set_init_tick(TIMER0, 0);
    timer_init(TIMER0);
    timer_set_clk(16000000);
    timer_set_cap_tick(TIMER0, cap_tick * sys_clk.pclk);
#else
    timer_set_init_tick(TIMER0, 0);
    timer_set_cap_tick(TIMER0, cap_tick * sys_clk.pclk);
#endif
    tlkhal_timer_cfg_t pCfg = {
        .chn = TIMER0,
        .mode = TIMER_MODE_SYSCLK,
    };
    tlkhal_timer_set_mode(&pCfg);
}

/**
 * @brief       This function is used to set next interrupt for audio task
 * @param[in]   tus - Time value in microseconds
 * @return      None.
 */
_attribute_ram_code_sec_ void tlkmdi_audio_task_set_next_irq(uint32_t tus)
{
    tlkhal_timer_cfg_t pCfg = {
        .chn = TIMER0,
        .mode = TIMER_MODE_SYSCLK,
    };
    tlkhal_timer_stop(&pCfg);
#if !MCU_CORE_TL752X_TEMP
    timer_set_init_tick(TIMER0, 0);
    timer_set_cap_tick(TIMER0, tus * sys_clk.pclk);
#else
    timer_init(TIMER0);
    timer_set_clk(16000000);
    timer_set_cap_tick(TIMER0, tus * sys_clk.pclk);
#endif
    tlkhal_timer_set_mode(&pCfg);
    tlkhal_timer_start(&pCfg);
}

/**
 * @brief       This function is used to setup and start audio timer
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audio_setup_and_start_timer(void)
{
#if MCU_CORE_TL752X_TEMP
    plic_interrupt_enable(IRQ_TIMER0_0);
    plic_set_priority(IRQ_TIMER0_0, 1);
#else
    plic_interrupt_enable(IRQ_TIMER0);
    plic_set_priority(IRQ_TIMER0, 1);
#endif
    tlkmdi_audio_task_set_next_irq(1500);
}


#endif // #if (TLK_MW_AUDIO_ENABLE)
