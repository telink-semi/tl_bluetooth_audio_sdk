/********************************************************************************************************
 * @file    ll_audio.h
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
#ifndef TLKMW_AUDIO_LL_AUDIO_LL_AUDIO_H_
#define TLKMW_AUDIO_LL_AUDIO_LL_AUDIO_H_

#include "common/types.h"
#if (TLKSTK_BT_TPS_ENABLE)

#define TLKALG_DSP_RET_NN_DATA_LEN_LL (320 * 2)

typedef struct
{
    uint8_t *p_tpsll_enc_buff;
    uint16_t tpsll_enc_buff_frame_size;
    uint16_t tpsll_enc_buff_num;
    uint16_t tpsll_enc_buff_wptr;
    int16_t  tpsll_enc_buff_rptr;
} tpsll_enc_buff_context_t;

extern tpsll_enc_buff_context_t g_tpsll_enc_buf_ctx;

/**
 * @brief      Set low latency audio volume in bt_voice async_audio mix mode.
 * @param[in]  vol - Volume level to set.
 * @return     None.
 */
void ll_audio_set_mix_vol(uint16_t vol);

/**
 * @brief      Switch bt_voice async_audio mix mode on or off.
 * @param[in]  onoff - 1 to turn on, 0 to turn off.
 * @return     None.
 */
void ll_audio_mix_switch(uint8_t onoff);

/**
 * @brief      Mix asynchronous audio and Bluetooth music in async_audio bt_music mix mode.
 * @param[in]  p0 - Pointer to first data buffer.
 * @param[in]  p1 - Pointer to second data buffer.
 * @param[in]  samples_num - Number of samples to mix.
 * @return     Number of mixed samples.
 */
uint16_t mix_ll_audio_stereo(int16_t *p0, int16_t *p1, uint16_t samples_num);

/**
 * @brief      Low latency audio decoder task for mixing with Bluetooth music.
 * @param      None.
 * @return     None.
 */
void ll_audio_mix_bt_music_mode_decoder_task_mcu(uint8_t times);

/**
 * @brief       Low latency main loop for mixing with Bluetooth music.
 * @param      None.
 * @return     None.
 */
void ll_audio_mix_mode_loop(void);

uint8_t ll_audio_is_in_mix_mode(void);

/**
 * @brief      Initialize the mixing of low latency audio and Bluetooth music.
 * @param      None.
 * @return     Initialization status (0 for success).
 */
int16_t ll_audio_bt_music_mix_init(void);

/**
 * @brief      Get the number of available PCM mix samples.
 * @param      None.
 * @return     Number of available PCM mix samples.
 */
int16_t get_number_of_pcm_mix_samples_available(void);

/**
 * @brief      Mix low latency audio and Bluetooth PCM data.
 * @param[in]  p0 - Pointer to first PCM data buffer.
 * @param[in]  p1 - Pointer to second PCM data buffer.
 * @param[in]  samples_num - Number of samples to process.
 * @return     None.
 */
void ll_audio_mix_pcm(int16_t *p0, int16_t *p1, int16_t samples_num);

/**
 * @brief      Adjust speaker PPM value.
 * @param[in]  ppm_spk_value - Pointer to PPM value to adjust.
 * @param[in]  samples - Number of samples used for adjustment.
 * @return     None.
 */
void ll_audio_spk_ppm_adjust(int *ppm_spk_value, int samples);

/**
 * @brief      Initialize speaker PPM settings for a specific channel.
 * @param[in]  channel - Audio channel to initialize.
 * @return     None.
 */
void ll_audio_spk_ppm_init(uint8_t channel);

/**
 * @brief      Deinitialize speaker PPM settings.
 * @param      None.
 * @return     None.
 */
void ll_audio_spk_ppm_deinit(void);

/**
 * @brief      Process speaker PPM for a specific channel.
 * @param[in]  ps - Source data pointer.
 * @param[in]  pd - Destination data pointer.
 * @param[in]  len - Length of data to process.
 * @param[in]  channel - Audio channel to process.
 * @return     Processed data length.
 */
uint16_t ll_audio_spk_ppm_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t channel);

/**
 * @brief      Handle received audio data.
 * @param[in]  tick - Timestamp of the received packet.
 * @param[in]  rx_packet_id - ID of the received packet.
 * @param[in]  wptr - Write pointer for data storage.
 * @param[in]  p_data - Pointer to received data.
 * @param[in]  len - Length of received data.
 * @return     None.
 */
void ll_audio_audio_rx(uint32_t tick, uint8_t rx_packet_id, uint8_t wptr, uint8_t *p_data, uint8_t len);

#if ((TLK_MW_DSP_COMM_ENABLE && !TLKADU_MIDBUF_ENABLE) && !TLK_CFG_HRA_ENABLE)

/**
 * @brief      Process DSP messages for low latency voice communication.
 * @param[in]  enc_buff_wptr - Write pointer for encoded buffer.
 * @param[in]  type - Message type identifier.
 * @return     None.
 */
void ll_voice_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type);

/**
 * @brief      Synchronize microphone and speaker indices for low latency voice communication.
 * @param[in]  sample - Sample value for synchronization.
 * @return     None.
 */
void ll_voice_dsp_dp_sync_mic_spk_index(uint16_t sample);

/**
 * @brief      Retrieve microphone data for low latency voice communication.
 * @param[in]  p_des - Destination buffer for retrieved data.
 * @param[in]  samples - Number of samples to retrieve.
 * @return     Retrieved data size.
 */
uint8_t ll_voice_dsp_dp_get_mic_data(int *p_des, uint16_t samples);

/**
 * @brief      Get available length of AVIA buffer.
 * @param[in]  wptr - Write pointer.
 * @param[in]  rptr - Read pointer.
 * @return     Available buffer length.
 */
uint16_t ll_voice_get_avia_len(uint16_t wptr, uint16_t rptr);

/**
 * @brief      Clear DSP return buffer status.
 * @param      None.
 * @return     None.
 */
void ll_voice_clear_dsp_ret_buff_status(void);

#endif

/**
 * @brief      Open microphone for low latency audio mixing.
 * @param      None.
 * @return     None.
 */
void ll_audio_mix_open_mic(void);

/**
 * @brief      Switch microphone for low latency audio mixing.
 * @param      None.
 * @return     None.
 */
void ll_audio_mix_switch_mic(void);

/**
 * @brief      Set FIFO IRQ threshold in bytes.
 * @param[in]  byte_num - Threshold in bytes.
 * @return     None.
 */
void ll_mix_set_fifo_irq_num(uint16_t byte_num);

/**
 * @brief      Handler function for FIFO0 IRQ.
 * @param      None.
 * @return     None.
 */
void ll_mix_fifo0_irq_handler_func(void);

/**
 * @brief      Set microphone mode for low latency audio mixing.
 * @param[in]  mode - Microphone mode to set.
 * @return     None.
 */
void ll_mix_voice_set_mic_mode(uint8_t mode);

/**
 * @brief      Get current microphone mode for low latency audio mixing.
 * @param      None.
 * @return     Current microphone mode.
 */
uint8_t ll_mix_voice_get_mic_mode(void);

/**
 * @brief      Set microphone start flag.
 * @param[in]  flag - Flag indicating whether the microphone is started.
 * @return     None.
 */
void ll_mix_set_mic_start_flag(uint8_t flag);

/**
 * @brief      Get microphone start flag.
 * @param      None.
 * @return     Microphone start flag.
 */
uint8_t ll_mix_get_mic_start_flag(void);

/**
 * @brief      Deinitialize microphone for low latency audio mixing.
 * @param      None.
 * @return     None.
 */
void ll_mix_mic_deinit(void);

/**
 * @brief      Set low latency microphone start flag for Bluetooth music mixing.
 * @param[in]  flag - Flag indicating whether the microphone is started.
 * @return     None.
 */
void ll_mix_bt_music_set_ll_mic_start_flag(uint8_t flag);

/**
 * @brief      Get low latency microphone start flag for Bluetooth music mixing.
 * @param      None.
 * @return     Low latency microphone start flag.
 */
uint8_t ll_mix_bt_music_get_ll_mic_start_flag(void);

/**
 * @brief      Deinitialize microphone for Bluetooth music mixing.
 * @param      None.
 * @return     None.
 */
void ll_mix_bt_music_mic_deinit(void);

/**
 * @brief      Initialize audio algorithms.
 * @param      None.
 * @return     None.
 */
void ll_audio_alg_init(void);

/**
 * @brief      Deinitialize audio algorithms.
 * @param      None.
 * @return     None.
 */
void ll_audio_alg_deinit(void);

/**
 * @brief      Get current audio mode.
 * @param[in]  pData - Data pointer for mode retrieval.
 * @return     Current audio mode.
 */
uint16_t ll_audio_get_mode(uint8_t *pData);

/**
 * @brief      Open codec for audio processing.
 * @param      None.
 * @return     None.
 */
void ll_audio_open_codec(void);

/**
 * @brief      Switch input source for audio processing.
 * @param      None.
 * @return     None.
 */
void ll_audio_switch_in(void);

/**
 * @brief      Switch output destination for audio processing.
 * @param      None.
 * @return     None.
 */
void ll_audio_switch_out(void);

uint8_t *ll_audio_mix_get_enc_buff_ptr(void);
void     ll_audio_mix_update_enc_buff_wptr(void);
void     ll_audio_mix_update_enc_buff_rptr(void);
uint8_t  ll_audio_mix_enc_buff_available(void);
void     ll_audio_bt_voice_mix_mode_uplink_xfer(void);
void     ll_audio_bt_music_mix_mode_uplink_xfer(void);

#endif
#endif /* TLKMW_AUDIO_LL_AUDIO_LL_AUDIO_H_ */
