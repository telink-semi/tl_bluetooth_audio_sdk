/********************************************************************************************************
 * @file    app_src_uac.h
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

#if MCU_CORE_TL752X_TEMP
#define TLK_USB_UAC_ENABLE 0
#else
#define TLK_USB_UAC_ENABLE 1
#endif

#define TLKBTP_CFG_RFC_ENABLE   (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_HFP_ENABLE   (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_HFPAG_ENABLE (1 && TLKBTP_CFG_HFP_ENABLE)
#if MCU_CORE_TL752X_TEMP
#define BT_A2DP_OUT_INPUT_TYPE 2 //TLKMDI_A2DP_OUT_TYPE_USB_AUDIO
#else
#define BT_A2DP_OUT_INPUT_TYPE TLKMDI_A2DP_OUT_TYPE_USB_AUDIO
#endif
#define TLK_DEV_CODEC_ENABLE 1

#if MCU_CORE_TL752X_TEMP
#define TLKDRV_CODEC_ICODEC_ENABLE (1 & TLK_DEV_CODEC_ENABLE)
#else
#define TLKDRV_CODEC_ICODEC_ENABLE (0 & TLK_DEV_CODEC_ENABLE)
#endif


#define LE_AUDIO_CODEC_INPUT_TYPE  LE_AUDIO_CODEC_TYPE_USB_AUDIO
#define LE_AUDIO_CODEC_OUTPUT_TYPE LE_AUDIO_CODEC_TYPE_USB_AUDIO

// #define LE_AUDIO_CODEC_INPUT_TYPE LE_AUDIO_CODEC_SAMPLE_SINE_WAVE
// #define LE_AUDIO_CODEC_OUTPUT_TYPE LE_AUDIO_CODEC_TYPE_NONE
