/********************************************************************************************************
 * @file    svc_gatt.h
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

//sdk fix service uuid handle
#define SERVICE_GENERIC_ACCESS_HDL     SERVICE_GATT_START_HANDLE
#define GAP_MAX_HDL_NUM                0x0F
#define SERVICE_GENERIC_ATTRIBUTE_HDL  SERVICE_GENERIC_ACCESS_HDL + GAP_MAX_HDL_NUM
#define GATT_MAX_HDL_NUM               0x10
#define SERVICE_DEVICE_INFORMATION_HDL SERVICE_GENERIC_ATTRIBUTE_HDL + GATT_MAX_HDL_NUM
#define DIS_MAX_HDL_NUM                0x20
#define SERVICE_BATTERY_HDL            SERVICE_DEVICE_INFORMATION_HDL + DIS_MAX_HDL_NUM
#define BAS_MAX_HDL_NUM                0x30
#define SERVICE_SCAN_PARAMETERS_HDL    SERVICE_BATTERY_HDL + BAS_MAX_HDL_NUM
#define ScPS_MAX_HDL_NUM               0x10
