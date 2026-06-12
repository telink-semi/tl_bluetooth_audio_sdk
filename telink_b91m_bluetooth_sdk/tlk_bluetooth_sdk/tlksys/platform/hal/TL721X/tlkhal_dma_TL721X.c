/********************************************************************************************************
 * @file    tlkhal_dma_TL721X.c
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
#include "../../api/tlkhal_api.h"
#include "drivers.h"
#if MCU_CORE_TYPE == CHIP_TYPE_TL721X

static uint32_t sTlkhalDmaMask = 0;

uint8_t tlkhal_dma_malloc_ex(uint8_t notAcceptChn0)
{
    uint8_t  pos = notAcceptChn0 == 0 ? 0 : 1;
    uint32_t r   = core_interrupt_disable();
    for (size_t i = pos; i < DMA_CNT; i++) {
        if (!(sTlkhalDmaMask & BIT(i))) {
            sTlkhalDmaMask |= BIT(i);
            core_restore_interrupt(r);
            return DMA0 + i;
        }
    }
    core_restore_interrupt(r);
    TLKHAL_ASSERT(0);
    return DMA_CNT;
}

uint8_t tlkhal_dma_malloc(void)
{
    return tlkhal_dma_malloc_ex(0);
}

void tlkhal_dma_free(uint8_t chn)
{
    TLKHAL_ASSERT(chn < DMA_CNT);
    if (sTlkhalDmaMask & BIT(chn)) {
        uint32_t r = core_interrupt_disable();
        sTlkhalDmaMask &= ~BIT(chn);
        core_restore_interrupt(r);
    }
}

uint32_t tlkhal_dma_getIdleNum(void)
{
    uint32_t cnt = 0;
    for (size_t i = 0; i < DMA_CNT; i++) {
        if (!(sTlkhalDmaMask & BIT(i))) {
            cnt++;
        }
    }
    return cnt;
}

uint32_t tlkhal_dma_getChnPoolMask(void)
{
    return sTlkhalDmaMask;
}

/**
 * @brief     Clears the irq of terminal count status for a specified DMA channel.
 * @param[in] dmaChn The DMA channel number to clear.
 * @returns   None.
 */
inline __attribute__((always_inline)) void tlkhal_dma_clearTcIrq(uint32_t dmaChn)
{
    TLKHAL_ASSERT(dmaChn <= DMA7);
    dma_clr_tc_irq_status(1 << dmaChn);
}

#endif
