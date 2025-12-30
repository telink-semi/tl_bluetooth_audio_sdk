/********************************************************************************************************
 * @file    rram_reg.h
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


#define RRAM_AHB0_BASE_ADDR           0x00400000

#define reg_rram_ahb0_rd_data_byte(j) REG_ADDR8(RRAM_AHB0_BASE_ADDR + j)

#define reg_rram_ahb0_rd_data_word(j) REG_ADDR32(RRAM_AHB0_BASE_ADDR + j)

#define RRAM_AHB1_BASE_ADDR           0x00500000

#define reg_rram_ahb1_rd_data_byte(j) REG_ADDR8(RRAM_AHB1_BASE_ADDR + j)

#define reg_rram_ahb1_rd_data_word(j) REG_ADDR32(RRAM_AHB1_BASE_ADDR + j)


#define RRAM_BASE_ADDR               0x80101000

/**
 * BIT[0:7]   data0[7:0] to transmit or received.
 */
#define reg_rram_wr_rd_data0 REG_ADDR8(RRAM_BASE_ADDR + 0x00)

/**
 * BIT[0:7]   data1[7:0] to transmit or received.
 */
#define reg_rram_wr_rd_data1 REG_ADDR8(RRAM_BASE_ADDR + 0x01)

/**
 * BIT[0:7]   data2[7:0] to transmit or received.
 */
#define reg_rram_wr_rd_data2 REG_ADDR8(RRAM_BASE_ADDR + 0x02)

/**
 * BIT[0:7]   data3[7:0] to transmit or received.
 */
#define reg_rram_wr_rd_data3           REG_ADDR8(RRAM_BASE_ADDR + 0x03)

#define reg_rram_wr_rd_data(j)         REG_ADDR8(RRAM_BASE_ADDR + 0x00 + (j))

#define reg_rram_wr_rd_data_word       REG_ADDR32(RRAM_BASE_ADDR + 0x00)

#define reg_rram_wr_rd_data_adr        (RRAM_BASE_ADDR + 0x00)

#define reg_rram_error_ahbslv_rdata(j) REG_ADDR8(RRAM_BASE_ADDR + 0x04 + (j))

#define reg_rram_error_ahbslv_rd_word  REG_ADDR32(RRAM_BASE_ADDR + 0x04)

#define reg_rram_error_addr_save0      REG_ADDR8(RRAM_BASE_ADDR + 0x08)

#define reg_rram_error_addr_save1      REG_ADDR8(RRAM_BASE_ADDR + 0x09)

#define reg_rram_error_addr_save       REG_ADDR16(RRAM_BASE_ADDR + 0x08)

#define reg_rram_core0_space_start     REG_ADDR8(RRAM_BASE_ADDR + 0x0a)

#define reg_rram_core0_space_size      REG_ADDR8(RRAM_BASE_ADDR + 0x0b)

#define reg_rram_core0_space_offset    REG_ADDR8(RRAM_BASE_ADDR + 0x0c)

#define reg_rram_core1_space_start     REG_ADDR8(RRAM_BASE_ADDR + 0x0d)

#define reg_rram_core1_space_size      REG_ADDR8(RRAM_BASE_ADDR + 0x0e)

#define reg_rram_core1_space_offset    REG_ADDR8(RRAM_BASE_ADDR + 0x0f)

#define reg_rram_par_ain0              REG_ADDR8(RRAM_BASE_ADDR + 0x10)

#define reg_rram_par_ain1              REG_ADDR8(RRAM_BASE_ADDR + 0x11)

#define reg_rram_par_ain               REG_ADDR16(RRAM_BASE_ADDR + 0x10)

#define reg_rram_par_dout_dly          REG_ADDR8(RRAM_BASE_ADDR + 0x12)

#define reg_rram_par_mode              REG_ADDR8(RRAM_BASE_ADDR + 0x13)

enum
{
    FLD_RRAM_PAR_MODE = BIT_RNG(0, 4),
};

#define reg_rram_data_len0  REG_ADDR8(RRAM_BASE_ADDR + 0x14)

#define reg_rram_data_len1  REG_ADDR8(RRAM_BASE_ADDR + 0x15)

#define reg_rram_data_len   REG_ADDR16(RRAM_BASE_ADDR + 0x14)

#define reg_rram_wr_rd_reg0 REG_ADDR8(RRAM_BASE_ADDR + 0x16)

#define reg_rram_wr_rd_reg1 REG_ADDR8(RRAM_BASE_ADDR + 0x17)

#define reg_rram_wr_rd_reg  REG_ADDR16(RRAM_BASE_ADDR + 0x16)

#define reg_rram_ctrl       REG_ADDR8(RRAM_BASE_ADDR + 0x18)

enum
{
    FLD_RRAM_CIPHER_BYPASS        = BIT(0),
    FLD_RRAM_AHBSLV_ERR_READY_H_L = BIT(1),
    FLD_RRAM_PAR_CLK_GATE_EN      = BIT(2),
    //    FLD_RRAM_ERR_IRQ_MASK               = BIT(3),
    //    FLD_RRAM_BUSY_TIMEOUT_IRQ_MASK      = BIT(4),
};

typedef enum
{
    FLD_RRAM_ERR_IRQ_MASK          = BIT(3),
    FLD_RRAM_BUSY_TIMEOUT_IRQ_MASK = BIT(4),
} rram_irq_mask_e;

#define reg_rram_clr REG_ADDR8(RRAM_BASE_ADDR + 0x1a)

enum
{
    FLD_RRAM_TXFIFO_CLR   = BIT(0),
    FLD_RRAM_RXFIFO_CLR   = BIT(1),
    FLD_RRAM_U0_CACHE_CLR = BIT(2),
    FLD_RRAM_U1_CACHE_CLR = BIT(3),
};

#define reg_rram_err_irq REG_ADDR8(RRAM_BASE_ADDR + 0x1b)

typedef enum
{
    FLD_RRAM_BUSY_TIMEOUT_IRQ_CLR = BIT(0),
} rram_irq_status_e;

#define reg_rram_state REG_ADDR8(RRAM_BASE_ADDR + 0x1c)

enum
{
    FLD_RRAM_CTRL_BUSY     = BIT(0),
    FLD_RRAM_RRAM_BUSY     = BIT(1),
    FLD_RRAM_CLR_BUSY      = BIT(2),
    FLD_RRAM_TX_READY      = BIT(3),
    FLD_RRAM_RX_READY      = BIT(4),
    FLD_RRAM_RXDMA_CYC     = BIT(5),
    FLD_RRAM_READREG_READY = BIT(6),
    FLD_RRAM_TXDMA_CYC     = BIT(7),
};

#define reg_rram_err_state REG_ADDR8(RRAM_BASE_ADDR + 0x1d)

typedef enum
{
    FLD_RRAM_ERR_R = BIT(0),
    FLD_RRAM_OTHER = BIT(1),
} rram_err_status_e;

#define reg_rram_busy_time_lim0 REG_ADDR8(RRAM_BASE_ADDR + 0x1e)

#define reg_rram_busy_time_lim1 REG_ADDR8(RRAM_BASE_ADDR + 0x1f)

#define reg_rram_busy_time_lim  REG_ADDR16(RRAM_BASE_ADDR + 0x1e)

#define reg_rram_spi_st         REG_ADDR8(RRAM_BASE_ADDR + 0x20)

enum
{
    FLD_RRAM_SPI_ENABLE = BIT(0),
    FLD_RRAM_SPI_BUSY   = BIT(1),
};

#define reg_rram_spi_cfg REG_ADDR8(RRAM_BASE_ADDR + 0x21)

enum
{
    FLD_RRAM_SPI_DIV_CLK_CFG = BIT_RNG(0, 1),
    FLD_RRAM_SPI_CNT         = BIT_RNG(2, 6),
    FLD_RRAM_SPI_TX_DONE     = BIT(7),
};

#define reg_rram_spi_wdata   REG_ADDR8(RRAM_BASE_ADDR + 0x22)

#define reg_rram_spi_rdata   REG_ADDR8(RRAM_BASE_ADDR + 0x24)

#define reg_rram_spi_read_st REG_ADDR8(RRAM_BASE_ADDR + 0x25)

enum
{
    FLD_RRAM_SPI_RDATA_VALID = BIT(0),
    FLD_RRAM_SPI_RX_CNT      = BIT_RNG(1, 5),
};
