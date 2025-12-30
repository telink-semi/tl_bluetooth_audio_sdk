/********************************************************************************************************
 * @file    tlkmdi_anc_handler.h
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

#if TLKALG_ANC_ENABLE

#define ANC_FRAME_SIZE    32
#define AUDIO_ANC_CHANNEL ANC0

typedef struct
{
    uint32_t ref_gain;
    uint32_t wz_gain;
    uint16_t ref_dc;
    uint8_t  ref_shift;
    uint8_t  wz_shift;
} anc_gain_para_t;

typedef enum
{
    WZ_FIR_STAGE_39  = 39,
    WZ_FIR_STAGE_78  = 78,
    WZ_FIR_STAGE_156 = 156,
    WZ_FIR_STAGE_312 = 312,
    WZ_FIR_STAGE_384 = 390,

    CZ_FIR_STAGE_32  = 32,
    CZ_FIR_STAGE_64  = 64,
    CZ_FIR_STAGE_128 = 150,
} anc_fir_stage_e;

typedef struct
{
    uint16_t        wz_stages;
    uint16_t        cz_stages[3];
    anc_fir_stage_e fir_stages[2]; // WZ/CZ
    int             wz_iir_para[24][5];
    int             cz_iir_para[24][5];
    short           cz_fir_para[150];
    short           wz_fir_para[390];
} anc_filter_para_t;

typedef enum
{
    FILTER_WZ  = 1,
    FILTER_CZ0 = 2,
    FILTER_CZ1 = 4,
    FILTER_CZ2 = 8,
} anc_fil_mode_e;

typedef enum
{
    SAMPLERATE_192K = 0,
    SAMPLERATE_384K,
    SAMPLERATE_768K,
} anc_sample_rate_e;

typedef enum
{
    FILTER_LEFT = 0,
    FILTER_RIGHT,
} anc_fil_chn_e;

typedef enum
{
    CMD_DOWNLOAD_IIR  = 1,
    CMD_DOWNLOAD_FIR  = 2,
    CMD_DOWNLOAD_GAIN = 4,
    CMD_SYNC          = 8,
} anc_cmd_type_e;

typedef enum
{
    ADAPTIVE_EN,
    ANC_EN,
    ANC_MUSIC_EN,
    ANC_SYNC,
} anc_mode_type_e;

typedef enum
{
    ALG_PARA,
    MODE_PARA,
} anc_para_type_e;

typedef struct
{
    bool dsp_dis_req_flag;
    bool dsp_sync_flag;
} tlkmdi_anc_ctl;

typedef struct
{
    anc_fil_mode_e    filter_mode;
    audio_anc_mode_e  alg_mode;
    anc_sample_rate_e samplerate;
    anc_fil_chn_e     filter_chn;
    anc_cmd_type_e    cmd_type;
    anc_filter_para_t filter_para;
    anc_gain_para_t   gain_para;
    int              *iir_ptr;
    short            *fir_ptr;
} anc_reg_para_t;

/**
 * @brief       Enable DMA for ANC
 * @param       None
 * @return      None
 */
void tlkmdi_anc_enable_dma(void);

/**
 * @brief       Request to close DSP
 * @param       None
 * @return      None
 */
void tlkmdi_anc_request_close_dsp(void);

/**
 * @brief       Initialize ANC IRQ
 * @param[in]   byte_num - Byte number for buffer threshold
 * @return      None
 */
void tlkmdi_anc_irq_init(uint16_t byte_num);

/**
 * @brief       Synchronize DSP samples for ANC music
 * @param[in]   samples - Number of samples to sync
 * @return      None
 */
void tlkmdi_anc_music_sync_dsp_samples(uint16_t samples);

/**
 * @brief       Set DSP ANC downlink enable
 * @param[in]   enable - Enable flag
 * @return      None
 */
void d25f_set_dsp_anc_downlink_enable(uint8_t enable);

/**
 * @brief       Set DSP ANC enable
 * @param[in]   enable - Enable flag
 * @return      None
 */
void d25f_set_dsp_anc_enable(uint8_t enable);

/**
 * @brief       Enable DSP ANC sync
 * @param       None
 * @return      None
 */
void d25f_set_dsp_anc_sync_enable(void);

/**
 * @brief       Check if DSP is synchronized
 * @param       None
 * @return      true if synchronized, false otherwise
 */
bool tlkmdi_anc_dsp_is_sync_data(void);

/**
 * @brief       Pause ANC processing
 * @param       None
 * @return      None
 */
void tlkmdi_anc_pausing(void);

/**
 * @brief       Process ANC mode parameters
 * @param[in]   data - Pointer to parameter data
 * @return      None
 */
void tlkalg_anc_proc_mode_para(int *data);

// void tlkmdi_anc_dsp_msg_process_callback(uint8_t enc_buff_wptr, uint8_t type);

/**
 * @brief       Load ANC parameters
 * @param[in]   data - Pointer to parameter data
 * @return      None
 */
void tlkalg_anc_load_para(uint8_t *data);

/**
 * @brief       Switch ANC adaptive mode
 * @param[in]   adapt_flag - Adaptive mode flag
 * @return      None
 */
void d25f_switch_anc_adaptive_mode(uint8_t adapt_flag);

/**
 * @brief       Deinitialize ANC IRQ
 * @param       None
 * @return      None
 */
void tlkmdi_anc_irq_deinit(void);
#endif
