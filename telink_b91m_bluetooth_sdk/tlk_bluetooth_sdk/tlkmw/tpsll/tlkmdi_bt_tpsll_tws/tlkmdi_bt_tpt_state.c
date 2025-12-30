/********************************************************************************************************
 * @file    tlkmdi_bt_tpt_state.c
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
#include "tlkmw/tlkmw.h"
#include "tlkapi/tlkapi.h"

static TlkMdiTptCaseState_t sTlkMdiTptLocalCaseState = {0};
static TlkMdiTptCaseState_t sTlkMdiTptPeerCaseState = {0};

/**
 * @brief       Check if the current device is acting as slave role
 * @return      true if device is in slave or observer role, false otherwise
 * @note        This function checks if the current TWS role is either slave or observer,
 *              which are considered as slave roles in the TWS (True Wireless Stereo) system
 */
__attribute__((weak))  bool tlkmdi_bt_tpt_isSlave()
{
    return false;
}

/**
 * @brief       Check if the current device is acting as Master or Arbiter in TWS connection
 * @return      true - Device is currently Master or Arbiter
 *              false - Device is not Master or Arbiter
 * @note        In TWS (True Wireless Stereo) systems, both Master and Arbiter roles
 *              are considered as master-like roles in terms of control and coordination
 */
__attribute__((weak))  bool tlkmdi_bt_tpt_isMaster()
{
    return false;
}

/**
 * @brief       Check if the current TWS role is single mode
 * @return      true if current role is single mode, false otherwise
 * @note        This function checks whether the TWS is operating in 
 *              single mode, which means only one device is active without a paired TWS partner.
 */
__attribute__((weak))  bool tlkmdi_bt_tpt_isSingle()
{
    return false;
}

/**
 * @brief       This function checks if the TWS connection is established.
 * @return      bool - TRUE if connected, FALSE if not connected.
 * @note        Connection is determined by checking that the current role is neither IDLE nor SINGLE.
 */
__attribute__((weak))  bool tlkmdi_bt_tpt_isConnected()
{
    return false;
}

/**
 * @brief       This function checks if the device is left earphone.
 * @param[in]   None.
 * @return      true if it is left earphone, false otherwise.
 */
__attribute__((weak))  bool tlkmdi_bt_tpt_isLeft()
{
    return false;
}

/**
 * @brief       This function checks if the device is in the charging case.
 * @param[in]   None.
 * @return      true if in the charging case, false otherwise.
 */
__attribute__((weak))  bool tlkmdi_bt_tpt_isInBox()
{
    return false;
}

/**
 * @brief       This function checks if the device is in ear.
 * @param[in]   None.
 * @return      true if the device is in ear, false otherwise.
 * @note        Current implementation assumes that if the device is not in the 
 *              charging case, then it is in ear. But this may not be accurate 
 *              as the device could be out of the case but not yet in ear (e.g. in air).
 */
__attribute__((weak))  bool tlkmdi_bt_tpt_isInEar()
{
    return !tlkmdi_bt_tpt_isInBox();

    //TODO: inEar Check,out of box may in air not in ear.
}

/**
 * @brief       This function gets the local device's case state.
 * @param[in]   None.
 * @return      The current case state of the local device.
 */
TlkMdiTptCaseState_t tlkmdi_bt_tpt_getLocalCaseState()
{
    return sTlkMdiTptLocalCaseState;
}

/**
 * @brief       This function gets the peer device's case state.
 * @param[in]   None.
 * @return      The current case state of the peer device.
 */
TlkMdiTptCaseState_t tlkmdi_bt_tpt_getPeerCaseState()
{
    return sTlkMdiTptPeerCaseState;
}

/**
 * @brief       This function sets the peer device's case state.
 * @param[in]   state - The case state to set for the peer device.
 * @return      None.
 */
void tlkmdi_bt_tpt_setPeerCaseState(TlkMdiTptCaseState_t state)
{
    sTlkMdiTptPeerCaseState = state;
}

/**
 * @brief       Set the local device case state
 * @param[in]   state - The case state to be set for local device
 * @return      None
 */
void tlkmdi_bt_tpt_setLocalCaseState(TlkMdiTptCaseState_t state)
{
    sTlkMdiTptLocalCaseState = state;
}