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

#if (TLKALG_BBF_PATH_SEL && TLKALG_BBF_ENABLE)
uint8_t *s_alg_bbf_buff_ptr = NULL;
#endif

#if TLKALG_AGC_ENABLE
#include "tlkalg/audio/agc/tlkalg_agc_interface.h"
uint8_t *g_alg_rc_cord_agc_buff = NULL;
#endif

tlkalg_recording_card_fill_enc_data_t tlkalg_recording_card_fill_enc_data_cb = NULL;
tlkalg_recording_card_fill_pcm_data_t tlkalg_recording_card_fill_pcm_data_cb = NULL;
extern tlkmdi_recording_card_t        s_recording_card_env;

int8_t alg_bone_data_switch = 2;

#if TLKALG_GET_AUDIO_DATA_EN
//_attribute_iram_data_
uint8_t spi_debug_buff[RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE + 3) * 2 + 8];
#endif
/**
 * @brief       This function initializes the recording card algorithm.
 * @param       none.
 * @return      none.
 */
void tlkmdi_recording_card_alg_init(void)
{
#if TLKALG_OPUS_ENC_ENABLE
    if (0 != audio_alg_get_interface_by_type(ALG_OPUS_ENC)->audio_alg_init(NULL, ALG_CHANNEL_LEFT)) {
        return;
    }
#endif

#if (TLKALG_BBF_PATH_SEL && TLKALG_BBF_ENABLE)
    if (s_alg_bbf_buff_ptr == NULL) {
        audio_alg_interface_t *p_alg_bbf = audio_alg_get_interface_by_type(ALG_BBF);
        p_alg_bbf->audio_alg_init(s_alg_bbf_buff_ptr, ALG_CHANNEL_STEREO);
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
        //		agc_para.target_out_level_dbfs = -9;
        //		agc_para.noise_level_dbfs_thld = -38;
        p_voice_alg_if->audio_alg_init(g_alg_rc_cord_agc_buff, 0);
    }
#endif

#if TLKALG_EQ_ENABLE
    audio_alg_interface_t *p_eq_alg_if    = audio_alg_get_interface_by_type(ALG_EQ);
    eq_para_t              tlkalg_eq_para = {
                     .samplerate = 16000,
                     .channel    = ALG_CHANNEL_LEFT,
                     .eq_type    = EQ_TYPE_VOICE_MIC,
                     .width      = ALG_WIDTH_16,
    };
    p_eq_alg_if->audio_alg_param_set(0, (void *)&tlkalg_eq_para);
    p_eq_alg_if->audio_alg_init(0, ALG_CHANNEL_LEFT);
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
    audio_alg_get_interface_by_type(ALG_OPUS_ENC)->audio_alg_deinit();
#endif

#if (TLKALG_BBF_PATH_SEL && TLKALG_BBF_ENABLE)
    if (s_alg_bbf_buff_ptr != NULL) {
        audio_alg_interface_t *p_alg_bbf = audio_alg_get_interface_by_type(ALG_BBF);
        tlkalg_free_func(s_alg_bbf_buff_ptr);
        p_alg_bbf->audio_alg_deinit();
        s_alg_bbf_buff_ptr = NULL;
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

#if TLKALG_EQ_ENABLE
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_EQ);
    p_audio_alg_if->audio_alg_deinit();
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
    audio_rxfifo_clear(TLKDRV_CODEC_MIC_FIFO);
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
    uint8_t      enc_buff[1500];
    adc_mono_int agc_out_buff[RECORDING_CARD_MIC_FRAME_SIZE];

#if TLKALG_GET_AUDIO_DATA_EN
    int16_t *ps = (int16_t *)data;
    int16_t *pd = (int16_t *)&spi_debug_buff[(4 + TLKALG_BBF_ENABLE + 1) * 2];

    for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE; j++) {
        pd[j * (TLKALG_BBF_ENABLE + 3)] = *ps++;
    }
//	spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)spi_debug_buff, RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE+3) * 2 + 8, SPI_MODE_WR_WRITE_ONLY);
#endif

#if TLKALG_AGC_ENABLE
    if (s_recording_card_env.nn_alg_en) {
        tlkalg_agc_proc_behind_nn((uint8_t *)data, (uint8_t *)agc_out_buff, ALG_WIDTH_16);
    } else {
        tmemcpy(agc_out_buff, data, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));
    }
#else
    tmemcpy(agc_out_buff, data, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));
#endif

#if TLKALG_GET_AUDIO_DATA_EN
    ps = (int16_t *)agc_out_buff;
    pd = (int16_t *)&spi_debug_buff[(4 + TLKALG_BBF_ENABLE + 2) * 2];
    for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE; j++) {
        pd[j * (TLKALG_BBF_ENABLE + 3)] = *ps++;
    }
    spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)spi_debug_buff, RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE + 3) * 2 + 8,
                              SPI_MODE_WR_WRITE_ONLY);
#endif

    int ret_num = audio_alg_get_interface_by_type(ALG_OPUS_ENC)->audio_alg_process((uint8_t *)agc_out_buff, (uint8_t *)enc_buff, (uint16_t)RECORDING_CARD_MIC_FRAME_SIZE, 0, 0);
    if (tlkalg_recording_card_fill_enc_data_cb != NULL) {
        tlkalg_recording_card_fill_enc_data_cb(enc_buff, ret_num + 8);
    }
    if (tlkalg_recording_card_fill_pcm_data_cb != NULL) {
        tlkalg_recording_card_fill_pcm_data_cb((uint8_t *)agc_out_buff, RECORDING_CARD_MIC_FRAME_SIZE * 2);
    }
}

void tlkmdi_audio_mic_preprocess(int16_t *data_ps, int8_t chn)
{
#if (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN)
    adc_mono_int data_preprocess_buff[RECORDING_CARD_MIC_FRAME_SIZE * TLKALG_BBF_6CH_EN];
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_4CH_EN)
    adc_mono_int data_preprocess_buff[RECORDING_CARD_MIC_FRAME_SIZE * TLKALG_BBF_4CH_EN];
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_2CH_EN)
    adc_mono_int data_preprocess_buff[RECORDING_CARD_MIC_FRAME_SIZE * TLKALG_BBF_2CH_EN];
#endif

#if (!TLKALG_BBF_ENABLE)
    (void)data_ps;
    (void)chn;
#else
    adc_mono_int *ps = (int16_t *)data_ps;
    adc_mono_int *pd = (int16_t *)data_preprocess_buff;

    if (chn == TLKALG_BBF_6CH_EN) {
        for (int16_t i = 0; i < 4; i++) {
            for (int16_t j = 0; j < 80; j++) {
                pd[480 * i + j]       = *ps++;
                pd[480 * i + 80 + j]  = *ps++;
                pd[480 * i + 160 + j] = *ps++;
                pd[480 * i + 240 + j] = *ps++;
                pd[480 * i + 320 + j] = *ps++;
                pd[480 * i + 400 + j] = *ps++;
            }
        }
    } else if (chn == TLKALG_BBF_4CH_EN) {
        for (int16_t i = 0; i < 4; i++) {
            for (int16_t j = 0; j < 80; j++) {
                pd[320 * i + j]       = *ps++;
                pd[320 * i + 80 + j]  = *ps++;
                pd[320 * i + 160 + j] = *ps++;
                pd[320 * i + 240 + j] = *ps++;
            }
        }
    } else if (chn == TLKALG_BBF_2CH_EN) {
        for (int16_t i = 0; i < 4; i++) {
            for (int16_t j = 0; j < 80; j++) {
                pd[160 * i + j]      = *ps++;
                pd[160 * i + 80 + j] = *ps++;
            }
        }
    }

    tmemcpy(data_ps, pd, RECORDING_CARD_MIC_FRAME_SIZE * chn * sizeof(adc_mono_int));
#endif
}

/**
 * @brief       This function handles the FIFO IRQ for the recording card.
 * @param       none.
 * @return      none.
 */

#if (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN)
int8_t tlkmdi_audio_mic_chn_convert(int16_t *ps0)
{
    adc_4ch_int data_convert_buff_4_ch[RECORDING_CARD_MIC_FRAME_SIZE];
    adc_int     data_convert_buff_stereo[RECORDING_CARD_MIC_FRAME_SIZE];

    bool ret = tlkdrv_codec_readMicData((uint8_t *)(data_convert_buff_4_ch), RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_4ch_int), 0);
    tlkdrv_codec_readMicData_1((uint8_t *)(data_convert_buff_stereo), RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_int), 0);

    adc_mono_int *ptr_scr1 = (adc_mono_int *)data_convert_buff_4_ch;
    adc_mono_int *ptr_scr2 = (adc_mono_int *)data_convert_buff_stereo;
    adc_mono_int *ptr_dest = (adc_mono_int *)ps0;
    for (int16_t i = 0; i < RECORDING_CARD_MIC_FRAME_SIZE; i++) {
        *ptr_dest++ = *ptr_scr1++;
        *ptr_dest++ = *ptr_scr1++;
        *ptr_dest++ = *ptr_scr1++;
        *ptr_dest++ = *ptr_scr1++;
        *ptr_dest++ = *ptr_scr2++;
        *ptr_dest++ = *ptr_scr2++;
    }

    return ret;
}

void tlkmdi_alg_6mic_work(adc_mono_int *mics_data_buff, adc_mono_int *dsp_out_buff)
{
    int16_t *ps = NULL;
    int16_t *pd = NULL;

    if (s_recording_card_env.nn_alg_en) {
#if (TLKALG_BBF_ENABLE)
        app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(BBF_NN_NS_16K_20MS_ID);
        p_dsp_app_ctx->alg_type          = VAD_NN_NS;
        d25f_send_audio_data_to_dsp((uint8_t *)mics_data_buff, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int) * TLKALG_BBF_ENABLE, BBF_NN_NS_16K_20MS_ID);
#else
        ps = (int16_t *)mics_data_buff;
        pd = (int16_t *)data_buff_mono;
        for (int16_t j = 0; j < 16 * 20;) {
            pd[j++] = *ps;
            ps      = ps + 6;
        }
        app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(NN_NS_16K_20MS_ID);
        p_dsp_app_ctx->alg_type          = VAD_NN_NS;
        d25f_send_audio_data_to_dsp((uint8_t *)data_buff_mono, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int), NN_NS_16K_20MS_ID);
#endif
        tlkmdi_recording_card_process_pcm_from_dsp((int8_t *)dsp_out_buff);
    } else {
        ps = (int16_t *)mics_data_buff;
        pd = (int16_t *)dsp_out_buff;
        for (int16_t j = 0; j < 16 * 20;) {
            pd[j++] = *ps;
            ps      = ps + 6;
        }
    }
}

void tlkmdi_alg_5mic_1bone_work(adc_mono_int *mics_data_buff, adc_mono_int *dsp_out_buff, adc_mono_int *bone_out_buff)
{
    int16_t *ps  = NULL;
    int16_t *pd  = NULL;
    int16_t *pd1 = NULL;
    int16_t *pd2 = NULL;

    adc_mono_int data_buff_5_ch[RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_6CH_EN - 1)];

    ps  = (int16_t *)mics_data_buff;
    pd  = (int16_t *)data_buff_5_ch;
    pd1 = (int16_t *)bone_out_buff;
    pd2 = (int16_t *)dsp_out_buff;

#if (BONE_TYPE_SEL == DIGITAL_BONE_MIC)
    for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE; j++) {
        pd2[j] = pd[j * 5] = *ps++;
        pd[j * 5 + 1]      = *ps++;
        pd[j * 5 + 2]      = *ps++;
        pd[j * 5 + 3]      = *ps++;
        pd1[j]             = *ps++;
        pd[j * 5 + 4]      = *ps++;
    }
#else
    for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE; j++) {
        pd2[j] = pd[j * 5] = *ps++;
        pd[j * 5 + 1]      = *ps++;
        pd[j * 5 + 2]      = *ps++;
        pd1[j]             = *ps++;
        pd[j * 5 + 3]      = *ps++;
        pd[j * 5 + 4]      = *ps++;
    }
#endif


    if (s_recording_card_env.nn_alg_en) {
        app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(BBF_NN_NS_16K_20MS_ID);
        p_dsp_app_ctx->alg_type          = VAD_NN_NS;
        d25f_send_audio_data_to_dsp((uint8_t *)data_buff_5_ch, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int) * (TLKALG_BBF_ENABLE - 1), BBF_NN_NS_16K_20MS_ID);
        tlkmdi_recording_card_process_pcm_from_dsp((int8_t *)dsp_out_buff);
    }

    /// TODO :filter process
#if TLKALG_EQ_ENABLE
    audio_alg_interface_t *p_alg_eq = audio_alg_get_interface_by_type(ALG_EQ);
    p_alg_eq->audio_alg_process((uint8_t *)bone_out_buff, (uint8_t *)bone_out_buff, 320, ALG_WIDTH_16, ALG_CHANNEL_LEFT);
#endif
}

int8_t tlkmdi_recording_card_fifo_irq_process_6(adc_mono_int *data_buff)
{
#if TLKALG_GET_AUDIO_DATA_EN
    int16_t *ps = NULL;
    int16_t *pd = NULL;
#endif

    adc_mono_int data_buff_6_ch[RECORDING_CARD_MIC_FRAME_SIZE * TLKALG_BBF_6CH_EN];
    adc_mono_int data_buff_mono[RECORDING_CARD_MIC_FRAME_SIZE];
#if TLKALG_BONE_CODUCTION_EN
    adc_mono_int bone_buff[RECORDING_CARD_MIC_FRAME_SIZE];
#endif

    // gpio_set_high_level(GPIO_CHN6);
    static uint8_t recording_frame_cnt = 0;
    uint16_t       wptr                = audio_get_rx_wptr(TLKDRV_CODEC_MIC_FIFO);
    uint16_t       threshold_value     = 0;

    if (wptr >= RECORDING_CARD_MIC_FRAME_SIZE * 2) {
        threshold_value = wptr - RECORDING_CARD_MIC_FRAME_SIZE * 2;
    }
    audio_set_rx_wptr(TLKDRV_CODEC_MIC_FIFO, threshold_value);

    if (s_recording_card_env.first_frame && (!recording_frame_cnt)) {
        tlkdrv_codec_between_sync(RECORDING_CARD_MIC_FRAME_SIZE, 4, 2);
    }

    int8_t ret = tlkmdi_audio_mic_chn_convert(data_buff_6_ch);


    if (s_recording_card_env.first_frame) {
        //		tlkapi_printf(APP_LOG_EN, "tlkdrv_codec_readMicData %d",wptr);
        recording_frame_cnt++;
        if (recording_frame_cnt > 9) {
            s_recording_card_env.first_frame = 0;
            recording_frame_cnt              = 0;
            adc_mono_int *ptr_dest           = data_buff_6_ch;
            for (int16_t i = 0; i < RECORDING_CARD_MIC_FRAME_SIZE; i++) {
                for (int8_t j = 0; j < TLKALG_BBF_6CH_EN; j++) {
                    *ptr_dest = (*ptr_dest) * i / RECORDING_CARD_MIC_FRAME_SIZE;
                    ptr_dest++;
                }
            }
        } else {
            tmemset((uint8_t *)(data_buff_6_ch), 0x00, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int) * TLKALG_BBF_6CH_EN);
        }
    }

    if (ret) {
#if (TLKALG_BBF_PATH_SEL)
        tlkmdi_audio_mic_preprocess(data_buff_6_ch, TLKALG_BBF_6CH_EN);
#if 1
        audio_alg_interface_t *p_alg_bbf = audio_alg_get_interface_by_type(ALG_BBF);
        p_alg_bbf->audio_alg_process((uint8_t *)data_buff_6_ch, (uint8_t *)data_buff_mono, 480, 0, 0);
        p_alg_bbf->audio_alg_process((uint8_t *)&data_buff_6_ch[480], (uint8_t *)&data_buff_mono[80], 480, 0, 0);
        p_alg_bbf->audio_alg_process((uint8_t *)&data_buff_6_ch[960], (uint8_t *)&data_buff_mono[160], 480, 0, 0);
        p_alg_bbf->audio_alg_process((uint8_t *)&data_buff_6_ch[1440], (uint8_t *)&data_buff_mono[240], 480, 0, 0);
#else
        tmemcpy((uint8_t *)data_buff_mono, (uint8_t *)data_buff_6_ch, 80 * sizeof(adc_mono_int));
        tmemcpy((uint8_t *)&data_buff_mono[80], (uint8_t *)&data_buff_6_ch[480], 80 * sizeof(adc_mono_int));
        tmemcpy((uint8_t *)&data_buff_mono[160], (uint8_t *)&data_buff_6_ch[960], 80 * sizeof(adc_mono_int));
        tmemcpy((uint8_t *)&data_buff_mono[240], (uint8_t *)&data_buff_6_ch[1440], 80 * sizeof(adc_mono_int));
#endif

#if TLKALG_GET_AUDIO_DATA_EN
        static uint16_t index           = 0;
        spi_debug_buff[0]               = 0x0F;
        spi_debug_buff[1]               = 0x0F;
        spi_debug_buff[2]               = 0x0F;
        spi_debug_buff[3]               = 0x0F;
        *(uint16_t *)&spi_debug_buff[4] = TLKALG_BBF_ENABLE + 3;
        *(uint16_t *)&spi_debug_buff[6] = index++;

        ps           = (int16_t *)data_buff_6_ch;
        int16_t *ps1 = (int16_t *)data_buff_mono;
        pd           = (int16_t *)&spi_debug_buff[8];
        for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE + 3);) {
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps1++;
            pd[j++] = 0x00;
            pd[j++] = 0x00;
        }
//		spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)spi_debug_buff, RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE+3) * 2 + 8, SPI_MODE_WR_WRITE_ONLY);
#endif
#endif

#if TLK_MW_DSP_COMM_ENABLE
        ///write data to DSP
#if TLKALG_GET_AUDIO_DATA_EN
        static uint16_t index           = 0;
        spi_debug_buff[0]               = 0x0F;
        spi_debug_buff[1]               = 0x0F;
        spi_debug_buff[2]               = 0x0F;
        spi_debug_buff[3]               = 0x0F;
        *(uint16_t *)&spi_debug_buff[4] = TLKALG_BBF_ENABLE + 3;
        *(uint16_t *)&spi_debug_buff[6] = index++;

        ps = (int16_t *)data_buff_6_ch;
        pd = (int16_t *)&spi_debug_buff[8];
        for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE + 3);) {
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = 0x00;
            pd[j++] = 0x00;
            pd[j++] = 0x00;
        }
//		spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)spi_debug_buff, RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE+3) * 2 + 8, SPI_MODE_WR_WRITE_ONLY);
#endif

#if (TLKALG_BONE_CODUCTION_EN)
        tlkmdi_alg_5mic_1bone_work(data_buff_6_ch, data_buff_mono, bone_buff);
#if TLKALG_GET_AUDIO_DATA_EN
        ps = (int16_t *)bone_buff;
        pd = (int16_t *)&spi_debug_buff[(4 + TLKALG_BBF_ENABLE) * 2];
        for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE; j++) {
            pd[j * (TLKALG_BBF_ENABLE + 3)] = *ps++;
        }
#endif
        if (alg_bone_data_switch == 0x02) { ///Audio Mixing output
            int sum, tmp1, tmp2;
            for (short i = 0; i < RECORDING_CARD_MIC_FRAME_SIZE; i++) {
                tmp1 = data_buff_mono[i];
                tmp2 = bone_buff[i];
                sum  = tmp1 + tmp2;
                if (sum > 32767) {
                    data_buff_mono[i] = 32767;
                } else if (sum < -32768) {
                    data_buff_mono[i] = -32768;
                } else {
                    data_buff_mono[i] = sum;
                }
            }
        } else if (alg_bone_data_switch == 0x01) { ///bone data output
            tmemcpy(data_buff_mono, bone_buff, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));
        }
#else
        tlkmdi_alg_6mic_work(data_buff_6_ch, data_buff_mono);
#endif

#endif
    }
    tmemcpy(data_buff, data_buff_mono, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));

    return ret;
}
#endif

#if (TLKALG_BBF_ENABLE == TLKALG_BBF_4CH_EN)
int8_t tlkmdi_recording_card_fifo_irq_process_4(adc_mono_int *data_buff)
{
    int16_t     *ps = NULL;
    int16_t     *pd = NULL;
    adc_4ch_int  data_buff_4_ch[RECORDING_CARD_MIC_FRAME_SIZE];
    adc_mono_int data_buff_mono[RECORDING_CARD_MIC_FRAME_SIZE];

    // gpio_set_high_level(GPIO_CHN6);
    static uint8_t recording_frame_cnt = 0;
    uint16_t       wptr                = audio_get_rx_wptr(TLKDRV_CODEC_MIC_FIFO);
    uint16_t       threshold_value     = 0;

    if (wptr >= RECORDING_CARD_MIC_FRAME_SIZE * 2) {
        threshold_value = wptr - RECORDING_CARD_MIC_FRAME_SIZE * 2;
    }
    audio_set_rx_wptr(TLKDRV_CODEC_MIC_FIFO, threshold_value);

    int8_t ret = tlkdrv_codec_readMicData((uint8_t *)(data_buff_4_ch), RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_4ch_int), 0);
    if (s_recording_card_env.first_frame) {
        //		tlkapi_printf(APP_LOG_EN, "tlkdrv_codec_readMicData %d",wptr);
        recording_frame_cnt++;
        if (recording_frame_cnt > 9) {
            s_recording_card_env.first_frame = 0;
            recording_frame_cnt              = 0;
            adc_mono_int *ptr                = (adc_mono_int *)data_buff_4_ch;
            for (int16_t i = 0; i < RECORDING_CARD_MIC_FRAME_SIZE; i++) {
                *ptr = (*ptr) * i / RECORDING_CARD_MIC_FRAME_SIZE;
                ptr++;
                *ptr = (*ptr) * i / RECORDING_CARD_MIC_FRAME_SIZE;
                ptr++;
                *ptr = (*ptr) * i / RECORDING_CARD_MIC_FRAME_SIZE;
                ptr++;
                *ptr = (*ptr) * i / RECORDING_CARD_MIC_FRAME_SIZE;
                ptr++;
            }
        } else {
            tmemset((uint8_t *)(data_buff_4_ch), 0x00, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_4ch_int));
        }
    }

    if (ret) {
#if (TLKALG_BBF_PATH_SEL)
        tlkmdi_audio_mic_preprocess((int16_t *)data_buff_4_ch, TLKALG_BBF_4CH_EN);

        audio_alg_interface_t *p_alg_bbf = audio_alg_get_interface_by_type(ALG_BBF);
        p_alg_bbf->audio_alg_process((uint8_t *)data_buff_4_ch, (uint8_t *)data_buff_mono, 320, 0, 0);
        p_alg_bbf->audio_alg_process((uint8_t *)&data_buff_4_ch[80], (uint8_t *)&data_buff_mono[80], 320, 0, 0);
        p_alg_bbf->audio_alg_process((uint8_t *)&data_buff_4_ch[160], (uint8_t *)&data_buff_mono[160], 320, 0, 0);
        p_alg_bbf->audio_alg_process((uint8_t *)&data_buff_4_ch[240], (uint8_t *)&data_buff_mono[240], 320, 0, 0);
#if TLKALG_GET_AUDIO_DATA_EN
        static uint16_t index           = 0;
        spi_debug_buff[0]               = 0x0F;
        spi_debug_buff[1]               = 0x0F;
        spi_debug_buff[2]               = 0x0F;
        spi_debug_buff[3]               = 0x0F;
        *(uint16_t *)&spi_debug_buff[4] = TLKALG_BBF_ENABLE + 3;
        *(uint16_t *)&spi_debug_buff[6] = index++;

        ps           = (int16_t *)data_buff_4_ch;
        int16_t *ps1 = data_buff_mono;
        pd           = (int16_t *)&spi_debug_buff[8];
        for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE + 1);) {
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps1++;
        }
        spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)spi_debug_buff, RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE + 1) * 2 + 8,
                                  SPI_MODE_WR_WRITE_ONLY);
#endif

#endif

#if TLK_MW_DSP_COMM_ENABLE
        ///write data to DSP
#if TLKALG_GET_AUDIO_DATA_EN
        static uint16_t index           = 0;
        spi_debug_buff[0]               = 0x0F;
        spi_debug_buff[1]               = 0x0F;
        spi_debug_buff[2]               = 0x0F;
        spi_debug_buff[3]               = 0x0F;
        *(uint16_t *)&spi_debug_buff[4] = TLKALG_BBF_ENABLE + 3;
        *(uint16_t *)&spi_debug_buff[6] = index++;

        ps = (int16_t *)data_buff_4_ch;
        pd = (int16_t *)&spi_debug_buff[8];
        for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE + 3);) {
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = 0x00;
            pd[j++] = 0x00;
            pd[j++] = 0x00;
        }
//			spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)spi_debug_buff, RECORDING_CARD_MIC_FRAME_SIZE * 5 * 2 + 8, SPI_MODE_WR_WRITE_ONLY);
#endif
        if (s_recording_card_env.nn_alg_en) {
#if (TLKALG_BBF_ENABLE)
            app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(BBF_NN_NS_16K_20MS_ID);
            p_dsp_app_ctx->alg_type          = VAD_NN_NS;
            d25f_send_audio_data_to_dsp((uint8_t *)data_buff_4_ch, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_4ch_int), BBF_NN_NS_16K_20MS_ID);
#else
            ps = (int16_t *)data_buff_4_ch;
            pd = (int16_t *)data_buff_mono;
            for (int16_t j = 0; j < 16 * 20;) {
                pd[j++] = *ps++;
                ps++;
                ps++;
                ps++;
            }
            app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(NN_NS_16K_20MS_ID);
            p_dsp_app_ctx->alg_type          = VAD_NN_NS;
            d25f_send_audio_data_to_dsp((uint8_t *)data_buff_mono, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int), NN_NS_16K_20MS_ID);
#endif
            tlkmdi_recording_card_process_pcm_from_dsp((int8_t *)data_buff_mono);
        } else {
            ps = (int16_t *)data_buff_4_ch;
            pd = (int16_t *)data_buff_mono;
            for (int16_t j = 0; j < 16 * 20;) {
                pd[j++] = *ps++;
                ps++;
                ps++;
                ps++;
            }
#endif
        }
    }

    tmemcpy(data_buff, data_buff_mono, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));

    return ret;
}
#endif

#if (TLKALG_BBF_ENABLE == TLKALG_BBF_2CH_EN)
int8_t tlkmdi_recording_card_fifo_irq_process_2(adc_mono_int *data_buff)
{
    int16_t     *ps = NULL;
    int16_t     *pd = NULL;
    adc_int      data_buff_stereo[RECORDING_CARD_MIC_FRAME_SIZE];
    adc_mono_int data_buff_mono[RECORDING_CARD_MIC_FRAME_SIZE];

    // gpio_set_high_level(GPIO_CHN6);
    static uint8_t recording_frame_cnt = 0;
    uint16_t       wptr                = audio_get_rx_wptr(TLKDRV_CODEC_MIC_FIFO);
    uint16_t       threshold_value     = 0;

    if (wptr >= RECORDING_CARD_MIC_FRAME_SIZE) {
        threshold_value = wptr - RECORDING_CARD_MIC_FRAME_SIZE;
    }
    audio_set_rx_wptr(TLKDRV_CODEC_MIC_FIFO, threshold_value);

    //    adc_int data_buff_stereo[RECORDING_CARD_MIC_FRAME_SIZE];
    //    adc_mono_int data_buff_mono[RECORDING_CARD_MIC_FRAME_SIZE];
    int8_t ret = tlkdrv_codec_readMicData((uint8_t *)(data_buff_stereo), RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_int), 0);
    if (s_recording_card_env.first_frame) {
        //		tlkapi_printf(APP_LOG_EN, "tlkdrv_codec_readMicData %d",wptr);
        recording_frame_cnt++;
        if (recording_frame_cnt > 9) {
            s_recording_card_env.first_frame = 0;
            recording_frame_cnt              = 0;
            adc_mono_int *ptr                = (adc_mono_int *)data_buff_stereo;
            for (int16_t i = 0; i < RECORDING_CARD_MIC_FRAME_SIZE; i++) {
                *ptr = (*ptr) * i / RECORDING_CARD_MIC_FRAME_SIZE;
                ptr++;
                *ptr = (*ptr) * i / RECORDING_CARD_MIC_FRAME_SIZE;
                ptr++;
            }
        } else {
            tmemset((uint8_t *)(data_buff_stereo), 0x00, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_int));
        }
    }

    if (ret) {
#if TLKALG_GET_AUDIO_DATA_EN
        static uint16_t index           = 0;
        spi_debug_buff[0]               = 0x0F;
        spi_debug_buff[1]               = 0x0F;
        spi_debug_buff[2]               = 0x0F;
        spi_debug_buff[3]               = 0x0F;
        *(uint16_t *)&spi_debug_buff[4] = TLKALG_BBF_ENABLE + 3;
        *(uint16_t *)&spi_debug_buff[6] = index++;

        ps = (int16_t *)data_buff_stereo;
        pd = (int16_t *)&spi_debug_buff[8];
        for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE + 3);) {
            pd[j++] = *ps++;
            pd[j++] = *ps++;
            pd[j++] = 0x00;
            pd[j++] = 0x00;
            pd[j++] = 0x00;
        }
//		spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)spi_debug_buff, RECORDING_CARD_MIC_FRAME_SIZE * 5 * 2 + 8, SPI_MODE_WR_WRITE_ONLY);
#else
        (void)ps;
        (void)pd;
#endif

#if TLKALG_BBF_PATH_SEL
        tlkmdi_audio_mic_preprocess((int16_t *)data_buff_stereo, TLKALG_BBF_2CH_EN);
        audio_alg_interface_t *p_alg_bbf = audio_alg_get_interface_by_type(ALG_BBF);
        p_alg_bbf->audio_alg_process((uint8_t *)data_buff_stereo, (uint8_t *)data_buff_mono, 160, 0, 0);
        p_alg_bbf->audio_alg_process((uint8_t *)&data_buff_stereo[80], (uint8_t *)&data_buff_mono[80], 160, 0, 0);
        p_alg_bbf->audio_alg_process((uint8_t *)&data_buff_stereo[160], (uint8_t *)&data_buff_mono[160], 160, 0, 0);
        p_alg_bbf->audio_alg_process((uint8_t *)&data_buff_stereo[240], (uint8_t *)&data_buff_mono[240], 160, 0, 0);

#if TLKALG_GET_AUDIO_DATA_EN
        ps = (int16_t *)data_buff_mono;
        pd = (int16_t *)&spi_debug_buff[(4 + TLKALG_BBF_ENABLE) * 2];

        for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE; j++) {
            pd[j * (TLKALG_BBF_ENABLE + 3)] = *ps++;
        }

//		spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)spi_debug_buff, RECORDING_CARD_MIC_FRAME_SIZE * 5 * 2 + 8, SPI_MODE_WR_WRITE_ONLY);
#endif
#endif

        ///write data to DSP
#if TLK_MW_DSP_COMM_ENABLE
        if (s_recording_card_env.nn_alg_en) {
            app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(BBF_NN_NS_16K_20MS_ID);
            p_dsp_app_ctx->alg_type          = VAD_NN_NS;
            d25f_send_audio_data_to_dsp((uint8_t *)data_buff_stereo, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_int), BBF_NN_NS_16K_20MS_ID);
            tlkmdi_recording_card_process_pcm_from_dsp((int8_t *)data_buff_mono);
        } else {
            ps = (int16_t *)data_buff_stereo;
            pd = (int16_t *)data_buff_mono;
            for (int16_t j = 0; j < 16 * 20;) {
                pd[j++] = *ps++;
                ps++;
            }
        }
#elif TLKALG_NN_NS_VAD_ENABLE
        if (s_recording_card_env.nn_alg_en) {
            audio_alg_interface_t *p_alg_nn_ns = audio_alg_get_interface_by_type(ALG_NN_NS_VAD);
            int                    ret_num     = p_alg_nn_ns->audio_alg_process((uint8_t *)data_buff_mono, (uint8_t *)&ret, (uint16_t)RECORDING_CARD_MIC_FRAME_SIZE, 0, 0);

            if (ret_num) {
                tlkapi_printf(APP_LOG_EN, "tlka_nn_ns_vad_process_frame:ERR %d", ret_num);
            }
        }
#else
        ps = (int16_t *)data_buff_stereo;
        pd = (int16_t *)data_buff_mono;
        for (int16_t j = 0; j < 16 * 20;) {
            pd[j++] = *ps++;
            ps++;
        }
#endif
    }

    tmemcpy(data_buff, data_buff_mono, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));

    return ret;
}
#endif

#if (TLKALG_BBF_ENABLE == TLKALG_BBF_DIS)
int8_t tlkmdi_recording_card_fifo_irq_process(adc_mono_int *data_buff)
{
    adc_mono_int   data_buff_mono[RECORDING_CARD_MIC_FRAME_SIZE];
    static uint8_t recording_frame_cnt = 0;
    static short   last_pcm_sample     = 0;


    uint16_t wptr            = audio_get_rx_wptr(TLKDRV_CODEC_MIC_FIFO);
    uint16_t threshold_value = 0;
    if (wptr >= RECORDING_CARD_MIC_FRAME_SIZE / 2) {
        threshold_value = wptr - RECORDING_CARD_MIC_FRAME_SIZE / 2;
    }
    audio_set_rx_wptr(TLKDRV_CODEC_MIC_FIFO, threshold_value);

    int8_t ret = tlkdrv_codec_readMicData((uint8_t *)(data_buff_mono), RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int), 0); //sizeof(adc_mono_int)
    if (s_recording_card_env.first_frame) {
        //		tlkapi_printf(APP_LOG_EN, "tlkdrv_codec_readMicData %d",tlkdrv_codec_getMicOffset());
        recording_frame_cnt++;
        if (recording_frame_cnt > 9) {
            for (int16_t i = 0; i < RECORDING_CARD_MIC_FRAME_SIZE; i++) {
                data_buff_mono[i] = data_buff_mono[i] * i / RECORDING_CARD_MIC_FRAME_SIZE;
            }
            s_recording_card_env.first_frame = 0;
            recording_frame_cnt              = 0;
        } else {
            if (recording_frame_cnt == 2) {
                s_recording_card_env.alg_stat_chg = 0;
            }
            tmemset((uint8_t *)(data_buff_mono), 0x00, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));
        }
    }

    if (ret) {
#if TLKALG_GET_AUDIO_DATA_EN
        static uint16_t index           = 0;
        spi_debug_buff[0]               = 0x0F;
        spi_debug_buff[1]               = 0x0F;
        spi_debug_buff[2]               = 0x0F;
        spi_debug_buff[3]               = 0x0F;
        *(uint16_t *)&spi_debug_buff[4] = TLKALG_BBF_ENABLE + 3;
        *(uint16_t *)&spi_debug_buff[6] = index++;

        int16_t *ps = (int16_t *)data_buff_mono;
        int16_t *pd = (int16_t *)&spi_debug_buff[8];
        for (int16_t j = 0; j < RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE + 3);) {
            pd[j++] = *ps++;
            pd[j++] = 0x00;
            pd[j++] = 0x00;
        }
//		spi_master_write_dma_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (uint8_t *)rcd_nch_data_buff, RECORDING_CARD_MIC_FRAME_SIZE * 5 * 2 + 8, SPI_MODE_WR_WRITE_ONLY);
#endif

#if TLK_MW_DSP_COMM_ENABLE
        ///write data to DSP
        if (s_recording_card_env.nn_alg_en) {
            app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(NN_NS_16K_20MS_ID);
            p_dsp_app_ctx->alg_type          = VAD_NN_NS;
            d25f_send_audio_data_to_dsp((uint8_t *)data_buff_mono, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int), NN_NS_16K_20MS_ID);
            tlkmdi_recording_card_process_pcm_from_dsp((int8_t *)data_buff_mono);
        } else
#endif
#if TLKALG_NN_NS_VAD_ENABLE
            if (s_recording_card_env.nn_alg_en) {

            audio_alg_interface_t *p_alg_nn_ns = audio_alg_get_interface_by_type(ALG_NN_NS_VAD);
            int                    ret_num     = p_alg_nn_ns->audio_alg_process((uint8_t *)data_buff_mono, (uint8_t *)&ret, (uint16_t)RECORDING_CARD_MIC_FRAME_SIZE, 0, 0);

            if (ret_num) {
                tlkapi_printf(APP_LOG_EN, "tlka_nn_ns_vad_process_frame:ERR %d", ret_num);
            }
        }
#endif
        {
        }
    }

    if (s_recording_card_env.alg_stat_chg) {
        recording_frame_cnt++;
        if (s_recording_card_env.nn_alg_en) {
            s_recording_card_env.alg_stat_chg = 0;
            recording_frame_cnt               = 0;
        } else {
            if (recording_frame_cnt == 2) {
                for (int16_t i = 0; i < RECORDING_CARD_MIC_FRAME_SIZE; i++) {
                    data_buff_mono[i] = data_buff_mono[i] * i / RECORDING_CARD_MIC_FRAME_SIZE;
                }
                s_recording_card_env.alg_stat_chg = 0;
                recording_frame_cnt               = 0;
            } else if (recording_frame_cnt == 1) {
                for (int16_t i = 0; i < RECORDING_CARD_MIC_FRAME_SIZE; i++) {
                    data_buff_mono[i] = last_pcm_sample * (RECORDING_CARD_MIC_FRAME_SIZE - i) / RECORDING_CARD_MIC_FRAME_SIZE;
                }
            }
            last_pcm_sample = data_buff_mono[RECORDING_CARD_MIC_FRAME_SIZE - 1];
        }
    }
    tmemcpy(data_buff, data_buff_mono, RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));


    return ret;
}
#endif

void tlkmdi_recording_card_fifo_irq_handler_func(void)
{
    adc_mono_int data_buff_mono[RECORDING_CARD_MIC_FRAME_SIZE] = {0};
    int8_t       ret                                           = 0;

#if (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN)
    ret = tlkmdi_recording_card_fifo_irq_process_6(data_buff_mono);
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_4CH_EN)
    ret = tlkmdi_recording_card_fifo_irq_process_4(data_buff_mono);
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_2CH_EN)
    ret = tlkmdi_recording_card_fifo_irq_process_2(data_buff_mono);
#else
    ret = tlkmdi_recording_card_fifo_irq_process(data_buff_mono);
#endif

    if ((!VAD_FUNCTION_ENABLE) || ret) {
        tlkmdi_recording_card_opus_process(data_buff_mono);
    }
}

#if TLK_MW_DSP_COMM_ENABLE
/**
 * @brief       This function processes DSP messages for the recording card.
 * @param[in]   enc_buff_wptr - write pointer of the encoded buffer.
 * @param[in]   type - message type.
 * @return      none.
 */
#define TLKMDI_RECORDING_CARD_DSP_QUEUE_SIZE 4

typedef struct
{
    uint32_t wptr;
    uint32_t rptr;
    uint8_t  buffer[TLKMDI_RECORDING_CARD_DSP_QUEUE_SIZE][RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int)];
} tlkmdi_recording_card_dsp_queue_t;

static tlkmdi_recording_card_dsp_queue_t s_tlkmdi_recording_card_dsp_queue;

void tlkmdi_recording_card_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type)
{
    (void)enc_buff_wptr;
    (void)type;
    //get data processed by nn_ns in dsp
    // gpio_set_high_level(GPIO_CHN4);
    //    tlkapi_printf(APP_LOG_EN, "tlkmdi_recording_card_dsp_msg_process_callback");
    uint16_t pcm_data_len;
#if 0 //TLKALG_BBF_ENABLE
    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&pcm_data_len, BBF_NN_NS_16K_20MS_ID);
#else
    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&pcm_data_len, NN_NS_16K_20MS_ID);
#endif
    if (pcm_data == NULL) {
        tlkapi_printf(APP_LOG_EN, "interphone pcm_data is NULL");
        return;
    }
    if (!s_recording_card_env.nn_alg_en) {
        return;
    }
    tlkmdi_recording_card_dsp_queue_t *queue = &s_tlkmdi_recording_card_dsp_queue;
    if ((queue->wptr + 1) % TLKMDI_RECORDING_CARD_DSP_QUEUE_SIZE == queue->rptr) {
        return; //queue full,can't happened must have bug.
    }
    memcpy(queue->buffer[s_tlkmdi_recording_card_dsp_queue.wptr], pcm_data, pcm_data_len);
    queue->wptr = (queue->wptr + 1) % TLKMDI_RECORDING_CARD_DSP_QUEUE_SIZE;
    // gpio_set_low_level(GPIO_CHN4);
}

void tlkmdi_recording_card_process_pcm_from_dsp(int8_t *pcm)
{
    tlkmdi_recording_card_dsp_queue_t *queue = &s_tlkmdi_recording_card_dsp_queue;

    if (queue->wptr != queue->rptr) {
        //        tlkmdi_recording_card_opus_process((adc_mono_int*)(queue->buffer[queue->rptr]));
        tmemcpy(pcm, queue->buffer[queue->rptr], RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));
        queue->rptr = (queue->rptr + 1) % TLKMDI_RECORDING_CARD_DSP_QUEUE_SIZE;
    } else {
        s_recording_card_env.alg_stat_chg = 1;
        tlkapi_printf(APP_LOG_EN, "s_tlkmdi_recording_card_dsp_queue %d ,%d", queue->rptr, queue->wptr);
        //    	tmemset(pcm,0x00,RECORDING_CARD_MIC_FRAME_SIZE * sizeof(adc_mono_int));
    }
}

void tlkmdi_recording_card_clean_dsp_queue(void)
{
    tlkmdi_recording_card_dsp_queue_t *queue = &s_tlkmdi_recording_card_dsp_queue;
    queue->rptr = queue->wptr = 0;
}

#endif
#endif
