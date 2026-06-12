/********************************************************************************************************
 * @file    ble_gap_acl.h
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
/** < GAP ACL connection update parameters. */
struct ble_host_gap_acl_connection_param
{
    uint16_t conn_interval_min;   /** < Minimum connection interval, range 0x0006 to 0x0C80, unit 1.25ms. */
    uint16_t conn_interval_max;   /** < Maximum connection interval, range 0x0006 to 0x0C80, unit 1.25ms. */
    uint16_t conn_latency;        /** < Connection latency, range 0x0000 to 0x01F3. */
    uint16_t supervision_timeout; /** < Supervision timeout, range 0x000A to 0x0C80, unit 10ms. */
    uint16_t min_ce_length;       /** < Minimum length of connection event, range 0x0000 to 0xFFFF, unit 0.625ms. */
    uint16_t max_ce_length;       /** < Maximum length of connection event, range 0x0000 to 0xFFFF, unit 0.625ms. */
    /** < Callback function to be called when the procedure is updated. interval unit is 1.25ms, timeout unit is 10ms. */
    void (*callback)(uint16_t conn_handle, uint16_t interval, uint16_t latency, uint16_t timeout);
};

/** < Update connection parameters with default parameters. */
#define BLE_HOST_GAP_ACL_UPDATE_CONN_PARAMS(conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, supervision_timeout_ms, cb) \
    BLE_HOST_GAP_ACL_UPDATE_CONN_PARAMS_ALL(conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, supervision_timeout_ms, 0, 0, cb)

/**
 *   @brief Read channel map of ACL connection.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[out] channel_map Channel map of ACL connection.
 *
 *   @return BLE_HOST_ERR_SUCC if read success, otherwise error code.
 */
int ble_host_gap_acl_read_channel_map(uint16_t conn_handle, uint64_t *p_channel_map);

/**
 *   @brief Update connection parameters of ACL connection(only for central role).
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] param Connection parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if update success, otherwise error code.
 */
int ble_host_gap_acl_connection_update_central(uint16_t conn_handle, const struct ble_host_gap_acl_connection_param *param);

/**
 *   @brief Feature exchange procedure of ACL connection.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] callback Callback function to be called when the procedure is updated.
 *
 *   @return BLE_HOST_ERR_SUCC if exchange success, otherwise error code.
 *
 *   @note If the connection is peripheral, need controller support peripheral initiate feature exchange.
 */
int ble_host_gap_acl_features_exchange(uint16_t conn_handle, void (*callback)(uint16_t conn_handle, uint64_t features));

/**
 *   @brief Version exchange procedure of ACL connection.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] callback Callback function to be called when the procedure is updated.
 *
 *   @return BLE_HOST_ERR_SUCC if exchange success, otherwise error code.
 */
int ble_host_gap_acl_version_exchange(uint16_t conn_handle, void (*callback)(uint16_t conn_handle, uint8_t version, uint16_t company_id, uint16_t subversion));

/**
 *   @brief Data length update procedure of ACL connection.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] tx_octets ACL max TX octets, range 0x001B to 0x00FB.
 *
 *   @return BLE_HOST_ERR_SUCC if update success, otherwise error code.
 */
int ble_host_gap_acl_data_length_update(uint16_t conn_handle, uint16_t tx_octets);

/**
 *   @brief PHY update procedure of ACL connection.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] tx_phy TX PHY, BLE_HOST_ACL_PHY_LE_1M, BLE_HOST_ACL_PHY_LE_2M or BLE_HOST_ACL_PHY_LE_CODED.
 *   @param[in] rx_phy RX PHY, BLE_HOST_ACL_PHY_LE_1M, BLE_HOST_ACL_PHY_LE_2M or BLE_HOST_ACL_PHY_LE_CODED.
 *   @param[in] callback Callback function to be called when the procedure is updated.
 *
 *   @return BLE_HOST_ERR_SUCC if update success, otherwise error code.
 */
int ble_host_gap_acl_phy_update(uint16_t conn_handle, uint8_t tx_phy, uint8_t rx_phy, void (*callback)(uint16_t conn_handle, uint8_t tx_phy, uint8_t rx_phy));

/**
 *   @brief Disconnect ACL connection.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if disconnect success, otherwise error code.
 */
int ble_host_gap_disconnect_acl(uint16_t conn_handle);

#define BLE_HOST_GAP_ACL_UPDATE_CONN_PARAMS_ALL(conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, cb) \
    (&(const struct ble_host_gap_acl_connection_param){.conn_interval_min   = conn_interval_min_ms / 1.25,                                                                      \
                                                       .conn_interval_max   = conn_interval_max_ms / 1.25,                                                                      \
                                                       .conn_latency        = conn_latency_count,                                                                               \
                                                       .supervision_timeout = supervision_timeout_ms / 10,                                                                      \
                                                       .min_ce_length       = min_ce_length_ms / 0.625,                                                                         \
                                                       .max_ce_length       = max_ce_length_ms / 0.625,                                                                         \
                                                       .callback            = cb})
