/********************************************************************************************************
 * @file    tlkdrv_icodec_tl752x.h
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
#ifndef TLKDRV_ICODEC_TL752X_H
#define TLKDRV_ICODEC_TL752X_H

#include "core/mcu_type.h"
#include "types.h"
#include <stdio.h>
#include <string.h>
//#include "clock.h"
#include "drivers/TL752X/driver/inc/drv_uart.h"
#include "drivers/TL752X/driver/inc/drv_audio_dac.h"
#include "drivers/TL752X/driver/inc/drv_audio_adc.h"
#include "drivers/TL752X/driver/modules/drv_gpio.h"
#include "drivers/TL752X/driver/inc/drv_cpr.h"
#include "drivers/TL752X/driver/inc/drv_dmac.h"
#include "drivers/TL752X/driver/hal/hal_systimer.h"
#include "drivers/TL752X/temp_cmpt/temp_cmpt.h"

#if (MCU_CORE_TYPE == MCU_CORE_TL752X)
#include "tlkdrv_codec.h"
#if (TLKDRV_CODEC_ICODEC_ENABLE)

#define LINE_IN_TO_LINE_OUT_CASE (1)
#define LINE_IN_TO_BUFF_CASE     (2)
#define BUFF_TO_LINE_OUT_CASE    (3)

#define CODEC_MODE_SEL           BUFF_TO_LINE_OUT_CASE


#define DLM_BUS_READ_OFFSET      0x40000000

#define DATA_BLOCK_NUM           2
#define DATA_BLOCK_SIZE          48 //64
#define DATA_BUFF_LEN            (DATA_BLOCK_NUM * DATA_BLOCK_SIZE)

extern dmac_handle_t audio_adc_dmac_handle;
extern dmac_handle_t audio_dac_dmac_handle;

extern uint32_t     st_audio_sys_clk;
extern void        *g_hd_hac_asrc_buffer;
extern volatile int hac_init;

/**
 * @brief DAC DMA transmission complete callback function
 * @param[in] arg - Callback argument
 * @return None
 */
extern void codec_dac_dma_tx_complete_cb(void *arg);

/**
 * @brief ADC DMA reception complete callback function
 * @param[in] arg - Callback argument
 * @return None
 */
extern void codec_adc_dma_rx_complete_cb(void *arg);

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


#endif // #if (TLKDRV_CODEC_ICODEC_ENABLE)
#endif // #if (MCU_CORE_TYPE == MCU_CORE_TL752X)

#endif // TLKDRV_ICODEC_TL752X_H
