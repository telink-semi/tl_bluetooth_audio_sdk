/********************************************************************************************************
 * @file    tlksys_pm.c
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
#include "stack/pm/pm_sys.h"

typedef struct
{
    uint32_t isPowerOffing     : 1;
    uint32_t isPowerOffForce   : 1;
    uint32_t isHaveSwitchClock : 1;
    uint32_t resv              : 29;
    uint32_t regMask;
    uint32_t powerMask;
    uint32_t suspendMask;
} tlksys_pm_ctrl_t;

struct tlksys_pm_clock_req_node_s
{
    uint16_t                           level;
    uint16_t                           reqId;
    struct tlksys_pm_clock_req_node_s *next;
};
typedef struct tlksys_pm_clock_req_node_s tlksys_pm_clock_req_node_t;


static tlksys_pm_ctrl_t            sTlksysPmCtrl         = {0};
static tlksys_pm_clock_req_node_t *sTlksysPmClockReqList = NULL;

/**
 * @brief  Request system to power off
 * @param[in] isforce : Whether to force power off
 * @returns  None.
 */
static void tlksys_pm_requestPowerOff(bool isforce)
{
    if (sTlksysPmCtrl.isPowerOffing) {
        return;
    }
    memset(&sTlksysPmCtrl, 0, sizeof(sTlksysPmCtrl));
    if (isforce) {
        sTlksysPmCtrl.isPowerOffForce = 1;
    } else {
        sTlksysPmCtrl.isPowerOffForce = 0;
    }
    sTlksysPmCtrl.isPowerOffing = 1;
    tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_PM);
}

/**
 * @brief  Switch system clock to specified level
 * @param[in] level : Target clock level
 * @returns  None.
 */
static void tlksys_pm_switchClock(uint8_t level)
{
    if (tlkhal_clock_getCurrentLevel(NULL) == level) {
        return;
    }
    tlkhal_clock_setLevel(level, NULL);
    sTlksysPmCtrl.isHaveSwitchClock = 1;
    tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_PM);
}

/**
 * @brief  Request higher CPU clock level
 * @param[in] level : Target clock level
 * @param[in] reqId : Request ID for this clock request
 * @returns  None.
 */
static void tlksys_pm_reqHighClockCore(uint8_t level, uint16_t reqId)
{
    tlksys_pm_clock_req_node_t *current = sTlksysPmClockReqList;

    while (current != NULL) {
        if (current->reqId == reqId) {
            return;
        }
        current = current->next;
    }

    tlksys_pm_clock_req_node_t *new_node = (tlksys_pm_clock_req_node_t *)tlkos_malloc(sizeof(tlksys_pm_clock_req_node_t));
    if (new_node == NULL) {
        return;
    }

    new_node->level = level;
    new_node->reqId = reqId;
    new_node->next  = NULL;

    if (sTlksysPmClockReqList == NULL) {
        sTlksysPmClockReqList = new_node;
    } else {
        current = sTlksysPmClockReqList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    if (tlkhal_clock_getCurrentLevel(NULL) >= level) {
        return;
    }
    tlksys_pm_switchClock(level);
}

/**
 * @brief  Cancel a previously registered high CPU clock request
 * @param[in] reqId : Request ID to cancel
 * @returns  None.
 */
static void tlksys_pm_cancelHighClockReqCore(uint16_t reqId)
{
    (void)reqId;
    tlksys_pm_clock_req_node_t *current = sTlksysPmClockReqList;
    tlksys_pm_clock_req_node_t *prev    = NULL;

    while (current != NULL) {
        if (current->reqId == reqId) {
            if (prev == NULL) {
                sTlksysPmClockReqList = current->next;
            } else {
                prev->next = current->next;
            }
            tlkos_free(current);

            if (sTlksysPmClockReqList == NULL) {
                tlksys_pm_switchClock(0);
            } else {
                tlksys_pm_clock_req_node_t *highest = sTlksysPmClockReqList;
                current                             = sTlksysPmClockReqList;
                while (current != NULL) {
                    if (current->level > highest->level) {
                        highest = current;
                    }
                    current = current->next;
                }
                tlksys_pm_switchClock(highest->level);
            }
            return;
        }
        prev    = current;
        current = current->next;
    }
}

/**
 * @brief  This function servers to register a bit to notify the power manager system 
 *         is need power off or suspend,Default bit level is 1(busy) after register.
 *         When all bits(registered) is clr to 0(unbusy), system will power off/suspend.
 *         When any bits(registered) is set to 1(busy), system can't power off/suspend.
 * @param[in] chnId : channel id : 0-31.
 * @returns  The result of register.
 */
int tlksys_pm_regChn(uint8_t chnId)
{
    if (sTlksysPmCtrl.isPowerOffing) {
        return -TLK_EFAIL;
    }
    if (chnId >= 32) {
        return -TLK_EPARAM;
    }
    tlksys_enter_critical();
    sTlksysPmCtrl.regMask |= (1 << chnId);
    tlksys_leave_critical();
    return tlksys_pm_setChn(chnId, 1, 1);
}

/**
 * @brief  This function servers to set busy flag to notify the power manager system 
 *         is need power off or suspend,Default bit level is 1(busy) after register.
 *         When all bits(registered) is clr to 0(unbusy), system will power off/suspend.
 *         When any bits(registered) is set to 1(busy), system can't power off/suspend.
 * @param[in] chnId : channel id : 0-31.
 * @param[in] isPowerOffBusy : 0 means not busy, 1 means busy, others means not change.
 * @param[in] isSuspendBusy  : 0 means not busy, 1 means busy, others means not change.
 * @returns  The result of set.
 */
int tlksys_pm_setChn(uint8_t chnID, uint8_t isPowerOffBusy, uint8_t isSuspendBusy)
{
    if (sTlksysPmCtrl.isPowerOffing) {
        return -TLK_EFAIL;
    }
    if (chnID >= 32 || ((sTlksysPmCtrl.regMask & (1 << chnID)) == 0)) {
        return -TLK_EPARAM;
    }
    tlksys_enter_critical();
    if (isPowerOffBusy == 1) {
        sTlksysPmCtrl.powerMask |= (1 << chnID);
    } else if (isPowerOffBusy == 0) {
        sTlksysPmCtrl.powerMask &= ~(1 << chnID);
    }
    if (isSuspendBusy == 1) {
        sTlksysPmCtrl.suspendMask |= (1 << chnID);
    } else if (isSuspendBusy == 0) {
        sTlksysPmCtrl.suspendMask &= ~(1 << chnID);
    }
    if (sTlksysPmCtrl.regMask != 0 && sTlksysPmCtrl.powerMask == 0) {
        tlksys_pm_requestPowerOff(false);
    }
    tlksys_leave_critical();
    return TLK_ENONE;
}

/**
 * @brief  This function servers to get is in progress of power off.
 * @param[out] isForce : Get the power off request is forced.
 * @returns  The result.
 */
bool tlksys_pm_isPowerOffInProgress(bool *isForce)
{
    if (isForce != NULL) {
        *isForce = (sTlksysPmCtrl.isPowerOffForce == 1);
    }
    return sTlksysPmCtrl.isPowerOffing == 1;
}

/**
 * @brief  This function servers to get is allowed to enter suspend.
 * @returns  The result of is allowed to enter suspend.
 */
bool tlksys_pm_isAllowedEnterSuspend(void)
{
#if (TLK_CFG_SUSPEND_ENABLE)
    if (tlksdk_pm_is_enabled()) {
        if (sTlksysPmCtrl.regMask == 0) {
            return true;
        }
        return sTlksysPmCtrl.suspendMask == 0;
    } else {
        return false;
    }
#else
    return false;
#endif
}

/**
 * @brief  This function servers to get whether system has switch clock.
 * @param[out] clr : clear the flag witch record system has switch clock.
 * @returns  The result.
 */
bool tlksys_pm_isHaveClockSwitch(bool clr)
{
    bool ret = sTlksysPmCtrl.isHaveSwitchClock == 1;
    if (clr) {
        tlksys_enter_critical();
        sTlksysPmCtrl.isHaveSwitchClock = 0;
        tlksys_leave_critical();
    }
    return ret;
}

/**
 * @brief  This function servers to power off.
 * @returns  None.
 */
void tlksys_pm_forcePowerOff(void)
{
    tlksys_enter_critical();
    tlksys_pm_requestPowerOff(true);
    tlksys_leave_critical();
}

/**
 * @brief  This function requests a higher CPU clock level.
 * @note   This function will register a request for a higher CPU clock level. 
 *         The CPU will run at the highest requested level among all active requests.
 *         When multiple requests are active, the CPU clock will be set to the highest level 
 *         requested by any of the requests.
 *         The request ID should be unique to allow proper cancellation of the request later.
 * @param[in] level : The desired CPU clock level.
 * @param[in] reqId : A unique identifier for this request.
 * @returns  None.
 */
void tlksys_pm_reqHighClock(uint8_t level, uint16_t reqId)
{
    (void)level;
    (void)reqId;
    (void)tlksys_pm_reqHighClockCore;
#if TLK_CFG_DYNAMIC_CLOCK_ENABLE
    tlksys_enter_critical();
    tlksys_pm_reqHighClockCore(level, reqId);
    tlksys_leave_critical();
#endif
}

/**
 * @brief  This function cancels a previously registered high CPU clock request.
 * @note   This function will cancel the high CPU clock request associated with the given request ID.
 *         If this was the last request at the highest level, the CPU clock may be reduced to a lower 
 *         level based on remaining active requests.
 * @param[in] reqId : The unique identifier of the request to cancel.
 * @returns  None.
 */
void tlksys_pm_cancelHighClockReq(uint16_t reqId)
{
    (void)reqId;
    (void)tlksys_pm_cancelHighClockReqCore;
#if TLK_CFG_DYNAMIC_CLOCK_ENABLE
    tlksys_enter_critical();
    tlksys_pm_cancelHighClockReqCore(reqId);
    tlksys_leave_critical();
#endif
}
