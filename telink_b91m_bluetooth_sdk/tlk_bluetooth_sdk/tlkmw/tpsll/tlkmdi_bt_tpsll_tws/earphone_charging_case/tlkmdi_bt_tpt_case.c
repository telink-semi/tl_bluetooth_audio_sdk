/********************************************************************************************************
 * @file    tlkmdi_bt_tpt_case.c
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
#if TLK_DEV_SY5500_ENABLE && TLKSTK_BTTPSLL_TWS_ENABLE

#define TLKMDI_BT_TPT_TWS_DBG_FLAG 0xFFFFFFFF
#define TLKMDI_BT_TPT_TWS_DBG_SIGN "[TWS_BOX]"

/**
 * @brief       This function gets the current state of the device.
 * @param[in]   isEnterPair - whether to enter pairing mode.
 * @return      Current state value.
 * @note        
 *              BIT0 BT services connected
 *              BIT1 Dongle connected
 *              BIT2 TWS connected
 *              BIT3 inquiry scan enabled
 *              BIT4 Paging(BT)
 *              BIT7 Master or slave earphone
 */
static uint8_t tlkmdi_bt_tpt_getState(bool isEnterPair)
{
    // uint8_t state = 0;
    // state |= tlkmdi_bt_tpt_isMaster() ? (1 << 7) : 0x00;
    // if(isEnterPair){
    //     state |= 1 << 3;
    //     return state;
    // }
    // return state;
    (void)isEnterPair;
    return 0;
}

/**
 * @brief       This function handles entering or leaving the charging case.
 * @param[in]   isEnter - true if entering the case, false if leaving.
 * @return      none.
 */
static void tlkmdi_bt_tpt_enterLeaveCaseDeal(bool isEnter)
{
    //1:change local state and inform peer ear
    TlkMdiTptCaseState_t state = tlkmdi_bt_tpt_getLocalCaseState();
    state.isLeavebox           = isEnter ? TLKMDI_BT_TPT_STATE_FALSE : TLKMDI_BT_TPT_STATE_TRUE;
    tlkmdi_bt_tpt_setLocalCaseState(state);
    tlkmdi_bt_tpt_boxStateSync(tlkmdi_bt_tpt_getLocalCaseState());

    TlkMdiTptCaseState_t peerState = tlkmdi_bt_tpt_getPeerCaseState();
    //2:if master ear enter box,slave ear is stall in air,do hand over(role change)
    if (isEnter && tlkmdi_bt_tpt_isMaster() && peerState.isLeavebox == TLKMDI_BT_TPT_STATE_TRUE) {
        tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_TPT_MSGID_START_HANDOVER, NULL, 0);
    }

    //3:if slave ear leave box,master ear stall in box,request hand over(role change)
    else if (!isEnter && tlkmdi_bt_tpt_isSlave() && peerState.isLeavebox == TLKMDI_BT_TPT_STATE_FALSE) {
        tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_TPT_MSGID_START_HANDOVER, NULL, 0);
    }
}

/**
 * @brief       This function handles charging case events.
 * @param[in]   evtID     - event identifier.
 * @param[in]   pData     - pointer to event data.
 * @param[in]   dataLen   - length of event data.
 * @param[in]   isNeedAck - whether acknowledgment is needed.
 * @return      none.
 */
static void tlkmdi_bt_tpt_case_evtHandle(uint16_t evtID, uint8_t *pData, uint16_t dataLen, bool isNeedAck)
{
    (void)pData;
    (void)dataLen;
    bool isEnterPair = false;
    tlkapi_trace(TLKMDI_BT_TPT_TWS_DBG_FLAG, TLKMDI_BT_TPT_TWS_DBG_SIGN, "TWS BOX EVT:%d", evtID);
    switch (evtID) {
    case TLKDRV_SY5500_EVT_BOX_OPEN:
        break;
    case TLKDRV_SY5500_EVT_BOX_CLOSE:

        break;
    case TLKDRV_SY5500_EVT_BOX_PAIR:
        tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_TPT_MSGID_3S_PAIR, pData, dataLen);
        isEnterPair = true;
        break;
    case TLKDRV_SY5500_EVT_BOX_FACTORY_RESET:
        tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_TPT_MSGID_10S_PAIR, pData, 6);
        break;
    case TLKDRV_SY5500_EVT_LEAVE_BOX:
    {
        uint8_t inear = 1;
        tlkmdi_bt_tpt_enterLeaveCaseDeal(false);
        tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_INEAR_STATE_CHANGE, &inear, 1);
    } break;
    case TLKDRV_SY5500_EVT_ENTER_BOX:
    {
        uint8_t inear = 0;
        tlkmdi_bt_tpt_enterLeaveCaseDeal(true);
        tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_INEAR_STATE_CHANGE, &inear, 1);
    } break;
    case TLKDRV_SY5500_EVT_BOX_GET_MAC:
        uint8_t btAddr[6];
        tlkmdi_tinySql_getBtMacAddress(btAddr);
        tlkdrv_sy5500_sendMacToBox(btAddr);
        break;
    case TLKDRV_SY5500_EVT_NEED_POWER_OFF:
        tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_TPT_MSGID_SHUT_DOWN, NULL, 0);
        break;
    }
    if (isNeedAck) {
        uint8_t state = tlkmdi_bt_tpt_getState(isEnterPair);
        tlkdrv_sy5500_sendStateToBox(state);
    }
}

/**
 * @brief       This function initializes the charging case module.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_bt_tpt_case_init(void)
{
    tlkdrv_sy5500_init(tlkmdi_bt_tpt_case_evtHandle);
}

/**
 * @brief       This function checks if the device is left earphone.
 * @param[in]   none.
 * @return      true if it is left earphone, false otherwise.
 */
bool tlkmdi_bt_tpt_isLeft()
{
    uint16_t usbID = tlkmdi_tinySql_getUsbID();
    return usbID == 0X120; //temp code
}

/**
 * @brief       This function checks if the device is in the charging case.
 * @param[in]   none.
 * @return      true if in the charging case, false otherwise.
 */
bool tlkmdi_bt_tpt_isInBox()
{
    TlkMdiTptCaseState_t state = tlkmdi_bt_tpt_getLocalCaseState();
    return state.isLeavebox != TLKMDI_BT_TPT_STATE_TRUE;
}
#endif