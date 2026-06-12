/********************************************************************************************************
 * @file    uuid.h
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
#pragma once

#include "stack/ble/host_v1/l2cap/att/inc/uuid128bit.h"

///////////////////////
//old UUID define
//////////////////////
/**
 *  @brief  GATT 16 bit UUID definition
 */
#define GATT_UUID_PRIMARY_SERVICE     0x2800 //!< Primary Service
#define GATT_UUID_SECONDARY_SERVICE   0x2801 //!< Secondary Service
#define GATT_UUID_INCLUDE             0x2802 //!< Include
#define GATT_UUID_CHARACTER           0x2803 //!< Characteristic
#define GATT_UUID_CHAR_EXT_PROPS      0x2900 //!< Characteristic Extended Properties
#define GATT_UUID_CHAR_USER_DESC      0x2901 //!< Characteristic User Description
#define GATT_UUID_CLIENT_CHAR_CFG     0x2902 //!< Client Characteristic Configuration
#define GATT_UUID_SERVER_CHAR_CFG     0x2903 //!< Server Characteristic Configuration
#define GATT_UUID_CHAR_PRESENT_FORMAT 0x2904 //!< Characteristic Present Format
#define GATT_UUID_CHAR_AGG_FORMAT     0x2905 //!< Characteristic Aggregate Format
#define GATT_UUID_VALID_RANGE         0x2906 //!< Valid Range
#define GATT_UUID_EXT_REPORT_REF      0x2907 //!< External Report Reference
#define GATT_UUID_REPORT_REF          0x2908 //!< Report Reference

#define GATT_UUID_DEVICE_NAME         0x2a00 //!< Report Reference
#define GATT_UUID_APPEARANCE          0x2a01
#define GATT_UUID_PERI_CONN_PARAM     0x2a04
#define GATT_UUID_SERVICE_CHANGE      0x2a05
#define GATT_UUID_BATTERY_LEVEL       0x2A19
#define GATT_UUID_FIRMWARE_VER        0x2a26 //!<Firmware Version
#define GATT_UUID_CENTRAL_ADDR_RES    0x2AA6
#define GATT_UUID_RAP_ONLY            0x2AC9

/**
 * @brief	hids_uuid Hids Characteristic UUID
 */
#define CHARACTERISTIC_UUID_HID_BOOT_KEY_INPUT   0x2A22 //!< HID Boot Keyboard Input Report
#define CHARACTERISTIC_UUID_HID_BOOT_KEY_OUTPUT  0x2A32 //!< HID Boot Keyboard Output Report
#define CHARACTERISTIC_UUID_HID_BOOT_MOUSE_INPUT 0x2A33 //!< HID Boot Mouse Input Report
#define CHARACTERISTIC_UUID_HID_INFORMATION      0x2A4A //!< HID Information
#define CHARACTERISTIC_UUID_HID_REPORT_MAP       0x2A4B //!< HID Report Map
#define CHARACTERISTIC_UUID_HID_CONTROL_POINT    0x2A4C //!< HID Control Point
#define CHARACTERISTIC_UUID_HID_REPORT           0x2A4D //!< HID Report
#define CHARACTERISTIC_UUID_HID_PROTOCOL_MODE    0x2A4E //!< HID Protocol Mode
