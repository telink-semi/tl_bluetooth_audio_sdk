/********************************************************************************************************
 * @file    tlkmdi_audio_dsp_bypass_test.c
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
#include "tlkmdi_audio_dsp_bypass_test.h"

#if TLK_AUDIO_DSP_BYPASS_TEST_ENABLE && (CHIP_TYPE == CHIP_TYPE_TL752X)


#define DSP_BYPASS_MODE   1
#define CODEC_BYPASS_MODE 2

uint8_t g_audio_codec_test_mode;
uint8_t g_audio_codec_test_started;

/**
 * @brief   Audio timer interrupt handler
 * @param   None
 * @return  None
 */
_attribute_retention_code_ void tlkmw_audio_timer_irq_handler(void)
{
    if (g_audio_codec_test_mode) {
        tlkmw_audio_codec_test_main();
    } else {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "error handle");
    }
}

/**
 * @brief   Open codec for test
 * @param   None
 * @return  None
 */
void tlkmw_audio_codec_test_open_codec(void)
{
    tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_STEREO,
#if AUDIO_PATH_24BITS_EN
                      TLKDRV_CODEC_BITDEPTH_24,
#else
                      TLKDRV_CODEC_BITDEPTH_16,
#endif
                      AUDIO_SAMPLE_RATE_48K, 0);

    tlkdrv_codec_sync_play_samples(48 * 20);
    tlkdrv_codec_sync_mic_samples(48 * 20);
}

/**
 * @brief   Callback function for processing dsp messages in codec
 * @param   enc_buff_wptr - encode buffer write pointer
 * @param   type - message type
 * @return  None
 */
void audio_codec_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type)
{
    (void)enc_buff_wptr;
    (void)type;

    uint16_t pcm_data_len;
    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&pcm_data_len, IPC_DATA_PATH_0);

    if (pcm_data == NULL) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "udio_codec_dsp pcm_data is NULL");
        return;
    }

    tlkdrv_codec_fillSpkBuff((uint8_t *)pcm_data, pcm_data_len);
}

extern signed short sin_48k_mono_d1_dbg[] __attribute__((aligned(4)));
codec_mono_int      pcm_mono[48];
codec_mono_int      pcm_stereo[48 * 2];

/**
 * @brief   Codec test player
 * @param   mode - test mode
 * @return  None
 */
_attribute_retention_code_ void tlkmw_audio_codec_test_player(uint8_t mode)
{
    //	(void)mode;
    /* 48K 1ms */
    uint16_t samples_num = 48;
    uint16_t codec_buffer_avail_size;

    //    int16_t rptr = (audio_get_tx_dma_rptr(gTlkdrvCodecSpkDmaChn)) - ((uint32_t)gpTlkDrvCodecSpkBuffer);
    codec_buffer_avail_size = tlkdrv_codec_get_spk_buf_idle_size();

    //    tlkapi_printf(1, "codec free_len: %d, rptr: %d", codec_buffer_avail_size, rptr);

    if (codec_buffer_avail_size > CODEC_SPK_FIFO_SAMPLES / 4) {
        if (mode == DSP_BYPASS_MODE) {
            tlkdrv_codec_readMicData((uint8_t *)pcm_stereo, samples_num * sizeof(codec_int), 0);

            app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(IPC_DATA_PATH_0);
            p_dsp_app_ctx->alg_type          = DSP_BYPASS;

            d25f_send_audio_data_to_dsp((uint8_t *)pcm_stereo, samples_num * sizeof(codec_int), IPC_DATA_PATH_0);
        } else if (mode == CODEC_BYPASS_MODE) {
#if 1
            bool ret = tlkdrv_codec_readMicData((uint8_t *)pcm_stereo, samples_num * sizeof(codec_int), 0);
            if (ret == false) {
                //				tlkapi_printf(1, "[QQQQ], get mic data error");
            }
#else
            static int sin_count = 0;

            for (uint16_t i = 0; i < samples_num; i++) {
                pcm_mono[i] = sin_48k_mono_d1_dbg[sin_count++];
                sin_count %= 48;
            }

            for (uint16_t i = 0; i < samples_num; i++) {
                pcm_stereo[2 * i]     = pcm_mono[i]; // >> 8;
                pcm_stereo[2 * i + 1] = pcm_mono[i]; // >> 8;
            }
#endif

            tlkdrv_codec_fillSpkBuff((uint8_t *)pcm_stereo, samples_num * sizeof(codec_int));
        }
    }
}

/**
 * @brief   Main function for codec test
 * @param   None
 * @return  None
 */
_attribute_retention_code_ void tlkmw_audio_codec_test_main(void)
{
    if (g_audio_codec_test_started == 0) {
        g_audio_codec_test_started = 1;
        tlkmw_audio_codec_test_open_codec();
        tlkapi_printf(1, "[test] [codec] enter codec mic spk loopback mode");

        if (g_audio_codec_test_mode == DSP_BYPASS_MODE) {
            tlkmw_dsp_resume();
            ipc_msg_register_data_process_done_cb(audio_codec_dsp_msg_process_callback, DSP_BYPASS, IPC_DATA_PATH_0);
            tlkapi_printf(1, "[test] [dsp] enter codec mic to dsp to spk loopback mode");
        }
    }

    // gpio_set_high_level(GPIO_PB12);
    // gpio_set_low_level(GPIO_PB12);

    tlkmdi_audio_task_set_next_irq(500);

    tlkmw_audio_codec_test_player(g_audio_codec_test_mode);
}

/**
 * @brief   Start codec test
 * @param   mode - test mode
 * @return  None
 */
void tlkmw_audio_start_codec_test(uint8_t mode)
{
    AUDIO_GLOBAL_INT_DISABLE();
    tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, tlkmw_audio_timer_irq_handler);

    g_audio_codec_test_mode = mode;

    if (g_audio_codec_test_mode == 0x00) {
        tlkmw_audio_stop_codec_test();
        return;
    }

    plic_interrupt_enable(IRQ_TIMER0_0);
    plic_set_priority(IRQ_TIMER0_0, 1);
    tlkmdi_audio_task_set_next_irq(500);

    AUDIO_GLOBAL_INT_RESTORE();
}

/**
 * @brief   Stop codec test
 * @param   None
 * @return  None
 */
void tlkmw_audio_stop_codec_test(void)
{
    g_audio_codec_test_started = 0;
}

#endif
