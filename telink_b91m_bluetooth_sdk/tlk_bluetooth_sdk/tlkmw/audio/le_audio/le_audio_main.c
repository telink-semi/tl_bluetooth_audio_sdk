/********************************************************************************************************
 * @file    le_audio_main.c
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
#include "common/types.h"

#include "tlkmw/tlkmw.h"

#include "le_audio_common.h"

#if (TLK_MW_LE_AUDIO_ENABLE)


/**
 * @brief       Execute LE Audio input processing and rearm main loop timer.
 * @return      none.
 */
void le_audio_main_loop(void)
{
    lea_input_main_process();
    tlkmdi_audio_runMainAfterTimeMs(3);
}

/**
 * @brief       Timer callback that stops current timer and processes output PCM.
 * @return      none.
 */
void le_audio_timer_irq(void)
{
    tlkmdi_audio_stop_timer();
    lea_output_timer_irq();
}


#endif //#if (TLK_MW_AUDIO_ENABLE)
