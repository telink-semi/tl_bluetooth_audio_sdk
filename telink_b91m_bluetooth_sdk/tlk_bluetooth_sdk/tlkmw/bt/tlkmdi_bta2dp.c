/********************************************************************************************************
 * @file    tlkmdi_bta2dp.c
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
#if (TLKBTP_CFG_A2DP_ENABLE)
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/btp/avrcp/cover_art/btp_coverArt.h"
#include "stack/bt/host/btp/avrcp/cover_art/btp_coverArtClt.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "string.h"

#define TLKMDI_BTA2DP_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_A2DP << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTA2DP_DBG_SIGN "[MA2DP]"

#if (TLK_STK_BT_ENABLE)
#if (TLKBTP_CFG_A2DPSNK_ENABLE)
static TlkApiTimer_t s_tlkmdi_a2dp_threshold_check_timer;
static uint8_t       s_tlkmdi_a2dp_threshold_check_cnt = 0;
#endif
static int  tlkmdi_btavrcp_peerEvtMaskEvt(uint8_t *pData, uint16_t dataLen);
static int  tlkmdi_btavrcp_trackChangeEvt(uint8_t *pData, uint16_t dataLen);
static int  tlkmdi_btavrcp_playbackPosChgEvt(uint8_t *pData, uint16_t dataLen);
static int  tlkmdi_btavrcp_playerAppSettingChgEvt(uint8_t *pData, uint16_t dataLen);
static int  tlkmdi_btavrcp_playerAddressChgEvt(uint8_t *pData, uint16_t dataLen);
static void tlkmdi_btavrcp_insCompleteCB(uint16_t aclHandle, uint8_t pduID, uint8_t status, void *pParam, uint16_t paramLen);
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
static void tlkmdi_btBrowsing_itemsReportCB(uint16_t aclHandle, uint8_t pduID, uint8_t status, uint8_t itemType, uint8_t itemNumb, void *pItemData);
static void tlkmdi_btBrowsing_insCompleteCB(uint16_t aclHandle, uint8_t pduID, uint8_t status, void *pParam, uint16_t paramLen);
#endif
#if (TLKBTP_CFG_AVRC_COVER_ART_ENABLE)
extern btp_coverArt_item_t *btp_coverArt_getUsedNode(uint16_t aclHandle, uint8_t usrID);
static int                  tlkmdi_btavrcp_coverArtGetImageEvt(uint8_t *pData, uint16_t dataLen);

static uint32_t sTlkMdiCoverArtRecvLen = 0;

#endif
#endif

/**
 * @brief       This function sends a music state change event to the host.
 * @param[in]   handle  - the connection handle of the A2DP connection.
 * @param[in]   state   - the music state to be sent.
 * @return      none.
 */
void tlkmdi_bta2dp_sendHostMusicStateChgEvt(uint16_t handle, uint8_t state)
{
    const uint8_t         bufferLen = sizeof(tlksys_msg_hostEvt_t) + sizeof(tlksys_msg_hostEvt_btAudStateChg_t);
    uint8_t               buffer[bufferLen];
    tlksys_msg_hostEvt_t *evt                  = (tlksys_msg_hostEvt_t *)buffer;
    evt->dataLen                               = sizeof(tlksys_msg_hostEvt_btAudStateChg_t);
    evt->hostType                              = TLKSYS_MSG_HOST_TYPE_BT;
    evt->msgID                                 = TLKSYS_MSG_BT_HOST_EVT_TYPE_AUD_STATE_CHG;
    tlksys_msg_hostEvt_btAudStateChg_t *evtDat = (tlksys_msg_hostEvt_btAudStateChg_t *)evt->data;
    evtDat->handle                             = handle;
    evtDat->state                              = state;
    evtDat->isMusic                            = 1;
    tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_HOST_EVT_COME, buffer, bufferLen);
}

/**
 * @brief       This function sends a music status change event.
 * @param[in]   handle  - the connection handle of the A2DP connection.
 * @param[in]   status  - the music status to be sent.
 * @return      none.
*/
static int tlkmdi_bta2dp_statusChgCB(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_a2dpStatusChangeEvt_t *pEvt;
    pEvt = (btp_a2dpStatusChangeEvt_t *)pData;

    if (pEvt->status == BTP_A2DP_STATUS_STREAM) {
        tlksys_task_setEvt(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_MUSIC_STATUS_NOTIFY);
        btp_avrcp_setPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PLAYING);
        tlkmdi_bta2dp_sendHostMusicStateChgEvt(pEvt->handle, TLK_STATE_OPENED);
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_A2DP_STATUS_STREAM");
    } else if (pEvt->status == BTP_A2DP_STATUS_PAUSED || pEvt->status == BTP_A2DP_STATUS_CLOSED) {
        btp_avrcp_setPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PAUSED);
        tlkmdi_bta2dp_sendHostMusicStateChgEvt(pEvt->handle, TLK_STATE_PAUSED);
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_A2DP_STATUS_PAUSED");
    }

    return TLK_ENONE;
}
#if (TLKBTP_CFG_A2DPSNK_ENABLE)

/**
 * @brief       This function is called when the audio threshold check timer expires.
 * @param[in]   pTimer  - the timer handle.
 * @param[in]   userArg - the user argument.
 * @return      none.
*/
static void tlkmdi_bt_s_tlkmdi_a2dp_threshold_check_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;

    uint16_t handle = (uint32_t)userArg & 0xFFFF;
    tlk_printf("tlkmdi_bt_s_tlkmdi_a2dp_threshold_check_timer handle[%d]", handle);
    bool threshold_flag = tlkmdi_audio_get_threshold_flag();

    if (s_tlkmdi_a2dp_threshold_check_cnt > 0) {
        s_tlkmdi_a2dp_threshold_check_cnt--;
    }

    if (threshold_flag == false) {
        /*Effective music package detected.*/
        tlkmdi_audio_set_check_threshold_flag(false);
        tlksys_timer_stop(TLKSYS_TASKID_HOST, &s_tlkmdi_a2dp_threshold_check_timer);
        return;
    }

    if (s_tlkmdi_a2dp_threshold_check_cnt == 0 && !btp_avrcp_remoteIsPlaying(handle)) {
        tlkmdi_audio_set_check_threshold_flag(false);
        tlkmdi_bta2dp_sendHostMusicStateChgEvt(handle, TLK_STATE_PAUSED);
        tlksys_timer_stop(TLKSYS_TASKID_HOST, &s_tlkmdi_a2dp_threshold_check_timer);
        return;
    }
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
}
#endif

/**
 * @brief       This function is called when the AVRCP status change event is received.
 * @param[in]   pData   - the event data.
 * @param[in]   dataLen - the length of the event data.
 * @return      TLK_ENONE if success, otherwise error code.
*/
static int tlkmdi_btavrcp_statusChgCB(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_avrcpStatusChangeEvt_t *pEvt;
    pEvt = (btp_avrcpStatusChangeEvt_t *)pData;
    // Fix PC(MACBook) not send AVRCP Change Cmd but Notify Rsp already change state when forward or backward.
    if (!pEvt->isNoty && pEvt->status != BTP_AVRCP_PLAY_STATE_PLAYING) {
        return TLK_ENONE;
    }

    if (pEvt->status == BTP_AVRCP_PLAY_STATE_PLAYING) {
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_AVRCP_PLAY_STATE_PLAYING");
#if TLKBTP_CFG_A2DPSNK_ENABLE
        bool isUnknow = true;
        btp_a2dpsnk_getCurrCodecEx(pEvt->handle, &isUnknow);
        if (isUnknow) {
            return TLK_ENONE; //a2dp not ready,no send start evt
        }
#endif
        tlkmdi_bta2dp_sendHostMusicStateChgEvt(pEvt->handle, TLK_STATE_OPENED);

    } else if (pEvt->status == BTP_AVRCP_PLAY_STATE_PAUSED || pEvt->status == BTP_AVRCP_PLAY_STATE_STOPPED) {
#if (TLKSTK_BT_TPS_ENABLE)
        uint8_t audioMode = app_tph_headset_get_mode();
#if (TLK_STK_TPH_ENABLE)
        if ((audioMode & TPH_HOST_MODE_BT_MUSIC) != 0 && (audioMode & TPH_HOST_MODE_DONGLE_AUDIO) != 0) {
#elif (TLK_STK_TPT_ENABLE)
        if ((audioMode & TPT_HOST_MODE_BT_MUSIC) != 0 && (audioMode & TPT_HOST_MODE_DONGLE_AUDIO) != 0) {
#endif
            tlkapi_warn(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_statusChgCB: not allowed exit mix mode!");
            return TLK_ENONE;
        }
#endif

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
        uint16_t play_handle = tlkmw_audio_btif_getPlayHandle();
        if (play_handle != pEvt->handle || btp_a2dpsnk_getStatus(pEvt->handle) != BTP_A2DP_STATUS_STREAM) {
            tlkmdi_bta2dp_sendHostMusicStateChgEvt(pEvt->handle, TLK_STATE_PAUSED);
        } else {
            tlkmdi_audio_set_check_threshold_flag(true);
            tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &s_tlkmdi_a2dp_threshold_check_timer, 1000000, false, tlkmdi_bt_s_tlkmdi_a2dp_threshold_check_timer,
                                      (void *)(uint32_t)pEvt->handle);
            tlksys_timer_reStart(TLKSYS_TASKID_HOST, &s_tlkmdi_a2dp_threshold_check_timer);
            s_tlkmdi_a2dp_threshold_check_cnt = 6; //500ms * 6
        }
#else
        tlkmdi_bta2dp_sendHostMusicStateChgEvt(pEvt->handle, TLK_STATE_PAUSED);
#endif
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_AVRCP_PLAY_STATE_STOPPED");
    }

    return TLK_ENONE;
}

BTP_EVT_REGISTER(BTP_EVTID_AVRCP_VOLUME_CHANGED, tlkmdi_btavrcp_volumeChangeEvt);
BTP_EVT_REGISTER(BTP_EVTID_AVRCP_PEER_EVT_MASK, tlkmdi_btavrcp_peerEvtMaskEvt);
BTP_EVT_REGISTER(BTP_EVTID_AVRCP_TRACK_CHANGED, tlkmdi_btavrcp_trackChangeEvt);
BTP_EVT_REGISTER(BTP_EVTID_AVRCP_PLAYBACK_POS_CHANGED, tlkmdi_btavrcp_playbackPosChgEvt);
BTP_EVT_REGISTER(BTP_EVTID_AVRCP_PLAYER_APP_SETTING_CHANGED, tlkmdi_btavrcp_playerAppSettingChgEvt);
BTP_EVT_REGISTER(BTP_EVTID_AVRCP_PLAYER_ADDRESSED_CHANGED, tlkmdi_btavrcp_playerAddressChgEvt);
BTP_EVT_REGISTER(BTP_EVTID_A2DPSNK_STATUS_CHANGED, tlkmdi_bta2dp_statusChgCB);
BTP_EVT_REGISTER(BTP_EVTID_AVRCP_STATUS_CHANGED, tlkmdi_btavrcp_statusChgCB);
#if (TLKBTP_CFG_AVRC_COVER_ART_ENABLE)
BTP_EVT_REGISTER(BTP_EVTID_COVER_ART_GET_IMAGE, tlkmdi_btavrcp_coverArtGetImageEvt);
#endif

/**
 * @brief       Initialize the A2DP module, register callback functions for AVRCP and browsing operations.
 * @param[in]   none.
 * @return      TLK_ENONE if success, otherwise error code.
 */
int tlkmdi_bta2dp_init(void)
{
    // Register AVRCP instruction completion callback
    (void)tlkmdi_btavrcp_insCompleteCB;
#if (TLKBTP_CFG_AVRCP_ENABLE)
    btp_avrcp_regInsCompleteCB(tlkmdi_btavrcp_insCompleteCB);
#endif
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    // Register browsing items report and instruction completion callbacks
    btp_browsing_regItemsReportCB(tlkmdi_btBrowsing_itemsReportCB);
    btp_browsing_regInsCompleteCB(tlkmdi_btBrowsing_insCompleteCB);
#endif // #if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)

    return TLK_ENONE;
}

/**
 * @brief       Handle A2DP connection event.
 *              This function processes connection events for A2DP connections.
 * @param[in]   aclHandle - The ACL connection handle
 * @param[in]   usrID     - User identifier for the connection
 * @return      none
 */
void tlkmdi_bta2dp_connectEvt(uint16_t aclHandle, uint8_t usrID)
{
    (void)aclHandle;
    (void)usrID;
}

/**
 * @brief       This function handles volume change events from AVRCP.
 * @param[in]   pData   - the event data.
 * @param[in]   dataLen - the length of the event data.
 * @return      TLK_ENONE if success, otherwise error code.
*/
__attribute__((weak)) int tlkmdi_btavrcp_volumeChangeEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t                     isIos = 0;
    btp_avrcpVolumeChangeEvt_t *pEvt;

    pEvt                        = (btp_avrcpVolumeChangeEvt_t *)pData;
    uint16_t             handle = pEvt->handle;
    tlkmdi_btacl_item_t *item   = tlkmdi_btacl_getConnItem(handle);
    if (item == NULL) {
        return -TLK_ESEEK;
    }
#if (TLKBTP_CFG_HFPHF_ENABLE)
    isIos = btp_hfphf_isIosDev(handle);
#endif
    tlkmdi_tinySql_setPairingDeviceVolume(item->btaddr, true, pEvt->volume, isIos);

    const uint8_t         bufferLen = sizeof(tlksys_msg_hostEvt_t) + sizeof(tlksys_msg_hostEvt_btVolChg_t);
    uint8_t               buffer[bufferLen];
    tlksys_msg_hostEvt_t *evt             = (tlksys_msg_hostEvt_t *)buffer;
    evt->dataLen                          = sizeof(tlksys_msg_hostEvt_btVolChg_t);
    evt->hostType                         = TLKSYS_MSG_HOST_TYPE_BT;
    evt->msgID                            = TLKSYS_MSG_BT_HOST_EVT_TYPE_VOLUME_CHG;
    tlksys_msg_hostEvt_btVolChg_t *evtDat = (tlksys_msg_hostEvt_btVolChg_t *)evt->data;
    evtDat->handle                        = handle;
    evtDat->isMusic                       = true;
    evtDat->volume                        = pEvt->volume;
    tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_HOST_EVT_COME, buffer, bufferLen);

    return TLK_ENONE;
}

/**
 * @brief       This function handles peer event mask changes from AVRCP.
 * @param[in]   pData   - the event data.
 * @param[in]   dataLen - the length of the event data.
 * @return      TLK_ENONE if success, otherwise error code.
*/
static int tlkmdi_btavrcp_peerEvtMaskEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_avrcpPeerEvtMaskEvt_t *pEvt = (btp_avrcpPeerEvtMaskEvt_t *)pData;
    if (btp_a2dp_isSrc(pEvt->handle)) {
        btp_avrcp_sendRegEventNotify(pEvt->handle, BTP_AVRCP_EVTID_VOLUME_CHANGED);
    } else if (btp_a2dp_isSnk(pEvt->handle)) {
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
        btp_browsing_connect(pEvt->handle);
#endif
        btp_avrcp_sendRegEventNotify(pEvt->handle, BTP_AVRCP_EVTID_PLAYBACK_STATUS_CHANGED);
        btp_avrcp_sendRegEventNotify(pEvt->handle, BTP_AVRCP_EVTID_TRACK_CHANGED);
    } else {
        /*A2dp is not connected.*/
        bth_acl_handle_t *pHandle = bth_handle_getConnAcl(pEvt->handle);
        if ((pHandle != NULL) && (pHandle->curRole == BTH_ROLE_SLAVE)) {
            btp_avrcp_sendRegEventNotify(pEvt->handle, BTP_AVRCP_EVTID_PLAYBACK_STATUS_CHANGED);
            btp_avrcp_sendRegEventNotify(pEvt->handle, BTP_AVRCP_EVTID_TRACK_CHANGED);
        }
    }
    return TLK_ENONE;
}

/**
 * @brief       This function handles track change events from AVRCP.
 * @param[in]   pData   - the event data.
 * @param[in]   dataLen - the length of the event data.
 * @return      TLK_ENONE if success, otherwise error code.
*/
static int tlkmdi_btavrcp_trackChangeEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
#if ((!TLKBTP_CFG_AVRCP_BROWSING_ENABLE) || (!TLKBTP_CFG_AVRC_COVER_ART_ENABLE))
    (void)pData;
#endif
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
    uint32_t                   attrList[8];
    uint8_t                    invalidID1[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t                    invalidID2[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    btp_avrcpTrackChangeEvt_t *pEvt;

    pEvt = (btp_avrcpTrackChangeEvt_t *)pData;
    tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_trackChangeEvt:", pEvt->pTrackID, 8);

    if (tmemcmp(pEvt->pTrackID, invalidID1, 8) == 0) {
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_trackChangeEvt: This player does not support browsing");
        return TLK_ENONE;
    }
    if (tmemcmp(pEvt->pTrackID, invalidID2, 8) == 0) {
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_trackChangeEvt: No music is playing");
        return TLK_ENONE;
    }

    /*Get the information of music by track ID*/
    attrList[0] = BTP_BROWSING_ATTID_TITLE;
    attrList[1] = BTP_BROWSING_ATTID_ARTIST_NAME;
    attrList[2] = BTP_BROWSING_ATTID_ALBUM_NAME;
    attrList[3] = BTP_BROWSING_ATTID_TRACK_NUMBER;
    attrList[4] = BTP_BROWSING_ATTID_TOTAL_TRACKS;
    attrList[5] = BTP_BROWSING_ATTID_GENRE;
    attrList[6] = BTP_BROWSING_ATTID_PLAYING_TIME;
    attrList[7] = BTP_BROWSING_ATTID_DEF_COVER_ART;
    btp_browsing_sendGetItemAttrCmd(pEvt->handle, BTP_BROWSING_SCOPE_NOW_PLAYGING, pEvt->pTrackID, 0, 8, attrList);
#endif
#if (TLKBTP_CFG_AVRC_COVER_ART_ENABLE)

    uint32_t                   attrList[8];
    uint8_t                    invalidID1[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    btp_avrcpTrackChangeEvt_t *pEvt;

    pEvt = (btp_avrcpTrackChangeEvt_t *)pData;

    if (tmemcmp(pEvt->pTrackID, invalidID1, 8) != 0) {
        btp_coverArtclt_startGetImage(pEvt->handle);
    } else {
        /*Get the information of music by track ID*/
        attrList[0] = BTP_BROWSING_ATTID_TITLE;
        attrList[1] = BTP_BROWSING_ATTID_ARTIST_NAME;
        attrList[2] = BTP_BROWSING_ATTID_ALBUM_NAME;
        attrList[3] = BTP_BROWSING_ATTID_TRACK_NUMBER;
        attrList[4] = BTP_BROWSING_ATTID_TOTAL_TRACKS;
        attrList[5] = BTP_BROWSING_ATTID_GENRE;
        attrList[6] = BTP_BROWSING_ATTID_PLAYING_TIME;
        attrList[7] = BTP_BROWSING_ATTID_DEF_COVER_ART;

        btp_avrcp_sendGetElementAttrCmd(pEvt->handle, invalidID1, attrList, 8);
    }

#endif
    return TLK_ENONE;
}

/**
 * @brief       This function handles playback position change events from AVRCP.
 * @param[in]   pData   - the event data.
 * @param[in]   dataLen - the length of the event data.
 * @return      TLK_ENONE if success, otherwise error code.
*/
static int tlkmdi_btavrcp_playbackPosChgEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_avrcpPlaybackPosChangeEvt_t *pEvt;

    pEvt = (btp_avrcpPlaybackPosChangeEvt_t *)pData;
    (void)pEvt;

    return TLK_ENONE;
}

/**
 * @brief       This function handles player application setting change events from AVRCP.
 * @param[in]   pData   - the event data.
 * @param[in]   dataLen - the length of the event data.
 * @return      TLK_ENONE if success, otherwise error code.
*/
static int tlkmdi_btavrcp_playerAppSettingChgEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_avrcpPlayerAppSettingChangeEvt_t *pEvt;

    pEvt = (btp_avrcpPlayerAppSettingChangeEvt_t *)pData;
    tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_playerAppSettingChgEvt:", pEvt->pParam, pEvt->paramLen);

    return TLK_ENONE;
}

/**
 * @brief       This function handles player addressed change events from AVRCP.
 * @param[in]   pData   - the event data.
 * @param[in]   dataLen - the length of the event data.
 * @return      TLK_ENONE if success, otherwise error code.
*/
static int tlkmdi_btavrcp_playerAddressChgEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_avrcpPlayerAddressedChangeEvt_t *pEvt;

    pEvt = (btp_avrcpPlayerAddressedChangeEvt_t *)pData;
    tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_playerAddressChgEvt:", pEvt->pParam, pEvt->paramLen);

    return TLK_ENONE;
}
#if (TLKBTP_CFG_AVRC_COVER_ART_ENABLE)

/**
 * @brief       This function handles cover art get image events from AVRCP.
 * @param[in]   pData   - the event data.
 * @param[in]   dataLen - the length of the event data.
 * @return      TLK_ENONE if success, otherwise error code.
*/
static int tlkmdi_btavrcp_coverArtGetImageEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_coverArtGetImageEvt_t *pEvt;

    btp_coverArt_item_t *pCoverArt;

    pEvt = (btp_coverArtGetImageEvt_t *)pData;

    if (pEvt->status != BTP_COVERARTC_EVT_STATUS_DATA) {
        tlkapi_warn(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_coverArtGetImageEvt: Abort");
        return -TLK_EFAIL;
    }

    if (!btp_a2dp_isSnk(pEvt->handle)) {
        tlkapi_error(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_coverArtGetImageEvt: error handle[%d]", pEvt->handle);
        return -TLK_EILLEGAL;
    }

    pCoverArt = btp_coverArt_getUsedNode(pEvt->handle, BTP_USRID_CLIENT);
    if (pCoverArt == NULL) {
        tlkapi_error(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_coverArtGetImageEvt: no coverArt item");
        return -TLK_ENOITEM;
    }
    /*TODO: Customer display mode to be confirmed*/
    if (pEvt->isFirst) {}

    if (pEvt->obex_state == 0xA0) { //'0xa0 == obex_ok'
        sTlkMdiCoverArtRecvLen += pEvt->bodyLen;
        tlk_printf("[obex_ok!!!!] sTlkMdiCoverArtTotalLen [%d], totalLen [%d]", sTlkMdiCoverArtRecvLen, pEvt->totalLen);
        sTlkMdiCoverArtRecvLen = 0;
    } else if (pEvt->obex_state == 0x90) { // obex continue
        sTlkMdiCoverArtRecvLen += pEvt->bodyLen;
    } else {
        tlkapi_error(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_coverArtGetImageEvt: error status[0x%0x]", pEvt->obex_state);
        return -TLK_ESTATUS;
    }

    return TLK_ENONE;
}


#endif
/**
 * @brief       This function handles instruction complete callbacks from AVRCP.
 * @param[in]   aclHandle - the ACL connection handle.
 * @param[in]   pduID     - the PDU ID.
 * @param[in]   status    - the status.
 * @param[in]   pParam    - the parameter.
 * @param[in]   paramLen  - the length of the parameter.
 * @return      none.
*/
static void tlkmdi_btavrcp_insCompleteCB(uint16_t aclHandle, uint8_t pduID, uint8_t status, void *pParam, uint16_t paramLen)
{
    (void)paramLen;
    (void)aclHandle;

    if (status != BTP_AVRCP_STATUS_CODE_OPERATE_WITHOUT_ERROR) {
        tlkapi_error(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_insCompleteCB: pduID[0x%02x], status[%d]", pduID, status);
        return;
    }
    if (pduID == BTP_AVRCP_PDUID_GET_PLAY_STATUS) {
        btp_avrcp_getPlayStatusRsp_t *pRsp = (btp_avrcp_getPlayStatusRsp_t *)pParam;
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "songLength: %dms, songPosition: %dms, playStatus: %d", pRsp->songLength, pRsp->songPosition,
                     pRsp->playStatus);
    } else if (pduID == BTP_AVRCP_PDUID_GET_ELEMENT_ATTR) { // Metadata Attributes for Current Media Item
        btp_avrcp_getElementAttrRsp_t *pRsp = (btp_avrcp_getElementAttrRsp_t *)pParam;
        (void)pRsp;

        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_insCompleteCB-BTP_AVRCP_PDUID_GET_ELEMENT_ATTR: number[%d]", pRsp->number);

#if (TLKBTP_CFG_COVERARTCLT_ENABLE)

        btp_coverArt_item_t *pCoverArt = btp_coverArt_getUsedNode(aclHandle, BTP_USRID_CLIENT);
        if (pCoverArt == NULL) {
            // tlkapi_error(TLKMDI_BTA2DP_DBG_FLAG,
            //              TLKMDI_BTA2DP_DBG_SIGN,
            //              "tlkmdi_btavrcp_insCompleteCB	pCoverArt note error");
            return;
        }

        uint8_t  buffer[256];
        uint16_t buffLen = 0;
        uint16_t tmpLen  = 0;
        bool     isGet   = false;

        tmemset(buffer, 0, 256);
        for (uint8_t index = 0; index < pRsp->number; index++) {
            if (pRsp->attID[index] != BTP_ELEMENTS_ATTR_ID_TITLE && pRsp->attID[index] != BTP_ELEMENTS_ATTR_ID_ARTIST_NAME &&
                pRsp->attID[index] != BTP_ELEMENTS_ATTR_ID_ALBUM_NAME && pRsp->attID[index] != BTP_ELEMENTS_ATTR_ID_COVER_ART) {
                continue;
            }

            buffer[buffLen++] = (pRsp->attID[index] & 0xFF);
            buffer[buffLen++] = (pRsp->charSet[index] & 0xFF);
            buffer[buffLen++] = ((pRsp->charSet[index] >> 8) & 0xFF);
            buffer[buffLen++] = (pRsp->length[index] & 0xFF);
            buffer[buffLen++] = ((pRsp->length[index] >> 8) & 0xFF);
            tmemcpy(buffer + buffLen, pRsp->pValue[index], pRsp->length[index]);
            buffLen += pRsp->length[index];

            if (pRsp->attID[index] == BTP_ELEMENTS_ATTR_ID_TITLE) {
                tmpLen = (pRsp->length[index] > IMAGE_TITLE_LEN_MAX) ? IMAGE_TITLE_LEN_MAX : pRsp->length[index];
                tmemset(pCoverArt->unit.imgTitle, 0, IMAGE_TITLE_LEN_MAX);
                tmemcpy(pCoverArt->unit.imgTitle, pRsp->pValue[index], tmpLen);
            }

            if (pRsp->attID[index] == BTP_ELEMENTS_ATTR_ID_ARTIST_NAME) {
                tmpLen = (pRsp->length[index] > IMAGE_ARTIST_LEN_MAX) ? IMAGE_ARTIST_LEN_MAX : pRsp->length[index];

                if (tmpLen == 0 || tmemcmp(pCoverArt->unit.imgArtist, pRsp->pValue[index], tmpLen) == 0) {
                    isGet = false;
                    continue;
                }

                /*The content that each device receives is not fixed every time. It could be a portion of the overall content.*/
                if (strstr((char *)pRsp->pValue[index], (char *)pCoverArt->unit.imgArtist) != NULL ||
                    strstr((char *)pCoverArt->unit.imgArtist, (char *)pRsp->pValue[index]) != NULL) {
                    tmemset(pCoverArt->unit.imgArtist, 0, IMAGE_ARTIST_LEN_MAX);
                    tmemcpy(pCoverArt->unit.imgArtist, pRsp->pValue[index], tmpLen);
                    isGet = false;
                    continue;
                }

                // if (tmemcmp(pCoverArt->unit.imgArtist, pRsp->pValue[index], strlen((char*)pCoverArt->unit.imgArtist)) ==
                //     0) {
                //     tmemset(pCoverArt->unit.imgArtist, 0, IMAGE_ARTIST_LEN_MAX);
                //     tmemcpy(pCoverArt->unit.imgArtist, pRsp->pValue[index], tmpLen);
                //     continue;
                // }
                isGet = true;
                tlk_printf("BTP_ELEMENTS_ATTR_ID_ARTIST_NAME true");
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, "[BIP]", "old:", pCoverArt->unit.imgArtist, tmpLen);
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, "[BIP]", "new:", pRsp->pValue[index], tmpLen);

                tmemset(pCoverArt->unit.imgArtist, 0, IMAGE_ARTIST_LEN_MAX);
                tmemcpy(pCoverArt->unit.imgArtist, pRsp->pValue[index], tmpLen);
            }

            if (pRsp->attID[index] == BTP_ELEMENTS_ATTR_ID_ALBUM_NAME) {
                tmpLen = (pRsp->length[index] > IMAGE_ARTIST_LEN_MAX) ? IMAGE_ARTIST_LEN_MAX : pRsp->length[index];

                if (tmpLen == 0 || tmemcmp(pCoverArt->unit.imgAblum, pRsp->pValue[index], tmpLen) == 0) {
                    isGet = false;
                    continue;
                }

                isGet = true;

                tlk_printf("BTP_ELEMENTS_ATTR_ID_ALBUM_NAME true");
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, "[BIP]", "old:", pCoverArt->unit.imgAblum, tmpLen);
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, "[BIP]", "new:", pRsp->pValue[index], tmpLen);

                tmemset(pCoverArt->unit.imgAblum, 0, IMAGE_ARTIST_LEN_MAX);
                tmemcpy(pCoverArt->unit.imgAblum, pRsp->pValue[index], tmpLen);
            }

            if (pRsp->attID[index] == BTP_ELEMENTS_ATTR_ID_COVER_ART) {
                tmpLen = (pRsp->length[index] > IMAGE_HANDLE_LEN_MAX) ? IMAGE_HANDLE_LEN_MAX : pRsp->length[index];

                if (tmemcmp(pCoverArt->unit.imgHandle, pRsp->pValue[index], tmpLen) != 0) {
                    tlk_printf("BTP_ELEMENTS_ATTR_ID_COVER_ART true isGet: %d", isGet);
                    tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, "[BIP]", "old:", pCoverArt->unit.imgHandle, tmpLen);
                    tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, "[BIP]", "new:", pRsp->pValue[index], tmpLen);

                    pCoverArt->unit.imgHandleLen = tmpLen;
                    tmemset(pCoverArt->unit.imgHandle, 0, IMAGE_HANDLE_LEN_MAX);
                    tmemcpy(pCoverArt->unit.imgHandle, pRsp->pValue[index], tmpLen);

                    if (isGet) {
                        tlk_printf("Start get image");
                        pCoverArt->flags |= BTP_COVERARTC_FLAG_WAIT_GET_IMAGE_HANDLE;
                        btp_coverArtclt_startGetImage(aclHandle);
                    }
                }
            }
        }

        /*The APP must enable the function allowing external devices to display lyrics; otherwise, this feature will only trigger once when switching songs.*/
        /*TODO: It is necessary to confirm the display method of the customer.*/
        if (1 || pCoverArt->isReport) {
            //send report
            //Send to UART Tool, just test for CoverArt.
            tlkmdi_comm_sendEvt(TLKPRT_COMM_MTYPE_AUDIO, TLKPRT_COMM_EVTID_AUDIO_LYRICS_REPORT, buffer, buffLen);
        }
#endif

    } else if (pduID == BTP_AVRCP_PDUID_SET_ABSOLUTE_VOLUME) { // Absolute Volume
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_insCompleteCB-BTP_AVRCP_PDUID_SET_ABSOLUTE_VOLUME:");
    } else if (pduID == BTP_AVRCP_PDUID_ADD_PLAYING) { // SetAddressedPlayer, AV/C CONTROL
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btavrcp_insCompleteCB-BTP_AVRCP_PDUID_ADD_PLAYING:");
    } else { //
        tlkapi_warn(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_insCompleteCB: Unknown pduID[%d]", pduID);
    }
}
#if (TLKBTP_CFG_AVRCP_BROWSING_ENABLE)
/**
 * @brief       This function handles item report callbacks from browsing.
 * @param[in]   aclHandle - the ACL connection handle.
 * @param[in]   pduID     - the PDU ID.
 * @param[in]   status    - the status.
 * @param[in]   itemType  - the item type.
 * @param[in]   itemNumb  - the number of items.
 * @param[in]   pItemData - the item data.
 * @return      none.
*/
static void tlkmdi_btBrowsing_itemsReportCB(uint16_t aclHandle, uint8_t pduID, uint8_t status, uint8_t itemType, uint8_t itemNumb, void *pItemData)
{
    if (status != TLK_ENONE) {
        tlkapi_error(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_itemsReportCB: item parse error [%d]", status);
        return;
    }
    if (itemType == BTP_BROWSING_ITEM_TYPE_MEDIA_PLAYER_ITEM) {
        uint8_t                    index;
        btp_browsing_playerItem_t *pItem = (btp_browsing_playerItem_t *)pItemData;
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_itemsReportCB: BTP_BROWSING_ITEM_TYPE_MEDIA_PLAYER_ITEM [%d]", itemNumb);
        for (index = 0; index < itemNumb; index++) {
            tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_MEDIA_PLAYER_ITEM: playerID[%d],playerType[%d],playStatus[%d]",
                         pItem[index].playerID, pItem[index].playerType, pItem[index].playStatus);
        }
        if (itemNumb != 0) {
            btp_browsing_setBrowsedPlayerCmd(aclHandle, pItem[0].playerID);
        }
    } else if (itemType == BTP_BROWSING_ITEM_TYPE_FOLDER_ITEM) {
        uint8_t                    index;
        btp_browsing_folderItem_t *pItem = (btp_browsing_folderItem_t *)pItemData;
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_itemsReportCB: BTP_BROWSING_ITEM_TYPE_FOLDER_ITEM [%d]", itemNumb);
        for (index = 0; index < itemNumb; index++) {
            tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_FOLDER_ITEM: folderType[%d],playabe[%d]", pItem[index].folderType,
                         pItem[index].playabe);
        }
    } else if (itemType == BTP_BROWSING_ITEM_TYPE_MEDIA_ELEMENT_ITEM) {
        uint8_t                     index;
        btp_browsing_elementItem_t *pItem = (btp_browsing_elementItem_t *)pItemData;
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_itemsReportCB: BTP_BROWSING_ITEM_TYPE_MEDIA_ELEMENT_ITEM [%d]", itemNumb);
        for (index = 0; index < itemNumb; index++) {
            tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_MEDIA_ELEMENT_ITEM: mediaType[%d],charSet[0x%x]", pItem[index].mediaType,
                         pItem[index].charSet);
        }
    } else if (itemType == BTP_BROWSING_ITEM_TYPE_ATTR_LIST) {
        uint8_t                  index;
        btp_browsing_attrItem_t *pItem = (btp_browsing_attrItem_t *)pItemData;
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_itemsReportCB: BTP_BROWSING_ITEM_TYPE_ATTR_LIST [%d]", itemNumb);
        for (index = 0; index < itemNumb; index++) {
            if (pItem[index].attrID == BTP_BROWSING_ATTID_TITLE) {
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_ATTR_LIST[Title]: ", pItem[index].pValue, pItem[index].valLens);
            } else if (pItem[index].attrID == BTP_BROWSING_ATTID_ARTIST_NAME) {
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_ATTR_LIST[ArtistName]: ", pItem[index].pValue, pItem[index].valLens);
            } else if (pItem[index].attrID == BTP_BROWSING_ATTID_ALBUM_NAME) {
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_ATTR_LIST[AlbumName]: ", pItem[index].pValue, pItem[index].valLens);
            } else if (pItem[index].attrID == BTP_BROWSING_ATTID_TRACK_NUMBER) {
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_ATTR_LIST[TrackNumb]: ", pItem[index].pValue, pItem[index].valLens);
            } else if (pItem[index].attrID == BTP_BROWSING_ATTID_TOTAL_TRACKS) {
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_ATTR_LIST[TotalTracks]: ", pItem[index].pValue, pItem[index].valLens);
            } else if (pItem[index].attrID == BTP_BROWSING_ATTID_GENRE) {
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_ATTR_LIST[Genre]: ", pItem[index].pValue, pItem[index].valLens);
            } else if (pItem[index].attrID == BTP_BROWSING_ATTID_PLAYING_TIME) {
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_ATTR_LIST[PlayTime]: ", pItem[index].pValue, pItem[index].valLens);
            } else if (pItem[index].attrID == BTP_BROWSING_ATTID_DEF_COVER_ART) {
                tlkapi_array(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_ATTR_LIST[CoverArt]: ", pItem[index].pValue, pItem[index].valLens);
            } else {
                tlkapi_warn(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "BTP_BROWSING_ITEM_TYPE_ATTR_LIST: unknown attrID [%d]", pItem[index].attrID);
            }
        }
    } else {
        tlkapi_warn(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_itemsReportCB: Unknown itemType[%d]", itemType);
    }
}

/**
 * @brief       This function handles instruction complete callbacks from browsing.
 * @param[in]   aclHandle - the ACL connection handle.
 * @param[in]   pduID     - the PDU ID.
 * @param[in]   status    - the status.
 * @param[in]   pParam    - the parameter.
 * @param[in]   paramLen  - the length of the parameter.
 * @return      none.
*/
static void tlkmdi_btBrowsing_insCompleteCB(uint16_t aclHandle, uint8_t pduID, uint8_t status, void *pParam, uint16_t paramLen)
{
    if (status != BTP_AVRCP_STATUS_CODE_OPERATE_WITHOUT_ERROR) {
        tlkapi_error(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_insCompleteCB: pduID[0x%02x], status[%d]", pduID, status);
        return;
    }
    if (pduID == BTP_AVRCP_PDUID_SET_BROWSED_PLAYER) {
    } else if (pduID == BTP_AVRCP_PDUID_CHANGE_PATH) {
        btp_browsing_changePathRsp_t *pRsp = (btp_browsing_changePathRsp_t *)pParam;
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_insCompleteCB-BTP_AVRCP_PDUID_CHANGE_PATH: itemNumb[%d]", pRsp->itemNumb);
    } else if (pduID == BTP_AVRCP_PDUID_GET_ITEM_ATTR) {
        btp_browsing_getItemAttrRsp_t *pRsp = (btp_browsing_getItemAttrRsp_t *)pParam;
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_insCompleteCB-BTP_AVRCP_PDUID_GET_ITEM_ATTR: itemNumb[%d]", pRsp->itemNumb);
    } else if (pduID == BTP_AVRCP_PDUID_SEARCH) {
        btp_browsing_searchRsp_t *pRsp = (btp_browsing_searchRsp_t *)pParam;
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_insCompleteCB-BTP_AVRCP_PDUID_SEARCH: itemNumb[%d]", pRsp->itemNumb);
    } else if (pduID == BTP_AVRCP_PDUID_GET_FOLDER_ITEMS) {
        btp_browsing_getFolderItemRsp_t *pRsp = (btp_browsing_getFolderItemRsp_t *)pParam;
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_insCompleteCB-BTP_AVRCP_PDUID_GET_TOTAL_ITEMS: uidCounter[%d],itemNumb[%d]",
                     pRsp->uidCounter, pRsp->itemNumb);
    } else if (pduID == BTP_AVRCP_PDUID_GET_TOTAL_ITEMS) {
        btp_browsing_getTotalItemRsp_t *pRsp = (btp_browsing_getTotalItemRsp_t *)pParam;
        tlkapi_trace(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_insCompleteCB-BTP_AVRCP_PDUID_GET_TOTAL_ITEMS: uidCounter[%d],itemNumb[%d]",
                     pRsp->uidCounter, pRsp->itemNumb);
    } else if (pduID == BTP_AVRCP_PDUID_GENERAL_REJECT) {
    } else {
        tlkapi_warn(TLKMDI_BTA2DP_DBG_FLAG, TLKMDI_BTA2DP_DBG_SIGN, "tlkmdi_btBrowsing_insCompleteCB: Unknown pduID[%d]", pduID);
    }
}
#endif // TLKBTP_CFG_AVRCP_BROWSING_ENABLE

#endif
