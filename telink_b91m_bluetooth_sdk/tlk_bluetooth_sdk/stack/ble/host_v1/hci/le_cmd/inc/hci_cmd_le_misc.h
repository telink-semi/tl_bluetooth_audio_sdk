/********************************************************************************************************
 * @file    hci_cmd_le_misc.h
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
 * @brief Sets the event mask for the HCI LE.
 *
 * @param[in] p_le_event_mask Pointer to the structure containing the LE event mask.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_event_mask(const struct ble_hci_le_set_event_mask_cp *p_le_event_mask);

/**
 * @brief Reads the buffer size for the HCI LE.
 *
 * @param[out] p_le_buf_size Pointer to the structure to store the response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_read_buffer_size(struct ble_hci_le_rd_buf_size_rp *p_le_buf_size);

/**
 * @brief Reads the buffer size v2 for the HCI LE.
 *
 * @param[out] p_le_buf_size_v2 Pointer to the structure to store the response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_read_buffer_size_v2(struct ble_hci_le_rd_buf_size_v2_rp *p_le_buf_size_v2);

/**
 * @brief Sets the host feature for the HCI LE.
 *
 * @param[in] p_le_host_feature Pointer to the structure containing the LE host feature.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_host_feature(const struct ble_hci_le_set_host_feature_cp *p_le_host_feature);

/**
 * @brief Set the random address for the HCI LE.
 *
 * @param[in] p_le_rand_addr Pointer to the structure containing the LE random address.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_random_address(const struct ble_hci_le_set_rand_addr_cp *p_le_rand_addr);

/**
 *   @brief Read the white list(filter accept list) size for the HCI LE.
 *
 *   @param[out] p_white_list_size Pointer to the structure to store the response parameters.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_read_white_list_size(struct ble_hci_le_rd_white_list_rp *p_white_list_size);

/**
 *   @brief Clear the white list(filter accept list) for the HCI LE.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_clear_white_list(void);

/**
 *   @brief Add a device to the white list(filter accept list) for the HCI LE.
 *
 *   @param[in] p_white_list Pointer to the structure containing the LE white list parameters.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_add_white_list(const struct ble_hci_le_add_white_list_cp *p_white_list);

/**
 *   @brief Remove a device from the white list(filter accept list) for the HCI LE.
 *
 *   @param[in] p_white_list Pointer to the structure containing the LE white list parameters.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_remove_white_list(const struct ble_hci_le_rmv_white_list_cp *p_white_list);

/**
 * @brief Generate a random number from the controller.
 *
 * @param[out] dst Pointer to the buffer to store the random number.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_rand(struct ble_hci_le_rand_rp *p_le_rand);

/**
 * @brief Generate a random number(8 octets) from the controller.
 *
 * @param[out] dst Pointer to the buffer to store the random number.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_rand_simple(uint8_t dst[8]);

/**
 * @brief Encrypts data using AES ECB.
 *
 * @param[in] p_le_encrypt Pointer to the structure containing the LE encrypt parameters.
 * @param[out] p_le_encrypt_rp Pointer to the structure to store the response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_encrypt(const struct ble_hci_le_encrypt_cp *p_le_encrypt, struct ble_hci_le_encrypt_rp *p_le_encrypt_rp);

/**
 * @brief Start LE encryption for the HCI LE.
 *
 * @param[in] p_le_start_enc Pointer to the structure containing the LE start encryption parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_start_encryption(const struct ble_hci_le_start_encrypt_cp *p_le_start_enc);

/**
 * @brief LE Long Term Key Request Reply.
 *
 * @param p_le_lt_key_req_reply Pointer to the structure containing the LE Long Term Key Request Reply parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 * @param p_le_lt_key_req_reply
 * @return
 */
int ble_host_hci_le_ltk_request_reply(const struct ble_hci_le_lt_key_req_reply_cp *p_le_lt_key_req_reply);

/**
 * @brief LE Long Term Key Request Negative Reply.
 *
 * @param p_le_lt_key_req_neg_reply Pointer to the structure containing the LE Long Term Key Request Negative Reply parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_ltk_request_negative_reply(struct ble_hci_le_lt_key_req_neg_reply_cp *p_le_lt_key_req_neg_reply);

/**
 * @brief       Set the host channel classification for LE.
 * @param[in]   p_host_chan_class    - pointer to the host channel classification parameters.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_set_host_channel_classification(const struct ble_hci_le_set_host_chan_class_cp *p_host_chan_class);

/**
 * @brief       Read the local P-256 public key.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_read_local_p_256_public_key(void);

/**
 * @brief       Generate a DH key using the local private key and peer public key.
 * @param[in]   p_le_gen_dhkey    - pointer to the DH key generation parameters.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_generate_dhkey(const struct ble_hci_le_gen_dhkey_cp *p_le_gen_dhkey);

/**
 * @brief       Add a device to the resolving list.
 * @param[in]   p_le_add_resolv_list_cp    - pointer to the parameters for adding device to resolving list.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_add_device_to_resolving_list(const struct ble_hci_le_add_resolv_list_cp *p_le_add_resolv_list_cp);

/**
 * @brief       Remove a device from the resolving list.
 * @param[in]   p_le_rmv_resolve_list_cp   - pointer to the parameters for removing device from resolving list.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_remove_device_from_resolving_list(const struct ble_hci_le_rmv_resolve_list_cp *p_le_rmv_resolve_list_cp);

/**
 * @brief       Clear all devices from the resolving list.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_clear_resolving_list(void);

/**
 * @brief       Read the size of the resolving list.
 * @param[out]  p_le_rd_resolving_list_size   - pointer to the structure to store the resolving list size.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_read_resolving_list_size(struct ble_hci_le_rd_resolv_list_size_rp *p_le_rd_resolving_list_size);

/**
 * @brief       Read the peer resolvable address.
 * @param[in]   p_le_rd_peer_resolvable_addr_cp    - pointer to the parameters for reading peer resolvable address.
 * @param[out]  p_le_rd_peer_resolvable_addr_rp    - pointer to the structure to store the peer resolvable address.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_read_peer_resolvable_address(const struct ble_hci_le_rd_peer_resolv_addr_cp *p_le_rd_peer_resolvable_addr_cp,
                                                 struct ble_hci_le_rd_peer_resolv_addr_rp       *p_le_rd_peer_resolvable_addr_rp);

/**
 * @brief       Read the local resolvable address.
 * @param[in]   p_le_rd_local_resolvable_addr_cp    - pointer to the parameters for reading local resolvable address.
 * @param[out]  p_le_rd_local_resolvable_addr_rp    - pointer to the structure to store the local resolvable address.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_read_local_resolvable_address(const struct ble_hci_le_rd_local_resolv_addr_cp *p_le_rd_local_resolvable_addr_cp,
                                                  struct ble_hci_le_rd_local_resolv_addr_rp       *p_le_rd_local_resolvable_addr_rp);

/**
 * @brief       Set the address resolution enable state.
 * @param[in]   p_le_set_addr_res_en_cp    - pointer to the address resolution enable parameters.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_hci_le_set_address_resolution_enable(const struct ble_hci_le_set_addr_res_en_cp *p_le_set_addr_res_en_cp);
