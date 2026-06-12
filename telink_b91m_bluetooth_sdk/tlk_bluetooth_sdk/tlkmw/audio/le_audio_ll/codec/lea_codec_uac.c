/********************************************************************************************************
 * @file    lea_codec_uac.c
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

#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"
#include "tlkalg/audio/mixer/tlkalg_2chmix.h"

#include "lea_codec.h"
#if TLK_MW_LEA_BMS_ENABLE
#if ((TLK_USB_UAC_ENABLE || TLK_USB_UAC_HS_ENABLE) && TLK_MW_LE_AUDIO_LOW_LATENCY_ENABLE)

#if (LE_AUDIO_CODEC_INPUT_TYPE == LE_AUDIO_CODEC_TYPE_USB_AUDIO)

/* The cache buffer of UAC state transit idle to streaming, must had cache samples. */
#define LE_UAC_INPUT_GET_CACHE_SAMPLE 5 * 48 // 5ms * 48kHz

#define USB_SAMPLE_DATA_SIZE          sizeof(int32_t)

#define USB_AUDIO_IN_USE_PPM          0

#if !USB_AUDIO_IN_USE_PPM
static u32 read_usb_last_time = 0;
#endif

enum
{
    LEA_UAC_STATE_IDLE = 0,
    LEA_UAC_STATE_STREAMING,
};

static uint8_t s_lea_uac_state = LEA_UAC_STATE_IDLE;
extern uint8_t gLeaPpmStatus;

#if TLK_USB_UAC_ENABLE
extern uint32_t ppm_tick_last;
extern uint8_t  g_ppm_in_flag;
extern uint32_t mic_ppm_tick_last;
#endif

/**
 * @brief       Clean input buffer.
 * @return      none.
 */
void lea_codec_input_clean_buffer(void)
{
    tlkusb_uac_clear_iso_out_buffer();
}


#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
static uint8_t *s_alg_voice_48to32_buff     = NULL;
static uint32_t lea_codec_input_sample_rate = 0;
#else
static uint8_t *s_alg_voice_48to16_buff     = NULL;
static uint32_t lea_codec_input_sample_rate = 0;
#endif

void lea_codec_input_stream_init(void)
{
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    lea_codec_input_sample_rate = lea_codec_get_input_sampling_rate();
#else
    lea_codec_input_sample_rate = lea_codec_get_input_sampling_rate();
#endif
    s_lea_uac_state = LEA_UAC_STATE_IDLE;
    tlkusb_uac_set_state(TLKUSB_UAC_MUSIC);
#if !USB_AUDIO_IN_USE_PPM
    read_usb_last_time = clock_time();
#endif
#if 0
#if TLK_ALG_PPM_ENABLE
#if TLKALG_PPM_SPK_ENABLE
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
    uint16_t ppm_spk_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
    uint8_t *spTlkMdiU2hMusicPpmBuff = (uint8_t *) tlkalg_malloc_func(ppm_spk_size);
    p_audio_alg_if->audio_alg_init(spTlkMdiU2hMusicPpmBuff, ALG_CHANNEL_STEREO);
#endif
    extern void tlkalg_u2s_ppm_clear_status(void);
    extern void tlkalg_u2s_ppm_set_val(int16_t val);
    tlkalg_u2s_ppm_clear_status();
    tlkalg_u2s_ppm_set_val(0);
#endif
#endif
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    audio_alg_interface_t *p_audio_alg_if   = audio_alg_get_interface_by_type(ALG_ASRC_48TO32_16BIT);
    uint16_t               asrc_48to16_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
    if (s_alg_voice_48to32_buff != NULL) {
        p_audio_alg_if->audio_alg_deinit();
        tlkalg_free_func(s_alg_voice_48to32_buff);
    }
    s_alg_voice_48to32_buff = (uint8_t *)tlkalg_malloc_func(asrc_48to16_size);
    p_audio_alg_if->audio_alg_init(s_alg_voice_48to32_buff, ALG_CHANNEL_STEREO);
#else
    // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
    if (!p_audio_alg_if) {
        assert(0);
    }
    uint16_t asrc_48to16_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
    if (s_alg_voice_48to16_buff != NULL) {
        p_audio_alg_if->audio_alg_deinit();
        tlkalg_free_func(s_alg_voice_48to16_buff);
    }
    s_alg_voice_48to16_buff = (uint8_t *)tlkalg_malloc_func(asrc_48to16_size);
    p_audio_alg_if->audio_alg_init(s_alg_voice_48to16_buff, ALG_CHANNEL_STEREO);
#endif
}

void lea_codec_input_stream_deinit(void)
{
#if !USB_AUDIO_IN_USE_PPM
    read_usb_last_time = 0;
#endif
    tlkusb_uac_set_state(TLKUSB_UAC_IDLE);
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    if (s_alg_voice_48to32_buff != NULL) {
        // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO32_16BIT);
        tlkalg_free_func(s_alg_voice_48to32_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_voice_48to32_buff = NULL;
    }
#else
    if (s_alg_voice_48to16_buff != NULL) {
        // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
        tlkalg_free_func(s_alg_voice_48to16_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_voice_48to16_buff = NULL;
    }
#endif
}

bool lea_codec_input_get_audio_data(int16_t *left_data, int16_t *right_data, uint16_t sample_num)
{
#if !USB_AUDIO_IN_USE_PPM
    if (read_usb_last_time && clock_time_exceed(read_usb_last_time, 10000000)) {
        tlkusb_uacspk_close();
        read_usb_last_time = 0;
        return false;
    }
#endif

    uint16_t sample_num_tmp = sample_num;

#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    if (lea_codec_input_sample_rate == 32000) {
        sample_num *= 1.5;
    } else if (lea_codec_input_sample_rate == 48000) {
    } else {
        return false; //not allow sample rate
    }

    int16_t pcm_stereo_32K[sample_num * 2 / 3 * 2];
#else
    if (lea_codec_input_sample_rate == 16000) {
        sample_num *= 3;
    } else if (lea_codec_input_sample_rate == 48000) {
    } else {
        return false; //not allow sample rate
    }
    int16_t pcm_stereo_16K[sample_num * 2 / 3];
#endif
    bool    ret = false;
    int16_t usb_uac_data[sample_num * 2];
    if (s_lea_uac_state == LEA_UAC_STATE_STREAMING) {
        if (tlkusb_uac_read_iso_out_samples(usb_uac_data, sample_num * USB_SAMPLE_DATA_SIZE / 2) != 0) {
            ret = true;
            //  tlkapi_trace(0xFFFFFFFF, "[QQQQ_spk]", "samples,:%d, wptr:%d, rptr:%d", tlkusb_uac_get_iso_out_used_samples(), g_tlk_usb_cfg.out_w, g_tlk_usb_cfg.out_r);
            // tlkapi_printf(1,"WQ  gLeaPpmStatus = %d",gLeaPpmStatus);
        } else {
            s_lea_uac_state = LEA_UAC_STATE_IDLE;
            lea_codec_input_clean_buffer();
            // gpio_toggle(GPIO_CHN10);
        }
    } else if (s_lea_uac_state == LEA_UAC_STATE_IDLE) {
        if (tlkusb_uac_get_iso_out_used_samples() > (sample_num + LE_UAC_INPUT_GET_CACHE_SAMPLE) * USB_SAMPLE_DATA_SIZE / 2) {
            tlkusb_uac_sync_iso_out_samples(APP_USB_ISO_OUT_BUFF_SIZE / 2);
#if (TLK_USB_UAC_ENABLE)
            ppm_tick_last = 0;
#endif
            tlkusb_uac_read_iso_out_samples(usb_uac_data, sample_num * USB_SAMPLE_DATA_SIZE / 2);
            ret             = true;
            s_lea_uac_state = LEA_UAC_STATE_STREAMING;
        }
    }
    int16_t *pcm_stream = usb_uac_data;
    if (ret == true) {
        // gpio_toggle(GPIO_CHN9);
#if !USB_AUDIO_IN_USE_PPM
        read_usb_last_time = clock_time();
#endif
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
        if (lea_codec_input_sample_rate == 32000) {
            // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO32_16BIT);
            p_audio_alg_if->audio_alg_process((uint8_t *)usb_uac_data, (uint8_t *)pcm_stereo_32K, sample_num, 0, 0);
            pcm_stream = pcm_stereo_32K;
        } else {
        }
#else
        if (lea_codec_input_sample_rate == 16000) { //refer LEA_SELECT_SAMPLING_FREQ_16000_HZ  = 3,  // 16000 Hz
            // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
            p_audio_alg_if->audio_alg_process((uint8_t *)usb_uac_data, (uint8_t *)pcm_stereo_16K, sample_num, 0, 0);
            pcm_stream = pcm_stereo_16K;
        } else {
        }
#endif

        for (int i = 0; i < sample_num_tmp; i++) {
            left_data[i]  = pcm_stream[2 * i];
            right_data[i] = pcm_stream[2 * i + 1];
        }
    }

    return ret;
}


#endif

#if (LE_AUDIO_CODEC_OUTPUT_TYPE == LE_AUDIO_CODEC_TYPE_USB_AUDIO)

#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
static uint8_t *s_alg_voice_asrc_buff = NULL;
static uint8_t  s_alg_asrc_type       = 0;
#else
static uint8_t *s_alg_voice_asrc_buff = NULL;
#endif

void lea_codec_output_stream_init(void)
{
    tlkusb_uac_set_state(TLKUSB_UAC_VOICE);
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    if (48000 == tlkusb_uac_get_iso_in_SampleRate()) {
        s_alg_asrc_type = ALG_ASRC_32TO48_16BIT;
    } else {
        s_alg_asrc_type = ALG_ASRC_32TO16_16BIT;
    }
    // audio_alg_interface_t *p_audio_alg_if   = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
    audio_alg_interface_t *p_audio_alg_if    = audio_alg_get_interface_by_type(s_alg_asrc_type);
    uint16_t               s_voice_asrc_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
    s_alg_voice_asrc_buff                    = (uint8_t *)tlkalg_malloc_func(s_voice_asrc_size);
    p_audio_alg_if->audio_alg_init(s_alg_voice_asrc_buff, ALG_CHANNEL_LEFT);
#else
    // audio_alg_interface_t *p_audio_alg_if   = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
    audio_alg_interface_t *p_audio_alg_if   = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
    uint16_t               asrc_16to48_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
    s_alg_voice_asrc_buff                   = (uint8_t *)tlkalg_malloc_func(asrc_16to48_size);
    p_audio_alg_if->audio_alg_init(s_alg_voice_asrc_buff, ALG_CHANNEL_LEFT);
#endif
}

void lea_codec_output_stream_deinit(void)
{
    tlkusb_uac_set_state(TLKUSB_UAC_IDLE);
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(s_alg_asrc_type);
    tlkalg_free_func(s_alg_voice_asrc_buff);
    p_audio_alg_if->audio_alg_deinit();
    s_alg_voice_asrc_buff = NULL;
#else
    // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
    tlkalg_free_func(s_alg_voice_asrc_buff);
    p_audio_alg_if->audio_alg_deinit();
    s_alg_voice_asrc_buff = NULL;
#endif
}

void lea_codec_output_set_audio_data(int16_t *left_data, int16_t *right_data, uint16_t sample_num)
{
    int16_t combine_data[sample_num];

    tlkalg_2chnmix(left_data, right_data, combine_data, 1, sample_num);

    // tlkapi_printf(1,"WQ  gLeaPpmStatus = %d",gLeaPpmStatus);
#if TLK_USB_UAC_ENABLE
    if (gLeaPpmStatus) {
        if (g_ppm_in_flag == 0) {
            g_ppm_in_flag     = 1;
            mic_ppm_tick_last = 0;
            tlkusb_uac_sync_ppm_in_samples(APP_USB_PPM_IN_BUFF_SIZE / 2);
        }
    }

#endif
    //  tlkapi_trace(0xFFFFFFFF, "[QQQQ_mic]", "samples:%d, wptr:%d, rptr:%d", tlkusb_uac_get_ppm_in_idle_samples(), g_tlk_usb_cfg.ppm_in_w, g_tlk_usb_cfg.ppm_in_r);
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    int16_t                pcm_mono[sample_num * 3 / 2];
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(s_alg_asrc_type);
    p_audio_alg_if->audio_alg_process((uint8_t *)combine_data, (uint8_t *)pcm_mono, sample_num, 0, 0);
    uint32_t pcm_mono_size = sample_num * 3 / 2;
    if (16000 == tlkusb_uac_get_iso_in_SampleRate()) {
        pcm_mono_size = sample_num / 2;
    }
    if (tlkusb_uac_write_in_samples(pcm_mono, pcm_mono_size) == 0) {}
#else
#if TLKALG_ASRC_16TO48_16BIT_ENABLE
    //upresample to 48k
    if (48000 == tlkusb_uac_get_iso_in_SampleRate()) {
        int16_t pcm_mono_48k[sample_num * 3];
        // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
        p_audio_alg_if->audio_alg_process((uint8_t *)combine_data, (uint8_t *)pcm_mono_48k, sample_num, 0, 0);
        if (tlkusb_uac_write_in_samples(pcm_mono_48k, sample_num * 3) == 0) {}
    } else {
        if (tlkusb_uac_write_in_samples(combine_data, sample_num) == 0) {}
    }
#else
    tlkusb_uac_write_in_samples(left_data, sample_num * sizeof(int16_t) / 2);
#endif
#endif
}
#endif

#if (LE_AUDIO_CODEC_INPUT_TYPE == LE_AUDIO_CODEC_TYPE_USB_AUDIO) || (LE_AUDIO_CODEC_OUTPUT_TYPE == LE_AUDIO_CODEC_TYPE_USB_AUDIO)
void lea_codec_in_output_stream_init(void)
{
    lea_codec_input_stream_init();
    lea_codec_output_stream_init();
}

void lea_codec_in_output_stream_deinit(void)
{
    lea_codec_input_stream_deinit();
    lea_codec_output_stream_deinit();
}

#endif

#endif //#if (TLK_MW_LE_AUDIO_LOW_LATENCY_ENABLE)
#else

#if ((TLK_USB_UAC_ENABLE || TLK_USB_UAC_HS_ENABLE) && TLK_MW_LE_AUDIO_LOW_LATENCY_ENABLE)

#if (LE_AUDIO_CODEC_INPUT_TYPE == LE_AUDIO_CODEC_TYPE_USB_AUDIO)

/* The cache buffer of UAC state transit idle to streaming, must had cache samples. */
#define LE_UAC_INPUT_GET_CACHE_SAMPLE 5 * 48 // 5ms * 48kHz

#define USB_SAMPLE_DATA_SIZE          sizeof(int32_t)

#define USB_AUDIO_IN_USE_PPM          0

#if !USB_AUDIO_IN_USE_PPM
static u32 read_usb_last_time = 0;
#endif

enum
{
    LEA_UAC_STATE_IDLE = 0,
    LEA_UAC_STATE_STREAMING,
};

static uint8_t s_lea_uac_state = LEA_UAC_STATE_IDLE;
#if (TLK_USB_UAC_ENABLE)
extern uint8_t gLeaPpmStatus;


extern uint32_t ppm_tick_last;
extern uint8_t  g_ppm_in_flag;
extern uint32_t mic_ppm_tick_last;

#endif

/**
 * @brief       Clean input buffer.
 * @return      none.
 */
void lea_codec_input_clean_buffer(void)
{
    tlkusb_uac_clear_iso_out_buffer();
}


#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
static uint8_t *s_alg_voice_48to32_buff     = NULL;
static uint32_t lea_codec_input_sample_rate = 0;
#else
static uint8_t *s_alg_voice_48to16_buff     = NULL;
static uint32_t lea_codec_input_sample_rate = 0;
#endif

void lea_codec_input_stream_init(void)
{
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    lea_codec_input_sample_rate = lea_codec_get_input_sampling_rate();
#else
    lea_codec_input_sample_rate = lea_codec_get_input_sampling_rate();
#endif
    s_lea_uac_state = LEA_UAC_STATE_IDLE;
    tlkusb_uac_set_state(TLKUSB_UAC_MUSIC);
#if !USB_AUDIO_IN_USE_PPM
    read_usb_last_time = clock_time();
#endif
#if 0
#if TLK_ALG_PPM_ENABLE
#if TLKALG_PPM_SPK_ENABLE
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
    uint16_t ppm_spk_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
    uint8_t *spTlkMdiU2hMusicPpmBuff = (uint8_t *) tlkalg_malloc_func(ppm_spk_size);
    p_audio_alg_if->audio_alg_init(spTlkMdiU2hMusicPpmBuff, ALG_CHANNEL_STEREO);
#endif
    extern void tlkalg_u2s_ppm_clear_status(void);
    extern void tlkalg_u2s_ppm_set_val(int16_t val);
    tlkalg_u2s_ppm_clear_status();
    tlkalg_u2s_ppm_set_val(0);
#endif
#endif
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    audio_alg_interface_t *p_audio_alg_if   = audio_alg_get_interface_by_type(ALG_ASRC_48TO32_16BIT);
    uint16_t               asrc_48to32_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
    if (s_alg_voice_48to32_buff != NULL) {
        p_audio_alg_if->audio_alg_deinit();
        tlkalg_free_func(s_alg_voice_48to32_buff);
    }
    s_alg_voice_48to32_buff = (uint8_t *)tlkalg_malloc_func(asrc_48to32_size);
    p_audio_alg_if->audio_alg_init(s_alg_voice_48to32_buff, ALG_CHANNEL_STEREO);
#else
    // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
    if (!p_audio_alg_if) {
        assert(0);
    }
    uint16_t asrc_48to16_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
    if (s_alg_voice_48to16_buff != NULL) {
        p_audio_alg_if->audio_alg_deinit();
        tlkalg_free_func(s_alg_voice_48to16_buff);
    }
    s_alg_voice_48to16_buff = (uint8_t *)tlkalg_malloc_func(asrc_48to16_size);
    p_audio_alg_if->audio_alg_init(s_alg_voice_48to16_buff, ALG_CHANNEL_STEREO);
#endif
}

void lea_codec_input_stream_deinit(void)
{
#if !USB_AUDIO_IN_USE_PPM
    read_usb_last_time = 0;
#endif
    tlkusb_uac_set_state(TLKUSB_UAC_IDLE);
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    if (s_alg_voice_48to32_buff != NULL) {
        // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO32_16BIT);
        tlkalg_free_func(s_alg_voice_48to32_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_voice_48to32_buff = NULL;
    }
#else
    if (s_alg_voice_48to16_buff != NULL) {
        // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
        tlkalg_free_func(s_alg_voice_48to16_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_voice_48to16_buff = NULL;
    }
#endif
}

bool lea_codec_input_get_audio_data(int16_t *left_data, int16_t *right_data, uint16_t sample_num)
{
#if !USB_AUDIO_IN_USE_PPM
    if (read_usb_last_time && clock_time_exceed(read_usb_last_time, 10000000)) {
        tlkusb_uacspk_close();
        read_usb_last_time = 0;
        return false;
    }
#endif

    uint16_t sample_num_tmp = sample_num;

#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    if (lea_codec_input_sample_rate == 32000) {
        sample_num *= 1.5;
    } else if (lea_codec_input_sample_rate == 48000) {
    } else {
        return false; //not allow sample rate
    }

    int16_t pcm_stereo_32K[sample_num * 2 / 3 * 2];
#else
    if (lea_codec_input_sample_rate == 16000) {
        sample_num *= 3;
    } else if (lea_codec_input_sample_rate == 48000) {
    } else {
        return false; //not allow sample rate
    }
    int16_t pcm_stereo_16K[sample_num * 2 / 3];
#endif
    bool    ret = false;
    int16_t usb_uac_data[sample_num * 2];
    if (s_lea_uac_state == LEA_UAC_STATE_STREAMING) {
        if (tlkusb_uac_read_iso_out_samples(usb_uac_data, sample_num * USB_SAMPLE_DATA_SIZE / 2) != 0) {
            ret = true;
            //  tlkapi_trace(0xFFFFFFFF, "[QQQQ_spk]", "samples,:%d, wptr:%d, rptr:%d", tlkusb_uac_get_iso_out_used_samples(), g_tlk_usb_cfg.out_w, g_tlk_usb_cfg.out_r);
            // tlkapi_printf(1,"WQ  gLeaPpmStatus = %d",gLeaPpmStatus);
        } else {
            s_lea_uac_state = LEA_UAC_STATE_IDLE;
            lea_codec_input_clean_buffer();
            // gpio_toggle(GPIO_CHN10);
        }
    } else if (s_lea_uac_state == LEA_UAC_STATE_IDLE) {
        if (tlkusb_uac_get_iso_out_used_samples() > (sample_num + LE_UAC_INPUT_GET_CACHE_SAMPLE) * USB_SAMPLE_DATA_SIZE / 2) {
            tlkusb_uac_sync_iso_out_samples(APP_USB_ISO_OUT_BUFF_SIZE / 2);
#if (TLK_USB_UAC_ENABLE)
            ppm_tick_last = 0;
#endif
            tlkusb_uac_read_iso_out_samples(usb_uac_data, sample_num * USB_SAMPLE_DATA_SIZE / 2);
            ret             = true;
            s_lea_uac_state = LEA_UAC_STATE_STREAMING;
        }
    }
    int16_t *pcm_stream = usb_uac_data;
    if (ret == true) {
        // gpio_toggle(GPIO_CHN9);
#if !USB_AUDIO_IN_USE_PPM
        read_usb_last_time = clock_time();
#endif
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
        if (lea_codec_input_sample_rate == 32000) {
            // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO32_16BIT);
            p_audio_alg_if->audio_alg_process((uint8_t *)usb_uac_data, (uint8_t *)pcm_stereo_32K, sample_num, 0, 0);
            pcm_stream = pcm_stereo_32K;
        } else {
        }
#else
        if (lea_codec_input_sample_rate == 16000) { //refer LEA_SELECT_SAMPLING_FREQ_16000_HZ  = 3,  // 16000 Hz
            // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
            p_audio_alg_if->audio_alg_process((uint8_t *)usb_uac_data, (uint8_t *)pcm_stereo_16K, sample_num, 0, 0);
            pcm_stream = pcm_stereo_16K;
        } else {
        }
#endif

        for (int i = 0; i < sample_num_tmp; i++) {
            left_data[i]  = pcm_stream[2 * i];
            right_data[i] = pcm_stream[2 * i + 1];
        }
    }

    return ret;
}


#endif

#if (LE_AUDIO_CODEC_OUTPUT_TYPE == LE_AUDIO_CODEC_TYPE_USB_AUDIO)

#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
static uint8_t *s_alg_voice_asrc_buff = NULL;
static uint8_t  s_alg_asrc_type       = 0;
#else
static uint8_t *s_alg_voice_asrc_buff = NULL;
#endif

/**
 * @brief       Initialize output stream.
 * @return      none.
 */
void lea_codec_output_stream_init(void)
{
    tlkusb_uac_set_state(TLKUSB_UAC_VOICE);
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    if (32000 == lea_codec_get_output_sampling_rate()) {
        if (48000 == tlkusb_uac_get_iso_in_SampleRate()) {
            s_alg_asrc_type = ALG_ASRC_32TO48_16BIT;
        } else {
            s_alg_asrc_type = ALG_ASRC_32TO16_16BIT;
        }
    } else if (16000 == lea_codec_get_output_sampling_rate()) {
        if (48000 == tlkusb_uac_get_iso_in_SampleRate()) {
            s_alg_asrc_type = ALG_ASRC_16TO48_16BIT;
        } else {
            s_alg_asrc_type = 0xFF;
        }
    }
    if (s_alg_asrc_type != 0xFF) {
        audio_alg_interface_t *p_audio_alg_if    = audio_alg_get_interface_by_type(s_alg_asrc_type);
        uint16_t               s_voice_asrc_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
        s_alg_voice_asrc_buff                    = (uint8_t *)tlkalg_malloc_func(s_voice_asrc_size);
        p_audio_alg_if->audio_alg_init(s_alg_voice_asrc_buff, ALG_CHANNEL_LEFT);
    }
#else
    // audio_alg_interface_t *p_audio_alg_if   = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
    audio_alg_interface_t *p_audio_alg_if   = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
    uint16_t               asrc_16to48_size = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_LEFT);
    s_alg_voice_asrc_buff                   = (uint8_t *)tlkalg_malloc_func(asrc_16to48_size);
    p_audio_alg_if->audio_alg_init(s_alg_voice_asrc_buff, ALG_CHANNEL_LEFT);
#endif
}

/**
 * @brief       Deinitialize output stream.
 * @return      none.
 */
void lea_codec_output_stream_deinit(void)
{
    tlkusb_uac_set_state(TLKUSB_UAC_IDLE);
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    if (s_alg_asrc_type != 0xFF) {
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(s_alg_asrc_type);
        tlkalg_free_func(s_alg_voice_asrc_buff);
        p_audio_alg_if->audio_alg_deinit();
        s_alg_voice_asrc_buff = NULL;
    }
#else
    // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
    tlkalg_free_func(s_alg_voice_asrc_buff);
    p_audio_alg_if->audio_alg_deinit();
    s_alg_voice_asrc_buff = NULL;
#endif
}

void lea_codec_output_set_audio_data(int16_t *left_data, int16_t *right_data, uint16_t sample_num)
{
    int16_t combine_data[sample_num];

    tlkalg_2chnmix(left_data, right_data, combine_data, 1, sample_num);

    // tlkapi_printf(1,"WQ  gLeaPpmStatus = %d",gLeaPpmStatus);
#if TLK_USB_UAC_ENABLE
    if (gLeaPpmStatus) {
        if (g_ppm_in_flag == 0) {
            g_ppm_in_flag     = 1;
            mic_ppm_tick_last = 0;
            tlkusb_uac_sync_ppm_in_samples(APP_USB_PPM_IN_BUFF_SIZE / 2);
        }
    }

#endif
    //  tlkapi_trace(0xFFFFFFFF, "[QQQQ_mic]", "samples:%d, wptr:%d, rptr:%d", tlkusb_uac_get_ppm_in_idle_samples(), g_tlk_usb_cfg.ppm_in_w, g_tlk_usb_cfg.ppm_in_r);
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
    if (32000 == lea_codec_get_output_sampling_rate()) {
        int16_t                pcm_mono[sample_num * 3 / 2];
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(s_alg_asrc_type);
        p_audio_alg_if->audio_alg_process((uint8_t *)combine_data, (uint8_t *)pcm_mono, sample_num, 0, 0);
        uint32_t pcm_mono_size = sample_num * 3 / 2;
        if (16000 == tlkusb_uac_get_iso_in_SampleRate()) {
            pcm_mono_size = sample_num / 2;
        }
        if (tlkusb_uac_write_in_samples(pcm_mono, pcm_mono_size) == 0) {}
    } else if (16000 == lea_codec_get_output_sampling_rate()) {
        if (48000 == tlkusb_uac_get_iso_in_SampleRate()) {
            int16_t                pcm_mono_48k[sample_num * 3];
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(s_alg_asrc_type);
            p_audio_alg_if->audio_alg_process((uint8_t *)combine_data, (uint8_t *)pcm_mono_48k, sample_num, 0, 0);
            if (tlkusb_uac_write_in_samples(pcm_mono_48k, sample_num * 3) == 0) {}
        } else {
            if (tlkusb_uac_write_in_samples(combine_data, sample_num) == 0) {}
        }
    }
#else
#if TLKALG_ASRC_16TO48_16BIT_ENABLE
    //upresample to 48k
    if (48000 == tlkusb_uac_get_iso_in_SampleRate()) {
        int16_t pcm_mono_48k[sample_num * 3];
        // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48);
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
        p_audio_alg_if->audio_alg_process((uint8_t *)combine_data, (uint8_t *)pcm_mono_48k, sample_num, 0, 0);
        if (tlkusb_uac_write_in_samples(pcm_mono_48k, sample_num * 3) == 0) {}
    } else {
        if (tlkusb_uac_write_in_samples(combine_data, sample_num) == 0) {}
    }
#else
    tlkusb_uac_write_in_samples(left_data, sample_num * sizeof(int16_t) / 2);
#endif
#endif
}
#endif

#if (LE_AUDIO_CODEC_INPUT_TYPE == LE_AUDIO_CODEC_TYPE_USB_AUDIO) || (LE_AUDIO_CODEC_OUTPUT_TYPE == LE_AUDIO_CODEC_TYPE_USB_AUDIO)
/**
 * @brief       Initialize both input and output stream.
 * @return      none.
 */
void lea_codec_in_output_stream_init(void)
{
    lea_codec_input_stream_init();
    lea_codec_output_stream_init();
}

/**
 * @brief       Deinitialize both input and output stream.
 * @return      none.
 */
void lea_codec_in_output_stream_deinit(void)
{
    lea_codec_input_stream_deinit();
    lea_codec_output_stream_deinit();
}

#endif

#endif //#if (TLK_MW_LE_AUDIO_LOW_LATENCY_ENABLE)

#endif
