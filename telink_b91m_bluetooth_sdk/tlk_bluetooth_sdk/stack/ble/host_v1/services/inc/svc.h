/********************************************************************************************************
 * @file    svc.h
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

#define SERVICE_GATT_START_HANDLE                       0x0001

// Redme note9 pro must hid start handle < 0x0100, select 0x00C0 is enough.
#define SERVICE_HID_START_HDL                           0x00C0

#define SERVICE_LE_AUDIO_START_HDL                      0x0200

#define SERVICE_CHANNEL_SOUNDING_START_HDL              0x0800

#define SERVICE_USER_16UUID_START_HDL                   0x4000

//Telink private Service all 128 uuid
#define SERVICE_TELINK_PRIVATE_START_HDL                0x8000

#define SERVICE_USER_128UUID_START_HDL                  0x9000

#define blc_gatts_addAttributeServiceGroup          ble_host_add_attribute_service_group
#define blc_gatts_removeAttributeServiceGroup       ble_host_remove_attribute_service_group
#define blc_gatts_calculateDatabaseHash(conn, databaseHash) ble_gatts_calculate_database_hash(databaseHash)
