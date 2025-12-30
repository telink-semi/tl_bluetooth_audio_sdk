/********************************************************************************************************
 * @file    tlkmdi_interphone_hfmgr.h
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
 * @brief   Callback function for interphone HF control
 * @param[in]   handle - Connection handle
 * @param[in]   isStart - Start flag
 * @param[in]   codec - Codec type
 * @return      None
 */
typedef void (*TlkmdiInterPhoneHfCtrlCB)(uint16_t handle, uint8_t isStart, uint8_t codec);

/**
 * @brief       Initialize interphone HF manager
 * @param[in]   cb - Callback function
 * @return      None
 */
void tlkmdi_interphone_hfmgr_init(TlkmdiInterPhoneHfCtrlCB cb);

/**
 * @brief       Pause interphone HF manager
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_hfmgr_pause(void);

/**
 * @brief       Resume interphone HF manager
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_hfmgr_resume(void);

/**
 * @brief       Get connection handle
 * @param       None
 * @return      Connection handle
 */
uint16_t tlkmdi_interphone_hfmgr_get_handle(void);