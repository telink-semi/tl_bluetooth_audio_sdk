/********************************************************************************************************
 * @file    tlkmw_anc_sch_ui_logic.c
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

/**
 * @brief       Core function for next/previous track control
 * @param[in]   isNext - Next track flag (1 for next, 0 for previous)
 * @return      None
 */
static void tlkmw_anc_sch_ui_next_prev_core(uint8_t isNext)
{
    if(tlkmw_anc_sch_isRunning() == 0){
        return;
    }
    if(tlkmw_anc_bt_voice_getRunningHandle()){
        return;
    }
    uint16_t handle = tlkmw_anc_bt_music_getRunningHandle();
    if(handle){
        uint8_t key = isNext ? AUD_BTIF_AVRCP_KEYID_FORWARD : AUD_BTIF_AVRCP_KEYID_BACKWARD;
        tlkmdi_audio_btif_avrcp_sendKey(handle, key);
        return;
    }
    handle = tlkmw_anc_tpsll_getRunningHandle();
    if(handle){
        uint8_t key = isNext ? AUD_TPSIF_KEYID_PLAY_FORWARD : AUD_TPSIF_KEYID_PLAY_BACKWARD;
        tlkmdi_audio_tpsif_sendKey(handle, key);   
    }
}

/**
 * @brief       Core function for volume control
 * @param[in]   isInc - Volume increment flag (1 for up, 0 for down)
 * @return      None
 */
static void tlkmw_anc_sch_ui_volume_core(uint8_t isInc)
{
    if(tlkmw_anc_sch_isRunning() == 0){
        return;
    }
    uint16_t handle = tlkmw_anc_bt_voice_getRunningHandle();
    if(handle){
        tlkmdi_audio_btif_VolumeOperate(handle, isInc, false);
        return;
    }
    handle = tlkmw_anc_bt_music_getRunningHandle();
    if(handle){
        tlkmdi_audio_btif_VolumeOperate(handle, isInc, true);
        return;
    }
    handle = tlkmw_anc_tpsll_getRunningHandle();
    if(handle){
        uint8_t key = isInc ? AUD_TPSIF_KEYID_VOL_UP : AUD_TPSIF_KEYID_VOL_DOWN;
        tlkmdi_audio_tpsif_sendKey(handle,key);
    }
}

/**
 * @brief       Core function for call control
 * @param[in]   isAccept - Call accept flag (1 for accept, 0 for reject)
 * @return      None
 */
static void tlkmw_anc_sch_ui_call_core(uint8_t isAccept)
{
    if(tlkmw_anc_sch_isRunning() == 0){
        return;
    }
    uint16_t handle = tlkmw_anc_bt_voice_getRunningHandle();
    if(handle == 0){
        return;
    }
    uint16_t msgID = isAccept ? TLKSYS_BT_MSGID_HF_SEND_CALL_ACCEPT : TLKSYS_BT_MSGID_HF_SEND_CALL_HUNGUP;
    tlksys_sendMsg(TLKSYS_TASKID_HOST, msgID, &handle, sizeof(handle));
}

/**
 * @brief       UI function for next track
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_next(void)
{
    tlkmw_anc_sch_ui_next_prev_core(1);
}

/**
 * @brief       UI function for previous track
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_prev(void)
{
    tlkmw_anc_sch_ui_next_prev_core(0);
}

/**
 * @brief       UI function for volume up
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_volume_up(void)
{
    tlkmw_anc_sch_ui_volume_core(1);
}

/**
 * @brief       UI function for volume down
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_volume_down(void)
{
    tlkmw_anc_sch_ui_volume_core(0);
}

/**
 * @brief       UI function for play/pause control
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_playpause(void)
{
    if(tlkmw_anc_sch_isRunning() == 0){
        return;
    }
    uint16_t handle = tlkmw_anc_bt_voice_getRunningHandle();
    if(handle){
        return;
    }
    handle = tlkmw_anc_bt_music_getRunningHandle();
    if(handle){
        tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PAUSE);
        return;
    }
    handle = tlkmw_anc_tpsll_getRunningHandle();
    if(handle){
        tlkmdi_audio_tpsif_sendKey(handle,AUD_TPSIF_KEYID_PLAY_PAUSE);
        return;
    }
    handle = tlkmw_anc_bt_music_getFirstPausedHandle();
    if(handle){
        tlkmdi_audio_btif_avrcp_sendKey(handle, AUD_BTIF_AVRCP_KEYID_PLAY);
        return;
    }
    tlkmdi_audio_tpsif_sendKey(0xFFF0,AUD_TPSIF_KEYID_PLAY_PAUSE);
}

/**
 * @brief       UI function for accepting call
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_call_accept(void)
{
    tlkmw_anc_sch_ui_call_core(1);
}

/**
 * @brief       UI function for rejecting call
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_ui_call_reject(void)
{
    tlkmw_anc_sch_ui_call_core(0);
}

#endif 