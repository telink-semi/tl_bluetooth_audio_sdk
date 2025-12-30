/********************************************************************************************************
 * @file    hid_iso.h
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


#define HID_ISO_REPORT_TYPE_INPUT                   0x00
#define HID_ISO_REPORT_TYPE_OUTPUT                  0x01
#define HID_ISO_HYBRID_MODE_ULL_REPORT_ENABLE_COUNT 2

union hid_iso_supp_report_intervals
{ // supported report intervals.

    struct
    {
        uint16_t interval_1ms    : 1; // Bit 0: 1ms
        uint16_t interval_2ms    : 1; // Bit 1: 2ms
        uint16_t interval_3ms    : 1; // Bit 2: 3ms
        uint16_t interval_4ms    : 1; // Bit 3: 4ms
        uint16_t interval_5ms    : 1; // Bit 4: 5ms
        uint16_t interval_1_25ms : 1; // Bit 5: 1.25ms
        uint16_t interval_2_5ms  : 1; // Bit 6: 2.5ms
        uint16_t interval_3_75ms : 1; // Bit 7: 3.75ms
        uint16_t interval_7_5ms  : 1; // Bit 8: 7.5ms
        uint16_t intervalRFU     : 7; // Bit 9-15: RFU
    };

    uint16_t intervals;
};

enum ble_hid_isos_error_code
{
    HID_ISO_ERROR_NONE,
    HID_ISO_ERROR_INVALID_PARAM,
    HID_ISO_ERROR_UNSUPPORTED_MODE,
    HID_ISO_ERROR_DEVICE_ALREADY_MODE,
    HID_ISO_ERROR_CCC_INVALID,
    HID_ISO_ERROR_NO_ULL_HID_PROPERTIES_HDL,
    HID_ISO_ERROR_NO_LE_HID_OPERATION_MODE_HDL,
    HID_ISO_ERROR_NONE_PTR,
    HID_ISO_ERROR_SEND_FAILED,
};

struct ble_hid_isos_hybrid_report_param
{
    uint8_t index;
    uint8_t confirmation_enable;
    uint8_t repetition_enable;
};

struct ble_hid_isos_select_hybrid_param
{
    uint8_t                                 CIG_ID;
    uint8_t                                 CIS_ID;
    union hid_iso_supp_report_intervals     suppInterval;
    uint8_t                                 max_sdu_size_for_input;
    uint8_t                                 max_sdu_size_for_output;
    uint8_t                                 indicesCnt;
    struct ble_hid_isos_hybrid_report_param indices[HID_ISO_HYBRID_MODE_ULL_REPORT_ENABLE_COUNT];
};

#include "hid_isos.h"
#include "hid_isoc.h"
