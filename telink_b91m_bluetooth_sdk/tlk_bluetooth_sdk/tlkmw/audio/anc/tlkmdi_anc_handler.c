/********************************************************************************************************
 * @file    tlkmdi_anc_handler.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
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
#include "tl_common.h"
#include "drivers.h"
#include "tlkmw/tlkmw.h"
#include "tlkmdi_anc_handler.h"

#if TLKALG_ANC_ENABLE
uint8_t anc_tool_cmd_flag = 0;
tlkmdi_anc_ctl g_anc_ctr = {0};

/**
 * @brief       Initialize ANC IRQ
 * @param[in]   byte_num - Byte number for buffer threshold
 * @return      None
 */
void tlkmdi_anc_irq_init(uint16_t byte_num)
{
    // plic_interrupt_enable(IRQ_DFIFO);

    audio_fifo_ptr_en(AUDIO_TX_FIFO0 + TLKDRV_ANC0_SPK_FIFO);
    audio_set_tx_rptr(TLKDRV_ANC0_SPK_FIFO, 0);
    audio_set_tx_buff_thres(TLKDRV_ANC0_SPK_FIFO, byte_num);
    audio_fifo_irq_en(AUDIO_TX_FIFO0 + TLKDRV_ANC0_SPK_FIFO);//enable TX_FIFO0

    // audio_fifo_ptr_en(AUDIO_RX_FIFO0 + TLKDRV_ANC0_MIC_FIFO);
    // audio_set_rx_wptr(TLKDRV_ANC0_MIC_FIFO, 0);
    // audio_set_rx_buff_thres(TLKDRV_ANC0_MIC_FIFO, byte_num);
    // audio_fifo_irq_en(AUDIO_RX_FIFO0 + TLKDRV_ANC0_MIC_FIFO);//enable RX_FIFO0
}

/**
 * @brief       Deinitialize ANC IRQ
 * @param       None
 * @return      None
 */
void tlkmdi_anc_irq_deinit(void)
{
    audio_fifo_ptr_dis(AUDIO_TX_FIFO0 + TLKDRV_ANC0_SPK_FIFO);
    audio_fifo_irq_dis(AUDIO_TX_FIFO0 + TLKDRV_ANC0_SPK_FIFO);

    // audio_fifo_ptr_dis(AUDIO_RX_FIFO0 + TLKDRV_ANC0_MIC_FIFO);
    // audio_fifo_irq_dis(AUDIO_RX_FIFO0 + TLKDRV_ANC0_MIC_FIFO);
}

//void tlkmdi_anc_music_sync_dsp_samples(uint16_t samples)
//{
//    audio_tx_dma_dis(TLKDRV_ANC0_SPK_DMA);
//    audio_rx_dma_dis(TLKDRV_ANC0_MIC_DMA);
//
//    #if TX_FIFO_IRQ_ENABLE
//    audio_fifo_irq_dis(AUDIO_TX_FIFO0 + TLKDRV_ANC0_SPK_FIFO);
//    #else
//    audio_fifo_irq_dis(AUDIO_RX_FIFO0 + TLKDRV_ANC0_MIC_FIFO);
//    #endif
//
//    g_codec_spk_buff[MID_BUFF_WPTR_INDEX] = (g_codec_spk_buff[MID_BUFF_RPTR_INDEX] + samples) % CODEC_SPK_MID_LEN;
//
//    tlkmdi_anc_irq_init(ANC_FRAME_SIZE * sizeof(adc_mono_int));
//
//    audio_tx_dma_en(TLKDRV_ANC0_SPK_DMA);
//    audio_rx_dma_en(TLKDRV_ANC0_MIC_DMA);
//}

/**
 * @brief       Set parameters to DSP
 * @param[in]   type - Parameter type
 * @param[in]   data - Pointer to parameter data
 * @param[in]   len - Length of parameter data
 * @return      None
 */
void tlkmdi_anc_set_para_to_dsp(uint8_t type, uint8_t *data, uint16_t len)
{
    tlkapi_printf(APP_LOG_EN, "tlkmdi_anc_set_para_to_dsp type %d, data %d", type, *data);
    set_param_t set_param;
    uint8_t *p_mcu_2_dsp_buff = d25f_get_enc_buff_ptr(IPC_SET_PARAM_PATH_2);
    audio_buffer_context_t *p_audio_buf_ctx = get_audio_buff_context_ptr(IPC_SET_PARAM_PATH_2);

    p_audio_buf_ctx->enc_audio_data_len = len;

    p_mcu_2_dsp_buff[0] = type;
    for (int i = 0; i < len; i++) {
        p_mcu_2_dsp_buff[i+1] = *data++;
    }
    
    set_param.buff_wptr = p_audio_buf_ctx->enc_audio_buff_wptr;
    set_param.data_len = p_audio_buf_ctx->enc_audio_data_len;
    set_param.para_type = ANC_PARA;

    d25f_update_enc_buff_wptr(IPC_SET_PARAM_PATH_2);
    d25f_send_request_to_dsp(IPC_D25F2DSP_SET_PARAM, (uint8_t *)&set_param, IPC_MSG_PAYLOAD_LEN);
}

/**
 * @brief       Switch ANC adaptive mode
 * @param[in]   adapt_flag - Adaptive mode flag
 * @return      None
 */
void d25f_switch_anc_adaptive_mode(uint8_t adapt_flag)
{
    tlkmdi_anc_set_para_to_dsp(ADAPTIVE_EN, &adapt_flag, 1);
}

/**
 * @brief       Set DSP ANC downlink enable
 * @param[in]   enable - Enable flag
 * @return      None
 */
void d25f_set_dsp_anc_downlink_enable(uint8_t enable)
{
    // if (enable) {
    //     tlkmdi_anc_pausing();
    // } else {
    //     g_anc_ctr.dsp_sync_flag = false;
    // }
    
    tlkmdi_anc_set_para_to_dsp(ANC_MUSIC_EN, &enable, 1);
}

/**
 * @brief       Set DSP ANC enable
 * @param[in]   enable - Enable flag
 * @return      None
 */
void d25f_set_dsp_anc_enable(uint8_t enable)
{
    tlkmdi_anc_set_para_to_dsp(ANC_EN, &enable, 1);
}

/**
 * @brief       Enable DSP ANC sync
 * @param       None
 * @return      None
 */
void d25f_set_dsp_anc_sync_enable(void)
{
    uint8_t enable = 1;
    tlkmdi_anc_set_para_to_dsp(ANC_SYNC, &enable, 1);
}

/**
 * @brief       Check if DSP is synchronized
 * @param       None
 * @return      true if synchronized, false otherwise
 */
bool tlkmdi_anc_dsp_is_sync_data(void)
{
    return g_anc_ctr.dsp_sync_flag;
}

/**
 * @brief       Request to close DSP
 * @param       None
 * @return      None
 */
void tlkmdi_anc_request_close_dsp(void)
{
    if (!g_anc_ctr.dsp_dis_req_flag) {
        g_anc_ctr.dsp_dis_req_flag = true;
    }
}

/**
 * @brief       Pause ANC processing
 * @param       None
 * @return      None
 */
void tlkmdi_anc_pausing(void)
{
    audio_tx_dma_dis(TLKDRV_ANC0_SPK_DMA);
    audio_rx_dma_dis(TLKDRV_ANC0_MIC_DMA);
    audio_clr_fifo_irq_status(AUDIO_TX_FIFO0 + TLKDRV_ANC0_SPK_FIFO);
    audio_set_tx_rptr(TLKDRV_ANC0_SPK_FIFO, 0);
}

/**
 * @brief       Enable DMA for ANC
 * @param       None
 * @return      None
 */
void tlkmdi_anc_enable_dma(void)
{
    audio_tx_dma_en(TLKDRV_ANC0_SPK_DMA);
    audio_rx_dma_en(TLKDRV_ANC0_MIC_DMA);
}

/**
 * @brief       Process ANC mode parameters
 * @param[in]   data - Pointer to parameter data
 * @return      None
 */
void tlkalg_anc_proc_mode_para(int *data)
{
    tlkapi_printf(APP_LOG_EN, "tlkalg_anc_proc_mode_para 0x%x", *data);
    if (g_anc_ctr.dsp_dis_req_flag) {
        g_anc_ctr.dsp_dis_req_flag = false;
        // tlkmw_dsp_pause();
    }

    switch (data[0]) {
        case(ADAPTIVE_EN):

        break;
        case(ANC_EN):
            
        break;
        case(ANC_MUSIC_EN):
            
        break;
        case(ANC_SYNC):
            g_anc_ctr.dsp_sync_flag = true;
            tlkmdi_anc_enable_dma();
        break;
        default:
        break;       
    }
}

/**
 * @brief       Load ANC parameters
 * @param[in]   data - Pointer to parameter data
 * @return      None
 */
void tlkalg_anc_load_para(uint8_t* data)
{
#if TLK_MW_DSP_COMM_ENABLE
	anc_reg_para_t *pdata = (anc_reg_para_t*)data;
    
//	audio_anc_mode_sel(AUDIO_ANC_CHANNEL, pdata->alg_mode);//0-FF	1-FB	2-HB	0x801428e2
    // tlkapi_printf(APP_LOG_EN, "alg mode %d, cmd_type %d", pdata->alg_mode, pdata->cmd_type);
    
	switch (pdata->alg_mode) {
        case ANC_MODE_FF:
        {
            audio_anc_latch_wz_fir_gain(AUDIO_ANC_CHANNEL);
            audio_anc_latch_ref_mic_gain(AUDIO_ANC_CHANNEL);
        
            break;
        }
        case ANC_MODE_FB:
        {
            audio_anc_latch_wz_fir_gain(AUDIO_ANC_CHANNEL);
            break;
        }
        case ANC_MODE_HB:
        {
            audio_anc_latch_wz_fir_gain(AUDIO_ANC_CHANNEL);
            audio_anc_latch_ref_mic_gain(AUDIO_ANC_CHANNEL);
            break;
        }
        default:
        break;
	}
    

	uint8_t p_cz_num = (pdata->filter_mode == FILTER_CZ0) ? 0 : (pdata->filter_mode == FILTER_CZ1 ? 1 : 2);

	if (anc_tool_cmd_flag == 1) {//single param by anc tool
		if (pdata->cmd_type == CMD_DOWNLOAD_IIR) {
			if (pdata->filter_mode == FILTER_WZ) {
				audio_anc_set_wz_iir_taps(AUDIO_ANC_CHANNEL, pdata->filter_para.wz_stages);
				audio_anc_update_wz_iir_coef(AUDIO_ANC_CHANNEL, pdata->filter_para.wz_iir_para, pdata->filter_para.wz_stages);
			} else {
			    audio_anc_set_cz_iir_taps(AUDIO_ANC_CHANNEL, p_cz_num, pdata->filter_para.cz_stages[p_cz_num]);
			    audio_anc_update_cz_iir_coef(AUDIO_ANC_CHANNEL, p_cz_num, pdata->filter_para.cz_iir_para, pdata->filter_para.cz_stages[p_cz_num]);
			}
		} else if (pdata->cmd_type == CMD_DOWNLOAD_FIR) {
			if (pdata->filter_mode == FILTER_WZ) {
				audio_anc_set_wz_fir_taps(AUDIO_ANC_CHANNEL, 390);
				audio_anc_update_wz_fir_coef(AUDIO_ANC_CHANNEL, pdata->filter_para.wz_fir_para, 390);
			} else {
				audio_anc_set_cz_fir_taps(AUDIO_ANC_CHANNEL, p_cz_num, 150);
			    audio_anc_update_cz_fir_coef(AUDIO_ANC_CHANNEL, p_cz_num, pdata->filter_para.cz_fir_para, 150);
			}
		} else if (pdata->cmd_type == CMD_DOWNLOAD_GAIN) {
			audio_anc_set_ref_mic_gain(AUDIO_ANC_CHANNEL, pdata->gain_para.ref_gain, pdata->gain_para.ref_shift);
			audio_anc_set_wz_fir_gain(AUDIO_ANC_CHANNEL, pdata->gain_para.wz_gain, pdata->gain_para.wz_shift);
		}
	} else {//dsp param total
		if (pdata->filter_mode & FILTER_WZ) {
			if (pdata->cmd_type & CMD_DOWNLOAD_IIR) {
				audio_anc_set_wz_iir_taps(AUDIO_ANC_CHANNEL, pdata->filter_para.wz_stages);
				audio_anc_update_wz_iir_coef(AUDIO_ANC_CHANNEL, pdata->filter_para.wz_iir_para, pdata->filter_para.wz_stages);
			}
			if (pdata->cmd_type & CMD_DOWNLOAD_FIR) {
				audio_anc_set_wz_fir_taps(AUDIO_ANC_CHANNEL, 390);
				audio_anc_update_wz_fir_coef(AUDIO_ANC_CHANNEL, pdata->filter_para.wz_fir_para, 390);
			}
		}

		if (pdata->filter_mode & FILTER_CZ0) {
			if (pdata->cmd_type & CMD_DOWNLOAD_IIR) {
			    audio_anc_set_cz_iir_taps(AUDIO_ANC_CHANNEL, p_cz_num,  pdata->filter_para.cz_stages[p_cz_num]);
			    audio_anc_update_cz_iir_coef(AUDIO_ANC_CHANNEL, p_cz_num, pdata->filter_para.cz_iir_para, pdata->filter_para.cz_stages[p_cz_num]);
			}
			if (pdata->cmd_type & CMD_DOWNLOAD_FIR) {
			    audio_anc_set_cz_fir_taps(AUDIO_ANC_CHANNEL, p_cz_num, 150);
			    audio_anc_update_cz_fir_coef(AUDIO_ANC_CHANNEL, p_cz_num, pdata->filter_para.cz_fir_para, 150);//wz_fir390_bypass
			}
		}

		if (pdata->cmd_type & CMD_DOWNLOAD_GAIN) {
            // tlkapi_printf(APP_LOG_EN, "anc set para gain ref_gain %d, wz_gain %d", pdata->gain_para.ref_gain, pdata->gain_para.wz_gain);
            audio_anc_set_ref_mic_gain(AUDIO_ANC_CHANNEL, pdata->gain_para.ref_gain, pdata->gain_para.ref_shift);
            audio_anc_set_wz_fir_gain(AUDIO_ANC_CHANNEL, pdata->gain_para.wz_gain, pdata->gain_para.wz_shift);
		}
        
	}
#else
    (void)data;
#endif
}

#endif
