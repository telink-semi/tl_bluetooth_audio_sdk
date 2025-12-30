/********************************************************************************************************
 * @file    hci_cmd_le_acl.h
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
 * @brief Reads the local supported features for the HCI LE.
 *
 * @param[out] p_le_local_supp_feat Pointer to the structure to store the response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_read_local_supported_features(struct ble_hci_le_rd_loc_supp_feat_rp *p_le_local_supp_feat);

/**
 * @brief LE connection update ACL parameter.
 *
 * @param[in] p_conn_update Pointer to th structure to LE connection update command parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_conn_update(const struct ble_hci_le_conn_update_cp *p_conn_update);

/**
 *   @brief Reads the remote features for the specified connection handle.
 *
 *   @param[in] p_rd_rem_feat Pointer to the command parameters.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_read_rem_feat(const struct ble_hci_le_rd_rem_feat_cp *p_rd_rem_feat);

/**
 *   @brief LE remote connection parameter request reply.
 *
 *   @param[in] p_conn_param_reply Pointer to the command parameters.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_remote_connection_parameter_request_reply(const struct ble_hci_le_rem_conn_param_rr_cp *p_conn_param_reply);

/**
 *   @brief LE remote connection parameter request negative reply.
 *
 *   @param[in] p_conn_param_neg_reply Pointer to the command parameters.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_remote_connection_parameter_request_negative_reply(const struct ble_hci_le_rem_conn_params_nrr_cp *p_conn_param_neg_reply);

/**
 *   @brief Sets the data length for the specified connection handle.
 *
 *   @param[in] p_set_data_len Pointer to the command parameters.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_data_len(const struct ble_hci_le_set_data_len_cp *p_set_data_len);

/**
 *   @brief Sets the PHY for the specified connection handle.
 *
 *   @param[in] p_set_phy Pointer to the command parameters.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_phy(const struct ble_hci_le_set_phy_cp *p_set_phy);
