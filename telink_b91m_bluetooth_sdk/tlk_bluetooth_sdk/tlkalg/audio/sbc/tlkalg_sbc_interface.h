/********************************************************************************************************
 * @file    tlkalg_sbc_interface.h
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
#ifndef __TLKALG_SBC_INTERFACE_H__
#define __TLKALG_SBC_INTERFACE_H__
#include "common/types.h"
#include "tlkalg/audio/audio_alg_interface.h"
#include "tlkalg/audio/sbc/tlka_hfp_plc_api.h"
#include "tlkalg/audio/sbc/tlka_sbc_api.h"


extern uint8_t msbc_silence_pkt[];

/*
 * @brief       This function updates the SBC encoder bitpool.
 * @param[in]   bitpool - the new bitpool value.
 * @return      None.
 */
void tlkalg_sbc_enc_update_bitpool(uint8_t bitpool);

/**
 * @brief       This function retrieves the size required for the SBC decoder buffer.
 * @param[in]   channel - the audio channel configuration.
 * @return      The size of the SBC decoder buffer, aligned to 4 bytes.
 */
uint16_t tlkalg_sbc_dec_get_size(uint8_t channel);

/**
 * @brief       This function initializes the SBC decoder.
 * @param[in]   p_buff  - the buffer to be used for the decoder.
 * @param[in]   channel - the audio channel configuration.
 * @return      The result of the initialization process.
 */
int8_t tlkalg_sbc_dec_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function deinitializes the SBC decoder.
 * @return      The result of the deinitialization process.
 */
int8_t tlkalg_sbc_dec_deinit(void);

/**
 * @brief       This function processes an SBC encoded audio frame.
 * @param[in]   ps      - the input buffer containing the encoded audio data.
 * @param[out]  pd      - the output buffer for the decoded audio data.
 * @param[in]   len     - the length of the input audio data.
 * @param[in]   width   - the width of the audio data.
 * @param[in]   channel - the audio channel configuration.
 * @return      The length of the decoded audio data.
 */
int tlkalg_sbc_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       This function retrieves the size required for the SBC encoder buffer.
 * @param[in]   channel - the audio channel configuration.
 * @return      The size of the SBC encoder buffer, aligned to 4 bytes.
 */
uint16_t tlkalg_sbc_enc_get_size(uint8_t channel);

/**
 * @brief       This function initializes the SBC encoder.
 * @param[in]   p_buff  - the buffer to be used for the encoder.
 * @param[in]   channel - the audio channel configuration.
 * @return      The result of the initialization process.
 */
int8_t tlkalg_sbc_enc_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function sets the encoder parameters for SBC.
 * @param[in]   type  - the type of parameter to set.
 * @param[in]   param - the parameter value to set.
 * @return      The result of setting the parameter.
 */
uint8_t tlkalg_sbc_enc_param_set(uint8_t type, void *param);

/**
 * @brief       This function deinitializes the SBC encoder.
 * @return      The result of the deinitialization process.
 */
int8_t tlkalg_sbc_enc_deinit(void);

uint16_t tlkalg_sbc_enc_get_data_len(void);

/**
 * @brief       This function processes an SBC encoded audio frame.
 * @param[in]   ps      - the input buffer containing the audio data to be encoded.
 * @param[out]  pd      - the output buffer for the encoded audio data.
 * @param[in]   len     - the length of the input audio data.
 * @param[in]   width   - the width of the audio data.
 * @param[in]   channel - the audio channel configuration.
 * @return      The length of the encoded audio data.
 */
int tlkalg_sbc_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       This function retrieves the size required for the mSBC decoder buffer.
 * @param[in]   channel - the audio channel configuration.
 * @return      The size of the mSBC decoder buffer, aligned to 4 bytes.
 */
uint16_t tlkalg_msbc_dec_get_size(uint8_t channel);

/**
 * @brief       This function initializes the mSBC decoder.
 * @param[in]   p_buff  - the buffer to be used for the decoder.
 * @param[in]   channel - the audio channel configuration.
 * @return      The result of the initialization process.
 */
int8_t tlkalg_msbc_dec_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function deinitializes the mSBC decoder.
 * @return      The result of the deinitialization process.
 */
int8_t tlkalg_msbc_dec_deinit(void);

/**
 * @brief       This function processes an mSBC encoded audio frame.
 * @param[in]   ps      - the input buffer containing the encoded audio data.
 * @param[out]  pd      - the output buffer for the decoded audio data.
 * @param[in]   len     - the length of the input audio data.
 * @param[in]   width   - the width of the audio data.
 * @param[in]   channel - the audio channel configuration.
 * @return      The length of the decoded audio data.
 */
int tlkalg_msbc_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

/**
 * @brief       This function retrieves the size required for the mSBC encoder buffer.
 * @param[in]   channel - the audio channel configuration.
 * @return      The size of the mSBC encoder buffer, aligned to 4 bytes.
 */
uint16_t tlkalg_msbc_enc_get_size(uint8_t channel);

/**
 * @brief       This function initializes the mSBC encoder.
 * @param[in]   p_buff  - the buffer to be used for the encoder.
 * @param[in]   channel - the audio channel configuration.
 * @return      The result of the initialization process.
 */
int8_t tlkalg_msbc_enc_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       This function deinitializes the mSBC encoder.
 * @return      The result of the deinitialization process.
 */
int8_t tlkalg_msbc_enc_deinit(void);

/**
 * @brief       This function processes an mSBC encoded audio frame.
 * @param[in]   ps      - the input buffer containing the audio data to be encoded.
 * @param[out]  pd      - the output buffer for the encoded audio data.
 * @param[in]   len     - the length of the input audio data.
 * @param[in]   width   - the width of the audio data.
 * @param[in]   channel - the audio channel configuration.
 * @return      The length of the encoded audio data.
 */
int tlkalg_msbc_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

#if TLKALG_MSBC_ENABLE_CH2
int8_t tlkalg_msbc_dec_ch2_init(uint8_t *p_buff, uint8_t channel);
int8_t tlkalg_msbc_dec_ch2_deinit(void);
int    tlkalg_msbc_dec_ch2_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

int8_t tlkalg_msbc_enc_ch2_init(uint8_t *p_buff, uint8_t channel);
int8_t tlkalg_msbc_enc_ch2_deinit(void);
int    tlkalg_msbc_enc_ch2_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
#endif

#if BT_VOICE_SPP_TEST
int8_t tlkalg_msbc_dec_micbuf_init(uint8_t *p_buff, uint8_t channel);
int8_t tlkalg_msbc_dec_spkbuf_init(uint8_t *p_buff, uint8_t channel);
int8_t tlkalg_msbc_dec_micbuf_deinit(void);
int8_t tlkalg_msbc_dec_spkbuf_deinit(void);
int    tlkalg_msbc_dec_micbuf_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
int    tlkalg_msbc_dec_spkbuf_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);

int8_t tlkalg_msbc_enc_micbuf_init(uint8_t *p_buff, uint8_t channel);
int8_t tlkalg_msbc_enc_spkbuf_init(uint8_t *p_buff, uint8_t channel);
int8_t tlkalg_msbc_enc_micbuf_deinit(void);
int8_t tlkalg_msbc_enc_spkbuf_deinit(void);
int    tlkalg_msbc_enc_micbuf_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
int    tlkalg_msbc_enc_spkbuf_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
#endif

#endif
