/********************************************************************************************************
 * @file    tlkmdi_audio_codec_test.c
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
#include "tlkapi/tlkapi.h"
#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkmdi_audio_codec_test.h"
#include "tlkalg/audio/lc3_plus/tlkalg_lc3_plus_interface.h"

#if (TLKBTP_CFG_A2DPSNK_ENABLE && MCU_CORE_TYPE == MCU_CORE_TL752X)

#define CODEC_TEST_SPK_MODE          1
#define CODEC_TEST_MIC_LOOPBACK_MODE 2

uint8_t g_codec_test_mode;
uint8_t g_codec_test_started;

extern int16_t  g_tone_buff[512];
extern int16_t *g_tone_buff_ptr;

/**
 * @brief       Main function for codec testing
 * @return      None
 */
void tlkmdi_codec_test_main(void);

#if AUDIO_CODEC_TEST

signed short sin_48k_mono_d1_dbg[] __attribute__((aligned(4))) = {
    0, 1069,  2120,  3134,  4095,  4986,  5792,  6499,  7094,  7568,  7912,  8121,  8191,  8121,  7912,  7568,  7094,  6499,  5792,  4986,  4096,  3134,  2120,  1069,
    0, -1069, -2120, -3134, -4095, -4986, -5792, -6499, -7094, -7568, -7912, -8121, -8191, -8121, -7912, -7568, -7094, -6499, -5792, -4986, -4096, -3134, -2120, -1069,
};

/**
 * @brief Convert PCM S32 format to S16 format.
 * @param[in] dst - Destination buffer for S16 data.
 * @param[in] src - Source buffer with S32 data.
 * @param[in] len - Length of data to convert.
 * @return Result of conversion.
 */
_attribute_ram_code_ int pcm_s32_2_s16(int16_t *dst, const int32_t *src, uint16_t len)
{
    if (!len || dst == NULL || src == NULL) {
        return -1;
    }

    while (len--) {
        *dst++ = *src++ >> 8;
    };

    return 0;
}

/**
 * @brief       Handle audio test timer interrupt
 * @return      None
 */
_attribute_retention_code_ void tlkmdi_audio_test_timer_irq_handler(void)
{
    if (g_codec_test_mode) {
        tlkmdi_codec_test_main();
    }
}

/**
 * @brief       Open codec for testing
 * @param[in]   mode - Test mode (1: speaker test, 2: mic loopback test)
 * @return      None
 */
void tlkmdi_audio_codec_test_open_codec(uint8_t mode)
{
    if (mode == CODEC_TEST_SPK_MODE) {
        tlkapi_printf(1, "tlkdrv_open_codec TLKDRV_CODEC_BITDEPTH_16 48K");

        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 48000, 0);

        //tlkdrv_codec_sync_speaker_samples(128 * 6);
        //        tlkdrv_codec_sync_play_samples(128 * 4);
    } else if (mode == CODEC_TEST_MIC_LOOPBACK_MODE) {
        tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, AUDIO_SAMPLE_RATE_48K, 0);

        tlkdrv_codec_sync_play_samples(48 * 10);

        tlkdrv_codec_sync_mic_samples(48 * 10);
    }
}

/**
 * @brief  tlkmdi_audio_codec_test_player, 48K
 *
 * @param[in]  mode: 1 speak test, play sine wav; 2 mic loopback test
 *
 * @returns none
 */
/**
 * @brief       Play audio for codec testing
 * @param[in]   mode - Test mode (1: speaker test, 2: mic loopback test)
 * @return      None
 */
_attribute_retention_code_ void tlkmdi_audio_codec_test_player(uint8_t mode)
{
    /* 48K 5ms */
    uint16_t       samples_num = 128;
    uint16_t       codec_buffer_avail_size;
    codec_mono_int pcm_mono[480 * 2];
    codec_mono_int pcm_stereo[480 * 2];
    uint16_t       i;

    int16_t pcm16_tone_left[240];
    int16_t pcm16_tone_right[240];

    int16_t rptr            = (audio_get_tx_dma_rptr(TLKDRV_CODEC_SPK_DMA)) - ((uint32_t)gpTlkDrvCodecSpkBuffer);
    codec_buffer_avail_size = tlkdrv_codec_get_spk_buf_idle_size();

    tlkapi_printf(0, "codec free_len: %d, rptr: %d", codec_buffer_avail_size, rptr);

    if (codec_buffer_avail_size > CODEC_SPK_FIFO_SAMPLES / 2) {
        if (mode == CODEC_TEST_SPK_MODE) {
            static int sin_count = 0;

            for (i = 0; i < samples_num; i++) {
                pcm_mono[i] = sin_48k_mono_d1_dbg[sin_count++];
                sin_count %= 48;
            }

            if (tone_is_playing()) {
                tmemset(g_tone_buff, 0, sizeof(g_tone_buff));
                tone_get_sample((int16_t *)g_tone_buff, samples_num * sizeof(tone_int), 48000);

                tmemset(pcm16_tone_left, 0, sizeof(pcm16_tone_left));
                pcm_s32_2_s16(pcm16_tone_left, (const int32_t *)pcm_mono, samples_num);

                tmemset(pcm16_tone_right, 0, sizeof(pcm16_tone_right));
                pcm_s32_2_s16(pcm16_tone_right, (const int32_t *)pcm_mono, samples_num);

                g_tone_buff_ptr = app_mix_tone(pcm16_tone_left, pcm16_tone_right, g_tone_buff, samples_num);

                for (i = 0; i < samples_num; i++) {
                    pcm_stereo[2 * i]     = (int32_t)pcm16_tone_left[i] << 8;
                    pcm_stereo[2 * i + 1] = (int32_t)pcm16_tone_right[i] << 8;

                    /*
                    int32_t left_tone = (int32_t)g_tone_buff[2*i] << 8;
                    int32_t right_tone = (int32_t)g_tone_buff[2*i+1] << 8;

                    pcm_stereo[2*i] = left_tone;
                    pcm_stereo[2*i + 1] = right_tone;
                    */
                }
            } else {
                for (i = 0; i < samples_num; i++) {
                    pcm_stereo[2 * i]     = pcm_mono[i]; // >> 8;
                    pcm_stereo[2 * i + 1] = pcm_mono[i]; // >> 8;
                }
            }

        } else if (mode == CODEC_TEST_MIC_LOOPBACK_MODE) {
            samples_num = 48 * 10 * 2;
            tlkdrv_codec_readMicData((uint8_t *)pcm_stereo, samples_num * sizeof(codec_mono_int), 0);

            uint8_t enc_data[480];
            //  gpio_set_high_level(GPIO_PC0);

            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
            if (p_audio_alg_if->audio_alg_process) {
                p_audio_alg_if->audio_alg_process((uint8_t *)pcm_stereo, (uint8_t *)enc_data, 480, ALG_WIDTH_24, ALG_CHANNEL_STEREO);
            }
            //  gpio_set_low_level(GPIO_PC0);

            //  gpio_set_high_level(GPIO_PC0);
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
            if (p_audio_alg_if->audio_alg_process) {
                p_audio_alg_if->audio_alg_process((uint8_t *)enc_data, (uint8_t *)pcm_mono, 90, ALG_WIDTH_24, ALG_CHANNEL_STEREO);
            }
            //  gpio_set_low_level(GPIO_PC0);
        }

        tlkdrv_codec_fillSpkBuff((uint8_t *)pcm_stereo, samples_num * sizeof(codec_int));
    }
}

/**
 * @brief       Main function for codec testing
 * @return      None
 */
void tlkmdi_codec_test_main(void)
{
    if (g_codec_test_started == 0) {
        g_codec_test_started = 1;
        tlkmdi_audio_codec_test_open_codec(g_codec_test_mode);
#if TLKALG_ALG_LOOPBACK_TEST_ENABLE
        tlkapi_trace(0xFFFFFFFF, "[QQQQ LC3 PLUS]", "lc3 plus tlkmdi_audio_codec_test_open_codec");
        tlkmdi_audio_alg_init();
#endif
    }

    tlkmdi_audio_task_set_next_irq(1000);

    tlkmdi_audio_codec_test_player(g_codec_test_mode);
}

/**
 * @brief  Start codec test mode
 *
 * @param[in]  mode: 1 speak test, play sine wav; 2 mic loopback test
 *
 * @returns none
 */
/**
 * @brief       Start codec test mode
 * @param[in]   mode - Test mode (1: speaker test, 2: mic loopback test)
 * @return      None
 */
void tlkmdi_audio_start_codec_test(uint8_t mode)
{
    AUDIO_GLOBAL_INT_DISABLE();
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, tlkmdi_audio_test_timer_irq_handler);

    g_codec_test_mode = mode;

#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
    plic_interrupt_enable(IRQ_TIMER0_0);
    plic_set_priority(IRQ_TIMER0_0, 1);
#else
    plic_interrupt_enable(IRQ_TIMER0);
    plic_set_priority(IRQ_TIMER0, 1);
#endif

    tlkmdi_audio_task_set_next_irq(1000);

    tlkapi_printf(1, "tlkmdi_audio_start_codec_test start timer");

    AUDIO_GLOBAL_INT_RESTORE();
}

/**
 * @brief       Stop codec test mode
 * @return      None
 */
void tlkmdi_audio_stop_codec_test(void)
{
    g_codec_test_started = 0;
}

#endif // LL_AUDIO_CODEC_TEST

#if TLKALG_ALG_LOOPBACK_TEST_ENABLE
uint8_t alg_lb_test_flag = 0;

/**
 * @brief       Set algorithm loopback enable flag
 * @param[in]   flag - Enable flag
 * @return      None
 */
void audio_alg_set_loopback_enable(uint8_t flag)
{
    uint8_t mode = 2;

    if (flag) {
        if (alg_lb_test_flag == 0) {
            alg_lb_test_flag = 1;
            mode             = 2;
            tlkmdi_audio_start_codec_test(mode);
            tlkapi_printf(1, "tlkmdi_audio_start_codec_test");
        }
    } else {
        if (alg_lb_test_flag == 1) {
            alg_lb_test_flag = 0;
        }
    }
}

/**
 * @brief       Get algorithm loopback enable flag
 * @return      Loopback enable flag
 */
uint8_t audio_alg_get_loopback_enable(void)
{
    return alg_lb_test_flag;
}

#if TLKALG_LC3_PLUS_ENC_ENABLE && TLKALG_LC3_PLUS_DEC_ENABLE
static uint8_t *s_alg_lc3_plus_dec_buffer         = NULL;
static uint8_t *s_alg_lc3_plus_enc_buffer         = NULL;
static uint8_t *s_alg_lc3_plus_dec_scratch_buffer = NULL;
static uint8_t *s_alg_lc3_plus_enc_scratch_buffer = NULL;
#endif

/**
 * @brief       Initialize audio algorithm for testing
 * @return      None
 */
void tlkmdi_audio_alg_init(void)
{
    audio_alg_interface_t *p_audio_alg_if;

#if TLKALG_LC3_PLUS_ENC_ENABLE && TLKALG_LC3_PLUS_DEC_ENABLE
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
    if (s_alg_lc3_plus_dec_buffer == NULL) {
        p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_DEC_NORMAL, NULL); //TODO:WQ
                                                                             //        	p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_DEC_ULTRA_LOW_LATENCY, NULL);
        //ENCORD BUFF
        uint8_t  size_param            = (ALG_SIZE_TYPE_ENCODER << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        uint16_t lc3_plus_dec_mem_size = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_dec_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_dec_mem_size);
        if (s_alg_lc3_plus_dec_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 plus dec buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_dec_buffer, size_param);

        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_dec_mem_size: %d", lc3_plus_dec_mem_size);

        //SCRATCH BUFF
        size_param                        = (ALG_SIZE_TYPE_SCRATCH << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        lc3_plus_dec_mem_size             = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_dec_scratch_buffer = (uint8_t *)tlkalg_malloc_func(lc3_plus_dec_mem_size);
        if (s_alg_lc3_plus_dec_scratch_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3 plus dec scratch buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_dec_scratch_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_dec_mem_size: %d", lc3_plus_dec_mem_size);
    }

    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
    if (s_alg_lc3_plus_enc_buffer == NULL) {
        p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_ENC_NORMAL, NULL); //TODO:WQ
                                                                             //        	p_audio_alg_if->audio_alg_param_set(LC3_PLUS_TYPE_ENC_ULTRA_LOW_LATENCY, NULL);
        //ENCORD BUFF
        uint8_t  size_param            = (ALG_SIZE_TYPE_ENCODER << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        uint16_t lc3_plus_enc_mem_size = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_enc_buffer      = (uint8_t *)tlkalg_malloc_func(lc3_plus_enc_mem_size);

        if (s_alg_lc3_plus_enc_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus enc buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_enc_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc_mem_size:: %d", lc3_plus_enc_mem_size);

        //SCRATCH BUFF
        size_param                        = (ALG_SIZE_TYPE_SCRATCH << 4) | (ALG_CHANNEL_STEREO & 0x0F);
        lc3_plus_enc_mem_size             = p_audio_alg_if->audio_alg_get_size(size_param);
        s_alg_lc3_plus_enc_scratch_buffer = (uint8_t *)tlkalg_malloc_func(lc3_plus_enc_mem_size);

        if (s_alg_lc3_plus_enc_scratch_buffer == NULL) {
            tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc scratch_buffer alloc failed");
        }
        p_audio_alg_if->audio_alg_init(s_alg_lc3_plus_enc_scratch_buffer, size_param);
        tlkapi_printf(APP_AUDIO_LOG_EN, "lc3_plus_enc_mem_size:: %d", lc3_plus_enc_mem_size);
    }

#endif
}
#endif

#endif
