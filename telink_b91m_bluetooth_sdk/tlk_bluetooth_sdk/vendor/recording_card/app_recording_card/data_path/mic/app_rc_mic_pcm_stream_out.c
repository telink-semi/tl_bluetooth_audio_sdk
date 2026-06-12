/********************************************************************************************************
 * @file    app_rc_mic_pcm_stream_out.c
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
#include "../app_rc_stream.h"
#include "../../app_recording_card_api.h"

#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define RC_MIC_REV_WAV_BUFFER_SIZE (1024 * 15)
#else
#define RC_MIC_REV_WAV_BUFFER_SIZE (1024 * 5)
#endif

typedef struct
{
    uint8_t                 isOpen;
    uint8_t                 resv[3];
    tlkapi_fifo_t           fifo;
    uint8_t                 fifoBuf[RC_MIC_REV_WAV_BUFFER_SIZE];
    rc_stream_data_rdy_cb_t rdyCb;
    TlkOsMutexHandle_t      mutex;
} micPcmOutCtrl_t;

static micPcmOutCtrl_t sMicPcmOutCtrl = {0};

#if TLKMW_RECORDING_CARD_EN
static void app_rc_mic_pcm_stream_in_recv_data(uint8_t *pData, uint16_t dataLen)
{
    tlkos_mutex_lock(sMicPcmOutCtrl.mutex);
    int ret = tlkapi_fifo_write(&sMicPcmOutCtrl.fifo, pData, dataLen);
    tlkos_mutex_unlock(sMicPcmOutCtrl.mutex);
    if (ret <= 0) {
        tlkapi_error(0xFFFFFFFF, "[MIC]", "mic pcm fifo fill error %d", ret);
        return;
    }
    tlksys_task_setEvt(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_GET_MIC_PCM);
}
#endif

static void app_rc_mic_pcm_stream_data_rdy(void)
{
    if (sMicPcmOutCtrl.rdyCb) {
        sMicPcmOutCtrl.rdyCb();
    }
}

static void app_rc_mic_pcm_stream_in_init(void)
{
    tlkos_mutex_create(&sMicPcmOutCtrl.mutex);
    tlkapi_fifo_init(&sMicPcmOutCtrl.fifo, TLKAPI_FIFO_CFG_WRITE_MUTEX_OPEN, sMicPcmOutCtrl.fifoBuf, RC_MIC_REV_WAV_BUFFER_SIZE);
    tlksys_task_regEvtCB(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_GET_MIC_PCM, app_rc_mic_pcm_stream_data_rdy);
}

static int app_rc_mic_pcm_stream_in_open(void *cfg)
{
    if (sMicPcmOutCtrl.isOpen) {
        return -TLK_EREPEAT;
    }
    sMicPcmOutCtrl.isOpen = 1;
    tlkapi_fifo_clear(&sMicPcmOutCtrl.fifo);
    sMicPcmOutCtrl.rdyCb = (rc_stream_data_rdy_cb_t)cfg;
#if TLKMW_RECORDING_CARD_EN
    tlkmdi_recording_card_fillPcmData_register(app_rc_mic_pcm_stream_in_recv_data);
#endif
#if TLK_MW_AUDIO_ENABLE
    tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_RECORDING_CARD, 0xFFFF);
#endif
    return TLK_ENONE;
}

static int app_rc_mic_pcm_stream_in_read(void *pBuffer, uint32_t reqReadLen, uint32_t *relReadLen)
{
    if (sMicPcmOutCtrl.isOpen == 0) {
        return -TLK_EFAIL;
    }
    tlkos_mutex_lock(sMicPcmOutCtrl.mutex);
    int ret = tlkapi_fifo_read(&sMicPcmOutCtrl.fifo, (uint8_t *)pBuffer, reqReadLen);
    tlkos_mutex_unlock(sMicPcmOutCtrl.mutex);
    if (ret <= 0) {
        return -TLK_ENODATA;
    }
    if (relReadLen) {
        *relReadLen = (uint32_t)ret;
    }
    return TLK_ENONE;
}

static void app_rc_mic_pcm_stream_in_close(void)
{
    if (sMicPcmOutCtrl.isOpen == 0) {
        return;
    }
    sMicPcmOutCtrl.isOpen = 0;
#if TLKMW_RECORDING_CARD_EN
    tlkmdi_recording_card_fillPcmData_register(NULL);
#endif
#if TLK_MW_AUDIO_ENABLE
    tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_RECORDING_CARD, 0xFFFF);
#endif
    sMicPcmOutCtrl.rdyCb = NULL;
}

const app_rc_stream_t app_rc_mic_pcm_stream_out = {
    .init  = app_rc_mic_pcm_stream_in_init,
    .open  = app_rc_mic_pcm_stream_in_open,
    .close = app_rc_mic_pcm_stream_in_close,
    .read  = app_rc_mic_pcm_stream_in_read,
};
