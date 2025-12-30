/********************************************************************************************************
 * @file    tlkaudio_debug_data.h
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
#ifndef TLKMW_AUDIO_DEBUG_DATA_H_
#define TLKMW_AUDIO_DEBUG_DATA_H_

#include "tl_common.h"

#if(MCU_CORE_TL752X_TEMP)
extern signed int   sin_48k_stereo_24bit[96] __attribute__((aligned(4)));
#endif

    #if (TLK_DBG_AUDIO_ENABLE)
extern signed short sin_48k_stereo[] __attribute__((aligned(4)));
extern signed short sin_48k_mono_d1[] __attribute__((aligned(4)));
extern signed short sin_48k_mono_d2[] __attribute__((aligned(4)));
extern signed int   sin_48k_mono_24bit_d1[] __attribute__((aligned(4)));
extern signed int   sin_48k_mono_24bit_d2[] __attribute__((aligned(4)));
extern signed int   sin_48k_stereo_24bit[] __attribute__((aligned(4)));
extern signed short sin_16k_mono[16];
extern signed short sin_16k_stereo[32];
extern int          array_count;

    #endif

/**
 * @brief       This function is used to get sine wave data for codec debugging
 * @param[out]  p_des - the destination buffer to store the sine wave data
 * @param[in]   sample_num - the number of samples to generate
 * @return      1 if success, 0 if failure.
 */
extern uint8_t tlkdrv_codec_get_sin_data(int16_t *p_des, uint16_t sample_num);

/**
 * @brief       This function is used to get sine wave data at 44.1KHz sample rate for codec debugging
 * @param[out]  p_des - the destination buffer to store the sine wave data
 * @param[in]   sample_num - the number of samples to generate
 * @return      1 if success, 0 if failure.
 */
extern uint8_t tlkdrv_codec_get_sin_44p1K_data(int32_t *p_des, uint16_t sample_num);

/**
 * @brief       This function is used to get stereo sine wave data at 44.1KHz sample rate for codec debugging
 * @param[out]  p_des - the destination buffer to store the sine wave data
 * @param[in]   sample_num - the number of samples to generate
 * @return      1 if success, 0 if failure.
 */
extern uint8_t tlkdrv_codec_get_sin_44p1K_stereo_data(int32_t *p_des, uint16_t sample_num);

#endif /* TLKMW_AUDIO_DEBUG_DATA_H_ */
