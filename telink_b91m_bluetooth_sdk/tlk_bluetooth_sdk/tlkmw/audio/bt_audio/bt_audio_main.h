/********************************************************************************************************
 * @file    bt_audio_main.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef __BT_AUDIO_MAIN_H__
#define __BT_AUDIO_MAIN_H__

#include "tlkmw/audio/audio_mw_manager.h"
#include "tlkmw/sys_dev/tlkmw_sysdev.h"


#define FLG_BTC_TWS_EN    (0x01UL << 11)

#define POWER_DEC         0
#define POWER_INC         1

#define BT_AUDIO_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_SNK << 16) | TLK_DEBUG_DBG_FLAG_ALL)

#define BT_AUDIO_DBG_SIGN NULL

typedef enum
{
    BT_AUDIO_SUCCESS            = 0x00,
    BT_AUDIO_ERROR              = 0x01,
    BT_AUDIO_LENGTH_INVALID     = 0x02,
    BT_AUDIO_PACKET_INVALID     = 0x03,
    BT_AUDIO_PACKET_LEN_INVALID = 0x04,
    BT_AUDIO_TYPE_INVALID       = 0x05,
} bt_audio_result_e;

typedef struct
{
    uint8_t voice_max_vol : 4;
    uint8_t music_max_vol : 4;
    uint8_t mode;         /**< mono:0; stereo 1. */
    uint8_t codec_select; /**< CODEC_EXT or CODEC_INT. */
    uint8_t status;       /**< idle, play, play_mute, powerdown. */

    uint16_t sample_rate;
    uint16_t codec_sample_rate;
    uint16_t samples_played;
    uint16_t mute_samples;
    uint16_t latency;

    int16_t mic_rptr;
    int16_t mic_wptr;
    int16_t play_wptr;
    int16_t play_rptr;

    uint16_t play_dec_samples;
    int16_t  music_vol_percent;
    int16_t  voice_vol_percent;
    uint16_t resv;

} codec_cfg_t;

extern codec_cfg_t g_codec_cfg;

typedef struct
{
    bool     irq_en;
    uint8_t  resv3byte[3];
    uint32_t tick_irq;

    uint8_t sco_packet_is_processing;

    // audio sync
    uint32_t stimer_irq_tick;

    uint32_t task_start_ts;
    uint32_t m_schedule_tick;
    uint32_t s_capture_tick;
    uint32_t bt_audio_sync_tick;
    uint8_t  sco_id;
} bt_audio_task_env_t;

extern bt_audio_task_env_t g_bt_audio_task_env;

typedef uint16_t (*bt_audio_get_pcm_data_callback_t)(int16_t *pd0, int16_t *pd1);
typedef uint16_t (*bt_audio_post_pcm_data_callback_t)(codec_int *p_des, uint16_t n, uint16_t sample_rate, int16_t *pwptr);
typedef uint32_t (*bt_audio_lowpower_callback_t)(void);

extern bt_audio_get_pcm_data_callback_t  bt_audio_get_pcm_data_cb;
extern bt_audio_post_pcm_data_callback_t bt_audio_post_pcm_data_cb;
extern bt_audio_lowpower_callback_t      bt_codec_lowpower_cb;

#if TLK_PCM_DATA_WR_EN
typedef uint16_t (*bt_audio_trans_pcm_data_callback_t)(int16_t *p_des, uint16_t dataLen);

extern bt_audio_trans_pcm_data_callback_t bt_audio_get_spk_data_cb;
extern bt_audio_trans_pcm_data_callback_t bt_audio_get_mic_data_cb;
extern bt_audio_trans_pcm_data_callback_t bt_audio_get_dsp_mic_data_cb;
#endif

#define AUDIO_TYPE_MUSIC 0
#define AUDIO_TYPE_VOICE 1

#if TLK_PCM_DATA_WR_EN
/**
 * @brief  register get speaker data callback function
 * @param[in]  p - callback function pointer
 * @returns none
 */
void bt_audio_register_get_spk_data_callback(void *p);

/**
 * @brief  register get mic data callback function
 * @param[in]  p - callback function pointer
 * @returns none
 */
void bt_audio_register_get_mic_data_callback(void *p);

/**
 * @brief  register get dsp mic data callback function
 * @param[in]  p - callback function pointer
 * @returns none
 */
void bt_audio_register_get_dsp_mic_data_callback(void *p);
#endif

/**
 * @brief  get bt audio task environment pointer
 * @param[in]  none
 * @returns bt audio task environment pointer
 */
bt_audio_task_env_t *get_bt_audio_task_env_ptr(void);

/**
 * @brief  timer1 irq handler, used to encode/decode sco and decode a2dp
 * @param[in]  none
 * @returns none
 */
void bt_audio_main(void);

/**
 * @brief  register audio task run function and set irq enable flag
 * @param[in]  p_callback - audio task run function
 * @param[in]  enable     - irq enable flag
 * @returns none
 */
void bt_audio_task_register_run_cb(void *p_callback, uint8_t enable);

/**
 * @brief  start audio task
 * @param[in]  none
 * @returns none
 */
void bt_audio_start_task(void);

/**
 * @brief  register get pcm data function
 * @param[in]  p - get pcm data function
 * @returns none
 */
void bt_audio_register_get_pcm_data_callback(void *p);

/**
 * @brief  control music volume
 * @param[in]  p      - music data
 * @param[in]  sample - music data sample number
 * @returns none
 */
void bt_audio_control_music_volume(int16_t *p, uint16_t sample);

/**
 * @brief  control music volume for right channel
 * @param[in]  p      - music data
 * @param[in]  sample - music data sample number
 * @returns none
 */
void bt_audio_control_music_volume_right(int16_t *p, uint16_t sample);

/**
 * @brief  get music volume percent
 * @param[in]  none
 * @returns volume percent
 */
int16_t bt_audio_get_music_vol_percent(void);

/**
 * @brief  get music and voice max volume flag
 * @param[in]  none
 * @returns music and voice max volume flag
 */
uint8_t bt_audio_get_max_vol_func(void);

/**
 * @brief  BT MIC data process task
 * @param[in]  none
 * @returns none
 */
void bt_audio_recorder_task(void);

/**
 * @brief  bt voice and music playback process, called by timer1_irq_handler
 * @param[in]  none
 * @returns none
 */
void bt_audio_player_task(void);

/**
 * @brief  control voice volume
 * @param[in]  p      - voice data
 * @param[in]  sample - voice data sample number
 * @returns none
 */
void bt_audio_control_voice_volume(int16_t *p, uint16_t sample);

/**
 * @brief  set voice volume percent
 * @param[in]  step - volume step
 * @returns none
 */
void bt_audio_set_voice_vol_percent(uint8_t step);

/**
 * @brief  set music volume percent
 * @param[in]  is_ios - whether is ios device
 * @param[in]  step   - volume step
 * @returns none
 */
void bt_audio_set_music_vol_percent(uint8_t is_ios, uint8_t step);

/**
 * @brief  set music volume percent by handle
 * @param[in]  handle - connection handle
 * @returns none
 */
void bt_audio_set_music_vol_percent_by_handle(uint16_t handle);

/**
 * @brief  set voice volume percent by handle
 * @param[in]  handle - connection handle
 * @returns none
 */
void bt_audio_set_voice_vol_percent_by_handle(uint16_t handle);

/**
 * @brief  get mute data
 * @param[in]  pd0 - left channel data pointer
 * @param[in]  pd1 - right channel data pointer
 * @returns mute data length
 */
uint16_t bt_audio_get_mute_data(int16_t *pd0, int16_t *pd1);

/**
 * @brief  bt audio main loop process
 * @param[in]  none
 * @returns none
 */
void bt_audio_main_loop(void);

/**
 * @brief  get pcm threshold flag
 * @param[in]  none
 * @returns threshold flag
 */
bool tlkmdi_audio_get_threshold_flag(void);

/**
 * @brief  set pcm check threshold flag
 * @param[in]  flag - threshold flag
 * @returns none
 */
void tlkmdi_audio_set_check_threshold_flag(bool flag);

#if AUDIO_TWS_MODE
/**
 * @brief  set tws speaker ppm value
 * @param[in]  ppm_spk_value - ppm value
 * @returns none
 */
void app_tws_spk_set_ppm_value(int ppm_spk_value);

/**
 * @brief  process tws speaker ppm
 * @param[in]  ps      - source data pointer
 * @param[in]  pd      - destination data pointer
 * @param[in]  len     - data length
 * @param[in]  channel - channel number
 * @returns processed data length
 */
uint16_t app_tws_spk_ppm_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t channel);

/**
 * @brief  initialize tws speaker ppm
 * @param[in]  channel - channel number
 * @returns none
 */
void app_tws_spk_ppm_init(uint8_t channel);

/**
 * @brief  deinitialize tws speaker ppm
 * @param[in]  none
 * @returns none
 */
void app_tws_spk_ppm_deinit(void);
#endif

#endif
