/********************************************************************************************************
 * @file    hci_cmd_vendor.h
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


#define BLE_HCI_OCF_VENDOR_CMD (0x0050)

enum BLE_HCI_VENDOR_SUB_CMD
{
    BLE_HCI_SUB_CMD_SET_BD_ADDR = 0x01,
    BLE_HCI_SUB_CMD_SET_ACL_LATENCY,
} __attribute__((packed));

/**
 *   @brief Set the BD address.
 *
 *   @param[in] p_bd_addr Pointer to the BD address.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_vendor_set_bd_address(const uint8_t *p_bd_addr);

/**
 *   @brief Set the ACL latency.
 *
 *   @param[in] latency The ACL latency.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_vendor_set_acl_latency(uint16_t latency);
