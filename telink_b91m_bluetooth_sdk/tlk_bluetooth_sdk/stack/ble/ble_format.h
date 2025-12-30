/********************************************************************************************************
 * @file    ble_format.h
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
#ifndef BLE_FORMAT_H
#define BLE_FORMAT_H


#include "stack/ble/ble_common.h"

#include "stack/ble/ble_ad_type.h"

typedef struct __attribute__((packed))
{
    unsigned char length;
    unsigned char type;
    unsigned char data[1];
} advData_str_t; //ADV data structure

#define APPEAR_GENERIC_HID  0x0300
#define APPEAR_HID_KEYBOARD 0x03C1
#define APPEAR_HID_MOUSE    0x03C2

typedef struct __attribute__((packed))
{
    u8  type;
    u8  rf_len;
    u16 l2capLen;
    u16 chanId;
    u8  opcode;
    u16 handle;
    u8  dat[20];
} rf_packet_att_t;

typedef struct __attribute__((packed))
{
    u8  type;
    u8  rf_len;
    u16 l2cap;
    u16 chanid;

    u8  att;
    u16 handle;

    u8 dat[20];

} rf_packet_att_data_t;

typedef struct __attribute__((packed))
{
    u8  type;
    u8  rf_len;
    u16 l2capLen;
    u16 chanId;
    u8  opcode;
    u8  datalen;
    u8  data[1]; // character_handle / property / value_handle / value
} rf_packet_att_readByTypeRsp_t;

typedef struct __attribute__((packed))
{
    u8  type;
    u8  rf_len;
    u16 l2capLen;
    u16 chanId;
    u8  opcode;
    u8  errOpcode;
    u16 errHandle;
    u8  errReason;
} rf_packet_att_errRsp_t;


#endif /* BLE_FORMAT_H */
