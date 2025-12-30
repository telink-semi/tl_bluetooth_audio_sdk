/********************************************************************************************************
 * @file    tlkmdi_bt_music.h
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
#ifndef TLKMDI_BT_MUSIC_H
#define TLKMDI_BT_MUSIC_H

/**
 * @brief  check if bt music is enabled
 * @param[in]  none
 * @returns bt music enable status
 */
uint16_t tlkmdi_bt_music_is_enabled(void);

/**
 * @brief  initial the A2DP sink block, register the data callback
 * @param[in]  none
 * @returns TLK_ENONE is success,other value is false
 */
int     tlkmdi_bt_music_init(void);

/**
 * @brief  start bt music
 * @param[in]  handle - connection handle
 * @param[in]  param  - start parameter
 * @returns TLK_ENONE is success,other value is false
 */
int     tlkmdi_bt_music_start(uint16_t handle, uint32_t param);

/**
 * @brief  close bt music
 * @param[in]  handle - connection handle
 * @returns TLK_ENONE is success,other value is false
 */
int     tlkmdi_bt_music_close(uint16_t handle);

/**
 * @brief  play next song
 * @param[in]  none
 * @returns TLK_ENONE is success,other value is false
 */
bool    tlkmdi_bt_music_next(void);

/**
 * @brief  play previous song
 * @param[in]  none
 * @returns TLK_ENONE is success,other value is false
 */
bool    tlkmdi_bt_music_previous(void);

/**
 * @brief  switch bt music state
 * @param[in]  handle - connection handle
 * @param[in]  status - switch status
 * @returns operation result
 */
bool    tlkmdi_bt_music_switch(uint16_t handle, uint8_t status);

/**
 * @brief  check if bt music is busy
 * @param[in]  none
 * @returns busy status
 */
bool    tlkmdi_bt_music_is_busy(void);

/**
 * @brief  operate bt music
 * @param[in]  handle  - connection handle
 * @param[in]  opcode  - operation code
 * @param[in]  pdata   - operation data
 * @param[in]  dataLen - data length
 * @returns operation result
 */
bool    tlkmdi_bt_music_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief  get avrcp volume function
 * @param[in]  step - volume step
 * @returns volume value
 */
uint8_t bt_audio_get_avrcp_vol_func(uint8_t step);

#if TLKALG_ASRC_441TO48_16BIT_TEMP_ENABLE
/**
 * @brief  initialize 441 to 48 conversion
 * @param[in]  none
 * @returns none
 */
void bt_music_441to48_init(void);

/**
 * @brief  deinitialize 441 to 48 conversion
 * @param[in]  none
 * @returns none
 */
void bt_music_441to48_deinit(void);
#endif

#endif // TLKMDI_BT_MUSIC_H
