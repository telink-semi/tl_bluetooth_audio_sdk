/********************************************************************************************************
 * @file    tlkdbg_gsu.c
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
#include "../../../tlkdbg_cfg.h"
#include "../private/tlkdbg_sync_stream_inner.h"

#if (TLKDBG_CFG_GSU_LOG_ENABLE)

#define TLKDBG_GSU_LOG_BAUD_RATE 1000000
static uint8_t sTlkDbgGsuIsInit = 0;

static void tlkdbg_gsu_putchar(uint8_t byte)
{
    uint16_t bits10 = (((uint16_t)byte) << 1) | 0x0200;
    // uint32_t irq = core_interrupt_disable();
    uint32_t time1 = clock_time();
    for (size_t index = 0; index < 10; index++) {
        uint32_t time2 = time1;
        while (time1 - time2 < SYSTEM_TIMER_TICK_1S / TLKDBG_GSU_LOG_BAUD_RATE) {
            time1 = clock_time();
        }
        uint8_t level = (bits10 & (1 << index)) ? 1 : 0;
        gpio_set_level(TLKDBG_CFG_GSU_LOG_PIN, level);
    }
    // core_restore_interrupt(irq);
}

static void tlkdbg_gsu_init_core(void)
{
    gpio_function_en(TLKDBG_CFG_GSU_LOG_PIN);
    gpio_set_up_down_res(TLKDBG_CFG_GSU_LOG_PIN, GPIO_PIN_PULLUP_1M);
    gpio_output_en(TLKDBG_CFG_GSU_LOG_PIN);
    gpio_set_high_level(TLKDBG_CFG_GSU_LOG_PIN);
}

void tlkdbg_gsu_write(uint8_t type, void *pData, uint16_t dataLen)
{
    (void)type;
    if (sTlkDbgGsuIsInit == 0) {
        tlkdbg_gsu_init_core();
        sTlkDbgGsuIsInit = 1;
    }
    uint8_t *buf = (uint8_t *)pData + TLK_DEBUG_LOG_CACHE_HEAD_RESERVE;
    for (uint16_t index = 0; index < dataLen - TLK_DEBUG_LOG_CACHE_RESERVE_LEN; index++) {
        tlkdbg_gsu_putchar(buf[index]);
    }
    tlkdbg_gsu_putchar('\r');
    tlkdbg_gsu_putchar('\n');
}

#endif
