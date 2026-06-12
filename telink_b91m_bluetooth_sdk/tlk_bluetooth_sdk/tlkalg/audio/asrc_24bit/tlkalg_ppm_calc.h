/********************************************************************************************************
 * @file    tlkalg_ppm_calc.h
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
#ifndef _TLKALG_PPM_CALC_H_
#define _TLKALG_PPM_CALC_H_

#define TLKALG_PPM_CALC_INTERVAL_MS 1000
#define TLKALG_PPM_MAX              500
#define TLKALG_PPM_SMOOTH_ALPHA     3 //1~10
#define MARGIN_NUM                  6
#define TLKALG_PPM_BUFF_SPK_LEN     ((300 + 6 + MARGIN_NUM) * 2) //*2 for stereo
#define TLKALG_PPM_BUFF_MIC_LEN     (16 + MARGIN_NUM)

typedef struct
{
    int16_t  sample_diff_spk;
    int16_t  sample_diff_mic;
    uint16_t alg_wptr_spk;
    uint16_t alg_rptr_spk;
    // uint16_t alg_wptr_mic;
    // uint16_t alg_rptr_mic;
    // uint16_t recv;
    int32_t ppm_buff_spk[TLKALG_PPM_BUFF_SPK_LEN];
    // int16_t  ppm_buff_mic[TLKALG_PPM_BUFF_MIC_LEN];
} tlkalg_ppm_calc_t;

typedef struct
{
    int32_t  last_tick;
    int32_t  current_ppm;
    int32_t  last_ppm;
    int32_t  asrc_diff;
    int32_t  ppm_remainder;
    int32_t  step_tick;
    int32_t  window_interval_tick;
    int32_t  asrc_alg_comp_tick;
    int32_t  tick_diff_integral;
    int32_t  window_diff_tick_sum;
    uint16_t window_width_count;
    uint16_t avg_count;
    bool     update_flag_spk;
    bool     update_flag_mic;
    bool     init_flag;
} tlkalg_ppm_state_t;

extern tlkalg_ppm_state_t g_ppm_state;
extern tlkalg_ppm_calc_t  g_ppm_calc_ctl;

void tlkalg_ppm_calc_init(void);
void tlkalg_ppm_calc_process(uint32_t tick);
int  tlkalg_ppm_get_ppm_val(void);
void tlkalg_ppm_calc_deint(void);

void tlkalg_ppm_calc_sync_buff(uint16_t sample);
bool tlkalg_ppm_calc_get_data(int32_t *data, uint16_t len);
bool tlkalg_ppm_calc_fill_data(int32_t *data, uint16_t len);

typedef enum
{
    ASRC_VOICE_MODE  = BIT(0),
    ASRC_MUSIC_MODE  = BIT(1),
    ASRC_MUSIC1_MODE = BIT(2),
} tlkalg_ppm_mode_e;

typedef struct
{
    unsigned int last_ptr;
    int          sample_ma;
    int          sample_last_ma;
    int          sample_ref;
    unsigned int last_tick;
    unsigned int ptr_mask;
    int          resample_diff;
    unsigned int sample_rate;

    int           cur_ppm;
    int           last_ppm;
    unsigned char reset;
    unsigned char ppm_set;
    unsigned char ppm_mode;
    unsigned char channel;
} tlkalg_cal_sample_ppm_t;

extern tlkalg_cal_sample_ppm_t g_uac_ppm_ctrl;

static inline int tlkalg_sample_ppm_mode_idle(tlkalg_cal_sample_ppm_t *ppm_ptr)
{
    return ppm_ptr->ppm_mode == 0;
}

static inline void tlkalg_set_sample_ppm_mode(tlkalg_cal_sample_ppm_t *ppm_ptr, unsigned char mode)
{
    ppm_ptr->ppm_mode = mode;
}

static inline void tlkalg_clear_sample_ppm_mode(tlkalg_cal_sample_ppm_t *ppm_ptr, unsigned char mode)
{
    ppm_ptr->ppm_mode &= ~mode;
}

static inline unsigned char tlkalg_get_sample_ppm_mode(tlkalg_cal_sample_ppm_t *ppm_ptr, unsigned char mode)
{
    return ppm_ptr->ppm_mode & mode;
}

static inline void tlkalg_set_sample_ppm_mask(tlkalg_cal_sample_ppm_t *ppm_ptr, unsigned int mask)
{
    ppm_ptr->ptr_mask = mask;
}

static inline void tlkalg_reset_sample_ppm_calc(tlkalg_cal_sample_ppm_t *ppm_ptr)
{
    ppm_ptr->reset = 1;
}

static inline void tlkalg_set_sample_ppm_channel(tlkalg_cal_sample_ppm_t *ppm_ptr, unsigned char chn)
{
    ppm_ptr->channel = chn;
}

static inline void tlkalg_add_sample_ppm_diff_samples(tlkalg_cal_sample_ppm_t *ppm_ptr, int diff)
{
    ppm_ptr->resample_diff += diff;
}

static inline int tlkalg_get_sample_ma_diff(tlkalg_cal_sample_ppm_t *ppm_ptr)
{
    return (ppm_ptr->sample_ma - ppm_ptr->sample_ref);
}

static inline unsigned int tlkalg_get_sample_ppm_sample_rate(tlkalg_cal_sample_ppm_t *ppm_ptr)
{
    return ppm_ptr->sample_rate;
}

void tlkalg_ppm_setting_init(unsigned int mask);
void tlkalg_calc_sample_ppm(tlkalg_cal_sample_ppm_t *ppm_ptr, unsigned int wptr, unsigned int tick, unsigned int ref, unsigned int sample_rate);
#endif
