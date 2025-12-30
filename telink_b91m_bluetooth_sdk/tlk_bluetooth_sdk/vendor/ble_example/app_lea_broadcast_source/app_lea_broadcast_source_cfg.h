/********************************************************************************************************
 * @file    app_lea_broadcast_source_cfg.h
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


#define APP_EXT_ADV_SETS_NUMBER       1
#define APP_EXT_ADV_DATA_LENGTH       200

#define APP_PERIODIC_ADV_SETS_NUMBER  1
#define APP_PERIODIC_ADV_DATA_LENGTH  200

#define APP_EXT_SCAN_ENABLE           1
#define APP_PERIODIC_ADV_SYNC_ENABLE  1
#define APP_BIG_SYNC_ENABLE           1
#define BIS_STREAM_48kHz_96kbps       1
#define BIS_STREAM_48kHz_80kbps       2
#define BIS_STREAM_32KHz_64Kbps       3
#define BIS_STREAM_24KHz_48Kbps       4

#define BIS_STREAM_CONFIG             BIS_STREAM_48kHz_96kbps
#define APP_PERIODIC_ADV_ENABLE       1
#define APP_BIG_ENABLE                1

#define TLK_MW_LEA_BMS_ENABLE 1
#undef TLKDRV_CODEC_ICODEC_ENABLE
#if (MCU_CORE_TYPE == MCU_CORE_TL322X)
#define LE_AUDIO_CODEC_INPUT_TYPE  LE_AUDIO_CODEC_TYPE_USB_AUDIO

#define TLK_USB_UAC_ENABLE 0
#define TLK_USB_UAC_HS_ENABLE      1
#define TLKDRV_CODEC_ICODEC_ENABLE 0
#elif defined(MCU_CORE_TL752X_TEMP) && MCU_CORE_TYPE == MCU_CORE_TL752X
#define LE_AUDIO_CODEC_INPUT_TYPE  LE_AUDIO_CODEC_TYPE_USB_AUDIO

#define TLK_USB_UAC_ENABLE 0
#define TLK_USB_UAC_HS_ENABLE      0
#define TLKDRV_CODEC_ICODEC_ENABLE 0
#else
#define LE_AUDIO_CODEC_INPUT_TYPE  LE_AUDIO_CODEC_TYPE_USB_AUDIO
#define TLK_USB_UAC_ENABLE 1
#define TLK_USB_UAC_HS_ENABLE      0
#define TLKDRV_CODEC_ICODEC_ENABLE 0
#endif


#define TLKALG_ASRC_16TO48_16BIT_ENABLE  1
#define TLKALG_ASRC_48TO16_16BIT_ENABLE  1

