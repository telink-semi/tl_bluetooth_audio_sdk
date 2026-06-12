/********************************************************************************************************
 * @file    app_irq.c
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
#include "app_config.h"
#include "app.h"
#include "stack/tpsll/tpsll.h"
#include "stack/btble.h"

#if (CONTROLLER_MODE == BTTPSLL_TWS)
struct TRAP_VAL
{
    uint32_t mcause;
    uint32_t mtval;
    uint32_t mepc;
    uint32_t mstatus;
    uint32_t mdcause;
    uint32_t ra;
};

volatile struct TRAP_VAL trap_value;

/**
 * @brief       Handler for application traps/exceptions
 * @param[in]   mtval   - Machine trap value register
 * @param[in]   mepc    - Machine exception program counter
 * @param[in]   mstatus - Machine status register
 * @param[in]   mcause  - Machine cause register
 * @param[in]   mdcause - Machine debug cause register
 * @param[in]   ra      - Return address
 * @return      0 - Always returns zero
 */
_attribute_ram_code_sec_ uint8_t app_trap_handler(uint32_t mtval, uint32_t mepc, uint32_t mstatus, uint32_t mcause, uint32_t mdcause, uint32_t ra)
{
    trap_value.mcause  = mcause;
    trap_value.mtval   = mtval;
    trap_value.mepc    = mepc;
    trap_value.mstatus = mstatus;
    trap_value.mdcause = mdcause;
    trap_value.ra      = ra;

    tlkapi_printf(1, "*** app_trap_handler *** %d %d %x %d", trap_value.mcause, trap_value.mtval, trap_value.mepc, trap_value.ra);
    tlkapi_send_string_u32s(1, "*** error code mark ***", 0xFFFFFFFF);

    core_interrupt_disable();
    while (1) {
#if (TLK_CFG_USB_ENABLE && TLKDBG_CFG_UDB_LOG_ENABLE)
        tlk_udb_usb_handle_irq();
#endif
        //         DBG_JUNWEI_CHN9_TOGGLE;
    }
    return 0;
}

/**
 * @brief       Entry point for trap/interrupt handling
 * @param[in]   none
 * @return      none
 */
_attribute_ram_code_sec_noinline_ void trap_entry(void) __attribute__((interrupt("machine"), aligned(64)));

/**
 * @brief       Entry point for trap/interrupt handling
 * @param[in]   none
 * @return      none
 */
_attribute_ram_code_sec_ void trap_entry(void)
{
#if (TLK_DEBUG_ENABLE)
    register uint32_t ra asm("x1");

    uint32_t mtval, mepc, mstatus, mcause, mdcause;
    mtval   = read_csr(NDS_MTVAL);
    mepc    = read_csr(NDS_MEPC);
    mstatus = read_csr(NDS_MSTATUS);
    mcause  = read_csr(NDS_MCAUSE);
    mdcause = read_csr(NDS_MDCAUSE);

    app_trap_handler(mtval, mepc, mstatus, mcause, mdcause, ra);
#else
    mcu_reboot();
#endif
}

/**
 * @brief		System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_retention_code_ void stimer_irq_handler(void)
{
    DBG_CHN15_HIGH;

    tlk_sys_irq_handler(IRQ_SYSTIMER);

    DBG_CHN15_LOW;
}
CLIC_ISR_REGISTER(stimer_irq_handler, IRQ_SYSTIMER)

/**
 * @brief		BLE RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_retention_code_ void ble_rf_irq_handler(void)
{
    DBG_CHN14_HIGH;

    tlk_sys_irq_handler(IRQ_ZB_RT);

    DBG_CHN14_LOW;
}
CLIC_ISR_REGISTER(ble_rf_irq_handler, IRQ_ZB_RT)

/**
 * @brief       TPSLL baseband timer interrupt handler
 * @param[in]   none
 * @return      none
 */
_attribute_retention_code_ void tlk_tpsll_bbtimer_irq_handler(void)
{
    tlk_tpsll_tpt_bbtimer_irq_handler();
}
CLIC_ISR_REGISTER(tlk_tpsll_bbtimer_irq_handler, IRQ_TIMER_BB)

/**
 * @brief       BT RF interrupt handler
 * @param[in]   none
 * @return      none
 */
_attribute_retention_code_ void bt_rf_irq_handler(void)
{
    tlk_sys_irq_handler(IRQ_ZB_BT);
}
CLIC_ISR_REGISTER(bt_rf_irq_handler, IRQ_ZB_BT)


#endif
