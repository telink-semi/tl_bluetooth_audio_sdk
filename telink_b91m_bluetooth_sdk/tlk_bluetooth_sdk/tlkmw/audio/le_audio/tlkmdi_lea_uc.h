/********************************************************************************************************
 * @file    tlkmdi_lea_uc.h
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
#ifndef TLKMDI_LEA_UC_H
#define TLKMDI_LEA_UC_H


/**
 * @brief       Initialize LE Audio unicast client control and callbacks.
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_uc_init(void);

/**
 * @brief       Disconnect helper (implementation TBD).
 */
void tlkmdi_lea_uc_disconnect(uint16_t handle, uint16_t acl_conn_handle);

/**
 * @brief       Send music play command to peer/USB stack.
 * @param[in]   handle  - LE Audio virtual handle.
 * @param[in]   param   - optional parameter (unused).
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_uc_music_start(uint16_t handle, uint32_t param);

/**
 * @brief       Send music pause command to peer/USB stack.
 * @param[in]   handle  - LE Audio virtual handle.
 * @return      TLK_ENONE on success.
 */
int tlkmdi_lea_uc_music_close(uint16_t handle);

/**
 * @brief       Request next track via USB controller.
 * @return      true always.
 */
bool tlkmdi_lea_uc_music_next(void);

/**
 * @brief       Request previous track via USB controller.
 * @return      true always.
 */
bool tlkmdi_lea_uc_music_previous(void);

/**
 * @brief       Execute operation for music session.
 * @param[in]   handle  - LE Audio virtual handle.
 * @param[in]   opcode  - TLKAUD opcode (volume, call, etc.).
 * @param[in]   pdata   - optional payload.
 * @param[in]   dataLen - payload length.
 * @return      true if opcode handled, false otherwise.
 */
bool tlkmdi_lea_uc_music_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief       Open/close music path depending on TLK_STATE command.
 * @param[in]   handle  - LE Audio virtual handle.
 * @param[in]   status  - TLK_STATE_* value.
 * @return      true if state transition accepted.
 */
bool tlkmdi_lea_uc_music_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Report whether music pipeline is active or pending stop.
 * @return      true when busy, false otherwise.
 */
bool tlkmdi_lea_uc_music_is_busy(void);

/**
 * @brief       Send voice start command (implementation TBD).
 */
int tlkmdi_lea_uc_voice_start(uint16_t handle, uint32_t param);

/**
 * @brief       Send voice close command (implementation TBD).
 */
int tlkmdi_lea_uc_voice_close(uint16_t handle);

/**
 * @brief       Skip to next voice preset (implementation TBD).
 */
bool tlkmdi_lea_uc_voice_next(void);

/**
 * @brief       Skip to previous voice preset (implementation TBD).
 */
bool tlkmdi_lea_uc_voice_previous(void);

/**
 * @brief       Execute operation for voice session.
 * @param[in]   handle  - LE Audio virtual handle.
 * @param[in]   opcode  - TLKAUD opcode.
 * @param[in]   pdata   - optional payload.
 * @param[in]   dataLen - payload length.
 * @return      true if opcode handled, false otherwise.
 */
bool tlkmdi_lea_uc_voice_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief       Open/close voice path depending on TLK_STATE command.
 * @param[in]   handle  - LE Audio virtual handle.
 * @param[in]   status  - TLK_STATE_* value.
 * @return      true if state transition accepted.
 */
bool tlkmdi_lea_uc_voice_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Report whether voice pipeline is active or pending stop.
 * @return      true when busy, false otherwise.
 */
bool tlkmdi_lea_uc_voice_is_busy(void);

struct prefer_codec_cfg
{
    uint8_t music_codec_select;
    uint8_t music_codec_number;
    uint8_t voice_codec_select;
    uint8_t voice_codec_number;

    struct
    {
        const struct ble_ase_codec_config *music_config;
    } music_codec_cfg[2];

    struct
    {
        const struct ble_ase_codec_config *music_config;
        const struct ble_ase_codec_config *voice_config;
    } voice_codec_cfg[2];
};

/**
 * @brief       Cache preferred codec list used when creating streams.
 * @param[in]   cfg - pointer to preference structure.
 * @return      TLK_ENONE.
 */
int tlkmdi_lea_uc_set_prefer_codec_cfg(struct prefer_codec_cfg *cfg);
#endif
