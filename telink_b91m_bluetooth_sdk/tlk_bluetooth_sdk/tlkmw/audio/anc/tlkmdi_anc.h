/********************************************************************************************************
 * @file    tlkmdi_anc.h
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
    bool    bt_music_busy;
    bool    bt_voice_busy;
    bool    ll_audio_busy;
} tlkmdi_anc_t;

/**
 * @brief       Initialize ANC module
 * @param       None
 * @return      0 if success, otherwise error code
 */
int tlkmdi_anc_init(void);

/**
 * @brief       Process next operation for ANC
 * @param       None
 * @return      true if success, false otherwise
 */
bool tlkmdi_anc_next(void);

/**
 * @brief       Process previous operation for ANC
 * @param       None
 * @return      true if success, false otherwise
 */
bool tlkmdi_anc_previous(void);

/**
 * @brief       Operate ANC with specific command
 * @param[in]   handle - Handle value
 * @param[in]   opcode - Operation code
 * @param[in]   pdata - Pointer to data
 * @param[in]   dataLen - Length of data
 * @return      true if success, false otherwise
 */
bool tlkmdi_anc_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief       Check if ANC is busy
 * @param       None
 * @return      true if busy, false otherwise
 */
bool tlkmdi_anc_is_busy(void);

/**
 * @brief       Switch ANC state
 * @param[in]   handle - Handle value
 * @param[in]   status - Status value
 * @return      true if success, false otherwise
 */
bool tlkmdi_anc_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Check if BT music is busy
 * @param       None
 * @return      true if busy, false otherwise
 */
bool tlkmdi_anc_btmusic_is_busy(void);

/**
 * @brief       Check if BT voice is busy
 * @param       None
 * @return      true if busy, false otherwise
 */
bool tlkmdi_anc_btvoice_is_busy(void);

/**
 * @brief       Check if LL audio is busy
 * @param       None
 * @return      true if busy, false otherwise
 */
bool tlkmdi_anc_ll_audio_is_busy(void);

//this function is not thread safe ,only can be call in audio thread,use msg queue.
/**
 * @brief       Enable or disable ANC function
 * @param[in]   enable - Enable flag (1 to enable, 0 to disable)
 * @return      None
 */
void tlkmw_anc_function_en(uint8_t enable);

/**
 * @brief       Check if ANC is enabled
 * @param       None
 * @return      1 if enabled, 0 if disabled
 */
uint8_t tlkmw_anc_is_enable(void);
