/********************************************************************************************************
 * @file    device_information.h
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


/**
 * @brief	device_char_uuid Device Information Characteristic UUID
 */
#define CHARACTERISTIC_UUID_MANU_NAME_STRING     0x2A29
#define CHARACTERISTIC_UUID_MODEL_NUM_STRING     0x2A24
#define CHARACTERISTIC_UUID_SERIAL_NUM_STRING    0x2A25
#define CHARACTERISTIC_UUID_HW_REVISION_STRING   0x2A27
#define CHARACTERISTIC_UUID_FW_REVISION_STRING   0x2A26
#define CHARACTERISTIC_UUID_SW_REVISION_STRING   0x2A28
#define CHARACTERISTIC_UUID_SYSTEM_ID            0x2A23
#define CHARACTERISTIC_UUID_IEEE_11073_CERT_LIST 0x2A2A
#define CHARACTERISTIC_UUID_PNP_ID               0x2A50
