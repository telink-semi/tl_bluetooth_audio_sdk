/********************************************************************************************************
 * @file    mouse_cfg.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "../hid/hid_def.h"

const unsigned char tlk_hid_mouse_report_map[] = {
    0x05, 0x01,                               // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                               // USAGE (Mouse)
    0xa1, 0x01,                               // COLLECTION (Application)
    0x85, BLE_HOST_HID_REPORT_ID_MOUSE_INPUT, //report ID 01
    0x09, 0x01,                               //   USAGE (Pointer)
    0xa1, 0x00,                               //   COLLECTION (Physical)
    0x05, 0x09,                               //     USAGE_PAGE (Button)
    // 1 is mouse left button,2 is mouse right button,3 is central button
    0x19, 0x01, //     USAGE_MINIMUM (Button 1)
    0x29, 0x05, //     USAGE_MAXIMUM (Button 5)
    0x15, 0x00, //     LOGICAL_MINIMUM (0)
    0x25, 0x01, //     LOGICAL_MAXIMUM (1)
    0x95, 0x05, //     REPORT_COUNT (3)
    0x75, 0x01, //     REPORT_SIZE (1)
    0x81, 0x02, //     INPUT (Data,Var,Abs)
    0x95, 0x01, //     REPORT_COUNT (1)
    0x75, 0x03, //     REPORT_SIZE (3)
    0x81, 0x01, //     INPUT (Cnst,Var,Abs)
    0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30, //     USAGE (X)
    0x09, 0x31, //     USAGE (Y)
    0x15, 0x81, //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
    0x75, 0x08, //     REPORT_SIZE (8)
    0x95, 0x02, //     REPORT_COUNT (2)
    0x81, 0x06, //     INPUT (Data,Var,Rel)
    0x09, 0x38, //     USAGE (Wheel)
    0x15, 0x81, //LOGICAL_MINIMUM (-127)
    0x25, 0x7f, //LOGICAL_MAXIMUM (127)
    0x75, 0x08, //REPORT_SIZE (8)
    0x95, 0x01, //REPORT_COUNT (1)
    0x81, 0x06, //INPUT (Data,Var,Rel)
    0xc0,       //   END_COLLECTION
    0xc0,       // END_COLLECTION
};

const unsigned short tlk_hid_mouse_report_map_len = sizeof(tlk_hid_mouse_report_map);
