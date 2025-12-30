/********************************************************************************************************
 * @file    tpsll_audio.h
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
#ifndef DRIVERS_TL322X_TPSLL_EXT_DRIVER_TPSLL_AUDIO_H_
#define DRIVERS_TL322X_TPSLL_EXT_DRIVER_TPSLL_AUDIO_H_

typedef enum
{
    AUDIO_RATE_EQUAL,
    AUDIO_RATE_GT_L0,
    AUDIO_RATE_GT_L1,
    AUDIO_RATE_LT_L0,
    AUDIO_RATE_LT_L1,
    AUDIO_MATCH_SIZE,
} audio_rate_match_e;

/////////////////////////////////////////////////////////////////////////
#define EXTERNAL_CODEC_ENABLE 0
#if EXTERNAL_CODEC_ENABLE
    /* sampling rate = 192M * (76 / 235) / (2 * 11) / 64 = 44.1K */
    /* 44.100580270793 */
    /* (1000 / 44.100580270793) * 24 * 57 = 31,020.00000000003 */
    #define TICK_OF_BLOCK_SAMPLE_44k     31020
    #define NUM_OF_SAMPLES_PER_BLOCK_44K 57
#else
    #define TICK_OF_BLOCK_SAMPLE_44k     26122
    #define NUM_OF_SAMPLES_PER_BLOCK_44K 48
#endif

#define TICK_OF_BLOCK_SAMPLE_48k     24000
#define NUM_OF_SAMPLES_PER_BLOCK_48K 48

#define TICK_OF_BLOCK_SAMPLE_96k     12000
#define NUM_OF_SAMPLES_PER_BLOCK_96K 48


#define audio_codec_adc_enable(t)

#endif /* DRIVERS_TL322X_TPSLL_EXT_DRIVER_TPSLL_AUDIO_H_ */
