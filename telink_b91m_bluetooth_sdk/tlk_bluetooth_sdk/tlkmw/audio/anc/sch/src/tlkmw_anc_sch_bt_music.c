/********************************************************************************************************
 * @file    tlkmw_anc_sch_bt_music.c
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

#define MAX_LINK_NUM 2

typedef struct
{
    uint8_t  state;
    uint8_t  resv;
    uint16_t handle;
} AncSchBtMusicLinkinfo_t;

typedef struct
{
    AncSchBtMusicLinkinfo_t link[MAX_LINK_NUM];
} AncSchBtMusic_t;

static AncSchBtMusic_t sTlkmwAncSchBtMusic = {0};

//Arrangement must be made in the following order:
//opened -> paused -> idle -> closed

/**
 * @brief       Request update for BT music scheduler
 * @param[in]   handle - Connection handle
 * @param[in]   isStart - Start flag
 * @return      None
 */
static void prv_anc_sch_bt_music_request_update(uint16_t handle, uint8_t isStart)
{
    (void)handle;
    (void)isStart;
    tlkmw_anc_sch_request_update(TLKMW_ANC_SCH_CB_TYPE_BT_MUSIC, handle, isStart);
}

/**
 * @brief       Find link index by handle
 * @param[in]   handle - Connection handle to find
 * @return      Link index, or MAX_LINK_NUM if not found
 */
static inline uint8_t prv_anc_sch_bt_music_findLink(uint16_t handle)
{
    AncSchBtMusic_t *mgr = &sTlkmwAncSchBtMusic;
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
 * @return      true if need scheduler, false otherwise
 */
static inline bool prv_anc_bt_music_state2open(uint8_t index)
{
    AncSchBtMusicLinkinfo_t *link = sTlkmwAncSchBtMusic.link;
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
    memmove(&link[newPos + 1], &link[newPos], (index - newPos) * sizeof(AncSchBtMusicLinkinfo_t));
    link[newPos].handle = handle;
    link[newPos].state  = TLK_STATE_OPENED;
    return newPos == 0;
}

/**
 * @brief       Change link state to paused
 * @param[in]   index - Link index
 * @return      true if need scheduler, false otherwise
 */
static inline bool prv_anc_bt_music_state2paused(uint8_t index)
{
    AncSchBtMusicLinkinfo_t *link = sTlkmwAncSchBtMusic.link;
    if (link[index].state != TLK_STATE_OPENED) {
        return false;
    }
    uint8_t  newPos = index + 1;
    uint16_t handle = link[index].handle;
    if (index == 0) {
        prv_anc_sch_bt_music_request_update(handle, false);
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
 * @return      true if need scheduler, false otherwise
 */
static inline bool prv_anc_bt_music_state2idle(uint8_t index)
{
    AncSchBtMusicLinkinfo_t *link = sTlkmwAncSchBtMusic.link;
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
    memmove(&link[newPos + 1], &link[newPos], (index - newPos) * sizeof(AncSchBtMusicLinkinfo_t));
    link[newPos].handle = handle;
    link[newPos].state  = TLK_STATE_IDLE;
    return false;
}

/**
 * @brief       Change link state to closed
 * @param[in]   index - Link index
 * @return      true if need scheduler, false otherwise
 */
static inline bool prv_anc_bt_music_state2close(uint8_t index)
{
    AncSchBtMusicLinkinfo_t *link = sTlkmwAncSchBtMusic.link;
    bool                     sch  = (link[index].state == TLK_STATE_OPENED && index == 0);
    if (sch) {
        prv_anc_sch_bt_music_request_update(link[index].handle, false);
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
 * @brief       BT music state change callback
 * @param[in]   handle - Connection handle
 * @param[in]   state - New state
 * @return      None
 */
static void tlkmw_anc_bt_music_state_chg_cb(uint16_t handle, uint8_t state)
{
    uint8_t index = prv_anc_sch_bt_music_findLink(handle);
    if (index == MAX_LINK_NUM) {
        return;
    }
    bool needSch = false;
    switch (state) {
    case TLK_STATE_OPENED:
        needSch = prv_anc_bt_music_state2open(index);
        break;
    case TLK_STATE_PAUSED:
        needSch = prv_anc_bt_music_state2paused(index);
        break;
    case TLK_STATE_IDLE:
        needSch = prv_anc_bt_music_state2idle(index);
        break;
    default:
        needSch = prv_anc_bt_music_state2close(index);
        break;
    }
    if (needSch == false) {
        return; // no need sch
    }
    handle = tlkmw_anc_bt_music_getRunningHandle();
    if (handle != 0) {
        prv_anc_sch_bt_music_request_update(handle, true);
    }
}

/**
 * @brief       Initialize BT music scheduler
 * @param       None
 * @return      None
 */
void tlkmw_anc_sch_bt_music_init(void)
{
    tlkmdi_audio_btif_addMusicStateChgCB(tlkmw_anc_bt_music_state_chg_cb, false);
}

/**
 * @brief       Get running handle of BT music
 * @param       None
 * @return      Running handle, or 0 if none
 */
uint16_t tlkmw_anc_bt_music_getRunningHandle(void)
{
    AncSchBtMusic_t *mgr = &sTlkmwAncSchBtMusic;
    if (mgr->link[0].state == TLK_STATE_OPENED) {
        return mgr->link[0].handle;
    }
    return 0;
}

/**
 * @brief       Get first paused handle of BT music
 * @param       None
 * @return      First paused handle, or 0 if none
 */
uint16_t tlkmw_anc_bt_music_getFirstPausedHandle(void)
{
    AncSchBtMusic_t *mgr = &sTlkmwAncSchBtMusic;
    for (uint8_t i = 0; i < MAX_LINK_NUM; i++) {
        if (mgr->link[i].state == TLK_STATE_OPENED || mgr->link[i].state == TLK_STATE_CLOSED) {
            break;
        }
        return mgr->link[i].handle;
    }
    return 0;
}


#endif