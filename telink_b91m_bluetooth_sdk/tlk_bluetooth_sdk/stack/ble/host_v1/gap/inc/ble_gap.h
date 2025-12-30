/********************************************************************************************************
 * @file    ble_gap.h
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

#define BLE_HOST_GAP_INVALID_CONN_HANDLE 0xFFFF

#define GAP_APPEARANCE_UNKNOWN           0x0000 //!< Unknown
#define GAP_APPEARANCE_GWAD              0x0940 // Generic Wearable Audio Device
#define GAP_APPEARANCE_EARBUD            0x0941 // Earbud
#define GAP_APPEARANCE_HEADSET           0x0942 // Headset
#define GAP_APPEARANCE_HEADPHONES        0x0943 // Headphones

#define GAP_APPEARANCE_GENERIC_HID       0x0300
#define GAP_APPEARANCE_HID_KEYBOARD      0x03C1
#define GAP_APPEARANCE_HID_MOUSE         0x03C2
#define GAP_APPEARANCE_GAMEPAD           0x03C4

enum ble_host_gap_error_code
{
    BLE_HOST_GAP_ERR_SUCCESS = 0,
    BLE_HOST_GAP_ERR_INVALID_PARAM,
    BLE_HOST_GAP_ERR_INVALID_CONN_HANDLE,
    BLE_HOST_GAP_ERR_INVALID_EXT_ADV_HANDLE,
    BLE_HOST_GAP_ERR_CREATE_TIMER_FAILED,
    BLE_HOST_GAP_ERR_CREATE_ACL_BUSY,
    BLE_HOST_GAP_ERR_CIS_ALREADY_EXISTS,
    BLE_HOST_GAP_ERR_CIS_INSUFFICIENT_RESOURCES,
    BLE_HOST_GAP_ERR_CIS_PERIPHERAL_CIG_ALREADY_EXISTS,
    BLE_HOST_GAP_ERR_CIS_PERIPHERAL_INSUFFICIENT_RESOURCES,

    BLE_HOST_GAP_ERR_CIS_CENTRAL_CIG_ALREADY_EXISTS,
    BLE_HOST_GAP_ERR_CIS_CENTRAL_INSUFFICIENT_RESOURCES,
    BLE_HOST_GAP_ERR_FILTER_NOT_SUPP,

    BLE_HOST_GAP_ERR_NOT_PAIRING_INFO,
    BLE_HOST_GAP_ERR_CONTROLLER_NOT_SUPPORT_RESOLVING_LIST,
    BLE_HOST_GAP_ERR_CONTROLLER_NOT_DIRECTED_ADV,
};

/**
 *   @brief this function is used to initialize the BLE GAP stack.
 *
 *
 *   @return None.
 */
void ble_host_gap_init(void);
