/********************************************************************************************************
 * @file    ll_dongle_main.c
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

#if (MCU_CORE_TYPE == MCU_CORE_TL721X && PROJ_TPSLL_AUDIO_DONGLE)
#define DUMP_APP_MSG1 1
#include "tlkmw/audio/audio_mw_manager.h"
#include "tlkmw/sys_dev/codec/tlkdrv_codec.h"
#include "drivers.h"
#include "stack/tpsll/tpd/tpd_host_interface.h"
//#include "stack/tpsll/tpd/tpd_mem.h"
//#include "stack/tpsll/tpd/tpd_acl.h"

#include "ll_dongle_main.h"
#include "tlklib/usb/uac/tlkusb_uac.h"

// decode lc3 from headset
#define TUS_MIC_ENC (3800)

// encode lc3 from local mic
#define TUS_SPK_DEC                         (1000)
#define TUS_MIC_ENC_OFFSET                  (1500)

#define ULTRA_LL_TUS_MIC_ENC                (1300)
#define ULTRA_LL_TUS_SPK_DEC                (1100)
#define ULTRA_LL_TUS_MIC_ENC_OFFSET         (0)

#define AUDIO_DBG_DATA_SLAVE_SAME_AS_MASTER 0
#define AUDIO_DBG_USE_MASTER_ENC_DATA       0
#if AUDIO_DBG_USE_MASTER_ENC_DATA
#define AUDIO_DBG_USE_SLAVE_ENC_DATA 0
#else
#define AUDIO_DBG_USE_SLAVE_ENC_DATA 0
#endif

#define LINEIN_DATA_USE_SIN_48K_DATA       1
#define AUDIO_SIMILATE_ENC_SIN_48K_DATA_EN (0) // todo by mingqian 20240831,

ll_dongle_audio_context_t ll_dongle_audio_ctx;


/*********************************************************************/
/*replace previous variable*/

uint8_t g_usb_mute = 0; /*async_dp_dongle_fill_tx_packet (1--> async.audio_req=0, pt->format=0)*/ /*g_usb_mute*/

uint8_t g_max_vol             = 0; /*async.max_vol*/
uint8_t g_audio_vol_set_state = 0;
//new variables add by junwei
uint8_t g_audio_dsample_pos = 0;
/***********************************************************************/

uint16_t      g_pcm_noise_max = 0;
int           audio_i2s_mode  = 0;
audio_music_t g_audio         = {0};

static bool last_usb_spk_on;

_attribute_ram_code_ void app_audio_check_playback_buffer(int32_t ref_num, int32_t tolerance);

/**
 * @brief     Check whether the voice data is lc3 mode.
 * @param[in] format voice data format.
 * @returns   Whether the voice data is lc3 mode.
 */
__INLINE bool app_mic_voice_data_is_lc3_mode(uint8_t format)
{
    return (AUDIO_FORMAT_LC3A == (AUDIO_FORMAT_MASK & format));
}

/**
 * @brief   Clear dongle spk status.
 * @param   none
 * @returns none.
 */
_attribute_ram_code_ void ll_dongle_clear_status(void)
{
    last_usb_spk_on = false;
}

/**
 * @brief   Processing mic voice data.
 * @param   none
 * @returns none.
 */
#if BITDEPTH_24_ENABLE
_attribute_ram_code_ void ll_dongle_audio_decode_task(void)
{
    if (!tlkmdi_ll_dongle_get_mic_state() || !tpd_dongle_is_connected()) {
        return;
    }

    // gpio_set_high_level(GPIO_PF1);
    // gpio_set_low_level(GPIO_PF1);

    int32_t pcm_mono_16k[APP_MIC_FRAME_SAMPLES];
    int32_t pcm_mono_48k[APP_MIC_PCM_SAMPLES];

    uint8_t *pkt_data = NULL;
    int32_t *pData    = pcm_mono_16k;

    uint16_t data_len = 0;
    int32_t  num_out  = 0;
    if (tpd_host_is_ultra_latency_mode()) {
        data_len = APP_MIC_LC3_ULTRA_LL_DEC_LEN;
        num_out  = APP_MIC_ULTRA_LL_FRAME_SAMPLES;
    } else {
        data_len = APP_MIC_LC3_DEC_LEN;
        num_out  = APP_MIC_FRAME_SAMPLES;
    }
#if TLKALG_PPM_ENABLE
    uint32_t reg_value;
    if (app_usb_ppm_get_init_flag_up()) {
        app_usb_ppm_clear_init_flag_up();

        reg_value   = irq_disable();
        int ppm_val = app_usb_ppm_get_value();
        tlkalg_ppm_set_val_mono(-ppm_val);
        irq_restore(reg_value);
    }
#endif

    // if (tpd_headset_is_cc_headset_connected()) {
    //     pkt_data = tpd_dongle_get_mic_data_ptr();
    // }

    // if (!tpd_headset_is_cc_headset_connected()) {
    // pkt_data = tpd_dongle_get_mic_data_ptr();
    // }
    pkt_data = tpd_dongle_get_mic_data_ptr();

    // if (pkt_data && app_mic_is_normal_voice_mode())
    if (pkt_data) {
        if (app_mic_voice_data_is_lc3_mode(pkt_data[0])) {
            /* LC3 format data decoding */
            // pkt_data[0] is idx
#ifdef SL01_le_audio_dec
            log_task_begin_irq(1, SL01_le_audio_dec);
#endif
            // uint8_t enc_lc3_24bit_data[] = {
            //     0x18, 0x97, 0xaf, 0x81, 0x3d, 0x9f, 0x53, 0x7f, 0x45, 0xcb,
            //     0xeb, 0xb0, 0x5d, 0xc6, 0x27, 0x01, 0x05, 0xda, 0x08, 0x7a,
            //     0x60, 0x18, 0xbe, 0x1d, 0x62, 0x15, 0x49, 0xf0, 0xc6, 0x27
            // };
#if TLKALG_LC3_24BIT_DEC_ENABLE
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_DEC);
#elif TLKALG_LC3_PLUS_DEC_ENABLE
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_DEC);
#endif
            uint8_t *p_audio = pkt_data + 1;
#ifdef DONGLE_AUDIO_PATH_GPIO_DEBUG
            gpio_write(GPIO_PF1, 1);
#endif
            if (p_audio_alg_if->audio_alg_process) {
                p_audio_alg_if->audio_alg_process(p_audio, (uint8_t *)pcm_mono_16k, data_len, ALG_WIDTH_24, ALG_CHANNEL_LEFT);
            }
#ifdef DONGLE_AUDIO_PATH_GPIO_DEBUG
            gpio_write(GPIO_PF1, 0);
#endif
#ifdef SL01_le_audio_dec
            log_task_end_irq(1, SL01_le_audio_dec);
#endif

        } else {
            tmemset(pcm_mono_16k, 0, num_out * sizeof(int32_t));
#ifdef SLEV_le_rx_miss_audio_pkt
            log_event_irq(1, SLEV_le_rx_miss_audio_pkt);
#endif
        }
        pkt_data[0] = 0;

        /* PPM processing of mic data. */
#if TLKALG_PPM_ENABLE
        for (int i = 0; i < num_out; i++) {
            pdes[i] = (psrc[i] >> 8) & 0x0000ffff;
        }

        num_out = tlkalg_ppm_process_mono(pcm_16bit, pcm_16bit, APP_MIC_FRAME_SAMPLES);
        if (num_out != APP_MIC_FRAME_SAMPLES) {
#ifdef SLET_ll_mic_pro
            log_tick(SL_STACK_TPH_ACL_EN, SLET_usb_mic_diff);
#endif
        }
#else
//		num_out = APP_MIC_FRAME_SAMPLES;
#endif

#if TLKALG_ASRC_16TO48_24BIT_ENABLE
        if (tlkusb_uac_get_iso_in_SampleRate() == 48000) {
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_24BIT);
            if (p_audio_alg_if->audio_alg_process) {
                int asrc_num_out = p_audio_alg_if->audio_alg_process((uint8_t *)pcm_mono_16k, (uint8_t *)pcm_mono_48k, num_out, ALG_WIDTH_24, ALG_CHANNEL_LEFT);
                pData            = pcm_mono_48k;
                if (asrc_num_out == num_out * 3) {
                    num_out = asrc_num_out;
                } else {
                    tlkapi_trace(0xFFFFFFFF, "[ALG_ASRC_16TO48_24BIT]", "ALG_ASRC_16TO48_24BIT length error");
                }
#if TLK_I2S_DEBUG_ENABLE
                static uint16_t   i2s_wptr = 0;
                extern signed int AUDIO_BUFF[2048];
                for (int j = 0; j < num_out; j++) {
                    AUDIO_BUFF[i2s_wptr++] = pcm_mono_48k[j];
                    i2s_wptr %= 2048;
                }
#endif
            }
        }
#else
#endif
        // pcm_final = pdes_proc;

#if TLKALG_PPM_ENABLE
        if (PPM_REF_MODE_MIC == app_usb_ppm_get_ref_mode()) {
            reg_value = irq_disable();
            if (APP_AUDIO_SAMPLE_RATE_48K == usb_audio_get_mic_cur_rate()) {
                g_ll_dongle_usb_resample_diff += num_out * 3 - num_out;
            } else {
                g_ll_dongle_usb_resample_diff += num_out - num_out;
            }
            irq_restore(reg_value);
        }
#endif

        //write data to usb buffer
        tlkusb_uac_write_in_samples(pData, num_out);
    }
}
#else
/**
 * @brief   Processing mic voice data.
 * @param   none
 * @returns none.
 */
_attribute_ram_code_ void ll_dongle_audio_decode_task(void)
{
    int16_t  pcm_raw[APP_MIC_PCM_SAMPLES + APP_MIC_PCM_BUFF_PREVENT_OVERFLOW_LEN];
    int16_t  pcm[APP_MIC_PCM_SAMPLES + APP_MIC_PCM_BUFF_PREVENT_OVERFLOW_LEN];
    int16_t *pcm_final;
    uint8_t *pkt_data = NULL;
    int16_t  mic_volume;
    int32_t  num_out = APP_MIC_FRAME_SAMPLES;
    uint32_t reg_value;

    if (tpd_headset_is_cc_headset_connected()) {
        pkt_data = tpd_dongle_get_mic_data_ptr();
    }

    for (int idx = 0; idx <= 1; idx++) {
#if APP_AUDIO_CODEC_EN
        if (0 == idx) {
            app_audio_check_playback_buffer(45, 5);
        }
#endif
        if (!tpd_headset_is_cc_headset_connected()) {
            pkt_data = tpd_dongle_get_mic_data_ptr();
        }
        if (pkt_data && app_mic_is_normal_voice_mode()) {
            /* There is voice mode and the headset is not talking over bluetooth */
#if APP_MIC_VOL_ADJUST_EN
            /* mic volume */
            mic_volume = app_mic_get_volume();
#endif
            if (app_mic_voice_data_is_msbc_mode(pkt_data[0])) {
                /* MSBC format data decoding */
                /* num_dec = */
                tlkalg_msbc_dec(pkt_data, APP_MIC_SBC_DEC_LEN, (uint8_t *)pcm_raw);
            } else {
                tmemset(pcm_raw, 0, APP_MIC_FRAME_SAMPLES * sizeof(int16_t));

#ifdef SLEV_le_rx_miss_audio_pkt
                log_event(SL_APP_AUDIO_EN, SLEV_le_rx_miss_audio_pkt);
#endif
                /*
                tlkapi_sendU32s(DUMP_APP_MSG1,
                                "MIC null",
                                tpd_env_get_fno() << 16,
                                (pkt_data[0] << 8) | idx,
                                g_usb_iso_cfg.num_iso_in,
                                g_usb_iso_cfg.in_r);
                */ // fix me
            }
            pkt_data[0] = 0;

            /* PPM processing of mic data. */
#if TLKALG_PPM_MONO_ENABLE
#else
            num_out = tlk_asrc_data_process_mono(0, pcm_raw, APP_MIC_FRAME_SAMPLES, pcm);
#endif
            if (APP_AUDIO_SAMPLE_RATE_48K == usb_audio_get_mic_cur_rate()) {
                /* USB host needs 48k sample rate data */

                /* Resample mic data from 16k to 48k */
#if !TLKALG_PPM_MONO_ENABLE
                // tlk_asrc_switch_flag(1);
#endif
                num_out = tlk_resample16to48_data((int *)pcm, num_out, (int *)pcm, 0);
            }
            pcm_final = pcm;

            if (PPM_REF_MODE_MIC == app_usb_ppm_get_ref_mode()) {
                reg_value = irq_disable();
                if (APP_AUDIO_SAMPLE_RATE_48K == usb_audio_get_mic_cur_rate()) {
                    g_ll_dongle_usb_resample_diff += APP_MIC_FRAME_SAMPLES * 3 - num_out;
                } else {
                    g_ll_dongle_usb_resample_diff += APP_MIC_FRAME_SAMPLES - num_out;
                }
                irq_restore(reg_value);
            }

            while (num_out--) {
                /* Save data into bi buffer */
#if APP_MIC_VOL_ADJUST_EN
                app_mic_iso_in_put_data(((*pcm_final) * mic_volume) / APP_MIC_VOLUME_MAX);
#else
                app_mic_iso_in_put_data(*pcm_final);
#endif
                pcm_final++;
            }
        }
#if APP_AUDIO_CODEC_EN
        else {
#ifdef SLET_le_audio_null
            log_tick(SL_APP_AUDIO_EN, SLET_le_audio_null);
#endif

            tmemset(pcm, 0, sizeof(pcm));
        }

#if APP_AUDIO_CODEC_MIC_MUSIC_MIX_EN
        /* Mix the mic data and spk data and store them into the playback buffer */
        app_mic_music_mix_playback(pcm, g_audio.pcm + APP_MIC_FRAME_SAMPLES * 3 * idx, APP_MIC_FRAME_SAMPLES * 3);
#elif APP_AUDIO_CODEC_MIC_DATA_ONLY_EN
        app_audio_mono_pcm_to_playback(pcm, APP_MIC_FRAME_SAMPLES * 3);
#elif APP_AUDIO_CODEC_MUSIC_DATA_ONLY_EN
        app_audio_pcm_to_playback(g_audio.pcm + APP_MIC_FRAME_SAMPLES * 3 * idx, APP_MIC_FRAME_SAMPLES * 3);
#endif

#endif
    }
}
#endif

/**
 * @brief   Sync samples in bo the buffer of iso-out.
 * @param   None
 * @returns The theoretical number of new samples since the last iso_out
 *          process.
 */
_attribute_ram_code_ uint32_t app_audio_sync_samples()
{
    uint32_t num_new_samples = 0;
#if (TLK_USB_UAC_ENABLE)
    uint32_t reg_value = irq_disable();

    /* Calculate the theoretical number of new samples since the last iso_out
     * process.
     */
    num_new_samples = APP_AUDIO_1MS_SAMPLES * (clock_time() - g_tlk_usb_cfg.tick_out) / APP_AUDIO_1MS_TICK;
    num_new_samples = (num_new_samples > APP_AUDIO_1MS_SAMPLES) ? APP_AUDIO_1MS_SAMPLES : num_new_samples;
    // g_usb_iso_cfg.out_r =
    //     g_usb_spk_write_position - (APP_AUDIO_SYNC_SAMPLES_REF + APP_AUDIO_SYNC_RESET_SAMPLE - num_new_samples);
    //tlkusb_uac_set_iso_out_rptr((APP_AUDIO_SYNC_SAMPLES_REF + APP_AUDIO_SYNC_RESET_SAMPLE - num_new_samples)*2); // *2: steroe

    // remove 2ms flow
    uint16_t sample = APP_AUDIO_SYNC_SAMPLES_REF + 180 + 48;
    if (tpd_host_is_ultra_latency_mode()) {
        sample /= 2;
    }

    tlkusb_uac_set_iso_out_rptr((sample - num_new_samples) * 2);

#ifdef SL16_usb_bo_sample
    log_b16(SL_APP_AUDIO_EN, SL16_usb_bo_sample, app_usb_iso_out_get_samples_number());
#endif
#ifdef SLET_ll_sync_tick
    log_tick(SL_STACK_TPH_ACL_EN, SLET_ll_sync_tick);
#endif

    irq_restore(reg_value);
#else
    // reg_rst2 &= ~FLD_RST2_AUD;
    // sleep_us(1);
    tlkdrv_codec_sync_mic_samples(APP_AUDIO_SYNC_SAMPLES_REF);
    // reg_rst2 |= FLD_RST2_AUD;
#endif

    return num_new_samples;
}

/**
 * @brief     Audio data samples sync.
 * @param[in] samples Number of reference samples.
 * @returns   The theoretical number of new samples since the last iso_out
 *          process.
 */
_attribute_ram_code_ uint32_t app_audio_sync_samples_check(int32_t samples)
{
    int32_t ndiff;
    int32_t num_new_samples = 0;


#if (TLK_USB_UAC_ENABLE)
#if 0
    uint32_t num_cur_samples = 0;
    // gpio_set_high_level(GPIO_PB0);
    uint32_t reg_value = irq_disable();

    /* Calculate the theoretical number of new samples since the last iso_out
     * process.
     */
    
    num_new_samples = APP_AUDIO_1MS_SAMPLES * (clock_time() - g_usb_iso_cfg.tick_out) / APP_AUDIO_1MS_TICK;
    num_new_samples = (num_new_samples > APP_AUDIO_1MS_SAMPLES) ? APP_AUDIO_1MS_SAMPLES : num_new_samples;

    /* Calculate the theoretical number of current samples */
    num_cur_samples =
        ((g_usb_spk_write_position - g_usb_iso_cfg.out_r) & APP_USB_ISO_OUT_BUFF_IDX_MASK) + num_new_samples;

    /* Calculate the difference between the theoretical current sample number
     * and the synchronous reference number.
     */
    ndiff = num_cur_samples - (APP_AUDIO_SYNC_SAMPLES_REF + APP_AUDIO_SYNC_RESET_SAMPLE);

    if (abs_ram(ndiff) > APP_AUDIO_SYNC_DIFF_TH) {
        /* There is a large gap between the current data samplese and the
         * synchronization reference value, so readjust the reading position
         * of the bo buffer
         * reduce the sample in bo buff to reduce delay
         */
        // tlkapi_sendU32s(DUMP_APP_MSG1, "reset usb sample number", tpd_env_get_fno(), num_cur_samples, num_new_samples, ndiff);
#ifdef SL16_usb_bo_sample
        log_b16(SL_APP_AUDIO_EN,SL16_usb_bo_sample, num_cur_samples);
#endif
#ifdef SLET_ll_resync_bo_buff
	    log_tick(SL_STACK_TPH_ACL_EN, SLET_ll_resync_bo_buff);
#endif
        
        app_usb_ppm_clear_status();
        g_usb_iso_cfg.out_r =
        (g_usb_spk_write_position - (APP_AUDIO_SYNC_SAMPLES_REF + APP_AUDIO_SYNC_RESET_SAMPLE - num_new_samples)) & APP_USB_ISO_OUT_BUFF_IDX_MASK;
#ifdef SL16_usb_bo_sample
        log_b16(SL_APP_AUDIO_EN,SL16_usb_bo_sample, app_usb_iso_out_get_samples_number());
#endif

    } else {
        // tlkapi_sendU32s(DUMP_APP_MSG1, "check usb sample number", tpd_env_get_fno(), num, tick, sync_samples);
    }
    // gpio_set_low_level(GPIO_PB0);
    irq_restore(reg_value);
#endif
#else  /* TLK_USB_UAC_ENABLE */
    int32_t tolerance = 4;

    num_cur_samples = tlkdrv_codec_get_mic_avail_samples();
    if (num_cur_samples < APP_AUDIO_SYNC_SAMPLES_REF - tolerance || num_cur_samples > APP_AUDIO_SYNC_SAMPLES_REF + tolerance) {
        //g_audio_codec_cfg.mic_rptr = (wptr - APP_AUDIO_SYNC_SAMPLES_REF) & TWS_MIC_FIFO_MAX;
        tlkdrv_codec_sync_mic_samples(APP_AUDIO_SYNC_SAMPLES_REF);
        // tlkapi_sendU32s(DUMP_APP_MSG1, "check line in sample number", tpd_env_get_fno(), num_cur_samples, tolerance,
        // APP_AUDIO_SYNC_SAMPLES_REF);
    } else {
        // tlkapi_sendU32s(DUMP_APP_MSG1, "check line in sample number", tpd_env_get_fno(), num_cur_samples, tolerance,
        // APP_AUDIO_SYNC_SAMPLES_REF);
    }
#endif /* TLK_USB_UAC_ENABLE */

    ndiff = g_audio.idx - samples;

    if (ndiff > 2 || ndiff < 0) {
        g_audio.idx = 0;
        tlkapi_sendU32s(DUMP_APP_MSG1, "g_audio.idx reset", g_audio.idx, ndiff, samples, 0);
    } else {
        for (int32_t i = 0; i < ndiff; i++) {
            g_audio.pcm[i] = g_audio.pcm[samples + i];
        }
        g_audio.idx = ndiff;
    }
#ifdef SL16_usb_g_audio_idx
    log_b16(SL_APP_AUDIO_EN, SL16_usb_g_audio_idx, g_audio.idx);
#endif
    // tlkapi_sendU32s(DUMP_APP_MSG1, "g_audio.idx", g_audio.idx, ndiff, samples, 0);

    return num_new_samples;
}

int32_t ll_sin_48k_mono_24bit[] __attribute__((aligned(4))) = {
    0,        1094933,  2171131,  3210181,  4194303,  5106660,  5931641,  6655129,  7264747,  7750062,  8102772,  8316841,  8388607,  8316841,  8102772,  7750062,
    7264746,  6655129,  5931641,  5106660,  4194303,  3210179,  2171130,  1094932,  0,        -1094933, -2171131, -3210181, -4194305, -5106661, -5931642, -6655129,
    -7264747, -7750063, -8102772, -8316841, -8388607, -8316841, -8102772, -7750062, -7264747, -6655129, -5931639, -5106660, -4194301, -3210180, -2171129, -1094933,
};

/**
 * @brief     Generate 24bit sin wave data.
 * @param[in] dest_ptr Pointer of destination.
 * @param[in] num Number of samples.
 * @returns    None.
 */
_attribute_ram_code_ void app_audio_get_24bit_sin_48k_data(int32_t *dest_ptr, int32_t num)
{
    static int sin_count = 0;
    int32_t    pcm_mono_tmp;

    for (uint16_t i = 0; i < num; i++) {
        pcm_mono_tmp        = ll_sin_48k_mono_24bit[sin_count++] / 10;
        dest_ptr[2 * i]     = pcm_mono_tmp;
        dest_ptr[2 * i + 1] = pcm_mono_tmp;
        sin_count %= 48;
    }
}

/**
 * @brief     Detect whether the usb audio is muted.
 * @param[in] src_ptr Pointer of audio data.
 * @param[in] len Length of audio data.
 * @returns   Whether the usb audio is muted.
 */
#if BITDEPTH_24_ENABLE
_attribute_ram_code_ void app_audio_data_mute_detect(const int32_t *src_ptr, int32_t len)
#else
_attribute_ram_code_ void app_audio_data_mute_detect(const int16_t *src_ptr, int32_t len)
#endif
{
    static uint32_t tick_active = 0;

    int32_t active = 0;
    for (int32_t i = 0; i < len; i++) {
        if (src_ptr[i]) {
            active = 1;
            break;
        }
    }

    if (active) {
        tick_active = clock_time() | 1;
        g_usb_mute  = 0;
    } else if (tick_active && clock_time_exceed(tick_active, 1500000)) {
        tick_active = 0;
        g_usb_mute  = 1;
    }
}

/**
 * @brief     Get samples from usb and store them in g_audio.pcm.
 * @param[in] len The number of samples to be stored in g_audio.pcm.
 * @returns   none.
 */
_attribute_ram_code_ void app_audio_get_audio_data(int32_t len)
{
    int32_t num_new = len + 1 - g_audio.idx;
    int32_t num_out;

    uint16_t buff_len = APP_AUDIO_10MS_SAMPLES + APP_AUDIO_PCM_BUFF_PREVENT_OVERFLOW_LEN;

    int32_t pcm_temp[buff_len * 2];
    /* Buffer starting with idx position in g_audio.pcm. */
    codec_int *cur_ptr = (codec_int *)(g_audio.pcm + g_audio.idx);

    if (num_new < 0) {
        tlkapi_sendU32s(DUMP_APP_MSG1, "get_audio_data error", tpd_env_get_fno(), g_audio.idx, len, 0);

        tmemset(g_audio.pcm, 0, len * sizeof(codec_int));

        g_audio.idx = len;
    } else {
#if ((!UDB_DEBUG_ENABLE) && TLK_USB_UAC_ENABLE)
        /* Get data from usb iso-out buffer. */
#if BITDEPTH_24_ENABLE
        // app_audio_get_usb_data((int64_t *)pcm_temp, num_new);
        if (0 == tlkusb_uac_read_iso_out_samples(pcm_temp, num_new * 2)) {
            return;
        }
        /* mute detect */
        // app_audio_data_mute_detect((int32_t *)pcm_temp, num_new * 2);
#else
        app_audio_get_usb_data((int32_t *)pcm_temp, num_new);
        /* mute detect */
        app_audio_data_mute_detect((int16_t *)pcm_temp, num_new * 2);
#endif
#else //TLK_USB_UAC_ENABLE
#if (!LINEIN_DATA_USE_SIN_48K_DATA)
        app_audio_get_linein_data(pcm_temp, num_new);
#else
        //app_audio_get_sin_48k_data(pcm_temp, num_new);
        app_audio_get_24bit_sin_48k_data(pcm_temp, num_new);
#endif
        int32_t mode = ((clock_time() >> 3) & 127) + 32;
        if (g_usb_mute) {
            for (int32_t i = 0; i < num_new; i++) {
                pcm_temp[i] = (i % mode) == 0 ? g_usb_mute : (i % mode) == 31 ? -g_usb_mute : 0;
            }
        }
#endif
        // static uint16_t i2s_wptr = 0;
        // extern signed int AUDIO_BUFF[2048];
        // for (int i = 0; i < num_new*2; i++) {
        //     AUDIO_BUFF[i2s_wptr++] = pcm_temp[i];
        //     i2s_wptr %= 2048;
        // }
        // int32_t send_data[1];
        // send_data[0] = num_new;
        // spi_master_write_dma(0, (uint8_t *)send_data, 4);
        // while(spi_is_busy(0));
        // gpio_set_high_level(GPIO_PE4);
        // gpio_set_low_level(GPIO_PE4);

        /* ASRC: num_out = num_new +/- 1 */
        // log_task(SL_APP_AUDIO_EN, SL01_le_audio_data, 1);
#if (!TLKALG_PPM_ENABLE)
        // tlk_asrc_switch_flag(1);
        num_out = num_new; //app_audio_asrc_process((int32_t *)pcm_temp, num_new, (int32_t *)cur_ptr);
        tmemcpy((int32_t *)cur_ptr, (int32_t *)pcm_temp, num_new * 8);

        //        int32_t pcm32_stereo_ppm[480*2];
        //        uint16_t ppm_len = ll_dongle_audio_ppm_process((uint8_t *)pcm_temp, (uint8_t *)pcm32_stereo_ppm, num_new, 3);
        //        (void)ppm_len;

#else
        uint32_t reg_value;
        //change int sample to short sample
        int32_t   pcm_16bit[APP_AUDIO_10MS_SAMPLES + APP_AUDIO_PCM_BUFF_PREVENT_OVERFLOW_LEN];
        int32_t   pcm_16bit_proc[APP_AUDIO_10MS_SAMPLES + APP_AUDIO_PCM_BUFF_PREVENT_OVERFLOW_LEN];
        codec_int pcm_temp_proc[APP_AUDIO_10MS_SAMPLES + APP_AUDIO_PCM_BUFF_PREVENT_OVERFLOW_LEN];

        int32_t *psrc      = (int32_t *)pcm_temp;
        short   *pdes      = (short *)pcm_16bit;
        short   *psrc_proc = (short *)pcm_16bit_proc;
        int32_t *pdes_proc = (int32_t *)pcm_temp_proc;

        for (int i = 0; i < num_new * 2; i++) {
            pdes[i] = (psrc[i] >> 8) & 0x0000ffff;
        }

        num_out = tlkalg_ppm_process_stereo(pcm_16bit, pcm_16bit_proc, num_new);

#ifdef SL16_ppm_out_sample
        log_b16(SL_APP_AUDIO_EN, SL16_ppm_out_sample, num_out);
#endif
        // num_out = num_new;
        // for (int k = 0; k < num_out; k++) {
        //     pcm_16bit_proc[k] = pcm_16bit[k];
        // }

        for (int j = 0; j < num_out * 2; j++) {
            pdes_proc[j] = psrc_proc[j] << 8;
        }

        tmemcpy((int32_t *)cur_ptr, (int32_t *)pcm_temp_proc, num_out * 8);

        if (PPM_REF_MODE_SPK == app_usb_ppm_get_ref_mode()) {
            reg_value = irq_disable();
            g_ll_dongle_usb_resample_diff += (num_out - num_new);
            irq_restore(reg_value);
        }
#endif

        g_audio.idx += num_out;
    }
}

/**
 * @brief     Check whether the current channel is the master headset channel.
 * @param[in] chnl audio channel.
 * @returns   none.
 */
static inline bool app_audio_channel_is_master(int32_t chnl)
{
    if (tpd_headset_is_cc_headset_connected()) {
        /* Left channel first */
        return (AUDIO_CHANNEL_LEFT == chnl);
    } else {
        return (tpd_headset_master_is_left() ? (AUDIO_CHANNEL_LEFT == chnl) : (AUDIO_CHANNEL_RIGHT == chnl));
    }
}

/**
 * @brief     Audio data encode handler.
 * @param[in] chnl audio channel.
 * @param[in] pcm16 audio data buffer.
 * @returns   none.
 */
uint8_t enc_out[90];
#if BITDEPTH_24_ENABLE
_attribute_ram_code_ void app_audio_enc_handler(int32_t chnl, int32_t *pcm32)
{
    /* The pointer used to save encoded data in txfifo. */
    uint8_t *enc_data_ptr = NULL;

    if ((AUDIO_CHANNEL_LEFT != chnl) && (AUDIO_CHANNEL_RIGHT != chnl)) {
        tlkapi_sendU32s(DUMP_APP_MSG1, "audio_enc_task: error! Invalid channel", chnl, 0, 0, 0);
        return;
    }
    if (NULL == pcm32) {
        /* invalid data pointer */
        tlkapi_sendU32s(DUMP_APP_MSG1, "audio_enc_task: error! Invalid data pointer", chnl, 0, 0, 0);
        return;
    }

    /* The spk data is encoded and sent only after the headset is connected and
     * synchronized with the dongle audio mode.
     */
    if (!dongle_audio_is_existed()) {
        g_audio_dsample_pos = 0;
        return;
    }


#ifdef SL08_ll_audio_chn
    log_b8(SL_APP_AUDIO_EN, SL08_ll_aud io_chn, chnl);
#endif

#ifdef SL01_le_audio_enc
    log_task(SL_APP_AUDIO_EN, SL01_le_audio_enc, 1);
#endif
    /* Bluetooth call and music is not in progress on the headset,keep 48k sampling
	 * rate.
	 */
    g_audio_dsample_pos      = 0;
    uint16_t lc3_enc_out_len = 0;
    if (tpd_host_is_ultra_latency_mode()) {
        lc3_enc_out_len = APP_AUDIO_ULTRA_LL_LC3A_FRMAE_LEN;
    } else {
        lc3_enc_out_len = APP_AUDIO_LC3A_FRMAE_LEN;
    }
    /* push packet to txfifo */
    enc_data_ptr = tpd_sco_music_msg_push_txfifo(AUDIO_FORMAT_LC3A, chnl, lc3_enc_out_len, tpd_msg_tx_done_handler);

#if (AUDIO_SIMILATE_ENC_SIN_48K_DATA_EN)

    if (enc_data_ptr) {
        //		tmemcpy(enc_data_ptr, &g_sin_48k_enc_data[0], 90);
        int ret = lc3_encode24(chnl, g_sin_48k_mono_10ms, APP_AUDIO_10MS_SAMPLES, enc_data_ptr);
    }

#else // real-lc3-encode
    /* After 480 samples of pcm data are encoded by lc3, 90 bytes of
	* data are generated and stored in enc_data_ptr.
	*/
    if (enc_data_ptr) {
        // DBG_MINGQIAN_CHN8_HIGH;
        uint16_t data_len = ll_audio_get_frame_length();

#if TLKALG_LC3_24BIT_ENC_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_ENC);
#elif TLKALG_LC3_PLUS_ENC_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
#endif

#ifdef DONGLE_AUDIO_PATH_GPIO_DEBUG
        gpio_write(GPIO_PF1, 1);
#endif
        if (p_audio_alg_if->audio_alg_process) {
            p_audio_alg_if->audio_alg_process((uint8_t *)pcm32, (uint8_t *)enc_data_ptr, data_len, ALG_WIDTH_24, chnl);
#if 0
           uint8_t lc3_plus_enc_out_data[90] = {
               0xff, 0xdd, 0x1c, 0xea, 0x18, 0x2c, 0xee, 0xd7, 0x7e, 0x6b,
               0x88, 0x0b, 0x28, 0x18, 0x01, 0x90, 0x97, 0x52, 0x44, 0x3c,
               0xf9, 0x6b, 0xd9, 0x4e, 0xa3, 0x63, 0xc3, 0x49, 0x57, 0x2f,
               0x36, 0x25, 0xff, 0xcb, 0xd6, 0x8f, 0x50, 0x0a, 0xaa, 0x87,
               0x50, 0x70, 0x65, 0x7c, 0x46, 0x11, 0x43, 0x22, 0xd0, 0x72,
               0x3b, 0x31, 0x14, 0xd1, 0x91, 0x42, 0xa3, 0x60, 0xcf, 0x0a,
               0x3d, 0x67, 0x05, 0xcd, 0x4e, 0x2e, 0x05, 0x53, 0xcf, 0x62,
               0x4a, 0x37, 0x46, 0x78, 0xab, 0xeb, 0x0c, 0xca, 0x4a, 0x12,
               0xf4, 0x5a, 0xde, 0xf3, 0x6e, 0x76, 0x72, 0xf1, 0x4c, 0x32,
           };

           tmemcpy(enc_data_ptr, lc3_plus_enc_out_data, 90);
#endif
        }
#ifdef DONGLE_AUDIO_PATH_GPIO_DEBUG
        gpio_write(GPIO_PF1, 0);
#endif
        // DBG_MINGQIAN_CHN8_LOW;
        // log_b16(SL_APP_AUDIO_EN,SL16_ll_dg_audio_s_enc_data,enc_data_ptr[0] << 8 | enc_data_ptr[1]);
    }
#endif

#if 0
    if (AUDIO_CHANNEL_RIGHT == chnl) {
    	for (int i = 0; i < lc3_enc_out_len; i++) {
    		enc_out[i] = enc_data_ptr[i];
    	}
    } else {
    	for (int i = 0; i < lc3_enc_out_len; i++) {
    		enc_data_ptr[i] = enc_out[i];
    	}
    }
#endif

    tpd_sco_music_msg_push_txfifo_done(chnl);

#ifdef SL01_le_audio_enc
    log_task(SL_APP_AUDIO_EN, SL01_le_audio_enc, 0);
#endif
}
#else
_attribute_ram_code_ void app_audio_enc_handler(int32_t chnl, int16_t *pcm16)

{
    static int16_t pcm_16k[APP_AUDIO_10MS_SAMPLES] = {0};  /* Buffer used to save 16k sampling rate data. */
    uint8_t       *enc_data_ptr                    = NULL; /* The pointer used to save encoded data in txfifo. */
#if (AUDIO_DBG_USE_MASTER_ENC_DATA || AUDIO_DBG_USE_SLAVE_ENC_DATA)
    static uint8_t *master_enc_data_ptr = NULL;
#endif
    if ((AUDIO_CHANNEL_LEFT != chnl) && (AUDIO_CHANNEL_RIGHT != chnl)) {
        tlkapi_sendU32s(DUMP_APP_MSG1, "audio_enc_task: error! Invalid channel", chnl, 0, 0, 0);
        return;
    }
    if (NULL == pcm16) {
        /* invalid data pointer */
        tlkapi_sendU32s(DUMP_APP_MSG1, "audio_enc_task: error! Invalid data pointer", chnl, 0, 0, 0);
        return;
    }

    /* The spk data is encoded and sent only after the headset is connected and
     * synchronized with the dongle audio mode.
     */
    if (!dongle_audio_is_existed()) {
        g_audio_dsample_pos = 0;
        return;
    }
#ifdef SL01_le_audio_enc
    log_task(SL_APP_AUDIO_EN, SL01_le_audio_enc, 1);
#endif

    /* When BT audio is existed (BT music/phone), it is necessary to
     * down-sample rate and send packet every 30ms(3*10ms)
     */
    if (!tpd_headset_is_cc_headset_connected() && bt_audio_is_existed()) {
        /* down-sample 480 samples of 48k sampling rate to 160 samples of 16k
         * sampling rate.
         */
        for (int32_t i = 0; i < APP_AUDIO_10MS_SAMPLES; i += APP_AUDIO_48K_TO_16K_IN_SAMPLES) {
            tlk_resample48to16_data((int *)(pcm16 + i), APP_AUDIO_48K_TO_16K_IN_SAMPLES, (int *)(pcm_16k + (g_audio_dsample_pos % 3) * APP_AUDIO_48K_TO_16K_OUT_SAMPLES + i / 3),
                                    0);
        }
#ifdef SL08_ll_dsample_pos
        log_b8(SL_STACK_TPD_ACL_EN, SL08_ll_dsample_pos, g_audio_dsample_pos);
#endif
        if ((g_audio_dsample_pos % 3) == 2) {
            /* Process 480 samples every 30ms */

            /*if bt music need to be mixed, close this code*/
#if !BT_AUDIO_MUSIC_MIXED_ENABLE
            if (bt_audio_is_music()) {
#ifdef SL01_le_audio_enc
                log_task(SL_APP_AUDIO_EN, SL01_le_audio_enc, 0);
#endif
                return;
            }
#endif
            /* push packet to txfifo */
            enc_data_ptr = tpd_sco_music_msg_push_txfifo(AUDIO_FORMAT_16K_MASK | AUDIO_FORMAT_LC3A, chnl, APP_AUDIO_LC3A_FRMAE_LEN, tpd_msg_tx_done_handler);

            if (enc_data_ptr) {
                /* After 480 samples of pcm data are encoded by lc3, 90 bytes of
                * data are generated and stored in enc_data_ptr.
                */
                tlkalg_lc3_enc_process(chnl, pcm_16k, APP_AUDIO_10MS_SAMPLES, enc_data_ptr);
                // log_b16(SL_APP_AUDIO_EN,SL16_ll_dg_audio_m_enc_data,enc_data_ptr[0] << 8 | enc_data_ptr[1]);
            }
            tpd_sco_music_msg_push_txfifo_done(chnl);
        }

        g_audio_dsample_pos++;

        if (!g_audio_dsample_pos) {
            g_audio_dsample_pos = 1;
        }
    } else {
        /* Bluetooth call and music is not in progress on the headset,keep 48k sampling
         * rate.
         */
        g_audio_dsample_pos = 0;

        if (tpd_headset_is_cc_headset_connected() && app_audio_channel_is_master(chnl)) {
            /* push packet to txfifo */
            enc_data_ptr = tpd_sco_music_msg_push_txfifo(AUDIO_FORMAT_LC3A, chnl, APP_AUDIO_LC3A_FRMAE_LEN, tpd_msg_tx_done_handler);

            if (enc_data_ptr) {
                tlkalg_lc3_enc_process(chnl, pcm16, APP_AUDIO_10MS_SAMPLES, enc_data_ptr);
                // log_b16(SL_APP_AUDIO_EN,SL16_ll_dg_audio_m_enc_data,enc_data_ptr[0] << 8 | enc_data_ptr[1]);
            }
        } else {
            /* push packet to txfifo */
            enc_data_ptr = tpd_sco_music_msg_push_txfifo(AUDIO_FORMAT_LC3A, chnl, APP_AUDIO_LC3A_FRMAE_LEN, tpd_msg_tx_done_handler);

            /* After 480 samples of pcm data are encoded by lc3, 90 bytes of
            * data are generated and stored in enc_data_ptr.
            */
            if (enc_data_ptr) {
#if AUDIO_DBG_USE_MASTER_ENC_DATA
                if ((tpd_headset_master_is_left() && (AUDIO_CHANNEL_LEFT == chnl)) || (!tpd_headset_master_is_left() && (AUDIO_CHANNEL_RIGHT == chnl))) { /* master channel */
                    master_enc_data_ptr = enc_data_ptr;

                    tlkalg_lc3_enc_process(chnl, pcm16, APP_AUDIO_10MS_SAMPLES, enc_data_ptr);

                } else {
                    tmemcpy(enc_data_ptr, master_enc_data_ptr, APP_AUDIO_LC3A_FRMAE_LEN); /* copy master enc data */

                    tlkalg_lc3_enc_process(chnl, pcm16, APP_AUDIO_10MS_SAMPLES, (uint8_t *)pcm_16k); /* drop slave enc data */
                }
#elif AUDIO_DBG_USE_SLAVE_ENC_DATA
                if ((tpd_headset_master_is_left() && (AUDIO_CHANNEL_LEFT == chnl)) || (!tpd_headset_master_is_left() && (AUDIO_CHANNEL_RIGHT == chnl))) { /* master channel */
                    master_enc_data_ptr = enc_data_ptr;

                    tlkalg_lc3_enc_process(chnl, pcm16, APP_AUDIO_10MS_SAMPLES, (uint8_t *)pcm_16k); /* drop master enc data */

                } else {
                    tlkalg_lc3_enc_process(chnl, pcm16, APP_AUDIO_10MS_SAMPLES, enc_data_ptr);

                    tmemcpy(master_enc_data_ptr, enc_data_ptr, APP_AUDIO_LC3A_FRMAE_LEN); /* copy slave enc data */
                }
#else
                tlkalg_lc3_enc_process(chnl, pcm16, APP_AUDIO_10MS_SAMPLES, enc_data_ptr);
#endif
                // log_b16(SL_APP_AUDIO_EN,SL16_ll_dg_audio_s_enc_data,enc_data_ptr[0] << 8 | enc_data_ptr[1]);
            }
        }

        tpd_sco_music_msg_push_txfifo_done(chnl);

        // if (((AUDIO_CHANNEL_LEFT == chnl) && tpd_headset_master_is_left()) || ((AUDIO_CHANNEL_LEFT != chnl) && !tpd_headset_master_is_left())) {
        //     log_b16(SL_APP_AUDIO_EN,SL16_ll_dg_audio_m_enc_data,enc_data_ptr[0] << 8 | enc_data_ptr[1]);
        // } else {
        //     log_b16(SL_APP_AUDIO_EN,SL16_ll_dg_audio_s_enc_data,enc_data_ptr[0] << 8 | enc_data_ptr[1]);
        // }
    }
#ifdef SL01_le_audio_enc
    log_task(SL_APP_AUDIO_EN, SL01_le_audio_enc, 0);
#endif
}
#endif


/**
 * @brief     Processing audio data.
 * @param[in] master_audio_chnl The master headset audio channel.
 * @returns   none.
 */
_attribute_ram_code_ void ll_dongle_audio_encode_task(void)
{
    /* handle data of one channel */
#if BITDEPTH_24_ENABLE
    int32_t pcm32[APP_AUDIO_10MS_SAMPLES + APP_AUDIO_PCM_BUFF_PREVENT_OVERFLOW_LEN];
    int32_t pcm32_r[APP_AUDIO_10MS_SAMPLES + APP_AUDIO_PCM_BUFF_PREVENT_OVERFLOW_LEN];
#else
    int16_t pcm16[APP_AUDIO_10MS_SAMPLES + APP_AUDIO_PCM_BUFF_PREVENT_OVERFLOW_LEN];
#endif

    uint16_t data_len = ll_audio_get_frame_length();

    //uint32_t t2ms;
    uint8_t         audio_chnl_shift_bits = 0;
    audio_channel_e master_audio_chnl     = AUDIO_CHANNEL_LEFT;

    if (!tpd_headset_is_cc_headset_connected() && !tpd_headset_master_is_left()) {
        master_audio_chnl = AUDIO_CHANNEL_RIGHT;
    }

#if TLKALG_PPM_ENABLE
    uint32_t reg_value;
    if (app_usb_ppm_get_init_flag_down()) {
        app_usb_ppm_clear_init_flag_down();
        reg_value   = irq_disable();
        int ppm_val = app_usb_ppm_get_value();
        tlkalg_ppm_set_val_stereo(ppm_val);
        irq_restore(reg_value);
    }
#endif

    // app_usb_iso_check_status();

    // if (!app_usb_get_audio_mode(APP_USB_AUDIO_MODE_SPK)) {
    if (!tlkmdi_ll_dongle_get_spk_state()) {
        // gpio_set_high_level(GPIO_PE6);
        // gpio_set_low_level(GPIO_PE6);
        /* audio mode stopped */
#if BITDEPTH_24_ENABLE
        uint8_t *enc_buff_ptr = (uint8_t *)pcm32;
#else
        uint8_t *enc_buff_ptr = (uint8_t *)pcm16;
#endif
        /* Restore the sampling rate flag of audio data transmission to 48k
         * mode.
         */
        g_audio_dsample_pos = 0;

        if (last_usb_spk_on) {
            tlkapi_sendU32s(DUMP_APP_MSG1, "reset_iso_out", tpd_env_get_fno(), 0, 0, 0);
            //app_usb_iso_out_reset_status();

            /* Reset index of g_audio.pcm */
            g_audio.idx = 0;

            /* Encode mute data by LC3 format, store in txfifo */
#if BITDEPTH_24_ENABLE
#if TLKALG_LC3_24BIT_ENC_ENABLE
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_24BIT_ENC);
#elif TLKALG_LC3_PLUS_ENC_ENABLE
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_LC3_PLUS_ENC);
#endif
            if (p_audio_alg_if->audio_alg_process) {
                p_audio_alg_if->audio_alg_process((uint8_t *)g_audio.pcm, (uint8_t *)enc_buff_ptr, data_len, ALG_WIDTH_24, master_audio_chnl);
            }
            //lc3_encode24(AUDIO_CHANNEL_LEFT, (int32_t *)g_audio.pcm, APP_AUDIO_10MS_SAMPLES, enc_buff_ptr);
#else
            tlkalg_lc3_enc_process(AUDIO_CHANNEL_LEFT, (int16_t *)g_audio.pcm, APP_AUDIO_10MS_SAMPLES, enc_buff_ptr);
#endif
            // disable by mingqian 20241116, it will make fifo msg id error, and copy unknown data to right channel,
            // Gradually in and out need to re-discuss with Mingqian, Junwei, Xiaogang.
            //            app_audio_msg_txfifo_init(enc_buff_ptr, APP_AUDIO_LC3A_FRMAE_LEN);

            /* clear buff_playback */
            tlkdrv_codec_muteSpkBuff();
        }
    } else { /* Music mode exists. */
        if (!last_usb_spk_on) {
            /* Music mode just started. */
            app_audio_sync_samples();
            // gpio_set_high_level(GPIO_PE7);
            // gpio_set_low_level(GPIO_PE7);
        }

        // gpio_set_high_level(GPIO_PF0);
        // gpio_set_low_level(GPIO_PF0);

        audio_chnl_shift_bits = (AUDIO_CHANNEL_RIGHT == master_audio_chnl) ? APP_AUDIO_CHANNEL_SHIFT_BITS : 0;

        app_audio_sync_samples_check(data_len);

#if 0
        /* Get the last 2 ms data stored in g_audio.pcm buffer last time. */
        if (!tpd_headset_is_cc_headset_connected() && bt_audio_is_existed()) {
            /* Bluetooth music and call is in progress on the headset, merge left and
             * right channel data.
             */
#if 0
        	//new function support 48K mixed audio and not need to mix left channel with right channel
            app_audio_mix_pcm16_from_pcm32(&g_audio.pcm[APP_AUDIO_8MS_SAMPLES], APP_AUDIO_2MS_SAMPLES, &pcm16[0]);
#endif
        } else {
            for (int32_t i = 0; i < APP_AUDIO_2MS_SAMPLES; i++) {
#if BITDEPTH_24_ENABLE
            	pcm32[i] = g_audio.pcm[i + APP_AUDIO_8MS_SAMPLES] >> audio_chnl_shift_bits;
#else
                pcm16[i] = g_audio.pcm[i + APP_AUDIO_8MS_SAMPLES] >> audio_chnl_shift_bits;
#endif
            }
        }
#endif

        if (!tpd_host_is_ultra_latency_mode()) {
            /* get 8 ms data first part here 300 */
            app_audio_get_audio_data(APP_AUDIO_AUDIO_PART1_SAMPLES);

            /* get 8 ms data second part here 60 */
            app_audio_get_audio_data(APP_AUDIO_AUDIO_PART1_SAMPLES + APP_AUDIO_AUDIO_PART2_SAMPLES);

            //t2ms = clock_time();

            /* get 8 ms data third part here 24 */
            app_audio_get_audio_data(APP_AUDIO_8MS_SAMPLES);
        }

        app_audio_get_audio_data(data_len);

        /* Store the previously obtained 8 ms data in the buffer. */
        if (!tpd_headset_is_cc_headset_connected() && bt_audio_is_existed()) {
            /* Bluetooth call is in progress on the headset, merge left and
             * right channel data.
             */
#if 0
        	//new function support 48K mixed audio and not need to mix left channel with right channel
            app_audio_mix_pcm16_from_pcm32(&g_audio.pcm[0], APP_AUDIO_8MS_SAMPLES, &pcm16[APP_AUDIO_2MS_SAMPLES]);
#endif
        } else {
            for (int32_t i = 0; i < data_len; i++) {
#if BITDEPTH_24_ENABLE
                pcm32[i] = g_audio.pcm[i] >> audio_chnl_shift_bits;
#else
                pcm16[APP_AUDIO_2MS_SAMPLES + i] = g_audio.pcm[i] >> audio_chnl_shift_bits;
#endif
            }

            for (int32_t i = 0; i < data_len; i++) {
#if BITDEPTH_24_ENABLE
                pcm32_r[i] = g_audio.pcm[i] >> (APP_AUDIO_CHANNEL_SHIFT_BITS - audio_chnl_shift_bits);
#else
                pcm16[APP_AUDIO_2MS_SAMPLES + i] = g_audio.pcm[i] >> audio_chnl_shift_bits;
#endif
            }
        }

        /* Encode the data of 1st channel and save it into txfifo */
#if BITDEPTH_24_ENABLE
        app_audio_enc_handler(master_audio_chnl, pcm32);
#else
        app_audio_enc_handler(master_audio_chnl, pcm16);
#endif

        /*
        while ((uint32_t)(clock_time() - t2ms) < APP_AUDIO_2MS_TICK) {
            continue;
        }
*/

        /* Get next 2 ms data 96*/
        //app_audio_get_audio_data(APP_AUDIO_10MS_SAMPLES);

        if (tpd_headset_is_cc_headset_connected() || (!bt_audio_is_existed() && !tpd_headset_is_single_role())) {
#if 0
            /*
            * exit encoder when BT audio is existed and current channel is right channel
            */
#if (!AUDIO_DBG_DATA_SLAVE_SAME_AS_MASTER)
            /* Store the whole 10 ms data in the buffer. */
            for (int32_t i = 0; i < APP_AUDIO_10MS_SAMPLES; i++) {
#if BITDEPTH_24_ENABLE
                //pcm32[i] = g_audio.pcm[i] >> (APP_AUDIO_CHANNEL_SHIFT_BITS - audio_chnl_shift_bits);
#else
                pcm16[i] = g_audio.pcm[i] >> (APP_AUDIO_CHANNEL_SHIFT_BITS - audio_chnl_shift_bits);
#endif
            }
#endif
#endif

            /* Encode the data of 2nd channel and save it into txfifo. */
#if BITDEPTH_24_ENABLE
            app_audio_enc_handler(((master_audio_chnl == AUDIO_CHANNEL_LEFT) ? AUDIO_CHANNEL_RIGHT : AUDIO_CHANNEL_LEFT), pcm32_r);
#else
            app_audio_enc_handler(((master_audio_chnl == AUDIO_CHANNEL_LEFT) ? AUDIO_CHANNEL_RIGHT : AUDIO_CHANNEL_LEFT), pcm16);
#endif
        }

        // g_ll_dongle_usb_ppm.t_m_samples_buff = (g_ll_dongle_usb_ppm.t_m_samples_buff << 16) | app_usb_iso_out_get_samples_number();
    }

    // last_usb_spk_on = app_usb_get_audio_mode(APP_USB_AUDIO_MODE_SPK);
    last_usb_spk_on = tlkmdi_ll_dongle_get_spk_state();
}

/**
 * @brief   Start the audio decode task.
 * @param   None
 * @return  0
 */
_attribute_ram_code_ uint8_t ll_dongle_audio_start_decode_task(void)
{
    uint8_t ret = 0;

    ll_dongle_audio_ctx.tick0 = clock_time() | 1;

    if (!tlkmdi_ll_dongle_music_is_busy()) {
        return 0;
    }

    uint32_t ref_tus_mic_enc = ll_audio_get_tus_mic_enc();

    tlkmdi_audio_task_set_next_irq(ref_tus_mic_enc + ll_dongle_audio_ctx.tus_mic);

    ll_dongle_audio_ctx.tick_timer = ll_dongle_audio_ctx.tick0 + (ref_tus_mic_enc + ll_dongle_audio_ctx.tus_mic) * SYSTEM_TIMER_TICK_1US;

    ll_dongle_audio_ctx.pkt_rptr = TLKMDI_LL_DONGLE_DEC;

    return ret;
}

/**
 * @brief   Main audio processing loop for the dongle.
 * @param   None
 * @return  None
 */
_attribute_ram_code_ void ll_dongle_audio_main(void)
{
#ifdef DONGLE_AUDIO_PATH_GPIO_DEBUG
    gpio_write(GPIO_PF2, 1);
#endif

    /* | lc3 decode from headset mic | lc3 encode send to headset| */
    /* state: IDLE -> DEC -> ENC -> IDLE */
    if (ll_dongle_audio_ctx.pkt_rptr == TLKMDI_LL_DONGLE_IDLE) {
        /* next state is decode */
        ll_dongle_audio_ctx.pkt_rptr = TLKMDI_LL_DONGLE_DEC;

        /* normal TUS_MIC_ENC (3800us) */
        /* bt voice: ll_dongle_audio_ctx.tus_mic: 1250us, other: 0 */
        uint32_t ref_tus_mic_enc = ll_audio_get_tus_mic_enc();
        tlkmdi_audio_task_set_next_irq(ref_tus_mic_enc + ll_dongle_audio_ctx.tus_mic);
    } else if (ll_dongle_audio_ctx.pkt_rptr == TLKMDI_LL_DONGLE_DEC) {
        /* next state is encode */
        ll_dongle_audio_ctx.pkt_rptr = TLKMDI_LL_DONGLE_ENC;

        /* ref_tus_mic_dec: TUS_SPK_DEC (1000) */
        /* bt audio: tus_single_tpsll_enc_offset = TUS_MIC_ENC_OFFSET (1000), other: 0 */
        uint32_t ref_tus_mic_dec = ll_audio_get_tus_spk_dec();
        tlkmdi_audio_task_set_next_irq(ref_tus_mic_dec + ll_dongle_audio_ctx.tus_single_tpsll_enc_offset);

        //uint32_t tmp = ref_tus_mic_dec + ll_dongle_audio_ctx.tus_single_tpsll_enc_offset;
        //tlkapi_printf(APP_AUDIO_LOG_EN, "enc: %d, off: %d", tmp, ll_dongle_audio_ctx.tus_single_tpsll_enc_offset);

        ll_dongle_audio_ctx.tick_timer += (ref_tus_mic_dec + ll_dongle_audio_ctx.tus_single_tpsll_enc_offset) * SYSTEM_TIMER_TICK_1US;
#ifdef SL01_ll_decode_task
        log_task_begin_irq(1, SL01_ll_decode_task);
#endif
        ll_dongle_audio_decode_task();
#ifdef SL01_ll_decode_task
        log_task_end_irq(1, SL01_ll_decode_task);
#endif
    } else if (ll_dongle_audio_ctx.pkt_rptr == TLKMDI_LL_DONGLE_ENC) {
        ll_dongle_audio_ctx.pkt_rptr = TLKMDI_LL_DONGLE_IDLE;

        uint32_t ref_tus_mic_enc      = ll_audio_get_tus_mic_enc();
        uint32_t ref_tus_mic_dec      = ll_audio_get_tus_spk_dec();
        uint16_t ref_tus_frame_length = ll_audio_get_tus_frame_length();
        uint32_t tick_us              = ref_tus_frame_length - ref_tus_mic_dec - ref_tus_mic_enc - ll_dongle_audio_ctx.tus_mic - ll_dongle_audio_ctx.tus_single_tpsll_enc_offset;
        tlkmdi_audio_task_set_next_irq(tick_us);

#ifdef SL01_ll_encode_task
        log_task_begin_irq(1, SL01_ll_encode_task);
#endif
        ll_dongle_audio_encode_task();
#ifdef SL01_ll_encode_task
        log_task_end_irq(1, SL01_ll_encode_task);
#endif
    }

#ifdef DONGLE_AUDIO_PATH_GPIO_DEBUG
    gpio_write(GPIO_PF2, 0);
#endif
}

/**
 * @brief   Enter audio mode for the dongle.
 * @param   None
 * @return  None
 */
void ll_dongle_audio_enter_audio_mode(void)
{
    uint32_t ref_tus_mic_enc_offset = ll_audio_get_tus_mic_enc_offset();

    plic_interrupt_enable(IRQ_TIMER0);
    plic_set_priority(IRQ_TIMER0, 1);
    timer_clr_irq_status(FLD_TMR0_MODE_IRQ);
    timer_set_irq_mask(FLD_TMR0_MODE_IRQ);

    // app_usb_set_audio_mode(APP_USB_AUDIO_MODE_SPK);
    // app_usb_set_audio_mode(APP_USB_AUDIO_MODE_MIC);

    // tpd_stimer_start_cb_register(ll_dongle_audio_start_decode_task);

    ll_dongle_audio_ctx.tick0                       = clock_time() | 1;
    ll_dongle_audio_ctx.tus_mic                     = 0;
    ll_dongle_audio_ctx.tus_single_tpsll_enc_offset = ref_tus_mic_enc_offset;
    ll_dongle_audio_ctx.tick_timer                  = 0;
    ll_dongle_audio_ctx.pkt_rptr                    = TLKMDI_LL_DONGLE_DEC;
}

/**
 * @brief   Callback function for handling audio path commands.
 * @param[in] cmd The command to process.
 *                TPD_SET_AUDIO_PATH_IDLE = 0x00
 *                TPD_STIMER_START_EVENT  = 0x01
 * @return  uint8_t - Result of the callback processing.
 */
_attribute_ram_code_ uint8_t ll_dongle_audio_path_callback(uint16_t cmd)
{
    // tlkapi_printf(APP_AUDIO_LOG_EN, "cmd/sco_data: %d, %d", cmd, tpd_env.dg_sco_audio_data_start_in_bt_phone);
    uint32_t ref_tus_mic_enc_offset = ll_audio_get_tus_mic_enc_offset();
    switch (cmd) {
    case TPD_STIMER_START_EVENT:
#ifdef DONGLE_AUDIO_PATH_GPIO_DEBUG
        gpio_write(GPIO_PF3, 0);
        gpio_write(GPIO_PF3, 1);
        gpio_write(GPIO_PF3, 0);
#endif
        // if ((tpd_host_get_dongle_mode() & TPD_HOST_MODE_DONGLE_AUDIO) &&
        //     (((tpd_host_get_dongle_mode() & TPD_HOST_MODE_BT_VOICE) && (tpd_get_sco_pos_in_bt_voice() == 0)) ||
        //     (!(tpd_host_get_dongle_mode() & TPD_HOST_MODE_BT_AUDIO)) ||
        //     (tpd_host_get_dongle_mode() & TPD_HOST_MODE_BT_MUSIC))) {
        //     gpio_set_high_level(GPIO_PE5);
        //     gpio_set_low_level(GPIO_PE5);
        //     ll_dongle_audio_start_decode_task();
        // }

        if ((tpd_host_get_dongle_mode() & TPD_HOST_MODE_DONGLE_AUDIO)) {
            if ((tpd_host_get_dongle_mode() & TPD_HOST_MODE_BT_VOICE)) {
                ll_dongle_audio_ctx.tus_mic = 1250;
            } else {
                ll_dongle_audio_ctx.tus_mic = 0;
            }

            if ((tpd_host_get_dongle_mode() & TPD_HOST_MODE_BT_AUDIO)) {
                ll_dongle_audio_ctx.tus_single_tpsll_enc_offset = 500;
            } else {
                ll_dongle_audio_ctx.tus_single_tpsll_enc_offset = ref_tus_mic_enc_offset;
            }

            if ((tpd_host_get_dongle_mode() & TPD_HOST_MODE_BT_VOICE) && !(tpd_get_sco_pos_in_bt_voice() == 0)) {
                return 0;
            }

            tpd_host_set_audio_path_status(TPD_HOST_DG_AUDIO_PATH_STATUS_RUNNING);
            ll_dongle_audio_start_decode_task();
        }

        break;
    case TPD_SET_AUDIO_PATH_IDLE:
        ll_dongle_clear_status();
        tlkmdi_audio_stop_timer();
        tpd_host_set_audio_path_status(TPD_HOST_DG_AUDIO_PATH_STATUS_IDLE);
        if (sTlkUsbUacEvt2StatusCB != NULL) {
            sTlkUsbUacEvt2StatusCB(TLK_UAC_MUSIC_STOP_RSP);
        }
        break;
    case TPD_DG_SCO_SETUP_SUCCESS:
        if (sTlkUsbUacEvt2StatusCB != NULL) {
            sTlkUsbUacEvt2StatusCB(TLK_UAC_MUSIC_START_RSP);
        }
        break;
    case TPD_DG_EXIT_SCO_COMPLETE:
        if (sTlkUsbUacEvt2StatusCB != NULL) {
            sTlkUsbUacEvt2StatusCB(TLK_UAC_MUSIC_STOP_RSP);
        }
        // TODO
        break;
    case TPD_SET_DG_AUDIO_PATH_PENDING:
        ll_dongle_clear_status();
        tlkmdi_audio_stop_timer();
        tpd_host_set_audio_path_status(TPD_HOST_DG_AUDIO_PATH_STATUS_IDLE);
        break;
    default:
        break;
    }

    return 0;
}

/**
 * @brief   Get the timestamp for microphone encoding offset.
 * @param   None
 * @return  uint32_t - The timestamp for microphone encoding offset.
 */
uint32_t ll_audio_get_tus_mic_enc_offset(void)
{
    return (tpd_host_is_ultra_latency_mode() ? ULTRA_LL_TUS_MIC_ENC_OFFSET : TUS_MIC_ENC_OFFSET);
}

/**
 * @brief Get MIC encoding timestamp.
 * @param[in] none
 * @return MIC encoding timestamp.
 */
uint32_t ll_audio_get_tus_mic_enc(void)
{
    return (tpd_host_is_ultra_latency_mode() ? ULTRA_LL_TUS_MIC_ENC : TUS_MIC_ENC);
}

/**
 * @brief Get speaker decoding timestamp.
 * @param[in] none
 * @return Speaker decoding timestamp.
 */
uint32_t ll_audio_get_tus_spk_dec(void)
{
    return (tpd_host_is_ultra_latency_mode() ? ULTRA_LL_TUS_SPK_DEC : TUS_SPK_DEC);
}

/**
 * @brief Get frame length for low latency audio.
 * @param[in] none
 * @return Frame length.
 */
uint16_t ll_audio_get_frame_length(void)
{
    return (tpd_host_is_ultra_latency_mode() ? APP_AUDIO_5MS_SAMPLES : APP_AUDIO_10MS_SAMPLES);
}

/**
 * @brief   Get the length of the timestamp frame.
 * @param   None
 * @return  uint16_t - The length of the timestamp frame.
 */
uint16_t ll_audio_get_tus_frame_length(void)
{
    return (tpd_host_is_ultra_latency_mode() ? 5000 : 10000);
}


#endif
