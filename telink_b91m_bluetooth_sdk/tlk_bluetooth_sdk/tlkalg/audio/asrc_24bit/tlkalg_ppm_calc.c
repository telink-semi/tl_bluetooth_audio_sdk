/********************************************************************************************************
 * @file    tlkalg_ppm_calc.c
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
#include "drivers.h"
#include "tlkalg_ppm_calc.h"

#if TLKALG_PPM_USB_CALC_ENABLE || TLKALG_PPM_CALC_BY_SAMPLE

#define TLKALG_PPM_WINDOW_WIDTH      50
#define TLKALG_PPM_TICK_PER_SAMPLE   500
#define TLKALG_PPM_WINDOW_IDEAL_TICK (1000 * SYSTEM_TIMER_TICK_1US * TLKALG_PPM_WINDOW_WIDTH)
//asrc compensation step tick, if the compensation is too large, it may cause audio jitter, if it's too small, it may cause slow convergence.
#define TLKALG_ASRC_STEP_TICK        30
#define TLKALG_PPM_INTEGRAL_MAX      3000
#define TLKALG_PPM_INTEGRAL_GAIN     1
#define TLKALG_PPM_INTEGRAL_GAIN_DIV 25

tlkalg_ppm_calc_t  g_ppm_calc_ctl = {0};
tlkalg_ppm_state_t g_ppm_state    = {0};

void tlkalg_ppm_calc_init(void)
{
    if (g_ppm_state.init_flag) {
        return;
    }

    tlk_printf("tlkalg_ppm_calc_init");
    memset(&g_ppm_calc_ctl, 0, sizeof(tlkalg_ppm_calc_t));
    memset(&g_ppm_state, 0, sizeof(tlkalg_ppm_state_t));

    g_ppm_calc_ctl.alg_wptr_spk = g_ppm_calc_ctl.alg_rptr_spk + ((MARGIN_NUM / 2) * 2); //*2 for stereo
    g_ppm_calc_ctl.alg_wptr_spk %= TLKALG_PPM_BUFF_SPK_LEN;

    g_ppm_state.update_flag_spk = false;
    g_ppm_state.update_flag_mic = false;
    g_ppm_state.step_tick       = TLKALG_ASRC_STEP_TICK;
    g_ppm_state.init_flag       = true;
}

//audio data receiver:
//  spk:diff_tick = remote_tick - local_tick
//  mic:diff_tick = local_tick  - remote_tick
//audio data sender:
//  spk:diff_tick = local_tick  - remote_tick
//  mic:diff_tick = remote_tick - local_tick
// int32_t send_data[15] = {0};
// bool print_flag = false;
_attribute_ram_code_sec_ void tlkalg_ppm_calc_process(uint32_t tick)
{
    int32_t interval_tick = 0;

    if (!g_ppm_state.init_flag) {
        tlk_printf("ppm calc not initialized");
        return;
    }

    if (g_ppm_state.last_tick == 0) {
        g_ppm_state.last_tick = tick;
        return;
    } else {
        interval_tick         = tick - g_ppm_state.last_tick;
        g_ppm_state.last_tick = tick;
    }

    g_ppm_state.window_width_count++;
    g_ppm_state.window_interval_tick += interval_tick;
    if (g_ppm_state.window_width_count < TLKALG_PPM_WINDOW_WIDTH) {
        return;
    }
    // print_flag = true;
    int32_t window_diff_tick = TLKALG_PPM_WINDOW_IDEAL_TICK - g_ppm_state.window_interval_tick;

    g_ppm_state.window_width_count   = 0;
    g_ppm_state.window_interval_tick = 0;

    //calculate the average window_diff_tick for the per 10 seconds,
    //and use it as the compensation for ASRC algorithm, which can speed up the convergence of ppm value.
    if (g_ppm_state.avg_count < (10000 / TLKALG_PPM_WINDOW_WIDTH)) {
        g_ppm_state.window_diff_tick_sum += window_diff_tick;
        g_ppm_state.avg_count++;
    } else if (g_ppm_state.avg_count == (10000 / TLKALG_PPM_WINDOW_WIDTH)) {
        g_ppm_state.step_tick = ((g_ppm_state.window_diff_tick_sum / g_ppm_state.avg_count) / 10 + 1) * 10;
        g_ppm_state.avg_count = 0;
    }

    if (g_ppm_state.asrc_diff != 0) {
        g_ppm_state.asrc_alg_comp_tick += (g_ppm_state.asrc_diff * TLKALG_PPM_TICK_PER_SAMPLE);
        g_ppm_state.asrc_diff = 0;
    }

    int32_t asrc_step_tick = 0;
    if (g_ppm_state.asrc_alg_comp_tick > 0) {
        asrc_step_tick = (g_ppm_state.asrc_alg_comp_tick > g_ppm_state.step_tick) ? g_ppm_state.step_tick : g_ppm_state.asrc_alg_comp_tick;
    } else if (g_ppm_state.asrc_alg_comp_tick < 0) {
        asrc_step_tick = (g_ppm_state.asrc_alg_comp_tick < -g_ppm_state.step_tick) ? -g_ppm_state.step_tick : g_ppm_state.asrc_alg_comp_tick;
    }
    g_ppm_state.asrc_alg_comp_tick -= asrc_step_tick;
    window_diff_tick += asrc_step_tick;

    g_ppm_state.tick_diff_integral += window_diff_tick;
    if (g_ppm_state.tick_diff_integral > TLKALG_PPM_INTEGRAL_MAX) {
        g_ppm_state.tick_diff_integral = TLKALG_PPM_INTEGRAL_MAX;
    } else if (g_ppm_state.tick_diff_integral < -TLKALG_PPM_INTEGRAL_MAX) {
        g_ppm_state.tick_diff_integral = -TLKALG_PPM_INTEGRAL_MAX;
    }

    int64_t pi_total          = (int64_t)window_diff_tick + g_ppm_state.tick_diff_integral * TLKALG_PPM_INTEGRAL_GAIN / TLKALG_PPM_INTEGRAL_GAIN_DIV;
    int64_t ppm_million_total = (int64_t)(pi_total * 1000000 + g_ppm_state.ppm_remainder);
    int     ppm_val_new       = ppm_million_total / TLKALG_PPM_WINDOW_IDEAL_TICK;
    g_ppm_state.ppm_remainder = ppm_million_total % TLKALG_PPM_WINDOW_IDEAL_TICK;

    g_ppm_state.current_ppm = (ppm_val_new * TLKALG_PPM_SMOOTH_ALPHA + g_ppm_state.last_ppm * (10 - TLKALG_PPM_SMOOTH_ALPHA)) / 10;

    if (g_ppm_state.current_ppm > TLKALG_PPM_MAX) {
        g_ppm_state.current_ppm = TLKALG_PPM_MAX;
    } else if (g_ppm_state.current_ppm < -TLKALG_PPM_MAX) {
        g_ppm_state.current_ppm = -TLKALG_PPM_MAX;
    }

    if (g_ppm_state.last_ppm != g_ppm_state.current_ppm) {
        g_ppm_state.update_flag_spk = true;
        g_ppm_state.update_flag_mic = true;
        g_ppm_state.last_ppm        = g_ppm_state.current_ppm;
    }
}

int tlkalg_ppm_get_ppm_val(void)
{
    return g_ppm_state.current_ppm;
}

void tlkalg_ppm_calc_deint(void)
{
    g_ppm_state.init_flag = false;
}

void tlkalg_ppm_calc_sync_buff(uint16_t sample)
{
    g_ppm_calc_ctl.alg_wptr_spk = (g_ppm_calc_ctl.alg_rptr_spk + sample) % TLKALG_PPM_BUFF_SPK_LEN;
    tlkapi_printf(APP_LOG_EN, "ppm buff wptr rptr %d %d", g_ppm_calc_ctl.alg_wptr_spk, g_ppm_calc_ctl.alg_rptr_spk);
}

uint16_t tlkalg_ppm_calc_get_idlelen(void)
{
    uint16_t len = 0;
    if (g_ppm_calc_ctl.alg_wptr_spk >= g_ppm_calc_ctl.alg_rptr_spk) {
        len = TLKALG_PPM_BUFF_SPK_LEN - (g_ppm_calc_ctl.alg_wptr_spk - g_ppm_calc_ctl.alg_rptr_spk);
    } else {
        len = g_ppm_calc_ctl.alg_rptr_spk - g_ppm_calc_ctl.alg_wptr_spk;
    }
    return len;
}

uint16_t tlkalg_ppm_calc_get_datalen(void)
{
    return (TLKALG_PPM_BUFF_SPK_LEN - tlkalg_ppm_calc_get_idlelen());
}

bool tlkalg_ppm_calc_get_data(int32_t *data, uint16_t len)
{
    if (data == NULL || len == 0) {
        tlkapi_printf(APP_LOG_EN, "tlkalg_ppm_calc_get_data error");
        return false;
    }

    if (tlkalg_ppm_calc_get_datalen() < len) {
        tlkapi_printf(APP_LOG_EN, "tlkalg_ppm_calc_get_data null");
        return false;
    }

    uint16_t templen = 0;
    uint8_t *buffer  = (uint8_t *)data;
    if (g_ppm_calc_ctl.alg_rptr_spk + len <= TLKALG_PPM_BUFF_SPK_LEN) {
        templen = len;
    } else {
        templen = (TLKALG_PPM_BUFF_SPK_LEN - g_ppm_calc_ctl.alg_rptr_spk);
    }
    if (templen != 0) {
        tmemcpy(buffer, (uint8_t *)&g_ppm_calc_ctl.ppm_buff_spk[g_ppm_calc_ctl.alg_rptr_spk], templen * sizeof(int32_t));
    }

    if (templen == len) {
        g_ppm_calc_ctl.alg_rptr_spk += len;
        g_ppm_calc_ctl.alg_rptr_spk %= TLKALG_PPM_BUFF_SPK_LEN;
    } else {
        g_ppm_calc_ctl.alg_rptr_spk = len - templen;
        tmemcpy((buffer + templen * sizeof(int32_t)), (uint8_t *)g_ppm_calc_ctl.ppm_buff_spk, g_ppm_calc_ctl.alg_rptr_spk * sizeof(int32_t));
    }

    return true;
}

bool tlkalg_ppm_calc_fill_data(int32_t *data, uint16_t len)
{
    if (data == NULL || len == 0) {
        tlkapi_printf(APP_LOG_EN, "tlkalg_ppm_calc_fill_data error");
        return false;
    }

    if (tlkalg_ppm_calc_get_idlelen() < len) {
        tlkapi_printf(APP_LOG_EN, "tlkalg_ppm_calc_fill_data overflow");
        return false;
    }

    uint16_t offset = 0;
    if (g_ppm_calc_ctl.alg_wptr_spk + len > TLKALG_PPM_BUFF_SPK_LEN) {
        offset = TLKALG_PPM_BUFF_SPK_LEN - g_ppm_calc_ctl.alg_wptr_spk;
    } else {
        offset = len;
    }
    tmemcpy((uint8_t *)&g_ppm_calc_ctl.ppm_buff_spk[g_ppm_calc_ctl.alg_wptr_spk], (uint8_t *)data, offset * sizeof(int32_t));

    if (offset < len) {
        tmemcpy((uint8_t *)g_ppm_calc_ctl.ppm_buff_spk, (uint8_t *)&data[offset], (len - offset) * sizeof(int32_t));
    }

    g_ppm_calc_ctl.alg_wptr_spk += len;
    g_ppm_calc_ctl.alg_wptr_spk %= TLKALG_PPM_BUFF_SPK_LEN;

    return true;
}

tlkalg_cal_sample_ppm_t g_uac_ppm_ctrl = {0};

void tlkalg_ppm_setting_init(unsigned int mask)
{
    tlkalg_reset_sample_ppm_calc(&g_uac_ppm_ctrl);
    tlkalg_set_sample_ppm_mode(&g_uac_ppm_ctrl, 0);
    tlkalg_set_sample_ppm_mask(&g_uac_ppm_ctrl, mask);
    tlkalg_set_sample_ppm_channel(&g_uac_ppm_ctrl, 2);
}

void tlkalg_calc_sample_ppm(tlkalg_cal_sample_ppm_t *ppm_ptr, unsigned int wptr, unsigned int tick, unsigned int ref, unsigned int sample_rate)
{
    if (ppm_ptr->reset) {
        ppm_ptr->reset          = 0;
        ppm_ptr->ppm_set        = 1;
        ppm_ptr->sample_ma      = ref;
        ppm_ptr->sample_ref     = ref;
        ppm_ptr->sample_last_ma = ref;
        ppm_ptr->last_tick      = tick;
        ppm_ptr->last_ptr       = wptr;
        ppm_ptr->cur_ppm        = 0;
        ppm_ptr->last_ppm       = 0;
        ppm_ptr->resample_diff  = 0;
        ppm_ptr->sample_rate    = sample_rate;
        tlkapi_printf(APP_LOG_EN, "ppm reset,wptr:%d,ref:%d,sample rate:%d", wptr, ref, sample_rate);
        return;
    }

    int sample = (wptr - ppm_ptr->last_ptr) & ppm_ptr->ptr_mask;
    if (sample > ppm_ptr->sample_ref + 30 * ppm_ptr->channel || sample < ppm_ptr->sample_ref - 30 * ppm_ptr->channel) {
        ppm_ptr->reset = 1;
        tlkapi_printf(APP_LOG_EN, "sample reset:%d,ref:%d", sample, ppm_ptr->sample_ref);
        return;
    }
    ppm_ptr->last_ptr = wptr;
    ppm_ptr->sample_ma -= ((signed int)sample - ppm_ptr->sample_ref);
    ppm_ptr->sample_ma += ppm_ptr->resample_diff;
    ppm_ptr->resample_diff = 0;

    int sa = ppm_ptr->sample_ma - ppm_ptr->sample_ref;
    int sb = ppm_ptr->sample_ma - ppm_ptr->sample_last_ma;

    if (((sa * sb) > 0 && abs_ram(sb) > 2 * ppm_ptr->channel) && clock_time_exceed(ppm_ptr->last_tick, 200000)) //( || (abs_ram(sb) > 5))
    {
        unsigned int time = (tick - ppm_ptr->last_tick) / SYSTEM_TIMER_TICK_1US;
        int          ppm  = (abs_ram(sb) * 1000000 / sample_rate) * 1000000 / time; //sa * 1000000 * 1000000/(sample_rate * time)

        if (ppm > 2 * ppm_ptr->channel) {
            ppm /= (2 * ppm_ptr->channel);
            if (ppm < 2 * ppm_ptr->channel) {
                ppm = 2 * ppm_ptr->channel;
            }
        }

        if (abs_ram(ppm) > 100) {
            ppm = ppm > 0 ? 100 : -100;
        }

        if (sb < 0) {
            ppm *= -1;
        }

        tlkapi_printf(APP_LOG_EN, "calc ppm:%d,last ppm:%d,cur ppm:%d,diff sample:%d,time:%d,sample ma:%d,sample ref:%d", ppm, ppm_ptr->last_ppm, ppm_ptr->cur_ppm, sb, time,
                      ppm_ptr->sample_ma, ppm_ptr->sample_ref);

        if ((ppm_ptr->last_ppm * ppm) < 0) {
            if (abs_ram(ppm) > abs_ram(ppm_ptr->last_ppm)) {
                ppm = -ppm_ptr->last_ppm;
            }
        } else {
            if ((abs_ram(ppm) < abs_ram(ppm_ptr->last_ppm)) && (abs_ram(ppm) > abs_ram(ppm_ptr->last_ppm) / 2)) {
                if (abs_ram(ppm) > 5) {
                    ppm = ppm_ptr->last_ppm / 2;
                }
            }
        }

        ppm_ptr->last_ppm = ppm;
        ppm_ptr->cur_ppm += ppm;
        if (abs_ram(ppm_ptr->cur_ppm) > 500) {
            //			my_dump_str_data(1,"ppm reset",&ppm_ptr->cur_ppm,4);
            ppm_ptr->reset   = 1;
            ppm_ptr->cur_ppm = 0;
        }

        if (ppm != 0) {
            ppm_ptr->ppm_set        = 1;
            ppm_ptr->last_tick      = tick;
            ppm_ptr->sample_last_ma = ppm_ptr->sample_ma;
        }
    }
    if ((tick - ppm_ptr->last_tick) > 50000000 * SYSTEM_TIMER_TICK_1US) {
        ppm_ptr->last_tick += 1000000 * SYSTEM_TIMER_TICK_1US;
    }
}
#endif
