/********************************************************************************************************
 * @file    tlkmdi_audio_btif_core.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#if (TLK_MW_AUDIO_ENABLE && TLK_STK_BT_ENABLE)

#define CB_ARRAY_NUM 3

static TlkMdiAudBtifVolChgCB sTlkMdiAudBtifVoiceVolChgCB = NULL;
static TlkMdiAudBtifVolChgCB sTlkMdiAudBtifMusicVolChgCB = NULL;

static uint8_t                      sTlkMdiAudBtifAudStateChgCBTriggerMode       = 0;
static TlkMdiAudBtifMusicStateChgCB sTlkMdiAudBtifMusicStateChgCBs[CB_ARRAY_NUM] = {0};
static TlkMdiAudBtifVoiceStateChgCB sTlkMdiAudBtifVoiceStateChgCBs[CB_ARRAY_NUM] = {0};

/**
 * @brief       Handle volume change events for music or voice
 * @param[in]   evt - Pointer to volume change event structure
 * @return      None
 */
static void tlkmdi_audio_btif_getVolumeChgDeal(tlksys_msg_hostEvt_btVolChg_t *evt)
{
    if (evt->isMusic && sTlkMdiAudBtifMusicVolChgCB) {
        sTlkMdiAudBtifMusicVolChgCB(evt->handle, evt->volume);
    } else if (!evt->isMusic && sTlkMdiAudBtifVoiceVolChgCB) {
        sTlkMdiAudBtifVoiceVolChgCB(evt->handle, evt->volume);
    }
}

/**
 * @brief       Handle voice state change events
 * @param[in]   evt - Pointer to audio state change event structure
 * @return      None
 */
static void tlkmdi_audio_btif_getVoiceStateChgDeal(tlksys_msg_hostEvt_btAudStateChg_t *evt)
{
    for (int i = 0; i < CB_ARRAY_NUM; i++) {
        if (sTlkMdiAudBtifVoiceStateChgCBs[i] == NULL) {
            continue;
        }
        sTlkMdiAudBtifVoiceStateChgCBs[i](evt->handle, evt->scoHandle, evt->state, evt->codecType);
    }
}

/**
 * @brief       Handle audio state change events
 * @param[in]   evt - Pointer to audio state change event structure
 * @return      None
 */
static void tlkmdi_audio_btif_getAudStateChgDeal(tlksys_msg_hostEvt_btAudStateChg_t *evt)
{
    tlk_printf("tlkmdi_audio_btif_getAudStateChgDeal evt isMusic:%d, handle[%d]", evt->isMusic, evt->handle);
    if (evt->isMusic == 0) {
        tlkmdi_audio_btif_getVoiceStateChgDeal(evt);
        return;
    }
    if (sTlkMdiAudBtifAudStateChgCBTriggerMode == 0) {
        for (int i = 0; i < CB_ARRAY_NUM; i++) {
            if (sTlkMdiAudBtifMusicStateChgCBs[i] == NULL) {
                continue;
            }
            sTlkMdiAudBtifMusicStateChgCBs[i](evt->handle, evt->state);
        }
    } else {
        for (int i = CB_ARRAY_NUM - 1; i >= 0; i--) {
            if (sTlkMdiAudBtifMusicStateChgCBs[i] == NULL) {
                continue;
            }
            sTlkMdiAudBtifMusicStateChgCBs[i](evt->handle, evt->state);
        }
    }
}

/**
 * @brief       Register callback for voice volume change events
 * @param[in]   cb - Callback function pointer
 * @return      None
 */
void tlkmdi_audio_btif_regVoiceVolChgCB(TlkMdiAudBtifVolChgCB cb)
{
    sTlkMdiAudBtifVoiceVolChgCB = cb;
}

/**
 * @brief       Register callback for music volume change events
 * @param[in]   cb - Callback function pointer
 * @return      None
 */
void tlkmdi_audio_btif_regMusicVolChgCB(TlkMdiAudBtifVolChgCB cb)
{
    sTlkMdiAudBtifMusicVolChgCB = cb;
}

/**
 * @brief       Add callback for music state change events
 * @param[in]   cb - Callback function pointer
 * @param[in]   isPushBack - Whether to add at the end of callback array
 * @return      TLK_ENONE: success; TLK_EPARAM: invalid parameter; TLK_EQUOTA: no space for new callback
 */
int tlkmdi_audio_btif_addMusicStateChgCB(TlkMdiAudBtifMusicStateChgCB cb, uint8_t isPushBack)
{
    if (cb == NULL) {
        return -TLK_EPARAM;
    }
    if (isPushBack) {
        for (int i = 0; i < CB_ARRAY_NUM; i++) {
            if (sTlkMdiAudBtifMusicStateChgCBs[i] != NULL) {
                continue;
            }
            sTlkMdiAudBtifMusicStateChgCBs[i] = cb;
            return TLK_ENONE;
        }
    } else {
        for (int i = CB_ARRAY_NUM - 1; i >= 0; i--) {
            if (sTlkMdiAudBtifMusicStateChgCBs[i] != NULL) {
                continue;
            }
            sTlkMdiAudBtifMusicStateChgCBs[i] = cb;
            return TLK_ENONE;
        }
    }

    return -TLK_EQUOTA;
}

/**
 * @brief       Add callback for voice state change events
 * @param[in]   cb - Callback function pointer
 * @return      TLK_ENONE: success; TLK_EPARAM: invalid parameter; TLK_EQUOTA: no space for new callback
 */
int tlkmdi_audio_btif_addVoiceStateChgCB(TlkMdiAudBtifVoiceStateChgCB cb)
{
    if (cb == NULL) {
        return -TLK_EPARAM;
    }
    for (int i = 0; i < CB_ARRAY_NUM; i++) {
        if (sTlkMdiAudBtifVoiceStateChgCBs[i] != NULL) {
            continue;
        }
        sTlkMdiAudBtifVoiceStateChgCBs[i] = cb;
        return TLK_ENONE;
    }

    return -TLK_EQUOTA;
}

/**
 * @brief       Set trigger mode for music state change callbacks
 * @param[in]   mode - Trigger mode (0: forward, non-zero: backward)
 * @return      None
 */
void tlkmdi_audio_btif_setMusicStateChgCBTriggerMode(uint8_t mode)
{
    sTlkMdiAudBtifAudStateChgCBTriggerMode = mode;
}

/**
 * @brief       Process host events related to Bluetooth audio interface
 * @param[in]   pData - Pointer to event data
 * @param[in]   dataLen - Length of event data
 * @return      None
 */
void tlkmdi_audio_btif_getHostEvtDeal(uint8_t *pData, uint8_t dataLen)
{
    if (dataLen < sizeof(tlksys_msg_hostEvt_t)) {
        return;
    }
    tlksys_msg_hostEvt_t *evt = (tlksys_msg_hostEvt_t *)pData;
    if (evt->hostType != TLKSYS_MSG_HOST_TYPE_BT) {
        return;
    }
    switch (evt->msgID) {
    case TLKSYS_MSG_BT_HOST_EVT_TYPE_VOLUME_CHG:
    {
        if (evt->dataLen != sizeof(tlksys_msg_hostEvt_btVolChg_t)) {
            return;
        }
        tlkmdi_audio_btif_getVolumeChgDeal((tlksys_msg_hostEvt_btVolChg_t *)evt->data);
    } break;
    case TLKSYS_MSG_BT_HOST_EVT_TYPE_AUD_STATE_CHG:
    {
        if (evt->dataLen != sizeof(tlksys_msg_hostEvt_btAudStateChg_t)) {
            return;
        }
        tlkmdi_audio_btif_getAudStateChgDeal((tlksys_msg_hostEvt_btAudStateChg_t *)evt->data);
    } break;
    }
}

#else

void tlkmdi_audio_btif_getHostEvtDeal(uint8_t *pData, uint8_t dataLen)
{
    (void)pData;
    (void)dataLen;
}

#endif // #if (TLK_MW_AUDIO_ENABLE)
