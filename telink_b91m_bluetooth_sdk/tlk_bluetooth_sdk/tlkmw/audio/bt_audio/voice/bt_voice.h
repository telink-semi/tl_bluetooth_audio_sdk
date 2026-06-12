/********************************************************************************************************
 * @file    bt_voice.h
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
#ifndef __BT_VOICE_H__
#define __BT_VOICE_H__

#define SPK_ENC_BUFF_NUM              8
#define SPK_ENC_BUFF_NUM_FORWARD      4
#define SPK_ENC_BUFF_SIZE             64
#define SPK_PCM_BUFF_NUM              8
#define SPK_PCM_BUFF_SIZE             (120 * 2)

#define BT_VOICE_PCM_SAMPLES          120

#define MIC_ENC_BUFF_NUM              8
#define MIC_ENC_BUFF_SIZE             64
#define MIC_PCM_BUFF_NUM              8
#define MIC_PCM_BUFF_SIZE             (120 * 2)

#define BT_VOICE_FLAG_PACKET_LOSS     1

#define TLKALG_DSP_RET_NN_DATA_LEN    (320 * 2)
#define SPK_BUFF_TOTAL_SIZE           (SPK_ENC_BUFF_SIZE * SPK_ENC_BUFF_NUM)
#define MIC_BUFF_TOTAL_SIZE           (MIC_ENC_BUFF_SIZE * MIC_ENC_BUFF_NUM)

#define TLKMDI_BT_VOICE_ENC_BUFF_SIZE (SCO_ENC_QUEUE_NUM * SPK_BUFF_TOTAL_SIZE + SCO_ENC_QUEUE_NUM * MIC_BUFF_TOTAL_SIZE)

extern uint8_t *g_spk_enc_buff_ptr[SCO_ENC_QUEUE_NUM];
extern uint8_t *g_mic_enc_buff_ptr[SCO_ENC_QUEUE_NUM];
extern uint8_t  g_msbc_enc_id;

enum
{
    BT_VOICE_ENC_TYPE_MUTE = 0,
    BT_VOICE_ENC_TYPE_SBC  = 1,
    BT_VOICE_ENC_TYPE_AAC  = 2,
};

/** BT voices status */
enum
{
    BT_VOICE_ST_IDLE               = 0,
    BT_VOICE_ST_INIT               = 1,
    BT_VOICE_ST_MIC_READY          = 32,
    BT_VOICE_ST_CODEC_READY        = SPK_ENC_BUFF_NUM * 2, // 16
    BT_VOICE_FORWARD_ST_IDLE       = 0xF0,
    BT_VOICE_FORWARD_ST_1ST_SCO_RX = 0xF1,
};

enum
{
    BT_VOICE_PLAYBACK_MODE         = 0,
    BT_VOICE_FORWARD_MODE          = 1,
    BT_VOICE_FORWARD_WITH_ALG_MODE = 2,
};

typedef int (*bt_voice_dec_func_callback_t)(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
typedef void (*bt_voice_enc_func_callback_t)(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
typedef void (*bt_voice_plc_func_callback_t)(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
typedef int (*bt_voice_proc_init_func_t)(void);

typedef struct
{
    int16_t  sync_ppm;
    uint16_t sync_samples;
    uint16_t sync_tick_ref; /**< task tick */
    uint8_t  sync_enc_buf_num;
    uint8_t  sync_frac;
    uint8_t  sync_id;
    uint8_t  sync_play_countdown;
} tws_voice_sync_cfg_t;

typedef struct
{
    uint32_t sco_rx_tick;

    uint32_t status;
    uint8_t  sco_forward_state[SCO_ENC_QUEUE_NUM];

    uint8_t spk_enc_rptr[SCO_ENC_QUEUE_NUM];
    uint8_t spk_enc_wptr[SCO_ENC_QUEUE_NUM];
    uint8_t spk_pcm_rptr[SCO_ENC_QUEUE_NUM];
    uint8_t spk_pcm_wptr[SCO_ENC_QUEUE_NUM];

    uint8_t mic_enc_rptr[SCO_ENC_QUEUE_NUM];
    uint8_t mic_enc_wptr[SCO_ENC_QUEUE_NUM];
    uint8_t mic_pcm_rptr[SCO_ENC_QUEUE_NUM];
    uint8_t mic_pcm_wptr[SCO_ENC_QUEUE_NUM];

    uint32_t init;
    uint32_t tick_frame;
    uint16_t samples_ref;
    uint16_t samples_last;

    bt_voice_dec_func_callback_t dec_func;
    bt_voice_enc_func_callback_t enc_func;
#if (SCO_FORWARD_WITH_ALG)
    bt_voice_dec_func_callback_t dec_forward_func;
    bt_voice_enc_func_callback_t enc_forward_func;
#endif
    uint16_t samples_cur;
    uint16_t samples;

    uint32_t sync_tick_ref; /**< task tick */
    uint16_t sync_samples_peer;

    // sync info 10 bytes
    int16_t  sync_ppm;
    uint16_t sync_samples;
    uint16_t sync_tick;
    uint8_t  sync_enc_buf_num;
    uint8_t  sync_frac;
    uint8_t  sync_id;
    uint8_t  sync_play_countdown;

    uint8_t sync_init;
    uint8_t codec_sync_init;

    uint8_t sco_data_ready;
#if AUDIO_TWS_MODE
    volatile uint8_t voice_play_sync;
#endif

    uint8_t rcv_sync_info;

    uint8_t mic_state;

    uint8_t bt_voice_mode;
    uint8_t sco_proc_id;
} __attribute__((aligned(4))) bt_voice_cfg_t;

extern bt_voice_cfg_t bt_voice_cfg;

extern uint8_t  bt_voice_hfp_codec[SCO_ENC_QUEUE_NUM];
extern uint8_t *g_mic_enc_buff_ptr[SCO_ENC_QUEUE_NUM];
extern uint8_t  msbc_silence_pkt[60];

/** 
 * @brief   Get BT voice environment pointer
 * @return  Pointer to bt_voice_cfg_t structure
 */
bt_voice_cfg_t *get_bt_voice_env_ptr(void);

/**
 * @brief   Register BT voice decode and encode function callbacks
 * @param[in]  penc - encode function callback
 * @param[in]  pdec - decode function callback
 * @returns    None
 */
void bt_voice_register_codec_callback(void *penc, void *pdec);

/**
 * @brief   Initialize microphone encode buffer and CVSD in BT voice mode
 * @param[in]  None
 * @returns    Operation result - true: success, false: failure
 */
bool bt_voice_audio_path_init(void);

/**
 * @brief   Deinitialize BT voice audio path
 * @param[in]  None
 * @returns    Operation result - true: success, false: failure
 */
bool bt_voice_audio_path_deinit(void);

/**
 * @brief   Reset codec configuration and mute playback buffer
 * @param[in]  None
 * @return     None
 */
void bt_voice_reset(void);

/**
 * @brief   Get SCO data from stack and push to encode buffer
 * @param[in]  id     - frame ID
 * @param[in]  p_data - SCO data start address
 * @param[in]  len    - SCO data length
 * @return     None
 */
void bt_voice_receive_sco_frame(uint8_t id, uint8_t *p_data, uint16_t len, uint16_t sco_handle);

/**
 * @brief   Controller calls this function to post encoded PCM data to controller buffer
 * @param[in]  id     - frame ID
 * @param[in]  p_data - address to restore encoded PCM data
 * @param[in]  len    - data length
 * @param[in]  sco_handle - SCO connection handle
 * @return     None
 */
void bt_voice_mic_enc_get_frame(uint8_t id, uint8_t *p_data, uint16_t len, uint16_t sco_handle);

/**
 * @brief   Get voice PCM data, process with EQ and encode PCM data
 * @param[in]  None
 * @return     None
 */
void bt_voice_process_mic_data(void);

/**
 * @brief   Set BT voice status to IDLE
 * @param[in]  None
 * @returns    None
 */
void bt_voice_set_idle(void);

#if AUDIO_TWS_MODE
/**
 * @brief   Secondary device check sync parameters periodically
 * @param[in]  p_master - master tws_music_sync_cfg
 * @param[in]  p_slave  - listener tws_music_sync_cfg
 *              sync_ppm(2B)+sync_samples(2B)+sync_tick(2B)+sync_enc_buf_num(1B)+
 *              sync_frac(1B)+sync_id(1B)+sync_play(1B)
 * @return     None
 */
void bt_voice_check_sync_cfg(uint8_t *p_master, uint8_t *p_slave);

/**
 * @brief   Both sides update sync parameters
 * @param[in]  tick  - task tick
 * @param[in]  p_des - async_status
 *              sync_ppm(2B)+sync_samples(2B)+sync_tick(2B)+sync_enc_buf_num(1B)+
 *              sync_frac(1B)+sync_id(1B)+sync_play(1B)
 * @return     None
 */
void bt_voice_update_sync_cfg(uint32_t tick, uint8_t *p_des);

/**
 * @brief   Initialize BT voice sync
 * @param[in]  None
 * @return     None
 */
void bt_voice_sync_init(void);

#endif

/**
 * @brief   Check if frame head is MSBC
 * @param[in]  p - SCO data
 * @returns    TRUE or FALSE
 */
bool msbc_is_frame_head(uint8_t *p);

/**
 * @brief   Check if MSBC data is all zero packet
 * @param[in]  p - SCO data
 * @returns    TRUE or FALSE
 */
bool msbc_is_all_zero_pkt(uint8_t *p);

/**
 * @brief   Get BT voice buffer
 * @param[in]  None
 * @returns    Buffer pointer
 */
uint8_t *bt_voice_get_buffer(void);

/**
 * @brief   Get playback data for voice
 * @param[out] p_des0 - left channel PCM data
 * @param[out] p_des1 - right channel PCM data
 * @returns    Data length
 */
uint16_t bt_voice_get_playback_data(int16_t *p_des0, int16_t *p_des1);

/**
 * @brief   Open codec for BT voice
 * @param[in]  None
 * @returns    None
 */
void bt_voice_open_codec(void);

/**
 * @brief   Close codec for BT voice
 * @param[in]  None
 * @returns    None
 */
void bt_voice_close_codec(void);

/**
 * @brief   Close ANC codec for BT voice
 * @param[in]  None
 * @returns    None
 */
void bt_voice_anc_close_codec(void);

/**
 * @brief   Set BT voice encode buffer
 * @param[in]  p_buff - buffer address
 * @returns    Buffer length
 */
uint16_t bt_voice_set_buffer(uint8_t *p_buff);

/**
 * @brief   DSP message process callback function
 * @param[in]  enc_buff_wptr - encode buffer write pointer
 * @param[in]  type          - message type
 * @returns    None
 */
void bt_voice_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type);

/**
 * @brief   Clear DSP return buffer status
 * @param[in]  None
 * @returns    None
 */
void bt_voice_clear_dsp_ret_buff_status(void);

/**
 * @brief   Switch in handler for BT voice
 * @param[in]  handle - connection handle
 * @returns    None
 */
void tlkmdi_btvoice_switch_in(uint16_t handle);

/**
 * @brief   Switch out handler for BT voice
 * @param[in]  handle - connection handle
 * @returns    None
 */
void tlkmdi_btvoice_switch_out(uint16_t handle);

void bt_voice_set_mode(uint8_t mode);

#if TLKMW_INTERPHONE_EN
/**
 * @brief   Set interphone master mode
 * @param[in]  None
 * @returns    None
 */
void bt_voice_set_interphone_master(void);

/**
 * @brief   Clear interphone master mode
 * @param[in]  None
 * @returns    None
 */
void bt_voice_clear_interphone_master(void);

/**
 * @brief   Check if master is in interphone mode
 * @param[in]  None
 * @returns    TRUE if master is in interphone mode, FALSE otherwise
 */
bool bt_voice_master_is_interphone(void);
#endif
#endif
