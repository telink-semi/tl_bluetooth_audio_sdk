/********************************************************************************************************
 * @file    tlkapp_audioUac.c
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
#if ((TLK_USB_UAC_ENABLE || TLK_USB_UAC_HS_ENABLE) && \
     (TLKBTP_CFG_HFPAG_ENABLE || TLKBTP_CFG_A2DPSRC_ENABLE || TLK_MW_LEA_UC_ENABLE || TLKDRV_CODEC_ICODEC_ENABLE || TLK_MW_LL_DONGLE_MUSIC_ENABLE))
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#if TLKADU_MIDBUF_ENABLE
#include "vendor/GameSir_Xiaoji/audio_mw/tlkaud_audio_mw.h"
#endif

typedef struct
{
    uint16_t spkEn     : 1;
    uint16_t micEn     : 1;
    uint16_t isPending : 1;
    uint16_t resv13bit : 13;
    uint16_t curState;
    uint32_t pending_tick;
} tlkapp_auduac_ctrl_t;

static tlkapp_auduac_ctrl_t sTlkAppAudUacCtrl;

/**
 * @brief       This function is used to handle audio uac idle state .
 * @param[in]   isSpkEn - is speaker enabled.
 * @param[in]   isMicEn - is mic enabled.
 * @return      true - need pending.
 */
static bool tlkapp_audioUac_stateIdle(bool isSpkEn, bool isMicEn)
{
    uint8_t type = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC;
    if (isMicEn == true) {
        type = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE;
    } else if (isSpkEn == true) {
        type = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC;
    } else {
        return false;
    }

#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
    (void)type;
    if (isSpkEn && isMicEn) { // idle -> UAC voice
        sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_VOICE;
        tlkaud_set_audio_mode(AUDIO_UAC_MIC);
        tlkaud_set_audio_mode(AUDIO_UAC_SPK);
        tlkalg_init_common(TLKALG_PPM_CHN0 | TLKALG_PPM_CHN1 | TLKALG_PPM_USB_CALC);
    } else if (isSpkEn && !isMicEn) { // idle -> UAC music
        tlkaud_set_audio_mode(AUDIO_UAC_SPK);
        tlkalg_init_common(TLKALG_PPM_CHN0 | TLKALG_PPM_USB_CALC);
        sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_MUSIC;
    } else if (!isSpkEn && isMicEn) { // idle -> UAC record (only Mic)
        tlkaud_set_audio_mode(AUDIO_UAC_MIC);
        tlkalg_init_common(TLKALG_PPM_CHN1 | TLKALG_PPM_USB_CALC);
        sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_RECORD;
    } else {
        tlkapi_error(0xFFFFFFFF, "[UAC]", "tlkapp_audioUac_stateIdle: error state, Spk:%d Mic:%d", isSpkEn, isMicEn);
        return false;
    }
    tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_UAC_LOCAL_AUDIO, 0xFFFF);
#if TLKADU_MIDBUF_ENABLE
    tlkaud_codec_task_set_mode(PLAYER_MODE_UAC);
#endif
    return false;
#else
    if (tlkapp_audioScheduler_SchPausedTask(true, type) != NULL) {
        return true;
    }
#endif
    return false;
}

/**
 * @brief       This function is used to handle audio uac music state .
 * @param[in]   isSpkEn - is speaker enabled.
 * @param[in]   isMicEn - is mic enabled.
 * @return      true - need pending.
 */
static bool tlkapp_audioUac_stateMusic(bool isSpkEn, bool isMicEn)
{
    bool isNeedPending = true;
    if (isSpkEn || isMicEn) { //case: music -> voice or music -> record
#if (!TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
        int optRes = tlkapp_audioScheduler_runningTaskSwitchType(TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE);
        if (optRes == TLK_ENONE) {
            return false;
        } else if (optRes == -TLK_EFAIL) {
            //note: this case should not happen,plz check operate api!
            tlkapi_error(0xFFFFFFFF, "[UAC]", "TaskSwitch FAIL!,check operate api!");
            return false;
        }
//optRes == -TLK_ENOSUPPORT ,change to idle and select new task.
#else
        if (isMicEn && isSpkEn) { // UAC music -> voice (MIC+SPK)
            const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
            if (nowTask != NULL && nowTask->info.audioType == TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE) {
                uint8_t opcode = TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE;
                tlkapp_audio_modinfOperate(nowTask->taskId, nowTask->info.optype, &opcode, 1);
            }
            sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_VOICE;
            tlkaud_set_audio_mode(AUDIO_UAC_MIC);
            tlkalg_init_common(TLKALG_PPM_CHN1 | TLKALG_PPM_USB_CALC);
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_UAC_LOCAL_AUDIO, 0xFFFF);
            return false;
        } else if (isMicEn && !isSpkEn) { // UAC music -> record (only Mic)
            const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
            if (nowTask != NULL && nowTask->info.audioType == TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE) {
                uint8_t opcode = TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE;
                tlkapp_audio_modinfOperate(nowTask->taskId, nowTask->info.optype, &opcode, 1);
            }
            sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_RECORD;
            tlkaud_clear_audio_mode(AUDIO_UAC_SPK);
            tlkalg_deinit_common(TLKALG_PPM_CHN0);
            tlkaud_set_audio_mode(AUDIO_UAC_MIC);
            tlkalg_init_common(TLKALG_PPM_CHN1 | TLKALG_PPM_USB_CALC);
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_UAC_LOCAL_AUDIO, 0xFFFF);
            return false;
        }
#endif // #if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
    }
    //case music -> idle
    if (tlkapp_audioScheduler_getRunningTask() == NULL) {
        return false;
    } else if (tlkapp_audioScheduler_getRunningTask()->info.audioType == TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE) {
        isNeedPending = false;
    }

#if (!TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
    uint32_t taskID = tlkapp_audioScheduler_getRunningTask()->taskId;
    if (tlkapp_audioScheduler_pauseTask(taskID) == TLK_ENONE) {
        return isNeedPending;
    }
#else
    (void)isNeedPending;
    tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_UAC_LOCAL_AUDIO, 0xFFFF); // UAC music -> idle
#if TLKADU_MIDBUF_ENABLE
    tlkaud_codec_task_clear_mode(PLAYER_MODE_UAC);
    tlkaud_clear_audio_mode(AUDIO_UAC_SPK);
    tlkalg_deinit_common(TLKALG_PPM_CHN0 | TLKALG_PPM_USB_CALC);
#endif
    sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_IDLE;
    return false;
#endif
    return false;
}

/**
 * @brief       This function  .
 * @param[in]   isSpkEn - is speaker enabled.
 * @param[in]   isMicEn - is mic enabled.
 * @return       true - need pending.
 */
static bool tlkapp_audioUac_stateVoice(bool isSpkEn, bool isMicEn)
{
    if (isMicEn && isSpkEn) {
        return false; // still voice state, no change
    }

    if (isSpkEn && !isMicEn) { //voice->music
#if (!TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
        int optRes = tlkapp_audioScheduler_runningTaskSwitchType(TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC);
        if (optRes == TLK_ENONE) {
            return false;
        } else if (optRes == -TLK_EFAIL) {
            //note: this case should not happen,plz check operate api!
            tlkapi_error(0xFFFFFFFF, "[UAC]", "TaskSwitch FAIL!,check operate api!");
            return false;
        }
//optRes == -TLK_ENOSUPPORT ,change to idle and select new task.
#else
        if (isSpkEn && !isMicEn) { // UAC voice -> music
            const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
            if (nowTask != NULL && nowTask->info.audioType == TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE) {
                uint8_t opcode = TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE;
                tlkapp_audio_modinfOperate(nowTask->taskId, nowTask->info.optype, &opcode, 1);
            }
            sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_MUSIC;
            tlkaud_clear_audio_mode(AUDIO_UAC_MIC);
            tlkalg_deinit_common(TLKALG_PPM_CHN1);
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_UAC_LOCAL_AUDIO, 0xFFFF);
            return false;
        }
#endif
    } else if (!isSpkEn && isMicEn) { // voice -> record (only Mic, close Spk)
#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
        const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
        if (nowTask != NULL && nowTask->info.audioType == TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE) {
            uint8_t opcode = TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE;
            tlkapp_audio_modinfOperate(nowTask->taskId, nowTask->info.optype, &opcode, 1);
        }
        sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_RECORD;
        tlkaud_clear_audio_mode(AUDIO_UAC_SPK);
        tlkalg_deinit_common(TLKALG_PPM_CHN0);
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_UAC_LOCAL_AUDIO, 0xFFFF);
        return false;
#endif
    }
    if (tlkapp_audioScheduler_getRunningTask() == NULL) {
        return false;
    }

#if (!TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
    uint32_t taskID = tlkapp_audioScheduler_getRunningTask()->taskId;
    if (tlkapp_audioScheduler_pauseTask(taskID) == TLK_ENONE) {
        return true;
    }
#else
    if (!isSpkEn && !isMicEn) {
        tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_UAC_LOCAL_AUDIO, 0xFFFF); // UAC voice -> idle
        sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_IDLE;
#if TLKADU_MIDBUF_ENABLE
        tlkaud_codec_task_clear_mode(PLAYER_MODE_UAC);
        tlkaud_clear_audio_mode(AUDIO_UAC_MIC);
        tlkaud_clear_audio_mode(AUDIO_UAC_SPK);
        tlkalg_deinit_common(TLKALG_PPM_CHN0 | TLKALG_PPM_CHN1 | TLKALG_PPM_USB_CALC);
#endif
    }
#endif
    return false;
}

/**
 * @brief       This function is used to handle audio uac record state (only Mic).
 * @param[in]   isSpkEn - is speaker enabled.
 * @param[in]   isMicEn - is mic enabled.
 * @return      true - need pending.
 */
static bool tlkapp_audioUac_stateRecord(bool isSpkEn, bool isMicEn)
{
    if (!isMicEn) {     // record -> idle or record -> music (close Mic)
        if (!isSpkEn) { // record -> idle (close Mic)
#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
            tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_UAC_LOCAL_AUDIO, 0xFFFF);
            sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_IDLE;
#if TLKADU_MIDBUF_ENABLE
            tlkaud_codec_task_clear_mode(PLAYER_MODE_UAC);
            tlkaud_clear_audio_mode(AUDIO_UAC_MIC);
            tlkalg_deinit_common(TLKALG_PPM_CHN1 | TLKALG_PPM_USB_CALC);
#endif
#endif
            return false;
        } else { // record -> music (close Mic, keep Spk)
#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
            const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
            if (nowTask != NULL && nowTask->info.audioType == TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE) {
                uint8_t opcode = TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE;
                tlkapp_audio_modinfOperate(nowTask->taskId, nowTask->info.optype, &opcode, 1);
            }
            sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_MUSIC;
            tlkaud_clear_audio_mode(AUDIO_UAC_MIC);
            tlkalg_deinit_common(TLKALG_PPM_CHN1);
            tlkaud_set_audio_mode(AUDIO_UAC_SPK);
            tlkalg_init_common(TLKALG_PPM_CHN0 | TLKALG_PPM_USB_CALC);
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_UAC_LOCAL_AUDIO, 0xFFFF);
#endif
            return false;
        }
    } else if (isSpkEn && isMicEn) { // record -> voice (open Spk, keep Mic)
#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
        const tlkapp_audioScheduler_node_t *nowTask = tlkapp_audioScheduler_getRunningTask();
        if (nowTask != NULL && nowTask->info.audioType == TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE) {
            uint8_t opcode = TLKAUD_OPCODE_TYPE_SWITCH_PATH_STATE;
            tlkapp_audio_modinfOperate(nowTask->taskId, nowTask->info.optype, &opcode, 1);
        }
        sTlkAppAudUacCtrl.curState = TLKAPP_AUDIO_UAC_STATE_VOICE;
        tlkaud_set_audio_mode(AUDIO_UAC_SPK);
        tlkalg_init_common(TLKALG_PPM_CHN0 | TLKALG_PPM_CHN1 | TLKALG_PPM_USB_CALC);
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_UAC_LOCAL_AUDIO, 0xFFFF);
#endif
        return false;
    }
    // still record state (only Mic), no change
    return false;
}

/**
 * @brief       This function is used to handle audio uac state change .
 * @param[in]   arg - state change param.
 * @return      true - need pending.
 */
static bool tlkapp_audioUac_StateChangeAction(void *arg)
{
    typedef bool (*uacStateChangeCB)(bool isSpkEn, bool isMicEn);
    static const uacStateChangeCB sTlkapp_audioUac_stateCBTab[TLKAPP_AUDIO_UAC_STATE_MAX] = {
        [TLKAPP_AUDIO_UAC_STATE_IDLE]   = tlkapp_audioUac_stateIdle,
        [TLKAPP_AUDIO_UAC_STATE_MUSIC]  = tlkapp_audioUac_stateMusic,
        [TLKAPP_AUDIO_UAC_STATE_VOICE]  = tlkapp_audioUac_stateVoice,
        [TLKAPP_AUDIO_UAC_STATE_RECORD] = tlkapp_audioUac_stateRecord,
    };
    tlkapp_uac_StateChgArg_t *param = arg;
    if (param->newState >= TLKAPP_AUDIO_UAC_STATE_MAX) {
        return false;
    }
    return sTlkapp_audioUac_stateCBTab[param->newState](param->isSpkEN, param->isMicEN);
}

/**
 * @brief       This function is used to handle audio uac round robin .
 * @param[in]   arg - round robin param.
 * @return      true - need pending.
 */
static bool tlkapp_audioUac_audTaskRoundRobin(void *arg)
{
    (void)arg;
    if (tlkapp_audioScheduler_roundRobin() != NULL) {
        return true;
    }
    return false;
}

static void tlkapp_audioUac_statusChangeEvt(bool isMicEn, bool isSpkEn);

typedef struct
{
    bool isMicEn;
    bool isSpkEn;
} tlkapp_audioUacState_pack_t;

/**
 * @brief       This function .
 * @param[in]   isMicEn - is mic enabled.
 * @param[in]   isSpkEn - is speaker enabled.
 * @return      none.
 */
static void tlkapp_audioUac_statusChangeEvt_package(void *arg, uint8_t len)
{
    if (len != sizeof(tlkapp_audioUacState_pack_t)) {
        return;
    }
    tlkapp_audioUacState_pack_t *pack = (tlkapp_audioUacState_pack_t *)arg;
    tlkapp_audioUac_statusChangeEvt(pack->isMicEn, pack->isSpkEn);
}

/**
 * @brief       This function is used to handle audio uac status change event.
 * @param[in]   isMicEn - is mic enabled.
 * @param[in]   isSpkEn - is speaker enabled.
 * @return      none.
 */
static void tlkapp_audioUac_statusChangeEvt_threadSafe(bool isMicEn, bool isSpkEn)
{
    tlkapp_audioUacState_pack_t pack = {
        .isMicEn = isMicEn,
        .isSpkEn = isSpkEn,
    };
    tlksys_runFuncInTaskWithArg(TLKSYS_TASKID_AUDIO, tlkapp_audioUac_statusChangeEvt_package, &pack, sizeof(tlkapp_audioUacState_pack_t));
}

static void tlkapp_audio_uacEvtNotify(uint8_t evtID);

static const uint8_t tlkapp_aud_uacEvt2StateTab[TLK_UAC_EVT_NUM] = {
    [TLK_UAC_MUSIC_START_RSP] = TLKAPP_AUDIO_UAC_STATE_MUSIC,
    [TLK_UAC_MUSIC_STOP_RSP]  = TLKAPP_AUDIO_UAC_STATE_IDLE,
    [TLK_UAC_VOICE_START_RSP] = TLKAPP_AUDIO_UAC_STATE_VOICE,
    [TLK_UAC_VOICE_STOP_RSP]  = TLKAPP_AUDIO_UAC_STATE_IDLE,
};

/**
 * @brief       This function initializes the audio UAC module.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioUac_init(void)
{
    /*Need Register UAC status change CB.*/

    tlkusb_uac_regStatusChangeCB(tlkapp_audioUac_statusChangeEvt_threadSafe);
#if TLKDRV_CODEC_ICODEC_ENABLE
    tlkapp_audioScheduler_taskInfo_t info = {
        .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC_AND_VOICE,
#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
        .optype   = TLKAUD_TYPE_UAC_LOCAL_AUDIO,
        .priority = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_UAC_LOCAL_AUDIO),
#else //(TLK_USB_UAC_PLAYBACK_ENABLE)
        .optype   = TLKAUD_TYPE_UAC_AUD,
        .priority = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_UAC_AUD),
#endif
        .state = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
    };
    uint8_t uacType = 0;
#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
    uacType = TLKAUD_TYPE_UAC_LOCAL_AUDIO;
#elif (TLK_USB_UAC_PLAYBACK_ENABLE)
    uacType = TLKAUD_TYPE_UAC_AUD;
#endif
    uint32_t                          taskID = 0xFFFF + ((uint32_t)uacType << 16);
    tlkapp_audioScheduler_extraInfo_t exInfo = {
        .stateChangeCB = tlkapp_audioUac_taskStateChgCB,
    };
    tlkapp_audioScheduler_updateTaskEx(taskID, info, 0, exInfo);
#endif // #if TLKDRV_CODEC_ICODEC_ENABLE
    tlkusb_usc_reg_evt2status_cb(tlkapp_audio_uacEvtNotify);
    return TLK_ENONE;
}

/**
 * @brief       This function deinitializes the audio UAC module.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audioUac_deinit(void)
{
    tlkusb_uac_regStatusChangeCB(NULL);
    uint32_t taskID = 0xFFFF + ((uint32_t)TLKAUD_TYPE_UAC_AUD << 16);
    tmemset(&sTlkAppAudUacCtrl, 0, sizeof(tlkapp_auduac_ctrl_t));
    return tlkapp_audioScheduler_deleteTask(taskID);
}

/**
 * @brief       This function processes the UAC event.
 * @param[in]   evtID - .
 * @return      0none.
 */
_attribute_ram_code_sec_ static void tlkapp_audio_uacEvtNotify(uint8_t evtID)
{
    sTlkAppAudUacCtrl.curState = tlkapp_aud_uacEvt2StateTab[evtID];
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkmdi_audu2h_uacEvtNotify evtID[%d], state[%d]", evtID, sTlkAppAudUacCtrl.curState);

    if (sTlkAppAudUacCtrl.curState == TLKAPP_AUDIO_UAC_STATE_IDLE) {}
    sTlkAppAudUacCtrl.isPending    = 0;
    sTlkAppAudUacCtrl.pending_tick = 0;
}

/**
 * @brief       This function processes the UAC status change event.
 * @param[in]   isMicEn - Mic enable.
 * @param[in]   isSpkEn - Spk enable.
 * @return      0none.
 */
static void tlkapp_audioUac_statusChangeEvt(bool isMicEn, bool isSpkEn)
{
    if ((sTlkAppAudUacCtrl.micEn == isMicEn) && (sTlkAppAudUacCtrl.spkEn == isSpkEn) && tlkapp_audioScheduler_getRunningTask() != NULL) {
        return;
    }

    uint8_t audioType = (isMicEn ? TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_VOICE : TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC);

    if (tlkapp_audioScheduler_getTaskNums(audioType) == 0) {
        return;
    }
    /*Pending Waiting state event. Only used in task which support switch.*/
    if (sTlkAppAudUacCtrl.isPending) {
        if (sTlkAppAudUacCtrl.pending_tick != 0 && clock_time_exceed(sTlkAppAudUacCtrl.pending_tick, TLKUSB_AUD_PENDING_TIME)) {
            sTlkAppAudUacCtrl.pending_tick = 0;
            sTlkAppAudUacCtrl.isPending    = 0;
            sTlkAppAudUacCtrl.curState     = TLKAPP_AUDIO_UAC_STATE_IDLE;
        }
        return;
    }

    sTlkAppAudUacCtrl.micEn = isMicEn;
    sTlkAppAudUacCtrl.spkEn = isSpkEn;

    tlkapp_uac_StateChgArg_t arg = {
        .newState = sTlkAppAudUacCtrl.curState,
        .isMicEN  = sTlkAppAudUacCtrl.micEn,
        .isSpkEN  = sTlkAppAudUacCtrl.spkEn,
    };

    if (tlkapp_audioUac_StateChangeAction(&arg) == true) {
        sTlkAppAudUacCtrl.isPending    = true;
        sTlkAppAudUacCtrl.pending_tick = clock_time() | 1;
    }

    return;
}

/**
 * @brief       This function switches the music path.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_ctrl_switchMusicPath(void)
{
    if (sTlkAppAudUacCtrl.curState != TLKAPP_AUDIO_UAC_STATE_MUSIC) {
        return -TLK_EFAIL;
    }
    if (tlkapp_audioUac_audTaskRoundRobin(NULL) == true) {
        sTlkAppAudUacCtrl.isPending    = 1;
        sTlkAppAudUacCtrl.pending_tick = clock_time() | 1;
    }
    return TLK_ENONE;
}

/**
 * @brief       This function serves as a callback for task state changes.
 * @param[in]   taskID    - the task ID.
 * @param[in]   oldState  - the previous state of the task.
 * @param[in]   newState  - the new state of the task.
 * @param[in]   UserArg   - user argument.
 * @return      none.
 */
void tlkapp_audioUac_taskStateChgCB(uint32_t taskID, uint8_t oldState, uint8_t newState, void *UserArg)
{
    (void)taskID;
    (void)UserArg;
    if (newState != TLKAPP_AUDIO_SCHEDULER_TASK_STATE_NOINIT) {
        return;
    }
    if (oldState == TLKAPP_AUDIO_SCHEDULER_TASK_STATE_RUNNING) {
        memset(&sTlkAppAudUacCtrl, 0, sizeof(sTlkAppAudUacCtrl));
    } else {
        sTlkAppAudUacCtrl.isPending    = 0;
        sTlkAppAudUacCtrl.pending_tick = 0;
    }
}

#else

/**
 * @brief       This function initializes the audio UAC module.
 * @param[in]   none.
 * @return      -TLK_ENOSUPPORT.
 */
int tlkapp_audioUac_init(void)
{
    return -TLK_ENOSUPPORT;
}

/**
 * @brief       This function deinitializes the audio UAC module.
 * @param[in]   none.
 * @return      -TLK_ENOSUPPORT.
 */
int tlkapp_audioUac_deinit(void)
{
    return -TLK_ENOSUPPORT;
}
#endif // #if (TLK_MW_AUDIO_ENABLE)