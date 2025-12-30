/********************************************************************************************************
 * @file    tlkmdi_lea_us.h
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
#ifndef TLKMDI_LEA_UNICAST_SERVER_H
#define TLKMDI_LEA_UNICAST_SERVER_H

/**
 * @brief       Get the BLE connection handle currently driving playback.
 * @return      Active connection handle or 0xFFFF if idle.
 */
uint16_t tlkmdi_lea_us_get_active_conn_handle(void);

/**
 * @brief       Initialize LE Audio unicast server states and callbacks.
 * @return      TLK_ENONE on success.
 */
int  tlkmdi_lea_us_init(void);

/**
 * @brief       Query whether music pipeline is currently active.
 * @return      true when active, false otherwise.
 */
bool tlkmdi_lea_us_music_is_busy(void);

/**
 * @brief       Issue music play command to remote server.
 * @param[in]   handle  - BLE ACL handle.
 * @param[in]   param   - optional (unused).
 * @return      TLK_ENONE on success.
 */
int  tlkmdi_lea_us_music_start(uint16_t handle, uint32_t param);

/**
 * @brief       Issue music pause command to remote server.
 * @param[in]   handle  - BLE ACL handle.
 * @return      TLK_ENONE on success.
 */
int  tlkmdi_lea_us_music_close(uint16_t handle);

/**
 * @brief       Send next-track command through GMCS.
 * @return      true on success.
 */
bool tlkmdi_lea_us_music_next(void);

/**
 * @brief       Send previous-track command through GMCS.
 * @return      true on success.
 */
bool tlkmdi_lea_us_music_previous(void);

/**
 * @brief       Execute opcode for music session (volume, call control).
 * @param[in]   handle  - BLE ACL handle.
 * @param[in]   opcode  - TLKAUD opcode.
 * @param[in]   pdata   - optional payload.
 * @param[in]   dataLen - payload length.
 * @return      true if opcode handled, false otherwise.
 */
bool tlkmdi_lea_us_music_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief       Start/stop music audio path for given handle.
 * @param[in]   handle  - BLE ACL handle.
 * @param[in]   status  - TLK_STATE_* command.
 * @return      true if transition accepted.
 */
bool tlkmdi_lea_us_music_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Query whether voice pipeline is currently active.
 * @return      true when active, false otherwise.
 */
bool tlkmdi_lea_us_voice_is_busy(void);

/**
 * @brief       Execute opcode for voice session.
 * @param[in]   handle  - BLE ACL handle.
 * @param[in]   opcode  - TLKAUD opcode.
 * @param[in]   pData   - optional payload.
 * @param[in]   dataLen - payload length.
 * @return      true if opcode handled, false otherwise.
 */
bool tlkmdi_lea_us_voice_operate(uint16_t handle, uint8_t opcode, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       Start/stop voice audio path for given handle.
 * @param[in]   handle  - BLE ACL handle.
 * @param[in]   status  - TLK_STATE_* command.
 * @return      true if transition accepted.
 */
bool tlkmdi_lea_us_voice_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Check if any LE Audio unicast server session is active.
 * @return      true when active, false otherwise.
 */
bool tlkmdi_lea_us_is_busy(void);

#endif
