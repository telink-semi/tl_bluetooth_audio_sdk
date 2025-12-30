/********************************************************************************************************
 * @file    gatt.h
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

/**
 *  @brief Definition Characteristic Properties field, core_v5.4 Vol 3, Part G, 3.3.1.1 (table 3.5).
*/
#define CHAR_PROP_BROADCAST            0x01
#define CHAR_PROP_READ                 0x02
#define CHAR_PROP_WRITE_WITHOUT_RSP    0x04
#define CHAR_PROP_WRITE                0x08
#define CHAR_PROP_NOTIFY               0x10
#define CHAR_PROP_INDICATE             0x20
#define CHAR_PROP_AUTHEN_SIGNED_WRITES 0x40
#define CHAR_PROP_EXTENDED_PROPERTIES  0x80

/**
 *  @brief Definition Characteristic Extended Properties field, core_v5.4 Vol 3, Part G, 3.3.3.1 (table 3.8).
*/
#define CHAR_EXT_PROP_RELIABLE_WRITE       0x01
#define CHAR_EXT_PROP_WRITABLE_AUXILIARIES 0x02

union characteristic_properties
{
    struct
    {
        uint8_t broadcast                 : 1;
        uint8_t read                      : 1;
        uint8_t writeWithoutResponse      : 1;
        uint8_t write                     : 1;
        uint8_t notify                    : 1;
        uint8_t indicate                  : 1;
        uint8_t authenticatedSignedWrites : 1;
        uint8_t extendedProperties        : 1;
    };

    uint8_t all;
};

enum ble_host_gatt_error_code
{
    BLE_GATT_SUCCESS = 0,
    BLE_GATT_ERR_INVALID_CONN_HANDLE,
    BLE_GATT_ERR_INVALID_PARAMS,
    BLE_GATT_ERR_INSUFFICIENT_RESOURCES,
    BLE_GATT_ERR_INVALID_ATTR_HANDLE,
    BLE_GATT_ERR_NOT_FOUND_SERVICE,
};

/**
 *   @brief this function is used to initialize the GATT module.
 *
 *   @return none.
 */
void ble_host_gatt_init(void);
