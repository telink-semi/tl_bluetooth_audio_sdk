/********************************************************************************************************
 * @file    stimer_reg.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
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
#ifndef STIMER_REG_H_
#define STIMER_REG_H_

#include "soc.h"
/*******************************      sys clock registers: 0x140200       ******************************/
#define STIMER_BASE_ADDR          0x140200

#define reg_system_tick           REG_ADDR32(STIMER_BASE_ADDR)

#define reg_system_irq_level_d25f REG_ADDR32(STIMER_BASE_ADDR + 0x4)

#define reg_system_irq_level_n22  REG_ADDR32(STIMER_BASE_ADDR + 0x28)

#define reg_system_irq_level_dsp  REG_ADDR32(STIMER_BASE_ADDR + 0x2c)

#define reg_system_irq_mask       REG_ADDR8(STIMER_BASE_ADDR + 0x8)

#define reg_system_cal_irq        REG_ADDR8(STIMER_BASE_ADDR + 0x9)

typedef enum
{
    FLD_SYSTEM_IRQ_D25F = BIT(0), /**<
                                            When the current tick value of the system timer reaches the preset tick value,the IRQ_SYSTIMER interrupt is generated,
                                            This mask must be turned on if you want to trigger an interrupt.
                                            If you do not trigger an interrupt and only polling can detect the state, this mask also needs to be turned on.
                                            Get interrupt status:stimer_get_irq_status(),clear interrupt status:stimer_clr_irq_status().
                                            The interrupt flag bit needs to be manually cleared.
                                          */
    FLD_SYSTEM_IRQ_N22  = BIT(1), /**<
                                            When the current tick value of the system timer reaches the preset tick value,the IRQ_SYSTIMER interrupt is generated,
                                            This mask must be turned on if you want to trigger an interrupt.
                                            If you do not trigger an interrupt and only polling can detect the state, this mask also needs to be turned on.
                                            Get interrupt status:stimer_get_irq_status(),clear interrupt status:stimer_clr_irq_status().
                                            The interrupt flag bit needs to be manually cleared.
                                          */
    FLD_SYSTEM_IRQ_DSP  = BIT(2), /**<
                                            When the current tick value of the system timer reaches the preset tick value,the IRQ_SYSTIMER interrupt is generated,
                                            This mask must be turned on if you want to trigger an interrupt.
                                            If you do not trigger an interrupt and only polling can detect the state, this mask also needs to be turned on.
                                            Get interrupt status:stimer_get_irq_status(),clear interrupt status:stimer_clr_irq_status().
                                            The interrupt flag bit needs to be manually cleared.
                                          */
    FLD_SYSTEM_32K_IRQ  = BIT(3), /**<
                                            When the system timer completes the calibration of 32k timer,the IRQ_SYSTIMER interrupt is generated,
                                            This mask must be turned on if you want to trigger an interrupt.
                                            If you do not trigger an interrupt and only polling can detect the state, this mask also needs to be turned on.
                                            Get interrupt status:stimer_get_irq_status(),clear interrupt status:stimer_clr_irq_status().
                                            The interrupt flag bit needs to be manually cleared.
                                         <p>
                                            In Non-dma mode,the received data is read by stimer_get_tracking_32k_value().
                                          */
} stimer_irq_e;

#define reg_system_ctrl REG_ADDR8(STIMER_BASE_ADDR + 0xa)

enum
{
    FLD_SYSTEM_32K_WR_EN    = BIT(0),
    FLD_SYSTEM_TIMER_EN     = BIT(1),
    FLD_SYSTEM_TIMER_AUTO   = BIT(2),
    FLD_SYSTEM_32K_TRACK_EN = BIT(3),
    FLD_SYSTEM_32K_CAL_MODE = BIT_RNG(4, 7), //(2^(16-cal_32k_mode)) cycles of 32k clock

};

#define reg_system_st REG_ADDR8(STIMER_BASE_ADDR + 0xb)

enum
{
    //The register write operation directly writes the corresponding bit.
    //Can not read out the whole register, and then modify the bit to be written, and then write in.
    FLD_SYSTEM_CMD_STOP         = BIT(1), //W:write 1, stop system timer when using auto mode
    FLD_SYSTEM_CMD_SYNC         = BIT(3), //W:write 1, start 32k count write; R:st_list3(wr_busy)
    FLD_SYSTEM_CLK_32K          = BIT(4), //R
    FLD_SYSTEM_CLR_RD_DONE      = BIT(5), //W:write 1, clear read 32k update flag; R:st_list5(rd_done)
    FLD_SYSTEM_RD_BUSY          = BIT(6), //R
    FLD_SYSTEM_CMD_SET_DLY_DONE = BIT(7), //R
};

#define reg_system_timer_set_32k  REG_ADDR32(STIMER_BASE_ADDR + 0xc)

#define reg_system_timer_read_32k REG_ADDR32(STIMER_BASE_ADDR + 0x10)

#define reg_system_cal_latch_32k  REG_ADDR32(STIMER_BASE_ADDR + 0x14)

#define reg_system_up_32k         REG_ADDR8(STIMER_BASE_ADDR + 0x18)

enum
{

    FLD_SYSTEM_UPDATE_UPON_32K  = BIT(0),
    FLD_SYSTEM_RUN_UPON_NXT_32K = BIT(1),
};

#endif /* STIMER_REG_H_ */
