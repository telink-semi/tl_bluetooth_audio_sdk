/********************************************************************************************************
 * @file    tlkdrv_icodec_tl721x.h
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
#ifndef TLKDRV_ICODEC_tl322x_H
#define TLKDRV_ICODEC_tl322x_H

#include "tl_common.h"
#include "tlkdrv_i2s_ex_codec_config.h"

#if (MCU_CORE_TYPE == MCU_CORE_TL721X && TLKDRV_CODEC_ICODEC_ENABLE)
#include "drivers.h"

#ifndef I2S_CODEC_SEL
#define I2S_CODEC_SEL 0
#endif

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

/**
 * @brief Set audio receive write pointer
 * @param[in] rx_fifo_chn - Receive FIFO channel
 * @param[in] val - Write pointer value
 * @return None
 */
extern void audio_set_rx_wptr(audio_fifo_chn_e rx_fifo_chn, unsigned short val);

/**
 * @brief Set DMIC digital gain
 * @param[in] gain - Gain value to set
 * @return 1 if successful, 0 if gain value is too large
 */
extern uint8_t tlkdrv_icodec_dmic_d_gain(uint8_t dmic_d_gain);

#endif //#if (MCU_CORE_TYPE == MCU_CORE_tl721x && TLKDRV_CODEC_ICODEC_ENABLE)
#endif
