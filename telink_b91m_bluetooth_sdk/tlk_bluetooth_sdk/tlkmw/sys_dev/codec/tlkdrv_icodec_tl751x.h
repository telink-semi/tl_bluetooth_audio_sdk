/********************************************************************************************************
 * @file    tlkdrv_icodec_tl751x.h
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
#ifndef TLKDRV_ICODEC_TL751X_H
#define TLKDRV_ICODEC_TL751X_H

#include "tl_common.h"

#if (MCU_CORE_TYPE == MCU_CORE_TL751X && TLKDRV_CODEC_ICODEC_ENABLE)
#include "drivers.h"

#if AUDIO_HD_HAC_EN
typedef struct
{
    uint8_t eq_bypass_en;
    uint8_t eq_reserved[3];
} tlkdrv_hac_eq_config_t;

typedef struct
{
    uint32_t frac_advance;
    uint32_t den_rate;
    int32_t  asrc_ppm;

    uint8_t int_advance;
    uint8_t asrc_ch;         //The channel number corresponds to the data bit. e.g:HAC_CHN0->bit0
    uint8_t ppm_updata_flag; //Update enabled flag
    uint8_t asrc_reserved;

    uint16_t asrc_in_rate;
    uint16_t asrc_out_rate;

} tlkdrv_asrc_t;

typedef struct
{
    signed short *in_addr;
    signed short *out_addr; //The channel number corresponds to the data bit. e.g:HAC_CHN0->bit0

    uint16_t data_len;
    uint8_t  asrc_in_dma;
    uint8_t  asrc_out_dma;

    uint8_t asrc_fifo;
    uint8_t asrc_bit_width;
    uint8_t asrc_mode; /// 0: 1:
    uint8_t asrc_reserved;
} tlkdrv_asrc_config_t;

typedef struct
{
    uint32_t hac_input_freq;
    uint32_t hac_output_freq;

    uint8_t hac_chn;
    uint8_t hac_count;
    uint8_t hac_eq_bypass_en;
    uint8_t hac_asrc_bypass_en;

    tlkdrv_hac_eq_config_t eq_para;
    tlkdrv_asrc_config_t   input_para;
    tlkdrv_asrc_t          asrc_para;

} tlkdrv_hac_config_t;

extern uint32_t st_audio_sys_clk;

extern tlkdrv_asrc_t        g_hac_asrc_spk_st;
extern tlkdrv_asrc_config_t g_hac_asrc_spk_para;

extern void        *g_hd_hac_asrc_buffer;
extern volatile int hac_init;

/**
 * @brief Set PPM value for HAC configuration
 * @param[in] p_asrc - Pointer to HAC configuration structure
 * @param[in] ppm - PPM value to set
 * @return None
 */
extern void tlkdrv_hac_set_ppm(tlkdrv_hac_config_t *p_asrc, int ppm);

/**
 * @brief Get PPM value from HAC configuration
 * @param[in] p_asrc - Pointer to HAC configuration structure
 * @return Current PPM value
 */
extern int tlkdrv_hac_get_ppm(tlkdrv_hac_config_t *p_asrc);

/**
 * @brief Get buffer size needed for HAC processing
 * @param[in] input_samp - Input sample rate
 * @param[in] output_samp - Output sample rate
 * @param[in] frame_len - Frame length
 * @param[in] data_width - Data width
 * @return Required buffer size
 */
extern int tlkdrv_hac_get_size(int input_samp, int output_samp, int frame_len, unsigned char data_width);

/**
 * @brief Initialize HAC module
 * @param[in] t_hac_st - Pointer to HAC configuration structure
 * @param[in] sysclk - System clock frequency
 * @param[in] input_sample - Input sample rate
 * @param[in] output_sample - Output sample rate
 * @param[in] frame_len - Frame length
 * @param[in] data_width - Data width
 * @return None
 */
extern void tlkdrv_hac_init(tlkdrv_hac_config_t *t_hac_st, uint32_t sysclk, int input_sample, int output_sample, int frame_len, unsigned char data_width);

/**
 * @brief Deinitialize HAC module
 * @param[in] hac_asrc_st - Pointer to HAC configuration structure
 * @return None
 */
extern void tlkdrv_hac_deinit(tlkdrv_hac_config_t *hac_asrc_st);

/**
 * @brief Set HAC parameters
 * @param[in] hac_asrc_st - Pointer to HAC configuration structure
 * @param[in] fifo_chn - FIFO channel
 * @param[in] in_dma - Input DMA channel
 * @param[in] out_dma - Output DMA channel
 * @param[in] chn - HAC channel
 * @param[in] tdm_ch - TDM channel
 * @param[in] bit_width - Bit width
 * @param[in] in_src - Input source buffer
 * @param[in] out_des - Output destination buffer
 * @param[in] packet_length - Packet length
 * @return None
 */
extern void tlk_hac_para_setting(tlkdrv_hac_config_t *hac_asrc_st, audio_fifo_chn_e fifo_chn, dma_chn_e in_dma, dma_chn_e out_dma, audio_hac_chn_e chn, unsigned char tdm_ch,
                                 audio_codec0_data_select_e bit_width, signed short *in_src, signed short *out_des, uint16_t packet_length);

/**
 * @brief Configure HAC settings
 * @param[in] hac_asrc_st - Pointer to HAC configuration structure
 * @return None
 */
extern void tlkdrv_hac_setting(tlkdrv_hac_config_t *hac_asrc_st);

/**
 * @brief Process HAC data
 * @param[in] hac_asrc_st - Pointer to HAC configuration structure
 * @param[in] in_buff - Input buffer
 * @param[in] out_buff - Output buffer
 * @param[in] input_num - Number of input samples
 * @return Number of output samples
 */
extern uint16_t tlkdrv_hac_process(tlkdrv_hac_config_t *hac_asrc_st, int *in_buff, int *out_buff, int input_num);

#endif //#if AUDIO_HD_HAC_EN

typedef struct
{
    uint8_t  IsEn;
    uint8_t  IsOpen;
    uint8_t  Channel;
    uint8_t  BitDepth;
    uint32_t SampleRate;

} tlkdrv_codec_cfg_t;

typedef struct
{
    uint8_t            isInit;
    uint8_t            sys_mclk;
    uint8_t            reserved[2];
    tlkdrv_codec_cfg_t codec_spk_cfg;
    tlkdrv_codec_cfg_t codec_mic_cfg;
} tlkdrv_icodec_t;

typedef struct
{
    uint8_t state;
    uint8_t type;
    int16_t delayMs;
    int16_t timerOnceMs;
    int16_t resv;
} tlkdrv_icodec_status_t;

#define CODEC_STAT1_DELAY_US 1000
#define CODEC_STAT0_DELAY_US 25000
#define CODEC_ADC_STATUS     0x08
#define CODEC_DAC_STATUS     0x80

extern uint8_t tl751x_codec_cur_power_mode;

/**
 * @brief Initialize TL751X internal codec hardware
 * @param None
 * @return None
 */
extern void tlkdrv_tl751x_codec_hd_init(void);

/**
 * @brief Deinitialize TL751X internal codec hardware
 * @param None
 * @return None
 */
extern void tlkdrv_tl751x_codec_hd_deinit(void);

/**
 * @brief Initialize DAC sidetone
 * @param[in] samplerate - Sample rate
 * @return Operation result status
 */
extern int tlkdrv_tl751x_dac_sidetone_init(int samplerate);

/**
 * @brief Enable ANC (Active Noise Cancellation) feature
 * @param[in] music - Music mode flag
 * @return None
 */
extern void tlkdrv_icodec_anc_enable(bool music);

/**
 * @brief Disable ANC (Active Noise Cancellation) feature
 * @param None
 * @return None
 */
extern void tlkdrv_icodec_anc_disable(void);

/**
 * @brief Set DMIC digital gain
 * @param[in] dmic_d_gain - Gain value to set
 * @return 1 if successful, 0 if gain value is too large
 */
extern uint8_t tlkdrv_icodec_dmic_d_gain(uint8_t dmic_d_gain);

#endif //#if (MCU_CORE_TYPE == MCU_CORE_TL751X && TLKDRV_CODEC_ICODEC_ENABLE)
#endif
