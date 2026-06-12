/********************************************************************************************************
 * @file    tlkos_kernel_freertos.c
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
#include "tlklib/os/tlkos_config.h"

#if TLKOS_CFG_FREERTOS_ENABLE
#include "tlklib/os/3rd-party/freertos-V5/include/FreeRTOS.h"
#include "tlklib/os/3rd-party/freertos-V5/include/task.h"
#include "tlklib/os/tlkos_api/tlkos_define.h"

_attribute_data_retention_sec_ static volatile unsigned char  sCoreCriticalThrd  = 0;
_attribute_data_retention_sec_ static volatile unsigned short sCoreCriticalCount = 0;

_attribute_os_heap_sec_ uint8_t ucHeap[configTOTAL_HEAP_SIZE];

/**
 * @brief      Initialization task function that executes the user initialization callback
 *             and then destroys itself.
 * @param[in]  arg - Pointer to the initialization function callback (TlkOsInitFunc_t)
 * @returns    None
 * @note       This task is created during kernel startup to execute the user-provided
 *             initialization function, then terminates itself after completion.
 */
static void tlkos_init_task(void *arg)
{
    TlkOsInitFunc_t initFunc = (TlkOsInitFunc_t)arg;
    vTaskSuspendAll();
    initFunc();
    xTaskResumeAll();
    tlkos_task_destroy(NULL);
}

/**
 * @brief     Gets the current interrupt state.
 * @returns   TLKOS_IRQ_STATE_IN_IRQ if in interrupt context, TLKOS_IRQ_STATE_NOT_IN_IRQ otherwise.
 */
_always_inline int tlkos_get_irqState(void)
{
    extern unsigned int g_plic_switch_sp_flag;
    if (g_plic_switch_sp_flag != 0) {
        return TLKOS_IRQ_STATE_IN_IRQ;
    }
    return TLKOS_IRQ_STATE_NOT_IN_IRQ;
}

/**
 * @brief     Gets the os current kernel state.
 * @returns   Refer to TLKOS_KERNEL_STATE_ENUM.
 */
_always_inline int tlkos_get_kernelState(void)
{
    switch (xTaskGetSchedulerState()) {
    case taskSCHEDULER_SUSPENDED:
        return TLKOS_KERNEL_STATE_SUSPEND;
    case taskSCHEDULER_NOT_STARTED:
        return TLKOS_KERNEL_STATE_NOT_START;
    case taskSCHEDULER_RUNNING:
        return TLKOS_KERNEL_STATE_RUNNING;
    }
    return TLKOS_KERNEL_STATE_UNKNOWN;
}

/**
 * @brief     Enters a critical section.
 * @returns   None.
 */
_attribute_ram_code_sec_noinline_ void tlkos_enter_critical(void)
{
#if (MCU_CORE_N22 == 0)
    uint32_t mie_en = read_csr(NDS_MSTATUS) & FLD_MSTATUS_MIE;
    if (mie_en == 0) {
        return;
    }
    if (tlkos_get_irqState() == TLKOS_IRQ_STATE_NOT_IN_IRQ) {
        vTaskSuspendAll();
    }
    uint32_t      r    = core_interrupt_disable();
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
    uint32_t r = core_interrupt_disable();
    if (r == 0) {
        return;
    }
    if (sCoreCriticalCount == 1) {
        plic_set_threshold(sCoreCriticalThrd);
        sCoreCriticalThrd = 0;
    }
    if (sCoreCriticalCount != 0) {
        sCoreCriticalCount--;
    }
    core_restore_interrupt(r);
    if (tlkos_get_irqState() == TLKOS_IRQ_STATE_NOT_IN_IRQ) {
        xTaskResumeAll();
    }
#endif
}

/**
 * @brief     Initializes the kernel.
 * @returns   None.
 */
void tlkos_init(void)
{
    tlkos_mem_init();
#if (MCU_DUAL_CORE_ENABLE && defined(MCU_CORE_N22))
    mtime_clk_init(CLK_32K_RC);
#else
    core_mie_enable(FLD_MIE_MSIE);
    plic_sw_interrupt_enable();
#endif

    tlkos_debug_ioInit();
}

/**
 * @brief     Starts the kernel scheduler.
 * @param[in] initFunc init functions.
 * @returns   None.
 */
void tlkos_start(TlkOsInitFunc_t initFunc)
{
    if (initFunc != NULL) {
        tlkos_task_create(tlkos_init_task, "init task", 8 * 1024, configMAX_PRIORITIES - 1, initFunc, NULL, NULL);
    }
    vTaskStartScheduler();
}

#endif //TLKOS_CFG_FREERTOS_ENABLE
