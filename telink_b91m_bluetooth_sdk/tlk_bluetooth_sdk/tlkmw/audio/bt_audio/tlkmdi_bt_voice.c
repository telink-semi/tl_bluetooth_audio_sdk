/********************************************************************************************************
 * @file    tlkmdi_bt_voice.c
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
#include "stack/bt/host/btp/a2dp/btp_a2dp.h"
#include "stack/bt/host/btp/avrcp/btp_avrcp.h"
#include "tlkmw/tlkmw.h"

#if (TLKBTP_CFG_HFP_ENABLE)
#define TLKMDI_BT_VOICE_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_CC_BT_AUDIO << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BT_VOICE_DBG_SIGN "[MDI BT_V]"

typedef struct
{
    uint8_t  enable;
    uint8_t  codec;
    uint16_t acl_handle;
} tlkmdi_bt_voice_env_t;

static tlkmdi_bt_voice_env_t s_tlk_mdi_bt_voice_env = {0};

#if AUDIO_TWS_MODE
extern void tlkmdi_mailbox_audio_sync_callback(void *p_data, uint16_t len);
extern void tlkmdi_mailbox_tws_slave_audio_sync_callback(void *p_data, uint16_t len);
#endif

/**
 * @brief  Check whether device status interception is required.
 * @param[in]  pData - data pointer
 * @param[in]  dataLen - data length
 * @returns intercept flag
 */
__attribute__((weak)) bool tlkmdi_bt_voice_is_intercept(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    return false;
}

/**
 * @brief  Notify bt voice status to others.
 * @param[in]  pData - data pointer
 * @param[in]  dataLen - data length
 * @returns    None
 */
__attribute__((weak)) void tlkmdi_bt_voice_status_notify(uint8_t status)
{
    (void)status;
}

/**
 * @brief  voice state change callback function
 * @param[in]  acl_handle - ACL connection handle
 * @param[in]  sco_handle - SCO connection handle
 * @param[in]  is_start   - start flag
 * @param[in]  codec      - codec type
 * @returns none
 */
static void tlkmdi_bt_voice_state_change_cb(uint16_t acl_handle, uint16_t sco_handle, uint8_t is_start, uint8_t codec)
{
    tlk_printf("tlkmdi_bt_voice_state_change_cb acl_handle: %d, sco_handle: %d, is_start: %d, codec: %d", acl_handle, sco_handle, is_start, codec);
    (void)sco_handle;
    if (tlkmdi_bt_voice_is_intercept((uint8_t *)&acl_handle, sizeof(uint16_t)) == true) {
        return;
    }

    if (is_start) {
        s_tlk_mdi_bt_voice_env.acl_handle = acl_handle;
        s_tlk_mdi_bt_voice_env.codec      = codec;

        if (tlkmdi_bt_tpt_isSlave()) {
            s_tlk_mdi_bt_voice_env.codec = btif_get_hfp_codec(SCO_ENC_QUEUE_ID_HF);
        } else {
            btif_set_hfp_codec(SCO_ENC_QUEUE_ID_HF, codec);
        }

        tlkapi_trace(TLKMDI_BT_VOICE_DBG_FLAG, TLKMDI_BT_VOICE_DBG_SIGN, "tlkmdi_bt_voice_enable, codec: %d", s_tlk_mdi_bt_voice_env.codec);

#if (!(TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE))
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_CC_BT_VOICE, acl_handle);
#endif
    } else {
        tlkapi_trace(TLKMDI_BT_VOICE_DBG_FLAG, TLKMDI_BT_VOICE_DBG_SIGN, "tlkmdi_bt_voice_disable");
        tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_CC_BT_VOICE, acl_handle);
        s_tlk_mdi_bt_voice_env.acl_handle = 0;
        s_tlk_mdi_bt_voice_env.codec      = 0;
    }
}

/**
 * @brief  voice volume change callback function
 * @param[in]  handle - connection handle
 * @param[in]  vol    - volume value
 * @returns none
 */
static void tlkmdi_bt_voice_vol_change_cb(uint16_t handle, uint8_t vol)
{
    (void)vol;
    if (handle != s_tlk_mdi_bt_voice_env.acl_handle) {
        return;
    }
    bt_audio_set_voice_vol_percent_by_handle(handle);
}

/**
 * @brief  initial bt voice module
 * @param[in]  none
 * @returns initialization result, TLK_ENONE means success
 */
int tlkmdi_bt_voice_init(void)
{
    tlkmdi_audio_btif_addVoiceStateChgCB(tlkmdi_bt_voice_state_change_cb);
    tlkmdi_audio_btif_regVoiceVolChgCB(tlkmdi_bt_voice_vol_change_cb);
    btif_register_sco_data_callback();

    return TLK_ENONE;
}

/**
 * @brief  switch bt voice state
 * @param[in]  handle - connection handle
 * @param[in]  status - switch status
 * @returns operation result
 */
bool tlkmdi_bt_voice_switch(uint16_t handle, uint8_t status)
{
    tlkapi_trace(TLKMDI_BT_VOICE_DBG_FLAG, TLKMDI_BT_VOICE_DBG_SIGN, "tlkmdi_bt_voice_switch, status: %d", status);

    if (status == TLK_STATE_OPENED) {
        s_tlk_mdi_bt_voice_env.enable = true;
#if TLK_CFG_HRA_ENABLE
        tlkmdi_hra_voice_init();
#endif
        tlkmdi_btvoice_switch_in(handle);
#if (!(TLKMW_RECORDING_CARD_EN && TLKBTP_CFG_HFPAG_ENABLE) || !TLKCFG_MULTI_MIC_EN)
        bt_voice_open_codec();
#endif

    } else {
        s_tlk_mdi_bt_voice_env.enable = false;

#if ((TLK_MW_DSP_COMM_ENABLE && !TLKADU_MIDBUF_ENABLE) && !TLK_CFG_HRA_ENABLE && !TLKMW_RECORDING_CARD_EN)
        tlkmw_dsp_pause();
        ipc_msg_register_data_process_done_cb(NULL, BT_VOICE, BT_VOICE_ID);
#endif
        tlkmdi_btvoice_switch_out(handle);
#if (!(TLKMW_RECORDING_CARD_EN && TLKBTP_CFG_HFPAG_ENABLE) || !TLKCFG_MULTI_MIC_EN)
        bt_voice_close_codec();
#endif
    }

#if (TLKMW_BT_1_TO_2_FORWARD_EN)
    tlkmdi_bt_voice_status_notify(status == TLK_STATE_OPENED ? 1 : 0);
#endif

    return true;
}

/**
 * @brief  check if bt voice is busy
 * @param[in]  none
 * @returns busy status
 */
_attribute_ram_code_sec_ bool tlkmdi_bt_voice_is_busy(void)
{
    return s_tlk_mdi_bt_voice_env.enable;
}

/**
 * @brief  operate bt voice
 * @param[in]  handle  - connection handle
 * @param[in]  opcode  - operation code
 * @param[in]  pdata   - operation data
 * @param[in]  dataLen - data length
 * @returns operation result
 */
bool tlkmdi_bt_voice_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    (void)handle;
    tlkapi_trace(TLKMDI_BT_VOICE_DBG_FLAG, TLKMDI_BT_VOICE_DBG_SIGN, "tlkmdi_bt_voice_operate opcode: %d", opcode);
    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        tlkmdi_audio_btif_VolumeOperate(handle, true, false);
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        tlkmdi_audio_btif_VolumeOperate(handle, false, false);
    } break;
    case TLKAUD_OPCODE_CALL_ACCEPT:
    {
        tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_HF_SEND_CALL_ACCEPT, &handle, sizeof(handle));
    } break;
    case TLKAUD_OPCODE_CALL_HUNGUP:
    {
        tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_HF_SEND_CALL_HUNGUP, &handle, sizeof(handle));
    } break;
    case TLKAUD_OPCODE_IS_SUPPORT_TONE_MIX:
        return true;
    case TLKAUD_OPCODE_IS_PLAY_RING:
    {
        if (s_tlk_mdi_bt_voice_env.enable) {
            if (handle != s_tlk_mdi_bt_voice_env.acl_handle) {
                tone_play(TONE_RING);
            }
            return true;
        }
        return false;
    }
    default:
        return false;
    }
    return true;
}

#endif
