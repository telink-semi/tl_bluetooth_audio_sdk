/********************************************************************************************************
 * @file    btp_hidInner.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef BTP_HID_INNER_H
#define BTP_HID_INNER_H

#if (TLKBTP_CFG_HID_ENABLE)


    #define BTP_HID_DBG_FLAG      ((TLK_MAJOR_DBGID_BTP << 24) | (TLK_MINOR_DBGID_BTP_HID << 16) | TLK_DEBUG_DBG_FLAG_ALL)
    #define BTP_HID_DBG_SIGN      "[HID]"


    #define BTP_HID_CONN_TIMEOUT2 (5000000 / BTP_TIMER_TIMEOUT)


    /* HIDP header masks */
    #define BTP_HID_HEADER_TRANS_MASK 0xf0
    #define BTP_HID_HEADER_PARAM_MASK 0x0f

    /* HIDP transaction types */
    #define BTP_HID_TRANS_HANDSHAKE    0x00
    #define BTP_HID_TRANS_HID_CONTROL  0x10
    #define BTP_HID_TRANS_GET_REPORT   0x40
    #define BTP_HID_TRANS_SET_REPORT   0x50
    #define BTP_HID_TRANS_GET_PROTOCOL 0x60
    #define BTP_HID_TRANS_SET_PROTOCOL 0x70
    #define BTP_HID_TRANS_GET_IDLE     0x80
    #define BTP_HID_TRANS_SET_IDLE     0x90
    #define BTP_HID_TRANS_DATA         0xa0
    #define BTP_HID_TRANS_DATC         0xb0

    /* HIDP control operation parameters */
    #define BTP_HID_CTRL_NOP                           0x00
    #define BTP_HID_CTRL_HARD_RESET                    0x01
    #define BTP_HID_CTRL_SOFT_RESET                    0x02
    #define BTP_HID_CTRL_SUSPEND                       0x03
    #define BTP_HID_CTRL_EXIT_SUSPEND                  0x04
    #define BTP_HID_CTRL_VIRTUAL_CABLE_UNPLUG          0x05


    #define BTP_HID_VIRTUAL_CABLE_UNPLUG               0
    #define BTP_HID_BOOT_PROTOCOL_MODE                 1
    #define BTP_HID_BLUETOOTH_VENDOR_ID                9
    #define BTP_HID_WAITING_FOR_RETURN                 10
    #define BTP_HID_WAITING_FOR_SEND_ACK               11


    #define BTP_HID_REPORT_ID_UNDECLARED               0
    #define BTP_HID_REPORT_ID_VALID                    1
    #define BTP_HID_REPORT_ID_INVALID                  2

    #define BTP_HID_CONTROL_PARAM_SUSPEND              0x03 // Go to reduced power mode.
    #define BTP_HID_CONTROL_PARAM_EXIT_SUSPEND         0x04 // Exit reduced power mode.
    #define BTP_HID_CONTROL_PARAM_VIRTUAL_CABLE_UNPLUG 0x05

    #define BTP_HID_BOOT_MODE_KEYBOARD_ID              1
    #define BTP_HID_BOOT_MODE_MOUSE_ID                 2

typedef struct
{
    uint8_t state;
    uint8_t usrID;
    uint16_t busys;
    uint16_t timeout;
    uint16_t ctrScid;
    uint16_t irqScid;
    uint16_t mtuSize;
    uint16_t aclHandle;
    uint16_t reportSize;

    uint8_t active;
    uint8_t errCode;
    uint8_t reportID;
    uint8_t reportType;

    uint8_t rtnMode;
    uint8_t fcsIsEn;
    uint8_t qosIsEn;
    uint8_t reserve01;
    uint16_t ctrEhnCtrl;
    uint16_t irqEhnCtrl;

    TlkApiTimer_t timer;
} btp_hid_item_t;

typedef struct
{
    uint8_t          rtnMode;
    uint8_t          qosIsEn;
    uint16_t          reserve;
    btp_hid_item_t *item;
} btp_hidp_ctrl_t;

int btp_hid_innerInit(uint8_t count);

int btp_hid_innerDeinit(void);

int btp_hid_innerGetMemLen(uint8_t count);

int btp_hid_enableQos(bool enable);
int btp_hid_enableRtnMode(bool enable, uint8_t rtnMode);

void btp_hid_destroy(uint16_t aclHandle);


/******************************************************************************
 * Function: HID reset the control block.
 * Descript: Defines reset the special busy hid control block
 * Params:
 *       @pItem[IN]--The hid control block to reset.
 * Return: null.
 *******************************************************************************/
void btp_hid_resetNode(btp_hid_item_t *pItem);

/******************************************************************************
 * Function: HID check the hid Idle control block num interface.
 * Descript: Defines check how many hid idle control block exist.
 * Params:
 * Return: Returning The total number of hid control block.
 *******************************************************************************/
uint8_t btp_hid_getIdleCount(void);
uint8_t btp_hid_getUsedCount(void);
uint8_t btp_hid_getConnCount(void);


/******************************************************************************
 * Function: HID Get the hid idle control block.
 * Descript: Defines get a new hid control block in resource.
 * Params:
 * Return: Returning The new hid control block.
 *******************************************************************************/
btp_hid_item_t *btp_hid_getIdleNode(void);

/******************************************************************************
 * Function: HID Get the hidp Busy control block.
 * Descript: Defines get the special busy hid control block
 *           in resource via cid.
 * Params:
 *       @aclHandle[IN]--The aclHandle to search the hid control block.
 * Return: Returning The busy hid control block.
 *******************************************************************************/
btp_hid_item_t *btp_hid_getUsedNode(uint16_t aclHandle);

/******************************************************************************
 * Function: HID Get the HID Used control block.
 * Descript: Defines get the client or server Used hid control block
 *           in resource via aclhandle and usrid.
 * Params:
 *       @aclHandle[IN]--The aclHandle to search the hid control block.
 * Return: Returning The used hid control block.
 *******************************************************************************/
btp_hid_item_t *btp_hid_getAnyUsedNode(void);
btp_hid_item_t *btp_hid_getAnyConnNode(void);
btp_hid_item_t *btp_hid_getAnyUsedNodeByChnID(uint16_t aclHandle, uint16_t cid);
btp_hid_item_t *btp_hid_getUsedNodeByIntcid(uint16_t aclHandle, uint16_t scid);


/******************************************************************************
 * Function: HID Get the hidp Used control block.
 * Descript: Defines get the special Connected hid control block
 *           in resource via aclhandle and scid.
 * Params:
 *       @aclHandle[IN]--The aclHandle to  search the hid control block.
 *       @scid[IN]--The scid to search the hid control block.
 * Return: Returning The connected hid control block.
 *******************************************************************************/
btp_hid_item_t *btp_hid_getUsedNodeByScid(uint16_t aclHandle, uint16_t scid);
btp_hid_item_t *btp_hid_getConnNodeByIntcid(uint16_t aclHandle, uint16_t scid);


/******************************************************************************
 * Function: HID Get the hidp Connected control block.
 * Descript: Defines get the special Connected hid control block
 *           in resource via cid.
 * Params:
 *       @aclHandle[IN]--The cid to search the hid control block.
 * Return: Returning The connected hid control block.
 *******************************************************************************/
btp_hid_item_t *btp_hid_getConnNode(uint16_t aclHandle);
btp_hid_item_t *btp_hid_getConnNodeByScid(uint16_t aclHandle, uint16_t scid);

/******************************************************************************
 * Function: HID Get the hidp Connected control block.
 * Descript: Defines get the special Connected hid control block
 *           in resource via cid.
 * Params:
 *       @cid[IN]--The cid to search the hid control block.
 * Return: Returning The connected hid control block.
 *******************************************************************************/
btp_hid_item_t *btp_hid_getConnNodeByChnID(uint16_t cid);


#endif // TLKBTP_CFG_HID_ENABLE

#endif // BTP_HID_INNER_H
