/********************************************************************************************************
 * @file    tlkmdi_tpsll.c
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
#include "stack/bt/host/btp/a2dp/btp_a2dp.h"
#include "stack/bt/host/btp/avrcp/btp_avrcp.h"
#include "stack/tpsll/tpsll.h"
#include "tlkalg/audio/lc3_plus/tlkalg_lc3_plus_interface.h"


#if (TLKSTK_BT_TPS_ENABLE)


    #ifndef APP_AUDIO_LOG_EN
        #define APP_AUDIO_LOG_EN (1)
    #else
        #undef APP_AUDIO_LOG_EN
        #define APP_AUDIO_LOG_EN (1)
    #endif

    #define TLKMDI_LL_AUDIO_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_CC_LL_AUDIO << 16) | TLK_DEBUG_DBG_FLAG_ALL)
    #define TLKMDI_LL_AUDIO_DBG_SIGN "[MDI TPSLL_AUD]"

typedef struct
{
    uint8_t  enable;
    uint16_t acl_handle;
} tlkmdi_tpsll_audio_context_t;

static tlkmdi_tpsll_audio_context_t s_tlk_mdi_tpsll_audio_ctx = {0};

/**
 * @brief      Handles tpsll audio mode change event.
 * @param[in]  mode - Audio mode
 * @return     none
 */
void tlkmdi_tpsll_audio_mode_change_cb(uint16_t mode)
{
    if (mode & TPH_HOST_MODE_DONGLE_AUDIO) {
        tlkmdi_tpsll_audio_enable();
    } else {
        tlkmdi_tpsll_audio_disable();
    }
}

/**
 * @brief      Handles tpsll mode change event.
 * @param[in]  pData - Pointer to event data
 * @param[in]  dataLen - Length of event data
 * @return     0
 */
int ll_audio_tpsll_event_mode_change_cb(uint8_t *pData, uint16_t dataLen)
{
    (void) dataLen;
    uint16_t mode = ll_audio_get_mode(pData);
    tlkapi_printf(APP_AUDIO_LOG_EN, "=== tpsll mode change: 0x%x", mode);
    const uint8_t bufferLen = sizeof(tlksys_msg_hostEvt_t)+sizeof(tlksys_msg_hostEvt_tpsAudStateChg_t);
    uint8_t buffer[bufferLen];
    tlksys_msg_hostEvt_t *evt = (tlksys_msg_hostEvt_t *)buffer;
    evt->dataLen = sizeof(tlksys_msg_hostEvt_tpsAudStateChg_t);
    evt->hostType = TLKSYS_MSG_HOST_TYPE_TPT;
    evt->msgID = TLKSYS_MSG_TPS_HOST_EVT_TYPE_AUD_STATE_CHG;
    tlksys_msg_hostEvt_tpsAudStateChg_t *evtDat = (tlksys_msg_hostEvt_tpsAudStateChg_t *) evt->data;
    evtDat->mode = mode;
    tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_HOST_EVT_COME, buffer, bufferLen);
    return 0;
}

TPSLL_EVT_REGISTER (TPSLL_EVTID_HEADSET_MODE_CHANGE, ll_audio_tpsll_event_mode_change_cb);

/**
 * @brief      Initializes TPSLL audio module
 * @param      None
 * @return     Status of initialization
 */
int tlkmdi_tpsll_audio_init(void)
{
    tlkmdi_audio_tpsif_addStateChgCB(tlkmdi_tpsll_audio_mode_change_cb);
    ll_audio_mailbox_callback_init();

    //mode change cb will happen in host thread
    //tlksys_timer_createStatic(TLKSYS_TASKID_HOST,&tlkmdi_tpsll_mode_filter_timer, 200*1000, false,tlkmdi_tpsll_mode_change_timer, NULL);
    return TLK_ENONE;
}

/**
 * @brief      Enables TPSLL audio functionality
 * @param      None
 * @return     Status of enabling operation
 */
int tlkmdi_tpsll_audio_enable(void)
{
    if (s_tlk_mdi_tpsll_audio_ctx.enable) {
        //return 0;
    }
    tlkapi_printf(APP_AUDIO_LOG_EN, "tlkmdi_tpsll_audio_enable");

    tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_TPH_AUDIO, 0xfff0);

    return s_tlk_mdi_tpsll_audio_ctx.enable;
}

/**
 * @brief      Disables TPSLL audio functionality
 * @param      None
 * @return     Status of disabling operation
 */
int tlkmdi_tpsll_audio_disable(void)
{
    if (!s_tlk_mdi_tpsll_audio_ctx.enable) {
        return 0;
    }

    tlkapi_printf(APP_AUDIO_LOG_EN, "tlkmdi_tpsll_audio_disable");

    tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_TPH_AUDIO, 0xfff0);

    return s_tlk_mdi_tpsll_audio_ctx.enable;
}

/**
 * @brief      Switches TPSLL audio connection
 * @param[in]  handle - Connection handle
 * @param[in]  status - Switch status
 * @return     True if switch successful, false otherwise
 */
bool tlkmdi_tpsll_audio_switch(uint16_t handle, uint8_t status)
{
    tlkapi_printf(APP_AUDIO_LOG_EN, "tlkmdi_ll_audio_switch, status: %d", status);

    if (status == TLK_STATE_OPENED) {
        s_tlk_mdi_tpsll_audio_ctx.enable     = true;
        s_tlk_mdi_tpsll_audio_ctx.acl_handle = handle;
        ll_audio_switch_in();
        ll_audio_open_codec();
    } else {
        s_tlk_mdi_tpsll_audio_ctx.enable = false;
        s_tlk_mdi_tpsll_audio_ctx.acl_handle = 0;
        ll_audio_switch_out();
        tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_SPK);
        tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_MIC);
        #if TLK_MW_DSP_COMM_ENABLE
        tlkmw_dsp_pause();
        #endif
    }

    return true;
}

/**
 * @brief      Checks if TPSLL audio is busy
 * @param      None
 * @return     True if busy, false otherwise
 */
bool tlkmdi_tpsll_audio_is_busy(void)
{
    return s_tlk_mdi_tpsll_audio_ctx.enable;
}

/**
 * @brief      Starts TPSLL audio stream
 * @param[in]  handle - Connection handle
 * @param[in]  param - Start parameters
 * @return     Status of start operation
 */
int tlkmdi_tpsll_audio_start(uint16_t handle, uint32_t param)
{
    (void)param;
    return tlkmdi_audio_tpsif_sendKey(handle,AUD_TPSIF_KEYID_PLAY_PAUSE);
}

/**
 * @brief      Closes TPSLL audio stream
 * @param[in]  handle - Connection handle
 * @return     Status of close operation
 */
int tlkmdi_tpsll_audio_close(uint16_t handle)
{
    return tlkmdi_audio_tpsif_sendKey(handle,AUD_TPSIF_KEYID_PLAY_PAUSE);
}

/**
 * @brief      Moves to next TPSLL audio item
 * @param      None
 * @return     True if next item exists, false otherwise
 */
bool tlkmdi_tpsll_audio_next(void)
{
    int ret = tlkmdi_audio_tpsif_sendKey(s_tlk_mdi_tpsll_audio_ctx.acl_handle,AUD_TPSIF_KEYID_PLAY_FORWARD);
    if (ret == TLK_ENONE) {
        return true;
    } 
    return false;
}

/**
 * @brief      Moves to previous TPSLL audio item
 * @param      None
 * @return     True if previous item exists, false otherwise
 */
bool tlkmdi_tpsll_audio_previous(void)
{
    int ret = tlkmdi_audio_tpsif_sendKey(s_tlk_mdi_tpsll_audio_ctx.acl_handle,AUD_TPSIF_KEYID_PLAY_BACKWARD);
    if (ret == TLK_ENONE) {
        return true;
    } 
    return false;
}

/**
 * @brief      Processes TPSLL audio operations
 * @param[in]  handle - Connection handle
 * @param[in]  opcode - Operation code
 * @param[in]  pdata - Data buffer
 * @param[in]  dataLen - Length of data
 * @return     True if operation processed successfully, false otherwise
 */
bool tlkmdi_tpsll_audio_process_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    (void)handle;
    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        tlkmdi_audio_tpsif_sendKey(s_tlk_mdi_tpsll_audio_ctx.acl_handle,AUD_TPSIF_KEYID_VOL_UP);
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        tlkmdi_audio_tpsif_sendKey(s_tlk_mdi_tpsll_audio_ctx.acl_handle,AUD_TPSIF_KEYID_VOL_DOWN);
    } break;
    case TLKAUD_OPCODE_TONE_MIX_ENABLE:
    {
        tlkmdi_audio_set_tone_mix_mode(1);
    } break;
    case TLKAUD_OPCODE_TONE_MIX_DISABLE:
    {
        tlkmdi_audio_set_tone_mix_mode(0);
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
#endif
