/********************************************************************************************************
 * @file    tlkmdi_recording_card_handler.h
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
#ifndef _TLKMDI_RECORDING_CARD_HANDLER_H_
#define _TLKMDI_RECORDING_CARD_HANDLER_H_

#define RECORDING_CARD_MIC_FRAME_SIZE (16 * 20)

#ifndef MIC_FRAME_SIZE
#define MIC_FRAME_SIZE RECORDING_CARD_MIC_FRAME_SIZE
#endif
#if TLKALG_GET_AUDIO_DATA_EN
extern uint8_t spi_debug_buff[RECORDING_CARD_MIC_FRAME_SIZE * (TLKALG_BBF_ENABLE + 3) * 2 + 8];
#endif

typedef void (*tlkalg_recording_card_fill_enc_data_t)(uint8_t *pbuff, uint16_t len);
typedef void (*tlkalg_recording_card_fill_pcm_data_t)(uint8_t *pbuff, uint16_t len);

extern int8_t alg_bone_data_switch;

/**
 * @brief       This function initializes the recording card algorithm.
 * @param       none.
 * @return      none.
 */
void tlkmdi_recording_card_alg_init(void);

/**
 * @brief       This function deinitializes the recording card algorithm.
 * @param       none.
 * @return      none.
 */
void tlkmdi_recording_card_alg_deinit(void);

/**
 * @brief       This function initializes the FIFO IRQ for the recording card.
 * @param[in]   byte_num - number of bytes for FIFO initialization.
 * @return      none.
 */
void tlkmdi_recording_card_fifo_irq_init(uint16_t byte_num);

/**
 * @brief       This function handles the FIFO IRQ for the recording card.
 * @param       none.
 * @return      none.
 */
void tlkmdi_recording_card_fifo_irq_handler_func(void);

int8_t tlkmdi_recording_card_fifo_irq_process_6(adc_mono_int *data_buff);
int8_t tlkmdi_recording_card_fifo_irq_process_4(adc_mono_int *data_buff);
int8_t tlkmdi_recording_card_fifo_irq_process_2(adc_mono_int *data_buff);
int8_t tlkmdi_recording_card_fifo_irq_process(adc_mono_int *data_buff);


void tlkmdi_recording_card_process_pcm_from_dsp(int8_t *pcm);

void tlkmdi_recording_card_clean_dsp_queue(void);

/**
 * @brief       This function processes DSP messages for the recording card.
 * @param[in]   enc_buff_wptr - write pointer of the encoded buffer.
 * @param[in]   type - message type.
 * @return      none.
 */
void tlkmdi_recording_card_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type);

/**
 * @brief       This function registers a callback to fill encoded data.
 * @param[in]   func - callback function pointer.
 * @return      none.
 */
void tlkmdi_recording_card_fillEncData_register(tlkalg_recording_card_fill_enc_data_t func);

/**
 * @brief       This function registers a callback to fill PCM data.
 * @param[in]   func - callback function pointer.
 * @return      none.
 */
void tlkmdi_recording_card_fillPcmData_register(tlkalg_recording_card_fill_pcm_data_t func);


#endif