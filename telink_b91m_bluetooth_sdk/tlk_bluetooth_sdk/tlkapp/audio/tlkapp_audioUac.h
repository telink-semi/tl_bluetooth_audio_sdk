/********************************************************************************************************
 * @file    tlkapp_audioUac.h
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
#ifndef TLKAPP_AUDIO_UAC_H
#define TLKAPP_AUDIO_UAC_H
#include "common/types.h"

enum
{
    TLKAPP_AUDIO_UAC_STATE_IDLE = 0,
    TLKAPP_AUDIO_UAC_STATE_MUSIC,
    TLKAPP_AUDIO_UAC_STATE_VOICE,
    TLKAPP_AUDIO_UAC_STATE_RECORD,
    TLKAPP_AUDIO_UAC_STATE_MAX,
};

typedef struct
{
    uint32_t newState;
    bool     isSpkEN;
    bool     isMicEN;
} tlkapp_uac_StateChgArg_t;

typedef struct
{
    uint8_t  optype;
    uint8_t  resv;
    uint16_t handle;
    bool     musicAttEN;
    bool     voiceAttEN;
} tlkapp_uac_CreateTaskArg_t;

/**
 * @brief       This function initializes the audio UAC module.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioUac_init(void);

/**
 * @brief       This function deinitializes the audio UAC module.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioUac_deinit(void);

/**
 * @brief       This function switches the music path.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_ctrl_switchMusicPath(void);

/**
 * @brief       This function serves as a callback for task state changes.
 * @param[in]   taskID    - the task ID.
 * @param[in]   oldState  - the previous state of the task.
 * @param[in]   newState  - the new state of the task.
 * @param[in]   UserArg   - user argument.
 * @return      none.
 */
void tlkapp_audioUac_taskStateChgCB(uint32_t taskID, uint8_t oldState, uint8_t newState, void *UserArg);
#endif // TLKAPP_AUDIO_UAC_H