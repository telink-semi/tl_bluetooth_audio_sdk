/********************************************************************************************************
 * @file    svc_telink.h
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

//OTA
#define SERVICE_TELINK_OTA_HDL SERVICE_TELINK_PRIVATE_START_HDL
#define OTA_MAX_HDL_NUM        0x08

// SPP
#define SERVICE_TLK_SPP_HDL     (SERVICE_TELINK_OTA_HDL + OTA_MAX_HDL_NUM)
#define SPP_MAX_HDL_NUM        0x08

// OTA Version 2
#define SERVICE_TLK_OTA2_HDL    (SERVICE_TLK_SPP_HDL + SPP_MAX_HDL_NUM)
#define OTA2_MAX_HDL_NUM       0x08
