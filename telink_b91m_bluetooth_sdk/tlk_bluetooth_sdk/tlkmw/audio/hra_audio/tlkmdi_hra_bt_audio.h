/********************************************************************************************************
 * @file    tlkmdi_hra_bt_audio.h
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
#ifndef _TLKMDI_HRA_BT_AUDIO_H_
#define _TLKMDI_HRA_BT_AUDIO_H_

#define TLKMDI_HRA_MUSIC_MIC_EN     1

#define HRA_MUSIC_ID            IPC_DATA_PATH_0
#define HRA_VOICE_ID            IPC_DATA_PATH_0

typedef struct
{
    uint16_t mid_spk_buff_wptr;
    uint16_t mid_spk_buff_rptr;
    uint16_t mid_mic_buff_wptr;
    uint16_t mid_mic_buff_rptr;
    uint32_t sample_rate;
    bool     voice_start_timer_flag;
    bool     stack_start_timer_flag;
} tlkmdi_hra_ctl;

#if AUDIO_PATH_24BITS_EN
typedef int64_t hra_codec_int;
#else
typedef int32_t hra_codec_int;
#endif

/**
 * @brief   Process BT audio data
 * @param   None
 * @return  None
 */
void tlkmdi_hra_bt_audio_proc(void);

/**
 * @brief   Send BT audio data to DSP
 * @param   None
 * @return  None
 */
void tlkmdi_hra_bt_audio_send_data_to_dsp(void);

/**
 * @brief   Initialize HRA music module
 * @param   None
 * @return  None
 */
void tlkmdi_hra_music_init(void);

/**
 * @brief   Callback function for processing music DSP messages
 * @param   enc_buff_wptr - encode buffer write pointer
 * @param   type - message type
 * @return  None
 */
void tlkmdi_hra_music_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type);

/**
 * @brief   Initialize HRA voice module
 * @param   None
 * @return  None
 */
void tlkmdi_hra_voice_init(void);

/**
 * @brief   Callback function for processing voice DSP messages
 * @param   enc_buff_wptr - encode buffer write pointer
 * @param   type - message type
 * @return  None
 */
void tlkmdi_hra_voice_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type);

/**
 * @brief   Set stack timer flag
 * @param   flag - timer flag value
 * @return  None
 */
void tlkmdi_hra_stack_set_timer_flag(bool flag);

/**
 * @brief   Get stack timer flag
 * @param   None
 * @return  stack timer flag
 */
bool tlkmdi_hra_stack_get_timer_flag(void);

/**
 * @brief   Get speaker buffer write pointer
 * @param   None
 * @return  speaker buffer write pointer
 */
uint16_t tlkmdi_hra_spk_get_wptr(void);

/**
 * @brief   Get speaker buffer read pointer
 * @param   None
 * @return  speaker buffer read pointer
 */
uint16_t tlkmdi_hra_spk_get_rptr(void);

/**
 * @brief   Get idle size of middle microphone buffer
 * @param   None
 * @return  idle size of middle microphone buffer
 */
uint32_t tlkmdi_hra_get_mid_mic_idle_size(void);

/**
 * @brief   Get available samples in middle microphone buffer
 * @param   None
 * @return  available samples count
 */
uint32_t tlkmdi_hra_get_mid_mic_available_sample(void);

/**
 * @brief   Fill middle microphone buffer with data
 * @param   pData - pointer to data to fill
 * @param   dataLen - length of data to fill
 * @return  operation status
 */
bool tlkmdi_hra_fill_mid_mic_buff(uint8_t *pData, uint16_t dataLen);

/**
 * @brief   Get data from middle microphone buffer
 * @param   pData - pointer to store data
 * @param   dataLen - length of data to get
 * @return  operation status
 */
bool tlkmdi_hra_get_data_mid_mic_buff(uint8_t *pData, uint16_t dataLen);

/**
 * @brief   Get idle size of middle speaker buffer
 * @param   None
 * @return  idle size of middle speaker buffer
 */
uint32_t tlkmdi_hra_get_mid_spk_idle_size(void);

/**
 * @brief   Get available samples in middle speaker buffer
 * @param   None
 * @return  available samples count
 */
uint32_t tlkmdi_hra_get_mid_spk_available_sample(void);

/**
 * @brief   Fill middle speaker buffer with data
 * @param   pData - pointer to data to fill
 * @param   dataLen - length of data to fill
 * @return  operation status
 */
bool tlkmdi_hra_fill_mid_spk_buff(uint8_t *pData, uint16_t dataLen);

/**
 * @brief   Get data from middle speaker buffer
 * @param   pData - pointer to store data
 * @param   dataLen - length of data to get
 * @return  operation status
 */
bool tlkmdi_hra_get_data_mid_spk_buff(uint8_t *pData, uint16_t dataLen);

#endif