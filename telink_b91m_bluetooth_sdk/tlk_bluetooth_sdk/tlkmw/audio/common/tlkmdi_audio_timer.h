/********************************************************************************************************
 * @file    tlkmdi_audio_timer.h
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
 * @brief       This function is used to start audio timer
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audio_start_timer(void);

/**
 * @brief       This function is used to stop audio timer
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audio_stop_timer(void);

/**
 * @brief       This function is used to set audio timer capture tick
 * @param[in]   cap_tick - Capture tick value
 * @return      None.
 */
void tlkmdi_audio_set_timer(uint32_t cap_tick);

/**
 * @brief       This function is used to set next interrupt for audio task
 * @param[in]   tus - Time value in microseconds
 * @return      None.
 */
void tlkmdi_audio_task_set_next_irq(uint32_t tus);

/**
 * @brief       This function is used to setup and start audio timer
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audio_setup_and_start_timer(void);