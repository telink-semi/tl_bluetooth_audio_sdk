/********************************************************************************************************
 * @file    audio_control.h
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
#ifndef __AUDIO_CONTROL_H__
#define __AUDIO_CONTROL_H__

#include "tlkmw/audio/tlkmw_audio.h"
#include "stack/stack.h"
#include "drivers.h"

extern uint32_t g_tick_bt_window_us;
extern uint32_t g_tick_bt_window;

/**
 * @brief       Set asynchronous TWS status flag
 * @param[in]   flag - Flag to set
 * @param[in]   en - Enable or disable the flag
 * @return      None
 */
void async_tws_set_status_flag(uint8_t flag, int en);

/**
 * @brief       Get asynchronous TWS status flag
 * @param[in]   flag - Flag to check
 * @return      Flag status
 */
int async_tws_get_status_flag(unsigned char flag);

/**
 * @brief       Set tone mixing mode
 * @param[in]   enable - Enable or disable tone mixing
 * @return      None
 */
void audio_ctl_set_tone_mix_mode(uint8_t enable);

/**
 * @brief       Get tone mixing mode
 * @return      Current tone mixing mode
 */
uint8_t audio_ctl_get_tone_mix_mode(void);

#endif
