/********************************************************************************************************
 * @file    tlkapp_host_tph_msg.c
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
#include "tlkmw/tlkmw.h"
#if (TLK_STK_BT_TPSLL_ENABLE)

/**
 * @brief       Handles the sending of key events for TPSLL Headset.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
static int tlkapp_host_tph_recvSendKeyDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    typedef int (*tphKeyFunc)(void);
    static const tphKeyFunc sTphKeyFuncs[AUD_TPSIF_KEYID_NUM] = {
        [AUD_TPSIF_KEYID_VOL_UP] = tlkmdi_bt_tph_keyFuncAudioVolumeUp,        [AUD_TPSIF_KEYID_VOL_DOWN] = tlkmdi_bt_tph_keyFuncAudioVolumeDown,
        [AUD_TPSIF_KEYID_PLAY_PAUSE] = tlkmdi_bt_tph_keyFuncMusicPlayPause,   [AUD_TPSIF_KEYID_PLAY_FORWARD] = tlkmdi_bt_tph_keyFuncMusicForward,
        [AUD_TPSIF_KEYID_PLAY_BACKWARD] = tlkmdi_bt_tph_keyFuncMusicBackward,
    };
    uint8_t key = pData[0];
    if (key >= AUD_TPSIF_KEYID_NUM) {
        return -TLK_EPARAM;
    }
    return sTphKeyFuncs[key]();
}

/**
 * @brief       Handles messages for the TPSLL Headset module.
 * @param[in]   msgID    - The message ID.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_host_tph_msgHandle(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    (void)msgID;
    (void)pData;
    (void)dataLen;
    switch (msgID) {
    case TLKSYS_TPH_MSGID_3S_PAIR:
        tlkmdi_bt_tph_pair_start(false);
        break;
    case TLKSYS_TPH_MSGID_10S_PAIR:
        tlkmdi_bt_tph_pair_start(true);
        break;
    case TLKSYS_TPH_MSGID_SEND_KEY:
        return tlkapp_host_tph_recvSendKeyDeal(pData, dataLen);
    default:
        return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}

#endif
