/********************************************************************************************************
 * @file    tlkmdi_bt_tpt_data_sync.c
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
#include "tlkmw/tlkmw.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"
#include "stack/tpsll/host/tpsll_hostevent.h"
#include "stack/tpsll/common/bttpsll_tws_profile.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bt_tws/btp_tws_master.h"
#include "stack/bt/host/bt_tws/btp_tws_slave.h"
#if (TLKSTK_BTTPSLL_TWS_ENABLE)

typedef enum
{
    DATA_SYNC_TYPE_AVRCP_KEY = 0,
    DATA_SYNC_TYPE_SIRI,
    DATA_SYNC_TYPE_CALL_CTRL,
    DATA_SYNC_TYPE_VOL_CTRL,
    DATA_SYNC_TYPE_TPS_HID_KEY,
    DATA_SYNC_TYPE_TPT_BOX_STATE_SYNC,
    DATA_SYNC_TYPE_REQ_HAND_OVER,
    DATA_SYNC_TYPE_ANC_SYNC,

    DATA_SYNC_TYPE_A2DP_MEDIA_CHANNEL_SYNC,
    DATA_SYNC_TYPE_HFP_CODEC_SYNC,
    DATA_SYNC_TYPE_A2DP_CHECK_PLAY_STATUS,
    DATA_SYNC_TYPE_TWS_PAIRING_SYNC,
    DATA_SYNC_TYPE_TWS_EXIT_LOW_LATENCY_SYNC,
    DATA_SYNC_TYPE_MAX,
} DATA_SYNC_TYPE_ENUM;

typedef enum
{
    UI_TIMER_SYNC_TYPE_LED = 0,
    UI_TIMER_SYNC_TYPE_TONE,
    UI_TIMER_SYNC_TYPE_MAX,
} UI_TIMER_SYNC_TYPE_ENUM;

typedef void (*dataSync_dealFunc_t)(void *, uint16_t);

typedef struct
{
    uint8_t  key;
    uint8_t  resv;
    uint16_t handle;
} dataSync_avrcp_key_t;

typedef struct
{
    uint8_t resv1;
} dataSync_siri_ctrl_t;

typedef struct
{
    uint8_t  isAccept;
    uint8_t  resv;
    uint16_t handle;
} dataSync_call_ctrl_t;

typedef struct
{
    uint8_t  isMusic;
    uint8_t  vol;
    uint16_t handle;
} dataSync_vol_ctrl_t;

typedef struct
{
    uint8_t  key;
    uint8_t  resv;
    uint16_t handle;
} dataSync_tps_hid_t;

typedef struct
{
    TlkMdiTptCaseState_t state;
} dataSync_box_state_t;

typedef struct
{
    uint8_t state;
} dataSync_anc_state_t;

typedef struct
{
    uint16_t                      handle;
    uint16_t                      resv;
    bttpsll_tws_avdtp_media_chn_t mediaChn;
} dataSync_a2dp_media_t;

typedef struct
{
    uint8_t  a2dpStatus;
    uint8_t  resv;
    uint16_t handle;
} dataSync_a2dp_status_t;

static TlkApiTimer_t            tlkmdi_bt_tpt_dataSync_timer = {0};
static TlkmdiBtTptToneTmrSyncCB sTlkmdiBtTptToneTmrSyncCB    = NULL;

/**
 * @brief       Send AVRCP key event to remote device
 * @param[in]   data    - Pointer to the data containing key information
 * @param[in]   dataLen - Length of the data
 * @return      none
 * @note        This function parses the received data and sends corresponding AVRCP key press and release events
 */
static void tlkmdi_bt_tpt_data_sync_sendAvrcpKey(void *data, uint16_t dataLen)
{
    if (dataLen != sizeof(dataSync_avrcp_key_t)) {
        return;
    }
    dataSync_avrcp_key_t *msg = (dataSync_avrcp_key_t *)data;
    btp_avrcp_sendKeyPress(msg->handle, msg->key);
    btp_avrcp_sendKeyRelease(msg->handle, msg->key);
}

/**
 * @brief       Trigger Siri voice assistant on remote device
 * @param[in]   data    - Pointer to the data (unused)
 * @param[in]   dataLen - Length of the data (should match size of dataSync_siri_ctrl_t)
 * @return      none
 * @note        This function gets current HFP handle and triggers Siri assistant if SCO connection doesn't exist
 */
static void tlkmdi_bt_tpt_data_sync_sendSiriCtrl(void *data, uint16_t dataLen)
{
    (void)data;
    if (dataLen != sizeof(dataSync_siri_ctrl_t)) {
        return;
    }
    uint16_t handle = btp_hfphf_getCurHandle();
    if (bth_handle_searchUsedSco(handle) == NULL) {
        tlkmdi_bthfphf_assistant(handle);
    }
}

/**
 * @brief       Handle call control operations on remote device
 * @param[in]   data    - Pointer to the data containing call control information
 * @param[in]   dataLen - Length of the data (should match size of dataSync_call_ctrl_t)
 * @return      none
 * @note        This function handles both accepting and rejecting calls based on the isAccept flag in the data
 */
static void tlkmdi_bt_tpt_data_sync_sendCallCtrl(void *data, uint16_t dataLen)
{
    if (dataLen != sizeof(dataSync_call_ctrl_t)) {
        return;
    }
    dataSync_call_ctrl_t *msg    = (dataSync_call_ctrl_t *)data;
    uint16_t              handle = msg->handle;
    if (msg->handle == 0xFFFF) { // TODO:BQ Modify:ZW Only fit signal link, multiple point have bug, random hang up one of phone.
        handle = btp_hfphf_getCurHandle();
    }
    tlkmdi_hfphf_ctrl_t *pItem = tlkmdi_hfphf_getItem(handle);
    if (pItem == NULL) {
        return;
    }
    if (msg->isAccept) {
        if (pItem->status == BTP_HFP_CALL_STATUS_START && pItem->setup_status == BTP_HFP_CALL_DIR_INCOMING) {
            btp_hfphf_answer(handle);
        } else if (pItem->status == BTP_HFP_CALL_STATUS_WAITING && pItem->setup_status == BTP_HFP_CALL_DIR_INCOMING) {
            tlkmdi_bthfphf_acceptWaitAndHoldActive(handle);
        }
    } else {
        if (pItem->status == BTP_HFP_CALL_STATUS_START && pItem->setup_status == BTP_HFP_CALL_DIR_INCOMING) {
            btp_hfphf_reject(handle);
        } else if (pItem->status || pItem->setup_status || pItem->held_status || bth_handle_searchConnSco(handle) != 0) {
            if (pItem->status == BTP_HFP_CALL_STATUS_WAITING && pItem->setup_status == BTP_HFP_CALL_DIR_INCOMING) {
                tlkmdi_bthfphf_rejectWaitAndKeepActive(handle);
            } else if (pItem->status == BTP_HFP_CALL_STATUS_ACTIVE && pItem->held_status != 0) {
                tlkmdi_bthfphf_hungupActiveAndResumeHold(handle);
            } else {
                btp_hfphf_hungUp(handle);
            }
        }
    }
}

/**
 * @brief       Synchronize volume control between devices
 * @param[in]   data    - Pointer to the data containing volume control information
 * @param[in]   dataLen - Length of the data (should match size of dataSync_vol_ctrl_t)
 * @return      none
 * @note        This function synchronizes either music or voice volume between TWS devices and sends appropriate events
 */
static void tlkmdi_bt_tpt_data_sync_sendVolCtrl(void *data, uint16_t dataLen)
{
    if (dataLen != sizeof(dataSync_vol_ctrl_t)) {
        return;
    }
    dataSync_vol_ctrl_t *msg  = (dataSync_vol_ctrl_t *)data;
    tlkmdi_btacl_item_t *item = tlkmdi_btacl_getConnItem(msg->handle);
    if (item == NULL) {
        return;
    }
    uint8_t isIos = 0;
    tlkmdi_tinySql_getPairingDeviceVolume(item->btaddr, msg->isMusic, NULL, &isIos);
    tlkmdi_tinySql_setPairingDeviceVolume(item->btaddr, msg->isMusic, msg->vol, isIos);
    if (msg->isMusic) {
        btp_avrcp_setVolume(msg->handle, msg->vol, false);
        const uint8_t         bufferLen = sizeof(tlksys_msg_hostEvt_t) + sizeof(tlksys_msg_hostEvt_btVolChg_t);
        uint8_t               buffer[bufferLen];
        tlksys_msg_hostEvt_t *evt             = (tlksys_msg_hostEvt_t *)buffer;
        evt->dataLen                          = sizeof(tlksys_msg_hostEvt_btVolChg_t);
        evt->hostType                         = TLKSYS_MSG_HOST_TYPE_BT;
        evt->msgID                            = TLKSYS_MSG_BT_HOST_EVT_TYPE_VOLUME_CHG;
        tlksys_msg_hostEvt_btVolChg_t *evtDat = (tlksys_msg_hostEvt_btVolChg_t *)evt->data;
        evtDat->handle                        = msg->handle;
        evtDat->isMusic                       = true;
        evtDat->volume                        = msg->vol;
        tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_HOST_EVT_COME, buffer, bufferLen);
    } else {
        btp_hfphf_setSpkVolumeByHandle(msg->handle, msg->vol);
        const uint8_t         bufferLen = sizeof(tlksys_msg_hostEvt_t) + sizeof(tlksys_msg_hostEvt_btVolChg_t);
        uint8_t               buffer[bufferLen];
        tlksys_msg_hostEvt_t *evt             = (tlksys_msg_hostEvt_t *)buffer;
        evt->dataLen                          = sizeof(tlksys_msg_hostEvt_btVolChg_t);
        evt->hostType                         = TLKSYS_MSG_HOST_TYPE_BT;
        evt->msgID                            = TLKSYS_MSG_BT_HOST_EVT_TYPE_VOLUME_CHG;
        tlksys_msg_hostEvt_btVolChg_t *evtDat = (tlksys_msg_hostEvt_btVolChg_t *)evt->data;
        evtDat->handle                        = msg->handle;
        evtDat->isMusic                       = false;
        evtDat->volume                        = msg->vol;
        tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_HOST_EVT_COME, buffer, bufferLen);
    }
}

/**
 * @brief       Handle HID key events from TPS interface
 * @param[in]   data    - Pointer to the data containing HID key information
 * @param[in]   dataLen - Length of the data (should match size of dataSync_tps_hid_t)
 * @return      none
 * @note        This function maps HID keys to specific audio control functions like volume control and playback control
 */
static void tlkmdi_bt_tpt_data_sync_sendTpsHid(void *data, uint16_t dataLen)
{
    typedef int (*tphKeyFunc)(void);
    static const tphKeyFunc sTphKeyFuncs[AUD_TPSIF_KEYID_NUM] = {
        [AUD_TPSIF_KEYID_VOL_UP] = tlkmdi_bt_tpsll_keyFuncAudioVolumeUp,        [AUD_TPSIF_KEYID_VOL_DOWN] = tlkmdi_bt_tpsll_keyFuncAudioVolumeDown,
        [AUD_TPSIF_KEYID_PLAY_PAUSE] = tlkmdi_bt_tpsll_keyFuncMusicPlayPause,   [AUD_TPSIF_KEYID_PLAY_FORWARD] = tlkmdi_bt_tpsll_keyFuncMusicForward,
        [AUD_TPSIF_KEYID_PLAY_BACKWARD] = tlkmdi_bt_tpsll_keyFuncMusicBackward,
    };
    if (dataLen != sizeof(dataSync_tps_hid_t)) {
        return;
    }
    dataSync_tps_hid_t *msg = (dataSync_tps_hid_t *)data;
    uint8_t             key = msg->key;
    if (key >= AUD_TPSIF_KEYID_NUM) {
        return;
    }
    sTphKeyFuncs[key]();
}

/**
 * @brief       Synchronize box state between TWS devices
 * @param[in]   data    - Pointer to the data containing box state information
 * @param[in]   dataLen - Length of the data (should match size of dataSync_box_state_t)
 * @return      none
 * @note        This function updates the peer device's case state information
 */
static void tlkmdi_bt_tpt_data_sync_sendBoxState(void *data, uint16_t dataLen)
{
    if (dataLen != sizeof(dataSync_box_state_t)) {
        return;
    }
    dataSync_box_state_t *msg = (dataSync_box_state_t *)data;
    tlkmdi_bt_tpt_setPeerCaseState(msg->state);
}

/**
 * @brief       Request handover operation between TWS devices
 * @param[in]   data    - Pointer to the data (unused)
 * @param[in]   dataLen - Length of the data (unused)
 * @return      none
 * @note        This function sends a message to initiate the handover process
 */
static void tlkmdi_bt_tpt_data_sync_reqHandOver(void *data, uint16_t dataLen)
{
    (void)data;
    (void)dataLen;
    tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_TPT_MSGID_START_HANDOVER, NULL, 0);
}

/**
 * @brief       Synchronize ANC (Active Noise Cancellation) state between TWS devices
 * @param[in]   data    - Pointer to the data containing ANC state information
 * @param[in]   dataLen - Length of the data (should match size of dataSync_anc_state_t)
 * @return      none
 * @note        This function sends a message to synchronize ANC state with the audio task
 */
static void tlkmdi_bt_tpt_data_sync_ancSync(void *data, uint16_t dataLen)
{
    if (dataLen != sizeof(dataSync_anc_state_t)) {
        return;
    }
    dataSync_anc_state_t *msg = (dataSync_anc_state_t *)data;
    tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_ENABLE_ANC, &msg->state, sizeof(msg->state));
}

/**
 * @brief       Synchronize A2DP media channel information between TWS devices
 * @param[in]   data    - Pointer to the data containing A2DP media channel information
 * @param[in]   dataLen - Length of the data (should match size of dataSync_a2dp_media_t)
 * @return      none
 * @note        This function forwards A2DP media channel handling to the TWS module
 */
static void tlkmdi_bt_tpt_data_sync_sendA2dpMediaSync(void *data, uint16_t dataLen)
{
    if (dataLen != sizeof(dataSync_a2dp_media_t)) {
        return;
    }

    dataSync_a2dp_media_t *msg = (dataSync_a2dp_media_t *)data;
    btp_tws_avdtp_media_channel_handle(msg->handle, (bttpsll_tws_avdtp_media_chn_t *)&msg->mediaChn);
}

/**
 * @brief       Synchronize A2DP play status between TWS devices
 * @param[in]   data    - Pointer to the data containing A2DP status information
 * @param[in]   dataLen - Length of the data (should match size of dataSync_a2dp_status_t)
 * @return      none
 * @note        This function checks the play status and synchronizes it between master and slave devices
 */
static void tlkmdi_bt_tpt_data_sync_sendA2dpCheckPlayStatus(void *data, uint16_t dataLen)
{
    if (dataLen != sizeof(dataSync_a2dp_status_t)) {
        return;
    }

    dataSync_a2dp_status_t *msg = (dataSync_a2dp_status_t *)data;
    if (tlkmdi_bt_tpt_isMaster()) {
        tlkmdi_bt_tpt_syncA2dpStatusDataHandler(msg->handle, NULL, 0);
    } else if (tlkmdi_bt_tpt_isSlave()) {
        tlkapi_printf(1, "[DATA_SYNC] <info> S check - recv play status %d from master", msg->a2dpStatus);
        if (msg->a2dpStatus == BTP_A2DP_STATUS_STREAM) {
            btp_tws_set_a2dp_stream_status();
        }
    }
}

static const dataSync_dealFunc_t tlkmdi_bt_tpt_data_sync_dealFuncs[DATA_SYNC_TYPE_MAX] = {
    [DATA_SYNC_TYPE_AVRCP_KEY]                 = tlkmdi_bt_tpt_data_sync_sendAvrcpKey,
    [DATA_SYNC_TYPE_SIRI]                      = tlkmdi_bt_tpt_data_sync_sendSiriCtrl,
    [DATA_SYNC_TYPE_CALL_CTRL]                 = tlkmdi_bt_tpt_data_sync_sendCallCtrl,
    [DATA_SYNC_TYPE_VOL_CTRL]                  = tlkmdi_bt_tpt_data_sync_sendVolCtrl,
    [DATA_SYNC_TYPE_TPS_HID_KEY]               = tlkmdi_bt_tpt_data_sync_sendTpsHid,
    [DATA_SYNC_TYPE_TPT_BOX_STATE_SYNC]        = tlkmdi_bt_tpt_data_sync_sendBoxState,
    [DATA_SYNC_TYPE_REQ_HAND_OVER]             = tlkmdi_bt_tpt_data_sync_reqHandOver,
    [DATA_SYNC_TYPE_ANC_SYNC]                  = tlkmdi_bt_tpt_data_sync_ancSync,
    [DATA_SYNC_TYPE_A2DP_MEDIA_CHANNEL_SYNC]   = tlkmdi_bt_tpt_data_sync_sendA2dpMediaSync,
    [DATA_SYNC_TYPE_A2DP_CHECK_PLAY_STATUS]    = tlkmdi_bt_tpt_data_sync_sendA2dpCheckPlayStatus,
    [DATA_SYNC_TYPE_TWS_PAIRING_SYNC]          = tlkmdi_bt_tpt_pair_start_sync_from_remote,
    [DATA_SYNC_TYPE_TWS_EXIT_LOW_LATENCY_SYNC] = tlkmdi_bt_tpt_headset_recvPeerExitLowLatencyHandler,
};

/**
 * @brief       This function processes data synchronization based on the specified type.
 * @param[in]   type    - the type of data synchronization to process.
 * @param[in]   pData   - pointer to the data buffer.
 * @param[in]   dataLen - length of the data in bytes.
 * @return      none.
 */
static void tlkmdi_bt_tpt_data_sync_getData(uint8_t type, uint8_t *pData, uint16_t dataLen)
{
    if (type >= DATA_SYNC_TYPE_MAX) {
        return;
    }
    tlkmdi_bt_tpt_data_sync_dealFuncs[type](pData, dataLen);
}

/**
 * @brief       Extract and process ACL data from received packet
 * @param[in]   pData    - Pointer to the received data packet
 * @param[in]   dataLen  - Length of the data packet
 * @return      TLK_ENONE - Operation completed successfully
 * @note        This function extracts the data type and length from the packet,
 *              then forwards the actual data to the processing function.
 *              The data format is: [reserved][reserved][type][len][data...]
 */
static int tlkmdi_bt_tpt_get_acl_data(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t type = pData[2];
    uint8_t len  = pData[3];
    tlkmdi_bt_tpt_data_sync_getData(type, pData + 4, len);
    return TLK_ENONE;
}

/**
 * @brief       Send data synchronization command to peer TWS device
 * @param[in]   type    - Type of data to be synchronized, refer to DATA_SYNC_TYPE_ENUM
 * @param[in]   pData   - Pointer to the data buffer to be sent
 * @param[in]   dataLen - Length of the data in bytes
 * @return      TLK_ENONE if successful or not connected, otherwise error code from tpsll_hci_sendAclDataCmd
 * @note        This function sends ACL data command to all headset devices in the TWS network.
 *              If TWS is not connected, it returns TLK_ENONE directly.
 */
static int tlkmdi_bt_tpt_data_sync_send(uint8_t type, uint8_t *pData, uint16_t dataLen)
{
    if (!tlkmdi_bt_tpt_isConnected()) {
        return TLK_ENONE;
    }
    return tpsll_hci_sendAclDataCmd(BTTPSLL_TWS_MSG_LINK_ID_HEADSET_ALL, BTTPSLL_TWS_MSG_ID_ACL, type, pData, dataLen);
}

/**
 * @brief       Check if data synchronization needs to be sent based on sync type
 * @param[in]   type    - Type of data synchronization, refer to DATA_SYNC_TYPE_ENUM.
 * @return      true if data needs to be sent, false otherwise
 * @note        Certain sync types always require sending, others depend on device role.
 *              When device is slave, remaining types also require sending.
 */
static bool tlkmdi_bt_tpt_data_sync_isNeedSend(uint8_t type)
{
    if (type == DATA_SYNC_TYPE_VOL_CTRL || type == DATA_SYNC_TYPE_A2DP_MEDIA_CHANNEL_SYNC || type == DATA_SYNC_TYPE_TWS_PAIRING_SYNC || type == DATA_SYNC_TYPE_ANC_SYNC ||
        type == DATA_SYNC_TYPE_TWS_EXIT_LOW_LATENCY_SYNC || type == DATA_SYNC_TYPE_A2DP_CHECK_PLAY_STATUS || type == DATA_SYNC_TYPE_TPT_BOX_STATE_SYNC) {
        return true;
    }
    return tlkmdi_bt_tpt_isSlave();
}

/**
 * @brief       Execute TPT UI operation based on data sync type
 * @param[in]   type    - Type of data synchronization to process
 * @param[in]   pData   - Pointer to the data buffer
 * @param[in]   dataLen - Length of the data in bytes
 * @return      none
 * @note        This function determines whether data needs to be sent or received based on device role
 *              and data sync type, then performs the appropriate action
 */
static void tlkmdi_bt_tpt_ui_exec(uint8_t type, uint8_t *pData, uint16_t dataLen)
{
    /* Determine if data needs to be sent based on device role and sync type */
    bool isNeedSend = tlkmdi_bt_tpt_data_sync_isNeedSend(type);
    if (!isNeedSend) {
        /* Receive and process data from peer device */
        tlkmdi_bt_tpt_data_sync_getData(type, pData, dataLen);
    } else {
        /* Send data to peer device */
        tlkmdi_bt_tpt_data_sync_send(type, pData, dataLen);
    }
}

/**
 * @brief       Timer callback function to trigger LED synchronization
 * @param[in]   pTimer   - Timer handle that triggered this callback
 * @param[in]   userArg  - User argument passed to the timer callback
 * @return      none
 * @note        This function is called by the timer subsystem and requests 
 *              LED synchronization with the peer device
 */
static void tlkmdi_bt_tpt_ledSync(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    tlkmdi_bt_tpt_requestLedSync();
}

/**
 * @brief       Process UI timer synchronization data received from TWS connection
 * @param[in]   pData    - pointer to the data buffer containing synchronization information.
 * @param[in]   dataLen  - length of the data in bytes, should be 7 bytes.
 * @return      TLK_ENONE if successful, -TLK_EPARAM if data length is invalid.
 * @note        This function parses the sync data and performs LED or tone timer synchronization
 *              based on the type field. For tone sync, it sends a message to audio task if device
 *              is slave, and calls registered callback function.
 */
static int tlkmdi_bt_tpt_get_ui_timer_sync(uint8_t *pData, uint16_t dataLen)
{
    if (dataLen != 7) {
        return -TLK_EPARAM;
    }
    uint8_t  type      = pData[0];
    uint16_t magicWord = pData[1] | ((uint16_t)pData[2] << 8);
    uint32_t syncTick  = pData[3] | ((uint32_t)pData[4] << 8) | ((uint32_t)pData[5] << 16) | ((uint32_t)pData[6] << 24);
    if (type == UI_TIMER_SYNC_TYPE_LED) {
#if TLK_DEV_LED_ENABLE
        tlkdrv_led_reSyncAll(syncTick);
#endif
    } else if (type == UI_TIMER_SYNC_TYPE_TONE) {
        if (tlkmdi_bt_tpt_isSlave()) {
            uint8_t data[2] = {(uint8_t)magicWord, 0};
            tlk_printf("tlkmdi_bt_tpt_get_ui_timer_sync, tone %x", magicWord);
            tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_START_TONE_CMD, data, 2);
        }
        if (sTlkmdiBtTptToneTmrSyncCB) {
            sTlkmdiBtTptToneTmrSyncCB(syncTick);
        }
    }
    return TLK_ENONE;
}

TPSLL_EVT_REGISTER(TPSLL_EVTID_TWS_ACL_MSG_DEAL, tlkmdi_bt_tpt_get_acl_data);
TPSLL_EVT_REGISTER(TPSLL_EVTID_TWS_UI_TIMER_SYNC, tlkmdi_bt_tpt_get_ui_timer_sync);

/**
 * @brief       Initialize the data synchronization module
 * @param[in]   none
 * @return      none
 * @note        This function creates a static timer for LED synchronization
 */
void tlkmdi_bt_tpt_data_sync_init(void)
{
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &tlkmdi_bt_tpt_dataSync_timer, 12 * 1000 * 1000, true, tlkmdi_bt_tpt_ledSync, NULL);
}

/**
 * @brief       Send AVRCP key command to remote device
 * @param[in]   handle  - Connection handle
 * @param[in]   key     - Key code to send
 * @return      none
 * @note        This function prepares and executes AVRCP key command synchronization
 */
void tlkmdi_bt_tpt_sendAvrcpKey(uint16_t handle, uint8_t key)
{
    uint8_t               buffer[sizeof(dataSync_avrcp_key_t)];
    dataSync_avrcp_key_t *msg = (dataSync_avrcp_key_t *)buffer;
    msg->handle               = handle;
    msg->key                  = key;
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_AVRCP_KEY, buffer, sizeof(dataSync_avrcp_key_t));
}

/**
 * @brief       Trigger Siri voice assistant
 * @param[in]   none
 * @return      none
 * @note        This function prepares and executes Siri trigger command synchronization
 */
void tlkmdi_bt_tpt_triggerSiri(void)
{
    uint8_t buffer[sizeof(dataSync_siri_ctrl_t)];
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_SIRI, buffer, sizeof(dataSync_siri_ctrl_t));
}

/**
 * @brief       Control call operations (accept/reject)
 * @param[in]   handle   - Connection handle
 * @param[in]   isAccept - Flag indicating whether to accept (true) or reject (false) the call
 * @return      none
 * @note        This function prepares and executes call control command synchronization
 */
void tlkmdi_bt_tpt_callCtrl(uint16_t handle, uint8_t isAccept)
{
    uint8_t               buffer[sizeof(dataSync_call_ctrl_t)];
    dataSync_call_ctrl_t *msg = (dataSync_call_ctrl_t *)buffer;
    msg->isAccept             = isAccept;
    msg->handle               = handle;
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_CALL_CTRL, buffer, sizeof(dataSync_call_ctrl_t));
}

/**
 * @brief       Perform initial volume synchronization
 * @param[in]   handle - Connection handle
 * @return      none
 * @note        This function retrieves stored volume values and synchronizes them, only works on master device
 */
void tlkmdi_bt_tpt_volFirstSync(uint16_t handle)
{
    if (tlkmdi_bt_tpt_isSlave()) {
        return;
    }
    tlkmdi_btacl_item_t *item = tlkmdi_btacl_getConnItem(handle);
    if (item == NULL) {
        return;
    }
    uint8_t musicVol = 0;
    uint8_t voiceVol = 0;
    tlkmdi_tinySql_getPairingDeviceVolume(item->btaddr, true, &musicVol, NULL);
    tlkmdi_tinySql_getPairingDeviceVolume(item->btaddr, false, &voiceVol, NULL);
    tlkmdi_bt_tpt_volSync(handle, true, musicVol);
    tlkmdi_bt_tpt_volSync(handle, false, voiceVol);
}

/**
 * @brief       Synchronize volume between TWS devices
 * @param[in]   handle  - Connection handle
 * @param[in]   isMusic - Flag indicating if it's music volume (true) or voice volume (false)
 * @param[in]   vol     - Volume level to synchronize
 * @return      none
 * @note        This function prepares and executes volume synchronization command
 */
void tlkmdi_bt_tpt_volSync(uint16_t handle, uint8_t isMusic, uint8_t vol)
{
    uint8_t              buffer[sizeof(dataSync_vol_ctrl_t)];
    dataSync_vol_ctrl_t *msg = (dataSync_vol_ctrl_t *)buffer;
    msg->isMusic             = isMusic;
    msg->vol                 = vol;
    msg->handle              = handle;
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_VOL_CTRL, buffer, sizeof(dataSync_vol_ctrl_t));
}

/**
 * @brief       Send HID key command to remote device
 * @param[in]   handle - Connection handle
 * @param[in]   key    - HID key code to send
 * @return      none
 * @note        This function prepares and executes HID key command synchronization
 */
void tlkmdi_bt_tpt_sendHidkey(uint16_t handle, uint8_t key)
{
    uint8_t             buffer[sizeof(dataSync_tps_hid_t)];
    dataSync_tps_hid_t *msg = (dataSync_tps_hid_t *)buffer;
    msg->handle             = handle;
    msg->key                = key;
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_TPS_HID_KEY, buffer, sizeof(dataSync_tps_hid_t));
}

/**
 * @brief       Synchronize charging case state between TWS devices
 * @param[in]   state - Case state to synchronize
 * @return      none
 * @note        This function prepares and executes case state synchronization command
 */
void tlkmdi_bt_tpt_boxStateSync(TlkMdiTptCaseState_t state)
{
    uint8_t               buffer[sizeof(dataSync_box_state_t)];
    dataSync_box_state_t *msg = (dataSync_box_state_t *)buffer;
    msg->state                = state;
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_TPT_BOX_STATE_SYNC, buffer, sizeof(dataSync_box_state_t));
}

/**
 * @brief       Request handover operation between TWS devices
 * @param[in]   none
 * @return      none
 * @note        This function executes handover request command synchronization
 */
void tlkmdi_bt_tpt_reqHandOver(void)
{
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_REQ_HAND_OVER, NULL, 0);
}

/**
 * @brief       Synchronize ANC (Active Noise Cancellation) state between TWS devices
 * @param[in]   state - ANC state to synchronize
 * @return      none
 * @note        This function prepares and executes ANC state synchronization command
 */
void tlkmdi_bt_tpt_syncAncState(uint8_t state)
{
    uint8_t               buffer[sizeof(dataSync_anc_state_t)];
    dataSync_anc_state_t *msg = (dataSync_anc_state_t *)buffer;
    msg->state                = state;
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_ANC_SYNC, buffer, sizeof(dataSync_anc_state_t));
}

/**
 * @brief       Synchronize A2DP media data from master to slave
 * @param[in]   handle  - Connection handle
 * @param[in]   pData   - Pointer to media data
 * @param[in]   dataLen - Length of media data
 * @return      none
 * @note        This function prepares and executes A2DP media data synchronization from master to slave
 */
void tlkmdi_bt_tpt_masterSyncA2dpMediaDataHandler(uint16_t handle, uint8_t *pData, uint8_t dataLen) // master -> slave
{
    uint8_t                buffer[sizeof(dataSync_a2dp_media_t)];
    dataSync_a2dp_media_t *msg = (dataSync_a2dp_media_t *)buffer;
    msg->handle                = handle;
    tmemcpy(&msg->mediaChn, pData, dataLen);
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_A2DP_MEDIA_CHANNEL_SYNC, buffer, sizeof(dataSync_a2dp_media_t));
}

/**
 * @brief       Synchronize TWS pairing request between devices
 * @param[in]   isRefactory - Refactory mode flag
 * @return      none
 * @note        This function executes TWS pairing synchronization command
 */
void tlkmdi_bt_tpt_sync_tws_pairing(uint8_t isRefactory)
{
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_TWS_PAIRING_SYNC, &isRefactory, 1);
}

/**
 * @brief       Synchronize exit low latency mode between TWS devices
 * @param[in]   none
 * @return      none
 * @note        This function executes exit low latency mode synchronization command
 */
void tlkmdi_bt_tpt_sync_tws_exitLowLatency(void)
{
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_TWS_EXIT_LOW_LATENCY_SYNC, NULL, 0);
}

/**
 * @brief       Synchronize A2DP status data between TWS devices
 * @param[in]   handle  - Connection handle
 * @param[in]   pData   - Pointer to status data (unused)
 * @param[in]   dataLen - Length of status data (unused)
 * @return      none
 * @note        This function prepares and executes A2DP status synchronization between devices
 */
void tlkmdi_bt_tpt_syncA2dpStatusDataHandler(uint16_t handle, uint8_t *pData, uint8_t dataLen)
{
    (void)pData;
    (void)dataLen;

    uint8_t                 buffer[sizeof(dataSync_a2dp_status_t)];
    dataSync_a2dp_status_t *msg = (dataSync_a2dp_status_t *)buffer;
    msg->handle                 = handle;
#if (TLKBTP_CFG_A2DPSNK_ENABLE) // TODO: ZEWEN. N22 compilation needs to exclude this file
    msg->a2dpStatus = btp_a2dpsnk_getStatus(handle);
    tlkapi_printf(1, "[DATA_SYNC] <info> syncA2dpStatusDataHandler: A2DP_state:%d", msg->a2dpStatus);
#endif
    tlkmdi_bt_tpt_ui_exec(DATA_SYNC_TYPE_A2DP_CHECK_PLAY_STATUS, buffer, sizeof(dataSync_a2dp_status_t));
}

/**
 * @brief       Enable or disable automatic LED synchronization
 * @param[in]   en - Enable flag (true to enable, false to disable)
 * @return      TLK_ENONE - Operation completed successfully
 * @note        When enabled, this function requests initial LED sync and starts the sync timer
 */
int tlkmdi_bt_tpt_setAutoLedSync(bool en)
{
    if (en) {
        tlkmdi_bt_tpt_requestLedSync();
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &tlkmdi_bt_tpt_dataSync_timer);
    } else {
        tlksys_timer_stop(TLKSYS_TASKID_HOST, &tlkmdi_bt_tpt_dataSync_timer);
    }
    return TLK_ENONE;
}

/**
 * @brief       Request LED synchronization with peer device
 * @param[in]   none
 * @return      TLK_ENONE - Operation completed successfully
 * @note        This function only works on master device, sends LED sync request to slave
 */
int tlkmdi_bt_tpt_requestLedSync(void)
{
    if (tlkmdi_bt_tpt_isSlave()) {
        return TLK_ENONE;
    }
    return tpsll_hci_sendRequestSyncHostTimerCmd(UI_TIMER_SYNC_TYPE_LED, 0);
}

/**
 * @brief       Request tone synchronization with peer device
 * @param[in]   tone_id - Tone identifier to synchronize
 * @return      TLK_ENONE - Operation completed successfully
 * @note        This function only works on master device, sends tone sync request to slave
 */
int tlkmdi_bt_tpt_requestToneSync(uint8_t tone_id)
{
    if (tlkmdi_bt_tpt_isSlave()) {
        return TLK_ENONE;
    }
    tlk_printf("tlkmdi_bt_tpt_requestToneSync, tone %x", tone_id);
    return tpsll_hci_sendRequestSyncHostTimerCmd(UI_TIMER_SYNC_TYPE_TONE, tone_id);
}

/**
 * @brief       Register callback function for tone synchronization
 * @param[in]   cb - Callback function pointer
 * @return      TLK_ENONE - Operation completed successfully
 * @note        This function stores the provided callback for later use during tone synchronization
 */
int tlkmdi_bt_tpt_regToneSyncCB(TlkmdiBtTptToneTmrSyncCB cb)
{
    sTlkmdiBtTptToneTmrSyncCB = cb;
    return TLK_ENONE;
}
#endif
