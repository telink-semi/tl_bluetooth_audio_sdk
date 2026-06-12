/********************************************************************************************************
 * @file    ble_gap_filter.h
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

// Macros for address type
#define BLE_HOST_GAP_FILTER_ADDR_TYPE_PUBLIC 0x00 /** < Public Device address */
#define BLE_HOST_GAP_FILTER_ADDR_TYPE_RANDOM 0x01 /** < Random Device address */

/**
 *   @brief Initialize the filter module.
 *
 *   This function should be called before any other function in the filter module.
 */
void ble_host_gap_filter_init(void);

/**
 *   @brief Clear all filter list entries.
 *
 *   This function removes all entries from the filter list.
 */
int ble_host_gap_filter_clear_list(void);

/**
 *   @brief Add a device address to the filter list.
 *
 *   @param[in] addr_type Address type of the device address.
 *   @param[in] addr Pointer to the device address.
 *
 *   @return BLE_HOST_ERR_SUCC if adding the device address to the filter list is successful.
 */
int ble_host_gap_filter_add_device_to_list(uint8_t addr_type, const uint8_t addr[6]);

/**
 *   @brief Add a device address to the filter list, include public and random address.
 *
 *   @param[in] addr Pointer to the device address.
 *
 *   @return BLE_HOST_ERR_SUCC if adding the device address to the filter list is successful.
 */
int ble_host_gap_filter_add_device_addr(const uint8_t addr[6]);

/**
 *   @brief Remove a device address from the filter list.
 *
 *   @param[in] addr_type Address type of the device address.
 *   @param[in] addr Pointer to the device address.
 *
 *   @return BLE_HOST_ERR_SUCC if removing the device address from the filter list is successful.
 */
int ble_host_gap_filter_remove_device_from_list(uint8_t addr_type, const uint8_t addr[6]);

/**
 *   @brief Remove a device address from the filter list, include public and random address.
 *
 *   @param[in] addr Pointer to the device address.
 *
 *   @return BLE_HOST_ERR_SUCC if removing the device address from the filter list is successful.
 */
int ble_host_gap_filter_remove_device_addr(const uint8_t addr[6]);
