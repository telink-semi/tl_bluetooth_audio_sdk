/********************************************************************************************************
 * @file    tlkalg_aac_interface.h
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
#include "tlkalg/audio/aac/tlka_aac_dec_api.h"

#ifndef CODEC_AAC_TLK_AAC_INTERFACE_API_H_
    #define CODEC_AAC_TLK_AAC_INTERFACE_API_H_
#if TLKALG_AAC_DEC_ENABLE
extern void                  *g_aac_dec_env_buf_ptr;
extern void                  *g_aac_scratch_buf_ptr;
extern TLKA_AAC_DEC_CFG_PARAS aacParas;
extern HANDLE_AACDECODER      aacDecoder_handle;

/**
 * @brief       Calculate the size required for AAC decoding and scratch buffer
 * @param[in]   channel - the number of channels
 * @return      Total size required for AAC decoding
 */
uint16_t tlkalg_aac_dec_get_size(uint8_t channel);

/**
 * @brief       Initialize AAC decoder with provided buffers
 * @param[in]   p_buff - pointer to the buffer for AAC decoding
 * @param[in]   channel - the number of channels
 * @return      0 on success, non-zero on failure
 */
int8_t   tlkalg_aac_dec_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief       Deinitialize AAC decoder
 * @return      0 on success, non-zero on failure
 */
int8_t   tlkalg_aac_dec_deinit(void);

/**
 * @brief       Process an AAC frame and decode it
 * @param[in]   ps - pointer to the source (undecoded) data
 * @param[in]   pd - pointer to the destination (decoded) data
 * @param[in]   len - length of the source data
 * @param[in]   width - the width of the audio data
 * @param[in]   channel - the number of channels
 * @return      Decoding state
 */
int      tlkalg_aac_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
#endif
    #if 0
/**
 * @brief       Set up the AAC required for the decoding and scratch buffer
 * @param[in]   p       - pointer to the system buffer for AAC decoding
 * @param[in]   ps      - pointer to the AAC module's scratch buffer system buffer
 * @return      size required for the AAC decoding
 */
extern int aac_set_dec_buffer_scratch (void *p, void *ps);

/**
 * @brief       Decode the left channel AAC data
 * @param[in]   ps      - pointer to the undecoded data
 * @param[in]   len     - length of the undecoded data
 * @param[out]  pd      - pointer to the decoded data
 * @return      decoding state
 */
extern int tlkalg_aac_dec_chn_left (const unsigned char *ps, int len, unsigned char *pd);

/**
 * @brief       Decode the right channel AAC data
 * @param[in]   ps      - pointer to the undecoded data
 * @param[in]   len     - length of the undecoded data
 * @param[out]  pd      - pointer to the decoded data
 * @return      decoding state
 */
extern int tlkalg_aac_dec_chn_right (const unsigned char *ps, int len, unsigned char *pd);

/**
 * @brief       Decode the stereo AAC data
 * @param[in]   ps      - pointer to the undecoded data
 * @param[in]   len     - length of the undecoded data
 * @param[out]  pd      - pointer to the decoded data
 * @return      decoding state
 */
extern int tlkalg_aac_dec_stereo (const unsigned char *ps, int len, unsigned char *pd);
    #endif
#endif /* CODEC_AAC_TLK_AAC_INTERFACE_API_H_ */
