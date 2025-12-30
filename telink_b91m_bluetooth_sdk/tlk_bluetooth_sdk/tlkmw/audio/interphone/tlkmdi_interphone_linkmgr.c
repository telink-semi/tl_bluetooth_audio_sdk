/********************************************************************************************************
 * @file    tlkmdi_interphone_linkmgr.c
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
#if TLKMW_INTERPHONE_EN
#include "tlkapi/tlkapi.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "tlkmw/tlkmw.h"

#define MAX_LINK_NUM        3
#define LINK_MGR_DEBUG_EN   1
#define A2DP_FILTER_TIME_MS 300

typedef struct
{
    uint8_t  state;
    uint8_t  resv;
    uint16_t handle;
} Linkinfo_t;

typedef struct
{
    Linkinfo_t                  link[MAX_LINK_NUM];
    TlkmdiInterPhoneMusicCtrlCB cb;
    TlkApiTimer_t               timer;
    uint8_t                     enable;
    uint8_t                     isRefuseA2dp;
} Linkmgr_t;

static Linkmgr_t sTlkmdiInterPhoneLinkmgr = {0};

//Arrangement must be made in the following order:
//opened -> paused -> idle -> closed

/**
 * @brief       Find link by handle
 * @param[in]   handle - Connection handle
 * @return      Link index, or MAX_LINK_NUM if not found
 */
static inline uint8_t prv_linkmgr_findLink(uint16_t handle)
{
    Linkmgr_t *mgr = &sTlkmdiInterPhoneLinkmgr;
    for (uint8_t i = 0; i < MAX_LINK_NUM; i++) {
        if (mgr->link[i].handle == handle) {
            return i; //find
        }
        if (mgr->link[i].state == TLK_STATE_CLOSED) {
            mgr->link[i].handle = handle;
            return i; //not find malloc
        }
    }
    return MAX_LINK_NUM;
}

/**
 * @brief       Change link state to open
 * @param[in]   index - Link index
 * @return      true if position is 0, false otherwise
 */
static inline bool prv_link_state2open(uint8_t index)
{
    Linkinfo_t *link = sTlkmdiInterPhoneLinkmgr.link;
    if (link[index].state == TLK_STATE_OPENED) {
        return false;
    }
    uint8_t newPos = 0;
    for (; newPos < MAX_LINK_NUM; newPos++) {
        if (link[newPos].state != TLK_STATE_OPENED) {
            break;
        }
    }
    uint16_t handle = link[index].handle;
    memmove(&link[newPos + 1], &link[newPos], (index - newPos) * sizeof(Linkinfo_t));
    link[newPos].handle = handle;
    link[newPos].state  = TLK_STATE_OPENED;
    return newPos == 0;
}

/**
 * @brief       Change link state to paused
 * @param[in]   index - Link index
 * @return      true if position is 0, false otherwise
 */
static inline bool prv_link_state2paused(uint8_t index)
{
    Linkinfo_t *link = sTlkmdiInterPhoneLinkmgr.link;
    if (link[index].state != TLK_STATE_OPENED) {
        return false;
    }
    uint8_t  newPos = index + 1;
    uint16_t handle = link[index].handle;
    if (index == 0 && sTlkmdiInterPhoneLinkmgr.enable) {
        sTlkmdiInterPhoneLinkmgr.cb(handle, false);
    }
    for (; newPos < MAX_LINK_NUM; newPos++) {
        if (link[newPos].state != TLK_STATE_OPENED) {
            break;
        }
        link[newPos - 1] = link[newPos];
    }
    link[newPos - 1].handle = handle;
    link[newPos - 1].state  = TLK_STATE_PAUSED;
    return index == 0;
}

/**
 * @brief       Change link state to idle
 * @param[in]   index - Link index
 * @return      false
 */
static inline bool prv_link_state2idle(uint8_t index)
{
    Linkinfo_t *link = sTlkmdiInterPhoneLinkmgr.link;
    if (link[index].state != TLK_STATE_CLOSED) {
        return false;
    }
    uint8_t newPos = 0;
    for (; newPos < MAX_LINK_NUM; newPos++) {
        if (link[newPos].state == TLK_STATE_CLOSED || link[newPos].state == TLK_STATE_IDLE) {
            break;
        }
    }
    uint16_t handle = link[index].handle;
    memmove(&link[newPos + 1], &link[newPos], (index - newPos) * sizeof(Linkinfo_t));
    link[newPos].handle = handle;
    link[newPos].state  = TLK_STATE_IDLE;
    return false;
}

/**
 * @brief       Change link state to close
 * @param[in]   index - Link index
 * @return      true if need to schedule, false otherwise
 */
static inline bool prv_link_state2close(uint8_t index)
{
    Linkinfo_t *link = sTlkmdiInterPhoneLinkmgr.link;
    bool        sch  = (link[index].state == TLK_STATE_OPENED && index == 0);
    if (sch && sTlkmdiInterPhoneLinkmgr.enable) {
        sTlkmdiInterPhoneLinkmgr.cb(link[index].handle, false);
    }
    uint8_t newPos = index + 1;
    for (; newPos < MAX_LINK_NUM; newPos++) {
        if (link[newPos].state == TLK_STATE_CLOSED) {
            break;
        }
        link[newPos - 1] = link[newPos];
    }
    link[newPos - 1].handle = 0;
    link[newPos - 1].state  = TLK_STATE_CLOSED;
    return sch;
}

/**
 * @brief       Debug link manager
 * @param       None
 * @return      None
 */
static void tlkmdi_interphone_linkmgr_debug(void)
{
    Linkinfo_t *link = sTlkmdiInterPhoneLinkmgr.link;
    tlkapi_printf(LINK_MGR_DEBUG_EN, "***LinkMgr (%d %d) (%d %d) (%d %d)", link[0].handle, link[0].state, link[1].handle, link[1].state, link[2].handle, link[2].state);
}

/**
 * @brief       Refuse A2DP timer callback
 * @param[in]   pTimer - Timer handle
 * @param[in]   userArg - User argument
 * @return      None
 */
static void tlkmdi_interphone_linkmgr_refuseA2dpTimer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    sTlkmdiInterPhoneLinkmgr.isRefuseA2dp = 0;
}

/**
 * @brief       Refuse A2DP for some time
 * @param       None
 * @return      None
 */
static void tlkmdi_interphone_linkmgr_refuseA2dpSomeTime(void)
{
    sTlkmdiInterPhoneLinkmgr.isRefuseA2dp = 1;
    tlksys_timer_reStart(TLKSYS_TASKID_AUDIO, &sTlkmdiInterPhoneLinkmgr.timer);
}

/**
 * @brief       Music state change callback
 * @param[in]   handle - Connection handle
 * @param[in]   state - Music state
 * @return      None
 */
static void tlkmdi_interphone_music_state_chg_cb(uint16_t handle, uint8_t state)
{
    uint8_t index = prv_linkmgr_findLink(handle);
    if (index == MAX_LINK_NUM) {
        return;
    }
    bool needSch      = false;
    bool isNeedRefuse = false;
    switch (state) {
    case TLK_STATE_OPENED:
        needSch = prv_link_state2open(index);
        break;
    case TLK_STATE_PAUSED:
        needSch      = prv_link_state2paused(index);
        isNeedRefuse = true;
        break;
    case TLK_STATE_IDLE:
        needSch = prv_link_state2idle(index);
        break;
    default:
        needSch      = prv_link_state2close(index);
        isNeedRefuse = true;
        break;
    }
    if (needSch == false) {
        tlkmdi_interphone_linkmgr_debug();
        return; // no need sch
    }
    handle = tlkmdi_interphone_linkmgr_getRunningHandle();
    if (handle == 0) {
        tlkmdi_interphone_clear_mode(INTERPHONE_MODE_MUSIC);
        tlkmdi_interphone_linkmgr_debug();
        return;
    }
    uint8_t isRunning = sTlkmdiInterPhoneLinkmgr.enable;
    tlkmdi_interphone_set_mode(INTERPHONE_MODE_MUSIC);
    if (isRunning) {
        sTlkmdiInterPhoneLinkmgr.cb(handle, true);
    }
    if (isNeedRefuse) {
        tlkmdi_interphone_linkmgr_refuseA2dpSomeTime();
    }
    tlkmdi_interphone_linkmgr_debug();
}

/**
 * @brief       Default callback function for interphone link manager
 * @param[in]   handle - Connection handle
 * @param[in]   isStart - Start flag
 * @return      None
 */
static void tlkmdi_interphone_linkmgr_default_cb(uint16_t handle, uint8_t isStart)
{
    (void)handle;
    (void)isStart;
}

/**
 * @brief       Initialize interphone link manager
 * @param[in]   cb - Callback function
 * @return      None
 */
void tlkmdi_interphone_linkmgr_init(TlkmdiInterPhoneMusicCtrlCB cb)
{
    if (cb == NULL) {
        cb = tlkmdi_interphone_linkmgr_default_cb;
    }
    sTlkmdiInterPhoneLinkmgr.cb = cb;
    tlksys_timer_createStatic(TLKSYS_TASKID_AUDIO, &sTlkmdiInterPhoneLinkmgr.timer, A2DP_FILTER_TIME_MS * 1000, false, tlkmdi_interphone_linkmgr_refuseA2dpTimer, NULL);
    tlkmdi_audio_btif_addMusicStateChgCB(tlkmdi_interphone_music_state_chg_cb, false);
}

/**
 * @brief       Get first paused handle
 * @param       None
 * @return      First paused handle
 */
uint16_t tlkmdi_interphone_linkmgr_getFirstPausedHandle(void)
{
    Linkmgr_t *mgr = &sTlkmdiInterPhoneLinkmgr;
    for (uint8_t i = 0; i < MAX_LINK_NUM; i++) {
        if (mgr->link[i].state == TLK_STATE_OPENED || mgr->link[i].state == TLK_STATE_CLOSED) {
            break;
        }
        return mgr->link[i].handle;
    }
    return 0;
}

/**
 * @brief       Get running handle
 * @param       None
 * @return      Running handle
 */
uint16_t tlkmdi_interphone_linkmgr_getRunningHandle(void)
{
    if (sTlkmdiInterPhoneLinkmgr.isRefuseA2dp) {
        return 0;
    }
    Linkmgr_t *mgr = &sTlkmdiInterPhoneLinkmgr;
    if (mgr->link[0].state == TLK_STATE_OPENED) {
        return mgr->link[0].handle;
    }
    return 0;
}

/**
 * @brief       Get valid handle
 * @param       None
 * @return      Valid handle
 */
uint16_t tlkmdi_interphone_linkmgr_getValidHandle(void)
{
    uint16_t handle = tlkmdi_interphone_linkmgr_getRunningHandle();
    if (handle == 0) {
        return tlkmdi_interphone_linkmgr_getFirstPausedHandle();
    }
    return handle;
}

/**
 * @brief       Pause interphone link manager
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_linkmgr_pause(void)
{
    if (!sTlkmdiInterPhoneLinkmgr.enable) {
        return;
    }
    sTlkmdiInterPhoneLinkmgr.enable = 0;
    uint16_t handle                 = tlkmdi_interphone_linkmgr_getRunningHandle();
    if (handle != 0) {
        sTlkmdiInterPhoneLinkmgr.cb(handle, false);
    }
}

/**
 * @brief       Resume interphone link manager
 * @param       None
 * @return      None
 */
void tlkmdi_interphone_linkmgr_resume(void)
{
    if (sTlkmdiInterPhoneLinkmgr.enable) {
        return;
    }
    sTlkmdiInterPhoneLinkmgr.enable = 1;
    uint16_t handle                 = tlkmdi_interphone_linkmgr_getRunningHandle();
    if (handle != 0) {
        tlkmdi_interphone_linkmgr_refuseA2dpSomeTime();
        sTlkmdiInterPhoneLinkmgr.cb(handle, true);
    }
}


#endif //TLKMW_INTERPHONE_EN
