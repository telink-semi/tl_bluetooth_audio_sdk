/********************************************************************************************************
 * @file    tlkapp_sysKey.c
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
#include "tlkapp/tlkapp.h"
#if (TLK_DEV_KEY_ENABLE)

typedef struct
{
    uint16_t taskID;
    uint16_t msgID;
} key_msg_t;

static const key_msg_t sTlkappSysKeyMsgTable[KEY_EVT_MODE_MAX] = {
    [KEY_EVT_MODE_NONE]                      = {.taskID = TLKSYS_TASKID_MAXNUM, .msgID = 0},
    [KEY_EVT_MODE_ENABLE_PAIRING_MODE]       = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_BT_MSGID_START_PAIR},
    [KEY_EVT_MODE_CALL_ACCEPT]               = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_CALL_ACCEPT},
    [KEY_EVT_MODE_CALL_HUNG_UP]              = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_CALL_HUNGUP},
    [KEY_EVT_MODE_VOLUME_DOWN]               = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_DEC_VOLUME},
    [KEY_EVT_MODE_VOLUME_UP]                 = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_INC_VOLUME},
    [KEY_EVT_MODE_MUSIC_FORWARD]             = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_PLAY_NEXT},
    [KEY_EVT_MODE_MUSIC_BACKWARD]            = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_PLAY_PREV},
    [KEY_EVT_MODE_MUSIC_START]               = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_PLAY_START},
    [KEY_EVT_MODE_MUSIC_CLOSE]               = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_PLAY_CLOSE},
    [KEY_EVT_MODE_MUSIC_PLAY_PAUSE]          = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_PLAY_PAUSE},
    [KEY_EVT_MODE_MUSIC_SWITCH]              = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_DONGLE_SWITCH},
    [KEY_EVT_MODE_ANC_SWITCH]                = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_TOGGLE_ANC},
    [KEY_EVT_MODE_BT_TPSLL_PAIRING_MODE_3S]  = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_TPH_MSGID_3S_PAIR},
    [KEY_EVT_MODE_BT_TPSLL_PAIRING_MODE_10S] = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_TPH_MSGID_10S_PAIR},
    [KEY_EVT_MODE_TRIGGER_SIRI]              = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_BT_MSGID_TRIGGER_SIRI},
    [KEY_EVT_MODE_TPD_ENABLE_PAIRING_MODE]   = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_TPD_MSGID_START_PAIR},
    // debug for tpsll dongle
    [KEY_EVT_MODE_TPD_USB_MODE_SWITCH] = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_TPD_MSGID_SWITCH_USB_MODE},
    [KEY_EVT_MODE_BTTPSLL_MIC_SWITCH]  = {.taskID = TLKSYS_TASKID_AUDIO, .msgID = TLKSYS_AUD_MSGID_SWITCH_MIC},
    // debug for tws master/slave sync
    [KEY_EVT_MODE_BT_TPT_PAIRING_MODE_3S]   = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_TPT_MSGID_3S_PAIR},
    [KEY_EVT_MODE_BT_TPT_PAIRING_MODE_10S]  = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_TPT_MSGID_10S_PAIR},
    [KEY_EVT_MODE_BT_TPT_ENTER_LOW_LATENCY] = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_TPT_MSGID_ENTER_LOW_LATENCY_MODE},
    [KEY_EVT_MODE_BT_TPT_START_HANDOVER]    = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_TPT_MSGID_START_HANDOVER},
    [KEY_EVT_VENDOR_CONFIG_1]               = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_1},
    [KEY_EVT_VENDOR_CONFIG_2]               = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_2},
    [KEY_EVT_VENDOR_CONFIG_3]               = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_3},
    [KEY_EVT_VENDOR_CONFIG_4]               = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_4},
    [KEY_EVT_VENDOR_CONFIG_5]               = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_5},
    [KEY_EVT_VENDOR_CONFIG_6]               = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_6},
    [KEY_EVT_VENDOR_CONFIG_7]               = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_7},
    [KEY_EVT_VENDOR_CONFIG_8]               = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_8},
    [KEY_EVT_VENDOR_CONFIG_9]               = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_9},
    [KEY_EVT_VENDOR_CONFIG_10]              = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_10},
    [KEY_EVT_VENDOR_CONFIG_11]              = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_11},
    [KEY_EVT_VENDOR_CONFIG_12]              = {.taskID = TLKSYS_TASKID_SYSTEM, .msgID = TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_12},
    [KEY_EVT_MODE_SYSTEM_POWEROFF_REQ]      = {.taskID = TLKSYS_TASKID_HOST, .msgID = TLKSYS_TPT_MSGID_SHUT_DOWN},

};

/**
 * @brief       Inserts key configurations.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_sysKey_insert(void)
{
#ifdef KEY1_ID
    tlkdrv_key_insert(TLKDRV_KEY_DID_KEY1, TLKDRV_KEY_EVTMSK_DEFAULT, KEY1_GPIO_IN, KEY1_GPIO_OUT, 0);
#endif

#ifdef KEY2_ID
    tlkdrv_key_insert(TLKDRV_KEY_DID_KEY2, TLKDRV_KEY_EVTMSK_DEFAULT, KEY2_GPIO_IN, KEY2_GPIO_OUT, 0);
#endif

#ifdef KEY3_ID
    tlkdrv_key_insert(TLKDRV_KEY_DID_KEY3, TLKDRV_KEY_EVTMSK_DEFAULT, KEY3_GPIO_IN, KEY3_GPIO_OUT, 0);
#endif

#ifdef KEY4_ID
    tlkdrv_key_insert(TLKDRV_KEY_DID_KEY4, TLKDRV_KEY_EVTMSK_DEFAULT, KEY4_GPIO_IN, KEY4_GPIO_OUT, 0);

#endif
}

/**
 * @brief       Initializes the system key module.
 * @param[in]   none.
 * @return      none.
 */
__attribute__((weak)) void tlkapp_sysKey_init(void)
{
    tlkapp_sysKey_insert();
    tlkapp_sysKey_initCompletedHook();
}

/**
 * @brief       Handles key event actions.
 * @param[in]   evtID - The ID of the key event.
 * @return      none.
 */
__attribute__((weak)) void tlkapp_sysKey_evtAction(uint16_t evtID)
{
    if (evtID >= KEY_EVT_MODE_MAX) {
        return;
    }
    tlksys_sendMsg(sTlkappSysKeyMsgTable[evtID].taskID, sTlkappSysKeyMsgTable[evtID].msgID, NULL, 0);
}

/**
 * @brief       Provides a hook function when the sys key init is completed.
 * @param[in]   none.
 * @return      none.
 */
__attribute__((weak)) void tlkapp_sysKey_initCompletedHook(void) {}

#endif
