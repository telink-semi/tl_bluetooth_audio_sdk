/********************************************************************************************************
 * @file    tlkalg_eq_interface.c
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
#include "tlkalg/audio/audio_alg_interface.h"

#if TLKALG_EQ_ENABLE
#include "tlka_eq_api.h"
#include "tlkalg_eq_interface.h"

#define EQ_VOICE_ENABLE           1
#define TLKALG_EQ_PARA_FLASH_ADDR 2

eq_para_t s_eq_param = {0};

static eq_mode_t s_eq_filter_music;
static eq_mode_t s_eq_filter_voice_mic;
static eq_mode_t s_eq_filter_voice_spk;

static float s_state_eq_voice_mic_left[4 * EQ_NSTAGE_VOICE_MIC_MAX]      = {0.0};
static float s_state_eq_voice_speaker_left[4 * EQ_NSTAGE_VOICE_SPK_MAX]  = {0.0};
static float s_state_eq_voice_speaker_right[4 * EQ_NSTAGE_VOICE_SPK_MAX] = {0.0};
static float s_state_eq_music_left[4 * EQ_NSTAGE_MUSIC_SPK_MAX]          = {0.0};
static float s_state_eq_music_right[4 * EQ_NSTAGE_MUSIC_SPK_MAX]         = {0.0};

static float s_coeff_music[EQ_NSTAGE_MUSIC_SPK_MAX * 5]     = {0.0};
static float s_coeff_voice_mic[EQ_NSTAGE_VOICE_MIC_MAX * 5] = {0.0};
static float s_coeff_voice_spk[EQ_NSTAGE_VOICE_SPK_MAX * 5] = {0.0};

static nds_bq_df1_f32_t s_instance_eq_run_left  = {0};
static nds_bq_df1_f32_t s_instance_eq_run_right = {0};
// static nds_bq_df1_f32_t* s_instance_eq_run_ptr_left           = NULL;
// static nds_bq_df1_f32_t* s_instance_eq_run_ptr_right          = NULL;


#if ((PROJ_RECORDING_CARD) && (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN) && (TLKALG_BONE_CODUCTION_EN))
#if (BONE_TYPE_SEL == DIGITAL_BONE_MIC)
static const unsigned char eq_digital_bone_mic_default_tab[] = {
    0x00, 0x00, 0x02, 0x18, 0x00, 0x00, 0x78, 0x00, 0xb0, 0x04, 0x02, 0x00, 0x5c, 0xf9, 0x50, 0x14, 0x60, 0x09, 0x00, 0x00,
};
#else
static const unsigned char eq_analog_bone_mic_default_tab[] = {
    0x00, 0x00, 0x02, 0x18, 0x00, 0x00, 0x78, 0x00, 0xb0, 0x04, 0x02, 0x00, 0x5c, 0xf9, 0xc0, 0x19, 0x60, 0x09, 0x00, 0x00,
};
#endif

#else

static const unsigned char eq_sys_default_tab[] = {
    /// 0826
    0x00, 0x00, 0x09, 0x15, 0x38, 0xff, 0x14, 0x00, 0xe8, 0x03, 0x02, 0x00, 0xae, 0xfc, 0x96, 0x00, 0xbc, 0x02, 0x00, 0x00, 0x70, 0xfe, 0x2c, 0x01, 0xbc, 0x02, 0x00,
    0x00, 0x3e, 0xfe, 0xe8, 0x03, 0xe8, 0x03, 0x00, 0x00, 0xc2, 0x01, 0xc4, 0x09, 0xdc, 0x05, 0x00, 0x00, 0xa2, 0xfe, 0xd8, 0x0e, 0xe8, 0x03, 0x00, 0x00, 0xa8, 0xfd,
    0x88, 0x13, 0xd0, 0x07, 0x00, 0x00, 0xb4, 0xfb, 0xf4, 0x1a, 0xd0, 0x07, 0x00, 0x00, 0x2c, 0x01, 0xc8, 0x32, 0xe8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /// CRC
};

// static const unsigned char eq_sys_default_tab[] = {
//     ///10stages
//     0x00, 0x00, 0x0a, 0x74,
//     0x30, 0xf8, 0x32, 0x00, 0xd0, 0x07, 0x00, 0x01,
//     0xb0, 0x04, 0x10, 0x09, 0xd0, 0x07, 0x00, 0x01,
//     0x50, 0xfb, 0xee, 0x11, 0xd0, 0x07, 0x00, 0x01,
//     0x40, 0x06, 0xd6, 0x1a, 0xd0, 0x07, 0x00, 0x01,
//     0x40, 0x06, 0xb4, 0x23, 0xd0, 0x07, 0x00, 0x01,
//     0xf0, 0xf1, 0x9c, 0x2c, 0xd0, 0x07, 0x00, 0x01,
//     0x50, 0xfb, 0x7a, 0x35, 0xd0, 0x07, 0x00, 0x01,
//     0xb0, 0x04, 0x58, 0x3e, 0xd0, 0x07, 0x00, 0x01,
//     0x40, 0x06, 0x74, 0x40, 0xd0, 0x07, 0x00, 0x01,
//     0x50, 0xfb, 0x44, 0x48, 0xd0, 0x07, 0x00, 0x01,
// };


static const unsigned char eq_voice_mic_default_tab[] = {
    /// ZZ
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xaa, 0x00, 0xe8, 0x03, 0x02, 0x00, 0x90, 0x01, 0x08, 0x07, 0xc8, 0x00, 0x00, 0x00,
};

static const unsigned char eq_voice_spk_default_tab[] = {
    /// highpass lowpass
    0x00, 0x00, 0x02, 0x05, 0x00, 0x00, 0xb4, 0x00, 0x84, 0x03, 0x02, 0x00, 0x00, 0x00, 0xb8, 0x0b, 0x84, 0x03, 0x01, 0x00,
};

#endif

/**
 * @brief   Resets all EQ states to zero.
 * @return  none.
 */
_attribute_ram_code_ void tlkalg_eq_reset_all_state(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_eq_reset_all_state");

    tmemset(&s_state_eq_music_left, 0, sizeof(s_state_eq_music_left));
    tmemset(&s_state_eq_music_right, 0, sizeof(s_state_eq_music_right));

    tmemset(&s_state_eq_voice_mic_left, 0, sizeof(s_state_eq_voice_mic_left));

    tmemset(&s_state_eq_voice_speaker_left, 0, sizeof(s_state_eq_voice_speaker_left));
    tmemset(&s_state_eq_voice_speaker_right, 0, sizeof(s_state_eq_voice_speaker_right));
}

/**
 * @brief   Calculates the coefficients for the EQ filter stages.
 * @param[in]   samplerate    - The sample rate of the audio.
 * @param[in]   filter      - Pointer to the EQ filter structure.
 * @param[out]  coeff       - Pointer to the buffer where coefficients will be stored.
 */
void tlkalg_eq_calculate_coefficient(int samplerate, eq_mode_t *filter, float *coeff)
{
    float              q_value     = 0.0;
    float              gain        = 0.0;
    float              eq_all_gain = 0.0;
    int                frequency   = 0;
    e_eq_filter_type_e type;
    int                ns = filter->eq_header.filter_sum;

    if (filter->eq_header.all_gain > 0) {
        eq_all_gain = (float)(filter->eq_header.all_gain / 10.0);
    }

    for (int i = 0; i < ns; i++) {
        type      = filter->eq_para[i].filter_type;
        q_value   = filter->eq_para[i].filter_q / 1000.0;
        frequency = filter->eq_para[i].freq_c;
        gain      = filter->eq_para[i].gain / 100.0;

        eq_calculate_coefficient_per_stage(samplerate, type, frequency, q_value, gain, (coeff + i * 5));
        if (i == filter->eq_header.max_gain_num) {
            float N          = powf(10.0, eq_all_gain / 20.0);
            coeff[i * 5]     = coeff[i * 5] / N;
            coeff[i * 5 + 1] = coeff[i * 5 + 1] / N;
            coeff[i * 5 + 2] = coeff[i * 5 + 2] / N;
        }
    }
}

/**
 * @brief   Reads EQ parameters from flash memory.
 * @param[in]   addr    - The address in flash memory to read from.
 */
void tlkalg_eq_read_para_from_flash(uint32_t addr)
{
    if (addr != 0) {
        //read flash and copy data to s_eq_filter_music/mic/spk
    } else {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "invalid eq para load flash addr");
#if ((PROJ_RECORDING_CARD) && (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN) && (TLKALG_BONE_CODUCTION_EN))
#if (BONE_TYPE_SEL == DIGITAL_BONE_MIC)
        tmemcpy(&s_eq_filter_voice_mic, eq_digital_bone_mic_default_tab, sizeof(eq_digital_bone_mic_default_tab));
#elif (BONE_TYPE_SEL == ANALOG_BONE_MIC)
        tmemcpy(&s_eq_filter_voice_mic, eq_analog_bone_mic_default_tab, sizeof(eq_analog_bone_mic_default_tab));
#endif
#else
#if (EQ_VOICE_ENABLE)
        tmemcpy(&s_eq_filter_voice_mic, eq_voice_mic_default_tab, sizeof(eq_voice_mic_default_tab));
        tmemcpy(&s_eq_filter_voice_spk, eq_voice_spk_default_tab, sizeof(eq_voice_spk_default_tab));
#endif
        tmemcpy(&s_eq_filter_music, eq_sys_default_tab, sizeof(eq_sys_default_tab));
#endif
    }
}

/**
 * @brief   Sets the EQ parameters for a given mode.
 * @param[in]   para    - Pointer to the EQ mode structure.
 */
void tlkalg_tool_para(eq_mode_t *para)
{
    tmemcpy((uint8_t *)&s_eq_filter_music, (uint8_t *)para, 4);
    tmemcpy((uint8_t *)&s_eq_filter_music + 4, (uint8_t *)para + 8, sizeof(eq_mode_t) - 8);
    tlkalg_eq_reset_all_state();
    tlkalg_eq_calculate_coefficient(s_eq_param.samplerate, &s_eq_filter_music, s_coeff_music);
}

/**
 * @brief   Initializes the EQ module.
 * @param[in]   p_buff    - Pointer to the buffer (unused).
 * @param[in]   channel   - The channel to initialize (unused).
 * @return      0 for successful initialization, -1 otherwise.
 */
int8_t tlkalg_eq_init(uint8_t *p_buff, uint8_t channel)
{
    (void)p_buff;
    (void)channel;

    tlkalg_eq_reset_all_state();

#if ((PROJ_RECORDING_CARD) && (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN) && (TLKALG_BONE_CODUCTION_EN))
    tlkalg_eq_calculate_coefficient(16000, &s_eq_filter_voice_mic, s_coeff_voice_mic);
#else
#if (EQ_VOICE_ENABLE)
    tlkalg_eq_calculate_coefficient(16000, &s_eq_filter_voice_mic, s_coeff_voice_mic);
    tlkalg_eq_calculate_coefficient(16000, &s_eq_filter_voice_spk, s_coeff_voice_spk);
#endif

    tlkalg_eq_calculate_coefficient(s_eq_param.samplerate, &s_eq_filter_music, s_coeff_music);
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_eq_init samplerate %d", s_eq_param.samplerate);
#endif

    return 0;
}

/**
 * @brief   Deinitializes the EQ module.
 * @return      0 for successful deinitialization, -1 otherwise.
 */
int8_t tlkalg_eq_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_eq_deinit");
    tlkalg_eq_reset_all_state();

    tmemset(&s_instance_eq_run_right, 0, sizeof(nds_bq_df1_f32_t));
    tmemset(&s_instance_eq_run_left, 0, sizeof(nds_bq_df1_f32_t));

    return 0;
}

/**
 * @brief   Sets the EQ parameters for the right channel.
 * @param[in]   eq_type - The type of EQ to apply.
 */
void tlkalg_eq_set_right_channel_para(e_eq_type_e eq_type)
{
    if (EQ_TYPE_VOICE_SPK == eq_type) {
        s_instance_eq_run_right.coeff  = s_coeff_voice_spk;
        s_instance_eq_run_right.nstage = s_eq_filter_voice_spk.eq_header.filter_sum;
        s_instance_eq_run_right.state  = s_state_eq_voice_speaker_right;
    } else if (EQ_TYPE_VOICE_MIC == eq_type) {
        s_instance_eq_run_right.coeff  = s_coeff_voice_mic;
        s_instance_eq_run_right.nstage = s_eq_filter_voice_mic.eq_header.filter_sum;
        s_instance_eq_run_right.state  = s_state_eq_voice_mic_left;
    } else if (EQ_TYPE_MUSIC == eq_type) {
        s_instance_eq_run_right.coeff  = s_coeff_music;
        s_instance_eq_run_right.nstage = s_eq_filter_music.eq_header.filter_sum;
        s_instance_eq_run_right.state  = s_state_eq_music_right;
    }
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "set right para stage:%d type:%d", s_instance_eq_run_right.nstage, eq_type);
}

/**
 * @brief   Sets the EQ parameters for the left channel.
 * @param[in]   eq_type - The type of EQ to apply.
 */
void tlkalg_eq_set_left_channel_para(e_eq_type_e eq_type)
{
    if (EQ_TYPE_VOICE_SPK == eq_type) {
        s_instance_eq_run_left.coeff  = s_coeff_voice_spk;
        s_instance_eq_run_left.nstage = s_eq_filter_voice_spk.eq_header.filter_sum;
        s_instance_eq_run_left.state  = s_state_eq_voice_speaker_left;
    } else if (EQ_TYPE_VOICE_MIC == eq_type) {
        s_instance_eq_run_left.coeff  = s_coeff_voice_mic;
        s_instance_eq_run_left.nstage = s_eq_filter_voice_mic.eq_header.filter_sum;
        s_instance_eq_run_left.state  = s_state_eq_voice_mic_left;
    } else if (EQ_TYPE_MUSIC == eq_type) {
        s_instance_eq_run_left.coeff  = s_coeff_music;
        s_instance_eq_run_left.nstage = s_eq_filter_music.eq_header.filter_sum;
        s_instance_eq_run_left.state  = s_state_eq_music_left;
    }
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "set left para stage:%d type:%d", s_instance_eq_run_left.nstage, eq_type);
}

/**
 * @brief   Sets the EQ parameters based on the type and channel.
 * @param[in]   type    - The type of EQ parameter to set.
 * @param[in]   param   - Pointer to the EQ parameter structure.
 * @return      0 for successful parameter setting, -1 otherwise.
 */
uint8_t tlkalg_eq_param_set(uint8_t type, void *param)
{
    (void)type;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_eq_param_set");
    tmemcpy(&s_eq_param, param, sizeof(eq_para_t));

    if (ALG_CHANNEL_RIGHT == s_eq_param.channel) {
        tlkalg_eq_set_right_channel_para(s_eq_param.eq_type);
    } else if (ALG_CHANNEL_LEFT == s_eq_param.channel) {
        tlkalg_eq_set_left_channel_para(s_eq_param.eq_type);
    } else if (ALG_CHANNEL_STEREO == s_eq_param.channel) {
        tlkalg_eq_set_left_channel_para(s_eq_param.eq_type);
        tlkalg_eq_set_right_channel_para(s_eq_param.eq_type);
    }

    return 0;
}

/**
 * @brief   Processes audio data using EQ for 16-bit samples.
 * @param[in]   ps      - Pointer to the source audio data.
 * @param[out]  pd      - Pointer to the destination audio data.
 * @param[in]   len     - Length of the audio data to process.
 * @param[in]   channel - The channel to process.
 * @return      0 for successful processing, -1 otherwise.
 */
int tlkalg_eq_process_16bit(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t channel)
{
    short *psrc = (short *)ps;
    short *pdes = (short *)pd;
    int    ret  = 0;

    int length = len;
    while (length > 0) {
        uint16_t ni = length > EQ_SAMPLE_NUM_MAX ? EQ_SAMPLE_NUM_MAX : length;
        length -= ni;

        if (channel == ALG_CHANNEL_RIGHT) {
            ret = eq_process_int16(&s_instance_eq_run_right, psrc, pdes, ni);
            psrc += ni;
            pdes += ni;
        } else if (channel == ALG_CHANNEL_LEFT) {
            ret = eq_process_int16(&s_instance_eq_run_left, psrc, pdes, ni);
            psrc += ni;
            pdes += ni;
        } else if (channel == ALG_CHANNEL_STEREO) {
            short data_in_left[EQ_SAMPLE_NUM_MAX], data_in_right[EQ_SAMPLE_NUM_MAX];
            short data_out_left[EQ_SAMPLE_NUM_MAX], data_out_right[EQ_SAMPLE_NUM_MAX];
            for (int i = 0; i < ni; i++) {
                data_in_left[i]  = psrc[2 * i];
                data_in_right[i] = psrc[2 * i + 1];
            }
            ret = eq_process_int16(&s_instance_eq_run_left, data_in_left, data_out_left, ni);
            ret = eq_process_int16(&s_instance_eq_run_right, data_in_right, data_out_right, ni);
            for (int j = 0; j < ni; j++) {
                pdes[2 * j]     = data_out_left[j];
                pdes[2 * j + 1] = data_out_right[j];
            }
            psrc += (ni * 2);
            pdes += (ni * 2);
        } else {
            ret = -1;
        }
    }
    return ret;
}

/**
 * @brief   Processes audio data using EQ for 24-bit samples.
 * @param[in]   ps      - Pointer to the source audio data.
 * @param[out]  pd      - Pointer to the destination audio data.
 * @param[in]   len     - Length of the audio data to process.
 * @param[in]   channel - The channel to process.
 * @return      0 for successful processing, -1 otherwise.
 */
int tlkalg_eq_process_24bit(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t channel)
{
    int *psrc = (int *)ps;
    int *pdes = (int *)pd;
    int  ret  = 0;

    int length = len;
    while (length > 0) {
        uint16_t ni = length > EQ_SAMPLE_NUM_MAX ? EQ_SAMPLE_NUM_MAX : length;
        length -= ni;

        if (channel == ALG_CHANNEL_RIGHT) {
            ret = eq_process_int32(&s_instance_eq_run_right, psrc, pdes, ni);
            psrc += ni;
            pdes += ni;
        } else if (channel == ALG_CHANNEL_LEFT) {
            ret = eq_process_int32(&s_instance_eq_run_left, psrc, pdes, ni);
            psrc += ni;
            pdes += ni;
        } else if (channel == ALG_CHANNEL_STEREO) {
            int data_in_left[ni], data_in_right[ni];
            int data_out_left[ni], data_out_right[ni];
            for (int i = 0; i < ni; i++) {
                data_in_left[i]  = psrc[2 * i];
                data_in_right[i] = psrc[2 * i + 1];
            }
            ret = eq_process_int32(&s_instance_eq_run_left, data_in_left, data_out_left, ni);
            ret = eq_process_int32(&s_instance_eq_run_right, data_in_right, data_out_right, ni);
            for (int j = 0; j < ni; j++) {
                pdes[2 * j]     = data_out_left[j];
                pdes[2 * j + 1] = data_out_right[j];
            }
            psrc += (ni * 2);
            pdes += (ni * 2);
        } else {
            ret = -1;
        }
    }

    return ret;
}

/**
 * @brief   Processes audio data using EQ based on the sample width.
 * @param[in]   ps      - Pointer to the source audio data.
 * @param[out]  pd      - Pointer to the destination audio data.
 * @param[in]   len     - Length of the audio data to process.
 * @param[in]   width   - The width of the audio samples (16-bit or 24-bit).
 * @param[in]   channel - The channel to process.
 * @return      0 for successful processing, -1 otherwise.
 */
int tlkalg_eq_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    if (ps == NULL || pd == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "eq process pointer null");
        return 0;
    }


    if (width == ALG_WIDTH_16) {
        tlkalg_eq_process_16bit(ps, pd, len, channel);
    } else if (width == ALG_WIDTH_24) {
        tlkalg_eq_process_24bit(ps, pd, len, channel);
    }

    return 0;
}

#endif
