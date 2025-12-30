/********************************************************************************************************
 * @file    bt_music.h
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
#ifndef __BT_MUSIC_H__
#define __BT_MUSIC_H__

#include "tlkmw/audio/audio_mw_manager.h"
#if TLKALG_EQ_ENABLE
#include "tlkalg/audio/audio_alg_interface.h"
#include "tlkalg/audio/eq/tlkalg_eq_interface.h"
#endif

#if 1 //(TLKBTP_CFG_A2DPSNK_ENABLE)
    #ifndef bt_audio_debug_ram_code
        #define bt_audio_debug_ram_code
    #endif

    #define SBC_MAX_FRAMESIZE 119

    #ifndef BT_MUSIC_ENC_SAMPLES
#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
        #define BT_MUSIC_ENC_SAMPLES (1024 * 45)
#else
        #define BT_MUSIC_ENC_SAMPLES (1024 * 12)
#endif
    #endif

    #define BT_MUSIC_AAC_ENC_BUF_SIZE (1024)
    #define BT_MUSIC_ENC_FIFO_SIZE    (BT_MUSIC_ENC_SAMPLES)

    #ifndef BT_MUSIC_SBC_BUFF_NUM_MAX
        #define BT_MUSIC_SBC_BUFF_NUM_MAX 360
    #endif

    #ifndef BT_MUSIC_AAC_BUFF_NUM_MAX
        #define BT_MUSIC_AAC_BUFF_NUM_MAX 20
        #define BT_MUSIC_AAC_BUFF_SIZE    670
    #endif

    #ifndef TMUSIC_AAC_BUFF_NUM_MAX
        #define TMUSIC_AAC_BUFF_NUM_MAX 18
    #endif

    #define AAC_PACKET_FLAG 0x47

#define CLK_MODE_96M      1
#define CLK_MODE_132M     2

#define BT_LATENCY_NORMAL    0
#define BT_LATENCY_PKTLOSS   1

/** music rate calibrate state */
#define FLG_WRITE_UPDATE 1
#define FLG_READ_UPDATE  2
#define FLG_RATE_RESET   4
#define PPM_MAX          400

/** BT MUSIC LowLatency buffer size. */
extern uint16_t         g_bt_music_ll_enc_buff_size;
extern volatile uint8_t g_bt_music_enc_mode_is_changing;
extern uint32_t         g_aac_frame_copy;

enum
{
    FLG_DUMP_SYNC_CHECK      = 1,
    FLG_DUMP_BUFF_STATUS     = 2,
    FLG_DUMP_PLAYBACK_STATUS = 4,
};

enum
{
    BT_MUSIC_SYNC_PLAY_IDLE  = 0,
    BT_MUSIC_SYNC_PLAY_INIT  = 1,
    BT_MUSIC_SYNC_PLAY_START = 2,
};

typedef int (*bt_music_dec_func_callback_t)(uint8_t *ps, int16_t *pd, uint16_t len);

typedef uint16_t (*bt_music_crc_func_callback_t)(uint16_t crc, uint8_t *pStart, uint16_t uSize);

typedef struct
{
    int16_t  sync_ppm;
    uint16_t sync_samples;     /**< decoded samples total num. */

    uint16_t sync_tick;
    uint16_t sync_enc_buf_num; /**< encoded frames total num. */
    int8_t   sync_frac;

    uint8_t sync_id;
    uint8_t sync_play;

    uint16_t sync_info1;
    uint16_t sync_info2;
    uint8_t  sync_info3;
} tws_music_sync_cfg_t;

typedef struct
{
    uint8_t  enc_buf_id;        /**< id(avdtp frame number) of most updated frame */
    uint16_t  enc_buf_rptr;      /**< encoded buf read ptr. */
    uint16_t  enc_buf_wptr;      /**< encoded buf write ptr. */
    uint16_t  enc_buf_num;       /**< encoded buf num. SBC: 86, AAC: 18 */
    uint16_t enc_buf_size;      /**< SBC: sbc_framesize or AAC: 1200 */
    uint16_t samples_per_frame; /**< Samples included in a frame. SBC: 128, AAC: 1024 */

    uint32_t enc_para;          /**< AAC codec param. */
    uint32_t sample_rate;


    uint32_t sync_tick_ref;    /**< task tick */

    int16_t  sync_ppm;
    uint16_t sync_samples;     /**< decoded samples total num. */
    uint16_t sync_tick;        /**< remainder ticks that is not enough to play a single sample */
    volatile uint16_t sync_enc_buf_num; /**< recv encoded frames total num. */
    int8_t   sync_frac;
    uint8_t sync_id;           /**< enc_buf_id */
    uint8_t sync_play_countdown;
    uint16_t sync_info1;
    uint16_t sync_info2;
    uint8_t sync_info3;

    uint8_t sync_play;         /**< 0x0 - 0x80 */
    uint8_t sync_countdown;

    bt_music_dec_func_callback_t dec_func;
    uint32_t                     tick_1s;
    uint32_t                     tick_last;
    uint32_t                     seconds;

    uint32_t seconds_last;
    int32_t  sample_avg;
    int32_t  sample_avg_last;
    int32_t  sample_avg_ref;

    uint16_t dump_mask;
    uint16_t buff_low;
    uint16_t buff_high;
    int16_t  rate_change;
    int16_t  ppm_debug;
    int16_t  sync_ppm_cur;
    uint8_t  sync_ppm_set;
    uint8_t  sync_frac_peer;
    uint16_t  enc_buf_mute_num;
    uint16_t  enc_buf_wptr_last;

    uint16_t aac_len[TMUSIC_AAC_BUFF_NUM_MAX];
    uint16_t aac_offset[TMUSIC_AAC_BUFF_NUM_MAX];

    uint8_t  src_low_latency;
    uint8_t  reset;
    uint16_t aac_offset_cur;

    uint8_t  sbc_framesize;
    uint8_t  sbc_channel_mode;
    uint8_t  skip_frames;
    uint16_t skip_samples;

    uint16_t sync_buf_num_peer;
    uint8_t sync_ppm_set_pending;
    uint16_t resv2byte;

    uint32_t tick_skip;

    uint32_t sync_stimer_tick;
    uint32_t sync_stimer_tick_last;

    uint8_t  rcv_sync_info;

    uint8_t  inc_peer_tx_power;
    uint8_t  inc_peer_tx_power_available;
    uint8_t  inc_peer_tx_power_is_pending;

    bt_music_crc_func_callback_t crc_func;
    uint32_t                     crc_mute;
    uint32_t                     tick_active;

    uint8_t latency_mode;
} bt_music_cfg_t;

extern bt_music_cfg_t bt_music_cfg;

extern uint8_t         *g_bt_music_enc_buf_ptr;
extern uint16_t         g_bt_music_buffer_size;
extern volatile uint8_t g_bt_music_enc_mode;

/**
 * @brief       Set ll encode buffer size
 * @param[in]   size_bytes - buffer size in bytes
 * @return      None
 */
void bt_music_ll_enc_buff_size_set(uint16_t size_bytes);

/**
 * @brief       Get ll encode buffer size
 * @param[in]   None
 * @return      ll encode buffer size
 */
uint16_t bt_music_ll_enc_buff_size_get(void);

/**
 * @brief       Push frames to encoded buffer
 * @param[in]   id  - A2DP packet serial number
 * @param[in]   p   - Data pointer
 * @param[in]   len - Data length
 * @return      BT_AUDIO_SUCCESS is successful, others are failed.
 */
uint8_t bt_music_push_frames_to_encoded_buffer(uint8_t id, uint8_t *p, uint16_t len);

/**
 * @brief   get encode data start address
 * @param[in]   none
 * @returns encode data start address
 */
uint8_t *bt_music_enc_get_frame(void);

/**
 * @brief   Get encoded frames, push data from enc buffer to play buffer
 *          Get ppm value and init ppm algorithm
 * @param[in]   num      - frame num
 * @param[in]   samples  - one frame include samples, A2DP: sbc->128, aac->1024
 *
 * @return ppm_set
 */
uint8_t bt_music_get_encoded_frame(uint8_t num, uint16_t samples);

/**
 * @brief   music play init
 * @param[in]   none
 * @returns none
 */
void bt_music_play_init(void);

/*
 * @brief  receive a2dp data, post encoded data to queue
 * @param[in] p_data   - A2DP data ptr
 * @param[in] len - A2DP packet data len
 * @returns BT_AUDIO_SUCCESS is successful, others are failed.
 */
uint8_t bt_music_receive_a2dp_frames(uint8_t *p_data, uint16_t len);

/**
 * @brief       CRC check for music encode frames
 * @param[in]   aclHandle - ACL connection handle
 * @param[in]   p         - Data pointer
 * @param[in]   len       - Data length
 * @return      BT_AUDIO_SUCCESS
 */
uint8_t bt_music_enc_crc_frames(uint16_t aclHandle, uint8_t *p, uint16_t len);

/**
 * @brief set music play status
 * @param[in]   status - IDLE INIT START
 * @returns none
 */
void bt_music_set_play_status(uint8_t status);

/**
 * @brief   set music play sample rate
 * @param[in]   sample_rate
 * @returns none
 */
void bt_music_open_codec(uint16_t sample_rate);

/**
 * @brief       Close codec for music
 * @param[in]   None
 * @return      None
 */
void bt_music_close_codec(void);

/**
 * @brief       Initialize audio path for music
 * @param[in]   None
 * @return      None
 */
void bt_music_audio_path_init(void);

/**
 * @brief       Deinitialize audio path for music
 * @param[in]   None
 * @return      None
 */
void bt_music_audio_path_deinit(void);

/**
 * @brief   bt_music_sync_task
 * @param[in]   none
 * @returns none
 */
void bt_music_sync_task(void);

/**
 * @brief   get music play data
 * @param[in]   p_des0 - address 0 to restore data
 * @param[in]   p_des1 - address 1 to restore data
 * @return  data length
 */
uint16_t bt_music_get_playback_data(int16_t *p_des0, int16_t *p_des1);

/**
 * @brief register decode function
 * @param[in]   p - decode function
 * @returns none
 */
void bt_music_register_dec_callback(void *p);

/**
 * @brief   register decode function according to left-right ear and decode algorithm
 * @param[in]   none
 * @returns none
 */
void bt_music_choice_channel(void);

/**
 * @brief set dump mask
 * @param[in]   mask - mask value
 * @returns none
 */
void bt_music_set_dump_mask(uint16_t mask);

/**
 * @brief set ppm value when debugging ppm function
 * @param[in]   offset - ppm value
 * @returns none
 */
void bt_music_ppm_debug(int16_t offset);

/**
 * @brief set low latency mode
 * @param[in]   enable - 0, off; 1, on
 * @returns none
 */
void bt_music_set_low_latency_mode(uint8_t enable);

/**
 * @brief       Check if low latency mode is enabled
 * @param[in]   None
 * @return      1 if low latency mode is enabled, 0 otherwise
 */
uint8_t bt_music_is_low_latency_mode(void);

#if AUDIO_TWS_MODE

/**
 * @brief       Initialize music synchronization
 * @param[in]   None
 * @return      None
 */
void bt_music_sync_init(void);

/**
 * @brief Used to synchronize samples when starting playback, both sides will call
 * @param[in]   none
 * @return none.
 */
void bt_music_sync_samples(void);

/**
 * @brief       Both sides update sync config every 64-slot (40ms)
 * @param[in]   tick_ref  - Reference tick for synchronization
 * @param[in]   p_des     - Pointer to buffer where sync configuration will be stored
 *                        The data format is: sync_ppm(2B) + sync_samples(2B) + sync_tick(2B) +
 *                        sync_enc_buf_num(1B) + sync_frac(1B) + sync_id(1B) + sync_play_countdown(1B)
 * @param[in]   fno       - Frame number (not used in function, included for potential future use)
 * @return      None
 */
void bt_music_update_sync_cfg(uint32_t tick_ref, uint8_t *p_des, uint16_t fno);

/**
 * @brief   Secondary device check sync parameters periodically
 * @param[in]   p_master  - master tws_music_sync_cfg
 * @param[in]   p_slave   - listener tws_music_sync_cfg
 *              sync_ppm(2B)+sync_samples(2B)+sync_tick(2B)+sync_enc_buf_num(1B)+
 *              sync_frac(1B)+sync_id(1B)+sync_play(1B)
 * @return none
 */
void bt_music_check_sync_cfg(uint8_t *p_master, uint8_t *p_slave);
#endif

/**
 * @brief  Init AAC decoder params and set sample rate.
 * @param[in] param - AAC params.
 * @param[in] p - a2dp parameter address.
 * @param[in] len - parameter length.
 * @returns sample_rate
 */
int bt_music_init_aac_decoder_params(uint32_t param, uint8_t *p, uint16_t len);

/**
 * @brief  Init SBC decoder params and set sample rate
 * @param[in] param - AAC params
 * @param[in] p - a2dp data
 * @param[in] len - a2dp data length
 * @returns sample_rate
 */
uint16_t bt_music_init_sbc_decoder_params(uint32_t param, uint8_t *p, uint16_t len);

/**
 * @brief   set sample rate and register decode function according to left-right
 *          ear and decode algorithm
 * @param[in]   sample_rate - sample rate
 * @returns 0
 */
//_attribute_no_inline_ uint8_t bt_music_init_sample_rate(uint16_t sample_rate);

/**
 * @brief   check crc calculate
 * @param[in]   crc - crc value
 * @param[in]   crc_len
 * @returns BT_AUDIO_SUCCESS
 */
uint8_t bt_music_enc_crc_check(uint32_t crc, uint32_t crc_len);

/**
 * @brief set enc buffer parameter.
 * @param[in]   num_of_buff - enc buff number SBC-86 AAC-18
 * @param[in]   size - buff size	SBC-sbc_framesize AAC-1200
 * @param[in]   samples - sample number in one frame SBC-128 AAC-1024
 * @returns none
 */
void bt_music_set_param(uint16_t num_of_buff, uint16_t size, uint16_t samples_per_frame);

/**
 * @brief       Set encode buffer
 * @param[in]   p_buff - buffer pointer
 * @return      BT_MUSIC_ENC_FIFO_SIZE
 */
uint16_t bt_music_set_enc_buffer(uint8_t *p_buff);

/**
 * @brief       Get encode buffer
 * @param[in]   None
 * @return      encode buffer pointer
 */
uint8_t *bt_music_get_enc_buffer(void);

/**
 * @brief       Get the number of available encoded buffers
 * @param[in]   None
 * @return      Number of available encoded buffers
 */
int16_t bt_music_get_num_of_enc_buff_avail(void);

/**
 * @brief       Set PPM debug value
 * @param[in]   ppm_debug - PPM debug value
 * @return      None
 */
void bt_music_set_ppm_debug(int16_t ppm_debug);

/**
 * @brief       Calibrate music rate
 * @param[in]   None
 * @return      Number of encoded buffers
 */
uint8_t bt_music_calibrate_rate(void);

/**
 * @brief       Enable or disable music calibration
 * @param[in]   enable - 1 to enable, 0 to disable
 * @return      None
 */
void bt_music_calibrate_enable(uint8_t enable);

/**
 * @brief       Music loop processing
 * @param[in]   None
 * @return      None
 */
void bt_music_loop(void);

#ifdef BT_MUSIC_SUPPORT_DYNAMIC_LATENCY
/**
 * @brief       Detect latency mode
 * @param[in]   enc_buf_num - Number of encoded buffers
 * @return      None
 */
void bt_music_latency_mode_detect(uint16_t enc_buf_num);

/**
 * @brief       Set latency mode
 * @param[in]   mode - Latency mode
 * @return      None
 */
void bt_music_set_latency_mode(uint8_t mode);

/**
 * @brief       Process dynamic latency
 * @param[in]   None
 * @return      None
 */
void bt_music_dynamic_latency_process(void);
#endif

/**
 * @brief       Switch into Bluetooth music mode
 * @param[in]   handle - Connection handle
 * @return      None
 */
void tlkmdi_btmusic_switch_in(uint16_t handle);

/**
 * @brief       Switch out of Bluetooth music mode
 * @param[in]   handle - Connection handle
 * @return      None
 */
void tlkmdi_btmusic_switch_out(uint16_t handle);

#if TLKALG_EQ_ENABLE
/**
 * @brief       Initialize Bluetooth music EQ algorithm
 * @param[in]   samplerate - Sample rate
 * @param[in]   chnl - Channel index
 * @param[in]   eq_type - EQ type
 * @return      None
 */
void bt_music_alg_eq_init(uint32_t samplerate, ALG_CHANNEL_IDX chnl, e_eq_type_e eq_type);
#endif
#if TLKALG_ASRC_441TO48_16BIT_TEMP_ENABLE
/**
 * @brief       Initialize 44.1kHz to 48kHz conversion
 * @param[in]   None
 * @return      None
 */
void bt_music_441to48_init(void);
/**
 * @brief       Deinitialize 44.1kHz to 48kHz conversion
 * @param[in]   None
 * @return      None
 */
void bt_music_441to48_deinit(void);
#endif
/**
 * @brief       Initialize Bluetooth music buffer and algorithms
 * @param[in]   None
 * @return      true if successful, false otherwise
 */
bool bt_music_buff_and_alg_init(void);
/**
 * @brief       Deinitialize Bluetooth music buffer and algorithms
 * @param[in]   None
 * @return      true if successful, false otherwise
 */
bool bt_music_buff_and_alg_deinit(void);
/**
 * @brief       Set Bluetooth music codec type
 * @param[in]   type - Codec type
 * @return      None
 */
void bt_music_set_codec_type(uint8_t type);
#if AUDIO_TWS_MODE
/**
 * @brief       Mailbox audio synchronization callback
 * @param[in]   p_data - Data pointer
 * @param[in]   len - Data length
 * @return      None
 */
void tlkmdi_mailbox_audio_sync_callback(void *p_data, uint16_t len);
/**
 * @brief       Mailbox TWS slave audio synchronization callback
 * @param[in]   p_data - Data pointer
 * @param[in]   len - Data length
 * @return      None
 */
void tlkmdi_mailbox_tws_slave_audio_sync_callback(void *p_data, uint16_t len);
/**
 * @brief       Mailbox TWS slave synchronization tick processing callback
 * @param[in]   p_data - Data pointer
 * @param[in]   len - Data length
 * @return      None
 */
void tlkmdi_mailbox_tws_slave_sync_tick_proc_callback(void *p_data, uint16_t len);
#endif
#endif
#endif
