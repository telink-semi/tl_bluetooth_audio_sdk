/********************************************************************************************************
 * @file    gamepad_cfg.c
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

const unsigned char tlk_hid_gamepad_report_map[] = {
    0x05, 0x01,  // Usage Page(Generic Desktop Ctrls)
    0x09, 0x05,  // Usage(Game Pad)
    0xA1, 0x01,  // Collection(Application)
    0x85, BLE_HOST_HID_REPORT_ID_GAMEPAD_INPUT,  //   Report ID(4)
    0x05, 0x09,  //   Usage Page(Button)
    0x19, 0x01,  //   Usage Minimum(Button 1)
    0x29, 0x10,  //   Usage Maximum(Button 16)
    0x15, 0x00,  //   Logical Minimum(0)
    0x25, 0x01,  //   Logical Maximum(1)
    0x75, 0x01,  //   Report Size(1)
    0x95, 0x10,  //   Report Count(16)
    0x81, 0x02,  //   Input(Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,  //   Usage Page(Generic Desktop Ctrls)
    0x15, 0x81,  //   Logical Minimum(-127)
    0x25, 0x7F,  //   Logical Maximum(127)
    0x09, 0x30,  //   Usage(X)
    0x09, 0x31,  //   Usage(Y)
    0x09, 0x32,  //   Usage(Z)
    0x09, 0x35,  //   Usage(Rz)
    0x75, 0x08,  //   Report Size(8)
    0x95, 0x04,  //   Report Count(4)
    0x81, 0x02,  //   Input(Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,        // End Collection
};

const unsigned short tlk_hid_gamepad_report_map_len = sizeof(tlk_hid_gamepad_report_map);
