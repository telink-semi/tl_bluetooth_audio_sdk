/********************************************************************************************************
 * @file    audio.c
 *
 * @brief   This is the source file for tl751x
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
#include "audio.h"
#include "lib/include/pm/pm.h"
#include "lib/include/stimer.h"
/**
 * @brief Audio rx fifo channel.
 * 
 */
static unsigned char audio_rx_fifo_chn;

/**
 * @brief Audio rx dma channel.
 * 
 */
static unsigned char audio_rx_dma_chn;

/**
 * @brief Audio tx fifo channel.
 * 
 */
static unsigned char audio_tx_fifo_chn;

/**
 * @brief Audio tx dma channel.
 * 
 */
static unsigned char audio_tx_dma_chn;

/**
 * @brief Audio tx dma list config table.
 * 
 */
static dma_chain_config_t g_audio_tx_dma_list_cfg[4];

/**
 * @brief Audio rx dma list config table.
 * 
 */
static dma_chain_config_t g_audio_rx_dma_list_cfg[4];

/**
 * @brief Audio i2s data/lr_clk invert config table.
 * 
 */
static i2s_invert_config_t audio_i2s_invert_config[3] = {
    {
     .i2s_lr_clk_invert_select = I2S_LR_CLK_INVERT_DIS,
     .i2s_data_invert_select   = I2S_DATA_INVERT_DIS,
     },
    {
     .i2s_lr_clk_invert_select = I2S_LR_CLK_INVERT_DIS,
     .i2s_data_invert_select   = I2S_DATA_INVERT_DIS,
     },
    {
     .i2s_lr_clk_invert_select = I2S_LR_CLK_INVERT_DIS,
     .i2s_data_invert_select   = I2S_DATA_INVERT_DIS,
     },
};

/**
 * @brief Audio rx dma config table.
 * 
 */
dma_config_t audio_dma_rx_config[4] = {
    {
     .dst_req_sel    = 0,
     .src_req_sel    = DMA_REQ_AUDIO0_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,
     .srcwidth       = DMA_CTR_WORD_WIDTH,
     .src_burst_size = DMA_BURST_1_WORD,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0,
     },
    {
     .dst_req_sel    = 0,
     .src_req_sel    = DMA_REQ_AUDIO1_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,
     .srcwidth       = DMA_CTR_WORD_WIDTH,
     .src_burst_size = DMA_BURST_1_WORD,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0,
     },
    {
     .dst_req_sel    = 0,
     .src_req_sel    = DMA_REQ_AUDIO2_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,
     .srcwidth       = DMA_CTR_WORD_WIDTH,
     .src_burst_size = DMA_BURST_1_WORD,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0,
     },
    {
     .dst_req_sel    = 0,
     .src_req_sel    = DMA_REQ_AUDIO3_RX,
     .dst_addr_ctrl  = DMA_ADDR_INCREMENT,
     .src_addr_ctrl  = DMA_ADDR_FIX,
     .dstmode        = DMA_NORMAL_MODE,
     .srcmode        = DMA_HANDSHAKE_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,
     .srcwidth       = DMA_CTR_WORD_WIDTH,
     .src_burst_size = DMA_BURST_1_WORD,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0,
     },
};

/**
 * @brief Audio tx dma config table.
 * 
 */
dma_config_t audio_dma_tx_config[4] = {
    {
     .dst_req_sel    = DMA_REQ_AUDIO0_TX,
     .src_req_sel    = 0,
     .dst_addr_ctrl  = DMA_ADDR_FIX,
     .src_addr_ctrl  = DMA_ADDR_INCREMENT,
     .dstmode        = DMA_HANDSHAKE_MODE,
     .srcmode        = DMA_NORMAL_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,
     .srcwidth       = DMA_CTR_WORD_WIDTH,
     .src_burst_size = DMA_BURST_1_WORD,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0,
     },
    {
     .dst_req_sel    = DMA_REQ_AUDIO1_TX,
     .src_req_sel    = 0,
     .dst_addr_ctrl  = DMA_ADDR_FIX,
     .src_addr_ctrl  = DMA_ADDR_INCREMENT,
     .dstmode        = DMA_HANDSHAKE_MODE,
     .srcmode        = DMA_NORMAL_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,
     .srcwidth       = DMA_CTR_WORD_WIDTH,
     .src_burst_size = DMA_BURST_1_WORD,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0,
     },
    {
     .dst_req_sel    = DMA_REQ_AUDIO2_TX,
     .src_req_sel    = 0,
     .dst_addr_ctrl  = DMA_ADDR_FIX,
     .src_addr_ctrl  = DMA_ADDR_INCREMENT,
     .dstmode        = DMA_HANDSHAKE_MODE,
     .srcmode        = DMA_NORMAL_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,
     .srcwidth       = DMA_CTR_WORD_WIDTH,
     .src_burst_size = DMA_BURST_1_WORD,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0,
     },
    {
     .dst_req_sel    = DMA_REQ_AUDIO3_TX,
     .src_req_sel    = 0,
     .dst_addr_ctrl  = DMA_ADDR_FIX,
     .src_addr_ctrl  = DMA_ADDR_INCREMENT,
     .dstmode        = DMA_HANDSHAKE_MODE,
     .srcmode        = DMA_NORMAL_MODE,
     .dstwidth       = DMA_CTR_WORD_WIDTH,
     .srcwidth       = DMA_CTR_WORD_WIDTH,
     .src_burst_size = DMA_BURST_1_WORD,
     .read_num_en    = 0,
     .priority       = 0,
     .write_num_en   = 0,
     .auto_en        = 0,
     },
};

/**********************************************************************************************************************
 *                                                Audio power/clock interface                                               *
 *********************************************************************************************************************/
/*!
 * @name Audio clock interface.
 * @{
 */

/**
 * @brief      This function serves to initialize audio.
 * @param[in]  audio_pll - audio pll clock select.
 * @return     none
 * @note       - When using the audio module, this interface must be configured first, otherwise the following interfaces will not take effect.
 *             - When the sampling rate is 44.1KHz, audio_pll needs to be set to AUDIO_PLL_CLK_33P8688M or AUDIO_PLL_CLK_169P344M.
 */
void audio_init(pll_audio_clk_e  clk)
{
    unsigned char aclk_div = 0;

    pm_set_dig_module_power_switch(FLD_PD_AUDIO_EN, PM_POWER_DOWN);
    pm_set_dig_module_power_switch(FLD_PD_AUDIO_EN, PM_POWER_UP);

    BM_SET(reg_rst2, FLD_RST2_AUDIO);
    BM_SET(reg_clk_en2, FLD_CLK2_AUDIO_EN);

    switch (clk) {
    case PLL_AUDIO_CLK_184P32M:  /*audio pll = 184.32Hz/5  = 36.864MHz*/
    case PLL_AUDIO_CLK_169P344M: /*audio pll = 169.344MHz/5= 33.8688MHz*/
        aclk_div = 5;
        break;
    case PLL_AUDIO_CLK_147P456M: /* audio pll = 147.456MHz/4=36.864MHz */
        aclk_div = 4;
        break;
    case PLL_AUDIO_CLK_36P864M:  /* audio pll = 36.864MHz */
        aclk_div = 1;
        break;
    case PLL_AUDIO_CLK_33P8688M: /* audio pll = 33.8688MHz */
        aclk_div = 1;
        break;
    default:
        aclk_div = 1;
        break;
    }

    reg_audio_clk_aclk_set = (reg_audio_clk_aclk_set & (~FLD_CLK_ACLK_SET)) | (aclk_div & FLD_CLK_ACLK_SET);
    BM_SET(reg_audio_clk_en_0, FLD_CLK_ACLK_EN);
}

/**
 * @}
 */
/**********************************************************************************************************************
 *                                                Audio anc interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio anc interface
 * @{
 */

/**
 * @brief      This function serves to set dac control resample fs.
 *
 * @param[in]  anc_chn - anc channel.
 * @param[in]  in_fs   - resample input fs.
 * @param[in]  out_fs  - resample output fs.
 * @note
 *             - when resample fs rely on dac, this function must be called
 */
void audio_dac_ctl_anc_resample_fs(audio_anc_chn_e anc_chn, audio_anc_resample_in_fs_e in_fs, audio_anc_resample_out_fs_e out_fs)
{
    char reg_data = 0;
    /* find dac cnt mode register data: 0: 48k->768k; 1: 96k->768k or 48k->384k; 2: 96k->384k. */
    if ((ANC_RESAMPLE_IN_FS_48K == in_fs) && (ANC_RESAMPLE_OUT_FS_768K == out_fs)) {
        reg_data = 0;
    } else if ((ANC_RESAMPLE_IN_FS_96K == in_fs) && (ANC_RESAMPLE_OUT_FS_384K == out_fs)) {
        reg_data = 2;
    } else {
        reg_data = 1;
    }

    reg_audio_anc_config2(anc_chn) = (reg_audio_anc_config2(anc_chn) & (~FLD_ANC_DAC_CNT_MODE)) |
                                     MASK_VAL(FLD_ANC_DAC_CNT_MODE, reg_data); /* set dac cnt mode to control audio pcm rate. */
}

/**
 * @brief      This function serves to set anc count mode.
 * 
 * @param[in]  anc_chn - anc channel.
 * @param[in]  in_fs   - anc input fs.
 * @param[in]  out_fs  - anc output fs.
 * @return     none
 */
void audio_anc_set_dac_cnt_mode(audio_anc_chn_e anc_chn, audio_anc_resample_in_fs_e in_fs, audio_anc_resample_out_fs_e out_fs)
{
    volatile unsigned char dac_cnt_mode = 0;
    if (in_fs == ANC_RESAMPLE_IN_FS_48K) {
        if (out_fs == ANC_RESAMPLE_OUT_FS_768K) {
            dac_cnt_mode = ANC_48K_IN_768K_OUT;
        } else {
            dac_cnt_mode = ANC_48K_IN_384K_OUT;
        }
    } else {
        if (out_fs == ANC_RESAMPLE_OUT_FS_768K) {
            dac_cnt_mode = ANC_96K_IN_768K_OUT;
        } else {
            dac_cnt_mode = ANC_96K_IN_384K_OUT;
        }
    }
    reg_audio_anc_config(anc_chn) = (reg_audio_anc_config(anc_chn) & (~(FLD_RE_INMODE_SEL | FLD_RE_OUTMODE_SEL))) |
                                    (MASK_VAL(FLD_RE_INMODE_SEL, in_fs) | MASK_VAL(FLD_RE_OUTMODE_SEL, out_fs));

    reg_audio_anc_config2(anc_chn) = (reg_audio_anc_config2(anc_chn) & (~FLD_ANC_DAC_CNT_MODE)) | MASK_VAL(FLD_ANC_DAC_CNT_MODE, dac_cnt_mode);
}

/**
 * @brief      This function serves to set the resample frequency of anc.
 *
 * @param[in]  anc_chn     - anc channel.
 * @param[in]  fs_decision - who decides the resample frequency of anc.
 * @param[in]  in_fs       - resample input fs.
 * @param[in]  out_fs      - resample output fs.
 */
void audio_anc_set_resample_in_out_fs(audio_anc_chn_e anc_chn, audio_anc_resample_fs_decision_e fs_decision, audio_anc_resample_in_fs_e in_fs, audio_anc_resample_out_fs_e out_fs)
{
    if (fs_decision == ANC_RESAMPLE_DAC_DECISION_FS) {
        BM_SET(reg_audio_anc_config(anc_chn), FLD_ANC_SRC_EN);
        BM_SET(reg_audio_anc_config(anc_chn), FLD_ANC_SRC_RATE_SEL);
        audio_dac_ctl_anc_resample_fs(anc_chn, in_fs, out_fs);
    } else {
        BM_CLR(reg_audio_anc_config(anc_chn), FLD_ANC_SRC_RATE_SEL);
    }

    audio_anc_set_resample_in_fs(anc_chn, in_fs);
    audio_anc_set_resample_out_fs(anc_chn, out_fs);
}

/**
 * @brief      This function servers to update anc wz biquad iir filter coefficients.
 * 
 * @param[in]  anc_chn - anc channel.
 * @param[in]  data    - wz biquad iir filter data.
 * @param[in]  taps    - wz biquad iir filter taps.
 * @return     none
 */
void audio_anc_update_wz_iir_coef(audio_anc_chn_e anc_chn, signed int data[24][5], unsigned short taps)
{
    reg_audio_anc_start(anc_chn) |= FLD_ANC_WZ_IIR_START;
    for (unsigned char i = 0; i < taps; i++)
    {
        reg_audio_anc_wz_iir_b0(anc_chn, i) = data[i][0];
        reg_audio_anc_wz_iir_b1(anc_chn, i) = data[i][1];
        reg_audio_anc_wz_iir_b2(anc_chn, i) = data[i][2];
        reg_audio_anc_wz_iir_a1(anc_chn, i) = data[i][3];
        reg_audio_anc_wz_iir_a2(anc_chn, i) = data[i][4];
    }
    reg_audio_anc_done(anc_chn) |= FLD_ANC_WZ_IIR_DONE;
}

/**
 * @brief      This function servers to set anc wz fir filter coefficients.
 *
 * @param[in]  anc_chn  - anc channel.
 * @param[in]  data     - wz fir filter data address.
 * @param[in]  data_len - wz fir data len max length is 390(wz fir max is 390taps).
 * @note
 *             - bypass coefficients: wz[0] = 0x4000, the rest of the parameters are all set to 0
 * @return     none
 */
void audio_anc_update_wz_fir_coef(audio_anc_chn_e anc_chn, signed short *data, unsigned short data_len)
{
    reg_audio_anc_start(anc_chn) |= FLD_ANC_WZ_FIR_START;
    for (unsigned int i = 0; i < data_len / 6; i++) {
        reg_audio_anc_wz_fir(anc_chn, i)          = data[i];          //h(i+0)
        reg_audio_anc_wz_fir(anc_chn, i + 65)     = data[i + 65];     //h(i+65)
        reg_audio_anc_wz_fir(anc_chn, i + 65 * 2) = data[i + 65 * 2]; //h(i+130)
        reg_audio_anc_wz_fir(anc_chn, i + 65 * 3) = data[i + 65 * 3]; //h(i+195)
        reg_audio_anc_wz_fir(anc_chn, i + 65 * 4) = data[i + 65 * 4]; //h(i+260)
        reg_audio_anc_wz_fir(anc_chn, i + 65 * 5) = data[i + 65 * 5]; //h(i+325)
    }
    reg_audio_anc_done(anc_chn) |= FLD_ANC_WZ_FIR_DONE;
}

/**
 * @brief      This function servers to update anc cz biquad iir filter coefficients.
 * 
 * @param[in]  anc_chn - anc channel.
 * @param[in]  cz      - cz select.
 * @param[in]  czbiq   - cz biquad iir filter data address, [b0, b1, b2, a1, a2].
 * @param[in]  taps    - cz biquad taps.
 * @return     none
 */
void audio_anc_update_cz_iir_coef(audio_anc_chn_e anc_chn, unsigned char cz, signed int czbiq[24][5],unsigned char taps)
{
    reg_audio_anc_start(anc_chn) |= BIT(cz * 2 + 2);//cz_iir_start
    for (unsigned char i = 0; i < taps; i++)
    {
        reg_audio_anc_cz_iir_b0(anc_chn,cz,i)= czbiq[i][0];//IIRx_b0(x- 0~23)
        reg_audio_anc_cz_iir_b1(anc_chn,cz,i)= czbiq[i][1];//IIRx_b1(x- 0~23)
        reg_audio_anc_cz_iir_b1(anc_chn,cz,i)= czbiq[i][2];//IIRx_b2(x- 0~23)
        reg_audio_anc_cz_iir_a1(anc_chn,cz,i)= czbiq[i][3];//IIRx_a1(x- 0~23)
        reg_audio_anc_cz_iir_a2(anc_chn,cz,i)= czbiq[i][4];//IIRx_b2(x- 0~23)
    }
    reg_audio_anc_done(anc_chn)|= BIT(cz * 2 + 2);//cz_iir_end
}

/**
 * @brief      This function servers to update anc cz fir filter coefficients.
 * 
 * @param[in]  anc_chn  - anc channel.
 * @param[in]  data     - cz fir filter data address.
 * @param[in]  data_len - cz fir data len max length is 150(cz fir max step is 150taps).
 * @return     none
 */
void audio_anc_update_cz_fir_coef(audio_anc_chn_e anc_chn, unsigned char cz, signed short *data, unsigned short data_len)
{
    reg_audio_anc_start(anc_chn) |= BIT(cz * 2 + 3);//cz_fir_start
    for (unsigned int i=0;i<data_len/6;i++)
    {
        reg_audio_anc_cz_fir(anc_chn,cz,i)     = data[i];//h(i+0)
        reg_audio_anc_cz_fir(anc_chn,cz,i+25)  = data[i+25];//h(i+25)
        reg_audio_anc_cz_fir(anc_chn,cz,i+25*2)= data[i+25*2];//h(i+50)
        reg_audio_anc_cz_fir(anc_chn,cz,i+25*3)= data[i+25*3];//h(i+75)
        reg_audio_anc_cz_fir(anc_chn,cz,i+25*4)= data[i+25*4];//h(i+100)
        reg_audio_anc_cz_fir(anc_chn,cz,i+25*5)= data[i+25*5];//h(i+125)
    }
    reg_audio_anc_done(anc_chn)|= BIT(cz * 2 + 3);//cz_fir_end
}

/**
 * @brief      This function servers to update anc hb1 coefficients.
 * @param[in]  anc_chn - anc channel.
 * @param[in]  data    - coefficient data address.
 * @note
 *             - anc hb1 coefficient length is 21 word, bit[0-25] valid.
 */
void audio_anc_update_hb1_coef(audio_anc_chn_e anc_chn, signed int *data)
{
    for (unsigned char i = 0; i < 21; i++)
    {
        reg_audio_anc_hb1_coef(anc_chn, i) = data[i] & FLD_ANC_HB1_COEF;
    }
}

/**
 * @brief      This function servers to update anc hb2 coefficients.
 * @param[in]  anc_chn - anc channel.
 * @param[in]  data    - coefficient data address.
 * @note
 *             - anc hb2 coefficient length is 7 word, bit[0-25] valid.
 */
void audio_anc_update_hb2_coef(audio_anc_chn_e anc_chn, signed int *data)
{
    for (unsigned char i = 0; i < 7; i++)
    {
        reg_audio_anc_hb2_coef(anc_chn, i) = data[i] & FLD_ANC_HB2_COEF;
    }
}

/**
 * @brief      This function servers to update anc hb3 coefficients.
 * @param[in]  anc_chn - anc channel.
 * @param[in]  data    - coefficient data address.
 * @note
 *             - anc hb3 coefficient length is 7 word, bit[0-25] valid.
 */
void audio_anc_update_hb3_coef(audio_anc_chn_e anc_chn, signed int *data)
{
    for (unsigned char i = 0; i < 7; i++)
    {
        reg_audio_anc_hb3_coef(anc_chn, i) = data[i] & FLD_ANC_HB3_COEF;
    }
}

/**
 * @brief      This function servers to update anc droop coefficients.
 * @param[in]  anc_chn - anc channel.
 * @param[in]  data    - coefficient data address.
 * @note
 *             - anc droop coefficient length is 5 word, bit[0-13] valid.
 */
void audio_anc_update_droop_coef(audio_anc_chn_e anc_chn, signed short *data)
{
    for (unsigned char i = 0; i < 5; i++)
    {
        reg_audio_anc_droop_coef(anc_chn, i) = data[i] & FLD_ANC_DROOP_COEF;
    }
}

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio asrc interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio asrc interface
 * @{
 */

/**
 * @brief      This function serves to update asrc droop coefficients.
 * 
 * @param[in]  asrc_chn  - asrc channel select.
 * @param[in]  d_coef    - asrc droop coefficients data address.
 * @param[in]  data_len  - asrc droop coefficients data length.
 * @return     none
 * @note
 *             - droop coef max length is 9.
 */
void audio_asrc_update_droop_coef(audio_asrc_chn_e asrc_chn, signed short *d_coef, unsigned char data_len)
{
    for (unsigned char i = 0; i < data_len; i++) {
        reg_audio_asrc_drop_coef(asrc_chn, i) = d_coef[i] & FLD_ASRC_DROP_COEF;
    }
}

/**
 * @brief      This function serves to update asrc half_band1 coefficients.
 * 
 * @param[in]  asrc_chn - asrc channel.
 * @param[in]  hb1_coef - asrc half_band1 coefficients data address.
 * @return     none
 * @note
 *             - asrc half_band1 coefficient length is 32 word, bit[0-25] valid.
 */
void audio_asrc_update_hb1_coef(audio_asrc_chn_e asrc_chn, signed int *hb1_coef)
{
    for (unsigned char i = 0; i < 32; i++) {
        reg_audio_asrc_hb1_coef(asrc_chn, i) = hb1_coef[i] & FLD_ASRC_HB1_COEF;
    }
}

/**
 * @brief      This function serves to update asrc half_band2 coefficients.
 * 
 * @param[in]  asrc_chn - asrc channel.
 * @param[in]  hb2_coef - asrc half_band2 coefficients data address. 
 * @return     none
 * @note
 *             - asrc half_band2 coefficient length is 7 word, bit[0-25] valid.
 */
void audio_asrc_update_hb2_coef(audio_asrc_chn_e asrc_chn, signed int *hb2_coef)
{
    for (unsigned char i = 0; i < 7; i++) {
        reg_audio_asrc_hb2_coef(asrc_chn, i) = hb2_coef[i] & FLD_ASRC_HB1_COEF;
    }
}

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio codec interface                                               *
 *********************************************************************************************************************/
/*!
 * @name Audio codec interface
 * @{
 */

/**
 * @brief      This function serves to power on codec0.
 * @param[in]  power_mode - codec0 power mode selection.
 * @param[in]  volt       - codec0 analog voltage selection.
 * @return     none
 */
void audio_codec0_power_on(audio_codec0_power_e power_mode, audio_codec0_volt_supply_e volt)
{
    switch (power_mode) {
    case AUDIO_CODEC0_DIGITAL_ONLY: /* only use codec0 dmic in. */
    case AUDIO_CODEC0_ADC_ONLY:     /* only use codec0 adc. */
        BM_SET(reg_audio_clk_en_0, FLD_CLK_ACLK_EN | FLD_CLK_CODEC0_EN);
        delay_us(1);                /* wait codec0 clock stable. */
        reg_audio_codec0_cr_vic = (reg_audio_codec0_cr_vic & (~(FLD_CODEC0_POWER_CTRL | FLD_CODEC0_AVD_1V8))) |
                                  MASK_VAL(FLD_CODEC0_POWER_CTRL, power_mode, FLD_CODEC0_AVD_1V8, volt);
        break;
    case AUDIO_CODEC0_DAC_ONLY: /* only use codec_dac. */
        BM_SET(reg_audio_clk_en_0, FLD_CLK_ACLK_EN | FLD_CLK_CODEC1_EN);
        delay_us(1);            /* wait codec0 clock stable. */
        reg_audio_codec_dac_cr_vic = (reg_audio_codec_dac_cr_vic & (~(FLD_CODEC0_POWER_CTRL | FLD_CODEC_DAC_AVD_1V8))) |
                                     MASK_VAL(FLD_CODEC0_POWER_CTRL, power_mode, FLD_CODEC_DAC_AVD_1V8, volt);
        break;
    case AUDIO_CODEC0_ADC_AND_DAC: /* use both codec0 adc and dac. */
        BM_SET(reg_audio_clk_en_0, FLD_CLK_ACLK_EN | FLD_CLK_CODEC0_EN | FLD_CLK_CODEC1_EN);
        delay_us(1);               /* wait codec0 clock stable. */
        reg_audio_codec0_cr_vic = (reg_audio_codec0_cr_vic & (~(FLD_CODEC0_POWER_CTRL | FLD_CODEC0_AVD_1V8))) |
                                  MASK_VAL(FLD_CODEC0_POWER_CTRL, 8, FLD_CODEC0_AVD_1V8, volt);
        reg_audio_codec_dac_cr_vic = (reg_audio_codec_dac_cr_vic & (~(FLD_CODEC0_POWER_CTRL | FLD_CODEC_DAC_AVD_1V8))) |
                                     MASK_VAL(FLD_CODEC0_POWER_CTRL, 4, FLD_CODEC_DAC_AVD_1V8, volt);
        break;
    case AUDIO_CODEC0_DIGITAL_AND_DAC: /* use both codec0 adc digital part and dac. */
        BM_SET(reg_audio_clk_en_0, FLD_CLK_ACLK_EN | FLD_CLK_CODEC0_EN | FLD_CLK_CODEC1_EN);
        delay_us(1);                   /* wait codec0 clock stable. */
        reg_audio_codec0_cr_vic = (reg_audio_codec0_cr_vic & (~(FLD_CODEC0_POWER_CTRL | FLD_CODEC0_AVD_1V8))) |
                                  MASK_VAL(FLD_CODEC0_POWER_CTRL, 0xe, FLD_CODEC0_AVD_1V8, volt);
        reg_audio_codec_dac_cr_vic = (reg_audio_codec_dac_cr_vic & (~(FLD_CODEC0_POWER_CTRL | FLD_CODEC_DAC_AVD_1V8))) |
                                     MASK_VAL(FLD_CODEC0_POWER_CTRL, 4, FLD_CODEC_DAC_AVD_1V8, volt);
        break;
    default:
        break;
    }

    switch (power_mode) {
    case AUDIO_CODEC0_ADC_ONLY:                             /* only use codec0 adc. */
        delay_ms(130);                                      /* 130ms codec power-up timing requirements. */
        while (!(reg_audio_codec0_sr & FLD_CODEC0_POP_ACK)) /* wait codec0 can be configured */
        {
            delay_us(1);
        }
        audio_codec0_set_micbias_state(AUDIO_AMIC_ADC_A1_A2, AUDIO_AMIC_BIAS_HIGH_IMPEDANCE);
        audio_codec0_set_micbias_state(AUDIO_AMIC_ADC_B1_B2, AUDIO_AMIC_BIAS_HIGH_IMPEDANCE);
        break;
    case AUDIO_CODEC0_DAC_ONLY:                                   /* only use codec_dac. */
    case AUDIO_CODEC0_DIGITAL_AND_DAC:                            /* use both codec0 adc digital part and dac. */
        delay_ms(130);                                            /* 130ms codec power-up timing requirements. */
        while (!(reg_audio_codec_dac_sr & FLD_CODEC_DAC_POP_ACK)) /* wait codec0 can be configured */
        {
            delay_us(1);
        }
        break;
    case AUDIO_CODEC0_ADC_AND_DAC:                                                                               /* use both codec0 adc and dac. */
        delay_ms(130);                                                                                           /* 130ms codec power-up timing requirements. */
        while (!(reg_audio_codec0_sr & FLD_CODEC0_POP_ACK) || !(reg_audio_codec_dac_sr & FLD_CODEC_DAC_POP_ACK)) /* wait codec0 can be configured */
        {
            delay_us(1);
        }
        audio_codec0_set_micbias_state(AUDIO_AMIC_ADC_A1_A2, AUDIO_AMIC_BIAS_HIGH_IMPEDANCE);
        audio_codec0_set_micbias_state(AUDIO_AMIC_ADC_B1_B2, AUDIO_AMIC_BIAS_HIGH_IMPEDANCE);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to power on codec1.
 * @param[in]  power_mode - codec1 power mode selection.
 * @return     none
 */
void audio_codec1_power_on(audio_codec1_power_e power_mode)
{
    BM_SET(reg_audio_clk_en_0, FLD_CLK_ACLK_EN | FLD_CLK_CODEC2_EN);
    delay_us(1); /* wait codec1 clock stable. */
    reg_audio_codec1_cr_vic = FLD_CODEC1_DIGITAL_SB;
    reg_audio_codec1_cr_vic = (reg_audio_codec1_cr_vic & (~FLD_CODEC1_DIGITAL_SB)) |
                              MASK_VAL(FLD_CODEC1_DIGITAL_SB, power_mode);
}

/**
 * @brief      This function serves to power down codec0 adc.
 * @param[in]  adc - adc channel.
 * @return     none
 * @note
 *             - This interface only powers down the analog ADC.
 */
void audio_codec0_adc_power_down(audio_codec0_input_select_e adc)
{
    unsigned char channel = adc & BIT_RNG(0, 3);

    audio_codec0_set_input_mute(channel, 1); /* adc mute. */
    audio_codec0_set_micbias(channel, 0);

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                                                                    /* ADC_A1. */
        BM_SET(reg_audio_codec0_cr_adca12, FLD_CODEC0_SB_ADCA1);                               /* adc A1 channel inactive */
        break;
    case AUDIO_DMIC_ADC_A2:                                                                    /* ADC_A2. */
        BM_SET(reg_audio_codec0_cr_adca12, FLD_CODEC0_SB_ADCA2);                               /* adc A2 channel inactive */
        break;
    case AUDIO_DMIC_ADC_A1_A2:                                                                 /* ADC_A1_A2. */
        reg_audio_codec0_cr_adca12 = (reg_audio_codec0_cr_adca12 & (~(FLD_CODEC0_SB_ADCA1 | FLD_CODEC0_SB_ADCA2))) |
                                     MASK_VAL(FLD_CODEC0_SB_ADCA1, 1, FLD_CODEC0_SB_ADCA2, 1); /* adc A1/A2 channel inactive */
        break;
    case AUDIO_DMIC_ADC_B1:                                                                    /* ADC_B1. */
        BM_SET(reg_audio_codec0_cr_adcb12, FLD_CODEC0_SB_ADCB1);                               /* adc B1 channel inactive */
        break;
    case AUDIO_DMIC_ADC_B2:                                                                    /* ADC_B2. */
        BM_SET(reg_audio_codec0_cr_adcb12, FLD_CODEC0_SB_ADCB2);                               /* adc B2 channel inactive */
        break;
    case AUDIO_DMIC_ADC_B1_B2:                                                                 /* ADC_B1_B2. */
        reg_audio_codec0_cr_adcb12 = (reg_audio_codec0_cr_adcb12 & (~(FLD_CODEC0_SB_ADCB1 | FLD_CODEC0_SB_ADCB2))) |
                                     MASK_VAL(FLD_CODEC0_SB_ADCB1, 1, FLD_CODEC0_SB_ADCB2, 1); /* adc B1/B2 channel inactive */
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to power on codec0 output.
 * @param[in]  output - output channel.
 * @return     none
 */
void audio_codec0_output_power_down(audio_codec0_output_select_e output)
{
    audio_codec0_set_output_mute(output, 1);                                                       /* dac mute. */

    switch (output) {
    case AUDIO_DAC_A1:                                                                             /* DAC_A1. */
        BM_SET(reg_audio_codec_dac_cr_daca12, FLD_CODEC0_SB_ADCA1);                                /* DAC A1 in power-down. */
        BM_SET(reg_audio_codec_dac_cr_hpa, FLD_CODEC_DAC_SB_HPA1);                                 /* Headphone A1 output stage is in power down. */
        break;
    case AUDIO_DAC_A2:                                                                             /* DAC_A1. */
        BM_SET(reg_audio_codec_dac_cr_daca12, FLD_CODEC0_SB_ADCA2);                                /* DAC A2 in power-down. */
        BM_SET(reg_audio_codec_dac_cr_hpa, FLD_CODEC_DAC_SB_HPA2);                                 /* Headphone A2 output stage is in power down. */
        break;
    case AUDIO_DAC_A1_A2:                                                                          /* DAC_A1_A2. */
        reg_audio_codec_dac_cr_daca12 = (reg_audio_codec_dac_cr_daca12 & (~(FLD_CODEC0_SB_ADCA1 | FLD_CODEC0_SB_ADCA2))) |
                                        MASK_VAL(FLD_CODEC0_SB_ADCA1, 1, FLD_CODEC0_SB_ADCA2, 1);  /* DAC A1/A2 in power-down. */
        reg_audio_codec_dac_cr_hpa = (reg_audio_codec_dac_cr_hpa & (~(FLD_CODEC_DAC_SB_HPA1 | FLD_CODEC_DAC_SB_HPA2))) |
                                     MASK_VAL(FLD_CODEC_DAC_SB_HPA1, 1, FLD_CODEC_DAC_SB_HPA2, 1); /* Headphone A1/A2 output stage is in power down. */
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to power down codec0.
 * @return     none
 * @note
 *             - After power down, cannot access codec0 registers.
 */
void audio_codec0_power_down(void)
{
    /* power down output. */
    audio_codec0_output_power_down(AUDIO_DAC_A1_A2);

    /* power down adc. */
    audio_codec0_adc_power_down(AUDIO_DMIC_ADC_A1_A2);
    audio_codec0_adc_power_down(AUDIO_DMIC_ADC_B1_B2);

    /* power down dmic. */
    audio_codec0_dmic_clk_en(AUDIO_DMIC_ADC_A1_A2, 0);
    audio_codec0_dmic_clk_en(AUDIO_DMIC_ADC_B1_B2, 0);

    /* power down codec0. */
    reg_audio_codec0_cr_vic    = reg_audio_codec0_cr_vic | 0x0f;
    reg_audio_codec_dac_cr_vic = reg_audio_codec_dac_cr_vic | 0x0f;
    BM_CLR(reg_audio_clk_en_0, FLD_CLK_CODEC0_EN); /* After clock disable, cannot access codec registers. */
    BM_CLR(reg_audio_clk_en_0, FLD_CLK_CODEC1_EN); /* After clock disable, cannot access codec registers. */
}

/**
 * @brief      This function serves to power down codec1.
 * @return     none
 * @note
 *             - After power down, cannot access codec1 registers.
 */
void audio_codec1_power_down(void)
{
    /* power down dmic. */
    audio_codec1_dmic_clk_en(AUDIO_CODEC1_DMIC_A1_A2, 0);

    /* power down codec0. */
    reg_audio_codec1_cr_vic = reg_audio_codec0_cr_vic | 1;
    BM_CLR(reg_audio_clk_en_0, FLD_CLK_CODEC2_EN); /* After clock disable, cannot access codec registers. */
}

/**
 * @brief      This function serves to enable/disable codec0 dmic clock.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: active dmic clock, 0: power-down dmic clock.
 */
void audio_codec0_dmic_clk_en(audio_codec0_input_select_e input, unsigned char enable)
{
    unsigned char channel = input & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                        /* ADC_A1. */
        reg_audio_codec0_cr_dmic_adca_sb = (reg_audio_codec0_cr_dmic_adca_sb & (~FLD_CODEC0_SB_DMIC_ADCA1)) |
                                           MASK_VAL(FLD_CODEC0_SB_DMIC_ADCA1, !enable);
        break;
    case AUDIO_DMIC_ADC_A2: /* ADC_A2. */
        reg_audio_codec0_cr_dmic_adca_sb = (reg_audio_codec0_cr_dmic_adca_sb & (~FLD_CODEC0_SB_DMIC_ADCA2)) |
                                           MASK_VAL(FLD_CODEC0_SB_DMIC_ADCA2, !enable);
        break;
    case AUDIO_DMIC_ADC_B1: /* ADC_B1. */
        reg_audio_codec0_cr_dmic_adcb_sb = (reg_audio_codec0_cr_dmic_adcb_sb & (~FLD_CODEC0_SB_DMIC_ADCB1)) |
                                           MASK_VAL(FLD_CODEC0_SB_DMIC_ADCB1, !enable);
        break;
    case AUDIO_DMIC_ADC_B2: /* ADC_B2. */
        reg_audio_codec0_cr_dmic_adcb_sb = (reg_audio_codec0_cr_dmic_adcb_sb & (~FLD_CODEC0_SB_DMIC_ADCB2)) |
                                           MASK_VAL(FLD_CODEC0_SB_DMIC_ADCB2, !enable);
        break;
    case AUDIO_DMIC_ADC_A1_A2: /*ADC_A1_A2. */
        reg_audio_codec0_cr_dmic_adca_sb = (reg_audio_codec0_cr_dmic_adca_sb & (~(FLD_CODEC0_SB_DMIC_ADCA1 | FLD_CODEC0_SB_DMIC_ADCA2))) |
                                           MASK_VAL(FLD_CODEC0_SB_DMIC_ADCA1, !enable, FLD_CODEC0_SB_DMIC_ADCA2, !enable);
        break;
    case AUDIO_DMIC_ADC_B1_B2: /*ADC_B1_B2. */
        reg_audio_codec0_cr_dmic_adcb_sb = (reg_audio_codec0_cr_dmic_adcb_sb & (~(FLD_CODEC0_SB_DMIC_ADCB1 | FLD_CODEC0_SB_DMIC_ADCB2))) |
                                           MASK_VAL(FLD_CODEC0_SB_DMIC_ADCB1, !enable, FLD_CODEC0_SB_DMIC_ADCB2, !enable);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to enable/disable codec1 dmic clock.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: active dmic clock, 0: power-down dmic clock.
 */
void audio_codec1_dmic_clk_en(audio_codec1_input_select_e input, unsigned char enable)
{
    unsigned char channel = input;

    switch (channel) {
    case AUDIO_CODEC1_DMIC_A1: /* ADC_A1. */
        reg_audio_codec1_cr_dmic_adca_sb = (reg_audio_codec1_cr_dmic_adca_sb & (~FLD_CODEC1_SB_DMIC_ADCA1)) |
                                           MASK_VAL(FLD_CODEC1_SB_DMIC_ADCA1, !enable);
        break;
    case AUDIO_CODEC1_DMIC_A2: /* ADC_A2. */
        reg_audio_codec1_cr_dmic_adca_sb = (reg_audio_codec1_cr_dmic_adca_sb & (~FLD_CODEC1_SB_DMIC_ADCA2)) |
                                           MASK_VAL(FLD_CODEC1_SB_DMIC_ADCA2, !enable);
        break;
    case AUDIO_CODEC1_DMIC_A1_A2: /*ADC_A1_A2. */
        reg_audio_codec1_cr_dmic_adca_sb = (reg_audio_codec1_cr_dmic_adca_sb & (~(FLD_CODEC1_SB_DMIC_ADCA1 | FLD_CODEC1_SB_DMIC_ADCA2))) |
                                           MASK_VAL(FLD_CODEC1_SB_DMIC_ADCA1, !enable, FLD_CODEC1_SB_DMIC_ADCA2, !enable);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to enable/disable codec0 micbias output.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: enable micbias, 0: disable micbias.
 * @return     none
 * @note
 *             - bias only for amic.
 */
void audio_codec0_set_micbias(audio_codec0_input_select_e input, unsigned char enable)
{
    unsigned char channel = input & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                        /* ADC_A1. */
    case AUDIO_DMIC_ADC_A2:                        /* ADC_A2. */
    case AUDIO_DMIC_ADC_B1:                        /* ADC_B1. */
    case AUDIO_DMIC_ADC_B2:                        /* ADC_B2. */
        reg_audio_codec0_cr_adc_mic(channel) = (reg_audio_codec0_cr_adc_mic(channel) & (~FLD_CODEC0_SB_MICBIAS)) |
                                               MASK_VAL(FLD_CODEC0_SB_MICBIAS, !enable);
        break;
    case AUDIO_DMIC_ADC_A1_A2: /*ADC_A1_A2. */
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A1) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A1) & (~FLD_CODEC0_SB_MICBIAS)) |
                                                         MASK_VAL(FLD_CODEC0_SB_MICBIAS, !enable);
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A2) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A2) & (~FLD_CODEC0_SB_MICBIAS)) |
                                                         MASK_VAL(FLD_CODEC0_SB_MICBIAS, !enable);
        break;
    case AUDIO_DMIC_ADC_B1_B2: /* ADC_B1_B2 */
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B1) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B1) & (~FLD_CODEC0_SB_MICBIAS)) |
                                                         MASK_VAL(FLD_CODEC0_SB_MICBIAS, !enable);
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B2) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B2) & (~FLD_CODEC0_SB_MICBIAS)) |
                                                         MASK_VAL(FLD_CODEC0_SB_MICBIAS, !enable);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec0 micbias output state when Microphone bias in power-down (reg_audio_codec0_cr_adc_mic[5], SB_MICBIAS = 1).
 * @param[in]  input  - input channel.
 * @param[in]  state - AUDIO_AMIC_BIAS_PULL_DOWN_ZERO or AUDIO_AMIC_BIAS_HIGH_IMPEDANCE.
 * @return     none
 * @note       If all the bias are connected together (BGA-packaged chip), it is recommended to configure the other unused bias to high resistance
 *             when a bias is being used to prevent current leakage. By default, the codec0 micbias has been configured to high-impedance after powered on if codec0 amic is used.
 *             - bias only for amic.
 */
void audio_codec0_set_micbias_state(audio_codec0_input_select_e input, audio_codec0_bias_output_state_e state)
{
    unsigned char channel = input & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                        /* ADC_A1. */
    case AUDIO_DMIC_ADC_A2:                        /* ADC_A2. */
    case AUDIO_DMIC_ADC_B1:                        /* ADC_B1. */
    case AUDIO_DMIC_ADC_B2:                        /* ADC_B2. */
        reg_audio_codec0_cr_adc_mic(channel) = (reg_audio_codec0_cr_adc_mic(channel) & (~FLD_CODEC0_MICBIAS1_V)) |
                                               MASK_VAL(FLD_CODEC0_MICBIAS1_V, state);
        break;
    case AUDIO_DMIC_ADC_A1_A2: /*ADC_A1_A2. */
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A1) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A1) & (~FLD_CODEC0_MICBIAS1_V)) |
                                                         MASK_VAL(FLD_CODEC0_MICBIAS1_V, state);
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A2) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A2) & (~FLD_CODEC0_MICBIAS1_V)) |
                                                         MASK_VAL(FLD_CODEC0_MICBIAS1_V, state);
        break;
    case AUDIO_DMIC_ADC_B1_B2: /* ADC_B1_B2 */
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B1) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B1) & (~FLD_CODEC0_MICBIAS1_V)) |
                                                         MASK_VAL(FLD_CODEC0_MICBIAS1_V, state);
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B2) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B2) & (~FLD_CODEC0_MICBIAS1_V)) |
                                                         MASK_VAL(FLD_CODEC0_MICBIAS1_V, state);
        break;
    default:
        break;
    }
}
/**
 * @brief      This function serves to set codec0 adc mode.
 * @param[in]  adc            - adc channel.
 * @param[in]  capacitor_mode - 1: Capacitor-coupled input, 0: Capacitor-less input (direct connection).
 * @return     none
 * @note
 *             - capacitor mode only for line_in or amic.
 */
void audio_codec0_set_adc_capacitor_mode(audio_codec0_input_select_e adc, audio_codec0_adc_capacitor_mode_e capacitor_mode)
{
    unsigned char channel = adc & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                      /* ADC_A1. */
    case AUDIO_DMIC_ADC_A2:                      /* ADC_A2. */
    case AUDIO_DMIC_ADC_B1:                      /* ADC_B1. */
    case AUDIO_DMIC_ADC_B2:                      /* ADC_B2. */
        reg_audio_codec0_cr_adc_mic(channel) = (reg_audio_codec0_cr_adc_mic(channel) & (~FLD_CODEC0_CAPCOUPLED)) |
                                               MASK_VAL(FLD_CODEC0_CAPCOUPLED, capacitor_mode);
        break;
    case AUDIO_DMIC_ADC_A1_A2: /*ADC_A1_A2. */
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A1) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A1) & (~FLD_CODEC0_CAPCOUPLED)) |
                                                         MASK_VAL(FLD_CODEC0_CAPCOUPLED, capacitor_mode);
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A2) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A2) & (~FLD_CODEC0_CAPCOUPLED)) |
                                                         MASK_VAL(FLD_CODEC0_CAPCOUPLED, capacitor_mode);
        break;
    case AUDIO_DMIC_ADC_B1_B2: /*ADC_B1_B2. */
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B1) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B1) & (~FLD_CODEC0_CAPCOUPLED)) |
                                                         MASK_VAL(FLD_CODEC0_CAPCOUPLED, capacitor_mode);
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B2) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B2) & (~FLD_CODEC0_CAPCOUPLED)) |
                                                         MASK_VAL(FLD_CODEC0_CAPCOUPLED, capacitor_mode);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec0 adc mode.
 * @param[in]  adc      - adc channel.
 * @param[in]  adc_mode - 1: differential input, 0: single-ended input.
 * @return     none
 * @note
 *             - adc mode only for line_in or amic.
 */
void audio_codec0_set_adc_mode(audio_codec0_input_select_e adc, audio_codec0_adc_mode_e adc_mode)
{
    unsigned char channel = adc & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                      /* ADC_A1. */
    case AUDIO_DMIC_ADC_A2:                      /* ADC_A2. */
    case AUDIO_DMIC_ADC_B1:                      /* ADC_B1. */
    case AUDIO_DMIC_ADC_B2:                      /* ADC_B2. */
        reg_audio_codec0_cr_adc_mic(channel) = (reg_audio_codec0_cr_adc_mic(channel) & (~FLD_CODEC0_MICDIFF)) |
                                               MASK_VAL(FLD_CODEC0_MICDIFF, adc_mode);
        break;
    case AUDIO_DMIC_ADC_A1_A2: /*ADC_A1_A2. */
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A1) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A1) & (~FLD_CODEC0_MICDIFF)) |
                                                         MASK_VAL(FLD_CODEC0_MICDIFF, adc_mode);
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A2) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_A2) & (~FLD_CODEC0_MICDIFF)) |
                                                         MASK_VAL(FLD_CODEC0_MICDIFF, adc_mode);
        break;
    case AUDIO_DMIC_ADC_B1_B2: /*ADC_B1_B2. */
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B1) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B1) & (~FLD_CODEC0_MICDIFF)) |
                                                         MASK_VAL(FLD_CODEC0_MICDIFF, adc_mode);
        reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B2) = (reg_audio_codec0_cr_adc_mic(AUDIO_DMIC_ADC_B2) & (~FLD_CODEC0_MICDIFF)) |
                                                         MASK_VAL(FLD_CODEC0_MICDIFF, adc_mode);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec0 adc power mode.
 * @param[in]  adc  - adc channel.
 * @param[in]  mode - adc power mode.
 * @return     none
 * @note
 *             - This interface only set line-in and amic power mode.
 */
void audio_codec0_set_adc_power_mode(audio_codec0_input_select_e adc, audio_codec0_adc_power_mode_e mode)
{
    unsigned char channel = adc & BIT_RNG(0, 3);

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                                                                    /* ADC_A1. */
    case AUDIO_DMIC_ADC_A2:                                                                    /* ADC_A2. */
    case AUDIO_DMIC_ADC_A1_A2:                                                                 /* ADC_A1_A2. */
        reg_audio_codec0_cr_adca12 = (reg_audio_codec0_cr_adca12 & (~FLD_CODEC0_ADCA_POWER_MODE)) |
                                     MASK_VAL(FLD_CODEC0_ADCA_POWER_MODE, mode);
        break;
    case AUDIO_DMIC_ADC_B1:                                                                    /* ADC_B1. */
    case AUDIO_DMIC_ADC_B2:                                                                    /* ADC_B2. */
    case AUDIO_DMIC_ADC_B1_B2:                                                                 /* ADC_B1_B2. */
        reg_audio_codec0_cr_adcb12 = (reg_audio_codec0_cr_adcb12 & (~FLD_CODEC0_ADCB_POWER_MODE)) |
                                     MASK_VAL(FLD_CODEC0_ADCB_POWER_MODE, mode);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec0 input mute.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: soft mute active, 0: soft mute inactive.
 * @return     none
 */
void audio_codec0_set_input_mute(audio_codec0_input_select_e input, unsigned char enable)
{
    unsigned char channel = input & BIT_RNG(0, 3);                                           /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                                                                  /* ADC_A1. */
    case AUDIO_DMIC_ADC_A2:                                                                  /* ADC_A2. */
    case AUDIO_DMIC_ADC_B1:                                                                  /* ADC_B1. */
    case AUDIO_DMIC_ADC_B2:                                                                  /* ADC_B2. */
        reg_audio_codec0_cr_adc_dgain(channel) = (reg_audio_codec0_cr_adc_dgain(channel) & (~FLD_CODEC0_ADC_SOFT_MUTE)) |
                                                 MASK_VAL(FLD_CODEC0_ADC_SOFT_MUTE, enable); /* ADC soft mute. */
        break;
    case AUDIO_DMIC_ADC_A1_A2:                                                               /*ADC_A1_A2. */
        reg_audio_codec0_cr_adca1_dgain = (reg_audio_codec0_cr_adca1_dgain & (~FLD_CODEC0_ADCA1_SOFT_MUTE)) |
                                          MASK_VAL(FLD_CODEC0_ADCA1_SOFT_MUTE, enable);      /* ADC_A1 soft mute. */
        reg_audio_codec0_cr_adca2_dgain = (reg_audio_codec0_cr_adca2_dgain & (~FLD_CODEC0_ADCA2_SOFT_MUTE)) |
                                          MASK_VAL(FLD_CODEC0_ADCA2_SOFT_MUTE, enable);      /* ADC_A2 soft mute. */
        break;
    case AUDIO_DMIC_ADC_B1_B2:                                                               /*ADC_B1_B2. */
        reg_audio_codec0_cr_adcb1_dgain = (reg_audio_codec0_cr_adcb1_dgain & (~FLD_CODEC0_ADCB1_SOFT_MUTE)) |
                                          MASK_VAL(FLD_CODEC0_ADCB1_SOFT_MUTE, enable);      /* ADC_B1 soft mute. */
        reg_audio_codec0_cr_adcb2_dgain = (reg_audio_codec0_cr_adcb2_dgain & (~FLD_CODEC0_ADCB2_SOFT_MUTE)) |
                                          MASK_VAL(FLD_CODEC0_ADCB2_SOFT_MUTE, enable);      /* ADC_B2 soft mute. */
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec1 input mute.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: soft mute active, 0: soft mute inactive.
 * @return     none
 */
void audio_codec1_set_input_mute(audio_codec1_input_select_e input, unsigned char enable)
{
    unsigned char channel = input;

    switch (channel) {
    case AUDIO_CODEC1_DMIC_A1:                                                      /* ADC_A1. */
        reg_audio_codec1_cr_adca1_dgain = (reg_audio_codec1_cr_adca1_dgain & (~FLD_CODEC1_ADCA1_SOFT_MUTE)) |
                                          MASK_VAL(FLD_CODEC1_ADCA1_SOFT_MUTE, enable); /* ADC soft mute. */
        break;
    case AUDIO_CODEC1_DMIC_A2:                                                      /* ADC_A2. */
        reg_audio_codec1_cr_adca2_dgain = (reg_audio_codec1_cr_adca2_dgain & (~FLD_CODEC1_ADCA2_SOFT_MUTE)) |
                                          MASK_VAL(FLD_CODEC1_ADCA2_SOFT_MUTE, enable); /* ADC soft mute. */
        break;
    case AUDIO_CODEC1_DMIC_A1_A2:                                                   /*ADC_A1_A2. */
        reg_audio_codec1_cr_adca1_dgain = (reg_audio_codec1_cr_adca1_dgain & (~FLD_CODEC1_ADCA1_SOFT_MUTE)) |
                                          MASK_VAL(FLD_CODEC1_ADCA1_SOFT_MUTE, enable); /* ADC soft mute. */
        reg_audio_codec1_cr_adca2_dgain = (reg_audio_codec1_cr_adca2_dgain & (~FLD_CODEC1_ADCA2_SOFT_MUTE)) |
                                          MASK_VAL(FLD_CODEC1_ADCA2_SOFT_MUTE, enable); /* ADC soft mute. */
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec0 output mute.
 * @param[in]  output - output channel.
 * @param[in]  enable - 1: soft mute active, 0: soft mute inactive.
 * @return     none
 */
void audio_codec0_set_output_mute(audio_codec0_output_select_e output, unsigned char enable)
{
    switch (output) {
    case AUDIO_DAC_A1:                                                                        /* DAC_A1. */
        reg_audio_codec_dac_cr_daca1_dgain = (reg_audio_codec_dac_cr_daca1_dgain & (~FLD_CODEC_DAC_DACA1_SOFT_MUTE)) |
                                             MASK_VAL(FLD_CODEC_DAC_DACA1_SOFT_MUTE, enable); /* DAC_A1 soft mute. */
        break;
    case AUDIO_DAC_A2:                                                                        /* DAC_A2. */
        reg_audio_codec_dac_cr_daca2_dgain = (reg_audio_codec_dac_cr_daca2_dgain & (~FLD_CODEC_DAC_DACA2_SOFT_MUTE)) |
                                             MASK_VAL(FLD_CODEC_DAC_DACA2_SOFT_MUTE, enable); /* DAC_A2 soft mute. */
        break;
    case AUDIO_DAC_A1_A2:                                                                     /* DAC_A1_A2. */
        reg_audio_codec_dac_cr_daca1_dgain = (reg_audio_codec_dac_cr_daca1_dgain & (~FLD_CODEC_DAC_DACA1_SOFT_MUTE)) |
                                             MASK_VAL(FLD_CODEC_DAC_DACA1_SOFT_MUTE, enable); /* DAC_A1 soft mute. */
        reg_audio_codec_dac_cr_daca2_dgain = (reg_audio_codec_dac_cr_daca2_dgain & (~FLD_CODEC_DAC_DACA2_SOFT_MUTE)) |
                                             MASK_VAL(FLD_CODEC_DAC_DACA2_SOFT_MUTE, enable); /* DAC_A2 soft mute. */
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec0 adc filter mode.
 * @param[in]  input - input channel.
 * @param[in]  fmode - filter mode.
 * @return     none
 */
void audio_codec0_set_input_filter_mode(audio_codec0_input_select_e input, audio_filter_mode_e fmode)
{
    unsigned char channel = input & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    if ((channel == AUDIO_DMIC_ADC_A1) || (channel == AUDIO_DMIC_ADC_A2) || (channel == AUDIO_DMIC_ADC_A1_A2)) /* ADC_A. */
    {
        reg_audio_codec0_cr_adca_freq_sel = (reg_audio_codec0_cr_adca_freq_sel & (~FLD_CODEC0_ADCA_FLT_CFG_SEL)) | MASK_VAL(FLD_CODEC0_ADCA_FLT_CFG_SEL, fmode);
    }
    else if ((channel == AUDIO_DMIC_ADC_B1) || (channel == AUDIO_DMIC_ADC_B2) || channel == AUDIO_DMIC_ADC_B1_B2) /* ADC_B. */
    {
        reg_audio_codec0_cr_adcb_freq_sel = (reg_audio_codec0_cr_adcb_freq_sel & (~FLD_CODEC0_ADCB_FLT_CFG_SEL)) | MASK_VAL(FLD_CODEC0_ADCA_FLT_CFG_SEL, fmode);
    }
}

/**
 * @brief      This function serves to set codec0 input sample rate.
 * @param[in]  input - input channel.
 * @param[in]  fs    - input sample rate.
 * @return     none
 * @note
 *             - ADC_A1/2 or ADC_B1/2 sample rates are set in pairs.
 */
void audio_codec0_set_input_fs(audio_codec0_input_select_e input, audio_sample_rate_e fs)
{
    unsigned char channel = input & BIT_RNG(0, 3);                                                                  /* bit[0-3] adc channel. */

    if ((channel == AUDIO_DMIC_ADC_A1) || (channel == AUDIO_DMIC_ADC_A2) || (channel == AUDIO_DMIC_ADC_A1_A2))      /* ADC_A. */
    {
        reg_audio_codec0_cr_adca_freq_sel = (reg_audio_codec0_cr_adca_freq_sel & (~FLD_CODEC0_ADCA_FREQ)) | (fs & 0xff);
        reg_audio_codec0_dec0_pcm_num     = fs >> 8;                                                                /* fs = 12.288MHz/11.2896MHz / (reg_audio_codec_dec_pcm_num + 1) */
    } else if ((channel == AUDIO_DMIC_ADC_B1) || (channel == AUDIO_DMIC_ADC_B2) || channel == AUDIO_DMIC_ADC_B1_B2) /* ADC_B. */
    {
        reg_audio_codec0_cr_adcb_freq_sel = (reg_audio_codec0_cr_adcb_freq_sel & (~FLD_CODEC0_ADCB_FREQ)) | (fs & 0xff);
        reg_audio_codec0_dec1_pcm_num     = fs >> 8;                                                                /* fs = 12.288MHz/11.2896MHz / (reg_audio_codec_dec_pcm_num + 1) */
    }
}

/**
 * @brief      This function serves to set codec1 input sample rate.
 * @param[in]  input - input channel.
 * @param[in]  fs    - input sample rate.
 * @return     none
 * @note
 *             - ADC_A1/2 sample rates are set in pairs.
 */
void audio_codec1_set_input_fs(audio_sample_rate_e fs)
{
    reg_audio_codec1_cr_adca_freq_sel = (reg_audio_codec1_cr_adca_freq_sel & (~FLD_CODEC0_ADCA_FREQ)) | (fs & 0xff);
    reg_audio_codec_dmic_dec2_pcm_num = fs >> 8; /* fs = 12.288MHz/11.2896MHz / (reg_audio_codec_dec_pcm_num + 1) */
}

/**
 * @brief      This function serves to set codec0 dac filter mode.
 * @param[in]  fmode - filter mode.
 * @return     none
 */
void audio_codec0_set_output_filter_mode(audio_filter_mode_e fmode)
{
    reg_audio_codec_dac_cr_daca_freq_sel = (reg_audio_codec_dac_cr_daca_freq_sel & (~FLD_CODEC_DACA_FLT_CFG_SEL)) | MASK_VAL(FLD_CODEC_DACA_FLT_CFG_SEL, fmode);
}

/**
 * @brief      This function serves to set codec0 output sample rate.
 * @param[in]  fs - output sample rate.
 * @return     none
 * @note
 *             - DAC_A1/2 sample rates are set in pairs.
 */
void audio_codec0_set_output_fs(audio_sample_rate_e fs)
{
    reg_audio_codec_dac_cr_daca_freq_sel = (reg_audio_codec_dac_cr_daca_freq_sel & (~FLD_CODEC_DACA_FREQ)) | (fs & 0xff);
    reg_audio_codec0_int_pcm_num         = fs >> 8; /* fs = 12.288MHz/11.2896MHz / (reg_audio_codec_dec_pcm_num + 1) */
}

/**
 * @brief      This function serves to set codec0 input data world length.
 * @param[in]  input - input channel.
 * @param[in]  wl    - world length.
 * @return     none
 * @note
 *             - ADC_A1/2 or ADC_B1/2 world length are set in pairs.
 */
void audio_codec0_set_input_wl(audio_codec0_input_select_e input, audio_codec0_data_select_e wl)
{
    unsigned char channel = input & BIT_RNG(0, 3);                                                             /* bit[0-3] adc channel. */

    if ((channel == AUDIO_DMIC_ADC_A1) || (channel == AUDIO_DMIC_ADC_A2) || (channel == AUDIO_DMIC_ADC_A1_A2)) /* ADC_A. */
    {
        reg_audio_codec0_cr_adca_ai = (reg_audio_codec0_cr_adca_ai & (~(FLD_CODEC0_ADCA_ADWL | FLD_CODEC0_ADCA_SLAVE))) |
                                      MASK_VAL(FLD_CODEC0_ADCA_SLAVE, 1, FLD_CODEC0_ADCA_ADWL, wl);
        reg_audio_codec_data_fmt_l = (reg_audio_codec_data_fmt_l & (~(FLD_CODEC_CTRL_ADC0_SL_SEL | FLD_CODEC_CTRL_ADC0_SR_SEL))) |
                                     MASK_VAL(FLD_CODEC_CTRL_ADC0_SL_SEL, !wl, FLD_CODEC_CTRL_ADC0_SR_SEL, !wl);    /* 16bit: 1, 18/20/24bit: 0 */
    } else if ((channel == AUDIO_DMIC_ADC_B1) || (channel == AUDIO_DMIC_ADC_B2) || channel == AUDIO_DMIC_ADC_B1_B2) /* ADC_B. */
    {
        reg_audio_codec0_cr_adcb_ai = (reg_audio_codec0_cr_adcb_ai & (~(FLD_CODEC0_ADCB_ADWL | FLD_CODEC0_ADCB_SLAVE))) |
                                      MASK_VAL(FLD_CODEC0_ADCB_SLAVE, 1, FLD_CODEC0_ADCB_ADWL, wl);
        reg_audio_codec_data_fmt_l = (reg_audio_codec_data_fmt_l & (~(FLD_CODEC_CTRL_ADC1_SL_SEL | FLD_CODEC_CTRL_ADC1_SR_SEL))) |
                                     MASK_VAL(FLD_CODEC_CTRL_ADC1_SL_SEL, !wl, FLD_CODEC_CTRL_ADC1_SR_SEL, !wl); /* 16bit: 1, 18/20/24bit: 0 */
    }
}

/**
 * @brief      This function serves to set codec1 input data world length.
 * @param[in]  input - input channel.
 * @param[in]  wl    - world length.
 * @return     none
 * @note
 *             - ADC_A1/2 world length are set in pairs.
 */
void audio_codec1_set_input_wl(audio_codec1_data_select_e wl)
{
    reg_audio_codec1_cr_adca_al = (reg_audio_codec1_cr_adca_al & (~(FLD_CODEC0_ADCA_ADWL | FLD_CODEC0_ADCA_SLAVE))) |
                                  MASK_VAL(FLD_CODEC0_ADCA_SLAVE, 1, FLD_CODEC0_ADCA_ADWL, wl);
    reg_audio_codec_data_fmt_l = (reg_audio_codec_data_fmt_l & (~(FLD_CODEC_CTRL_ADC2_SL_SEL | FLD_CODEC_CTRL_ADC2_SR_SEL))) |
                                 MASK_VAL(FLD_CODEC_CTRL_ADC2_SL_SEL, !wl, FLD_CODEC_CTRL_ADC2_SR_SEL, !wl); /* 16bit: 1, 18/20/24bit: 0 */
}

/**
 * @brief      This function serves to set codec0 output data world length.
 * @param[in]  output - output channel.
 * @param[in]  wl     - world length.
 * @return     none
 */
void audio_codec0_set_output_wl(audio_codec0_output_select_e output, audio_codec0_data_select_e wl)
{
    switch (output) {
    case AUDIO_DAC_A1:                                                                                         /* DAC_A1. */
        reg_audio_codec_data_fmt_l = (reg_audio_codec_data_fmt_l & (~FLD_CODEC_CTRL_DAC_SL_SEL)) |
                                     MASK_VAL(FLD_CODEC_CTRL_DAC_SL_SEL, !wl);                                 /* 16bit: 1, 20/24bit: 0 */
        break;
    case AUDIO_DAC_A2:                                                                                         /* DAC_A2. */
        reg_audio_codec_data_fmt_l = (reg_audio_codec_data_fmt_l & (~FLD_CODEC_CTRL_DAC_SR_SEL)) |
                                     MASK_VAL(FLD_CODEC_CTRL_DAC_SR_SEL, !wl);                                 /* 16bit: 1, 20/24bit: 0 */
        break;
    case AUDIO_DAC_A1_A2:                                                                                      /* DAC_A1_A2. */
        reg_audio_codec_data_fmt_l = (reg_audio_codec_data_fmt_l & (~(FLD_CODEC_CTRL_DAC_SL_SEL | FLD_CODEC_CTRL_DAC_SR_SEL))) |
                                     MASK_VAL(FLD_CODEC_CTRL_DAC_SL_SEL, !wl, FLD_CODEC_CTRL_DAC_SR_SEL, !wl); /* 16bit: 1, 20/24bit: 0 */
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec0 input analog gain.
 * @param[in]  input - input channel.
 * @param[in]  gain  - input analog gain.
 * @return     none
 * @note
 *             - input analog gain only for line_in or amic.
 */
void audio_codec0_set_input_again(audio_codec0_input_select_e input, audio_codec0_input_again_e gain)
{
    unsigned char channel = input & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                        /* ADC_A1. */
        reg_audio_codec0_gcr_mica12 = (reg_audio_codec0_gcr_mica12 & (~FLD_CODEC0_GIMA1)) | MASK_VAL(FLD_CODEC0_GIMA1, gain);
        break;
    case AUDIO_DMIC_ADC_A2:                        /* ADC_A2. */
        reg_audio_codec0_gcr_mica12 = (reg_audio_codec0_gcr_mica12 & (~FLD_CODEC0_GIMA2)) | MASK_VAL(FLD_CODEC0_GIMA2, gain);
        break;
    case AUDIO_DMIC_ADC_B1:                        /* ADC_B1. */
        reg_audio_codec0_gcr_micb12 = (reg_audio_codec0_gcr_micb12 & (~FLD_CODEC0_GIMB1)) | MASK_VAL(FLD_CODEC0_GIMB1, gain);
        break;
    case AUDIO_DMIC_ADC_B2:                        /* ADC_B2. */
        reg_audio_codec0_gcr_micb12 = (reg_audio_codec0_gcr_micb12 & (~FLD_CODEC0_GIMB2)) | MASK_VAL(FLD_CODEC0_GIMB2, gain);
        break;
    case AUDIO_DMIC_ADC_A1_A2:                     /* ADC_A1_A2 .*/
        reg_audio_codec0_gcr_mica12 = (gain << 4) | gain;
        break;
    case AUDIO_DMIC_ADC_B1_B2:                     /* ADC_B1_B2 .*/
        reg_audio_codec0_gcr_micb12 = (gain << 4) | gain;
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec0 input digital gain.
 * @param[in]  input - input channel.
 * @param[in]  gain  - input digital gain.
 * @return     none
 */
void audio_codec0_set_input_dgain(audio_codec0_input_select_e input, audio_codec_input_dgain_e gain)
{
    unsigned char channel = input & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                        /* ADC_A1 */
    case AUDIO_DMIC_ADC_A2:                        /* ADC_A2 */
    case AUDIO_DMIC_ADC_B1:                        /* ADC_B1 */
    case AUDIO_DMIC_ADC_B2:                        /* ADC_B2 */
        reg_audio_codec0_cr_adc_dgain(channel) = (reg_audio_codec0_cr_adc_dgain(channel) & (~FLD_CODEC0_GOD_ADC)) | gain;
        break;
    case AUDIO_DMIC_ADC_A1_A2:                     /*ADC_A1_A2 */
        reg_audio_codec0_cr_adca1_dgain = (reg_audio_codec0_cr_adca1_dgain & (~FLD_CODEC0_GOD_ADCA1)) | gain;
        reg_audio_codec0_cr_adca2_dgain = (reg_audio_codec0_cr_adca2_dgain & (~FLD_CODEC0_GOD_ADCA2)) | gain;
        break;
    case AUDIO_DMIC_ADC_B1_B2:                     /*ADC_B1_B2 */
        reg_audio_codec0_cr_adcb1_dgain = (reg_audio_codec0_cr_adcb1_dgain & (~FLD_CODEC0_GID_ADCB1)) | gain;
        reg_audio_codec0_cr_adcb2_dgain = (reg_audio_codec0_cr_adcb2_dgain & (~FLD_CODEC0_GID_ADCB2)) | gain;
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec1 input digital gain.
 * @param[in]  input - input channel.
 * @param[in]  gain  - input digital gain.
 * @return     none
 */
void audio_codec1_set_input_dgain(audio_codec1_input_select_e input, audio_codec_input_dgain_e gain)
{
    switch (input) {
    case AUDIO_CODEC1_DMIC_A1:    /* ADC_A1 */
        reg_audio_codec1_cr_adca1_dgain = (reg_audio_codec1_cr_adca1_dgain & (~FLD_CODEC1_GID_ADCA1)) | gain;
        break;
    case AUDIO_CODEC1_DMIC_A2:    /* ADC_A2 */
        reg_audio_codec1_cr_adca2_dgain = (reg_audio_codec1_cr_adca2_dgain & (~FLD_CODEC1_GID_ADCA2)) | gain;
        break;
    case AUDIO_CODEC1_DMIC_A1_A2: /* ADC_B1 */
        reg_audio_codec1_cr_adca1_dgain = (reg_audio_codec1_cr_adca1_dgain & (~FLD_CODEC1_GID_ADCA1)) | gain;
        reg_audio_codec1_cr_adca2_dgain = (reg_audio_codec1_cr_adca2_dgain & (~FLD_CODEC1_GID_ADCA2)) | gain;
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec0 output analog gain.
 * @param[in]  output - output channel.
 * @param[in]  gain   - output analog gain.
 * @return     none
 */
void audio_codec0_set_output_again(audio_codec0_output_select_e output, audio_codec0_output_again_e gain)
{
    switch (output) {
    case AUDIO_DAC_A1:    /* DAC_A1. */
        reg_audio_codec_dac_gcr_hpa(AUDIO_DAC_A1) = (reg_audio_codec_dac_gcr_hpa(AUDIO_DAC_A1) & (~FLD_CODEC_DAC_GOA)) | gain;
        break;
    case AUDIO_DAC_A2:    /* DAC_A2. */
        reg_audio_codec_dac_gcr_hpa(AUDIO_DAC_A2) = (reg_audio_codec_dac_gcr_hpa(AUDIO_DAC_A2) & (~FLD_CODEC_DAC_GOA)) | gain;
        break;
    case AUDIO_DAC_A1_A2: /* DAC_A1_A2. */
        reg_audio_codec_dac_gcr_hpa(AUDIO_DAC_A1) = (reg_audio_codec_dac_gcr_hpa(AUDIO_DAC_A1) & (~FLD_CODEC_DAC_GOA)) | gain;
        reg_audio_codec_dac_gcr_hpa(AUDIO_DAC_A2) = (reg_audio_codec_dac_gcr_hpa(AUDIO_DAC_A2) & (~FLD_CODEC_DAC_GOA)) | gain;
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to set codec0 output digital gain.
 * @param[in]  output - output channel.
 * @param[in]  gain   - output digital gain.
 * @return     none
 */
void audio_codec0_set_output_dgain(audio_codec0_output_select_e output, audio_codec0_output_dgain_e gain)
{
    switch (output) {
    case AUDIO_DAC_A1:    /* DAC_A1. */
        reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A1) = (reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A1) & (~FLD_CODEC_DAC_GOD_DAC)) | gain;
        break;
    case AUDIO_DAC_A2:    /* DAC_A2. */
        reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A2) = (reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A2) & (~FLD_CODEC_DAC_GOD_DAC)) | gain;
        break;
    case AUDIO_DAC_A1_A2: /* DAC_A1_A2. */
        reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A1) = (reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A1) & (~FLD_CODEC_DAC_GOD_DAC)) | gain;
        reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A2) = (reg_audio_codec_dac_cr_dac_dgain(AUDIO_DAC_A2) & (~FLD_CODEC_DAC_GOD_DAC)) | gain;
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to enable/disable codec0 input SNR optimisation.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: adc SNR optimisation active, 0:adc SNR optimisation inactive.
 * @return     none
 * @note
 *             - ADC_A1/2 or ADC_B1/2 SNR optimisation are set in pairs.
 */
void audio_codec0_set_input_snr_opt(audio_codec0_input_select_e input, unsigned char enable)
{
    unsigned char channel = input & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                        /*ADC_A1 */
    case AUDIO_DMIC_ADC_A2:                        /*ADC_A2 */
    case AUDIO_DMIC_ADC_A1_A2:                     /*ADC_A1_A2 */
        reg_audio_codec0_adca_12_alc_0 = (reg_audio_codec0_adca_12_alc_0 & (~FLD_CODEC0_ADCA_12_SNR_OPT_EN)) |
                                         MASK_VAL(FLD_CODEC0_ADCA_12_SNR_OPT_EN, enable);
        break;
    case AUDIO_DMIC_ADC_B1:                        /*ADC_B1 */
    case AUDIO_DMIC_ADC_B2:                        /*ADC_B2 */
    case AUDIO_DMIC_ADC_B1_B2:                     /*ADC_B1_B2 */
        reg_audio_codec0_adcb_12_alc_0 = (reg_audio_codec0_adcb_12_alc_0 & (~FLD_CODEC0_ADCB_12_SNR_OPT_EN)) |
                                         MASK_VAL(FLD_CODEC0_ADCB_12_SNR_OPT_EN, enable);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to enable/disable codec0 input.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: enable, 0: disable.
 * @return     none
 * @note
 *             - Must be disable when switching the input sample rate.
 *             - ADC_A1/2 or ADC_B1/2 are set in pairs.
 */
void audio_codec0_input_en(audio_codec0_input_select_e input, unsigned char enable)
{
    unsigned char channel = input & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                        /*ADC_A1 */
    case AUDIO_DMIC_ADC_A2:                        /*ADC_A2 */
    case AUDIO_DMIC_ADC_A1_A2:                     /*ADC_A1_A2 */
        reg_audio_codec_ctrl = (reg_audio_codec_ctrl & (~FLD_CODEC_CTRL_ADC0_MST_EN)) | MASK_VAL(FLD_CODEC_CTRL_ADC0_MST_EN, enable);
        break;
    case AUDIO_DMIC_ADC_B1:                        /*ADC_B1 */
    case AUDIO_DMIC_ADC_B2:                        /*ADC_B2 */
    case AUDIO_DMIC_ADC_B1_B2:                     /*ADC_B1_B2 */
        reg_audio_codec_ctrl = (reg_audio_codec_ctrl & (~FLD_CODEC_CTRL_ADC1_MST_EN)) | MASK_VAL(FLD_CODEC_CTRL_ADC1_MST_EN, enable);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to enable/disable codec1 input.
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: enable, 0: disable.
 * @return     none
 * @note
 *             - Must be disable when switching the input sample rate.
 *             - ADC_A1/2 are set in pairs.
 */
void audio_codec1_input_en(unsigned char enable)
{
    reg_audio_codec_ctrl = (reg_audio_codec_ctrl & (~FLD_CODEC_CTRL_ADC2_MST_EN)) | MASK_VAL(FLD_CODEC_CTRL_ADC2_MST_EN, enable);
}

/**
 * @brief      This function serves to enable/disable codec0 input HPF(High Pass Filter).
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: adc High Pass Filter active, 0:adc High Pass Filter inactive.
 * @return     none
 * @note
 *             - ADC_A1/2 or ADC_B1/2 High Pass Filter are set in pairs.
 */
void audio_codec0_input_hpf_en(audio_codec0_input_select_e input, unsigned char enable)
{
    unsigned char channel = input & BIT_RNG(0, 3); /* bit[0-3] adc channel. */

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                        /* ADC_A1. */
    case AUDIO_DMIC_ADC_A2:                        /* ADC_A2. */
    case AUDIO_DMIC_ADC_A1_A2:                     /* ADC_A1_A2. */
        reg_audio_codec0_cr_adca_hpf = (reg_audio_codec0_cr_adca_hpf & (~FLD_CODEC0_ADCA12_HPF_EN)) | enable;
        break;
    case AUDIO_DMIC_ADC_B1:                        /* ADC_B1. */
    case AUDIO_DMIC_ADC_B2:                        /* ADC_B2. */
    case AUDIO_DMIC_ADC_B1_B2:                     /* ADC_B1_B2. */
        reg_audio_codec0_cr_adcb_hpf = (reg_audio_codec0_cr_adcb_hpf & (~FLD_CODEC0_ADCB12_HPF_EN)) | enable;
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to enable/disable codec1 input HPF(High Pass Filter).
 * @param[in]  input  - input channel.
 * @param[in]  enable - 1: adc High Pass Filter active, 0:adc High Pass Filter inactive.
 * @return     none
 * @note
 *             - ADC_A1/2 High Pass Filter are set in pairs.
 */
void audio_codec1_input_hpf_en(unsigned char enable)
{
    reg_audio_codec1_cr_adca_hpf = (reg_audio_codec1_cr_adca_hpf & (~FLD_CODEC0_ADCA12_HPF_EN)) | enable;
}

/**
 * @brief      This function serves to init codec0 input.
 * @param[in]  input_config - codec0 input config.
 * @return     none
 */
void audio_codec0_input_init(audio_codec0_input_config_t *input_config)
{
    unsigned char channel    = input_config->input_src & BIT_RNG(0, 3);
    unsigned char input_type = 0;                            /* 0: analog data, 1: digital data. */

    audio_codec0_set_input_mute(input_config->input_src, 1); /* adc mute. */
    audio_codec0_set_input_wl(input_config->input_src, input_config->data_format);
    audio_codec0_set_input_fs(input_config->input_src, input_config->sample_rate);
    audio_codec0_input_hpf_en(input_config->input_src, 1);

    if (input_config->input_src & BIT(5))                                                  /* amic enable bias and set differential input. */
    {
        input_type = 0;                                                                    /* analog data. */
        audio_codec0_set_micbias(input_config->input_src, 1);                              /* enable bias output. */
        audio_codec0_set_adc_capacitor_mode(input_config->input_src, AUDIO_CODEC0_ADC_CAPACITOR_COUPLED);
        audio_codec0_set_adc_mode(input_config->input_src, AUDIO_CODEC0_ADC_DIFFERENTIAL); /* adc differential input. */
        audio_codec0_set_input_snr_opt(input_config->input_src,1);                         /* Avoid difference in ADC Sampling Data When a BDT active/reset occurs. */
    } else if (input_config->input_src & BIT(4))                                           /* line_in set differential input. */
    {
        input_type = 0;                                                                    /* analog data. */
        audio_codec0_set_adc_capacitor_mode(input_config->input_src, AUDIO_CODEC0_ADC_CAPACITOR_COUPLED);
        audio_codec0_set_adc_mode(input_config->input_src, AUDIO_CODEC0_ADC_DIFFERENTIAL); /* adc differential input. */
        audio_codec0_set_input_snr_opt(input_config->input_src,1);                         /* Avoid difference in ADC Sampling Data When a BDT active/reset occurs. */
    } else {
        input_type = 1;                                                                    /* digital data. */
        audio_codec0_dmic_clk_en(input_config->input_src, 1);                              /* enable dmic clock. */
    }

    switch (channel) {
    case AUDIO_DMIC_ADC_A1:                                                                   /* ADC_A1. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC0_MST_EN);                             /* audio adc A master en, codec adc A is slave. */
        if (0 == input_type)                                                                  /* analog data. */
        {
            BM_CLR(reg_audio_codec0_cr_adca12, FLD_CODEC0_SB_ADCA1);                          /* adc A1 channel active. */
        }
        reg_audio_codec0_cr_mic_adca_12_sel = (reg_audio_codec0_cr_mic_adca_12_sel & ~(FLD_CODEC0_ADCA1_MIC_SEL)) |
                                              MASK_VAL(FLD_CODEC0_ADCA1_MIC_SEL, input_type); /* select ADC_A1 input data type. */
        BM_CLR(reg_audio_codec0_cr_adca_ai_sb, FLD_CODEC0_SB_AICR_ADCA12);                    /* ADCA12 audio interface active. */
        break;
    case AUDIO_DMIC_ADC_A2:                                                                   /* ADC_A2. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC0_MST_EN);                             /* audio adc A master en, codec adc A is slave. */
        if (0 == input_type)                                                                  /* analog data. */
        {
            BM_CLR(reg_audio_codec0_cr_adca12, FLD_CODEC0_SB_ADCA2);                          /* adc A2 channel active. */
        }
        reg_audio_codec0_cr_mic_adca_12_sel = (reg_audio_codec0_cr_mic_adca_12_sel & ~(FLD_CODEC0_ADCA2_MIC_SEL)) |
                                              MASK_VAL(FLD_CODEC0_ADCA2_MIC_SEL, input_type); /* select ADC_A2 input data type. */
        BM_CLR(reg_audio_codec0_cr_adca_ai_sb, FLD_CODEC0_SB_AICR_ADCA12);                    /* ADCA12 audio interface active. */
        break;
    case AUDIO_DMIC_ADC_B1:                                                                   /* ADC_B1. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC1_MST_EN);                             /* audio adc B master en, codec adc B is slave. */
        if (0 == input_type)                                                                  /* analog data. */
        {
            BM_CLR(reg_audio_codec0_cr_adcb12, FLD_CODEC0_SB_ADCB1);                          /* adc B1 channel active. */
        }
        else if (1 == input_type)                                                             /* digital data, set dmic1 clk(dmic0 clk default is 3072KHZ). */
        {
            audio_codec0_set_dmic_b_clk(AUDIO_CODEC0_DMIC_CLK_3072KHZ);
        }
        reg_audio_codec0_cr_mic_adcb_12_sel = (reg_audio_codec0_cr_mic_adcb_12_sel & ~(FLD_CODEC0_ADCB1_MIC_SEL)) |
                                              MASK_VAL(FLD_CODEC0_ADCB1_MIC_SEL, input_type); /* select ADC_B1 input data type. */
        BM_CLR(reg_audio_codec0_cr_adcb_ai_sb, FLD_CODEC0_SB_AICR_ADCB12);                    /* ADCB12 audio interface active. */
        break;
    case AUDIO_DMIC_ADC_B2:                                                                   /* ADC_B2. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC1_MST_EN);                             /* audio adc B master en, codec adc B is slave. */
        if (0 == input_type)                                                                  /* analog data. */
        {
            BM_CLR(reg_audio_codec0_cr_adcb12, FLD_CODEC0_SB_ADCB2);                          /* adc B2 channel active. */
        }
        else if (1 == input_type)                                                             /* digital data, set dmic1 clk(dmic0 clk default is 3072KHZ). */
        {
            audio_codec0_set_dmic_b_clk(AUDIO_CODEC0_DMIC_CLK_3072KHZ);
        }
        reg_audio_codec0_cr_mic_adcb_12_sel = (reg_audio_codec0_cr_mic_adcb_12_sel & ~(FLD_CODEC0_ADCB2_MIC_SEL)) |
                                              MASK_VAL(FLD_CODEC0_ADCB2_MIC_SEL, input_type); /* select ADC_B1 input data type. */
        BM_CLR(reg_audio_codec0_cr_adcb_ai_sb, FLD_CODEC0_SB_AICR_ADCB12);                    /* ADCB12 audio interface active. */
        break;
    case AUDIO_DMIC_ADC_A1_A2:                                                                                                      /* ADC_A1_A2. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC0_MST_EN);                                                                   /* audio adc A master en, codec adc A is slave. */
        if (0 == input_type)                                                                                                        /* analog data. */
        {
            reg_audio_codec0_cr_adca12 = (reg_audio_codec0_cr_adca12 & (~(FLD_CODEC0_SB_ADCA1 | FLD_CODEC0_SB_ADCA2))) |
                                         MASK_VAL(FLD_CODEC0_SB_ADCA1, 0, FLD_CODEC0_SB_ADCA2, 0);                                  /* adc A1/A2 channel active. */
        }
        reg_audio_codec0_cr_mic_adca_12_sel =
            (reg_audio_codec0_cr_mic_adca_12_sel & ~((FLD_CODEC0_ADCA1_MIC_SEL | FLD_CODEC0_ADCA2_MIC_SEL))) |
            MASK_VAL(FLD_CODEC0_ADCA1_MIC_SEL, input_type, FLD_CODEC0_ADCA2_MIC_SEL, input_type);                                   /* select ADC_A1/ADC_A2 input data type. */
        BM_CLR(reg_audio_codec0_cr_adca_ai_sb, FLD_CODEC0_SB_AICR_ADCA12);                                                          /* ADCA12 audio interface active. */
        break;
    case AUDIO_DMIC_ADC_B1_B2:                                                                                                          /* ADC_B1_B2. */
        BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC1_MST_EN);                                                                   /* audio adc B master en, codec adc B is slave. */
        if (0 == input_type)                                                                                                        /* analog data. */
        {
            reg_audio_codec0_cr_adcb12 = (reg_audio_codec0_cr_adcb12 & (~(FLD_CODEC0_SB_ADCB1 | FLD_CODEC0_SB_ADCB2))) |
                                         MASK_VAL(FLD_CODEC0_SB_ADCB1, 0, FLD_CODEC0_SB_ADCB2, 0);                                  /* adc B1/B2 channel active. */
        }
        else if (1 == input_type)                                                                                                   /* digital data, set dmic1 clk(dmic0 clk default is 3072KHZ). */
        {
            audio_codec0_set_dmic_b_clk(AUDIO_CODEC0_DMIC_CLK_3072KHZ);
        }
        reg_audio_codec0_cr_mic_adcb_12_sel = (reg_audio_codec0_cr_mic_adcb_12_sel & ~(FLD_CODEC0_ADCB1_MIC_SEL | FLD_CODEC0_ADCB2_MIC_SEL)) |
                                              MASK_VAL(FLD_CODEC0_ADCB1_MIC_SEL, input_type, FLD_CODEC0_ADCB2_MIC_SEL, input_type); /* select ADC_B1 input data type. */
        BM_CLR(reg_audio_codec0_cr_adcb_ai_sb, FLD_CODEC0_SB_AICR_ADCB12);                                                          /* ADCB12 audio interface active. */
        break;
    default:
        break;
    }
    if (0 == input_type)                                     /* analog data. */
    {
        delay_ms(1);                                         /* codec power-up timing requirements. */
    }
    audio_codec0_set_input_mute(input_config->input_src, 0); /* adc un-mute. */
    delay_ms(25);                                            /* codec power-up timing requirements. */
    audio_codec0_set_input_snr_opt(input_config->input_src,0);
}

/**
 * @brief      This function serves to init codec1 input.
 * @param[in]  input_config - codec0 input config.
 * @return     none
 */
void audio_codec1_input_init(audio_codec1_input_config_t *input_config)
{
    audio_codec1_set_input_mute(input_config->input_src, 1);           /* adc mute. */
    audio_codec1_set_input_wl(input_config->data_format);
    audio_codec1_set_input_fs(input_config->sample_rate);
    audio_codec1_input_hpf_en(1);
    audio_codec1_dmic_clk_en(input_config->input_src, 1);              /* enable dmic clock. */

    BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_ADC2_MST_EN);          /* audio adc A master en, codec adc A is slave. */
    BM_CLR(reg_audio_codec1_cr_adca_al_sb, FLD_CODEC1_SB_AICR_ADCA12); /* ADCA12 audio interface active. */

    delay_ms(1);                                                       /* codec power-up timing requirements. */
    audio_codec1_set_input_mute(input_config->input_src, 0);           /* adc un-mute. */
    delay_ms(25);                                                      /* codec power-up timing requirements. */
}

/**
 * @brief      This function serves to init codec0 output config.
 * @param[in]  output_config - codec0 output config.
 * @return     none
 */
void audio_codec0_output_init(audio_codec0_output_config_t *output_config)
{
    audio_codec0_set_output_mute(output_config->output_dst, 1);                                         /* dac mute. */
    audio_codec0_set_output_wl(output_config->output_dst, output_config->data_format);
    audio_codec0_set_output_fs(output_config->sample_rate);
    BM_SET(reg_audio_codec_ctrl, FLD_CODEC_CTRL_DAC_MST_EN);                                            /* audio dac master en, codec dac is slave. */

    reg_audio_codec_dac_cr_daca_ai = (reg_audio_codec_dac_cr_daca_ai & (~(FLD_CODEC_DAC_SB_AICR_DACA | FLD_CODEC_DACA_SLAVE))) |
                                     MASK_VAL(FLD_CODEC_DAC_SB_AICR_DACA, 0, FLD_CODEC_DACA_SLAVE, 1);  /* DACA audio interface active and select slave. */

    switch (output_config->output_dst) {
    case AUDIO_DAC_A1:                                                                                  /* DAC_A1. */
        BM_CLR(reg_audio_codec_dac_cr_daca12, FLD_CODEC_DAC_SB_DACA1);                                  /* DAC A1 active. */
        BM_CLR(reg_audio_codec_dac_cr_hpa, FLD_CODEC_DAC_SB_HPA1);                                      /* Headphone A1 output stage is active. */
        break;
    case AUDIO_DAC_A2:                                                                                  /* DAC_A2. */
        BM_CLR(reg_audio_codec_dac_cr_daca12, FLD_CODEC_DAC_SB_DACA2);                                  /* DAC A2 active. */
        BM_CLR(reg_audio_codec_dac_cr_hpa, FLD_CODEC_DAC_SB_HPA2);                                      /* Headphone A2 output stage is active. */
        break;
    case AUDIO_DAC_A1_A2:                                                                               /* DAC_A1_A2. */
        reg_audio_codec_dac_cr_daca12 = (reg_audio_codec_dac_cr_daca12 & (~(FLD_CODEC_DAC_SB_DACA1 | FLD_CODEC_DAC_SB_DACA2))) |
                                        MASK_VAL(FLD_CODEC_DAC_SB_DACA1, 0, FLD_CODEC_DAC_SB_DACA2, 0); /* DAC A1/A2 active. */
        reg_audio_codec_dac_cr_hpa = (reg_audio_codec_dac_cr_hpa & (~(FLD_CODEC_DAC_SB_HPA1 | FLD_CODEC_DAC_SB_HPA2))) |
                                     MASK_VAL(FLD_CODEC_DAC_SB_HPA1, 0, FLD_CODEC_DAC_SB_HPA2, 0);      /* Headphone A1/A2 output stage is active. */
        break;
    default:
        break;
    }
    audio_codec0_set_output_again(output_config->output_dst, AUDIO_OUT_A_GAIN_0_DB);
    delay_ms(1);                                                /* codec power-up timing requirements */
    audio_codec0_set_output_mute(output_config->output_dst, 0); /* dac un-mute. */
    delay_ms(25);                                               /* codec power-up timing requirements */
}

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio dma/fifo interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio dma/fifo interface
 * @{
 */

/**
 * @brief      This function serves to config  rx_dma channel.
 * @param[in]  chn          - dma channel
 * @param[in]  dst_addr     - Pointer to data buffer, it must be 4-bytes aligned address.
 *                            and the actual buffer size defined by the user needs to be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in]  data_len     - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @param[in]  head_of_list - the head address of dma llp.
 * @return     none
 */
void audio_rx_dma_config(dma_chn_e chn, unsigned short *dst_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    audio_rx_dma_chn = chn;
    audio_set_rx_buff_len(audio_rx_fifo_chn, data_len);
    dma_config(chn, &audio_dma_rx_config[audio_rx_fifo_chn]);
    dma_set_address(chn, REG_AUDIO_FIFO_ADDR(audio_rx_fifo_chn), (unsigned int)(dst_addr));
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)(head_of_list);
}

/**
 * @brief      This function serves to set rx dma chain transfer.
 * @param[in]  config_addr - the head of list of llp_pointer.
 * @param[in]  llpointer   - the next element of llp_pointer.
 * @param[in]  dst_addr    - Pointer to data buffer, it must be 4-bytes aligned address and the actual buffer size defined by the user needs to
 *                           be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in]  data_len    - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @return     none
 */
void audio_rx_dma_add_list_element(dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned short *dst_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(audio_rx_dma_chn) | FLD_DMA_CHANNEL_ENABLE;
    config_addr->dma_chain_src_addr = REG_AUDIO_FIFO_ADDR(audio_rx_fifo_chn);
    config_addr->dma_chain_dst_addr = (unsigned int)(dst_addr);
    config_addr->dma_chain_data_len = dma_cal_size(data_len, 4);
    config_addr->dma_chain_llp_ptr  = (unsigned int)(llpointer);
}

/**
 * @brief      This function serves to set audio rx dma chain transfer.
 * @param[in]  rx_fifo_chn - rx fifo select.
 * @param[in]  chn         - dma channel.
 * @param[in]  in_buff     - Pointer to data buffer, it must be 4-bytes aligned address and the actual buffer size defined by the user needs to
 *                           be not smaller than the data_len, otherwise there may be an out-of-bounds problem.
 * @param[in]  buff_size   - Length of DMA in bytes, it must be set to a multiple of 4. The maximum value that can be set is 0x10000.
 * @return     none
 */
void audio_rx_dma_chain_init(audio_fifo_chn_e rx_fifo_chn, dma_chn_e chn, unsigned short *in_buff, unsigned int buff_size)
{
    audio_rx_fifo_chn = rx_fifo_chn;
    audio_rx_dma_config(chn, (unsigned short *)in_buff, buff_size, &g_audio_rx_dma_list_cfg[rx_fifo_chn]);
    audio_rx_dma_add_list_element(&g_audio_rx_dma_list_cfg[rx_fifo_chn], &g_audio_rx_dma_list_cfg[rx_fifo_chn], (unsigned short *)in_buff, buff_size);
}

/**
 * @brief      This function serves to config  tx_dma channel.
 * @param[in]  chn          - dma channel.
 * @param[in]  src_addr     - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in]  data_len     - Length of DMA in bytes, range from 1 to 0x10000.
 * @param[in]  head_of_list - the head address of dma llp.
 * @return     none
 */
void audio_tx_dma_config(dma_chn_e chn, unsigned short *src_addr, unsigned int data_len, dma_chain_config_t *head_of_list)
{
    audio_tx_dma_chn = chn;
    audio_set_tx_buff_len(audio_tx_fifo_chn, data_len);
    dma_config(chn, &audio_dma_tx_config[audio_tx_fifo_chn]);
    dma_set_address(chn, (unsigned int)(src_addr), REG_AUDIO_FIFO_ADDR(audio_tx_fifo_chn));
    dma_set_size(chn, data_len, DMA_WORD_WIDTH);
    reg_dma_llp(chn) = (unsigned int)head_of_list;
}

/**
 * @brief      This function serves to set tx dma chain transfer.
 * @param[in]  config_addr - the head of list of llp_pointer.
 * @param[in]  llpointer   - the next element of llp_pointer.
 * @param[in]  src_addr    - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in]  data_len    - Length of DMA in bytes, range from 1 to 0x10000.
 * @return     none
 */
void audio_tx_dma_add_list_element(dma_chain_config_t *config_addr, dma_chain_config_t *llpointer, unsigned short *src_addr, unsigned int data_len)
{
    config_addr->dma_chain_ctl      = reg_dma_ctrl(audio_tx_dma_chn) | FLD_DMA_CHANNEL_ENABLE;
    config_addr->dma_chain_src_addr = (unsigned int)src_addr;
    config_addr->dma_chain_dst_addr = REG_AUDIO_FIFO_ADDR(audio_tx_fifo_chn);
    config_addr->dma_chain_data_len = dma_cal_size(data_len, 4);
    config_addr->dma_chain_llp_ptr  = (unsigned int)llpointer;
}

/**
 * @brief      This function serves to initialize audio tx dma chain transfer.
 * @param[in]  tx_fifo_chn - tx fifo select.
 * @param[in]  chn         - dma channel.
 * @param[in]  out_buff    - Pointer to data buffer, it must be 4-bytes aligned address.
 * @param[in]  buff_size   - Length of DMA in bytes, range from 1 to 0x10000.
 * @return     none
 */
void audio_tx_dma_chain_init(audio_fifo_chn_e tx_fifo_chn, dma_chn_e chn, unsigned short *out_buff, unsigned int buff_size)
{
    audio_tx_fifo_chn = tx_fifo_chn;
    audio_tx_dma_config(chn, (unsigned short *)out_buff, buff_size, &g_audio_tx_dma_list_cfg[tx_fifo_chn]);
    audio_tx_dma_add_list_element(&g_audio_tx_dma_list_cfg[tx_fifo_chn], &g_audio_tx_dma_list_cfg[tx_fifo_chn], (unsigned short *)out_buff, buff_size);
}

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio hac interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio hac interface
 * @{
 */

/**
 * @brief      This function servers to update hac biquad filter coefficients.
 * 
 * @param[in]  hac_chn - hac channel.
 * @param[in]  biquad  - biquad step audio_hac_biquad_e.
 * @param[in]  data    - biquad filter data address, [b0, b1, b2, a1, a2].
 * @return     none
 */
void audio_hac_update_biquad_coef(audio_hac_chn_e hac_chn, audio_hac_biquad_e biquad, signed int *data)
{
    reg_audio_hac_bq_b0(hac_chn, biquad) = data[0];
    reg_audio_hac_bq_b1(hac_chn, biquad) = data[1];
    reg_audio_hac_bq_b2(hac_chn, biquad) = data[2];
    reg_audio_hac_bq_a1(hac_chn, biquad) = data[3];
    reg_audio_hac_bq_a2(hac_chn, biquad) = data[4];
}

/**
* @brief      This function servers to config eq bypass.
*
* @param[in]  hac_chn - hac channel.
* @return     none
*/
void audio_hac_bypass_eq(audio_hac_chn_e hac_chn)
{
    signed int biq[5] = {0x10000000, 0, 0, 0, 0}; /* This set of parameters is used to bypass the EQ. */

    //config EQ bypass
    for (int i = 0; i < 10; i++) {
        audio_hac_update_biquad_coef(hac_chn, i, biq);
    }
    //eq_config_en
    audio_hac_eq_config_en(hac_chn);
}

/**
 * @brief      This function servers to select hac's asrc input and output fs.
 *
 * @param[in]  hac_chn   - hac channel.
 * @param[in]  fs_in     - input fs.
 * @param[in]  fs_out    - output fs.
 * @param[in]  ppm       - ppm value.
 * @return     none
 * @note       support fs in and out:
 *              - IN_16K_OUT_16K
 *              - IN_32K_OUT_16K
 *              - IN_44P1K_OUT_16K
 *              - IN_48K_OUT_16K
 *              - IN_96K_OUT_16K
 *              - IN_16K_OUT_32K
 *              - IN_32K_OUT_32K
 *              - IN_44P1K_OUT_32K
 *              - IN_48K_OUT_32K
 *              - IN_96K_OUT_32K
 *              - IN_16K_OUT_44P1K
 *              - IN_32K_OUT_44P1K
 *              - IN_44P1K_OUT_44P1
 *              - IN_48K_OUT_44P1K
 *              - IN_96K_OUT_44P1K
 *              - IN_16K_OUT_48K
 *              - IN_32K_OUT_48K
 *              - IN_44P1K_OUT_48K
 *              - IN_48K_OUT_48K
 *              - IN_96K_OUT_48K
 *              - IN_16K_OUT_96K
 *              - IN_32K_OUT_96K
 *              - IN_44P1K_OUT_96K
 *              - IN_48K_OUT_96K
 *              - IN_96K_OUT_96K
 *              - IN_768K_OUT_96K
 *              - IN_192K_OUT_192K
 *              - IN_384K_OUT_384K
 *              - IN_768K_OUT_768K
 * 
 */
void audio_hac_asrc_fs_in_out(audio_hac_chn_e hac_chn, int fs_in, int fs_out, int ppm, int tdm_chn)
{
    int  frac_advance = 0;
    int  den_rate     = 0;
    char int_advance  = 0;

    if (fs_in == fs_out) {
        den_rate = 1000000;
        if (ppm <= 0) {
            frac_advance = 1000000 + ppm * 16;
            int_advance  = 15;
        } else {
            frac_advance = ppm * 16;
            int_advance  = 16;
        }
    } else if ((fs_in == 32000 && fs_out == 16000) || (fs_in == 96000 && fs_out == 48000)) {
        den_rate = 1000000;
        if (ppm <= 0) {
            frac_advance = 1000000 + ppm * 32;
            int_advance  = 31;
        } else {
            frac_advance = ppm * 32;
            int_advance  = 32;
        }
    }

    else if ((fs_in == 48000 && fs_out == 16000) || (fs_in == 96000 && fs_out == 32000)) {
        den_rate = 1000000;
        if (ppm <= 0) {
            frac_advance = 1000000 + ppm * 48;
            int_advance  = 47;
        } else {
            frac_advance = ppm * 48;
            int_advance  = 48;
        }
    }

    else if (fs_in == 96000 && fs_out == 16000) {
        den_rate = 1000000;
        if (ppm <= 0) {
            frac_advance = 1000000 + ppm * 96;
            int_advance  = 95;
        } else {
            frac_advance = ppm * 96;
            int_advance  = 96;
        }
    }

    else if ((fs_in == 16000 && fs_out == 32000) || (fs_in == 48000 && fs_out == 96000)) {
        den_rate = 1000000;
        if (ppm <= 0) {
            frac_advance = 1000000 + ppm * 8;
            int_advance  = 7;
        } else {
            frac_advance = ppm * 8;
            int_advance  = 8;
        }
    }

    else if (fs_in == 48000 && fs_out == 32000) {
        den_rate = 1000000;
        if (ppm <= 0) {
            frac_advance = 1000000 + ppm * 24;
            int_advance  = 23;
        } else {
            frac_advance = ppm * 24;
            int_advance  = 24;
        }
    }

    else if ((fs_in == 16000 && fs_out == 48000) || (fs_in == 32000 && fs_out == 96000)) {
        den_rate     = 3000000;
        frac_advance = 1000000 + ppm * 16;
        int_advance  = 5;
    }

    else if (fs_in == 32000 && fs_out == 48000) {
        den_rate     = 3000000;
        frac_advance = 2000000 + ppm * 32;
        int_advance  = 10;
    }

    else if (fs_in == 16000 && fs_out == 96000) {
        den_rate     = 3000000;
        frac_advance = 2000000 + ppm * 8;
        int_advance  = 2;
    }

    else if (fs_in == 44100 && fs_out == 16000) {
        den_rate     = 10000000;
        frac_advance = 1000000 + ppm * 441;
        int_advance  = 44;
    }

    else if (fs_in == 44100 && fs_out == 32000) {
        den_rate     = 20000000;
        frac_advance = 1000000 + ppm * 441;
        int_advance  = 22;
    }

    else if (fs_in == 44100 && fs_out == 48000) {
        den_rate     = 10000000;
        frac_advance = 7000000 + ppm * 147;
        int_advance  = 14;
    }

    else if (fs_in == 44100 && fs_out == 96000) {
        den_rate     = 20000000;
        frac_advance = 7000000 + ppm * 147;
        int_advance  = 7;
    }

    else if (fs_in == 16000 && fs_out == 44100) {
        den_rate     = 1378125; /* 459375 * 3 */
        frac_advance = 1109375 + ppm * 8;
        int_advance  = 5;
    }

    else if (fs_in == 32000 && fs_out == 44100) {
        den_rate     = 1378125; /* 459375 * 3 */
        frac_advance = 840625 + ppm * 16;
        int_advance  = 11;
    }

    else if (fs_in == 48000 && fs_out == 44100) {
        den_rate     = 459375;
        frac_advance = 190625 + ppm * 8;
        int_advance  = 17;
    }

    else if (fs_in == 96000 && fs_out == 44100) {
        den_rate     = 459375;
        frac_advance = 381250 + ppm * 16;
        int_advance  = 34;
    } else if (fs_in == 768000 && fs_out == 96000) {
        den_rate     = 1000000;
        frac_advance = 1000000;
        int_advance  = 127;
    }

    audio_hac_set_interval(hac_chn, fs_in, tdm_chn);
    audio_hac_set_frac_adc(hac_chn, frac_advance);
    audio_hac_set_den_rate(hac_chn, den_rate);
    audio_hac_set_int_adv(hac_chn, int_advance);
    audio_hac_lag_int_config_done(hac_chn);
}

/**
 * @brief      This function servers to set hac interval.
 *
 * @param[in]  hac_chn - hac channel.
 * @param[in]  in_fs   - sample rate.
 * @param[in]  tdm_chn - tdm channel.
 * @return     none
 */
void audio_hac_set_interval(audio_hac_chn_e hac_chn, int in_fs, int tdm_chn)
{
    if (tdm_chn == 4) {
        if (in_fs > 48000) {
            reg_audio_hac_asrc_interval = (reg_audio_hac_asrc_interval & (~(0x03 << ((hac_chn) * 2))));
        } else if (in_fs < 44100) {
            reg_audio_hac_asrc_interval = ((reg_audio_hac_asrc_interval & (~(0x03 << ((hac_chn) * 2)))) | (0x03 << ((hac_chn) * 2)));
        } else {
            reg_audio_hac_asrc_interval = ((reg_audio_hac_asrc_interval & (~(0x03 << ((hac_chn) * 2)))) | (0x02 << ((hac_chn) * 2)));
        }
    } else if (tdm_chn == 2) {
        if (in_fs > 96000) {
            reg_audio_hac_asrc_interval = (reg_audio_hac_asrc_interval & (~(0x03 << ((hac_chn) * 2))));
        } else if (in_fs < 96000) {
            reg_audio_hac_asrc_interval = ((reg_audio_hac_asrc_interval & (~(0x03 << ((hac_chn) * 2)))) | (0x03 << ((hac_chn) * 2)));
        } else {
            reg_audio_hac_asrc_interval = ((reg_audio_hac_asrc_interval & (~(0x03 << ((hac_chn) * 2)))) | (0x02 << ((hac_chn) * 2)));
        }
    } else {
        if (in_fs > 192000) {
            reg_audio_hac_asrc_interval = (reg_audio_hac_asrc_interval & (~(0x03 << ((hac_chn) * 2))));
        } else if (in_fs < 192000) {
            reg_audio_hac_asrc_interval = ((reg_audio_hac_asrc_interval & (~(0x03 << ((hac_chn) * 2)))) | (0x03 << ((hac_chn) * 2)));
        } else {
            reg_audio_hac_asrc_interval = ((reg_audio_hac_asrc_interval & (~(0x03 << ((hac_chn) * 2)))) | (0x02 << ((hac_chn) * 2)));
        }
    }
}

/**
 * @brief      This function serves to get hac tx fifo cnt.
 * 
 * @param[in]  hac_chn - hac channel.
 * @return     hac tx fifo cnt.
 */
unsigned int audio_hac_get_txfifo_cnt(audio_fifo_chn_e fifo_chn)
{
    unsigned int fifo_cnt = 0;

    BM_SET(reg_audio_hac_tx_fifo_cnt_ind(fifo_chn), FLD_HAC_TX_FIFO_INDICATE); /* Write 1 means software need to read txfifo_cnt */

    while (BM_IS_CLR(reg_audio_hac_tx_fifo_cnt_ind(fifo_chn), FLD_HAC_TX_FIFO_INDICATE))
        ;

    fifo_cnt = reg_audio_hac_tx_fifo_cnt_ind(fifo_chn) & FLD_HAC_TX_FIFO_CNT;
    //     while(1);
    BM_CLR(reg_audio_hac_tx_fifo_cnt_ind(fifo_chn), FLD_HAC_TX_FIFO_INDICATE); /* clear tx data done */

    return fifo_cnt;
}

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio I2S interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio I2S interface
 * @{
 */

/**
 * @brief      This function serves to config i2s line mode.
 * 
 * @param[in]  i2s_sel   - i2s select.
 * @param[in]  io_mode - line mode.
 * @return     none
 */
void audio_i2s_set_io_mode(i2s_select_e i2s_sel, i2s_io_mode_e io_mode)
{
    switch (io_mode) {
    case I2S_5_LINE_MODE:
        reg_audio_i2s_route(i2s_sel) = (reg_audio_i2s_route(i2s_sel) & (~FLD_I2S_MODE)) | I2S_5_LINE_MODE;
        break;
    case I2S_4_LINE_DAC_MODE:
        reg_audio_i2s_route(i2s_sel) = (reg_audio_i2s_route(i2s_sel) & (~FLD_I2S_MODE)) | I2S_4_LINE_DAC_MODE;
        break;
    case I2S_4_LINE_ADC_MODE:
        reg_audio_i2s_route(i2s_sel) = (reg_audio_i2s_route(i2s_sel) & (~FLD_I2S_MODE)) | I2S_4_LINE_ADC_MODE;
        break;
    case I2S_2_LANE_TX_MODE:
        BM_SET(reg_audio_i2s_cfg3(i2s_sel), FLD_I2S_TX_2LINE_EN);
        break;
    case I2S_2_LANE_RX_MODE:
        BM_SET(reg_audio_i2s_cfg3(i2s_sel), FLD_I2S_RX_2LINE_EN);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to config i2s0 interface, word length, and m/s.
 * @param[in]  i2s_sel      - i2s channel select
 * @param[in]  i2s_format   - interface protocol
 * @param[in]  wl           - audio data word length
 * @param[in]  m_s          - select i2s as master or slave
 * @param[in]  i2s_config_t - the ptr of i2s_config_t that configure i2s lr_clk phase and lr_clk swap.
 *  i2s_config_t->i2s_lr_clk_invert_select-lr_clk phase control(in RJ,LJ or i2s modes),in i2s mode(opposite phasing in  RJ,LJ mode), 0=right channel data when lr_clk high ,1=right channel data when lr_clk low.
 *                                                                                     in DSP mode(in DSP mode only), DSP mode A/B select,0=DSP mode A ,1=DSP mode B.
 *            i2s_config_t->i2s_data_invert_select - 0=left channel data left,1=right channel data left.
 * but data output channel will be inverted,you can also set i2s_config_t->i2s_data_invert_select=1 to recovery it.
 * @return    none
 */
void audio_i2s_config(i2s_select_e i2s_sel, i2s_mode_select_e i2s_format, i2s_wl_mode_e wl, i2s_m_s_mode_e m_s, i2s_invert_config_t *i2s_config_t)
{
    reg_audio_i2s_cfg1(i2s_sel) = (reg_audio_i2s_cfg1(i2s_sel) & (~(FLD_I2S_ADC_DCI_MS | FLD_I2S_DAC_DCI_MS))) |
                                  MASK_VAL(FLD_I2S_ADC_DCI_MS, m_s, FLD_I2S_DAC_DCI_MS, m_s);

    reg_audio_i2s_cfg2(i2s_sel) = (reg_audio_i2s_cfg2(i2s_sel) & (~(FLD_I2S_Wl | FLD_I2S_FORMAT))) |
                                  MASK_VAL(FLD_I2S_Wl, wl, FLD_I2S_FORMAT, i2s_format);

    reg_audio_i2s_cfg3(i2s_sel) =
        (reg_audio_i2s_cfg3(i2s_sel) & (~(FLD_I2S_LR_SWAP | FLD_I2S_LRP))) |
        MASK_VAL(FLD_I2S_LR_SWAP, i2s_config_t->i2s_data_invert_select, FLD_I2S_LRP, i2s_config_t->i2s_lr_clk_invert_select);
}

/**
 * @brief      This function serves to config tdm mode, word length, slot width, and master/slave.
 * 
 * @param[in]  tdm_mode       - tdm mode.
 * @param[in]  tdm_slot_width - tdm slow width.
 * @param[in]  rx_ch_num      - tdm rx channel num.
 * @param[in]  tx_ch_num      - tdm tx channel num.
 * @return     none
 */
void audio_i2s_tdm_config(i2s_tdm_mode_select_e tdm_mode, i2s_tdm_slot_width_e tdm_slot_width, unsigned char rx_ch_num, unsigned char tx_ch_num)
{
    reg_audio_i2s0_tdm_cfg = MASK_VAL(FLD_I2S_TDM_RX_CH_NUM, ((rx_ch_num - 1) >> 1), FLD_I2S_TDM_TX_CH_NUM, ((tx_ch_num - 1) >> 1), FLD_I2S_TDM_MODE, tdm_mode, FLD_I2S_TDM_SLOT, tdm_slot_width);
}

/**
 * @brief      This function serves to set sampling rate when i2s as master.
 * @param[in]  i2s_select - i2s channel select
 * @param[in]  i2s_clk_config                         i2s_clk_config[2]                   i2s_clk_config[3]-->lrclk_adc(sampling rate)
                                                             ||                                 ||
 *  audio_clk(36.864M default)------->div---->i2s_clk--->2 * div(div = 0, bypass)--->blck----->div
 *                                    ||                                                        ||
 *                     i2s_clk_config[0]/i2s_clk_config[1]                               i2s_clk_config[4]-->lrclk_dac (sampling rate)
 *
 *  For example: sampling rate = 16K, i2s_clk_config[5] = { 1, 3, 6, 64, 64 }, sampling rate = 36.864MHz * (1 / 3) / (2 * 6) / (64)  = 16KHz.
 * @return    none
 * @attention The default is from audio_clk 36.864M(default). If the pll is changed, the clk will be changed accordingly.
 */
void audio_i2s_set_clock(i2s_select_e i2s_select, unsigned short *i2s_clk_config)
{
    audio_i2s_set_clk(i2s_select, i2s_clk_config[0], i2s_clk_config[1]);
    audio_i2s_set_bclk(i2s_select, i2s_clk_config[2]);
    audio_i2s_set_lrclk(i2s_select, i2s_clk_config[3], i2s_clk_config[4]);
}

/**
 * @brief      This function serves to initialize configuration i2s.
 * @param[in]  i2s_config - the relevant configuration struct pointer @see audio_i2s_config_t.
 * @return     none
 */
void audio_i2s_config_init(audio_i2s_config_t *i2s_config)
{
    audio_i2s_set_pin(i2s_config->i2s_select, i2s_config->pin_config);
    if (i2s_config->master_slave_mode == I2S_AS_MASTER_EN) {
        audio_i2s_set_clock(i2s_config->i2s_select, i2s_config->sample_rate);
    }
    audio_i2s_set_io_mode(i2s_config->i2s_select, i2s_config->io_mode);
    audio_i2s_config(i2s_config->i2s_select, i2s_config->i2s_mode, i2s_config->data_width, i2s_config->master_slave_mode, &audio_i2s_invert_config[i2s_config->i2s_select]);

    if (i2s_config->i2s_mode == I2S_TDM_MODE && i2s_config->i2s_select == I2S0) {
        audio_i2s_tdm_config(i2s_config->tdm_mode, i2s_config->tdm_slot_width, (i2s_config->sample_rate[3] / (16 + i2s_config->tdm_slot_width * 8)), i2s_config->sample_rate[4] / (16 + i2s_config->tdm_slot_width * 8));
    }

    reg_audio_clk_en_0 |= BIT(1 + i2s_config->i2s_select);
    audio_i2s_clk_en(i2s_config->i2s_select);
}

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio matrix interface                                              *
 *********************************************************************************************************************/
/*!
 * @name Audio matrix interface
 * @{
 */

/**
 * @brief      This function serves to select fifo rx route source and data format.
 *
 * @param[in]  fifo_num    - fifo channel.
 * @param[in]  route_from  - fifo rx route from.
 * @param[in]  data_format - fifo rx data format(route from i2s/anc/adc/hac valid), others select FIFO_RX_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_rx_fifo_route(audio_fifo_chn_e fifo_num, audio_matrix_fifo_rx_route_e route_from, audio_matrix_fifo_rx_format_e data_format)
{
    reg_audio_matrix_fifo_wr_sel(fifo_num) = (reg_audio_matrix_fifo_wr_sel(fifo_num) & (~FLD_MATRIX_FIFO_WR_SEL)) | route_from;

    switch (route_from) {
    case FIFO_RX_ROUTE_I2S0_RX:
        reg_audio_matrix_i2s0_rx_sel = (reg_audio_matrix_i2s0_rx_sel & (~FLD_MATRIX_I2S0_RX_SEL)) | MASK_VAL(FLD_MATRIX_I2S0_RX_SEL, data_format);
        break;
    case FIFO_RX_ROUTE_I2S1_RX:
        reg_audio_matrix_i2s1_rx_sel = (reg_audio_matrix_i2s1_rx_sel & (~FLD_MATRIX_I2S1_RX_SEL)) | MASK_VAL(FLD_MATRIX_I2S1_RX_SEL, data_format);
        break;
    case FIFO_RX_ROUTE_I2S2_RX:
        reg_audio_matrix_i2s2_rx_sel = (reg_audio_matrix_i2s2_rx_sel & (~FLD_MATRIX_I2S2_RX_SEL)) | MASK_VAL(FLD_MATRIX_I2S2_RX_SEL, data_format);
        break;
    case FIFO_RX_ROUTE_ANC0:
        if (fifo_num < FIFO2) {
            reg_audio_matrix_anc0_rx_sel = (reg_audio_matrix_anc0_rx_sel & (~FLD_MATRIX_ANC0_RX_SEL)) | MASK_VAL(FLD_MATRIX_ANC0_RX_SEL, data_format);
        } else {
            reg_audio_matrix_anc0_rx_sel = (reg_audio_matrix_anc0_rx_sel & (~FLD_MATRIX_ANC0_RX_V2_SEL)) | MASK_VAL(FLD_MATRIX_ANC0_RX_V2_SEL, data_format);
        }
        break;
    case FIFO_RX_ROUTE_ANC1:
        if (fifo_num < FIFO2) {
            reg_audio_matrix_anc1_rx_sel = (reg_audio_matrix_anc1_rx_sel & (~FLD_MATRIX_ANC1_RX_SEL)) | MASK_VAL(FLD_MATRIX_ANC1_RX_SEL, data_format);
        } else {
            reg_audio_matrix_anc1_rx_sel = (reg_audio_matrix_anc1_rx_sel & (~FLD_MATRIX_ANC1_RX_V2_SEL)) | MASK_VAL(FLD_MATRIX_ANC1_RX_V2_SEL, data_format);
        }
        break;
    case FIFO_RX_ROUTE_CODEC0_ADCA:
        reg_audio_matrix_adc01_sel = (reg_audio_matrix_adc01_sel & (~FLD_MATRIX_ADC0_SEL)) | MASK_VAL(FLD_MATRIX_ADC0_SEL, data_format);
        break;
    case FIFO_RX_ROUTE_CODEC0_ADCB:
        reg_audio_matrix_adc01_sel = (reg_audio_matrix_adc01_sel & (~FLD_MATRIX_ADC1_SEL)) | MASK_VAL(FLD_MATRIX_ADC1_SEL, data_format);
        break;
    case FIFO_RX_ROUTE_CODEC1_ADCA:
        reg_audio_matrix_adc2_sel = (reg_audio_matrix_adc2_sel & (~FLD_MATRIX_ADC2_SEL)) | MASK_VAL(FLD_MATRIX_ADC2_SEL, data_format);
        break;
    case FIFO_RX_ROUTE_HAC_DATA0:
        reg_audio_matrix_fifo_rx_hac01_sel = ((reg_audio_matrix_fifo_rx_hac01_sel & (~FLD_MATRIX_FIFO_RX_HAC0_SEL)) | MASK_VAL(FLD_MATRIX_FIFO_RX_HAC0_SEL, data_format));
        break;
    case FIFO_RX_ROUTE_HAC_DATA1:
        reg_audio_matrix_fifo_rx_hac01_sel = ((reg_audio_matrix_fifo_rx_hac01_sel & (~FLD_MATRIX_FIFO_RX_HAC1_SEL)) | MASK_VAL(FLD_MATRIX_FIFO_RX_HAC1_SEL, data_format));
        break;
    case FIFO_RX_ROUTE_HAC_TDM0_DATA2:
        reg_audio_matrix_fifo_rx_hac23_sel = (reg_audio_matrix_fifo_rx_hac23_sel & (~FLD_MATRIX_FIFO_RX_HAC2_SEL)) | MASK_VAL(FLD_MATRIX_FIFO_RX_HAC2_SEL, data_format);
        break;
    case FIFO_RX_ROUTE_HAC_TDM1_DATA3:
        reg_audio_matrix_fifo_rx_hac23_sel = (reg_audio_matrix_fifo_rx_hac23_sel & (~FLD_MATRIX_FIFO_RX_HAC3_SEL)) | MASK_VAL(FLD_MATRIX_FIFO_RX_HAC3_SEL, data_format);
        break;
    case FIFO_RX_ROUTE_HAC_TDM01_DATA23:
        reg_audio_matrix_fifo_rx_hac23_sel = (reg_audio_matrix_fifo_rx_hac23_sel & (~FLD_MATRIX_FIFO_RX_HAC2_SEL)) | MASK_VAL(FLD_MATRIX_FIFO_RX_HAC2_SEL, data_format);
        reg_audio_matrix_fifo_rx_hac23_sel = (reg_audio_matrix_fifo_rx_hac23_sel & (~FLD_MATRIX_FIFO_RX_HAC3_SEL)) | MASK_VAL(FLD_MATRIX_FIFO_RX_HAC3_SEL, data_format);
        break;
    default:
        break;
    }
}

/**
 * @brief   This function serves to select i2s tx route source and data format.
 *
 * @param[in]  i2s_tx_chn  - i2s tx channel.
 * @param[in]  route_from  - i2s tx route from.
 * @param[in]  data_format - i2s tx data format(route from fifo/hac2/hac3 valid), others select I2S_TX_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_i2s_tx_route(audio_i2s_tx_chn_e i2s_tx_chn, audio_matrix_i2s_tx_route_e route_from, audio_matrix_i2s_tx_format_e data_format)
{
    /* matrix route select. */
    switch (i2s_tx_chn) {
    case I2S0_CHN0:
        reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN0) = (reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN0) & (~FLD_MATRIX_I2S0_EVEN_TX_SEL)) |
                                                     MASK_VAL(FLD_MATRIX_I2S0_EVEN_TX_SEL, route_from);
        break;
    case I2S0_CHN1:
        reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN1) = (reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN1) & (~FLD_MATRIX_I2S0_ODD_TX_SEL)) |
                                                     MASK_VAL(FLD_MATRIX_I2S0_ODD_TX_SEL, route_from);
        break;
    case I2S0_CHN2:
        reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN2) = (reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN2) & (~FLD_MATRIX_I2S0_EVEN_TX_SEL)) |
                                                     MASK_VAL(FLD_MATRIX_I2S0_EVEN_TX_SEL, route_from);
        break;
    case I2S0_CHN3:
        reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN3) = (reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN3) & (~FLD_MATRIX_I2S0_ODD_TX_SEL)) |
                                                     MASK_VAL(FLD_MATRIX_I2S0_ODD_TX_SEL, route_from);
        break;
    case I2S0_CHN4:
        reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN4) = (reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN4) & (~FLD_MATRIX_I2S0_EVEN_TX_SEL)) |
                                                     MASK_VAL(FLD_MATRIX_I2S0_EVEN_TX_SEL, route_from);
        break;
    case I2S0_CHN5:
        reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN5) = (reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN5) & (~FLD_MATRIX_I2S0_ODD_TX_SEL)) |
                                                     MASK_VAL(FLD_MATRIX_I2S0_ODD_TX_SEL, route_from);
        break;
    case I2S0_CHN6:
        reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN6) = (reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN6) & (~FLD_MATRIX_I2S0_EVEN_TX_SEL)) |
                                                     MASK_VAL(FLD_MATRIX_I2S0_EVEN_TX_SEL, route_from);
        break;
    case I2S0_CHN7:
        reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN7) = (reg_audio_matrix_i2s0_ch_tx_sel(I2S0_CHN7) & (~FLD_MATRIX_I2S0_ODD_TX_SEL)) |
                                                     MASK_VAL(FLD_MATRIX_I2S0_ODD_TX_SEL, route_from);
        break;
    case I2S1_CHN0:
        reg_audio_matrix_i2s1_ch01_tx_sel = (reg_audio_matrix_i2s1_ch01_tx_sel & (~FLD_MATRIX_I2S1_CH0_TX_SEL)) |
                                            MASK_VAL(FLD_MATRIX_I2S1_CH0_TX_SEL, route_from);
        break;
    case I2S1_CHN1:
        reg_audio_matrix_i2s1_ch01_tx_sel = (reg_audio_matrix_i2s1_ch01_tx_sel & (~FLD_MATRIX_I2S1_CH1_TX_SEL)) |
                                            MASK_VAL(FLD_MATRIX_I2S1_CH1_TX_SEL, route_from);
        break;
    case I2S2_CHN0:
        reg_audio_matrix_i2s2_ch01_tx_sel = (reg_audio_matrix_i2s2_ch01_tx_sel & (~FLD_MATRIX_I2S2_CH0_TX_SEL)) |
                                            MASK_VAL(FLD_MATRIX_I2S2_CH0_TX_SEL, route_from);
        break;
    case I2S2_CHN1:
        reg_audio_matrix_i2s2_ch01_tx_sel = (reg_audio_matrix_i2s2_ch01_tx_sel & (~FLD_MATRIX_I2S2_CH1_TX_SEL)) |
                                            MASK_VAL(FLD_MATRIX_I2S2_CH1_TX_SEL, route_from);
        break;
    default:
        break;
    }

    /* find i2s num. */
    unsigned char i2s_select = 0;
    if (i2s_tx_chn <= I2S0_CHN7) {
        i2s_select = I2S0;
    } else if ((i2s_tx_chn > I2S0_CHN7) && (i2s_tx_chn <= I2S1_CHN1)) {
        i2s_select = I2S1;
    } else {
        i2s_select = I2S2;
    }

    if (route_from == I2S_TX_ROUTE_FIFO) /* set i2s data format. */
    {
        switch (i2s_select) {
        case I2S0:
            reg_audio_matrix_i2s0_tx_dma_sel = (reg_audio_matrix_i2s0_tx_dma_sel & (~FLD_MATRIX_I2S0_TX_DMA_SEL)) | data_format;
            break;
        case I2S1:
            reg_audio_matrix_i2s1_tx_dma_sel = (reg_audio_matrix_i2s1_tx_dma_sel & (~FLD_MATRIX_I2S1_TX_DMA_SEL)) | data_format;
            break;
        case I2S2:
            reg_audio_matrix_i2s2_tx_dma_sel = (reg_audio_matrix_i2s2_tx_dma_sel & (~FLD_MATRIX_I2S2_TX_DMA_SEL)) | data_format;
            break;
        default:
            break;
        }
    } else if (route_from == I2S_TX_ROUTE_HAC_TDM0_DATA0123) /* set hac2 data format. */
    {
        unsigned char chn_mask = (i2s_tx_chn % 2) ? FLD_MATRIX_I2S_TX_HAC_ODD_SEL : FLD_MATRIX_I2S_TX_HAC_EVEN_SEL;

        reg_audio_matrix_i2s_tx_hac_sel(i2s_select, HAC_CHN2) =
            (reg_audio_matrix_i2s_tx_hac_sel(i2s_select, HAC_CHN2) & (~chn_mask)) |
            MASK_VAL(chn_mask, data_format);
    } else if (route_from == I2S_TX_ROUTE_HAC_TDM1_DATA0123) /* set hac3 data format. */
    {
        unsigned char chn_mask = (i2s_tx_chn % 2) ? FLD_MATRIX_I2S_TX_HAC_ODD_SEL : FLD_MATRIX_I2S_TX_HAC_EVEN_SEL;

        reg_audio_matrix_i2s_tx_hac_sel(i2s_select, HAC_CHN3) =
            (reg_audio_matrix_i2s_tx_hac_sel(i2s_select, HAC_CHN3) & (~chn_mask)) |
            MASK_VAL(chn_mask, data_format);
    } else if (route_from == I2S_TX_ROUTE_HAC_TDM01_DATA0123) /* set hac3 data format. */
    {
        unsigned char chn_mask = (i2s_tx_chn % 2) ? FLD_MATRIX_I2S_TX_HAC_ODD_SEL : FLD_MATRIX_I2S_TX_HAC_EVEN_SEL;

        reg_audio_matrix_i2s_tx_hac_sel(i2s_select, HAC_CHN2) =
            (reg_audio_matrix_i2s_tx_hac_sel(i2s_select, HAC_CHN2) & (~chn_mask)) |
            MASK_VAL(chn_mask, data_format);
        reg_audio_matrix_i2s_tx_hac_sel(i2s_select, HAC_CHN3) =
            (reg_audio_matrix_i2s_tx_hac_sel(i2s_select, HAC_CHN3) & (~chn_mask)) |
            MASK_VAL(chn_mask, data_format);
    }
}

/**
 * @brief      This function serves to select anc_src route source and format.
 *
 * @param[in]  anc_chn     - anc channel.
 * @param[in]  route_from  - anc_src route from.
 * @param[in]  data_format - anc src data format(route from fifo/i2s/adc valid), others select ANC_SRC_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_anc_src_route(audio_anc_chn_e anc_chn, audio_matrix_anc_src_route_e route_from, audio_matrix_anc_src_format_e data_format)
{
    reg_audio_matrix_anc_src_sel(anc_chn) = (reg_audio_matrix_anc_src_sel(anc_chn) & (~FLD_MATRIX_ANC_SRC_SEL)) | (route_from);
    if (anc_chn == ANC0) {
        switch (route_from) {
        case ANC_SRC_ROUTE_FIFO:
            reg_audio_matrix_anc_src_dma_sel(ANC0) = (reg_audio_matrix_anc_src_dma_sel(ANC0) & (~FLD_MATRIX_ANC_SRC_DMA_SEL)) |
                                                     MASK_VAL(FLD_MATRIX_ANC_SRC_DMA_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_I2S0_RX:
            reg_audio_matrix_i2s_rx_anc_sel_0(0) = (reg_audio_matrix_i2s_rx_anc_sel_0(0) & (~FLD_MATRIX_I2S_RX_ANC0_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC0_SRC_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_I2S1_RX:
            reg_audio_matrix_i2s_rx_anc_sel_0(1) = (reg_audio_matrix_i2s_rx_anc_sel_0(1) & (~FLD_MATRIX_I2S_RX_ANC0_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC0_SRC_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_I2S2_RX:
            reg_audio_matrix_i2s_rx_anc_sel_0(2) = (reg_audio_matrix_i2s_rx_anc_sel_0(2) & (~FLD_MATRIX_I2S_RX_ANC0_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC0_SRC_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_CODEC0_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_0(0) = (reg_audio_matrix_adc_rx_anc_sel_0(0) & (~FLD_MATRIX_ADC_RX_ANC0_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC0_SRC_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_CODEC0_ADCB:
            reg_audio_matrix_adc_rx_anc_sel_0(1) = (reg_audio_matrix_adc_rx_anc_sel_0(1) & (~FLD_MATRIX_ADC_RX_ANC0_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC0_SRC_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_CODEC1_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_0(2) = (reg_audio_matrix_adc_rx_anc_sel_0(2) & (~FLD_MATRIX_ADC_RX_ANC0_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC0_SRC_SEL, data_format);
            break;
        default:
            break;
        }
    } else {
        switch (route_from) {
        case ANC_SRC_ROUTE_FIFO:
            reg_audio_matrix_anc_src_dma_sel(ANC1) = (reg_audio_matrix_anc_src_dma_sel(ANC0) & (~FLD_MATRIX_ANC_SRC_DMA_SEL)) |
                                                     MASK_VAL(FLD_MATRIX_ANC_SRC_DMA_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_I2S0_RX:
            reg_audio_matrix_i2s_rx_anc_sel_1(0) = (reg_audio_matrix_i2s_rx_anc_sel_1(0) & (~FLD_MATRIX_I2S_RX_ANC1_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC1_SRC_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_I2S1_RX:
            reg_audio_matrix_i2s_rx_anc_sel_1(1) = (reg_audio_matrix_i2s_rx_anc_sel_1(1) & (~FLD_MATRIX_I2S_RX_ANC1_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC1_SRC_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_I2S2_RX:
            reg_audio_matrix_i2s_rx_anc_sel_1(2) = (reg_audio_matrix_i2s_rx_anc_sel_1(2) & (~FLD_MATRIX_I2S_RX_ANC1_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC1_SRC_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_CODEC0_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_1(0) = (reg_audio_matrix_adc_rx_anc_sel_1(0) & (~FLD_MATRIX_ADC_RX_ANC1_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC1_SRC_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_CODEC0_ADCB:
            reg_audio_matrix_adc_rx_anc_sel_1(1) = (reg_audio_matrix_adc_rx_anc_sel_1(1) & (~FLD_MATRIX_ADC_RX_ANC1_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC1_SRC_SEL, data_format);
            break;
        case ANC_SRC_ROUTE_CODEC1_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_1(2) = (reg_audio_matrix_adc_rx_anc_sel_1(2) & (~FLD_MATRIX_ADC_RX_ANC1_SRC_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC1_SRC_SEL, data_format);
            break;
        default:
            break;
        }
    }
}

/**
 * @brief      This function serves to select anc_src route source and format.
 *
 * @param[in]  anc_chn     - anc channel.
 * @param[in]  route_from  - anc_ref route from.
 * @param[in]  data_format - anc ref data format(route from i2s/adc valid), others select ANC_REF_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_anc_ref_route(audio_anc_chn_e anc_chn, audio_matrix_anc_ref_route_e route_from, audio_matrix_anc_ref_format_e data_format)
{
    reg_audio_matrix_anc_ref_sel(anc_chn) = (reg_audio_matrix_anc_ref_sel(anc_chn) & (~FLD_MATRIX_ANC_REF_SEL)) | (route_from);

    if (anc_chn == ANC0) {
        switch (route_from) {
        case ANC_REF_ROUTE_I2S0_RX:
            reg_audio_matrix_i2s_rx_anc_sel_0(0) = (reg_audio_matrix_i2s_rx_anc_sel_0(0) & (~FLD_MATRIX_I2S_RX_ANC0_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC0_REF_SEL, data_format);
            break;
        case ANC_REF_ROUTE_I2S1_RX:
            reg_audio_matrix_i2s_rx_anc_sel_0(1) = (reg_audio_matrix_i2s_rx_anc_sel_0(1) & (~FLD_MATRIX_I2S_RX_ANC0_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC0_REF_SEL, data_format);
            break;
        case ANC_REF_ROUTE_I2S2_RX:
            reg_audio_matrix_i2s_rx_anc_sel_0(2) = (reg_audio_matrix_i2s_rx_anc_sel_0(2) & (~FLD_MATRIX_I2S_RX_ANC0_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC0_REF_SEL, data_format);
            break;
        case ANC_REF_ROUTE_CODEC0_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_0(0) = (reg_audio_matrix_adc_rx_anc_sel_0(0) & (~FLD_MATRIX_ADC_RX_ANC0_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC0_REF_SEL, data_format);
            break;
        case ANC_REF_ROUTE_CODEC0_ADCB:
            reg_audio_matrix_adc_rx_anc_sel_0(1) = (reg_audio_matrix_adc_rx_anc_sel_0(1) & (~FLD_MATRIX_ADC_RX_ANC0_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC0_REF_SEL, data_format);
            break;
        case ANC_REF_ROUTE_CODEC1_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_0(2) = (reg_audio_matrix_adc_rx_anc_sel_0(2) & (~FLD_MATRIX_ADC_RX_ANC0_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC0_REF_SEL, data_format);
            break;
        default:
            break;
        }
    } else {
        switch (route_from) {
        case ANC_REF_ROUTE_I2S0_RX:
            reg_audio_matrix_i2s_rx_anc_sel_2(0) = (reg_audio_matrix_i2s_rx_anc_sel_2(0) & (~FLD_MATRIX_I2S_RX_ANC1_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC1_REF_SEL, data_format);
            break;
        case ANC_REF_ROUTE_I2S1_RX:
            reg_audio_matrix_i2s_rx_anc_sel_2(1) = (reg_audio_matrix_i2s_rx_anc_sel_2(1) & (~FLD_MATRIX_I2S_RX_ANC1_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC1_REF_SEL, data_format);
            break;
        case ANC_REF_ROUTE_I2S2_RX:
            reg_audio_matrix_i2s_rx_anc_sel_2(2) = (reg_audio_matrix_i2s_rx_anc_sel_2(2) & (~FLD_MATRIX_I2S_RX_ANC1_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC1_REF_SEL, data_format);
            break;
        case ANC_REF_ROUTE_CODEC0_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_2(0) = (reg_audio_matrix_adc_rx_anc_sel_2(0) & (~FLD_MATRIX_ADC_RX_ANC1_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC1_REF_SEL, data_format);
            break;
        case ANC_REF_ROUTE_CODEC0_ADCB:
            reg_audio_matrix_adc_rx_anc_sel_2(1) = (reg_audio_matrix_adc_rx_anc_sel_2(1) & (~FLD_MATRIX_ADC_RX_ANC1_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC1_REF_SEL, data_format);
            break;
        case ANC_REF_ROUTE_CODEC1_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_2(2) = (reg_audio_matrix_adc_rx_anc_sel_2(2) & (~FLD_MATRIX_ADC_RX_ANC1_REF_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC1_REF_SEL, data_format);
            break;
        default:
            break;
        }
    }
}

/**
 * @brief      This function serves to select anc_err route source and data format.
 *
 * @param[in]  anc_chn     - anc channel.
 * @param[in]  route_from  - anc_err route from.
 * @param[in]  data_format - anc err data format(route from i2s/adc valid), others select ANC_ERR_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_anc_err_route(audio_anc_chn_e anc_chn, audio_matrix_anc_err_route_e route_from, audio_matrix_anc_err_format_e data_format)
{
    reg_audio_matrix_anc_err_sel(anc_chn) = (reg_audio_matrix_anc_err_sel(anc_chn) & (~FLD_MATRIX_ANC_ERR_SEL)) | (route_from);
    if (anc_chn == ANC0) {
        switch (route_from) {
        case ANC_ERR_ROUTE_I2S0_RX:
            reg_audio_matrix_i2s_rx_anc_sel_1(0) = (reg_audio_matrix_i2s_rx_anc_sel_1(0) & (~FLD_MATRIX_I2S_RX_ANC0_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC0_ERR_SEL, data_format);
            break;
        case ANC_ERR_ROUTE_I2S1_RX:
            reg_audio_matrix_i2s_rx_anc_sel_1(1) = (reg_audio_matrix_i2s_rx_anc_sel_1(1) & (~FLD_MATRIX_I2S_RX_ANC0_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC0_ERR_SEL, data_format);
            break;
        case ANC_ERR_ROUTE_I2S2_RX:
            reg_audio_matrix_i2s_rx_anc_sel_1(2) = (reg_audio_matrix_i2s_rx_anc_sel_1(2) & (~FLD_MATRIX_I2S_RX_ANC0_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC0_ERR_SEL, data_format);
            break;
        case ANC_ERR_ROUTE_CODEC0_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_1(0) = (reg_audio_matrix_adc_rx_anc_sel_1(0) & (~FLD_MATRIX_ADC_RX_ANC0_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC0_ERR_SEL, data_format);
            break;
        case ANC_ERR_ROUTE_CODEC0_ADCB:
            reg_audio_matrix_adc_rx_anc_sel_1(1) = (reg_audio_matrix_adc_rx_anc_sel_1(1) & (~FLD_MATRIX_ADC_RX_ANC0_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC0_ERR_SEL, data_format);
            break;
        case ANC_ERR_ROUTE_CODEC1_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_1(2) = (reg_audio_matrix_adc_rx_anc_sel_1(2) & (~FLD_MATRIX_ADC_RX_ANC0_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC0_ERR_SEL, data_format);
            break;
        default:
            break;
        }
    } else {
        switch (route_from) {
        case ANC_ERR_ROUTE_I2S0_RX:
            reg_audio_matrix_i2s_rx_anc_sel_2(0) = (reg_audio_matrix_i2s_rx_anc_sel_2(0) & (~FLD_MATRIX_I2S_RX_ANC1_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC1_ERR_SEL, data_format);
            break;
        case ANC_ERR_ROUTE_I2S1_RX:
            reg_audio_matrix_i2s_rx_anc_sel_2(1) = (reg_audio_matrix_i2s_rx_anc_sel_2(1) & (~FLD_MATRIX_I2S_RX_ANC1_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC1_ERR_SEL, data_format);
            break;
        case ANC_ERR_ROUTE_I2S2_RX:
            reg_audio_matrix_i2s_rx_anc_sel_2(2) = (reg_audio_matrix_i2s_rx_anc_sel_2(2) & (~FLD_MATRIX_I2S_RX_ANC1_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_I2S_RX_ANC1_ERR_SEL, data_format);
            break;
        case ANC_ERR_ROUTE_CODEC0_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_2(0) = (reg_audio_matrix_adc_rx_anc_sel_2(0) & (~FLD_MATRIX_ADC_RX_ANC1_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC1_ERR_SEL, data_format);
            break;
        case ANC_ERR_ROUTE_CODEC0_ADCB:
            reg_audio_matrix_adc_rx_anc_sel_2(1) = (reg_audio_matrix_adc_rx_anc_sel_2(1) & (~FLD_MATRIX_ADC_RX_ANC1_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC1_ERR_SEL, data_format);
            break;
        case ANC_ERR_ROUTE_CODEC1_ADCA:
            reg_audio_matrix_adc_rx_anc_sel_2(2) = (reg_audio_matrix_adc_rx_anc_sel_2(2) & (~FLD_MATRIX_ADC_RX_ANC1_ERR_SEL)) |
                                                   MASK_VAL(FLD_MATRIX_ADC_RX_ANC1_ERR_SEL, data_format);
            break;
        default:
            break;
        }
    }
}

/**
 * @brief      This function serves to select dac route source and data format.
 *
 * @param[in]  dac_chn     - dac channel.
 * @param[in]  route_from  - dac route from.
 * @param[in]  data_format - dac data format(route from fifo valid), others select DAC_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_dac_route(audio_codec0_output_select_e dac_chn, audio_matrix_dac_route_e route_from, audio_matrix_dac_format_e data_format)
{
    switch (dac_chn) {
    case AUDIO_DAC_A1:
        reg_audio_matrix_dac_sel = (reg_audio_matrix_dac_sel & (~FLD_MATRIX_DAC_L_SEL)) | route_from;
        break;
    case AUDIO_DAC_A2:
        /* DAC_L_SEL should also be set and set the same value with DAC_R_SEL even only using DAC_A2 */
    case AUDIO_DAC_A1_A2:
        reg_audio_matrix_dac_sel = MASK_VAL(FLD_MATRIX_DAC_L_SEL, route_from, FLD_MATRIX_DAC_R_SEL, route_from);
        break;
    default:
        break;
    }

    if (route_from == DAC_ROUTE_FIFO) {
        reg_audio_matrix_dac_dma_sel = (reg_audio_matrix_dac_dma_sel & (~FLD_MATRIX_DAC_DMA_SEL)) | MASK_VAL(FLD_MATRIX_DAC_DMA_SEL, data_format);
    }
}

/**
 * @brief      This function serves to select hac route source and data format.
 *
 * @param[in]  hac_chn     - hac channel.
 * @param[in]  route_from  - hac route from.
 * @param[in]  data_format - hac data format(route from i2s/adc valid), others select HAC_DATA_FORMAT_INVALID.
 * @return     none
 */
void audio_matrix_set_hac_route(audio_hac_chn_e hac_chn, audio_matrix_hac_route_e route_from, audio_matrix_hac_format_e data_format)
{
    reg_audio_matrix_hac_tx_sel(hac_chn) = (reg_audio_matrix_hac_tx_sel(hac_chn) & (~FLD_MATRIX_HAC_TX_SEL)) | route_from;

    unsigned char chn_mask  = (hac_chn % 2) ? FLD_MATRIX_I2S_RX_HAC_ODD_SEL : FLD_MATRIX_I2S_RX_HAC_EVEN_SEL;
    unsigned char data_mask = 0x00;

    switch (route_from) {
    case HAC_DATA_ROUTE_FIFO0:
    case HAC_DATA_ROUTE_FIFO1:
    case HAC_DATA_ROUTE_FIFO2:
    case HAC_DATA_ROUTE_FIFO3:
        if (hac_chn < HAC_CHN2) {
            data_mask = (hac_chn % 2) ? FLD_MATRIX_HAC1_TX_DMAN_DATA_BITS_SEL : FLD_MATRIX_HAC0_TX_DMAN_DATA_BITS_SEL;
        } else {
            data_mask = (hac_chn % 2) ? FLD_MATRIX_HAC3_TX_DMAN_DATA_BITS_SEL : FLD_MATRIX_HAC2_TX_DMAN_DATA_BITS_SEL;
        }
        if (hac_chn < HAC_CHN2) {
            reg_audio_matrix_hac01_tx_dma_sel(route_from) = ((reg_audio_matrix_hac01_tx_dma_sel(route_from) & (~data_mask)) |
                                                             MASK_VAL(data_mask, data_format));
        } else {
            reg_audio_matrix_hac23_tx_dman_sel(route_from) = ((reg_audio_matrix_hac23_tx_dman_sel(route_from) & (~data_mask)) |
                                                              MASK_VAL(data_mask, data_format));
        }
        break;
    case HAC_DATA_ROUTE_I2S0_RX:
        reg_audio_matrix_i2s_rx_hac_sel(I2S0, hac_chn) = (reg_audio_matrix_i2s_rx_hac_sel(I2S0, hac_chn) & (~chn_mask)) |
                                                         MASK_VAL(chn_mask, data_format);
        break;
    case HAC_DATA_ROUTE_I2S1_RX:
        reg_audio_matrix_i2s_rx_hac_sel(I2S1, hac_chn) = (reg_audio_matrix_i2s_rx_hac_sel(I2S1, hac_chn) & (~chn_mask)) |
                                                         MASK_VAL(chn_mask, data_format);
        break;
    case HAC_DATA_ROUTE_I2S2_RX:
        reg_audio_matrix_i2s_rx_hac_sel(I2S2, hac_chn) = (reg_audio_matrix_i2s_rx_hac_sel(I2S2, hac_chn) & (~chn_mask)) |
                                                         MASK_VAL(chn_mask, data_format);
        break;
    case HAC_DATA_ROUTE_ADC0:
        reg_audio_matrix_adc_rx_hac_sel(0, hac_chn) = (reg_audio_matrix_adc_rx_hac_sel(0, hac_chn) & (~chn_mask)) | MASK_VAL(chn_mask, data_format);
        break;
    case HAC_DATA_ROUTE_ADC1:
        reg_audio_matrix_adc_rx_hac_sel(1, hac_chn) = (reg_audio_matrix_adc_rx_hac_sel(1, hac_chn) & (~chn_mask)) | MASK_VAL(chn_mask, data_format);
        break;
    case HAC_DATA_ROUTE_ADC2:
        reg_audio_matrix_adc_rx_hac_sel(2, hac_chn) = (reg_audio_matrix_adc_rx_hac_sel(2, hac_chn) & (~chn_mask)) | MASK_VAL(chn_mask, data_format);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function serves to select side_tone route source and data format.
 *
 * @param[in]  sd_chn      - side_tone channel.
 * @param[in]  route_from  - side_tone route from.
 * @param[in]  data_format - side tone data format(route from fifo/i2s/adc valid), others select SIDE_TONE_DATA_FORMAT_INVALID.
 * @return     none
 */

void audio_matrix_set_side_tone_route(audio_side_tone_chn_e sd_chn, audio_matrix_side_tone_route_e route_from,
                                  audio_matrix_side_tone_format_e data_format)
{
    reg_audio_matrix_sdtn_sel(sd_chn) = (reg_audio_matrix_sdtn_sel(sd_chn) & (~FLD_MATRIX_SDTN_SEL)) | route_from;

    switch (route_from) {
    case SIDE_TONE_ROUTE_FIFO:
        reg_audio_matrix_sdtn_dma_sel(sd_chn) = (reg_audio_matrix_sdtn_dma_sel(sd_chn) & (~FLD_MATRIX_SDTN_DMA_SEL)) |
                                                MASK_VAL(FLD_MATRIX_SDTN_DMA_SEL, data_format);
        break;
    case SIDE_TONE_ROUTE_I2S0_RX:
        reg_audio_matrix_i2s_sdtn_sel(sd_chn) = (reg_audio_matrix_i2s_sdtn_sel(sd_chn) & (~FLD_MATRIX_I2S0_SDTN_SEL)) |
                                                MASK_VAL(FLD_MATRIX_I2S0_SDTN_SEL, data_format);
        break;
    case SIDE_TONE_ROUTE_I2S1_RX:
        reg_audio_matrix_i2s_sdtn_sel(sd_chn) = (reg_audio_matrix_i2s_sdtn_sel(sd_chn) & (~FLD_MATRIX_I2S1_SDTN_SEL)) |
                                                MASK_VAL(FLD_MATRIX_I2S1_SDTN_SEL, data_format);
        break;
    case SIDE_TONE_ROUTE_I2S2_RX:
        reg_audio_matrix_i2s_adc_sdtn_sel(sd_chn) = (reg_audio_matrix_i2s_adc_sdtn_sel(sd_chn) & (~FLD_MATRIX_I2S2_SDTN_SEL)) |
                                                    MASK_VAL(FLD_MATRIX_I2S2_SDTN_SEL, data_format);
        break;
    case SIDE_TONE_ROUTE_ADC0:
        reg_audio_matrix_i2s_adc_sdtn_sel(sd_chn) = (reg_audio_matrix_i2s_adc_sdtn_sel(sd_chn) & (~FLD_MATRIX_ADC0_SDTN_SEL)) |
                                                    MASK_VAL(FLD_MATRIX_ADC0_SDTN_SEL, data_format);
        break;
    case SIDE_TONE_ROUTE_ADC1:
        reg_audio_matrix_adc_sdtn_sel(sd_chn) = (reg_audio_matrix_adc_sdtn_sel(sd_chn) & (~FLD_MATRIX_ADC1_SDTN_SEL)) |
                                                MASK_VAL(FLD_MATRIX_ADC1_SDTN_SEL, data_format);
        break;
    case SIDE_TONE_ROUTE_ADC2:
        reg_audio_matrix_adc_sdtn_sel(sd_chn) = (reg_audio_matrix_adc_sdtn_sel(sd_chn) & (~FLD_MATRIX_ADC2_SDTN_SEL)) |
                                                MASK_VAL(FLD_MATRIX_ADC2_SDTN_SEL, data_format);
        break;
    default:
        break;
    }
}

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Audio pin interface                                                 *
 *********************************************************************************************************************/
/*!
 * @name Audio pin interface
 * @{
 */

/**
 * @brief      This function configures codec0 stream0 dmic pin.
 * @param[in]  dmic0_data - the data of dmic pin
 * @param[in]  dmic0_clk1 - the clk1 of dmic pin
 * @param[in]  dmic0_clk2 - the clk2 of dmic pin,if need not set clk2, please set GPIO_NONE_PIN.
 * @return     none
 */
void audio_codec0_set_dmic_a_pin(gpio_func_pin_e dmic0_data, gpio_func_pin_e dmic0_clk1, gpio_func_pin_e dmic0_clk2)
{
    /* codec0 dmic0 data. */
    gpio_input_en((gpio_pin_e)dmic0_data);
    gpio_set_mux_function(dmic0_data, DMIC0_DAT_I);
    gpio_function_dis((gpio_pin_e)dmic0_data);
    /* codec0 dmic0 clock1. */
    gpio_set_mux_function(dmic0_clk1, DMIC0_CLK0);
    gpio_function_dis((gpio_pin_e)dmic0_clk1);
    /* codec0 dmic1 clock2. */
    if (dmic0_clk2 != GPIO_NONE_PIN) {
        gpio_set_mux_function(dmic0_clk2, DMIC0_CLK1);
        gpio_function_dis((gpio_pin_e)dmic0_clk2);
    }
}

/**
 * @brief      This function configures codec0 stream1 dmic pin.
 * @param[in]  dmic1_data - the data of dmic pin.
 * @param[in]  dmic1_clk1 - the clk1 of dmic pin.
 * @param[in]  dmic1_clk2 - the clk2 of dmic pin, if need not set clk2,please set GPIO_NONE_PIN.
 * @return     none
 */
void audio_codec0_set_dmic_b_pin(gpio_func_pin_e dmic1_data, gpio_func_pin_e dmic1_clk1, gpio_func_pin_e dmic1_clk2)
{
    /* codec0 dmic1 data. */
    gpio_input_en((gpio_pin_e)dmic1_data);
    gpio_set_mux_function(dmic1_data, DMIC1_DAT_I);
    gpio_function_dis((gpio_pin_e)dmic1_data);
    /* codec0 dmic1 clock1. */
    gpio_set_mux_function(dmic1_clk1, DMIC1_CLK0);
    gpio_function_dis((gpio_pin_e)dmic1_clk1);
    /* codec0 dmic1 clock2. */
    if (dmic1_clk2 != GPIO_NONE_PIN) {
        gpio_set_mux_function(dmic1_clk2, DMIC1_CLK1);
        gpio_function_dis((gpio_pin_e)dmic1_clk2);
    }
}

/**
 * @brief      This function configures codec1 stream0 dmic pin.
 * @param[in]  dmic2_data - the data of dmic pin.
 * @param[in]  dmic2_clk1 - the clk1 of dmic pin.
 * @param[in]  dmic2_clk2 - the clk2 of dmic pin, if need not set clk2,please set GPIO_NONE_PIN.
 * @return     none
 */
void audio_codec1_set_dmic_a_pin(gpio_func_pin_e dmic2_data, gpio_func_pin_e dmic2_clk1, gpio_func_pin_e dmic2_clk2)
{
    /* codec1 dmic2 data. */
    gpio_input_en((gpio_pin_e)dmic2_data);
    gpio_set_mux_function(dmic2_data, DMIC2_DAT_I);
    gpio_function_dis((gpio_pin_e)dmic2_data);
    /* codec1 dmic2 clock1. */
    gpio_set_mux_function(dmic2_clk1, DMIC2_CLK0);
    gpio_function_dis((gpio_pin_e)dmic2_clk1);
    /* codec1 dmic2 clock2. */
    if (dmic2_clk2 != GPIO_NONE_PIN) {
        gpio_set_mux_function(dmic2_clk2, DMIC2_CLK1);
        gpio_function_dis((gpio_pin_e)dmic2_clk2);
    }
}

/**
 * @brief      This function serves to configure i2s pin.
 * @param[in]  i2s_select - channel select.
 * @param[in]  config     - i2s config pin struct.
 * @return     none
 */
void audio_i2s_set_pin(i2s_select_e i2s_select, i2s_pin_config_t *config)
{
    gpio_input_en((gpio_pin_e)config->bclk_pin);
    gpio_set_mux_function((gpio_func_pin_e)config->bclk_pin, I2S0_BCK_IO + i2s_select * 6);
    gpio_function_dis((gpio_pin_e)config->bclk_pin);

    if (config->adc_lr_clk_pin != GPIO_NONE_PIN) {
        gpio_input_en((gpio_pin_e)config->adc_lr_clk_pin);
        gpio_set_mux_function((gpio_func_pin_e)config->adc_lr_clk_pin, I2S0_LR0_IO + i2s_select * 6);
        gpio_function_dis((gpio_pin_e)config->adc_lr_clk_pin);
    }

    if (config->adc_dat_pin != GPIO_NONE_PIN) {
        gpio_input_en((gpio_pin_e)config->adc_dat_pin);
        gpio_set_mux_function((gpio_func_pin_e)config->adc_dat_pin, I2S0_DAT0_IO + i2s_select * 6);
        gpio_function_dis((gpio_pin_e)config->adc_dat_pin);
    }

    if (config->dac_lr_clk_pin != GPIO_NONE_PIN) {
        gpio_input_en((gpio_pin_e)config->dac_lr_clk_pin);
        gpio_set_mux_function((gpio_func_pin_e)config->dac_lr_clk_pin, I2S0_LR1_IO + i2s_select * 6);
        gpio_function_dis((gpio_pin_e)config->dac_lr_clk_pin);
    }

    if (config->dac_dat_pin != GPIO_NONE_PIN) {
        gpio_input_en((gpio_pin_e)config->dac_dat_pin);
        gpio_set_mux_function((gpio_func_pin_e)config->dac_dat_pin, I2S0_DAT1_IO + i2s_select * 6);
        gpio_function_dis((gpio_pin_e)config->dac_dat_pin);
    }
}

/**
 * @brief      This function serves to configure spdif pin.
 * @param[in]  config     - spdif config pin struct.
 * @return     none
 */
void audio_spdif_set_pin(spdif_pin_config_t *config)
{
    if (config->spdif_rx_pin != GPIO_NONE_PIN) {
        gpio_input_en((gpio_pin_e)config->spdif_rx_pin);
        gpio_set_mux_function((gpio_func_pin_e)config->spdif_rx_pin, SPDIF_RX);
        gpio_function_dis((gpio_pin_e)config->spdif_rx_pin);
    }

    if (config->spdif_tx_pin != GPIO_NONE_PIN) {
        gpio_input_en((gpio_pin_e)config->spdif_tx_pin);
        gpio_set_mux_function((gpio_func_pin_e)config->spdif_tx_pin, SPDIF_TX);
        gpio_function_dis((gpio_pin_e)config->spdif_tx_pin);
    }
}

/**********************************************************************************************************************
 *                                                Audio spdif interface                                               *
 *********************************************************************************************************************/
/*!
 * @name Audio spdif interface
 * @{
 */
/**
 * @brief      This function serves to set spdif rx fs.
 * @param[in]  clk - audio pll clock
 * @return     none
 * @note       only support PLL_AUDIO_CLK_169P344M and PLL_AUDIO_CLK_147P456M
 */
void audio_spdif_set_rx_fs(pll_audio_clk_e clk)
{
    if (clk == PLL_AUDIO_CLK_169P344M)
    {
        reg_audio_spdif_fs_192_min1= 3;
        reg_audio_spdif_fs_192_min2= 10;
        reg_audio_spdif_fs_192_min3= 17;
        reg_audio_spdif_fs_192_max1= 9;
        reg_audio_spdif_fs_192_max2= 16;
        reg_audio_spdif_fs_192_max3= 23;

        reg_audio_spdif_fs_96_min1 = 10;
        reg_audio_spdif_fs_96_min2 = 24;
        reg_audio_spdif_fs_96_min3 = 38;
        reg_audio_spdif_fs_96_max1 = 16;
        reg_audio_spdif_fs_96_max2 = 30;
        reg_audio_spdif_fs_96_max3 = 44;

        reg_audio_spdif_fs_48_min1 = 24;
        reg_audio_spdif_fs_48_min2 = 52;
        reg_audio_spdif_fs_48_min3 = 79;
        reg_audio_spdif_fs_48_max1 = 30;
        reg_audio_spdif_fs_48_max2 = 58;
        reg_audio_spdif_fs_48_max3 = 85;

        reg_audio_spdif_fs_44p1_min1= 27;
        reg_audio_spdif_fs_44p1_min2= 57;
        reg_audio_spdif_fs_44p1_min3= 87;
        reg_audio_spdif_fs_44p1_max1= 33;
        reg_audio_spdif_fs_44p1_max2= 63;
        reg_audio_spdif_fs_44p1_max3= 93;

        reg_audio_spdif_fs_32_min1 = 38;
        reg_audio_spdif_fs_32_min2 = 79;
        reg_audio_spdif_fs_32_min3 = 121;
        reg_audio_spdif_fs_32_max1 = 44;
        reg_audio_spdif_fs_32_max2 = 85;
        reg_audio_spdif_fs_32_max3 = 127;
    }
    else if (clk == PLL_AUDIO_CLK_147P456M)
    {
        reg_audio_spdif_fs_192_min1= 3;
        reg_audio_spdif_fs_192_min2= 9;
        reg_audio_spdif_fs_192_min3= 15;
        reg_audio_spdif_fs_192_max1= 9;
        reg_audio_spdif_fs_192_max2= 15;
        reg_audio_spdif_fs_192_max3= 21;

        reg_audio_spdif_fs_96_min1 = 9;
        reg_audio_spdif_fs_96_min2 = 21;
        reg_audio_spdif_fs_96_min3 = 33;
        reg_audio_spdif_fs_96_max1 = 15;
        reg_audio_spdif_fs_96_max2 = 27;
        reg_audio_spdif_fs_96_max3 = 39;

        reg_audio_spdif_fs_48_min1 = 21;
        reg_audio_spdif_fs_48_min2 = 45;
        reg_audio_spdif_fs_48_min3 = 69;
        reg_audio_spdif_fs_48_max1 = 27;
        reg_audio_spdif_fs_48_max2 = 51;
        reg_audio_spdif_fs_48_max3 = 75;

        reg_audio_spdif_fs_44p1_min1= 23;
        reg_audio_spdif_fs_44p1_min2= 49;
        reg_audio_spdif_fs_44p1_min3= 75;
        reg_audio_spdif_fs_44p1_max1= 29;
        reg_audio_spdif_fs_44p1_max2= 55;
        reg_audio_spdif_fs_44p1_max3= 81;

        reg_audio_spdif_fs_32_min1 = 33;
        reg_audio_spdif_fs_32_min2 = 69;
        reg_audio_spdif_fs_32_min3 = 105;
        reg_audio_spdif_fs_32_max1 = 39;
        reg_audio_spdif_fs_32_max2 = 75;
        reg_audio_spdif_fs_32_max3 = 111;
    }
}
/**
 * @}
 */
