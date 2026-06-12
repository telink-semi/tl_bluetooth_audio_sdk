/********************************************************************************************************
 * @file    bt_audio_interface.h
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
#ifndef __AUDIO_BT_INTERFACE_H__
#define __AUDIO_BT_INTERFACE_H__

#include "tl_common.h"
#include "tlkapi/tlkapi.h"

#if SCO_ENC_QUEUE_ENABLE

#define SCO_ENC_BUFF_FRAME_SIZE 64
#define SCO_ENC_BUFF_NUM        3

#define SCO_ENC_QUEUE_ID_0      0
#define SCO_ENC_QUEUE_ID_1      1

#define SCO_ENC_QUEUE_ID_HF     SCO_ENC_QUEUE_ID_0
#define SCO_ENC_QUEUE_ID_AG     SCO_ENC_QUEUE_ID_1

typedef struct sco_enc_buff_context
{
    uint8_t *p_sco_enc_buff;
    uint16_t sco_enc_buff_frame_size;
    uint16_t sco_enc_buff_num;
    uint16_t sco_enc_buff_wptr;
    uint16_t sco_enc_buff_rptr;
} sco_enc_buff_context_t;

extern sco_enc_buff_context_t g_sco_enc_buf_ctx[SCO_ENC_QUEUE_NUM];
extern uint8_t                g_sco_data_buff[SCO_ENC_QUEUE_NUM][SCO_ENC_BUFF_FRAME_SIZE * SCO_ENC_BUFF_NUM];
#endif

/**
 * @brief  whether poweroff is ongoing
 *
 * @param[in]  none
 *
 * @returns 0-no 1-yes
 */
uint8_t btif_power_off_is_ongoing(void);

/**
 * @brief  whether has voice
 *
 * @param[in]  none
 *
 * @returns 0-not 1-has voice
 */
uint8_t btif_bt_has_voice(void);

/**
 * @brief  whether has sco
 *
 * @param[in]  none
 *
 * @returns 0-not 1-has sco
 */
uint8_t btif_bt_has_sco(void);

/**
 * @brief  whether has music
 *
 * @param[in]  none
 *
 * @returns 0-not 1-has music
 */
uint8_t btif_bt_has_music(void);

/**
 * @brief  whether poweroff tick is set
 *
 * @param[in]  none
 *
 * @returns 0-no 1-yes
 */
uint8_t btif_poweroff_tick_is_set(void);

/**
 * @brief  whether mic is muted
 *
 * @param[in]  none
 *
 * @returns 0-unmute 1-mute
 */
uint8_t btif_mic_is_muted(void);

/**
 * @brief  get mute flag
 *
 * @param[in]  none
 *
 * @returns 0-unmute 1-mute
 */
uint8_t btif_get_mic_mute_flag(void);

/**
 * @brief  receive a2dp data, called by A2DP profile
 * @param[in] aclHandle - ACL connection handle
 * @param[in] p         - A2DP data ptr
 * @param[in] len       - A2DP packet data len
 * @returns none
 */
void btif_receive_a2dp_frame(uint16_t aclHandle, uint8_t *p, uint16_t len);

/**
 * @brief  get sco data from stack and push to encode buffer
 * @param[in] id          - frame id
 * @param[in] sco_handle  - sco connection handle
 * @param[in] p           - sco data start address
 * @param[in] len         - sco data length
 * @returns none
 */
void btif_downlink_receive_sco_frame(uint8_t id, uint16_t sco_handle, uint8_t *p, uint16_t len);

/**
 * @brief  get sco data from encode buffer and send to stack
 * @param[in] id          - frame id
 * @param[in] sco_handle  - sco connection handle
 * @param[in] p           - sco data start address
 * @param[in] len         - sco data length
 * @returns none
 */
void btif_uplink_get_sco_frame(uint8_t id, uint16_t sco_handle, uint8_t *p, uint16_t len);

/**
 * @brief  send enhanced power control to peer
 * @param[in]  mode: POWER_INC or POWER_DEC
 * @returns none
 */
void btif_audio_power_control(uint8_t mode);

/**
 * @brief  set hfp codec
 * @param[in]  queue_id - queue id
 * @param[in]  hfp_codec: HFP_CODEC_ID_CVSD HFP_CODEC_ID_MSBC
 * @returns none
 */
void btif_set_hfp_codec(uint8_t queue_id, uint8_t hfp_codec);

/**
 * @brief  get hfp codec
 * @param[in]  queue_id - queue id
 * @returns HFP_CODEC_ID_CVSD HFP_CODEC_ID_MSBC
 */
uint8_t btif_get_hfp_codec(uint8_t queue_id);

/**
 * @brief  register sco data callback
 * @param[in]  none
 * @returns 0-success, others-failure
 */
int btif_register_sco_data_callback(void);

/**
 * @brief  register a2dp data receive callback
 * @param[in]  none
 * @returns 0-success, others-failure
 */
int btif_register_a2dp_data_recv_callback(void);

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
/**
 * @brief  get a2dp sink sample rate
 * @param[in]  acl_handle - ACL connection handle
 * @returns sample rate
 */
uint32_t btif_a2dp_snk_get_sample_rate(uint16_t acl_handle);
#endif

/**
 * @brief  initialize mic encode buffer
 * @param[in]  none
 * @returns none
 */
void bt_voice_init_mic_enc_buff(void);

/**
 * @brief  get sco encode buffer available count
 * @param[in]  none
 * @returns available count
 */
uint8_t btif_get_sco_enc_buff_available(uint8_t queue_id);

/**
 * @brief  update sco encode buffer read pointer
 * @param[in] queue_id
 */
void btif_update_sco_enc_buff_rptr(uint8_t queue_id);

/**
 * @brief  get sco data from stack and push to encode buffer
 * @param[in] id          - frame id
 * @param[in] sco_handle  - sco connection handle
 * @param[in] p_data      - sco data start address
 * @param[in] len         - sco data length
 * @returns none
 */
void    btif_voice_recv_sco_frame(uint8_t id, uint16_t sco_handle, uint8_t *p_data, uint16_t len);
void    btif_voice_get_sco_frame(uint8_t id, uint16_t sco_handle, uint8_t *p, uint16_t len);
uint8_t btif_get_sco_queue_id(uint16_t sco_handle);

#endif
