/********************************************************************************************************
 * @file    hci_cmd_info_param.h
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
 * @brief Reads the local version information.
 *
 * @param[out] p_ver_info Pointer to the structure to store the local version information.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_read_local_version_info(struct ble_hci_ip_rd_local_ver_rp *p_ver_info);

/**
 * @brief Reads the Bluetooth device address.
 *
 * @param[out] p_bd_addr Pointer to the structure to store the Bluetooth device address.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_read_bd_address(struct ble_hci_ip_rd_bd_addr_rp *p_bd_addr);

/**
 * @brief Reads the local supported features.
 *
 * @param[out] p_local_supp_feat Pointer to the structure to store the local supported features.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_read_local_supported_features(struct ble_hci_ip_rd_loc_supp_feat_rp *p_local_supp_feat);

/**
 * @brief Reads the local supported commands.
 *
 * @param[out] p_local_supp_cmd Pointer to the structure to store the local supported commands.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_read_local_supported_commands(struct ble_hci_ip_rd_loc_supp_cmd_rp *p_local_supp_cmd);
