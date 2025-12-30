/********************************************************************************************************
 * @file    hci_cmd_le_ext_adv.h
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


#define BLE_HCI_LE_MAX_SUPPORTED_EXT_ADV_SET_COUNT 0x3F // Maximum number of supported CISes is 4,spec says 0-0x3F
#define BLE_HCI_LE_MAX_EXT_ADV_DATA_LEN            251  // Maximum length of extended advertising data

struct ble_hci_le_set_ext_adv_data_full_cp
{
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t fragment_pref;
    uint8_t adv_data_len;
    uint8_t adv_data[BLE_HCI_LE_MAX_EXT_ADV_DATA_LEN];
} __attribute__((packed));

struct ble_hci_le_set_ext_scan_rsp_data_full_cp
{
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t fragment_pref;
    uint8_t scan_rsp_len;
    uint8_t scan_rsp[BLE_HCI_LE_MAX_EXT_ADV_DATA_LEN];
} __attribute__((packed));

struct ble_hci_le_set_ext_adv_enable_full_cp
{
    uint8_t        enable;
    uint8_t        num_sets;
    struct adv_set sets[BLE_HCI_LE_MAX_SUPPORTED_EXT_ADV_SET_COUNT];
} __attribute__((packed));

/**
 * @brief Sets the extended advertising parameters.
 *
 * @param[in] p_ext_adv_params Pointer to the structure containing the extended advertising parameters.
 * @param[out] p_ext_adv_params_rp Pointer to the structure to store the response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_ext_adv_params(const struct ble_hci_le_set_ext_adv_params_cp *p_ext_adv_params, struct ble_hci_le_set_ext_adv_params_rp *p_ext_adv_params_rp);

/**
 * @brief Sets the extended advertising data.
 *
 * @param[in] p_ext_adv_data Pointer to the structure containing the extended advertising data.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_ext_adv_data(const struct ble_hci_le_set_ext_adv_data_full_cp *p_ext_adv_data);

/**
 * @brief Sets the extended scan response data.
 *
 * @param[in] p_ext_scan_rsp_data Pointer to the structure containing the extended scan response data.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_ext_scan_rsp_data(const struct ble_hci_le_set_ext_scan_rsp_data_full_cp *p_ext_scan_rsp_data);

/**
 * @brief Enables or disables extended advertising.
 *
 * @param[in] p_ext_adv_enable Pointer to the structure containing the extended advertising enable parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_ext_adv_enable(const struct ble_hci_le_set_ext_adv_enable_full_cp *p_ext_adv_enable);

/**
 * @brief Reads the maximum advertising data length.
 *
 * @param[out] p_max_adv_data_len_rp Pointer to the structure to store the response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_read_max_adv_data_len(struct ble_hci_le_rd_max_adv_data_len_rp *p_max_adv_data_len_rp);

/**
 * @brief Reads the number of advertising sets.
 *
 * @param[out] p_num_of_adv_sets_rp Pointer to the structure to store the response parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_read_number_of_adv_sets(struct ble_hci_le_rd_num_of_adv_sets_rp *p_num_of_adv_sets_rp);

/**
 * @brief Removes an advertising set.
 *
 * @param[in] p_remove_adv_set_cp Pointer to the structure containing the parameters to remove an advertising set.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_remove_adv_set(const struct ble_hci_le_remove_adv_set_cp *p_remove_adv_set_cp);

/**
 * @brief Clears all advertising sets.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_clear_adv_sets(void);
