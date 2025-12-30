/********************************************************************************************************
 * @file    hci_cmd_le_pa.h
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


#define BLE_HCI_LE_MAX_PERIODIC_ADV_DATA_LEN 252 // Maximum length of periodic advertising data

struct ble_hci_le_set_periodic_adv_data_full_cp
{
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t adv_data_len;
    uint8_t adv_data[BLE_HCI_LE_MAX_PERIODIC_ADV_DATA_LEN];
} __attribute__((packed));

/**
 * @brief HCI LE Set Periodic Advertising Parameters command parameters.
 * @details The HCI LE Set Periodic Advertising Parameters command is used to set the
 *          periodic advertising parameters for a given advertising set.
 *
 * @param[in] p_periodic_adv_params Pointer to the HCI LE Set Periodic Advertising Parameters command parameters.
 *
 * @return Zero on success or error code on failure.
 */
int ble_host_hci_le_set_periodic_adv_params(const struct ble_hci_le_set_periodic_adv_params_cp *p_periodic_adv_params);

/**
 * @brief HCI LE Set Periodic Advertising Data command parameters.
 * @details The HCI LE Set Periodic Advertising Data command is used to set the
 *          periodic advertising data for a given advertising set.
 *
 * @param[in] p_periodic_adv_data Pointer to the HCI LE Set Periodic Advertising Data command parameters.
 *
 * @return Zero on success or error code on failure.
 */
int ble_host_hci_le_set_periodic_adv_data(const struct ble_hci_le_set_periodic_adv_data_full_cp *p_periodic_adv_data);

/**
 * @brief HCI LE Set Periodic Advertising Enable command parameters.
 * @details The HCI LE Set Periodic Advertising Enable command is used to enable or disable
 *          the periodic advertising for a given advertising set.
 *
 * @param[in] p_periodic_adv_enable Pointer to the HCI LE Set Periodic Advertising Enable command parameters.
 *
 * @return Zero on success or error code on failure.
 */
int ble_host_hci_le_set_periodic_adv_enable(const struct ble_hci_le_set_periodic_adv_enable_cp *p_periodic_adv_enable);


/**
 * @brief Sets up an ISO data path for the host.
 *
 * @param[in] p_create_sync Pointer to the structure containing the create sync parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_periodic_adv_create_sync(const struct ble_hci_le_periodic_adv_create_sync_cp *p_create_sync);

/**
 * @brief Cancels the periodic advertising create sync.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_periodic_adv_create_sync_cancel(void);

/**
 * @brief Terminates the periodic advertising sync.
 *
 * @param[in] p_terminate_sync Pointer to the structure containing the terminate sync parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_periodic_adv_terminate_sync(const struct ble_hci_le_periodic_adv_term_sync_cp *p_terminate_sync);

/**
 * @brief Le Set Periodic Advertising Receive Enable command parameters.
 *
 * @param[in] p_set_enable Pointer to the structure containing the set enable parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_periodic_adv_receive_enable(const struct ble_hci_le_periodic_adv_receive_enable_cp *p_set_enable);

/**
 * @brief Le Periodic Advertising Sync Transfer command parameters.
 *
 * @param[in] p_sync_transfer Pointer to the structure containing the sync transfer parameters.
 * @param[out] p_sync_transfer_rp Pointer to the structure containing the sync transfer response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_periodic_adv_sync_transfer(const struct ble_hci_le_periodic_adv_sync_transfer_cp *p_sync_transfer,
                                               struct ble_hci_le_periodic_adv_sync_transfer_rp       *p_sync_transfer_rp);

/**
 * @brief Le Periodic Advertising Set Info Transfer command parameters.
 *
 * @param[in] p_set_info_transfer Pointer to the structure containing the set info transfer parameters.
 * @param[out] p_set_info_transfer_rp Pointer to the structure containing the set info transfer response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_periodic_adv_set_info_transfer(const struct ble_hci_le_periodic_adv_set_info_transfer_cp *p_set_info_transfer,
                                                   struct ble_hci_le_periodic_adv_set_info_transfer_rp       *p_set_info_transfer_rp);

/**
 * @brief Le Set Periodic Advertising Sync Transfer Parameters command parameters.
 *
 * @param[in] p_sync_transfer_params Pointer to the structure containing the sync transfer parameters.
 * @param[out] p_sync_transfer_params_rp Pointer to the structure containing the sync transfer response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_periodic_adv_sync_transfer_params(const struct ble_hci_le_periodic_adv_sync_transfer_params_cp *p_sync_transfer_params,
                                                          struct ble_hci_le_periodic_adv_sync_transfer_params_rp       *p_sync_transfer_params_rp);

/**
 * @brief Le Set Default Periodic Advertising Sync Transfer Parameters command parameters.
 *
 * @param[in] p_default_param Pointer to the structure containing the default parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_default_periodic_adv_sync_transfer_params(const struct ble_hci_le_set_default_periodic_sync_transfer_params_cp *p_default_param);
