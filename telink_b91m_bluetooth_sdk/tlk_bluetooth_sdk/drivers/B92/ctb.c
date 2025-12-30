/********************************************************************************************************
 * @file    ctb.c
 *
 * @brief   This is the source file for B92
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "ctb.h"
#include "gpio.h"
#include "lpc.h"

/**
 * @brief       This function serves to initialize CTB.
 * @param[in]   none
 * @return      none
 */
void ctb_init(void)
{
    ctb_set_interval_sel(CTB_INTERVAL_0MS);
    ctb_set_samp_num_sel(CTB_CYCLE_16);
    ctb_channel_en(CTB_CHANNEL_NUM_8);
    ctb_set_irq_mask(CTB_CHANNEL_NUM_8);
    ctb_set_th_h(2);
    ctb_en();
    ctb_auto_en();
    ctb_pwdn_en();
    ctb_ext_cap_en();
    ctb_rst();
}
