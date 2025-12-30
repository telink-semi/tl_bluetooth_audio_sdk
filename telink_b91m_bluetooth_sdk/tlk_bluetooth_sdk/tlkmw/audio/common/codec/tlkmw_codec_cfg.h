/********************************************************************************************************
 * @file    tlkmw_codec_cfg.h
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

/* temp use this test codec suspend,wait all ok */
/* ziyu codec temp code*/
#ifndef TLKMW_CODEC_NEW_VERSION
#define TLKMW_CODEC_NEW_VERSION 1
#endif

#ifndef TLKMW_CODEC_CFG_SPEAKER_EN
#define TLKMW_CODEC_CFG_SPEAKER_EN 1
#endif

#ifndef TLKMW_CODEC_CFG_MIC_EN
#define TLKMW_CODEC_CFG_MIC_EN 1
#endif

#ifndef TLKMW_CODEC_CFG_DYNAMIC_POWER_ON 
#define TLKMW_CODEC_CFG_DYNAMIC_POWER_ON (1 && TLK_CFG_SUSPEND_ENABLE)
#endif

#ifndef TLKMW_CODEC_CFG_FEATURE_FADE_IN
#define TLKMW_CODEC_CFG_FEATURE_FADE_IN 1
#endif

#ifndef TLKMW_CODEC_CFG_FEATURE_FADE_OUT
#define TLKMW_CODEC_CFG_FEATURE_FADE_OUT 1
#endif