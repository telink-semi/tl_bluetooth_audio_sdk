/********************************************************************************************************
 * @file    ble_gap_iso.h
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

#define BLE_GAP_ISO_INVALID_ISO_HANDLE (0xFFFF)

/**
 *   @brief Initialize the ISO layer.
 *
 *   @return None.
 */
void ble_host_gap_iso_init(void);

/**
 *   @brief Set the data path direction for an ISO connection, Host to Controller.
 *
 *   @param[in]  iso_conn_handle  The ISO connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_iso_set_data_path_h2c(uint16_t iso_conn_handle);

/**
 *   @brief Set the data path direction for an ISO connection, Controller to Host.
 *
 *   @param[in]  iso_conn_handle  The ISO connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_iso_set_data_path_c2h(uint16_t iso_conn_handle);

/**
 *   @brief Remove the data path direction for an ISO connection, Host to Controller.
 *
 *   @param[in]  iso_conn_handle  The ISO connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_iso_remove_data_path_h2c(uint16_t iso_conn_handle);

/**
 *   @brief Remove the data path direction for an ISO connection, Controller to Host.
 *
 *   @param[in]  iso_conn_handle  The ISO connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_iso_remove_data_path_c2h(uint16_t iso_conn_handle);

/**
 *   @brief Remove the data path direction for an ISO connection, both Host to Controller and Controller to Host.
 *
 *   @param[in]  iso_conn_handle  The ISO connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_iso_remove_data_path_both(uint16_t iso_conn_handle);

/**
 *   @brief Get the transmit time stamp for an ISO connection.
 *
 *   @param[in]  iso_conn_handle  The ISO connection handle.
 *
 *   @return The transmit time stamp.
 */
uint32_t ble_host_gap_iso_get_tx_time_stamp(uint16_t iso_conn_handle);