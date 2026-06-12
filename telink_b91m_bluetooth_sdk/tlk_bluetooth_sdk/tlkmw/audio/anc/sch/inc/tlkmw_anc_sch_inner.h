/********************************************************************************************************
 * @file    tlkmw_anc_sch_inner.h
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

#include "tlkmw/audio/anc/sch/tlkmw_anc_sch.h"

/**
 * @brief       Get the running handle of BT music
 * @param       None
 * @return      Running handle
 */
uint16_t tlkmw_anc_bt_music_getRunningHandle(void);

/**
 * @brief       Get the first paused handle of BT music
 * @param       None
 * @return      First paused handle
 */
uint16_t tlkmw_anc_bt_music_getFirstPausedHandle(void);

/**
 * @brief       Initialize BT music scheduler
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_bt_music_init(void);

/**
 * @brief       Get the running handle of BT voice
 * @param       None
 * @return      Running handle
 */
uint16_t tlkmw_anc_bt_voice_getRunningHandle(void);

/**
 * @brief       Initialize BT voice scheduler
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_bt_voice_init(void);

/**
 * @brief       Get the running handle of TPSLL
 * @param       None
 * @return      Running handle
 */
uint16_t tlkmw_anc_tpsll_getRunningHandle(void);

/**
 * @brief       Initialize TPSLL scheduler
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_tpsll_init(void);

/**
 * @brief       Request ANC scheduler update
 * @param[in]   type - Type of update
 * @param[in]   handle - Handle value
 * @param[in]   isStart - Start flag
 * @return      None
 */
void tlkmw_anc_sch_request_update(uint8_t type, uint16_t handle, uint8_t isStart);

/**
 * @brief       Accept UI call request
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_call_accept(void);

/**
 * @brief       Reject UI call request
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_call_reject(void);