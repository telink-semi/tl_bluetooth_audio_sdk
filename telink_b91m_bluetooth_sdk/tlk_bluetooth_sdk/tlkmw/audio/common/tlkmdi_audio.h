/********************************************************************************************************
 * @file    tlkmdi_audio.h
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
 * @brief       This function is used to initialize audio module
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audio_init(void);

/**
 * @brief       This function is used to send audio start event
 * @param[in]   audChn - Audio channel
 * @param[in]   handle - Connection handle
 * @return      TLK_ENONE indicates success; other value indicates failure
 */
int tlkmdi_audio_sendStartEvt(uint8_t audChn, uint16_t handle);

/**
 * @brief       This function is used to send audio start event with priority
 * @param[in]   audChn - Audio channel
 * @param[in]   handle - Connection handle
 * @param[in]   priority - Priority level
 * @return      TLK_ENONE indicates success; other value indicates failure
 */
int tlkmdi_audio_sendStartEvtEx(uint8_t audChn, uint16_t handle, uint8_t priority);

/**
 * @brief       This function is used to send audio close event
 * @param[in]   audChn - Audio channel
 * @param[in]   handle - Connection handle
 * @return      TLK_ENONE indicates success; other value indicates failure
 */
int tlkmdi_audio_sendCloseEvt(uint8_t audChn, uint16_t handle);

/**
 * @brief       This function is used to send audio close event with delete option
 * @param[in]   audChn - Audio channel
 * @param[in]   handle - Connection handle
 * @param[in]   isDelete - Delete flag
 * @return      TLK_ENONE indicates success; other value indicates failure
 */
int tlkmdi_audio_sendCloseEvtEx(uint8_t audChn, uint16_t handle, uint8_t isDelete);

/**
 * @brief       This function is used to send debug message output event
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audio_sendDebugMsgOutput(void);

/**
 * @brief       This function is used to send TX power change event
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audio_sendChangeTxPower(void);
