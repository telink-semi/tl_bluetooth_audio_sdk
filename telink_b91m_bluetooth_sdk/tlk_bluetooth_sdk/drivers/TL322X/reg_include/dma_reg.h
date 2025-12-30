/********************************************************************************************************
 * @file    dma_reg.h
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
#ifndef DMA_REG_H
#define DMA_REG_H
#include "soc.h"
/*******************************    dma registers:  dma0 0x80100400 /dma1 0x80101800    ******************************/
#define DMA_BASE_ADDR 0x80100400
#define reg_dma_id    REG_ADDR32(0x80100400 + ((i) >> 3) * 0x1400)
#define reg_dma_cfg   REG_ADDR32(0x80100410 + ((i) >> 3) * 0x1400)

enum
{
    FLD_DMA_CHANNEL_NUM = BIT_RNG(0, 3),
    FLD_DMA_FIFO_DEPTH  = BIT_RNG(4, 9),
    FLD_DMA_REQ_NUM     = BIT_RNG(10, 14),
    FLD_DMA_REQ_SYNC    = BIT(30),
    FLD_DMA_CHANINXFR   = BIT(31),
};

#define reg_dma_pem_task_en    REG_ADDR8(0x80100400 + 0x23)

#define reg_dma_err_mask(i) REG_ADDR8(DMA_BASE_ADDR + ((i) >> 3) * 0x1400 + 0x29)

enum
{
    FLD_DMA_CHANNEL0_ERR_MASK = BIT(0),
    FLD_DMA_CHANNEL1_ERR_MASK = BIT(1),
    FLD_DMA_CHANNEL2_ERR_MASK = BIT(2),
    FLD_DMA_CHANNEL3_ERR_MASK = BIT(3),
    FLD_DMA_CHANNEL4_ERR_MASK = BIT(4),
    FLD_DMA_CHANNEL5_ERR_MASK = BIT(5),
    FLD_DMA_CHANNEL6_ERR_MASK = BIT(6),
    FLD_DMA_CHANNEL7_ERR_MASK = BIT(7),
};

#define reg_dma_ctrl_llp(i) REG_ADDR8(DMA_BASE_ADDR + 0x25 + ((i) >> 3) * 0x1400)
#define reg_dma_abt_mask(i) REG_ADDR8(DMA_BASE_ADDR + ((i) >> 3) * 0x1400 + 0x2a)

enum
{
    FLD_DMA_CHANNEL0_ABT_MASK = BIT(0),
    FLD_DMA_CHANNEL1_ABT_MASK = BIT(1),
    FLD_DMA_CHANNEL2_ABT_MASK = BIT(2),
    FLD_DMA_CHANNEL3_ABT_MASK = BIT(3),
    FLD_DMA_CHANNEL4_ABT_MASK = BIT(4),
    FLD_DMA_CHANNEL5_ABT_MASK = BIT(5),
    FLD_DMA_CHANNEL6_ABT_MASK = BIT(6),
    FLD_DMA_CHANNEL7_ABT_MASK = BIT(7),
};

#define reg_dma_ctrl(i) REG_ADDR32(DMA_BASE_ADDR + 0x44 + ((i) >> 3) * 0x1400 + ((i) % 8) * 0x14)

enum
{
    FLD_DMA_CHANNEL_ENABLE        = BIT(0),
    FLD_DMA_CHANNEL_TC_MASK       = BIT(1),
    FLD_DMA_CHANNEL_DST_REQ_SEL   = BIT_RNG(2, 7),
    FLD_DMA_CHANNEL_SRC_REQ_SEL   = BIT_RNG(8, 13),
    FLD_DMA_CHANNEL_DST_ADDR_CTRL = BIT_RNG(14, 15),
    FLD_DMA_CHANNEL_SRC_ADDR_CTRL = BIT_RNG(16, 17),
    FLD_DMA_CHANNEL_DST_MODE      = BIT(18),
    FLD_DMA_CHANNEL_SRC_MODE      = BIT(19),
    FLD_DMA_CHANNEL_DST_WIDTH     = BIT_RNG(20, 21),
    FLD_DMA_CHANNEL_SRC_WIDTH     = BIT_RNG(22, 23),
};

#define reg_dma_ctr0(i)    REG_ADDR8(DMA_BASE_ADDR + 0x44 + ((i) >> 3) * 0x1400 + ((i) % 8) * 0x14)
#define reg_dma_ch_cr3(i)  REG_ADDR8(DMA_BASE_ADDR + 0x47 + ((i) >> 3) * 0x1400 + ((i) % 8) * 0x14)

#define reg_dma_err_isr(i) REG_ADDR8(DMA_BASE_ADDR + 0x30 + (((i) > 0x80) ? 1 : 0) * 0x1400)
#define reg_dma_abt_isr(i) REG_ADDR8(DMA_BASE_ADDR + 0x31 + (((i) > 0x80) ? 1 : 0) * 0x1400)
#define reg_dma_tc_isr(i)  REG_ADDR8(DMA_BASE_ADDR + 0x32 + (((i) > 0x80) ? 1 : 0) * 0x1400)

enum
{
    FLD_DMA_CHANNEL0_IRQ = BIT(0),
    FLD_DMA_CHANNEL1_IRQ = BIT(1),
    FLD_DMA_CHANNEL2_IRQ = BIT(2),
    FLD_DMA_CHANNEL3_IRQ = BIT(3),
    FLD_DMA_CHANNEL4_IRQ = BIT(4),
    FLD_DMA_CHANNEL5_IRQ = BIT(5),
    FLD_DMA_CHANNEL6_IRQ = BIT(6),
    FLD_DMA_CHANNEL7_IRQ = BIT(7),
};

#define reg_dma_ctr3(i) REG_ADDR8(DMA_BASE_ADDR + 0x47 + ((i) >> 3) * 0x1400 + ((i) % 8) * 0x14)

enum
{
    FLD_DMA_SRC_BURST_SIZE = BIT_RNG(0, 2),
    FLD_DMA_R_NUM_EN       = BIT(4),
    FLD_DMA_PRIORITY       = BIT(5),
    FLD_DMA_W_NUM_EN       = BIT(6),
    FLD_DMA_AUTO_ENABLE_EN = BIT(7),
};

#define reg_dma_src_addr(i) REG_ADDR32(DMA_BASE_ADDR + 0x48 + ((i) >> 3) * 0x1400 + ((i) % 8) * 0x14)
#define reg_dma_dst_addr(i) REG_ADDR32(DMA_BASE_ADDR + 0x4c + ((i) >> 3) * 0x1400 + ((i) % 8) * 0x14)

#define reg_dma_size0(i)    REG_ADDR16(DMA_BASE_ADDR + 0x50 + ((i) >> 3) * 0x1400 + ((i) % 8) * 0x14)

enum
{
    FLD_DMA_TANS_SIZE0 = BIT_RNG(0, 15),
};

#define reg_dma_size1(i) REG_ADDR8(DMA_BASE_ADDR + 0x52 + ((i) >> 3) * 0x1400 + ((i) % 8) * 0x14)

enum
{
    FLD_DMA_TANS_SIZE1 = BIT_RNG(0, 5),
    FLD_DMA_TANS_IDX   = BIT_RNG(6, 7),
};

#define reg_dma_llp(i)          REG_ADDR32(DMA_BASE_ADDR + 0x54 + ((i) >> 3) * 0x1400 + ((i) % 8) * 0x14)
#define reg_dma_llp_int_mode(i) REG_ADDR8(DMA_BASE_ADDR + 0x113 + ((((i) % 8) > 3) ? 1 : 0) + ((i) >> 3) * 0x1400)

#endif
