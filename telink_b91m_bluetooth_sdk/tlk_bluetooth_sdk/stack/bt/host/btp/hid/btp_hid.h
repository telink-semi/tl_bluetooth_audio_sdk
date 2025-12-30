/********************************************************************************************************
 * @file    btp_hid.h
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
#ifndef BTP_HID_H
#define BTP_HID_H


/* HIDP handshake results */
#define BTP_HID_HSHK_SUCCESS                 0x00
#define BTP_HID_HSHK_NOT_READY               0x01
#define BTP_HID_HSHK_ERR_INVALID_REPORT_ID   0x02
#define BTP_HID_HSHK_ERR_UNSUPPORTED_REQUEST 0x03
#define BTP_HID_HSHK_ERR_INVALID_PARAMETER   0x04
#define BTP_HID_HSHK_ERR_UNKNOWN             0x0e
#define BTP_HID_HSHK_ERR_FATAL               0x0f

/* HIDP data transaction headers */
#define BTP_HID_DATA_RSRVD_MASK    0x0c
#define BTP_HID_DATA_RTYPE_OTHER   0x00
#define BTP_HID_DATA_RTYPE_INPUT   0x01
#define BTP_HID_DATA_RTYPE_OUTPUT  0x02
#define BTP_HID_DATA_RTYPE_FEATURE 0x03

/* HIDP protocol header parameters */
#define BTP_HID_PROTO_BOOT                              0x00
#define BTP_HID_PROTO_REPORT                            0x01
#define BTP_HID_PROTO_MODE_REPORT_WITH_FALLBACK_TO_BOOT 0x02


#define BTP_HID_RPT_DATA_SIZE_MASK                      (0x03)
#define BTP_HID_RPT_TYPE_MASK                           (0x0C)
#define BTP_HID_RPT_TAG_MASK                            (0xF0)

#define BTP_HID_RPT_TYPE_MAIN                           (0x00)
#define BTP_HID_RPT_TYPE_GLOBAL                         (0x04)
#define BTP_HID_RPT_TYPE_LOCAL                          (0x08)

#define BTP_HID_RPT_DATA_BITS_0                         (0x00)
#define BTP_HID_RPT_DATA_BITS_8                         (0x01)
#define BTP_HID_RPT_DATA_BITS_16                        (0x02)
#define BTP_HID_RPT_DATA_BITS_32                        (0x03)
#define BTP_HID_RPT_DATA_BITS(Data_bits)                BTP_HID_RPT_DATA_BITS_##Data_bits

#define BTP_HID_RPT_DATA_ENCODE_0(Data)
#define BTP_HID_RPT_DATA_ENCODE_8(Data)                   , (Data & 0xFF)
#define BTP_HID_RPT_DATA_ENCODE_16(Data)                  BTP_HID_RPT_DATA_ENCODE_8(Data) BTP_HID_RPT_DATA_ENCODE_8(Data >> 8)
#define BTP_HID_RPT_DATA_ENCODE_32(Data)                  BTP_HID_RPT_DATA_ENCODE_16(Data) BTP_HID_RPT_DATA_ENCODE_16(Data >> 16)
#define BTP_HID_RPT_DATA_ENCODE(Data_bits, ...)           BTP_HID_RPT_DATA_ENCODE_##Data_bits(__VA_ARGS__)


#define BTP_HID_RPT_DATA_ENTRY(Type, Tag, Data_bits, ...) (Type | Tag | BTP_HID_RPT_DATA_BITS(Data_bits)) BTP_HID_RPT_DATA_ENCODE(Data_bits, (__VA_ARGS__))

// data item for Input/Output/Feature
#define BTP_HID_IOF_CONSTANT           BIT(0)
#define BTP_HID_IOF_DATA               (0 << 0)
#define BTP_HID_IOF_VARIABLE           BIT(1)
#define BTP_HID_IOF_ARRAY              (0 << 1)
#define BTP_HID_IOF_RELATIVE           BIT(2)
#define BTP_HID_IOF_ABSOLUTE           (0 << 2)
#define BTP_HID_IOF_WRAP               BIT(3)
#define BTP_HID_IOF_NO_WRAP            (0 << 3)
#define BTP_HID_IOF_NON_LINEAR         BIT(4)
#define BTP_HID_IOF_LINEAR             (0 << 4)
#define BTP_HID_IOF_NO_PREFERRED_STATE BIT(5)
#define BTP_HID_IOF_PREFERRED_STATE    (0 << 5)
#define BTP_HID_IOF_NULLSTATE          BIT(6)
#define BTP_HID_IOF_NO_NULL_POSITION   (0 << 6)
#define BTP_HID_IOF_VOLATILE           BIT(7)
#define BTP_HID_IOF_NON_VOLATILE       (0 << 7)
#define BTP_HID_IOF_BUFFERED_BYTES     BIT(8)
#define BTP_HID_IOF_BITFIELD           (0 << 8)

// HID report definitions
#define BTP_HID_RPT_INPUT(Data_bits, ...)            BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_MAIN, 0x80, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_OUTPUT(Data_bits, ...)           BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_MAIN, 0x90, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_COLLECTION(Data_bits, ...)       BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_MAIN, 0xA0, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_FEATURE(Data_bits, ...)          BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_MAIN, 0xB0, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_END_COLLECTION(Data_bits, ...)   BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_MAIN, 0xC0, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_USAGE_PAGE(Data_bits, ...)       BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0x00, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_LOGICAL_MINIMUM(Data_bits, ...)  BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0x10, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_LOGICAL_MAXIMUM(Data_bits, ...)  BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0x20, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_PHYSICAL_MINIMUM(Data_bits, ...) BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0x30, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_PHYSICAL_MAXIMUM(Data_bits, ...) BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0x40, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_UNIT_EXPONENT(Data_bits, ...)    BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0x50, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_UNIT(Data_bits, ...)             BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0x60, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_REPORT_SIZE(Data_bits, ...)      BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0x70, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_REPORT_ID(Data_bits, ...)        BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0x80, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_REPORT_COUNT(Data_bits, ...)     BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0x90, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_PUSH(Data_bits, ...)             BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0xA0, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_POP(Data_bits, ...)              BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_GLOBAL, 0xB0, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_USAGE(Data_bits, ...)            BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_LOCAL, 0x00, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_USAGE_MINIMUM(Data_bits, ...)    BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_LOCAL, 0x10, Data_bits, __VA_ARGS__)
#define BTP_HID_RPT_USAGE_MAXIMUM(Data_bits, ...)    BTP_HID_RPT_DATA_ENTRY(BTP_HID_RPT_TYPE_LOCAL, 0x20, Data_bits, __VA_ARGS__)


typedef uint (*BtpHidSetProtocolCallback)(uint16_t aclHandle, uint8_t protoMode);
typedef uint (*BtpHidGetProtocolCallback)(uint16_t aclHandle, uint8_t *pProtoMode);

typedef uint (*BtpHidSetReportCallback)(uint16_t aclHandle, uint8_t reportType, uint8_t reportID, uint8_t *pData, uint16_t datalen);
typedef uint (*BtpHidGetReportCallback)(uint16_t aclHandle, uint8_t reportType, uint8_t reportID, uint8_t *pBuff, uint16_t *pBuffLen);

typedef int (*BtpHiddRecvDataCallback)(uint16_t handle, uint8_t *pData, uint16_t dataLen);


/******************************************************************************
 * Function: HID initial interface
 * Descript: Defines trigger the initial flow of HID source
 * Params:
 * 		@count[IN]--the HID number, refer to TLK_BT_HID_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value is returned means the initial process fail.
 *******************************************************************************/
int btp_hid_init(uint8_t count);

/******************************************************************************
 * Function: HID deinitial interface
 * Descript: Defines trigger the initial flow of HID source
 * Params:
 * 		@count[IN]--the HID number, refer to TLK_BT_HID_MAX_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int btp_hid_deinit(void);

/******************************************************************************
 * Function: btp_hid_getMemLen
 * Descript: HID get the size of the required number of nodes interface.
 * Params:
 * 		@count[IN]--the HID number, refer to TLK_BT_HID_MAX_NUMB
 * Return: Actual node memory size required(unit: byte).
 *******************************************************************************/
int btp_hid_getMemLen(uint8_t count);

int btp_hid_connect(uint16_t handle, uint8_t usrID);
int btp_hid_disconn(uint16_t handle, uint8_t usrID);


/******************************************************************************
 * Function: HID destroy interface.
 * Descript: Defines destroy the hid control block.
 * Params: [IN]handle--ACL handle
 * Return: Returning TLK_ENONE is success.
 *******************************************************************************/
void btp_hid_destroy(uint16_t handle);

extern int btp_hid_enableQos(bool enable);
extern int btp_hid_enableRtnMode(bool enable, uint8_t rtnMode);


/******************************************************************************
 * Function: HID device initial interface.
 * Descript: Defines the interface for initial hid connected control block exist.
 * Params: [IN]callback--callback function
 * Return: Returning TLK_ENONE is success.
 *******************************************************************************/
extern void btp_hidd_regCB(BtpHidSetReportCallback setReportCB, BtpHidGetReportCallback getReportCB, BtpHidSetProtocolCallback setProtocolCB,
                           BtpHidGetProtocolCallback getProtocolCB, BtpHiddRecvDataCallback ctrDataCB, BtpHiddRecvDataCallback irqDataCB);

/******************************************************************************
 * Function: HID connect interface.
 * Descript: Defines hid connect interface.
 * Params: [IN]handle--ACL handle
 * Return: Returning TLK_ENONE is success.
 *******************************************************************************/
extern int btp_hidd_connect(uint16_t handle);

/******************************************************************************
 * Function: HID disconnect interface.
 * Descript: Defines hid disconnect interface.
 * Params: [IN]handle--ACL handle
 * Return: Returning TLK_ENONE is success.
 *******************************************************************************/
extern int btp_hidd_disconn(uint16_t handle);

extern uint16_t btp_hidd_getAnyConnHandle(void);

extern int btp_hidd_sendData(uint16_t aclHandle, uint8_t reportID, uint8_t reportType, uint8_t *pData, uint16_t dataLen);
extern int btp_hidd_sendDataWithoutReportID(uint16_t aclHandle, uint8_t reportType, uint8_t *pData, uint16_t dataLen);


int btp_hid_sendIrqData(uint16_t aclHandle, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen);
int btp_hid_sendCtrData(uint16_t aclHandle, uint8_t *pHead, uint16_t headLen, uint8_t *pData, uint16_t dataLen);


#endif // #ifndef BTP_HID_H
