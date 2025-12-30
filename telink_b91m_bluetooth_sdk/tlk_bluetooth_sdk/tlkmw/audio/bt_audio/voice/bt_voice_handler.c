/********************************************************************************************************
 * @file    bt_voice_handler.c
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
#include "tlkmw/common/tlkaudio_debug_data.h"
#include "tlkmw/audio/tlkmw_audio.h"
#include "drivers.h"
#include "tlkmw/sys_dev/tlkmw_sysdev.h"
#include "tlkalg/audio/audio_alg_interface.h"

#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpt_state.h"

#ifndef MSBC_SYNCWORD
#define MSBC_SYNCWORD 0xAD
#endif

#ifndef CODEC_MUTE_MIC_EN
#define CODEC_MUTE_MIC_EN 0
#endif

#ifndef LE_AUDIO_BT_VOICE_MIX_ENABLE
#define LE_AUDIO_BT_VOICE_MIX_ENABLE 0
#endif

#ifndef BT_AUDIO_BUFFER_CHECK
#define BT_AUDIO_BUFFER_CHECK 0
#endif

#define PCM_BLOCK_SIZE 136

#if (TLKBTP_CFG_HFP_ENABLE)
#if (TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
#define TLKALG_DSP_RET_NN_DATA_LEN     (320 * 2)
#define TLKALG_DSP_RET_BYTE_PER_SAMPLE 2
adc_mono_int    dsp_ret_data_buff[TLKALG_DSP_RET_NN_DATA_LEN];
static uint16_t bt_dsp_wptr = 0;
static uint16_t bt_dsp_rptr = 0;
#endif

bt_voice_cfg_t bt_voice_cfg = {
    .tick = 0,

    .spk_enc_rptr = 0,
    .spk_enc_wptr = 0,
    .spk_pcm_rptr = 0,
    .spk_pcm_wptr = 0,

    .mic_enc_rptr = 0,
    .mic_enc_wptr = 0,
    .mic_pcm_rptr = 0,
    .mic_pcm_wptr = 0,

    .status = 0,
    .init   = 0,

    .sync_init = 0,
};

uint8_t *g_spk_enc_buff_ptr = NULL;
uint8_t *g_mic_enc_buff_ptr = NULL;

uint8_t voice_msbc_silence_pkt[60] = {0x01, 0x08, 0xad, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, 0x00, 0x77, 0x6d, 0xb6, 0xdd, 0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6d,
                                      0xdd, 0xb6, 0xdb, 0x77, 0x6d, 0xb6, 0xdd, 0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6d, 0xdd, 0xb6, 0xdb, 0x77, 0x6d, 0xb6, 0xdd,
                                      0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6d, 0xdd, 0xb6, 0xdb, 0x77, 0x6d, 0xb6, 0xdd, 0xdb, 0x6d, 0xb7, 0x76, 0xdb, 0x6c, 0x00};

#if 0
signed short sin_48k_mono_d1_bbb[] __attribute__((aligned(4))) = {
    0,     1069,  2120,  3134,  4095,  4986,  5792,  6499,  7094,  7568,
    7912,  8121,  8191,  8121,  7912,  7568,  7094,  6499,  5792,  4986,
    4096,  3134,  2120,  1069,  0,    -1069, -2120, -3134, -4095, -4986,
    -5792, -6499, -7094, -7568, -7912, -8121, -8191, -8121, -7912, -7568,
    -7094, -6499, -5792, -4986, -4096, -3134, -2120, -1069
};

signed short sin_16k_mono_bbb[16] = {
    0, 12539, 23169, 30272, 32767, 30272, 23169, 12539,
    4, -12539, -23169, -30272, -32767, -30272, -23169, -12539
};
#endif

/**
 * @brief   Get BT voice environment pointer
 * @param[in]  None
 * @returns    Pointer to bt_voice_cfg_t structure
 */
audio_ram_code bt_voice_cfg_t *get_bt_voice_env_ptr(void)
{
    return &bt_voice_cfg;
}

/**
 * @brief   Set BT voice encode buffer
 * @param[in]  p_buff - buffer address
 * @returns    Buffer length
 */
uint16_t bt_voice_set_buffer(uint8_t *p_buff)
{
    g_spk_enc_buff_ptr = p_buff;

    g_mic_enc_buff_ptr = g_spk_enc_buff_ptr + SPK_ENC_BUFF_SIZE * SPK_ENC_BUFF_NUM;

    return SPK_ENC_BUFF_SIZE * SPK_ENC_BUFF_NUM + MIC_ENC_BUFF_SIZE * MIC_ENC_BUFF_NUM;
}

/**
 * @brief   Get BT voice buffer
 * @param[in]  None
 * @returns    Buffer pointer
 */
uint8_t *bt_voice_get_buffer(void)
{
    return g_spk_enc_buff_ptr;
}

/**
 * @brief   Register BT voice decode and encode function callbacks
 * @param[in]  p_enc_func - encode function callback
 * @param[in]  p_dec_func - decode function callback
 * @returns    None
 */
void bt_voice_register_codec_callback(void *p_enc_func, void *p_dec_func)
{
    bt_voice_cfg.dec_func = p_dec_func;
    bt_voice_cfg.enc_func = p_enc_func;
}

/**
 * @brief   Set BT voice status to IDLE
 * @param[in]  None
 * @returns    None
 */
void bt_voice_set_idle(void)
{
    AUDIO_GLOBAL_INT_DISABLE();
    bt_voice_cfg.status = BT_VOICE_ST_IDLE;
    AUDIO_GLOBAL_INT_RESTORE();
}

/**
 * @brief   Fill BT voice mic encode buffer with default data
 * @param[in]  None
 * @returns    None
 */
audio_ram_code void bt_voice_init_mic_enc_buff(void)
{
    uint8_t  i;
    uint8_t *p_des;
    uint8_t  msbc_head[4] = {0x08, 0x38, 0xc8, 0xf8};

    for (i = 0; i < MIC_ENC_BUFF_NUM; i++) {
        p_des = g_mic_enc_buff_ptr + i * MIC_ENC_BUFF_SIZE;

        /*
        if (flash_is_idle()) {
            tmemcpy(p_des, msbc_silence_pkt, 60);
        } else {
            tmemset(p_des, 0, 60);
        }
*/
        tmemcpy(p_des, voice_msbc_silence_pkt, 60);

        p_des[0] = 0x01;
        p_des[1] = msbc_head[i & 3];
    }
}

/**
 * @brief   Reset codec configuration and mute playback buffer
 * @param[in]  None
 * @return     None
 */
void bt_voice_reset(void)
{
    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "@bt_voice_reset %d %d %d %d %d", bt_voice_cfg.init, bt_voice_cfg.status, bt_voice_cfg.tick, bt_voice_cfg.spk_enc_wptr,
                 bt_voice_cfg.spk_enc_rptr);
#if TLKMW_INTERPHONE_EN
    if (!tlkmdi_interphone_is_busy())
#endif
    {
        tlkdrv_codec_muteSpkBuff();
    }
    bt_voice_init_mic_enc_buff();

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<ADC disable> %d %d", bt_voice_cfg.tick, stimer_get_tick());
    tlkapi_array(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "<ADC disable>, bt_voice_cfg", (uint8_t *)&bt_voice_cfg, 16);

    audio_codec_flag_set(CODEC_FLAG_VOICE, 0);
#if AUDIO_TWS_MODE
    async_tws_set_status_flag(TSYNC_FLAG_SCO, 0);
#endif

    bt_voice_cfg.status       = BT_VOICE_ST_IDLE;
    bt_voice_cfg.init         = 0;
    bt_voice_cfg.sync_init    = 0;
    bt_voice_cfg.spk_enc_wptr = 0;
    bt_voice_cfg.spk_enc_rptr = 0;
    bt_voice_cfg.tick         = 0;
}

/**
 * @brief   Get microphone data for BT voice codec
 * @param[in]  p_des   - destination buffer for PCM data
 * @param[in]  samples - number of samples to get
 * @returns    Operation result - TRUE: success, FALSE: failure
 */
audio_ram_code uint8_t bt_voice_codec_get_mic_data(int16_t *p_des, uint16_t samples)
{
#if AUDIO_TWS_MODE
    if (tlkmdi_bt_tpt_isSlave()) {
        return TRUE;
    }
#endif

#if 0
    adc_int mic_stereo_48k[480];
    int     mic_mono_48k[480];
    short   mic_mono_48k_16bit[480];
    short   mic_mono_16k_16bit[160];

    tlkdrv_codec_readMicData((uint8_t *)mic_stereo_48k, samples * 3 * sizeof(adc_int), 0); //stereo
    int *psrc = (int *)mic_stereo_48k;

    //tlkapi_printf(APP_AUDIO_LOG_EN, "mic src: %x %x %x %x", psrc[0], psrc[1], psrc[2], psrc[3]);

    for (int i = 0; i < samples * 3; i++) { //stereo -> mono
        mic_mono_48k[i] = psrc[2 * i];
    }

    for (int j = 0; j < samples * 3; j++) { //24bit -> 16bit
        mic_mono_48k_16bit[j] = (mic_mono_48k[j] >> 8) & 0xffff;
    }
#if TLKALG_ASRC_48TO16_16BIT_ENABLE
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
    p_audio_alg_if->audio_alg_process((uint8_t *)mic_mono_48k_16bit, (uint8_t *)mic_mono_16k_16bit, samples * 3, 0, 0);
#endif
    for (int k = 0; k < samples; k++) {
        *p_des++ = mic_mono_16k_16bit[k];
    }
#else
    adc_int mic_stereo_16k[120];
#if TLK_CFG_HRA_ENABLE
    tlkmdi_hra_get_data_mid_mic_buff((uint8_t *)mic_stereo_16k, samples * sizeof(adc_int));
#else
    tlkdrv_codec_readMicData((uint8_t *)mic_stereo_16k, samples * sizeof(adc_int), 0); //stereo
#endif
    int *psrc = (int *)mic_stereo_16k;
    for (int i = 0; i < samples; i++) { //stereo -> mono & 24bit -> 16bit
        *p_des++ = (psrc[2 * i] >> 8) & 0xffff;
    }
#endif

    return TRUE;
}

#if (TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
uint32_t g_bt_voice_alg_spk_rptr = 0;

/**
 * @brief   Synchronize microphone and speaker index for DSP data processing
 * @param[in]  sample - sample count for synchronization
 * @returns    None
 */
void bt_voice_dsp_dp_sync_mic_spk_index(uint16_t sample)
{
    uint32_t mic_rptr       = tlkdrv_codec_sync_mic_samples(sample);
    g_bt_voice_alg_spk_rptr = (mic_rptr - 180) & (CODEC_SPK_FIFO_SAMPLES - 1);
}

/**
 * @brief   Get microphone data for DSP data processing
 * @param[in]  p_des   - destination buffer for PCM data
 * @param[in]  samples - number of samples to get
 * @returns    Operation result - TRUE: success, FALSE: failure
 */
audio_ram_code uint8_t bt_voice_dsp_dp_get_mic_data(int16_t *p_des, uint16_t samples)
{
    adc_int mic_stereo_48k_24bit[360];
    short   mic_stereo_48k_16bit[360 * 2];
    short   mic_stereo_16k_16bit[360 * 2];

    short  buff_alg_in[360] = {0};
    short *pout             = buff_alg_in;

    tlkdrv_codec_readMicData((uint8_t *)mic_stereo_48k_24bit, samples * 3 * sizeof(adc_int), 0); //stereo
    int *psrc = (int *)mic_stereo_48k_24bit;

    for (int j = 0; j < samples * 3 * 2; j++) { //24bit -> 16bit
        mic_stereo_48k_16bit[j] = (psrc[j] >> 8) & 0xffff;
    }
    // audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16);
    audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_ASRC_48TO16_16BIT);
    p_audio_alg_if->audio_alg_process((uint8_t *)mic_stereo_48k_16bit, (uint8_t *)mic_stereo_16k_16bit, samples * 3, 0, 0);

    // mic data LRLRL...
    for (int k = 0; k < samples * 2; k++) {
        buff_alg_in[k] = mic_stereo_16k_16bit[k];
    }

    // spk data
    int *spk_psrc = (int *)g_codec_spk_buff;
    for (int j = 0; j < samples; j++) {
        buff_alg_in[samples * 2 + j] = (spk_psrc[g_bt_voice_alg_spk_rptr] >> 8) & 0xffff;
        g_bt_voice_alg_spk_rptr      = ((g_bt_voice_alg_spk_rptr + 6) & (CODEC_SPK_FIFO_SAMPLES - 1));
    }

    for (int k = 0; k < samples * 3; k++) {
        *p_des++ = *pout++;
    }

    return TRUE;
}

/**
 * @brief   Get available length of data in DSP buffer
 * @param[in]  wptr - write pointer
 * @param[in]  rptr - read pointer
 * @returns    Available data length
 */
uint16_t bt_voice_get_avia_len(uint16_t wptr, uint16_t rptr)
{
    uint16_t avia = 0;
    if (wptr >= rptr) {
        avia = wptr - rptr;
    } else {
        avia = TLKALG_DSP_RET_NN_DATA_LEN + wptr - rptr;
    }
    return avia;
}

/**
 * @brief   Clear DSP return buffer status
 * @param[in]  None
 * @returns    None
 */
void bt_voice_clear_dsp_ret_buff_status(void)
{
    tmemset(dsp_ret_data_buff, 0, TLKALG_DSP_RET_NN_DATA_LEN * sizeof(short));
    bt_dsp_wptr = 0;
    bt_dsp_rptr = 0;
}

/**
 * @brief   DSP message process callback function
 * @param[in]  enc_buff_wptr - encode buffer write pointer
 * @param[in]  type          - message type
 * @returns    None
 */
void bt_voice_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type)
{
    (void)enc_buff_wptr;
    (void)type;

    // uint8_t *p_des;
    uint16_t pcm_data_len;

    // gpio_set_high_level(GPIO_CHN2);
    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&pcm_data_len, BT_VOICE_ID);
    if (pcm_data == NULL) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "bt voice pcm_data is NULL");
        return;
    }
    short *pdes = (short *)pcm_data;
#if TLKALG_AGC_ENABLE
    // gpio_set_high_level(GPIO_CHN1);
    short agc_out_buff[320];
    tlkalg_agc_proc_behind_nn((uint8_t *)pcm_data, (uint8_t *)agc_out_buff);
    pdes = (short *)agc_out_buff;
    // gpio_set_low_level(GPIO_CHN1);
#endif

#if TLKALG_ANS_ENABLE
    // gpio_set_high_level(GPIO_CHN1);
    short ans_out_buff[320];
    tlkalg_ans_proc_behind_nn((uint8_t *)pdes, (uint8_t *)ans_out_buff);
    pdes = (short *)ans_out_buff;
    // gpio_set_low_level(GPIO_CHN1);
#endif

    for (int i = 0; i < pcm_data_len / TLKALG_DSP_RET_BYTE_PER_SAMPLE; i++) {
        dsp_ret_data_buff[bt_dsp_wptr++] = *pdes++;
        bt_dsp_wptr %= TLKALG_DSP_RET_NN_DATA_LEN;
    }

    //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "enc w: %d", bt_voice_cfg.mic_enc_wptr);
}
#endif

/**
 * @brief   Get voice PCM data, process with EQ and encode PCM data
 * @param[in]  None
 * @return     None
 */
audio_ram_code void bt_voice_process_mic_data(void)
{
#if (!(TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
#if AUDIO_PATH_24BITS_EN
    //int32_t pcm_stereo[120 * 2];
#else
    int16_t pcm_stereo[120 * 2];
#endif
#endif

    int16_t pcm[120 * 3];
#if TLK_MW_DSP_COMM_ENABLE && DSP_NN_NS_BUFF_DISENABLE
    int16_t pcm_to_dsp[320 * 3];
    uint8_t pcm_ret = 0;
#endif
    uint8_t samples = 120;
    uint8_t ret     = 0;
    uint8_t mute_mic;
    uint8_t i;

    // if (tone_is_playing() && tone_get_cur_id() == TONE_PAIRING) {
    //     return;
    // }

    /* 20 ms idle, reset */
    if (bt_voice_cfg.tick && clock_time_exceed(bt_voice_cfg.tick, 40000) && (!tone_is_playing())) {
        bt_voice_reset();
#if AUDIO_TWS_MODE
        async_tws_set_status_flag(TSYNC_FLAG_SCO, 0);
#endif
        // audio_codec_flag_set(CODEC_FLAG_VOICE, 0);
    }

    if (!audio_codec_flag_get(CODEC_FLAG_VOICE) || bt_voice_cfg.status == BT_VOICE_ST_IDLE) {
        return;
    }

    if (bt_voice_cfg.status < BT_VOICE_ST_CODEC_READY) {
        if (bt_voice_cfg.status == BT_VOICE_ST_INIT) {
            if (bt_voice_cfg.init == 0) {
                bt_voice_cfg.init = 1;
            }

            bt_voice_cfg.status = BT_VOICE_ST_INIT + 1;
        }
    } else if (bt_voice_cfg.status == BT_VOICE_ST_CODEC_READY) {
        bt_voice_cfg.status++;

#if (TLKMW_INTERPHONE_EN)
        if (!tlkmdi_interphone_is_busy())
#endif
        {
            tlkdrv_codec_sync_mic_samples(120 * 2);
        }


        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "voice codec ready, samples_ref:%d, status:%d", bt_voice_cfg.samples_ref, bt_voice_cfg.status);
    } else if ((bt_voice_cfg.status > BT_VOICE_ST_CODEC_READY) && (bt_voice_cfg.status <= BT_VOICE_ST_MIC_READY)) {
        bt_voice_cfg.status++;
    }

#if AUDIO_TWS_MODE
    if (tlkmdi_bt_tpt_isMaster() || tlkmdi_bt_tpt_isSingle()) {
        if (!bt_voice_cfg.mic_state) {
#if (TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
            tlkmw_dsp_resume();
            bt_voice_clear_dsp_ret_buff_status();
            ipc_msg_register_data_process_done_cb(bt_voice_dsp_msg_process_callback, BT_VOICE, BT_VOICE_ID);
#if DSP_NN_NS_BUFF_DISENABLE
            dsp_voice_nn_comm_reset_status();
#endif
#endif

            tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_MIC, TLKDRV_CODEC_CHANNEL_STEREO, TLKDRV_CODEC_BITDEPTH_24, AUDIO_SAMPLE_RATE_16K, 0);
            tlkdrv_codec_sync_mic_samples(240);
            bt_voice_cfg.mic_state = 1;
            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "BT MASTER/SINGLE VOICE OPEN MIC");
        }
    } else if (tlkmdi_bt_tpt_isSlave()) {
        if (bt_voice_cfg.mic_state) {
            tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);

#if (TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
            tlkmw_dsp_pause();
#endif

            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "BT SLAVE VOICE CLOSE MIC");
            bt_voice_cfg.mic_state = 0;
        }
    }
#endif

    if (bt_voice_cfg.status) {
#if (!(TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))

        // #if (TLK_ALG_HYBRID_ENABLE)
        // ret = tlkalg_get_mic_frame(pcm, samples);
        // #else
#if AUDIO_PATH_24BITS_EN
        /*
        ret = tlkdrv_codec_readMicData((uint8_t *)pcm_stereo, samples * 2 * 4, 0);
        for (i = 0; i < 120; i++) {
            pcm[i] = pcm_stereo[2*i + 1] >> 8;
        }
        */

#if 0 //(TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)  //0: only nn_ns running in dsp and no other voice processing alg running
            ret = bt_voice_dsp_dp_get_mic_data(pcm, samples);
#else
#if DONGLE_VOICE_MIC_EN
        if (ll_mix_voice_get_mic_mode() && ll_audio_is_phone_mode()) {
            ret = 1;
            //tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "bt_voice_process_mic_data ");
        } else
#endif
        {
            ret = bt_voice_codec_get_mic_data(pcm, samples);
            // ret = tlkdrv_codec_readMicData((uint8_t *)pcm, samples * sizeof(adc_int), 0);
        }
        // #else
        // ret = bt_voice_codec_get_mic_data(pcm, samples);
        // #endif
#endif

        /*
        static int sin_16k_count = 0;

        for (i = 0; i < 120; i++) {
            pcm[i] = sin_16k_mono_bbb[sin_16k_count++];
            sin_16k_count %= 16;
        }
        */
#else
#if TLK_CFG_HRA_ENABLE
        ret = tlkmdi_hra_get_data_mid_mic_buff((uint8_t *)pcm_stereo, samples * sizeof(adc_int));
#else
#if TLKMW_INTERPHONE_EN
        if (tlkmdi_interphone_is_busy()) {
            ret = tlkmdi_interphone_getData_BtUpBuff((uint8_t *)pcm_stereo, samples * sizeof(adc_int));
        } else
#endif
        {
            ret = tlkdrv_codec_readMicData((uint8_t *)pcm_stereo, samples * sizeof(adc_int), 0);
        }
#endif
        for (i = 0; i < 120; i++) {
            pcm[i] = pcm_stereo[2 * i]; //TODO by congxiu
        }
#if TLK_PCM_DATA_WR_EN
        if (bt_audio_get_mic_data_cb) {
            bt_audio_get_mic_data_cb(pcm, samples);
        }

#endif

#endif

        // #endif
#else
        ret = tlkmdi_audu2h_voice_getPcmData((uint8_t *)pcm, samples);
#endif

#if 0
        static signed short sin_16k_mono_sbc[]__attribute__((aligned(4))) = {
                0, 12539, 23169, 30272, 32767, 30272, 23169, 12539, 4, -12539, -23169, -30272, -32767, -30272, -23169, -12539};
        static int count = 0;
        for(int i = 0; i < 120; i++) {
            pcm[i] = sin_16k_mono_sbc[count++];
            count %= 16;
        }
        ret = 1;
#endif

#if AUDIO_TWS_MODE
        if (tlkmdi_bt_tpt_isSlave()) {
            return;
        }
#endif

        if (ret != 0) {
#if CODEC_MUTE_MIC_EN
            mute_mic = btif_get_mic_mute_flag();
#else
            mute_mic = 0;
#endif
            if (mute_mic || (bt_voice_cfg.status < BT_VOICE_ST_MIC_READY)) {
                tmemset(pcm, 0, sizeof(pcm));
            } else if (bt_voice_cfg.status == BT_VOICE_ST_MIC_READY) {
                for (i = 0; i < 120; i++) {
#if DONGLE_VOICE_MIC_EN
                    if (ll_mix_voice_get_mic_mode() && ll_audio_is_phone_mode()) {
                        pcm[i] = 0;
                    } else {
                        pcm[i] = (pcm[i] * i + 60) / 120;
                    }
#else
                    pcm[i] = (pcm[i] * i + 60) / 120;
#endif
                }
            }

            if (bt_voice_cfg.status > BT_VOICE_ST_INIT) {
#if (TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
#if TLKMW_INTERPHONE_EN
                if (!tlkmdi_interphone_is_busy())
#elif DONGLE_VOICE_MIC_EN
                if (!ll_mix_voice_get_mic_mode() || (ll_mix_voice_get_mic_mode() && !ll_audio_is_phone_mode())) //bt mic
#endif
                {
                    app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(BT_VOICE_ID);
                    p_dsp_app_ctx->alg_type          = BT_VOICE;

#if DSP_NN_NS_BUFF_DISENABLE
                    // gpio_set_high_level(GPIO_PB0);
                    // gpio_set_low_level(GPIO_PB0);
                    pcm_ret = d25f_get_bt_voice_available_nn_buff_len(pcm, pcm_to_dsp, 120);
                    if (pcm_ret == 1) {
                        ret = d25f_send_audio_data_to_dsp((uint8_t *)pcm_to_dsp, 16 * 20 * 2, BT_VOICE_ID);
                    }
#else
#if TLKALG_ANC_ENABLE
                    if (!tlkmdi_anc_is_busy() || (tlkmdi_anc_dsp_is_sync_data() && tlkmdi_anc_is_busy()))
#endif
                    {
                        ret = d25f_send_audio_data_to_dsp((uint8_t *)pcm, 120 * 2, BT_VOICE_ID);
                    }
//tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "dsp send data");
#endif
                }

#endif
                bt_voice_cfg.mic_enc_wptr = bt_voice_cfg.spk_enc_wptr & (MIC_ENC_BUFF_NUM - 1);
                uint8_t *p_des            = g_mic_enc_buff_ptr + (bt_voice_cfg.mic_enc_wptr & (MIC_ENC_BUFF_NUM - 1)) * MIC_ENC_BUFF_SIZE;

#if EQ_ENABLE
                //log_task(SL_TWS_VOICE_LOG_EN, SL01_audio_eq, 1);
                tlkalg_eq_data_process_mono(EQ_TYPE_VOICE_MIC, EQ_CHANNEL_LEFT, (uint8_t *)pcm, 120 * EQ_DATA_TYPE_INT16, EQ_DATA_TYPE_INT16);
                //log_task(SL_TWS_VOICE_LOG_EN, SL01_audio_eq, 0);
#endif

#ifdef SL01_bt_audio_enc
                log_task_begin_irq(SL_BT_VOICE_LOG_EN, SL01_bt_audio_enc);
#endif

#if DONGLE_VOICE_MIC_EN
                if ((ll_mix_voice_get_mic_mode()) && ll_audio_is_phone_mode()) {
                    tmemcpy(p_des, voice_msbc_silence_pkt, 60);
                } else
#endif
                // #else
                {
#if (TLK_MW_DSP_COMM_ENABLE && !TLK_CFG_HRA_ENABLE)
#if TLKMW_INTERPHONE_EN
                    if (tlkmdi_interphone_is_busy()) {
                        bt_voice_cfg.enc_func((uint8_t *)pcm, p_des, 240, 0, 3);
                    } else
#endif
                    {
                        if (bt_voice_get_avia_len(bt_dsp_wptr, bt_dsp_rptr) >= 120) {
                            short dsp_ret_enc_fram[120];
#if TLKALG_ANC_ENABLE
                            if (tlkmdi_anc_is_busy() && !tlkmdi_anc_dsp_is_sync_data()) {
                                for (int j = 0; j < 120; j++) {
                                    dsp_ret_enc_fram[j] = 0;
                                }
                            } else
#endif
                            {
                                for (int j = 0; j < 120; j++) {
                                    dsp_ret_enc_fram[j] = dsp_ret_data_buff[bt_dsp_rptr++];
                                    bt_dsp_rptr %= TLKALG_DSP_RET_NN_DATA_LEN;
                                }
                            }

                            bt_voice_cfg.enc_func((uint8_t *)dsp_ret_enc_fram, p_des, 240, 0, 3);
                        } else {
                            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "[test] bt voice have not enough dsp data");
                        }
                    }
#else
                    bt_voice_cfg.enc_func((uint8_t *)pcm, p_des, 240, 0, 3);
#endif
                }
                // #endif

#ifdef SL01_bt_audio_enc
                log_task_end_irq(SL_BT_VOICE_LOG_EN, SL01_bt_audio_enc);
#endif

                // #endif
            }
        } else {
#if 0
            tlkapi_trace(BT_AUDIO_DBG_FLAG,
                         BT_AUDIO_DBG_SIGN,
                         "voice get mic data fail, status: %d, mic_enc_wptr:%d, init:%d, ret:%d",
                         bt_voice_cfg.status,
                         bt_voice_cfg.mic_enc_wptr,
                         bt_voice_cfg.init,
                         ret);
#endif
        }
    }
}

/**
 * @brief   Decode encoded voice data then post PCM to p_des0 and p_des1
 * @param[out]  p_des0 - left channel PCM data
 * @param[out]  p_des1 - right channel PCM data
 * @return      Data length
 */
audio_ram_code uint16_t bt_voice_get_playback_data(int16_t *p_des0, int16_t *p_des1)
{
    uint16_t ret = FALSE;
    uint8_t *p_voice_src;
    // int16_t pcm_tmp[120];

#if AUDIO_TWS_MODE
    if (async_tws_get_status_flag(TSYNC_FLAG_SCO) && (bt_voice_cfg.status >= BT_VOICE_ST_CODEC_READY))
#else
    if ((bt_voice_cfg.status >= BT_VOICE_ST_CODEC_READY))
#endif
    {
        bt_voice_cfg.spk_enc_rptr = bt_voice_cfg.spk_enc_wptr;
        //log_b16_general(1, SL16_bt_sco_enc_wptr, bt_voice_cfg.spk_enc_wptr | 0x80 << 8);
        p_voice_src = g_spk_enc_buff_ptr + (bt_voice_cfg.spk_enc_rptr & (SPK_ENC_BUFF_NUM - 1)) * SPK_ENC_BUFF_SIZE;

#if AUDIO_TWS_MODE
        bt_voice_cfg_t *p_bt_voice_env = get_bt_voice_env_ptr();
        if (!p_bt_voice_env->voice_play_sync) {
            return 120;
        }
#endif

        if (audio_codec_flag_get(CODEC_FLAG_VOICE)) {
            if (bt_voice_cfg.dec_func) {
                int16_t *p0 = p_des0;

                //if (p_voice_src[0] != BT_VOICE_FLAG_PACKET_LOSS)
                {
#ifdef SL01_bt_audio_dec
                    log_task_begin_irq(SL_BT_VOICE_LOG_EN, SL01_bt_audio_dec);
#endif
#if TWS_AUDIO_PATH_GPIO_DEBUG
                    gpio_write(GPIO_PC0, 1);
#endif
                    ret = bt_voice_cfg.dec_func(p_voice_src, (uint8_t *)p0, 60, 0, 1);

#if TWS_AUDIO_PATH_GPIO_DEBUG
                    gpio_write(GPIO_PC0, 0);
#endif
#ifdef SL01_bt_audio_dec
                    log_task_end_irq(SL_BT_VOICE_LOG_EN, SL01_bt_audio_dec);
#endif

                    /*
                    ret = tlkdrv_codec_get_sin_data(pcm_tmp, BT_VOICE_PCM_SAMPLES);
                    for (i = 0; i < BT_VOICE_PCM_SAMPLES; i++) {
                        *p0++ = pcm_tmp[i];
                    }
                    */

                    if (FALSE == ret) {
                        if (HFP_CODEC_ID_MSBC == btif_get_hfp_codec()) {
                            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "#bt_MSBC_DEC %d", ret);
                        } else {
                            tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "#bt_CVSD_DEC %d", ret);
                        }
                    }
                }

                if (TRUE == ret) {
#if HFP_PLC_EN
                    if (HFP_CODEC_ID_MSBC == btif_get_hfp_codec()) {
                        tlka_hfp_plc_good_frame((plc_state_t *)g_hfp_plc_buf_ptr, p0, p0);
                    } else {
                        tlka_cvsd_g711plc_addtohistory((TLKA_CVSD_PLC *)g_hfp_plc_buf_ptr, p0);
                    }
#endif
                } else {
#if HFP_PLC_EN
                    if (HFP_CODEC_ID_MSBC == btif_get_hfp_codec()) {
                        tmemcpy(p_voice_src + 2, msbc_silence_pkt, 60);
                        ret = bt_voice_cfg.dec_func(p_voice_src, p0, 60, 0, 0);
                        tlka_hfp_plc_bad_frame((plc_state_t *)g_hfp_plc_buf_ptr, p0, p0);

                        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "bt_voice_spk_plc:MSBC %d", bt_voice_cfg.spk_enc_rptr);
                    } else {
                        tmemset(p0, 0, 120 * 2);
                        tlka_cvsd_g711plc_dofe((TLKA_CVSD_PLC *)g_hfp_plc_buf_ptr, p0);

                        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "bt_voice_spk_plc:CVSD %d", bt_voice_cfg.spk_enc_rptr);
                    }
#endif
                }
            }
            bt_audio_control_voice_volume(p_des0, 120);
        } else {
            bt_audio_get_mute_data(p_des0, p_des1);
        }

        return 120;
    } else if (bt_voice_cfg.status == BT_VOICE_ST_IDLE) {
        ret = bt_audio_get_mute_data(p_des0, p_des1);
        return ret;
    }
#if (!(TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
    return 0;
#else
    ret = bt_audio_get_mute_data(p_des0, p_des1);
    return ret;
#endif
}

/**
 * @brief   Dump BT voice configuration info
 * @param[in]  None
 * @return     None
 */
void bt_voice_cfg_dump(void)
{
    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "bt_voice_cfg:sts/init,dec_f/enc_f %d %d %x %x", bt_voice_cfg.status, bt_voice_cfg.init, bt_voice_cfg.dec_func,
                 bt_voice_cfg.enc_func);

    tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "spk_enc R/W,spk_pcm R/W %d %d %d %d", bt_voice_cfg.spk_enc_rptr, bt_voice_cfg.spk_enc_wptr, bt_voice_cfg.spk_pcm_rptr,
                 bt_voice_cfg.spk_pcm_wptr);
}

#if TLKMW_INTERPHONE_EN
bool master_is_interphone = false;

/**
 * @brief   Set interphone master mode
 * @param[in]  None
 * @returns    None
 */
void bt_voice_set_interphone_master(void)
{
    master_is_interphone = true;
}

/**
 * @brief   Clear interphone master mode
 * @param[in]  None
 * @returns    None
 */
void bt_voice_clear_interphone_master(void)
{
    master_is_interphone = false;
}

/**
 * @brief   Check if master is in interphone mode
 * @param[in]  None
 * @returns    TRUE if master is in interphone mode, FALSE otherwise
 */
bool bt_voice_master_is_interphone(void)
{
    return master_is_interphone;
}
#endif
#endif
