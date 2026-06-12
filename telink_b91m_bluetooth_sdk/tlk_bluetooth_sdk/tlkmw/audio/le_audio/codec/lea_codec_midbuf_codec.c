/********************************************************************************************************
 * @file    lea_codec_midbuf_codec.c
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
#include "common/types.h"

#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkalg/audio/asrc_24bit/tlka_asrc_api.h"

#include "lea_codec.h"
#include "lea_codec_hal.h"

#if TLKADU_MIDBUF_ENABLE
#include "vendor/GameSir_Xiaoji/audio_mw/tlkaud_audio_mw.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#endif

#if (!LEA_CODEC_NO_ASRC && TLK_MW_LE_AUDIO_ENABLE)

#if (LE_AUDIO_CODEC_INPUT_TYPE == LE_AUDIO_CODEC_MIDDLE_BUF)

#if TLKADU_MIDBUF_ENABLE
extern void tlkaud_mainloop_thread(void);
extern void tlkaud_timer_thread(void);
extern void tlkaud_high_priority_thread(void);
extern void tlkaud_fifo_irq_handler(void);
extern void tlkaud_fifo_config(uint16_t byte_num);
extern void tlkaud_fifo_irq_disable(void);
#endif

static uint8_t *s_codec_input_asrc_buffer   = NULL;
static uint8_t  s_codec_input_asrc_type     = TLKA_ASRC_FLAG_END;
static uint32_t s_codec_input_sampling_rate = 0;
static uint8_t  s_codec_input_count         = 0;

/**
 * @brief       Initialize ASRC module for input stream.
 * @return      none.
 */
static void lea_codec_input_init_asrc_module(void)
{
    s_codec_input_sampling_rate = lea_codec_get_input_sampling_rate();
    s_codec_input_asrc_type     = TLKA_ASRC_FLAG_END;

    if (s_codec_input_sampling_rate == 8000) {
        // ASRC current not support.
        s_codec_input_count = 24;
    } else if (s_codec_input_sampling_rate == 16000) {
        s_codec_input_asrc_type = TLKA_ASRC_48_TO_16;
#if AUDIO_PATH_24BITS_EN
        s_codec_input_count = 24;
#else
        s_codec_input_count = 12;
#endif
    } else if (s_codec_input_sampling_rate == 24000) {
        s_codec_input_asrc_type = TLKA_ASRC_48_TO_24;
#if AUDIO_PATH_24BITS_EN
        s_codec_input_count = 16;
#else
        s_codec_input_count = 8;
#endif
    } else if (s_codec_input_sampling_rate == 32000) {
        s_codec_input_asrc_type = TLKA_ASRC_48_TO_32;
#if AUDIO_PATH_24BITS_EN
        s_codec_input_count = 12;
#else
        s_codec_input_count = 6;
#endif
    } else if (s_codec_input_sampling_rate == 48000) {
        // not need ASRC.
#if AUDIO_PATH_24BITS_EN
        s_codec_input_count = 8;
#else
        s_codec_input_count = 4;
#endif
    }
    // tlk_printf("input samplerate:%d", s_codec_input_sampling_rate);

    if (s_codec_input_asrc_type != TLKA_ASRC_FLAG_END) {
        uint16_t asrc_buffer_size = tlka_asrc_16_bit_get_size(s_codec_input_asrc_type, TLKA_ASRC_STEREO);
        s_codec_input_asrc_buffer = (uint8_t *)tlkalg_malloc_func(asrc_buffer_size);
        tlk_printf("codec input asrc buffer size:%d, ptr:%x", asrc_buffer_size, s_codec_input_asrc_buffer);
        if (s_codec_input_asrc_buffer != NULL) {
            int ret = tlka_asrc_16_bit_init(s_codec_input_asrc_buffer, s_codec_input_asrc_type, TLKA_ASRC_STEREO);
            tlk_printf("codec input asrc init ret:%d", ret);
        }
    }
}

/**
 * @brief       Deinitialize ASRC module for input stream.
 * @return      none.
 */
static void lea_codec_input_deinit_asrc_module(void)
{
    s_codec_input_asrc_type = TLKA_ASRC_FLAG_END;

    if (s_codec_input_asrc_buffer != NULL) {
        tlkalg_free_func(s_codec_input_asrc_buffer);
        s_codec_input_asrc_buffer = NULL;
    }
}

/**
 * @brief       Initialize input stream.
 * @return      none.
 */
void lea_codec_input_stream_init(void)
{
    lea_codec_input_init_asrc_module();

    tlkaud_set_audio_mode(AUDIO_LEA_MIC);
    // #if AUDIO_PATH_24BITS_EN
    //     tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, 48000, 0);
    // #else
    //     tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 48000, 0);
    // #endif
}

/**
 * @brief       Deinitialize input stream.
 * @return      none.
 */
void lea_codec_input_stream_deinit(void)
{
    lea_codec_input_deinit_asrc_module();
    // tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
    tlkaud_clear_audio_mode(AUDIO_LEA_MIC);
}

/**
 * @brief       Clean input buffer.
 * @return      none.
 */
void lea_codec_input_clean_buffer(void)
{
    tlkdrv_codec_sync_mic_samples(5 * 48);
}

bool lea_get_mic_data_stereo(uint8_t *pdata, uint16_t samples)
{
    mic_buf_typ data_buff[samples];
    mic_chnl_e  mic_chnl = 0;
    // gpio_set_high_level(GPIO_CHN1);
    bool ret = tlkaud_sidetone_get_mic_data_48k(data_buff, samples, &mic_chnl);
    // gpio_set_low_level(GPIO_CHN1);
    if (ret) {
#if CODEC_ADC_MONO_MODE
        mic_buf_typ *pdes = (mic_buf_typ *)pdata;
        for (int i = 0; i < samples; i++) {
            pdes[2 * i]     = data_buff[i];
            pdes[2 * i + 1] = data_buff[i];
        }
#else
        tmemcpy(pdata, data_buff, samples * sizeof(mic_buf_typ));
#endif
        // if (mic_chnl == MIC_CHNL_MONO) {
        //     adc_mono* pdes = (adc_mono*)pdata;
        //     for (int i = 0; i < samples; i++) {
        //         pdes[2*i] = data_buff[i];
        //         pdes[2*i+1] = data_buff[i];
        //     }
        // } else if (mic_chnl == MIC_CHNL_STEREO) {
        //     tmemcpy(pdata, data_buff, samples*sizeof(mic_buf_typ));
        // }
    }
    return ret;
}

// adc_mono mic_stereo_48k_24bit[480 * 2];

/**
 * @brief       Get input audio data.
 * @param[out]  left_data    - pointer to left channel audio data buffer.
 * @param[out]  right_data   - pointer to right channel audio data buffer.
 * @param[in]   sample_num   - number of samples per channel to read.
 * @return      true if data is successfully read, false otherwise.
 */
bool lea_codec_input_get_audio_data(int16_t *left_data, int16_t *right_data, uint16_t sample_num)
{
    if ((tlkaud_get_mic_des() == MIC_TO_UAC) && (tlkaud_get_mic_mode() & AUDIO_UAC_MIC)) {
        //lea fill mute data
        for (int i = 0; i < sample_num; i++) {
            left_data[i] = right_data[i] = 0;
        }

#if (TLK_USB_UAC_ENABLE)
        //uac fill mic data
        mic_buf_typ mic_data_24bit[160];
        int16_t     mic_data_16bit[160];
        mic_chnl_e  mic_chnl = 0;
        tlkaud_sidetone_get_mic_data_16k(mic_data_24bit, 160, &mic_chnl);
        for (int j = 0; j < 160; j++) {
            mic_data_16bit[j] = (mic_data_24bit[j] >> 8) & 0xffff;
        }
        tlkusb_uac_write_iso_in_samples((int16_t *)mic_data_16bit, 160);
#endif

    } else {
#if (TLK_USB_UAC_ENABLE)
        //fill uac mute data
        int16_t uac_data[160];
        tmemset(uac_data, 0, 160 * sizeof(int16_t));
        tlkusb_uac_write_iso_in_samples((int16_t *)uac_data, 160);
#endif

        //fill lea mic data
        uint16_t sample_num_48k_bytes = sample_num * s_codec_input_count;
        int16_t  mic_stereo_48k_16bit[480 * 2];
        adc_mono mic_stereo_48k_24bit[480 * 2];

        // tlk_printf("LEA in: sample_num: %d, s_codec_input_count: %d", sample_num, s_codec_input_count);

        // if (tlkdrv_codec_readMicData((uint8_t *)mic_stereo_48k_24bit, sample_num_48k_bytes, NULL) == false) {
        //     return false;
        // }
        if (lea_get_mic_data_stereo((uint8_t *)mic_stereo_48k_24bit, sample_num_48k_bytes / 8) == false) {
            return false;
        }

        for (int i = 0; i < 480 * 2; i++) { //24bit -> 16bit
            mic_stereo_48k_16bit[i] = (mic_stereo_48k_24bit[i] >> 8) & 0xffff;
        }

        int16_t  pcm_stereo_16bit[sample_num * 2];
        int16_t *p_pcm_stereo = mic_stereo_48k_16bit;
        if (s_codec_input_asrc_type != TLKA_ASRC_FLAG_END) {
            tlka_asrc_16_bit_process_frame(s_codec_input_asrc_buffer, mic_stereo_48k_16bit, sample_num_48k_bytes / 8, pcm_stereo_16bit);
            p_pcm_stereo = pcm_stereo_16bit;
        }

        for (int i = 0; i < sample_num; i++) {
            left_data[i]  = p_pcm_stereo[i * 2];
            right_data[i] = p_pcm_stereo[i * 2 + 1];
        }
    }
    return true;
}

#endif

#if (LE_AUDIO_CODEC_OUTPUT_TYPE == LE_AUDIO_CODEC_MIDDLE_BUF)
static uint8_t *s_codec_output_asrc_buffer   = NULL;
static uint8_t  s_codec_output_asrc_type     = TLKA_ASRC_FLAG_END;
static uint32_t s_codec_output_sampling_rate = 0;
static uint8_t  s_codec_output_count         = 0;

/**
 * @brief       Initialize ASRC module for output stream.
 * @return      none.
 */
static void lea_codec_output_init_asrc_module(void)
{
    s_codec_output_sampling_rate = lea_codec_get_output_sampling_rate();
    s_codec_output_asrc_type     = TLKA_ASRC_FLAG_END;

    if (s_codec_output_sampling_rate == 8000) {
        // ASRC current not support.
        s_codec_output_count = 24;
    } else if (s_codec_output_sampling_rate == 16000) {
        s_codec_output_asrc_type = TLKA_ASRC_16_TO_48;
        s_codec_output_count     = 12;
    } else if (s_codec_output_sampling_rate == 24000) {
        s_codec_output_asrc_type = TLKA_ASRC_24_TO_48;
        s_codec_output_count     = 8;
    } else if (s_codec_output_sampling_rate == 32000) {
        s_codec_output_asrc_type = TLKA_ASRC_32_TO_48;
        s_codec_output_count     = 6;
    } else if (s_codec_output_sampling_rate == 48000) {
        // not need ASRC.
        s_codec_output_count = 4;
    }

    tlk_printf("sampling_rate: %d", s_codec_output_sampling_rate);

    if (s_codec_output_asrc_type != TLKA_ASRC_FLAG_END) {
        uint16_t asrc_buffer_size  = tlka_asrc_16_bit_get_size(s_codec_output_asrc_type, TLKA_ASRC_STEREO);
        s_codec_output_asrc_buffer = (uint8_t *)tlkalg_malloc_func(asrc_buffer_size);
        tlk_printf("codec output asrc buffer size:%d, ptr:%x", asrc_buffer_size, s_codec_output_asrc_buffer);
        if (s_codec_output_asrc_buffer != NULL) {
            int ret = tlka_asrc_16_bit_init(s_codec_output_asrc_buffer, s_codec_output_asrc_type, TLKA_ASRC_STEREO);
            tlk_printf("codec output asrc init ret:%d", ret);
        }
    }
}

/**
 * @brief       Deinitialize ASRC module for output stream.
 * @return      none.
 */
static void lea_codec_output_deinit_asrc_module(void)
{
    s_codec_output_asrc_type = TLKA_ASRC_FLAG_END;

    if (s_codec_output_asrc_buffer != NULL) {
        tlkalg_free_func(s_codec_output_asrc_buffer);
        s_codec_output_asrc_buffer = NULL;
    }
}

static bool sink_sync_flag = false;

/**
 * @brief       Initialize output stream.
 * @return      none.
 */
void lea_codec_output_stream_init(void)
{
    lea_codec_output_init_asrc_module();
    // tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, 48000, 0);
}

/**
 * @brief       Deinitialize output stream.
 * @return      none.
 */
void lea_codec_output_stream_deinit(void)
{
    lea_codec_output_deinit_asrc_module();
    // tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_SPK);
    tlkaud_clear_audio_mode(AUDIO_LEA_SPK);
    sink_sync_flag = false;
}

/********* volume control code begin *********/
const int16_t lea_music_vol_table[16] = {
    0,   //-99   0   0
    2,   //-58   8   1
    5,   //-50   7   2
    11,  //-43   5   3
    20,  //-38   5   4
    36,  //-33   4   5
    58,  //-29   4   6
    92,  //-25   4   7
    147, //-21   4   8
    230, //-17   3   9
    328, //-14   2   10
    410, //-12   2   11
    512, //-10   2   12
    649, //-8    2   13
    812, //-6    2   14
    1024 //-4    0   15
};
int16_t g_lea_music_vol = 328;

/**
 * @brief       Control music volume.
 * @param[in]   p       - pointer to music data buffer.
 * @param[in]   sample  - number of samples in the buffer.
 * @return      none.
 */
static void lea_codec_control_volume(int16_t *p, uint16_t sample)
{
    uint8_t vol     = lea_codec_get_output_volume();
    vol             = vol >> 4; //vol = vol / 16;
    g_lea_music_vol = lea_music_vol_table[vol];

    static int16_t s_lea_music_vol_inner = 0;
    for (int i = 0; i < sample; i++) {
        int tmp = p[i] * s_lea_music_vol_inner;
        tmp /= 1024;
        p[i] = tmp;

        if (s_lea_music_vol_inner < g_lea_music_vol) {
            s_lea_music_vol_inner++;
        } else if (s_lea_music_vol_inner > g_lea_music_vol) {
            s_lea_music_vol_inner--;
        }
    }
}

/*********  volume control code end  *********/

/**
 * @brief       Set output audio data.
 * @param[in]   left_data    - pointer to left channel audio data.
 * @param[in]   right_data   - pointer to right channel audio data.
 * @param[in]   sample_num   - number of samples per channel.
 * @return      none.
 */
void lea_codec_output_set_audio_data(int16_t *left_data, int16_t *right_data, uint16_t sample_num)
{
    int16_t pcm_stereo[sample_num * 2];
    for (size_t i = 0; i < sample_num; i++) {
        pcm_stereo[2 * i]     = left_data[i];
        pcm_stereo[2 * i + 1] = right_data[i];
    }
    lea_codec_control_volume(pcm_stereo, sample_num * 2);

    uint16_t sample_num_48k = 480;
    int16_t  codec_pcm_stereo[sample_num_48k * 2]; /** < support max count. */

    int16_t *p_pcm_stereo = pcm_stereo;
    if (s_codec_output_asrc_type != TLKA_ASRC_FLAG_END) {
        tlka_asrc_16_bit_process_frame(s_codec_output_asrc_buffer, pcm_stereo, sample_num, codec_pcm_stereo);
        p_pcm_stereo = codec_pcm_stereo;
    }

    if (tone_is_playing()) {
        tone_get_sample(p_pcm_stereo, sample_num_48k * sizeof(tone_int), 48000);
    }
    if (!sink_sync_flag) {
        sink_sync_flag = true;
//tlkdrv_codec_sync_play_samples(48);
#if TLKADU_MIDBUF_ENABLE
        tlkaud_codec_task_init();
        tlkaud_codec_task_set_mode(PLAYER_MODE_LEA);

        tlkaud_set_audio_mode(AUDIO_LEA_SPK);
        // #if AUDIO_PATH_24BITS_EN
        //         tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, 48000, 0);
        // #else
        //         tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_SPK, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 48000, 0);
        // #endif
#if !TLKAUD_LEA_PLAY_FLOW_CTRL_EN
        extern void tlkmdi_midbuf_sync_spk(uint16_t sample);
        tlkmdi_midbuf_sync_spk(48 * 2);
#endif
#endif
    }
    tlk_lea_codec_write_output_value((uint8_t *)p_pcm_stereo, sample_num * s_codec_output_count);
}

#endif

#if (LE_AUDIO_CODEC_INPUT_TYPE == LE_AUDIO_CODEC_MIDDLE_BUF && LE_AUDIO_CODEC_OUTPUT_TYPE == LE_AUDIO_CODEC_MIDDLE_BUF)
/**
 * @brief       Initialize both input and output stream.
 * @return      none.
 */
void lea_codec_in_output_stream_init(void)
{
    lea_codec_output_init_asrc_module();
    lea_codec_input_init_asrc_module();

    tlkaud_set_audio_mode(AUDIO_LEA_MIC);
    // tlkaud_set_audio_mode(AUDIO_LEA_SPK);
    // configure codec always using 48kHz sample rate.
    // #if AUDIO_PATH_24BITS_EN
    //     tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, 48000, 0);
    // #else
    //     tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_BOTH, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_16, 48000, 0);
    // #endif
}

/**
 * @brief       Deinitialize both input and output stream.
 * @return      none.
 */
void lea_codec_in_output_stream_deinit(void)
{
    lea_codec_output_deinit_asrc_module();
    lea_codec_input_deinit_asrc_module();
    // tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_BOTH);
    tlkaud_clear_audio_mode(AUDIO_LEA_MIC);
    tlkaud_clear_audio_mode(AUDIO_LEA_SPK);
    sink_sync_flag = false;
}
#endif


#if LE_AUDIO_CODEC_OUTPUT_TYPE == LE_AUDIO_CODEC_MIDDLE_BUF

uint32_t g_codec_rate;

/**
 * @brief       Continue tone playback.
 * @return      none.
 */
void lea_us_tone_continue(void)
{
    uint16_t sample_num = 480;

    int16_t pData[sample_num * 2];
    memset(pData, 0, sample_num * 2 * sizeof(int16_t));
    if (tone_is_playing()) {
        tone_get_sample((int16_t *)pData, sample_num * sizeof(tone_int), 48000);
    }

    tlkdrv_codec_fillSpkBuff((uint8_t *)pData, sample_num * sizeof(int16_t) * 2);

    if (tone_is_playing()) {
        tlkmdi_audio_task_set_next_irq(10000);
    }
}

#endif

#endif //#if (TLK_MW_LE_AUDIO_ENABLE)
