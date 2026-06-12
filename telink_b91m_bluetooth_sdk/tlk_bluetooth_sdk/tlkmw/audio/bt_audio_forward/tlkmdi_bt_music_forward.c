/********************************************************************************************************
 * @file    tlkmdi_bt_music_forward.c
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
} tlkmdi_bt_music_forward_ctrl_t;

static tlkmdi_bt_music_forward_ctrl_t s_tlkmdi_bt_music_forward_ctrl;

uint16_t tlkmdi_bt_music_forward_get_acl_handle(void)
{
    return s_tlkmdi_bt_music_forward_ctrl.aclHandle;
}

static int tlkmdi_bt_music_forward_a2dpStatusEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    btp_a2dpStatusChangeEvt_t *pEvt;

    pEvt = (btp_a2dpStatusChangeEvt_t *)pData;

    tlk_printf("tlkmdi_bt_music_forward_a2dpStatusEvt, aclHandle[%d], enable[%d], status[%d]", s_tlkmdi_bt_music_forward_ctrl.aclHandle, s_tlkmdi_bt_music_forward_ctrl.enable,
               pEvt->status);

    if (pEvt->status == BTP_A2DP_STATUS_STREAM) {
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_BT_MUSIC_FORWARD, pEvt->handle);
    } else {
        btp_avrcp_notifyPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PAUSED);
        if (s_tlkmdi_bt_music_forward_ctrl.enable) {
            tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_BT_MUSIC_FORWARD, s_tlkmdi_bt_music_forward_ctrl.aclHandle);
        }
    }
    return TLK_ENONE;
}

BTP_EVT_REGISTER(BTP_EVTID_A2DPSRC_STATUS_CHANGED, tlkmdi_bt_music_forward_a2dpStatusEvt);

/**
 * @brief  state change callback function
 * @param[in]  handle - connection handle
 * @param[in]  state  - state value
 * @returns none
 */
static void tlkmdi_bt_music_forward_state_change_cb(uint16_t handle, uint8_t state)
{
    tlk_printf("tlkmdi_bt_music_forward_state_change_cb handle = %d, state = %d", handle, state);

    if (state == TLK_STATE_OPENED) {
        if (btp_a2dp_isSnk(handle)) {
            return;
        }

        if (!s_tlkmdi_bt_music_forward_ctrl.enable) {
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_BT_MUSIC_FORWARD, handle);
        } else if (btp_a2dpsrc_getStatus(handle) == BTP_A2DP_STATUS_STREAM) { //TODO: Need check.
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_BT_MUSIC_FORWARD, handle);
        }
    } else {
        if (s_tlkmdi_bt_music_forward_ctrl.enable) {
            tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_BT_MUSIC_FORWARD, s_tlkmdi_bt_music_forward_ctrl.aclHandle);
        }
    }
}

/**
 * @brief  volume change callback function
 * @param[in]  handle - connection handle
 * @param[in]  vol    - volume value
 * @returns none
 */
static void tlkmdi_bt_music_forward_vol_change_cb(uint16_t handle, uint8_t vol)
{
    tlk_printf("tlkmdi_bt_music_forward_vol_change_cb handle = %d, vol = %d", handle, vol);
    (void)vol;
    if (handle != btp_a2dp_getSnkHandle()) {
        return;
    }
    bt_audio_set_music_vol_percent_by_handle(handle);
}

int tlkmdi_bt_music_forward_init(void)
{
    tlkmdi_audio_btif_addMusicStateChgCB(tlkmdi_bt_music_forward_state_change_cb, false);
    tlkmdi_audio_btif_regMusicVolChgCB(tlkmdi_bt_music_forward_vol_change_cb);

    return TLK_ENONE;
}

/**
 * @brief  start bt music
 * @param[in]  handle - connection handle
 * @param[in]  param  - start parameter
 * @returns TLK_ENONE is success,other value is false
 */
int tlkmdi_bt_music_forward_start(uint16_t handle, uint32_t param)
{
    (void)handle;
    (void)param;
    if (s_tlkmdi_bt_music_forward_ctrl.enable) {
        return -TLK_EREPEAT;
    }

    uint16_t snkHandle = btp_a2dp_getSnkHandle();

    if (snkHandle != 0) {
        tlkmdi_audio_btif_avrcp_sendKey(snkHandle, AUD_BTIF_AVRCP_KEYID_PLAY);

        uint8_t start_mode = (param >> 28); //high 4bit
        if (start_mode == TLKAUD_START_MODE_PLAY_PREV) {
            tlkmdi_audio_btif_avrcp_sendKey(snkHandle, AUD_BTIF_AVRCP_KEYID_BACKWARD);
        } else if (start_mode == TLKAUD_START_MODE_PLAY_NEXT) {
            tlkmdi_audio_btif_avrcp_sendKey(snkHandle, AUD_BTIF_AVRCP_KEYID_FORWARD);
        }
    }
    return TLK_ENONE;
}

/**
 * @brief  close bt music
 * @param[in]  handle - connection handle
 * @returns TLK_ENONE is success,other value is false
 */
int tlkmdi_bt_music_forward_close(uint16_t handle)
{
    (void)handle;
    uint16_t snkHandle = btp_a2dp_getSnkHandle();
    if (snkHandle != 0) {
        tlkmdi_audio_btif_avrcp_sendKey(snkHandle, AUD_BTIF_AVRCP_KEYID_PAUSE);
    }
    return TLK_ENONE;
}

/**
 * @brief  play next song
 * @param[in]  none
 * @returns TLK_ENONE is success,other value is false
 */
bool tlkmdi_bt_music_forward_next(void)
{
    if (!s_tlkmdi_bt_music_forward_ctrl.enable) {
        return -TLK_EREPEAT;
    }

    uint16_t snkHandle = btp_a2dp_getSnkHandle();
    if (snkHandle != 0) {
        tlkmdi_audio_btif_avrcp_sendKey(snkHandle, AUD_BTIF_AVRCP_KEYID_FORWARD);
    }
    return TLK_ENONE;
}

/**
 * @brief  play previous song
 * @param[in]  none
 * @returns TLK_ENONE is success,other value is false
 */
bool tlkmdi_bt_music_forward_previous(void)
{
    if (!s_tlkmdi_bt_music_forward_ctrl.enable) {
        return -TLK_EREPEAT;
    }

    uint16_t snkHandle = btp_a2dp_getSnkHandle();
    if (snkHandle != 0) {
        tlkmdi_audio_btif_avrcp_sendKey(snkHandle, AUD_BTIF_AVRCP_KEYID_BACKWARD);
    }
    return TLK_ENONE;
}

/**
 * @brief  operate bt music forward
 * @param[in]  handle  - connection handle
 * @param[in]  opcode  - operation code
 * @param[in]  pdata   - operation data
 * @param[in]  dataLen - data length
 * @returns operation result
 */
bool tlkmdi_bt_music_forward_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)opcode;
    (void)dataLen;
    (void)handle;
    tlk_printf("tlkmdi_bt_music_forward_operate handle %d opcode %d", handle, opcode);

    uint16_t snkHandle = btp_a2dp_getSnkHandle();

    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        tlkmdi_audio_btif_VolumeOperate(snkHandle, true, true);
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        tlkmdi_audio_btif_VolumeOperate(snkHandle, false, true);
    } break;
    default:
    {
        return false;
    } break;
    }
    return true;
}

void tlkmdi_bt_music_forward_receive_a2dp_frame(uint16_t aclHandle, uint8_t *p_data, uint16_t len)
{
    (void)p_data;
    (void)len;

    if (aclHandle != btp_a2dp_getSnkHandle()) {
        tlk_printf("a2dp frame handle error aclHandle:%d, needHandle:%d", aclHandle, btp_a2dp_getSnkHandle());
        return;
    }

    // tlkapi_array(0xffffffff, "[TEST]", "a2dp frame:", p_data, len);
    uint16_t seqNumber = 0;
    uint32_t timeStamp = 0;

    ARRAY_TO_UINT16H(p_data, 2, seqNumber);
    ARRAY_TO_UINT32H(p_data, 4, timeStamp);

    btp_a2dpsrc_sendMediaData(s_tlkmdi_bt_music_forward_ctrl.aclHandle, seqNumber, timeStamp, p_data + 12, len - 12);
}

bool tlkmdi_bt_music_forward_switch(uint16_t handle, uint8_t status)
{
    (void)status;
    tlk_printf("tlkmdi_bt_music_forward_switch handle %d status %d", handle, status);

    if (status == TLK_STATE_OPENED) {
        s_tlkmdi_bt_music_forward_ctrl.enable    = true;
        s_tlkmdi_bt_music_forward_ctrl.aclHandle = handle;

        bt_music_forward_switch_in(handle);
        // #if (TLKBTP_CFG_A2DPSNK_ENABLE)
        // btp_a2dpsnk_regRecvDataCB(tlkmdi_bt_music_forward_receive_a2dp_frame);
        // #endif
    } else {
        s_tlkmdi_bt_music_forward_ctrl.enable    = false;
        s_tlkmdi_bt_music_forward_ctrl.aclHandle = 0;
        bt_music_forward_switch_out(handle);
        // #if (TLKBTP_CFG_A2DPSNK_ENABLE)
        // btp_a2dpsnk_regRecvDataCB(NULL);
        // #endif
    }

    void app_bt_ui_forward_msg_push(uint8_t event_id, uint32_t param);
    app_bt_ui_forward_msg_push(4, status == TLK_STATE_OPENED ? 1 : 0); //APP_BT_FORWARD_MSG_MUSIC_FORWARD_STATUS_CHANGE

    return true;
}

bool tlkmdi_bt_music_forward_isBusy(void)
{
    return s_tlkmdi_bt_music_forward_ctrl.enable;
}

#endif
