/********************************************************************************************************
 * @file    dis.h
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

#define BLE_DIS_PNP_ID_VENDOR_ID_SIG_ASSIGNED 0x01
#define BLE_DIS_PNP_ID_VENDOR_ID_UIF_ASSIGNED 0x02

struct ble_dis_system_id
{
    uint64_t manufacturer : 40; /** < Manufacturer Identifier */
    uint64_t oui          : 24; /** < Organizationally Unique Identifier */
};

struct ble_dis_pnp_id
{
    uint8_t  vendor_id_source;
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t product_version;
} __attribute__((packed));
