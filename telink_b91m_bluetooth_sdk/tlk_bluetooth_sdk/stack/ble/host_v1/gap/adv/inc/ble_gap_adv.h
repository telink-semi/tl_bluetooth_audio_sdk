/********************************************************************************************************
 * @file    ble_gap_adv.h
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

#define BLE_HOST_GAP_ADV_FILTER_DISABLE                0x00
#define BLE_HOST_GAP_ADV_FILTER_ENABLE_SCAN            0x01
#define BLE_HOST_GAP_ADV_FILTER_ENABLE_CONN            0x02
#define BLE_HOST_GAP_ADV_FILTER_ENABLE_ALL             0x03

#define BLE_HOST_GAP_ADV_TYPE_ADV_IND                  0x00
#define BLE_HOST_GAP_ADV_TYPE_ADV_DIRECT_IND_HIGH_DUTY 0x01
#define BLE_HOST_GAP_ADV_TYPE_ADV_SCAN_IND             0x02
#define BLE_HOST_GAP_ADV_TYPE_ADV_NONCONN_IND          0x03
#define BLE_HOST_GAP_ADV_TYPE_ADV_DIRECT_IND_LOW_DUTY  0x04

struct ble_gap_adv_set_param
{
    uint16_t interval_min;   /** < Advertising Interval Min, unit 0.625ms */
    uint16_t interval_max;   /** < Advertising Interval Max, unit 0.625ms */
    uint8_t  adv_type;       /** < Advertising Type */
    uint8_t  peer_addr_type; /** < Peer Address Type */
    uint8_t  peer_addr[6];   /** < Peer Address */
    uint8_t  channel_map;    /** < Advertising Channel Map */
    uint8_t  filter_policy;  /** < Advertising Filter Policy */
};

#define BLE_HOST_GAP_ADV_SET_ADV_IND(min_interval, max_interval) \
    BLE_HOST_GAP_ADV_SET_PARAM(min_interval, max_interval, BLE_HOST_GAP_ADV_TYPE_ADV_IND, 0x7, BLE_HOST_GAP_ADV_FILTER_DISABLE)

#define BLE_HOST_GAP_ADV_SET_ADV_SCAN_IND(min_interval, max_interval) \
    BLE_HOST_GAP_ADV_SET_PARAM(min_interval, max_interval, BLE_HOST_GAP_ADV_TYPE_ADV_SCAN_IND, 0x7, BLE_HOST_GAP_ADV_FILTER_DISABLE)

#define BLE_HOST_GAP_ADV_SET_ADV_NONCONN_IND(min_interval, max_interval) \
    BLE_HOST_GAP_ADV_SET_PARAM(min_interval, max_interval, BLE_HOST_GAP_ADV_TYPE_ADV_NONCONN_IND, 0x7, BLE_HOST_GAP_ADV_FILTER_DISABLE)


/**
 * @brief Set advertising parameters
 *
 * This function sets the advertising parameters including interval, type,
 * channel map, and filter policy for the advertising set.
 *
 * @param[in] param    Pointer to the advertising parameter structure
 *
 * @return BLE_HOST_ERR_SUCC on success, error code otherwise
 */
int ble_host_gap_adv_set_param(const struct ble_gap_adv_set_param *param);

/**
 * @brief Set directed advertising parameters
 *
 * This function sets the parameters for directed advertising (high duty or
 * low duty cycle) to a specific peer device.
 *
 * @param[in] high_duty        true for high duty cycle, false for low duty cycle
 * @param[in] peer_addr_type   Peer address type (public or random)
 * @param[in] peer_addr        Peer device address (6 bytes)
 *
 * @return BLE_HOST_ERR_SUCC on success, error code otherwise
 */
int ble_host_gap_adv_set_direct_ind_param(bool high_duty, uint8_t peer_addr_type, const uint8_t peer_addr[6]);

/**
 * @brief Set advertising data
 *
 * This function sets the advertising data that will be transmitted in
 * advertising packets. The maximum length is 31 bytes.
 *
 * @param[in] adv_data         Pointer to the advertising data buffer
 * @param[in] adv_data_len     Length of the advertising data (max 31 bytes)
 *
 * @return BLE_HOST_ERR_SUCC on success, error code otherwise
 */
int ble_host_gap_adv_set_adv_data(const uint8_t *adv_data, uint8_t adv_data_len);

/**
 * @brief Set scan response data
 *
 * This function sets the scan response data that will be transmitted when
 * a scanner requests additional information. The maximum length is 31 bytes.
 *
 * @param[in] scan_rsp         Pointer to the scan response data buffer
 * @param[in] scan_rsp_len     Length of the scan response data (max 31 bytes)
 *
 * @return BLE_HOST_ERR_SUCC on success, error code otherwise
 */
int ble_host_gap_adv_set_scan_rsp_data(const uint8_t *scan_rsp, uint8_t scan_rsp_len);

/**
 * @brief Start advertising
 *
 * This function starts the advertising process using the previously configured
 * advertising parameters and data.
 *
 * @return BLE_HOST_ERR_SUCC on success, error code otherwise
 */
int ble_host_gap_adv_start(void);

/**
 * @brief Stop advertising
 *
 * This function stops the advertising process.
 *
 * @return BLE_HOST_ERR_SUCC on success, error code otherwise
 */
int ble_host_gap_adv_stop(void);

/**
 * @brief Set connectable advertising parameters with LTV data
 *
 * This function sets up connectable advertising (ADV_IND) with the specified
 * interval and packs the advertising data and scan response data from LTV
 * (Length-Type-Value) format arrays.
 *
 * @param[in] iterval_ms       Advertising interval in milliseconds
 * @param[in] adv_data         Pointer to array of LTV data structures for advertising data (can be NULL)
 * @param[in] scan_rsp_data    Pointer to array of LTV data structures for scan response (can be NULL)
 *
 * @return BLE_HOST_ERR_SUCC on success, error code otherwise
 */
int ble_host_gap_adv_set_adv_ind_param(uint16_t iterval_ms, const struct ltv_data **adv_data, const struct ltv_data **scan_rsp_data);


#define BLE_HOST_GAP_ADV_SET_PARAM(min_interval, max_interval, type, map, filter) \
    (&(struct ble_gap_adv_set_param){                                             \
        .interval_min   = (min_interval) / 0.625,                                 \
        .interval_max   = (max_interval) / 0.625,                                 \
        .adv_type       = (type),                                                 \
        .peer_addr_type = 0,                                                      \
        .peer_addr      = {0},                                                    \
        .channel_map    = (map),                                                  \
        .filter_policy  = (filter),                                               \
    })
