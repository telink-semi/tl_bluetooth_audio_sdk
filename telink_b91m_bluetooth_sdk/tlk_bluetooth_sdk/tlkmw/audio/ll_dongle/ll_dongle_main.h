/********************************************************************************************************
 * @file    ll_dongle_main.h
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
#ifndef __APP_AUDIO_H_
#define __APP_AUDIO_H_

#if ((MCU_CORE_TYPE == MCU_CORE_TL721X) || (MCU_CORE_TYPE == MCU_CORE_TL322X))

#if (MCU_CORE_TYPE == MCU_CORE_TL322X)
#define UDB_DEBUG_ENABLE 1
#else
#define UDB_DEBUG_ENABLE 0
#endif
#define BITDEPTH_24_ENABLE 1

#define APP_USB_ASRC_DEBUG 0
#define APP_USB_MIC        0

#define APP_AUDIO_DEBUG_EN 0

/** Local playback */
#define APP_AUDIO_CODEC_EN                 (APP_MODE != APP_MODE_AUDIO_AUTO)

#define APP_AUDIO_CODEC_MIC_MUSIC_MIX_EN   0
#define APP_AUDIO_CODEC_MIC_DATA_ONLY_EN   0
#define APP_AUDIO_CODEC_MUSIC_DATA_ONLY_EN 1

#define APP_MIC_VOL_ADJUST_EN              0

#define APP_VOLUME_RELATIVE_VALUE_NUM      101
#define APP_VOLUME_SCALE_NUM               101
#define APP_CC_VOLUME_SCALE_NUM            101
#define APP_AUDIO_SPEAKER_VOL_LINEAR_MIN   0
#define APP_AUDIO_SPEAKER_VOL_LINEAR_MAX   16384

#define APP_AUDIO_1MS_TICK                 (1000 * 24) //TICK_PER_US = 24
#define APP_AUDIO_2MS_TICK                 (2 * APP_AUDIO_1MS_TICK)

#if BITDEPTH_24_ENABLE
#define APP_AUDIO_CHANNEL_SHIFT_BITS 32
#else
#define APP_AUDIO_CHANNEL_SHIFT_BITS 16
#endif

#define APP_HALF(n)                        ((n) >> 1)

#define APP_AUDIO_ADJUST_VOLUME_SHIFT_BITS 14
#define APP_AUDIO_DEFAULT_VOL_DB           0x7fff
#define APP_AUDIO_DEFAULT_VOL_STEP         0xff
#define APP_AUDIO_SPK_MUTE_MASK            0x80
#define APP_AUDIO_SPK_VOL_GRDL_STEP        2
#define APP_AUDIO_VALID_SAMPLE_TH          5

#define APP_AUDIO_DATA_MUTE_DETECT_TIME    1500000
#define APP_AUDIO_DATA_ACTIVE_DETECT_TIME  2000000
/** Sampling rate */
#define APP_AUDIO_SAMPLE_RATE_16K 16000
#define APP_AUDIO_SAMPLE_RATE_48K 48000
/** Number of audio data samples in 1ms */
#define APP_AUDIO_1MS_SAMPLES (APP_AUDIO_SAMPLE_RATE_48K / 1000)
/** Number of audio data samples in 2ms */
#define APP_AUDIO_2MS_SAMPLES (APP_AUDIO_1MS_SAMPLES * 2)
/** Number of audio data samples in 8ms */
#define APP_AUDIO_8MS_SAMPLES (APP_AUDIO_1MS_SAMPLES * 8)
/** Number of audio data samples in 10ms */
#define APP_AUDIO_10MS_SAMPLES                  (APP_AUDIO_1MS_SAMPLES * 10)
#define APP_AUDIO_5MS_SAMPLES                   (APP_AUDIO_1MS_SAMPLES * 5)
#define APP_AUDIO_AUDIO_PART1_SAMPLES           300
#define APP_AUDIO_AUDIO_PART2_SAMPLES           60
#define APP_AUDIO_48K_TO_16K_IN_SAMPLES         (APP_AUDIO_10MS_SAMPLES / 4) /** 120 */
#define APP_AUDIO_48K_TO_16K_OUT_SAMPLES        160

#define APP_AUDIO_PCM_BUFF_PREVENT_OVERFLOW_LEN 8
#define APP_AUDIO_SYNC_RESET_SAMPLE             (84 + 48)
#define APP_AUDIO_SYNC_SAMPLES_REF              305
#define APP_AUDIO_SYNC_DIFF_TH                  60

#define APP_AUDIO_DATA_BUFF_OFFSET_NUM          64
#define APP_AUDIO_DATA_MSBC_COPY_OFFSET         8
#define APP_AUDIO_DATA_MSBC_COPY_LEN            53
#define APP_AUDIO_DATA_LC3_COPY_LEN             40
/** The page length of encoded data buffer in async.data. */
#define APP_AUDIO_ENC_DATA_PAGE_LEN 128

/** The offset position in async.data page to save encoded data */
#define APP_AUDIO_ENC_DATA_PAGE_DATA_OFFSET 14

#define APP_SYNC_MIC_TASK_FLAG_MASK         0x10
#define APP_SYNC_CMD_MIC_DATA_FORMAT_MASK   0xF8
#define APP_SYNC_AUDIO_DATA_PKT_PTR_MASK    0x07
#define APP_SYNC_HEADSET_SLOT_NO(fno)       ((fno >> 4) & 15)

#define APP_MIC_DEFAULT_VOLUME              1023
#define APP_MIC_VOLUME_MAX                  1024
#define APP_MIC_MUTE_MASK                   0x80
#define APP_MIC_VOL_IDX_MAX                 20
/** In order to prevent the problem of data delay when the communication status
 *  is bad, delay 4 * 5ms to process data.
 */
#define APP_MIC_DATA_PROCESSING_DELAY         4
#define APP_MIC_DATA_OFFSET                   4
#define APP_MIC_FRAME_SAMPLES                 160
#define APP_MIC_ULTRA_LL_FRAME_SAMPLES        80
#define APP_MIC_PCM_SAMPLES                   (APP_MIC_FRAME_SAMPLES * 3)
#define APP_MIC_PCM_BUFF_PREVENT_OVERFLOW_LEN 4
#define APP_MIC_LC3_DEC_BUFF_LEN              10736
#define APP_MIC_LC3_DEC_SCRATCH_BUFF_LEN      4096
#define APP_ALG_AUDIO_BUFF_LEN                6144
#define APP_MIC_LC3_ENC_BUFF_LEN              5500
#define APP_ENC_SCRATCH_BUFF_LEN              (1024 * 10)
#define APP_MIC_SBC_DEC_LEN                   41
#define APP_MIC_LC3_DEC_LEN                   30
#define APP_MIC_LC3_ULTRA_LL_DEC_LEN          20

#define APP_SBC_DEC_BLOCKS                    10
#define APP_SBC_DEC_BITPOOL                   26

#define APP_ASRC_PROCESS_MAX_NUM              168

#define APP_ASRC_PPM_BASE_FILTER_LEN          12

typedef struct
{
    uint8_t  tone_st;
    uint16_t vol;
    uint32_t pkt_audio;
    uint8_t  audio_mute;
    uint8_t  plc_num;

    uint8_t audio_per;
    uint8_t sync;
    uint8_t dev_mode_peer;
    uint8_t peer_bt_mode;
    uint8_t dev_bt_mode_last;

    uint8_t pkt_wptr;
    uint8_t pkt_rptr;
    uint8_t pkt_len;

    uint8_t  wptr;
    uint8_t  format;
    uint16_t ll_rcvd;

    uint32_t tick0;
    uint32_t tus_mic;
    uint32_t tus_single_tpsll_enc_offset;
    uint32_t tick_timer;
    uint16_t dump_mask;

    /* mix mode */
    uint8_t  rx_pkt;
    uint32_t mix_tick;
    uint8_t  mix_wptr;
    uint16_t bt_mix_reset;
} __attribute__((aligned(4))) ll_dongle_audio_context_t;

extern ll_dongle_audio_context_t ll_dongle_audio_ctx;

typedef struct audio_music
{
    int16_t idx; /**< index */
#if BITDEPTH_24_ENABLE
    int64_t pcm[APP_AUDIO_10MS_SAMPLES + APP_AUDIO_PCM_BUFF_PREVENT_OVERFLOW_LEN];
#else
    int32_t pcm[APP_AUDIO_10MS_SAMPLES + APP_AUDIO_PCM_BUFF_PREVENT_OVERFLOW_LEN];
#endif
} audio_music_t;

extern audio_music_t g_audio;

typedef enum
{
    TLKMDI_LL_DONGLE_IDLE = 0,
    TLKMDI_LL_DONGLE_ENC,
    TLKMDI_LL_DONGLE_DEC,
    TLKMDI_LL_DONGLE_BT_VOICE_MIX_ENC,
    TLKMDI_LL_DONGLE_STATE_MAX
} tlkmdi_ll_dongle_state_e;

/********************************************************/
/*TEST*/
extern uint8_t g_usb_mute; /*async.usb_mute*/
extern uint8_t g_max_vol;  /*async.max_vol*/

#define ASYNC_FLOW_48K_MODE               0x80
#define ASYNC_FLOW_16K_MODE               0x40


#define APP_AUDIO_LC3A_FRMAE_LEN          90
#define APP_AUDIO_ULTRA_LL_LC3A_FRMAE_LEN 50

#ifndef APP_AUDIO_LC3A_FRMAE_LEN
#define APP_AUDIO_LC3A_FRMAE_LEN 90
#endif

extern uint8_t g_audio_dsample_pos;

/********************************************************/

/**
 * @brief   Main audio processing loop for the dongle.
 * @param   None
 * @return  None
 */
void ll_dongle_audio_main(void);

/**
 * @brief   Enter audio mode for the dongle.
 * @param   None
 * @return  None
 */
void ll_dongle_audio_enter_audio_mode(void);

/**
 * @brief   Clear all status flags for the dongle.
 * @param   None
 * @return  None
 */
void ll_dongle_clear_status(void);

/**
 * @brief   Callback function for handling audio path commands.
 * @param[in] cmd The command to process.
 * @return  uint8_t - Result of the callback processing.
 */
uint8_t ll_dongle_audio_path_callback(uint16_t cmd);

/**
 * @brief   Get the current microphone sampling rate.
 * @param   None
 * @return  uint32_t - The current microphone sampling rate.
 */
uint32_t usb_audio_get_mic_cur_rate(void);

/**
 * @brief   Get the timestamp for microphone encoding offset.
 * @param   None
 * @return  uint32_t - The timestamp for microphone encoding offset.
 */
uint32_t ll_audio_get_tus_mic_enc_offset(void);

/**
 * @brief   Get the timestamp for microphone encoding.
 * @param   None
 * @return  uint32_t - The timestamp for microphone encoding.
 */
uint32_t ll_audio_get_tus_mic_enc(void);

/**
 * @brief   Get the timestamp for speaker decoding.
 * @param   None
 * @return  uint32_t - The timestamp for speaker decoding.
 */
uint32_t ll_audio_get_tus_spk_dec(void);

/**
 * @brief   Get the length of the timestamp frame.
 * @param   None
 * @return  uint16_t - The length of the timestamp frame.
 */
uint16_t ll_audio_get_tus_frame_length(void);

/**
 * @brief Get frame length for low latency audio.
 * @param[in] none
 * @return Frame length.
 */
uint16_t ll_audio_get_frame_length(void);

#endif

#endif /*__APP_AUDIO_H_*/
