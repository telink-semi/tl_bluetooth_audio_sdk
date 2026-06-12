/********************************************************************************************************
 * @file    ll_audio_main.h
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
#ifndef __LL_AUDIO_MAIN_H__
#define __LL_AUDIO_MAIN_H__

#if (TLKSTK_BT_TPS_ENABLE)


#include "stack/tpsll/controller/tph/tph_host_interface.h"

#define APP_AUDIO_LOG_EN         (1)
#define EQ_SAMPLES_STEP          40
#define SAMPLES                  480
#define PCM_MIX_SAMPLES          (SAMPLES * 3 + 240)
#define LL_10MS_SAMPLES          480
#define LL_10MS_MIC_LC3_DATA_LEN (30)

/** TPSLL ENCODE BUFFER */
#define TPSLL_AUDIO_PACKET_LEN    (ASYNC_AUDIO_LC3A_FRMAE * 2 + 2)
#define TPSLL_ENC_BUFF_FRAME_SIZE TPSLL_AUDIO_PACKET_LEN
#define TPSLL_ENC_BUFF_NUM        16

/** TPSLL ULTRA LL MODE */
#define ULTRA_LL_SAMPLES              240
#define ULTRA_LL_PCM_MIX_SAMPLES      (ULTRA_LL_SAMPLES * 3 + 20)
#define ULTRA_LL_5MS_SAMPLES          240
#define ULTRA_LL_5MS_MIC_LC3_DATA_LEN (20)

/** TPSLL ULTRA LL MODE  ENCODE BUFFER */
#define TPSLL_ULTRA_LL_AUDIO_PACKET_LEN    (ASYNC_AUDIO_LC3A_ULTRA_LL_FRMAE * 2 + 2)
#define TPSLL_ULTRA_LL_ENC_BUFF_FRAME_SIZE TPSLL_ULTRA_LL_AUDIO_PACKET_LEN


#define LL_AUDIO_DBG_FLAG                  ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_SNK << 16) | TLK_DEBUG_DBG_FLAG_ALL)

#define LL_AUDIO_DBG_SIGN                  NULL

#define TPSLL_LATENCY_NORMAL               0
#define TPSLL_LATENCY_PKTLOSS              1

#define MIC_BUF_CAP                        (160 * 2 + 32) // 352

extern uint8_t *g_ll_audio_dec_buf_ptr;
extern int16_t  g_ll_voice_mode;

//extern codec_mono_int pcm_chn1[96];

typedef struct async_audio_context
{
    uint32_t pkt_audio;
    uint8_t  audio_mute;
    uint8_t  plc_num;

    uint8_t audio_per;
    uint8_t pkt_rptr;
    uint8_t wptr;

    uint32_t tick0;
    uint32_t tick_stimer_trigger;
    uint32_t tus_mic;
    uint32_t tick_timer;
    uint16_t dump_mask;
    uint8_t  timer_started;
    uint32_t data_rcv_tick;

    uint8_t  latency_mode;
    uint16_t pcm_buf_mix_samples;

    /* mix mode */
    uint8_t           is_mix_mode;
    uint32_t          mix_tick;
    uint8_t           mix_wptr;
    uint8_t           mix_rptr;
    uint8_t           mix_decoded_rptr;
    uint8_t           rx_packet_id;
    uint8_t           sco_num_in_bt;
    uint8_t           stimer_ref_wptr;
    uint8_t           next_tpsll_rptr_and_flag;
    volatile uint32_t stimer_irq_tick;
    uint8_t           bt_page_state;
    uint8_t           tpsll_uplink_enc_pkt_sent_cnt;
    uint8_t           tpsll_mix_dec_num;

    volatile uint8_t tpsll_mix_dec_loop_ongoing;
    volatile uint8_t tpsll_5ms_dec_times;
    volatile uint8_t tpsll_dec_is_ongoing;
    volatile uint8_t tpsll_dec_update_mix_buf_is_ongoing;
    volatile uint8_t tpsll_enc_is_ongoing;
    volatile uint8_t tpsll_mix_bt_dec_cnt;
    volatile uint8_t tpsll_mix_lc3_dec_cnt;
    uint8_t          mic_state;
} __attribute__((aligned(4))) async_audio_context_t;

extern async_audio_context_t async_audio_ctx;

typedef struct
{
    uint8_t ultra_ll_last_mode;
    uint8_t ultra_ll_flag;
    uint8_t ultra_ll_mode_change;
} tpsll_ultra_ll_context_t;

extern tpsll_ultra_ll_context_t g_tpsll_ultra_ll_ctx;

enum
{
    LL_BT_MUSIC_MIX_DEC_S1_START = 0x1,
    LL_BT_MUSIC_MIX_DEC_S1_END   = 0x2,
    LL_BT_MUSIC_MIX_DEC_S2_START = 0x3,
    LL_BT_MUSIC_MIX_DEC_S2_END   = 0x4,
};

enum
{
    AYNC_FORMAT_1M       = 0x28,
    AYNC_FORMAT_1M25     = 0x29,
    AYNC_FORMAT_MIC_TEST = 0x2b,
};

enum
{
    ASYNC_AUDIO_LC3A_FRMAE = 90,
    ASYNC_AUDIO_DATA_LEN   = ASYNC_AUDIO_LC3A_FRMAE,

    ASYNC_AUDIO_LC3A_ULTRA_LL_FRMAE = 45,
    ASYNC_AUDIO_ULTRA_LL_DATA_LEN   = ASYNC_AUDIO_LC3A_ULTRA_LL_FRMAE,

    // TPH_RF_PACKET_DONGLE_MAX_MSG_PDU_LENGTH(90) + 12
    ASYNC_AUDIO_DATA_FLAG   = 0,
    ASYNC_AUDIO_DATA_READY0 = 1,
    ASYNC_AUDIO_DATA_READY1 = 2,
    ASYNC_AUDIO_DATA_NULL0  = 4,
    ASYNC_AUDIO_DATA_NULL1  = 8,

    ASYNC_BT_VOICE_OFFSET = 540,

    ASYNC_SYNC_REF    = 154,
    ASYNC_SYNC_WINDOW = 200,

    ASYNC_SYNC_AUDIO_TAG  = 0x9c,
    ASYNC_PAIRING_CHN     = 10,
    ASYNC_PAIRING_RSSI_TH = 0x38,

    ASYNC_HEARTBEAT_MODE = 10,

    ASYNC_LATENCY_PKTLOSS = 6,
    ASYNC_LATENCY_NORMAL  = 2,

    ASYNC_FLG_RX_ERROR = BIT(6),
    ASYNC_FLG_RX_CHN   = BIT(7),

    ASYNC_MODE_DEV0 = 0x04,
    ASYNC_MODE_DEV1 = 0x08,
};

typedef enum
{
    ASYNC_DUMP_CHN_PER      = 1,
    ASYNC_DUMP_CHN_MAP      = 2,
    ASYNC_DUMP_SAMPLE_DRIFT = 4,
    ASYNC_DUMP_NULL_PACKET  = 8,
    ASYNC_DUMP_XCAP_CAL     = 0x10,
    ASYNC_DUMP_DEC_SAMPLES  = 0x20,
    ASYNC_DUMP_RUN_TIME     = 0x40,
    ASYNC_DUMP_SYNC_STATUS  = 0x80,
    ASYNC_DUMP_SYNC_CLKN    = 0x100,
} async_dump_e;

typedef enum
{
    ASYNC_AUDIO_FORMAT_MSBC     = 0,
    ASYNC_AUDIO_FORMAT_LC3A     = 1,
    ASYNC_AUDIO_FORMAT_16K_MASK = 0x04,
} app_tph_audio_format_e;

/**
 * @brief Set low latency audio mode.
 * @param[in] mode - Audio mode to set.
 * @return none
 */
void ll_audio_set_mode(uint8_t mode);

/**
 * @brief Low latency audio main handler, including encoding and decoding process.
 * @param[in] none
 * @return none
 */
void ll_audio_main(void);

/**
 * @brief Low latency audio mic data process task, called by timer0 irq handler.
 * @param[in] idx - Mic ID.
 * @return none
 */
uint8_t ll_audio_mic_data_process_task_mcu(uint8_t idx);

/**
 * @brief Check mic samples in FIFO and reset MIC samples if they are not within the threshold.
 * @param[in] samples_num - Number of samples.
 * @param[in] tolerance - Tolerance value.
 * @return none
 */
void ll_audio_sync_mic_samples(uint16_t samples_num, uint16_t tolerance);

/**
 * @brief Check the samples in FIFO and reset playback samples if they are not within the threshold.
 * @param[in] none
 * @return none
 */
void ll_audio_sync_samples(void);

/**
 * @brief Check the low latency audio status then update async audio parameters.
 * @param[in] p_src - Async audio data.
 * @return none
 */
void ll_audio_packet_check(uint8_t *p_src);

/*disable by mingqian 20250704, confirm by xiaogang.*/
#if (0)
void ll_audio_update_sco_msg_rx_params(uint32_t tick, tph_sco_msg_pdu_header_for_host_t *p_pdu_format);
#endif

/**
 * @brief Start timer encode task for low latency audio.
 * @param[in] none
 * @return Status of starting the timer.
 */
uint8_t ll_audio_start_timer_encode_task(void);

/**
 * @brief Convert PCM S32 format to S16 format.
 * @param[in] dst - Destination buffer for S16 data.
 * @param[in] src - Source buffer with S32 data.
 * @param[in] len - Length of data to convert.
 * @return Result of conversion.
 */
int pcm_s32_2_s16(int16_t *dst, const int32_t *src, uint16_t len);

/**
 * @brief Convert PCM16 to PCM32 format.
 * @param[in] channel - Audio channel.
 * @param[in] ps - PCM16 source data.
 * @param[in] samples - Number of samples.
 * @param[out] pcm32 - Converted PCM32 data.
 * @return none
 */
void pcm16_2_pcm32(uint8_t channel, int16_t *ps, uint16_t samples, int32_t *pcm32);

/**
 * @brief Initialize mailbox callback for low latency audio.
 * @param[in] none
 * @return none
 */
void ll_audio_mailbox_callback_init(void);

/**
 * @brief Enter audio mode for low latency audio.
 * @param[in] none
 * @return none
 */
void ll_audio_enter_audio_mode(void);

/**
 * @brief Exit audio mode for low latency audio.
 * @param[in] none
 * @return none
 */
void ll_audio_exit_audio_mode(void);

/**
 * @brief Get mode of TPH headset.
 * @param[in] none
 * @return Headset mode.
 */
uint8_t app_tph_headset_get_mode(void);

/**
 * @brief Check if phone mode is active.
 * @param[in] none
 * @return Phone mode status.
 */
int8_t ll_audio_is_phone_mode(void);

/**
 * @brief Get encoder buffer for TPSLL.
 * @param[in] none
 * @return Pointer to encoder buffer.
 */
uint8_t *tpsll_get_enc_buff(void);

/**
 * @brief Callback function for mailbox stimer start event.
 * @param[in] data - Event data.
 * @return none
 */
void tlk_mailbox_stimer_start_evt_callback(u8 *data);

/**
 * @brief Initialize encoder buffer for TPSLL.
 * @param[in] p_tpsll_audio_buff - Buffer for TPSLL audio.
 * @return none
 */
void tpsll_enc_buff_init(uint8_t *p_tpsll_audio_buff);

/**
 * @brief Get pointer to encoder buffer for TPSLL.
 * @param[in] none
 * @return Pointer to encoder buffer.
 */
uint8_t *tpsll_get_enc_buff_ptr(void);

/**
 * @brief Update read pointer of encoder buffer for TPSLL.
 * @param[in] none
 * @return none
 */
void tpsll_update_enc_buff_rptr(void);

/**
 * @brief Update write pointer of encoder buffer for TPSLL.
 * @param[in] none
 * @return none
 */
void tpsll_update_enc_buff_wptr(void);

/**
 * @brief Check availability of encoder buffer for TPSLL.
 * @param[in] none
 * @return Availability status.
 */
uint8_t tpsll_get_enc_buff_available(void);

/**
 * @brief Get encoder buffer by write pointer for TPSLL.
 * @param[in] wptr - Write pointer.
 * @return Pointer to encoder buffer.
 */
uint8_t *tpsll_get_enc_buff_by_wptr(uint16_t wptr);

/**
 * @brief Get encoder buffer by read pointer for TPSLL.
 * @param[in] rptr - Read pointer.
 * @return Pointer to encoder buffer.
 */
uint8_t *tpsll_get_enc_buff_by_rptr(uint8_t rptr);

/**
 * @brief Toggle GPIO for testing purposes.
 * @param[in] times - Number of toggles.
 * @return none
 */
void ll_aud_gpio_toggle_test(uint8_t times);

/**
 * @brief Set ultra-low latency flag.
 * @param[in] flag - Flag value.
 * @return none
 */
void ll_audio_set_ultra_low_latency_flag(uint8_t flag);

/**
 * @brief Get ultra-low latency flag.
 * @param[in] none
 * @return Ultra-low latency flag.
 */
uint8_t ll_audio_get_ultra_low_latency_flag(void);

/**
 * @brief Get MIC encoding timestamp.
 * @param[in] none
 * @return MIC encoding timestamp.
 */
uint32_t ll_audio_get_tus_mic_enc(void);

/**
 * @brief Get speaker decoding timestamp.
 * @param[in] none
 * @return Speaker decoding timestamp.
 */
uint32_t ll_audio_get_tus_spk_dec(void);

/**
 * @brief Get frame length for low latency audio.
 * @param[in] none
 * @return Frame length.
 */
uint32_t ll_audio_get_frame_length(void);

/**
 * @brief Get encoder frame length.
 * @param[in] none
 * @return Encoder frame length.
 */
uint16_t ll_audio_get_enc_frame_length(void);

/**
 * @brief Get headset encoder frame length.
 * @param[in] none
 * @return Encoder frame length.
 */
uint16_t ll_audio_get_headset_enc_frame_length(void);
/**
 * @brief Get samples length for low latency audio.
 * @param[in] none
 * @return Samples length.
 */
uint16_t ll_audio_get_samples_length(void);

#endif
#endif
