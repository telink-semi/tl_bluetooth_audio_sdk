/********************************************************************************************************
 * @file    bt_audio_main.c
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
#include "drivers.h"
#include "tlkmw/common/tlkaudio_debug_data.h"
#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkmw/sys_dev/tlkmw_sysdev.h"

#if AUDIO_TWS_MODE
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpt_state.h"
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpsll_tws.h"
#endif

#if TLKADU_MIDBUF_ENABLE
#include "vendor/GameSir_Xiaoji/audio_mw/tlkaud_codec_task.h"
#endif

#if !(MCU_CORE_TYPE == CHIP_TYPE_B91)
#include <nds_utils_math.h>
#include <riscv_dsp_utils_math.h>
#endif

#if (TLKBTP_CFG_HFP_ENABLE) // xiaogang will check later
volatile int16_t g_bt_voice_vol = 0;
#endif

#if (TLKBTP_CFG_HFP_ENABLE || TLKBTP_CFG_A2DPSNK_ENABLE)
#define SL_APP_AUDIO_EN SL_STACK_TPH_ACL_EN

#ifndef ALG_EC_ENABLE
#define ALG_EC_ENABLE 0
#endif

#ifndef CODEC_RAM_CODE_EN
#define CODEC_RAM_CODE_EN 0
#endif

#ifndef audio_ram_code

#if CODEC_RAM_CODE_EN
#define audio_ram_code _attribute_ram_code_sec_noinline_
#else
#define audio_ram_code
#endif /* CODEC_RAM_CODE_EN */

#endif /* audio_ram_code */

#ifndef CODEC_VOICE_MIX_DELAY_TEST_EN
#define CODEC_VOICE_MIX_DELAY_TEST_EN 0
#endif

#if (TLK_CFG_HRA_ENABLE)
#define PCM_BLOCK_SIZE 140
#else
#define PCM_BLOCK_SIZE 145
#endif

volatile int16_t g_bt_music_vol       = 0;
volatile int16_t g_bt_music_vol_right = 0;

bt_audio_task_env_t g_bt_audio_task_env;

#if TONE_SYNC_PLAYBACK_MODE
uint16_t g_tone_sync_play_wptr;
uint32_t g_tone_wptr_samples_offset;
#endif

static volatile uint8_t s_timer1_irq_process_is_ongoing = 0;
#if ((TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
extern int tlkmdi_u2h_voice_fillSpkData(uint8_t *pBuffer, uint16_t dataLen);
#endif

bt_audio_get_pcm_data_callback_t  bt_audio_get_pcm_data_cb  = NULL;
bt_audio_post_pcm_data_callback_t bt_audio_post_pcm_data_cb = NULL;

#if TLK_PCM_DATA_WR_EN
bt_audio_trans_pcm_data_callback_t bt_audio_get_spk_data_cb     = NULL;
bt_audio_trans_pcm_data_callback_t bt_audio_get_mic_data_cb     = NULL;
bt_audio_trans_pcm_data_callback_t bt_audio_get_dsp_mic_data_cb = NULL;
#endif

extern int16_t  g_tone_buff[300];
extern int16_t *g_tone_buff_ptr;

#if (TLKSTK_BT_TPS_ENABLE)
extern uint8_t g_ll_audio_mix_mode;
#endif

#if AUDIO_TWS_MODE
/* PPM */
static uint8_t *s_alg_tws_spk_ppm_buff  = NULL;
int             g_app_tws_spk_ppm_ready = 0;
#endif

#if SCO_ENC_QUEUE_ENABLE
extern void btif_process_sco_enc_buff(void);
#endif

extern uint8_t bt_music_dec_update_sync_enc_buf_num;

static bool g_pcm_threshold_check_flag = false;
static bool g_pcm_threshold_flag       = true; //Threshold determination of PCM data.

#define MUSIC_VOLUME_EXPANDER     8
#define SAMPLES_NUM_CHANGE_VOLUME 10
int16_t bt_music_vol_table_ios[17] = {
    0 * MUSIC_VOLUME_EXPANDER,   //-99   0   0
    1 * MUSIC_VOLUME_EXPANDER,   //-64   6   1
    2 * MUSIC_VOLUME_EXPANDER,   //-58   8   2
    5 * MUSIC_VOLUME_EXPANDER,   //-50   7   3
    11 * MUSIC_VOLUME_EXPANDER,  //-43   5   4
    20 * MUSIC_VOLUME_EXPANDER,  //-38   5   5
    36 * MUSIC_VOLUME_EXPANDER,  //-33   4   6
    58 * MUSIC_VOLUME_EXPANDER,  //-29   4   7
    92 * MUSIC_VOLUME_EXPANDER,  //-25   4   8
    147 * MUSIC_VOLUME_EXPANDER, //-21   4   9
    230 * MUSIC_VOLUME_EXPANDER, //-17   3   10
    328 * MUSIC_VOLUME_EXPANDER, //-14   2   11
    410 * MUSIC_VOLUME_EXPANDER, //-12   2   12
    512 * MUSIC_VOLUME_EXPANDER, //-10   2   13
    649 * MUSIC_VOLUME_EXPANDER, //-8    2   14
    812 * MUSIC_VOLUME_EXPANDER, //-6    2   15
    1024 * MUSIC_VOLUME_EXPANDER //-4    0   16
};

int16_t bt_music_vol_table_android[16] = {
    0 * MUSIC_VOLUME_EXPANDER,   //-99   0   0
    2 * MUSIC_VOLUME_EXPANDER,   //-58   8   1
    5 * MUSIC_VOLUME_EXPANDER,   //-50   7   2
    11 * MUSIC_VOLUME_EXPANDER,  //-43   5   3
    20 * MUSIC_VOLUME_EXPANDER,  //-38   5   4
    36 * MUSIC_VOLUME_EXPANDER,  //-33   4   5
    58 * MUSIC_VOLUME_EXPANDER,  //-29   4   6
    92 * MUSIC_VOLUME_EXPANDER,  //-25   4   7
    147 * MUSIC_VOLUME_EXPANDER, //-21   4   8
    230 * MUSIC_VOLUME_EXPANDER, //-17   3   9
    328 * MUSIC_VOLUME_EXPANDER, //-14   2   10
    410 * MUSIC_VOLUME_EXPANDER, //-12   2   11
    512 * MUSIC_VOLUME_EXPANDER, //-10   2   12
    649 * MUSIC_VOLUME_EXPANDER, //-8    2   13
    812 * MUSIC_VOLUME_EXPANDER, //-6    2   14
    1024 * MUSIC_VOLUME_EXPANDER //-4    0   15
};

int16_t bt_voice_vol_table[16] = {
    0,       //-99   0
    2 * 8,   //-58   8
    5 * 8,   //-50   7
    11 * 8,  //-43   5
    20 * 8,  //-38   5
    36 * 8,  //-33   4
    58 * 8,  //-29   4
    92 * 8,  //-25   4
    147 * 8, //-21   4
    230 * 8, //-17   3
    328 * 8, //-14   2
    410 * 8, //-12   2
    512 * 8, //-10   2
    649 * 8, //-8    2
    812 * 8, //-6    2
    1024 * 8 //-4    0
};

codec_cfg_t g_codec_cfg = {
    .sample_rate = 44100,
};

static const uint8_t bt_music_avrcp_absVol_table[16] = {0, 8, 16, 25, 33, 42, 50, 59, 67, 76, 84, 93, 101, 110, 118, 127};

uint32_t g_codec_fifo_size = 0;

/**
 * @brief  calculate dB value of 16-bit audio data
 * @param[in]  data - audio data pointer
 * @param[in]  len  - data length
 * @returns dB value
 */
float tlkmdi_audio_dB_calc_16bit(short *data, uint16_t len)
{
    float temp         = 0.0;
    float rms_frame    = 0.0;
    float rms_frame_db = 0.0;
    for (int i = 0; i < len; i++) {
        temp = data[i];
        temp /= (1 << 15); //16bit-15    24bit-23
        rms_frame += temp * temp;
    }
    rms_frame    = riscv_dsp_sqrt_f32(rms_frame / len);
    rms_frame_db = 20.0 * (riscv_dsp_log_f32(rms_frame) / riscv_dsp_log_f32(10.0));

    // if (rms_frame_db < mute_threshold) {
    //     for (int j = 0; j < len; j++) {
    //         data[j] = 0;
    //     }
    // tlkapi_printf(APP_LOG_EN, "mute en %fdB", rms_frame_db);
    // }

    return rms_frame_db;
}

/**
 * @brief  check if audio dB is less than threshold
 * @param[in]  thr  - threshold value
 * @param[in]  data - audio data pointer
 * @param[in]  len  - data length
 * @returns true if less than threshold, false otherwise
 */
bool tlkmdi_audio_dB_is_less_threshold(float thr, short *data, uint16_t len)
{
    if (tlkmdi_audio_dB_calc_16bit(data, len) < thr) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief  get pcm threshold flag
 * @param[in]  none
 * @returns threshold flag
 */
bool tlkmdi_audio_get_threshold_flag(void)
{
    return g_pcm_threshold_flag;
}

/**
 * @brief  set pcm check threshold flag
 * @param[in]  flag - threshold flag
 * @returns none
 */
void tlkmdi_audio_set_check_threshold_flag(bool flag)
{
    g_pcm_threshold_check_flag = flag;
}


#if TLK_PCM_DATA_WR_EN
/**
 * @brief  register get speaker data callback function
 * @param[in]  p - callback function pointer
 * @returns none
 */
void bt_audio_register_get_spk_data_callback(void *p)
{
    bt_audio_get_spk_data_cb = (bt_audio_trans_pcm_data_callback_t)p;
}

/**
 * @brief  register get mic data callback function
 * @param[in]  p - callback function pointer
 * @returns none
 */
void bt_audio_register_get_mic_data_callback(void *p)
{
    bt_audio_get_mic_data_cb = (bt_audio_trans_pcm_data_callback_t)p;
}

/**
 * @brief  register get dsp mic data callback function
 * @param[in]  p - callback function pointer
 * @returns none
 */
void bt_audio_register_get_dsp_mic_data_callback(void *p)
{
    bt_audio_get_dsp_mic_data_cb = (bt_audio_trans_pcm_data_callback_t)p;
}
#endif

/**
 * @brief  toggle bt music gpio for debugging
 * @param[in]  times - toggle times
 * @returns none
 */
_attribute_ram_code_sec_ void bt_music_gpio_toggle(uint8_t times)
{
    for (uint8_t i = 0; i < times; i++) {
#if 0 //BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
       gpio_write(GPIO_PF5, 0);
       gpio_write(GPIO_PF5, 1);
       gpio_write(GPIO_PF5, 0);
#endif
    }
}

/**
 * @brief  get bt audio task environment pointer
 * @param[in]  none
 * @returns bt audio task environment pointer
 */
audio_ram_code bt_audio_task_env_t *get_bt_audio_task_env_ptr(void)
{
    return &g_bt_audio_task_env;
}

/**
 * @brief  register get pcm data function
 * @param[in]  p - get pcm data function
 * @returns none
 */
void bt_audio_register_get_pcm_data_callback(void *p)
{
    bt_audio_get_pcm_data_cb = (bt_audio_get_pcm_data_callback_t)p;
}

/**
 * @brief  register post pcm data function
 * @param[in]  p - post pcm data function
 * @returns none
 */
void bt_audio_register_post_pcm_data_callback(void *p)
{
    bt_audio_post_pcm_data_cb = (bt_audio_post_pcm_data_callback_t)p;
}

/**
 * @brief  control music volume
 * @param[in]  p      - music data
 * @param[in]  sample - music data sample number
 * @returns none
 */
audio_ram_code void bt_audio_control_music_volume(int16_t *p, uint16_t sample)
{
    int            tmp = 0;
    int            i;
    static uint8_t change_count = 0;

    for (i = 0; i < sample; i++) {
        tmp = p[i] * g_bt_music_vol;
        tmp /= (1024 * MUSIC_VOLUME_EXPANDER);
        p[i] = tmp;

        if (g_bt_music_vol < g_codec_cfg.music_vol_percent) {
            change_count++;
            if (change_count == SAMPLES_NUM_CHANGE_VOLUME) {
                change_count = 0;
                g_bt_music_vol++;
            }
            // g_bt_music_vol++;
        } else if (g_bt_music_vol > g_codec_cfg.music_vol_percent) {
            change_count++;
            if (change_count == SAMPLES_NUM_CHANGE_VOLUME) {
                change_count = 0;
                g_bt_music_vol--;
            }
            // g_bt_music_vol--;
        }
    }
}

/**
 * @brief  control music volume for right channel
 * @param[in]  p      - music data
 * @param[in]  sample - music data sample number
 * @returns none
 */
audio_ram_code void bt_audio_control_music_volume_right(int16_t *p, uint16_t sample)
{
    int            tmp = 0;
    int            i;
    static uint8_t change_count_r = 0;

    for (i = 0; i < sample; i++) {
        tmp = p[i] * g_bt_music_vol_right;
        tmp /= (1024 * MUSIC_VOLUME_EXPANDER);
        p[i] = tmp;

        if (g_bt_music_vol_right < g_codec_cfg.music_vol_percent) {
            change_count_r++;
            if (change_count_r == SAMPLES_NUM_CHANGE_VOLUME) {
                change_count_r = 0;
                g_bt_music_vol_right++;
            }
            // g_bt_music_vol_right++;
        } else if (g_bt_music_vol_right > g_codec_cfg.music_vol_percent) {
            change_count_r++;
            if (change_count_r == SAMPLES_NUM_CHANGE_VOLUME) {
                change_count_r = 0;
                g_bt_music_vol_right--;
            }
            // g_bt_music_vol_right--;
        }
    }
}

/**
 * @brief  register audio task run function and set irq enable flag
 * @param[in]  p_callback - audio task run function
 * @param[in]  enable     - irq enable flag
 * @returns none
 */
void bt_audio_task_register_run_cb(void *p_callback, uint8_t enable)
{
    (void)p_callback;
    AUDIO_GLOBAL_INT_DISABLE();

    g_bt_audio_task_env.irq_en = enable;

    if (enable) {
        /* can not delete this code because other project is different with this project in async init */
#if (MCU_CORE_TL752X_TEMP)
        plic_interrupt_enable(IRQ_TIMER0_0);

        plic_set_priority(IRQ_TIMER0_0, 1);
#else
        plic_interrupt_enable(IRQ_TIMER0);

        plic_set_priority(IRQ_TIMER0, 1);
#endif


#if TLK_CFG_A2DP_TO_BIS_ENABLE
        if (tlkmdi_bt_music_is_busy()) {
            g_bt_audio_task_env.tick_irq = clock_time() + 1500 * TICK_PER_US;
            tlkmdi_audio_task_set_next_irq(1500);
        }
#else
        g_bt_audio_task_env.tick_irq = clock_time() + 1500 * TICK_PER_US;
        tlkmdi_audio_task_set_next_irq(1500);
#endif
    } else {
        tlkmdi_audio_stop_timer();
    }

    AUDIO_GLOBAL_INT_RESTORE();
}

/**
 * @brief  whether tone is playing in 48K or 44.1K
 * @param[in]  none
 * @returns TRUE or FALSE
 */
uint8_t tone_playing_is_in_music_mode(void)
{
    return (tone_is_playing() && (g_codec_cfg.sample_rate == 48000 || g_codec_cfg.sample_rate == 44100));
}

/**
 * @brief  check if bt audio voice needs decode
 * @param[in]  none
 * @returns 1 if need decode, 0 otherwise
 */
audio_ram_code uint8_t bt_audio_voice_is_need_decode(void)
{
    uint8_t  ret                       = 0;
    uint16_t codec_buffer_avail_size   = tlkdrv_codec_get_spk_buf_idle_size();
    uint16_t codec_buff_size_threshold = CODEC_SPK_FIFO_SAMPLES / 2;

#if TLKMW_INTERPHONE_EN
    codec_buff_size_threshold = CODEC_SPK_FIFO_SAMPLES / 2;
#else
    codec_buff_size_threshold = CODEC_SPK_FIFO_SAMPLES / 2;
#endif

#if TLKSTK_BTTPSLL_TWS_ENABLE
    if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
        codec_buff_size_threshold = CODEC_SPK_FIFO_SAMPLES / 4;
    }
#endif

    if (codec_buffer_avail_size > codec_buff_size_threshold) {
        ret = 1;
    }

    return ret;
}

/**
 * @brief  bt audio main loop process
 * @param[in]  none
 * @returns none
 */
audio_ram_code void bt_audio_main_loop(void)
{
#if TLKBTP_CFG_A2DPSNK_ENABLE
    bt_music_loop();
#endif

#if ((TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
    if (tlkmdi_audu2h_voice_isBusy()) {
#elif (TLKBTP_CFG_HFP_ENABLE)
    if (tlkmdi_bt_voice_is_busy()
#if (TLKMW_BT_1_TO_2_FORWARD_EN)
        || tlkmdi_bt_voice_forward_isBusy()
#endif
#if TLKMW_INTERPHONE_EN
        || tlkmdi_interphone_is_busy()
#endif
#if TLKALG_ANC_ENABLE
        || tlkmdi_anc_btvoice_is_busy()
#endif
    ) {
#else
    {
#endif

#if DONGLE_VOICE_MIC_EN && (!TLK_CFG_HRA_ENABLE)
        // ll_audio_bt_voice_mix_mode_uplink_xfer();
        ll_audio_mix_mode_loop();
#if AUDIO_TWS_MODE
        if (tlkmdi_bt_tpt_isMaster() || tlkmdi_bt_tpt_isSingle())
#endif
        {
            ll_audio_mix_switch_mic(); //TODO: Need Evt Trigger
        }
#endif

#if (TLKBTP_CFG_HFP_ENABLE)
#if SCO_ENC_QUEUE_ENABLE
#if (TLKMW_BT_1_TO_2_FORWARD_EN)
        if (tlkmdi_bt_voice_forward_isBusy()) {
            bt_voice_forward_main_loop();
        } else
#endif
        {
            btif_process_sco_enc_buff();
            if (!bt_audio_irq_task_is_running()
#if TLKSTK_BTTPSLL_TWS_ENABLE
                && bt_audio_voice_is_need_decode()
#endif
            ) {
                btif_process_sco_enc_buff();
            } else {
                uint8_t sco_enc_buff_rcv = btif_get_sco_enc_buff_available(SCO_ENC_QUEUE_ID_HF);
                if (sco_enc_buff_rcv > 0) {
                    tlkmdi_audio_runMainAfterTimeMs(2);
                }
            }
        }
#endif
#endif
    }

#if (TLKSTK_BT_TPS_ENABLE)
#if (LE_AUDIO_BT_MUSIC_MIX_ENABLE || LE_AUDIO_BT_VOICE_MIX_ENABLE)
#if (TLK_STK_TPH_ENABLE)
    if (app_tph_headset_get_mode() & TPH_HOST_MODE_DONGLE_AUDIO) {
#elif (TLK_STK_TPT_ENABLE)
    if (app_tph_headset_get_mode() & TPT_HOST_MODE_DONGLE_AUDIO) {
#endif
        tlkmdi_audio_runMainAfterTimeMs(3);
    }
    if ((tlkmdi_bt_music_is_busy()
#if TLKALG_ANC_ENABLE
         || tlkmdi_anc_btmusic_is_busy()
#endif
             ) &&
        ll_audio_is_in_mix_mode()) {
        ll_audio_bt_music_mix_mode_uplink_xfer();
        async_audio_ctx.tpsll_mix_dec_loop_ongoing = 1;
        ll_audio_mix_bt_music_mode_decoder_task_mcu(1);
        ll_audio_mix_mode_loop();
        ll_audio_bt_music_mix_mode_uplink_xfer();
        if (async_audio_ctx.latency_mode != TPSLL_LATENCY_PKTLOSS || async_audio_ctx.tpsll_mix_lc3_dec_cnt < 2) {
            ll_audio_mix_bt_music_mode_decoder_task_mcu(2);
        }
        async_audio_ctx.tpsll_mix_dec_loop_ongoing = 0;
    }
#if DONGLE_VOICE_MIC_EN && (!TLK_CFG_HRA_ENABLE)
#if AUDIO_TWS_MODE
    if (tlkmdi_bt_tpt_isMaster() || tlkmdi_bt_tpt_isSingle()) {
        if (tlkmdi_bt_music_is_busy()
#if TLKALG_ANC_ENABLE
            || tlkmdi_anc_btmusic_is_busy()
#endif
        ) {
            ll_audio_mix_open_mic(); //TODO: Need Evt Trigger
        }
    }
#else
    if (tlkmdi_bt_music_is_busy()
#if TLKALG_ANC_ENABLE
        || tlkmdi_anc_btmusic_is_busy()
#endif
    ) {
        ll_audio_mix_open_mic(); //TODO: Need Evt Trigger
    }
#endif

#endif
#endif
#endif

#if TLKADU_MIDBUF_ENABLE
    if (tlkaud_codec_task_old2new()) {
        //close bt audio scene and reopen it
        tlkaud_codec_task_reopen_cur_audio_task();
    }
#endif
}

audio_ram_code void bt_audio_forward_audio_irq_task(void)
{
    if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
        bt_audio_recorder_task();
    }
}

audio_ram_code uint8_t bt_audio_irq_task_is_running(void)
{
    return s_timer1_irq_process_is_ongoing;
}

void bt_audio_forward_alg_mode_player_task(void);

/**
 * @brief  timer1 irq handler, used to encode/decode sco and decode a2dp
 * @param[in]  none
 * @returns none
 */
_attribute_retention_code_ void bt_audio_main(void)
{
    uint32_t tdiff_us, tdiff_tick;
    //uint16_t rptr = codec_get_speaker_rptr(buff_playback);
    bt_audio_task_env_t *p_bt_audio_task_env = get_bt_audio_task_env_ptr();

#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
    gpio_write(GPIO_PB0, 1);
#endif
    tlkmdi_audio_stop_timer();

#if AUDIO_TWS_MODE
    bt_voice_cfg_t *p_bt_voice_env = get_bt_voice_env_ptr();
    if (p_bt_voice_env->voice_play_sync) {
        if (bt_voice_cfg.sync_samples < 900 && bt_voice_cfg.sync_samples > 800) {
            //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "sync_samples: %d", bt_voice_cfg.sync_samples);
            //bt_voice_spk_ppm_adjust(&spk_ppm, bt_voice_cfg.sync_samples);
        }
    }
#endif

    s_timer1_irq_process_is_ongoing = 1;

#ifdef SL01_audio_task
    log_task_begin_irq(SL_AUDIO_PATH_LOG_EN, SL01_audio_task);
#endif

    if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
#if (AUDIO_TWS_MODE == 0)
        if (tone_is_playing()) {
            if (clock_time() > g_bt_audio_task_env.tick_irq) {
                tdiff_tick = clock_time() - g_bt_audio_task_env.tick_irq;
            } else {
                tdiff_tick = 0xffffffff - g_bt_audio_task_env.tick_irq + clock_time();
            }

            tdiff_us = 7500 - tdiff_tick / TICK_PER_US;
            g_bt_audio_task_env.tick_irq += 7500 * TICK_PER_US;

            tlkmdi_audio_task_set_next_irq(tdiff_us);
        } else {
#if (TLK_CFG_HRA_ENABLE)
            tlkmdi_audio_task_set_next_irq(1500);
#endif
        }
#elif TLK_CFG_HRA_ENABLE
        tlkmdi_audio_task_set_next_irq(1500);
#endif
    } else {
#if (!(TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
        if (clock_time() > g_bt_audio_task_env.tick_irq) {
            tdiff_tick = clock_time() - g_bt_audio_task_env.tick_irq;
        } else {
            tdiff_tick = 0xffffffff - g_bt_audio_task_env.tick_irq + clock_time();
        }

        tdiff_us = tdiff_tick / TICK_PER_US;

        if (tdiff_us > 1000) {
            tdiff_us                     = 1000;
            g_bt_audio_task_env.tick_irq = clock_time() + tdiff_us * TICK_PER_US;
        } else {
#if AUDIO_HD_HAC_EN
            if (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type()) {
                tdiff_us = 10700 - tdiff_us;
                g_bt_audio_task_env.tick_irq += 10700 * TICK_PER_US;
            } else {
                tdiff_us = 1500 - tdiff_us;
                g_bt_audio_task_env.tick_irq += 1500 * TICK_PER_US;
            }
#else
            tdiff_us = 1500 - tdiff_us;
            g_bt_audio_task_env.tick_irq += 1500 * TICK_PER_US;
#endif
        }

#if (TLKADU_MIDBUF_ENABLE)
        tlkmdi_audio_task_set_next_irq(1000);
#else
        tlkmdi_audio_task_set_next_irq(tdiff_us);
#endif
#endif
    }

    if (g_bt_audio_task_env.irq_en) {
#if (TLK_CFG_HRA_ENABLE)
        tlkmdi_hra_bt_audio_proc();
        if (audio_codec_flag_get(CODEC_FLAG_MUSIC)) {
#endif
#if (TLKMW_BT_1_TO_2_FORWARD_EN)
            if (tlkmdi_bt_voice_forward_isBusy()) {
                bt_audio_recorder_task();
                bt_audio_forward_alg_mode_player_task();
            } else
#endif
            {
                bt_audio_recorder_task();
                bt_audio_player_task();
            }

#if (TLK_CFG_HRA_ENABLE)
        } else if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
            if (tlkmdi_hra_stack_get_timer_flag()) {
                tlkmdi_hra_stack_set_timer_flag(false);
                bt_audio_recorder_task();
                bt_audio_player_task();
            }
        }
#endif
    }

#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
    gpio_write(GPIO_PB0, 0);
#endif
    //log_task(SL_APP_AUDIO_EN, SL01_bt_timer0_irq, 0);
    s_timer1_irq_process_is_ongoing               = 0;
    p_bt_audio_task_env->sco_packet_is_processing = 0;

#if (TLKBTP_CFG_HFP_ENABLE)
    if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
        uint8_t sco_enc_buff_rcv = btif_get_sco_enc_buff_available(SCO_ENC_QUEUE_ID_HF);
        if (sco_enc_buff_rcv > 0) {
            tlkmdi_audio_runMainAfterTimeMs(2);
        }

#ifdef SL16_sco_rcv_num
        log_b16(SL_AUDIO_PATH_LOG_EN, SL16_sco_rcv_num, sco_enc_buff_rcv);
#endif
    }
#endif

#ifdef SL01_audio_task
    log_task_end_irq(SL_AUDIO_PATH_LOG_EN, SL01_audio_task);
#endif
}

/**
 * @brief  BT MIC data process task
 * @param[in]  none
 * @returns none
 */
audio_ram_code void bt_audio_recorder_task(void)
{
#if (TLKBTP_CFG_HFP_ENABLE)
    if (bt_voice_cfg.bt_voice_mode == BT_VOICE_PLAYBACK_MODE) {
        bt_voice_process_mic_data();
    }
#endif
}

#ifdef DUMP_AUDIO_TASK_STACK_USAGE
uint32_t             g_bt_audio_stack_usage = 0;
extern tlksys_task_t sTlkSysTaskList[TLKSYS_TASKID_MAXNUM];

/**
 * @brief  print bt audio task stack information
 * @param[in]  none
 * @returns none
 */
void bt_audio_print_task_stack_info(void)
{
    uint32_t g_bt_audio_stack_max_used      = TLKMDI_AUDIO_IRQ_TASK_STASK_SIZE - g_bt_audio_stack_usage;
    uint32_t g_bt_audio_stack_usage_percent = (g_bt_audio_stack_max_used / TLKMDI_AUDIO_IRQ_TASK_STASK_SIZE) * 100;

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "remains:%d, max used:%d, usage_percent max:%d", g_bt_audio_stack_usage, g_bt_audio_stack_max_used,
                 g_bt_audio_stack_usage_percent);
}
#endif

//#define CODEC_BUF_DEBUG

audio_ram_code void bt_audio_forward_alg_mode_player_task(void)
{
    int16_t  buff_mix0[300];
    int16_t  buff_mix1[300];
    uint16_t dec_pcm_num = 0;

    dec_pcm_num = bt_audio_get_pcm_data_cb((int16_t *)buff_mix0, (int16_t *)buff_mix1);
    if (dec_pcm_num == 3) {}
}

/**
 * @brief  bt voice and music playback process, called by timer1_irq_handler
 * @param[in]  none
 * @returns none
 */
audio_ram_code void bt_audio_player_task(void)
{
#if (AAC_CODEC_ENABLE)
#if AUDIO_PATH_24BITS_EN
    int32_t pcm_stereo[PCM_BLOCK_SIZE * 2];

    int16_t buff_mix0[1024 + 4]; // left channel
    int16_t buff_mix1[1024 + 4]; // right channel
#else
    int16_t buff_mix0[1024 + 128];
    int16_t buff_mix1[1024 + 128];
    int32_t pcm[PCM_BLOCK_SIZE];
#endif
#else
#if AUDIO_PATH_24BITS_EN
    int32_t pcm_stereo[PCM_BLOCK_SIZE * 2];

    int32_t buff_mix0[128 + 64]; // left channel
    int32_t buff_mix1[128 + 64]; // right channel
#else
    int16_t buff_mix0[128 + 64];
    int16_t buff_mix1[128 + 64];
    int32_t pcm[PCM_BLOCK_SIZE];
#endif
#endif

#if AUDIO_TWS_MODE
    int16_t pcm_mono_48k_ppm[360 + 200];
#endif

#if ((TLKCFG_MULTI_MIC_EN || TLKSTK_BT_TPS_ENABLE) && TLKALG_ASRC_16TO48_16BIT_ENABLE)
    int16_t pcm_mono_48k_l[360 + 80];
    int16_t pcm_mono_48k_r[360 + 80];
#endif

    uint16_t dec_pcm_num = 0;
    uint16_t pcm_num, samples_num;
    int16_t *ps0 = NULL;
    int16_t *ps1 = NULL;
    uint16_t i   = 0;

#ifdef SL16_codec_fifo_samples
    int samples_in_fifo = tlkdrv_codec_get_spk_avail_samples();
    log_b16(SL_AUDIO_PATH_LOG_EN, SL16_codec_fifo_samples, samples_in_fifo);
#endif

    /* ps0: left, ps1: right */
    ps0 = (int16_t *)buff_mix0;
    ps1 = (int16_t *)buff_mix1;

#if (TLKSTK_BT_TPS_ENABLE)
    if (async_audio_ctx.tpsll_dec_update_mix_buf_is_ongoing == 1 && g_ll_audio_mix_mode == 1) {
        bt_music_gpio_toggle(2);
        return;
    }
#endif

#if (!(TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
    uint16_t codec_buffer_avail_size = 0;
#if TLK_CFG_A2DP_TO_BIS_ENABLE
    if (tlkmdi_a2dp_to_bis_is_busy()) {
        codec_buffer_avail_size = tlkmdi_a2dp_to_bis_get_middle_idle_size();
    } else if (tlkmdi_bt_music_is_busy()) {
        codec_buffer_avail_size = tlkdrv_codec_get_spk_buf_idle_size();
    }
#else
#if TLKMW_INTERPHONE_EN
    if (tlkmdi_interphone_is_busy()) {
        codec_buffer_avail_size = tlkmdi_interphone_get_BtDownBuff_IdleLen() / sizeof(codec_int);
    } else
#endif
    {
        codec_buffer_avail_size = tlkdrv_codec_get_spk_buf_idle_size();
    }
#if TLK_CFG_HRA_ENABLE
    uint16_t mid_spk_buffer_avail_size = tlkmdi_hra_get_mid_spk_idle_size();
#endif
#endif

#ifdef SL16_codec_rptr
    //uint32_t rptr = tlkdrv_codec_get_speaker_rptr();
    log_b16_irq(SL_APP_AUDIO_EN, SL16_codec_rptr, codec_buffer_avail_size);
#endif

#ifdef SL16_dsp_rx_rwptr
    log_b16_irq(SL_APP_AUDIO_EN, SL16_dsp_rx_wptr, bt_music_get_num_of_enc_buff_avail());
#endif

    //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "codec_buffer_avail_size: %d\n", codec_buffer_avail_size);
#if (TLKMW_INTERPHONE_EN || TLK_CFG_A2DP_TO_BIS_ENABLE)
    uint16_t codec_buff_size_threshold = CODEC_SPK_FIFO_SAMPLES / 2;
#else
    uint16_t codec_buff_size_threshold = CODEC_SPK_FIFO_SAMPLES * 60 / 100; //TWS_PLAY_FIFO_SIZE / 2;
#endif

#if TLKSTK_BTTPSLL_TWS_ENABLE
    if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
        codec_buff_size_threshold = CODEC_SPK_FIFO_SAMPLES / 4;
    }
#endif

#ifdef CODEC_BUF_DEBUG
    static uint16_t g_audio_debug_cnt = 0;
    static uint32_t g_last_tick       = 0;
    uint16_t        num_of_speaker_samples0;
    uint16_t        spk_buf_idle_size0;
    uint16_t        num_of_speaker_samples1;
    uint16_t        spk_buf_idle_size1;

    num_of_speaker_samples0 = tlkdrv_codec_get_spk_avail_samples();
    spk_buf_idle_size0      = tlkdrv_codec_get_spk_buf_idle_size();
#endif

    // int samples_in_fifo = tlkdrv_codec_get_spk_avail_samples();
    // uint8_t a2dp_continu_dec = 0;
    // if (ll_audio_is_in_mix_mode()) {
    //     if (clock_time_exceed(async_audio_ctx.stimer_irq_tick, 1250*5)) {
    //         a2dp_continu_dec = 1;
    //     }
    // }

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
    //extern volatile uint8_t g_bt_music_enc_mode_is_changing; //!g_bt_music_enc_mode_is_changing &&
    if ((codec_buffer_avail_size > codec_buff_size_threshold) // || (a2dp_continu_dec && samples_in_fifo < 300)
#if TLK_CFG_HRA_ENABLE
        && (mid_spk_buffer_avail_size > codec_buff_size_threshold)
#endif
    )
#elif TLKMW_RECORDING_CARD_EN
    (void)codec_buff_size_threshold;
    (void)codec_buffer_avail_size;
    if (bt_audio_get_pcm_data_cb)
#else
    if (bt_audio_get_pcm_data_cb && (codec_buffer_avail_size > codec_buff_size_threshold))
#endif // TLKBTP_CFG_A2DPSNK_ENABLE
#endif // !(TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE)
    {

#if (TLKSTK_BT_TPS_ENABLE)
#if BT_TPSLL_OPTIMIZE_LATENCY_TEST
        if (async_audio_ctx.tpsll_dec_is_ongoing && async_audio_ctx.tpsll_5ms_dec_times == 1) {
            // gpio_write(GPIO_PA2, 0);
            // gpio_write(GPIO_PA2, 1);
            // gpio_write(GPIO_PA2, 0);
            return;
        }
#else
        if (async_audio_ctx.tpsll_dec_is_ongoing || async_audio_ctx.tpsll_enc_is_ongoing) {
            async_audio_ctx.tpsll_mix_bt_dec_cnt++;
            if (async_audio_ctx.tpsll_mix_bt_dec_cnt >= 2) {
                bt_music_gpio_toggle(3);
                return;
            }
        } else {
            async_audio_ctx.tpsll_mix_bt_dec_cnt = 0;
        }
#endif
#endif

#if (TLKMW_RECORDING_CARD_EN && TLKBTP_CFG_HFPAG_ENABLE)
        extern bool tlkmdi_record_fill_mic_data_from_uac(uint16_t samples);
        tlkmdi_record_fill_mic_data_from_uac(120 * 2);
#endif

        /* muisc decode or voice decode, mix0 is left, mix1 is right */
        if (bt_audio_get_pcm_data_cb) {
            dec_pcm_num = bt_audio_get_pcm_data_cb((int16_t *)buff_mix0, (int16_t *)buff_mix1);
        } else {
            return;
        }

        if (dec_pcm_num == 3) {
            // bt voice, push pcm data to playback in dsp callback
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "skip.. ");
            bt_music_gpio_toggle(4);
            return;
        }

        if (dec_pcm_num == 0) {
            bt_music_gpio_toggle(1);
#if AUDIO_PATH_DBG
            tlkapi_error(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "play_fail, get 0 pcm data, %d", codec_buffer_avail_size);
#endif
            return;
        } else {
#if 0
            tlkapi_error(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "@@@ play_OK, get %d pcm data, %d", dec_pcm_num, codec_buffer_avail_size);
#endif
        }

        (void)g_pcm_threshold_flag;
        (void)g_pcm_threshold_check_flag;
        if (g_pcm_threshold_check_flag) {
            g_pcm_threshold_flag = tlkmdi_audio_dB_is_less_threshold(-200, (short *)buff_mix0, dec_pcm_num);
        }

#if ((TLKCFG_MULTI_MIC_EN || TLKSTK_BT_TPS_ENABLE) && (TLKALG_ASRC_16TO48_16BIT_ENABLE || AUDIO_HD_HAC_EN))
        if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
#if TLKALG_ASRC_16TO48_16BIT_ENABLE
            uint16_t samples_num_48K = 3 * dec_pcm_num;
#endif

#if 0 //AUDIO_HD_HAC_EN
            //gpio_set_high_level(GPIO_CHN1);
            int voice_dec_stereo_16k[120+20];
            int voice_dec_stereo_48k[360 + 80];
            for (int k = 0; k < dec_pcm_num; k++) {
                voice_dec_stereo_16k[k] = (uint16_t)ps0[k] | (ps0[k] << 16);
            }
            //joint to stereo because current hac interface not support mono processing
            samples_num_48K = tlkdrv_hac_process(&(gp_hac_cfg_st[0]), (int * )voice_dec_stereo_16k, (int * )voice_dec_stereo_48k, dec_pcm_num);
            int16_t *p_48k = (int16_t *)voice_dec_stereo_48k;
            for (int j = 0; j < samples_num_48K; j++) {
                pcm_mono_48k[j] = p_48k[2*j];
            }
            dec_pcm_num = samples_num_48K;
            //gpio_set_low_level(GPIO_CHN1);
#elif TLKALG_ASRC_16TO48_16BIT_ENABLE
            //gpio_set_high_level(GPIO_CHN0);
            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_16TO48_16BIT);
            p_audio_alg_if->audio_alg_process((uint8_t *)ps0, (uint8_t *)pcm_mono_48k_l, 120, 0, 0);
            //gpio_set_low_level(GPIO_CHN0);
            dec_pcm_num = samples_num_48K;

#if (CODEC_VOICE_MIX_DELAY_TEST_EN)
            for (int iIndex = 0; iIndex < samples_num_48K; iIndex++) {
                pcm_mono_48k[iIndex] = 0;
            }
#endif

            if (audio_flag_get(FLG_BTC_TWS_EN)) {
                ps0 = pcm_mono_48k_l;
            } else {
                ps0 = pcm_mono_48k_l;
                ps1 = pcm_mono_48k_r;
                for (i = 0; i < samples_num_48K; i++) {
                    pcm_mono_48k_r[i] = pcm_mono_48k_l[i];
                }
            }

#else
            ps1 = (int16_t *)buff_mix0;
#endif

#if AUDIO_TWS_MODE
            if (g_app_tws_spk_ppm_ready) {
                uint16_t length = app_tws_spk_ppm_process((uint8_t *)ps0, (uint8_t *)pcm_mono_48k_ppm, dec_pcm_num, 1);
                ps0             = pcm_mono_48k_ppm;

                if (length != dec_pcm_num) {
                    //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "== VOICE ppm len: %d, num:%d", length, dec_pcm_num);
                }

                dec_pcm_num = length;
            }
#endif // AUDIO_TWS_MODE
        }
#endif // ((TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE) && (TLKALG_ASRC_16TO48_16BIT_ENABLE || AUDIO_HD_HAC_EN) && (MCU_CORE_TYPE == MCU_CORE_TL751X))

#if (TLKSTK_BT_TPS_ENABLE && (LE_AUDIO_BT_MUSIC_MIX_ENABLE || LE_AUDIO_BT_VOICE_MIX_ENABLE))
        if (!(tlkmdi_tpsll_audio_is_busy()
#if TLKALG_ANC_ENABLE
              || tlkmdi_anc_ll_audio_is_busy()
#endif
                  )) {
            if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
                if (bt_voice_cfg.sync_init == 1 && bt_voice_cfg.codec_sync_init == 0) {
                    bt_voice_cfg.codec_sync_init = 1;
#if TLKADU_MIDBUF_ENABLE
                    if (tlkaud_codec_task_if_can_start()) {
                        //tlkmdi_midbuf_sync_spk(240);
                    } else {
#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
                        gpio_write(GPIO_PA1, 0);
                        gpio_write(GPIO_PA1, 1);
                        gpio_write(GPIO_PA1, 0);
#endif

                        tlkdrv_codec_sync_play_samples(200); //240*3 + 25
                    }
#else
                    tlkdrv_codec_sync_play_samples(240);
#endif
                }

                if (bt_voice_cfg.codec_sync_init) {
                    mix_ll_audio_stereo(ps0, ps1, dec_pcm_num);
                }
            } else {
                mix_ll_audio_stereo(ps0, ps1, dec_pcm_num);
            }
        }
#endif

        pcm_num = dec_pcm_num;
        //uint8_t ret = tlkdrv_codec_get_sin_data(pcm_tmp, BT_VOICE_PCM_SAMPLES);

#if 0
        static signed short sin_16k_mono_sbc[]__attribute__((aligned(4))) = {
                0, 12539, 23169, 30272, 32767, 30272, 23169, 12539, 4, -12539, -23169, -30272, -32767, -30272, -23169, -12539};
        static int count = 0;
        for(int i = 0; i < 120; i++) {
            pcm_tmp[i] = sin_16k_mono_sbc[count++];
            count %= 16;
        }
#endif

        while (pcm_num) {
            samples_num = pcm_num >= PCM_BLOCK_SIZE ? PCM_BLOCK_SIZE : pcm_num;
            pcm_num -= samples_num;
            if (samples_num <= 0) {
                return;
            }

            if (tone_is_playing()
#if TLKMW_INTERPHONE_EN
                && !tlkmdi_interphone_is_busy()
#endif
            ) {
#if AUDIO_PATH_24BITS_EN
                tmemset(g_tone_buff, 0, sizeof(g_tone_buff));
                tone_get_sample(g_tone_buff, samples_num * sizeof(tone_int), 48000);

                if (tlkmdi_audio_get_tone_mix_mode()) {
                    g_tone_buff_ptr = app_mix_tone(ps0, ps1, g_tone_buff, samples_num);

                    // TWS MODE
                    if (audio_flag_get(FLG_BTC_TWS_EN)) {
                        for (int k = 0; k < samples_num; k++) {
                            pcm_stereo[k] = (int32_t)ps0[k] << 8;
                        }
                    } else {
                        for (int k = 0; k < samples_num; k++) {
                            pcm_stereo[2 * k]     = (int32_t)ps0[k] << 8;
                            pcm_stereo[2 * k + 1] = (int32_t)ps1[k] << 8;
                        }
                    }
                } else {
                    // TWS MODE
                    if (audio_flag_get(FLG_BTC_TWS_EN)) {
                        for (int k = 0; k < samples_num; k++) {
                            int32_t left_tone = (int32_t)g_tone_buff[k] << 8;
                            pcm_stereo[k]     = left_tone;
                        }
                    } else {
                        for (int k = 0; k < samples_num; k++) {
                            int32_t left_tone  = (int32_t)g_tone_buff[2 * k] << 8;
                            int32_t right_tone = (int32_t)g_tone_buff[2 * k + 1] << 8;

                            pcm_stereo[2 * k]     = left_tone;
                            pcm_stereo[2 * k + 1] = right_tone;
                        }
                    }
                }
#else
                for (i = 0; i < samples_num; i++) {
                    if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
                        pcm[i] = (uint16_t)ps0[i] | (ps0[i] << 16);
                    } else {
                        pcm[i] = (uint16_t)ps0[i] | (ps1[i] << 16);
                    }
                }

                if (!tlkmdi_audio_get_tone_mix_mode()) {
                    tmemset(pcm, 0, sizeof(pcm));
                }
                tone_get_sample((int16_t *)pcm, samples_num * sizeof(tone_int), g_codec_cfg.sample_rate);
#endif // AUDIO_PATH_24BITS_EN
            } else {
                /* tone is not playing */
                if (audio_flag_get(FLG_BTC_TWS_EN)) {
#if AUDIO_PATH_24BITS_EN
                    for (i = 0; i < samples_num; i++) {
#if TLK_CFG_HRA_ENABLE
                        pcm_stereo[2 * i]     = ps0[i] << 8;
                        pcm_stereo[2 * i + 1] = ps1[i] << 8;
#else
                        pcm_stereo[i] = ps0[i] << 8;
#endif
                    }
#else
                    for (i = 0; i < samples_num; i++) {
                        if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
                            pcm[i] = (uint16_t)ps0[i] | (ps0[i] << 16);
                        } else {
                            pcm[i] = (uint16_t)ps0[i] | (ps1[i] << 16);
                        }
                    }
#endif
                } else {
                    /* Headset */
#if AUDIO_PATH_24BITS_EN
                    for (i = 0; i < samples_num; i++) {
                        pcm_stereo[2 * i]     = ps0[i] << 8;
                        pcm_stereo[2 * i + 1] = ps1[i] << 8;
                    }
#else
                    for (i = 0; i < samples_num; i++) {
                        if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
                            pcm[i] = (uint16_t)ps0[i] | (ps0[i] << 16);
                        } else {
                            pcm[i] = (uint16_t)ps0[i] | (ps1[i] << 16);
                        }
                    }
#endif
                }
            }

#if 0
			static int count1 = 0;
			extern signed short  sin_48k_stereo1[];
			int temp = 0;
			for (i = 0; i < samples_num; i++) {
				temp = sin_48k_stereo1[count1*2+1];
				pcm[i] = (temp<<16)+sin_48k_stereo1[count1*2];
				count1 = (count1+1)%48;
			}
#endif

#if (!(TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
#if AUDIO_PATH_24BITS_EN
#if (TLKALG_EQ_ENABLE && !TLKMW_INTERPHONE_EN)
            if (audio_codec_flag_get(CODEC_FLAG_MUSIC)) {
                audio_alg_interface_t *p_audio_alg = audio_alg_get_interface_by_type(ALG_EQ);
#if AUDIO_TWS_MODE
                p_audio_alg->audio_alg_process((uint8_t *)pcm_stereo, (uint8_t *)pcm_stereo, samples_num, ALG_WIDTH_24, ALG_CHANNEL_LEFT);
#else
                p_audio_alg->audio_alg_process((uint8_t *)pcm_stereo, (uint8_t *)pcm_stereo, samples_num, ALG_WIDTH_24, ALG_CHANNEL_STEREO);
#endif
            }
#endif

#if TLK_CFG_HRA_ENABLE
            tlkmdi_hra_fill_mid_spk_buff((uint8_t *)pcm_stereo, samples_num * 4 * 2);
#else
            if (audio_flag_get(FLG_BTC_TWS_EN)) {
                tlkdrv_codec_fillSpkBuff((uint8_t *)pcm_stereo, samples_num * 4);
            } else {
                tlkdrv_codec_fillSpkBuff((uint8_t *)pcm_stereo, samples_num * 4 * 2);
            }

            bt_music_dec_update_sync_enc_buf_num = 0;
#endif
#else
            uint16_t fill_num = samples_num;
#if (TLKALG_ASRC_441TO48_16BIT_ENABLE && !(TLK_STK_BT_TPSLL_ENABLE || TLKSTK_BTTPSLL_TWS_ENABLE))
            if (g_codec_cfg.sample_rate == 44100
#if TLKMW_INTERPHONE_EN
                && tlkmdi_interphone_is_busy()
#endif
            ) {
                audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO48_16BIT);
                fill_num                              = p_audio_alg_if->audio_alg_process((uint8_t *)pcm, (uint8_t *)pcm, samples_num, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
            }
#elif TLKALG_ASRC_441TO16_16BIT_ENABLE
            if (g_codec_cfg.sample_rate == 44100) {
                audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_441TO16_16BIT);
                fill_num                              = p_audio_alg_if->audio_alg_process((uint8_t *)pcm, (uint8_t *)pcm, samples_num, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
                // tlkmdi_a2dp_to_bis_fill_middle_buff((uint8_t *)pcm, fill_num * sizeof(codec_int));
            }
#endif

#if (TLKALG_EQ_ENABLE && !TLKMW_INTERPHONE_EN)
            if (audio_codec_flag_get(CODEC_FLAG_MUSIC)) {
                audio_alg_interface_t *p_audio_alg = audio_alg_get_interface_by_type(ALG_EQ);
                p_audio_alg->audio_alg_process((uint8_t *)pcm, (uint8_t *)pcm, fill_num, ALG_WIDTH_16, ALG_CHANNEL_STEREO);
            }
#endif

#if TLK_CFG_A2DP_TO_BIS_ENABLE
            if (tlkmdi_a2dp_to_bis_is_busy()) {
                tlkmdi_a2dp_to_bis_fill_middle_buff((uint8_t *)pcm, fill_num * sizeof(codec_int));
            } else if (tlkmdi_bt_music_is_busy()) {
                tlkdrv_codec_fillSpkBuff((uint8_t *)pcm, fill_num * sizeof(codec_int));
            }
#elif TLK_CFG_HRA_ENABLE
            tlkmdi_hra_fill_mid_spk_buff((uint8_t *)pcm, fill_num * sizeof(codec_int));
#elif TLKMW_INTERPHONE_EN
            if (bt_voice_master_is_interphone()) {
                tlkmdi_interphone_fill_i2stx_buff((adc_int *)pcm, fill_num);
            }
            if (tlkmdi_interphone_is_busy()) {
                tlkmdi_interphone_fill_BtDownBuff((uint8_t *)pcm, fill_num * sizeof(codec_int));
            } else {
                tlkdrv_codec_fillSpkBuff((uint8_t *)pcm, fill_num * sizeof(codec_int));
            }
#elif (TLKMW_RECORDING_CARD_EN && TLKBTP_CFG_HFPAG_ENABLE)
            extern bool tlkmdi_record_fill_spk_data_to_uac(int16_t * pData, uint16_t samples);
            tlkmdi_record_fill_spk_data_to_uac((int16_t *)pcm, fill_num * 2);
#else
// music sbc 512, voice 120*2*2
#if TLK_PCM_DATA_WR_EN
            if (bt_audio_get_spk_data_cb) {
                bt_audio_get_spk_data_cb((int16_t *)pcm, fill_num * 2);
            }
#endif
            tlkdrv_codec_fillSpkBuff((uint8_t *)pcm, fill_num * sizeof(codec_int));
#endif
#endif
#else
#if (AUDIO_PATH_24BITS_EN)
            tlkmdi_u2h_voice_fillSpkData((uint8_t *)pcm_stereo, samples_num * sizeof(codec_int));
#else
            tlkmdi_u2h_voice_fillSpkData((uint8_t *)pcm, samples_num * sizeof(codec_int));
#endif // #if (AUDIO_PATH_24BITS_EN)
#endif

            ps0 += samples_num;
            ps1 += samples_num;
        }

#ifdef CODEC_BUF_DEBUG
        num_of_speaker_samples1 = tlkdrv_codec_get_spk_avail_samples();
        spk_buf_idle_size1      = tlkdrv_codec_get_spk_buf_idle_size();
        uint32_t cur_tick       = clock_time();
        uint32_t diff_tick_us   = 0;

        if (g_last_tick) {
            diff_tick_us = (cur_tick - g_last_tick) / 24;
        }
        g_last_tick = cur_tick;

        g_audio_debug_cnt++;
        if (g_audio_debug_cnt == 5) {
            g_audio_debug_cnt = 0;
        }

        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "buf: %d/%d, samples:%d/%d, cnt:%d, tdiff:%d, tick:%x", spk_buf_idle_size0, spk_buf_idle_size1, num_of_speaker_samples0,
                     num_of_speaker_samples1, g_audio_debug_cnt, diff_tick_us, cur_tick);
#endif
    }

    g_codec_fifo_size = tlkdrv_codec_get_spk_avail_samples();

#ifdef DUMP_AUDIO_TASK_STACK_USAGE
    g_bt_audio_stack_usage = tlkos_task_getStackWaterMark(sTlkSysTaskList[TLKSYS_TASKID_AUDIO].taskHandle);
#endif
}

/**
 * @brief  get music volume step
 * @param[in]  is_ios: is ios system
 * @param[in]  percent: volume percent
 * @returns volume step
 */
uint8_t bt_audio_get_music_vol_step(uint8_t is_ios, int16_t percent)
{
    uint8_t i;

    int16_t *tab      = is_ios ? bt_music_vol_table_ios : bt_music_vol_table_android;
    int      max_step = is_ios ? 17 : 16;

    for (i = 0; i < max_step - 1; i++) {
        if (percent == tab[i]) {
            return i;
        } else if (percent > tab[i] && percent <= tab[i + 1]) {
            return i + 1;
        }
    }

    /* default use 1/4 volume. */
    return max_step / 4;
}

/**
 * @brief  set voice volume percent
 * @param[in]  step - volume step
 * @returns none
 */
void bt_audio_set_voice_vol_percent(uint8_t step)
{
    uint8_t tmp = step;

    if (tmp > 15) {
        tmp = 15;
        //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "Err:voice_vol_percent_set %d", step);
    }

    g_codec_cfg.voice_vol_percent = bt_voice_vol_table[(tmp & 0x0f)];

    if (tmp == 15) {
        g_codec_cfg.voice_max_vol = 1;
    } else {
        g_codec_cfg.voice_max_vol = 0;
    }

    /*
    tlkapi_trace(BT_AUDIO_DBG_FLAG,
                 BT_AUDIO_DBG_SIGN,
                 "@bt_audio_set_voice_vol_percent %d %d %d %d",
                 step,
                 g_codec_cfg.voice_max_vol,
                 g_codec_cfg.music_vol_percent,
                 g_codec_cfg.voice_vol_percent);
    */
}

void bt_audio_set_voice_vol_percent_by_handle(uint16_t handle)
{
    uint8_t step = 0;
    tlkmdi_audio_btif_getVolumeStep(handle, false, &step, NULL);
    tlkapi_printf(1, "## bt_audio_set_voice_vol_percent_by_handle step:%d", step);
    bt_audio_set_voice_vol_percent(step);
}

/**
 * @brief  set music volume percent
 * @param[in]  is_ios - is ios system
 * @param[in]  step - volume step
 * @returns none
 */
void bt_audio_set_music_vol_percent(uint8_t is_ios, uint8_t step)
{
    if (is_ios && step < 17) {
        g_codec_cfg.music_vol_percent = bt_music_vol_table_ios[step];

        if (step == 16) {
            g_codec_cfg.music_max_vol = 1;
        } else {
            g_codec_cfg.music_max_vol = 0;
        }
    } else if (!is_ios && step < 16) {
        g_codec_cfg.music_vol_percent = bt_music_vol_table_android[step];

        if (step == 15) {
            g_codec_cfg.music_max_vol = 1;
        } else {
            g_codec_cfg.music_max_vol = 0;
        }
    }

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "@bt_audio_set_music_vol_percent, step:%d, ios:%d, m_vol:%d, v_vol:%d", step, is_ios, g_codec_cfg.music_vol_percent,
                 g_codec_cfg.voice_vol_percent);
}

/**
 * @brief   Set music volume percentage by handle.
 * @param[in]   handle  - Connection handle.
 * @return      None.
 */
void bt_audio_set_music_vol_percent_by_handle(uint16_t handle)
{
    uint8_t isIos = 0;
    uint8_t step  = 0;
    tlkmdi_audio_btif_getVolumeStep(handle, true, &step, &isIos);
    bt_audio_set_music_vol_percent(isIos, step);
}

/**
 * @brief   Get AVRCP volume function value.
 * @param[in]   step  - Volume step value.
 * @return      AVRCP volume value, or TLK_EFAIL if step is greater than 16.
 */
uint8_t bt_audio_get_avrcp_vol_func(uint8_t step)
{
    if (step >= 16) {
        return TLK_EFAIL;
    }
    return bt_music_avrcp_absVol_table[step];
}

/**
 * @brief  get music and voice max volume flag
 * @param[in]  none
 * @returns music and voice max volume flag
 */
uint8_t bt_audio_get_max_vol_func(void)
{
    return (g_codec_cfg.music_max_vol | (g_codec_cfg.voice_max_vol << 4));
}

/**
 * @brief  get voice volume percent
 * @param[in]  none
 * @returns volume percent
 */
#if (TLKBTP_CFG_HFP_ENABLE)
int16_t bt_audio_get_voice_vol_percent(void)
{
    return g_bt_voice_vol;
}
#endif
/**
 * @brief  get music volume percent
 * @param[in]  none
 * @returns volume percent
 */
int16_t bt_audio_get_music_vol_percent(void)
{
    return g_bt_music_vol;
}

#endif

/**
* @brief  Fill mute data to given buffer.
* @param[in]  pd0 - start address 0
* @param[in]  pd1 - start address 1
* @returns one_frame_samples: mute data length
*/
audio_ram_code uint16_t bt_audio_get_mute_data(int16_t *pd0, int16_t *pd1)
{
#define TWS_PLAY_BLOCK_SIZE 128

    uint16_t one_frame_samples;
    uint16_t i;

    if (audio_codec_flag_get(CODEC_FLAG_MUSIC)) {
        one_frame_samples = (SEPID_AAC == tlkmw_audio_btif_music_get_avdtp_seid_type() ? 1024 : 128);
    }
#if (TLKBTP_CFG_HFP_ENABLE) // xiaogang will check later
    else if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
        one_frame_samples = 120; //(HFP_CODEC_ID_MSBC == btif_get_hfp_codec() ? 120 : 120);
    }
#endif
    else {
        one_frame_samples = TWS_PLAY_BLOCK_SIZE;
    }

    if (pd0 != NULL) {
        for (i = 0; i < one_frame_samples; i++) {
            *pd0++ = 0;
        }
    }

    if (pd1 != NULL) {
        for (i = 0; i < one_frame_samples; i++) {
            *pd1++ = 0;
        }
    }

    return one_frame_samples;
}

/**
 * @brief  control voice volume
 * @param[in]  p - voice data
 * @param[in]  sample - voice data sample number
 * @returns none
 */
#if (TLKBTP_CFG_HFP_ENABLE) // xiaogang will check later
audio_ram_code void bt_audio_control_voice_volume(int16_t *p, uint16_t sample)
{
    uint16_t i;

#if BT_SCO_RMV_FADE_OUT_EN
    static volatile int16_t s_bt_voice_vol_fade_out = 0;

    if (bt_ll_sco_is_removing_get()) {
        for (i = 0; i < sample; i++) {
            if (s_bt_voice_vol_fade_out > 0) {
                s_bt_voice_vol_fade_out--;
            }
            p[i] = p[i] * s_bt_voice_vol_fade_out / 8192;
        }

        return;
    } else {
        s_bt_voice_vol_fade_out = g_bt_voice_vol;
    }
#endif

    for (i = 0; i < sample; i++) {
        p[i] = p[i] * g_bt_voice_vol / 8192;

        if (g_bt_voice_vol < g_codec_cfg.voice_vol_percent) {
            g_bt_voice_vol++;
        } else if (g_bt_voice_vol > g_codec_cfg.voice_vol_percent) {
            g_bt_voice_vol--;
        }
    }
}

#if AUDIO_TWS_MODE

int g_tws_spk_ppm_value;

/**
 * @brief  set tws speaker ppm value
 * @param[in]  ppm_spk_value - ppm value
 * @returns none
 */
void app_tws_spk_set_ppm_value(int ppm_spk_value)
{
    //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "=== new ppm: %d", ppm_spk_value);

    audio_alg_interface_t *p_lea_spk_alg_if = audio_alg_get_interface_by_type(ALG_PPM_TWS_SPK);
    tlkmdi_audio_enterCritical();
    p_lea_spk_alg_if->audio_alg_param_set(0, &ppm_spk_value);
    g_tws_spk_ppm_value = ppm_spk_value;
    tlkmdi_audio_leaveCritical();

    if (g_app_tws_spk_ppm_ready == 0) {
        g_app_tws_spk_ppm_ready = 1;
    }
}

/**
 * @brief  process tws speaker ppm
 * @param[in]  ps      - source data pointer
 * @param[in]  pd      - destination data pointer
 * @param[in]  len     - data length
 * @param[in]  channel - channel number
 * @returns processed data length
 */
uint16_t app_tws_spk_ppm_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t channel)
{
    uint16_t       ret   = 0;
    static u32     count = 0;
    static int16_t diff  = 0;

    audio_alg_interface_t *p_lea_spk_alg_if = audio_alg_get_interface_by_type(ALG_PPM_TWS_SPK);
    tlkmdi_audio_enterCritical();
    ret = p_lea_spk_alg_if->audio_alg_process(ps, pd, len, 0, channel);
    tlkmdi_audio_leaveCritical();
    if (ret != len) {
        count++;
        diff += (len - ret);
        if (!(count % 0x7ff)) {
            diff = 0;
        }
    }
    return ret;
}

/**
 * @brief  initialize tws speaker ppm
 * @param[in]  channel - channel number
 * @returns none
 */
void app_tws_spk_ppm_init(uint8_t channel)
{
    audio_alg_interface_t *p_tws_spk_alg_if;
    p_tws_spk_alg_if      = audio_alg_get_interface_by_type(ALG_PPM_TWS_SPK);
    uint16_t ppm_spk_size = p_tws_spk_alg_if->audio_alg_get_size(channel);

    s_alg_tws_spk_ppm_buff = (uint8_t *)tlkalg_malloc_func(ppm_spk_size);
    if (s_alg_tws_spk_ppm_buff == NULL) {
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "s_alg_tws_spk_ppm_buff alloc failed");
        return;
    }
    p_tws_spk_alg_if->audio_alg_init(s_alg_tws_spk_ppm_buff, channel);
    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "app_tws_spk_ppm mem_size: %d", ppm_spk_size);
}

/**
 * @brief  deinitialize tws speaker ppm
 * @param[in]  none
 * @returns none
 */
void app_tws_spk_ppm_deinit(void)
{
    audio_alg_interface_t *p_tws_spk_alg_if;
    p_tws_spk_alg_if = audio_alg_get_interface_by_type(ALG_PPM_TWS_SPK);
    if (s_alg_tws_spk_ppm_buff != NULL) {
        tlkalg_free_func(s_alg_tws_spk_ppm_buff);
        p_tws_spk_alg_if->audio_alg_deinit();
        s_alg_tws_spk_ppm_buff = NULL;
    }
    g_app_tws_spk_ppm_ready = 0;
}

#endif // AUDIO_TWS_MODE

/**
 * @brief  printf voice and music volume
 * @param[in]  none
 * @returns none
 */
void bt_audio_printf_vol(void)
{
#if (TLKBTP_CFG_HFP_ENABLE)
    tlkapi_printf(1, "## g_bt_voice_vol:%d,voice_vol_percent:%d", g_bt_voice_vol, g_codec_cfg.voice_vol_percent);
#endif
    tlkapi_printf(1, "## g_bt_music_vol:%d, music_vol_percent:%d", g_bt_music_vol, g_codec_cfg.music_vol_percent);
}
#endif
