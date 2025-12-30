/********************************************************************************************************
 * @file    app_src_codec.h
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

#define TLKBTP_CFG_RFC_ENABLE     (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_HFP_ENABLE     (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_HFPAG_ENABLE   (1 && TLKBTP_CFG_HFP_ENABLE)

#define TLK_DEV_CODEC_ENABLE       1
#define TLKDRV_CODEC_ICODEC_ENABLE (1 & TLK_DEV_CODEC_ENABLE)


#define LE_AUDIO_CODEC_INPUT_TYPE  LE_AUDIO_CODEC_TYPE_CODEC
#define LE_AUDIO_CODEC_OUTPUT_TYPE LE_AUDIO_CODEC_TYPE_CODEC
