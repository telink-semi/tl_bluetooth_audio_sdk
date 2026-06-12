/********************************************************************************************************
 * @file    ble_gap_ext_adv.h
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
#define BLE_HOST_GAP_EXT_ADV_INVALID_ADV_HANDLE 0xFF

#define BLE_HOST_GAP_EXT_ADV_FILTER_DISABLE     0x00
#define BLE_HOST_GAP_EXT_ADV_FILTER_ENABLE_SCAN 0x01
#define BLE_HOST_GAP_EXT_ADV_FILTER_ENABLE_CONN 0x02
#define BLE_HOST_GAP_EXT_ADV_FILTER_ENABLE_ALL  0x03

enum ble_host_gap_ext_adv_type
{
    BLE_HOST_GAP_EXT_ADV_TYPE_ADV_IND                  = 0x13,
    BLE_HOST_GAP_EXT_ADV_TYPE_ADV_DIRECT_IND_LOW_DUTY  = 0x15,
    BLE_HOST_GAP_EXT_ADV_TYPE_ADV_DIRECT_IND_HIGH_DUTY = 0x1D,
    BLE_HOST_GAP_EXT_ADV_TYPE_ADV_SCAN_IND             = 0x12,
    BLE_HOST_GAP_EXT_ADV_TYPE_ADV_NONCONN_IND          = 0x10,

    BLE_HOST_GAP_EXT_ADV_TYPE_NON_CONNECTABLE_NON_SCANNABLE        = 0x00,
    BLE_HOST_GAP_EXT_ADV_TYPE_CONNECTABLE                          = 0x01,
    BLE_HOST_GAP_EXT_ADV_TYPE_SCANNABLE                            = 0x02,
    BLE_HOST_GAP_EXT_ADV_TYPE_NON_CONNECTABLE_NON_SCANNABLE_DIRECT = 0x04,
    BLE_HOST_GAP_EXT_ADV_TYPE_CONNECTABLE_DIRECT                   = 0x05,
    BLE_HOST_GAP_EXT_ADV_TYPE_SCANNABLE_DIRECT                     = 0x06,

    BLE_HOST_GAP_EXT_ADV_TYPE_ANONYMOUS        = 0x20,
    BLE_HOST_GAP_EXT_ADV_TYPE_INCLUDE_TX_POWER = 0x40,
};

struct ble_host_gap_extend_adv_set_param
{
    uint16_t type;                   /** < Advertising type, refer to enum ble_host_gap_ext_adv_type. */
    uint32_t interval_min;           /** < Primary Advertising Interval Min, unit 0.625ms, range 0x000020 to 0xFFFFFF. */
    uint32_t interval_max;           /** < Primary Advertising Interval Max, unit 0.625ms, range 0x000020 to 0xFFFFFF. */
    uint8_t  channel_map;            /** < Primary Advertising channel map. */
    uint8_t  peer_addr_type;         /** < Peer Address Type */
    uint8_t  peer_addr[6];           /** < Peer Address */
    uint8_t  filter_policy;          /** < Advertising Filter Policy */
    int8_t   tx_power;               /** < Advertising TX Power, range -127dBm to +20dBm */
    uint8_t  primary_phy;            /** < Primary advertising PHYs. 0x01 - LE 1M, 0x03 - LE Coded. */
    uint8_t  secondary_adv_max_skip; /** < Maximum number of advertising events to skip after the first one. */
    uint8_t  secondary_phy;          /** < Secondary advertising PHYs. 0x01 - LE 1M, 0x02 - LE 2M, 0x03 - LE Coded. */
    uint8_t  sid;                    /** < Value of the Advertising SID, range 0x00 to 0x0F. */
    uint8_t  scan_req_notify;        /** < Scan request notification enable. */
};

#define BLE_HOST_GAP_EXT_ADV_SET_PARAM(min_interval, max_interval, adv_type, map, filter, tx_power_value, pri_phy, max_skip, sec_phy, sid_value, enable) \
    (&(struct ble_host_gap_extend_adv_set_param){                                                                                                        \
        .type                   = adv_type,                                                                                                              \
        .interval_min           = (min_interval) / 0.625,                                                                                                \
        .interval_max           = (max_interval) / 0.625,                                                                                                \
        .channel_map            = map,                                                                                                                   \
        .peer_addr_type         = 0,                                                                                                                     \
        .peer_addr              = {0},                                                                                                                   \
        .filter_policy          = (filter),                                                                                                              \
        .tx_power               = (tx_power_value),                                                                                                      \
        .primary_phy            = (pri_phy),                                                                                                             \
        .secondary_adv_max_skip = (max_skip),                                                                                                            \
        .secondary_phy          = (sec_phy),                                                                                                             \
        .sid                    = (sid_value),                                                                                                           \
        .scan_req_notify        = (enable),                                                                                                              \
    })

#define BLE_HOST_GAP_EXT_ADV_SET_PARAM_LEGACY(min_interval, max_interval, adv_type, map, filter, tx_power_value) \
    BLE_HOST_GAP_EXT_ADV_SET_PARAM(min_interval, max_interval, adv_type, map, filter, tx_power_value, 0x01, 0, 0x01, 0, 0)

#define BLE_HOST_GAP_EXT_ADV_SET_ADV_IND(min_interval, max_interval) \
    BLE_HOST_GAP_EXT_ADV_SET_PARAM_LEGACY(min_interval, max_interval, BLE_HOST_GAP_EXT_ADV_TYPE_ADV_IND, 0x7, BLE_HOST_GAP_EXT_ADV_FILTER_DISABLE, 0x00)

#define BLE_HOST_GAP_EXT_ADV_SET_ADV_SCAN_IND(min_interval, max_interval) \
    BLE_HOST_GAP_EXT_ADV_SET_PARAM_LEGACY(min_interval, max_interval, BLE_HOST_GAP_EXT_ADV_TYPE_ADV_SCAN_IND, 0x7, BLE_HOST_GAP_EXT_ADV_FILTER_DISABLE, 0x00)

#define BLE_HOST_GAP_EXT_ADV_SET_ADV_NONCONN_IND(min_interval, max_interval) \
    BLE_HOST_GAP_EXT_ADV_SET_PARAM_LEGACY(min_interval, max_interval, BLE_HOST_GAP_EXT_ADV_TYPE_ADV_NONCONN_IND, 0x7, BLE_HOST_GAP_EXT_ADV_FILTER_DISABLE, 0x00)

#define BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_PARAM(min_interval, max_interval, map, tx_power_value, pri_phy, sec_phy, sid_value)                                            \
    BLE_HOST_GAP_EXT_ADV_SET_PARAM(min_interval, max_interval, BLE_HOST_GAP_EXT_ADV_TYPE_NON_CONNECTABLE_NON_SCANNABLE, map, BLE_HOST_GAP_EXT_ADV_FILTER_DISABLE, tx_power_value, \
                                   pri_phy, 0, sec_phy, sid_value, 0)

#define BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_1M_1M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_PARAM(min_interval, max_interval, 0x7, 0, 0x01, 0x01, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_1M_2M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_PARAM(min_interval, max_interval, 0x7, 0, 0x01, 0x02, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_1M_CODED(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_PARAM(min_interval, max_interval, 0x7, 0, 0x01, 0x03, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_CODED_1M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_PARAM(min_interval, max_interval, 0x7, 0, 0x03, 0x01, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_CODED_2M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_PARAM(min_interval, max_interval, 0x7, 0, 0x03, 0x02, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_CODED_CODED(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_PARAM(min_interval, max_interval, 0x7, 0, 0x03, 0x03, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN(min_interval, max_interval, sid_value) BLE_HOST_GAP_EXT_ADV_SET_NON_CONN_NON_SCAN_1M_2M(min_interval, max_interval, sid_value)


#define BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_PARAM(min_interval, max_interval, map, tx_power_value, pri_phy, sec_phy, sid_value)                                            \
    BLE_HOST_GAP_EXT_ADV_SET_PARAM(min_interval, max_interval, BLE_HOST_GAP_EXT_ADV_TYPE_CONNECTABLE, map, BLE_HOST_GAP_EXT_ADV_FILTER_DISABLE, tx_power_value, pri_phy, 0, \
                                   sec_phy, sid_value, 0)

#define BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_1M_1M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x01, 0x01, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_1M_2M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x01, 0x02, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_1M_CODED(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x01, 0x03, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_CODED_1M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x03, 0x01, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_CODED_2M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x03, 0x02, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_CODED_CODED(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x03, 0x03, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE(min_interval, max_interval, sid_value) BLE_HOST_GAP_EXT_ADV_SET_CONNECTABLE_1M_2M(min_interval, max_interval, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_PARAM(min_interval, max_interval, map, tx_power_value, pri_phy, sec_phy, sid_value)                                                     \
    BLE_HOST_GAP_EXT_ADV_SET_PARAM(min_interval, max_interval, BLE_HOST_GAP_EXT_ADV_TYPE_SCANNABLE, map, BLE_HOST_GAP_EXT_ADV_FILTER_DISABLE, tx_power_value, pri_phy, 0, sec_phy, \
                                   sid_value, 0)

#define BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_1M_1M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x01, 0x01, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_1M_2M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x01, 0x02, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_1M_CODED(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x01, 0x03, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_CODED_1M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x03, 0x01, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_CODED_2M(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x03, 0x02, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_CODED_CODED(min_interval, max_interval, sid_value) \
    BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_PARAM(min_interval, max_interval, 0x7, 0, 0x03, 0x03, sid_value)

#define BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE(min_interval, max_interval, sid_value) BLE_HOST_GAP_EXT_ADV_SET_SCANNABLE_1M_2M(min_interval, max_interval, sid_value)


/**
 *   @brief Initialize the extended advertising module.
 *
 *   @return None.
 */
void ble_host_gap_extend_adv_init(void);

/**
 *   @brief Create a new extended advertising set.
 *
 *   @return The handle of the new extended advertising set.
 *             - BLE_HOST_GAP_EXT_ADV_INVALID_ADV_HANDLE if the maximum number of advertising sets is reached.
 */
uint8_t ble_host_gap_extend_adv_create_set(void);

/**
 *   @brief Delete an existing extended advertising set.
 *
 *   @param[in] adv_handle The handle of the extended advertising set to delete.
 *
 *   @return True if the set is successfully deleted, False otherwise.
 */
bool ble_host_gap_extend_adv_delete_set(uint8_t adv_handle);

/**
 *   @brief Set the parameters of an existing extended advertising set.
 *
 *   @param[in] adv_handle The handle of the extended advertising set to set.
 *   @param[in] param The parameters to set.
 *
 *   @return BLE_HOST_ERR_SUCC if set extended advertising parameters successfully, otherwise an error code.
 */
int ble_host_gap_extend_adv_set_param(uint8_t adv_handle, struct ble_host_gap_extend_adv_set_param *param);

/**
 *   @brief Set the advertising data of an existing extended advertising set.
 *
 *   @param[in] adv_handle The handle of the extended advertising set to set.
 *   @param[in] data The advertising data to set.
 *   @param[in] len The length of the advertising data.
 *
 *   @return BLE_HOST_ERR_SUCC if set advertising data successfully, otherwise an error code.
 */
int ble_host_gap_extend_adv_set_advertising_data(uint8_t adv_handle, const uint8_t *data, uint8_t len);

/**
 *   @brief Set the scan response data of an existing extended advertising set.
 *
 *   @param[in] adv_handle The handle of the extended advertising set to set.
 *   @param[in] data The scan response data to set.
 *   @param[in] len The length of the scan response data.
 *
 *   @return BLE_HOST_ERR_SUCC if set scan response data successfully, otherwise an error code.
 */
int ble_host_gap_extend_adv_set_scan_response_data(uint8_t adv_handle, const uint8_t *data, uint8_t len);

/**
 *   @brief Start an existing extended advertising set.
 *
 *   @param[in] adv_handle The handle of the extended advertising set to start.
 *
 *   @return BLE_HOST_ERR_SUCC if start extended advertising successfully, otherwise an error code.
 */
int ble_host_gap_extend_adv_start(uint8_t adv_handle);

/**
 *   @brief Stop an existing extended advertising set.
 *
 *   @param[in] adv_handle The handle of the extended advertising set to stop.
 *
 *   @return BLE_HOST_ERR_SUCC if stop extended advertising successfully, otherwise an error code.
 */
int ble_host_gap_extend_adv_stop(uint8_t adv_handle);

/**
 *   @brief Set the random address of an existing extended advertising set.
 *
 *   @param[in] adv_handle The handle of the extended advertising set to set.
 *   @param[in] addr The random address to set.
 *
 *   @return BLE_HOST_ERR_SUCC if set random address successfully, otherwise an error code.
 */
int ble_host_gap_extend_adv_set_rand_addr(uint8_t adv_handle, const uint8_t *addr);
