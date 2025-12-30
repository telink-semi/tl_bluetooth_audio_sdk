/********************************************************************************************************
 * @file    tlkapp_audioModinf.h
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
#ifndef TLKAPP_AUDIO_MODINF_H
#define TLKAPP_AUDIO_MODINF_H

typedef struct
{
    int (*Init)(void);
    bool (*Switch)(uint16_t handle, uint8_t status);
    void (*Timer)(void);
    bool (*IsBusy)(void);

    int (*Start)(uint16_t handle, uint32_t param);
    int (*Close)(uint16_t handle);
    bool (*ToNext)(void);
    bool (*ToPrev)(void);
    bool (*operate)(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);
} tlkapp_audio_modinf_t;

int tlkapp_audio_modinfNodeInit(void);

/**
 * @brief       This function switches the audio module state.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @param[in]   handle  - the connection handle of the current audio initiator.
 * @param[in]   status  - the state of the audio control.
 *                      TLK_STATE_OPENED -- The audio is on.
 *                      TLK_STATE_CLOSED -- The audio is off.
 *                      TLK_STATE_PAUSED -- The audio is paused.
 * @return      Operating results. True means success, others means failure.
 */
bool tlkapp_audio_modinfSwitch(TLKAUD_TYPE_ENUM optype, uint16_t handle, uint8_t status);

/**
 * @brief       This function handles the timing callback for the audio module.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @return      none.
 */
void tlkapp_audio_modinfTimer(TLKAUD_TYPE_ENUM optype);

/**
 * @brief       This function checks if the audio module is busy.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @return      Operating results. True means audio is busy, others means audio is idle.
 */
bool tlkapp_audio_modinfIsBusy(TLKAUD_TYPE_ENUM optype);

/**
 * @brief       This function starts the audio module.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @param[in]   handle  - the connection handle of the current audio initiator.
 * @param[in]   param   - control parameters of the current audio initiator.
 * @return      Operating results. TLK_ENONE means success, others means failure.
 */
int tlkapp_audio_modinfStart(TLKAUD_TYPE_ENUM optype, uint16_t handle, uint32_t param);

/**
 * @brief       This function closes the audio module.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @param[in]   handle  - the connection handle of the current audio initiator.
 * @return      Operating results. TLK_ENONE means success, others means failure.
 */
int tlkapp_audio_modinfClose(TLKAUD_TYPE_ENUM optype, uint16_t handle);

/**
 * @brief       This function plays the next track.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @return      Operating results. true means success, others means failure.
 */
bool tlkapp_audio_modinfToNext(TLKAUD_TYPE_ENUM optype);

/**
 * @brief       This function plays the previous track.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @return      Operating results. True means success, others means failure.
 */
bool tlkapp_audio_modinfToPrev(TLKAUD_TYPE_ENUM optype);

/**
 * @brief       This function operates on the audio module with specific data.
 * @param[in]   handle  - the connection handle of the current audio initiator.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      true if success, false if failure.
 */
bool tlkapp_audio_modinfOperate(uint16_t handle, TLKAUD_TYPE_ENUM optype, uint8_t *pData, uint16_t dataLen);


#endif // TLKAPP_AUDIO_MODINF_H
