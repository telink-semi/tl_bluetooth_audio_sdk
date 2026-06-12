/********************************************************************************************************
 * @file    le_audio_common.c
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
#include <string.h>
#include <sys/queue.h>

#include "common/types.h"

#include "tlkalg/audio/audio_alg_interface.h"
#include "tlkalg/audio/lc3/tlkalg_lc3_interface.h"

#include "tlkmw/audio/tlkmw_audio.h"

#include "tl_common.h"
#include "tlkapi/tlkapi.h"

#include "stack/ble/ble.h"

#include "drivers.h"

#include "codec/lea_codec.h"

#include "le_audio_common.h"

#if (TLK_MW_LE_AUDIO_LOW_LATENCY_ENABLE)

#define LEA_FIX_TIMESTAMP_ERROR      1

#define LE_AUDIO_ISO_HANDLE_INVALID  0xFFFF

#define LE_AUDIO_MAX_FRAME_SAMPLE    480 // now support 48kHz 10ms, 48 * 10 = 480Byte

#define LE_AUDIO_MAX_PER_CODEC_FRAME 155 // LC3_48_6

#define LE_AUDIO_CONFIG_MAX_COUNT    2

struct lea_config_info
{
    uint8_t           enable;
    uint32_t          all_location;
    uint16_t          sample_interval; // support 10ms and 7.5ms, unit is 1us.
    uint16_t          sample_count;    // not support 44.1kHz.
    uint8_t          *lc3_alg_buffer;  //must
    struct lea_config config[LE_AUDIO_CONFIG_MAX_COUNT];
};

static struct lea_config_info s_le_audio_input;

static struct lea_config_info s_le_audio_output;

#if TLKALG_PPM_SPK_ENABLE
lea_ppm_ctl_t   lea_ppm_ctl;
lea_ppm_state_t lea_ppm_state;
#endif

#if TLKALG_PPM_SPK_ENABLE
void tlkalg_lea_ppm_init(int samplerate)
{
    // tlk_printf("tlkalg_lea_ppm_init samplerate %d", samplerate);
    memset(&lea_ppm_ctl, 0, sizeof(lea_ppm_ctl));
    lea_ppm_state.samplerate      = samplerate;
    lea_ppm_state.sample_nominal  = lea_ppm_state.samplerate / 1000 * LEA_PPM_CALC_INTERVAL_MS;
    lea_ppm_state.current_ppm     = 0;
    lea_ppm_state.last_ppm        = 0;
    lea_ppm_state.tick_remainder  = 0;
    lea_ppm_state.asrc_diff       = 0;
    lea_ppm_state.tick_per_sample = SYSTEM_TIMER_TICK_1US * 1000 / (lea_ppm_state.samplerate / 1000);
    lea_ppm_state.ppm_remainder   = 0;
    lea_ppm_state.update_flag     = false;
}

//diff_tick = local_tick - remote_tick
void tlkalg_lea_ppm_calc(int32_t diff_tick)
{
    int32_t diff_tick_total = diff_tick + lea_ppm_state.tick_remainder;
    if (lea_ppm_state.asrc_diff != 0) {
        diff_tick_total += (lea_ppm_state.asrc_diff * lea_ppm_state.tick_per_sample);
        lea_ppm_state.asrc_diff = 0;
    }
    int32_t sample_diff          = diff_tick_total / lea_ppm_state.tick_per_sample;
    lea_ppm_state.tick_remainder = diff_tick_total % lea_ppm_state.tick_per_sample;

    int ppm_million_total       = sample_diff * 1000000 + lea_ppm_state.ppm_remainder;
    int ppm_val                 = ppm_million_total / lea_ppm_state.sample_nominal;
    lea_ppm_state.ppm_remainder = ppm_million_total % lea_ppm_state.sample_nominal;

    lea_ppm_state.current_ppm = (ppm_val * LEA_PPM_SMOOTH_ALPHA + lea_ppm_state.last_ppm * (10 - LEA_PPM_SMOOTH_ALPHA)) / 10;
    lea_ppm_state.current_ppm = lea_ppm_state.current_ppm * 5 / 4;

    if (lea_ppm_state.current_ppm > LEA_PPM_MAX) {
        lea_ppm_state.current_ppm = LEA_PPM_MAX;
    } else if (lea_ppm_state.current_ppm < -LEA_PPM_MAX) {
        lea_ppm_state.current_ppm = -LEA_PPM_MAX;
    }

    // tlk_printf("ppm val cur last %d %d %d", ppm_val, lea_ppm_state.current_ppm, lea_ppm_state.last_ppm);

    if (lea_ppm_state.last_ppm != lea_ppm_state.current_ppm) {
        lea_ppm_state.update_flag = true;
        lea_ppm_state.last_ppm    = lea_ppm_state.current_ppm;
    }
}
#endif

/**
 * @brief       Reset input configuration cache and mark ISO handles invalid.
 * @return      none.
 */
void lea_input_config_initial(void)
{
    memset(&s_le_audio_input, 0, sizeof(struct lea_config_info));
    s_le_audio_input.config[0].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
    s_le_audio_input.config[1].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
}

/**
 * @brief       Reset output configuration cache and mark ISO handles invalid.
 * @return      none.
 */
void lea_output_config_initial(void)
{
    memset(&s_le_audio_output, 0, sizeof(struct lea_config_info));
    s_le_audio_output.config[0].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
    s_le_audio_output.config[1].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
}

/**
 * @brief       Calculate channel count encoded by the provided location bitmap.
 * @param[in]   location    - bitmap of audio locations.
 * @return      2 when stereo bitmap is set, otherwise 1.
 */
static int lea_calc_location_valid_count(uint32_t location)
{
    return location == 3 ? 2 : 1;
}

/******************LE Audio common configure LC3 encode and decode API***********************/
/*
 * Unicast Client maybe can configure LC3 after configure codec operation.
 * 		if connected TWS(check CSIS Size == 2), must set all TWS LC3 configuration.
 *
 * 	Unicast Server should configure LC3 after all ASE(sink or source) enter enable mode.
 */
struct lea_lc3_config_param
{
    struct lea_config_info *config_info;
    audio_alg_interface_t  *alg_lc3;
};

/**
 * @brief       Free LC3 algorithm buffer and deinit interface.
 * @param[in]   p_lc3_config_param   - LC3 configuration context.
 * @return      0 always.
 */
static int lea_release_lc3_module(struct lea_lc3_config_param *p_lc3_config_param)
{
    uint8_t *p_lc3_alg_buffer = p_lc3_config_param->config_info->lc3_alg_buffer;

    if (p_lc3_alg_buffer != NULL) {
        p_lc3_config_param->alg_lc3->audio_alg_deinit();
        tlkalg_free_func(p_lc3_alg_buffer);
        tlk_printf("LC3 free buffer is %x", p_lc3_alg_buffer);
        p_lc3_config_param->config_info->lc3_alg_buffer = NULL;
    }
    return 0;
}

/**
 * @brief       Allocate and initialize LC3 algorithm instance if needed.
 * @param[in]   p_lc3_config_param   - LC3 configuration context.
 * @return      0 on success, negative value when already configured or alloc fails.
 */
static int lea_config_lc3_module(struct lea_lc3_config_param *p_lc3_config_param)
{
    uint8_t *p_lc3_alg_buffer = p_lc3_config_param->config_info->lc3_alg_buffer;

    if (p_lc3_alg_buffer != NULL) {
        return -1;
    }

    int lc3_channel_count = lea_calc_location_valid_count(p_lc3_config_param->config_info->all_location);

    p_lc3_alg_buffer = (uint8_t *)tlkalg_malloc_func(p_lc3_config_param->alg_lc3->audio_alg_get_size(lc3_channel_count));

    tlk_printf("lc3 malloc is %x %x", p_lc3_alg_buffer, p_lc3_config_param->alg_lc3->audio_alg_get_size(lc3_channel_count));
    if (p_lc3_alg_buffer == NULL) {
        return -2;
    }

    p_lc3_config_param->alg_lc3->audio_alg_init(p_lc3_alg_buffer, lc3_channel_count);
    p_lc3_config_param->config_info->lc3_alg_buffer = p_lc3_alg_buffer;
    return 0;
}

/**
 * @brief       Configure LC3 encoder workspace for all input locations.
 * @param[in]   location    - bitmap of LE Audio locations.
 * @return      0 on success, negative value otherwise.
 */
int lea_set_input_all_location(uint32_t location)
{
    s_le_audio_input.all_location                = location;
    struct lea_lc3_config_param lc3_config_param = {
        .config_info = &s_le_audio_input,
        .alg_lc3     = audio_alg_get_interface_by_type(ALG_LC3_ENC),
    };
    return lea_config_lc3_module(&lc3_config_param);
}

/**
 * @brief       Release LC3 encoder workspace allocated for input.
 * @return      0 on success, negative value otherwise.
 */
int lea_release_input_location(void)
{
    struct lea_lc3_config_param lc3_config_param = {
        .config_info = &s_le_audio_input,
        .alg_lc3     = audio_alg_get_interface_by_type(ALG_LC3_ENC),
    };

    return lea_release_lc3_module(&lc3_config_param);
}

/**
 * @brief       Configure LC3 decoder workspace for all output locations.
 * @param[in]   location    - bitmap of LE Audio locations.
 * @return      0 on success, negative value otherwise.
 */
int lea_set_output_all_location(uint32_t location)
{
    s_le_audio_output.all_location               = location;
    struct lea_lc3_config_param lc3_config_param = {
        .config_info = &s_le_audio_output,
        .alg_lc3     = audio_alg_get_interface_by_type(ALG_LC3_DEC),
    };
    return lea_config_lc3_module(&lc3_config_param);
}

/**
 * @brief       Release LC3 decoder workspace allocated for output.
 * @return      0 on success, negative value otherwise.
 */
int lea_release_output_location(void)
{
    struct lea_lc3_config_param lc3_config_param = {
        .config_info = &s_le_audio_output,
        .alg_lc3     = audio_alg_get_interface_by_type(ALG_LC3_DEC),
    };

    return lea_release_lc3_module(&lc3_config_param);
}

/**
 * @brief       Program LC3 encoder/decoder channel parameters for a specific ASE.
 * @param[in]   location    - aggregate location bitmap across ASEs.
 * @param[in]   p_config    - ASE configuration.
 * @param[in]   p_alg_lc3   - LC3 algorithm interface.
 * @return      none.
 */
static void lea_set_lc3_start(uint32_t location, const struct lea_config *p_config, audio_alg_interface_t *p_alg_lc3)
{
    struct lc3_initial_param_bap lc3Init = {
        .channelIndex  = 0,
        .samplingFreq  = p_config->samplingFrequency,
        .perCodecFrame = p_config->frameOctets,
        .frameDuration = p_config->frameDuration,
    };

    if (p_config->location == LEA_LOCATION_FRONT_LEFT) {
        p_alg_lc3->audio_alg_param_set(LC3_TYPE_SET_PARAM_BAP, &lc3Init);
    } else if (p_config->location == LEA_LOCATION_FRONT_RIGHT) {
        if (location == (LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT)) {
            lc3Init.channelIndex = 1;
        }
        p_alg_lc3->audio_alg_param_set(LC3_TYPE_SET_PARAM_BAP, &lc3Init);
    } else {
        p_alg_lc3->audio_alg_param_set(LC3_TYPE_SET_PARAM_BAP, &lc3Init);
        lc3Init.channelIndex = 1;
        p_alg_lc3->audio_alg_param_set(LC3_TYPE_SET_PARAM_BAP, &lc3Init);
    }
}

/**
 * @brief       Cache a new ASE configuration, ensuring frequency/duration consistency.
 * @param[in]   p_config_info   - configuration group (input/output).
 * @param[in]   p_config        - new ASE configuration.
 * @return      0 on success, negative value when conflicts detected.
 */
static int lea_set_audio_config(struct lea_config_info *p_config_info, const struct lea_config *p_config)
{
    int free_config_index = 0xFF;
    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        if (p_config_info->config[i].iso_handle == LE_AUDIO_ISO_HANDLE_INVALID && free_config_index == 0xFF) {
            free_config_index = i;
        }

        if (p_config_info->config[i].iso_handle == p_config->iso_handle) {
            return -1; //this iso handle had configure
        }

        if (p_config_info->config[i].iso_handle != LE_AUDIO_ISO_HANDLE_INVALID) {
            if (p_config_info->config[i].samplingFrequency != p_config->samplingFrequency || p_config_info->config[i].frameDuration != p_config->frameDuration) {
                return -2; // must same frequency and duration.
            }
        }
    }

    if (free_config_index == 0xFF) {
        return -3;
    }

    memcpy(&p_config_info->config[free_config_index], p_config, sizeof(struct lea_config));
    return 0;
}

/**
 * @brief       Mark cached ASE configuration as invalid for an ISO handle.
 * @param[in]   p_config_info   - configuration group (input/output).
 * @param[in]   iso_handle      - ISO handle to release.
 * @return      none.
 */
static void lea_release_audio_config(struct lea_config_info *p_config_info, uint16_t iso_handle)
{
    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        if (p_config_info->config[i].iso_handle == iso_handle) {
            p_config_info->config[i].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
            break;
        }
    }
}

/******************LE Audio common configuration LC3 encode and decode API***********************/

/******************LE Audio Input *********************************/
static uint32_t s_le_audio_get_input_time = 0;
static uint32_t s_le_audio_cig_tick       = 0;

static uint32_t lea_calculate_input_time(uint32_t cig_ref_tick, uint32_t sample_interval_us)
{
    uint32_t sample_interval_tick = sample_interval_us * SYSTEM_TIMER_TICK_1US;

    cig_ref_tick -= SYSTEM_TIMER_TICK_1US * 3000;

    if ((int32_t)(stimer_get_tick() - cig_ref_tick) > 0) {
        cig_ref_tick += sample_interval_tick;
    }
    return cig_ref_tick;
}

static inline uint32_t systimer_us_until(uint32_t future_tick)
{
    int32_t diff_tick = (int32_t)(future_tick - stimer_get_tick());

    if (diff_tick <= 0) {
        return 0;
    }

    return diff_tick / SYSTEM_TIMER_TICK_1US;
}

/**
 * @brief       Enable audio input path and reset acquisition timer.
 * @param[in]   get_time    - reference timestamp (optional).
 * @return      none.
 */
void lea_open_input(uint32_t get_time)
{
    s_le_audio_input.enable = true;

    s_le_audio_get_input_time = get_time; //lea_calculate_input_time(get_time | 1, s_le_audio_input.sample_interval);
    s_le_audio_cig_tick       = get_time;
}

/**
 * @brief       Disable audio input path and clear ASE state.
 * @return      none.
 */
void lea_close_input(void)
{
    s_le_audio_input.enable   = false;
    s_le_audio_get_input_time = 0;

    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        s_le_audio_input.config[i].iso_handle = LE_AUDIO_ISO_HANDLE_INVALID;
    }
}

/**
 * @brief       Translate LC3 selectors to PCM sample count per frame.
 * @param[in]   frequency   - LC3 sampling frequency selector.
 * @param[in]   duration    - LC3 frame duration selector.
 * @return      Number of PCM samples per channel per frame.
 */
static uint32_t lea_calc_sample_count(uint8_t frequency, uint8_t duration)
{
    if (duration == LEA_SELECT_FRAME_DURATION_10) {
        if (frequency == LEA_SELECT_SAMPLING_FREQ_8000_HZ) {
            return 8 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_16000_HZ) {
            return 16 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_24000_HZ) {
            return 24 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_32000_HZ) {
            return 32 * 10;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_48000_HZ) {
            return 48 * 10;
        }
    } else if (duration == LEA_SELECT_FRAME_DURATION_7_5) {
        if (frequency == LEA_SELECT_SAMPLING_FREQ_8000_HZ) {
            return 8 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_16000_HZ) {
            return 16 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_24000_HZ) {
            return 24 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_32000_HZ) {
            return 32 * 7.5;
        } else if (frequency == LEA_SELECT_SAMPLING_FREQ_48000_HZ) {
            return 48 * 7.5;
        }
    }
    return 0;
}

/**
 * @brief       Program input sampling count and interval based on BAP config.
 * @param[in]   frequency   - LC3 sampling frequency selector.
 * @param[in]   duration    - LC3 frame duration selector.
 * @return      none.
 */
void lea_set_input_sample_config_bap(uint8_t frequency, uint8_t duration)
{
#if (!TLK_CFG_TEMP_DRAM_OPTM)
    s_le_audio_input.sample_count = lea_calc_sample_count(frequency, duration);
#endif
    s_le_audio_input.sample_interval = duration == LEA_SELECT_FRAME_DURATION_7_5 ? 7500 : 10000;
    s_le_audio_get_input_time        = lea_calculate_input_time(s_le_audio_get_input_time, s_le_audio_input.sample_interval);
    uint32_t us_left                 = systimer_us_until(s_le_audio_get_input_time);
    uint32_t sample_keep             = (uint32_t)((float)(12 * 1000 - us_left) * 0.024);

    tlk_printf("sample_keep %d", sample_keep);

    tlkdrv_codec_sync_mic_samples(sample_keep);
}

/**
 * @brief       Store ASE specific input configuration and start LC3 encoder.
 * @param[in]   p_config    - pointer to ASE configuration.
 * @return      none.
 */
void lea_set_input_config(const struct lea_config *p_config)
{
    lea_set_audio_config(&s_le_audio_input, p_config);
    lea_set_lc3_start(s_le_audio_input.all_location, p_config, audio_alg_get_interface_by_type(ALG_LC3_ENC));
}

/**
 * @brief       Remove stored input configuration for specific ISO handle.
 * @param[in]   iso_handle  - ISO connection handle.
 * @return      none.
 */
void lea_release_input_config(uint16_t iso_handle)
{
    lea_release_audio_config(&s_le_audio_input, iso_handle);
}

static inline _Bool clock_time_exceed_future(uint32_t ref, uint32_t us)
{
    uint32_t target = ref + us * SYSTEM_TIMER_TICK_1US;
    return ((int32_t)(stimer_get_tick() - target) >= 0);
}

void lea_input_alg_process(uint8_t *ps, uint8_t *pd, uint16_t len)
{
    (void)ps, (void)pd, (void)len;

#if (TLKALG_NN_NS_ENABLE || TLKALG_AGC_ENABLE || TLKALG_EQ_ENABLE || TLKALG_PPM_SPK_ENABLE)
    uint8_t *psrc = ps;
#endif

#if (TLKALG_PPM_SPK_ENABLE)
    audio_alg_interface_t *p_alg_eq = audio_alg_get_interface_by_type(ALG_PPM_SPK);

    if (lea_ppm_state.update_flag) {
        lea_ppm_state.update_flag = false;
        int ppm_val               = lea_ppm_state.current_ppm;
        p_alg_eq->audio_alg_param_set(0, &ppm_val);
    }

    short ppm_out_buff[481];
    int   out_len = p_alg_eq->audio_alg_process((uint8_t *)psrc, (uint8_t *)ppm_out_buff, len, ALG_WIDTH_16, ALG_CHANNEL_LEFT);
    for (int j = 0; j < out_len; j++) {
        lea_ppm_ctl.alg_data_buffer[lea_ppm_ctl.alg_wptr++] = ppm_out_buff[j];
        lea_ppm_ctl.alg_wptr %= PPM_ALG_BUFF_LEN;
    }

    int diff = out_len - len;
    if (diff != 0) {
        // tlk_printf("out in len %d %d ", out_len, len);
        lea_ppm_ctl.sample_diff = diff;
        lea_ppm_state.asrc_diff += diff;
    }

    short *pdes = (short *)pd;
    len         = s_le_audio_input.sample_count;
    for (int i = 0; i < len; i++) {
        *pdes++ = lea_ppm_ctl.alg_data_buffer[lea_ppm_ctl.alg_rptr++];
        lea_ppm_ctl.alg_rptr %= PPM_ALG_BUFF_LEN;
    }
    psrc = pd;
#endif

#if TLKALG_NN_NS_ENABLE
    audio_alg_interface_t *p_alg_nn = audio_alg_get_interface_by_type(ALG_NN_NS);
    int                    ret_num  = p_alg_nn->audio_alg_process((uint8_t *)psrc, (uint8_t *)pd, len, 0, 0);
    psrc                            = pd;
#endif

#if TLKALG_AGC_ENABLE
    audio_alg_interface_t *p_alg_agc = audio_alg_get_interface_by_type(ALG_AGC);
    p_alg_agc->audio_alg_process((uint8_t *)psrc, (uint8_t *)pd, len, ALG_WIDTH_16, 0);
    psrc = pd;
#endif

#if TLKALG_EQ_ENABLE
    audio_alg_interface_t *p_alg_eq = audio_alg_get_interface_by_type(ALG_EQ);
    p_alg_eq->audio_alg_process((uint8_t *)psrc, (uint8_t *)pd, len, ALG_WIDTH_16, ALG_CHANNEL_LEFT);
#endif
}

/**
 * @brief       Periodic input task: read PCM, encode LC3, send ISO SDUs.
 * @return      none.
 */
void lea_input_main_process(void)
{
    static uint32_t lea_input_calcu_time_count = 0;
    if (!s_le_audio_input.enable || (int32_t)(stimer_get_tick() - s_le_audio_get_input_time) <= 0) {
        return;
    }
    adc_mono_int left_data[LE_AUDIO_MAX_FRAME_SAMPLE], right_data[LE_AUDIO_MAX_FRAME_SAMPLE];

    s_le_audio_get_input_time += s_le_audio_input.sample_interval * SYSTEM_TIMER_TICK_1US;

    uint16_t data_num = s_le_audio_input.sample_count;
#if TLKALG_PPM_SPK_ENABLE
    if (lea_ppm_ctl.sample_diff != 0) {
        data_num                = s_le_audio_input.sample_count - lea_ppm_ctl.sample_diff;
        lea_ppm_ctl.sample_diff = 0;
    }
#endif

    if (!lea_codec_input_get_audio_data(left_data, right_data, data_num)) {
        return;
    }
    audio_alg_interface_t *p_lc3_en_alg = audio_alg_get_interface_by_type(ALG_LC3_ENC);
    uint8_t                encode_data[LE_AUDIO_MAX_PER_CODEC_FRAME * 2]; // left + right;
    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        struct lea_config *p_input_config = &s_le_audio_input.config[i];
        if (p_input_config->iso_handle == LE_AUDIO_ISO_HANDLE_INVALID) {
            continue;
        }
        if (lea_input_calcu_time_count++ == (LEA_PPM_CALC_INTERVAL_MS / 10 - 1)) { //need to update the time every 10 seconds, with 10ms iso interval,
            uint32_t current_cig_tick = ble_host_gap_iso_get_tx_time_stamp(p_input_config->iso_handle);
            s_le_audio_get_input_time = lea_calculate_input_time(current_cig_tick, s_le_audio_input.sample_interval);

            int32_t tick_diff = (current_cig_tick - s_le_audio_cig_tick + 5 * SYSTEM_TIMER_TICK_1MS) % (SYSTEM_TIMER_TICK_1MS * 10) - 5 * SYSTEM_TIMER_TICK_1MS;
            tlk_printf("diff tick mic %d %d %d", clock_time(), tick_diff, tlkdrv_codec_get_mic_avail_samples());

#if TLKALG_PPM_SPK_ENABLE
            tlkalg_lea_ppm_calc(tick_diff);
#endif

            s_le_audio_cig_tick        = current_cig_tick;
            lea_input_calcu_time_count = 0;
        }

        if (p_input_config->location == LEA_LOCATION_FRONT_LEFT) {
            lea_input_alg_process((uint8_t *)left_data, (uint8_t *)left_data, data_num);
        } else if (p_input_config->location == LEA_LOCATION_FRONT_RIGHT) {
            lea_input_alg_process((uint8_t *)right_data, (uint8_t *)right_data, data_num);
        }

        if (p_input_config->location == LEA_LOCATION_FRONT_LEFT) {
            p_lc3_en_alg->audio_alg_process((uint8_t *)left_data, encode_data, s_le_audio_input.sample_count * sizeof(int16_t), 16, 0); //LC3 default 0.
        } else if (p_input_config->location == LEA_LOCATION_FRONT_RIGHT) {
            int lc3_index = 0;
            if (s_le_audio_input.all_location == (LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT)) {
                lc3_index = 1;
            }
            p_lc3_en_alg->audio_alg_process((uint8_t *)right_data, encode_data + p_input_config->frameOctets, s_le_audio_input.sample_count * sizeof(int16_t), 16,
                                            lc3_index); //LC3 default 0.
        } else {
            p_lc3_en_alg->audio_alg_process((uint8_t *)left_data, encode_data, s_le_audio_input.sample_count * sizeof(int16_t), 16, 0);
            p_lc3_en_alg->audio_alg_process((uint8_t *)right_data, encode_data + p_input_config->frameOctets, s_le_audio_input.sample_count * sizeof(int16_t), 16, 1);
        }
    }

    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        struct lea_config *p_input_config = &s_le_audio_input.config[i];

        if (p_input_config->iso_handle == LE_AUDIO_ISO_HANDLE_INVALID) {
            continue;
        }
        if (p_input_config->location == LEA_LOCATION_FRONT_LEFT) {
            ble_host_send_ble_iso_data(p_input_config->iso_handle, 0, encode_data, p_input_config->frameOctets);
        } else if (p_input_config->location == LEA_LOCATION_FRONT_RIGHT) {
            ble_host_send_ble_iso_data(p_input_config->iso_handle, 0, encode_data + p_input_config->frameOctets, p_input_config->frameOctets);
        } else {
            ble_host_send_ble_iso_data(p_input_config->iso_handle, 0, encode_data, 2 * p_input_config->frameOctets);
        }
    }
}

/******************LE Audio Output *********************************/
static void lea_output_receive_sdu_packet(uint16_t conn_handle, uint32_t timestamp, uint16_t iso_data_len, const uint8_t *p_iso_data);

/**
 * @brief       Register ISO sink callback and prepare output path.
 * @return      none.
 */
void lea_open_output(void)
{
    ble_host_iso_data_init(lea_output_receive_sdu_packet);
}

static void lea_release_all_output_audio_pkt(void);

/**
 * @brief       Stop ISO sink and release queued output audio packets.
 * @return      none.
 */
void lea_close_output(void)
{
    ble_host_iso_data_deinit();

    lea_release_all_output_audio_pkt();
    lea_release_output_location();
}

/**
 * @brief       Store ASE specific output configuration and start LC3 decoder.
 * @param[in]   p_config    - pointer to ASE configuration.
 * @return      none.
 */
void lea_set_output_config(const struct lea_config *p_config)
{
    lea_set_audio_config(&s_le_audio_output, p_config);
    lea_set_lc3_start(s_le_audio_output.all_location, p_config, audio_alg_get_interface_by_type(ALG_LC3_DEC));
}

/**
 * @brief       Program output sampling count and interval based on BAP config.
 * @param[in]   frequency   - LC3 sampling frequency selector.
 * @param[in]   duration    - LC3 frame duration selector.
 * @return      none.
 */
void lea_set_output_sample_config_bap(uint8_t frequency, uint8_t duration)
{
    s_le_audio_output.sample_count    = lea_calc_sample_count(frequency, duration);
    s_le_audio_output.sample_interval = duration == LEA_SELECT_FRAME_DURATION_7_5 ? 7500 : 10000;
}

/**
 * @brief       Remove stored output configuration for specific ISO handle.
 * @param[in]   iso_handle  - ISO connection handle.
 * @return      none.
 */
void lea_release_output_config(uint16_t iso_handle)
{
    lea_release_audio_config(&s_le_audio_output, iso_handle);
}

struct lea_output_audio_pkt_node
{
    SLIST_ENTRY(lea_output_audio_pkt_node) next;
    uint32_t render_point;
    uint32_t location;
    int16_t *left_data;
    int16_t *right_data;
};

SLIST_HEAD(lea_output_audio_pkt, lea_output_audio_pkt_node);

static struct lea_output_audio_pkt output_pkt_header = SLIST_HEAD_INITIALIZER(output_pkt_header);

/**
 * @brief       Remove the first queued output packet and free memory.
 * @return      none.
 */
static void lea_free_output_pkt_header(void)
{
    struct lea_output_audio_pkt_node *pNode = SLIST_FIRST(&output_pkt_header);
    SLIST_REMOVE_HEAD(&output_pkt_header, next); //TODO:

    // tlk_printf("free is %x clock:%x", pNode, clock_time());
    tlkalg_free_func((uint8_t *)pNode);
}

/**
 * @brief       Flush and free all queued output packets.
 * @return      none.
 */
static void lea_release_all_output_audio_pkt(void)
{
    while (!SLIST_EMPTY(&output_pkt_header)) {
        lea_free_output_pkt_header();
    }
}

/**
 * @brief       Allocate an output packet structure with PCM buffers.
 * @param[in]   render_point   - timestamp when packet should be rendered.
 * @return      pointer to allocated node (never NULL, blocks on failure).
 */
static struct lea_output_audio_pkt_node *lea_malloc_output_audio_pkt_node(uint32_t render_point)
{
    struct lea_output_audio_pkt_node *pNode =
        (struct lea_output_audio_pkt_node *)tlkalg_malloc_func(sizeof(struct lea_output_audio_pkt_node) + 2 * sizeof(int16_t) * s_le_audio_output.sample_count);

    // tlk_printf("malloc is %x %x", pNode, clock_time());
    if (pNode == NULL) {
        tlkapi_printf(1, "pNode is Null,malloc fail");
        tlkmdi_audmem_print();
        // malloc failed.
        while (1) {
            /* USB DEBUG */
#if TLK_USB_UDB_ENABLE
            tlk_udb_usb_handle_irq();
#endif
        }
    }

    pNode->render_point = render_point;
    pNode->location     = 0;
    pNode->left_data    = (int16_t *)(((uint8_t *)pNode) + sizeof(struct lea_output_audio_pkt_node));
    pNode->right_data   = (int16_t *)(((uint8_t *)pNode->left_data) + sizeof(int16_t) * s_le_audio_output.sample_count);
    return pNode;
}

/**
 * @brief       Insert packet node ordered by render point or reuse existing one.
 * @param[in]   render_point   - timestamp to insert.
 * @return      pointer to node representing that render point.
 */
static struct lea_output_audio_pkt_node *lea_insert_output_audio_pkt(uint32_t render_point)
{
    bool                              new_node_used = false;
    struct lea_output_audio_pkt_node *pNewNode      = lea_malloc_output_audio_pkt_node(render_point);
    struct lea_output_audio_pkt_node *pNode         = pNewNode;
    //    tlk_printf("malloc pNewNode is %p", pNewNode);
    tlksys_enter_critical();
    // tlkapi_printf(1, "header is %x, render_point is %x, current clock it %x", output_pkt_header.slh_first, render_point, clock_time());
    if (SLIST_EMPTY(&output_pkt_header)) {
        uint32_t next_irq_us = (render_point - clock_time()) / SYSTEM_TIMER_TICK_1US;
        if (next_irq_us > 200 * 1000) { // if next render point is to far.
            tlksys_leave_critical();
            tlkalg_free_func((uint8_t *)pNewNode);
            return NULL;
        }
        SLIST_INSERT_HEAD(&output_pkt_header, pNewNode, next);
        // tlkapi_printf(1, "insert first packet");
        // tlkapi_printf(1, "insert first packet next time is %x %x %x", render_point, clock_time(), render_point - clock_time());
        tlkmdi_audio_task_set_next_irq(next_irq_us);
        new_node_used = true;
    } else {
        pNode                                   = SLIST_FIRST(&output_pkt_header);
        struct lea_output_audio_pkt_node *pPrev = NULL;
        while (pNode != NULL) {
#if LEA_FIX_TIMESTAMP_ERROR
            bool check_timestamp = false;
            if (pNode->render_point > render_point) {
                check_timestamp = (pNode->render_point - render_point) < 200 * SYSTEM_TIMER_TICK_1US;
            } else {
                check_timestamp = (render_point - pNode->render_point) < 200 * SYSTEM_TIMER_TICK_1US;
            }
            if (check_timestamp) {
#else
            if (pNode->render_point == render_point) {
#endif
                tlksys_leave_critical();
                tlkalg_free_func((uint8_t *)pNewNode);
                // tlk_printf("free pNewNode is %p", pNewNode);
                //  tlkapi_printf(1, "found node.");
                return pNode;
            }

            // TODO: IAL need report event packet.
            if ((pNode->render_point - render_point) < (uint32_t)BIT(31)) {
                break;
            }

            pPrev = pNode;

            pNode = SLIST_NEXT(pNode, next);
        }
        //  tlkapi_printf(1, "create new node.");
        pNode         = pNewNode;
        new_node_used = true;
        if (pPrev == NULL) {
            SLIST_INSERT_HEAD(&output_pkt_header, pNode, next);
        } else {
            SLIST_INSERT_AFTER(pPrev, pNode, next);
        }
    }
    tlksys_leave_critical();
    if (new_node_used == false) {
        // tlk_printf("free pNewNode is %p", pNewNode);
        tlkalg_free_func((uint8_t *)pNewNode);
    }
    return pNode;
}

/**
 * @brief       Decode incoming ISO SDU into PCM buffers and queue for render.
 * @param[in]   p_lea_config   - ASE configuration describing ISO handle.
 * @param[in]   timestamp      - SDU timestamp from controller.
 * @param[in]   iso_data_len   - length of ISO payload.
 * @param[in]   p_iso_data     - pointer to ISO payload.
 * @return      none.
 */
static void lea_output_deal_sdu_packet(struct lea_config *p_lea_config, uint32_t timestamp, uint16_t iso_data_len, const uint8_t *p_iso_data)
{
    int frame_octets = p_lea_config->frameOctets;
    int location     = p_lea_config->location;
    int location_cnt = lea_calc_location_valid_count(location);

    int frame_time = (p_lea_config->frameDuration == LEA_SELECT_FRAME_DURATION_10 ? 10000 : 7500) * SYSTEM_TIMER_TICK_1US;

    int            sdu_len  = iso_data_len;
    const uint8_t *sdu_data = p_iso_data;

    uint32_t render_point = timestamp + 3 * SYSTEM_TIMER_TICK_1MS;

    for (int i = 0; i < p_lea_config->blocks; i++) {
        struct lc3_dec_set_bec_detect detect = {
            .channelIndex = 0,
            .detect       = 0,
        };

        if (sdu_len < frame_octets * location_cnt) {
            detect.detect = 1;
        }

        struct lea_output_audio_pkt_node *audio_pkt = lea_insert_output_audio_pkt(render_point);
        if (audio_pkt != NULL) {
            audio_alg_interface_t *p_lc3_dec_alg = audio_alg_get_interface_by_type(ALG_LC3_DEC);
            if (location == LEA_LOCATION_FRONT_LEFT) {
                p_lc3_dec_alg->audio_alg_param_set(LC3_TYPE_SET_BEC_DETECT, &detect);
                p_lc3_dec_alg->audio_alg_process((uint8_t *)sdu_data, (uint8_t *)audio_pkt->left_data, frame_octets, 16, 0);
                audio_pkt->location |= LEA_LOCATION_FRONT_LEFT;
            } else if (location == LEA_LOCATION_FRONT_RIGHT) {
                if (s_le_audio_output.all_location == (LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT)) {
                    detect.channelIndex = 1;
                }
                p_lc3_dec_alg->audio_alg_param_set(LC3_TYPE_SET_BEC_DETECT, &detect);
                p_lc3_dec_alg->audio_alg_process((uint8_t *)sdu_data, (uint8_t *)audio_pkt->right_data, frame_octets, 16, detect.channelIndex);
                audio_pkt->location |= LEA_LOCATION_FRONT_RIGHT;
            } else {
                p_lc3_dec_alg->audio_alg_param_set(LC3_TYPE_SET_BEC_DETECT, &detect);
                detect.channelIndex = 1;
                p_lc3_dec_alg->audio_alg_param_set(LC3_TYPE_SET_BEC_DETECT, &detect);
                p_lc3_dec_alg->audio_alg_process((uint8_t *)sdu_data, (uint8_t *)audio_pkt->left_data, frame_octets, 16, 0);
                p_lc3_dec_alg->audio_alg_process((uint8_t *)sdu_data + frame_octets, (uint8_t *)audio_pkt->right_data, frame_octets, 16, 1);
                audio_pkt->location |= (LEA_LOCATION_FRONT_LEFT | LEA_LOCATION_FRONT_RIGHT);
            }
        }

        sdu_data += frame_octets * location_cnt;
        sdu_len -= frame_octets * location_cnt;
        render_point += frame_time;
    }
}

/**
 * @brief       ISO callback that dispatches SDUs to the matching ASE config.
 * @param[in]   conn_handle    - ISO connection handle.
 * @param[in]   timestamp      - SDU timestamp.
 * @param[in]   iso_data_len   - length of ISO payload.
 * @param[in]   p_iso_data     - pointer to ISO payload.
 * @return      none.
 */
static void lea_output_receive_sdu_packet(uint16_t conn_handle, uint32_t timestamp, uint16_t iso_data_len, const uint8_t *p_iso_data)
{
    for (int i = 0; i < LE_AUDIO_CONFIG_MAX_COUNT; i++) {
        struct lea_config *p_lea_config = &s_le_audio_output.config[i];

        if (p_lea_config->iso_handle == conn_handle) {
            lea_output_deal_sdu_packet(p_lea_config, timestamp, iso_data_len, p_iso_data);
            break;
        }
    }
}

/**
 * @brief       Program audio timer IRQ based on head packet render time.
 * @return      none.
 */
static void lea_output_set_next_irq(void)
{
    struct lea_output_audio_pkt_node *pNode = SLIST_FIRST(&output_pkt_header);

    if (pNode != NULL) {
        uint32_t next_irq_us = (pNode->render_point - clock_time()) / SYSTEM_TIMER_TICK_1US;
        if (next_irq_us > 20 * 1000) {    // if next render point is to far.
            lea_free_output_pkt_header(); // free the first node.
            lea_output_set_next_irq();    // check next node.
        } else {
            tlkmdi_audio_task_set_next_irq(next_irq_us);
        }
    }
}

/**
 * @brief       Audio timer interrupt handler that renders queued PCM frames.
 * @return      none.
 */
void lea_output_timer_irq(void)
{
    struct lea_output_audio_pkt_node *pNode = SLIST_FIRST(&output_pkt_header);

    if (pNode == NULL) {
        return;
    }

    //gpio_toggle(GPIO_CHN4);
    int16_t *left_data  = lea_codec_get_mute_buffer();
    int16_t *right_data = lea_codec_get_mute_buffer();
    int      sample_num = s_le_audio_output.sample_count;

    if (pNode->location & LEA_LOCATION_FRONT_LEFT) {
        left_data = pNode->left_data;
    }

    if (pNode->location & LEA_LOCATION_FRONT_RIGHT) {
        right_data = pNode->right_data;
    }

    lea_codec_output_set_audio_data(left_data, right_data, sample_num);

    lea_free_output_pkt_header();

    lea_output_set_next_irq();
}

#endif //#if (TLKAPI_LE_AUDIO_COMMON_ENABLE)
