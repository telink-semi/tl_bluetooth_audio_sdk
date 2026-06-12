/********************************************************************************************************
 * @file    tlkmdi_interphone_handler.c
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
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "tlkmdi_interphone_handler.h"
#if TLKALG_EQ_ENABLE
#include "tlkalg/audio/eq/tlkalg_eq_interface.h"
#endif

#if TLKMW_INTERPHONE_EN
_attribute_iram_data_ codec_int          interphone_i2s_rx_buff[INTERPHONE_I2S_SIZES];
_attribute_iram_data_ adc_int            interphone_i2s_tx_buff[INTERPHONE_I2S_SIZES];
_attribute_iram_data_ volatile codec_int g_interphone_bt_down_buff[CODEC_SPK_FIFO_SAMPLES];
volatile adc_int                         g_interphone_bt_up_buff[CODEC_MIC_FIFO_SAMPLES];

static tlkmdi_interphone_ctl   s_interphone_ctl = {0};
extern tlkmdi_interphone_env_t s_tlk_mdi_interphone_env;

static uint8_t *s_interphone_alg_asrc_16to48_buff = NULL;

adc_mono_int data_buff[MIC_FRAME_SIZE];

#if (TLKALG_ASRC_441TO48_16BIT_ENABLE)
static uint8_t *s_interphone_alg_asrc_441to48_buff = NULL;
#endif

#if (TLKALG_ASRC_441TO48_16BIT_BACKUP_ENABLE)
static uint8_t *s_interphone_alg_asrc_441to48_backup_buff = NULL;
#endif

#if TLKALG_EQ_ENABLE
static uint8_t *s_interphone_alg_eq_buff = NULL;
#endif
#if TLKALG_AGC_ENABLE
extern uint8_t *g_alg_voice_agc_buff;
#endif
#if TLKALG_ANS_ENABLE
extern uint8_t *g_alg_voice_ans_buff;
#endif
#if TLKALG_ANS_SPK_ENABLE
uint8_t *g_alg_ans_spk_buff = NULL;
#endif
#if TLKALG_AEC_ENABLE
uint8_t *g_alg_voice_aec_buff = NULL;
#endif

static uint8_t *s_interphone_alg_sbc_dec_buff = NULL;
static uint8_t *s_interphone_alg_aac_dec_buff = NULL;
#if (TLKALG_PPM_SPK_ENABLE)
static uint8_t *s_interphone_alg_ppm_stereo_buff = NULL;
#endif
static int        interphone_i2s_wptr   = 0;
static int        interphone_i2s_rptr   = 0;
interphone_mode_e interphone_mode_flag  = 0;
interphone_mode_e interphone_voice_flag = 0;
interphone_mode_e interphone_spk_flag   = 0;
uint16_t          one_frame_times       = 0;

unsigned short interphone_i2s_48k_config[5] = {1, 3, 2, 64, 64}; /* sampling rate = pll1_clk(default 36.864MHz) * (1 / 3) / (2 * 2) / (64)  = 48KHz */
unsigned short interphone_i2s_16k_config[5] = {1, 3, 6, 64, 64}; /* sampling rate = pll1_clk(default 36.864MHz) * (1 / 3) / (2 * 6) / (64)  = 16KHz */
#if (TLKHW_TYPE != TLKHW_TL751X_EVK_C1T368A110_V1_0)
i2s_pin_config_t interphone_i2s_pin_config = {
    .bclk_pin       = GPIO_FC_PI2,
    .adc_lr_clk_pin = GPIO_FC_PI5,
    .dac_lr_clk_pin = GPIO_FC_PI5,
    .adc_dat_pin    = GPIO_FC_PI3,
    .dac_dat_pin    = GPIO_FC_PI4,
};
#else
i2s_pin_config_t interphone_i2s_pin_config = {
    .bclk_pin       = GPIO_FC_PB1,
    .adc_lr_clk_pin = GPIO_FC_PB2,
    .dac_lr_clk_pin = GPIO_FC_PB4,
    .adc_dat_pin    = GPIO_FC_PB3,
    .dac_dat_pin    = GPIO_FC_PB5,
};
#endif

audio_i2s_config_t interphone_audio_i2s_config = {
    .i2s_select        = I2S2,
    .i2s_mode          = I2S_I2S_MODE,
    .pin_config        = &interphone_i2s_pin_config,
    .data_width        = I2S_BIT_16_DATA,
    .master_slave_mode = I2S_AS_MASTER_EN,
    .sample_rate       = interphone_i2s_16k_config,
    .io_mode           = I2S_5_LINE_MODE,
};

/**
 * @brief       Initialize I2S TX for interphone
 * @param[in]   out_buff - Output buffer
 * @param[in]   buff_size - Buffer size
 * @return      None
 */
void tlkdrv_interphone_i2s_tx_init(unsigned short *out_buff, unsigned int buff_size)
{
    /* tx dma init. */
    audio_tx_dma_chain_init(INTERPHONE_I2S_FIFO, INTERPHONE_I2S_TX_DMA, out_buff, buff_size);
}

/**
 * @brief       Initialize I2S RX for interphone
 * @param[in]   in_buff - Input buffer
 * @param[in]   buff_size - Buffer size
 * @return      None
 */
void tlkdrv_interphone_i2s_rx_init(unsigned short *in_buff, unsigned int buff_size)
{
    /* rx dma init. */
    audio_rx_dma_chain_init(INTERPHONE_I2S_FIFO, INTERPHONE_I2S_RX_DMA, in_buff, buff_size);
}

/**
 * @brief       Initialize I2S for interphone
 * @param       None
 * @return      None
 */
void tlkdrv_interphone_i2s_init(void)
{
    audio_i2s_config_init(&interphone_audio_i2s_config);

    /* matrix input config. */
    audio_matrix_set_rx_fifo_route(INTERPHONE_I2S_FIFO, FIFO_RX_ROUTE_I2S2_RX, FIFO_RX_I2S_RX_CHN01_16); /* rx fifo source select i2s0 */

    /* matrix output config. */
    audio_matrix_set_i2s_tx_route(I2S2_CHN0, I2S_TX_ROUTE_FIFO, I2S_TX_FIFO0_16_STEREO + INTERPHONE_I2S_FIFO); /* i2s0_ch0 tx sel fifo */
    audio_matrix_set_i2s_tx_route(I2S2_CHN1, I2S_TX_ROUTE_FIFO, I2S_TX_FIFO0_16_STEREO + INTERPHONE_I2S_FIFO); /* i2s0_ch1 tx sel fifo */
}

/**
 * @brief       Mute I2S TX buffer
 * @param       None
 * @return      None
 */
void tlkmdi_mute_i2s_tx_buff(void)
{
    for (int i = 0; i < INTERPHONE_I2S_SIZES; i++) {
        interphone_i2s_tx_buff[i] = 0;
    }
}

/**
 * @brief       Mute I2S RX buffer
 * @param       None
 * @return      None
 */
void tlkmdi_mute_i2s_rx_buff(void)
{
    for (int i = 0; i < INTERPHONE_I2S_SIZES; i++) {
        interphone_i2s_rx_buff[i] = 0;
    }
}

/**
 * @brief       Set loop tick
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_set_looptick(void)
{
    s_interphone_ctl.last_times_tick = clock_time();
}

/**
 * @brief       Handle interphone timer interrupt
 * @param       None
 * @return      None
 */
_attribute_retention_code_ void tlkmdi_interphone_timer_irq(void)
{
    uint32_t tdiff_us;

    tlkmdi_audio_stop_timer();

    if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        tdiff_us = 10700;
    } else {
        tdiff_us = 1500;
    }

    tlkmdi_audio_task_set_next_irq(tdiff_us);

    if (g_bt_audio_task_env.irq_en) {
        bt_audio_player_task();
    }
}

/**
 * @brief       Interphone main loop
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_mainloop(void)
{
    if (s_tlk_mdi_interphone_env.bt_music_started) {
        uint32_t tick_diff = 0;
        uint32_t cur_tick  = clock_time();
        if (cur_tick > s_interphone_ctl.last_times_tick) {
            tick_diff = cur_tick - s_interphone_ctl.last_times_tick;
        } else {
            tick_diff = 0xffffffff - s_interphone_ctl.last_times_tick + cur_tick;
        }

        if (tick_diff > one_frame_times * TICK_PER_US) {
            s_interphone_ctl.last_times_tick = cur_tick;
            bt_audio_player_task();
        }
    }
}

/**
 * @brief       Clear DSP status
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_clear_dsp_status(void)
{
    tlkmdi_interphone_voice_alg_deinit();
    s_interphone_ctl.dis_dsp_request      = false;
    s_interphone_ctl.send_seq             = 0;
    s_interphone_ctl.recv_seq             = 0;
    s_interphone_ctl.queue_one_frame_data = false;
}

/**
 * @brief       Disable DSP response
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_dis_dsp_response(void)
{
    if (s_interphone_ctl.dis_dsp_request && s_interphone_ctl.data_state == MCU_RECV && s_interphone_ctl.send_seq == s_interphone_ctl.recv_seq) {
#if TLK_MW_DSP_COMM_ENABLE
        tlkmw_dsp_pause();
#endif
        tlkmdi_interphone_voice_alg_deinit();
        s_interphone_ctl.dis_dsp_request = false;
        s_interphone_ctl.send_seq        = 0;
        s_interphone_ctl.recv_seq        = 0;
        tlkapi_printf(APP_LOG_EN, "close dsp clk");
        if (s_interphone_ctl.queue_one_frame_data) {
            tlkmdi_interphone_data_mixing((uint8_t *)data_buff);
            s_interphone_ctl.queue_one_frame_data = false;
            tlkapi_printf(APP_LOG_EN, "fill one frame music data queued before dsp closed");
        }
    }
}

/**
 * @brief       Initialize BT music algorithm for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_btmusic_alg_init(void)
{
    audio_alg_interface_t *p_audio_alg_if = NULL;

    if (bt_music_get_enc_buffer() == NULL) {
        uint8_t *p_bt_music_enc_buf = (uint8_t *)tlkmdi_audmem_calloc(BT_MUSIC_ENC_FIFO_SIZE);
        if (p_bt_music_enc_buf == NULL) {
            tlkapi_printf(APP_LOG_EN, "malloc music enc buff failed");
        } else {
            bt_music_set_enc_buffer(p_bt_music_enc_buf);
        }
    }

    if (s_tlk_mdi_interphone_env.codec_type == SEPID_SBC) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SBC_DEC);

        if (s_interphone_alg_sbc_dec_buff == NULL) {
            uint16_t sbc_dec_mem_size     = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
            s_interphone_alg_sbc_dec_buff = (uint8_t *)tlkalg_malloc_func(sbc_dec_mem_size);
            p_audio_alg_if->audio_alg_init(s_interphone_alg_sbc_dec_buff, ALG_CHANNEL_STEREO);

            tlkapi_printf(APP_LOG_EN, "sbc_dec_mem_size: %d", sbc_dec_mem_size);
        }

#if AUDIO_HD_HAC_EN
        if (g_hd_hac_asrc_buffer == NULL) {
            int hd_hac_buff      = tlkdrv_hac_get_size(44100, 48000, 128, 24);
            g_hd_hac_asrc_buffer = (uint8_t *)tlkalg_malloc_func(hd_hac_buff);
            if (g_hd_hac_asrc_buffer == NULL) {
                tlkapi_printf(APP_LOG_EN, "ASRC buff alloc failed");
                return;
            }
            tlkapi_printf(APP_LOG_EN, "SBC ASRC _size: %d addr%d", hd_hac_buff, g_hd_hac_asrc_buffer);
        }
#endif
    } else if (s_tlk_mdi_interphone_env.codec_type == SEPID_AAC) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AAC_DEC);

        if (s_interphone_alg_aac_dec_buff == NULL) {
            uint16_t aac_dec_mem_size     = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
            s_interphone_alg_aac_dec_buff = (uint8_t *)tlkalg_malloc_func(aac_dec_mem_size);
            p_audio_alg_if->audio_alg_init(s_interphone_alg_aac_dec_buff, ALG_CHANNEL_STEREO);

            tlkapi_printf(APP_LOG_EN, "aac_dec_mem_size: %d", aac_dec_mem_size);
        }

#if AUDIO_HD_HAC_EN
        if (g_hd_hac_asrc_buffer == NULL) {
            int hd_hac_buff      = tlkdrv_hac_get_size(44100, 48000, 1024, 24);
            g_hd_hac_asrc_buffer = (uint8_t *)tlkalg_malloc_func(hd_hac_buff);
            if (g_hd_hac_asrc_buffer == NULL) {
                tlkapi_printf(APP_LOG_EN, "ASRC buff alloc failed");
                return;
            }
            tlkapi_printf(APP_LOG_EN, "AAC ASRC _size: %d addr%d", hd_hac_buff, g_hd_hac_asrc_buffer);
        }
#endif
    }

    // #if TLKALG_ASRC_441TO48_16BIT_ENABLE
    // if (s_interphone_alg_asrc_441to48_buff == NULL) {
    //     p_audio_alg_if             = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
    //     uint16_t asrc_441to48_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
    //     s_interphone_alg_asrc_441to48_buff    = (uint8_t *)tlkalg_malloc_func(asrc_441to48_size);
    //     if (s_interphone_alg_asrc_441to48_buff == NULL) {
    //         tlkapi_printf(APP_LOG_EN, "asrc 441to48 alloc failed");
    //         return ;
    //     }
    //     p_audio_alg_if->audio_alg_init(s_interphone_alg_asrc_441to48_buff, ALG_CHANNEL_STEREO);
    // }
    // #endif
#if TLKALG_EQ_ENABLE
    if (s_interphone_alg_eq_buff == NULL) {
        p_audio_alg_if               = audio_alg_get_interface_by_type(ALG_EQ);
        s_interphone_alg_eq_buff     = (uint8_t *)tlkalg_malloc_func(4);
        eq_para_t interphone_eq_para = {
            .samplerate = 48000,
            .channel    = ALG_CHANNEL_STEREO,
            .eq_type    = EQ_TYPE_MUSIC,
            .width      = ALG_WIDTH_16,
        };
        p_audio_alg_if->audio_alg_param_set(0, (void *)&interphone_eq_para);
        p_audio_alg_if->audio_alg_init(s_interphone_alg_eq_buff, ALG_CHANNEL_STEREO);
    }
#endif

#if (TLKALG_PPM_SPK_ENABLE)
    if (s_interphone_alg_ppm_stereo_buff == NULL) {
        p_audio_alg_if               = audio_alg_get_interface_by_type(ALG_PPM_SPK);
        uint16_t ppm_stereo_mem_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        tlkapi_printf(APP_LOG_EN, "ppm_stereo_mem_size: %d", ppm_stereo_mem_size);
        s_interphone_alg_ppm_stereo_buff = (uint8_t *)tlkalg_malloc_func(ppm_stereo_mem_size);
        if (s_interphone_alg_ppm_stereo_buff == NULL) {
            tlkapi_printf(APP_LOG_EN, "ppm alloc failed");
            return;
        }
        p_audio_alg_if->audio_alg_init(s_interphone_alg_ppm_stereo_buff, ALG_CHANNEL_STEREO);
    }
#endif
}

/**
 * @brief       Deinitialize BT music algorithm for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_btmusic_alg_deinit(void)
{
    audio_alg_interface_t *p_audio_alg_if = NULL;

    uint8_t *p_bt_music_enc_buf = bt_music_get_enc_buffer();
    if (p_bt_music_enc_buf != NULL) {
        tlkmdi_audmem_free(p_bt_music_enc_buf);
        bt_music_set_enc_buffer(NULL);
    }

    if (s_tlk_mdi_interphone_env.codec_type == SEPID_SBC) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SBC_DEC);

        if (s_interphone_alg_sbc_dec_buff != NULL) {
            tlkalg_free_func(s_interphone_alg_sbc_dec_buff);
            p_audio_alg_if->audio_alg_deinit();
            s_interphone_alg_sbc_dec_buff = NULL;
        }

#if AUDIO_HD_HAC_EN
        if (g_hd_hac_asrc_buffer != NULL) {
            tlkalg_free_func(g_hd_hac_asrc_buffer);
            tlkdrv_hac_deinit(g_hd_hac_asrc_buffer);
            g_hd_hac_asrc_buffer = NULL;
        }
#endif

    } else if (s_tlk_mdi_interphone_env.codec_type == SEPID_AAC) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AAC_DEC);

        if (s_interphone_alg_aac_dec_buff != NULL) {
            tlkalg_free_func(s_interphone_alg_aac_dec_buff);
            p_audio_alg_if->audio_alg_deinit();
            s_interphone_alg_aac_dec_buff = NULL;
        }

#if AUDIO_HD_HAC_EN
        if (g_hd_hac_asrc_buffer != NULL) {
            tlkalg_free_func(g_hd_hac_asrc_buffer);
            tlkdrv_hac_deinit(g_hd_hac_asrc_buffer);
            g_hd_hac_asrc_buffer = NULL;
        }
#endif
    }

    // #if TLKALG_ASRC_441TO48_16BIT_ENABLE
    // p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
    // if (s_interphone_alg_asrc_441to48_buff != NULL) {
    //     tlkalg_free_func(s_interphone_alg_asrc_441to48_buff);
    //     p_audio_alg_if->audio_alg_deinit();
    //     s_interphone_alg_asrc_441to48_buff = NULL;
    // }
    // #endif
#if TLKALG_EQ_ENABLE
    if (s_interphone_alg_eq_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_EQ);
        tlkalg_free_func(s_interphone_alg_eq_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_interphone_alg_eq_buff = NULL;
    }
#endif

#if (TLKALG_PPM_SPK_ENABLE)
    if (s_interphone_alg_ppm_stereo_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
        tlkalg_free_func(s_interphone_alg_ppm_stereo_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_interphone_alg_ppm_stereo_buff = NULL;
    }
#endif
}

/**
 * @brief       Initialize interphone algorithm temporarily
 * @param       None
 * @return      None
 */
//avoid 441to48 asrc init costing too long time to block other operation
void tlkmdi_interphone_alg_init_temp(void)
{
#if (TLKALG_ASRC_441TO48_16BIT_BACKUP_ENABLE || TLKALG_ASRC_441TO48_16BIT_ENABLE)
    audio_alg_interface_t *p_audio_alg_if = NULL;
#endif

#if TLKALG_ASRC_441TO48_16BIT_ENABLE
    if (s_interphone_alg_asrc_441to48_buff == NULL) {
        p_audio_alg_if                     = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
        uint16_t asrc_441to48_size         = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        s_interphone_alg_asrc_441to48_buff = (uint8_t *)tlkalg_malloc_func(asrc_441to48_size);
        if (s_interphone_alg_asrc_441to48_buff == NULL) {
            tlkapi_printf(APP_LOG_EN, "asrc 441to48 alloc failed");
            return;
        }
        p_audio_alg_if->audio_alg_init(s_interphone_alg_asrc_441to48_buff, ALG_CHANNEL_STEREO);
    }
#endif
}

/**
 * @brief       Initialize voice algorithm for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_voice_alg_init(void)
{
    audio_alg_interface_t *p_audio_alg_if = NULL;

#if TLKALG_ASRC_16TO48_16BIT_ENABLE
    //i2s_adc or voice_down data 16k to 48k
    if (s_interphone_alg_asrc_16to48_buff == NULL) {
        p_audio_alg_if                    = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
        uint16_t asrc_16to48_size         = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        s_interphone_alg_asrc_16to48_buff = (uint8_t *)tlkalg_malloc_func(asrc_16to48_size);
        if (s_interphone_alg_asrc_16to48_buff == NULL) {
            tlkapi_printf(APP_LOG_EN, "asrc 16to48 alloc failed");
            return;
        }
        p_audio_alg_if->audio_alg_init(s_interphone_alg_asrc_16to48_buff, ALG_CHANNEL_STEREO);
    }
#endif

#if TLKALG_AGC_ENABLE
    if (g_alg_voice_agc_buff == NULL) {
        p_audio_alg_if       = audio_alg_get_interface_by_type(ALG_AGC);
        uint16_t agc_size    = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
        g_alg_voice_agc_buff = (uint8_t *)tlkalg_malloc_func(agc_size);
        if (g_alg_voice_agc_buff == NULL) {
            tlkapi_printf(APP_LOG_EN, "agc alloc failed");
            return;
        }
        p_audio_alg_if->audio_alg_init(g_alg_voice_agc_buff, ALG_CHANNEL_LEFT);
    }
#endif
#if TLKALG_AEC_ENABLE
    if (g_alg_voice_aec_buff == NULL) {
        p_audio_alg_if       = audio_alg_get_interface_by_type(ALG_AEC);
        uint16_t aec_size    = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
        g_alg_voice_aec_buff = (uint8_t *)tlkalg_malloc_func(aec_size);
        if (g_alg_voice_aec_buff == NULL) {
            tlkapi_printf(APP_LOG_EN, "aec alloc failed");
            return;
        }
        p_audio_alg_if->audio_alg_init(g_alg_voice_aec_buff, ALG_CHANNEL_LEFT);
    }
#endif
#if TLKALG_ANS_ENABLE
    if (g_alg_voice_ans_buff == NULL) {
        p_audio_alg_if       = audio_alg_get_interface_by_type(ALG_ANS);
        uint16_t ans_size    = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
        g_alg_voice_ans_buff = (uint8_t *)tlkalg_malloc_func(ans_size);
        if (g_alg_voice_ans_buff == NULL) {
            tlkapi_printf(APP_LOG_EN, "ans alloc failed");
            return;
        }
        p_audio_alg_if->audio_alg_init(g_alg_voice_ans_buff, ALG_CHANNEL_LEFT);
    }
#endif

#if TLKALG_ANS_SPK_ENABLE
    if (g_alg_ans_spk_buff == NULL) {
        p_audio_alg_if        = audio_alg_get_interface_by_type(ALG_SPK_ANS);
        uint16_t ans_spk_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
        g_alg_ans_spk_buff    = (uint8_t *)tlkalg_malloc_func(ans_spk_size);
        if (g_alg_ans_spk_buff == NULL) {
            tlkapi_printf(APP_LOG_EN, "ERR:ANS alloc Failed");
            return;
        }
        p_audio_alg_if->audio_alg_init(g_alg_ans_spk_buff, ALG_CHANNEL_LEFT);
    }
#endif
}

/**
 * @brief       Deinitialize voice algorithm for interphone
 * @param       None
 * @return      None
 */
//i2s_adc or voice_down data 16k to 48k
void tlkmdi_interphone_voice_alg_deinit(void)
{
    audio_alg_interface_t *p_audio_alg_if = NULL;

#if TLKALG_ASRC_16TO48_16BIT_ENABLE
    if (s_interphone_alg_asrc_16to48_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
        tlkalg_free_func(s_interphone_alg_asrc_16to48_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_interphone_alg_asrc_16to48_buff = NULL;
    }
#endif

#if TLKALG_AGC_ENABLE
    if (g_alg_voice_agc_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AGC);
        tlkalg_free_func(g_alg_voice_agc_buff);
        p_audio_alg_if->audio_alg_deinit();
        g_alg_voice_agc_buff = NULL;
    }
#endif

#if TLKALG_AEC_ENABLE
    if (g_alg_voice_aec_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AEC);
        tlkalg_free_func(g_alg_voice_aec_buff);
        p_audio_alg_if->audio_alg_deinit();
        g_alg_voice_aec_buff = NULL;
    }
#endif

#if TLKALG_ANS_ENABLE
    if (g_alg_voice_ans_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ANS);
        tlkalg_free_func(g_alg_voice_ans_buff);
        p_audio_alg_if->audio_alg_deinit();
        g_alg_voice_ans_buff = NULL;
    }
#endif

#if TLKALG_ANS_SPK_ENABLE
    if (g_alg_ans_spk_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SPK_ANS);
        tlkalg_free_func(g_alg_ans_spk_buff);
        p_audio_alg_if->audio_alg_deinit();
        g_alg_ans_spk_buff = NULL;
    }
#endif
}

/**
 * @brief       Deinitialize interphone algorithm
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_alg_deinit(void)
{
    audio_alg_interface_t *p_audio_alg_if = NULL;

    uint8_t *p_bt_music_enc_buf = bt_music_get_enc_buffer();
    if (p_bt_music_enc_buf != NULL) {
        tlkmdi_audmem_free(p_bt_music_enc_buf);
        bt_music_set_enc_buffer(NULL);
    }

    if (s_tlk_mdi_interphone_env.codec_type == SEPID_SBC) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SBC_DEC);

        if (s_interphone_alg_sbc_dec_buff != NULL) {
            tlkalg_free_func(s_interphone_alg_sbc_dec_buff);
            p_audio_alg_if->audio_alg_deinit();
            s_interphone_alg_sbc_dec_buff = NULL;
        }
    } else if (s_tlk_mdi_interphone_env.codec_type == SEPID_AAC) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AAC_DEC);

        if (s_interphone_alg_aac_dec_buff != NULL) {
            tlkalg_free_func(s_interphone_alg_aac_dec_buff);
            p_audio_alg_if->audio_alg_deinit();
            s_interphone_alg_aac_dec_buff = NULL;
        }
    }

#if TLKALG_ASRC_16TO48_16BIT_ENABLE
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
    if (s_interphone_alg_asrc_16to48_buff != NULL) {
        tlkalg_free_func(s_interphone_alg_asrc_16to48_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_interphone_alg_asrc_16to48_buff = NULL;
    }
#endif

    // #if TLKALG_ASRC_441TO48_16BIT_ENABLE
    // p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
    // if (s_interphone_alg_asrc_441to48_buff != NULL) {
    //     tlkalg_free_func(s_interphone_alg_asrc_441to48_buff);
    //     p_audio_alg_if->audio_alg_deinit();
    //     s_interphone_alg_asrc_441to48_buff = NULL;
    // }
    // #endif

#if TLKALG_EQ_ENABLE
    if (s_interphone_alg_eq_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_EQ);
        tlkalg_free_func(s_interphone_alg_eq_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_interphone_alg_eq_buff = NULL;
    }
#endif

#if TLKALG_AGC_ENABLE
    if (g_alg_voice_agc_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AGC);
        tlkalg_free_func(g_alg_voice_agc_buff);
        p_audio_alg_if->audio_alg_deinit();
        g_alg_voice_agc_buff = NULL;
    }
#endif

#if TLKALG_AEC_ENABLE
    if (g_alg_voice_aec_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AEC);
        tlkalg_free_func(g_alg_voice_aec_buff);
        p_audio_alg_if->audio_alg_deinit();
        g_alg_voice_aec_buff = NULL;
    }
#endif

#if TLKALG_ANS_ENABLE
    if (g_alg_voice_ans_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ANS);
        tlkalg_free_func(g_alg_voice_ans_buff);
        p_audio_alg_if->audio_alg_deinit();
        g_alg_voice_ans_buff = NULL;
    }
#endif

#if TLKALG_ANS_SPK_ENABLE
    if (g_alg_ans_spk_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SPK_ANS);
        tlkalg_free_func(g_alg_ans_spk_buff);
        p_audio_alg_if->audio_alg_deinit();
        g_alg_ans_spk_buff = NULL;
    }
#endif

#if (TLKALG_PPM_SPK_ENABLE)
    if (s_interphone_alg_ppm_stereo_buff != NULL) {
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
        tlkalg_free_func(s_interphone_alg_ppm_stereo_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_interphone_alg_ppm_stereo_buff = NULL;
    }
#endif
}

/**
 * @brief       Open codec for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_open_codec(void)
{
    s_interphone_ctl.mic_samplerate = 16000;
#if SPK_SAMPLERATE_16K
    s_interphone_ctl.spk_samplerate = 16000;
#else
    s_interphone_ctl.spk_samplerate = 48000;
#endif
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_16, s_interphone_ctl.mic_samplerate, 0);
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, s_interphone_ctl.spk_samplerate, 0);
}

/**
 * @brief       Open microphone for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_open_mic(void)
{
    s_interphone_ctl.mic_samplerate = 16000;
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_LEFT, TLKDRV_CODEC_BITDEPTH_16, s_interphone_ctl.mic_samplerate, 0);
}

/**
 * @brief       Initialize speaker HAC for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_spk_hac_init(void)
{
#if AUDIO_HD_HAC_EN
#if SPK_SAMPLERATE_16K
    s_interphone_ctl.spk_samplerate = 16000;
#else
    s_interphone_ctl.spk_samplerate = 48000;
#endif

    st_audio_sys_clk      = 36864000;
    int one_frame_samples = 0;
    if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        one_frame_samples = 1024;
    } else if (SEPID_SBC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        one_frame_samples = 128;
    } else if (SEPID_LHDC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
        one_frame_samples = 128;
    }

    tlkapi_printf(APP_LOG_EN, "hac init samplrate in %d out %d frame_len %d", g_codec_cfg.sample_rate, s_interphone_ctl.spk_samplerate, one_frame_samples);
    if (hac_init && g_hd_hac_asrc_buffer) {
        tlkdrv_hac_deinit(g_hd_hac_asrc_buffer);
    }
    tlkdrv_hac_init(g_hd_hac_asrc_buffer, st_audio_sys_clk, g_codec_cfg.sample_rate, s_interphone_ctl.spk_samplerate, one_frame_samples, 16);
#endif
}

#if TLKALG_EQ_ENABLE
/**
 * @brief       Initialize EQ for interphone
 * @param[in]   samplerate - Sample rate
 * @param[in]   chnl - Channel index
 * @param[in]   eq_type - EQ type
 * @return      None
 */
void tlkmdi_interphone_eq_init(uint32_t samplerate, ALG_CHANNEL_IDX chnl, e_eq_type_e eq_type)
{
    tlkapi_printf(APP_LOG_EN, "bt_music_alg_eq_init");
    if (s_interphone_alg_eq_buff == NULL) {
        tlkapi_printf(APP_LOG_EN, "EQ set para and init");
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_EQ);
        s_interphone_alg_eq_buff              = (uint8_t *)tlkalg_malloc_func(4);
        eq_para_t btmusic_eq_para             = {
                        .samplerate = samplerate,
                        .channel    = chnl,
                        .eq_type    = eq_type,
            // .width = ALG_WIDTH_16,
        };
        p_audio_alg_if->audio_alg_param_set(0, (void *)&btmusic_eq_para);
        p_audio_alg_if->audio_alg_init(s_interphone_alg_eq_buff, ALG_CHANNEL_STEREO);
    }
}
#endif

/**
 * @brief       Open speaker for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_open_spk(void)
{
#if SPK_SAMPLERATE_16K
    s_interphone_ctl.spk_samplerate = 16000;
#else
    s_interphone_ctl.spk_samplerate = 48000;
#endif
    // tlkmdi_interphone_spk_hac_init();
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, s_interphone_ctl.spk_samplerate, 0);
#if TLKALG_EQ_ENABLE
    tlkmdi_interphone_eq_init(s_interphone_ctl.spk_samplerate, ALG_CHANNEL_STEREO, EQ_TYPE_MUSIC);
#endif
}

/**
 * @brief       Initialize I2S for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_i2s_init(void)
{
    ///I2S init
    tlkdrv_interphone_i2s_init();
    tlkdrv_interphone_i2s_tx_init((unsigned short *)interphone_i2s_tx_buff, (unsigned int)sizeof(interphone_i2s_tx_buff));
    tlkdrv_interphone_i2s_rx_init((unsigned short *)interphone_i2s_rx_buff, (unsigned int)sizeof(interphone_i2s_rx_buff));
}

/**
 * @brief       Initialize FIFO interrupt for interphone
 * @param[in]   byte_num - Byte number for interrupt trigger
 * @return      None
 */
void tlkmdi_interphone_fifo_irq_init(uint16_t byte_num)
{
    plic_interrupt_enable(IRQ_DFIFO);
    plic_set_priority(IRQ_DFIFO, 2);
#if TX_FIFO_IRQ_ENABLE
    audio_fifo_ptr_en(AUDIO_TX_FIFO0);
    audio_set_tx_rptr(TLKDRV_CODEC_SPK_FIFO, 0);
    audio_set_tx_buff_thres(TLKDRV_CODEC_SPK_FIFO, byte_num);
    audio_fifo_irq_en(AUDIO_TX_FIFO0); //enable TX_FIFO0
#else
    audio_fifo_ptr_en(AUDIO_RX_FIFO0);
    audio_set_rx_wptr(TLKDRV_CODEC_MIC_FIFO, 0);
    audio_set_rx_buff_thres(TLKDRV_CODEC_MIC_FIFO, byte_num);
    audio_fifo_irq_en(AUDIO_RX_FIFO0); //enable RX_FIFO0
#endif
}

/**
 * @brief       Set interphone mode
 * @param[in]   mode - Mode to set
 * @return      None
 */
void tlkmdi_interphone_set_mode(interphone_mode_e mode)
{
    if (interphone_mode_flag == 0) {
        interphone_mode_flag |= mode;
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_INTRTPHONE, 0xffff);
    } else {
        interphone_mode_flag |= mode;
    }

    tlkapi_printf(APP_LOG_EN, "tlkmdi_interphone_set_mode %d", interphone_mode_flag);
}

/**
 * @brief       Clear interphone mode
 * @param[in]   mode - Mode to clear
 * @return      None
 */
void tlkmdi_interphone_clear_mode(interphone_mode_e mode)
{
    if (interphone_mode_flag == 0) {
        return;
    }
    interphone_mode_flag &= (~mode);
    tlkapi_printf(APP_LOG_EN, "tlkmdi_interphone_clear_mode %d", interphone_mode_flag);
    if (interphone_mode_flag == 0) {
        tlkapi_printf(APP_LOG_EN, "close interphone scene");
        // gpio_set_high_level(GPIO_CHN0);
        tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_INTRTPHONE, 0xffff);
        // gpio_set_low_level(GPIO_CHN0);
    }
}

/**
 * @brief       Get interphone mode
 * @param       None
 * @return      Current interphone mode
 */
interphone_mode_e tlkmdi_interphone_get_mode(void)
{
    return interphone_mode_flag;
}

/**
 * @brief       Enable voice for interphone
 * @param[in]   mode_flag - Mode flag
 * @return      None
 */
void tlkmdi_interphone_voice_enable(interphone_mode_e mode_flag)
{
    if ((mode_flag & INTERPHONE_MODE_MESH) || (mode_flag & INTERPHONE_MODE_AG)) {
        if (interphone_voice_flag == 0) {
            s_tlk_mdi_interphone_env.mic_first_flag = true;
            if (s_interphone_ctl.dis_dsp_request) {
                s_interphone_ctl.dis_dsp_request = false;
            }
            tlkmdi_interphone_open_mic();
            tlkmdi_interphone_voice_alg_init();
#if TLK_MW_DSP_COMM_ENABLE
            tlkmw_dsp_resume();
#endif
            tlkapi_printf(APP_LOG_EN, "enable dsp clk and open mic");
        }
        interphone_voice_flag |= mode_flag;
        tlkapi_printf(APP_LOG_EN, "enable func interphone voice flag %d", interphone_voice_flag);
    }
}

/**
 * @brief       Disable voice for interphone
 * @param[in]   mode_flag - Mode flag
 * @return      None
 */
void tlkmdi_interphone_voice_disable(interphone_mode_e mode_flag)
{
    if (interphone_voice_flag == 0) {
        return;
    }
    if ((mode_flag & INTERPHONE_MODE_MESH) || (mode_flag & INTERPHONE_MODE_AG)) {
        interphone_voice_flag &= (~mode_flag);
        tlkapi_printf(APP_LOG_EN, "disable func interphone voice flag %d", interphone_voice_flag);
        if (interphone_voice_flag == 0) {
            tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
            // tlkmdi_interphone_voice_alg_deinit();
            tlkapi_printf(APP_LOG_EN, "close mic");
#if TLK_MW_DSP_COMM_ENABLE
            s_interphone_ctl.dis_dsp_request = true;
            tlkapi_printf(APP_LOG_EN, "inform close dsp");
            tlkmdi_interphone_dis_dsp_response();
#endif
        }
    }
}

/**
 * @brief       Enable speaker for interphone
 * @param[in]   mode_flag - Mode flag
 * @return      None
 */
void tlkmdi_interphone_spk_enable(interphone_mode_e mode_flag)
{
    if ((mode_flag == INTERPHONE_MODE_MUSIC) && !(interphone_spk_flag & INTERPHONE_MODE_MUSIC)) {
        tlkmdi_interphone_spk_hac_init();
    }

    if (interphone_spk_flag == 0) {
        s_tlk_mdi_interphone_env.spk_first_flag = true;
        tlkmdi_interphone_open_spk();
    }
    interphone_spk_flag |= mode_flag;
    tlkapi_printf(APP_LOG_EN, "enable func interphone spk flag %d", interphone_spk_flag);
}

/**
 * @brief       Disable speaker mode for interphone
 * @param[in]   mode_flag - Mode flag
 * @return      None
 */
void tlkmdi_interphone_spk_disable_mode(interphone_mode_e mode_flag)
{
    if (interphone_spk_flag == 0) {
        return;
    }
    interphone_spk_flag &= (~mode_flag);
}

/**
 * @brief       Disable speaker for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_spk_disable(void)
{
    interphone_spk_flag = 0;
}

/**
 * @brief       Enable voice AG for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_voice_ag_enable(void)
{
    tlkmdi_interphone_spk_enable(INTERPHONE_MODE_AG);
    tlkmdi_interphone_voice_enable(INTERPHONE_MODE_AG);
    s_tlk_mdi_interphone_env.bt_ag_sco_started = true;
}

/**
 * @brief       Get I2S TX read pointer
 * @param       None
 * @return      I2S TX read pointer
 */
uint16_t tlkmdi_interphone_get_i2stx_rptr(void)
{
    uint32_t i2s_dma_rptr = (audio_get_tx_dma_rptr(INTERPHONE_I2S_TX_DMA) - (uint32_t)interphone_i2s_tx_buff);
    i2s_dma_rptr          = (i2s_dma_rptr + (sizeof(adc_int) - 1)) / sizeof(adc_int);
    return i2s_dma_rptr;
}

/**
 * @brief       Get idle length of I2S TX buffer
 * @param       None
 * @return      Idle length
 */
uint16_t tlkmdi_interphone_get_i2stx_idle_len(void)
{
    uint16_t rptr     = tlkmdi_interphone_get_i2stx_rptr();
    uint16_t idle_len = 0;
    if (interphone_i2s_wptr >= rptr) {
        idle_len = INTERPHONE_I2S_SIZES - (interphone_i2s_wptr - rptr);
    } else {
        idle_len = rptr - interphone_i2s_wptr;
    }

    return idle_len;
}

/**
 * @brief       Fill I2S TX buffer
 * @param[in]   data - Data to fill
 * @param[in]   len - Length of data
 * @return      None
 */
void tlkmdi_interphone_fill_i2stx_buff(adc_int *data, uint16_t len)
{
    if (tlkmdi_interphone_get_i2stx_idle_len() < len) {
        tlkapi_printf(APP_LOG_EN, "i2s_tx buff full %d %d", tlkmdi_interphone_get_i2stx_idle_len(), len);
        return;
    }

    for (int i = 0; i < len; i++) {
        interphone_i2s_tx_buff[interphone_i2s_wptr++] = data[i];
        interphone_i2s_wptr %= INTERPHONE_I2S_SIZES;
    }
}

/**
 * @brief       Get I2S RX write pointer
 * @param       None
 * @return      I2S RX write pointer
 */
uint16_t tlkmdi_interphone_get_i2srx_wptr(void)
{
    uint32_t i2s_dma_wptr = (audio_get_rx_dma_wptr(INTERPHONE_I2S_RX_DMA) - (uint32_t)interphone_i2s_rx_buff);
    i2s_dma_wptr          = (i2s_dma_wptr + (sizeof(codec_int) - 1)) / sizeof(codec_int);
    return i2s_dma_wptr;
}

/**
 * @brief       Get available samples in I2S RX buffer
 * @param       None
 * @return      Available samples count
 */
uint16_t tlkmdi_interphone_get_i2srx_ava_samples(void)
{
    uint16_t wptr        = tlkmdi_interphone_get_i2srx_wptr();
    uint16_t ava_samples = 0;
    if (wptr >= interphone_i2s_rptr) {
        ava_samples = wptr - interphone_i2s_rptr;
    } else {
        ava_samples = INTERPHONE_I2S_SIZES - (interphone_i2s_rptr - wptr);
    }
    return ava_samples;
}

/**
 * @brief       Get data from I2S RX buffer
 * @param[out]  buff - Buffer to store data
 * @param[in]   len - Length of data to get
 * @return      None
 */
void tlkmdi_interphone_get_i2srx_data(codec_int *buff, uint16_t len)
{
    if (tlkmdi_interphone_get_i2srx_ava_samples() < len) {
        tlkapi_printf(APP_LOG_EN, "i2s_rx buff null %d %d", tlkmdi_interphone_get_i2srx_ava_samples(), len);
        return;
    }

    for (int i = 0; i < MIC_FRAME_SIZE; i++) {
        buff[i] = interphone_i2s_rx_buff[interphone_i2s_rptr++];
        interphone_i2s_rptr %= INTERPHONE_I2S_SIZES;
    }
}

/**
 * @brief       Transfer I2S data
 * @param[in]   txdata - TX data buffer
 * @param[in]   rxdata - RX data buffer
 * @return      Operation result
 */
int tlkmdi_interphone_i2s_data_trans(uint8_t *txdata, uint8_t *rxdata)
{
    if (!s_tlk_mdi_interphone_env.bt_ag_sco_started) {
        codec_int i2s_rx_16k_buff[MIC_FRAME_SIZE];
        adc_int   i2s_tx_stereo_buff[MIC_FRAME_SIZE];
#if !PLAY_STEREO_DATA
        adc_mono_int *psrc = (adc_mono_int *)txdata;
#else
        adc_int *psrc = (adc_int *)txdata;
#endif

        if (s_tlk_mdi_interphone_env.mesh_first_flag) {
            s_tlk_mdi_interphone_env.mesh_first_flag = false;
            tlkmdi_interphone_sync_is2tx_sample(16 * 35); //nn_ns+agc+ans( 20ms frame ) processing time is about 22ms
            tlkmdi_interphone_sync_is2rx_sample(MIC_FRAME_SIZE + 16 * 5);
        }

        //fill i2s tx data
        for (int i = 0; i < MIC_FRAME_SIZE; i++) {
#if !PLAY_STEREO_DATA
            i2s_tx_stereo_buff[i] = (uint16_t)psrc[i] | (psrc[i] << 16);
#else
            i2s_tx_stereo_buff[i] = psrc[i];
#endif
        }
        tlkmdi_interphone_fill_i2stx_buff(i2s_tx_stereo_buff, MIC_FRAME_SIZE);

        //get i2s rx data
        tlkmdi_interphone_get_i2srx_data(i2s_rx_16k_buff, MIC_FRAME_SIZE);

#if SPK_SAMPLERATE_16K
        return MIC_FRAME_SIZE;
#else

#if (TLKALG_ANS_SPK_ENABLE)
        tlkalg_ans_multiframe((uint8_t *)i2s_rx_16k_buff, (uint8_t *)i2s_rx_16k_buff, MIC_FRAME_SIZE / 16 / 5);
#endif

        //asrc 16k->48k
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
        int                    num            = p_audio_alg_if->audio_alg_process((uint8_t *)i2s_rx_16k_buff, (uint8_t *)rxdata, MIC_FRAME_SIZE, 0, 0);

        // tlkapi_printf(APP_LOG_EN, "asrc num in out %d %d", MIC_FRAME_SIZE, num);
        return num;
#endif
    } else {
        codec_int *pdes = (codec_int *)rxdata;
        adc_int   *psrc = (adc_int *)txdata;
        //fill i2s tx data
        tlkmdi_interphone_fill_i2stx_buff(psrc, MIC_FRAME_SIZE);

        //get i2s rx data
        tlkmdi_interphone_get_i2srx_data(pdes, MIC_FRAME_SIZE);

#if (TLKALG_ANS_SPK_ENABLE)
        tlkalg_ans_multiframe((uint8_t *)pdes, (uint8_t *)pdes, MIC_FRAME_SIZE / 16 / 5);
#endif

        return MIC_FRAME_SIZE;
    }
}

/**
 * @brief       Mix PCM 16-bit data
 * @param[in]   p0 - First PCM data buffer
 * @param[in]   p1 - Second PCM data buffer
 * @param[out]  pdes - Destination buffer
 * @param[in]   samples_num - Number of samples
 * @return      None
 */
_attribute_ram_code_ void tlkmdi_interphone_mix_pcm_16bit(int16_t *p0, int16_t *p1, int16_t *pdes, int16_t samples_num)
{
    int mix_data = 0;

    for (int i = 0; i < samples_num; i++) {
        mix_data = (int)(*p0++) + (int)(*p1++);

        if (mix_data > 32767) {
            mix_data = 32767;
        } else if (mix_data < -32768) {
            mix_data = -32768;
        }

        *pdes++ = mix_data;
    }
}

/**
 * @brief       Synchronize I2S TX sample
 * @param[in]   sample - Sample count
 * @return      None
 */
void tlkmdi_interphone_sync_is2tx_sample(uint16_t sample)
{
    uint32_t i2s_dma_rptr = tlkmdi_interphone_get_i2stx_rptr();
    interphone_i2s_wptr   = (i2s_dma_rptr + sample) % INTERPHONE_I2S_SIZES;
    tlkapi_printf(APP_LOG_EN, "i2s_tx wptr rptr %d %d", interphone_i2s_wptr, i2s_dma_rptr);
}

/**
 * @brief       Synchronize I2S RX sample
 * @param[in]   sample - Sample count
 * @return      None
 */
void tlkmdi_interphone_sync_is2rx_sample(uint16_t sample)
{
    uint16_t i2s_rx_wptr = tlkmdi_interphone_get_i2srx_wptr();
    if (i2s_rx_wptr >= sample) {
        interphone_i2s_rptr = i2s_rx_wptr - sample;
    } else {
        interphone_i2s_rptr = INTERPHONE_I2S_SIZES - (sample - i2s_rx_wptr);
    }

    tlkapi_printf(APP_LOG_EN, "i2s_rx wptr rptr %d %d", i2s_rx_wptr, interphone_i2s_rptr);
}

/**
 * @brief       Mix interphone data
 * @param[in]   mic_data - Microphone data buffer
 * @return      None
 */
void tlkmdi_interphone_data_mixing(uint8_t *mic_data)
{
    codec_int interphone_spk_buff[SPK_FRAME_SIZE];
    uint16_t  interphone_spk_len = 0;

    if (!s_tlk_mdi_interphone_env.bt_ag_sco_started) { ///mesh/BTmusic
        if ((tlkmdi_interphone_get_mode() & INTERPHONE_MODE_MESH) && s_tlk_mdi_interphone_env.mesh_started) {
            tlkmdi_interphone_i2s_data_trans((uint8_t *)mic_data, (uint8_t *)interphone_spk_buff);
            //mix i2s_rx data with bt down pcm
            if (s_tlk_mdi_interphone_env.bt_music_started) { ///BTmusic
                codec_int bt_pcm_48k_buff[SPK_FRAME_SIZE];
                tlkmdi_interphone_getData_BtDownBuff((uint8_t *)bt_pcm_48k_buff, SPK_FRAME_SIZE * sizeof(codec_int));
                tlkmdi_interphone_mix_pcm_16bit((int16_t *)interphone_spk_buff, (int16_t *)bt_pcm_48k_buff, (int16_t *)interphone_spk_buff, SPK_FRAME_SIZE * 2); //*2 for mono
            }
        } else {
            (void)mic_data;
            tlkmdi_interphone_getData_BtDownBuff((uint8_t *)interphone_spk_buff, MIC_FRAME_SIZE * 3 * sizeof(codec_int));
        }

        interphone_spk_len = SPK_FRAME_SIZE * sizeof(codec_int);
    } else {                                             //SCO_AG/ mesh
        codec_int dsp_data_16k_stereo[MIC_FRAME_SIZE];   //stereo
        short    *dsp_data_16k_mono = (short *)mic_data; //mono
        codec_int bt_pcm_16k_buff[MIC_FRAME_SIZE];       //stereo

        //mic data mono to stereo
        for (int i = 0; i < MIC_FRAME_SIZE; i++) {
            dsp_data_16k_stereo[i] = (uint16_t)dsp_data_16k_mono[i] | (dsp_data_16k_mono[i] << 16);
        }

        //get bt voice down data
        if (!tlkmdi_interphone_getData_BtDownBuff((uint8_t *)bt_pcm_16k_buff, MIC_FRAME_SIZE * sizeof(codec_int))) {
            for (int j = 0; j < MIC_FRAME_SIZE; j++) {
                bt_pcm_16k_buff[j] = 0;
            }
        }

        if ((tlkmdi_interphone_get_mode() & INTERPHONE_MODE_MESH) && s_tlk_mdi_interphone_env.mesh_started) { ///mesh
            codec_int i2s_rx_16k_buff[MIC_FRAME_SIZE];                                                        //stereo
            codec_int mic_mix_btdown[MIC_FRAME_SIZE];

            //mix mic/dsp_data and voice_down_data, send to i2s_dac and get i2s_adc
            tlkmdi_interphone_mix_pcm_16bit((int16_t *)dsp_data_16k_stereo, (int16_t *)bt_pcm_16k_buff, (int16_t *)mic_mix_btdown, MIC_FRAME_SIZE * 2);
            tlkmdi_interphone_i2s_data_trans((uint8_t *)mic_mix_btdown, (uint8_t *)i2s_rx_16k_buff);

            //mix voice_down_data and i2s_adc_data, send to spk
            codec_int i2s_mix_btdown[MIC_FRAME_SIZE];
            tlkmdi_interphone_mix_pcm_16bit((int16_t *)bt_pcm_16k_buff, (int16_t *)i2s_rx_16k_buff, (int16_t *)i2s_mix_btdown, MIC_FRAME_SIZE * 2);

#if (TLKALG_ANS_SPK_ENABLE)
            tlkalg_ans_multiframe((uint8_t *)bt_pcm_16k_buff, (uint8_t *)bt_pcm_16k_buff, MIC_FRAME_SIZE / 16 / 5);
#endif

#if TLKALG_ASRC_16TO48_16BIT_ENABLE
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
            int                    num            = p_audio_alg_if->audio_alg_process((uint8_t *)i2s_mix_btdown, (uint8_t *)interphone_spk_buff, MIC_FRAME_SIZE, 0, 0);
            interphone_spk_len                    = num * sizeof(codec_int);
#endif

            //mix mic/dsp_data and i2s_adc_data, send to voice up
            codec_int i2s_mix_mic[MIC_FRAME_SIZE];
            tlkmdi_interphone_mix_pcm_16bit((int16_t *)dsp_data_16k_stereo, (int16_t *)i2s_rx_16k_buff, (int16_t *)i2s_mix_mic, MIC_FRAME_SIZE * 2);
            tlkmdi_interphone_fill_BtUpBuff((uint8_t *)i2s_mix_mic, MIC_FRAME_SIZE * sizeof(codec_int));
        } else {
#if (TLKALG_ANS_SPK_ENABLE)
            tlkalg_ans_multiframe((uint8_t *)interphone_spk_buff, (uint8_t *)interphone_spk_buff, MIC_FRAME_SIZE / 16 / 5);
#endif

#if TLKALG_ASRC_16TO48_16BIT_ENABLE
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
            int                    num            = p_audio_alg_if->audio_alg_process((uint8_t *)bt_pcm_16k_buff, (uint8_t *)interphone_spk_buff, MIC_FRAME_SIZE, 0, 0);
            interphone_spk_len                    = num * sizeof(codec_int);
#endif
            tlkmdi_interphone_fill_BtUpBuff((uint8_t *)dsp_data_16k_stereo, MIC_FRAME_SIZE * sizeof(codec_int));
        }
    }

#if TLKALG_EQ_ENABLE
    audio_alg_interface_t *p_audio_alg = audio_alg_get_interface_by_type(ALG_EQ);
    p_audio_alg->audio_alg_process((uint8_t *)interphone_spk_buff, (uint8_t *)interphone_spk_buff, SPK_FRAME_SIZE, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
#endif

    if (tone_is_playing()) {
        // tlkapi_printf(APP_LOG_EN, "tone playing in interphone");
        for (int i = 0; i < SPK_FRAME_SIZE; i++) {
            interphone_spk_buff[i] = 0;
        }
        tone_get_sample((int16_t *)interphone_spk_buff, SPK_FRAME_SIZE * sizeof(tone_int), s_interphone_ctl.spk_samplerate);
    }

    if (s_tlk_mdi_interphone_env.spk_first_flag) {
        s_tlk_mdi_interphone_env.spk_first_flag = false;
        tlkdrv_codec_sync_speaker_samples(48 * 35); //nn_ns+agc+ans( 20ms frame ) processing time is about 22ms
    }

    // uint16_t send_data[3];
    // send_data[0] = tlkdrv_codec_getSpkOffset()/sizeof(codec_int);
    // send_data[1] = tlkdrv_codec_get_speaker_rptr();
    // send_data[2] = tlkdrv_codec_get_spk_avail_samples();
    // spi_master_write_dma(0, (uint8_t *)send_data, 6);
    // while(spi_is_busy(0));

    tlkdrv_codec_fillSpkBuff((uint8_t *)interphone_spk_buff, interphone_spk_len);
}

/**
 * @brief       Handle FIFO interrupt for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_fifo_irq_handler_func(void)
{
#if TX_FIFO_IRQ_ENABLE
    uint16_t rptr            = audio_get_tx_rptr(TLKDRV_CODEC_SPK_FIFO);
    uint16_t threshold_value = 0;
    if (rptr >= SPK_FRAME_SIZE) {
        threshold_value = rptr - SPK_FRAME_SIZE;
    }
    audio_set_tx_rptr(TLKDRV_CODEC_SPK_FIFO, threshold_value);
#else
    uint16_t wptr            = audio_get_rx_wptr(TLKDRV_CODEC_MIC_FIFO);
    uint16_t threshold_value = 0;
    if (wptr >= MIC_FRAME_SIZE) {
        threshold_value = wptr - MIC_FRAME_SIZE;
    }
    audio_set_rx_wptr(TLKDRV_CODEC_MIC_FIFO, threshold_value);
#endif

    if (s_tlk_mdi_interphone_env.mic_first_flag) {
        s_tlk_mdi_interphone_env.mic_first_flag = false;
        tlkdrv_codec_sync_mic_samples((MIC_FRAME_SIZE + 16) / 2); // /2 for mono
    }

    // adc_mono_int data_buff[MIC_FRAME_SIZE];
    bool ret = 0;

    if (((tlkmdi_interphone_get_mode() & INTERPHONE_MODE_MESH) || (tlkmdi_interphone_get_mode() & INTERPHONE_MODE_AG)) &&
        (s_tlk_mdi_interphone_env.bt_ag_sco_started || s_tlk_mdi_interphone_env.mesh_started)) {
        if ((tlkdrv_codec_getMicDataLen() / sizeof(adc_mono_int)) > MIC_FRAME_SIZE) {
            ret = tlkdrv_codec_readMicData((uint8_t *)(data_buff), MIC_FRAME_SIZE * 2, 0); //sizeof(adc_mono_int)
        } else {
            tlkapi_printf(APP_LOG_EN, "mic buff null %d %d", MIC_FRAME_SIZE, tlkdrv_codec_getMicDataLen() / sizeof(adc_mono_int));
            return;
        }

        if (ret) {
#if TLK_MW_DSP_COMM_ENABLE
            ///write data to DSP
            app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(NN_NS_16K_20MS_ID);
            p_dsp_app_ctx->alg_type          = VAD_NN_NS;
            d25f_send_audio_data_to_dsp((uint8_t *)data_buff, MIC_FRAME_SIZE * sizeof(adc_mono_int), NN_NS_16K_20MS_ID);
            s_interphone_ctl.data_state = MCU_WAIT_RESP;
            s_interphone_ctl.send_seq++;
#else
            uint8_t *pdes = (uint8_t *)data_buff;
#if TLKALG_AGC_ENABLE
            adc_mono_int agc_out_buff[320];
            tlkalg_agc_proc_behind_nn((uint8_t *)data_buff, (uint8_t *)agc_out_buff, ALG_WIDTH_16);
            pdes = (uint8_t *)agc_out_buff;
#endif
            tlkmdi_interphone_data_mixing(pdes);
#endif
        }
    } else if (tlkmdi_interphone_get_mode() & INTERPHONE_MODE_MUSIC) {
        if (!s_interphone_ctl.dis_dsp_request) {
            tlkmdi_interphone_data_mixing((uint8_t *)data_buff);
        } else {
            s_interphone_ctl.queue_one_frame_data = true;
        }
    } else {
        tlkapi_printf(APP_LOG_EN, "mode %d ag-flag %d mesh-flag %d", tlkmdi_interphone_get_mode(), s_tlk_mdi_interphone_env.bt_ag_sco_started,
                      s_tlk_mdi_interphone_env.mesh_started);
        if (tone_is_playing()) {
            codec_int tone_play_buff[SPK_FRAME_SIZE];
            for (int i = 0; i < SPK_FRAME_SIZE; i++) {
                tone_play_buff[i] = 0;
            }
            tone_get_sample((int16_t *)tone_play_buff, SPK_FRAME_SIZE * sizeof(tone_int), s_interphone_ctl.spk_samplerate);
            tlkdrv_codec_fillSpkBuff((uint8_t *)tone_play_buff, SPK_FRAME_SIZE * sizeof(tone_int));
        }
    }
}

#if TLK_MW_DSP_COMM_ENABLE
/**
 * @brief       Callback function for processing DSP messages
 * @param[in]   enc_buff_wptr - Encode buffer write pointer
 * @param[in]   type - Message type
 * @return      None
 */
void tlkmdi_interphone_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type)
{
    (void)enc_buff_wptr;
    (void)type;

    //get data processed by nn_ns in dsp
    uint16_t pcm_data_len;
    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&pcm_data_len, NN_NS_16K_20MS_ID);
    if (pcm_data == NULL) {
        tlkapi_printf(APP_LOG_EN, "interphone pcm_data is NULL");
        return;
    }
#if TLKALG_AGC_ENABLE
    adc_mono_int agc_out_buff[320];
#if !PLAY_STEREO_DATA
    tlkalg_agc_proc_behind_nn(pcm_data, (uint8_t *)agc_out_buff, ALG_WIDTH_16);
    pcm_data = (uint8_t *)agc_out_buff;
#else
    adc_mono_int  agc_in_buff[320];
    adc_int       mix_buff[320];
    adc_mono_int *psrc = (adc_mono_int *)pcm_data;
    adc_mono_int *pdes = (adc_mono_int *)mix_buff;
    for (int i = 0; i < 320; i++) {
        agc_in_buff[i] = psrc[2 * i];
        //    	rawdata_buff[i] = psrc[2*i+1];
    }
    tlkalg_agc_proc_behind_nn((uint8_t *)agc_in_buff, (uint8_t *)agc_out_buff, ALG_WIDTH_16);
    for (int j = 0; j < 320; j++) {
        pdes[2 * j] = agc_out_buff[j];
        //    	pdes[2*j+1] = rawdata_buff[j];
        pdes[2 * j + 1] = agc_in_buff[j];
    }
    pcm_data = (uint8_t *)pdes;
#endif
#endif
#if TLKALG_ANS_ENABLE
    adc_mono_int ans_out_buff[MIC_FRAME_SIZE];
    tlkalg_ans_proc_behind_nn(pcm_data, (uint8_t *)ans_out_buff);
    pcm_data = (uint8_t *)ans_out_buff;
#endif
    tlkmdi_interphone_data_mixing(pcm_data);
    s_interphone_ctl.data_state = MCU_RECV;
    s_interphone_ctl.recv_seq++;

    tlkmdi_interphone_dis_dsp_response();
}
#endif

/**
 * @brief       Initialize interphone buffers
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_buff_init(void)
{
    s_interphone_ctl.down_buff_len = CODEC_SPK_FIFO_SAMPLES * sizeof(codec_int);
    s_interphone_ctl.up_buff_len   = CODEC_MIC_FIFO_SAMPLES * sizeof(adc_int);
}

/**
 * @brief       Mute BT down buffer
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_mute_BtDownBuff(void)
{
    for (int i = 0; i < CODEC_SPK_FIFO_SAMPLES; i++) {
        g_interphone_bt_down_buff[i] = 0;
    }
}

/**
 * @brief       Mute BT up buffer
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_mute_BtUpBuff(void)
{
    for (int i = 0; i < CODEC_MIC_FIFO_SAMPLES; i++) {
        g_interphone_bt_up_buff[i] = 0;
    }
}

/**
 * @brief       Synchronize BT down buffer
 * @param[in]   sample - Sample count
 * @return      None
 */
void tlkmdi_interphone_sync_BtDownBuff(uint16_t sample)
{
    s_interphone_ctl.down_buff_wptr = (s_interphone_ctl.down_buff_rptr + sample * sizeof(codec_int)) % s_interphone_ctl.down_buff_len;
    tlkapi_printf(APP_LOG_EN, "sync BtDownBuff wptr rptr %d %d", s_interphone_ctl.down_buff_wptr, s_interphone_ctl.down_buff_rptr);
}

/**
 * @brief       Get BT down buffer write pointer
 * @param       None
 * @return      Write pointer
 */
uint16_t tlkmdi_interphone_get_BtDownBuff_wptr(void)
{
    return s_interphone_ctl.down_buff_wptr;
}

/**
 * @brief       Get BT down buffer read pointer
 * @param       None
 * @return      Read pointer
 */
uint16_t tlkmdi_interphone_get_BtDownBuff_rptr(void)
{
    return s_interphone_ctl.down_buff_rptr;
}

/**
 * @brief       Get idle length of BT down buffer
 * @param       None
 * @return      Idle length
 */
uint16_t tlkmdi_interphone_get_BtDownBuff_IdleLen(void)
{
    uint16_t len = 0;
    if (s_interphone_ctl.down_buff_wptr >= s_interphone_ctl.down_buff_rptr) {
        len = s_interphone_ctl.down_buff_len - (s_interphone_ctl.down_buff_wptr - s_interphone_ctl.down_buff_rptr);
    } else {
        len = s_interphone_ctl.down_buff_rptr - s_interphone_ctl.down_buff_wptr;
    }
    return len;
}

/**
 * @brief       Get data length of BT down buffer
 * @param       None
 * @return      Data length
 */
uint16_t tlkmdi_interphone_get_BtDownBuff_DataLen(void)
{
    return (s_interphone_ctl.down_buff_len - tlkmdi_interphone_get_BtDownBuff_IdleLen());
}

/**
 * @brief       Get data from BT down buffer
 * @param[out]  data - Data buffer
 * @param[in]   len - Length of data to get
 * @return      true - Success, false - Failure
 */
bool tlkmdi_interphone_getData_BtDownBuff(uint8_t *data, uint16_t len)
{
    uint16_t tempLen;

    if (data == NULL || len == 0 || (len & 0x01) != 0) {
        tlkapi_printf(APP_LOG_EN, "get data BtDownBuff error");
        return false;
    }

    if (tlkmdi_interphone_get_BtDownBuff_DataLen() < len) {
        // gpio_set_high_level(GPIO_CHN2);
        // gpio_set_low_level(GPIO_CHN2);
        tlkapi_printf(APP_LOG_EN, "get BtDownBuff null");
        return false;
    }

    if (s_interphone_ctl.down_buff_rptr + len <= s_interphone_ctl.down_buff_len) {
        tempLen = len;
    } else {
        tempLen = s_interphone_ctl.down_buff_len - s_interphone_ctl.down_buff_rptr;
    }

    if (tempLen != 0) {
        tmemcpy(data, ((uint8_t *)g_interphone_bt_down_buff) + s_interphone_ctl.down_buff_rptr, tempLen);
    }

    if (tempLen == len) {
        s_interphone_ctl.down_buff_rptr += tempLen;
    } else {
        s_interphone_ctl.down_buff_rptr = len - tempLen;
        tmemcpy(data + tempLen, ((uint8_t *)g_interphone_bt_down_buff), s_interphone_ctl.down_buff_rptr);
    }

    s_interphone_ctl.down_buff_rptr %= s_interphone_ctl.down_buff_len;

    return true;
}

/**
 * @brief       Fill BT down buffer with data
 * @param[in]   pData - Data to fill
 * @param[in]   dataLen - Length of data
 * @return      true - Success, false - Failure
 */
bool tlkmdi_interphone_fill_BtDownBuff(uint8_t *pData, uint16_t dataLen)
{
    uint16_t offset;
    uint8_t *pBuffer = (uint8_t *)g_interphone_bt_down_buff;

    if (pData == NULL || dataLen == 0 || (dataLen & 0x01) != 0) {
        tlkapi_printf(APP_LOG_EN, "fill BtDownBuff error");
        return false;
    }

    if (tlkmdi_interphone_get_BtDownBuff_IdleLen() < dataLen) {
        tlkapi_printf(APP_LOG_EN, "fill BtDownBuff overflow");
        return false;
    }

    if (s_interphone_ctl.down_buff_wptr + dataLen > s_interphone_ctl.down_buff_len) {
        offset = s_interphone_ctl.down_buff_len - s_interphone_ctl.down_buff_wptr;
    } else {
        offset = dataLen;
    }

    tmemcpy(pBuffer + s_interphone_ctl.down_buff_wptr, pData, offset);
    if (offset < dataLen) {
        tmemcpy(pBuffer, pData + offset, dataLen - offset);
    }

    s_interphone_ctl.down_buff_wptr += dataLen;
    s_interphone_ctl.down_buff_wptr %= s_interphone_ctl.down_buff_len;

    return true;
}

/**
 * @brief       Synchronize BT up buffer
 * @param[in]   sample - Sample count
 * @return      None
 */
void tlkmdi_interphone_sync_BtUpBuff(uint16_t sample)
{
    s_interphone_ctl.up_buff_rptr = (s_interphone_ctl.up_buff_wptr - sample * sizeof(codec_int)) & (s_interphone_ctl.up_buff_len - 1);
    tlkapi_printf(APP_LOG_EN, "sync BtUpBuff wptr rptr %d %d", s_interphone_ctl.up_buff_wptr, s_interphone_ctl.up_buff_rptr);
}

/**
 * @brief       Get idle length of BT up buffer
 * @param       None
 * @return      Idle length
 */
uint16_t tlkmdi_interphone_get_BtUpBuff_IdleLen(void)
{
    uint16_t len = 0;
    if (s_interphone_ctl.up_buff_wptr >= s_interphone_ctl.up_buff_rptr) {
        len = s_interphone_ctl.up_buff_len - (s_interphone_ctl.up_buff_wptr - s_interphone_ctl.up_buff_rptr);
    } else {
        len = s_interphone_ctl.up_buff_rptr - s_interphone_ctl.up_buff_wptr;
    }
    return len;
}

/**
 * @brief       Get data length of BT up buffer
 * @param       None
 * @return      Data length
 */
uint16_t tlkmdi_interphone_get_BtUpBuff_DataLen(void)
{
    return (s_interphone_ctl.up_buff_len - tlkmdi_interphone_get_BtUpBuff_IdleLen());
}

/**
 * @brief       Get data from BT up buffer
 * @param[out]  data - Data buffer
 * @param[in]   len - Length of data to get
 * @return      true - Success, false - Failure
 */
bool tlkmdi_interphone_getData_BtUpBuff(uint8_t *data, uint16_t len)
{
    uint16_t tempLen;

    if (data == NULL || len == 0 || (len & 0x01) != 0) {
        tlkapi_printf(APP_LOG_EN, "get data BtUpBuff error");
        return false;
    }

    if (tlkmdi_interphone_get_BtUpBuff_DataLen() < len) {
        tlkapi_printf(APP_LOG_EN, "get BtUpBuff null");
        return false;
    }

    if (s_interphone_ctl.up_buff_rptr + len <= s_interphone_ctl.up_buff_len) {
        tempLen = len;
    } else {
        tempLen = s_interphone_ctl.up_buff_len - s_interphone_ctl.up_buff_rptr;
    }

    if (tempLen != 0) {
        tmemcpy(data, ((uint8_t *)g_interphone_bt_up_buff) + s_interphone_ctl.up_buff_rptr, tempLen);
    }

    if (tempLen == len) {
        s_interphone_ctl.up_buff_rptr += tempLen;
    } else {
        s_interphone_ctl.up_buff_rptr = len - tempLen;
        tmemcpy(data + tempLen, ((uint8_t *)g_interphone_bt_up_buff), s_interphone_ctl.up_buff_rptr);
    }

    s_interphone_ctl.up_buff_rptr %= s_interphone_ctl.up_buff_len;

    return true;
}

/**
 * @brief       Fill BT up buffer with data
 * @param[in]   pData - Data to fill
 * @param[in]   dataLen - Length of data
 * @return      true - Success, false - Failure
 */
bool tlkmdi_interphone_fill_BtUpBuff(uint8_t *pData, uint16_t dataLen)
{
    uint16_t offset;
    uint8_t *pBuffer = (uint8_t *)g_interphone_bt_up_buff;

    if (pData == NULL || dataLen == 0 || (dataLen & 0x01) != 0) {
        tlkapi_printf(APP_LOG_EN, "fill BtUpBuff error");
        return false;
    }

    if (tlkmdi_interphone_get_BtUpBuff_IdleLen() < dataLen) {
        tlkapi_printf(APP_LOG_EN, "fill BtUpBuff overflow");
        return false;
    }

    if (s_interphone_ctl.up_buff_wptr + dataLen > s_interphone_ctl.up_buff_len) {
        offset = s_interphone_ctl.up_buff_len - s_interphone_ctl.up_buff_wptr;
    } else {
        offset = dataLen;
    }

    tmemcpy(pBuffer + s_interphone_ctl.up_buff_wptr, pData, offset);
    if (offset < dataLen) {
        tmemcpy(pBuffer, pData + offset, dataLen - offset);
    }

    s_interphone_ctl.up_buff_wptr += dataLen;
    s_interphone_ctl.up_buff_wptr %= s_interphone_ctl.up_buff_len;

    return true;
}

#endif //TLKMW_INTERPHONE_EN
