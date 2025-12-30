/********************************************************************************************************
 * @file    tlkmdi_bt_ctrl.c
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
#if (TLK_STK_BT_ENABLE)

/*Note: following enum or struct only use in this .c file*/
typedef enum
{
    NO_BUSY         = 0,
    FORCE_IDLE      = 1 << 0,
    WAIT_DISCON_ALL = 1 << 1,
} TLKMDI_BT_CTRL_BUSY_ENUM;

typedef struct
{
    uint32_t               busyFlag;
    TlkMdiBtCtrlFinishedCB cb;
    TlkApiTimer_t          timer;
} tlkmdi_bt_ctrl_t;

static tlkmdi_bt_ctrl_t sTlkMdiBtDestroyCtrl = {0};

/**
 * @brief       Disconnect all Bluetooth connections.
 *              This function iterates through all connection items and disconnects
 *              any active connections or cancels pending connection attempts.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmdi_bt_disconnectAll(void)
{
    for (uint8_t i = 0; i < TLKMDI_BTACL_ITEM_NUMB; i++) {
        tlkmdi_btacl_item_t *pItem = tlkmdi_btacl_getConnItemByIndex(i);
        if (pItem == NULL) {
            continue;
        }
        if (pItem->handle == 0) {
            tlkmdi_btacl_cancel(pItem->btaddr);
        } else {
            tlkmdi_btacl_disconn(pItem->handle, 0x13);
        }
    }
}

/**
 * @brief       Timer callback function for Bluetooth control operations.
 *              This function handles the timeout logic for forcing Bluetooth to idle state.
 *              It checks the current status and invokes the callback when appropriate.
 * @param[in]   pTimer   - timer handle.
 * @param[in]   userArg  - user argument (not used in this implementation).
 * @return      none.
 */
static void tlkmdi_bt_ctrl_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    if (sTlkMdiBtDestroyCtrl.busyFlag & FORCE_IDLE) {
        //case :reset to idle finished
        if (tlkmdi_btRecon_isInBusy()) {
            return;
        }
        if (tlkmdi_btacl_getUsedCount() != 0) {
            if ((sTlkMdiBtDestroyCtrl.busyFlag & WAIT_DISCON_ALL) == 0) {
                tlkmdi_bt_disconnectAll();
                sTlkMdiBtDestroyCtrl.busyFlag |= WAIT_DISCON_ALL;
            }
            return;
        }
        sTlkMdiBtDestroyCtrl.busyFlag &= ~FORCE_IDLE;
        sTlkMdiBtDestroyCtrl.busyFlag &= ~WAIT_DISCON_ALL;
        if (sTlkMdiBtDestroyCtrl.cb != NULL) {
            sTlkMdiBtDestroyCtrl.cb();
        }
        tlksys_timer_stop(TLKSYS_TASKID_HOST, pTimer);
    } else {
        tlksys_timer_stop(TLKSYS_TASKID_HOST, pTimer);
    }
}

/**
 * @brief Force bt to enter the idle state.
 *        This function stops bt scanning and disables Bluetooth reconnection. 
 *        Upon completion, the provided callback function will be invoked.
 *
 * @param[in] finishedCB  The callback function pointer that will be called upon completion of the operation.
 *                        - If the callback function is not needed, pass NULL.
 *                        - The callback function should be a valid function pointer to avoid undefined behavior.
 *
 * @return The process result,TLK_ENONE means success, others mean fail.
 */
int tlkmdi_bt_forceToIdle(TlkMdiBtCtrlFinishedCB finishedCB)
{
    if (sTlkMdiBtDestroyCtrl.busyFlag != NO_BUSY) {
        return -TLK_EBUSY;
    }
    sTlkMdiBtDestroyCtrl.cb = finishedCB;
    if (tlkmdi_btRecon_isInBusy()) {
        tlkmdi_btRecon_close();
    }
    tlkmdi_btParing_stop();
    tlkmdi_btSet_scan(TLKMDI_BTSCAN_MODE_BOTH_DISABLE, 0);
    sTlkMdiBtDestroyCtrl.busyFlag |= FORCE_IDLE;
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &sTlkMdiBtDestroyCtrl.timer, 50 * 1000, true, tlkmdi_bt_ctrl_timer, NULL);
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &sTlkMdiBtDestroyCtrl.timer);
    return TLK_ENONE;
}

/**
 * @brief Get bt state is ready.
 *        Not ready means bt host has not init or is in deinit/paused/force2idle process. 
 *
 * @param[in] none.
 * 
 * @return True means ready,false means not ready.
 */
bool tlkmdi_bt_isReady(void)
{
    return sTlkMdiBtDestroyCtrl.busyFlag == 0;
}

#endif // #if (TLK_STK_BT_ENABLE)
