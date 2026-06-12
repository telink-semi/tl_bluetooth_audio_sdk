/********************************************************************************************************
 * @file    ble_gatts.h
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
 *   @brief Function for calculating the database hash.
 *
 *   @param[out] database_hash Pointer to a buffer where the hash will be stored.
 *
 *   @note The hash is calculated based on the content of the database.
 */
void ble_gatts_calculate_database_hash(uint8_t database_hash[16]);

/**
 *   @brief Function for calculating the database hash for a specific connection handle.
 *
 *   @param[in] conn_handle Connection handle for which the hash will be calculated.
 *   @param[out] database_hash Pointer to a buffer where the hash will be stored.
 *
 *   @note The hash is calculated based on the content of the database for the specified connection handle.
 */
void ble_gatts_calculate_database_hash_by_conn_handle(uint16_t conn_handle, uint8_t database_hash[16]);

void ble_host_gatts_init(void);
