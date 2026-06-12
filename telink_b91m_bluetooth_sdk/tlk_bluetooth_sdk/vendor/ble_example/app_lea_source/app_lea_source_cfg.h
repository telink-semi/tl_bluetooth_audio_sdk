/********************************************************************************************************
 * @file    app_lea_source_cfg.h
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

// LE Audio Music.
#define TLK_MW_LEA_UC_MUSIC_ENABLE (1)
// MIC function.
#define TLK_MW_LEA_UC_MIC_ENABLE (1)
// voice function.
#define TLK_MW_LEA_UC_VOICE_ENABLE (1)

#define TLKAPP_LEMGR_ENABLE        (1)

#if (MCU_CORE_TYPE == MCU_CORE_TL322X)
#define TLK_USB_UAC_ENABLE    0
#define TLK_USB_UAC_HS_ENABLE 1
#else
#define TLK_USB_UAC_ENABLE    1
#define TLK_USB_UAC_HS_ENABLE 0
#endif

#undef TLKDRV_CODEC_ICODEC_ENABLE
#undef TLKDRV_CODEC_I2S_MASTER_ENABLE
#define TLKDRV_CODEC_ICODEC_ENABLE          0
#define TLKDRV_CODEC_I2S_MASTER_ENABLE      0

#define LE_AUDIO_CODEC_INPUT_TYPE           LE_AUDIO_CODEC_TYPE_USB_AUDIO
#define LE_AUDIO_CODEC_OUTPUT_TYPE          LE_AUDIO_CODEC_TYPE_USB_AUDIO

#define BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM 1
#if BLE_AUDIO_ENABLE_32KHZ_VOICE_STREAM
#define TLKALG_ASRC_48TO32_16BIT_ENABLE 1
#define TLKALG_ASRC_32TO48_16BIT_ENABLE 1
#define TLKALG_ASRC_32TO16_16BIT_ENABLE 1

#define TLK_CFG_UART_TOOL_ENABLE        1

#define TLKMW_ACL_CENTRAL_MAX_NUM       2
#define ACL_CONN_MAX_RX_OCTETS          40
#define ACL_CENTRAL_MAX_TX_OCTETS       40

#endif
