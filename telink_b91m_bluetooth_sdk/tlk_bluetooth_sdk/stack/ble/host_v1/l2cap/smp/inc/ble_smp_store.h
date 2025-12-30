/********************************************************************************************************
 * @file    ble_smp_store.h
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

struct ble_host_smp_store_key
{
    uint8_t is_central              : 1;
    uint8_t secure_connections_flag : 1;
    uint8_t encrypt_key_size        : 5;
    uint8_t rfu                     : 1;

    uint8_t pairing_mode;
    uint8_t peer_ota_addr_type;
    uint8_t peer_ota_addr[6];
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];

    uint32_t pairing_index;
    uint16_t peripheral_ediv;
    uint16_t central_ediv;
    uint64_t peripheral_random;
    uint64_t central_random;

    uint8_t peer_irk[16];
    uint8_t peripheral_ltk[16];
    uint8_t central_ltk[16];
};

/**
 * @brief       Initializes the SMP storage with the maximum number of bound devices per role.
 * @param[in]   peripheral_max_bound_num  - maximum stored pairings for the peripheral role.
 * @param[in]   central_max_bound_num     - maximum stored pairings for the central role.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 * @note        none.
 */
int ble_host_smp_store_init(uint8_t peripheral_max_bound_num, uint8_t central_max_bound_num);

/**
 * @brief       Deinitializes the SMP storage module and releases resources.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 * @note        none.
 */
int ble_host_smp_store_deinit(void);

/**
 * @brief       Sets the current device index for subsequent storage operations.
 * @param[in]   device_index    - index of the target device record.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 * @note        none.
 */
int ble_host_smp_store_set_device_index(uint8_t device_index);

/**
 * @brief       Deletes pairing information associated with a connection handle.
 * @param[in]   conn_handle - the connection handle.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 * @note        none.
 */
int ble_host_smp_store_delete_pairing_by_conn_handle(uint16_t conn_handle);

/**
 * @brief       Deletes pairing information for a specific peer address.
 * @param[in]   addr_type   - BLE address type of the peer.
 * @param[in]   addr        - pointer to the 6-byte peer address.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 * @note        none.
 */
int ble_host_smp_store_delete_pairing(uint8_t addr_type, const uint8_t addr[6]);

/**
 * @brief       Retrieves pairing information by connection handle.
 * @param[in]   conn_handle - the connection handle.
 * @return      Pointer to the pairing record, or NULL if not found.
 * @note        none.
 */
struct ble_host_smp_store_key *ble_host_smp_store_get_pairing_info_by_conn_handle(uint16_t conn_handle);

/**
 * @brief       Retrieves pairing information by peer address.
 * @param[in]   addr_type   - BLE address type of the peer.
 * @param[in]   addr        - pointer to the 6-byte peer address.
 * @return      Pointer to the pairing record, or NULL if not found.
 * @note        none.
 */
struct ble_host_smp_store_key *ble_host_smp_store_get_pairing_info(uint8_t addr_type, const uint8_t addr[6]);

/**
 * @brief       Gets the current number of stored central-role pairings.
 * @return      Number of central pairings currently stored.
 * @note        none.
 */
int ble_host_smp_store_get_current_number_central(void);

/**
 * @brief       Gets the current number of stored peripheral-role pairings.
 * @return      Number of peripheral pairings currently stored.
 * @note        none.
 */
int ble_host_smp_store_get_current_number_peripheral(void);

/**
 * @brief       Deletes all stored pairing information for the central role.
 * @return      none.
 * @note        none.
 */
void ble_host_smp_store_delete_all_info_central(void);

/**
 * @brief       Deletes all stored pairing information for the peripheral role.
 * @return      none.
 * @note        none.
 */
void ble_host_smp_store_delete_all_info_peripheral(void);

/**
 * @brief       Loads a central-role pairing record by index.
 * @param[in]   index   - zero-based index of the stored record.
 * @return      Pointer to the pairing record, or NULL if the index is invalid.
 * @note        none.
 */
struct ble_host_smp_store_key *ble_host_smp_store_load_info_by_index_central(uint32_t index);

/**
 * @brief       Loads a peripheral-role pairing record by index.
 * @param[in]   index   - zero-based index of the stored record.
 * @return      Pointer to the pairing record, or NULL if the index is invalid.
 * @note        none.
 */
struct ble_host_smp_store_key *ble_host_smp_store_load_info_by_index_peripheral(uint32_t index);
