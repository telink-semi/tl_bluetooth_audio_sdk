/********************************************************************************************************
 * @file    tlkdrv_dsp.h
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

/**
 * @brief  initialize dsp module
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_init(void);

/**
 * @brief  pause dsp module
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_pause(void);

/**
 * @brief  resume dsp module
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_resume(void);

/**
 * @brief  dsp boot ok callback function
 * @param[in]  none
 * @returns none
 */
void tlkdrv_dsp_bootOkCB(void);

/**
 * @brief  check if dsp is working
 * @param[in]  none
 * @returns working status, true means working
 */
bool tlkdrv_dsp_isWorking(void);