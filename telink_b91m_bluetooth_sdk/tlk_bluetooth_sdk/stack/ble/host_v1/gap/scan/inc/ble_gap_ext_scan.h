/********************************************************************************************************
 * @file    ble_gap_ext_scan.h
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

#define BLE_HOST_GAP_EXT_SCAN_SCANNING_PHY_1M        0x01
#define BLE_HOST_GAP_EXT_SCAN_SCANNING_PHY_CODED     0x04

#define BLE_HOST_GAP_EXT_SCAN_PARAM_BASIC_UNFILTERED 0x00
#define BLE_HOST_GAP_EXT_SCAN_PARAM_BASIC_FILTERED   0x01
#define BLE_HOST_GAP_EXT_SCAN_PARAM_EXT_UNFILTERED   0x02
#define BLE_HOST_GAP_EXT_SCAN_PARAM_EXT_FILTERED     0x03

#define BLE_HOST_GAP_EXT_SCAN_INTERVAL_MIN           0x0004
#define BLE_HOST_GAP_EXT_SCAN_INTERVAL_MAX           0xFFFF
#define BLE_HOST_GAP_EXT_SCAN_WINDOW_MIN             0x0004
#define BLE_HOST_GAP_EXT_SCAN_WINDOW_MAX             0xFFFF

#define BLE_HOST_GAP_EXT_SCAN_FILTER_DISABLE         0x00
#define BLE_HOST_GAP_EXT_SCAN_FILTER_ENABLE          0x01
#define BLE_HOST_GAP_EXT_SCAN_FILTER_RESET_PERIOD    0x02

#define BLE_HOST_GAP_EXT_SCAN_DURATION_FOREVER       0x00

#define BLE_HOST_GAP_EXT_SCAN_PERIOD_FOREVER         0x00

/**
 *   @brief Initialize BLE GAP extended scan module.
 *
 *   @return None.
 */
void ble_host_gap_ext_scan_init(void);

/**
 *   @brief Set extended scan parameters, only used BLE 1M PHY to scan device.
 *
 *   @param[in] filter_policy  Filter policy.
 *   @param[in] params         Extended scan parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if set extended scan parameters successfully, otherwise return error code.
 */
int ble_host_gap_ext_scan_set_param_only_1M_phy(uint8_t filter_policy, const struct ble_host_gap_scan_params *params);

/**
 *   @brief Set extended scan parameters, only used BLE Coded PHY to scan device.
 *
 *   @param[in] filter_policy  Filter policy.
 *   @param[in] params         Extended scan parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if set extended scan parameters successfully, otherwise return error code.
 *
 *   @note If the high layer only want scan the device that sent ADV_EXT_IND using LE Coded PHY, called this function.
 */
int ble_host_gap_ext_scan_set_param_only_coded_phy(uint8_t filter_policy, const struct ble_host_gap_scan_params *params);

/**
 *   @brief Set extended scan parameters, used both BLE 1M PHY and Coded PHY to scan device.
 *
 *   @param[in] filter_policy  Filter policy.
 *   @param[in] params_1M      Extended scan parameters, used BLE 1M PHY.
 *   @param[in] params_coded   Extended scan parameters, used BLE Coded PHY.
 *
 *   @return BLE_HOST_ERR_SUCC if set extended scan parameters successfully, otherwise return error code.
 *
 *   @note If the high layer want scan some device that sent ADV_EXT_IND using LE 1M PHY and Coded PHY or sent legacy advertising
 *          called this function.
 */
int ble_host_gap_ext_scan_set_param_all_phy(uint8_t filter_policy, const struct ble_host_gap_scan_params *params_1M, const struct ble_host_gap_scan_params *params_coded);

/**
 *   @note This function called ble_host_gap_ext_scan_set_param_only_1M_phy api.
 */
int ble_host_gap_ext_scan_set_param(uint8_t filter_policy, const struct ble_host_gap_scan_params *params);

/**
 *   @brief Set extended scan enable or disable.
 *
 *   @param[in] enable         Enable or disable extended scan.
 *   @param[in] filter         Filter policy, BLE_HOST_GAP_EXT_SCAN_FILTER_DISABLE or BLE_HOST_GAP_EXT_SCAN_FILTER_ENABLE.
 *   @param[in] duration       Scan duration, 0x0000 means forever, units of 10ms.
 *   @param[in] period         Scan period, 0x0000 means forever, units of 1.28sec.
 *
 *   @return BLE_HOST_ERR_SUCC if set extended scan enable or disable successfully, otherwise return error code.
 */
int ble_host_gap_ext_scan_set_enable(bool enable, uint8_t filter, uint16_t duration, uint16_t period);

/**
 *   @brief Enable extended scan using default parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if enable extended scan successfully, otherwise return error code.
 *
 *   @note This function called ble_host_gap_ext_scan_set_enable api, filter set BLE_HOST_GAP_EXT_SCAN_FILTER_ENABLE,
 *         duration set 0x0000, period set 0x0000.
 */
int ble_host_gap_ext_scan_enable(void);

/**
 *   @brief Disable extended scan.
 *
 *   @return BLE_HOST_ERR_SUCC if disable extended scan successfully, otherwise return error code.
 *
 *   @note This function called ble_host_gap_ext_scan_set_enable api, enable set false.
 */
int ble_host_gap_ext_scan_disable(void);
