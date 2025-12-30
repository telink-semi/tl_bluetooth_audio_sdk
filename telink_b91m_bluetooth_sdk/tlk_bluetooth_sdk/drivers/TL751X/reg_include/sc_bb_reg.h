/********************************************************************************************************
 * @file    sc_bb_reg.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
 * @date    2024
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

/**
 * @brief   The RAM address of the N22.
 * --------------------------------------------------
 *            |           Address         |   Size   
 * --------------------------------------------------
 * N22  | ILM |  0x50020000 ~ 0x50080000  |  384 KB
 *      | DLM |  0x50080000 ~ 0x500a0000  |  128 KB
 * --------------------------------------------------
 */
#define N22_ILM_BASE       (0x50020000)
#define N22_DLM_BASE       (0x50080000)

#define SC_BB_BASE_ADDR    (0xD4400000)

#define reg_n22_ra         REG_ADDR32(SC_BB_BASE_ADDR + 0x00)

#define reg_n22_sp         REG_ADDR32(SC_BB_BASE_ADDR + 0x04)

#define reg_n22_pc         REG_ADDR32(SC_BB_BASE_ADDR + 0x08)

#define reg_n22_mcause     REG_ADDR32(SC_BB_BASE_ADDR + 0x0c)

#define reg_n22_mepc       REG_ADDR32(SC_BB_BASE_ADDR + 0x10)

#define reg_n22_rst_vector REG_ADDR32(SC_BB_BASE_ADDR + 0x14)

#define reg_n22_rst        REG_ADDR16(SC_BB_BASE_ADDR + 0x18)
#define reg_n22_rst0       REG_ADDR8(SC_BB_BASE_ADDR + 0x18)

enum
{
    FLD_RST0_N22_CORE  = BIT(0),
    FLD_RST0_N22_LM    = BIT(1),
    FLD_RST0_ZB        = BIT(2),
    FLD_RST0_ZB_MSTCLK = BIT(3),
    FLD_RST0_ZB_LPCLK  = BIT(4),
    FLD_RST0_ZB_CRYPT  = BIT(5),
    FLD_RST0_DMA_BB    = BIT(7),
};

#define reg_n22_rst1 REG_ADDR8(SC_BB_BASE_ADDR + 0x19)

enum
{
    FLD_RST1_N22_RAMCRC = BIT(0),
    FLD_RST1_RST_BB     = BIT(2),
    FLD_RST0_TIMER_BB   = BIT(3),
    FLD_RST0_MDM_PON    = BIT(4),
};

#define reg_n22_clk_en  REG_ADDR16(SC_BB_BASE_ADDR + 0x1a)
#define reg_n22_clk_en0 REG_ADDR8(SC_BB_BASE_ADDR + 0x1a)

enum
{
    FLD_CLK0_N22_CORE_EN = BIT(0),
    FLD_CLK0_N22_LM_EN   = BIT(1),
    FLD_CLK0_ZB_HCLK_EN  = BIT(2),
    FLD_CLK0_ZB_MSTCLK   = BIT(3),
    FLD_CLK0_ZB_LPCLK    = BIT(4),
    FLD_CLK0_DMA_BB_EN   = BIT(7),
};

#define reg_n22_clk_en1 REG_ADDR8(SC_BB_BASE_ADDR + 0x1b)

enum
{
    FLD_CLK0_MTIMER   = BIT(1),
    FLD_CLK0_CLK_BB   = BIT(2),
    FLD_CLK0_TIMER_BB = BIT(3),
};

#define reg_n22_ctrl0 REG_ADDR8(SC_BB_BASE_ADDR + 0x1c)

enum
{
    FLD_N22_ICACHE_DISABLE_INIT = BIT(0),
};

#define reg_n22_clkmod REG_ADDR8(SC_BB_BASE_ADDR + 0x1d)

enum
{
    FLD_ZB_MST_MOD = BIT_RNG(0, 3),
};

#define reg_n22_clkzb32k_sel REG_ADDR8(SC_BB_BASE_ADDR + 0x1e)

enum
{
    FLD_CLKZB32K_SEL = BIT_RNG(0, 3),
    FLD_CLKBB_SEL    = BIT(4),
};

#define N22_SRAM_EMA_REG_ADDR (SC_BB_BASE_ADDR + 0x20)
#define N22_SRAM_EMA_DATA_LEN 2

#define reg_n22_sram_cfg      REG_ADDR16(N22_SRAM_EMA_REG_ADDR)
#define reg_n22_sram_cfg0     REG_ADDR8(N22_SRAM_EMA_REG_ADDR)

enum
{
    FLD_N22_SRAM_EMA  = BIT_RNG(0, 2),
    FLD_N22_SRAM_EMAW = BIT_RNG(3, 4),
    FLD_N22_SRAM_EMAS = BIT(5),
};

#define reg_n22_sram_cfg1 REG_ADDR8(SC_BB_BASE_ADDR + 0x21)

enum
{
    FLD_N22_SRAM_RAWL  = BIT(0),
    FLD_N22_SRAM_RAWLM = BIT_RNG(1, 2),
    FLD_N22_SRAM_WABL  = BIT(3),
    FLD_N22_SRAM_WABLM = BIT_RNG(4, 6),
};

#define reg_n22_ilm_crc_ctrl REG_ADDR16(SC_DSP_BASE_ADDR + 0x24)

enum
{
    FLD_N22_ILM_CRC_SIZE = BIT_RNG(0, 8),
    FLD_N22_CRC_TRIG_ILM = BIT(9),
};

#define reg_n22_ilm_crc_result REG_ADDR16(SC_DSP_BASE_ADDR + 0x26)

#define reg_n22_dlm_crc_ctrl   REG_ADDR8(SC_DSP_BASE_ADDR + 0x28)

enum
{
    FLD_N22_DLM_CRC_SIZE = BIT_RNG(0, 6),
};

#define reg_n22_dlm_crc_trig REG_ADDR8(SC_DSP_BASE_ADDR + 0x29)

enum
{
    FLD_N22_CRC_TRIG_DLM = BIT(7),
};

#define reg_n22_dlm_crc_result REG_ADDR16(SC_DSP_BASE_ADDR + 0x2a)

#define reg_n22_zb_clksel      REG_ADDR8(SC_DSP_BASE_ADDR + 0x2c)

enum
{
    FLD_N22_ZB_CLKSEL = BIT_RNG(0, 5),
};

#define reg_n22_stall_ctrl REG_ADDR8(SC_DSP_BASE_ADDR + 0x2d)

enum
{
    FLD_N22_WFI_MODE     = BIT(0),
    FLD_CORE_SLEEP_VALUE = BIT(1),
    FLD_RX_EVT           = BIT(2),
    FLD_TX_EVT           = BIT(3),
};

#endif