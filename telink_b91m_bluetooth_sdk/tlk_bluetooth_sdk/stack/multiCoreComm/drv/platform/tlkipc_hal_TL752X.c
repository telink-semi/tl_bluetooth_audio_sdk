/********************************************************************************************************
 * @file    tlkipc_hal_TL752X.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "drivers.h"
#include "tlkipc_port.h"

#define TLK_STD_MAILBOX_MODE (MCU_CORE_TYPE != MCU_CORE_TL752X)

#if (MCU_DUAL_CORE_ENABLE && !TLK_STD_MAILBOX_MODE)

#define TLK_MAILBOX_DRV_CAPACITY 4

void tlkipc_hal_mailbox_init(void)
{
#if defined(MCU_CORE_N22)
    mailbox_set_irq_mask(FLD_MAILBOX_D25F_TO_N22_IRQ);
    clic_set_priority(IRQ_IPCM, IRQ_PRI_LEV3);
    clic_interrupt_vector_en(IRQ_IPCM);
    clic_interrupt_enable(IRQ_IPCM);
#else
    mailbox_set_irq_mask(FLD_MAILBOX_N22_TO_D25F_IRQ);
    plic_set_priority(IRQ_IPCM, 3);
    plic_interrupt_enable(IRQ_IPCM);
#endif
}

_attribute_ram_code_sec_ void tlkipc_hal_mailbox_send_by_reg(void *pData)
{
#ifdef MCU_CORE_N22
    mailbox_n22_set_d25f_msg((unsigned int *)pData);
#else
    mailbox_d25f_set_n22_msg((unsigned int *)pData);
#endif
}

_attribute_ram_code_sec_ void tlkipc_hal_mailbox_send(void *pData, uint32_t dataLen)
{
    if (dataLen > TLK_MAILBOX_DRV_CAPACITY) {
        tlkipc_hal_mailbox_send_by_ram(pData, dataLen);
    }
    tlkipc_hal_mailbox_send_by_reg(pData);
}

_attribute_ram_code_sec_ uint8_t tlkipc_hal_is_mailbox_busy(void)
{
#ifdef MCU_CORE_N22
    uint32_t intr_status = IPCM->CPU0_INTR_STATUS0;
    if (intr_status & (1 << 0)) {
        return true;
    }
#else
    uint32_t intr_status = IPCM->CPU1_INTR_STATUS0;
    if (intr_status & (1 << 0)) {
        return true;
    }
#endif
    return false;
}

#ifdef MCU_CORE_N22
_attribute_ram_code_sec_ void tlk_controller_core_ipc_irq_handler(void)
{
    if ((mailbox_get_irq_status() & FLD_MAILBOX_D25F_TO_N22_IRQ) == 0) {
        return;
    }
    uint32_t msg[2] = {0};
    mailbox_n22_get_d25f_msg((unsigned int *)msg);
    uint8_t isFirst = tlkipc_mailbox_first_receive_check_hook(msg[0]);
    if (!isFirst) {
        tlkipc_hal_mailbox_get_by_ram(msg);
        tlkipc_mailbox_receive_hook((uint8_t *)msg);
    }
    mailbox_clr_irq_status(FLD_MAILBOX_D25F_TO_N22_IRQ);
}
CLIC_ISR_REGISTER(tlk_controller_core_ipc_irq_handler, IRQ_IPCM)

#else
_attribute_ram_code_sec_ void tlk_main_core_ipc_irq_handler(void)
{
    if ((mailbox_get_irq_status() & FLD_MAILBOX_N22_TO_D25F_IRQ) == 0) {
        return;
    }
    uint32_t msg[2] = {0};
    tlkipc_hal_mailbox_get_by_ram(msg);
    uint8_t isFirst = tlkipc_mailbox_first_receive_check_hook(msg[0]);
    if (!isFirst) {
        tlkipc_mailbox_receive_hook((uint8_t *)msg);
    }
    mailbox_clr_irq_status(FLD_MAILBOX_N22_TO_D25F_IRQ);
}
#endif

#endif
