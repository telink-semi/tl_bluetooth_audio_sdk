/********************************************************************************************************
 * @file    tlkmdi_hra_bt_audio.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/btp/spp/btp_spp.h"
#if (TLK_CFG_HRA_ENABLE)

#define TLKMDI_HRA_MUSIC_CHN_NUM 2

hra_codec_int g_spk_mid_buff[CODEC_SPK_FIFO_SAMPLES];
adc_int       g_mic_mid_buff[CODEC_MIC_FIFO_SAMPLES];

static tlkmdi_hra_ctl s_hra_ctl = {0};

/**
 * @brief   Initialize HRA music module
 * @param   None
 * @return  None
 */
void tlkmdi_hra_music_init(void)
{
    memset(&s_hra_ctl, 0, sizeof(s_hra_ctl));
    ipc_msg_register_data_process_done_cb(tlkmdi_hra_music_dsp_msg_process_callback, HRA_MUSIC, HRA_MUSIC_ID);
}

/**
 * @brief   Initialize HRA voice module
 * @param   None
 * @return  None
 */
void tlkmdi_hra_voice_init(void)
{
    memset(&s_hra_ctl, 0, sizeof(s_hra_ctl));
    ipc_msg_register_data_process_done_cb(tlkmdi_hra_voice_dsp_msg_process_callback, HRA_VOICE, HRA_VOICE_ID);
}

// void tlkmdi_hra_voice_set_timer_flag(bool flag)
// {
//     s_hra_ctl.voice_start_timer_flag = flag;
// }

// bool tlkmdi_hra_voice_get_timer_flag(void)
// {
//     return s_hra_ctl.voice_start_timer_flag;
// }

/**
 * @brief   Set stack timer flag
 * @param   flag - timer flag value
 * @return  None
 */
void tlkmdi_hra_stack_set_timer_flag(bool flag)
{
    s_hra_ctl.stack_start_timer_flag = flag;
}

/**
 * @brief   Get stack timer flag
 * @param   None
 * @return  stack timer flag
 */
bool tlkmdi_hra_stack_get_timer_flag(void)
{
    return s_hra_ctl.stack_start_timer_flag;
}

/**
 * @brief   Process BT audio data
 * @param   None
 * @return  None
 */
void tlkmdi_hra_bt_audio_proc(void)
{
    d25f_process_ipc_msg();
    tlkmdi_hra_bt_audio_send_data_to_dsp();
}

#if BT_VOICE_SPP_TEST
uint32_t   len_spkinput  = 0;
uint32_t   len_spkoutput = 0;
uint32_t   len_micinput  = 0;
uint32_t   len_micoutput = 0;
static int flag_num      = 0;
static int flag_dec_out  = 0;

int16_t             buff_spkw[128];
int16_t             buff_spkr[120];
int16_t             buff_micw[128];
int16_t             buff_micr[120];
codec_mono_int      data_spkbuff_dec_out[240];
codec_mono_int      data_micbuff_dec_out[240];
int16_t             data_spkbuff_enc[120 * 2];
int16_t             data_spkbuff_dec[120 * 2];
int16_t             data_micbuff_enc[120 * 2];
int16_t             data_micbuff_dec[120 * 2];
static unsigned int buff_wptr       = 0;
static unsigned int buff_rptr       = 0;
static unsigned int dec_spkout_wptr = 0;
static unsigned int dec_micout_wptr = 0;
static unsigned int spk_count       = 0;
static unsigned int mic_count       = 0;


#define SPP_BUFF_BLOCK_NUM 10
uint8_t          spp_buff_rptr                      = 0;
uint8_t          spp_buff_wptr                      = 0;
volatile uint8_t spp_buff[128 * SPP_BUFF_BLOCK_NUM] = {0};
static uint8_t   time_count                         = 0;

void tlk_send_mic_and_spk_data_by_spp(void)
{
    int8_t blocks_in_buff = 0;
    blocks_in_buff        = spp_buff_wptr - spp_buff_rptr;
    if (blocks_in_buff < 0) {
        blocks_in_buff += SPP_BUFF_BLOCK_NUM;
    }

    tlkmdi_btacl_item_t *pItem = NULL;
    pItem                      = tlkmdi_btacl_getConnItemByIndex(0);

    if (blocks_in_buff > 1) {
        if (pItem) {
            int ret = btp_spp_sendData(pItem->handle, NULL, 0, (uint8_t *)(spp_buff + spp_buff_rptr * 128), 128 * 2);

            if (0 == ret) {
                spp_buff_rptr = (spp_buff_rptr + 2) % SPP_BUFF_BLOCK_NUM;
            } else {
            }
        }
    }
}
#endif


/**
 * @brief   Send BT audio data to DSP
 * @param   None
 * @return  None
 */
void tlkmdi_hra_bt_audio_send_data_to_dsp(void)
{
    uint8_t hra_samplerate = 48;

    if (tlkmdi_bt_music_is_busy()) {
        uint16_t sample_num_m = hra_samplerate * MCU2DSP_DATA_MS;
        if (tlkmdi_hra_get_mid_spk_available_sample() >= sample_num_m) {
            adc_mono_int data_buff[48 * MCU2DSP_DATA_MS * 3]; //2chn-spk 1chn-mic
            bool         ret = tlkmdi_hra_get_data_mid_spk_buff((uint8_t *)data_buff, sample_num_m * sizeof(hra_codec_int));

#if TLKMDI_HRA_MUSIC_MIC_EN
            adc_int       mic_data_stereo[48 * MCU2DSP_DATA_MS];
            adc_mono_int *pmic    = (adc_mono_int *)mic_data_stereo;
            bool          ret_mic = tlkdrv_codec_readMicData((uint8_t *)mic_data_stereo, sample_num_m * sizeof(adc_int), 0);
            if (ret_mic) {
                for (int i = 0; i < sample_num_m; i++) {
                    data_buff[i + 48 * MCU2DSP_DATA_MS * 2] = pmic[2 * i];
                }
            } else {
                for (int i = 0; i < sample_num_m; i++) {
                    data_buff[i + 48 * MCU2DSP_DATA_MS * 2] = 0;
                }
            }

#endif

            if (ret) {
                app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(HRA_MUSIC_ID);
                p_dsp_app_ctx->alg_type          = HRA_MUSIC;
                // gpio_set_high_level(GPIO_CHN2);
                d25f_send_audio_data_to_dsp((uint8_t *)data_buff, 48 * MCU2DSP_DATA_MS * 3 * sizeof(adc_mono_int), HRA_AUDIO_ID);
                // gpio_set_low_level(GPIO_CHN2);
            }
        }
    } else if (tlkmdi_bt_voice_is_busy()) {
        if (tlkmdi_hra_get_mid_spk_available_sample() >= 16 * MCU2DSP_DATA_MS
            /*&& tlkmdi_hra_get_mid_mic_available_sample() >= 16 * MCU2DSP_DATA_MS*/) {
            uint16_t       sample_num = 16 * MCU2DSP_DATA_MS;
            codec_mono_int data_buff[16 * MCU2DSP_DATA_MS * (2 + 1)]; //16k | 2ms | 1chn_spk | 2chn_mic
            hra_codec_int  music_stereo[16 * MCU2DSP_DATA_MS];
#if BT_VOICE_SPP_TEST
            int16_t data_micbuff[16 * MCU2DSP_DATA_MS];
            int16_t data_spkbuff[16 * MCU2DSP_DATA_MS];
#endif

            // gpio_set_high_level(GPIO_CHANNEL2);
            bool ret = tlkmdi_hra_get_data_mid_spk_buff((uint8_t *)music_stereo, sample_num * sizeof(hra_codec_int));
            if (!ret) {
                tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "hra voice get spk data error");
                return;
            }

#if TWS_ENABLE
            uint8_t channel = tlkmdi_bt_tpt_audio_getCurChannel();
            tlkmdi_bt_tpt_audio_getMonoPcmData((codec_mono_int *)music_stereo, (codec_mono_int *)data_buff, sample_num, channel);

            if (tlkmdi_bt_tpt_isMaster() || tlkmdi_bt_tpt_isSingle()) {
                tlkdrv_codec_readMicData((uint8_t *)(data_buff + sample_num), sample_num * sizeof(adc_int), 0);
            } else {
                tmemset((uint8_t *)(data_buff + sample_num), 0, sample_num * sizeof(adc_int));
            }
#else
            codec_mono_int *p_music = (codec_mono_int *)music_stereo;
            for (uint16_t i = 0; i < sample_num; i++) {
                data_buff[i] = p_music[2 * i];
            }
            tlkdrv_codec_readMicData((uint8_t *)(data_buff + sample_num), sample_num * sizeof(adc_int), 0);
#endif


#if BT_VOICE_SPP_TEST
            for (int i = 0; i < 16 * MCU2DSP_DATA_MS; i++) {
                data_spkbuff[i] = (data_buff[i] >> 8) & 0xffff;                            //get spk data(24bit -> 16bit)
                data_micbuff[i] = (data_buff[2 * i + 16 * MCU2DSP_DATA_MS] >> 8) & 0xffff; //get left_mic data(24bit -> 16bit)
            }

            for (int i = 0; i < 16 * MCU2DSP_DATA_MS; i++) {
                data_buff[i]                            = 0; //clear spk data
                data_buff[2 * i + 16 * MCU2DSP_DATA_MS] = 0; //clear mic data
            }

            for (int i = 0; i < 16 * MCU2DSP_DATA_MS; i++) {
                buff_spkw[buff_wptr] = data_spkbuff[i];
                buff_spkr[buff_rptr] = buff_spkw[buff_wptr];

                buff_micw[buff_wptr] = data_micbuff[i];
                buff_micr[buff_rptr] = buff_micw[buff_wptr];

                buff_wptr = (buff_wptr + 1) % 128; //8ms

                if (buff_rptr == 119) {
                    flag_num = 1;
                }

                buff_rptr = (buff_rptr + 1) % 120; //7.5ms

                {
                    //enc and dec
                    if (flag_num) {
                        len_spkinput  = tlkalg_msbc_enc_spkbuf_process((uint8_t *)buff_spkr, (uint8_t *)data_spkbuff_enc, 120 * 2, 0, 3);
                        len_spkoutput = tlkalg_msbc_dec_spkbuf_process((uint8_t *)data_spkbuff_enc, (uint8_t *)data_spkbuff_dec, (uint16_t)(len_spkinput), 0, 3);

                        for (uint32_t j = 0; j < len_spkoutput; j++) {                                   //len_spkoutput == 120 in theory
                            data_spkbuff_dec_out[dec_spkout_wptr++] = (int32_t)data_spkbuff_dec[j] << 8; //16bit -> 24bit
                            dec_spkout_wptr %= 240;
                        }

                        len_micinput  = tlkalg_msbc_enc_micbuf_process((uint8_t *)buff_micr, (uint8_t *)data_micbuff_enc, 120 * 2, 0, 3);
                        len_micoutput = tlkalg_msbc_dec_micbuf_process((uint8_t *)data_micbuff_enc, (uint8_t *)data_micbuff_dec, (uint16_t)(len_micinput), 0, 3);

                        for (uint32_t j = 0; j < len_micoutput; j++) {                                   //len_micoutput == 120 in theory
                            data_micbuff_dec_out[dec_micout_wptr++] = (int32_t)data_micbuff_dec[j] << 8; //16bit -> 24bit
                            dec_micout_wptr %= 240;
                        }

                        //push enc data to spp_buff
                        memcpy((void *)&spp_buff[spp_buff_wptr * 128], (const void *)&data_spkbuff_enc[0],
                               (uint16_t)(6 + len_spkinput + 1)); //6:packet head, len_micinput(57):packet data, 1:parity bit
                        memcpy((void *)&spp_buff[spp_buff_wptr * 128 + 64], (const void *)&data_micbuff_enc[0], (uint16_t)(6 + len_micinput + 1));


                        spp_buff_wptr = (spp_buff_wptr + 1) % SPP_BUFF_BLOCK_NUM;
                        time_count++;
                        flag_num = 0;
                        flag_dec_out++;
                    }

                    if (flag_dec_out >= 2) { //already get 15ms data

                        if (2 == time_count) {
                            tlk_send_mic_and_spk_data_by_spp(); //spp out-15ms send once
                            time_count %= 2;
                        }

                        data_buff[i]                            = data_spkbuff_dec_out[spk_count++]; // test spk
                        data_buff[2 * i + 16 * MCU2DSP_DATA_MS] = data_micbuff_dec_out[mic_count++]; // test mic
                        spk_count %= 240;
                        mic_count %= 240;
                    }
                }
            }
#endif

            // gpio_set_low_level(GPIO_CHANNEL2);

            // #if TLKALG_MIC_SPK_MONO_ENABLE
            // short data_buff_mono[16 * MCU2DSP_DATA_MS * 1 * 2];
            // short *data_buff_mic = data_buff + 16 * MCU2DSP_DATA_MS * TLKMDI_HRA_MUSIC_CHN_NUM;
            // tlkdrv_codec_readMicData((uint8_t *)data_buff_mono, sample_num * sizeof(adc_int) / 2, 0);
            // for (int i = 0; i < 16 * MCU2DSP_DATA_MS; i++) {
            // 	data_buff_mic[2*i] = data_buff_mono[i];
            // 	data_buff_mic[2*i+1] = data_buff_mono[i];
            // }
            // #else
            // tlkdrv_codec_readMicData((uint8_t *)(data_buff + 16 * MCU2DSP_DATA_MS), sample_num * sizeof(adc_int), 0);
            // #endif

            if (ret) {
                app_dsp_context_t *p_dsp_app_ctx = d25f_get_dsp_app_ctx(HRA_VOICE_ID);
                p_dsp_app_ctx->alg_type          = HRA_VOICE;
                d25f_send_audio_data_to_dsp((uint8_t *)data_buff, sample_num * 3 * sizeof(codec_mono_int), HRA_VOICE_ID);
            } else {
                tlkapi_trace(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "hra voice get mic data error");
            }
        }
    }
}

/**
 * @brief   Callback function for processing music DSP messages
 * @param   enc_buff_wptr - encode buffer write pointer
 * @param   type - message type
 * @return  None
 */
void tlkmdi_hra_music_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type)
{
    (void)enc_buff_wptr;
    (void)type;

    uint16_t pcm_data_len;
    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&pcm_data_len, HRA_MUSIC_ID);
    if (pcm_data == NULL) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "hra music pcm_data is NULL");
        return;
    }

#if TWS_ENABLE
    codec_mono_int pcm_data_mono[48 * 2];
    uint16_t       pcm_sample_per_chn = pcm_data_len / sizeof(codec_mono_int) / 3;
    uint16_t       fill_len           = pcm_data_len / 3;

    uint8_t channel = tlkmdi_bt_tpt_audio_getCurChannel();
    tlkmdi_bt_tpt_audio_getMonoPcmData((codec_mono_int *)pcm_data, (codec_mono_int *)pcm_data_mono, pcm_sample_per_chn, channel);
    // tlkmdi_bt_tpt_audio_getMonoPcmData((codec_mono_int *)pcm_data, (codec_mono_int *)pcm_data_mono, pcm_data_len/sizeof(hra_codec_int), channel);
    // codec_mono_int* psrc = (codec_mono_int*)pcm_data;
    // for (int i = 0; i < pcm_sample_per_chn; i++) {
    //     pcm_data_mono[i] = psrc[48*4+i];
    // }

    //tws only support mono dac
    // gpio_set_high_level(GPIO_CHN4);
    tlkdrv_codec_fillSpkBuff((uint8_t *)pcm_data_mono, fill_len);
// gpio_set_low_level(GPIO_CHN4);
#else
    tlkdrv_codec_fillSpkBuff((uint8_t *)pcm_data, pcm_data_len);
#endif
}

/**
 * @brief   Callback function for processing voice DSP messages
 * @param   enc_buff_wptr - encode buffer write pointer
 * @param   type - message type
 * @return  None
 */
void tlkmdi_hra_voice_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type)
{
    (void)enc_buff_wptr;
    (void)type;

    uint16_t pcm_data_len;
    uint8_t *pcm_data = d25f_get_pcm_data_from_dsp(&pcm_data_len, HRA_VOICE_ID);
    if (pcm_data == NULL) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "hra voice pcm_data is NULL");
        return;
    }

    // gpio_set_high_level(GPIO_CHANNEL5);
    // gpio_set_low_level(GPIO_CHANNEL5);
    // uint16_t send_data1[3];
    // send_data1[0] = tlkdrv_codec_getSpkOffset()/sizeof(codec_int);
    // send_data1[1] = tlkdrv_codec_get_speaker_rptr();
    // send_data1[2] = tlkdrv_codec_get_spk_avail_samples();
    // spi_master_write_dma(0, (uint8_t *)send_data1, 6);
    // while(spi_is_busy(0));

    codec_mono_int *psrc = (codec_mono_int *)pcm_data;                         //spk...spk-micl-micr...micl-micr
    codec_mono_int  spk_data[16 * MCU2DSP_DATA_MS];                            //mono spk
    adc_mono_int    mic_data[16 * MCU2DSP_DATA_MS * TLKMDI_HRA_MUSIC_CHN_NUM]; //stereo mic

    for (int i = 0; i < 16 * MCU2DSP_DATA_MS; i++) {
        spk_data[i]         = *(psrc + i);
        mic_data[2 * i]     = *(psrc + 16 * MCU2DSP_DATA_MS + 2 * i);
        mic_data[2 * i + 1] = *(psrc + 16 * MCU2DSP_DATA_MS + 2 * i + 1);
    }

    // #if TLKALG_MIC_SPK_MONO_ENABLE
    // short spk_data_mono[16 * MCU2DSP_DATA_MS];

    // for (int i = 0; i < 16 * MCU2DSP_DATA_MS; i++) {
    // 	spk_data_mono[i] = spk_data[2*i+1];
    // }

    // tlkdrv_codec_fillSpkBuff((uint8_t *)spk_data_mono, 16 * MCU2DSP_DATA_MS * 1 *sizeof(short));
    // #else
    // tlkdrv_codec_fillSpkBuff((uint8_t *)spk_data, 16 * MCU2DSP_DATA_MS * TLKMDI_HRA_MUSIC_CHN_NUM*sizeof(codec_mono_int));
    // #endif
    tlkdrv_codec_fillSpkBuff((uint8_t *)spk_data, 16 * MCU2DSP_DATA_MS * sizeof(codec_mono_int));
    tlkmdi_hra_fill_mid_mic_buff((uint8_t *)mic_data, 16 * MCU2DSP_DATA_MS * 2 * sizeof(adc_mono_int));
}

/**
 * @brief   Get idle size of middle microphone buffer
 * @param   None
 * @return  idle size of middle microphone buffer
 */
uint32_t tlkmdi_hra_get_mid_mic_idle_size(void)
{
    uint32_t idle = 0;

    if (s_hra_ctl.mid_mic_buff_rptr > s_hra_ctl.mid_mic_buff_wptr) {
        idle = s_hra_ctl.mid_mic_buff_rptr - s_hra_ctl.mid_mic_buff_wptr;
    } else {
        idle = CODEC_MIC_FIFO_SAMPLES + s_hra_ctl.mid_mic_buff_rptr - s_hra_ctl.mid_mic_buff_wptr;
    }

    return idle;
}

/**
 * @brief   Get available samples in middle microphone buffer
 * @param   None
 * @return  available samples count
 */
uint32_t tlkmdi_hra_get_mid_mic_available_sample(void)
{
    uint32_t available = CODEC_MIC_FIFO_SAMPLES - tlkmdi_hra_get_mid_mic_idle_size();
    return available;
}

/**
 * @brief   Fill middle microphone buffer with data
 * @param   pData - pointer to data to fill
 * @param   dataLen - length of data to fill
 * @return  operation status
 */
bool tlkmdi_hra_fill_mid_mic_buff(uint8_t *pData, uint16_t dataLen)
{
    uint32_t wptr;
    //    uint32_t rptr;
    uint16_t unUsed;
    uint16_t offset;
    uint8_t *pBuffer = (uint8_t *)g_mic_mid_buff;

    wptr   = s_hra_ctl.mid_mic_buff_wptr * sizeof(adc_int);
    unUsed = tlkmdi_hra_get_mid_mic_idle_size() * sizeof(adc_int);

    if (unUsed < dataLen) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "hra mic fill fail: %d %d %d %d", s_hra_ctl.mid_mic_buff_wptr, s_hra_ctl.mid_mic_buff_rptr,
                    tlkmdi_hra_get_mid_mic_idle_size(), dataLen / sizeof(adc_int));
        return false;
    }

    if (wptr + dataLen > CODEC_MIC_FIFO_SAMPLES * sizeof(adc_int)) {
        offset = CODEC_MIC_FIFO_SAMPLES * sizeof(adc_int) - wptr;
    } else {
        offset = dataLen;
    }
    tmemcpy(pBuffer + wptr, pData, offset);
    if (offset < dataLen) {
        tmemcpy(pBuffer, pData + offset, dataLen - offset);
    }

    wptr += dataLen;
    wptr &= (CODEC_MIC_FIFO_SAMPLES * sizeof(adc_int) - 1);

    s_hra_ctl.mid_mic_buff_wptr = wptr / sizeof(adc_int);

    return true;
}

/**
 * @brief   Get data from middle microphone buffer
 * @param   pData - pointer to store data
 * @param   dataLen - length of data to get
 * @return  operation status
 */
bool tlkmdi_hra_get_data_mid_mic_buff(uint8_t *pData, uint16_t dataLen)
{
    uint32_t wptr;
    uint32_t rptr;
    uint16_t available;
    uint16_t offset;
    uint8_t *pBuffer = (uint8_t *)g_mic_mid_buff;

    wptr      = s_hra_ctl.mid_mic_buff_wptr * sizeof(adc_int);
    rptr      = s_hra_ctl.mid_mic_buff_rptr * sizeof(adc_int);
    available = tlkmdi_hra_get_mid_mic_available_sample() * sizeof(adc_int);

    if (available < dataLen) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "hra mic get fail: %d %d %d %d", rptr, wptr, available, dataLen);
        return false;
    }

    if (rptr + dataLen <= CODEC_MIC_FIFO_SAMPLES * sizeof(adc_int)) {
        offset = dataLen;
    } else {
        offset = CODEC_MIC_FIFO_SAMPLES * sizeof(adc_int) - rptr;
    }
    tmemcpy(pData, pBuffer + rptr, offset);
    if (offset < dataLen) {
        tmemcpy(pData + offset, pBuffer, dataLen - offset);
    }

    rptr += dataLen;
    rptr &= (CODEC_MIC_FIFO_SAMPLES * sizeof(adc_int) - 1);

    s_hra_ctl.mid_mic_buff_rptr = rptr / sizeof(adc_int);

    return true;
}

/**
 * @brief   Get speaker buffer write pointer
 * @param   None
 * @return  speaker buffer write pointer
 */
uint16_t tlkmdi_hra_spk_get_wptr(void)
{
    return s_hra_ctl.mid_spk_buff_wptr;
}

/**
 * @brief   Get speaker buffer read pointer
 * @param   None
 * @return  speaker buffer read pointer
 */
uint16_t tlkmdi_hra_spk_get_rptr(void)
{
    return s_hra_ctl.mid_spk_buff_rptr;
}

/**
 * @brief   Get idle size of middle speaker buffer
 * @param   None
 * @return  idle size of middle speaker buffer
 */
uint32_t tlkmdi_hra_get_mid_spk_idle_size(void)
{
    uint32_t idle = 0;

    if (s_hra_ctl.mid_spk_buff_rptr > s_hra_ctl.mid_spk_buff_wptr) {
        idle = s_hra_ctl.mid_spk_buff_rptr - s_hra_ctl.mid_spk_buff_wptr;
    } else {
        idle = CODEC_SPK_FIFO_SAMPLES + s_hra_ctl.mid_spk_buff_rptr - s_hra_ctl.mid_spk_buff_wptr;
    }

    return idle;
}

/**
 * @brief   Get available samples in middle speaker buffer
 * @param   None
 * @return  available samples count
 */
uint32_t tlkmdi_hra_get_mid_spk_available_sample(void)
{
    uint32_t available = CODEC_SPK_FIFO_SAMPLES - tlkmdi_hra_get_mid_spk_idle_size();
    return available;
}

/**
 * @brief   Fill middle speaker buffer with data
 * @param   pData - pointer to data to fill
 * @param   dataLen - length of data to fill
 * @return  operation status
 */
bool tlkmdi_hra_fill_mid_spk_buff(uint8_t *pData, uint16_t dataLen)
{
    uint32_t wptr;
    //    uint32_t rptr;
    uint16_t unUsed;
    uint16_t offset;
    uint8_t *pBuffer = (uint8_t *)g_spk_mid_buff;

    wptr   = s_hra_ctl.mid_spk_buff_wptr * sizeof(hra_codec_int);
    unUsed = tlkmdi_hra_get_mid_spk_idle_size() * sizeof(hra_codec_int);

    if (unUsed < dataLen) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "hra spk fill fail: %d %d %d %d", s_hra_ctl.mid_spk_buff_wptr, s_hra_ctl.mid_spk_buff_rptr,
                    tlkmdi_hra_get_mid_spk_idle_size(), dataLen / sizeof(hra_codec_int));
        return false;
    }

    if (wptr + dataLen > CODEC_SPK_FIFO_SAMPLES * sizeof(hra_codec_int)) {
        offset = CODEC_SPK_FIFO_SAMPLES * sizeof(hra_codec_int) - wptr;
    } else {
        offset = dataLen;
    }
    tmemcpy(pBuffer + wptr, pData, offset);
    if (offset < dataLen) {
        tmemcpy(pBuffer, pData + offset, dataLen - offset);
    }

    wptr += dataLen;
    wptr &= (CODEC_SPK_FIFO_SAMPLES * sizeof(hra_codec_int) - 1);

    s_hra_ctl.mid_spk_buff_wptr = wptr / sizeof(hra_codec_int);

    return true;
}

/**
 * @brief   Get data from middle speaker buffer
 * @param   pData - pointer to store data
 * @param   dataLen - length of data to get
 * @return  operation status
 */
bool tlkmdi_hra_get_data_mid_spk_buff(uint8_t *pData, uint16_t dataLen)
{
    uint32_t wptr;
    uint32_t rptr;
    uint16_t available;
    uint16_t offset;
    uint8_t *pBuffer = (uint8_t *)g_spk_mid_buff;

    wptr      = s_hra_ctl.mid_spk_buff_wptr * sizeof(hra_codec_int);
    rptr      = s_hra_ctl.mid_spk_buff_rptr * sizeof(hra_codec_int);
    available = tlkmdi_hra_get_mid_spk_available_sample() * sizeof(hra_codec_int);

    if (available < dataLen) {
        tlkapi_warn(TLKDRV_CODEC_DBG_FLAG, TLKDRV_CODEC_DBG_SIGN, "hra spk get fail: %d %d %d %d", rptr, wptr, available, dataLen);
        return false;
    }

    if (rptr + dataLen <= CODEC_SPK_FIFO_SAMPLES * sizeof(hra_codec_int)) {
        offset = dataLen;
    } else {
        offset = CODEC_SPK_FIFO_SAMPLES * sizeof(hra_codec_int) - rptr;
    }
    tmemcpy(pData, pBuffer + rptr, offset);
    if (offset < dataLen) {
        tmemcpy(pData + offset, pBuffer, dataLen - offset);
    }

    rptr += dataLen;
    rptr &= (CODEC_SPK_FIFO_SAMPLES * sizeof(hra_codec_int) - 1);

    s_hra_ctl.mid_spk_buff_rptr = rptr / sizeof(hra_codec_int);

    return true;
}
#endif
