/********************************************************************************************************
 * @file    tlkmdi_bt_tph_key.c
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
#include "tlkapi/tlkapi.h"
#if (TLKSTK_BT_TPS_ENABLE)
#include "tlkmw/tlkmw.h"
#include "stack/tpsll/tpsll.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"


#define TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG ((TLK_MAJOR_DBGID_MW << 24) | (TLK_MINOR_DBGID_MDI_TPSLL_CCHEADSET << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTACL_DBG_SIGN            "[TPSLL_KEY]"


static TlkApiTimer_t tlkmdi_bt_tpsll_keyTimer;

/**
 * @brief       This function sends a key message via the host HAL interface.
 * @param[in]   cmd     - the command to be sent.
 * @param[in]   pData   - the data associated with the command.
 * @param[in]   dataLen - the length of the data.
 * @return      none.
 * @note        
 */
static void tlkmdi_bt_tph_sendKeyMsg(uint8_t cmd, uint8_t *pData, uint8_t dataLen)
{
    tph_host_hal_send_pdu_msg(cmd, pData, dataLen, NULL);
}

/**
 * @brief       This function handles a key press event by sending the key data.
 * @param[in]   keyData - the data representing the key press.
 * @param[in]   dataLen - the length of the key data.
 * @return      none.
 * @note        
 */
static void tlkmdi_bt_tph_keyPress(uint8_t *keyData, uint8_t dataLen)
{
    tlkmdi_bt_tph_sendKeyMsg(TPH_HOST_MSG_PDU_ACL_CMD_UI, keyData, dataLen);
    tlkapi_array(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tpsll key press", keyData, dataLen);
}

/**
 * @brief       This function handles a key release event by sending the appropriate key release command.
 * @return      none.
 * @note        
 */
static void tlkmdi_bt_tph_keyRelease(void)
{
    uint8_t key[2] = {TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY, TLK_MDI_HID_AUDIO_RELEASE};
    tlkmdi_bt_tph_sendKeyMsg(TPH_HOST_MSG_PDU_ACL_CMD_UI, key, sizeof(key));
    tlkapi_array(TLKMDI_BT_TPSLL_HEADSET_DBG_FLAG, TLKMDI_BTACL_DBG_SIGN, "tpsll key release", key, sizeof(key));
}

/**
 * @brief       This function is called when a key release timer expires.
 * @param[in]   pTimer  - the timer handle.
 * @param[in]   userArg - user argument passed to the timer callback.
 * @return      none.
 * @note        
 */
static void tlkmdi_bt_tph_keyTimerFunc(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    tlkmdi_bt_tph_keyRelease();
}

/**
 * @brief       This function schedules a key release event to occur after a specified delay.
 * @param[in]   waitUs  - the delay in microseconds before the key release.
 * @return      none.
 * @note        
 */
static void tlkmdi_bt_tph_keyReleaseLater(uint32_t waitUs)
{
    if (waitUs <= 50) {
        //too short release now.
        tlkmdi_bt_tph_keyRelease();
    } else {
        tlksys_timer_setPeriod(TLKSYS_TASKID_HOST, &tlkmdi_bt_tpsll_keyTimer, waitUs);
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &tlkmdi_bt_tpsll_keyTimer);
    }
}

/**
 * @brief       This function initializes the key handling system by creating a static timer.
 * @return      TLK_ENONE if successful, error code otherwise.
 * @note        
 */
int tlkmdi_bt_tph_key_init(void)
{
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &tlkmdi_bt_tpsll_keyTimer, 1000, false, tlkmdi_bt_tph_keyTimerFunc, NULL);
    return TLK_ENONE;
}

/**
 * @brief       key event - tpsll_call_answer_end.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncCallAnswerEnd(void)
{
    uint8_t key[2] = {TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY, TLK_MDI_HID_AUDIO_PAUSE};
    tlkmdi_bt_tph_keyPress(key, sizeof(key));
    tlkmdi_bt_tph_keyReleaseLater(0);
    return TLK_ENONE;
}

/**
 * @brief       key event - tpsll_call_reject.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncCallReject(void)
{
    uint8_t key[2] = {TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY, TLK_MDI_HID_AUDIO_PAUSE};
    tlkmdi_bt_tph_keyPress(key, sizeof(key));
    tlkmdi_bt_tph_keyReleaseLater(2000 * 1000);
    return TLK_ENONE;
}

/**
 * @brief       key event - tpsll_audio_volume_up.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncAudioVolumeUp(void)
{
    uint8_t key[2] = {TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY, TLK_MDI_HID_AUDIO_VOL_UP};
    tlkmdi_bt_tph_keyPress(key, sizeof(key));
    tlkmdi_bt_tph_keyReleaseLater(0);
    return TLK_ENONE;
}

/**
 * @brief       key event - tpsll_audio_volume_down.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncAudioVolumeDown(void)
{
    uint8_t key[2] = {TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY, TLK_MDI_HID_AUDIO_VOL_DOWN};
    tlkmdi_bt_tph_keyPress(key, sizeof(key));
    tlkmdi_bt_tph_keyReleaseLater(0);
    return TLK_ENONE;
}

/**
 * @brief       key event - tpsll_music_play_pause.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncMusicPlayPause(void)
{
    uint8_t key[2] = {TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY, TLK_MDI_HID_AUDIO_PLAY_PAUSE};
    tlkmdi_bt_tph_keyPress(key, sizeof(key));
    tlkmdi_bt_tph_keyReleaseLater(10 * 1000);
    return TLK_ENONE;
}

/**
 * @brief       key event - tpsll_music_backward.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncMusicBackward(void)
{
    uint8_t key[2] = {TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY, TLK_MDI_HID_AUDIO_BACKWARD};
    tlkmdi_bt_tph_keyPress(key, sizeof(key));
    tlkmdi_bt_tph_keyReleaseLater(10 * 1000);
    return TLK_ENONE;
}

/**
 * @brief       key event - tpsll_music_forward.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncMusicForward(void)
{
    uint8_t key[2] = {TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY, TLK_MDI_HID_AUDIO_FORWARD};
    tlkmdi_bt_tph_keyPress(key, sizeof(key));
    tlkmdi_bt_tph_keyReleaseLater(10 * 1000);
    return TLK_ENONE;
}

/**
 * @brief       key event - tpsll_save_pair_info.
 * @returns     0.
 */
int tlkmdi_bt_tph_keyFuncSavePairInfo(void)
{
    uint8_t key[2] = {TLK_MDI_DONGLE_ACL_CMD_MEDIA_KEY, TLK_MDI_HID_AUDIO_FORWARD};
    tlkmdi_bt_tph_keyPress(key, sizeof(key));
    tlkmdi_bt_tph_keyReleaseLater(10 * 1000);
    return TLK_ENONE;
}

#endif // #if (TLK_STK_BT_TPSLL_ENABLE)
