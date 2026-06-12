/********************************************************************************************************
 * @file    tlkmdi_audio_tpsif_key.c
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

/**
 * @brief Get the HID message ID based on the enabled TPS interface type.
 * @return The HID message ID.
 */
static uint16_t tlkmdi_audio_tpsif_getHidMsgID(void)
{
#if TLK_STK_BT_TPSLL_ENABLE
    return TLKSYS_TPH_MSGID_SEND_KEY;
#elif TLKSTK_BTTPSLL_TWS_ENABLE
    return TLKSYS_TPT_MSGID_SEND_KEY;
#else
    return 0XFFFF;
#endif
}

/**
 * @brief  Trigger one TPS hid key .
 * @param[in]  handle - acl handle
 * @param[in]  key - hid key id,refer to AUD_TPSIF_KEYID_ENUM
 * @returns Returning TLK_ENONE(0x00) means the send process success.
 *          If others value is returned means the send process fail.
 */
int tlkmdi_audio_tpsif_sendKey(uint16_t handle, uint8_t key)
{
    uint8_t buffer[4];
    uint8_t buffLen   = 0;
    buffer[buffLen++] = key;
    buffer[buffLen++] = (handle & 0x00FF);
    buffer[buffLen++] = (handle & 0xFF00) >> 8;
    uint16_t msgID    = tlkmdi_audio_tpsif_getHidMsgID();
    return tlksys_sendMsg(TLKSYS_TASKID_HOST, msgID, buffer, buffLen);
}

#endif // #if (TLK_MW_AUDIO_ENABLE)
