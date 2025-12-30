/********************************************************************************************************
 * @file    audio_common.c
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
#include "tlkmw/audio/tlkmw_audio.h"
#if TLK_ALG_HYBRID_ENABLE
#include "tlkalg/audio/hybrid/tlkalg_hybrid_interface.h"
#endif
#include "drivers.h"
#if TLKMDI_AUDIO_DB_CALC_ENABLE
#include <nds_utils_math.h>
#include <riscv_dsp_utils_math.h>
#endif

#if (TLK_MW_AUDIO_ENABLE)
static uint8_t s_tone_mix_mode = 0;
volatile uint32_t codec_flag = 0;

uint32_t g_alg_mic_rptr = 0;
uint32_t g_alg_mic_wptr = 0;
uint32_t g_alg_spk_rptr = 0;

#if TLK_ALG_HYBRID_ENABLE
/**
 * @brief       Synchronize mic and speaker index for hybrid algorithm
 * @param[in]   sample - Sample count
 * @return      None
 */
void tlkalg_sync_mic_spk_index(uint16_t sample)
{
    g_alg_mic_wptr = ((audio_get_rx_dma_wptr(TLKDRV_CODEC_MIC_DMA)) - ((uint32_t)g_codec_mic_buff)) / sizeof(adc_int);
    g_alg_mic_rptr = (g_alg_mic_wptr - sample) & (CODEC_MIC_FIFO_SAMPLES - 1);
    g_alg_spk_rptr = (g_alg_mic_rptr - hybrid_delay) & (CODEC_SPK_FIFO_SAMPLES - 1);
}
#endif

#if TLKALG_AGC_ENABLE
/**
 * @brief       Process AGC after neural network processing
 * @param[in]   ps - Source buffer pointer
 * @param[out]  pd - Destination buffer pointer
 * @return      None
 */
void tlkalg_agc_proc_behind_nn(uint8_t *ps, uint8_t *pd)
{
    //20ms frame length
    adc_mono_int dsp_ret_buff[MIC_FRAME_SIZE], agc_out_buff[MIC_FRAME_SIZE];
    adc_mono_int *psrc = (adc_mono_int*)ps;
    adc_mono_int *pdes = (adc_mono_int*)pd;
    for (int i = 0; i < MIC_FRAME_SIZE; i++) {
        dsp_ret_buff[i] = *psrc++;
    }

    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AGC);
    p_audio_alg_if->audio_alg_process((uint8_t *)dsp_ret_buff, (uint8_t *)agc_out_buff, 160, 0, 0);
    p_audio_alg_if->audio_alg_process((uint8_t *)(&dsp_ret_buff[160]), (uint8_t *)(&agc_out_buff[160]), 160, 0, 0);

    for(int j = 0; j < MIC_FRAME_SIZE; j++) {
        *pdes++ = agc_out_buff[j];
    }
}
#endif

#if TLKALG_ANS_ENABLE
/**
 * @brief       Process ANS after neural network processing
 * @param[in]   ps - Source buffer pointer
 * @param[out]  pd - Destination buffer pointer
 * @return      None
 */
void tlkalg_ans_proc_behind_nn(uint8_t *ps, uint8_t *pd)
{
    //20ms frame length
    adc_mono_int dsp_ret_buff[MIC_FRAME_SIZE], agc_out_buff[MIC_FRAME_SIZE];
    adc_mono_int *psrc = (adc_mono_int*)ps;
    adc_mono_int *pdes = (adc_mono_int*)pd;
    for (int i = 0; i < MIC_FRAME_SIZE; i++) {
        dsp_ret_buff[i] = *psrc++;
    }

    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ANS);
    p_audio_alg_if->audio_alg_process((uint8_t *)dsp_ret_buff, (uint8_t *)agc_out_buff, 80, 0, 0);
    p_audio_alg_if->audio_alg_process((uint8_t *)(&dsp_ret_buff[80]), (uint8_t *)(&agc_out_buff[80]), 80, 0, 0);
    p_audio_alg_if->audio_alg_process((uint8_t *)(&dsp_ret_buff[160]), (uint8_t *)(&agc_out_buff[160]), 80, 0, 0);
    p_audio_alg_if->audio_alg_process((uint8_t *)(&dsp_ret_buff[240]), (uint8_t *)(&agc_out_buff[240]), 80, 0, 0);

    for(int j = 0; j < MIC_FRAME_SIZE; j++) {
        *pdes++ = agc_out_buff[j];
    }
}
#endif

/**
 * @brief       Check if ANC BT music is busy (without macro)
 * @return      True if busy, false otherwise
 */
bool tlkmdi_anc_btmusic_is_busy_no_macro(void)
{
    #if TLKALG_ANC_ENABLE
    return tlkmdi_anc_btmusic_is_busy();
    #else
    return 0;
    #endif
}

/**
 * @brief       Check if ANC BT voice is busy (without macro)
 * @return      True if busy, false otherwise
 */
bool tlkmdi_anc_btvoice_is_busy_no_macro(void)
{
    #if TLKALG_ANC_ENABLE
    return tlkmdi_anc_btvoice_is_busy();
    #else
    return 0;
    #endif
}

/**
 * @brief       Inform DSP synchronization for ANC
 * @return      None
 */
void tlkmdi_anc_inform_dsp_sync(void)
{
#if (TLKALG_ANC_ENABLE && TLK_MW_DSP_COMM_ENABLE)
    if (tlkmdi_anc_is_busy()) {
        d25f_set_dsp_anc_sync_enable();
    }
#endif
}

#if(TLKALG_ANS_SPK_ENABLE)
/**
 * @brief       Process multi-frame ANS for speaker
 * @param[in]   ps - Source buffer pointer
 * @param[out]  pd - Destination buffer pointer
 * @param[in]   frame_num - Number of frames
 * @return      None
 * @note        Process 20ms frame length
 */
void tlkalg_ans_multiframe(uint8_t *ps, uint8_t *pd ,uint8_t frame_num)
{
    //20ms frame length
    adc_mono_int ans_in[MIC_FRAME_SIZE*2], ans_out[MIC_FRAME_SIZE*2];
    adc_mono_int *psrc = (adc_mono_int*)ps;
    adc_mono_int *pdes = (adc_mono_int*)pd;
    for (int i = 0; i < MIC_FRAME_SIZE; i++) {
    	ans_in[i] = *psrc++;
    	ans_in[i] = *psrc++;
    }

#if 1
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SPK_ANS);
	for(int i=0;i<frame_num;i++) {
		p_audio_alg_if->audio_alg_process((uint8_t *)(&ans_in[80*i]), (uint8_t *)(&ans_out[80*i]), 80, 0, 0);
	}
//    (void)frame_num;
//    p_audio_alg_if->audio_alg_process((uint8_t *)ans_in, (uint8_t *)ans_out, 80, 0, 0);
//	p_audio_alg_if->audio_alg_process((uint8_t *)(&ans_in[80]), (uint8_t *)(&ans_out[80]), 80, 0, 0);
//	p_audio_alg_if->audio_alg_process((uint8_t *)(&ans_in[160]), (uint8_t *)(&ans_out[160]), 80, 0, 0);
//	p_audio_alg_if->audio_alg_process((uint8_t *)(&ans_in[240]), (uint8_t *)(&ans_out[240]), 80, 0, 0);
#else
    (void)frame_num;
    for (int k = 0; k < MIC_FRAME_SIZE; k++) {
    	ans_out[k] = ans_in[k];
        }
#endif

    for(int j = 0; j < MIC_FRAME_SIZE; j++) {
        *pdes++ = ans_out[j];
        *pdes++ = ans_out[j];
    }
}
#endif

#if 0
/**
 * @brief       Get microphone frame data
 * @param[out]  pdes - Destination buffer pointer
 * @param[in]   len - Data length
 * @return      Status
 */
uint8_t tlkalg_get_mic_frame(int16_t *pdes, uint16_t len)
{
    uint8_t ret              = 1;
    short   buff_alg_in[360] = {0};

    // short pcm_stereo[240] = {0};
    #if (TLKALG_AGC_ENABLE)
    short buff_hybrid_out[120];
    #endif
    #if TLKALG_AGC_ENABLE
    short buff_agc_out[120];
    #endif

    #if (TLKALG_AGC_ENABLE || TLKALG_AGC_ENABLE)
    short                 *mic_ptr = buff_alg_in;
    audio_alg_interface_t *p_audio_alg_if;
    #endif
    short *pout = buff_alg_in;


    //    g_alg_mic_wptr = tlkdrv_codec_get_mic_wptr();
    //    uint16_t mic_samples = (g_alg_mic_wptr - g_alg_mic_rptr) & (CODEC_MIC_FIFO_SAMPLES - 1);

    //get mic data     stereo
    for (int i = 0; i < len; i++) {
        buff_alg_in[i]       = g_codec_mic_buff[(g_alg_mic_rptr + i) & (CODEC_MIC_FIFO_SAMPLES - 1)] & 0xffff;
        buff_alg_in[i + len] = 0;
    }
    g_alg_mic_rptr = (g_alg_mic_rptr + len) & (CODEC_MIC_FIFO_SAMPLES - 1);

    #if TLK_ALG_HYBRID_ENABLE
    //get spk data
    if (alg_ctrl & AEC_CTRL_MASK) {
        for (int j = 0; j < len; j++) {
            buff_alg_in[len * 2 + j] = g_codec_spk_buff[(g_alg_spk_rptr + j) & (CODEC_SPK_FIFO_SAMPLES - 1)];
        }
    }

    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_HYBRID);
    p_audio_alg_if->audio_alg_process((uint8_t *)mic_ptr, (uint8_t *)buff_hybrid_out, len, 0, 0);
    mic_ptr = pout = buff_hybrid_out;
    #endif

    #if TLKALG_AGC_ENABLE
    p_audio_alg_if = audio_alg_get_interface_by_type(ALG_AGC);
    p_audio_alg_if->audio_alg_process((uint8_t *)mic_ptr, (uint8_t *)buff_agc_out, len, 0, 0);
    pout = buff_agc_out;
    #endif

    for (int k = 0; k < len; k++) {
        *pdes++ = *pout++;
    }

    return ret;
}
#endif

/**
 * @brief       Set tone mix mode
 * @param[in]   mode - Tone mix mode
 * @return      None
 */
void tlkmdi_audio_set_tone_mix_mode(uint8_t mode)
{
    s_tone_mix_mode = mode;
}

/**
 * @brief       Get tone mix mode
 * @return      Current tone mix mode
 */
uint8_t tlkmdi_audio_get_tone_mix_mode(void)
{
    return s_tone_mix_mode;
}

/**
 * @brief       Get the codec DAC type
 * @return      Current codec DAC type
 */
AUDIO_CODEC_DAC_TYPE_ENUM tlkmdi_audio_get_codec_dac_type(void)
{
#if AUDIO_TWS_MODE
    return AUDIO_CODEC_DAC_MONO;
#else
    return AUDIO_CODEC_DAC_STEREO;
#endif
}

#if TLKMDI_AUDIO_DB_CALC_ENABLE
/**
 * @brief       Calculate dB value for 16-bit audio data
 * @param[in]   data - Audio data pointer
 * @param[in]   len - Data length
 * @return      RMS value in dB
 */
float tlkmdi_audio_dB_calc_16bit(short *data, uint16_t len)
{
    float temp = 0.0;
    float rms_frame = 0.0;
    float rms_frame_db = 0.0;
    for (int i = 0; i < len; i++) {
        temp = data[i];
        temp /= (1 << 15);//16bit-15    24bit-23
        rms_frame += temp * temp;
    }
    rms_frame = riscv_dsp_sqrt_f32(rms_frame / len);
    rms_frame_db = 20.0 * (riscv_dsp_log_f32(rms_frame)/riscv_dsp_log_f32(10.0));

    // if (rms_frame_db < mute_threshold) {
    //     for (int j = 0; j < len; j++) {
    //         data[j] = 0;
    //     }
        // tlkapi_printf(APP_LOG_EN, "mute en %fdB", rms_frame_db);
    // }

    return rms_frame_db;
}

/**
 * @brief       Check if audio dB is less than threshold
 * @param[in]   thr - Threshold value
 * @param[in]   data - Audio data pointer
 * @param[in]   len - Data length
 * @return      True if less than threshold, false otherwise
 */
bool tlkmdi_audio_dB_is_less_threshold(float thr, short *data, uint16_t len)
{
    if (tlkmdi_audio_dB_calc_16bit(data, len) < thr) {
        return true;
    } else {
        return false;
    }
}
#endif
#endif
