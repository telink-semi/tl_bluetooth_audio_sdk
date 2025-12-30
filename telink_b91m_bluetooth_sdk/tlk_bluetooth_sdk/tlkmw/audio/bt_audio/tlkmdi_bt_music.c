/********************************************************************************************************
 * @file    tlkmdi_bt_music.c
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
#if (TLKALG_EQ_ENABLE && !TLKMW_INTERPHONE_EN)
#include "tlkalg/audio/eq/tlkalg_eq_interface.h"
#endif

#if (TLKBTP_CFG_A2DPSNK_ENABLE)

#define TLKMDI_BT_MUSIC_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_SNK << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BT_MUSIC_DBG_SIGN "[MDI BT_M]"

// #define TLKMDI_BT_MUSIC_ENC_FIFO_SIZE BT_MUSIC_ENC_FIFO_SIZE

typedef struct
{
    uint8_t  enable;
    uint8_t  codec_type;
    uint16_t acl_handle;
} tlkmdi_bt_music_env_t;

static tlkmdi_bt_music_env_t s_tlk_mdi_bt_music_env = {0};
uint8_t                      g_bt_music_enable_flag = 0;

/**
 * @brief  check if bt music is enabled
 * @param[in]  none
 * @returns bt music enable status
 */
uint16_t tlkmdi_bt_music_is_enabled(void)
{
    return s_tlk_mdi_bt_music_env.enable;
}

/**
 * @brief  receive a2dp data, called by A2DP profile
 * @param[in]  aclHandle - ACL connection handle
 * @param[in]  p_data    - A2DP data ptr
 * @param[in]  len       - A2DP packet data len
 * @returns BT_AUDIO_SUCCESS is successful, others are failed
 */
uint8_t tlkmdi_bt_music_rcv_a2dp_frame(uint16_t aclHandle, uint8_t *p_data, int16_t len)
{
    (void)aclHandle;
    return bt_music_receive_a2dp_frames(p_data, len);
}

/**
 * @brief  volume change callback function
 * @param[in]  handle - connection handle
 * @param[in]  vol    - volume value
 * @returns none
 */
static void tlkmdi_bt_music_vol_change_cb(uint16_t handle, uint8_t vol)
{
    (void)vol;
    if (handle != s_tlk_mdi_bt_music_env.acl_handle) {
        return;
    }
    bt_audio_set_music_vol_percent_by_handle(handle);
}

/**
 * @brief  state change callback function
 * @param[in]  handle - connection handle
 * @param[in]  state  - state value
 * @returns none
 */
static void tlkmdi_bt_music_state_change_cb(uint16_t handle, uint8_t state)
{
    if (state == TLK_STATE_OPENED) {
        if (!s_tlk_mdi_bt_music_env.enable) {
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_CC_BT_MUSIC, handle);
        } else if (btp_a2dpsnk_getStatus(handle) == BTP_A2DP_STATUS_STREAM) {
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_CC_BT_MUSIC, handle);
        }
    } else if (state == TLK_STATE_PAUSED || state == TLK_STATE_CLOSED) {
        tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_CC_BT_MUSIC, handle);
    }
}

/**
 * @brief  initial the A2DP sink block, register the data callback
 * @param[in]  none
 * @returns TLK_ENONE is success,other value is false
 */
int tlkmdi_bt_music_init(void)
{
    tlkapi_trace(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "tlkmdi_bt_music_init");

    tlkmdi_audio_btif_addMusicStateChgCB(tlkmdi_bt_music_state_change_cb, false);
    tlkmdi_audio_btif_regMusicVolChgCB(tlkmdi_bt_music_vol_change_cb);
    return TLK_ENONE;
}

/**
 * @brief  start bt music
 * @param[in]  handle - connection handle
 * @param[in]  param  - start parameter
 * @returns TLK_ENONE is success,other value is false
 */
int tlkmdi_bt_music_start(uint16_t handle, uint32_t param)
{
    (void)param;
    if (s_tlk_mdi_bt_music_env.enable) {
        return -TLK_EREPEAT;
    }

    tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PLAY);

    uint8_t start_mode = (param >> 28); //high 4bit
    if (start_mode == TLKAUD_START_MODE_PLAY_PREV) {
        tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_BACKWARD);
    } else if (start_mode == TLKAUD_START_MODE_PLAY_NEXT) {
        tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_FORWARD);
    }

    return TLK_ENONE;
}

/**
 * @brief  close bt music
 * @param[in]  handle - connection handle
 * @returns TLK_ENONE is success,other value is false
 */
int tlkmdi_bt_music_close(uint16_t handle)
{
    tlkapi_trace(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "tlkmdi_bt_music_close");

    tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PAUSE);

    return TLK_ENONE;
}

/**
 * @brief  play next song
 * @param[in]  none
 * @returns TLK_ENONE is success,other value is false
 */
bool tlkmdi_bt_music_next(void)
{
    if (!s_tlk_mdi_bt_music_env.enable) {
        return -TLK_EREPEAT;
    }

    tlkmdi_audio_btif_avrcp_sendKey(s_tlk_mdi_bt_music_env.acl_handle, AUD_BTIF_AVRCP_KEYID_FORWARD);
    return TLK_ENONE;
}

/**
 * @brief  play previous song
 * @param[in]  none
 * @returns TLK_ENONE is success,other value is false
 */
bool tlkmdi_bt_music_previous(void)
{
    if (!s_tlk_mdi_bt_music_env.enable) {
        return -TLK_EREPEAT;
    }

    tlkmdi_audio_btif_avrcp_sendKey(s_tlk_mdi_bt_music_env.acl_handle, AUD_BTIF_AVRCP_KEYID_BACKWARD);

    return TLK_ENONE;
}

/**
 * @brief  operate bt music
 * @param[in]  handle  - connection handle
 * @param[in]  opcode  - operation code
 * @param[in]  pdata   - operation data
 * @param[in]  dataLen - data length
 * @returns operation result
 */
bool tlkmdi_bt_music_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        tlkmdi_audio_btif_VolumeOperate(handle, true, true);
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        tlkmdi_audio_btif_VolumeOperate(handle, false, true);
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
    {
        return true;
    }
    default:
    {
        return false;
    } break;
    }
    return true;
}

/**
 * @brief  gpio toggle test function
 * @param[in]  times - toggle times
 * @returns none
 */
_attribute_ram_code_sec_ void gpio_toggle_test(uint8_t times)
{
    for (uint8_t i = 0; i < times; i++) {
#if TWS_AUDIO_PATH_GPIO_DEBUG
        gpio_write(GPIO_PC1, 0);
        gpio_write(GPIO_PC1, 1);
        gpio_write(GPIO_PC1, 0);
#endif
    }
}

/**
 * @brief  switch bt music state
 * @param[in]  handle - connection handle
 * @param[in]  status - switch status
 * @returns operation result
 */
bool tlkmdi_bt_music_switch(uint16_t handle, uint8_t status)
{
    tlkapi_trace(TLKMDI_BT_MUSIC_DBG_FLAG, TLKMDI_BT_MUSIC_DBG_SIGN, "tlkmdi_bt_music_switch: handle:%d, status: %d", handle, status);

    if (status == TLK_STATE_CLOSED && s_tlk_mdi_bt_music_env.acl_handle != handle) {
        return false;
    }

    if (status == TLK_STATE_OPENED) {
        g_bt_music_enable_flag = 1;
        bt_music_close_codec();
        s_tlk_mdi_bt_music_env.acl_handle = handle;
        s_tlk_mdi_bt_music_env.enable     = true;
#if TLK_CFG_HRA_ENABLE
        tlkmdi_hra_music_init();
#endif
        tlkmdi_btmusic_switch_in(handle);
    } else {
        g_bt_music_enable_flag        = 0;
        s_tlk_mdi_bt_music_env.enable = false;
        bt_music_close_codec();
        tlkmdi_btmusic_switch_out(handle);
    }

    return true;
}

/**
 * @brief  check if bt music is busy
 * @param[in]  none
 * @returns busy status
 */
_attribute_ram_code_sec_ bool tlkmdi_bt_music_is_busy(void)
{
    return s_tlk_mdi_bt_music_env.enable;
}

#endif
