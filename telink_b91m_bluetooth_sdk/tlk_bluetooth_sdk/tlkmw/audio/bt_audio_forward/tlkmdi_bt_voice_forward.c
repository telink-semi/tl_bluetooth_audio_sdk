/********************************************************************************************************
 * @file    tlkmdi_bt_voice_forward.c
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
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "tlkmw/tlkmw.h"
#if (TLKMW_BT_1_TO_2_FORWARD_EN)

typedef struct
{
    uint8_t  enable;
    uint8_t  resv;
    uint16_t aclHandle;
} tlkmdi_bt_voice_forward_ctrl_t;

static tlkmdi_bt_voice_forward_ctrl_t s_tlkmdi_bt_voice_forward_ctrl;

/**
 * @brief  voice state change callback function
 * @param[in]  acl_handle - ACL connection handle
 * @param[in]  sco_handle - SCO connection handle
 * @param[in]  is_start   - start flag
 * @param[in]  codec      - codec type
 * @returns none
 */
static void tlkmdi_bt_voice_forward_state_change_cb(uint16_t acl_handle, uint16_t sco_handle, uint8_t is_start, uint8_t codec)
{
    tlk_printf("tlkmdi_bt_voice_forward_state_change_cb acl_handle: %d, sco_handle: %d, is_start: %d, codec: %d", acl_handle, sco_handle, is_start, codec);
    (void)sco_handle;
    (void)codec;

    if (is_start) {
        if (acl_handle != btp_hfp_getAgHandle()) {
            return;
        }
        btif_set_hfp_codec(SCO_ENC_QUEUE_ID_AG, codec);
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_BT_VOICE_FORWARD, acl_handle);
    } else {
        if (s_tlkmdi_bt_voice_forward_ctrl.enable) {
            tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_BT_VOICE_FORWARD, s_tlkmdi_bt_voice_forward_ctrl.aclHandle);
        }
    }
}

int tlkmdi_bt_voice_forward_init(void)
{
    tlkmdi_audio_btif_addVoiceStateChgCB(tlkmdi_bt_voice_forward_state_change_cb);
    return TLK_ENONE;
}

/**
 * @brief  operate bt voice relay
 * @param[in]  handle  - connection handle
 * @param[in]  opcode  - operation code
 * @param[in]  pdata   - operation data
 * @param[in]  dataLen - data length
 * @returns operation result
 */
bool tlkmdi_bt_voice_forward_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    (void)handle;
    tlk_printf("tlkmdi_bt_voice_forward_operate handle:%d, opcode: %d", handle, opcode);
    uint16_t hfHandle = btp_hfp_getHfHandle();
    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        tlk_printf("ret = [%d]", tlkmdi_audio_btif_VolumeOperate(handle, true, false));
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        tlk_printf("ret = [%d]", tlkmdi_audio_btif_VolumeOperate(handle, false, false));
    } break;
    case TLKAUD_OPCODE_CALL_ACCEPT:
    {
        tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_HF_SEND_CALL_ACCEPT, &hfHandle, sizeof(hfHandle));
    } break;
    case TLKAUD_OPCODE_CALL_HUNGUP:
    {
        tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_HF_SEND_CALL_HUNGUP, &hfHandle, sizeof(hfHandle));
    } break;
    // case TLKAUD_OPCODE_IS_SUPPORT_TONE_MIX:
    //     return true;
    // case TLKAUD_OPCODE_IS_PLAY_RING:
    // {
    //     return false;
    // }
    default:
        return false;
    }
    return true;
}

bool tlkmdi_bt_voice_forward_switch(uint16_t handle, uint8_t status)
{
    (void)status;
    tlk_printf("tlkmdi_bt_voice_forward_switch handle %d status %d", handle, status);

    if (status == TLK_STATE_OPENED) {
        s_tlkmdi_bt_voice_forward_ctrl.enable = true;
        bt_voice_forward_switch_in(handle);
        //tlkmdi_btvoice_switch_in(handle);
        s_tlkmdi_bt_voice_forward_ctrl.aclHandle = handle;
    } else {
        s_tlkmdi_bt_voice_forward_ctrl.enable = false;
        bt_voice_forward_switch_out(handle);
        //tlkmdi_btvoice_switch_out(handle);
        s_tlkmdi_bt_voice_forward_ctrl.aclHandle = 0;
    }

    void app_bt_ui_forward_msg_push(uint8_t event_id, uint32_t param);
    app_bt_ui_forward_msg_push(2, status == TLK_STATE_OPENED ? 1 : 0); //APP_BT_FORWARD_MSG_VOICE_FORWARD_STATUS_CHANGE

    return true;
}

bool tlkmdi_bt_voice_forward_isBusy(void)
{
    return s_tlkmdi_bt_voice_forward_ctrl.enable;
}

#endif
