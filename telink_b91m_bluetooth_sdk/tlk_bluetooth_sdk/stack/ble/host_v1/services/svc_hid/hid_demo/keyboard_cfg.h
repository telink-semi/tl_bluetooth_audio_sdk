/********************************************************************************************************
 * @file    keyboard_cfg.h
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


#define HID_INPUT_REPORT_NUM            2
#define HID_OUTPUT_REPORT_NUM           1
#define HID_FEATURE_REPORT_NUM          0

#define HID_INPUT_REPORT_1_ID           BLE_HOST_HID_REPORT_ID_KEYBOARD_INPUT
#define HID_INPUT_REPORT_2_ID           BLE_HOST_HID_REPORT_ID_CONSUME_CONTROL_INPUT
#define HID_OUTPUT_REPORT_1_ID          BLE_HOST_HID_REPORT_ID_KEYBOARD_INPUT

// extern const unsigned char tlk_hid_keyboard_report_map[];
// extern const unsigned short tlk_hid_keyboard_report_map_len;

// struct tlk_hid_keyboard_data {
//     union {
//         struct {
//             unsigned char left_control : 1;
//             unsigned char left_shift : 1;
//             unsigned char left_alt : 1;
//             unsigned char left_GUI : 1;
//             unsigned char right_control : 1;
//             unsigned char right_shift : 1;
//             unsigned char right_alt : 1;
//             unsigned char right_GUI : 1;
//         };
//         unsigned char modifiers;
//     };
//     unsigned char reserved;
//     unsigned char key[6];
// };
