/********************************************************************************************************
 * @file    tlkalg_eq_interface.h
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
#ifndef __TLKALG_EQ_INTERFACE_H__
#define __TLKALG_EQ_INTERFACE_H__
#include "tlkalg/audio/audio_alg_interface.h"

#define EQ_SAMPLE_NUM_MAX               512

#define EQ_FILTER_STAGE_MAX             9
#define EQ_NSTAGE_MUSIC_SPK_MAX         9
#define EQ_NSTAGE_VOICE_SPK_MAX         4
#define EQ_NSTAGE_VOICE_MIC_MAX         4

typedef enum
{
    EQ_TYPE_MUSIC     = 0,
    EQ_TYPE_VOICE_MIC = 1,
    EQ_TYPE_VOICE_SPK = 2,
} e_eq_type_e;

typedef struct _eq_mode_para
{
    int16_t  gain;        /**< Gain range(BBEE/1000): -327.68db ~ +327.67db*/
    uint16_t freq_c;      /**< Center frequency range 20Hz ~ 24kHz*/
    uint16_t filter_q;    /**< Q value range(HHGG/1000) 0 ~ 65.535*/
    uint8_t  filter_type; /**< filter type:1 ~ 8*/
    uint8_t  rsvd;        /**< reserved*/
} eq_mode_para_t;

typedef struct _eq_mode_header
{
    uint8_t idx;          /**< index: fixed mode:0 ~ 21	customer mode:0xfefe*/
    uint8_t max_gain_num; /**< */
    uint8_t filter_sum;   /**< number of filter*/
    uint8_t all_gain;     /**< Full-band gain reduction range:0~25.6dB*/
} eq_mode_header_t;

typedef struct _eq_mode
{
    eq_mode_header_t eq_header;                     /**< refer to eq_mode_header_t 4 bytes*/
    eq_mode_para_t   eq_para[EQ_FILTER_STAGE_MAX];  /**< refer to eq_mode_para_t 8*9 bytes*/
    int32_t          CRC;                           /**< reserved 4 bytes*/
} eq_mode_t;

typedef struct _eq_para
{
    uint32_t            samplerate;
    e_eq_type_e         eq_type;
    ALG_CHANNEL_IDX     channel;
    ALG_BITDEPTH        width;
} eq_para_t;

/**
 * @brief   Reads EQ parameters from flash memory.
 * @param[in]   addr    - The address in flash memory to read from.
 */
void tlkalg_eq_read_para_from_flash(uint32_t addr);

/**
 * @brief   Sets the EQ parameters for a given mode.
 * @param[in]   para    - Pointer to the EQ mode structure.
 */
void tlkalg_tool_para(eq_mode_t *para);

/**
 * @brief   Initializes the EQ module.
 * @param[in]   p_buff    - Pointer to the buffer (unused).
 * @param[in]   channel   - The channel to initialize (unused).
 * @return      0 for successful initialization, -1 otherwise.
 */
int8_t tlkalg_eq_init(uint8_t *p_buff, uint8_t channel);

/**
 * @brief   Deinitializes the EQ module.
 * @return      0 for successful deinitialization, -1 otherwise.
 */
int8_t tlkalg_eq_deinit(void);

/**
 * @brief   Sets the EQ parameters based on the type and channel.
 * @param[in]   type    - The type of EQ parameter to set.
 * @param[in]   param   - Pointer to the EQ parameter structure.
 * @return      0 for successful parameter setting, -1 otherwise.
 */
uint8_t tlkalg_eq_param_set(uint8_t type, void *param);

/**
 * @brief   Processes audio data using EQ for 16-bit samples.
 * @param[in]   ps      - Pointer to the source audio data.
 * @param[out]  pd      - Pointer to the destination audio data.
 * @param[in]   len     - Length of the audio data to process.
 * @param[in]   channel - The channel to process.
 * @return      0 for successful processing, -1 otherwise.
 */
int tlkalg_eq_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel);
#endif
