/********************************************************************************************************
 * @file    tlkusb_uacMic.c
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
#include "tlklib/usb/tlkusb_stdio.h"
#include "tlklib/usb/uac/tlkusb_uacDefine.h"
#if (TLKUSB_UAC_MIC_ENABLE)
#include "tlklib/usb/uac/tlkusb_uacDesc.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"
#include "tlkalg/audio/ppm/tlkalg_u2s_ppm.h"
#include "tlkalg/audio/audio_alg_interface.h"

#include "drivers.h"
#include "stack/stack.h"
#include "tlkalg/audio/asrc_24bit/tlkalg_ppm_calc.h"
extern uint8_t gLeaPpmStatus;
// #if TLK_SIMPLE_PPM_EN
uint32_t mic_ppm_tick_last      = 0;
int32_t  mic_tick_diff_val      = 0;
int32_t  mic_spk_samples        = 0;
int      mic_ppm_sample_counter = 0;
uint32_t mic_over_tick[5]       = {0};
uint32_t mic_ppm_tick           = 0;
uint8_t  mic_g_ppm_flag         = 0;
uint32_t mic_over_time_us       = PPM_WORK_OVERTIMER_US;
// #endif
uint8_t s_enable_ppm = 0x00;

static uint8_t sTlkUsbUacMicEnable = false;

static tlku2h_ppm_para_t tlkusb_uacmic_ppmpara = {.start_flag = false, .tick_last = 0, .tick_rem = 0, .sample_diff = 0};

/**
 * @brief     Initialize the UAC microphone module
 * @return    TLK_ENONE
 */
int tlkusb_uacmic_init(void)
{
    sTlkUsbUacMicEnable = false;
    s_enable_ppm        = 1;
    return TLK_ENONE;
}

/**
 * @brief     Enable or disable PPM functionality
 * @param[in] enable Enable(1) or disable(0) PPM
 * @return    none
 */
void tlkusb_uacmic_ppm_enable(uint8_t enable)
{
    s_enable_ppm = enable;
}

/**
 * @brief     Get the enable status of UAC microphone
 * @return    true if enabled, false otherwise
 */
bool tlkusb_uacmic_getEnable(void)
{
    return sTlkUsbUacMicEnable;
}

/**
 * @brief     Enable or disable the UAC microphone
 * @param[in] enable Enable(true) or disable(false) the microphone
 * @return    none
 */
void tlkusb_uacmic_setEnable(bool enable)
{
    sTlkUsbUacMicEnable = enable;
    if (enable) {
        reg_usb_ep_ctrl(TLKUSB_UAC_EDP_MIC) = 0;
        reg_usb_ep_ptr(TLKUSB_UAC_EDP_MIC)  = 0;
        reg_usb_ep_ctrl(TLKUSB_UAC_EDP_MIC) = 0x01; // ACK first packet
        reg_usb_ep_ctrl(TLKUSB_UAC_EDP_MIC) = BIT(0);
    } else {
        g_tlk_usb_cfg.tick_in   = 0;
        g_tlk_usb_cfg.iso_in_en = false;
        tlkusb_uac_reset_in_config();
        if (sTlkUsbReportUacStatusCB != NULL) {
            sTlkUsbReportUacStatusCB(g_tlk_usb_cfg.iso_in_en, g_tlk_usb_cfg.iso_out_en || g_tlk_usb_cfg.iso_out1_en);
        }
    }
}

/**
 * @brief     Set the volume level for the microphone
 * @param[in] volume Volume level to set
 * @return    none
 */
void tlkusb_uacmic_setVolume(int16_t volume)
{
    g_tlk_usb_cfg.in_volume = volume;
    if (volume < TLKUSB_AUDMIC_VOL_MIN) {
        g_tlk_usb_cfg.in_vol_step = 0;
    } else {
        g_tlk_usb_cfg.in_vol_step = (volume - TLKUSB_AUDMIC_VOL_MIN) / TLKUSB_AUDMIC_VOL_RES;
    }
}

/**
 * @brief     Mute or unmute the microphone
 * @param[in] enable Mute(true) or unmute(false)
 * @return    none
 */
void tlkusb_uacmic_enterMute(bool enable)
{
    g_tlk_usb_cfg.in_mute = enable;
}

/**
 * @brief     Handle set interface command for the microphone
 * @param[in] type Type of the feature to set
 * @return    TLK_ENONE
 */
int tlkusb_uacmic_setInfCmdDeal(int type)
{
    if (type == AUDIO_FEATURE_MUTE) {
        g_tlk_usb_cfg.in_mute = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
    } else if (type == AUDIO_FEATURE_VOLUME) {
        u16 val = tlkusb_hal_read_ctrl_ep_u16(TLK_CFG_USB_UAC_INDEX);
        tlkusb_uacmic_setVolume(val);
    } else {
    }
    return TLK_ENONE;
}

/**
 * @brief     Handle get interface command for the microphone
 * @param[in] req  Request type
 * @param[in] type Type of the feature to get
 * @return    TLK_ENONE if successful, -TLK_ENOSUPPORT for unsupported requests
 */
int tlkusb_uacmic_getInfCmdDeal(int req, int type)
{
    if (type == AUDIO_FEATURE_MUTE) {
        tlkusb_hal_write_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX, g_tlk_usb_cfg.in_mute);
    } else if (type == AUDIO_FEATURE_VOLUME) {
        switch (req) {
        case AUDIO_REQ_GetCurrent:
            usbhw_write_ctrl_ep_u16(g_tlk_usb_cfg.in_volume);
            break;
        case AUDIO_REQ_GetMinimum:
            usbhw_write_ctrl_ep_u16(TLKUSB_AUDMIC_VOL_MIN);
            break;
        case AUDIO_REQ_GetMaximum:
            usbhw_write_ctrl_ep_u16(TLKUSB_AUDMIC_VOL_MAX);
            break;
        case AUDIO_REQ_GetResolution:
            usbhw_write_ctrl_ep_u16(TLKUSB_AUDMIC_VOL_RES);
            break;
        default:
            return -TLK_ENOSUPPORT;
        }
    } else {
        return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}

/**
 * @brief     Handle set endpoint command for the microphone
 * @param[in] type Type of the endpoint control
 * @return    TLK_ENONE
 */
int tlkusb_uacmic_setEdpCmdDeal(int type)
{
    if (type == AUDIO_EPCONTROL_SamplingFreq) {
        uint32_t value               = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
        g_tlk_usb_cfg.in_sample_rate = value;
        value                        = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
        g_tlk_usb_cfg.in_sample_rate |= value << 8;
        value = tlkusb_hal_read_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX);
        g_tlk_usb_cfg.in_sample_rate |= value << 16;
        // TODO: Sample Rate Changed
    }
    return TLK_ENONE;
}

/**
 * @brief     Stop the PPM processing
 * @return    none
 */
void tlkusb_uacmic_ppm_stop(void)
{
#if TLKLIB_UAC_PPM_ENABLE
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_MIC);
    int                    ppm_value      = 0;
    p_audio_alg_if->audio_alg_param_set(0, &ppm_value);

    tlkalg_u2s_ppm_clear_status();
    if (tlkalg_u2s_ppm_get_init_flag_up()) {
        tlkalg_u2s_ppm_clear_init_flag_up();
    }
#endif
    tmemset(&tlkusb_uacmic_ppmpara, 0, sizeof(tlkusb_uacmic_ppmpara));
}

/**
 * @brief     Process PPM for the microphone data
 * @param[in] tick Current system tick
 * @return    none
 */
void tlkusb_uacmic_ppm_process(uint32_t tick)
{
    uint32_t tick_diff              = tick - tlkusb_uacmic_ppmpara.tick_last + tlkusb_uacmic_ppmpara.tick_rem;
    tlkusb_uacmic_ppmpara.tick_last = tick;

    uint16_t samples_per_ms = (g_tlk_usb_cfg.in_sample_rate / 1000);

    uint16_t used_samples = tlkusb_uac_get_iso_in_used_samples();

    if (!tlkusb_uacmic_ppmpara.start_flag) {
        if (used_samples >= (samples_per_ms * 8)) {
            tlkusb_uac_sync_ppm_in_samples(samples_per_ms * 15); //wptr - rptr = 7.5ms*2
            tlkusb_uacmic_ppmpara.start_flag = true;
            tlkapi_trace(0xFFFFFFFF, "[TEST]", "usb start length %d", tlkusb_uac_get_ppm_in_used_samples());
        }
        return;
    }

    uint16_t samples_num           = (tick_diff / (SYSTEM_TIMER_TICK_1US * 1000)) * samples_per_ms;
    tlkusb_uacmic_ppmpara.tick_rem = tick_diff % (SYSTEM_TIMER_TICK_1US * 1000);

    if (used_samples >= samples_num) {
#if TLKLIB_UAC_PPM_ENABLE
        audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_MIC);
        if (tlkalg_u2s_ppm_get_init_flag_up()) {
            tlkalg_u2s_ppm_clear_init_flag_up();
            int ppm_value = -tlkalg_u2s_ppm_get_val();
            if (16000 == g_tlk_usb_cfg.in_sample_rate) {
                ppm_value = ppm_value * 4 / 5;
            }

            p_audio_alg_if->audio_alg_param_set(0, &ppm_value);
        }
#endif
        if (samples_num > samples_per_ms * 12) {
            samples_num = samples_per_ms * 12;
        }
#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
        int32_t u2h_data[samples_per_ms * 12]; //12ms data  48k samplerate mono
#else
        int16_t u2h_data[samples_per_ms * 12]; //12ms data  48k samplerate mono
#endif

        for (uint16_t index = 0; index < samples_num; index++) {
            u2h_data[index] = g_tlk_usb_cfg.iso_in[g_tlk_usb_cfg.in_r & APP_USB_ISO_IN_BUFF_IDX_MASK];
            g_tlk_usb_cfg.in_r++;
            g_tlk_usb_cfg.in_r &= APP_USB_ISO_IN_BUFF_IDX_MASK;
        }

#if TLKLIB_UAC_PPM_ENABLE
#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
        int32_t ppm_pro_data[samples_per_ms * 12]; //12ms data   48k samplerate mono
#else
        int16_t ppm_pro_data[samples_per_ms * 12]; //12ms data   48k samplerate mono
#endif

        int len_out = p_audio_alg_if->audio_alg_process((uint8_t *)u2h_data, (uint8_t *)ppm_pro_data, samples_num, 0, 0);
        if (-1 == len_out) {
            tlkapi_trace(0xFFFFFFFF, "[ERROR]", "ppm len oversize %d", samples_num);
        }

        tlkusb_uacmic_ppmpara.sample_diff = samples_num - len_out;
        if (48000 == g_tlk_usb_cfg.in_sample_rate) {
            g_usb_resample_diff += tlkusb_uacmic_ppmpara.sample_diff;
        } else if (16000 == g_tlk_usb_cfg.in_sample_rate) {
            g_usb_resample_diff += tlkusb_uacmic_ppmpara.sample_diff * 3; //16k samplerate *3; 48k samplerate *1
        }


        // if (len_out != samples_num) {    //this if can disable, but don't delete
        // uint16_t ppm_used_samples = tlkusb_uac_get_ppm_in_used_samples();
        // tlkapi_trace(0xFFFFFFFF, "[TEST]", "u2h usb %d %d out in %d %d", used_samples, ppm_used_samples, len_out, samples_num);
        // }
        if (tlkusb_uac_write_ppm_in_samples(ppm_pro_data, len_out) == 0) {
        } else {
        }
#else
        tlkusb_uac_write_ppm_in_samples(u2h_data, samples_num);
#endif

    } else {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "u2h fifo len error %d %d %x %x %x", used_samples, samples_num, tick_diff, tlkusb_uacmic_ppmpara.tick_last,
                     tlkusb_uacmic_ppmpara.tick_rem);
    }
}

//static signed short sin_16k[]__attribute__((aligned(4))) = {
//    0, 12539, 23169, 30272, 32767, 30272, 23169, 12539, 4, -12539, -23169, -30272, -32767, -30272, -23169, -12539,
//};
//
//static signed short sin_48k_mono[] __attribute__((aligned(4))) = {
//    0,
//    1069,
//    2120,
//    3134,
//    4095,
//    4986,
//    5792,
//    6499,
//    7094,
//    7568,
//    7912,
//    8121,
//    8191,
//    8121,
//    7912,
//    7568,
//    7094,
//    6499,
//    5792,
//    4986,
//    4096,
//    3134,
//    2120,
//    1069,
//    0,
//    -1069,
//    -2120,
//    -3134,
//    -4095,
//    -4986,
//    -5792,
//    -6499,
//    -7094,
//    -7568,
//    -7912,
//    -8121,
//    -8191,
//    -8121,
//    -7912,
//    -7568,
//    -7094,
//    -6499,
//    -5792,
//    -4986,
//    -4096,
//    -3134,
//    -2120,
//    -1069,
//};

/**
 * @brief     Write one channel sample data to the USB endpoint
 * @param[in] sample_data Sample data to write
 * @return    none
 */
#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
__INLINE void tlkusb_iso_in_write_ep_one_chn(int32_t sample_data)
{
    reg_usb_ep_dat(TLKUSB_UAC_EDP_MIC) = sample_data;
    reg_usb_ep_dat(TLKUSB_UAC_EDP_MIC) = sample_data >> 8;
    reg_usb_ep_dat(TLKUSB_UAC_EDP_MIC) = sample_data >> 16;
}
#else
__INLINE void tlkusb_iso_in_write_ep_one_chn(int16_t sample_data)
{
    reg_usb_ep_dat(TLKUSB_UAC_EDP_MIC) = sample_data;
    reg_usb_ep_dat(TLKUSB_UAC_EDP_MIC) = sample_data >> 8;
}
#endif

/**
 * @brief     Fill USB microphone data endpoint with audio data
 * @param[in] tick Current system tick
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void tlkusb_uacmic_fillData(uint32_t tick)
{
    uint8_t need_samples = 0;

#if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
    int32_t data = 0;
#if (TLKLIB_UAC_PPM_MIC_ENABLE)
    static int32_t mic_data_last = 0;
    static int32_t mic_data_next = 0;
#endif
#else
    int16_t data = 0;
#if (TLKLIB_UAC_PPM_MIC_ENABLE)
    static int16_t mic_data_last = 0;
    static int16_t mic_data_next = 0;
#endif
#endif

    bool is_fill_mute = false;

    /*Only Mono is supported now.*/
    need_samples = (g_tlk_usb_cfg.in_sample_rate / 1000);

    usbhw_clr_eps_irq_inline(BIT(TLKUSB_UAC_EDP_MIC));
    uint32_t intvTick     = tick - g_tlk_usb_cfg.tick_in;
    g_tlk_usb_cfg.tick_in = tick;

#ifndef _B91_
    usbhw_reset_ep_ptr(TLKUSB_UAC_EDP_MIC);
#else
    reg_usb_ep_ptr(TLKUSB_UAC_EDP_MIC) = 0;
#endif

    if (intvTick > (SYSTEM_TIMER_TICK_1US * 1500) && (intvTick / SYSTEM_TIMER_TICK_1US < 10000)) {
        reg_usb_ep_ctrl(TLKUSB_UAC_EDP_MIC) = BIT(0);
        return;
    }
#if !TLKLIB_UAC_PPM_MIC_ENABLE
    if (tlkusb_uac_get_iso_in_used_samples() < need_samples) {
#else
    if (tlkusb_uac_get_ppm_in_used_samples() < need_samples) {
#endif
        is_fill_mute = true;
    } else {
    }

    if (!sTlkUsbUacMicEnable || is_fill_mute) {
        for (uint16_t index = 0; index < need_samples; index++) {
            tlkusb_iso_in_write_ep_one_chn(0x00);
        }
        reg_usb_ep_ctrl(TLKUSB_UAC_EDP_MIC) = BIT(0);
    } else {
        for (uint16_t index = 0; index < need_samples; index++) {
#if !TLKLIB_UAC_PPM_MIC_ENABLE
            data = g_tlk_usb_cfg.iso_in[g_tlk_usb_cfg.in_r & APP_USB_ISO_IN_BUFF_IDX_MASK];
            g_tlk_usb_cfg.in_r++;
            g_tlk_usb_cfg.in_r &= APP_USB_ISO_IN_BUFF_IDX_MASK;
#else
            data = g_tlk_usb_cfg.ppm_in[g_tlk_usb_cfg.ppm_in_r & APP_USB_PPM_IN_BUFF_IDX_MASK];
            g_tlk_usb_cfg.ppm_in_r++;
            g_tlk_usb_cfg.ppm_in_r &= APP_USB_PPM_IN_BUFF_IDX_MASK;
#endif

#if (TLKLIB_UAC_PPM_MIC_ENABLE)
            if (gLeaPpmStatus) {
                if ((mic_ppm_sample_counter != 0)) { //add sample
                    mic_data_next = g_tlk_usb_cfg.ppm_in[g_tlk_usb_cfg.ppm_in_r & APP_USB_PPM_IN_BUFF_IDX_MASK];
                    if ((data > 0 && mic_data_last < 0) || (data < 0 && mic_data_last > 0) || clock_time_exceed(mic_ppm_tick, (mic_over_time_us * 8 / 10))) { //80% overtime
                        if (mic_ppm_sample_counter < 0) {                                                                                                     //lose sample
                            mic_ppm_sample_counter++;
                            data = (abs_ram(data) < abs_ram(mic_data_last) ? mic_data_last : data);
                            g_tlk_usb_cfg.ppm_in_r--;
                            g_tlk_usb_cfg.ppm_in_r &= APP_USB_PPM_IN_BUFF_IDX_MASK;
                        } else if (mic_ppm_sample_counter > 0) { //insert sample
                            mic_ppm_sample_counter--;
                            g_tlk_usb_cfg.ppm_in_r++;
                            g_tlk_usb_cfg.ppm_in_r &= APP_USB_PPM_IN_BUFF_IDX_MASK;
                            data = (abs_ram(data) > abs_ram(mic_data_next) ? mic_data_next : data);
                        }
                    }
                }
                mic_data_last = data;
            }
#endif
            /*sin test*/
            //             if (g_tlk_usb_cfg.in_sample_rate == 48000) {
            //                 data = sin_48k_mono[index];
            //             } else {
            //                 data = sin_16k[index];
            //             }
            // #if (TLKUSB_AUD_MIC_RESOLUTION_BIT == 24)
            //             data = (data << 8) & 0x00ffffff;
            // #endif

            tlkusb_iso_in_write_ep_one_chn(data);
        }
#if (TLKLIB_UAC_PPM_MIC_ENABLE)
        if (gLeaPpmStatus) {
            mic_spk_samples = TICK_PER_MS / need_samples;
            if (mic_ppm_tick_last) {
                mic_tick_diff_val += tick - mic_ppm_tick_last - TICK_PER_MS;

                if (abs_ram(mic_tick_diff_val) >= mic_spk_samples * 2) {
                    if (mic_tick_diff_val < 0) {
                        mic_ppm_sample_counter--;
                        mic_tick_diff_val += mic_spk_samples;
                    } else if (mic_tick_diff_val > 0) {
                        mic_ppm_sample_counter++;
                        mic_tick_diff_val -= mic_spk_samples;
                    }
                    if (mic_ppm_tick > 0) {
                        mic_over_tick[mic_g_ppm_flag % 5] = tick - mic_ppm_tick; //SYSTEM_TIMER_TICK_1US
                        if (mic_over_tick[4] == 0) {
                            mic_over_time_us =
                                (mic_over_tick[0] + mic_over_tick[1] + mic_over_tick[2] + mic_over_tick[3] + mic_over_tick[4]) / (mic_g_ppm_flag + 1) / SYSTEM_TIMER_TICK_1US;
                        } else {
                            mic_over_time_us = (mic_over_tick[0] + mic_over_tick[1] + mic_over_tick[2] + mic_over_tick[3] + mic_over_tick[4]) / 5 / SYSTEM_TIMER_TICK_1US;
                        }
                        mic_g_ppm_flag++;
                        mic_g_ppm_flag %= 5;
                    }
                    mic_ppm_tick = tick;
                }
            }

            if (mic_ppm_tick_last == 0) {
                mic_tick_diff_val      = 0;
                mic_data_last          = 0;
                mic_ppm_sample_counter = 0;
                mic_g_ppm_flag         = 0;
                tmemcpy(mic_over_tick, 0, sizeof(mic_over_tick));
                mic_ppm_tick     = 0;
                mic_over_time_us = 0;
            }
            mic_ppm_tick_last = tick;
        }
#endif
        reg_usb_ep_ctrl(TLKUSB_UAC_EDP_MIC) = BIT(0);
    }

#if TLKLIB_UAC_PPM_MIC_ENABLE
    if (tlkusb_uac_get_state() == TLKUSB_UAC_VOICE) {
        g_usb_ppm.t_m_samples_cur += TLKALG_USB_OUT_PPM_SAMPLES;
        tlkalg_u2s_ppm_calculation(tick);
    }

#endif
#if TLKALG_PPM_CALC_BY_SAMPLE
    if (tlkalg_sample_ppm_mode_idle(&g_uac_ppm_ctrl)) {
        tlkalg_set_sample_ppm_mode(&g_uac_ppm_ctrl, ASRC_VOICE_MODE);
        tlkalg_reset_sample_ppm_calc(&g_uac_ppm_ctrl);
        tlkalg_set_sample_ppm_mask(&g_uac_ppm_ctrl, APP_USB_ISO_IN_BUFF_IDX_MASK);
        tlkalg_set_sample_ppm_channel(&g_uac_ppm_ctrl, tlkusb_uac_get_iso_in_Channels());
    }
#endif
}

#endif // #if (TLKUSB_UAC_MIC_ENABLE)
