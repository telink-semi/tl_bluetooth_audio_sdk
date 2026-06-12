/********************************************************************************************************
 * @file    tlkmdi_audu2h_voice.h
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
#ifndef TLKMDI_U2H_VOICE_H
#define TLKMDI_U2H_VOICE_H

#define TLKMW_U2H_VOICE_WAIT_SCO_TIMEOUT 8000000 //s

typedef struct
{
    uint8_t  enable;
    uint8_t  waitFlag;
    uint16_t handle;
    uint32_t waitTimer; //wait sco timer
} tlkmdi_audu2h_voice_ctrl_t;

typedef enum
{
    TLKMDI_AUDU2H_VOICE_WAIT_FLAG_NONE = 0,
    TLKMDI_AUDU2H_VOICE_WAIT_SCO_ESTB  = 0x01, //wait esco establish event
} TLKMDI_AUDU2H_VOICE_WAIT_FLAG_ENUM;

/**
 * @brief       Starts the U2H voice processing for a given connection handle.
 * @param[in]   handle  - The connection handle.
 * @param[in]   param   - Additional parameters for voice start.
 * @return      Returns 0 if successful, otherwise returns an error code.
 */
int tlkmdi_audu2h_voice_start(uint16_t handle, uint32_t param);

/**
 * @brief       Initializes the U2H voice module.
 * @param       None
 * @return      Returns 0 if successful, otherwise returns an error code.
 */
int tlkmdi_audu2h_voice_init(void);

/**
 * @brief       Timer function to handle periodic tasks for U2H voice.
 * @param       None
 * @return      None
 */
void tlkmdi_audu2h_voice_timer(void);

/**
 * @brief       Switches the voice state based on the provided status.
 * @param[in]   handle  - The connection handle.
 * @param[in]   status  - The new status to set.
 * @return      Returns true if the switch is successful, false otherwise.
 */
bool tlkmdi_audu2h_voice_switch(uint16_t handle, uint8_t status);

/**
 * @brief       Checks if the U2H voice module is busy.
 * @param       None
 * @return      Returns true if busy, false otherwise.
 */
bool tlkmdi_audu2h_voice_isBusy(void);

/**
 * @brief       Operates on the U2H voice module with the given opcode and data.
 * @param[in]   opcode  - The operation code.
 * @param[in]   pdata   - Pointer to the data buffer.
 * @param[in]   dataLen - Length of the data buffer.
 * @return      Returns true if the operation was successful, false otherwise.
 */
bool tlkmdi_audu2h_voice_operate(uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief       Retrieves PCM data from the U2H voice module.
 * @param[out]  pBuffer - Buffer to store the retrieved PCM data.
 * @param[in]   dataLen - Length of the buffer.
 * @return      Returns 0 if successful, otherwise returns an error code.
 */
int tlkmdi_audu2h_voice_getPcmData(uint8_t *pBuffer, uint16_t dataLen);

/**
 * @brief       Disables the U2H voice functionality.
 * @param       None
 * @return      None
 */
void tlkmdi_u2h_voice_disable(void);
#endif //TLKMDI_U2H_VOICE_H
