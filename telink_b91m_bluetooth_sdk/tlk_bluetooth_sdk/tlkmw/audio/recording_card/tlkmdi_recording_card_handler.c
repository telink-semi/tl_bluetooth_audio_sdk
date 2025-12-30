/********************************************************************************************************
 * @file    tlkmdi_recording_card_handler.c
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
#include "tlkmdi_recording_card_handler.h"

#if TLKMW_RECORDING_CARD_EN
#if TLKALG_OPUS_ENC_ENABLE
uint8_t *s_recording_card_opus_enc_ptr = NULL;
#endif
#if TLKALG_OPUS_DEC_ENABLE
uint8_t *s_recording_card_opus_dec_ptr = NULL;
#endif

#if TLKALG_AGC_ENABLE
uint8_t *g_alg_rc_cord_agc_buff = NULL;
#endif

tlkalg_recording_card_fill_enc_data_t tlkalg_recording_card_fill_enc_data_cb = NULL;
tlkalg_recording_card_fill_pcm_data_t tlkalg_recording_card_fill_pcm_data_cb = NULL;
extern tlkmdi_recording_card_t        s_recording_card_env;

/**
 * @brief       This function initializes the recording card algorithm.
 * @param       none.
 * @return      none.
 */
void tlkmdi_recording_card_alg_init(void)
{
#if TLKALG_OPUS_ENC_ENABLE
    if (s_recording_card_opus_enc_ptr == NULL) {
        audio_alg_interface_t *p_alg_enc     = audio_alg_get_interface_by_type(ALG_OPUS_ENC);
        uint16_t               opus_enc_size = p_alg_enc->audio_alg_get_size(ALG_CHANNEL_LEFT);
        s_recording_card_opus_enc_ptr        = (uint8_t *)tlkalg_malloc_func(opus_enc_size);
        if (s_recording_card_opus_enc_ptr == NULL) {
            tlkapi_printf(APP_LOG_EN, "opus enc alloc failed");
            return;
        }
        p_alg_enc->audio_alg_init(s_recording_card_opus_enc_ptr, ALG_CHANNEL_LEFT);
    }
#endif
#if TLKALG_OPUS_DEC_ENABLE
    if (s_recording_card_opus_dec_ptr == NULL) {
        audio_alg_interface_t *p_alg_dec     = audio_alg_get_interface_by_type(ALG_OPUS_DEC);
        uint16_t               opus_dec_size = p_alg_dec->audio_alg_get_size(ALG_CHANNEL_LEFT);
        s_recording_card_opus_dec_ptr        = (uint8_t *)tlkalg_malloc_func(opus_dec_size);
        if (s_recording_card_opus_dec_ptr == NULL) {
            tlkapi_printf(APP_LOG_EN, "opus enc alloc failed");
            return;
        }
        p_alg_dec->audio_alg_init(s_recording_card_opus_dec_ptr, ALG_CHANNEL_LEFT);
    }
#endif

#if TLKALG_NN_NS_VAD_ENABLE
    extern uint8_t *g_nn_ns_vad_buf_ptr;
    if (g_nn_ns_vad_buf_ptr == NULL) {
        audio_alg_interface_t *p_alg_nn_ns = audio_alg_get_interface_by_type(ALG_NN_NS_VAD);
        uint16_t               nn_ns_size  = p_alg_nn_ns->audio_alg_get_size(ALG_CHANNEL_LEFT);
        g_nn_ns_vad_buf_ptr                = (uint8_t *)tlkalg_malloc_func(nn_ns_size);
        if (g_nn_ns_vad_buf_ptr == NULL) {
            tlkapi_printf(APP_LOG_EN, "NN_NS alloc failed:%d", nn_ns_size);
            return;
        }
        p_alg_nn_ns->audio_alg_init(g_nn_ns_vad_buf_ptr, ALG_CHANNEL_LEFT);
    }
#endif

#if TLKALG_AGC_ENABLE
    audio_alg_interface_t *p_voice_alg_if = audio_alg_get_interface_by_type(ALG_AGC);
    if (g_alg_rc_cord_agc_buff == NULL) {
        uint16_t agc_size      = p_voice_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
        g_alg_rc_cord_agc_buff = (uint8_t *)tlkalg_malloc_func(agc_size);
        if (g_alg_rc_cord_agc_buff == NULL) {
            tlkapi_printf(APP_LOG_EN, "AGC alloc failed:%d", agc_size);
            return;
        }
        //		extern AGC_Param agc_para;
        //		agc_para.frame_size = 160;
        //		agc_para.target_out_level_dbfs = -6;
        p_voice_alg_if->audio_alg_init(g_alg_rc_cord_agc_buff, 0);
    }
#endif
}

/**
 * @brief       This function deinitializes the recording card algorithm.
 * @param       none.
 * @return      none.
 */
void tlkmdi_recording_card_alg_deinit(void)
{
#if TLKALG_OPUS_ENC_ENABLE
    if (s_recording_card_opus_enc_ptr != NULL) {
        audio_alg_interface_t *p_alg_enc = audio_alg_get_interface_by_type(ALG_OPUS_ENC);
        tlkalg_free_func(s_recording_card_opus_enc_ptr);
        p_alg_enc->audio_alg_deinit();
        s_recording_card_opus_enc_ptr = NULL;
    }
#endif
#if TLKALG_OPUS_DEC_ENABLE
    if (s_recording_card_opus_dec_ptr != NULL) {
        audio_alg_interface_t *p_alg_dec = audio_alg_get_interface_by_type(ALG_OPUS_DEC);
        tlkalg_free_func(s_recording_card_opus_dec_ptr);
        p_alg_dec->audio_alg_deinit();
        s_recording_card_opus_dec_ptr = NULL;
    }
#endif

#if TLKALG_NN_NS_VAD_ENABLE
    extern uint8_t *g_nn_ns_vad_buf_ptr;
    if (g_nn_ns_vad_buf_ptr != NULL) {
        audio_alg_interface_t *p_alg_nn_ns = audio_alg_get_interface_by_type(ALG_NN_NS_VAD);
        tlkalg_free_func(g_nn_ns_vad_buf_ptr);
        p_alg_nn_ns->audio_alg_deinit();
        g_nn_ns_vad_buf_ptr = NULL;
    }
#endif

#if TLKALG_AGC_ENABLE
    if (g_alg_rc_cord_agc_buff != NULL) {
        tlkalg_free_func(g_alg_rc_cord_agc_buff);
        g_alg_rc_cord_agc_buff = NULL;

        audio_alg_interface_t *p_voice_alg_if = audio_alg_get_interface_by_type(ALG_AGC);
        p_voice_alg_if->audio_alg_deinit();
    }
#endif
}

/**
 * @brief       This function initializes the FIFO IRQ for the recording card.
 * @param[in]   byte_num - number of bytes for FIFO initialization.
 * @return      none.
 */
void tlkmdi_recording_card_fifo_irq_init(uint16_t byte_num)
{
    plic_interrupt_enable(IRQ_DFIFO);
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
    audio_fifo_ptr_en(AUDIO_RX_FIFO0);
    audio_set_rx_wptr(TLKDRV_CODEC_MIC_FIFO, 0);
    audio_set_rx_buff_thres(TLKDRV_CODEC_MIC_FIFO, byte_num);
    audio_fifo_irq_en(AUDIO_RX_FIFO0); //enable RX_FIFO0
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
    audio_rxfifo_wptr_en(TLKDRV_CODEC_MIC_FIFO);
    audio_set_rx_wptr(TLKDRV_CODEC_MIC_FIFO, 0);
    audio_set_rx_fifo_threshold(TLKDRV_CODEC_MIC_FIFO, byte_num);
    audio_rxfifo_irq_en(TLKDRV_CODEC_MIC_FIFO); //enable RX_FIFO0
#endif
}

/**
 * @brief       This function registers a callback to fill encoded data.
 * @param[in]   func - callback function pointer.
 * @return      none.
 */
void tlkmdi_recording_card_fillEncData_register(tlkalg_recording_card_fill_enc_data_t func)
{
    tlkalg_recording_card_fill_enc_data_cb = func;
}

/**
 * @brief       This function registers a callback to fill PCM data.
 * @param[in]   func - callback function pointer.
 * @return      none.
 */
void tlkmdi_recording_card_fillPcmData_register(tlkalg_recording_card_fill_pcm_data_t func)
{
    tlkalg_recording_card_fill_pcm_data_cb = func;
}

/**
 * @brief       This function processes the OPUS encoding.
 * @param[in]   data - pointer to the data to be processed.
 * @return      none.
 */
void tlkmdi_recording_card_opus_process(adc_mono_int *data)
{
    // gpio_set_high_level(GPIO_PC2);
    uint8_t enc_buff[1500];

#if TLKALG_AGC_ENABLE
    tlkalg_agc_proc_behind_nn((uint8_t *)data, (uint8_t *)data);
#endif
    //    gpio_toggle(GPIO_PC2);
    //    gpio_set_high_level(GPIO_PC3);
    audio_alg_interface_t *p_alg_enc = audio_alg_get_interface_by_type(ALG_OPUS_ENC);
    int                    ret_num   = p_alg_enc->audio_alg_process((uint8_t *)data, (uint8_t *)enc_buff, (uint16_t)RECORDING_CARD_MIC_FRAME_SIZE, 0, 0);
    //    gpio_set_low_level(GPIO_PC3);

#if TLKALG_OPUS_DEC_ENABLE
    adc_mono_int           dec_buff[RECORDING_CARD_MIC_FRAME_SIZE];
    audio_alg_interface_t *p_alg_dec = audio_alg_get_interface_by_type(ALG_OPUS_DEC);
    int                    dec_num   = p_alg_dec->audio_alg_process((uint8_t *)&enc_buff[2], (uint8_t *)dec_buff, ret_num, 0, 0);

    tlkdrv_codec_fillSpkBuff((uint8_t *)(dec_buff), dec_num * sizeof(adc_mono_int));
#else
    if (tlkalg_recording_card_fill_enc_data_cb != NULL) {
        tlkalg_recording_card_fill_enc_data_cb(enc_buff, ret_num + 8);
    }
    if (tlkalg_recording_card_fill_pcm_data_cb != NULL) {
        tlkalg_recording_card_fill_pcm_data_cb((uint8_t *)data, RECORDING_CARD_MIC_FRAME_SIZE * 2);
    }

#endif
    // gpio_set_low_level(GPIO_CHN1);
}

/**
 * @brief       This function handles the FIFO IRQ for the recording card.
 * @param       none.
 * @return      none.
 */
void tlkmdi_recording_card_fifo_irq_handler_func(void)
{
    // gpio_set_high_level(GPIO_CHN6);
    static uint8_t recording_frame_cnt = 0;
    uint16_t       wptr                = audio_get_rx_wptr(TLKDRV_CODEC_MIC_FIFO);
    uint16_t       threshold_value     = 0;
    if (wptr >= RECORDING_CARD_MIC_FRAME_SIZE / 2) {
        threshold_value = wptr - RECORDING_CARD_MIC_FRAME_SIZE / 2;
    }
    audio_set_rx_wptr(TLKDRV_CODEC_MIC_FIFO, threshold_value);

    adc_mono_int data_buff_mono[RECORDING_CARD_MIC_FRAME_SIZE];
    bool         ret = 0;
    if ((tlkdrv_codec_getMicDataLen() / sizeof(adc_mono_int)) >= RECORDING_CARD_MIC_FRAME_SIZE) {
        if (s_recording_card_env.first_frame) {
            wptr = (tlkdrv_codec_getMicOffset() + RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int)) % (tlkdrv_codec_getMicBuffLen() - sizeof(adc_mono_int));
            tlkdrv_codec_setMicOffset(wptr);
            //tlkapi_printf(APP_LOG_EN, "tlkdrv_codec_readMicData %d",wptr);
            recording_frame_cnt++;
            tmemset((uint8_t *)(data_buff_mono), 0x00, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));
            if (recording_frame_cnt > 50) {
                s_recording_card_env.first_frame = 0;
                recording_frame_cnt              = 0;
            }
        } else {
            ret = tlkdrv_codec_readMicData((uint8_t *)(data_buff_mono), RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int), 0); //sizeof(adc_mono_int)
        }
    } else {
        tlkapi_printf(APP_LOG_EN, "mic buff null %d %d", RECORDING_CARD_MIC_FRAME_SIZE, tlkdrv_codec_getMicDataLen() / sizeof(adc_mono_int));
        return;
    }


    if (ret) {
        int vad_flag = 1;
#if TLK_MW_DSP_COMM_ENABLE
        ///write data to DSP
        if (s_recording_card_env.nn_alg_en) {
            app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(NN_NS_16K_20MS_ID);
            p_dsp_app_ctx->alg_type          = VAD_NN_NS;
            d25f_send_audio_data_to_dsp((uint8_t *)data_buff_mono, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int), NN_NS_16K_20MS_ID);
        } else
#endif
#if TLKALG_NN_NS_VAD_ENABLE
            if (s_recording_card_env.nn_alg_en) {

#if 0 ///for test
        	uint8_t spi_debug_buff[650];
        	spi_debug_buff[0] = 0x0F;
			spi_debug_buff[1] = 0x0F;
			spi_debug_buff[2] = 0x0F;
			spi_debug_buff[3] = 0x0F;
			tmemcpy((void *)&spi_debug_buff[4],(const void *)data_buff_mono,RECORDING_CARD_MIC_FRAME_SIZE*2);
			spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)spi_debug_buff, 650, SPI_MODE_WR_WRITE_ONLY);
#endif
            audio_alg_interface_t *p_alg_enc = audio_alg_get_interface_by_type(ALG_NN_NS_VAD);
            int                    ret_num   = p_alg_enc->audio_alg_process((uint8_t *)data_buff_mono, (uint8_t *)&vad_flag, (uint16_t)RECORDING_CARD_MIC_FRAME_SIZE, 0, 0);

#if 0 ///for test
			spi_debug_buff[0] = 0xF0;
			spi_debug_buff[1] = 0xF0;
			spi_debug_buff[2] = 0xF0;
			spi_debug_buff[3] = 0xF0 | vad_flag;
        tmemcpy((void *)&spi_debug_buff[4],(const void *)data_buff_mono,RECORDING_CARD_MIC_FRAME_SIZE*2);
		spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)spi_debug_buff, 650, SPI_MODE_WR_WRITE_ONLY);
#endif

            if (ret_num) {
                tlkapi_printf(APP_LOG_EN, "tlka_nn_ns_vad_process_frame:ERR %d", ret_num);
            }
        }
#endif
        {
            if ((!VAD_FUNCTION_ENABLE) || vad_flag) {
                tlkmdi_recording_card_opus_process(data_buff_mono);
            } else {
                //        		tmemset(data_buff_mono,0x00,RECORDING_CARD_MIC_FRAME_SIZE*2);
                //				tlkmdi_recording_card_opus_process(data_buff_mono);
            }
        }
    }
}

#if TLK_MW_DSP_COMM_ENABLE
/**
 * @brief       This function processes DSP messages for the recording card.
 * @param[in]   enc_buff_wptr - write pointer of the encoded buffer.
 * @param[in]   type - message type.
 * @return      none.
 */
void tlkmdi_recording_card_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type)
{
    (void)enc_buff_wptr;
    (void)type;
    //get data processed by nn_ns in dsp
    // gpio_set_high_level(GPIO_CHN4);
    uint16_t pcm_data_len;
    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&pcm_data_len, NN_NS_16K_20MS_ID);
    if (pcm_data == NULL) {
        tlkapi_printf(APP_LOG_EN, "interphone pcm_data is NULL");
        return;
    }
    if (!s_recording_card_env.nn_alg_en) {
        return;
    }
    tlkmdi_recording_card_opus_process((adc_mono_int *)pcm_data);
    // gpio_set_low_level(GPIO_CHN4);
}
#endif
#endif
