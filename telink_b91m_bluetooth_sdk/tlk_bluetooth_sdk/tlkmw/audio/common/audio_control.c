/********************************************************************************************************
 * @file    audio_control.c
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
#include "tl_common.h"
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/audio/tlkmw_audio.h"
#include "stack/bt/host/btp/a2dp/btp_a2dp.h"
#include "tlkmw/sys_dev/tlkmw_sysdev.h"

uint8_t g_tone_mix_enable = 0;

int tsync_mode = 0;

/**
 * @brief       Set asynchronous TWS status flag
 * @param[in]   flag - Flag to set
 * @param[in]   en - Enable or disable the flag
 * @return      None
 */
_attribute_ram_code_sec_noinline_ void async_tws_set_status_flag(uint8_t flag, int en)
{
    AUDIO_GLOBAL_INT_DISABLE();
    if (en) {
        tsync_mode |= flag;
    } else {
        tsync_mode &= ~flag;
    }
    AUDIO_GLOBAL_INT_RESTORE();
}

/**
 * @brief       Get asynchronous TWS status flag
 * @param[in]   flag - Flag to check
 * @return      Flag status
 */
_attribute_ram_code_sec_noinline_ int async_tws_get_status_flag(unsigned char flag)
{
    AUDIO_GLOBAL_INT_DISABLE();

    int ret = (tsync_mode & flag);

    AUDIO_GLOBAL_INT_RESTORE();

    return ret;
}

/**
 * @brief       Set tone mixing mode
 * @param[in]   enable - Enable or disable tone mixing
 * @return      None
 */
void audio_ctl_set_tone_mix_mode(uint8_t enable)
{
    g_tone_mix_enable = enable;
}

/**
 * @brief       Get tone mixing mode
 * @return      Current tone mixing mode
 */
uint8_t audio_ctl_get_tone_mix_mode(void)
{
    return g_tone_mix_enable;
}

#if (TLKBTP_CFG_HFP_ENABLE && TLKBTP_CFG_A2DPSNK_ENABLE)
uint32_t g_tick_bt_window_us = 0;
uint32_t g_tick_bt_window    = 0;

uint32_t g_delay_sw_le_tick = 0;
uint32_t g_delay_sw_bt_tick = 0;

#endif
