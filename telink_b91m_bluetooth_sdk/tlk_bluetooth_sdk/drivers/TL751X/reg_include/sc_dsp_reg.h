/********************************************************************************************************
 * @file    sc_dsp_reg.h
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
#ifndef SC_DSP_REG_H_
#define SC_DSP_REG_H_

#include "bit.h"

/**
 * @brief   The RAM address of the DSP can be accessed by AHB or Core address, 
 *          but it is differ for D25F access which must use AHB address otherwise will access cache.
 *          The ILM of DSP must access by words.
 * ------------------------------------------------------------------------------------------------
 *            |  AHB Start address |   AHB End address   | Core Start address |  Core End address
 * ------------------------------------------------------------------------------------------------
 * DSP  | ILM |     0x82100000     | 0x82100000 + 256 KB |     0x02100000     | 0x02100000 + 256 KB
 *      | DLM |     0x82000000     | 0x82000000 + 512 KB |     0x02000000     | 0x02000000 + 512 KB
 * ------------------------------------------------------------------------------------------------
 */
#define DSP_ILM_BASE     (0x82100000)
#define DSP_DLM_BASE     (0x82000000)

#define SC_DSP_BASE_ADDR (0x80240180)

#define reg_dsp_ctrl     REG_ADDR32(SC_DSP_BASE_ADDR + 0x00)
#define reg_dsp_ctrl0    REG_ADDR16(SC_DSP_BASE_ADDR + 0x00)

enum
{
    FLD_DSP_CTRL0_DEBUG_EN      = BIT(0),
    FLD_DSP_CTRL0_STALL         = BIT(1),
    FLD_DSP_CTRL0_START_VEC_SEL = BIT(2),
};

#define reg_dsp_ctrl1 REG_ADDR16(SC_DSP_BASE_ADDR + 0x02)

enum
{
    FLD_DSP_CTRL1_WAIT_MODE = BIT(0),
};

#define reg_dsp_debug_pc      REG_ADDR32(SC_DSP_BASE_ADDR + 0x04)

#define reg_dsp_debug_data    REG_ADDR32(SC_DSP_BASE_ADDR + 0x08)

#define reg_dsp_debug_inst    REG_ADDR32(SC_DSP_BASE_ADDR + 0x0c)

#define reg_dsp_debug_status  REG_ADDR32(SC_DSP_BASE_ADDR + 0x10)

#define reg_dsp_debug_ls0addr REG_ADDR32(SC_DSP_BASE_ADDR + 0x14)

#define reg_dsp_debug_ls0data REG_ADDR32(SC_DSP_BASE_ADDR + 0x18)

#define reg_dsp_debug_ls0stat REG_ADDR32(SC_DSP_BASE_ADDR + 0x1c)

#define reg_dsp_debug_ls1addr REG_ADDR32(SC_DSP_BASE_ADDR + 0x20)

#define reg_dsp_debug_ls1data REG_ADDR32(SC_DSP_BASE_ADDR + 0x24)

#define reg_dsp_debug_ls1stat REG_ADDR32(SC_DSP_BASE_ADDR + 0x28)

#define reg_dsp_rst           REG_ADDR16(SC_DSP_BASE_ADDR + 0x2c)
#define reg_dsp_rst0          REG_ADDR8(SC_DSP_BASE_ADDR + 0x2c)

enum
{
    FLD_DSP_EN       = BIT(0),
    FLD_DSP_TRST_N   = BIT(1),
    FLD_DSP_DRESET_N = BIT(2),
    FLD_DSP_RAMCRC_N = BIT(3),
};

#define reg_dsp_clken  REG_ADDR16(SC_DSP_BASE_ADDR + 0x2e)
#define reg_dsp_clken0 REG_ADDR8(SC_DSP_BASE_ADDR + 0x2e)

enum
{
    FLD_DSP_CLK_EN = BIT(0),
};

#define reg_dsp_reset_vector  REG_ADDR32(SC_DSP_BASE_ADDR + 0x30)

#define DSP_SRAM_EMA_REG_ADDR (SC_DSP_BASE_ADDR + 0x34)
#define DSP_SRAM_EMA_DATA_LEN 4

#define reg_dsp_sram_cfg      REG_ADDR16(DSP_SRAM_EMA_REG_ADDR)
#define reg_dsp_sram_cfg0     REG_ADDR8(DSP_SRAM_EMA_REG_ADDR)

enum
{
    FLD_DSP_SRAM_EMA  = BIT_RNG(0, 2),
    FLD_DSP_SRAM_EMAW = BIT_RNG(3, 4),
    FLD_DSP_SRAM_EMAS = BIT(5),
};

#define reg_dsp_sram_cfg1 REG_ADDR8(SC_DSP_BASE_ADDR + 0x35)

enum
{
    FLD_DSP_SRAM_RAWL  = BIT(0),
    FLD_DSP_SRAM_RAWLM = BIT_RNG(1, 2),
    FLD_DSP_SRAM_WABL  = BIT(3),
    FLD_DSP_SRAM_WABLM = BIT_RNG(4, 6),
};

#define reg_dsp_reg_cfg  REG_ADDR16(SC_DSP_BASE_ADDR + 0x36)
#define reg_dsp_reg_cfg0 REG_ADDR8(SC_DSP_BASE_ADDR + 0x36)

enum
{
    FLD_DSP_REG_EMA  = BIT_RNG(0, 2),
    FLD_DSP_REG_EMAW = BIT_RNG(3, 4),
    FLD_DSP_REG_EMAS = BIT(5),
};

#define reg_dsp_reg_cfg1 REG_ADDR8(SC_DSP_BASE_ADDR + 0x37)

enum
{
    FLD_DSP_REG_RAWL  = BIT(0),
    FLD_DSP_REG_RAWLM = BIT_RNG(1, 2),
    FLD_DSP_REG_WABL  = BIT(3),
    FLD_DSP_REG_WABLM = BIT_RNG(4, 6),
};

#define reg_dsp_icache_crc_ctrl REG_ADDR8(SC_DSP_BASE_ADDR + 0x38)

enum
{
    FLD_DSP_ICACHE_CRC_SIZE = BIT_RNG(0, 6),
};

#define reg_dsp_icache_crc_trig REG_ADDR8(SC_DSP_BASE_ADDR + 0x39)

enum
{
    FLD_DSP_CRC_TRIG_ICACHE = BIT(7),
};

#define reg_dsp_icache_crc_result REG_ADDR16(SC_DSP_BASE_ADDR + 0x3a)

#define reg_dsp_iram0_crc_ctrl    REG_ADDR8(SC_DSP_BASE_ADDR + 0x3c)

enum
{
    FLD_DSP_IRAM0_CRC_SIZE = BIT_RNG(0, 6),
};

#define reg_dsp_iram0_crc_trig REG_ADDR8(SC_DSP_BASE_ADDR + 0x3d)

enum
{
    FLD_DSP_CRC_TRIG_IRAM0 = BIT(7),
};

#define reg_dsp_iram0_crc_result REG_ADDR16(SC_DSP_BASE_ADDR + 0x3e)

#define reg_dsp_iram1_crc_ctrl   REG_ADDR8(SC_DSP_BASE_ADDR + 0x40)

enum
{
    FLD_DSP_IRAM1_CRC_SIZE = BIT_RNG(0, 6),
};

#define reg_dsp_iram1_crc_trig REG_ADDR8(SC_DSP_BASE_ADDR + 0x41)

enum
{
    FLD_DSP_CRC_TRIG_IRAM1 = BIT(7),
};

#define reg_dsp_iram1_crc_result REG_ADDR16(SC_DSP_BASE_ADDR + 0x42)

#define reg_dsp_itag_crc_ctrl    REG_ADDR8(SC_DSP_BASE_ADDR + 0x44)

enum
{
    FLD_DSP_ITAG_CRC_SIZE = BIT_RNG(0, 1),
};

#define reg_dsp_itag_crc_trig REG_ADDR8(SC_DSP_BASE_ADDR + 0x45)

enum
{
    FLD_DSP_CRC_TRIG_ITAG = BIT(7),
};

#define reg_dsp_itag_crc_result REG_ADDR16(SC_DSP_BASE_ADDR + 0x46)

#define reg_dsp_pfaultinfo0     REG_ADDR32(SC_DSP_BASE_ADDR + 0x48)
#define reg_dsp_pfaultinfo4     REG_ADDR32(SC_DSP_BASE_ADDR + 0x4c)

#define reg_dsp_info            REG_ADDR8(SC_DSP_BASE_ADDR + 0x50)

enum
{
    FLD_DOUBLE_EXCEPTION_ERROR = BIT(0),
    FLD_PFATAL_ERROR           = BIT(1),
    FLD_PFAULT_INFO_VALID      = BIT(2),
};
#endif