/********************************************************************************************************
 * @file    tlksys_pm.h
 *
 * @brief   This is the header file for TLSR/TL
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
#pragma once

/**
 * @note   All functions are not supported for calls within interrupts (IRQ/ISR).
 */

/**
 * @brief  This function servers to register a bit to notify the power manager system 
 *         is need power off or suspend,Default bit level is 1(busy) after register.
 *         When all bits(registered) is clr to 0(unbusy), system will power off/suspend.
 *         When any bits(registered) is set to 1(busy), system can't power off/suspend.
 * @param[in] chnId : channel id : 0-31.
 * @returns  The result of register.
 */
int tlksys_pm_regChn(uint8_t chnId);

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
int tlksys_pm_setChn(uint8_t chnID, uint8_t isPowerOffBusy, uint8_t isSuspendBusy);

/**
 * @brief  This function servers to get is in progress of power off.
 * @param[out] isForce : Get the power off request is forced.
 * @returns  The result.
 */
bool tlksys_pm_isPowerOffInProgress(bool *isForce);

/**
 * @brief  This function servers to get is allowed to enter suspend.
 * @returns  The result.
 */
bool tlksys_pm_isAllowedEnterSuspend(void);

/**
 * @brief  This function servers to get whether system has switch clock.
 * @param[out] clr : clear the flag witch record system has switch clock.
 * @returns  The result.
 */
bool tlksys_pm_isHaveClockSwitch(bool clr);

/**
 * @brief  This function servers to power off.
 * @returns  None.
 */
void tlksys_pm_forcePowerOff(void);

/**
 * @brief  This function requests a higher CPU clock level.
 * @note   This function will register a request for a higher CPU clock level. 
 *         The CPU will run at the highest requested level among all active requests.
 *         When multiple requests are active, the CPU clock will be set to the highest level 
 *         requested by any of the requests.
 *         The request ID should be unique to allow proper cancellation of the request later.
 * @param[in] level : The desired CPU clock level.
 * @param[in] reqId : A unique identifier for this request.
 */

void tlksys_pm_reqHighClock(uint8_t level, uint16_t reqId);

/**
 * @brief  This function cancels a previously registered high CPU clock request.
 * @note   This function will cancel the high CPU clock request associated with the given request ID.
 *         If this was the last request at the highest level, the CPU clock may be reduced to a lower 
 *         level based on remaining active requests.
 * @param[in] reqId : The unique identifier of the request to cancel.
 */
void tlksys_pm_cancelHighClockReq(uint16_t reqId);