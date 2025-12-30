/********************************************************************************************************
 * @file    btp_hidd.h
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
#ifndef BTP_HIDD_H
#define BTP_HIDD_H

#define BTP_HIDD_GET_BUFFER_SIZE 256

typedef enum
{
    BTP_HID_BUSY_NONE                 = 0x0000,
    BTP_HID_BUSY_SEND_HANDSHAKE       = 0x0001,
    BTP_HID_BUSY_SEND_GETREPORT_RSP   = 0x0002,
    BTP_HID_BUSY_SEND_GETPROTOCOL_RSP = 0x0008,
    BTP_HID_BUSY_SEND_CONTROL_RSP     = 0x0020,
} BTP_HIDP_BUSYS_ENUM;

int btp_hidd_init(void);


/******************************************************************************
 * Function: HID device initial interface.
 * Descript: Defines the interface for initial hid connected control block exist.
 * Params: [IN]callback--callback function
 * Return: Returning TLK_ENONE is success.
 *******************************************************************************/
void btp_hidd_regCB(BtpHidSetReportCallback setReportCB, BtpHidGetReportCallback getReportCB, BtpHidSetProtocolCallback setProtocolCB, BtpHidGetProtocolCallback getProtocolCB,
                    BtpHiddRecvDataCallback ctrDataCB, BtpHiddRecvDataCallback irqDataCB);

/******************************************************************************
 * Function: HID connect interface.
 * Descript: Defines hid connect interface.
 * Params: [IN]handle--ACL handle
 * Return: Returning TLK_ENONE is success.
 *******************************************************************************/
int btp_hidd_connect(uint16_t handle);

/******************************************************************************
 * Function: HID disconnect interface.
 * Descript: Defines hid disconnect interface.
 * Params: [IN]handle--ACL handle
 * Return: Returning TLK_ENONE is success.
 *******************************************************************************/
int btp_hidd_disconn(uint16_t handle);


/******************************************************************************
 * Function: HID send device report data interface.
 * Descript: Defines send the device data.
 * Params: [IN]pData--the user data
 *         [IN]datalen--the data length
 * Return: Returning The total number of hid control block.
 *******************************************************************************/
int btp_hidd_sendData(uint16_t aclHandle, uint8_t reportID, uint8_t reportType, uint8_t *pData, uint16_t dataLen);
int btp_hidd_sendDataWithoutReportID(uint16_t aclHandle, uint8_t reportType, uint8_t *pData, uint16_t dataLen);

uint16_t btp_hidd_getAnyConnHandle(void);


void btp_hidd_connectEvt(btp_hid_item_t *pItem);
void btp_hidd_disconnEvt(btp_hid_item_t *pItem);

void btp_hidd_ctrRdataCB(btp_hid_item_t *pHid, uint8_t *pData, uint16_t dataLen);
void btp_hidd_irqRdataCB(btp_hid_item_t *pHid, uint8_t *pData, uint16_t dataLen);


#endif // BTP_HIDD_H
