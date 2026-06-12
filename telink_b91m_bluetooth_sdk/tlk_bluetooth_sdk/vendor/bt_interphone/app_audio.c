/********************************************************************************************************
 * @file    app_audio.c
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
#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkapp/tlkapp.h"


#if TLK_PCM_DATA_WR_EN

audio_ram_code uint16_t app_audio_get_spk_data(int16_t *p_des, uint16_t dataLen)
{
    (void)dataLen;

    if (NULL == p_des) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "app_audio_get_spk_data error");
        return 0;
    }

    return 0;
}

audio_ram_code uint16_t app_audio_get_mic_data(int16_t *p_des, uint16_t dataLen)
{
    (void)dataLen;

    if (NULL == p_des) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "app_audio_get_mic_data error");
        return 0;
    }

    return 0;
}

audio_ram_code uint16_t app_audio_get_dsp_mic_data(int16_t *p_des, uint16_t dataLen)
{
    (void)dataLen;

    if (NULL == p_des) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "app_audio_get_dsp_mic_data error");
        return 0;
    }

    return 0;
}
#endif

void app_audio_get_pcm_data_init(void)
{
#if TLK_PCM_DATA_WR_EN
    bt_audio_register_get_spk_data_callback(app_audio_get_spk_data);
    bt_audio_register_get_mic_data_callback(app_audio_get_mic_data);
    bt_audio_register_get_dsp_mic_data_callback(app_audio_get_dsp_mic_data);
#endif
}

void app_audio_create_interphone_task(void)
{
    tlkapp_audioScheduler_taskInfo_t info = {
        .audioType = TLKAPP_AUDIO_SCHEDULER_AUDIO_TYPE_MUSIC,
        .optype    = TLKAUD_TYPE_INTRTPHONE,
        .priority  = tlkapp_audioScheduler_getDefaultPriority(TLKAUD_TYPE_INTRTPHONE),
        .state     = TLKAPP_AUDIO_SCHEDULER_TASK_STATE_IDLE,
    };
    uint32_t taskID = 0XFFFF + ((uint32_t)TLKAUD_TYPE_INTRTPHONE << 16);
    tlkapp_audioScheduler_updateTask(taskID, info, 0);
}

void tlkapp_audio_taskInitCompletedHook(void)
{
    app_audio_get_pcm_data_init();
    app_audio_create_interphone_task();
    tlkmdi_audio_set_tone_mix_mode(1);
}
