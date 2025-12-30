/********************************************************************************************************
 * @file    tlkmdi_btpairing.c
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
#include "tlkmw/bt/tlkmw_bt.h"

#if (TLK_MW_BTPAIRING_ENABLE)

#define TLKMDI_BTREC_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_REC << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTREC_DBG_SIGN "[MDI]"

#define TLKMDI_BTSPAIRING_MODE_TIME_S          120     //120S

#define TLKMDI_BTSPAIRING_CHECK_INTERVAL       5000    //5ms
#define TLKMDI_BTSPAIRING_CHECK_INTERVAL_100MS 100000  // 100ms

typedef enum
{
    TLKMDI_BTPAIRING_MODE_IDLE,
    TLKMDI_BTPAIRING_MODE_WAITING_DISABLE_SCAN,
    TLKMDI_BTPAIRING_MODE_WAITING_CLEAN_LINK, /*cancel bt reconnection, or disconnect link*/
    TLKMDI_BTPAIRING_MODE_CLEAN_LINK,
    TLKMDI_BTPAIRING_MODE_WAITING_ACTIVE,
    TLKMDI_BTPAIRING_MODE_ACTIVE,
} TLKMDI_BTPAIRING_MODE_ENUM;

typedef struct
{
    uint16_t       state;
    uint16_t       timeout; /* the number of 1s unit */
    TlkApiTimer_t  timer;
} tlkmdi_bt_pairing_t;

static tlkmdi_bt_pairing_t sTlkMdiBtParingCtrl = {0};
static TlkMdiBtPairingCB   sTlkMdiBtParingCB   = NULL;

/**
 * @brief      bt pairing timer callback
 * @param[in]  pTimer: timer handle
 * @param[in]  userArg: user argument
 * @returns    none
*/
static void tlkmdi_btParing_timer(TlkApiTimerHandle_t pTimer, void* userArg)
{
    (void)pTimer;
    (void)userArg;
    uint8_t bt_link_count = tlkmdi_btacl_getUsedCount();

    if (sTlkMdiBtParingCtrl.state == TLKMDI_BTPAIRING_MODE_WAITING_DISABLE_SCAN) {
        if (tlkmdi_btGetScan_state() == TLKMDI_BTSCAN_MODE_BOTH_DISABLE) {
            sTlkMdiBtParingCtrl.state = TLKMDI_BTPAIRING_MODE_WAITING_CLEAN_LINK;
        } else {
            tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
            return;
        }
    }

    if (sTlkMdiBtParingCtrl.state == TLKMDI_BTPAIRING_MODE_WAITING_CLEAN_LINK) {
        if (tlkmdi_btRecon_isInBusy()) {
            tlkmdi_btRecon_close();
            sTlkMdiBtParingCtrl.state = TLKMDI_BTPAIRING_MODE_CLEAN_LINK;
        }
        if (bt_link_count) {
            for (uint8_t i = 0; i < TLKMDI_BTACL_ITEM_NUMB; i++) {
                tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getConnItemByIndex(i);
                if (pItem) {
                    tlkapi_array(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btParing_timer discon btaddr", pItem->btaddr, 6);
                    tlkmdi_btacl_disconn(pItem->handle, 0x13);
                }
            }
            sTlkMdiBtParingCtrl.state = TLKMDI_BTPAIRING_MODE_CLEAN_LINK;
        }

        if (sTlkMdiBtParingCtrl.state == TLKMDI_BTPAIRING_MODE_WAITING_CLEAN_LINK) { /* no link need to be clean */
            sTlkMdiBtParingCtrl.state = TLKMDI_BTPAIRING_MODE_WAITING_ACTIVE;
        }
    }


    if (sTlkMdiBtParingCtrl.state == TLKMDI_BTPAIRING_MODE_CLEAN_LINK) {
        if (tlkmdi_btRecon_isInBusy() == false && bt_link_count == 0) {
            sTlkMdiBtParingCtrl.state = TLKMDI_BTPAIRING_MODE_WAITING_ACTIVE;
        }
    }

    if (sTlkMdiBtParingCtrl.state == TLKMDI_BTPAIRING_MODE_WAITING_ACTIVE) {
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, TLKMDI_BTSPAIRING_MODE_TIME_S);
        sTlkMdiBtParingCtrl.state = TLKMDI_BTPAIRING_MODE_ACTIVE;
        tlksys_timer_setPeriod(TLKSYS_TASKID_HOST,&sTlkMdiBtParingCtrl.timer,TLKMDI_BTSPAIRING_CHECK_INTERVAL_100MS);

        sTlkMdiBtParingCtrl.timeout = (TLKMDI_BTSPAIRING_MODE_TIME_S * 1000 * 1000) / TLKMDI_BTSPAIRING_CHECK_INTERVAL_100MS; /* counter pairing mode timeout 2mins*/
        if (sTlkMdiBtParingCB != NULL) {
            sTlkMdiBtParingCB(TLKMDI_BTPAIRING_CB_STATE_ACTIVE);
        }
    }

    if (sTlkMdiBtParingCtrl.state == TLKMDI_BTPAIRING_MODE_ACTIVE) {
        if (sTlkMdiBtParingCtrl.timeout == 0) {
            sTlkMdiBtParingCtrl.state = TLKMDI_BTPAIRING_MODE_IDLE;
            if (sTlkMdiBtParingCB != NULL) {
                sTlkMdiBtParingCB(TLKMDI_BTPAIRING_CB_STATE_TIMEOUT);
            }
            tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btParing_timer timeout over!");
            return;
        }

        sTlkMdiBtParingCtrl.timeout--;
        if (bt_link_count) {
            sTlkMdiBtParingCtrl.timeout = 0;
            tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btParing_timer timeout BT connected!");
            return;
        }

    }
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
}

/**
 * @brief  Register callback function for bt pairing.
 *
 * @param[in] cb :Callback function
 *
 * @returns  none.
 */
void tlkmdi_btPairing_regCB(TlkMdiBtPairingCB cb)
{
    sTlkMdiBtParingCB = cb;
}

/**
 * @brief   Check if the bt pairing mode is in progress.
 *
 * @param[in]  none.
 *
 * @returns   The function returns a bool value.
 *            - true: Bluetooth pairing mode is in progress.
 *            - false: Bluetooth pairing mode is not in progress.
 */
bool tlkmdi_btPairing_isInProgress(void)
{
    return sTlkMdiBtParingCtrl.state != TLKMDI_BTPAIRING_MODE_IDLE;
}

/**
 * @brief  start bt pairing.
 *
 * @param[in] none.
 *
 * @return The process result,TLK_ENONE means success, negative value mean fail.
 * 
 * @note Not thread safe,must call in bt host's thread.
 */
int tlkmdi_btParing_start(void)
{
    tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btParing_start sTlkMdiBtParingCtrl.state %x", sTlkMdiBtParingCtrl.state);
    if(tlkmdi_bt_isReady() == false){
        return -TLK_ENOREADY;
    }
    if (sTlkMdiBtParingCtrl.state != TLKMDI_BTPAIRING_MODE_ACTIVE && sTlkMdiBtParingCtrl.state != TLKMDI_BTPAIRING_MODE_IDLE) {
        tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btParing_start busy");
        return -TLK_EBUSY;
    }

    tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtParingCtrl.timer);

    memset(&sTlkMdiBtParingCtrl, 0, sizeof(sTlkMdiBtParingCtrl));

    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &sTlkMdiBtParingCtrl.timer, TLKMDI_BTSPAIRING_CHECK_INTERVAL, false, tlkmdi_btParing_timer, &sTlkMdiBtParingCtrl);
    sTlkMdiBtParingCtrl.state = TLKMDI_BTPAIRING_MODE_IDLE;

    if (tlkmdi_btGetScan_state() != TLKMDI_BTSCAN_MODE_BOTH_DISABLE) {
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
        sTlkMdiBtParingCtrl.state = TLKMDI_BTPAIRING_MODE_WAITING_DISABLE_SCAN;
    } else {
        sTlkMdiBtParingCtrl.state = TLKMDI_BTPAIRING_MODE_WAITING_CLEAN_LINK;
    }


    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtParingCtrl.timer);

    return TLK_ENONE;
}

/**
 * @brief  stop bt pairing.
 *
 * @param[in] none.
 *
 * @return The process result,TLK_ENONE means success, negative value mean fail.
 * 
 * @note Not thread safe,must call in bt host's thread.
 */
int tlkmdi_btParing_stop(void)
{
    if(sTlkMdiBtParingCtrl.state == TLKMDI_BTPAIRING_MODE_IDLE){
        return TLK_ENONE;
    }
    tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btParing_stop");
    tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtParingCtrl.timer);
    if(sTlkMdiBtParingCtrl.state == TLKMDI_BTPAIRING_MODE_ACTIVE && sTlkMdiBtParingCB != NULL){  
        sTlkMdiBtParingCB(TLKMDI_BTPAIRING_CB_STATE_CANCEL);
    }
    memset(&sTlkMdiBtParingCtrl, 0, sizeof(sTlkMdiBtParingCtrl));
    return TLK_ENONE;
}

#endif //#if (TLK_STK_BT_ENABLE)
