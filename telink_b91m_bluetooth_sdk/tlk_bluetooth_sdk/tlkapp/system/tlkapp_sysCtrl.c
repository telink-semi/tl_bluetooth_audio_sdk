/********************************************************************************************************
 * @file    tlkapp_sysCtrl.c
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
#if (TLK_CFG_SYSTEM_ENABLE)
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"


#define TLKAPP_SYS_FLASH_SAVE_DELAY_TIME (5 * 1000 * 1000)

static uint8_t                    sTlkAppSysUIHandleCnt = 0;
static uint8_t                    sTlkAppSysUIState     = 0;
static TlkApiTimer_t              sTlkAppSysSaveTimer;
static tlkapp_sysUI_handleState_t sTlkAppSysUIHandleState[TLKAPP_UI_HANDLE_MAX_NUM];

/**
 * @brief       Timer callback function to save flash data.
 * @param[in]   pTimer    - Pointer to the timer handle.
 * @param[in]   userArg   - User-defined argument.
 * @return      none.
 */
static void tlkapp_sysCtrl_flashSaveTimer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
#ifndef TL753X_ADAPT
    if (tlkmdi_tinySql_isRequestSave()) {
        tlkmdi_tinySql_save();
    }
#endif
}

/**
 * @brief       Initializes the system control module.
 * @param[in]   none.
 * @return      Returns TLK_ENONE on success.
 */
int tlkapp_sys_ctrlInit(void)
{
    sTlkAppSysUIState     = 0;
    sTlkAppSysUIHandleCnt = 0;
    tlkmdi_tinySql_init();
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &sTlkAppSysSaveTimer, TLKAPP_SYS_FLASH_SAVE_DELAY_TIME, false, tlkapp_sysCtrl_flashSaveTimer, NULL);
    return TLK_ENONE;
}

/**
 * @brief       Handles flash saving requests.
 * @param[in]   none.
 * @return      none.
 */
void tlkapp_sys_flashHandler(void)
{
    if (tlkmdi_tinySql_isRequestSave() == false) {
        return;
    }
    tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM, &sTlkAppSysSaveTimer);
}

/**
 * @brief       Sends a start tone message.
 * @param[in]   tone_indx - Index of the tone to play.
 * @return      none.
 */
static void tlkapp_sysUI_sendStartToneMsg(uint8_t tone_indx)
{
    (void)tone_indx;
#if (MCU_CORE_TL752X_TEMP)
    return;
#endif
    uint8_t data[2] = {tone_indx, 1};
    tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_START_TONE_CMD, data, 2);
}

/**
 * @brief       Performs UI actions based on the given state.
 * @param[in]   state - The new UI state.
 * @return      none.
 */
static void tlkapp_sysUI_action(TLKAPP_UI_STATE_ENUM state)
{
#if (TLK_DEV_LED_ENABLE)
    tlkapp_sysLed_funAction((uint8_t)state);
#endif
    switch (state) {
    case TLKAPP_UI_STATE_PARING:
        tlkapp_sysUI_sendStartToneMsg(TONE_PAIRING);
        break;
    case TLKAPP_UI_STATE_LOWBATTARY:
        tlkapp_sysUI_sendStartToneMsg(TONE_LOW_POWER);
        break;
    default:
        break;
    }
    sTlkAppSysUIState = state;
}

/**
 * @brief       Handles state changes for the UI.
 * @param[in]   newState - The new state to apply.
 * @return      none.
 */
static void tlkapp_sysUI_handleStateChangeAction(TLKAPP_UI_STATE_ENUM newState)
{
    switch (newState) {
    case TLKAPP_UI_STATE_CONNECTED:
        tlkapp_sysUI_sendStartToneMsg(TONE_CONNECTED);
        break;
    case TLKAPP_UI_STATE_IDLE:
        tlkapp_sysUI_sendStartToneMsg(TONE_DISCONNECTED);
        break;
    default:
        break;
    }
}

/**
 * @brief       Checks the current UI state.
 * @param[in]   none.
 * @return      Returns the current UI state.
 */
static uint8_t tlkapp_sysUI_checkState(void)
{
    for (int index = 0; index < sTlkAppSysUIHandleCnt; index++) {
        if (TLKAPP_UI_STATE_IDLE != sTlkAppSysUIHandleState[index].state) {
            return TLKAPP_UI_STATE_CONNECTED;
        }
    }
    return TLKAPP_UI_STATE_IDLE;
}

/**
 * @brief       Updates the handle state for the UI.
 * @param[in]   group  - The group ID.
 * @param[in]   handle - The handle ID.
 * @param[in]   state  - The new state.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_sysUI_updateHandleStateCore(uint8_t group, uint16_t handle, uint8_t state)
{
    if (state >= TLKAPP_UI_STATE_MAX) {
        return -TLK_EPARAM;
    }
    if (group == TLKAPP_UI_HANDLE_GROUP_SYS) {
        if (TLKAPP_UI_STATE_IDLE == state) {
            state = tlkapp_sysUI_checkState();
        } //idle must check is truly idle.
        tlkapp_sysUI_action(state);
        return TLK_ENONE;
    }
    int index = 0;
    for (index = 0; index < sTlkAppSysUIHandleCnt; index++) {
        if (handle != sTlkAppSysUIHandleState[index].handle) {
            continue;
        }
        sTlkAppSysUIHandleState[index].group = group;
        if (state == sTlkAppSysUIHandleState[index].state) {
            return TLK_ENONE; //no change
        }
        break;
    }
    if (index == sTlkAppSysUIHandleCnt) {
        //not found/new handle
        if (state == TLKAPP_UI_STATE_IDLE) {
            return -TLK_EPARAM;
        }
        if (sTlkAppSysUIHandleCnt >= TLKAPP_UI_HANDLE_MAX_NUM) {
            return -TLK_EFULL;
        }
        sTlkAppSysUIHandleCnt++;
        sTlkAppSysUIHandleState[index].group  = group;
        sTlkAppSysUIHandleState[index].handle = handle;
    }
    if (state == TLKAPP_UI_STATE_IDLE) {
        tmemcpy(&sTlkAppSysUIHandleState[index], &sTlkAppSysUIHandleState[index + 1], sizeof(tlkapp_sysUI_handleState_t) * (sTlkAppSysUIHandleCnt - index - 1));
        sTlkAppSysUIHandleCnt--;
    } else {
        sTlkAppSysUIHandleState[index].state = state;
    }
    tlkapp_sysUI_handleStateChangeAction(state);
    if (sTlkAppSysUIState == TLKAPP_UI_STATE_PARING && state == TLKAPP_UI_STATE_IDLE) {
        return TLK_ENONE; //paring mode not allowed change ui to idle
    }
    uint8_t nowState = tlkapp_sysUI_checkState();
    tlkapp_sysUI_action(nowState);
    return TLK_ENONE;
}

/**
 * @brief       Updates the handle state with mutex protection.
 * @param[in]   group  - The group ID.
 * @param[in]   handle - The handle ID.
 * @param[in]   state  - The new state.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_sysUI_updateHandleState(uint8_t group, uint16_t handle, uint8_t state)
{
    tlksys_mutex_lock(TLKSYS_MUTEX_UI);
    int res = tlkapp_sysUI_updateHandleStateCore(group, handle, state);
    tlksys_mutex_unlock(TLKSYS_MUTEX_UI);
    return res;
}

/**
 * @brief       Hook function called when host link manager detects a link change.
 * @param[in]   type    - The link type.
 * @param[in]   handle  - The handle ID.
 * @param[in]   isDel   - Indicates if the link is deleted.
 * @return      none.
 */
void tlkmw_host_linkmgr_linkChangeHook(uint8_t type, uint16_t handle, bool isDel)
{
    uint8_t state = isDel ? TLKAPP_UI_STATE_IDLE : TLKAPP_UI_STATE_CONNECTED;
    switch (type) {
    case TLKMDI_HOST_LINK_TYPE_BT:
    {
        tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_BT, handle, state);
    } break;
    case TLKMDI_HOST_LINK_TYPE_BLE:
    {
        tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_BLE, handle, state);
    } break;
    }
}

#endif // #if (TLK_CFG_SYSTEM_ENABLE)
