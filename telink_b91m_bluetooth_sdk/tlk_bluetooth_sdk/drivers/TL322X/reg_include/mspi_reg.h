/********************************************************************************************************
 * @file    mspi_reg.h
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
#pragma once

#include "soc.h"

#define MSPI_BASE_ADDR 0xA3FFFF00

/**
 * BIT[0:7]   data0[7:0] to transmit or received.
 */
#define reg_mspi_wr_rd_data0 REG_ADDR8(MSPI_BASE_ADDR + 0x00)

/**
 * BIT[0:7]   data1[7:0] to transmit or received.
 */
#define reg_mspi_wr_rd_data1 REG_ADDR8(MSPI_BASE_ADDR + 0x01)

/**
 * BIT[0:7]   data2[7:0] to transmit or received.
 */
#define reg_mspi_wr_rd_data2 REG_ADDR8(MSPI_BASE_ADDR + 0x02)

/**
 * BIT[0:7]   data3[7:0] to transmit or received.
 */
#define reg_mspi_wr_rd_data3     REG_ADDR8(MSPI_BASE_ADDR + 0x03)

#define reg_mspi_wr_rd_data(j)   REG_ADDR8(MSPI_BASE_ADDR + 0x00 + (j))

#define reg_mspi_wr_rd_data_word REG_ADDR32(MSPI_BASE_ADDR + 0x00)

#define reg_mspi_wr_rd_data_adr  (MSPI_BASE_ADDR + 0x00)

#define reg_mspi_cmd             REG_ADDR8(MSPI_BASE_ADDR + 0x04)

#define reg_mspi_ctrl0           REG_ADDR8(MSPI_BASE_ADDR + 0x05)

enum
{
    FLD_MSPI_RXFIFO_INT_EN = BIT(2),
    FLD_MSPI_TXFIFO_INT_EN = BIT(3),
    FLD_MSPI_END_INT_EN    = BIT(4),
    FLD_MSPI_RX_DMA_EN     = BIT(6),
    FLD_MSPI_TX_DMA_EN     = BIT(7),
};

#define reg_mspi_cmd1   REG_ADDR8(MSPI_BASE_ADDR + 0x06)

#define reg_mspi_timing REG_ADDR8(MSPI_BASE_ADDR + 0x07)

enum
{
    FLD_MSPI_CS2SCLK = BIT_RNG(0, 2),
    FLD_MSPI_CSHT    = BIT_RNG(3, 7),
};

#define reg_mspi_ctrl1 REG_ADDR8(MSPI_BASE_ADDR + 0x08)

enum
{
    FLD_MSPI_DUAL     = BIT(0),
    FLD_MSPI_DUAD     = BIT(1),
    FLD_MSPI_ADDR_LEN = BIT_RNG(2, 3),
    FLD_MSPI_ADDR_FMT = BIT(4),
    FLD_MSPI_ADDR_EN  = BIT(5),
    FLD_MSPI_CMD_FMT  = BIT(6),
    FLD_MSPI_CMD_EN   = BIT(7),
};

#define reg_mspi_ctrl2 REG_ADDR8(MSPI_BASE_ADDR + 0x09)

enum
{
    FLD_MSPI_DUMMY     = BIT_RNG(0, 3),
    FLD_MSPI_TRANSMODE = BIT_RNG(4, 7),
};

#define reg_mspi_ctrl REG_ADDR16(MSPI_BASE_ADDR + 0x08)

/**
 * This is a special control register for register reading flash, XIP read flash can not be used, so "_reg" is added.
 */
#define reg_mspi_reg_ctrl0 REG_ADDR8(MSPI_BASE_ADDR + 0x0a)

enum
{
    FLD_MSPI_CMD1_EN      = BIT(0),
    FLD_MSPI_REG_TOKEN_EN = BIT(2),
    FLD_MSPI_CSHT_HIGH    = BIT_RNG(4, 5),
    FLD_MSPI_DUMMY_HOLD   = BIT(6),
};

#define reg_mspi_xip_wr_tcem_set REG_ADDR8(MSPI_BASE_ADDR + 0x0b)

#define reg_mspi_xip_rd_tcem_set REG_ADDR8(MSPI_BASE_ADDR + 0x1f)

/**
 * BIT[0:7]  mspi_addr0.
 */
#define reg_mspi_addr0 REG_ADDR8(MSPI_BASE_ADDR + 0x0c)

/**
 * BIT[0:7]  mspi_addr1.
 */
#define reg_mspi_addr1 REG_ADDR8(MSPI_BASE_ADDR + 0x0d)

/**
 * BIT[0:7]  mspi_addr2.
 */
#define reg_mspi_addr2 REG_ADDR8(MSPI_BASE_ADDR + 0x0e)

/**
 * BIT[0:7]  mspi_addr3.
 */
#define reg_mspi_addr3 REG_ADDR8(MSPI_BASE_ADDR + 0x0f)

#define reg_mspi_addr  REG_ADDR32(MSPI_BASE_ADDR + 0x0c)

/**
 * BIT[0:7]   transfer count0 for write data.master only
 */
#define reg_mspi_tx_cnt REG_ADDR32(MSPI_BASE_ADDR + 0x10)

/**
 * BIT[0:7]   transfer count0 for read data.master only
 */
#define reg_mspi_rx_cnt REG_ADDR32(MSPI_BASE_ADDR + 0x14)

#define reg_mspi_ctrl3  REG_ADDR8(MSPI_BASE_ADDR + 0x18)

enum
{
    FLD_MSPI_LSB       = BIT(0),
    FLD_MSPI_3LINE     = BIT(1),
    FLD_MSPI_WORK_MODE = BIT_RNG(2, 3),

    FLD_MSPI_DMATX_EOF_CLRTXFIFO_EN = BIT(5),
    FLD_MSPI_DMARX_EOF_CLRRXFIFO_EN = BIT(6),
    FLD_MSPI_AUTO_HRREADY_EN        = BIT(7),
};

#define reg_mspi_txfifo_thres REG_ADDR8(MSPI_BASE_ADDR + 0x19)

enum
{
    FLD_MSPI_TXFIFO_THRES = BIT_RNG(0, 5),
};

#define reg_mspi_rxfifo_thres REG_ADDR8(MSPI_BASE_ADDR + 0x1a)

enum
{
    FLD_MSPI_RXFIFO_THRES = BIT_RNG(0, 5),
};

#define reg_mspi_ctrl4 REG_ADDR8(MSPI_BASE_ADDR + 0x1c)

enum
{
    FLD_MSPI_DMA_TRIG_SPI_EN     = BIT(0),
    FLD_MSPI_TXDMA_REQ_AFTER_CMD = BIT(1),
    FLD_MSPI_XIP_STOP            = BIT(2),
    FLD_MSPI_XIP_ENABLE          = BIT(3),
    FLD_MSPI_DUMMY_CNT_ADD       = BIT_RNG(4, 7),
};

/**
 * BIT[0:7]  Use this combined with xip_mode being xip sequential mode.default page boundary size is 32byte, 2^page_size.
 */
#define reg_mspi_page_size       REG_ADDR8(MSPI_BASE_ADDR + 0x1d)

#define reg_mspi_xip_timeout_cnt REG_ADDR8(MSPI_BASE_ADDR + 0x1e)

/**
 * BIT[0:7]  xip_addr_offset0.
 */
#define reg_mspi_xip_addr_offset REG_ADDR8(MSPI_BASE_ADDR + 0x22)

/**
 * BIT[0:7]  when XIP_TIMEOUT_MODE enable,timeout period=spi_clock_out_period*timeout_cnt
 */
#define reg_mspi_txfifo_status REG_ADDR8(MSPI_BASE_ADDR + 0x24)

enum
{
    FLD_MSPI_TXFIFO_ENTRIES = BIT_RNG(0, 6),
    FLD_MSPI_TXFIFO_FULL    = BIT(7),
};

/**
 * BIT[0:7]  when XIP_TIMEOUT_MODE enable,timeout period=spi_clock_out_period*timeout_cnt
 */
#define reg_mspi_rxfifo_status REG_ADDR8(MSPI_BASE_ADDR + 0x25)

enum
{
    FLD_MSPI_RXFIFO_ENTRIES = BIT_RNG(0, 6),
    FLD_MSPI_RXFIFO_EMPTY   = BIT(7),
};

#define reg_mspi_status REG_ADDR8(MSPI_BASE_ADDR + 0x28)

enum
{
    FLD_MSPI_SOFT_RESET       = BIT(2),
    FLD_MSPI_XIP_REG_ARB_ERR  = BIT(3),
    FLD_MSPI_RXFIFO_CLR_LEVEL = BIT(4),
    FLD_MSPI_TXFIFO_CLR_LEVEL = BIT(5),
    FLD_MSPI_BUSY             = BIT(7),
};

#define reg_mspi_int_status0 REG_ADDR8(MSPI_BASE_ADDR + 0x2a)

enum
{
    FLD_MSPI_RXF_THRES_INT_STA = BIT(2),
    FLD_MSPI_TXF_THRES_INT_STA = BIT(3),
    FLD_MSPI_TRANS_END_INT_STA = BIT(4),
};

#define reg_mspi_reg_xip_err_addr_b0 REG_ADDR8(MSPI_BASE_ADDR + 0x88)
#define reg_mspi_reg_xip_err_addr_b1 REG_ADDR8(MSPI_BASE_ADDR + 0x89)
#define reg_mspi_reg_xip_err_addr_b2 REG_ADDR8(MSPI_BASE_ADDR + 0x8a)
#define reg_mspi_reg_xip_err_addr_b3 REG_ADDR8(MSPI_BASE_ADDR + 0x8b)
#define reg_mspi_reg_xip_err_status  REG_ADDR8(MSPI_BASE_ADDR + 0x8c)

enum
{
    FLD_MSPI_REG_XIP_ERR_STATUS = BIT(0),
    FLD_MSPI_XIP_REG_ERR_STATUS = BIT(1),
};

#define reg_mspi_xip_rd_config(i) REG_ADDR32(MSPI_BASE_ADDR + 0x90 + 8 * i)

#define reg_mspi_xip_rd_fmt(i)    REG_ADDR8(MSPI_BASE_ADDR + 0x90 + 8 * i)

enum
{
    FLD_MSPI_XIP_RD_DUAL  = BIT(0),
    FLD_MSPI_XIP_RD_QUAD  = BIT(1),
    FLD_MSPI_XIP_ADDR_LEN = BIT_RNG(2, 3),
    FLD_MSPI_XIP_ADDR_FMT = BIT(4),
    FLD_MSPI_XIP_ADDR_EN  = BIT(5),
    FLD_MSPI_XIP_CMD_FMT  = BIT(6),
    FLD_MSPI_XIP_CMD_EN   = BIT(7),
};

#define reg_mspi_xip_rd_trnsmode(i) REG_ADDR8(MSPI_BASE_ADDR + 0x91 + 8 * i)

enum
{
    FLD_MSPI_XIP_RD_DUMMY_CNT = BIT_RNG(0, 3),
    FLD_MSPI_XIP_RD_TRANSMODE = BIT_RNG(4, 7),
};

#define reg_mspi_xip_rd_ctrl0(i) REG_ADDR8(MSPI_BASE_ADDR + 0x92 + 8 * i)

enum
{
    FLD_MSPI_XIP_RD_CMD1_EN       = BIT(0),
    FLD_MSPI_XIP_TOKEN_EN         = BIT(2),
    FLD_MSPI_XIP_RD_DUMMY_CNT_ADD = BIT(3),
    FLD_MSPI_XIP_PAGE_MODE_EN     = BIT(5),
    FLD_MSPI_XIP_TIMEOUT_MODE_EN  = BIT(6),
    FLD_MSPI_XIP_TCEM_MODE_EN     = BIT(7),
};

#define reg_mspi_xip_rd_cmd(i)    REG_ADDR8(MSPI_BASE_ADDR + 0x93 + 8 * i)

#define reg_mspi_xip_wr_config(i) REG_ADDR32(MSPI_BASE_ADDR + 0x94 + 8 * i)
#define reg_mspi_xip_wr_fmt(i)    REG_ADDR8(MSPI_BASE_ADDR + 0x94 + 8 * i)

enum
{
    FLD_MSPI_XIP_WR_DUAL     = BIT(0),
    FLD_MSPI_XIP_WR_QUAD     = BIT(1),
    FLD_MSPI_XIP_WR_ADDR_LEN = BIT_RNG(2, 3),
    FLD_MSPI_XIP_WR_ADDR_FMT = BIT(4),
    FLD_MSPI_XIP_WR_ADDR_EN  = BIT(5),
    FLD_MSPI_XIP_WR_CMD_FMT  = BIT(6),
    FLD_MSPI_XIP_WR_CMD_EN   = BIT(7),
};

#define reg_mspi_xip_wr_transmode(i) REG_ADDR8(MSPI_BASE_ADDR + 0x95 + 8 * i)

enum
{
    FLD_MSPI_XIP_WR_DUMMY_CNT = BIT_RNG(0, 3),
    FLD_MSPI_XIP_WR_TRANSMODE = BIT_RNG(4, 7),
};

#define reg_mspi_xip_wr_ctrl0(i) REG_ADDR8(MSPI_BASE_ADDR + 0x96 + 8 * i)

enum
{
    FLD_MSPI_XIP_WR_CMD1_EN = BIT(0),
};

#define reg_mspi_xip_wr_cmd(i) REG_ADDR8(MSPI_BASE_ADDR + 0x97 + 8 * i)

#define reg_mspi_xip_size_set  REG_ADDR8(MSPI_BASE_ADDR + 0xb0)

enum
{
    FLD_MSPI_XIP0_END_ADDR = BIT_RNG(0, 1),
    FLD_MSPI_XIP1_END_ADDR = BIT_RNG(2, 3),
    FLD_MSPI_XIP2_END_ADDR = BIT_RNG(4, 5),
    FLD_MSPI_XIP3_END_ADDR = BIT_RNG(6, 7),
};

#define reg_mspi_xip_rd_cmd1(i)   REG_ADDR8(MSPI_BASE_ADDR + 0xb4 + 2 * i)

#define reg_mspi_xip_wr_cmd1(i)   REG_ADDR8(MSPI_BASE_ADDR + 0xb5 + 2 * i)

#define reg_mspi_xip_core0_start  REG_ADDR16(MSPI_BASE_ADDR + 0xc0)

#define reg_mspi_xip_core0_size   REG_ADDR16(MSPI_BASE_ADDR + 0xc2)

#define reg_mspi_xip_core0_offset REG_ADDR16(MSPI_BASE_ADDR + 0xc4)

#define reg_mspi_xip_core1_start  REG_ADDR16(MSPI_BASE_ADDR + 0xc8)

#define reg_mspi_xip_core1_size   REG_ADDR16(MSPI_BASE_ADDR + 0xca)

#define reg_mspi_xip_core1_offset REG_ADDR16(MSPI_BASE_ADDR + 0xcc)

#define reg_mspi_xip_core2_start  REG_ADDR16(MSPI_BASE_ADDR + 0xd0)

#define reg_mspi_xip_core2_size   REG_ADDR16(MSPI_BASE_ADDR + 0xd2)

#define reg_mspi_xip_core2_offset REG_ADDR16(MSPI_BASE_ADDR + 0xd4)

#define reg_mspi_rxdll_ctrl0      REG_ADDR8(MSPI_BASE_ADDR + 0xd8)

enum
{
    FLD_MSPI_RXDLL_CP_SEL      = BIT_RNG(0, 1),
    FLD_MSPI_RXDLL_CLK_FB_SEL  = BIT(2),
    FLD_MSPI_RXDLL_PFD_MUX_SEL = BIT_RNG(3, 5),
    FLD_MSPI_RXDLL_LOCK_DET_EN = BIT(6),
    FLD_MSPI_RXDLL_PD          = BIT(7),
};

#define reg_mspi_rxdll_ctrl1 REG_ADDR8(MSPI_BASE_ADDR + 0xd9)

enum
{
    FLD_MSPI_RXDLL_DLY_SEL = BIT_RNG(0, 2),
    FLD_MSPI_RXDLL_MODE    = BIT(4),
};

#define reg_mspi_txdll_ctrl0 REG_ADDR8(MSPI_BASE_ADDR + 0xda)

enum
{
    FLD_MSPI_TXDLL_CP_SEL      = BIT_RNG(0, 1),
    FLD_MSPI_TXDLL_CLK_FB_SEL  = BIT(2),
    FLD_MSPI_TXDLL_PFD_MUX_SEL = BIT_RNG(3, 5),
    FLD_MSPI_TXDLL_LOCK_DET_EN = BIT(6),
    FLD_MSPI_TXDLL_PD          = BIT(7),
};

#define reg_mspi_txdll_ctrl1 REG_ADDR8(MSPI_BASE_ADDR + 0xdb)

enum
{
    FLD_MSPI_TXDLL_DLY_SEL = BIT_RNG(0, 2),
    FLD_MSPI_TXDLL_MODE    = BIT(4),
};

#define reg_mspi_dll_status REG_ADDR8(MSPI_BASE_ADDR + 0xdc)

enum
{
    FLD_MSPI_RXDLL_DET_OUT = BIT(0),
    FLD_MSPI_TXDLL_DET_OUT = BIT(1),
};

#define reg_mspi_ddr_ctrl0 REG_ADDR8(MSPI_BASE_ADDR + 0xe0)

enum
{
    FLD_MSPI_XIP_DATA_DDR_MODE = BIT(0),
    FLD_MSPI_XIP_ADDR_DDR_MODE = BIT(1),
    FLD_MSPI_XIP_CMD_DDR_MODE  = BIT(2),
    FLD_MSPI_XIP_DDR_POS       = BIT(3),
};

#define reg_mspi_ddr_ctrl1 REG_ADDR8(MSPI_BASE_ADDR + 0xe1)

enum
{
    FLD_MSPI_REG_DATA_DDR_MODE = BIT(0),
    FLD_MSPI_REG_ADDR_DDR_MODE = BIT(1),
    FLD_MSPI_REG_CMD_DDR_MODE  = BIT(2),
    FLD_MSPI_REG_DDR_POS       = BIT(3),
};

#define reg_mspi_xip_token_val REG_ADDR8(MSPI_BASE_ADDR + 0xe2)

#define reg_mspi_reg_token_val REG_ADDR8(MSPI_BASE_ADDR + 0xe3)
