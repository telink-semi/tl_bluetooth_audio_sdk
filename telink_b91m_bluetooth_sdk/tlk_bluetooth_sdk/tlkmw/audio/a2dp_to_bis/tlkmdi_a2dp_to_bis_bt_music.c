/********************************************************************************************************
 * @file    tlkmdi_a2dp_to_bis_bt_music.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "drivers.h"
#include "tlkmw/common/tlkaudio_debug_data.h"
#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkmw/sys_dev/tlkmw_sysdev.h"
#include "tlkmdi_a2dp_to_bis_bt_music.h"
#if TLK_CFG_A2DP_TO_BIS_ENABLE
static uint32_t a2dp_dec_tick = 0;
uint32_t dec_intval_us = 0;

/**
 * @brief       Set decoder tick value for A2DP to BIS conversion
 * @param[in]   ticks - Tick value to set
 * @return      None
 */
void a2dp_to_bis_set_dec_ticks(uint32_t ticks)
{
    a2dp_dec_tick = ticks;
}

/**
 * @brief       Main loop for A2DP to BIS Bluetooth music processing
 * @param       None
 * @return      None
 */
audio_ram_code void a2dp_to_bis_bt_main_loop(void)
{
    #if (TLKBTP_CFG_A2DPSNK_ENABLE)
    if (tlkmdi_a2dp_to_bis_is_busy()) {
#if AUDIO_HD_HAC_EN
        uint32_t tdiff_tick = 0;
        if (clock_time() > a2dp_dec_tick) {
            tdiff_tick = clock_time() - a2dp_dec_tick;
        } else {
            tdiff_tick = 0xffffffff - a2dp_dec_tick + clock_time();
        }

        if ((tdiff_tick / SYSTEM_TIMER_TICK_1US) > dec_intval_us) {
            a2dp_dec_tick = clock_time();
            bt_audio_player_task();
        }
#else
        bt_audio_player_task();
#endif
    }
    #endif
}
#endif