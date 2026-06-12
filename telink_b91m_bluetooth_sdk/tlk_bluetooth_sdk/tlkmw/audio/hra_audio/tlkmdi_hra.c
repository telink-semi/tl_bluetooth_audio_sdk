/********************************************************************************************************
 * @file    tlkmdi_hra.c
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
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "tlkmdi_hra.h"

#if (TLK_CFG_HRA_ENABLE)

static uint8_t s_tlkmdi_hra_i2s0_tdm_tx_dma = 0;
static uint8_t s_tlkmdi_hra_i2s0_tdm_rx_dma = 0;
static uint8_t s_tlkmdi_hra_codec_micb_dma  = 0;

#define I2S0_TDM_TX_DMA s_tlkmdi_hra_i2s0_tdm_tx_dma
#define I2S0_TDM_RX_DMA s_tlkmdi_hra_i2s0_tdm_rx_dma
#define CODEC_MICB_DMA  s_tlkmdi_hra_codec_micb_dma

unsigned short audio_tdm_4_chn_48k_config[5] = {1, 3, 1, 128, 128}; /* sampling rate = 36.864MHz * (1 / 2) / (2 * 1) / (128)  = 48KHz */
unsigned short audio_tdm_4_chn_16k_config[5] = {1, 3, 3, 128, 128}; /* sampling rate = 36.864MHz * (1 / 2) / (2 * 1) / (128)  = 48KHz */

tlkmdi_hra_env_t s_hra_env_t = {0};
short            buff_tdm_tx[TDM_TX_FIFO_SIZE];
short            buff_tdm_rx[TDM_RX_FIFO_SIZE];
#if MIC_CHNL_NUM4_EN
adc_int g_codec_micb_buff[CODEC_MIC_FIFO_SAMPLES];
#endif

#if JTAG_DEBUG_ENABLE
/**
 * @brief       Initialize DSP JTAG for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_dsp_jtag_init(void)
{
    dsp_jtag_pin_st dsp_jtag_pin;

    dsp_jtag_pin.tck = GPIO_PE4; //new board
    dsp_jtag_pin.tms = GPIO_PE5;
    dsp_jtag_pin.tdo = GPIO_PE6;
    dsp_jtag_pin.tdi = GPIO_PE7;

    dsp_jtag_enable(&dsp_jtag_pin);

    reg_dsp_rst0 |= BIT(2);
}

/**
 * @brief       Confirm DSP acknowledge overtime
 * @param       None
 * @return      None
 */
void app_audio_dsp_ack_overtime_confirm(void)
{
    if (!s_hra_env_t.dsp2d25f_irq_flag && !s_hra_env_t.overtime_flag && s_hra_env_t.send_status) {
        if (clock_time_exceed(s_hra_env_t.send_tick, 1500)) {
            uint16_t sample_num = (s_hra_env_t.samplerate / 1000 * 2);
            short    mute_data[48 * 2];

            s_hra_env_t.overtime_flag = true;

            for (int i = 0; i < sample_num; i++) {
                mute_data[i] = 0;
            }

            tlkdrv_codec_fillSpkBuff((uint8_t *)mute_data, sample_num * 2);
        } else {
        }
    } else {
    }
}

/**
 * @brief       Handle timer interrupt for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_timer_irq_handler(void)
{
    tlkmdi_audio_stop_timer();
    tlkmdi_audio_task_set_next_irq(TIMER0_INTERVAL_US);

    if (clock_time_exceed(s_hra_env_t.loop_tick, 300)) {
        tlkmdi_hra_main_loop();
    }
}

/**
 * @brief       Initialize timer for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_timer_init(void)
{
#if (MCU_CORE_TL752X_TEMP)
    plic_interrupt_enable(IRQ_TIMER0_0);
    plic_set_priority(IRQ_TIMER0_0, 1);
#else
    plic_interrupt_enable(IRQ_TIMER0);
    plic_set_priority(IRQ_TIMER0, 1);
#endif
    tlkmdi_audio_task_set_next_irq(TIMER0_INTERVAL_US);
}
#endif

/**
 * @brief       Initialize HRA module
 * @param       None
 * @return      1 - Success
 */
int tlkmdi_hra_init(void)
{
    I2S0_TDM_TX_DMA = tlkhal_dma_malloc();
    I2S0_TDM_RX_DMA = tlkhal_dma_malloc();
    CODEC_MICB_DMA  = tlkhal_dma_malloc();

#if JTAG_DEBUG_ENABLE
    tlkmdi_hra_dsp_jtag_init();
#endif

#if TLKALG_HRA_16K_EN
    s_hra_env_t.samplerate = 16000;
#else
    s_hra_env_t.samplerate = 48000;
#endif

    return 1;
}

/**
 * @brief       Switch HRA state
 * @param[in]   handle - Connection handle
 * @param[in]   status - Status to switch to (1: enable, 0: disable)
 * @return      true - Operation successful
 */
bool tlkmdi_hra_switch(uint16_t handle, uint8_t status)
{
    tlkapi_printf(APP_LOG_EN, "tlkmdi_hra_switch: handle:%d, status: %d", handle, status);

    if (status == 1) {
        // gpio_set_high_level(GPIO_CHN1);
        s_hra_env_t.enable = 1;
#if JTAG_DEBUG_ENABLE
        tlkmdi_hra_dsp_jtag_init();
        tlkmdi_hra_timer_init();
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, tlkmdi_hra_timer_irq_handler);
#endif
#if (TLK_MW_DSP_COMM_ENABLE)
        tlkmw_dsp_resume();
#endif
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_FIFO, tlkmdi_hra_fifo0_irq_handler_func);
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_MAIN, tlkmdi_hra_main_loop);
        ipc_msg_register_data_process_done_cb(tlkmdi_hra_dsp_msg_process_callback, HRA_ALG, HRA_AUDIO_ID);
        tlkmdi_hra_open_codec();
        tlkdrv_codec_muteSpkBuff();
#if TLKALG_MIC_SPK_MONO_ENABLE
        tlkmdi_hra_set_fifo_irq_num(s_hra_env_t.samplerate / 1000 * 1 * MCU2DSP_DATA_MS * MCU2DSP_DATA_WIDTH_BYTE);
#else
        tlkmdi_hra_set_fifo_irq_num(s_hra_env_t.samplerate / 1000 * MCU2DSP_MICA_CHNL_NUM * MCU2DSP_DATA_MS * MCU2DSP_DATA_WIDTH_BYTE);
#endif
        // tlkmdi_hra_tdm_init();
        // d25f_init_ipc_buffer();
        // gpio_set_low_level(GPIO_CHN1);
    } else {
        // gpio_set_high_level(GPIO_CHN1);
        s_hra_env_t.enable      = 0;
        s_hra_env_t.start_flag  = false;
        s_hra_env_t.send_status = 0;
#if JTAG_DEBUG_ENABLE
        tlkmdi_audio_stop_timer();
#endif
        // #if (TLK_MW_DSP_COMM_ENABLE)
        // tlkmw_dsp_pause();
        // #endif
        // tlkmdi_hra_tdm_disable();
        tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_BOTH);
        audio_fifo_irq_dis(AUDIO_RX_FIFO0);
        // gpio_set_low_level(GPIO_CHN1);
    }
    return 1;
}

/**
 * @brief       Check if HRA is busy
 * @param       None
 * @return      true - HRA is busy, false - HRA is idle
 */
bool tlkmdi_hra_is_busy(void)
{
    return s_hra_env_t.enable;
}

/**
 * @brief       Callback function for processing DSP messages
 * @param[in]   enc_buff_wptr - Encode buffer write pointer
 * @param[in]   type - Message type
 * @return      None
 */
void tlkmdi_hra_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type)
{
    (void)enc_buff_wptr;
    (void)type;

    //put data to playback buffer
    uint16_t pcm_data_len;
    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&pcm_data_len, HRA_AUDIO_ID);
    if (pcm_data == NULL) {
        tlkapi_printf(APP_LOG_EN, "hra pcm_data is NULL");
        return;
    }

    short   *pdata = (short *)pcm_data;
    short    play_data[48 * 2 * 2]; //48k*2ms*2chn
    uint16_t sample_per_frame = pcm_data_len / MCU2DSP_TOTAL_CHNL_NUM / MCU2DSP_DATA_WIDTH_BYTE;
#if MIC_CHNL_NUM4_EN
    short *pdata_micb = pdata + sample_per_frame * MCU2DSP_MICA_CHNL_NUM;
#endif

#if TLKALG_MIC_SPK_MONO_ENABLE || TLKALG_MIC_STEREO_SPK_MONO_ENABLE
    for (int i = 0; i < sample_per_frame; i++) {
        play_data[i] = pdata[2 * i];
        // play_data[i] = 0;
    }
    // gpio_set_high_level(GPIO_CHN4);
    tlkdrv_codec_fillSpkBuff((uint8_t *)play_data, sample_per_frame * 1 * MCU2DSP_DATA_WIDTH_BYTE);
// gpio_set_low_level(GPIO_CHN4);
#else
    for (int i = 0; i < sample_per_frame; i++) {
        play_data[2 * i] = pdata[2 * i];
#if MIC_CHNL_NUM4_EN
        play_data[2 * i + 1] = pdata_micb[2 * i];
#else
        play_data[2 * i + 1] = pdata[2 * i + 1];
#endif
    }

    tlkdrv_codec_fillSpkBuff((uint8_t *)play_data, sample_per_frame * 2 * MCU2DSP_DATA_WIDTH_BYTE);
#endif
    if (s_hra_env_t.tdm_status == TLKMDI_HRA_TDM_EN) {
        for (int j = 0; j < sample_per_frame; j++) {
            buff_tdm_tx[s_hra_env_t.tdm_tx_wptr++ & TDM_TX_FIFO_MAX] = pdata[sample_per_frame * MCU2DSP_MIC_CHNL_NUM + MCU2DSP_TDM_CHNL_NUM * j];
            buff_tdm_tx[s_hra_env_t.tdm_tx_wptr++ & TDM_TX_FIFO_MAX] = pdata[sample_per_frame * MCU2DSP_MIC_CHNL_NUM + MCU2DSP_TDM_CHNL_NUM * j + 1];
            buff_tdm_tx[s_hra_env_t.tdm_tx_wptr++ & TDM_TX_FIFO_MAX] = pdata[sample_per_frame * MCU2DSP_MIC_CHNL_NUM + MCU2DSP_TDM_CHNL_NUM * j + 2];
            buff_tdm_tx[s_hra_env_t.tdm_tx_wptr++ & TDM_TX_FIFO_MAX] = pdata[sample_per_frame * MCU2DSP_MIC_CHNL_NUM + MCU2DSP_TDM_CHNL_NUM * j + 3];
        }
    }
}

/**
 * @brief       Main loop for HRA processing
 * @param       None
 * @return      None
 */
void tlkmdi_hra_main_loop(void)
{
#if JTAG_DEBUG_ENABLE
    s_hra_env_t.loop_tick = stimer_get_tick();
    app_audio_dsp_ack_overtime_confirm();
#endif
}

/**
 * @brief       Handle FIFO0 interrupt for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_fifo0_irq_handler_func(void)
{
    uint16_t wptr            = audio_get_rx_wptr(AUDIO_RX_FIFO);
    uint16_t threshold_value = 0;
    if (wptr != 0 && wptr >= 32) {
        threshold_value = wptr - 32;
    }

    audio_set_rx_wptr(AUDIO_RX_FIFO, threshold_value);

    //send data to dsp
    if (!s_hra_env_t.send_status) {
        s_hra_env_t.send_status = true;

        tlkdrv_codec_sync_speaker_samples(s_hra_env_t.samplerate / 1000);     //sample rate*ms*chnl
        tlkdrv_codec_sync_mic_samples(s_hra_env_t.samplerate / 1000 * 2 + 4); //sample rate*ms*chnl
#if MIC_CHNL_NUM4_EN
        tlkmdi_hra_sync_micb_samples(s_hra_env_t.samplerate / 1000 * 2 + 4);
#endif
        codec_sync_tdm_rx_samples((s_hra_env_t.samplerate / 1000) * MCU2DSP_DATA_MS * MCU2DSP_TDM_CHNL_NUM);
        codec_sync_tdm_tx_samples((s_hra_env_t.samplerate / 1000) * MCU2DSP_DATA_MS * MCU2DSP_TDM_CHNL_NUM);
    }

#if 0
    // uint16_t send_data[6];
    // send_data[0] = tlkdrv_codec_get_mic_wptr();
    // send_data[1] = tlkdrv_codec_getMicOffset()/sizeof(adc_int);
    // send_data[2] = (send_data[0] - send_data[1]) & (CODEC_MIC_FIFO_SAMPLES - 1);
    // send_data[3] = tlkdrv_codec_getSpkOffset()/sizeof(codec_int);
    // send_data[4] = tlkdrv_codec_get_speaker_rptr();
    // send_data[5] = (send_data[3] - send_data[4]) & (CODEC_SPK_FIFO_SAMPLES - 1);

    // spi_master_write_dma(0, (uint8_t *)send_data, 12);
    // while(spi_is_busy(0));
#endif
    // gpio_set_high_level(GPIO_CHANNEL2);
    // gpio_set_low_level(GPIO_CHANNEL2);

    //get mic data
    //mic data: adca_micl | adca_micr | adcb_micl | adcb_micr | i2s_tdm0 | i2s_tdm1 | i2s_tdm2 | i2s_tdm3
    short data_buff[48 * MCU2DSP_DATA_MS * MCU2DSP_TOTAL_CHNL_NUM];
#if 1
#if TLKALG_MIC_SPK_MONO_ENABLE
    short data_buff_mono[48 * MCU2DSP_DATA_MS * 2];
    bool  ret = tlkdrv_codec_readMicData((uint8_t *)data_buff_mono, (s_hra_env_t.samplerate / 1000) * 1 * MCU2DSP_DATA_MS * MCU2DSP_DATA_WIDTH_BYTE, 0); //len in byte
#else
    bool ret = tlkdrv_codec_readMicData((uint8_t *)data_buff, (s_hra_env_t.samplerate / 1000) * MCU2DSP_MICA_CHNL_NUM * MCU2DSP_DATA_MS * MCU2DSP_DATA_WIDTH_BYTE, 0); //len in byte
#endif

#if MIC_CHNL_NUM4_EN
    tlkmdi_hra_get_micb_buff_data((uint8_t *)&data_buff[(s_hra_env_t.samplerate / 1000) * MCU2DSP_MICA_CHNL_NUM * MCU2DSP_DATA_MS],
                                  (s_hra_env_t.samplerate / 1000) * MCU2DSP_MICB_CHNL_NUM * MCU2DSP_DATA_MS * MCU2DSP_DATA_WIDTH_BYTE);
#endif
#else
    short hra_sine_data[32] = {
        0, 0, 12539,  12539,  23169,  23169,  30272,  30272,  32767,  32767,  30272,  30272,  23169,  23169,  12539,  12539,
        4, 4, -12539, -12539, -23169, -23169, -30272, -30272, -32767, -32767, -30272, -30272, -23169, -23169, -12539, -12539,
    };
    bool           ret       = true;
    static uint8_t hra_count = 0;
    for (int i = 0; i < 64; i++) {
        data_buff[i] = hra_sine_data[hra_count++];
        hra_count %= 32;
    }
#endif
    if (ret) {
#if TLKALG_MIC_SPK_MONO_ENABLE
        short *p_mono_mic = data_buff;
        for (int i = 0; i < ((s_hra_env_t.samplerate / 1000) * MCU2DSP_DATA_MS * 1); i++) {
            *p_mono_mic++ = data_buff_mono[i];
            *p_mono_mic++ = data_buff_mono[i];
        }

#endif
        app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(HRA_AUDIO_ID);
        p_dsp_app_ctx->alg_type          = HRA_ALG;

        if (s_hra_env_t.tdm_status == TLKMDI_HRA_TDM_EN) {
            short *ptdm = data_buff + ((s_hra_env_t.samplerate / 1000) * MCU2DSP_MIC_CHNL_NUM * MCU2DSP_DATA_MS);
            for (int i = 0; i < ((s_hra_env_t.samplerate / 1000) * MCU2DSP_DATA_MS * MCU2DSP_TDM_CHNL_NUM); i++) {
                *ptdm++ = buff_tdm_rx[s_hra_env_t.tdm_rx_rptr++];
                s_hra_env_t.tdm_rx_rptr &= TDM_RX_FIFO_MAX;
            }
        }

        // gpio_set_high_level(GPIO_CHN2);
        d25f_send_audio_data_to_dsp((uint8_t *)data_buff, (s_hra_env_t.samplerate / 1000) * MCU2DSP_DATA_MS * MCU2DSP_TOTAL_CHNL_NUM * MCU2DSP_DATA_WIDTH_BYTE,
                                    HRA_AUDIO_ID); //len in byte
                                                   // gpio_set_low_level(GPIO_CHN2);

#if JTAG_DEBUG_ENABLE
        s_hra_env_t.dsp2d25f_irq_flag = false;
        s_hra_env_t.overtime_flag     = false;
        s_hra_env_t.send_tick         = stimer_get_tick();
#endif
    } else {
    }
}

/**
 * @brief       Open codec for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_open_codec(void)
{
// tlkdrv_codec_init_env();
// tlkdrv_icodec_hra_flag_enable();
#if TLKALG_MIC_SPK_MONO_ENABLE
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_16, s_hra_env_t.samplerate, 0);
#else
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, s_hra_env_t.samplerate, 0);
#endif

    audio_codec0_input_config_t codec0_input_config_stream0 = {
#if (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
#if TLKALG_MIC_SPK_MONO_ENABLE
        .input_src = AUDIO_DMIC_ADC_A1,
#else
        .input_src = AUDIO_DMIC_ADC_A1_A2,
#endif
#elif (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
#if TLKALG_MIC_SPK_MONO_ENABLE
        .input_src = AUDIO_AMIC_ADC_A1,
#else
        .input_src = AUDIO_AMIC_ADC_A1_A2,
#endif
#else
#if TLKALG_MIC_SPK_MONO_ENABLE
        .input_src = AUDIO_LINEIN_ADC_A1,
#else
        .input_src = AUDIO_LINEIN_ADC_A1_A2,
#endif
#endif
        .data_format = AUDIO_CODEC0_BIT_16_DATA,
        // .sample_rate = AUDIO_16K,
    };

#if MIC_CHNL_NUM4_EN
    audio_codec0_input_config_t codec0_input_config_stream1 = {
#if (CODEC_INPUT_MODE == CODEC_INPUT_DMIC)
        .input_src = AUDIO_DMIC_ADC_B1_B2,
#elif (CODEC_INPUT_MODE == CODEC_INPUT_AMIC)
        .input_src = AUDIO_AMIC_ADC_B1_B2,
#else
        .input_src = AUDIO_LINEIN_ADC_B1_B2,
#endif
        .data_format = AUDIO_CODEC0_BIT_16_DATA,
        // .sample_rate = AUDIO_16K,
    };
#endif

    audio_codec0_output_config_t codec0_output_config = {
#if TLKALG_MIC_SPK_MONO_ENABLE || TLKALG_MIC_STEREO_SPK_MONO_ENABLE
        .output_dst = AUDIO_DAC_A1,
#else
        .output_dst = AUDIO_DAC_A1_A2,
#endif
        .data_format = AUDIO_CODEC0_BIT_16_DATA,
    };

    if (s_hra_env_t.samplerate == 16000) {
        codec0_input_config_stream0.sample_rate = AUDIO_16K;
        codec0_output_config.sample_rate        = AUDIO_16K;
#if MIC_CHNL_NUM4_EN
        codec0_input_config_stream1.sample_rate = AUDIO_16K;
#endif
    } else if (s_hra_env_t.samplerate == 48000) {
        codec0_input_config_stream0.sample_rate = AUDIO_48K;
        codec0_output_config.sample_rate        = AUDIO_48K;
#if MIC_CHNL_NUM4_EN
        codec0_input_config_stream1.sample_rate = AUDIO_48K;
#endif
    }

    /*********************** input config ****************************/
    if (codec0_input_config_stream0.input_src == AUDIO_DMIC_ADC_A1_A2 || codec0_input_config_stream0.input_src == AUDIO_DMIC_ADC_A1) {
        audio_codec0_set_dmic_a_pin((gpio_func_pin_e)TLKDRV_ICODEC_DMIC_DATA_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK0_PIN, (gpio_func_pin_e)TLKDRV_ICODEC_DMIC_CLK1_PIN);
        audio_codec0_set_input_dgain(codec0_input_config_stream0.input_src, AUDIO_IN_D_GAIN_20_DB);
    } else if (codec0_input_config_stream0.input_src == AUDIO_AMIC_ADC_A1_A2 || codec0_input_config_stream0.input_src == AUDIO_AMIC_ADC_A1) {
        audio_codec0_set_input_again(codec0_input_config_stream0.input_src, AUDIO_IN_A_GAIN_8_DB);
        audio_codec0_set_input_dgain(codec0_input_config_stream0.input_src, AUDIO_IN_D_GAIN_12_DB);
    }

/* matrix input config. */
#if TLKALG_MIC_SPK_MONO_ENABLE
    audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCA, FIFO_RX_CODEC0_ADCA_A1_16BIT);
#else
    audio_matrix_set_rx_fifo_route(TLKDRV_CODEC_MIC_FIFO, FIFO_RX_ROUTE_CODEC0_ADCA, FIFO_RX_CODEC0_ADCA_A1_A2_16BIT);
#endif /* rx dma init. */

    audio_rx_dma_chain_init(TLKDRV_CODEC_MIC_FIFO, gTlkdrvCodecMicDmaChn, (unsigned short *)g_codec_mic_buff, sizeof(g_codec_mic_buff)); //FIFO0 -> buff_record
    audio_rx_dma_en(gTlkdrvCodecMicDmaChn); /* the rx dma enable must precede the adc enable. */
    audio_codec0_input_init(&codec0_input_config_stream0);

#if MIC_CHNL_NUM4_EN
    if (codec0_input_config_stream1.input_src == AUDIO_DMIC_ADC_B1_B2) {
        audio_codec0_set_dmic_b_pin((gpio_func_pin_e)HRA_DMIC_DATA_PIN, (gpio_func_pin_e)HRA_DMIC_CLK0_PIN, (gpio_func_pin_e)HRA_DMIC_CLK1_PIN);
        audio_codec0_set_input_dgain(codec0_input_config_stream1.input_src, AUDIO_IN_D_GAIN_20_DB);
    } else if (codec0_input_config_stream1.input_src == AUDIO_AMIC_ADC_B1_B2) {
        audio_codec0_set_input_again(codec0_input_config_stream1.input_src, AUDIO_IN_A_GAIN_8_DB);
        audio_codec0_set_input_dgain(codec0_input_config_stream1.input_src, AUDIO_IN_D_GAIN_12_DB);
    }
    /* matrix input config. */
    audio_matrix_set_rx_fifo_route(CODEC_MICB_FIFO, FIFO_RX_ROUTE_CODEC0_ADCB, FIFO_RX_CODEC0_ADCB_B1_B2_16BIT);

    /* rx dma init. */
    audio_rx_dma_chain_init(CODEC_MICB_FIFO, CODEC_MICB_DMA, (unsigned short *)g_codec_micb_buff, sizeof(g_codec_micb_buff)); //FIFO3 -> buff_record
    audio_rx_dma_en(CODEC_MICB_DMA);                                                                                          /* the rx dma enable must precede the adc enable. */
    audio_codec0_input_init(&codec0_input_config_stream1);
#endif

#if CODEC_LOW_POWER_MODE_EN
    reg_audio_codec0_cr_adca12 = (reg_audio_codec0_cr_adca12 & (~FLD_CODEC0_ADCA_POWER_MODE)) | MASK_VAL(FLD_CODEC0_ADCA_POWER_MODE, AUDIO_CODEC0_ADC_ULTRA_LOW_POWER_MODE);
#endif
/*********************** output config ****************************/

/* matrix output config. */
#if TLKALG_MIC_SPK_MONO_ENABLE || TLKALG_MIC_STEREO_SPK_MONO_ENABLE
    audio_matrix_set_dac_route(codec0_output_config.output_dst, DAC_ROUTE_FIFO, DAC_FIFO_MONO_16BIT_FIFO0);
#else
    audio_matrix_set_dac_route(codec0_output_config.output_dst, DAC_ROUTE_FIFO, DAC_FIFO_STEREO_16BIT_FIFO0);
#endif

    audio_codec0_set_output_again(codec0_output_config.output_dst, AUDIO_OUT_A_GAIN_0_DB);
    audio_codec0_set_output_dgain(codec0_output_config.output_dst, AUDIO_OUT_D_GAIN_0_DB);

    /* tx dma init. */
    audio_tx_dma_chain_init(TLKDRV_CODEC_SPK_FIFO, gTlkdrvCodecSpkDmaChn, (unsigned short *)g_codec_spk_buff, sizeof(g_codec_spk_buff));
    audio_tx_dma_en(gTlkdrvCodecSpkDmaChn);
    audio_codec0_output_init(&codec0_output_config);
}

/**
 * @brief       Set FIFO interrupt number
 * @param[in]   byte_num - Number of bytes for interrupt trigger
 * @return      None
 */
//byte_num:samplerate*chnl*ms*Byte
void tlkmdi_hra_set_fifo_irq_num(uint16_t byte_num)
{
    plic_interrupt_enable(IRQ_DFIFO);
    audio_fifo_ptr_en(AUDIO_RX_FIFO0);
    audio_set_rx_wptr(AUDIO_RX_FIFO, 0);
    audio_set_rx_buff_thres(AUDIO_RX_FIFO, byte_num);

    audio_fifo_irq_en(AUDIO_RX_FIFO0); //enable RX_FIFO0
}

/**
 * @brief       Clear send status
 * @param       None
 * @return      None
 */
void tlkmdi_hra_clear_send_status(void)
{
    s_hra_env_t.send_status = false;
}

/**
 * @brief       Set sample rate
 * @param[in]   sr - Sample rate to set
 * @return      None
 */
void tlkmdi_hra_set_samplerate(uint16_t sr)
{
    if (s_hra_env_t.samplerate == sr) {
        tlkapi_printf(APP_LOG_EN, "The same sample rate");
        return;
    }

    s_hra_env_t.samplerate = sr;
    // tlkmdi_hra_set_fifo_irq_num(s_hra_env_t.samplerate/1000 * 2 * 2 * 2);
    tlkmdi_hra_open_codec();
    // tlkmdi_hra_tdm_init();
    tlkmdi_hra_clear_send_status();
}

/**
 * @brief       Initialize TDM for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_tdm_init(void)
{
    i2s_pin_config_t i2s_pin_config = {
        .bclk_pin       = HRA_I2S_BCLK_PIN, //I2S_BCLK_PA0  I2S_BCLK_PA5
        .adc_lr_clk_pin = HRA_I2S_ADC_LRCLK_PIN,
        .adc_dat_pin    = HRA_I2S_ADC_DAT_PIN,
        .dac_lr_clk_pin = HRA_I2S_DAC_LRCLK_PIN,
        .dac_dat_pin    = HRA_I2S_DAC_DAT_PIN,
    };

    audio_i2s_config_t audio_i2s_config = {
        .i2s_select        = I2S0,
        .i2s_mode          = I2S_TDM_MODE,
        .tdm_mode          = I2S_TDM_MODE_C,
        .tdm_slot_width    = I2S_TDM_SLOT_WIDTH_32,
        .pin_config        = &i2s_pin_config,
        .data_width        = I2S_BIT_16_DATA,
        .master_slave_mode = I2S_AS_MASTER_EN,
        .io_mode           = I2S_5_LINE_MODE,
#if (TLKALG_HRA_16K_EN)
        .sample_rate = audio_tdm_4_chn_16k_config,
#else
        .sample_rate = audio_tdm_4_chn_48k_config,
#endif
    };

    if (s_hra_env_t.samplerate == 16000) {
        audio_i2s_config.sample_rate = audio_tdm_4_chn_16k_config;
    } else if (s_hra_env_t.samplerate == 48000) {
        audio_i2s_config.sample_rate = audio_tdm_4_chn_48k_config;
    }
    audio_i2s_config_init(&audio_i2s_config);
    audio_i2s_clk_en(I2S0);

    /* matrix input config */
    audio_matrix_set_rx_fifo_route(I2S0_TDM_RX_FIFO, FIFO_RX_ROUTE_I2S0_RX, FIFO_RX_I2S0_TDM_16); /* fifo0 source select i2s0 */

    /* matrix output config */
    audio_matrix_set_i2s_tx_route(I2S0_CHN0, I2S_TX_ROUTE_FIFO, 0x21); /* i2s0_ch0 sel fifo0 */ //I2S0_TX_FIFO0_20_OR_24_TDM_I2S0
    audio_matrix_set_i2s_tx_route(I2S0_CHN1, I2S_TX_ROUTE_FIFO, 0x21); /* i2s0_ch1 sel fifo0 */ //0x1c--24bit	0x20--16bit
    audio_matrix_set_i2s_tx_route(I2S0_CHN2, I2S_TX_ROUTE_FIFO, 0x21);                          /* i2s0_ch2 sel fifo0 */
    audio_matrix_set_i2s_tx_route(I2S0_CHN3, I2S_TX_ROUTE_FIFO, 0x21);                          /* i2s0_ch3 sel fifo0 */

    //    audio_tx_dma_chain_init(I2S0_TDM_TX_FIFO, I2S0_TDM_TX_DMA, (unsigned short *)tdm_tx_sine_16k, sizeof(tdm_tx_sine_16k));
    audio_tx_dma_chain_init(I2S0_TDM_TX_FIFO, I2S0_TDM_TX_DMA, (unsigned short *)buff_tdm_tx, sizeof(buff_tdm_tx));
    audio_rx_dma_chain_init(I2S0_TDM_RX_FIFO, I2S0_TDM_RX_DMA, (unsigned short *)buff_tdm_rx, sizeof(buff_tdm_rx));
}

/**
 * @brief       Enable TDM for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_tdm_enable(void)
{
    if (s_hra_env_t.tdm_status != TLKMDI_HRA_TDM_EN) {
        s_hra_env_t.tdm_status = TLKMDI_HRA_TDM_EN;
        dma_chn_en(I2S0_TDM_TX_DMA);
        dma_chn_en(I2S0_TDM_RX_DMA);
    }
}

/**
 * @brief       Disable TDM for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_tdm_disable(void)
{
    if (s_hra_env_t.tdm_status != TLKMDI_HRA_TDM_DIS) {
        s_hra_env_t.tdm_status = TLKMDI_HRA_TDM_DIS;
        dma_chn_dis(I2S0_TDM_TX_DMA);
        dma_chn_dis(I2S0_TDM_RX_DMA);
    }
}
#if MIC_CHNL_NUM4_EN
/**
 * @brief       Synchronize MICB samples
 * @param[in]   samples - Number of samples to synchronize
 * @return      None
 */
void tlkmdi_hra_sync_micb_samples(uint16_t samples)
{
    uint16_t wptr         = ((audio_get_rx_dma_wptr(CODEC_MICB_DMA)) - ((uint32_t)g_codec_micb_buff)) / sizeof(adc_int);
    s_hra_env_t.micb_rptr = (wptr - samples) & (CODEC_MIC_FIFO_SAMPLES - 1);
}

/**
 * @brief       Get available samples in MICB buffer
 * @param       None
 * @return      Available samples count
 */
uint16_t tlkmdi_hra_get_micb_buff_available_sample(void)
{
    uint16_t available = 0;
    uint16_t wptr      = ((audio_get_rx_dma_wptr(CODEC_MICB_DMA)) - ((uint32_t)g_codec_micb_buff)) / sizeof(adc_int);
    if (wptr > s_hra_env_t.micb_rptr) {
        available = wptr - s_hra_env_t.micb_rptr;
    } else {
        available = CODEC_MIC_FIFO_SAMPLES + wptr - s_hra_env_t.micb_rptr;
    }

    return available;
}

/**
 * @brief       Get MICB buffer data
 * @param[out]  pBuffer - Buffer to store data
 * @param[in]   buffLen - Buffer length
 * @return      true - Success, false - Failure
 */
bool tlkmdi_hra_get_micb_buff_data(uint8_t *pBuffer, uint16_t buffLen)
{
    uint16_t tempLen;
    uint16_t buff_len_byte = CODEC_MIC_FIFO_SAMPLES * sizeof(adc_int);
    uint16_t dataLen       = tlkmdi_hra_get_micb_buff_available_sample() * sizeof(adc_int);

    if (dataLen < buffLen) {
        return false;
    }

    uint32_t rptr = s_hra_env_t.micb_rptr * sizeof(adc_int);

    if (rptr + buffLen <= buff_len_byte) {
        tempLen = buffLen;
    } else {
        tempLen = buff_len_byte - rptr;
    }

    if (tempLen != 0) {
        tmemcpy(pBuffer, ((uint8_t *)g_codec_micb_buff) + rptr, tempLen);
    }

    if (tempLen == buffLen) {
        rptr += tempLen;
    } else {
        rptr = buffLen - tempLen;
        tmemcpy(pBuffer + tempLen, ((uint8_t *)g_codec_micb_buff), rptr);
    }

    rptr &= (buff_len_byte - 1);

    s_hra_env_t.micb_rptr = rptr / sizeof(adc_int);

    return true;
}
#endif

/**
 * @brief       Get TDM TX read pointer
 * @param       None
 * @return      TDM TX read pointer
 */
uint32_t codec_get_tdm_tx_rptr(void)
{
    return ((audio_get_tx_dma_rptr(I2S0_TDM_TX_DMA) - (uint32_t)buff_tdm_tx) / sizeof(short));
}

/**
 * @brief       Get TDM RX write pointer
 * @param       None
 * @return      TDM RX write pointer
 */
uint32_t codec_get_tdm_rx_wptr(void)
{
    return ((audio_get_rx_dma_wptr(I2S0_TDM_RX_DMA) - (uint32_t)buff_tdm_rx) / sizeof(short));
}

/**
 * @brief       Synchronize TDM TX samples
 * @param[in]   samples - Number of samples to synchronize
 * @return      None
 */
void codec_sync_tdm_tx_samples(uint16_t samples)
{
    uint32_t rptr           = codec_get_tdm_tx_rptr();
    s_hra_env_t.tdm_tx_wptr = (rptr + samples) & TDM_TX_FIFO_MAX;

    if ((s_hra_env_t.tdm_tx_wptr % 4) != 0) {
        s_hra_env_t.tdm_tx_wptr = (s_hra_env_t.tdm_tx_wptr + (s_hra_env_t.tdm_tx_wptr % 4)) & TDM_RX_FIFO_MAX;
    }
}

/**
 * @brief       Synchronize TDM RX samples
 * @param[in]   samples - Number of samples to synchronize
 * @return      None
 */
void codec_sync_tdm_rx_samples(uint16_t samples)
{
    uint32_t wptr           = codec_get_tdm_rx_wptr();
    s_hra_env_t.tdm_rx_rptr = (wptr - samples) & TDM_RX_FIFO_MAX;

    if ((s_hra_env_t.tdm_rx_rptr % 4) != 0) {
        s_hra_env_t.tdm_rx_rptr = (s_hra_env_t.tdm_rx_rptr - (s_hra_env_t.tdm_rx_rptr % 4)) & TDM_RX_FIFO_MAX;
    }
}
#endif
