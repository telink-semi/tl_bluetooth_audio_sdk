/********************************************************************************************************
 * @file    keyboard_cfg.c
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

const unsigned char tlk_hid_keyboard_report_map[] = {
    //keyboard report in
    0x05, 0x01,                         // Usage Pg (Generic Desktop)
    0x09, 0x06,                         // Usage (Keyboard)
    0xA1, 0x01,                         // Collection: (Application)
    0x85,
    BLE_HOST_HID_REPORT_ID_KEYBOARD_INPUT, // Report Id (keyboard)
    0x05, 0x07,                         // Usage Pg (Key Codes)
    0x19, 0xE0,                         // Usage Min (224)  VK_CTRL:0xe0
    0x29, 0xE7,                         // Usage Max (231)  VK_RWIN:0xe7
    0x15, 0x00,                         // Log Min (0)
    0x25, 0x01,                         // Log Max (1)
    // Modifier byte
    0x75, 0x01,                         // Report Size (1)   1 bit * 8
    0x95, 0x08,                         // Report Count (8)
    0x81, 0x02,                         // Input: (Data, Variable, Absolute)
    // Reserved byte
    0x95, 0x01,                         // Report Count (1)
    0x75, 0x08,                         // Report Size (8)
    0x81, 0x01,                         // Input: (static constant)

    //keyboard output
    //5 bit led ctrl: NumLock CapsLock ScrollLock Compose kana
    0x95, 0x05, //Report Count (5)
    0x75, 0x01, //Report Size (1)
    0x05, 0x08, //Usage Pg (LEDs )
    0x19, 0x01, //Usage Min
    0x29, 0x05, //Usage Max
    0x91, 0x02, //Output (Data, Variable, Absolute)
    //3 bit reserved
    0x95, 0x01, //Report Count (1)
    0x75, 0x03, //Report Size (3)
    0x91, 0x01, //Output (static constant)

    // Key arrays (6 bytes)
    0x95, 0x06, // Report Count (6)
    0x75, 0x08, // Report Size (8)
    0x15, 0x00, // Log Min (0)
    0x25, 0xF1, // Log Max (241)
    0x05, 0x07, // Usage Pg (Key Codes)
    0x19, 0x00, // Usage Min (0)
    0x29, 0xf1, // Usage Max (241)
    0x81, 0x00, // Input: (Data, Array)
    0xC0, // End Collection

    //consumer report in
    0x05, 0x0C,                                // Usage Page (Consumer)
    0x09, 0x01,                                // Usage (Consumer Control)
    0xA1, 0x01,                                // Collection (Application)
    0x85, BLE_HOST_HID_REPORT_ID_CONSUME_CONTROL_INPUT, //     Report Id
    0x75, 0x10,                                //global, report size 16 bits
    0x95, 0x01,                                //global, report count 1
    0x15, 0x01,                                //global, min  0x01
    0x26, 0x8c, 0x02,                          //global, max  0x28c
    0x19, 0x01,                                //local, min   0x01
    0x2a, 0x8c, 0x02,                          //local, max    0x28c
    0x81, 0x00,                                //main,  input data variable, absolute
    0xc0,                                      //main, end collection
};

const unsigned short tlk_hid_keyboard_report_map_len = sizeof(tlk_hid_keyboard_report_map);
