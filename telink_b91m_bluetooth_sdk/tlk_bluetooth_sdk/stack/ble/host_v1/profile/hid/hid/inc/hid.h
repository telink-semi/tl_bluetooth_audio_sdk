/********************************************************************************************************
 * @file    hid.h
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

enum
{
    HID_CONTROL_POINT_ENTER_SUSPEND = 0x00,
    HID_CONTROL_POINT_EXIT_SUSPEND,
};

struct ble_hid_information
{
    uint16_t bcd_HID;
    uint8_t  b_count_code;

    union
    {
        struct
        {
            uint8_t remote_wake          : 1;
            uint8_t normally_connectable : 1;
        };

        uint8_t Flags;
    };
} __attribute__((packed));

struct ble_host_hid_boot_keyboard_input
{
    union
    {
        struct
        {
            uint8_t left_control  : 1;
            uint8_t left_shift    : 1;
            uint8_t left_alt      : 1;
            uint8_t left_GUI      : 1;
            uint8_t right_control : 1;
            uint8_t right_shift   : 1;
            uint8_t right_alt     : 1;
            uint8_t right_GUI     : 1;
        };

        uint8_t modifiers;
    };

    uint8_t reserved;
    uint8_t key[6];
};

struct ble_hid_boot_mouse_input
{
    union
    {
        struct
        {
            uint8_t left   : 1;
            uint8_t right  : 1;
            uint8_t middle : 1;
        };

        uint8_t button;
    };

    int8_t  x;
    int8_t  y;
    uint8_t wheel_delta;
};

struct ble_hid_report_reference
{
    uint8_t report_id;
    uint8_t report_type;
};

//Protocol Mode Characteristic Value
#define BOOT_PROTOCOL_MODE            0x00
#define REPORT_PROTOCOL_MODE          0x01
#define CHECK_PROTOCOL_MODE(mode)     ((mode) == BOOT_PROTOCOL_MODE || (mode) == REPORT_PROTOCOL_MODE)

#define HID_SUPPORT_REPORT_HANDLE_MAX 8 //cannot change.