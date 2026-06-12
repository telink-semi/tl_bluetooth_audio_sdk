/********************************************************************************************************
 * @file    tlkmdi_hra.h
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
#ifndef _TLKMDI_HRA_H_
#define _TLKMDI_HRA_H_

#define MIC_CHNL_NUM4_EN        0
#define AUDIO_FIFO_IRQ_EN       1
#define TLKALG_HRA_16K_EN       1
#define CODEC_LOW_POWER_MODE_EN 1

#define AUDIO_RX_FIFO           FIFO0
#define I2S0_TDM_TX_FIFO        FIFO1
#define I2S0_TDM_RX_FIFO        FIFO1
#define CODEC_MICB_FIFO         FIFO2
#define HRA_AUDIO_ID            IPC_DATA_PATH_0

#define MCU2DSP_MICA_CHNL_NUM   2
#define MCU2DSP_MICB_CHNL_NUM   2
#define MCU2DSP_MIC_CHNL_NUM    4
#define MCU2DSP_TDM_CHNL_NUM    4
#define MCU2DSP_DATA_MS         2
#define MCU2DSP_DATA_WIDTH_BYTE 2
#define MCU2DSP_TOTAL_CHNL_NUM  (MCU2DSP_MIC_CHNL_NUM + MCU2DSP_TDM_CHNL_NUM)

#define TDM_TX_FIFO_SIZE        2048
#define TDM_TX_FIFO_MAX         (TDM_TX_FIFO_SIZE - 1)
#define TDM_TX_FIFO_SIZE_BYTE   (TDM_TX_FIFO_SIZE * sizeof(short))

#define TDM_RX_FIFO_SIZE        2048
#define TDM_RX_FIFO_MAX         (TDM_RX_FIFO_SIZE - 1)
#define TDM_RX_FIFO_SIZE_BYTE   (TDM_RX_FIFO_SIZE * sizeof(short))

#define TIMER0_INTERVAL_US      400

typedef enum
{
    TLKMDI_HRA_TDM_DIS = 0,
    TLKMDI_HRA_TDM_EN,
    TLKMDI_HRA_TDM_MAX
} tlkmdi_hra_tdm_status_e;

typedef struct
{
    bool                    start_flag;
    bool                    dsp2d25f_irq_flag;
    bool                    overtime_flag;
    bool                    enable;
    bool                    send_status;
    bool                    play_status;
    bool                    loop_flag;
    tlkmdi_hra_tdm_status_e tdm_status;
    uint16_t                samplerate;
    uint16_t                tdm_rx_rptr;
    uint16_t                tdm_tx_wptr;
    uint16_t                micb_wptr;
    uint16_t                micb_rptr;
    uint32_t                send_tick;
    uint32_t                loop_tick;
} tlkmdi_hra_env_t;

/**
 * @brief       Initialize HRA module
 * @param       None
 * @return      1 - Success
 */
int tlkmdi_hra_init(void);

/**
 * @brief       Switch HRA state
 * @param[in]   handle - Connection handle
 * @param[in]   status - Status to switch to (1: enable, 0: disable)
 * @return      true - Operation successful
 */
bool tlkmdi_hra_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Check if HRA is busy
 * @param       None
 * @return      true - HRA is busy, false - HRA is idle
 */
bool tlkmdi_hra_is_busy(void);

/**
 * @brief       Handle timer interrupt for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_timer_irq_handler(void);

/**
 * @brief       Initialize timer for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_timer_init(void);

/**
 * @brief       Initialize DSP JTAG for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_dsp_jtag_init(void);

/**
 * @brief       Callback function for processing DSP messages
 * @param[in]   enc_buff_wptr - Encode buffer write pointer
 * @param[in]   type - Message type
 * @return      None
 */
void tlkmdi_hra_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type);

/**
 * @brief       Open codec for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_open_codec(void);

/**
 * @brief       Main loop for HRA processing
 * @param       None
 * @return      None
 */
void tlkmdi_hra_main_loop(void);

/**
 * @brief       Handle FIFO0 interrupt for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_fifo0_irq_handler_func(void);

/**
 * @brief       Set FIFO interrupt number
 * @param[in]   byte_num - Number of bytes for interrupt trigger
 * @return      None
 */
void tlkmdi_hra_set_fifo_irq_num(uint16_t byte_num);

/**
 * @brief       Confirm DSP acknowledge overtime
 * @param       None
 * @return      None
 */
void app_audio_dsp_ack_overtime_confirm(void);

/**
 * @brief       Clear send status
 * @param       None
 * @return      None
 */
void tlkmdi_hra_clear_send_status(void);

/**
 * @brief       Set sample rate
 * @param[in]   sr - Sample rate to set
 * @return      None
 */
void tlkmdi_hra_set_samplerate(uint16_t sr);

/**
 * @brief       Initialize TDM for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_tdm_init(void);

/**
 * @brief       Enable TDM for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_tdm_enable(void);

/**
 * @brief       Disable TDM for HRA
 * @param       None
 * @return      None
 */
void tlkmdi_hra_tdm_disable(void);

/**
 * @brief       Synchronize MICB samples
 * @param[in]   samples - Number of samples to synchronize
 * @return      None
 */
void tlkmdi_hra_sync_micb_samples(uint16_t samples);

/**
 * @brief       Get MICB buffer data
 * @param[out]  pBuffer - Buffer to store data
 * @param[in]   buffLen - Buffer length
 * @return      true - Success, false - Failure
 */
bool tlkmdi_hra_get_micb_buff_data(uint8_t *pBuffer, uint16_t buffLen);

/**
 * @brief       Get TDM TX read pointer
 * @param       None
 * @return      TDM TX read pointer
 */
uint32_t codec_get_tdm_tx_rptr(void);

/**
 * @brief       Get TDM RX write pointer
 * @param       None
 * @return      TDM RX write pointer
 */
uint32_t codec_get_tdm_rx_wptr(void);

/**
 * @brief       Synchronize TDM TX samples
 * @param[in]   samples - Number of samples to synchronize
 * @return      None
 */
void codec_sync_tdm_tx_samples(uint16_t samples);

/**
 * @brief       Synchronize TDM RX samples
 * @param[in]   samples - Number of samples to synchronize
 * @return      None
 */
void codec_sync_tdm_rx_samples(uint16_t samples);

#endif
