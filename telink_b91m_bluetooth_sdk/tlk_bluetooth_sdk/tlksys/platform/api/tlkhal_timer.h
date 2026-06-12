/********************************************************************************************************
 * @file    tlkhal_timer.h
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

typedef struct
{
    uint8_t  chn;
    uint8_t  mode;
    uint16_t resv;
} tlkhal_timer_cfg_t;

/**
 * @brief  Start timer
 * @param[in] cfg : Pointer to timer configuration structure
 * @returns  None.
 */
void tlkhal_timer_start(const tlkhal_timer_cfg_t *cfg);

/**
 * @brief  Stop timer
 * @param[in] cfg : Pointer to timer configuration structure
 * @returns  None.
 */
void tlkhal_timer_stop(const tlkhal_timer_cfg_t *cfg);

/**
 * @brief  Set timer mode
 * @param[in] cfg : Pointer to timer configuration structure
 * @returns  None.
 */
void tlkhal_timer_set_mode(const tlkhal_timer_cfg_t *cfg);