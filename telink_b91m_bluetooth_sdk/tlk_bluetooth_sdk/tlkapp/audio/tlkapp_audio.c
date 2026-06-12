/********************************************************************************************************
 * @file    tlkapp_audio.c
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
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#if (TLK_MW_AUDIO_ENABLE)

#ifndef TLKAPP_AUDIO_TASK_STASK_SIZE
#define TLKAPP_AUDIO_TASK_STASK_SIZE (8 * 1024)
#endif

static TlkApiTimer_t sTlkAppAudioCurTimer;
static uint8_t       sTlkAppAudioCodecIdleTmrCount = 0;

/**
 * @brief       This function closes audio in case of crash.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_audio_crashClose(void)
{
    const tlkapp_audioScheduler_node_t *audioTask = tlkapp_audioScheduler_getRunningTask();
    if (audioTask == NULL) {
        return;
    }
    if (audioTask->info.optype == TLKAUD_TYPE_TONE) {
        tlkapp_audioScheduler_deleteTask(audioTask->taskId);
    } else {
        tlkapp_audioScheduler_pauseTask(audioTask->taskId);
    }
}

/**
 * @brief       This function handles the audio timer event.
 * @param[in]   pTimer  - the timer handle.
 * @param[in]   userArg - user argument.
 * @return      none.
 */
static void tlkapp_audio_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    const tlkapp_audioScheduler_node_t *audioTask = tlkapp_audioScheduler_getRunningTask();
    if (audioTask == NULL) {
        return;
    }
    uint8_t audioOptype = audioTask->info.optype;
    tlkapp_audio_modinfTimer(audioOptype);

    if (!tlkapp_audio_modinfIsBusy(audioOptype)) {
        if (sTlkAppAudioCodecIdleTmrCount == 0) {
            tlkapp_audio_crashClose();
            return;
        }
        sTlkAppAudioCodecIdleTmrCount--;
    }
    tlksys_timer_reStart(TLKSYS_TASKID_AUDIO, pTimer);
}

/**
 * @brief       This function initializes the audio task.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_audio_init(void)
{
    for (uint32_t index = TLKSYS_TASK_EVT_AUD_CORE + 1; index < TLKSYS_TASK_EVT_AUD_NUM; index++) {
        tlksys_task_regEvtCB(TLKSYS_TASKID_AUDIO, index, tlkmdi_audio_getEvtTask(index));
    }
    tlksys_timer_createStatic(TLKSYS_TASKID_AUDIO, &sTlkAppAudioCurTimer, TLKAPP_AUDIO_TIMEOUT, false, tlkapp_audio_timer, NULL);
    tlkapp_audioScheduler_featureInit();
    tlkmdi_audio_init();
    tlkapp_audio_modinfNodeInit();
#if (!TLK_STK_BT_TPSLL_ENABLE) // ZEWEN modify for 3 mode switch
    tlkapp_audioUac_init();
#endif
    tlksys_pm_regChn(TLKSYS_PM_CHN_AUD);
    tlksys_pm_setChn(TLKSYS_PM_CHN_AUD, 0, 0);
    tlkapp_audio_taskInitCompletedHook();
}

/**
 * @brief       This function handles input messages for the audio task.
 * @param[in]   msgID   - message ID.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      0 if success, otherwise failed.
 */
static int tlkapp_audio_input(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    if (msgID == TLKSYS_AUD_MSGID_RUN_FUNC) {
        const uint8_t headLen = sizeof(tlksys_requestRunFunc_t);
        if (dataLen == headLen) {
            tlksys_requestRunFunc_t func = *(tlksys_requestRunFunc_t *)pData;
            func();
        } else if (dataLen >= headLen) {
            tlksys_requestRunFuncWithArg_t func = *(tlksys_requestRunFuncWithArg_t *)pData;
            func(pData + headLen, dataLen - headLen);
        } else {
            return -TLK_EPARAM;
        }
        return TLK_ENONE;
    }
    return tlkapp_audio_msgHandle(msgID, pData, dataLen);
}

/**
 * @brief       This function gets the configuration of the audio task.
 * @param[in]   none.
 * @return      Pointer to the task configuration.
 */
const tlksys_task_cfg_t *tlkapp_audio_getTaskCfg(void)
{
    _attribute_os_heap_sec_ __attribute__((aligned(4))) static uint8_t sTlkAppAudioTaskBuffer[TLKAPP_AUDIO_TASK_STASK_SIZE + TLKSYS_TASK_EXTRA_STATIC_BUFFER_SIZE];
    (void)sTlkAppAudioTaskBuffer;
    static const tlksys_task_cfg_t sTlkAppAudioTask = {
        .Init      = tlkapp_audio_init,
        .Input     = tlkapp_audio_input,
        .priority  = TLKSYS_TASK_AUDIO_PRIORITY,
        .stackSize = TLKAPP_AUDIO_TASK_STASK_SIZE + TLKSYS_TASK_EXTRA_STATIC_BUFFER_SIZE, //do not use sizeof(sTlkAppSystemTaskBuffer)
        .pTaskName = "AUDIO",
#if TLK_CFG_RTOS_ENABLE
        .pTaskStaticBuffer = &sTlkAppAudioTaskBuffer,
#endif
    };
    return &sTlkAppAudioTask;
}

/**
 * @brief       This function starts the audio handler.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_audio_startHandler(void)
{
    tlksys_pm_reqHighClock(TLK_CFG_AUDIO_CLOCK_LEVEL, TLKSYS_CLOCK_CHN_AUDIO);
    sTlkAppAudioCodecIdleTmrCount = TLKAPP_AUDIO_CODEC_IDLE_TIMEOUT;
    tlksys_timer_reStart(TLKSYS_TASKID_AUDIO, &sTlkAppAudioCurTimer);
    tlkmw_host_linkmgr_setSysBusy(1);
    tlksys_pm_setChn(TLKSYS_PM_CHN_AUD, 0, 1);
}

/**
 * @brief       This function closes the audio handler.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_audio_closeHandler(void)
{
    tlksys_pm_cancelHighClockReq(TLKSYS_CLOCK_CHN_AUDIO);
    sTlkAppAudioCodecIdleTmrCount = 0;
    tlksys_timer_stop(TLKSYS_TASKID_AUDIO, &sTlkAppAudioCurTimer);
    tlkmw_host_linkmgr_setSysBusy(0);
    tlksys_pm_setChn(TLKSYS_PM_CHN_AUD, 0, 0);
}

/**
 * @brief     Provides a hook function when the audio task init completed.
 * @param[in] None.
 * @returns   None.
 */
__attribute__((weak)) void tlkapp_audio_taskInitCompletedHook(void) {}

#endif // #if (TLK_MW_AUDIO_ENABLE)
