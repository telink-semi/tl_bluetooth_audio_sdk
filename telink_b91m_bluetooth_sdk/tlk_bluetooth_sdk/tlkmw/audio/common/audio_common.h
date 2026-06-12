/********************************************************************************************************
 * @file    audio_common.h
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
#ifndef __AUDIO_COMMON_H__
#define __AUDIO_COMMON_H__

#include "tlkalg/audio/audio_alg_interface.h"

#define AUDIO_GLOBAL_INT_DISABLE() //tlksys_enter_critical()
#define AUDIO_GLOBAL_INT_RESTORE() //tlksys_leave_critical()

extern volatile uint32_t codec_flag;

enum
{
    CODEC_FLAG_MUSIC    = (0x01L << 0),
    CODEC_FLAG_BT_MUSIC = (0x01L << 0),
    CODEC_FLAG_VOICE    = (0x01L << 1),
    CODEC_FLAG_BT_VOICE = (0x01L << 1),

    CODEC_FLAG_LE_MUSIC = (0x01L << 2),
    CODEC_FLAG_LE_VOICE = (0x01L << 3),
    CODEC_FLAG_IDLE     = (0x01L << 7),

    CODEC_FLAG_EQ_MUSIC_EN     = (0x01L << 4),
    CODEC_FLAG_EQ_VOICE_MIC_EN = (0x01L << 5),
    CODEC_FLAG_EQ_VOICE_SPK_EN = (0x01L << 6),

    CODEC_FLAG_MUSIC_EQ_STATR     = (0x01L << 8),
    CODEC_FLAG_VOICE_EQ_MIC_STATR = (0x01L << 9),
    CODEC_FLAG_VOICE_EQ_SPK_STATR = (0x01L << 10),
    CODEC_FLAG_BT_VOICE_IDLE      = (0x01L << 11),
};

typedef enum
{
    AUDIO_CODEC_DAC_MONO,
    AUDIO_CODEC_DAC_STEREO,
    AUDIO_CODEC_DAC_TYPE_NUM,
} AUDIO_CODEC_DAC_TYPE_ENUM;

/**
 * @brief       Get the codec DAC type
 * @return      Current codec DAC type
 */
AUDIO_CODEC_DAC_TYPE_ENUM tlkmdi_audio_get_codec_dac_type(void);

/**
 * @brief       Set audio codec flag
 * @param[in]   flag - Flag to set
 * @param[in]   en - Enable or disable the flag
 * @return      None
 */
static inline void audio_codec_flag_set(unsigned int flag, int en)
{
    AUDIO_GLOBAL_INT_DISABLE();
    if (en) {
        codec_flag |= flag;
    } else {
        codec_flag &= ~flag;
    }
    AUDIO_GLOBAL_INT_RESTORE();
}

/**
 * @brief       Get audio codec flag status
 * @param[in]   flag - Flag to check
 * @return      Flag status
 */
static inline unsigned int audio_codec_flag_get(unsigned int flag)
{
    u32 ret = 0;
    AUDIO_GLOBAL_INT_DISABLE();

    ret = codec_flag & flag;

    AUDIO_GLOBAL_INT_RESTORE();

    return ret;
}

/**
 * @brief       Check if ANC BT music is busy (without macro)
 * @return      True if busy, false otherwise
 */
bool tlkmdi_anc_btmusic_is_busy_no_macro(void);

/**
 * @brief       Check if ANC BT voice is busy (without macro)
 * @return      True if busy, false otherwise
 */
bool tlkmdi_anc_btvoice_is_busy_no_macro(void);

/**
 * @brief       Inform DSP synchronization for ANC
 * @return      None
 */
void tlkmdi_anc_inform_dsp_sync(void);

/**
 * @brief       Synchronize mic and speaker index for hybrid algorithm
 * @param[in]   sample - Sample count
 * @return      None
 */
void tlkalg_sync_mic_spk_index(uint16_t sample);

/**
 * @brief       Process AGC after neural network processing
 * @param[in]   ps - Source buffer pointer
 * @param[out]  pd - Destination buffer pointer
 * @return      None
 */
void tlkalg_agc_proc_behind_nn(uint8_t *ps, uint8_t *pd, uint8_t width);

/**
 * @brief       Process ANS after neural network processing
 * @param[in]   ps - Source buffer pointer
 * @param[out]  pd - Destination buffer pointer
 * @return      None
 */
void tlkalg_ans_proc_behind_nn(uint8_t *ps, uint8_t *pd);

/**
 * @brief       Get microphone frame data
 * @param[out]  pdes - Destination buffer pointer
 * @param[in]   len - Data length
 * @return      Status
 */
uint8_t tlkalg_get_mic_frame(int16_t *pdes, uint16_t len);

/**
 * @brief       Set tone mixing mode
 * @param[in]   mode - Mixing mode to set
 * @return      None
 */
void tlkmdi_audio_set_tone_mix_mode(uint8_t mode);

/**
 * @brief       Get current tone mixing mode
 * @return      Current mixing mode
 */
uint8_t tlkmdi_audio_get_tone_mix_mode(void);

/**
 * @brief       Process multi-frame ANS when speaker ANS is enabled
 * @param[in]   ps - Source buffer pointer
 * @param[out]  pd - Destination buffer pointer
 * @param[in]   frame_num - Number of frames to process
 * @return      None
 */
#if (TLKALG_ANS_SPK_ENABLE)
void tlkalg_ans_multiframe(uint8_t *ps, uint8_t *pd, uint8_t frame_num);
#endif

/**
 * @brief       Check if audio dB level is less than threshold
 * @param[in]   thr - Threshold value
 * @param[in]   data - Audio data buffer
 * @param[in]   len - Data length
 * @return      True if less than threshold, false otherwise
 */
bool tlkmdi_audio_dB_is_less_threshold(float thr, short *data, uint16_t len);

#endif
