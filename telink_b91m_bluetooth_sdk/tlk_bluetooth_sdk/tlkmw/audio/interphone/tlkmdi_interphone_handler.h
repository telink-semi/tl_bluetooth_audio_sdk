/********************************************************************************************************
 * @file    tlkmdi_interphone_handler.h
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
#ifndef TLKMDI_INTERPHONE_HANDLER_H_
#define TLKMDI_INTERPHONE_HANDLER_H_

#if TLKMW_INTERPHONE_EN

#define MIC_FRAME_SIZE       (16 * 20)
#define SPK_FRAME_SIZE       (48 * 20)
#define INTERPHONE_I2S_SIZES (MIC_FRAME_SIZE * 6)
#define SPK_SAMPLERATE_16K   0
#define PLAY_STEREO_DATA     0

extern uint8_t g_tlkmdi_interphone_i2s_rx_dma;
extern uint8_t g_tlkmdi_interphone_i2s_tx_dma;
#define INTERPHONE_I2S_RX_DMA g_tlkmdi_interphone_i2s_rx_dma
#define INTERPHONE_I2S_TX_DMA g_tlkmdi_interphone_i2s_tx_dma

typedef enum
{
    MCU_WAIT_RESP = BIT(0),
    MCU_RECV      = BIT(1),
} interphone_mcu_dsp_state_e;

typedef struct
{
    uint16_t                   down_buff_wptr;
    uint16_t                   down_buff_rptr;
    uint16_t                   up_buff_wptr;
    uint16_t                   up_buff_rptr;
    uint16_t                   down_buff_len;
    uint16_t                   up_buff_len;
    uint32_t                   spk_samplerate;
    uint32_t                   mic_samplerate;
    uint32_t                   last_times_tick;
    bool                       queue_one_frame_data;
    bool                       dis_dsp_request;
    uint8_t                    send_seq;
    uint8_t                    recv_seq;
    interphone_mcu_dsp_state_e data_state;
    // bool     bis_sync_flag;
    // bool     timer_big_alig_flag;
} tlkmdi_interphone_ctl;

typedef enum
{
    INTERPHONE_MODE_MESH  = BIT(0),
    INTERPHONE_MODE_MUSIC = BIT(1),
    INTERPHONE_MODE_AG    = BIT(2),
} interphone_mode_e;

extern uint16_t one_frame_times;

/**
 * @brief       Mix interphone data
 * @param[in]   mic_data - Microphone data buffer
 * @return      None
 */
void tlkmdi_interphone_data_mixing(uint8_t *mic_data);

/**
 * @brief       Initialize interphone algorithm temporarily
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_alg_init_temp(void);

/**
 * @brief       Mute I2S TX buffer
 * @param       None
 * @return      None
 */
void tlkmdi_mute_i2s_tx_buff(void);

/**
 * @brief       Mute I2S RX buffer
 * @param       None
 * @return      None
 */
void tlkmdi_mute_i2s_rx_buff(void);

/**
 * @brief       Fill I2S TX buffer
 * @param[in]   data - Data to fill
 * @param[in]   len - Length of data
 * @return      None
 */
void tlkmdi_interphone_fill_i2stx_buff(adc_int *data, uint16_t len);

/**
 * @brief       Set loop tick
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_set_looptick(void);

/**
 * @brief       Interphone main loop
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_mainloop(void);

/**
 * @brief       Handle interphone timer interrupt
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_timer_irq(void);

/**
 * @brief       Clear DSP status
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_clear_dsp_status(void);

/**
 * @brief       Disable DSP response
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_dis_dsp_response(void);

/**
 * @brief       Initialize BT music algorithm for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_btmusic_alg_init(void);

/**
 * @brief       Deinitialize BT music algorithm for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_btmusic_alg_deinit(void);

/**
 * @brief       Initialize voice algorithm for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_voice_alg_init(void);

/**
 * @brief       Deinitialize voice algorithm for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_voice_alg_deinit(void);

/**
 * @brief       Deinitialize interphone algorithm
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_alg_deinit(void);

/**
 * @brief       Open codec for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_open_codec(void);

/**
 * @brief       Open microphone for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_open_mic(void);

/**
 * @brief       Open speaker for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_open_spk(void);

/**
 * @brief       Initialize I2S for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_i2s_init(void);

/**
 * @brief       Synchronize I2S RX sample
 * @param[in]   sample - Sample count
 * @return      None
 */
void tlkmdi_interphone_sync_is2rx_sample(uint16_t sample);

/**
 * @brief       Synchronize I2S TX sample
 * @param[in]   sample - Sample count
 * @return      None
 */
void tlkmdi_interphone_sync_is2tx_sample(uint16_t sample);

/**
 * @brief       Initialize FIFO interrupt for interphone
 * @param[in]   byte_num - Byte number for interrupt trigger
 * @return      None
 */
void tlkmdi_interphone_fifo_irq_init(uint16_t byte_num);

/**
 * @brief       Transfer I2S data
 * @param[in]   txdata - TX data buffer
 * @param[in]   rxdata - RX data buffer
 * @return      Operation result
 */
int tlkmdi_interphone_i2s_data_trans(uint8_t *txdata, uint8_t *rxdata);

/**
 * @brief       Handle FIFO interrupt for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_fifo_irq_handler_func(void);

/**
 * @brief       Enable BT music for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_btmusic_enable(void);

/**
 * @brief       Enable voice AG for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_voice_ag_enable(void);

/**
 * @brief       Set interphone mode
 * @param[in]   mode - Mode to set
 * @return      None
 */
void tlkmdi_interphone_set_mode(interphone_mode_e mode);

/**
 * @brief       Clear interphone mode
 * @param[in]   mode - Mode to clear
 * @return      None
 */
void tlkmdi_interphone_clear_mode(interphone_mode_e mode);

/**
 * @brief       Get interphone mode
 * @param       None
 * @return      Current interphone mode
 */
interphone_mode_e tlkmdi_interphone_get_mode(void);

/**
 * @brief       Enable voice for interphone
 * @param[in]   mode_flag - Mode flag
 * @return      None
 */
void tlkmdi_interphone_voice_enable(interphone_mode_e mode_flag);

/**
 * @brief       Disable voice for interphone
 * @param[in]   mode_flag - Mode flag
 * @return      None
 */
void tlkmdi_interphone_voice_disable(interphone_mode_e mode_flag);

/**
 * @brief       Enable speaker for interphone
 * @param[in]   mode_flag - Mode flag
 * @return      None
 */
void tlkmdi_interphone_spk_enable(interphone_mode_e mode_flag);

/**
 * @brief       Disable speaker mode for interphone
 * @param[in]   mode_flag - Mode flag
 * @return      None
 */
void tlkmdi_interphone_spk_disable_mode(interphone_mode_e mode_flag);

/**
 * @brief       Disable speaker for interphone
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_spk_disable(void);

/**
 * @brief       Initialize interphone buffers
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_buff_init(void);

/**
 * @brief       Mute BT down buffer
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_mute_BtDownBuff(void);

/**
 * @brief       Mute BT up buffer
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_mute_BtUpBuff(void);

/**
 * @brief       Get BT down buffer write pointer
 * @param       None
 * @return      Write pointer
 */
uint16_t tlkmdi_interphone_get_BtDownBuff_wptr(void);

/**
 * @brief       Get BT down buffer read pointer
 * @param       None
 * @return      Read pointer
 */
uint16_t tlkmdi_interphone_get_BtDownBuff_rptr(void);

/**
 * @brief       Get idle length of BT down buffer
 * @param       None
 * @return      Idle length
 */
uint16_t tlkmdi_interphone_get_BtDownBuff_IdleLen(void);

/**
 * @brief       Get data length of BT down buffer
 * @param       None
 * @return      Data length
 */
uint16_t tlkmdi_interphone_get_BtDownBuff_DataLen(void);

/**
 * @brief       Get data from BT down buffer
 * @param[out]  data - Data buffer
 * @param[in]   len - Length of data to get
 * @return      true - Success, false - Failure
 */
bool tlkmdi_interphone_getData_BtDownBuff(uint8_t *data, uint16_t len);

/**
 * @brief       Fill BT down buffer with data
 * @param[in]   pData - Data to fill
 * @param[in]   dataLen - Length of data
 * @return      true - Success, false - Failure
 */
bool tlkmdi_interphone_fill_BtDownBuff(uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Get idle length of BT up buffer
 * @param       None
 * @return      Idle length
 */
uint16_t tlkmdi_interphone_get_BtUpBuff_IdleLen(void);

/**
 * @brief       Get data length of BT up buffer
 * @param       None
 * @return      Data length
 */
uint16_t tlkmdi_interphone_get_BtUpBuff_DataLen(void);

/**
 * @brief       Get data from BT up buffer
 * @param[out]  data - Data buffer
 * @param[in]   len - Length of data to get
 * @return      true - Success, false - Failure
 */
bool tlkmdi_interphone_getData_BtUpBuff(uint8_t *data, uint16_t len);

/**
 * @brief       Fill BT up buffer with data
 * @param[in]   pData - Data to fill
 * @param[in]   dataLen - Length of data
 * @return      true - Success, false - Failure
 */
bool tlkmdi_interphone_fill_BtUpBuff(uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Synchronize BT down buffer
 * @param[in]   sample - Sample count
 * @return      None
 */
void tlkmdi_interphone_sync_BtDownBuff(uint16_t sample);

/**
 * @brief       Synchronize BT up buffer
 * @param[in]   sample - Sample count
 * @return      None
 */
void tlkmdi_interphone_sync_BtUpBuff(uint16_t sample);

#if TLK_MW_DSP_COMM_ENABLE
/**
 * @brief       Callback function for processing DSP messages
 * @param[in]   enc_buff_wptr - Encode buffer write pointer
 * @param[in]   type - Message type
 * @return      None
 */
void tlkmdi_interphone_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type);
#endif
#endif //TLKMW_INTERPHONE_EN

#endif
