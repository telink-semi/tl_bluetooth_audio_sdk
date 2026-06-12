/********************************************************************************************************
 * @file    tlkos_kernel_baremetal.c
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
#include "tlkapi/tlkapi.h"
#include "tlklib/os/tlkos_config.h"

#if TLKOS_CFG_BAREMETAL_ENABLE
#include "drivers.h"

_attribute_data_retention_sec_ static volatile unsigned char  sCoreCriticalThrd  = 0;
_attribute_data_retention_sec_ static volatile unsigned short sCoreCriticalCount = 0;

/**
 * @brief       This function initializes the internal timer for kernel.
 * @return      none.
 */
static void tlkos_timer_init_inner(void)
{
#if (MCU_CORE_N22 == 0)
#if (MCU_CORE_TL752X_TEMP)
    plic_set_priority(IRQ_TIMER0_1, 1);
#else
    plic_set_priority(IRQ_TIMER1, 1);
#endif

#if (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TL752X_TEMP)
    timer_set_irq_mask(FLD_TMR1_MODE_IRQ);
#endif
    timer_stop(TIMER1);
#if (!MCU_CORE_TL752X_TEMP)
    timer_set_init_tick(TIMER1, 0);
    timer_set_cap_tick(TIMER1, 2 * 1000 * sys_clk.pclk);
#else
    timer_init(TIMER0);
    timer_set_cap_tick(TIMER1, 2 * 1000 * sys_clk.pclk);
#endif
    timer_set_mode(TIMER1, TIMER_MODE_SYSCLK);
    timer_start(TIMER1);
#if (MCU_CORE_TL752X_TEMP)
    plic_interrupt_enable(IRQ_TIMER0_1);
#else
    plic_interrupt_enable(IRQ_TIMER1);
#endif
#endif
}

/**
 * @brief     Gets the current interrupt state.
 * @returns   TLKOS_IRQ_STATE_IN_IRQ if in interrupt context, TLKOS_IRQ_STATE_NOT_IN_IRQ otherwise.
 */
_always_inline int tlkos_get_irqState(void)
{
    return TLKOS_IRQ_STATE_UNKNOWN;
}

/**
 * @brief     Gets the os current kernel state.
 * @returns   Refer to TLKOS_KERNEL_STATE_ENUM.
 */
_always_inline int tlkos_get_kernelState(void)
{
    return TLKOS_KERNEL_STATE_UNKNOWN;
}

/**
 * @brief     Enters a critical section.
 * @returns   None.
 */
_attribute_ram_code_sec_noinline_ void tlkos_enter_critical(void)
{
#if (MCU_CORE_N22 == 0)
    unsigned int  r    = core_interrupt_disable();
    unsigned char thrd = reg_irq_threshold & 0xFF;
    sCoreCriticalCount++;
    if (sCoreCriticalCount == 1) {
        sCoreCriticalThrd = thrd;
    }
    if (thrd < 1) {
        plic_set_threshold(1);
    }
    core_restore_interrupt(r);
#endif
}

/**
 * @brief     Leaves a critical section.
 * @returns   None.
 */
_attribute_ram_code_sec_noinline_ void tlkos_leave_critical(void)
{
#if (MCU_CORE_N22 == 0)
    unsigned int r = core_interrupt_disable();
    if (sCoreCriticalCount == 1) {
        plic_set_threshold(sCoreCriticalThrd);
        sCoreCriticalThrd = 0;
    }
    if (sCoreCriticalCount != 0) {
        sCoreCriticalCount--;
    }
    core_restore_interrupt(r);
#endif
}

/**
 * @brief     Initializes the kernel.
 * @returns   None.
 */
void tlkos_init(void)
{
    tlkos_mem_init();
    tlkos_timer_init_inner();
}

/**
 * @brief     Starts the kernel scheduler.
 * @param[in] initFunc init functions.
 * @returns   None.
 */
void tlkos_start(TlkOsInitFunc_t initFunc)
{
    if (initFunc == NULL) {
        return;
    }
    initFunc();
}

#endif //TLKOS_CFG_FREERTOS_ENABLE
