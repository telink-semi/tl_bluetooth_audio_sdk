/********************************************************************************************************
 * @file    tlkmdi_bt_recon.c
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
#include "stack/bt/host/bth/bth_stdio.h"
#include "tlkmw/bt/tlkmw_bt.h"
#if (TLK_MW_BTREC_ENABLE)

#define TLKMDI_BTREC_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_REC << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTREC_DBG_SIGN "[MDI]"

static tlkmdi_btrecon_t sTlkMdiBtReconCtrl = {0};

/**
 * @brief       This function checks the Bluetooth reconnection is in a busy state.
 * @return      true is busy. If the state is between TLKMDI_BTRECON_STATE_IDLE and TLKMDI_BTRECON_STATE_WAIT_STOP.
 *              false not busy.
 * @note        
 */
bool tlkmdi_btRecon_isInBusy(void)
{
    if (sTlkMdiBtReconCtrl.state > TLKMDI_BTRECON_STATE_IDLE && sTlkMdiBtReconCtrl.state < TLKMDI_BTRECON_STATE_WAIT_STOP) {
        return true;
    }
    return false;
}

/**
 * @brief       This function get current reconnect state.
 * @param[in]   none;
 * @return      sTlkMdiBtReconCtrl.state.
 * @note        none;
 */
uint8_t tlkmdi_get_btRecon_state(void)
{
    return sTlkMdiBtReconCtrl.state;
}

/**
 * @brief       This function get current page addr of remote device.
 * @param[in]   none;
 * @return      BT addr pointer.
 * @note        none;        
 */
uint8_t *tlkmdi_btRecon_getPageAddr(void)
{
    if (sTlkMdiBtReconCtrl.state != TLKMDI_BTRECON_STATE_IDLE) {
        return sTlkMdiBtReconCtrl.pageAddr;
    }
    return NULL;
}

/**
 * @brief       This function is timer callback function for bt reconnect flow management.
 * @param[in]   pTimer. The timer struct pointer.
 * @param[in]   userArg. The user argument.
 * @return      True: need run contiune, false stop.
 * @note        none.
 */
static void tlkmdi_btRecon_timer(TlkApiTimerHandle_t pTimer, void* userArg)
{
    (void)userArg;
    int ret = 0;
    tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btRecon_timer recon  state:%d !", sTlkMdiBtReconCtrl.state);
    if (sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_STATE_IDLE) {
        return;
    }

    if (sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_STATE_PAGE) {
        tlkmdi_btacl_cancel(sTlkMdiBtReconCtrl.pageAddr);
        sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_STATE_WAIT_PAGE_CANCEL; /* wait next time page, During this time, scan should be opened*/
    } else if (sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_STATE_INTERVAL) {
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
        sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_STATE_START;
    }else if(sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_CANCEL_SCAN){
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
        sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_STATE_START;
    }
    if (sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_STATE_START) {
        if (sTlkMdiBtReconCtrl.retry_num) {
            tlksys_timer_setPeriod(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon, TLKMDI_BTRECON_PAGE_INTERVAL);
            ret = tlkmdi_btacl_connect(sTlkMdiBtReconCtrl.pageAddr, sTlkMdiBtReconCtrl.devClass, TLKMDI_BTRECON_PAGE_INTERVAL);
            if (ret == TLK_ENONE) {
                sTlkMdiBtReconCtrl.retry_num--;
                sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_STATE_PAGE;
            } else {
                uint8_t idle_acl = tlkmdi_btacl_getIdleCount();

                tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btRecon_start fail ret 0x%x", ret);
                tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_state_check);
                tmemset(&sTlkMdiBtReconCtrl, 0, sizeof(tlkmdi_btrecon_t));
                if (idle_acl == TLKMDI_BTACL_ITEM_NUMB) {
                    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
                } else if (idle_acl) {
                    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 120);
                }
                return;
            }
        } else {
            uint8_t idle_acl = tlkmdi_btacl_getIdleCount();
            tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_state_check);
            tmemset(&sTlkMdiBtReconCtrl, 0, sizeof(tlkmdi_btrecon_t));

            tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btRecon_check_timer recon over-FAIL!");

            if (idle_acl == TLKMDI_BTACL_ITEM_NUMB) {
                tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
            } else if (idle_acl) {
                tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 120);
            }
            return;
        }
    }
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
}

/**
 * @brief       This function is timer callback function for BT acl state check during reconnection.
 * @param[in]   pTimer. The timer struct pointer.
 * @param[in]   userArg. The user argument.
 * @return      True: need run contiune, false stop.
 * @note        none.
 */
static void tlkmdi_btRecon_check_timer(TlkApiTimerHandle_t pTimer, void* userArg)
{
    (void)pTimer;
    (void)userArg;
    tlkmdi_btacl_item_t *pItem;

    if (sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_STATE_IDLE) {
        return;
    }
    pItem = tlkmdi_btacl_searchUsedItem(sTlkMdiBtReconCtrl.pageAddr);
    if (sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_STATE_PAGE) {
        if (pItem && pItem->state == TLK_STATE_CONNECT) { /* page success  waiting for profile connection */

            tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btRecon_check_timer TLK_STATE_CONNECT ");
            sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_STATE_WAIT_PROFILE;
            tlksys_timer_setPeriod(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon, TLKMDI_BTRECON_PAGE_INTERVAL);
            tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon);
        } else if (pItem == NULL) { /* page fail  waiting for next  page retray */
            sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_STATE_START;
            // tlksys_timer_setPeriod(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon, 1);
            tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btRecon_check_timer page success but profile FAIL wait rety! ");
        }

    } else if (sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_STATE_WAIT_PAGE_CANCEL && pItem == NULL) {
        sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_STATE_INTERVAL;

        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, TLKMDI_BTRECON_INTERVAL_TIME / 1000000 + 2);
        tlksys_timer_setPeriod(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon, TLKMDI_BTRECON_INTERVAL_TIME);
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon);

    } else if (sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_STATE_WAIT_STOP && pItem == NULL) { /* reconnect stop by user */
        tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon);
        tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_state_check);
        tmemset(&sTlkMdiBtReconCtrl, 0, sizeof(tlkmdi_btrecon_t));
        return;

    } else if (sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_STATE_WAIT_PROFILE) {
        if (pItem && pItem->connFlag != 0) { /* page success and there is one profile connected at least, so judge reconnection is success and finished */
            tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btRecon_check_timer recon over-SUCCESS!");
            tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon);
            tmemset(&sTlkMdiBtReconCtrl, 0, sizeof(tlkmdi_btrecon_t));
            // send app msg ,initial second dev reconnet
            return;
        } else if (pItem == NULL) { /* page fail  waiting for next  page retray */
            // sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_STATE_START;
            sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_CANCEL_SCAN;
            // tlksys_timer_setPeriod(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon, 1);
        } else {
            // waitint profile connect.
        }
    }
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
}

/**
 * @brief       This function is start BT reconnection process call by app layer.
 * @param[in]   pPageAddr : Remote device addr pointer. 
 * @param[in]   devClass  : The class of device of remote device.
 * @param[in]   retry_num : Reconnection retry number(number 1 is about 12s,10s page+2s open page_sacn)
 * @return      The process result.
 * @note        none.
 */
int tlkmdi_btRecon_start(uint8_t *pPageAddr, uint32_t devClass, uint8_t retry_num)
{
    int ret = 0;
    if(tlkmdi_bt_isReady() == false){
        return -TLK_ENOREADY;
    }
    if (sTlkMdiBtReconCtrl.state != TLKMDI_BTRECON_STATE_IDLE) {
        return -TLK_EBUSY;
    }
    if (tlkmdi_btGetScan_state() != TLKMDI_BTSCAN_MODE_BOTH_DISABLE) {
        tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
    }

    tmemset(&sTlkMdiBtReconCtrl, 0, sizeof(tlkmdi_btrecon_t));
    tmemcpy(sTlkMdiBtReconCtrl.pageAddr, pPageAddr, 6);
    sTlkMdiBtReconCtrl.retry_num = retry_num;
    sTlkMdiBtReconCtrl.devClass  = devClass;

    tlksys_timer_createStatic(TLKSYS_TASKID_HOST,&sTlkMdiBtReconCtrl.timer_recon, TLKMDI_BTRECON_PAGE_INTERVAL, false, tlkmdi_btRecon_timer, &sTlkMdiBtReconCtrl);
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST,&sTlkMdiBtReconCtrl.timer_state_check, TLKMDI_BTRECON_CHECK_INTERVAL, false, tlkmdi_btRecon_check_timer, &sTlkMdiBtReconCtrl);
    ret = tlkmdi_btacl_connect(sTlkMdiBtReconCtrl.pageAddr, sTlkMdiBtReconCtrl.devClass, TLKMDI_BTRECON_PAGE_INTERVAL);
    if (ret == TLK_ENONE) {
        sTlkMdiBtReconCtrl.retry_num--;
        sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_STATE_PAGE;
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon);
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_state_check);
    } else {
        uint8_t idle_acl = tlkmdi_btacl_getIdleCount();
        tmemset(&sTlkMdiBtReconCtrl, 0, sizeof(tlkmdi_btrecon_t));
        tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btRecon_start fail ret 0x%x", ret);
        if (idle_acl == TLKMDI_BTACL_ITEM_NUMB) {
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_SCAN, 120);
        } else if (idle_acl) {
            tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_PAGE_SCAN, 120);
        }
    }
    return TLK_ENONE;
}

/**
 * @brief       This function is stop BT reconnection process call by app layer.
 * @param[in]   none
 * @return      The process result.
 * @note        none.
 */
int tlkmdi_btRecon_close(void)
{
    tlkmdi_btacl_item_t *pItem;
    tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btRecon_close  sTlkMdiBtReconCtrl.state 0x%x", sTlkMdiBtReconCtrl.state);
    pItem = tlkmdi_btacl_searchUsedItem(sTlkMdiBtReconCtrl.pageAddr);
    if (sTlkMdiBtReconCtrl.state > TLKMDI_BTRECON_STATE_IDLE && sTlkMdiBtReconCtrl.state < TLKMDI_BTRECON_STATE_WAIT_PROFILE) {
        if (sTlkMdiBtReconCtrl.state == TLKMDI_BTRECON_STATE_PAGE && pItem && pItem->state == TLK_STATE_CONNING) {
            tlkmdi_btacl_cancel(sTlkMdiBtReconCtrl.pageAddr);
            sTlkMdiBtReconCtrl.state = TLKMDI_BTRECON_STATE_WAIT_STOP;
            return TLK_ENONE;
        }
    }
    tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_recon);
    tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtReconCtrl.timer_state_check);

    tmemset(&sTlkMdiBtReconCtrl, 0, sizeof(tlkmdi_btrecon_t));

    return TLK_ENONE;
}

/**
 * @brief       This function returns the remind retry_num from the recon control structure.
 * @return      The remaining retry_num.
 * @note        
 */
uint8_t tlkmdi_bt_recon_getRemindRetryNum(void)
{
    return sTlkMdiBtReconCtrl.retry_num;
}

#else

bool tlkmdi_btRecon_isInBusy(void)
{
    return false;
}

int tlkmdi_btRecon_close(void)
{
    return TLK_ENONE;
}

#endif
