/********************************************************************************************************
 * @file    tlkmw_anc_sch_weak.c
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

/**
 * @brief       Enable or disable ANC function
 * @param[in]   enable - Enable flag (1 to enable, 0 to disable)
 * @return      None
 */
__attribute__((weak)) void tlkmw_anc_function_en(uint8_t enable)
{
    (void)enable;
}

/**
 * @brief       Check if ANC is enabled
 * @param       None
 * @return      1 if enabled, 0 if disabled
 */
__attribute__((weak)) uint8_t tlkmw_anc_is_enable(void)
{
    return 0;
}