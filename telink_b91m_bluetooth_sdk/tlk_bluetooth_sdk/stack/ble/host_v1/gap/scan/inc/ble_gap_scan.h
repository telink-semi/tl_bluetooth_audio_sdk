/********************************************************************************************************
 * @file    ble_gap_scan.h
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

/** < No scan request PDUs shall be sent. */
#define BLE_HOST_GAP_SCAN_PASSIVE_SCAN 0x00
/** < Scan request PDUs may be sent. */
#define BLE_HOST_GAP_SCAN_ACTIVE_SCAN            0x01

#define BLE_HOST_GAP_SCAN_PARAM_BASIC_UNFILTERED 0x00
#define BLE_HOST_GAP_SCAN_PARAM_BASIC_FILTERED   0x01
#define BLE_HOST_GAP_SCAN_PARAM_EXT_UNFILTERED   0x02
#define BLE_HOST_GAP_SCAN_PARAM_EXT_FILTERED     0x03

#define BLE_HOST_GAP_SCAN_FILTER_DISABLE         0x00
#define BLE_HOST_GAP_SCAN_FILTER_ENABLE          0x01

struct ble_host_gap_scan_params
{
    uint8_t scan_type; /** < BLE_HOST_GAP_SCAN_PASSIVE_SCAN or BLE_HOST_GAP_SCAN_ACTIVE_SCAN. */
                       /** < Units of 0.625ms. Range: 0x0004 to 0xFFFF(extend scan). Range: 0x0004 to 0x4000(legacy scan). */
    uint16_t scan_interval;
    /** < Units of 0.625ms. Range: 0x0004 to 0xFFFF(extend scan). Range: 0x0004 to 0x4000(legacy scan). */
    uint16_t scan_window;
};

#define BLE_HOST_GAP_PASSIVE_SCAN(interval, window)      \
    (&(struct ble_host_gap_scan_params){                 \
        .scan_type     = BLE_HOST_GAP_SCAN_PASSIVE_SCAN, \
        .scan_interval = (interval) / 0.625,             \
        .scan_window   = (window) / 0.625,               \
    })

#define BLE_HOST_GAP_ACTIVE_SCAN(interval, window)      \
    (&(struct ble_host_gap_scan_params){                \
        .scan_type     = BLE_HOST_GAP_SCAN_ACTIVE_SCAN, \
        .scan_interval = (interval) / 0.625,            \
        .scan_window   = (window) / 0.625,              \
    })

/** < Structure to configure the scan filter. */
struct ble_host_gap_scan_filter_param
{
    uint16_t filter_legacy_adv        : 1; /** < Filter all legacy advertising packets. */
    uint16_t filter_extended_adv      : 1; /** < Filter all extended advertising packets. */
    uint16_t filter_connectable       : 1; /** < Filter all connectable advertising packets. */
    uint16_t filter_scannable         : 1; /** < Filter all scannable advertising packets. */
    uint16_t filter_directed          : 1; /** < Filter all directed advertising packets. */
    uint16_t filter_non_conn_non_scan : 1; /** < Filter all non-connectable and non-scannable advertising packets. */
};

#define BLE_HOST_GAP_SCAN_FILTER_ALL_NON_CONN  \
    (&(struct ble_host_gap_scan_filter_param){ \
        .filter_legacy_adv        = 0,         \
        .filter_extended_adv      = 0,         \
        .filter_connectable       = 0,         \
        .filter_scannable         = 1,         \
        .filter_directed          = 1,         \
        .filter_non_conn_non_scan = 1,         \
    })

/**
 *   @brief Initialize BLE GAP scan module.
 *
 *   @return None.
 */
void ble_host_gap_scan_init(void);

/**
 *   @brief Set scan parameters.
 *
 *   @param[in] filter_policy  Filter policy.
 *   @param[in] params         scan parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if set scan parameters successfully, otherwise return error code.
 */
int ble_host_gap_scan_set_param(uint8_t filter_policy, const struct ble_host_gap_scan_params *params);

/**
 *   @brief Set scan enable or disable.
 *
 *   @param[in] enable         Enable or disable scan.
 *   @param[in] filter         Filter policy, BLE_HOST_GAP_SCAN_FILTER_DISABLE or BLE_HOST_GAP_SCAN_FILTER_ENABLE.
 *
 *   @return BLE_HOST_ERR_SUCC if set scan enable or disable successfully, otherwise return error code.
 */
int ble_host_gap_scan_set_enable(bool enable, uint8_t filter);

/**
 *   @brief Enable scan using default parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if enable scan successfully, otherwise return error code.
 *
 *   @note This function called ble_host_gap_ext_scan_set_enable api, filter set BLE_HOST_GAP_SCAN_FILTER_ENABLE,
 */
int ble_host_gap_scan_enable(void);

/**
 *   @brief Disable scan.
 *
 *   @return BLE_HOST_ERR_SUCC if disable scan successfully, otherwise return error code.
 *
 *   @note This function called ble_host_gap_ext_scan_set_enable api, enable set false.
 */
int ble_host_gap_scan_disable(void);

/**
 *   @brief Set the BLE Scan filter for all GAP event dispatches.
 *
 *   @param[in] filter Pointer to the filter parameters.
 *
 *   @return None.
 */
void ble_host_gap_scan_set_filter(const struct ble_host_gap_scan_filter_param *filter);

/**
 *   @brief Clear the BLE Scan filter for all GAP event dispatches.
 *
 *   @return None.
 */
void ble_host_gap_scan_clear_filter(void);

/**
 *   @brief Get the BLE Scan filter.
 *
 *   @return Pointer to the filter parameters.
 */
struct ble_host_gap_scan_filter_param *ble_gap_scan_get_filter(void);
