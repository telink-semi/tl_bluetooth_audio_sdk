/********************************************************************************************************
 * @file    sc_bb_reg.h
 *
 * @brief   This is the header file for tl322x
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef SC_BB_REG_H_
#define SC_BB_REG_H_

#include "bit.h"
#include "soc.h"


#define N22_ILM_BASE       (0xd0000000)
#define N22_DLM_BASE       (0xd0080000)

#define SC_N22_BASE_ADDR   (0x80140380)

#define reg_n22_ra         REG_ADDR32(SC_N22_BASE_ADDR + 0x00)

#define reg_n22_sp         REG_ADDR32(SC_N22_BASE_ADDR + 0x04)

#define reg_n22_pc         REG_ADDR32(SC_N22_BASE_ADDR + 0x08)

#define reg_n22_mcause     REG_ADDR32(SC_N22_BASE_ADDR + 0x0c)

#define reg_n22_mepc       REG_ADDR32(SC_N22_BASE_ADDR + 0x10)

#define reg_n22_rst_vector REG_ADDR32(SC_N22_BASE_ADDR + 0x14)

#define reg_n22_ctrl0      REG_ADDR8(SC_N22_BASE_ADDR + 0x1a)

enum
{
    FLD_N22_ICACHE_DISABLE_INIT = BIT(0),
};

#define reg_n22_stall_ctrl REG_ADDR8(SC_N22_BASE_ADDR + 0x1b)

enum
{
    FLD_N22_WFI_MODE     = BIT(0),
    FLD_CORE_SLEEP_VALUE = BIT(1),
    FLD_RX_EVT           = BIT(2),
    FLD_TX_EVT           = BIT(3),
};

#define SC_BB_BASE_ADDR (0x80170c00)

#define reg_n22_rst     REG_ADDR16(SC_BB_BASE_ADDR + 0x18)
#define reg_n22_rst0    REG_ADDR8(SC_BB_BASE_ADDR + 0x18)

enum
{
    FLD_RST0_ZB     = BIT(2),
    FLD_RST0_ZB_PON = BIT(6),
    FLD_RST0_DMA_BB = BIT(7),
};

#define reg_n22_rst1 REG_ADDR8(SC_BB_BASE_ADDR + 0x19)

enum
{
    FLD_RST1_RSTL_BB     = BIT(2), // Clears RF state machine and some internal states of the link layer.
    FLD_RST1_RST_MDM     = BIT(3), // Clears all digital logic states related to mdm, the related configuration will not be lost after reset.
    FLD_RST1_RSTL_STIMER = BIT(4), // Resets BB STIMER status. RW registers remain, read-only registers will be cleared, and BB timer tick will be restarted.
};

#define reg_n22_clk_en  REG_ADDR16(SC_BB_BASE_ADDR + 0x1a)
#define reg_n22_clk_en0 REG_ADDR8(SC_BB_BASE_ADDR + 0x1a)

enum
{
    FLD_CLK0_ZB_HCLK_EN = BIT(2),
    FLD_CLK0_DMA_BB_EN  = BIT(7),
};

#define reg_n22_clk_en1 REG_ADDR8(SC_BB_BASE_ADDR + 0x1b)

enum
{
    FLD_CLK1_CLK_BB      = BIT(3),
    FLD_CLK1_CLKZB32K_LP = BIT(4),
};

#define reg_n22_clkzb32k_sel REG_ADDR8(SC_BB_BASE_ADDR + 0x1e)

enum
{
    FLD_CLKZB32K_SEL = BIT_RNG(0, 3),
};


#endif
