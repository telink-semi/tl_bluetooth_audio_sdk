/********************************************************************************************************
 * @file    tlkmdi_audio_tpsif_core.c
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

#if (TLK_MW_AUDIO_ENABLE)

#define TPSLL_CB_ARRAY_NUM 2

static TlkMdiAudTpsifStateChgCB sTlkMdiAudTpsifStateChgCBs[TPSLL_CB_ARRAY_NUM] = {0};

/**
 * @brief Process audio state change events and notify registered callbacks.
 * @param[in] evt - Pointer to the audio state change event data.
 */
static void tlkmdi_audio_tpsif_getAudStateChgDeal(tlksys_msg_hostEvt_tpsAudStateChg_t *evt)
{
    for (int i = 0; i < TPSLL_CB_ARRAY_NUM; i++) {
        if (sTlkMdiAudTpsifStateChgCBs[i] == NULL) {
            continue;
        }
        sTlkMdiAudTpsifStateChgCBs[i](evt->mode);
    }
}

/**
 * @brief Add a callback function for state change notification.
 * @param[in] cb - Callback function to be added.
 * @return 0 - Success.
 * @return TLK_EPARAM - Invalid parameter.
 * @return TLK_EQUOTA - No space for new callback.
 */
int tlkmdi_audio_tpsif_addStateChgCB(TlkMdiAudTpsifStateChgCB cb)
{
    if (cb == NULL) {
        return -TLK_EPARAM;
    }
    for (int i = 0; i < TPSLL_CB_ARRAY_NUM; i++) {
        if (sTlkMdiAudTpsifStateChgCBs[i] != NULL) {
            continue;
        }
        sTlkMdiAudTpsifStateChgCBs[i] = cb;
        return TLK_ENONE;
    }
    return -TLK_EQUOTA;
}

/**
 * @brief Get the TPS interface type based on enabled features.
 * @return The TPS interface type enumeration value.
 */
AUD_TPSIF_TYPE_ENUM tlkmdi_audio_tpsif_getType(void)
{
#if TLK_STK_BT_TPSLL_ENABLE
    return AUD_TPSIF_TYPE_TPH;
#elif TLKSTK_BTTPSLL_TWS_ENABLE
    return AUD_TPSIF_TYPE_TPT;
#else
    return AUD_TPSIF_TYPE_UNKNOWN;
#endif
}

/**
 * @brief Process host event data.
 * @param[in] pData - Pointer to the data buffer.
 * @param[in] dataLen - Length of the data.
 */
void tlkmdi_audio_tpsif_getHostEvtDeal(uint8_t *pData, uint8_t dataLen)
{
    if (dataLen < sizeof(tlksys_msg_hostEvt_t)) {
        return;
    }
    tlksys_msg_hostEvt_t *evt = (tlksys_msg_hostEvt_t *)pData;
    if (evt->hostType != TLKSYS_MSG_HOST_TYPE_TPH && evt->hostType != TLKSYS_MSG_HOST_TYPE_TPT) {
        return;
    }
    switch (evt->msgID) {
    case TLKSYS_MSG_TPS_HOST_EVT_TYPE_AUD_STATE_CHG:
    {
        if (evt->dataLen != sizeof(tlksys_msg_hostEvt_tpsAudStateChg_t)) {
            return;
        }
        tlkmdi_audio_tpsif_getAudStateChgDeal((tlksys_msg_hostEvt_tpsAudStateChg_t *)evt->data);
    } break;
    }
}

#endif // #if (TLK_MW_AUDIO_ENABLE)
