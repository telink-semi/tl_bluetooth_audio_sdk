/********************************************************************************************************
 * @file    keyscan_reg.h
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
#ifndef KEYSCAN_REG_H_
#define KEYSCAN_REG_H_
#include "soc.h"
/*******************************      keyscan registers: 0x140700      ******************************/


#define REG_KEYSCAN_BASE 0x140700

#define reg_ks_col_mask0 REG_ADDR32(REG_KEYSCAN_BASE)

#define reg_ks_row_sel0  REG_ADDR32(REG_KEYSCAN_BASE + 0x04)

#define reg_ks_row_sel4  REG_ADDR8(REG_KEYSCAN_BASE + 0x08)

#define reg_ks_end_flag  REG_ADDR8(REG_KEYSCAN_BASE + 0x09)

#define reg_ks_en        REG_ADDR8(REG_KEYSCAN_BASE + 0x0a)

enum
{
    FLD_KS_EN          = BIT(0),
    FLD_KS_CLK_EN      = BIT(1),
    FLD_KS_IE          = BIT(2),
    FLD_KS_IN_INV      = BIT(3),
    FLD_KS_OUT_INV     = BIT(4),
    FLD_KS_MOD         = BIT(5),
    FLD_KS_RESET       = BIT(6),
    FLD_KS_TRI_CHK_DIS = BIT(7),
};

#define reg_ks_frm_num REG_ADDR8(REG_KEYSCAN_BASE + 0x0b)

enum
{
    FLD_KS_FRM_NUM         = BIT_RNG(0, 4),
    FLD_KS_DEBOUNCE_PERIOD = BIT_RNG(5, 7),
};

#define reg_ks_irq REG_ADDR8(REG_KEYSCAN_BASE + 0x0c)

enum
{
    FLD_KS_RPTR    = BIT_RNG(0, 4),
    FLD_KS_FRM_END = BIT(7),
};

#define reg_ks_rptr REG_ADDR8(REG_KEYSCAN_BASE + 0x0d)

enum
{
    FLD_KS_R_WPTR      = BIT_RNG(0, 4),
    FLD_KS_KEY_DET_CAP = BIT(6),
    FLD_KS_STATE       = BIT(7),
};

#define reg_ks_wptr REG_ADDR8(REG_KEYSCAN_BASE + 0x0e)

enum
{
    FLD_KS_WPTR    = BIT_RNG(0, 4),
    FLD_KS_NO_KEY  = BIT(5),
    FLD_KS_KEY_DET = BIT(6),
    FLD_KS_CNT_EN  = BIT(7),
};

#define reg_ks_gated REG_ADDR8(REG_KEYSCAN_BASE + 0x0f)

enum
{
    FLD_KS_CNT_H     = BIT_RNG(0, 2),
    FLD_KS_RXDONE_IRQ = BIT(3),
    FLD_KS_GATED_CLR = BIT(4),
    FLD_KS_GATED     = BIT(5),
    FLD_KS_CNT16_EN  = BIT(6),
    FLD_KS_WAKE_UP   = BIT(7),
};

#define reg_ks_key  REG_ADDR8(REG_KEYSCAN_BASE + 0x10)
#define reg_ks_lptr REG_ADDR8(REG_KEYSCAN_BASE + 0x11)

enum
{
    FLD_KS_LPTR = BIT_RNG(0, 4),
};

#define reg_ks_cnt128 REG_ADDR8(REG_KEYSCAN_BASE + 0x12)

enum
{
    FLD_KS_CNT = BIT_RNG(0, 6),
};

#define reg_ks_cnt16 REG_ADDR8(REG_KEYSCAN_BASE + 0x13)

enum
{
    FLD_KS_CNT_16  = BIT_RNG(0, 3),
    FLD_KS_ROW_NUM = BIT_RNG(4, 6),
};

#define reg_ks_ctrl0 REG_ADDR8(REG_KEYSCAN_BASE + 0x14)

enum
{
    FLD_KS_PEM_EVENT_EN = BIT(0),
    FLD_KS_PEM_TASK_EN  = BIT_RNG(1, 2),
    FLD_KS_ADC_SAMPLE   = BIT_RNG(4, 7),
};

#define reg_ks_a_en0 REG_ADDR8(REG_KEYSCAN_BASE + 0x15)

enum
{
    FLD_KS_A_IE0       = BIT(0),
    FLD_KS_A_IE1       = BIT(1),
    FLD_KS_CAPTURE_SEL = BIT(2),
    FLD_KS_CAPTURE_IO  = BIT(3),
    FLD_KS_RXDMA_EN    = BIT(4),
    FLD_KS_RXDONE_IE   = BIT(5),
    FLD_KS_RXDMA_CACHE = BIT_RNG(6, 7),
};

#define reg_ks_a_en1 REG_ADDR8(REG_KEYSCAN_BASE + 0x16)

enum
{
    FLD_KS_A_EN     = BIT(0),
    FLD_KS_CLK_A_EN = BIT(1),
    FLD_KS_MOD_A    = BIT(2),
    FLD_KS_ADC_EN   = BIT(3),
    FLD_KS_ADC1_EN  = BIT(4),
    FLD_KS_IO_MUX   = BIT_RNG(6, 7),
};

#define reg_ks_a_setl0      REG_ADDR8(REG_KEYSCAN_BASE + 0x17) //Settle time the scan period

#define reg_ks_a_setl1      REG_ADDR8(REG_KEYSCAN_BASE + 0x18) //Settle time when Capture start for ADC0

#define reg_ks_a_setl2      REG_ADDR8(REG_KEYSCAN_BASE + 0x19) //Settle time when Capture start for ADC1

#define reg_ks_a_setl3      REG_ADDR8(REG_KEYSCAN_BASE + 0x1a) //Settle time the power manager

#define reg_ks_a_setl4      REG_ADDR8(REG_KEYSCAN_BASE + 0x1b) //Settle time the start time of scan after the Power

#define reg_ks_buffer_dma   REG_ADDR32(REG_KEYSCAN_BASE + 0x1c)

#define reg_ks_pwr_on_setl(i) REG_ADDR8(REG_KEYSCAN_BASE + 0x20 + i)
#define reg_ks_pwr_on_setl0 REG_ADDR8(REG_KEYSCAN_BASE + 0x20)
#define reg_ks_pwr_on_setl1 REG_ADDR8(REG_KEYSCAN_BASE + 0x21)
#define reg_ks_pwr_on_setl2 REG_ADDR8(REG_KEYSCAN_BASE + 0x22)
#define reg_ks_pwr_on_setl3 REG_ADDR8(REG_KEYSCAN_BASE + 0x23)
#define reg_ks_pwr_on_setl4 REG_ADDR8(REG_KEYSCAN_BASE + 0x24)
#define reg_ks_pwr_on_setl5 REG_ADDR8(REG_KEYSCAN_BASE + 0x25)
#define reg_ks_pwr_on_setl6 REG_ADDR8(REG_KEYSCAN_BASE + 0x26)
#define reg_ks_pwr_on_setl7 REG_ADDR8(REG_KEYSCAN_BASE + 0x27)

#define reg_ks_pwr_dn_setl(i) REG_ADDR8(REG_KEYSCAN_BASE + 0x28 + i)
#define reg_ks_pwr_dn_setl0 REG_ADDR8(REG_KEYSCAN_BASE + 0x28)
#define reg_ks_pwr_dn_setl1 REG_ADDR8(REG_KEYSCAN_BASE + 0x29)
#define reg_ks_pwr_dn_setl2 REG_ADDR8(REG_KEYSCAN_BASE + 0x2a)
#define reg_ks_pwr_dn_setl3 REG_ADDR8(REG_KEYSCAN_BASE + 0x2b)
#define reg_ks_pwr_dn_setl4 REG_ADDR8(REG_KEYSCAN_BASE + 0x2c)
#define reg_ks_pwr_dn_setl5 REG_ADDR8(REG_KEYSCAN_BASE + 0x2d)
#define reg_ks_pwr_dn_setl6 REG_ADDR8(REG_KEYSCAN_BASE + 0x2e)
#define reg_ks_pwr_dn_setl7 REG_ADDR8(REG_KEYSCAN_BASE + 0x2f)

#define reg_ks_buffer       REG_ADDR32(REG_KEYSCAN_BASE + 0x30)

#define reg_ks_a_status     REG_ADDR8(REG_KEYSCAN_BASE + 0x34)

enum
{
    FLD_KS_FRM_END_STA   = BIT(0),//ks_ana ADC0 interrupt, write 1 to clear
    FLD_KS_FRM_END1_STA  = BIT(1),//ks_ana ADC1 interrupt, write 1 to clear
    FLD_KS_WAKEA_UP_STA  = BIT(2),
    FLD_KS_WAKEA1_UP_STA = BIT(3),
    FLD_KS_SCAN_EN       = BIT(4),//ks_ana enable
    FLD_KS_CNT_CAPTURE1  = BIT_RNG(5, 7),
};

#define reg_ks_pwr_ptr     REG_ADDR8(REG_KEYSCAN_BASE + 0x35)
#define reg_ks_scan_ptr    REG_ADDR8(REG_KEYSCAN_BASE + 0x36)
#define reg_ks_capture_ptr REG_ADDR8(REG_KEYSCAN_BASE + 0x37)

enum
{
    FLD_KS_CNT_CAPTURE_IO = BIT_RNG(0, 3),
    FLD_KS_CNT_CAPTURE    = BIT_RNG(4, 6),
    FLD_KS_CAPTURE_EN     = BIT(7),
};

#define reg_ks_a_en2 REG_ADDR8(REG_KEYSCAN_BASE + 0x38)

enum
{
    FLD_KS_PWR_SWITCH_MANUAL     = BIT(0),
    FLD_KS_CHANNEL_SWITCH_MANUAL = BIT(1),
    FLD_KS_IRQ_DLY_EN            = BIT(2),
    FLD_KS_SETL_TIME_SCAN2_EN    = BIT(3),
    FLD_KS_SCAN2_EN_IO           = BIT(4),
    FLD_KS_SCAN2_EN              = BIT(5),
};

#define reg_ks_pwr_switch_val     REG_ADDR8(REG_KEYSCAN_BASE + 0x39)
#define reg_ks_channel_switch_val REG_ADDR8(REG_KEYSCAN_BASE + 0x3a)
#define reg_ks_a_setl5            REG_ADDR8(REG_KEYSCAN_BASE + 0x3b)

#endif
