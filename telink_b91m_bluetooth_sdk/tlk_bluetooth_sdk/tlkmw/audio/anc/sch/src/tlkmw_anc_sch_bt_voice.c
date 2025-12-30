/********************************************************************************************************
 * @file    tlkmw_anc_sch_bt_voice.c
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
#if TLKALG_ANC_ENABLE

#include "tlkmw/tlkmw.h"
#include "../inc/tlkmw_anc_sch_inner.h"

typedef struct
{
    uint16_t handle;
} AncSchBtVoice_t;

static AncSchBtVoice_t sAncSchBtVoice = {0};

/**
 * @brief       BT voice state change callback
 * @param[in]   acl_handle - ACL connection handle
 * @param[in]   sco_handle - SCO connection handle
 * @param[in]   is_start - Start flag
 * @param[in]   codec - Codec type
 * @return      None
 */
static void tlkmw_anc_sch_bt_voice_state_change_cb(uint16_t acl_handle, uint16_t sco_handle, uint8_t is_start, uint8_t codec)
{
    (void)sco_handle;
    (void)codec;
    if (is_start) {
        if (acl_handle == 0 || tlkmw_audio_btif_handle_is_ag(acl_handle) || sAncSchBtVoice.handle != 0) {
            return;
        }
        sAncSchBtVoice.handle = acl_handle;
    } else {
        if (sAncSchBtVoice.handle != acl_handle) {
            return;
        }
        sAncSchBtVoice.handle = 0;
    }
    tlkmw_anc_sch_request_update(TLKMW_ANC_SCH_CB_TYPE_BT_VOICE, acl_handle, is_start);
}

/**
 * @brief       Initialize BT voice scheduler
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_bt_voice_init(void)
{
    tlkmdi_audio_btif_addVoiceStateChgCB(tlkmw_anc_sch_bt_voice_state_change_cb);
}

/**
 * @brief       Get running handle of BT voice
 * @param       None
 * @return      Running handle
 */
uint16_t tlkmw_anc_bt_voice_getRunningHandle(void)
{
    return sAncSchBtVoice.handle;
}
#endif