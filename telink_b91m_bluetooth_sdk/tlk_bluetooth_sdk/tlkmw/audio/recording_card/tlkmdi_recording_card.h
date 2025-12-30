/********************************************************************************************************
 * @file    tlkmdi_recording_card.h
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
#ifndef _TLKMDI_RECORDING_CARD_H_
#define _TLKMDI_RECORDING_CARD_H_
typedef struct
{
    bool    busy;
    bool    first_frame;
    bool	nn_alg_en;
    unsigned char gain;
} tlkmdi_recording_card_t;

/**
 * @brief       This function is used to initialize the recording card module.
 * @param       none.
 * @return      TLK_ENONE if success, otherwise error code.
 */
int tlkmdi_recording_card_init(void);

/**
 * @brief       This function checks if the recording card module is busy.
 * @param       none.
 * @return      true if busy, false otherwise.
 */
bool tlkmdi_recording_card_is_busy(void);

/**
 * @brief       This function switches the recording card status.
 * @param[in]   handle - the connection handle.
 * @param[in]   status - the switch status.
 * @return      true if successful, false otherwise.
 */
bool tlkmdi_recording_card_switch(uint16_t handle, uint8_t status);

/**
 * @brief       This function enables or disables the recording card algorithm.
 * @param[in]   en - enable flag.
 * @return      none.
 */
void tlkmdi_recording_card_alg_en(bool en);

/**
 * @brief       This function sets the gain for the recording card.
 * @param[in]   gain - the gain value to set.
 * @return      none.
 */
void tlkmdi_recording_card_gain_set(unsigned char gain);

#endif