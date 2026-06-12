/********************************************************************************************************
 * @file    tlkalg_eq.h
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
/*
 * tlkalg_eq.h
 *
 *  Created on: 2024年12月31日
 *      Author: Admin
 */


#ifndef VENDOR_MULTI_CORE_AUDIO_DEMO_AUDIO_ALG_EQ_TLKALG_EQ_H_
#define VENDOR_MULTI_CORE_AUDIO_DEMO_AUDIO_ALG_EQ_TLKALG_EQ_H_

#define TLKALG_EQ_MAX_FILTER_STAGE 9
#define TLKALG_EQ_MAX_SAMPLE       128

typedef enum
{
    EQ_CMD_DISABLE_EQ = 0,
    EQ_CMD_ENABLE_EQ,
    EQ_CMD_DOWNLOAD,
    EQ_CMD_SYNC,
} e_eq_cmd_type_e;

typedef enum
{
    EQ_DATA_TYPE_INT16 = 2,
    EQ_DATA_TYPE_INT32 = 4,
} e_eq_data_type_e;

typedef enum
{
    EQ_HANDLE_MUSIC_L = 0,
    EQ_HANDLE_MUSIC_R,
    EQ_HANDLE_MUSIC_S, /** stereo*/
    EQ_HANDLE_MIC,
    EQ_HANDLE_SPK,
} e_eq_handle_e;

typedef struct _eq_mode_para
{
    int16_t  gain;         /**< Gain range(BBEE/1000): -327.68db ~ +327.67db*/
    uint16_t freq_c;       /**< Center frequency range 20Hz ~ 24kHz*/
    uint16_t filter_q;     /**< Q value range(HHGG/1000) 0 ~ 65.535*/
    uint8_t  filter_type;  /**< filter type:1 ~ 8*/
                           //	uint8_t	 cvsd;        /**< reserved*/
    uint8_t filter_enable; /**< filter enable flag*/
} eq_mode_para_t;

typedef struct _eq_mode_header
{
    uint8_t       idx;          /**< index: fixed mode:0 ~ 21	customer mode:0xfefe*/
    uint8_t       max_gain_num; /**< */
    uint8_t       filter_sum;   /**< number of filter*/
    uint8_t       all_gain;     /**< Full-band gain reduction range:0~25.6dB*/
    e_eq_handle_e eq_mode;
} eq_mode_header_t;

typedef struct _eq_mode
{
    eq_mode_header_t eq_header;                           /**< refer to eq_mode_header_t 4 bytes*/
    eq_mode_para_t   eq_para[TLKALG_EQ_MAX_FILTER_STAGE]; /**< refer to eq_mode_para_t 8*9 bytes*/
    int32_t          CRC;                                 /**< reserved 4 bytes*/
} eq_mode_t;

/**
 * @brief   Handle equalizer parameter debug commands.
 * @param[in]   p_data  - Pointer to the command data buffer.
 * @param[in]   data_len- Length of the command data.
 */
void app_eq_para_debug_handler(uint8_t *p_data, uint16_t data_len);

/**
 * @brief   Load equalizer parameters from a buffer.
 * @param[in]   para    - Pointer to the buffer containing equalizer parameters.
 */
void tlkalg_eq_load_para(uint8_t *data);

/**
 * @brief   Synchronize equalizer parameters from a buffer.
 * @param[in]   buff    - Pointer to the buffer containing equalizer parameters.
 */
void tlkalg_eq_sync_para(uint8_t *buff);

/**
 * @brief   Allocate and set equalizer parameters based on the command data.
 * @param[in]   ps      - Pointer to the command data buffer.
 * @param[in]   len     - Length of the command data.
 */
void tlkalg_eq_allocate_parameter(unsigned char *ps, int len);

#endif /* VENDOR_MULTI_CORE_AUDIO_DEMO_AUDIO_ALG_EQ_TLKALG_EQ_H_ */
