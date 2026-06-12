/********************************************************************************************************
 * @file    ll_audio_dsp.c
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

#if (TLKSTK_BT_TPS_ENABLE)

#if ((TLK_MW_DSP_COMM_ENABLE && !TLKADU_MIDBUF_ENABLE) && !TLK_CFG_HRA_ENABLE)

int      ll_dsp_ret_data_buff[TLKALG_DSP_RET_NN_DATA_LEN_LL];
uint16_t ll_dsp_wptr = 0;
uint16_t ll_dsp_rptr = 0;

uint32_t g_ll_voice_alg_spk_rptr = 0;

/**
 * @brief      Synchronize microphone and speaker indices for low latency voice communication.
 * @param[in]  sample - Sample value for synchronization.
 * @return     None.
 */
void ll_voice_dsp_dp_sync_mic_spk_index(uint16_t sample)
{
    uint32_t mic_rptr       = tlkdrv_codec_sync_mic_samples(sample);
    g_ll_voice_alg_spk_rptr = (mic_rptr - 180) & (CODEC_SPK_FIFO_SAMPLES - 1);
}

/**
 * @brief      Retrieve microphone data for low latency voice communication.
 * @param[in]  p_des - Destination buffer for retrieved data.
 * @param[in]  samples - Number of samples to retrieve.
 * @return     Retrieved data size.
 */
audio_ram_code uint8_t ll_voice_dsp_dp_get_mic_data(int *p_des, uint16_t samples)
{
    adc_int mic_stereo_48k_24bit[480];


    int  buff_alg_in[480] = {0};
    int *pout             = buff_alg_in;

    tlkdrv_codec_readMicData((uint8_t *)mic_stereo_48k_24bit, samples * 3 * sizeof(adc_int), 0); //stereo
#if 0
    int *psrc = (int *)mic_stereo_48k_24bit;
    short mic_stereo_48k_16bit[480*2];
    short mic_stereo_16k_16bit[480*2];

    for (int j = 0; j < samples * 3 * 2; j++) { //24bit -> 16bit
        mic_stereo_48k_16bit[j] = (psrc[j] >> 8) & 0xffff;
    }
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
    p_audio_alg_if->audio_alg_process((uint8_t *)mic_stereo_48k_16bit, (uint8_t *)mic_stereo_16k_16bit, samples * 3, 0, 0);

    // mic data LRLRL... 16bit -> 24bit
    for (int k = 0; k < samples*2; k++) {
    	buff_alg_in[k] = (int)mic_stereo_16k_16bit[k] << 8;
    }
#else
    adc_int                mic_stereo_16k_24bit[160];
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_24BIT);
    p_audio_alg_if->audio_alg_process((uint8_t *)mic_stereo_48k_24bit, (uint8_t *)mic_stereo_16k_24bit, samples * 3, 0, 0);

    int *mic_psrc = (int *)mic_stereo_16k_24bit;
    for (int k = 0; k < samples * 2; k++) {
        buff_alg_in[k] = mic_psrc[k];
    }
#endif
    // spk data     +6 pick one sample from 3 stereo 48k samples
    int *spk_psrc = (int *)g_codec_spk_buff;
    for (int j = 0; j < samples; j++) {
        buff_alg_in[samples * 2 + j] = spk_psrc[g_ll_voice_alg_spk_rptr];
        g_ll_voice_alg_spk_rptr      = ((g_ll_voice_alg_spk_rptr + 6) & (CODEC_SPK_FIFO_SAMPLES - 1));
    }

    for (int k = 0; k < samples * 3; k++) {
        *p_des++ = *pout++;
    }

    return TRUE;
}

/**
 * @brief      Get available length of AVIA buffer.
 * @param[in]  wptr - Write pointer.
 * @param[in]  rptr - Read pointer.
 * @return     Available buffer length.
 */
uint16_t ll_voice_get_avia_len(uint16_t wptr, uint16_t rptr)
{
    uint16_t avia = 0;
    if (wptr >= rptr) {
        avia = wptr - rptr;
    } else {
        avia = TLKALG_DSP_RET_NN_DATA_LEN_LL + wptr - rptr;
    }
    return avia;
}

/**
 * @brief      Clear DSP return buffer status.
 * @param      None.
 * @return     None.
 */
void ll_voice_clear_dsp_ret_buff_status(void)
{
    for (int i = 0; i < TLKALG_DSP_RET_NN_DATA_LEN_LL; i++) {
        ll_dsp_ret_data_buff[i] = 0;
    }
    ll_dsp_wptr = 0;
    ll_dsp_rptr = 0;
}

/**
 * @brief      Process DSP messages for low latency voice communication.
 * @param[in]  enc_buff_wptr - Write pointer for encoded buffer.
 * @param[in]  type - Message type identifier.
 * @return     None.
 */
void ll_voice_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type)
{
    (void)enc_buff_wptr;
    (void)type;

    uint16_t pcm_data_len;
    // uint8_t enc_data[64];

    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&pcm_data_len, LL_VOICE_ID);
    if (pcm_data == NULL) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "ll voice pcm_data is NULL");
        return;
    }

    int *p_src = (int *)pcm_data;
    // tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "dsp ret len %d", pcm_data_len);
    for (int i = 0; i < pcm_data_len / 4; i++) {
        ll_dsp_ret_data_buff[ll_dsp_wptr++] = *p_src++;
        ll_dsp_wptr %= TLKALG_DSP_RET_NN_DATA_LEN_LL;
    }
}
#endif
#endif
