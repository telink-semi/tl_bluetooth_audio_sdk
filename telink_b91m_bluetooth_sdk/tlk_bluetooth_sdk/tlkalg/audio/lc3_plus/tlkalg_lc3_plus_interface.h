/********************************************************************************************************
 * @file    tlkalg_lc3_plus_interface.h
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
#pragma once
#include "tlkalg/audio/audio_alg_interface.h"
#include "tlka_lc3a_api.h"

#if TLKALG_LC3_PLUS_ENC_ENABLE || TLKALG_LC3_PLUS_DEC_ENABLE

typedef enum
{
    LC3_PLUS_TYPE_ENC_NORMAL,
    LC3_PLUS_TYPE_ENC_ULTRA_LOW_LATENCY,
    LC3_PLUS_TYPE_ENC_UNKNOWN,
} lc3p_enc_type;

typedef enum
{
    LC3_PLUS_TYPE_DEC_NORMAL,
    LC3_PLUS_TYPE_DEC_ULTRA_LOW_LATENCY,
    LC3_PLUS_TYPE_DEC_UNKNOWN,
} lc3p_dec_type;

typedef enum
{
    LC3P_SR_16K = 16000,
    LC3P_SR_48K = 48000,
} lc3p_samplerate;

typedef enum
{
    LC3P_CHN_SINGLE = 1,
    LC3P_CHN_STEREO = 2,
} lc3p_channel_num;

typedef enum
{
    LC3P_BITRATE_16k = 16000,
    LC3P_BITRATE_24k = 24000,
    LC3P_BITRATE_32k = 32000,
    LC3P_BITRATE_48k = 48000,
    LC3P_BITRATE_72k = 72000,
    LC3P_BITRATE_80k = 80000,
    LC3P_BITRATE_96k = 96000,
} lc3p_bitrate_num;

/**
 * @brief       Set LC3 Plus encoder parameters
 * @param[in]   type - Encoder type
 * @param[in]   param - Pointer to parameter structure
 * @return      Operation result, 1 for success, 0 for failure
 */
uint8_t tlkalg_lc3_plus_enc_set_param(uint8_t type, void *param);
/**
 * @brief       Set LC3 Plus decoder parameters
 * @param[in]   type - Decoder type
 * @param[in]   param - Pointer to parameter structure
 * @return      Operation result, 1 for success, 0 for failure
 */
uint8_t tlkalg_lc3_plus_dec_set_param(uint8_t type, void *param);
/**
 * @brief       Change LC3 Plus encoder channel count
 * @param[in]   channel_in - Input channel count
 * @return      Output channel count
 */
uint8_t tlkalg_lc3_plus_enc_channel_change(uint8_t channel_in);
/**
 * @brief       Change LC3 Plus decoder channel count
 * @param[in]   channel_in - Input channel count
 * @return      Output channel count
 */
uint8_t tlkalg_lc3_plus_dec_channel_change(uint8_t channel_in);

/**
 * @brief       Initialize LC3 Plus encoder
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      Operation result, 0 for success
 */
int8_t tlkalg_lc3_plus_enc_init(uint8_t *p_buff, uint8_t channel);
/**
 * @brief       Initialize LC3 Plus decoder
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      Operation result, 0 for success
 */
int8_t tlkalg_lc3_plus_dec_init(uint8_t *p_buff, uint8_t channel);
/**
 * @brief       Deinitialize LC3 Plus encoder
 * @return      Operation result, 0 for success
 */
int8_t tlkalg_lc3_plus_enc_deinit(void);
/**
 * @brief       Deinitialize LC3 Plus decoder
 * @return      Operation result, 0 for success
 */
int8_t tlkalg_lc3_plus_dec_deinit(void);
/**
 * @brief       Get LC3 Plus encoder size
 * @param[in]   channel - Channel configuration
 * @return      Required size in bytes
 */
uint16_t tlkalg_lc3_plus_enc_get_size(uint8_t channel);
/**
 * @brief       Get LC3 Plus decoder size
 * @param[in]   channel - Channel configuration
 * @return      Required size in bytes
 */
uint16_t tlkalg_lc3_plus_dec_get_size(uint8_t channel);
/**
 * @brief       Get LC3 Plus encoder size by type
 * @param[in]   size_type_in - Size type
 * @param[in]   chnl_in - Channel count
 * @return      Required size in bytes
 */
uint16_t tlkalg_lc3_plus_enc_get_size_type(uint8_t size_type_in, uint8_t chnl_in);
/**
 * @brief       Get LC3 Plus decoder size by type
 * @param[in]   size_type_in - Size type
 * @param[in]   chnl_in - Channel count
 * @return      Required size in bytes
 */
uint16_t tlkalg_lc3_plus_dec_get_size_type(uint8_t size_type_in, uint8_t chnl_in);

/**
 * @brief       Process LC3 Plus encoding
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Operation result, 1 for success, 0 for failure
 */
int tlkalg_lc3_plus_enc_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
/**
 * @brief       Process LC3 Plus decoding
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Operation result, 1 for success, 0 for failure
 */
int tlkalg_lc3_plus_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
/**
 * @brief       Deinterleave integer data
 * @param[in]   in - Input data pointer
 * @param[out]  out - Output data pointers array
 * @param[in]   n - Number of samples
 * @param[in]   channels - Number of channels
 * @return      None
 */
void deinterleave_int(int32_t *in, int32_t **out, int32_t n, int32_t channels);
/**
 * @brief       Interleave integer data
 * @param[in]   in - Input data pointers array
 * @param[out]  out - Output data pointer
 * @param[in]   n - Number of samples
 * @param[in]   channels - Number of channels
 * @return      None
 */
void interleave_int(int32_t **in, int32_t *out, int32_t n, int32_t channels);


//uint8_t lc3_plus_16k_enc_out_data[60] = {
//	0x18,
//	0x97,
//	0xaf,
//	0xf9,
//	0x7a,
//	0x21,
//	0xa7,
//	0xd6,
//	0x9f,
//	0x5a,
//	0x5a,
//	0x71,
//	0xc7,
//	0xba,
//	0x60,
//	0x10,
//	0xea,
//	0xa1,
//	0x93,
//	0xb7,
//	0x1,
//	0x89,
//	0x3e,
//	0x1d,
//	0x62,
//	0x15,
//	0x49,
//	0xf1,
//	0x82,
//	0x23,
//
//	0x18,
//	0x97,
//	0xaf,
//	0xf9,
//	0x7a,
//	0x21,
//	0xa7,
//	0xd6,
//	0x9f,
//	0x5a,
//	0x5a,
//	0x71,
//	0xc7,
//	0xba,
//	0x60,
//	0x10,
//	0xea,
//	0xa1,
//	0x93,
//	0xb7,
//	0x1,
//	0x89,
//	0x3e,
//	0x1d,
//	0x62,
//	0x15,
//	0x49,
//	0xf1,
//	0x82,
//	0x23,
//  };
//
//uint8_t lc3_plus_enc_out_data[90]  ={
//	0xff,
//	0xdd,
//	0x1c,
//	0xea,
//	0x18,
//	0x2c,
//	0xee,
//	0xd7,
//	0x7e,
//	0x6b,
//	0x88,
//	0xb,
//	0x28,
//	0x18,
//	0x1,
//	0x90,
//	0x97,
//	0x52,
//	0x44,
//	0x3c,
//	0xf9,
//	0x6b,
//	0xd9,
//	0x4e,
//	0xa3,
//	0x63,
//	0xc3,
//	0x49,
//	0x57,
//	0x2f,
//	0x36,
//	0x25,
//	0xff,
//	0xcb,
//	0xd6,
//	0x8f,
//	0x50,
//	0xa,
//	0xaa,
//	0x87,
//	0x50,
//	0x70,
//	0x65,
//	0x7c,
//	0x46,
//	0x11,
//	0x43,
//	0x22,
//	0xd0,
//	0x72,
//	0x3b,
//	0x31,
//	0x14,
//	0xd1,
//	0x91,
//	0x42,
//	0xa3,
//	0x60,
//	0xcf,
//	0xa,
//	0x3d,
//	0x67,
//	0x5,
//	0xcd,
//	0x4e,
//	0x2e,
//	0x5,
//	0x53,
//	0xcf,
//	0x62,
//	0x4a,
//	0x37,
//	0x46,
//	0x78,
//	0xab,
//	0xeb,
//	0xc,
//	0xca,
//	0x4a,
//	0x12,
//	0xf4,
//	0x5a,
//	0xde,
//	0xf3,
//	0x6e,
//	0x76,
//	0x72,
//	0xf1,
//	0x4c,
//	0x32,
//};

#endif
