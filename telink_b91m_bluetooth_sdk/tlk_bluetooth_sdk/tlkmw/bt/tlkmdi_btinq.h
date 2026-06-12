/********************************************************************************************************
 * @file    tlkmdi_btinq.h
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
#ifndef TLKMDI_BTINQ_H
#define TLKMDI_BTINQ_H


#define TLKMDI_BTINQ_TIMEOUT        100000 // us
#define TLKMDI_BTINQ_TIMEOUT_MS     (TLKMDI_BTINQ_TIMEOUT / 1000)

#define TLKMDI_BTINQ_MAX_RETRY_NUMB 3
#define TLKMDI_BTINQ_ITEM_NUMB      10
#define TLKMDI_BTINQ_NAME_LENS      19

#define TLKMDI_BTINQ_WAIT_TIMEOUT   (5000000 / TLKMDI_BTINQ_TIMEOUT) //30*100ms=3s

typedef enum
{
    TLKMDI_BTINQ_DTYPE_MISC    = 0x00, //
    TLKMDI_BTINQ_DTYPE_PC      = 0x01, // PC
    TLKMDI_BTINQ_DTYPE_PHONE   = 0x02, // Phone
    TLKMDI_BTINQ_DTYPE_HEADSET = 0x04, // headset
    TLKMDI_BTINQ_DTYPE_UNKNOWN = 0xFF,
} TLKMDI_BTINQ_DTYPE_ENUM;

typedef enum
{
    TLKMDI_BTINQ_STATE_IDLE = 0,
    TLKMDI_BTINQ_STATE_INQUIRY,
    TLKMDI_BTINQ_STATE_GETNAME,
    TLKMDI_BTINQ_STATE_CLOSING,
} TLKMDI_BTINQ_STATE_ENUM;

typedef enum
{
    TLKMDI_BTINQ_STAGE_NONE = 0,

    // TLKMDI_BTINQ_STATE_INQUIRY
    TLKMDI_BTINQ_INQUIRY_STAGE_START = 1,
    TLKMDI_BTINQ_INQUIRY_STAGE_WAIT_START_RSP,
    TLKMDI_BTINQ_INQUIRY_STAGE_DOING,
    TLKMDI_BTINQ_INQUIRY_STAGE_CLOSE,
    TLKMDI_BTINQ_INQUIRY_STAGE_WAIT_CLOSE_RSP,

    // TLKMDI_BTINQ_STATE_GETNAME
    TLKMDI_BTINQ_GETNAME_STAGE_START = 1, // Cancel Inquiry
    TLKMDI_BTINQ_GETNAME_STAGE_SEND_GET,
    TLKMDI_BTINQ_GETNAME_STAGE_WAIT_GET_RSP,

    // TLKMDI_BTINQ_STATE_CLOSING
    TLKMDI_BTINQ_CLOSING_STAGE_CANCEL_GETNAME = 1,
    TLKMDI_BTINQ_CLOSING_STAGE_CANCEL_INQUIRY,
    TLKMDI_BTINQ_CLOSING_STAGE_WAIT_CANCEL,
    TLKMDI_BTINQ_CLOSING_STAGE_INQUIRY_OVER,

} TLKMDI_BTINQ_STAGE_ENUM;

typedef enum
{
    TLKMDI_BTINQ_ITEM_STATE_NONE = 0,
    TLKMDI_BTINQ_ITEM_STATE_WAIT,
    TLKMDI_BTINQ_ITEM_STATE_OVER,
} TLKMDI_BTINQ_ITEM_STATE_ENUM;

typedef struct
{
    uint8_t rssi;
    uint8_t state; // TLKMDI_BTINQ_ITEM_STATE_ENUM
    uint8_t smode; // Scan Mode
    uint8_t dtype;

    uint8_t  nameLen; // Name Length
    uint8_t  reserve;
    uint8_t  btaddr[6];
    uint8_t  btname[TLKMDI_BTINQ_NAME_LENS + 1];
    uint16_t reserve2B;
    uint16_t clkOff;
    uint32_t devClass;
} tlkmdi_btinq_item_t;

typedef struct
{
    uint8_t state;
    uint8_t retry_num;
    uint8_t stage;
    uint8_t inqType; // pc: 1, phone:2, network_dev :3,	headset 4, keyboard:5,	spp_dev : 0x0f;

    uint8_t curNumb;
    uint8_t maxNumb;
    uint8_t nameIdx;
    uint8_t rssiThd;

    uint16_t inqWind;
    uint16_t timeout;

    TlkApiTimer_t       timer;
    tlkmdi_btinq_item_t item[TLKMDI_BTINQ_ITEM_NUMB];
} tlkmdi_btinq_ctrl_t;

typedef int (*TlkMdiBtInqReportCallBack)(uint32_t devClass, uint8_t rssi, uint8_t nameLen, uint8_t *pBtaddr, uint8_t *pBtName);
typedef void (*TlkMdiBtInqCompleteCallBack)(void);


/**
 * @brief  The inquiry param initialize function.
 * @param  None.
 * @return TLK_ENONE: Success, others: Failed.
*/
int tlkmdi_btinq_init(void);

/**
 * @brief  Get whether the Inquiry process is busy or not.
 * @param  None.
 * @return True if the Inquiry process is busy, otherwise False.
*/
bool tlkmdi_btinq_isBusy(void);

/**
 * @brief  Start the Inquiry process.
 * @param  inqType: Inquiry device type.
 * @param  rssiThd: RSSI threshold.
 * @param  maxNumb: Maximum number of devices to be searched.
 * @param  inqWind: Inquiry window.
 * @return TLK_ENONE if successful, otherwise error code.
*/
int tlkmdi_btinq_start(uint8_t inqType, uint8_t rssiThd, uint8_t maxNumb,
                       uint8_t inqWind); // timeout: unit-s

/**
 * @brief  Stop the Inquiry process.
 * @param  None.
 * @return None.
*/
void tlkmdi_btinq_close(void);

/**
 * @brief  Reset the Inquiry parameters.
 * @param  None.
 * @return None.
*/
void tlkmdi_btinq_reset(void);


/**
 * @brief  Register the callback function for reporting the BT devices.
 * @param  reportCB[IN]--The callback function for reporting the BT devices.
 * @param  completeCB[IN]--The callback function for complete the BT devices inquiry.
 * @return None.
*/
void tlkmdi_btinq_regCallback(TlkMdiBtInqReportCallBack reportCB, TlkMdiBtInqCompleteCallBack completeCB);

/**
 * @brief  Clean the Inquiry items.
 * @param  None.
 * @return None.
*/
void tlkmdi_btinq_cleanItems(void);

/**
 * @brief  Get the item by index.
 * @param  index[IN]--The index of item.
 * @return The block of item.
*/
tlkmdi_btinq_item_t *tlkmdi_btinq_getItem(uint8_t index);

/**
 * @brief  Get the idle item.
 * @param  None.
 * @return The idle item.
*/
tlkmdi_btinq_item_t *tlkmdi_btinq_getIdleItem(void);

/**
 * @brief  Get the item by address.
 * @param  pAddr[IN]--The address of item.
 * @return The item.
*/
tlkmdi_btinq_item_t *tlkmdi_btinq_getUsedItem(uint8_t *pAddr);

/**
 * @brief  Print the list of searched BT device.
 * @param  None.
 * @return None.
*/
void tlkmdi_btinq_printList(void);


#endif // TLKMDI_BTINQ_H
