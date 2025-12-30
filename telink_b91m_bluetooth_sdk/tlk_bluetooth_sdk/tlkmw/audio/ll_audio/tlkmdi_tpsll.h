/********************************************************************************************************
 * @file    tlkmdi_tpsll.h
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
#ifndef TLKMDI_TPSLL_H
#define TLKMDI_TPSLL_H

/**
 * @brief      Initializes TPSLL audio module
 * @param      None
 * @return     Status of initialization
 */
int     tlkmdi_tpsll_audio_init(void);

/**
 * @brief      Enables TPSLL audio functionality
 * @param      None
 * @return     Status of enabling operation
 */
int     tlkmdi_tpsll_audio_enable(void);

/**
 * @brief      Disables TPSLL audio functionality
 * @param      None
 * @return     Status of disabling operation
 */
int     tlkmdi_tpsll_audio_disable(void);

/**
 * @brief      Switches TPSLL audio connection
 * @param[in]  handle - Connection handle
 * @param[in]  status - Switch status
 * @return     True if switch successful, false otherwise
 */
bool    tlkmdi_tpsll_audio_switch(uint16_t handle, uint8_t status);

/**
 * @brief      Checks if TPSLL audio is busy
 * @param      None
 * @return     True if busy, false otherwise
 */
bool    tlkmdi_tpsll_audio_is_busy(void);

/**
 * @brief      Starts TPSLL audio stream
 * @param[in]  handle - Connection handle
 * @param[in]  param - Start parameters
 * @return     Status of start operation
 */
int     tlkmdi_tpsll_audio_start(uint16_t handle, uint32_t param);

/**
 * @brief      Closes TPSLL audio stream
 * @param[in]  handle - Connection handle
 * @return     Status of close operation
 */
int     tlkmdi_tpsll_audio_close(uint16_t handle);

/**
 * @brief      Moves to next TPSLL audio item
 * @param      None
 * @return     True if next item exists, false otherwise
 */
bool    tlkmdi_tpsll_audio_next(void);

/**
 * @brief      Moves to previous TPSLL audio item
 * @param      None
 * @return     True if previous item exists, false otherwise
 */
bool    tlkmdi_tpsll_audio_previous(void);

/**
 * @brief      Processes TPSLL audio operations
 * @param[in]  handle - Connection handle
 * @param[in]  opcode - Operation code
 * @param[in]  pdata - Data buffer
 * @param[in]  dataLen - Length of data
 * @return     True if operation processed successfully, false otherwise
 */
bool    tlkmdi_tpsll_audio_process_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

#endif // TLKMDI_TPSLL_H
