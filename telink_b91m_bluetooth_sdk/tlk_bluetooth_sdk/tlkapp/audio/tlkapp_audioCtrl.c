/********************************************************************************************************
 * @file    tlkapp_audioCtrl.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#if (TLK_MW_AUDIO_ENABLE)

/**
 * @brief       This function starts playing audio.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_startPlay(void)
{
    const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
    if (nowTask != NULL) {
        uint8_t opcode = TLKAUD_OPCODE_IS_CUSTOMIZED_PLAYPAUSE;
        if (tlkapp_audio_modinfOperate(nowTask->taskId, nowTask->info.optype, &opcode, 1) == false) {
            return -TLK_EFAIL;
        }
        return tlkapp_audio_modinfStart(nowTask->info.optype, nowTask->taskId, 0);
    }
    const tlkapp_audioScheduler_node_t *taskNode = tlkapp_audioScheduler_SchPausedTask(false, TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC);
    if (taskNode == NULL) {
        return -TLK_EFAIL;
    }
    uint16_t handle = (uint16_t)taskNode->taskId;
    uint8_t  optype = taskNode->info.optype;
    return tlkapp_audio_modinfStart(optype, handle, 0);
}

/**
 * @brief       This function closes current playing audio.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_closePlay(void)
{
    const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
    if (nowTask != NULL && (nowTask->info.audioType & TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC)) {
        return tlkapp_audio_modinfClose(nowTask->info.optype, (uint16_t)nowTask->taskId);
    }
    return -TLK_EFAIL;
}

/**
 * @brief       This function pauses or resumes audio playback.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_PlayPause(void)
{
    //pause logic:
    const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
    if (nowTask != NULL) {
        if ((nowTask->info.audioType & TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC) == 0) {
            return -TLK_EFAIL;
        }
        uint8_t opcode = TLKAUD_OPCODE_IS_CUSTOMIZED_PLAYPAUSE;
        if (tlkapp_audio_modinfOperate(nowTask->taskId, nowTask->info.optype, &opcode, 1) == true) {
            opcode   = TLKAUD_OPCODE_TRIGGER_CUSTOMIZED_PLAYPAUSE;
            bool res = tlkapp_audio_modinfOperate(nowTask->taskId, nowTask->info.optype, &opcode, 1);
            return res ? TLK_ENONE : -TLK_EFAIL;
        }
        return tlkapp_audio_modinfClose(nowTask->info.optype, (uint16_t)nowTask->taskId);
    }
    //play logic:
    const tlkapp_audioScheduler_node_t *taskNode = tlkapp_audioScheduler_SchPausedTask(false, TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC);
    if (taskNode == NULL) {
        return -TLK_EFAIL;
    }
    uint16_t handle = (uint16_t)taskNode->taskId;
    uint8_t  optype = taskNode->info.optype;
    return tlkapp_audio_modinfStart(optype, handle, 0);
}

/**
 * @brief       This function plays the next audio track.
 * @param[in]   none.
 * @return      true if success, false if failure.
 */
bool tlkapp_audio_playNext(void)
{
    const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
    if (nowTask != NULL) {
        return tlkapp_audio_modinfToNext(nowTask->info.optype);
    }
    const tlkapp_audioScheduler_node_t *taskNode = tlkapp_audioScheduler_SchPausedTask(false, TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC);
    if (taskNode == NULL) {
        return false;
    }
    uint16_t handle = (uint16_t)taskNode->taskId;
    uint8_t  optype = taskNode->info.optype;

    //TLKAUD_START_MODE_PLAY_PREV = 0X8
    if (tlkapp_audio_modinfStart(optype, handle, 0x80000000) != TLK_ENONE) {
        return false;
    }
    return true;
}

/**
 * @brief       This function plays the previous audio track.
 * @param[in]   none.
 * @return      true if success, false if failure.
 */
bool tlkapp_audio_playPrev(void)
{
    const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
    if (nowTask != NULL) {
        return tlkapp_audio_modinfToPrev(nowTask->info.optype);
    }
    const tlkapp_audioScheduler_node_t *taskNode = tlkapp_audioScheduler_SchPausedTask(false, TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC);
    if (taskNode == NULL) {
        return false;
    }
    uint16_t handle = (uint16_t)taskNode->taskId;
    uint8_t  optype = taskNode->info.optype;

    //TLKAUD_START_MODE_PLAY_PREV = 0X4
    if (tlkapp_audio_modinfStart(optype, handle, 0x40000000) != TLK_ENONE) {
        return false;
    }
    return true;
}

/**
 * @brief       This function starts playing a tone.
 * @param[in]   fileIndex - the tone file index.
 * @param[in]   isActive  - whether to play immediately.
 * @return      true if success, false if failure.
 */
bool tlkapp_audio_startTone(uint8_t fileIndex, uint8_t isActive)
{
    (void)isActive;
    (void)fileIndex;
#if TLK_CFG_TONE_ENABLE
    uint32_t                            isNeedCreateToneTask = 0xFFFF0000;
    const tlkapp_audioScheduler_node_t *nowTask              = tlkapp_audioScheduler_getRunningTask();
    if (nowTask != NULL) {
        uint8_t opcode = TLKAUD_OPCODE_IS_SUPPORT_TONE_MIX;
        bool    res    = tlkapp_audio_modinfOperate((uint16_t)nowTask->taskId, (uint16_t)nowTask->info.optype, &opcode, 1);
        if (res == true) {
            isNeedCreateToneTask = 0;
        }
    }
    uint32_t param = isNeedCreateToneTask | fileIndex;
    param |= (uint32_t)isActive << 8;
    return tlkapp_audio_modinfStart(TLKAUD_TYPE_TONE, TLK_INVALID_HANDLE, param);
#else
    return false;
#endif
}

/**
 * @brief       This function stops playing a tone.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_audio_stopTone(void)
{
    tlkapp_audio_modinfClose(TLKAUD_TYPE_TONE, TLK_INVALID_HANDLE);
}

/**
 * @brief       This function controls audio volume.
 * @param[in]   isInc - whether to increase volume (true) or decrease (false).
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_volumeCtrl(uint8_t isInc)
{
    uint8_t volType;
    if (isInc) {
        volType = TLKAUD_OPCODE_VOLUME_INC;
    } else {
        volType = TLKAUD_OPCODE_VOLUME_DEC;
    }
    const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
    if (nowTask != NULL) {
        bool ret = tlkapp_audio_modinfOperate((uint16_t)nowTask->taskId, nowTask->info.optype, &volType, 1);
        return ret ? TLK_ENONE : -TLK_EFAIL;
    }
    const tlkapp_audioScheduler_node_t *taskNode = tlkapp_audioScheduler_SchPausedTask(false, TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC);
    if (taskNode == NULL) {
        return -TLK_ENOOBJECT;
    }

    uint8_t optype = taskNode->info.optype;
    bool    res    = tlkapp_audio_modinfOperate((uint16_t)taskNode->taskId, optype, &volType, 1);
    return res ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief       This function controls audio calls.
 * @param[in]   opcode - the operation code.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_callCtrl(uint8_t opcode)
{
    bool res = false;
    if (opcode != TLKAUD_OPCODE_CALL_ACCEPT && opcode != TLKAUD_OPCODE_CALL_HUNGUP) {
        return -TLK_EPARAM;
    }

    const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
    if (nowTask != NULL) {
        res = tlkapp_audio_modinfOperate((uint16_t)nowTask->taskId, (uint16_t)nowTask->info.optype, &opcode, 1);
    }
    if (res == false) {
        uint16_t msgID  = opcode == TLKAUD_OPCODE_CALL_ACCEPT ? TLKSYS_BT_MSGID_HF_SEND_CALL_ACCEPT : TLKSYS_BT_MSGID_HF_SEND_CALL_HUNGUP;
        uint16_t handle = 0xFFFF;
        return tlksys_sendMsg(TLKSYS_TASKID_HOST, msgID, &handle, 2);
    }
    return TLK_ENONE;
}
#endif // #if (TLK_MW_AUDIO_ENABLE)
