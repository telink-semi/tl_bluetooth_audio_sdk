/********************************************************************************************************
 * @file    hci_cmd_le_scan.h
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

struct ble_hci_le_set_ext_scan_params_full_cp
{
    uint8_t            own_addr_type;
    uint8_t            filter_policy;
    uint8_t            phys;
    struct scan_params scans[2]; // core_v6.0 only support LE 1M phy and LE coded PHY.
} __attribute__((packed));

/**
 *   @brief Sets up the scan parameters.
 *
 *   @param[in] p_scan_param Pointer to the structure containing the scan parameters.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_scan_param(const struct ble_hci_le_set_scan_params_cp *p_scan_param);

/**
 *   @brief Enables the scan.
 *
 *   @param[in] p_scan_enable Pointer to the structure containing the scan enable parameters.
 *
 *   @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_scan_enable(const struct ble_hci_le_set_scan_enable_cp *p_scan_enable);

/**
 * @brief Sets up the extended scan parameters.
 *
 * @param[in] p_ext_scan_param Pointer to the structure containing the extended scan parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_ext_scan_param(const struct ble_hci_le_set_ext_scan_params_full_cp *p_ext_scan_param);

/**
 * @brief Enables the extended scan.
 *
 * @param[in] p_ext_scan_enable Pointer to the structure containing the extended scan enable parameters.
 *
 * @return int Returns 0 on success, or an error code on failure.
 */
int ble_host_hci_le_set_ext_scan_enable(const struct ble_hci_le_set_ext_scan_enable_cp *p_ext_scan_enable);
