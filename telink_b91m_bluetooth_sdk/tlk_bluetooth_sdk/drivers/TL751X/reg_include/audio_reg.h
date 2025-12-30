/********************************************************************************************************
 * @file    audio_reg.h
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
#ifndef AUDIO_REG_H
#define AUDIO_REG_H
#include "soc.h"

#define REG_AUDIO_FIFO0             0x120000
#define REG_AUDIO_FIFO1             0x120040
#define REG_AUDIO_FIFO2             0x120080
#define REG_AUDIO_FIFO3             0x1200c0
#define REG_AUDIO_FIFO_ADDR(i)      (REG_AUDIO_FIFO0 + 0x40 * (i))

#define REG_HAC_FIFO0               0x130000
#define REG_HAC_FIFO1               0x130004
#define REG_HAC_FIFO2               0x130008
#define REG_HAC_FIFO3               0x13000c
#define REG_HAC_FIFO_ADDR(i)        (REG_HAC_FIFO0 + 4 * (i))

#define REG_AUDIO_I2S0_TDM_BASE     0x144000
#define REG_AUDIO_I2S1_BASE         0x144020
#define REG_AUDIO_I2S2_BASE         0x144040
#define REG_AUDIO_I2S_ADDR(i)       (REG_AUDIO_I2S0_TDM_BASE + 0x20 * (i))

#define REG_AUDIO_SPDIF_BASE        0x144060

#define REG_AUDIO_HAC_BASE          0x1440a0

#define REG_AUDIO_MATRIX_BASE       0x144480

#define REG_AUDIO_ANC0              0x144540
#define REG_AUDIO_ANC1              0x145dc0
#define REG_AUDIO_ANC(i)            (REG_AUDIO_ANC0 + 0x1880 * (i))

#define REG_AUDIO_ANC0_BASE         0x145ac0
#define REG_AUDIO_ANC1_BASE         0x147340
#define REG_AUDIO_ANC_BASE(i)       (REG_AUDIO_ANC0_BASE + 0x1880 * (i))

#define REG_AUDIO_CODEC_CTRL_BASE   0x147640

#define REG_AUDIO_CLK_IRQ_CTRL_BASE 0x1476c0

#define REG_AUDIO_CODEC0_BASE       0x147700
#define REG_AUDIO_CODEC_DAC_BASE    0x147740
#define REG_AUDIO_CODEC1_DMIC_BASE  0x147760

#define REG_AUDIO_ASRC_COFF_BASE    0x1477b0
#define REG_AUDIO_DMA_BASE          0x147ad0

/**************************************************** I2S register *****************************************************************/
#define reg_audio_i2s_cfg1(i2s) REG_ADDR8(REG_AUDIO_I2S_ADDR(i2s)) /* i2s[0-2] */

enum
{
    FLD_I2S_CLK_EN     = BIT(2),
    FLD_I2S_CLK_DIV2   = BIT(3),
    FLD_I2S_BCLK_INV_O = BIT(4),
    FLD_I2S_ADC_DCI_MS = BIT(5),
    FLD_I2S_DAC_DCI_MS = BIT(6),
};

#define reg_audio_i2s_cfg2(i2s) REG_ADDR8(REG_AUDIO_I2S_ADDR(i2s) + 0x01) /* i2s[0-2] */

enum
{
    FLD_I2S_ADC_MBCLK_LOOP = BIT(0),
    FLD_I2S_DAC_MBCLK_LOOP = BIT(1),
    FLD_I2S_FRM_INV        = BIT(2),
    FLD_I2S_Wl             = BIT_RNG(3, 4),
    FLD_I2S_FORMAT         = BIT_RNG(5, 7),                               /**< i2s1/i2s2 bit[5-6] */
};

#define reg_audio_i2s_cfg3(i2s) REG_ADDR8(REG_AUDIO_I2S_ADDR(i2s) + 0x02) /* i2s[0-2] */

enum
{
    FLD_I2S_LR_SWAP     = BIT(0),
    FLD_I2S_RX_2LINE_EN = BIT(1),
    FLD_I2S_TX_2LINE_EN = BIT(2),
    FLD_I2S_DAC_FRM_EN  = BIT(3),
    FLD_I2S_ADC_FRM_EN  = BIT(4),
    FLD_I2S_TX_DAT_SEL  = BIT(5),
    FLD_I2S_LRP         = BIT(6),
};

#define reg_audio_i2s_route(i2s) REG_ADDR8(REG_AUDIO_I2S_ADDR(i2s) + 0x03) /* i2s[0-2] */

enum
{
    FLD_I2S_MODE         = BIT_RNG(0, 1),
    FLD_I2S_PAD_BCLK_SEL = BIT(2),
    FLD_I2S_REC_BIT_SEL  = BIT(3),
    FLD_I2S_SCHEDULE_EN  = BIT(4),
};

#define reg_audio_i2s_int_pcm_num(i2s) REG_ADDR16(REG_AUDIO_I2S_ADDR(i2s) + 0x04) /* i2s[0-2] */

enum
{
    FLD_I2S_INT_PCM_NUM = BIT_RNG(0, 12),
};

#define reg_audio_i2s_dec_pcm_num(i2s) REG_ADDR16(REG_AUDIO_I2S_ADDR(i2s) + 0x06) /* i2s[0-2] */

enum
{
    FLD_I2S_DEC_PCM_NUM = BIT_RNG(0, 12),
};

#define reg_audio_i2s_pcm_clk_num(i2s) REG_ADDR8(REG_AUDIO_I2S_ADDR(i2s) + 0x08) /* i2s[0-2] */

#define reg_audio_i2s0_dac_tune        REG_ADDR8(REG_AUDIO_I2S0_TDM_BASE + 0x09)

enum
{
    FLD_I2S_DAC_TUNE_L1 = BIT_RNG(0, 3),
    FLD_I2S_DAC_TUNE_L2 = BIT_RNG(4, 7),
};

#define reg_audio_i2s0_adc_tune REG_ADDR8(REG_AUDIO_I2S0_TDM_BASE + 0x0a)

enum
{
    FLD_I2S_ADC_TUNE_L1 = BIT_RNG(0, 3),
    FLD_I2S_ADC_TUNE_L2 = BIT_RNG(4, 7),
};

#define reg_audio_i2s0_fifo_cfg REG_ADDR8(REG_AUDIO_I2S0_TDM_BASE + 0x0b)

enum
{
    FLD_I2S_TX_FIFO_LESS_L1 = BIT(0),
    FLD_I2S_TX_FIFO_LESS_L2 = BIT(1),
    FLD_I2S_TX_FIFO_MORE_L1 = BIT(2),
    FLD_I2S_TX_FIFO_MORE_L2 = BIT(3),
    FLD_I2S_RX_FIFO_LESS_L1 = BIT(4),
    FLD_I2S_RX_FIFO_LESS_L2 = BIT(5),
    FLD_I2S_RX_FIFO_MORE_L1 = BIT(6),
    FLD_I2S_RX_FIFO_MORE_L2 = BIT(7),
};

#define reg_audio_i2s_stimer_target(i2s) REG_ADDR32(REG_AUDIO_I2S_ADDR(i2s) + 0x0c) /* i2s[0-2] */

#define reg_audio_i2s0_align_cfg         REG_ADDR8(REG_AUDIO_I2S0_TDM_BASE + 0x10)

enum
{
    FLD_I2S_ALIGN_EN   = BIT(0),
    FLD_I2S_ALIGN_CTRL = BIT_RNG(1, 3),
    FLD_I2S_ALIGN_MASK = BIT(4),
    FLD_I2S_CLK_SEL    = BIT(5),
};

#define reg_audio_i2s0_tdm_cfg REG_ADDR8(REG_AUDIO_I2S0_TDM_BASE + 0x11)

enum
{
    FLD_I2S_TDM_RX_CH_NUM = BIT_RNG(0, 1),
    FLD_I2S_TDM_TX_CH_NUM = BIT_RNG(2, 3),
    FLD_I2S_TDM_MODE      = BIT_RNG(4, 5),
    FLD_I2S_TDM_SLOT      = BIT_RNG(6, 7),
};

#define reg_audio_i2s0_timer_th REG_ADDR32(REG_AUDIO_I2S0_TDM_BASE + 0x14)

/**************************************************** SPDIF register *****************************************************************/
#define reg_audio_spdif_fs_192_min1  REG_ADDR8(REG_AUDIO_SPDIF_BASE)
#define reg_audio_spdif_fs_192_min2  REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x01)
#define reg_audio_spdif_fs_192_min3  REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x02)

#define reg_audio_spdif_fs_192_max1  REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x03)
#define reg_audio_spdif_fs_192_max2  REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x04)
#define reg_audio_spdif_fs_192_max3  REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x05)

#define reg_audio_spdif_fs_96_min1   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x06)
#define reg_audio_spdif_fs_96_min2   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x07)
#define reg_audio_spdif_fs_96_min3   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x08)

#define reg_audio_spdif_fs_96_max1   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x09)
#define reg_audio_spdif_fs_96_max2   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x0a)
#define reg_audio_spdif_fs_96_max3   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x0b)

#define reg_audio_spdif_fs_48_min1   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x0c)
#define reg_audio_spdif_fs_48_min2   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x0d)
#define reg_audio_spdif_fs_48_min3   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x0e)

#define reg_audio_spdif_fs_48_max1   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x0f)
#define reg_audio_spdif_fs_48_max2   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x10)
#define reg_audio_spdif_fs_48_max3   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x11)

#define reg_audio_spdif_fs_44p1_min1 REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x12)
#define reg_audio_spdif_fs_44p1_min2 REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x13)
#define reg_audio_spdif_fs_44p1_min3 REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x14)

#define reg_audio_spdif_fs_44p1_max1 REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x15)
#define reg_audio_spdif_fs_44p1_max2 REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x16)
#define reg_audio_spdif_fs_44p1_max3 REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x17)

#define reg_audio_spdif_fs_32_min1   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x18)
#define reg_audio_spdif_fs_32_min2   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x19)
#define reg_audio_spdif_fs_32_min3   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x1a)

#define reg_audio_spdif_fs_32_max1   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x1b)
#define reg_audio_spdif_fs_32_max2   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x1c)
#define reg_audio_spdif_fs_32_max3   REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x1d)

#define reg_audio_spdif_config       REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x1e)

enum
{
    FLD_SPDIF_ENCODER_EN      = BIT(0),
    FLD_SPDIF_DECODER_EN      = BIT(1),
    FLD_SPDIF_PARITY_SELECT   = BIT(2),
    FLD_SPDIF_PARITY_RESULT   = BIT(3),
    FLD_SPDIF_DBG_CLK_EN      = BIT(4),
    FLD_SPDIF_RX_FMT          = BIT(5),
    FLD_SPDIF_TX_FMT          = BIT(6),
    FLD_SPDIF_PREAMBLE_SELECT = BIT(7),
};

#define reg_audio_spdif_tx_div REG_ADDR8(REG_AUDIO_SPDIF_BASE + 0x1f)

enum
{
    FLD_SPDIF_TX_DIV_NUM = BIT_RNG(0, 4),
};

/**************************************************** HAC register *****************************************************************/
#define reg_audio_hac_asrc_ch_en REG_ADDR8(REG_AUDIO_HAC_BASE)

enum
{
    FLD_HAC_ASRC_CH0_EN = BIT(0),
    FLD_HAC_ASRC_CH1_EN = BIT(1),
    FLD_HAC_ASRC_CH2_EN = BIT(2),
    FLD_HAC_ASRC_CH3_EN = BIT(3),
};

#define reg_audio_hac_eq_config_en REG_ADDR8(REG_AUDIO_HAC_BASE + 0x01)

enum
{
    FLD_HAC_CH0_CONFIG_EN = BIT(0),
    FLD_HAC_CH1_CONFIG_EN = BIT(1),
    FLD_HAC_CH2_CONFIG_EN = BIT(2),
    FLD_HAC_CH3_CONFIG_EN = BIT(3),
};

#define reg_audio_hac_int_status REG_ADDR8(REG_AUDIO_HAC_BASE + 0x02)

enum
{
    FLD_HAC_RXFIFO0_INT = BIT(0), /**< W1C */
    FLD_HAC_RXFIFO1_INT = BIT(1), /**< W1C */
    FLD_HAC_RXFIFO2_INT = BIT(2), /**< W1C */
    FLD_HAC_RXFIFO3_INT = BIT(3), /**< W1C */
};

#define reg_audio_hac_tdm_num REG_ADDR8(REG_AUDIO_HAC_BASE + 0x03)

enum
{
    FLD_HAC_TDM0_NUM = BIT_RNG(0, 2),
    FLD_HAC_TDM1_NUM = BIT_RNG(4, 6),
};

#define reg_audio_hac_frac_adc(hac) REG_ADDR32(REG_AUDIO_HAC_BASE + 0x04 + ((hac) << 2)) /* hac[0-3] */

enum
{
    FLD_HAC_FRAC_ADC = BIT_RNG(0, 25),
};

#define reg_audio_hac_den_rate(hac) REG_ADDR32(REG_AUDIO_HAC_BASE + 0x14 + ((hac) << 2)) /* hac[0-3] */

enum
{
    FLD_HAC_DEN_RATE = BIT_RNG(0, 25),
};

#define reg_audio_hac_bq_b0(hac, bq) REG_ADDR32(REG_AUDIO_HAC_BASE + 0x24 + (bq) * 0x14 + (hac) * 0xc8) /* hac[0-3] bq[0-9] */
#define reg_audio_hac_bq_b1(hac, bq) REG_ADDR32(REG_AUDIO_HAC_BASE + 0x28 + (bq) * 0x14 + (hac) * 0xc8) /* hac[0-3] bq[0-9] */
#define reg_audio_hac_bq_b2(hac, bq) REG_ADDR32(REG_AUDIO_HAC_BASE + 0x2c + (bq) * 0x14 + (hac) * 0xc8) /* hac[0-3] bq[0-9] */
#define reg_audio_hac_bq_a1(hac, bq) REG_ADDR32(REG_AUDIO_HAC_BASE + 0x30 + (bq) * 0x14 + (hac) * 0xc8) /* hac[0-3] bq[0-9] */
#define reg_audio_hac_bq_a2(hac, bq) REG_ADDR32(REG_AUDIO_HAC_BASE + 0x34 + (bq) * 0x14 + (hac) * 0xc8) /* hac[0-3] bq[0-9] */

#define reg_audio_hac_tx_fifo_clr    REG_ADDR8(REG_AUDIO_HAC_BASE + 0x344)

enum
{
    FLD_HAC_TXFIFO0_CLR = BIT(0),
    FLD_HAC_TXFIFO1_CLR = BIT(1),
    FLD_HAC_TXFIFO2_CLR = BIT(2),
    FLD_HAC_TXFIFO3_CLR = BIT(3),
};

#define reg_audio_hac_mux_sel REG_ADDR8(REG_AUDIO_HAC_BASE + 0x345)

enum
{
    FLD_HAC_MUX_SEL = BIT_RNG(0, 3),
};

#define reg_audio_hac_rx_afifo_clr REG_ADDR8(REG_AUDIO_HAC_BASE + 0x346)

enum
{
    FLD_HAC_RX_AFIFO0_CLR = BIT(0),
    FLD_HAC_RX_AFIFO1_CLR = BIT(1),
    FLD_HAC_RX_AFIFO2_CLR = BIT(2),
    FLD_HAC_RX_AFIFO3_CLR = BIT(3),
};

#define reg_audio_hac_tx_fifo_overnum REG_ADDR8(REG_AUDIO_HAC_BASE + 0x347)

enum
{
    FLD_HAC_TXFIFO0_OVERRUN = BIT(0),
    FLD_HAC_TXFIFO1_OVERRUN = BIT(1),
    FLD_HAC_TXFIFO2_OVERRUN = BIT(2),
    FLD_HAC_TXFIFO3_OVERRUN = BIT(3),
};

#define reg_audio_hac_rx_afifo_num(hac) REG_ADDR8(REG_AUDIO_HAC_BASE + 0x348 + (hac)) /* hac[0-3] */

enum
{
    FLD_HAC_RX_AFIFO_NUM = BIT_RNG(0, 3),
};

#define reg_audio_hac_arb_fifo_clr REG_ADDR8(REG_AUDIO_HAC_BASE + 0X34c)

enum
{
    FLD_HAC_ARB_FIFO_CLR = BIT(0),
};

#define reg_audio_hac_hmst_sel REG_ADDR8(REG_AUDIO_HAC_BASE + 0x34d)

enum
{
    FLD_HAC_HMST_SEL = BIT_RNG(0, 3),
};

#define reg_audio_hac_haddr_set REG_ADDR8(REG_AUDIO_HAC_BASE + 0x34e)

enum
{
    FLD_HAC_HADDR0_SET = BIT(0),
    FLD_HAC_HADDR1_SET = BIT(1),
    FLD_HAC_HADDR2_SET = BIT(2),
    FLD_HAC_HADDR3_SET = BIT(3),
};

#define reg_audio_hac_ch_level   REG_ADDR8(REG_AUDIO_HAC_BASE + 0x34f)

#define reg_audio_hac_haddr(hac) REG_ADDR32(REG_AUDIO_HAC_BASE + 0x350 + ((hac) << 2)) /* hac[0-3] */

#define reg_audio_hac_hburst     REG_ADDR8(REG_AUDIO_HAC_BASE + 0x360)

enum
{
    FLD_HAC_HBURST   = BIT_RNG(0, 2),
    FLD_HAC_CROSS_1K = BIT(4),
};

#define reg_audio_hac_rx_fifo_clr REG_ADDR8(REG_AUDIO_HAC_BASE + 0x361)

enum
{
    FLD_HAC_RXFIFO0_CLR = BIT(0),
    FLD_HAC_RXFIFO1_CLR = BIT(1),
    FLD_HAC_RXFIFO2_CLR = BIT(2),
    FLD_HAC_RXFIFO3_CLR = BIT(3),
};

#define reg_audio_hac_bypass_asrc REG_ADDR8(REG_AUDIO_HAC_BASE + 0x362)

enum
{
    FLD_HAC_BYPASS_ASRC0 = BIT(0),
    FLD_HAC_BYPASS_ASRC1 = BIT(1),
    FLD_HAC_BYPASS_ASRC2 = BIT(2),
    FLD_HAC_BYPASS_ASRC3 = BIT(3),
};

#define reg_audio_hac_txafifo_num REG_ADDR8(REG_AUDIO_HAC_BASE + 0x363)

enum
{
    FLD_HAC_TX_AFIFO_NUM = BIT_RNG(0, 4),
};

#define reg_audio_hac_rxfifo_overnum REG_ADDR8(REG_AUDIO_HAC_BASE + 0x364)

enum
{
    FLD_HAC_RXFIFO0_OVERRUN = BIT(0),
    FLD_HAC_RXFIFO1_OVERRUN = BIT(1),
    FLD_HAC_RXFIFO2_OVERRUN = BIT(2),
    FLD_HAC_RXFIFO3_OVERRUN = BIT(3),
};

#define reg_audio_hac_mtrx_in_fmt_sel REG_ADDR8(REG_AUDIO_HAC_BASE + 0x365)

enum
{
    FLD_HAC_MTRX_IN_FMT_SEL0 = BIT(0),
    FLD_HAC_MTRX_IN_FMT_SEL1 = BIT(1),
    FLD_HAC_MTRX_IN_FMT_SEL2 = BIT(2),
    FLD_HAC_MTRX_IN_FMT_SEL3 = BIT(3),
};

#define reg_audio_hac_mtrx_out_fmt_sel_l REG_ADDR8(REG_AUDIO_HAC_BASE + 0x366)

enum
{
    FLD_HAC_MTRX_OUT_FMT_SEL0 = BIT_RNG(0, 1),
    FLD_HAC_MTRX_OUT_FMT_SEL1 = BIT_RNG(2, 3),
    FLD_HAC_MTRX_OUT_FMT_SEL2 = BIT_RNG(4, 5),
    FLD_HAC_MTRX_OUT_FMT_SEL3 = BIT_RNG(6, 7),
};

#define reg_audio_hac_ahb_dat_sel REG_ADDR8(REG_AUDIO_HAC_BASE + 0x367)

enum
{
    FLD_HAC_AHB_MST_DAT_SEL = BIT_RNG(0, 1),
    FLD_HAC_AHB_SLV_DAT_SEL = BIT(2),
};

#define reg_audio_hac_txfifo_rd_num(hac) REG_ADDR16(REG_AUDIO_HAC_BASE + 0x368 + ((hac) << 1)) /* hac[0-3] */

enum
{
    FLD_HAC_TXFIFO_RD_NUM = BIT_RNG(0, 11),
};

#define reg_audio_hac_asrc_interval REG_ADDR8(REG_AUDIO_HAC_BASE + 0x370)

enum
{
    FLD_HAC_INTERVAL_ASRC0 = BIT_RNG(0, 1),
    FLD_HAC_INTERVAL_ASRC1 = BIT_RNG(2, 3),
    FLD_HAC_INTERVAL_ASRC2 = BIT_RNG(4, 5),
    FLD_HAC_INTERVAL_ASRC3 = BIT_RNG(6, 7),
};

#define reg_audio_hac_rxafifo_thres(hac) REG_ADDR8(REG_AUDIO_HAC_BASE + 0x371 + ((hac) >> 1)) /* hac[0-3] */

enum
{
    FLD_HAC_RX_AFIFO_EVEN_THRES = BIT_RNG(0, 3),
    FLD_HAC_RX_AFIFO_ODD_THRES  = BIT_RNG(4, 7),
};

#define reg_audio_hac_int_en REG_ADDR8(REG_AUDIO_HAC_BASE + 0x373)

enum
{
    FLD_HAC_INT0_EN = BIT(0),
    FLD_HAC_INT1_EN = BIT(1),
    FLD_HAC_INT2_EN = BIT(2),
    FLD_HAC_INT3_EN = BIT(3),
};

#define reg_audio_tx_fifo_num(hac) REG_ADDR8(REG_AUDIO_HAC_BASE + 0x378 + (hac)) /* hac[0-3] */

enum
{
    FLD_HAC_TXFIFO_NUM = BIT_RNG(0, 6),                                          /**< hac0, hac1[0-4] */
};

#define reg_audio_hac_int_adv(hac) REG_ADDR8(REG_AUDIO_HAC_BASE + 0x380 + (hac)) /* hac[0-3] */

enum
{
    FLD_HAC_INT_ADV = BIT_RNG(0, 6),
};

#define reg_audio_hac_rd_1_num(hac) REG_ADDR16(REG_AUDIO_HAC_BASE + 0x384 + ((hac - 2) << 1)) /* hac[2-3] */

enum
{
    FLD_HAC_RD_1_NUM = BIT_RNG(0, 11),
};

#define reg_audio_hac_rd_num(hac) REG_ADDR16(REG_AUDIO_HAC_BASE + 0x388 + ((hac) << 1)) /* hac[0-3] */

enum
{
    FLD_HAC_RD_NUM = BIT_RNG(0, 11),
};

#define reg_audio_hac_fifo_cnt_clr REG_ADDR8(REG_AUDIO_HAC_BASE + 0x390)

enum
{
    FLD_HAC_FIFO0_CNT_CLR = BIT(0),
    FLD_HAC_FIFO1_CNT_CLR = BIT(1),
    FLD_HAC_FIFO2_CNT_CLR = BIT(2),
    FLD_HAC_FIFO3_CNT_CLR = BIT(3),
};

#define reg_audio_hac_asrc_lag_update REG_ADDR8(REG_AUDIO_HAC_BASE + 0x391)

enum
{
    FLD_HAC_ASRC0_LAG_UPDATE = BIT(0),
    FLD_HAC_ASRC1_LAG_UPDATE = BIT(1),
    FLD_HAC_ASRC2_LAG_UPDATE = BIT(2),
    FLD_HAC_ASRC3_LAG_UPDATE = BIT(3),
};

#define reg_audio_hac_asrc_6ch_en REG_ADDR8(REG_AUDIO_HAC_BASE + 0x392)

enum
{
    FLD_HAC_ASRC_6CH_HAC3_FIRST_EN = BIT(0),
    FLD_HAC_ASRC_6CH_HAC2_FIRST_EN = BIT(1),
};

#define reg_audio_hac_asrc_timeout(asrc) REG_ADDR16(REG_AUDIO_HAC_BASE + 0x398 + ((asrc) << 1)) /* asrc[0-3] */

enum
{
    FLD_HAC_ASRC_TIMEOUT = BIT_RNG(0, 8),
};

#define reg_audio_hac_rx_fifo_cnt(hac) REG_ADDR32(REG_AUDIO_HAC_BASE + 0x3a0 + ((hac) << 2)) /* hac[0-3] */

enum
{
    FLD_HAC_RX_FIFO_CNT = BIT_RNG(0, 23),
};

#define reg_audio_hac_tx_fifo_cnt_ind(hac) REG_ADDR32(REG_AUDIO_HAC_BASE + 0x3b0 + ((hac) << 2)) /* hac[0-3] */

enum
{
    FLD_HAC_TX_FIFO_CNT      = BIT_RNG(0, 23),
    FLD_HAC_TX_FIFO_INDICATE = BIT(24),
};

/**************************************************** MATRIX register *****************************************************************/
#define reg_audio_matrix_srst REG_ADDR8(REG_AUDIO_MATRIX_BASE)

enum
{
    FLD_MATRIX_I2S0_RX_SRST = BIT(0),
    FLD_MATRIX_I2S1_RX_SRST = BIT(1),
    FLD_MATRIX_I2S2_RX_SRST = BIT(2),
    FLD_MATRIX_ANC0_RX_SRST = BIT(3),
    FLD_MATRIX_ANC1_RX_SRST = BIT(4),
    FLD_MATRIX_ADC0_RX_SRST = BIT(5),
    FLD_MATRIX_ADC1_RX_SRST = BIT(6),
    FLD_MATRIX_ADC2_RX_SRST = BIT(7),
};

#define reg_audio_matrix_srst2 REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x01)

enum
{
    FLD_MATRIX_I2S0_TX_SRST  = BIT(0),
    FLD_MATRIX_I2S1_TX_SRST  = BIT(1),
    FLD_MATRIX_I2S2_TX_SRST  = BIT(2),
    FLD_MATRIX_ANC0_SRC_SRST = BIT(3),
    FLD_MATRIX_DACL_SRST     = BIT(4),
    FLD_MATRIX_SDTN_SRST     = BIT(5),
    /* RVSD */
};

#define reg_audio_matrix_i2s0_rx_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x02)

enum
{
    FLD_MATRIX_I2S0_RX_SEL = BIT_RNG(0, 2),
};

#define reg_audio_matrix_i2s0_ch01_tx_sel    REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x03)
#define reg_audio_matrix_i2s0_ch23_tx_sel    REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x04)
#define reg_audio_matrix_i2s0_ch45_tx_sel    REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x05)
#define reg_audio_matrix_i2s0_ch67_tx_sel    REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x06)
#define reg_audio_matrix_i2s0_ch_tx_sel(chn) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x03 + (chn >> 1)) /* i2s0_chn[0-7] */

enum
{
    FLD_MATRIX_I2S0_EVEN_TX_SEL = BIT_RNG(0, 3),
    FLD_MATRIX_I2S0_ODD_TX_SEL  = BIT_RNG(4, 7),
};

#define reg_audio_matrix_i2s0_tx_dma_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x07)

enum
{
    FLD_MATRIX_I2S0_TX_DMA_SEL = BIT_RNG(0, 5),
};

#define reg_audio_matrix_i2s1_tx_dma_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x08)

enum
{
    FLD_MATRIX_I2S1_TX_DMA_SEL = BIT_RNG(0, 4),
};

#define reg_audio_matrix_i2s1_rx_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x08)

enum
{
    FLD_MATRIX_I2S1_RX_SEL = BIT_RNG(5, 7),
};

#define reg_audio_matrix_i2s2_tx_dma_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x09)

enum
{
    FLD_MATRIX_I2S2_TX_DMA_SEL = BIT_RNG(0, 4),
};

#define reg_audio_matrix_i2s1_ch01_tx_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x0a)

enum
{
    FLD_MATRIX_I2S1_CH0_TX_SEL = BIT_RNG(0, 3),
    FLD_MATRIX_I2S1_CH1_TX_SEL = BIT_RNG(4, 7),
};

#define reg_audio_matrix_i2s2_rx_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x0c)

enum
{
    FLD_MATRIX_I2S2_RX_SEL = BIT_RNG(0, 3),
};

#define reg_audio_matrix_i2s2_ch01_tx_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x0d)

enum
{
    FLD_MATRIX_I2S2_CH0_TX_SEL = BIT_RNG(0, 3),
    FLD_MATRIX_I2S2_CH1_TX_SEL = BIT_RNG(4, 7),
};

#define reg_audio_matrix_anc0_rx_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x0e)

enum
{
    FLD_MATRIX_ANC0_RX_SEL    = BIT_RNG(0, 1),
    FLD_MATRIX_ANC0_RX_V2_SEL = BIT_RNG(4, 5),
};

#define reg_audio_matrix_anc1_rx_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x0f)

enum
{
    FLD_MATRIX_ANC1_RX_SEL    = BIT_RNG(0, 2),
    FLD_MATRIX_ANC1_RX_V2_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_anc0_src_dma_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x10)
#define reg_audio_matrix_anc1_src_dma_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x11)
#define reg_audio_matrix_anc_src_dma_sel(anc) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x10 + (anc)) /* anc[0-1] */

enum
{
    FLD_MATRIX_ANC_SRC_DMA_SEL = BIT_RNG(0, 3),
};

#define reg_audio_matrix_dac_dma_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x12)

enum
{
    FLD_MATRIX_DAC_DMA_SEL = BIT_RNG(0, 4),
};

#define reg_audio_matrix_adc01_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x13)

enum
{
    FLD_MATRIX_ADC0_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_ADC1_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_adc2_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x14)

enum
{
    FLD_MATRIX_ADC2_SEL = BIT_RNG(0, 2),
};

#define reg_audio_matrix_dac_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x15)

enum
{
    FLD_MATRIX_DAC_L_SEL = BIT_RNG(0, 3),
    FLD_MATRIX_DAC_R_SEL = BIT_RNG(4, 7),
};

#define reg_audio_matrix_tx_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x16)

enum
{
    FLD_MATRIX_SPDIF_TX_SEL = BIT_RNG(0, 3),
    FLD_MATRIX_USB_TX_SEL   = BIT_RNG(4, 7),
};

#define reg_audio_matrix_hac0_tx_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x18)
#define reg_audio_matrix_hac1_tx_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x19)
#define reg_audio_matrix_hac2_tx_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x1a)
#define reg_audio_matrix_hac3_tx_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x1b)
#define reg_audio_matrix_hac_tx_sel(hac) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x18 + (hac)) /* hac[0-3] */

enum
{
    FLD_MATRIX_HAC_TX_SEL = BIT_RNG(0, 4),
};

#define reg_audio_matrix_anc0_src_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x1e)
#define reg_audio_matrix_anc1_src_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x21)
#define reg_audio_matrix_anc_src_sel(anc) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x1e + (anc) * 3) /* anc[0-1] */

enum
{
    FLD_MATRIX_ANC_SRC_SEL = BIT_RNG(0, 4),
};

#define reg_audio_matrix_anc0_ref_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x1f)
#define reg_audio_matrix_anc1_ref_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x22)
#define reg_audio_matrix_anc_ref_sel(anc) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x1f + (anc) * 3) /* anc[0-1] */

enum
{
    FLD_MATRIX_ANC_REF_SEL = BIT_RNG(0, 4),
};

#define reg_audio_matrix_anc0_err_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x20)
#define reg_audio_matrix_anc1_err_sel     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x23)
#define reg_audio_matrix_anc_err_sel(anc) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x20 + (anc) * 3) /* anc[0-1] */

enum
{
    FLD_MATRIX_ANC_ERR_SEL = BIT_RNG(0, 4),
};

#define reg_audio_matrix_fifo0_wr_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x24)
#define reg_audio_matrix_fifo1_wr_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x25)
#define reg_audio_matrix_fifo2_wr_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x26)
#define reg_audio_matrix_fifo3_wr_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x27)
#define reg_audio_matrix_fifo_wr_sel(fifo) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x24 + (fifo)) /* fifo[0-3] */

enum
{
    FLD_MATRIX_FIFO_WR_SEL = BIT_RNG(0, 4),
};

#define reg_audio_matrix_i2s0_rx_hac01_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x28)
#define reg_audio_matrix_i2s0_rx_hac23_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x29)
#define reg_audio_matrix_i2s1_rx_hac01_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x2f)
#define reg_audio_matrix_i2s1_rx_hac23_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x30)
#define reg_audio_matrix_i2s2_rx_hac01_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x36)
#define reg_audio_matrix_i2s2_rx_hac23_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x37)
#define reg_audio_matrix_i2s_rx_hac_sel(i2s, hac) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x28 + (i2s) * 7 + ((hac) >> 1)) /* i2s[0-2] hac[0-3] */

enum
{
    FLD_MATRIX_I2S_RX_HAC_EVEN_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_I2S_RX_HAC_ODD_SEL  = BIT_RNG(4, 6),
};

#define reg_audio_matrix_i2s0_tx_hac2_sel         REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x2a)
#define reg_audio_matrix_i2s0_tx_hac3_sel         REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x2b)
#define reg_audio_matrix_i2s1_tx_hac2_sel         REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x31)
#define reg_audio_matrix_i2s1_tx_hac3_sel         REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x32)
#define reg_audio_matrix_i2s2_tx_hac2_sel         REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x38)
#define reg_audio_matrix_i2s2_tx_hac3_sel         REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x39)
#define reg_audio_matrix_i2s_tx_hac_sel(i2s, hac) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x2a + (i2s) * 7 + ((hac) % 2)) /* i2s[0-2] hac[2-3] */

enum
{
    FLD_MATRIX_I2S_TX_HAC_EVEN_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_I2S_TX_HAC_ODD_SEL  = BIT_RNG(4, 6),
};

#define reg_audio_matrix_i2s0_rx_anc_sel_0     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x2c)
#define reg_audio_matrix_i2s1_rx_anc_sel_0     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x33)
#define reg_audio_matrix_i2s2_rx_anc_sel_0     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x3a)
#define reg_audio_matrix_i2s_rx_anc_sel_0(i2s) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x2c + (i2s) * 7) /* i2s[0-2] */

enum
{
    FLD_MATRIX_I2S_RX_ANC0_SRC_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_I2S_RX_ANC0_REF_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_i2s0_rx_anc_sel_1     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x2d)
#define reg_audio_matrix_i2s1_rx_anc_sel_1     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x34)
#define reg_audio_matrix_i2s2_rx_anc_sel_1     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x3b)
#define reg_audio_matrix_i2s_rx_anc_sel_1(i2s) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x2d + (i2s) * 7) /* i2s[0-2] */

enum
{
    FLD_MATRIX_I2S_RX_ANC0_ERR_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_I2S_RX_ANC1_SRC_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_i2s0_rx_anc_sel_2     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x2e)
#define reg_audio_matrix_i2s1_rx_anc_sel_2     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x35)
#define reg_audio_matrix_i2s2_rx_anc_sel_2     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x3c)
#define reg_audio_matrix_i2s_rx_anc_sel_2(i2s) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x2e + (i2s) * 7) /* i2s[0-2] */

enum
{
    FLD_MATRIX_I2S_RX_ANC1_REF_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_I2S_RX_ANC1_ERR_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_adc0_rx_hac01_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x3d)
#define reg_audio_matrix_adc0_rx_hac23_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x3e)
#define reg_audio_matrix_adc1_rx_hac01_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x44)
#define reg_audio_matrix_adc1_rx_hac23_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x45)
#define reg_audio_matrix_adc2_rx_hac01_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x4b)
#define reg_audio_matrix_adc2_rx_hac23_sel        REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x4c)
#define reg_audio_matrix_adc_rx_hac_sel(adc, hac) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x3d + (adc) * 7 + ((hac) >> 1)) /* adc[0-2] hac[0-3] */

enum
{
    FLD_MATRIX_ADC_RX_HAC_EVEN_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_ADC_RX_HAC_ODD_SEL  = BIT_RNG(4, 6),
};

#define reg_audio_matrix_adc0_rx_anc_sel_0     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x41)
#define reg_audio_matrix_adc1_rx_anc_sel_0     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x48)
#define reg_audio_matrix_adc2_rx_anc_sel_0     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x4f)
#define reg_audio_matrix_adc_rx_anc_sel_0(adc) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x41 + (adc) * 7) /* adc[0-2] */

enum
{
    FLD_MATRIX_ADC_RX_ANC0_SRC_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_ADC_RX_ANC0_REF_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_adc0_rx_anc_sel_1     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x42)
#define reg_audio_matrix_adc1_rx_anc_sel_1     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x49)
#define reg_audio_matrix_adc2_rx_anc_sel_1     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x50)
#define reg_audio_matrix_adc_rx_anc_sel_1(adc) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x42 + (adc) * 7) /* adc[0-2] */

enum
{
    FLD_MATRIX_ADC_RX_ANC0_ERR_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_ADC_RX_ANC1_SRC_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_adc0_rx_anc_sel_2     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x43)
#define reg_audio_matrix_adc1_rx_anc_sel_2     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x4a)
#define reg_audio_matrix_adc2_rx_anc_sel_2     REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x51)
#define reg_audio_matrix_adc_rx_anc_sel_2(adc) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x43 + (adc) * 7) /* adc[0-2] */

enum
{
    FLD_MATRIX_ADC_RX_ANC1_REF_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_ADC_RX_ANC1_ERR_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_sdtn0_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x52)
#define reg_audio_matrix_sdtn1_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x53)
#define reg_audio_matrix_sdtn2_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x54)
#define reg_audio_matrix_sdtn3_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x55)
#define reg_audio_matrix_sdtn4_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x56)
#define reg_audio_matrix_sdtn5_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x57)
#define reg_audio_matrix_sdtn6_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x58)
#define reg_audio_matrix_sdtn7_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x59)
#define reg_audio_matrix_sdtn_sel(sdtn) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x52 + (sdtn)) /* sdtn[0-7] */

enum
{
    FLD_MATRIX_SDTN_SEL = BIT_RNG(0, 4),
};

#define reg_audio_matrix_i2s_sdtn0_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x5a)
#define reg_audio_matrix_i2s_sdtn1_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x5d)
#define reg_audio_matrix_i2s_sdtn2_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x60)
#define reg_audio_matrix_i2s_sdtn3_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x63)
#define reg_audio_matrix_i2s_sdtn4_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x66)
#define reg_audio_matrix_i2s_sdtn5_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x69)
#define reg_audio_matrix_i2s_sdtn6_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x6c)
#define reg_audio_matrix_i2s_sdtn7_sel      REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x6f)
#define reg_audio_matrix_i2s_sdtn_sel(sdtn) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x5a + (sdtn) * 3) /* sdtn[0-7] */

enum
{
    FLD_MATRIX_I2S0_SDTN_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_I2S1_SDTN_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_i2s_adc_sdtn_sel(sdtn) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x5b + (sdtn) * 3) /* sdtn[0-7] */

enum
{
    FLD_MATRIX_I2S2_SDTN_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_ADC0_SDTN_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_adc_sdtn_sel(sdtn) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x5c + (sdtn) * 3) /* sdtn[0-7] */

enum
{
    FLD_MATRIX_ADC1_SDTN_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_ADC2_SDTN_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_sdtn_en REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x74)

enum
{
    FLD_MATRIX_SDTN0_EN = BIT(0),
    FLD_MATRIX_SDTN1_EN = BIT(1),
    FLD_MATRIX_SDTN2_EN = BIT(2),
    FLD_MATRIX_SDTN3_EN = BIT(3),
    FLD_MATRIX_SDTN4_EN = BIT(4),
    FLD_MATRIX_SDTN5_EN = BIT(5),
    FLD_MATRIX_SDTN6_EN = BIT(6),
    FLD_MATRIX_SDTN7_EN = BIT(7),
};

#define reg_audio_matrix_sdtn_dma_mode REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x75)

enum
{
    FLD_MATRIX_SDTN01_DMA_MODE = BIT(0),
    FLD_MATRIX_SDTN23_DMA_MODE = BIT(1),
    FLD_MATRIX_SDTN45_DMA_MODE = BIT(2),
    FLD_MATRIX_SDTN67_DMA_MODE = BIT(3),
};

#define reg_audio_matrix_sdtn_gain_adc(sdtn) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x76 + ((sdtn) >> 1)) /* sdtn[0-7] */

enum
{
    FLD_MATRIX_SDTN_EVEN_ADC = BIT_RNG(0, 3),
    FLD_MATRIX_SDTN_ODD_ADC  = BIT_RNG(4, 7),
};

#define reg_audio_matrix_sdtn_gain_dac(sdtn) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x7a + ((sdtn) >> 1)) /* sdtn[0-7] */

enum
{
    FLD_MATRIX_SDTN_EVEN_DAC = BIT_RNG(0, 3),
    FLD_MATRIX_SDTN_ODD_DAC  = BIT_RNG(4, 7),
};

#define reg_audio_matrix_sdtn_req_count(sdtns) REG_ADDR16(REG_AUDIO_MATRIX_BASE + 0x80 + (sdtns << 1)) /* sdtns[0-3] */

enum
{
    FLD_MATRIX_SDTN_REQ_COUNT = BIT_RNG(0, 11)
};

#define reg_audio_matrix_hac01_tx_dma_sel(fifo) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x88 + (((fifo) - 1) << 1))

enum
{
    FLD_MATRIX_HAC0_TX_DMAN_DATA_BITS_SEL = BIT_RNG(0, 1),
    FLD_MATRIX_HAC1_TX_DMAN_DATA_BITS_SEL = BIT_RNG(4, 5),
};

#define reg_audio_matrix_hac23_tx_dman_sel(fifo) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x89 + (((fifo) - 1) << 1))

enum
{
    FLD_MATRIX_HAC2_TX_DMAN_DATA_BITS_SEL = BIT_RNG(0, 2),
    FLD_MATRIX_HAC3_TX_DMAN_DATA_BITS_SEL = BIT_RNG(4, 6),
};

#define reg_audio_matrix_sdtn_dma_sel(sdtn) REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x90 + ((sdtn) >> 1)) /* sdtn[0-7] */

enum
{
    FLD_MATRIX_SDTN_DMA_SEL = BIT_RNG(0, 4),
};

#define reg_audio_matrix_tx_chn_swap REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x94)

enum
{
    FLD_MATRIX_I2S0_TX_CHN_SWAP = BIT(0),
    FLD_MATRIX_I2S1_TX_CHN_SWAP = BIT(1),
    FLD_MATRIX_I2S2_TX_CHN_SWAP = BIT(2),
    FLD_MATRIX_DAC_TX_CHN_SWAP  = BIT(3),
};

#define reg_audio_matrix_fifo_rx_hac01_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x95)

enum
{
    FLD_MATRIX_FIFO_RX_HAC0_SEL = BIT_RNG(0, 1),
    FLD_MATRIX_FIFO_RX_HAC1_SEL = BIT_RNG(4, 5),
};

#define reg_audio_matrix_fifo_rx_hac23_sel REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x96)

enum
{
    FLD_MATRIX_FIFO_RX_HAC2_SEL = BIT_RNG(0, 3),
    FLD_MATRIX_FIFO_RX_HAC3_SEL = BIT_RNG(4, 7),
};

#define reg_audio_hac_tdm_tx_dma_ch_num REG_ADDR8(REG_AUDIO_MATRIX_BASE + 0x97)

enum
{
    FLD_HAC_TDM_TX_DMA0_CH_NUM = BIT_RNG(0, 1),
    FLD_HAC_TDM_TX_DMA1_CH_NUM = BIT_RNG(2, 3),
    FLD_HAC_TDM_TX_DMA2_CH_NUM = BIT_RNG(4, 5),
    FLD_HAC_TDM_TX_DMA3_CH_NUM = BIT_RNG(6, 7),
};

/**************************************************** ANC register *****************************************************************/
/* AUDIO_ANC register */
#define reg_audio_anc_wz_iir_b0(anc, wz_iir)  REG_ADDR32(REG_AUDIO_ANC(anc) + 0x00 + (wz_iir) * 0x20)                                           /* anc[0-1] wz_iir[0-23] */
#define reg_audio_anc_wz_iir_b1(anc, wz_iir)  REG_ADDR32(REG_AUDIO_ANC(anc) + 0x04 + (wz_iir) * 0x20)                                           /* anc[0-1] wz_iir[0-23] */
#define reg_audio_anc_wz_iir_b2(anc, wz_iir)  REG_ADDR32(REG_AUDIO_ANC(anc) + 0x08 + (wz_iir) * 0x20)                                           /* anc[0-1] wz_iir[0-23] */
#define reg_audio_anc_wz_iir_a1(anc, wz_iir)  REG_ADDR32(REG_AUDIO_ANC(anc) + 0x10 + (wz_iir) * 0x20)                                           /* anc[0-1] wz_iir[0-23] */
#define reg_audio_anc_wz_iir_a2(anc, wz_iir)  REG_ADDR32(REG_AUDIO_ANC(anc) + 0x14 + (wz_iir) * 0x20)                                           /* anc[0-1] wz_iir[0-23] */

#define reg_audio_anc_wz_fir(anc, wz_fir)     REG_ADDR16(REG_AUDIO_ANC(anc) + 0x300 + ((wz_fir) / 65) * 0x02 + ((wz_fir) % 65) * 0x10)          /* anc[0-1] wz_fir[0-389] */

#define reg_audio_anc_cz_iir_b0(anc, cz, iir) REG_ADDR32(REG_AUDIO_ANC(anc) + 0x740 + (cz) * 0x4c0 + (iir) * 0x20)                              /* anc[0-1] cz[0-2] iir[0-23] */
#define reg_audio_anc_cz_iir_b1(anc, cz, iir) REG_ADDR32(REG_AUDIO_ANC(anc) + 0x744 + (cz) * 0x4c0 + (iir) * 0x20)                              /* anc[0-1] cz[0-2] iir[0-23] */
#define reg_audio_anc_cz_iir_b2(anc, cz, iir) REG_ADDR32(REG_AUDIO_ANC(anc) + 0x748 + (cz) * 0x4c0 + (iir) * 0x20)                              /* anc[0-1] cz[0-2] iir[0-23] */
#define reg_audio_anc_cz_iir_a1(anc, cz, iir) REG_ADDR32(REG_AUDIO_ANC(anc) + 0x750 + (cz) * 0x4c0 + (iir) * 0x20)                              /* anc[0-1] cz[0-2] iir[0-23] */
#define reg_audio_anc_cz_iir_a2(anc, cz, iir) REG_ADDR32(REG_AUDIO_ANC(anc) + 0x754 + (cz) * 0x4c0 + (iir) * 0x20)                              /* anc[0-1] cz[0-2] iir[0-23] */

#define reg_audio_anc_cz_fir(anc, cz, fir)    REG_ADDR16(REG_AUDIO_ANC(anc) + 0xa40 + (cz) * 0x4c0 + ((fir) / 25) * 0x02 + ((fir) % 25) * 0x10) /* anc[0-1] cz[0-2] fir[0-149] */

/* AUDIO_ANC_BASE register */
#define reg_audio_anc_config(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc)) /* anc[0-1] */

enum
{
    FLD_TX_FIFO_CLR_PCLK = BIT(0),
    FLD_TX_RPTR_EN       = BIT(1),
    FLD_RE_INMODE_SEL    = BIT(2),
    FLD_RE_OUTMODE_SEL   = BIT(3),
    FLD_AHB_ANC_SEL      = BIT(4),
    FLD_ANC_SRC_EN       = BIT(5),
    FLD_ANC_SRC_RATE_SEL = BIT(6),
    FLD_ANC_SPK_SEL      = BIT(7),
};

#define reg_audio_anc_config1(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0x01) /* anc[0-1] */

enum
{
    FLD_ANC_WZ_SRST_EN           = BIT(0),
    FLD_ANC_CZ0_SRST_EN          = BIT(1),
    FLD_ANC_CZ1_SRST_EN          = BIT(2),
    FLD_ANC_CZ2_SRST_EN          = BIT(3),
    FLD_ANC_SOFT_RST_EN          = BIT(4),
    FLD_ANC_POST_DATA_CLR        = BIT(5),
    FLD_ANC_REF_GAIN_SHIFT_LATCH = BIT(6),
    FLD_ANC_WZ_GAIN_SHIFT_LATCH  = BIT(7),
};

#define reg_audio_anc_config2(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0x02) /* anc[0-1] */

enum
{
    FLD_ANC_DAC_CNT_MODE = BIT_RNG(0, 1),
    FLD_ANC_ADD_CZ1_SEL  = BIT(2),
    FLD_ANC_ADD_WZ_SEL   = BIT(3),
    FLD_ANC_MODE_SEL     = BIT_RNG(4, 5),
};

#define reg_audio_anc_config3(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0x03) /* anc[0-1] */

enum
{
    FLD_ANC_TX_FIFO_TRIG_NUM = BIT_RNG(0, 4),
};

#define reg_audio_anc_ref_gain(anc) REG_ADDR32(REG_AUDIO_ANC_BASE(anc) + 0x04) /* anc[0-1] */

enum
{
    FLD_ANC_REF_GAIN = BIT_RNG(0, 27),
};

#define reg_audio_anc_wz_gain(anc) REG_ADDR32(REG_AUDIO_ANC_BASE(anc) + 0x08) /* anc[0-1] */

enum
{
    FLD_ANC_WZ_GAIN = BIT_RNG(0, 27),
};

#define reg_audio_anc_ref_gain_shift(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0x0c) /* anc[0-1] */

enum
{
    FLD_ANC_REF_GAIN_SHIFT = BIT_RNG(0, 5),
};

#define reg_audio_anc_gain_mul_shift(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0x0d) /* anc[0-1] */

enum
{
    FLD_ANC_GAIN_MUL_SHIFT = BIT_RNG(0, 5),
};

#define reg_audio_anc_hb1_coef(anc, coef1) REG_ADDR32(REG_AUDIO_ANC_BASE(anc) + 0x10 + ((coef1) << 2)) /* anc[0-1] coef1[0-20] */

enum
{
    FLD_ANC_HB1_COEF = BIT_RNG(0, 25),
};

#define reg_audio_anc_hb2_coef(anc, coef2) REG_ADDR32(REG_AUDIO_ANC_BASE(anc) + 0x64 + ((coef2) << 2)) /* anc[0-1] coef2[0-6] */

enum
{
    FLD_ANC_HB2_COEF = BIT_RNG(0, 25),
};

#define reg_audio_anc_hb3_coef(anc, coef3) REG_ADDR32(REG_AUDIO_ANC_BASE(anc) + 0x80 + ((coef3) << 2)) /* anc[0-1] coef3[0-6] */

enum
{
    FLD_ANC_HB3_COEF = BIT_RNG(0, 25),
};

#define reg_audio_anc_fmt_sel(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0x9c) /* anc[0-1] */

enum
{
    FLD_ANC_PRE_FMT_SEL  = BIT_RNG(0, 1),
    FLD_ANC_POST_FMT_SEL = BIT_RNG(2, 3),
    FLD_ANC_PLAY_FMT_SEL = BIT_RNG(4, 5),
};

#define reg_audio_anc_fmt_sel1(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0x9d) /* anc[0-1] */

enum
{
    FLD_ANC_SRC_FMT_SEL = BIT(0),
    FLD_ANC_REF_FMT_SEL = BIT(1),
    FLD_ANC_ERR_FMT_SEL = BIT(2),
};

#define reg_audio_anc_start(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0x9e) /* anc[0-1] */

enum
{
    FLD_ANC_WZ_IIR_START  = BIT(0),
    FLD_ANC_WZ_FIR_START  = BIT(1),
    FLD_ANC_CZ0_IIR_START = BIT(2),
    FLD_ANC_CZ0_FIR_START = BIT(3),
    FLD_ANC_CZ1_IIR_START = BIT(4),
    FLD_ANC_CZ1_FIR_START = BIT(5),
    FLD_ANC_CZ2_IIR_START = BIT(6),
    FLD_ANC_CZ2_FIR_START = BIT(7),
};

#define reg_audio_anc_done(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0x9f) /* anc[0-1] */

enum
{
    FLD_ANC_WZ_IIR_DONE  = BIT(0),
    FLD_ANC_WZ_FIR_DONE  = BIT(1),
    FLD_ANC_CZ0_IIR_DONE = BIT(2),
    FLD_ANC_CZ0_FIR_DONE = BIT(3),
    FLD_ANC_CZ1_IIR_DONE = BIT(4),
    FLD_ANC_CZ1_FIR_DONE = BIT(5),
    FLD_ANC_CZ2_IIR_DONE = BIT(6),
    FLD_ANC_CZ2_FIR_DONE = BIT(7),
};

#define reg_audio_anc_droop_coef(anc, d_coef) REG_ADDR16(REG_AUDIO_ANC_BASE(anc) + 0xa0 + (((d_coef)) << 1)) /* anc[0-1] d_coef[0-4] */

enum
{
    FLD_ANC_DROOP_COEF = BIT_RNG(0, 13),
};

#define reg_audio_anc_wz_iir_taps(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xaa) /* anc[0-1] */

enum
{
    FLD_ANC_WZ_IIR_TAPS      = BIT_RNG(0, 4),
    FLD_ANC_WZ_FIR_TAPS_BIT8 = BIT(7),
};

#define reg_audio_anc_wz_fir_taps(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xab) /* anc[0-1] */

enum
{
    FLD_ANC_WZ_FIR_TAPS = BIT_RNG(0, 7),
};


#define reg_audio_anc_cz_iir_taps(anc, cz)      REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xac + cz*2) /* anc[0-1] */
#define reg_audio_anc_cz0_iir_taps(anc)         REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xac) /* anc[0-1] */
#define reg_audio_anc_cz1_iir_taps(anc)         REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xae) /* anc[0-1] */
#define reg_audio_anc_cz2_iir_taps(anc)         REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xb0) /* anc[0-1] */

enum
{
    FLD_ANC_CZ_IIR_TAPS = BIT_RNG(0, 4),
};

#define reg_audio_anc_cz_fir_taps(anc, cz)      REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xad + cz*2) /* anc[0-1] */
#define reg_audio_anc_cz0_fir_taps(anc)         REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xad) /* anc[0-1] */
#define reg_audio_anc_cz1_fir_taps(anc)         REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xaf) /* anc[0-1] */
#define reg_audio_anc_cz2_fir_taps(anc)         REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xb1) /* anc[0-1] */
#define reg_audio_anc_tx_fifo_max(anc)          REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xb2) /* anc[0-1] */

enum
{
    FLD_ANC_TX_FIFO_MAX = BIT_RNG(0, 7),

};

#define reg_audio_coef_num_max(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xb3) /* anc[0-1] */

enum
{
    FLD_ANC_COEF_NUM_MAX = BIT_RNG(0, 3),
};

#define reg_audio_anc_postdat_fifo(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xb4) /* anc[0-1] */

enum
{
    FLD_ANC_TX_FIFO_CLR_HCLK  = BIT(0),
    FLD_ANC_POSTDAT_EMPTY     = BIT(4),
    FLD_ANC_POSTDAT_FULL      = BIT(5),
    FLD_ANC_POSTDAT_UNDERRUN  = BIT(6),
    FLD_ANC_POSTDAT_UNDERFLOW = BIT(7),
};

#define reg_audio_anc_txfifo_rptr(anc) REG_ADDR16(REG_AUDIO_ANC_BASE(anc) + 0xb5) /* anc[0-1] */

enum
{
    FLD_ANC_TX_FIFO_RPTR = BIT_RNG(0, 7)
};

#define reg_audio_anc_txfifo_irq_st(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xb7) /* anc[0-1] */

enum
{
    FLD_ANC_TX_FIFO_IRQ_ST = BIT(0),
    FLD_ANC_COEF_LATCH     = BIT(1),
};

#define reg_audio_anc_txfifo_th(anc) REG_ADDR8(REG_AUDIO_ANC_BASE(anc) + 0xb8) /* anc[0-1] */

enum
{
    FLD_ANC_TX_FIFO_TH = BIT_RNG(0, 10)
};

#define reg_audio_anc_ref_dc(anc) REG_ADDR32(REG_AUDIO_ANC_BASE(anc) + 0xbc) /* anc[0-1] */

enum
{
    FLD_ANC_REF_DC = BIT_RNG(0, 23),
};

/**************************************************** CODEC control register *****************************************************************/
#define reg_audio_codec_ctrl REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE)

enum
{
    FLD_CODEC_CTRL_DAC_MST_EN  = BIT(0),
    FLD_CODEC_CTRL_ADC0_MST_EN = BIT(1),
    FLD_CODEC_CTRL_ADC1_MST_EN = BIT(2),
    FLD_CODEC_CTRL_ADC2_MST_EN = BIT(3),
};

#define reg_audio_codec_data_fmt_l REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x02)

enum
{
    FLD_CODEC_CTRL_ADC0_SL_SEL = BIT(0),
    FLD_CODEC_CTRL_ADC0_SR_SEL = BIT(1),
    FLD_CODEC_CTRL_ADC1_SL_SEL = BIT(2),
    FLD_CODEC_CTRL_ADC1_SR_SEL = BIT(3),
    FLD_CODEC_CTRL_ADC2_SL_SEL = BIT(4),
    FLD_CODEC_CTRL_ADC2_SR_SEL = BIT(5),
    FLD_CODEC_CTRL_DAC_SL_SEL  = BIT(6),
    FLD_CODEC_CTRL_DAC_SR_SEL  = BIT(7),
};

#define reg_audio_codec0_int_pcm_num REG_ADDR16(REG_AUDIO_CODEC_CTRL_BASE + 0x04)

enum
{
    FLD_CODEC0_INT_PCM_NUM = BIT_RNG(0, 12),
};

#define reg_audio_codec0_dec0_pcm_num REG_ADDR16(REG_AUDIO_CODEC_CTRL_BASE + 0x08)

enum
{
    FLD_CODEC0_DEC0_PCM_NUM = BIT_RNG(0, 12),
};

#define reg_audio_codec0_dec1_pcm_num REG_ADDR16(REG_AUDIO_CODEC_CTRL_BASE + 0x0c)

enum
{
    FLD_CODEC0_DEC1_PCM_NUM = BIT_RNG(0, 12),
};

#define reg_audio_codec_dmic_dec2_pcm_num REG_ADDR16(REG_AUDIO_CODEC_CTRL_BASE + 0x10)

enum
{
    FLD_CODEC_DMIC_DEC2_PCM_NUM = BIT_RNG(0, 12),
};

#define reg_audio_codec_dac_stimer_target      REG_ADDR32(REG_AUDIO_CODEC_CTRL_BASE + 0x14)
#define reg_audio_codec_adc0_stimer_target     REG_ADDR32(REG_AUDIO_CODEC_CTRL_BASE + 0x18)
#define reg_audio_codec_adc1_stimer_target     REG_ADDR32(REG_AUDIO_CODEC_CTRL_BASE + 0x1c)
#define reg_audio_codec_adc2_stimer_target     REG_ADDR32(REG_AUDIO_CODEC_CTRL_BASE + 0x20)
#define reg_audio_codec_adc_stimer_target(adc) REG_ADDR32(REG_AUDIO_CODEC_CTRL_BASE + 0x18 + ((adc) << 2)) /* adc[0-2] */

#define reg_audio_codec_misc_ctrl              REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x28)

enum
{
    FLD_CODEC_DAC_SCHEDULE_EN  = BIT(0),
    FLD_CODEC_ADC0_SCHEDULE_EN = BIT(1),
    FLD_CODEC_ADC1_SCHEDULE_EN = BIT(2),
    FLD_CODEC_ADC2_SCHEDULE_EN = BIT(3),
    FLD_CODEC_DAC_MODE         = BIT(4),
};

#define reg_audio_codec0_auto_bist_ctrl       REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x29)
#define reg_audio_codec1_auto_bist_ctrl       REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x2e)
#define reg_audio_codec2_auto_bist_ctrl       REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x32)
#define reg_audio_codec_auto_bist_ctrl(codec) REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x29 + (codec) * 5 - (codec == 2 ? 1 : 0)) /* codec[0-2] */

enum
{
    FLD_CODEC_AUTO_BIST_EN       = BIT(0),
    FLD_CODEC_AUTO_BIST_TRIG     = BIT(1),
    FLD_CODEC_AUTO_BIST_TRIG_END = BIT(2),
    FLD_CODEC_AUTO_BIST_CLK_END  = BIT(3),
    FLD_CODEC_AUTO_BIST_TRIG_RD  = BIT(4),
    FLD_CODEC_AUTO_BIST_CLK_RD   = BIT(5),
};

#define reg_audio_codec0_manual_bist_ctrl       REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x2a)
#define reg_audio_codec1_manual_bist_ctrl       REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x2f)
#define reg_audio_codec2_manual_bist_ctrl       REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x33)
#define reg_audio_codec_manual_bist_ctrl(codec) REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x2a + (codec) * 5 - (codec == 2 ? 1 : 0)) /* codec[0-2] */

enum
{
    FLD_CODEC_BIST_MODE        = BIT(0),
    FLD_CODEC_BIST_NRST        = BIT(1),
    FLD_CODEC_BIST_START       = BIT(2),
    FLD_CODEC_BIST_CLK_EN      = BIT(3),
    FLD_CODEC_BIST_ERR         = BIT(4),
    FLD_CODEC_BIST_FAIL        = BIT(5),
    FLD_CODEC_BIST_END         = BIT(6),
    FLD_CODEC_BIST_ERR_CNT_CLR = BIT(7),
};

#define reg_audio_codec0_bist_err_cnt       REG_ADDR16(REG_AUDIO_CODEC_CTRL_BASE + 0x2c)
#define reg_audio_codec1_bist_err_cnt       REG_ADDR16(REG_AUDIO_CODEC_CTRL_BASE + 0x30)
#define reg_audio_codec2_bist_err_cnt       REG_ADDR16(REG_AUDIO_CODEC_CTRL_BASE + 0x34)
#define reg_audio_codec_bist_err_cnt(codec) REG_ADDR16(REG_AUDIO_CODEC_CTRL_BASE + 0x2c + (codec) * 4) /* codec[0-2] */

#define reg_audio_codec_ms                  REG_ADDR8(REG_AUDIO_CODEC_CTRL_BASE + 0x36)

enum
{
    FLD_CODEC_DMIC_MS_MODE = BIT(0),
    FLD_CODEC_ADCB_MS_MODE = BIT(1),
    FLD_CODEC_ADCA_MS_MODE = BIT(2),
    FLD_CODEC_DACA_MS_MODE = BIT(3),
};

/**************************************************** CLK and IRQ control register *****************************************************************/
#define reg_audio_clk_en_0 REG_ADDR8(REG_AUDIO_CLK_IRQ_CTRL_BASE)

enum
{
    FLD_CLK_ACLK_EN   = BIT(0),
    FLD_CLK_I2S0_EN   = BIT(1),
    FLD_CLK_I2S1_EN   = BIT(2),
    FLD_CLK_I2S2_EN   = BIT(3),
    FLD_CLK_SPDIF_EN  = BIT(4),
    FLD_CLK_CODEC0_EN = BIT(5),
    FLD_CLK_CODEC1_EN = BIT(6),
    FLD_CLK_CODEC2_EN = BIT(7),
};

#define reg_audio_clk_en_1 REG_ADDR8(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x01)

enum
{
    FLD_CLK_HAC0_EN = BIT(0),
    FLD_CLK_HAC1_EN = BIT(1),
    FLD_CLK_HAC2_EN = BIT(2),
    FLD_CLK_HAC3_EN = BIT(3),
    FLD_CLK_ANC0_EN = BIT(4),
    FLD_CLK_ANC1_EN = BIT(5),
};

#define reg_audio_clk_i2s_step(i2s) REG_ADDR16(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x02 + ((i2s) << 1)) /* i2s[0-2] */

enum
{
    FLD_CLK_I2S_STEP = BIT_RNG(0, 14)
};

#define reg_audio_clk_i2s_mod(i2s) REG_ADDR16(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x08 + ((i2s) << 1)) /* i2s[0-2] */


#define reg_audio_clk_irq_status0  REG_ADDR8(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x10)

enum
{
    FLD_DMA_FIFO_IRQ0 = BIT(0),
    FLD_DMA_FIFO_IRQ1 = BIT(1),
    FLD_DMA_FIFO_IRQ2 = BIT(2),
    FLD_DMA_FIFO_IRQ3 = BIT(3),
    FLD_DMA_FIFO_IRQ4 = BIT(4),
    FLD_DMA_FIFO_IRQ5 = BIT(5),
    FLD_DMA_FIFO_IRQ6 = BIT(6),
    FLD_DMA_FIFO_IRQ7 = BIT(7),
};

#define reg_audio_clk_irq_status1 REG_ADDR8(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x11)

enum
{
    FLD_HAC_IRQ0 = BIT(0),
    FLD_HAC_IRQ1 = BIT(1),
    FLD_HAC_IRQ2 = BIT(2),
    FLD_HAC_IRQ3 = BIT(3),
};

#define reg_audio_clk_irq_status2 REG_ADDR8(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x12)

enum
{
    FLD_CODEC_IRQ     = BIT(0),
    FLD_HMST_RESP_IRQ = BIT(1),
};

#define reg_audio_clk_aclk_set REG_ADDR8(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x13)

enum
{
    FLD_CLK_ACLK_SET = BIT_RNG(0, 3),
};

#define reg_audio_clk_hmst_resp_irq_ctrl REG_ADDR8(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x14)

enum
{
    FLD_HMST_RESP_IRQ_EN  = BIT(0),
    FLD_HMST_RESP_IRQ_CLR = BIT(4),
};

#define reg_audio_clk_rst_en_l REG_ADDR8(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x18)

enum
{
    FLD_CLK_RST_I2S0_EN    = BIT(0),
    FLD_CLK_RST_I2S1_EN    = BIT(1),
    FLD_CLK_RST_I2S2_EN    = BIT(2),
    FLD_CLK_RST_ANC0_EN    = BIT(3),
    FLD_CLK_RST_ANC1_EN    = BIT(4),
    FLD_CLK_RST_CODEC0_EN  = BIT(5),
    FLD_CLK_RST_CODEC2_EN  = BIT(6),
    FLD_CLK_RST_CODECIF_EN = BIT(7),
};

#define reg_audio_clk_rst_en_h REG_ADDR8(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x19)

enum
{
    FLD_CLK_RST_SDT0_EN   = BIT(0),
    FLD_CLK_RST_SDT1_EN   = BIT(1),
    FLD_CLK_RST_SDT2_EN   = BIT(2),
    FLD_CLK_RST_SDT3_EN   = BIT(3),
    FLD_CLK_RST_HAC_EN    = BIT(4),
    FLD_CLK_RST_SPDIF_EN  = BIT(5),
    FLD_CLK_RST_MATRIX_EN = BIT(6),
    FLD_CLK_RST_CODEC1_EN = BIT(7),
};

#define reg_audio_clk_rst_en_2 REG_ADDR8(REG_AUDIO_CLK_IRQ_CTRL_BASE + 0x1a)

enum
{
    FLD_CLK_RST_ANC0_FCLK_EN   = BIT(0),
    FLD_CLK_RST_ANC1_FCLK_EN   = BIT(1),
    FLD_CLK_RST_ANC0_RSMP_N_EN = BIT(4),
    FLD_CLK_RST_ANC1_RSMP_N_EN = BIT(5),
};

/**************************************************** CODEC0 register *****************************************************************/
#define reg_audio_codec0_sr REG_ADDR8(REG_AUDIO_CODEC0_BASE)

enum
{
    FLD_CODEC0_POP_ACK = BIT(7),
};

#define reg_audio_codec0_sr_pum REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x01)

enum
{
    FLD_CODEC0_ADCB_SMUTE_IN_PROGRESS = BIT(4),
    FLD_CODEC0_ADCA_SMUTE_IN_PROGRESS = BIT(5),
};

#define reg_audio_codec0_cr_vic REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x02)

enum
{
    FLD_CODEC0_POWER_CTRL       = BIT_RNG(0, 3),
    FLD_CODEC0_DIGITAL_SB       = BIT(0),
    FLD_CODEC0_ANALOG_SB        = BIT(1),
    FLD_CODEC0_ANALOG_ADC_SLEEP = BIT(2),
    /* RVSD */
    FLD_CODEC0_AVD_1V8 = BIT(4),
};

#define reg_audio_codec0_cr_adca_ai REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x04)

enum
{
    FLD_CODEC0_ADCA_AUDIOIF = BIT_RNG(0, 1),
    FLD_CODEC0_ADCA_PAIRING = BIT(2),
    /* RVSD */
    FLD_CODEC0_ADCA_SLAVE = BIT(5),
    FLD_CODEC0_ADCA_ADWL  = BIT_RNG(6, 7),
};

#define reg_audio_codec0_cr_adca_ai_sb REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x05)

enum
{
    FLD_CODEC0_SB_AICR_ADCA12 = BIT(0),
};

#define reg_audio_codec0_cr_adca_ai_mix_l REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x06)

enum
{
    FLD_CODEC0_AIADCA1_SEL = BIT_RNG(0, 1),
    FLD_CODEC0_AIADCA2_SEL = BIT_RNG(2, 3),
};

#define reg_audio_codec0_cr_adcb_ai REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x07)

enum
{
    FLD_CODEC0_ADCB_AUDIOIF = BIT_RNG(0, 1),
    FLD_CODEC0_ADCB_PAIRING = BIT(2),
    /* RVSD */
    FLD_CODEC0_ADCB_SLAVE = BIT(5),
    FLD_CODEC0_ADCB_ADWL  = BIT_RNG(6, 7),
};

#define reg_audio_codec0_cr_adcb_ai_sb REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x08)

enum
{
    FLD_CODEC0_SB_AICR_ADCB12 = BIT(0),
};

#define reg_audio_codec0_cr_adcb_ai_mix_l REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x09)

enum
{
    FLD_CODEC0_AIADCB1_SEL = BIT_RNG(0, 1),
    FLD_CODEC0_AIADCB2_SEL = BIT_RNG(2, 3),
};

#define reg_audio_codec0_cr_adca_freq_sel REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x0a)

enum
{
    FLD_CODEC0_ADCA_FREQ        = BIT_RNG(0, 3),
    FLD_CODEC0_ADCA_FLT_CFG_SEL = BIT_RNG(4, 6),
};

#define reg_audio_codec0_cr_adcb_freq_sel REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x0b)

enum
{
    FLD_CODEC0_ADCB_FREQ        = BIT_RNG(0, 3),
    FLD_CODEC0_ADCB_FLT_CFG_SEL = BIT_RNG(4, 6),
};

#define reg_audio_codec0_cr_adca_hpf REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x12)

enum
{
    FLD_CODEC0_ADCA12_HPF_EN = BIT(0),
};

#define reg_audio_codec0_cr_adcb_hpf REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x13)

enum
{
    FLD_CODEC0_ADCB12_HPF_EN = BIT(0),
};

#define reg_audio_codec0_cr_adca_wnf REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x14)

enum
{
    FLD_CODEC0_ADCA12_WNF = BIT_RNG(0, 1),
};

#define reg_audio_codec0_cr_adcb_wnf REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x15)

enum
{
    FLD_CODEC0_ADCB12_WNF = BIT_RNG(0, 1),
};

#define reg_audio_codec0_cr_dmic_adca_sb REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x16)

enum
{
    FLD_CODEC0_SB_DMIC_ADCA1 = BIT(0),
    FLD_CODEC0_SB_DMIC_ADCA2 = BIT(1),
};

#define reg_audio_codec0_cr_dmic_adca_12_rate REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x18)

enum
{
    FLD_CODEC0_ADCA12_DMIC_RATE = BIT_RNG(0, 4),
};

#define reg_audio_codec0_cr_mic_adca_12_sel REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x19)

enum
{
    FLD_CODEC0_ADCA1_MIC_SEL       = BIT(0),
    FLD_CODEC0_ADCA1_MIC_LR_SWAP   = BIT(1),
    FLD_CODEC0_ADCA1_MIC_HARD_MUTE = BIT(2),
    FLD_CODEC0_ADCA2_MIC_SEL       = BIT(3),
    FLD_CODEC0_ADCA2_MIC_LR_SWAP   = BIT(4),
    FLD_CODEC0_ADCA2_MIC_HARD_MUTE = BIT(5),
    /* RVSD */
};

#define reg_audio_codec0_cr_dmic_adcb_sb REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x1a)

enum
{
    FLD_CODEC0_SB_DMIC_ADCB1 = BIT(0),
    FLD_CODEC0_SB_DMIC_ADCB2 = BIT(1),
};

#define reg_audio_codec0_cr_dmic_adcb_12_rate REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x1c)

enum
{
    FLD_CODEC0_ADCB12_DMIC_RATE = BIT_RNG(0, 4),
};

#define reg_audio_codec0_cr_mic_adcb_12_sel REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x1d)

enum
{
    FLD_CODEC0_ADCB1_MIC_SEL       = BIT(0),
    FLD_CODEC0_ADCB1_MIC_LR_SWAP   = BIT(1),
    FLD_CODEC0_ADCB1_MIC_HARD_MUTE = BIT(2),
    FLD_CODEC0_ADCB2_MIC_SEL       = BIT(3),
    FLD_CODEC0_ADCB2_MIC_LR_SWAP   = BIT(4),
    FLD_CODEC0_ADCB2_MIC_HARD_MUTE = BIT(5),
};

#define reg_audio_codec0_cr_adc_mic(adc) REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x1e + (adc))

enum
{
    FLD_CODEC0_CAPCOUPLED = BIT(0),
    FLD_CODEC0_SB_MICBIAS = BIT(5),
    FLD_CODEC0_MICDIFF    = BIT(6),
    FLD_CODEC0_MICBIAS1_V = BIT(7),
};

#define reg_audio_codec0_cr_adc_mica1 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x1e)

enum
{
    FLD_CODEC0_CAPCOUPLEDA1 = BIT(0),
    FLD_CODEC0_SB_MICBIASA1 = BIT(5),
    FLD_CODEC0_MICDIFFA1    = BIT(6),
    FLD_CODEC0_MICBIASA1_V  = BIT(7),
};

#define reg_audio_codec0_cr_adc_mica2 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x1f)

enum
{
    FLD_CODEC0_CAPCOUPLEDA2 = BIT(0),
    FLD_CODEC0_SB_MICBIASA2 = BIT(5),
    FLD_CODEC0_MICDIFFA2    = BIT(6),
    FLD_CODEC0_MICBIASA2_V  = BIT(7),
};

#define reg_audio_codec0_cr_adc_micb1 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x20)

enum
{
    FLD_CODEC0_CAPCOUPLEDAB1 = BIT(0),
    FLD_CODEC0_SB_MICBIASB1  = BIT(5),
    FLD_CODEC0_MICDIFFB1    = BIT(6),
    FLD_CODEC0_MICBIASB1_V   = BIT(7),
};

#define reg_audio_codec0_cr_adc_micb2 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x21)

enum
{
    FLD_CODEC0_CAPCOUPLEDAB2 = BIT(0),
    FLD_CODEC0_SB_MICBIASB2  = BIT(5),
    FLD_CODEC0_MICDIFFB2    = BIT(6),
    FLD_CODEC0_MICBIASB2_V   = BIT(7),
};

#define reg_audio_codec0_gcr_mica12 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x22)

enum
{
    FLD_CODEC0_GIMA1 = BIT_RNG(0, 3),
    FLD_CODEC0_GIMA2 = BIT_RNG(4, 7),
};

#define reg_audio_codec0_gcr_micb12 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x23)

enum
{
    FLD_CODEC0_GIMB1 = BIT_RNG(0, 3),
    FLD_CODEC0_GIMB2 = BIT_RNG(4, 7),
};

#define reg_audio_codec0_cr_adca12 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x24)

enum
{
    FLD_CODEC0_ADCA_POWER_MODE = BIT_RNG(2, 3),
    FLD_CODEC0_SB_ADCA1        = BIT(4),
    FLD_CODEC0_SB_ADCA2        = BIT(5),
};

#define reg_audio_codec0_cr_adcb12 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x25)

enum
{
    FLD_CODEC0_ADCB_POWER_MODE = BIT_RNG(2, 3),
    FLD_CODEC0_SB_ADCB1        = BIT(4),
    FLD_CODEC0_SB_ADCB2        = BIT(5),
};

#define reg_audio_codec0_cr_adc_dgain(adc) REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x26 + (adc))

enum
{
    FLD_CODEC0_GOD_ADC       = BIT_RNG(0, 6),
    FLD_CODEC0_ADC_SOFT_MUTE = BIT(7),
};

#define reg_audio_codec0_cr_adca1_dgain REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x26)

enum
{
    FLD_CODEC0_GOD_ADCA1       = BIT_RNG(0, 6),
    FLD_CODEC0_ADCA1_SOFT_MUTE = BIT(7),
};

#define reg_audio_codec0_cr_adca2_dgain REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x27)

enum
{
    FLD_CODEC0_GOD_ADCA2       = BIT_RNG(0, 6),
    FLD_CODEC0_ADCA2_SOFT_MUTE = BIT(7),
};

#define reg_audio_codec0_cr_adcb1_dgain REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x28)

enum
{
    FLD_CODEC0_GID_ADCB1       = BIT_RNG(0, 6),
    FLD_CODEC0_ADCB1_SOFT_MUTE = BIT(7),
};

#define reg_audio_codec0_cr_adcb2_dgain REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x29)

enum
{
    FLD_CODEC0_GID_ADCB2       = BIT_RNG(0, 6),
    FLD_CODEC0_ADCB2_SOFT_MUTE = BIT(7),
};

#define reg_audio_codec0_adca_12_alc_0 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x2a)

enum
{
    FLD_CODEC0_ADCA_12_SNR_OPT_EN = BIT(0),
    FLD_CODEC0_ADCA_12_ALC_EN     = BIT(1),
    FLD_CODEC0_ADCA_12_TARGET     = BIT_RNG(2, 5),
    FLD_CODEC0_ADCA_12_ALC_STEREO = BIT(6),
};

#define reg_audio_codec0_adca_12_alc_1 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x2b)

enum
{
    FLD_CODEC0_ADCA_12_HOLD   = BIT_RNG(0, 3),
    FLD_CODEC0_ADCA_12_NG_THR = BIT_RNG(4, 6),
    FLD_CODEC0_ADCA_12_NG_EN  = BIT(7),
};

#define reg_audio_codec0_adca_12_alc_2 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x2c)

enum
{
    FLD_CODEC0_ADCA_12_DCY = BIT_RNG(0, 3),
    FLD_CODEC0_ADCA_12_ATK = BIT_RNG(4, 7),
};

#define reg_audio_codec0_adca_12_alc_3 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x2d)

enum
{
    FLD_CODEC0_ADCA_12_ALC_MAX = BIT_RNG(0, 4)
};

#define reg_audio_codec0_adca_12_alc_4 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x2e)

enum
{
    FLD_CODEC0_ADCA_12_ALC_MIN = BIT_RNG(0, 4)
};

#define reg_audio_codec0_adcb_12_alc_0 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x2f)

enum
{
    FLD_CODEC0_ADCB_12_SNR_OPT_EN = BIT(0),
    FLD_CODEC0_ADCB_12_ALC_EN     = BIT(1),
    FLD_CODEC0_ADCB_12_TARGET     = BIT_RNG(2, 5),
    FLD_CODEC0_ADCB_12_ALC_STEREO = BIT(6),
};

#define reg_audio_codec0_adcb_12_alc_1 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x30)

enum
{
    FLD_CODEC0_ADCB_12_HOLD   = BIT_RNG(0, 3),
    FLD_CODEC0_ADCB_12_NG_THR = BIT_RNG(4, 6),
    FLD_CODEC0_ADCB_12_NG_EN  = BIT(7),
};

#define reg_audio_codec0_adcb_12_alc_2 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x31)

enum
{
    FLD_CODEC0_ADCB_12_DCY = BIT_RNG(0, 3),
    FLD_CODEC0_ADCB_12_ATK = BIT_RNG(4, 7),
};

#define reg_audio_codec0_adcb_12_alc_3 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x32)

enum
{
    FLD_CODEC0_ADCB_12_ALC_MAX = BIT_RNG(0, 4),
};

#define reg_audio_codec0_adcb_12_alc_4 REG_ADDR8(REG_AUDIO_CODEC0_BASE + 0x33)

enum
{
    FLD_CODEC0_ADCB_12_ALC_MIN = BIT_RNG(0, 4),
};

/**************************************************** CODEC_DAC register *****************************************************************/
#define reg_audio_codec_dac_sr REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE)

enum
{
    FLD_CODEC_DAC_IRQ_PENDING = BIT(6),
    FLD_CODEC_DAC_POP_ACK     = BIT(7),
};

#define reg_audio_codec_dac_sr_pum REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x01)

enum
{
    FLD_CODEC_DAC_SMUTE_IN_PROGRESS = BIT(7),
};

#define reg_audio_codec_dac_icr REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x02)

enum
{
    FLD_CODEC_DAC_INT_FORM = BIT_RNG(6, 7),
};

#define reg_audio_codec_dac_imr REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x03)

enum
{
    FLD_CODEC_DAC_SC_MASK = BIT(4),
};

#define reg_audio_codec_dac_ifr REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x04)

enum
{
    FLD_CODEC_DAC_SC = BIT(4),
};

#define reg_audio_codec_dac_cr_vic REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x05)

enum
{
    FLD_CODEC_DAC_DIGITAL_SB = BIT(0),
    FLD_CODEC_DAC_ANALOG_SB  = BIT(1),
    /* RVSD */
    FLD_CODEC_DAC_ANALOG_DAC_SLEEP = BIT(3),
    FLD_CODEC_DAC_AVD_1V8          = BIT(4),
};

#define reg_audio_codec_dac_cr_daca_ai REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x07)

enum
{
    FLD_CODEC_DACA_AUDIOIF     = BIT_RNG(0, 1),
    FLD_CODEC_DACA_PAIRING     = BIT(2),
    FLD_CODEC_DAC_SB_AICR_DACA = BIT(4),
    FLD_CODEC_DACA_SLAVE       = BIT(5),
};

#define reg_audio_codec_dac_cr_daca_freq_sel REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x09)

enum
{
    FLD_CODEC_DACA_FREQ        = BIT_RNG(0, 3),
    FLD_CODEC_DACA_FLT_CFG_SEL = BIT_RNG(4, 6),
};

#define reg_audio_codec_dac_cr_hpa REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x0b)

enum
{
    FLD_CODEC_DAC_SB_HPA2 = BIT(3),
    FLD_CODEC_DAC_SB_HPA1 = BIT(4),
};

#define reg_audio_codec_dac_gcr_hpa(dac) REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x0c + (dac))

enum
{
    FLD_CODEC_DAC_GOA = BIT_RNG(0, 4),
};

#define reg_audio_codec_dac_gcr_hpa1 REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x0c)

enum
{
    FLD_CODEC_DAC_GOA1 = BIT_RNG(0, 4),
};

#define reg_audio_codec_dac_gcr_hpa2 REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x0d)

enum
{
    FLD_CODEC_DAC_GOA2 = BIT_RNG(0, 4),
};

#define reg_audio_codec_dac_cr_daca12 REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x0e)

enum
{
    FLD_CODEC_DAC_SB_DACA1 = BIT(4),
    FLD_CODEC_DAC_SB_DACA2 = BIT(5),
};

#define reg_audio_codec_dac_cr_dac_dgain(dac) REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x0f + (dac))

enum
{
    FLD_CODEC_DAC_GOD_DAC       = BIT_RNG(0, 6),
    FLD_CODEC_DAC_DAC_SOFT_MUTE = BIT(7),
};

#define reg_audio_codec_dac_cr_daca1_dgain REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x0f)

enum
{
    FLD_CODEC_DAC_GOD_DACA1       = BIT_RNG(0, 6),
    FLD_CODEC_DAC_DACA1_SOFT_MUTE = BIT(7),
};

#define reg_audio_codec_dac_cr_daca2_dgain REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x10)

enum
{
    FLD_CODEC_DAC_GOD_DACA2       = BIT_RNG(0, 6),
    FLD_CODEC_DAC_DACA2_SOFT_MUTE = BIT(7),
};

#define reg_audio_codec_dac_cr_daca12_agc REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x11)

enum
{
    FLD_CODEC_DAC_DACA12_SNR_OPT_EN = BIT(6),
    FLD_CODEC_DAC_DACA12_DRC_EN     = BIT(7),
};

#define reg_audio_codec_dac_a12_sc_drc_0 REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x12)

enum
{
    FLD_CODEC_DAC_A1_THRES = BIT_RNG(0, 4),
};

#define reg_audio_codec_dac_a12_sc_drc_1 REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x13)

enum
{
    FLD_CODEC_DAC_A1_COMP = BIT_RNG(0, 2),
};

#define reg_audio_codec_dac_a12_sc_drc_2 REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x14)

enum
{
    FLD_CODEC_DAC_A2_THRES = BIT_RNG(0, 4),
};

#define reg_audio_codec_dac_a12_sc_drc_3 REG_ADDR8(REG_AUDIO_CODEC_DAC_BASE + 0x15)

enum
{
    FLD_CODEC_DAC_A2_COMP = BIT_RNG(0, 2),
};

/**************************************************** CODEC1 register *****************************************************************/
#define reg_audio_codec1_sr_pun REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x01)

enum
{
    FLD_CODEC1_ADCA_SMUTE_IN_PROGRESS = BIT(5),
};

#define reg_audio_codec1_cr_vic REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x02)

enum
{
    FLD_CODEC1_DIGITAL_SB = BIT(0),
};

#define reg_audio_codec1_cr_ck REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x03)

enum
{
    FLD_CODEC1_MCLK_FREQ_SEL = BIT_RNG(0, 1),
    FLD_CODEC1_MCLK_DIV_SEL  = BIT_RNG(6, 7),
};

#define reg_audio_codec1_cr_adca_al REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x04)

enum
{
    FLD_CODEC1_ADCA_AUDIOIF = BIT_RNG(0, 1),
    FLD_CODEC1_ADCA_PAIRING = BIT(2),
    FLD_CODEC1_ADCA_SLAVE   = BIT(5),
    FLD_CODEC1_ADCA_ADWL    = BIT_RNG(6, 7),
};

#define reg_audio_codec1_cr_adca_al_sb REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x05)

enum
{
    FLD_CODEC1_SB_AICR_ADCA12 = BIT(0),
};

#define reg_audio_codec1_cr_adca_ai_mix_l REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x06)

enum
{
    FLD_CODEC1_AIADCA1_SEL = BIT_RNG(0, 1),
    FLD_CODEC1_AIADCA2_SEL = BIT_RNG(2, 3),
};

#define reg_audio_codec1_cr_adca_freq_sel REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x07)

enum
{
    FLD_CODEC1_ADCA_FREQ            = BIT_RNG(0, 3),
    FLD_CODEC1_ADCA_FLT_CFG_SEL     = BIT_RNG(4, 6),
    FLD_CODEC1_ADCA_FLT_AUTOCFG_DIS = BIT(7),
};

#define reg_audio_codec1_cr_adaa_dmd_flt_sel REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x08)

enum
{
    FLD_CODEC1_ADCA_DMD_FLT_SEL  = BIT_RNG(0, 1),
    FLD_CODEC1_ADCA_CIC4_LOB_SEL = BIT_RNG(2, 3),
};

#define reg_audio_codec1_cr_adac_dec_flt_sel REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x09)

enum
{
    FLD_CODEC1_ADCA_DEC_FLT_SEL = BIT_RNG(0, 1),
    FLD_CODEC1_ADCA_FIR_CFG_SEL = BIT_RNG(3, 7),
};

#define reg_audio_codec1_cr_adac_dec_osr REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x0a)

enum
{
    FLD_CODEC1_ADCA_DEC_OSR_SEL = BIT_RNG(0, 3),
    FLD_CODEC1_ADCA_CIC_OSR_SEL = BIT_RNG(4, 5),
    FLD_CODEC1_ADCA_FIR_OSR_SEL = BIT_RNG(6, 7),
};

#define reg_audio_codec1_cr_adca_hpf REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x0b)

enum
{
    FLD_CODEC1_ADCA12_HPF_EN = BIT(0),
};

#define reg_audio_codec1_cr_adca_wnf REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x0c)

enum
{
    FLD_CODEC1_ADCA12_WNF = BIT_RNG(0, 1),
};

#define reg_audio_codec1_cr_dmic_adca_sb REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x0d)

enum
{
    FLD_CODEC1_SB_DMIC_ADCA1 = BIT(0),
    FLD_CODEC1_SB_DMIC_ADCA2 = BIT(1),
};

#define reg_audio_codec1_cr_dmic_adca REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x0e)

enum
{
    FLD_CODEC1_ADCA_DMIC_INTERP_EN = BIT_RNG(0, 1),
};

#define reg_audio_codec1_cr_dmic_adca_12_rate REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x0f)

enum
{
    FLD_CODEC1_ADCA12_DMIC_RATE = BIT_RNG(0, 4),
};

#define reg_audio_codec1_cr_adca1_dgain REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x11)

enum
{
    FLD_CODEC1_GID_ADCA1       = BIT_RNG(0, 6),
    FLD_CODEC1_ADCA1_SOFT_MUTE = BIT(7),
};

#define reg_audio_codec1_cr_adca2_dgain REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x12)

enum
{
    FLD_CODEC1_GID_ADCA2       = BIT_RNG(0, 6),
    FLD_CODEC1_ADCA2_SOFT_MUTE = BIT(7),
};

#define reg_audio_codec1_adca_12_alc_0 REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x13)

enum
{
    FLD_CODEC1_ADCA_12_ALC_EN     = BIT(1),
    FLD_CODEC1_ADCA_12_TARGET     = BIT_RNG(2, 5),
    FLD_CODEC1_ADCA_12_ALC_STEREO = BIT(6),
};

#define reg_audio_codec1_adca_12_alc_1 REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x14)

enum
{
    FLD_CODEC1_ADCA_12_HOLD   = BIT_RNG(0, 3),
    FLD_CODEC1_ADCA_12_NG_THR = BIT_RNG(4, 6),
    FLD_CODEC1_ADCA_12_NG_EN  = BIT(7),
};

#define reg_audio_codec1_adca_12_alc_2 REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x15)

enum
{
    FLD_CODEC1_ADCA_12_DCY = BIT_RNG(0, 3),
    FLD_CODEC1_ADCA_12_ATK = BIT_RNG(4, 7),
};

#define reg_audio_codec1_adca_12_alc_3 REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x16)

enum
{
    FLD_CODEC1_ADCA_12_ALC_MAX = BIT_RNG(0, 4)
};

#define reg_audio_codec1_adca_12_alc_4 REG_ADDR8(REG_AUDIO_CODEC1_DMIC_BASE + 0x17)

enum
{
    FLD_CODEC1_ADCA_12_ALC_MIN = BIT_RNG(0, 4)
};

/**************************************************** ASRC register *****************************************************************/
#define reg_audio_asrc_drop_coef(asrc, d_coef) REG_ADDR16(REG_AUDIO_ASRC_COFF_BASE + (asrc) * 0xb0 + ((d_coef) << 1)) /* asrc[0-3], d_coef[0-8] */

enum
{
    FLD_ASRC_DROP_COEF = BIT_RNG(0, 13),
};

#define reg_audio_asrc_drop_len_mod(asrc) REG_ADDR8(REG_AUDIO_ASRC_COFF_BASE + 0x12 + (asrc) * 0xb0) /* asrc[0-3] */

enum
{
    FLD_ASRC_DROP_LEN_MOD = BIT_RNG(0, 1),                                                                                /**< 0: 9taps, 1: 13taps, 2: 17taps*/
};

#define reg_audio_asrc_hb1_coef(asrc, coef1) REG_ADDR32(REG_AUDIO_ASRC_COFF_BASE + 0x14 + (asrc) * 0xb0 + ((coef1) << 2)) /* asrc[0-3], coef1[0-31] */

enum
{
    FLD_ASRC_HB1_COEF = BIT_RNG(0, 25),
};

#define reg_audio_asrc_hb2_coef(asrc, coef2) REG_ADDR32(REG_AUDIO_ASRC_COFF_BASE + 0x94 + (asrc) * 0xb0 + ((coef2) << 2)) /* asrc[0-3], coef2[0-6] */

enum
{
    FLD_ASRC_HB2_COEF = BIT_RNG(0, 25),
};

#define reg_audio_asrc_int_next_update(asrc)  REG_ADDR8(REG_AUDIO_ASRC_COFF_BASE + 0x2c0 + (asrc))         /* asrc[0-3] */

#define reg_audio_asrc_frac_next_update(asrc) REG_ADDR32(REG_AUDIO_ASRC_COFF_BASE + 0x2d0 + ((asrc) << 2)) /* asrc[0-3] */

enum
{
    FLD_ASRC_FRAC_NEXT_UPDATE = BIT_RNG(0, 26),
};

#define reg_audio_asrc_set_int_frac REG_ADDR8(REG_AUDIO_ASRC_COFF_BASE + 0x304)

enum
{
    FLD_ASRC_SET0_INT_FEAC = BIT(0),
    FLD_ASRC_SET1_INT_FEAC = BIT(1),
    FLD_ASRC_SET2_INT_FEAC = BIT(2),
    FLD_ASRC_SET3_INT_FEAC = BIT(3),
};

#define reg_audio_asrc_int_next_rd(asrc)  REG_ADDR8(REG_AUDIO_ASRC_COFF_BASE + 0x308 + (asrc))         /* asrc[0-3] */

#define reg_audio_asrc_frac_next_rd(asrc) REG_ADDR32(REG_AUDIO_ASRC_COFF_BASE + 0x30c + ((asrc) << 2)) /* asrc[0-3] */

enum
{
    FLD_ASRC_FRAC_NEXT_RD = BIT_RNG(0, 26),
};

/**************************************************** AUDIO DMA/FIFO register *****************************************************************/
#define reg_audio_dma_tx_fifo_trig_num(fifo) REG_ADDR8(REG_AUDIO_DMA_BASE + (fifo)) /* fifo[0-3] */

enum
{
    FLD_TX_FIFO_TRIG_NUM = BIT_RNG(0, 4),
};

#define reg_audio_dma_rx_fifo_trig_num(fifo) REG_ADDR8(REG_AUDIO_DMA_BASE + 0x04 + (fifo)) /* fifo[0-3] */

enum
{
    FLD_RX_FIFO_TRIG_NUM = BIT_RNG(0, 4),
};

#define reg_audio_dma_fifo_clr REG_ADDR8(REG_AUDIO_DMA_BASE + 0x08)

enum
{
    FLD_TX_FIFO0_CLR = BIT(0),
    FLD_TX_FIFO1_CLR = BIT(1),
    FLD_TX_FIFO2_CLR = BIT(2),
    FLD_TX_FIFO3_CLR = BIT(3),
    FLD_RX_FIFO0_CLR = BIT(4),
    FLD_RX_FIFO1_CLR = BIT(5),
    FLD_RX_FIFO2_CLR = BIT(6),
    FLD_RX_FIFO3_CLR = BIT(7),
};

#define reg_audio_dma_tx_fifo_num(fifo) REG_ADDR8(REG_AUDIO_DMA_BASE + 0x09 + (fifo)) /* fifo[0-3] */

enum
{
    FLD_TX_FIFO_NUM = BIT_RNG(0, 4),
};

#define reg_audio_dma_rx_fifo_num(fifo) REG_ADDR8(REG_AUDIO_DMA_BASE + 0x0d + (fifo)) /* fifo[0-3] */

enum
{
    FLD_RX_FIFO_NUM = BIT_RNG(0, 4),
};

#define reg_audio_dma_ptr_en REG_ADDR8(REG_AUDIO_DMA_BASE + 0x11)

enum
{
    FLD_TX_FIFO0_RPTR_EN = BIT(0),
    FLD_TX_FIFO1_RPTR_EN = BIT(1),
    FLD_TX_FIFO2_RPTR_EN = BIT(2),
    FLD_TX_FIFO3_RPTR_EN = BIT(3),
    FLD_RX_FIFO0_WPTR_EN = BIT(4),
    FLD_RX_FIFO1_WPTR_EN = BIT(5),
    FLD_RX_FIFO2_WPTR_EN = BIT(6),
    FLD_RX_FIFO3_WPTR_EN = BIT(7),
};

#define reg_audio_dma_irq_st REG_ADDR8(REG_AUDIO_DMA_BASE + 0x12)

enum
{
    FLD_TX_FIFO0_IRQ_ST = BIT(0), /**< W1C */
    FLD_TX_FIFO1_IRQ_ST = BIT(1), /**< W1C */
    FLD_TX_FIFO2_IRQ_ST = BIT(2), /**< W1C */
    FLD_TX_FIFO3_IRQ_ST = BIT(3), /**< W1C */
    FLD_RX_FIFO0_IRQ_ST = BIT(4), /**< W1C */
    FLD_RX_FIFO1_IRQ_ST = BIT(5), /**< W1C */
    FLD_RX_FIFO2_IRQ_ST = BIT(6), /**< W1C */
    FLD_RX_FIFO3_IRQ_ST = BIT(7), /**< W1C */
};

#define reg_audio_dma_irq_en REG_ADDR8(REG_AUDIO_DMA_BASE + 0x13)

enum
{
    FLD_TX_FIFO0_IRQ_EM = BIT(0),
    FLD_TX_FIFO1_IRQ_EM = BIT(1),
    FLD_TX_FIFO2_IRQ_EM = BIT(2),
    FLD_TX_FIFO3_IRQ_EM = BIT(3),
    FLD_RX_FIFO0_IRQ_EM = BIT(4),
    FLD_RX_FIFO1_IRQ_EM = BIT(5),
    FLD_RX_FIFO2_IRQ_EM = BIT(6),
    FLD_RX_FIFO3_IRQ_EM = BIT(7),
};

#define reg_audio_dma_tx_rptr(fifo)  REG_ADDR16(REG_AUDIO_DMA_BASE + 0x14 + (fifo) * 0x0c) /* fifo[0-3] */
#define reg_audio_dma_tx_max(fifo)   REG_ADDR16(REG_AUDIO_DMA_BASE + 0x16 + (fifo) * 0x0c) /* fifo[0-3] */
#define reg_audio_dma_tx_th(fifo)    REG_ADDR16(REG_AUDIO_DMA_BASE + 0x18 + (fifo) * 0x0c) /* fifo[0-3] */

#define reg_audio_dma_rx_wptr(fifo)  REG_ADDR16(REG_AUDIO_DMA_BASE + 0x1a + (fifo) * 0x0c) /* fifo[0-3] */
#define reg_audio_dma_rx_max(fifo)   REG_ADDR16(REG_AUDIO_DMA_BASE + 0x1c + (fifo) * 0x0c) /* fifo[0-3] */
#define reg_audio_dma_rx_th(fifo)    REG_ADDR16(REG_AUDIO_DMA_BASE + 0x1e + (fifo) * 0x0c) /* fifo[0-3] */

#define reg_audio_dma_txfifo_dr_mode REG_ADDR8(REG_AUDIO_DMA_BASE + 0x44)

enum
{
    FLD_TX_FIFO0_DR_MODE = BIT(0),
    FLD_TX_FIFO1_DR_MODE = BIT(1),
    FLD_TX_FIFO2_DR_MODE = BIT(2),
    FLD_TX_FIFO3_DR_MODE = BIT(3),
};

#define reg_audio_dma_fifo_status REG_ADDR8(REG_AUDIO_DMA_BASE + 0x45)

enum
{
    FLD_TX_FIFO0_UNDERRUN = BIT(0),
    FLD_TX_FIFO1_UNDERRUN = BIT(1),
    FLD_TX_FIFO2_UNDERRUN = BIT(2),
    FLD_TX_FIFO3_UNDERRUN = BIT(3),
    FLD_RX_FIFO0_OVERRUN  = BIT(4),
    FLD_RX_FIFO1_OVERRUN  = BIT(5),
    FLD_RX_FIFO2_OVERRUN  = BIT(6),
    FLD_RX_FIFO3_OVERRUN  = BIT(7),
};
#endif
