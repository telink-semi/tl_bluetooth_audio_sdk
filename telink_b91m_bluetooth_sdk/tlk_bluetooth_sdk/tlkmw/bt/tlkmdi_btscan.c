/********************************************************************************************************
 * @file    tlkmdi_btscan.c
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
#include "tlkmdi_btscan.h"
#include "stack/bt/host/bth/bth_stdio.h"


#if (TLK_STK_BT_ENABLE)
#define TLKMDI_BTREC_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_REC << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTREC_DBG_SIGN "[MDI]"

static tlkmdi_bt_scan_t sTlkMdiBtScanCtrl = {0};

/**
 * @brief       This function handles the Bluetooth scan timer, adjusting the scan state based on the timeout.
 * @param[in]   pTimer    - the handle of the timer.
 * @param[in]   userArg   - the user argument passed to the timer callback (unused in this function).
 * @return      none.
 * @note        If the timeout is set to 0, the scan remains active. Otherwise, it decrements the timeout and 
 *              restarts the timer every 5 seconds, logging the timeout and scan state. When the timeout reaches 0, 
 *              it disables Bluetooth scan.
 */
static void tlkmdi_btSetScan_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    //If timeout is set to 0, keep scan on.
    if (sTlkMdiBtScanCtrl.timeout == 0) {
        return;
    }

    if (sTlkMdiBtScanCtrl.timeout % 5 == 0) { //show log per 5 second
        tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btSetScan_timer timeout:%d,cur_scan %d", sTlkMdiBtScanCtrl.timeout, sTlkMdiBtScanCtrl.cur_scan_state);
    }

    sTlkMdiBtScanCtrl.timeout--;
    if (sTlkMdiBtScanCtrl.timeout) {
        tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
        return;
    }
    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
}

/**
 * @brief       This function initializes the Bluetooth scan process by creating a static timer.
 * @return      none.
 * @note        The timer is configured to run in the HOST task, with a time unit defined by TLKMDI_BTSCAN_TIME_UNIT.
 *              It uses the tlkmdi_btSetScan_timer function as its callback.
 */
void tlkmdi_btScan_process_init(void)
{
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &sTlkMdiBtScanCtrl.timer, TLKMDI_BTSCAN_TIME_UNIT, false, tlkmdi_btSetScan_timer, &sTlkMdiBtScanCtrl);
}

/**
 * @brief       This function processes the HCI command complete event callback for setting the scan.
 * @param[in]   pData   - pointer to the data received in the HCI command event. Only include result status.
 * @param[in]   dataLen - length of hci event buffer.
 * @return      none.
 * @note        The function checks the status of the HCI command and updates the scan state accordingly.
 */
int tlkmdi_btSetScan_hciCmdEvt_cb(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t status = *pData;
    tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btSetScan_hciCmdEvt_cb status:%d,cur_state:%d, waiting_value:%d,waiting_flag %d,pending_value:%d,timeout:%d",
                 status, sTlkMdiBtScanCtrl.cur_scan_state, sTlkMdiBtScanCtrl.waiting_confirm_value, sTlkMdiBtScanCtrl.waiting_flag, sTlkMdiBtScanCtrl.pending_scan_value,
                 sTlkMdiBtScanCtrl.timeout);

    if (BTH_HCI_ERROR_NONE == status && sTlkMdiBtScanCtrl.waiting_flag) {
        sTlkMdiBtScanCtrl.cur_scan_state = sTlkMdiBtScanCtrl.waiting_confirm_value;
        if (sTlkMdiBtScanCtrl.cur_scan_state == TLKMDI_BTSCAN_MODE_BOTH_DISABLE) { /*scan timer expired or be set as both disable during open scan */
            tlksys_timer_stop(TLKSYS_TASKID_HOST, &sTlkMdiBtScanCtrl.timer);
            sTlkMdiBtScanCtrl.timeout = 0;
        } else {
            tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtScanCtrl.timer); /*Start timing the scan time */
            /*If new scan is 0xFFFF, that means only update the scan value, no need to start the timer.*/
            sTlkMdiBtScanCtrl.timeout = (sTlkMdiBtScanCtrl.waiting_confirm_value_time == 0xFFFF) ? sTlkMdiBtScanCtrl.timeout : sTlkMdiBtScanCtrl.waiting_confirm_value_time;
        }
    }
    sTlkMdiBtScanCtrl.waiting_flag               = 0;
    sTlkMdiBtScanCtrl.waiting_confirm_value      = 0;
    sTlkMdiBtScanCtrl.waiting_confirm_value_time = 0;

    if (sTlkMdiBtScanCtrl.pending_scanflag) {
        if (bth_hci_sendWriteScanEnableCmd(sTlkMdiBtScanCtrl.pending_scan_value) == TLK_ENONE) {
            sTlkMdiBtScanCtrl.waiting_flag               = 1;
            sTlkMdiBtScanCtrl.waiting_confirm_value      = sTlkMdiBtScanCtrl.pending_scan_value;
            sTlkMdiBtScanCtrl.waiting_confirm_value_time = sTlkMdiBtScanCtrl.pending_scan_value_time;
        } else {
            tlkapi_error(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btSetScan_hciCmdEvt_cb scan_value write pending_scan HCI SEND FAIL!");
        }
        sTlkMdiBtScanCtrl.pending_scan_value      = 0;
        sTlkMdiBtScanCtrl.pending_scan_value_time = 0;
        sTlkMdiBtScanCtrl.pending_scanflag        = 0;
    }
    return 0;
}

/**
 * @brief       This function is called by the application to set the Bluetooth scan mode.
 * @param[in]   scan_value - the scan mode to be set.
 * @param[in]   time       - time duration, if scan_value is TLKMDI_BTSCAN_MODE_BOTH_DISABLE time will be neglect. Unit:S.
 * 					         If value is not TLKMDI_BTSCAN_MODE_BOTH_DISABLE and time is set as 0, the scan will always on.
 *                           If time is set as 0xFFFF, then only update the scan value, timeout continues to count down.
 * @return      none.
 * @note        If the scan mode is already set to the same value, the function returns mode immediately. If the scan mode is different,
 *              the function sends a HCI command to set the scan mode. If the HCI command is sent successfully, the function sets a timer
 *              to wait for the HCI command complete event. If the HCI command fails, the function sets the scan mode as pending and
 *              waits for the next scan mode change. If the scan mode is set to TLKMDI_BTSCAN_MODE_BOTH_DISABLE, the function stops the timer.
 *              If the scan mode is set to a value other than TLKMDI_BTSCAN_MODE_BOTH_DISABLE, the function starts the timer with the specified time.
 */
void tlkmdi_btSet_scan(uint8_t scan_value, uint16_t time)
{
    if (tlkmdi_bt_isReady() == false) {
        return;
    }
    tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN,
                 "tlkmdi_btSet_scan setscan_time %d-%d,cur_val:%d wait_flag-val:%d-%d, cur_timeout:%d,sTlkMdiBtScanCtrl.pending-value %d", scan_value, time,
                 sTlkMdiBtScanCtrl.cur_scan_state, sTlkMdiBtScanCtrl.waiting_flag, sTlkMdiBtScanCtrl.waiting_confirm_value, sTlkMdiBtScanCtrl.timeout,
                 sTlkMdiBtScanCtrl.pending_scan_value);

    if (scan_value > TLKMDI_BTSCAN_MODE_BOTH_DISABLE && tlkmdi_btacl_getIdleCount() == 0) {
        tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btSet_scan FAIL no resource");
        return;
    }
    if (scan_value > TLKMDI_BTSCAN_MODE_BOTH_SCAN) {
        return;
    }

    if (scan_value == sTlkMdiBtScanCtrl.cur_scan_state && sTlkMdiBtScanCtrl.waiting_flag == 0 && sTlkMdiBtScanCtrl.pending_scan_value_time == 0) {
        tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btSet_scan scan_value same as current value return!!");
        if (scan_value != TLKMDI_BTSCAN_MODE_BOTH_DISABLE) {
            /*only update the time length*/
            if (time != 0xFFFF) {
                sTlkMdiBtScanCtrl.timeout = time;
            }
        }
        return;
    }

    if (scan_value == sTlkMdiBtScanCtrl.waiting_confirm_value && sTlkMdiBtScanCtrl.waiting_flag && sTlkMdiBtScanCtrl.pending_scan_value_time == 0) {
        tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btSet_scan scan_value same as waiting_confirm_value return!!");
        if (scan_value != TLKMDI_BTSCAN_MODE_BOTH_DISABLE) {
            /*only update the time length*/
            if (time != 0xFFFF) {
                sTlkMdiBtScanCtrl.waiting_confirm_value_time = time;
            }
        }
        return;
    }

    if (sTlkMdiBtScanCtrl.pending_scan_value_time && scan_value == sTlkMdiBtScanCtrl.pending_scan_value) {
        if (scan_value != TLKMDI_BTSCAN_MODE_BOTH_DISABLE) {
            tlkapi_trace(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btSet_scan scan_value same as pending_scan_value return!!");
            /*only update the time length*/
            if (time != 0xFFFF) {
                sTlkMdiBtScanCtrl.pending_scan_value_time = time;
            }
        }
        return;
    }

    if (sTlkMdiBtScanCtrl.waiting_flag == 0) {
        if (bth_hci_sendWriteScanEnableCmd(scan_value) == TLK_ENONE) {
            sTlkMdiBtScanCtrl.waiting_flag               = 1;
            sTlkMdiBtScanCtrl.waiting_confirm_value      = scan_value;
            sTlkMdiBtScanCtrl.waiting_confirm_value_time = time;
        } else {
            tlkapi_error(TLKMDI_BTREC_DBG_FLAG, TLKMDI_BTREC_DBG_SIGN, "tlkmdi_btSet_scan scan_value HCI SEND FAIL!");
        }
    } else {
        sTlkMdiBtScanCtrl.pending_scan_value      = scan_value;
        sTlkMdiBtScanCtrl.pending_scan_value_time = time;
        sTlkMdiBtScanCtrl.pending_scanflag        = 1;
    }
}

/**
 * @brief       This function get the current scan state based on the scan control flags.
 * @return      The current state or pending state(**consider will be set successful) .
 * @note        If waiting_flag is not set, it returns cur_scan_state.
 *              If pending_scan_value_time is set, it returns pending_scan_value.
 *              Otherwise, it returns waiting_confirm_value.
 */
uint8_t tlkmdi_btGetScan_state(void)
{
    if (sTlkMdiBtScanCtrl.waiting_flag == 0) {
        return sTlkMdiBtScanCtrl.cur_scan_state;
    }
    if (sTlkMdiBtScanCtrl.pending_scan_value_time) {
        return sTlkMdiBtScanCtrl.pending_scan_value;
    }
    return sTlkMdiBtScanCtrl.waiting_confirm_value;
}

/**
 * @brief       This function returns the remaining scan time from the scan control structure.
 * @return      The remaining scan time.
 * @note        
 */
uint16_t tlkmdi_btscan_getRemainedScanTime(void)
{
    return sTlkMdiBtScanCtrl.timeout;
}

/**
 * @brief       This function retrieves the current scan state.
 * @return      The current scan state.
 * @note        
 */
uint8_t tlkmdi_btscan_getCurScanState(void)
{
    return sTlkMdiBtScanCtrl.cur_scan_state;
}

#endif //#if (TLK_STK_BT_ENABLE)
