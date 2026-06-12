/********************************************************************************************************
 * @file    tlkmdi_btinq.c
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
#if (TLK_MW_BTINQ_ENABLE)
#include "tlkmw/bt/tlkmw_bt.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/hci/hci_cmd.h"

#define TLKMDI_BTINQ_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_INQ << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTINQ_DBG_SIGN "[MDI]"

static void tlkmdi_btinq_reportDevice(tlkmdi_btinq_item_t *pItem);


static TlkMdiBtInqReportCallBack   sTlkmdiBtInqReportCB;
static TlkMdiBtInqCompleteCallBack sTlkmdiBtInqCompleteCB;

static tlkmdi_btinq_ctrl_t stlk_inq_ctrl;

/**
 * @brief  Get whether the Inquiry process is busy or not.
 * @param  None.
 * @return True if the Inquiry process is busy, otherwise False.
*/
bool tlkmdi_btinq_isBusy(void)
{
    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_IDLE) {
        return false;
    } else {
        return true;
    }
}

/**
 * @brief  Start the Inquiry process.
 * @param  inqType: Inquiry device type.
 * @param  rssiThd: RSSI threshold.
 * @param  maxNumb: Maximum number of devices to be searched.
 * @param  inqWind: Inquiry window.
 * @return TLK_ENONE if successful, otherwise error code.
*/
int tlkmdi_btinq_start(uint8_t inqType, uint8_t rssiThd, uint8_t maxNumb, uint8_t inqWind)
{
    if (stlk_inq_ctrl.state != TLKMDI_BTINQ_STATE_IDLE) {
        return -TLK_EBUSY;
    }

    if (inqWind < 3) {
        inqWind = 3;
    } else if (inqWind > 60) {
        inqWind = 60;
    }

    if (inqWind > 100) {
        inqWind = 100;
    }
    if (maxNumb > TLKMDI_BTINQ_ITEM_NUMB) {
        maxNumb = TLKMDI_BTINQ_ITEM_NUMB;
    }

    if (maxNumb == 0) {
        maxNumb = TLKMDI_BTINQ_ITEM_NUMB;
    }

    stlk_inq_ctrl.inqType   = inqType;
    stlk_inq_ctrl.curNumb   = 0;
    stlk_inq_ctrl.nameIdx   = 0;
    stlk_inq_ctrl.inqWind   = ((uint32_t)inqWind * 1000) / TLKMDI_BTINQ_TIMEOUT_MS;
    stlk_inq_ctrl.maxNumb   = maxNumb;
    stlk_inq_ctrl.rssiThd   = rssiThd;
    stlk_inq_ctrl.retry_num = TLKMDI_BTINQ_MAX_RETRY_NUMB;

    stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_INQUIRY;
    stlk_inq_ctrl.stage = TLKMDI_BTINQ_INQUIRY_STAGE_START;

    tlkapi_trace(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, "tlkmdi_btinq_start type[%d], maxNumb[%d], inqWind[%d]...", stlk_inq_ctrl.inqType, stlk_inq_ctrl.maxNumb,
                 stlk_inq_ctrl.inqWind);

    tlksys_timer_reStart(TLKSYS_TASKID_HOST, &stlk_inq_ctrl.timer);

    return TLK_ENONE;
}

/**
 * @brief  Stop the Inquiry process.
 * @param  None.
 * @return None.
*/
void tlkmdi_btinq_close(void)
{
    uint8_t stage;

    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_IDLE || stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_CLOSING) {
        return;
    }

    stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_INQUIRY) {
        if (stlk_inq_ctrl.stage != TLKMDI_BTINQ_INQUIRY_STAGE_WAIT_CLOSE_RSP) {
            stage = TLKMDI_BTINQ_CLOSING_STAGE_CANCEL_INQUIRY;
        }
    } else if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_GETNAME) {
        stage = TLKMDI_BTINQ_CLOSING_STAGE_CANCEL_GETNAME;
    }

    stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_CLOSING;
    stlk_inq_ctrl.stage = stage;

    stlk_inq_ctrl.timeout = TLKMDI_BTINQ_WAIT_TIMEOUT;
}

/**
 * @brief  Reset the Inquiry parameters.
 * @param  None.
 * @return None.
*/
void tlkmdi_btinq_reset(void)
{
    tlksys_timer_stop(TLKSYS_TASKID_HOST, &stlk_inq_ctrl.timer);
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkmdi_btinq_ctrl_t)));
    memset(&stlk_inq_ctrl, 0, sizeof(tlkmdi_btinq_ctrl_t));
}

/**
 * @brief  Register the callback function for reporting the BT devices.
 * @param  reportCB[IN]--The callback function for reporting the BT devices.
 * @param  completeCB[IN]--The callback function for complete the BT devices inquiry.
 * @return None.
*/
void tlkmdi_btinq_regCallback(TlkMdiBtInqReportCallBack reportCB, TlkMdiBtInqCompleteCallBack completeCB)
{
    sTlkmdiBtInqReportCB   = reportCB;
    sTlkmdiBtInqCompleteCB = completeCB;
}

/**
 * @brief  Clean the Inquiry items.
 * @param  None.
 * @return None.
*/
void tlkmdi_btinq_cleanItems(void)
{
    stlk_inq_ctrl.curNumb = 0;
}

/**
 * @brief  Get the number of devices that have obtained the name.
 * @param  None.
 * @return The number of items.
*/
int tlkmdi_btinq_getReadyItemCount(void)
{
    uint8_t index;
    uint8_t count = 0;
    for (index = 0; index < stlk_inq_ctrl.curNumb; index++) {
        if (stlk_inq_ctrl.item[index].nameLen != 0) {
            count++;
        }
    }
    return count;
}

/**
 * @brief  Get the item by index.
 * @param  index[IN]--The index of item.
 * @return The block of item.
*/
tlkmdi_btinq_item_t *tlkmdi_btinq_getItem(uint8_t index)
{
    if (index >= TLKMDI_BTINQ_ITEM_NUMB) {
        return NULL;
    }
    return &stlk_inq_ctrl.item[index];
}

/**
 * @brief  Get the idle item.
 * @param  None.
 * @return The idle item.
*/
tlkmdi_btinq_item_t *tlkmdi_btinq_getIdleItem(void)
{
    tlkmdi_btinq_item_t *pItem;

    if (stlk_inq_ctrl.curNumb >= stlk_inq_ctrl.maxNumb) {
        return NULL;
    }

    pItem = &stlk_inq_ctrl.item[stlk_inq_ctrl.curNumb];
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkmdi_btinq_item_t)));
    memset(pItem, 0, sizeof(tlkmdi_btinq_item_t));

    stlk_inq_ctrl.curNumb++;

    return pItem;
}

/**
 * @brief  Get the item by address.
 * @param  pAddr[IN]--The address of item.
 * @return The item.
*/
tlkmdi_btinq_item_t *tlkmdi_btinq_getUsedItem(uint8_t *pAddr)
{
    uint8_t index;
    for (index = 0; index < stlk_inq_ctrl.curNumb; index++) {
        if (tmemcmp(pAddr, stlk_inq_ctrl.item[index].btaddr, 6) == 0) {
            break;
        }
    }
    if (index == stlk_inq_ctrl.curNumb) {
        return NULL;
    }
    return &stlk_inq_ctrl.item[index];
}

/**
 * @brief  Print the list of searched BT device.
 * @param  None.
 * @return None.
*/
void tlkmdi_btinq_printList(void)
{
#if (TLK_DEBUG_ENABLE)
    int     i, k;
    uint8_t buffLen;
    char    buffer[128];

    tlkapi_trace(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, "<---------------Device list------------->");
    for (i = 0; i < stlk_inq_ctrl.curNumb; i++) {
        tlkmdi_btinq_item_t *pItem = &stlk_inq_ctrl.item[i];
        buffLen = tlkapi_sprintf(buffer, "[INQ]->DEVICE %02x %02x %02x %02x %02x %02x, RSSI:%d, NAME:", stlk_inq_ctrl.item[i].btaddr[0], stlk_inq_ctrl.item[i].btaddr[1],
                                 stlk_inq_ctrl.item[i].btaddr[2], stlk_inq_ctrl.item[i].btaddr[3], stlk_inq_ctrl.item[i].btaddr[4], stlk_inq_ctrl.item[i].btaddr[5],
                                 (int)stlk_inq_ctrl.item[i].rssi);
        for (k = 0; k < pItem->nameLen && buffLen < 120; k++) {
            buffLen += tlkapi_sprintf(buffer + buffLen, "%c", stlk_inq_ctrl.item[i].btname[k]);
        }
        buffer[buffLen] = 0;
        tlkapi_trace(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, buffer, 0, 0);
    }
    tlkapi_trace(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, "<---------------List end---------------->");
#endif
}

/**
 * @brief  The inquiry process.
 * @param  None.
 * @return None.
*/
static void tlkmdi_btinq_inquiryProcs(void)
{
    if (stlk_inq_ctrl.state != TLKMDI_BTINQ_STATE_INQUIRY && stlk_inq_ctrl.stage > TLKMDI_BTINQ_INQUIRY_STAGE_WAIT_CLOSE_RSP) {
        return;
    }

    if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_INQUIRY_STAGE_START) {
        if (stlk_inq_ctrl.retry_num == 0) {
            stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_CLOSING;
            stlk_inq_ctrl.stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
            tlk_printf("Inquiry retry numb is [%d], switch to Closing process.", TLKMDI_BTINQ_MAX_RETRY_NUMB);
            return;
        }

        if (bth_hci_sendWriteInquiryModeCmd(0x02) == TLK_ENONE && bth_hci_sendInquiryCmd(INQUIRY_LENGTH_30S72, 0xFF) == TLK_ENONE) {
            stlk_inq_ctrl.stage   = TLKMDI_BTINQ_INQUIRY_STAGE_WAIT_START_RSP;
            stlk_inq_ctrl.timeout = TLKMDI_BTINQ_WAIT_TIMEOUT;
            tlk_printf("tlkmdi_btinq_inquiryProcs stage start, switch to wait start response.");
        } else {
            tlk_printf("Start inquiry fail due to hci send fail, switch to IDLE.");
            stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_IDLE;
        }
    } else if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_INQUIRY_STAGE_WAIT_START_RSP) {
        if (stlk_inq_ctrl.timeout != 0) {
            stlk_inq_ctrl.timeout--;
        }
        if (stlk_inq_ctrl.timeout != 0) {
            return;
        }
        tlk_printf("Wait start response timeout, switch to Closing process.");
        stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_CLOSING;
        stlk_inq_ctrl.stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
    } else if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_INQUIRY_STAGE_DOING) {
        if (stlk_inq_ctrl.timeout != 0) {
            stlk_inq_ctrl.timeout--;
        }
        if (stlk_inq_ctrl.timeout != 0) {
            return;
        }
        tlk_printf("Inquiry timeout, retry_num[%d], curNumb[%d], maxNum[%d]", stlk_inq_ctrl.retry_num, stlk_inq_ctrl.curNumb, stlk_inq_ctrl.maxNumb);
        if (stlk_inq_ctrl.curNumb > 0 && tlkmdi_btinq_getReadyItemCount() != stlk_inq_ctrl.curNumb) {
            tlk_printf("This inquiry is over, switch to GetName Process.");
            stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_GETNAME;
            stlk_inq_ctrl.stage = TLKMDI_BTINQ_GETNAME_STAGE_START;
        } else {
            stlk_inq_ctrl.stage = TLKMDI_BTINQ_INQUIRY_STAGE_CLOSE;
        }
    } else if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_INQUIRY_STAGE_CLOSE) {
        if (bth_hci_sendInquiryCancelCmd() == TLK_ENONE) {
            stlk_inq_ctrl.stage   = TLKMDI_BTINQ_INQUIRY_STAGE_WAIT_CLOSE_RSP;
            stlk_inq_ctrl.timeout = TLKMDI_BTINQ_WAIT_TIMEOUT;
        }
    } else if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_INQUIRY_STAGE_WAIT_CLOSE_RSP) {
        if (stlk_inq_ctrl.timeout != 0) {
            stlk_inq_ctrl.timeout--;
        }
        if (stlk_inq_ctrl.timeout != 0) {
            return;
        }
        stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_CLOSING;
        stlk_inq_ctrl.stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
        tlk_printf("Wait close inquiry response timeout, switch to Closing process.");
    }
}

/**
 * @brief  The get device name process.
 * @param  None.
 * @return None.
*/
static void tlkmdi_btinq_getNameProcs(void)
{
    if (stlk_inq_ctrl.state != TLKMDI_BTINQ_STATE_GETNAME && stlk_inq_ctrl.stage > TLKMDI_BTINQ_GETNAME_STAGE_WAIT_GET_RSP) {
        return;
    }

    if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_GETNAME_STAGE_START) {
        if (stlk_inq_ctrl.curNumb == 0 || tlkmdi_btinq_getReadyItemCount() == stlk_inq_ctrl.curNumb) { //No device need to get name.
            if (stlk_inq_ctrl.retry_num && (stlk_inq_ctrl.curNumb < stlk_inq_ctrl.maxNumb)) {
                /*Start next inquiry.*/
                stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_INQUIRY;
                stlk_inq_ctrl.stage = TLKMDI_BTINQ_INQUIRY_STAGE_START;
            } else {
                stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_CLOSING;
                stlk_inq_ctrl.stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
                tlkapi_trace(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, "tlkmdi_btinq_getNameProcs: Switch to Closing");
            }
        } else {
            stlk_inq_ctrl.nameIdx = stlk_inq_ctrl.curNumb - 1;
            stlk_inq_ctrl.stage   = TLKMDI_BTINQ_GETNAME_STAGE_SEND_GET;
        }
    } else if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_GETNAME_STAGE_SEND_GET) {
        int                  index;
        tlkmdi_btinq_item_t *pItem = NULL;
        for (index = stlk_inq_ctrl.nameIdx; index >= 0; index--) {
            if (stlk_inq_ctrl.item[index].nameLen == 0) {
                break;
            }
        }
        if (index < stlk_inq_ctrl.curNumb) {
            pItem = &stlk_inq_ctrl.item[index];
        }

        if (pItem == NULL) { //No device need to get name.
            if (stlk_inq_ctrl.retry_num && (stlk_inq_ctrl.curNumb < stlk_inq_ctrl.maxNumb)) {
                /*Start next inquiry.*/
                stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_INQUIRY;
                stlk_inq_ctrl.stage = TLKMDI_BTINQ_INQUIRY_STAGE_START;
            } else {
                stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_CLOSING;
                stlk_inq_ctrl.stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
                tlkapi_trace(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, "tlkmdi_btinq_getNameProcs: Switch to Closing");
            }
        } else if (bth_hci_sendRemoteNameReqCmd(pItem->btaddr, pItem->smode, pItem->clkOff) == TLK_ENONE) {
            stlk_inq_ctrl.stage   = TLKMDI_BTINQ_GETNAME_STAGE_WAIT_GET_RSP;
            stlk_inq_ctrl.timeout = TLKMDI_BTINQ_WAIT_TIMEOUT;
            stlk_inq_ctrl.nameIdx = index;
        }
    } else if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_GETNAME_STAGE_WAIT_GET_RSP) {
        if (stlk_inq_ctrl.timeout != 0) {
            stlk_inq_ctrl.timeout--;
        }
        if (stlk_inq_ctrl.timeout != 0) {
            return;
        }

        stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_CLOSING;
        stlk_inq_ctrl.stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
        tlk_printf("Wait get name response timeout, switch to closing process.");
    }
}

/**
 * @brief  The closing process.
 * @param  None.
 * @return None.
*/
static void tlkmdi_btinq_closingProcs(void)
{
    if (stlk_inq_ctrl.state != TLKMDI_BTINQ_STATE_CLOSING && stlk_inq_ctrl.stage > TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER) {
        return;
    }

    if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_CLOSING_STAGE_CANCEL_GETNAME) {
        tlkmdi_btinq_item_t *pItem = tlkmdi_btinq_getItem(stlk_inq_ctrl.nameIdx);
        if (pItem == NULL) {
            stlk_inq_ctrl.stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
        } else if (bth_hci_sendRemoteNameReqCancelCmd(pItem->btaddr) == TLK_ENONE) {
            stlk_inq_ctrl.stage   = TLKMDI_BTINQ_CLOSING_STAGE_WAIT_CANCEL;
            stlk_inq_ctrl.timeout = TLKMDI_BTINQ_WAIT_TIMEOUT;
        }
    } else if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_CLOSING_STAGE_CANCEL_INQUIRY) {
        if (bth_hci_sendInquiryCancelCmd() == TLK_ENONE) {
            stlk_inq_ctrl.stage   = TLKMDI_BTINQ_CLOSING_STAGE_WAIT_CANCEL;
            stlk_inq_ctrl.timeout = TLKMDI_BTINQ_WAIT_TIMEOUT;
        }
    } else if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_CLOSING_STAGE_WAIT_CANCEL) {
        if (stlk_inq_ctrl.timeout != 0) {
            stlk_inq_ctrl.timeout--;
        }
        if (stlk_inq_ctrl.timeout != 0) {
            return;
        }
        stlk_inq_ctrl.stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
    } else if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER) {
        tlkapi_trace(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, "tlkmdi_btinq_closingProcs: Inquiry Over!");
        tlkmdi_btinq_printList();
        if (sTlkmdiBtInqCompleteCB != NULL) {
            sTlkmdiBtInqCompleteCB();
        }
        stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_IDLE;
        stlk_inq_ctrl.stage = TLKMDI_BTINQ_STAGE_NONE;
    }
}

/**
 * @brief  The timer process.
 * @param  pTimer: The timer handle.
 * @param  userArg: The user argument.
 * @return None.
*/
static void tlkmdi_btinq_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_IDLE) {
        return;
    }

    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_INQUIRY) {
        tlkmdi_btinq_inquiryProcs();
    } else if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_GETNAME) {
        tlkmdi_btinq_getNameProcs();
    } else if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_CLOSING) {
        tlkmdi_btinq_closingProcs();
    }

    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_IDLE) {
        tlkapi_trace(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, "tlkmdi_btinq_timer: over [timer arrive]");
        return;
    }
    tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
}

/**
 * @brief  The inquiry result event handler.
 * @param  pData: The event data.
 * @param  dataLen: The event data length.
 * @return TLK_ENONE: Success, others: Failed.
*/
static int tlkmdi_btinq_resultEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t                 dtype;
    tlkmdi_btinq_item_t    *pItem;
    bth_inquiryResultEvt_t *pEvt;

    if (stlk_inq_ctrl.state != TLKMDI_BTINQ_STATE_INQUIRY || stlk_inq_ctrl.stage != TLKMDI_BTINQ_INQUIRY_STAGE_DOING) {
        tlkapi_error(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, "tlkmdi_btinq_resultEvt: State Error");
        return TLK_ENONE;
    }

    pEvt  = (bth_inquiryResultEvt_t *)pData;
    dtype = bth_devClassToDevType(pEvt->dtype);
    if (pEvt->dtype == 0 || dtype != stlk_inq_ctrl.inqType) {
        return TLK_ENONE;
    }

    if ((int8_t)stlk_inq_ctrl.rssiThd > (int8_t)pEvt->rssi) {
        return TLK_ENONE;
    }

    pItem = tlkmdi_btinq_getUsedItem(pEvt->mac);

    if (pItem == NULL) {
        pItem = tlkmdi_btinq_getIdleItem();
    } else {
        if (pItem->rssi != pEvt->rssi) {
            pItem->rssi = pEvt->rssi;
        }
        if (pItem->nameLen == 0 && pEvt->nameLen != 0) {
            if (pEvt->nameLen <= TLKMDI_BTINQ_NAME_LENS) {
                pItem->nameLen = pEvt->nameLen;
            } else {
                pItem->nameLen = TLKMDI_BTINQ_NAME_LENS; // Name Length
            }

            if (pItem->nameLen != 0) {
                tmemcpy(pItem->btname, pEvt->pName, pItem->nameLen);
            }
            pItem->btname[pItem->nameLen] = 0;
        }
        return TLK_ENONE;
    }

    if (pItem == NULL) {
        tlkapi_trace(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, "Inquiry Device Is Full!");
        return TLK_ENONE;
    }

    pItem->rssi     = pEvt->rssi;
    pItem->smode    = pEvt->smode; // Scan Mode
    pItem->dtype    = dtype;
    pItem->clkOff   = pEvt->clock;
    pItem->devClass = pEvt->dtype;

    if (pEvt->nameLen <= TLKMDI_BTINQ_NAME_LENS) {
        pItem->nameLen = pEvt->nameLen;
    } else {
        pItem->nameLen = TLKMDI_BTINQ_NAME_LENS; // Name Length
    }
    tmemcpy(pItem->btaddr, pEvt->mac, 6);
    if (pItem->nameLen != 0) {
        tmemcpy(pItem->btname, pEvt->pName, pItem->nameLen);
    }
    pItem->btname[pItem->nameLen] = 0;

    uint8_t buffer[128];
    uint8_t buffLen = tlkapi_sprintf((char *)buffer, "[INQ]->DEVICE %02x %02x %02x %02x %02x %02x, RSSI:%d, NAME:", pItem->btaddr[0], pItem->btaddr[1], pItem->btaddr[2],
                                     pItem->btaddr[3], pItem->btaddr[4], pItem->btaddr[5], (int)pItem->rssi);

    buffLen += tlkapi_sprintf((char *)buffer + buffLen, "%s", pItem->btname);
    buffer[buffLen] = 0;
    tlkapi_trace(TLKMDI_BTINQ_DBG_FLAG, TLKMDI_BTINQ_DBG_SIGN, (char *)buffer, 0, 0);

    if (pItem->nameLen > 0) {
        tlkmdi_btinq_reportDevice(pItem);
    }
    if (stlk_inq_ctrl.curNumb >= stlk_inq_ctrl.maxNumb) {
        stlk_inq_ctrl.stage = TLKMDI_BTINQ_INQUIRY_STAGE_CLOSE;
    }

    return TLK_ENONE;
}

/**
 * @brief  The inquiry complete event handler.
 * @param  pData: The event data.
 * @param  dataLen: The event data length.
 * @return TLK_ENONE: Success, others: Failed.
*/
static int tlkmdi_btinq_completeEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    tlk_printf("tlkmdi_btinq_completeEvt state[%d], stage[%d], retry[%d], cur[%d], max[%d]", stlk_inq_ctrl.state, stlk_inq_ctrl.stage, stlk_inq_ctrl.retry_num,
               stlk_inq_ctrl.curNumb, stlk_inq_ctrl.maxNumb);
    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_IDLE) {
        return -TLK_ESTATUS;
    }

    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_INQUIRY) {
        if (stlk_inq_ctrl.curNumb > 0 && tlkmdi_btinq_getReadyItemCount() < stlk_inq_ctrl.curNumb) {
            stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_GETNAME;
            stlk_inq_ctrl.stage = TLKMDI_BTINQ_GETNAME_STAGE_START;
        } else if (stlk_inq_ctrl.retry_num > 0) {
            stlk_inq_ctrl.stage = TLKMDI_BTINQ_INQUIRY_STAGE_START;
        }
    } else if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_GETNAME) {
    }

    return TLK_ENONE;
}

/**
 * @brief  The remote name request complete event handler.
 * @param  pData: The event data.
 * @param  dataLen: The event data length.
 * @return TLK_ENONE: Success, others: Failed.
*/
static int tlkmdi_btinq_getNameCompleteEvt(uint8_t *pData, uint16_t dataLen)
{
    bth_getNameCompleteEvt_t *pEvt = NULL;
    if (pData != NULL && dataLen >= sizeof(bth_getNameCompleteEvt_t)) {
        pEvt = (bth_getNameCompleteEvt_t *)pData;
    }

    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_GETNAME) {
        if (stlk_inq_ctrl.stage == TLKMDI_BTINQ_GETNAME_STAGE_WAIT_GET_RSP) {
            stlk_inq_ctrl.timeout = TLKMDI_BTINQ_WAIT_TIMEOUT;
            if (pEvt != NULL && pEvt->status == 0x00) {
                tlkmdi_btinq_item_t *pItem = tlkmdi_btinq_getUsedItem(pEvt->btaddr);
                if (pItem != NULL && pItem->nameLen == 0) {
                    pItem->nameLen = pEvt->nameLen;
                    if (pItem->nameLen > TLKMDI_BTINQ_NAME_LENS) {
                        pItem->nameLen = TLKMDI_BTINQ_NAME_LENS;
                    }
                    if (pItem->nameLen != 0) {
                        tmemcpy(pItem->btname, pEvt->pName, pItem->nameLen);
                    }
                    pItem->btname[pItem->nameLen] = 0;
                    tlkmdi_btinq_reportDevice(pItem);
                    stlk_inq_ctrl.stage = TLKMDI_BTINQ_GETNAME_STAGE_START;
                }
                if (pItem != NULL && stlk_inq_ctrl.nameIdx != 0) {
                    stlk_inq_ctrl.nameIdx--;
                }
            }
        }
    }
    return TLK_ENONE;
}

static int tlkmdi_btinq_inquiry_cancel_complete_Evt(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    /*Cancel Inquiry complete*/
    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_INQUIRY && stlk_inq_ctrl.stage == TLKMDI_BTINQ_INQUIRY_STAGE_WAIT_CLOSE_RSP) {
        stlk_inq_ctrl.state = TLKMDI_BTINQ_STATE_GETNAME;
        stlk_inq_ctrl.stage = TLKMDI_BTINQ_GETNAME_STAGE_START;
    } else if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_CLOSING) {
        stlk_inq_ctrl.stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
    }
    return TLK_ENONE;
}

static int tlkmdi_btinq_getName_cancel_complete_Evt(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    /*Cancel Inquiry complete*/
    if (stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_CLOSING) {
        stlk_inq_ctrl.stage = TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER;
    }
    return TLK_ENONE;
}

static int tlkmdi_btinq_inquiry_statusEvt(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < 1) {
        return -TLK_EPARAM;
    }
    uint8_t status = *pData;
    if (status == BTH_HCI_ERROR_NONE && stlk_inq_ctrl.state == TLKMDI_BTINQ_STATE_INQUIRY && stlk_inq_ctrl.stage == TLKMDI_BTINQ_INQUIRY_STAGE_WAIT_START_RSP) {
        stlk_inq_ctrl.stage   = TLKMDI_BTINQ_INQUIRY_STAGE_DOING;
        stlk_inq_ctrl.timeout = 300; //TODO
        stlk_inq_ctrl.retry_num--;
        tlk_printf("Received inquiry start response, switch to inquiry doing.");
    } else {
        tlk_printf("tlkmdi_btinq_inquiry_statusEvt error stage[%d]", stlk_inq_ctrl.stage);
    }
    return TLK_ENONE;
}

/**
 * @brief  Report the device information to the upper layer.
 * @param  pItem: The device information.
 * @return None.
*/
static void tlkmdi_btinq_reportDevice(tlkmdi_btinq_item_t *pItem)
{
    if (sTlkmdiBtInqReportCB == NULL) {
        pItem->state = TLKMDI_BTINQ_ITEM_STATE_OVER;
    } else {
        if (sTlkmdiBtInqReportCB(pItem->devClass, pItem->rssi, pItem->nameLen, pItem->btaddr, pItem->btname) == TLK_ENONE) {
            pItem->state = TLKMDI_BTINQ_ITEM_STATE_OVER;
        } else {
            pItem->state = TLKMDI_BTINQ_ITEM_STATE_WAIT;
        }
    }
}

BTH_EVT_REGISTER(BTH_EVTID_INQUIRY_RESULT, tlkmdi_btinq_resultEvt);
BTH_EVT_REGISTER(BTH_EVTID_INQUIRY_COMPLETE, tlkmdi_btinq_completeEvt);
BTH_EVT_REGISTER(BTH_EVTID_GETNAME_COMPLETE, tlkmdi_btinq_getNameCompleteEvt);
BTH_EVT_REGISTER(BTH_EVTID_START_INQUIRY_STATUS, tlkmdi_btinq_inquiry_statusEvt);
BTH_EVT_REGISTER(BTH_EVTID_CANCEL_INQUIRY_COMPLETE, tlkmdi_btinq_inquiry_cancel_complete_Evt);
BTH_EVT_REGISTER(BTH_EVTID_CANCEL_GET_NAME_COMPLETE, tlkmdi_btinq_getName_cancel_complete_Evt);

/**
 * @brief  The inquiry param initialize function.
 * @param  None.
 * @return TLK_ENONE: Success, others: Failed.
*/
int tlkmdi_btinq_init(void)
{
    STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(sizeof(tlkmdi_btinq_ctrl_t)));
    memset(&stlk_inq_ctrl, 0, sizeof(tlkmdi_btinq_ctrl_t));

    sTlkmdiBtInqReportCB   = NULL;
    sTlkmdiBtInqCompleteCB = NULL;

    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &stlk_inq_ctrl.timer, TLKMDI_BTINQ_TIMEOUT, false, tlkmdi_btinq_timer, NULL); //100ms

    return TLK_ENONE;
}


#endif // #if (TLK_MW_BTINQ_ENABLE)
