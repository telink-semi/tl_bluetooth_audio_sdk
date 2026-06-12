/********************************************************************************************************
 * @file    tlkmdi_interphone_linkmgr.h
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
 * @brief   Callback function for interphone music control
 * @param[in]   handle - Connection handle
 * @param[in]   isStart - Start flag
 * @return      None
 */
typedef void (*TlkmdiInterPhoneMusicCtrlCB)(uint16_t handle, uint8_t isStart);

/**
 * @brief       Initialize interphone link manager
 * @param[in]   cb - Callback function
 * @return      None
 */
void tlkmdi_interphone_linkmgr_init(TlkmdiInterPhoneMusicCtrlCB cb);

/**
 * @brief       Get running handle
 * @param       None
 * @return      Running handle
 */
uint16_t tlkmdi_interphone_linkmgr_getRunningHandle(void);

/**
 * @brief       Get first paused handle
 * @param       None
 * @return      First paused handle
 */
uint16_t tlkmdi_interphone_linkmgr_getFirstPausedHandle(void);

/**
 * @brief       Get valid handle
 * @param       None
 * @return      Valid handle
 */
uint16_t tlkmdi_interphone_linkmgr_getValidHandle(void);

/**
 * @brief       Pause interphone link manager
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_linkmgr_pause(void);

/**
 * @brief       Resume interphone link manager
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_linkmgr_resume(void);