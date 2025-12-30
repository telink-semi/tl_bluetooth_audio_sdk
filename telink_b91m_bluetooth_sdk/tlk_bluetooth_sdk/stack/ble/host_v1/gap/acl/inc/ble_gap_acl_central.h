/********************************************************************************************************
 * @file    ble_gap_acl_central.h
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

/** < Filter policy for create connection command. */
#define BLE_HOST_GAP_ACL_CENTRAL_INITIATOR_FILTER_POLICY_DISABLE     0x00
#define BLE_HOST_GAP_ACL_CENTRAL_INITIATOR_FILTER_POLICY_ACCEPT_LIST 0x01

/**
 *   @brief Create Connection Callback function type.
 *
 *   @param[in] result  True if create connection successfully completes, otherwise false.
 *
 *   This function is called when create connection successfully completes or timeout occurs.
 */
typedef void (*ble_host_gap_acl_central_create_callback)(bool result);

/**
 *   @brief Cancel Connection Callback function type.
 *
 *   This function is called when cancel connection successfully completes.
 */
typedef void (*ble_host_gap_acl_central_cancel_callback)(void);

/** < Create a ACL Connection(by legacy create connection command) with default parameters. */
#define BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS_HIGH(addr_type, addr, cb)     BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS_DEFAULT(addr_type, addr, 8, 15, cb)

#define BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS_BALANCED(addr_type, addr, cb) BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS_DEFAULT(addr_type, addr, 30, 50, cb)

#define BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS_LOW(addr_type, addr, cb)      BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS_DEFAULT(addr_type, addr, 100, 125, cb)

/** < Structure create connection by legacy command parameters. */
struct ble_host_gap_acl_central_create_conn_params
{
    uint16_t scan_interval;  /** < Scan interval, range 0x0004 to 0x4000, unit 0.625ms. */
    uint16_t scan_window;    /** < Scan window, range 0x0004 to 0x4000, unit 0.625ms. */
    uint8_t  filter_policy;  /** < 0x00 - no filter, 0x01 - Accept List. */
    uint8_t  peer_addr_type; /** < Peer address type. */
    uint8_t  peer_addr[6];   /** < Peer address. */

    uint16_t conn_interval_min;   /** < Minimum connection interval, range 0x0006 to 0x0C80, unit 1.25ms. */
    uint16_t conn_interval_max;   /** < Maximum connection interval, range 0x0006 to 0x0C80, unit 1.25ms. */
    uint16_t conn_latency;        /** < Connection latency, range 0x0000 to 0x01F3. */
    uint16_t supervision_timeout; /** < Supervision timeout, range 0x000A to 0x0C80, unit 10ms. */
    uint16_t min_ce_length;       /** < Minimum length of connection event, range 0x0000 to 0xFFFF, unit 0.625ms. */
    uint16_t max_ce_length;       /** < Maximum length of connection event, range 0x0000 to 0xFFFF, unit 0.625ms. */

    uint16_t                                 create_timeout; /** < Create connection timeout, unit 1ms. range 200 */
    ble_host_gap_acl_central_create_callback callback;       // callback function to be called when create connection timeout occurs
};

/** < Create a ACL Connection(by extended create connection command) with default parameters. */
#define BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_HIGH(addr_type, addr, cb)     BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_DEFAULT(addr_type, addr, 8, 15, cb)

#define BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_BALANCED(addr_type, addr, cb) BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_DEFAULT(addr_type, addr, 30, 40, cb)

#define BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_LOW(addr_type, addr, cb)      BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_DEFAULT(addr_type, addr, 100, 125, cb)

#define BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_WITH_FILTER(addr_type, conn_interval_min_ms, conn_interval_max_ms, cb) \
    (&(struct ble_host_gap_acl_central_ext_create_conn_params){                                                                \
        .filter_policy   = BLE_HOST_GAP_ACL_CENTRAL_INITIATOR_FILTER_POLICY_ACCEPT_LIST,                                       \
        .peer_addr_type  = addr_type,                                                                                          \
        .peer_addr       = {0, 0, 0, 0, 0, 0},                                                                                 \
        .initiating_phys = 0x03,                                                                                               \
        .conn_params[0] =                                                                                                      \
            {                                                                                                                  \
                .scan_interval       = 60 / 0.625,                                                                             \
                .scan_window         = 60 / 0.625,                                                                             \
                .conn_interval_min   = conn_interval_min_ms / 1.25,                                                            \
                .conn_interval_max   = conn_interval_max_ms / 1.25,                                                            \
                .conn_latency        = 0x0000,                                                                                 \
                .supervision_timeout = 5000 / 10,                                                                              \
                .min_ce_length       = 0x0000 / 0.625,                                                                         \
                .max_ce_length       = 0x0000 / 0.625,                                                                         \
            },                                                                                                                 \
        .conn_params[1] =                                                                                                      \
            {                                                                                                                  \
                .scan_interval       = 60 / 0.625,                                                                             \
                .scan_window         = 60 / 0.625,                                                                             \
                .conn_interval_min   = conn_interval_min_ms / 1.25,                                                            \
                .conn_interval_max   = conn_interval_max_ms / 1.25,                                                            \
                .conn_latency        = 0x0000,                                                                                 \
                .supervision_timeout = 5000 / 10,                                                                              \
                .min_ce_length       = 0x0000 / 0.625,                                                                         \
                .max_ce_length       = 0x0000 / 0.625,                                                                         \
            },                                                                                                                 \
        .create_timeout = 10000,                                                                                               \
        .callback       = cb,                                                                                                  \
    })

/** < Structure create connection by extended command parameters. */
struct ble_host_gap_acl_central_ext_create_conn_params
{
    uint8_t filter_policy;   /** < 0x00 - no filter, 0x01 - Accept List. */
    uint8_t peer_addr_type;  /** < Peer address type. */
    uint8_t peer_addr[6];    /** < Peer address. */
    uint8_t initiating_phys; /** < Initiating PHYs, bit0 scan 1M phy, bit2 scan coded phy, bit2 connect parameter 2M phy. */

    struct
    {
        uint16_t scan_interval;       /** < Scan interval, range 0x0004 to 0xFFFF, unit 0.625ms. */
        uint16_t scan_window;         /** < Scan window, range 0x0004 to 0xFFFF, unit 0.625ms. */
        uint16_t conn_interval_min;   /** < Minimum connection interval, range 0x0006 to 0x0C80, unit 1.25ms. */
        uint16_t conn_interval_max;   /** < Maximum connection interval, range 0x0006 to 0x0C80, unit 1.25ms. */
        uint16_t conn_latency;        /** < Connection latency, range 0x0000 to 0x01F3. */
        uint16_t supervision_timeout; /** < Supervision timeout, range 0x000A to 0x0C80, unit 10ms. */
        uint16_t min_ce_length;       /** < Minimum length of connection event, range 0x0000 to 0xFFFF, unit 0.625ms. */
        uint16_t max_ce_length;       /** < Maximum length of connection event, range 0x0000 to 0xFFFF, unit 0.625ms. */
    } conn_params[3];

    uint16_t                                 create_timeout; /** < Create connection timeout, unit 1ms. range 200 */
    ble_host_gap_acl_central_create_callback callback;       // callback function to be called when create connection timeout occurs
};

/**
 *   @brief Create an ACL connection by legacy create connection command.
 *
 *   @param[in] params Pointer to the structure containing the connection parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if create connection command is sent successfully, otherwise error code.
 */
int ble_host_gap_acl_central_create_connection(const struct ble_host_gap_acl_central_create_conn_params *params);

/**
 *   @brief Create an ACL connection by extended create connection command.
 *
 *   @param[in] params Pointer to the structure containing the connection parameters.
 *
 *   @return BLE_HOST_ERR_SUCC if extended create connection command is sent successfully, otherwise error code.
 */
int ble_host_gap_acl_central_ext_create_connection(const struct ble_host_gap_acl_central_ext_create_conn_params *params);

/**
 *   @brief Cancel an ACL connection.
 *
 *   @param[in] callback Pointer to the callback function to be called when cancel connection command is completed.
 *
 *   @return BLE_HOST_ERR_SUCC if cancel connection command is sent successfully, otherwise error code.
 */
int ble_host_gap_acl_central_cancel_create_connection(ble_host_gap_acl_central_cancel_callback callback);

#define BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS(addr_type, addr, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, \
                                                        supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, create_timeout_ms, cb)                                 \
    (&(const struct ble_host_gap_acl_central_create_conn_params){                                                                                                          \
        .scan_interval       = scan_interval_ms / 0.625,                                                                                                                   \
        .scan_window         = scan_window_ms / 0.625,                                                                                                                     \
        .filter_policy       = BLE_HOST_GAP_ACL_CENTRAL_INITIATOR_FILTER_POLICY_DISABLE,                                                                                   \
        .peer_addr_type      = addr_type,                                                                                                                                  \
        .peer_addr           = {addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]},                                                                                     \
        .conn_interval_min   = conn_interval_min_ms / 1.25,                                                                                                                \
        .conn_interval_max   = conn_interval_max_ms / 1.25,                                                                                                                \
        .conn_latency        = conn_latency_count,                                                                                                                         \
        .supervision_timeout = supervision_timeout_ms / 10,                                                                                                                \
        .min_ce_length       = min_ce_length_ms / 0.625,                                                                                                                   \
        .max_ce_length       = max_ce_length_ms / 0.625,                                                                                                                   \
        .create_timeout      = create_timeout_ms,                                                                                                                          \
        .callback            = cb,                                                                                                                                         \
    })

#define BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS_DEFAULT(addr_type, addr, conn_interval_min_ms, conn_interval_max_ms, cb) \
    BLE_HOST_GAP_ACL_CENTRAL_SET_CREATE_CONN_PARAMS(addr_type, addr, 50, 50, conn_interval_min_ms, conn_interval_max_ms, 0x0000, 5000, 0x0000, 0x0000, 10000, cb)


#define BLE_HOST_GAP_ACL_CENTRAL_SET_EXT_CREATE_CONN_PARAMS(                                                                                                                      \
    filter, addr_type, addr, phys, scan_interval_ms_1, scan_window_ms_1, conn_interval_min_ms_1, conn_interval_max_ms_1, conn_latency_count_1, supervision_timeout_ms_1,          \
    min_ce_length_ms_1, max_ce_length_ms_1, scan_interval_ms_2, scan_window_ms_2, conn_interval_min_ms_2, conn_interval_max_ms_2, conn_latency_count_2, supervision_timeout_ms_2, \
    min_ce_length_ms_2, max_ce_length_ms_2, scan_interval_ms_3, scan_window_ms_3, conn_interval_min_ms_3, conn_interval_max_ms_3, conn_latency_count_3, supervision_timeout_ms_3, \
    min_ce_length_ms_3, max_ce_length_ms_3, create_timeout_ms, cb)                                                                                                                \
    (&(struct ble_host_gap_acl_central_ext_create_conn_params){                                                                                                                   \
        .filter_policy   = filter,                                                                                                                                                \
        .peer_addr_type  = addr_type,                                                                                                                                             \
        .peer_addr       = {addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]},                                                                                                \
        .initiating_phys = phys,                                                                                                                                                  \
        .conn_params[0] =                                                                                                                                                         \
            {                                                                                                                                                                     \
                .scan_interval       = scan_interval_ms_1 / 0.625,                                                                                                                \
                .scan_window         = scan_window_ms_1 / 0.625,                                                                                                                  \
                .conn_interval_min   = conn_interval_min_ms_1 / 1.25,                                                                                                             \
                .conn_interval_max   = conn_interval_max_ms_1 / 1.25,                                                                                                             \
                .conn_latency        = conn_latency_count_1,                                                                                                                      \
                .supervision_timeout = supervision_timeout_ms_1 / 10,                                                                                                             \
                .min_ce_length       = min_ce_length_ms_1 / 0.625,                                                                                                                \
                .max_ce_length       = max_ce_length_ms_1 / 0.625,                                                                                                                \
            },                                                                                                                                                                    \
        .conn_params[1] =                                                                                                                                                         \
            {                                                                                                                                                                     \
                .scan_interval       = scan_interval_ms_2 / 0.625,                                                                                                                \
                .scan_window         = scan_window_ms_2 / 0.625,                                                                                                                  \
                .conn_interval_min   = conn_interval_min_ms_2 / 1.25,                                                                                                             \
                .conn_interval_max   = conn_interval_max_ms_2 / 1.25,                                                                                                             \
                .conn_latency        = conn_latency_count_2,                                                                                                                      \
                .supervision_timeout = supervision_timeout_ms_2 / 10,                                                                                                             \
                .min_ce_length       = min_ce_length_ms_2 / 0.625,                                                                                                                \
                .max_ce_length       = max_ce_length_ms_2 / 0.625,                                                                                                                \
            },                                                                                                                                                                    \
        .conn_params[2] =                                                                                                                                                         \
            {                                                                                                                                                                     \
                .scan_interval       = scan_interval_ms_3 / 0.625,                                                                                                                \
                .scan_window         = scan_window_ms_3 / 0.625,                                                                                                                  \
                .conn_interval_min   = conn_interval_min_ms_3 / 1.25,                                                                                                             \
                .conn_interval_max   = conn_interval_max_ms_3 / 1.25,                                                                                                             \
                .conn_latency        = conn_latency_count_3,                                                                                                                      \
                .supervision_timeout = supervision_timeout_ms_3 / 10,                                                                                                             \
                .min_ce_length       = min_ce_length_ms_3 / 0.625,                                                                                                                \
                .max_ce_length       = max_ce_length_ms_3 / 0.625,                                                                                                                \
            },                                                                                                                                                                    \
        .create_timeout = create_timeout_ms,                                                                                                                                      \
        .callback       = cb,                                                                                                                                                     \
    })


#define BLE_HOST_GAP_ACL_CENTRAL_SET_EXT_CREATE_CONN_PARAMS_1(filter, addr_type, addr, phys, scan_interval_ms_1, scan_window_ms_1, conn_interval_min_ms_1, conn_interval_max_ms_1, \
                                                              conn_latency_count_1, supervision_timeout_ms_1, min_ce_length_ms_1, max_ce_length_ms_1, create_timeout_ms, cb)       \
    (&(struct ble_host_gap_acl_central_ext_create_conn_params){                                                                                                                    \
        .filter_policy   = filter,                                                                                                                                                 \
        .peer_addr_type  = addr_type,                                                                                                                                              \
        .peer_addr       = {addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]},                                                                                                 \
        .initiating_phys = phys,                                                                                                                                                   \
        .conn_params[0] =                                                                                                                                                          \
            {                                                                                                                                                                      \
                .scan_interval       = scan_interval_ms_1 / 0.625,                                                                                                                 \
                .scan_window         = scan_window_ms_1 / 0.625,                                                                                                                   \
                .conn_interval_min   = conn_interval_min_ms_1 / 1.25,                                                                                                              \
                .conn_interval_max   = conn_interval_max_ms_1 / 1.25,                                                                                                              \
                .conn_latency        = conn_latency_count_1,                                                                                                                       \
                .supervision_timeout = supervision_timeout_ms_1 / 10,                                                                                                              \
                .min_ce_length       = min_ce_length_ms_1 / 0.625,                                                                                                                 \
                .max_ce_length       = max_ce_length_ms_1 / 0.625,                                                                                                                 \
            },                                                                                                                                                                     \
        .create_timeout = create_timeout_ms,                                                                                                                                       \
        .callback       = cb,                                                                                                                                                      \
    })

#define BLE_HOST_GAP_ACL_CENTRAL_SET_EXT_CREATE_CONN_PARAMS_2(filter, addr_type, addr, phys, scan_interval_ms_1, scan_window_ms_1, conn_interval_min_ms_1, conn_interval_max_ms_1, \
                                                              conn_latency_count_1, supervision_timeout_ms_1, min_ce_length_ms_1, max_ce_length_ms_1, scan_interval_ms_2,          \
                                                              scan_window_ms_2, conn_interval_min_ms_2, conn_interval_max_ms_2, conn_latency_count_2, supervision_timeout_ms_2,    \
                                                              min_ce_length_ms_2, max_ce_length_ms_2, create_timeout_ms, cb)                                                       \
    (&(struct ble_host_gap_acl_central_ext_create_conn_params){                                                                                                                    \
        .filter_policy   = filter,                                                                                                                                                 \
        .peer_addr_type  = addr_type,                                                                                                                                              \
        .peer_addr       = {addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]},                                                                                                 \
        .initiating_phys = phys,                                                                                                                                                   \
        .conn_params[0] =                                                                                                                                                          \
            {                                                                                                                                                                      \
                .scan_interval       = scan_interval_ms_1 / 0.625,                                                                                                                 \
                .scan_window         = scan_window_ms_1 / 0.625,                                                                                                                   \
                .conn_interval_min   = conn_interval_min_ms_1 / 1.25,                                                                                                              \
                .conn_interval_max   = conn_interval_max_ms_1 / 1.25,                                                                                                              \
                .conn_latency        = conn_latency_count_1,                                                                                                                       \
                .supervision_timeout = supervision_timeout_ms_1 / 10,                                                                                                              \
                .min_ce_length       = min_ce_length_ms_1 / 0.625,                                                                                                                 \
                .max_ce_length       = max_ce_length_ms_1 / 0.625,                                                                                                                 \
            },                                                                                                                                                                     \
        .conn_params[1] =                                                                                                                                                          \
            {                                                                                                                                                                      \
                .scan_interval       = scan_interval_ms_2 / 0.625,                                                                                                                 \
                .scan_window         = scan_window_ms_2 / 0.625,                                                                                                                   \
                .conn_interval_min   = conn_interval_min_ms_2 / 1.25,                                                                                                              \
                .conn_interval_max   = conn_interval_max_ms_2 / 1.25,                                                                                                              \
                .conn_latency        = conn_latency_count_2,                                                                                                                       \
                .supervision_timeout = supervision_timeout_ms_2 / 10,                                                                                                              \
                .min_ce_length       = min_ce_length_ms_2 / 0.625,                                                                                                                 \
                .max_ce_length       = max_ce_length_ms_2 / 0.625,                                                                                                                 \
            },                                                                                                                                                                     \
        .create_timeout = create_timeout_ms,                                                                                                                                       \
        .callback       = cb,                                                                                                                                                      \
    })

#define BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_1M_PHY(addr_type, addr, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, \
                                                               supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, create_timeout_ms, cb)                                 \
    BLE_HOST_GAP_ACL_CENTRAL_SET_EXT_CREATE_CONN_PARAMS_1(BLE_HOST_GAP_ACL_CENTRAL_INITIATOR_FILTER_POLICY_DISABLE, addr_type, addr, 0x01, scan_interval_ms, scan_window_ms,      \
                                                          conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, supervision_timeout_ms, min_ce_length_ms,               \
                                                          max_ce_length_ms, create_timeout_ms, cb)

#define BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_CODED_PHY(addr_type, addr, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms,             \
                                                                  conn_latency_count, supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, create_timeout_ms, cb)     \
    BLE_HOST_GAP_ACL_CENTRAL_SET_EXT_CREATE_CONN_PARAMS_1(BLE_HOST_GAP_ACL_CENTRAL_INITIATOR_FILTER_POLICY_DISABLE, addr_type, addr, 0x04, scan_interval_ms, scan_window_ms, \
                                                          conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, supervision_timeout_ms, min_ce_length_ms,          \
                                                          max_ce_length_ms, create_timeout_ms, cb)

#define BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_1M_2M_PHY(addr_type, addr, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms,              \
                                                                  conn_latency_count, supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, create_timeout_ms, cb)      \
    BLE_HOST_GAP_ACL_CENTRAL_SET_EXT_CREATE_CONN_PARAMS_2(BLE_HOST_GAP_ACL_CENTRAL_INITIATOR_FILTER_POLICY_DISABLE, addr_type, addr, 0x03, scan_interval_ms, scan_window_ms,  \
                                                          conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, supervision_timeout_ms, min_ce_length_ms,           \
                                                          max_ce_length_ms, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, \
                                                          supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, create_timeout_ms, cb)

#define BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_2M_CODED_PHY(addr_type, addr, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms,           \
                                                                     conn_latency_count, supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, create_timeout_ms, cb)   \
    BLE_HOST_GAP_ACL_CENTRAL_SET_EXT_CREATE_CONN_PARAMS_2(BLE_HOST_GAP_ACL_CENTRAL_INITIATOR_FILTER_POLICY_DISABLE, addr_type, addr, 0x06, scan_interval_ms, scan_window_ms,  \
                                                          conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, supervision_timeout_ms, min_ce_length_ms,           \
                                                          max_ce_length_ms, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms, conn_latency_count, \
                                                          supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, create_timeout_ms, cb)

#define BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_1M_2M_CODED_PHY(addr_type, addr, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms,         \
                                                                        conn_latency_count, supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, create_timeout_ms, cb) \
    BLE_HOST_GAP_ACL_CENTRAL_SET_EXT_CREATE_CONN_PARAMS(                                                                                                                       \
        BLE_HOST_GAP_ACL_CENTRAL_INITIATOR_FILTER_POLICY_DISABLE, addr_type, addr, 0x07, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms,         \
        conn_latency_count, supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms,          \
        conn_latency_count, supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, scan_interval_ms, scan_window_ms, conn_interval_min_ms, conn_interval_max_ms,          \
        conn_latency_count, supervision_timeout_ms, min_ce_length_ms, max_ce_length_ms, create_timeout_ms, cb)

#define BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_DEFAULT(addr_type, addr, conn_interval_min_ms, conn_interval_max_ms, cb) \
    BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_1M_2M_PHY(addr_type, addr, 60, 60, conn_interval_min_ms, conn_interval_max_ms, 0x0000, 5000, 0x0000, 0x0000, 10000, cb)
