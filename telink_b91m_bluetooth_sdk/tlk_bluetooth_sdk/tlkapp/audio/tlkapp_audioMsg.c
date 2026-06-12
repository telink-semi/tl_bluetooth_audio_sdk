/********************************************************************************************************
 * @file    tlkapp_audioMsg.c
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
#if (TLK_MW_AUDIO_ENABLE)
#include "stack/bt/host/btp/btp_stdio.h"
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"

#define TLKAPP_AUDIO_DBG_FLAG 0xFFFFFFFF
#define TLKAPP_AUDIO_DBG_SIGN "[AUD]"

typedef enum
{
    TLKAPP_AUDIOSCHEDULER_MSG_OPID_UPDATEEX = 0,
    TLKAPP_AUDIOSCHEDULER_MSG_OPID_UPDATE,
    TLKAPP_AUDIOSCHEDULER_MSG_OPID_DELETE,
} TLKAPP_AUDIOSCHEDULER_MSG_OPID_ENUM;

typedef struct
{
    uint32_t                          opID;
    uint32_t                          taskID;
    uint32_t                          sameDevTaskId;
    tlkapp_audioScheduler_taskInfo_t  info;
    tlkapp_audioScheduler_extraInfo_t exInfo;
} tlkapp_audioScheduler_msg_t;

/**
 * @brief       This function deals with audio start events.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      0 if success, otherwise fail.
 */
static int tlkapp_audio_startEvtDeal(uint8_t *pData, uint8_t dataLen)
{
    if (dataLen != 4) {
        return -TLK_EPARAM;
    }
    uint8_t  optype   = pData[0];
    uint8_t  priority = pData[1];
    uint16_t handle   = ((uint16_t)pData[3] << 8) | pData[2];
    tlkapi_trace(TLKAPP_AUDIO_DBG_FLAG, TLKAPP_AUDIO_DBG_SIGN, "tlkapp_audio_startEvtDeal: optype-%d", optype);
    uint8_t audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC;
    if (optype == TLKAUD_TYPE_CC_BT_VOICE || optype == TLKAUD_TYPE_LEA_US_VOICE || optype == TLKAUD_TYPE_BT_VOICE_FORWARD) {
        audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE;
    } else if (optype == TLKAUD_TYPE_UAC_LOCAL_AUDIO) {
        audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE;
    }
    uint32_t taskID = handle + ((uint32_t)optype << 16);
    if (optype == TLKAUD_TYPE_CC_BT_VOICE && !tlkmdi_audio_btif_allowedCreateScoWithoutHfp(handle)) {
        return tlkapp_audioScheduler_resumeTask(taskID);
    }
    if (priority == 0xff) {
        priority = tlkapp_audioScheduler_getDefaultPriority(optype);
    }
//TODO: ZIYU TEMP CODE ,need clean
#if TLKMW_INTERPHONE_EN
    if (optype == TLKAUD_TYPE_CC_BT_MUSIC) {
        return TLK_ENONE;
    }
#endif
    tlkapp_audioScheduler_taskInfo_t info = {
        .audioType = audioType,
        .optype    = optype,
        .priority  = priority,
        .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_READY,
    };

    tlkapp_audioScheduler_updateTask(taskID, info, TLKAPP_AUDIO_SCHEDULER_NO_CHANGED);
    return TLK_ENONE;
}

/**
 * @brief       This function deals with audio close events.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      0 if success, otherwise fail.
 */
static int tlkapp_audio_closeEvtDeal(uint8_t *pData, uint8_t dataLen)
{
    (void)dataLen;
    uint8_t  optype   = pData[0];
    uint8_t  isDelete = pData[1];
    uint16_t handle   = ((uint16_t)pData[3] << 8) | pData[2];
    tlkapi_trace(TLKAPP_AUDIO_DBG_FLAG, TLKAPP_AUDIO_DBG_SIGN, "tlkapp_audio_closeEvtDeal: optype-%d,handle-%d", optype, handle);
#if TLKMW_INTERPHONE_EN
    if (optype == TLKAUD_TYPE_CC_BT_MUSIC) {
        return TLK_ENONE;
    }
#endif
    uint32_t taskID = handle + ((uint32_t)optype << 16);
    if (isDelete) {
        tlkapp_audioScheduler_deleteTask(taskID);
    } else {
        tlkapp_audioScheduler_pauseTask(taskID);
    }
    return TLK_ENONE;
}

/**
 * @brief       This function deals with start tone commands.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      0 if success, otherwise fail.
 */
static int tlkapp_audio_startToneCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    (void)dataLen;
    bool res = tlkapp_audio_startTone(pData[0], pData[1]);
    return res ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief       This function deals with ring play events.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      0 if success, otherwise fail.
 */
static int tlkapp_audio_ringPlayDeal(uint8_t *pData, uint8_t dataLen)
{
    (void)dataLen;
    uint16_t                            handle  = pData[0] + pData[1] * 256;
    const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
    if (nowTask != NULL) {
        uint8_t opcode     = TLKAUD_OPCODE_IS_PLAY_RING;
        bool    isPlayRing = tlkapp_audio_modinfOperate(handle, (uint16_t)nowTask->info.optype, &opcode, 1);
        if (isPlayRing == true) {
            return -TLK_EREPEAT; //no need play ring tone;
        }
    }
    bool res = tlkapp_audio_startTone(TONE_RING, true);
    return res ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief       This function switches dongle music.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
static int tlkapp_audio_switchDongleMusic(void)
{
#if (TLK_USB_UAC_ENABLE && (TLKBTP_CFG_A2DPSRC_ENABLE))
    uint8_t optype = tlkapp_audioScheduler_getCurOptype();
    if (optype != TLKAUD_TYPE_A2DP_OUT && optype != TLKAUD_TYPE_LEA_UC_MUSIC) {
        return -TLK_ENOSUPPORT;
    }
    return tlkapp_audio_ctrl_switchMusicPath(); //TODO
#else
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       This function switches TPSLL BT microphone.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
static int tlkapp_audio_switchTpsllBtMic()
{
#if DONGLE_VOICE_MIC_EN
    extern uint8_t g_voice_mic_mode;
    // choose mix mic mode: 00:bt mic  01:tpsll mic
    ll_mix_voice_set_mic_mode(g_voice_mic_mode ? 0 : 1);
#else
    return -TLK_ENOSUPPORT;
#endif

    return TLK_ENONE;
}

/**
 * @brief       This function deals with audio task operations.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      0 if success, otherwise fail.
 */
static int tlkapp_audio_recTaskOperateDeal(uint8_t *pData, uint8_t dataLen)
{
    if (dataLen != sizeof(tlkapp_audioScheduler_msg_t)) {
        return -TLK_EPARAM;
    }
    tlkapp_audioScheduler_msg_t *msg = (tlkapp_audioScheduler_msg_t *)pData;
    switch (msg->opID) {
    case TLKAPP_AUDIOSCHEDULER_MSG_OPID_UPDATEEX:
        return tlkapp_audioScheduler_updateTaskEx(msg->taskID, msg->info, msg->sameDevTaskId, msg->exInfo);
    case TLKAPP_AUDIOSCHEDULER_MSG_OPID_UPDATE:
        return tlkapp_audioScheduler_updateTask(msg->taskID, msg->info, msg->sameDevTaskId);
    case TLKAPP_AUDIOSCHEDULER_MSG_OPID_DELETE:
        return tlkapp_audioScheduler_deleteTask(msg->taskID);
    }
    return -TLK_ENOSUPPORT;
}

/**
 * @brief       This function deals with in-ear state changes.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      0 if success, otherwise fail.
 */
static int tlkapp_audio_recInEarStateChgDeal(uint8_t *pData, uint8_t dataLen)
{
    if (dataLen != 1) {
        return -TLK_EPARAM;
    }
    uint8_t isInEar = pData[0];
    (void)isInEar;
#if (TLK_DEV_CODEC_ENABLE)
    tlkdrv_codec_forceMuteEnable(!isInEar);
#endif
    return TLK_ENONE;
}

/**
 * @brief       This function handles audio messages.
 * @param[in]   msgID   - the message ID.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_msgHandle(uint8_t msgID, uint8_t *pData, uint16_t dataLen)
{
    switch (msgID) {
    case TLKSYS_AUD_MSGID_START_EVT:
        return tlkapp_audio_startEvtDeal(pData, dataLen);
    case TLKSYS_AUD_MSGID_CLOSE_EVT:
        return tlkapp_audio_closeEvtDeal(pData, dataLen);
    case TLKSYS_AUD_MSGID_START_TONE_CMD:
        return tlkapp_audio_startToneCmdDeal(pData, dataLen);
    case TLKSYS_AUD_MSGID_HOST_EVT_COME:
        return tlkmdi_audio_hostif_getHostEvtDeal(pData, dataLen);
    case TLKSYS_AUD_MSGID_TASK_OPERATE:
        return tlkapp_audio_recTaskOperateDeal(pData, dataLen);
    case TLKSYS_AUD_MSGID_INEAR_STATE_CHANGE:
        return tlkapp_audio_recInEarStateChgDeal(pData, dataLen);
    case TLKSYS_AUD_MSGID_PLAY_START:
        return tlkapp_audio_startPlay();
    case TLKSYS_AUD_MSGID_PLAY_CLOSE:
        return tlkapp_audio_closePlay();
    case TLKSYS_AUD_MSGID_PLAY_PAUSE:
        return tlkapp_audio_PlayPause();
    case TLKSYS_AUD_MSGID_PLAY_NEXT:
        return tlkapp_audio_playNext();
    case TLKSYS_AUD_MSGID_PLAY_PREV:
        return tlkapp_audio_playPrev();
    case TLKSYS_AUD_MSGID_INC_VOLUME:
        return tlkapp_audio_volumeCtrl(true);
    case TLKSYS_AUD_MSGID_DEC_VOLUME:
        return tlkapp_audio_volumeCtrl(false);
    case TLKSYS_AUD_MSGID_CALL_ACCEPT:
        return tlkapp_audio_callCtrl(TLKAUD_OPCODE_CALL_ACCEPT);
    case TLKSYS_AUD_MSGID_CALL_HUNGUP:
        return tlkapp_audio_callCtrl(TLKAUD_OPCODE_CALL_HUNGUP);
    case TLKSYS_AUD_MSGID_RING_PLAY:
        return tlkapp_audio_ringPlayDeal(pData, dataLen);
    case TLKSYS_AUD_MSGID_DONGLE_SWITCH:
        return tlkapp_audio_switchDongleMusic();
    case TLKSYS_AUD_MSGID_SWITCH_MIC:
        return tlkapp_audio_switchTpsllBtMic();
#if TLKALG_ANC_ENABLE
    case TLKSYS_AUD_MSGID_TOGGLE_ANC:
        tlkmw_anc_function_en(!tlkmw_anc_is_enable());
        return TLK_ENONE;
    case TLKSYS_AUD_MSGID_ENABLE_ANC:
        tlkmw_anc_function_en(pData[0]);
        return TLK_ENONE;
#endif
    }
    return -TLK_ENOSUPPORT;
}

/**
 * @brief       This function safely updates a task with extended information.
 * @param[in]   taskId        - the task ID.
 * @param[in]   info          - task information.
 * @param[in]   sameDevTaskId - same device task ID.
 * @param[in]   exInfo        - extended information.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_updateTaskExSafe(uint32_t taskId, tlkapp_audioScheduler_taskInfo_t info, uint32_t sameDevTaskId, tlkapp_audioScheduler_extraInfo_t exInfo)
{
    tlkapp_audioScheduler_msg_t msg;
    msg.opID          = TLKAPP_AUDIOSCHEDULER_MSG_OPID_UPDATEEX;
    msg.taskID        = taskId;
    msg.info          = info;
    msg.sameDevTaskId = sameDevTaskId;
    msg.exInfo        = exInfo;
    return tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_TASK_OPERATE, &msg, sizeof(msg));
}

/**
 * @brief       This function safely updates a task.
 * @param[in]   taskId        - the task ID.
 * @param[in]   info          - task information.
 * @param[in]   sameDevTaskId - same device task ID.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_updateTaskSafe(uint32_t taskId, tlkapp_audioScheduler_taskInfo_t info, uint32_t sameDevTaskId)
{
    tlkapp_audioScheduler_msg_t msg;
    msg.opID          = TLKAPP_AUDIOSCHEDULER_MSG_OPID_UPDATE;
    msg.taskID        = taskId;
    msg.info          = info;
    msg.sameDevTaskId = sameDevTaskId;
    return tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_TASK_OPERATE, &msg, sizeof(msg));
}

/**
 * @brief       This function safely deletes a task.
 * @param[in]   taskId - the task ID.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioScheduler_deleteTaskSafe(uint32_t taskId)
{
    tlkapp_audioScheduler_msg_t msg;
    msg.opID   = TLKAPP_AUDIOSCHEDULER_MSG_OPID_DELETE;
    msg.taskID = taskId;
    return tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_TASK_OPERATE, &msg, sizeof(msg));
}
#endif // #if (TLK_MW_AUDIO_ENABLE)
