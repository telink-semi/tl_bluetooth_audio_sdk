/********************************************************************************************************
 * @file    tlkmdi_ll_dongle_music.h
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
#ifndef TLKMDI_LL_DONGLE_MUSIC_H_
#define TLKMDI_LL_DONGLE_MUSIC_H_

#define APP_AUDIO_LOG_EN (1)

/**
 * @brief       Switch the music status for a given handle.
 * @param[in]   handle  - The connection handle.
 * @param[in]   status  - The target status to switch to.
 * @return      bool    - Returns true if the switch is successful, false otherwise.
 */
bool tlkmdi_ll_dongle_music_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Check if the music module is busy.
 * @param       None
 * @return      bool    - Returns true if the music module is busy, false otherwise.
 */
bool tlkmdi_ll_dongle_music_is_busy(void);

/**
 * @brief       Initialize the music module.
 * @param       None
 * @return      int     - Returns 0 if initialization is successful, error code otherwise.
 */
int tlkmdi_ll_dongle_music_init(void);

/**
 * @brief       Start the music playback for a given handle with specified parameters.
 * @param[in]   handle  - The connection handle.
 * @param[in]   param   - Additional parameters for music playback.
 * @return      int     - Returns 0 if start is successful, error code otherwise.
 */
int tlkmdi_ll_dongle_music_start(uint16_t handle, uint32_t param);

/**
 * @brief       Close the music playback for a given handle.
 * @param[in]   handle  - The connection handle.
 * @return      int     - Returns 0 if close is successful, error code otherwise.
 */
int tlkmdi_ll_dongle_music_close(uint16_t handle);

/**
 * @brief       Skip to the next music track.
 * @param       None
 * @return      bool    - Returns true if skip is successful, false otherwise.
 */
bool tlkmdi_ll_dongle_music_next(void);

/**
 * @brief       Skip to the previous music track.
 * @param       None
 * @return      bool    - Returns true if skip is successful, false otherwise.
 */
bool tlkmdi_ll_dongle_music_previous(void);

/**
 * @brief       Operate on the music module with an opcode and data.
 * @param[in]   handle  - The connection handle.
 * @param[in]   opcode  - Operation code.
 * @param[in]   pdata   - Pointer to the data buffer.
 * @param[in]   dataLen - Length of the data.
 * @return      bool    - Returns true if operation is successful, false otherwise.
 */
bool tlkmdi_ll_dongle_music_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief       Main loop processing for the music module.
 * @param       None
 * @return      None
 */
void tlkmdi_ll_dongle_mainloop(void);

/**
 * @brief       Get the current state of the microphone.
 * @param       None
 * @return      bool    - Returns true if the microphone is active, false otherwise.
 */
bool tlkmdi_ll_dongle_get_mic_state(void);

/**
 * @brief       Get the current state of the speaker.
 * @param       None
 * @return      bool    - Returns true if the speaker is active, false otherwise.
 */
bool tlkmdi_ll_dongle_get_spk_state(void);

/**
 * @brief       Process audio PPM data from source to destination.
 * @param[in]   ps      - Pointer to the source buffer.
 * @param[out]  pd      - Pointer to the destination buffer.
 * @param[in]   len     - Length of the data to process.
 * @param[in]   channel - Audio channel information.
 * @return      uint16_t- Number of processed bytes.
 */
uint16_t ll_dongle_audio_ppm_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t channel);

#endif
