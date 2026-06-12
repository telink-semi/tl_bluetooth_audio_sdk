/********************************************************************************************************
 * @file    tlkmdi_audio_btif_a2dp_src.c
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
#if (TLK_MW_AUDIO_ENABLE && TLKBTP_CFG_A2DPSRC_ENABLE && TEMP_A2DP_OUT_NEW_VERSION)
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "tlkmdi_audio_btif_a2dp.h"

#undef tlk_this
#define tlk_this ((tlkmdi_audio_btif_a2dp_src_mgr_t *)(&tlkmdi_audio_btif_a2dp_src_mgr))
//used as this same with cpp
#undef LOG_EN
#define LOG_EN 1

typedef struct
{
    uint8_t                           malloced;
    uint8_t                           state;
    uint16_t                          handle;
    uint32_t                          freq;
    TlkMdiAudBtifA2dpSrcStartResultCB cb;
    TlkApiTimer_t                     timer;
} tlkmdi_audio_btif_a2dp_src_item_t;

typedef struct
{
    tlkmdi_audio_btif_a2dp_src_item_t item[TLK_STK_BTACL_NUMB];
    TlkMdiAudBtifA2dpSrcStatusChgCB   status_chg_cb;
} tlkmdi_audio_btif_a2dp_src_mgr_t;

typedef struct
{
    uint16_t                          handle;
    uint16_t                          resv;
    uint32_t                          freq;
    TlkMdiAudBtifA2dpSrcStartResultCB cb;
} tlkmdi_audio_btif_a2dp_src_start_package_t;

typedef struct
{
    uint16_t                          handle;
    uint16_t                          resv;
    int32_t                           reason;
    TlkMdiAudBtifA2dpSrcStartResultCB cb;
} tlkmdi_audio_btif_a2dp_src_start_cb_package_t;

typedef enum
{
    TLKMDI_AUDIO_BTIF_A2DP_SRC_ITEM_STATE_NONE = 0,
    TLKMDI_AUDIO_BTIF_A2DP_SRC_ITEM_STATE_CONFIGING,
    TLKMDI_AUDIO_BTIF_A2DP_SRC_ITEM_STATE_STARTING,
} tlkmdi_audio_btif_a2dp_src_item_state_e;

static tlkmdi_audio_btif_a2dp_src_mgr_t tlkmdi_audio_btif_a2dp_src_mgr = {0};

static tlkmdi_audio_btif_a2dp_src_item_t *tlkmdi_audio_btif_a2dp_src_item_malloc(void)
{
    for (size_t i = 0; i < TLK_STK_BTACL_NUMB; i++) {
        if (tlk_this->item[i].malloced) {
            continue;
        }
        memset(&(tlk_this->item[i]), 0, sizeof(tlk_this->item[i]));
        tlk_this->item[i].malloced = 1;

        return &(tlk_this->item[i]);
    }
    return NULL;
}

static inline void tlkmdi_audio_btif_a2dp_src_item_free(tlkmdi_audio_btif_a2dp_src_item_t *item)
{
    item->malloced = 0;
}

static tlkmdi_audio_btif_a2dp_src_item_t *tlkmdi_audio_btif_a2dp_src_item_find(uint16_t handle)
{
    for (size_t i = 0; i < TLK_STK_BTACL_NUMB; i++) {
        if (tlk_this->item[i].malloced == 0 || tlk_this->item[i].handle != handle) {
            continue;
        }
        return &(tlk_this->item[i]);
    }
    return NULL;
}

static void tlkmdi_audio_btif_a2dp_src_item_send_start_result_thread_safe(void *pData, uint8_t dataLen)
{
    (void)dataLen;
    tlkmdi_audio_btif_a2dp_src_start_cb_package_t *package = (tlkmdi_audio_btif_a2dp_src_start_cb_package_t *)pData;
    package->cb(package->handle, package->reason);
}

static void tlkmdi_audio_btif_a2dp_src_item_send_start_result(tlkmdi_audio_btif_a2dp_src_item_t *item, int32_t reason)
{
    if (item->cb != NULL) {
        tlkmdi_audio_btif_a2dp_src_start_cb_package_t package = {
            .cb     = item->cb,
            .handle = item->handle,
            .reason = reason,
        };
        tlksys_runFuncInTaskWithArg(TLKSYS_TASKID_AUDIO, tlkmdi_audio_btif_a2dp_src_item_send_start_result_thread_safe, &package, sizeof(package));
    }

    tlkapi_printf(LOG_EN, "tlkmdi_audio_btif_a2dp_src_item_send_start_result [%d],[%d]", item->handle, reason);
    tlksys_timer_destroy(TLKSYS_TASKID_HOST, &item->timer);
    tlkmdi_audio_btif_a2dp_src_item_free(item);
}

static void tlkmdi_audio_btif_a2dp_src_item_timer(TlkApiTimerHandle_t timer, void *arg)
{
    (void)timer;
    tlkmdi_audio_btif_a2dp_src_item_t *item = (tlkmdi_audio_btif_a2dp_src_item_t *)arg;
    tlkmdi_audio_btif_a2dp_src_item_send_start_result(item, -TLK_ETIMEOUT);
}

static void tlkmdi_audio_btif_a2dp_src_item_state_to_starting(tlkmdi_audio_btif_a2dp_src_item_t *item)
{
    int ret = btp_a2dpsrc_start(item->handle);

    if (ret != TLK_ENONE) {
        btp_avrcp_notifyPlayState(item->handle, BTP_AVRCP_PLAY_STATE_PAUSED);
        tlkmdi_audio_btif_a2dp_src_item_send_start_result(item, ret);
    } else {
        item->state = TLKMDI_AUDIO_BTIF_A2DP_SRC_ITEM_STATE_STARTING;
        tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &item->timer, 3 * 1000 * 1000, 0, tlkmdi_audio_btif_a2dp_src_item_timer, item);
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &item->timer);
    }
}

static void tlkmdi_audio_btif_a2dp_src_item_state_to_reconfiging(tlkmdi_audio_btif_a2dp_src_item_t *item)
{
    int ret = btp_a2dpSrc_setSampleRate01(item->handle, item->freq);
    if (ret != TLK_ENONE) {
        tlkmdi_audio_btif_a2dp_src_item_send_start_result(item, ret);
        return;
    }
    // item->state = TLKMDI_AUDIO_BTIF_A2DP_SRC_ITEM_STATE_CONFIGING;
    // tlksys_timer_createStatic(TLKSYS_TASKID_HOST,&item->timer,3 * 1000 * 1000,0,tlkmdi_audio_btif_a2dp_src_item_timer,item);
    // tlksys_timer_reStart(TLKSYS_TASKID_HOST,&item->timer);

    //TODO: reconfig evt api can't be used,now directly to starting state
    tlkmdi_audio_btif_a2dp_src_item_state_to_starting(item);
}

static void tlkmdi_audio_btif_a2dp_src_start_core(uint16_t handle, uint32_t freq, TlkMdiAudBtifA2dpSrcStartResultCB cb)
{
    tlkmdi_audio_btif_a2dp_src_item_t *item = tlkmdi_audio_btif_a2dp_src_item_find(handle);
    if (item != NULL) {
        return;
    }
    item = tlkmdi_audio_btif_a2dp_src_item_malloc();
    if (item == NULL) {
        return;
    }
    item->handle = handle;
    item->freq   = freq;
    item->cb     = cb;
    btp_avrcp_notifyPlayState(handle, BTP_AVRCP_PLAY_STATE_PLAYING);

    if (btp_a2dpsrc_getSampleRate(handle) != freq) {
        tlkmdi_audio_btif_a2dp_src_item_state_to_reconfiging(item);
    } else {
        tlkmdi_audio_btif_a2dp_src_item_state_to_starting(item);
    }
}

static void tlkmdi_audio_btif_a2dp_src_start_thread_safe(void *pData, uint8_t dataLen)
{
    (void)dataLen;
    tlkmdi_audio_btif_a2dp_src_start_package_t *package = (tlkmdi_audio_btif_a2dp_src_start_package_t *)pData;
    tlkmdi_audio_btif_a2dp_src_start_core(package->handle, package->freq, package->cb);
}

static void tlkmdi_audio_btif_a2dp_src_stop_core(uint16_t handle)
{
    btp_avrcp_notifyPlayState(handle, BTP_AVRCP_PLAY_STATE_PAUSED);
    btp_a2dpsrc_suspend(handle);
}

static void tlkmdi_audio_btif_a2dp_src_stop_thread_safe(void *pData, uint8_t dataLen)
{
    (void)dataLen;
    uint16_t handle = *(uint16_t *)pData;
    tlkmdi_audio_btif_a2dp_src_stop_core(handle);
}

static int tlkmdi_audio_btif_a2dp_src_reConfigCompleteEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;

    btp_a2dpReconfigCompleteEvt_t     *pEvt   = (btp_a2dpReconfigCompleteEvt_t *)pData;
    uint16_t                           handle = pEvt->handle;
    tlkmdi_audio_btif_a2dp_src_item_t *item   = tlkmdi_audio_btif_a2dp_src_item_find(handle);
    if (item == NULL || item->state != TLKMDI_AUDIO_BTIF_A2DP_SRC_ITEM_STATE_CONFIGING) {
        return TLK_ENONE;
    }
    tlkmdi_audio_btif_a2dp_src_item_state_to_starting(item);
    if (pEvt->status != TLK_ENONE) {
        //TODO:
    }
    return TLK_ENONE;
}

BTP_EVT_REGISTER(BTP_EVTID_A2DPSRC_RECONFIG_COMPLETE, tlkmdi_audio_btif_a2dp_src_reConfigCompleteEvt);

static void tlkmdi_audio_btif_a2dp_src_run_status_evt_cb_thread_safe(void *pData, uint8_t dataLen)
{
    if (tlk_this->status_chg_cb == NULL || dataLen != sizeof(btp_a2dpStatusChangeEvt_t)) {
        return;
    }
    btp_a2dpStatusChangeEvt_t *pEvt     = (btp_a2dpStatusChangeEvt_t *)pData;
    uint8_t                    is_start = (pEvt->status == BTP_A2DP_STATUS_STREAM);
    tlk_this->status_chg_cb(pEvt->handle, is_start, pEvt->mtuSize);
}

static int tlkmdi_audio_btif_a2dp_src_statusEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_a2dpStatusChangeEvt_t *pEvt = (btp_a2dpStatusChangeEvt_t *)pData;
    if (pEvt->status == BTP_A2DP_STATUS_STREAM) {
        tlkmdi_audio_btif_a2dp_src_item_t *item = tlkmdi_audio_btif_a2dp_src_item_find(pEvt->handle);
        if (item != NULL && item->state == TLKMDI_AUDIO_BTIF_A2DP_SRC_ITEM_STATE_STARTING) {
            tlkmdi_audio_btif_a2dp_src_item_send_start_result(item, TLK_ENONE);
        }
        btp_avrcp_notifyPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PLAYING);
    } else {
        btp_avrcp_notifyPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PAUSED);
    }
    if (tlk_this->status_chg_cb == NULL) {
        return TLK_ENONE;
    }
    tlksys_runFuncInTaskWithArg(TLKSYS_TASKID_HOST, tlkmdi_audio_btif_a2dp_src_run_status_evt_cb_thread_safe, pEvt, sizeof(btp_a2dpStatusChangeEvt_t));
    return TLK_ENONE;
}

BTP_EVT_REGISTER(BTP_EVTID_A2DPSRC_STATUS_CHANGED, tlkmdi_audio_btif_a2dp_src_statusEvt);

// api is following:
void tlkmdi_audio_btif_a2dp_src_start(uint16_t handle, uint32_t freq, TlkMdiAudBtifA2dpSrcStartResultCB cb)
{
    tlkmdi_audio_btif_a2dp_src_start_core(handle, freq, cb);
    tlkmdi_audio_btif_a2dp_src_start_package_t package = {
        .handle = handle,
        .freq   = freq,
        .cb     = cb,
    };
    tlksys_runFuncInTaskWithArg(TLKSYS_TASKID_HOST, tlkmdi_audio_btif_a2dp_src_start_thread_safe, &package, sizeof(package));
}

void tlkmdi_audio_btif_a2dp_src_stop(uint16_t handle)
{
    tlksys_runFuncInTaskWithArg(TLKSYS_TASKID_HOST, tlkmdi_audio_btif_a2dp_src_stop_thread_safe, &handle, sizeof(handle));
}

void tlkmdi_audio_btif_a2dp_src_register_status_change_cb(TlkMdiAudBtifA2dpSrcStatusChgCB cb)
{
    tlk_this->status_chg_cb = cb;
}

#endif
