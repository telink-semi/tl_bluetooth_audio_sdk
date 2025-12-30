/********************************************************************************************************
 * @file    timer_bb_reg.h
 *
 * @brief   This is the header file for tl322x
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
#ifndef TIMER_BB_REG_H
#define TIMER_BB_REG_H
#include "soc.h"

/*******************************      timer_bb registers: 0x80240100     ******************************/
#define TIMER_BB_BASE_ADDR 0x80240100
#define reg_tmr_bb_ctrl0   REG_ADDR8(TIMER_BB_BASE_ADDR)
#define reg_tmr_bb_ctrl16  REG_ADDR16(TIMER_BB_BASE_ADDR)
#define reg_tmr_bb_ctrl32  REG_ADDR32(TIMER_BB_BASE_ADDR)

enum
{
    FLD_TMR_BB0_MODE   = BIT(0),
    FLD_TMR_BB0_EN     = BIT(1),
    FLD_TMR_BB0_NOWRAP = BIT(2),
    FLD_TMR_BB1_MODE   = BIT(4),
    FLD_TMR_BB1_EN     = BIT(5),
    FLD_TMR_BB1_NOWRAP = BIT(6),
};

#define reg_tmr_bb_ctrl1 REG_ADDR8(TIMER_BB_BASE_ADDR + 0x01)

enum
{
    FLD_TMR_BB2_MODE   = BIT(0),
    FLD_TMR_BB2_EN     = BIT(1),
    FLD_TMR_BB2_NOWRAP = BIT(2),
};

#define reg_tmr_bb_int_mask REG_ADDR8(TIMER_BB_BASE_ADDR + 0x02)

enum
{
    FLD_TMR_BB0_INT_MASK = BIT(0),
    FLD_TMR_BB1_INT_MASK = BIT(1),
    FLD_TMR_BB2_INT_MASK = BIT(2),
};

#define reg_tmr_bb_int_sts REG_ADDR8(TIMER_BB_BASE_ADDR + 0x03)

enum
{
    FLD_TMR_BB0_INT_STATUS = BIT(0),
    FLD_TMR_BB1_INT_STATUS = BIT(1),
    FLD_TMR_BB2_INT_STATUS = BIT(2),
};

#define reg_tmr_bb0_capt   REG_ADDR32(TIMER_BB_BASE_ADDR + 0x04)
#define reg_tmr_bb1_capt   REG_ADDR32(TIMER_BB_BASE_ADDR + 0x08)
#define reg_tmr_bb2_capt   REG_ADDR32(TIMER_BB_BASE_ADDR + 0x0c)
#define reg_tmr_bb_capt(i) REG_ADDR32((TIMER_BB_BASE_ADDR + 0x04) + ((i) << 2))

#define reg_tmr_bb0_tick   REG_ADDR32(TIMER_BB_BASE_ADDR + 0x10)
#define reg_tmr_bb1_tick   REG_ADDR32(TIMER_BB_BASE_ADDR + 0x14)
#define reg_tmr_bb2_tick   REG_ADDR32(TIMER_BB_BASE_ADDR + 0x18)
#define reg_tmr_bb_tick(i) REG_ADDR32((TIMER_BB_BASE_ADDR + 0x10) + ((i) << 2))

#endif
