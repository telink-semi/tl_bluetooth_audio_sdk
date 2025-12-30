/********************************************************************************************************
 * @file    ble_gap_evt_dispatch.h
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

enum ble_host_gap_evt_type
{
    BLE_HOST_GAP_EVT_LE_ACL,
    BLE_HOST_GAP_EVT_ENCRYPT,
    BLE_HOST_GAP_EVT_LE_SCAN,
    BLE_HOST_GAP_EVT_VENDOR_EVENT,
};

enum ble_host_gap_evt_id
{
    BLE_HOST_GAP_EVT_LE_ACL_EVENT_START = 0x100,
    BLE_HOST_GAP_EVT_LE_CONNECT,                /** < event data refer to struct ble_host_gap_evt_le_connect */
    BLE_HOST_GAP_EVT_LE_DISCONNECT,             /** < event data refer to struct ble_host_gap_evt_le_disconnect */
    BLE_HOST_GAP_EVT_LE_CONNECT_UPDATE,         /** < event data refer to struct ble_host_gap_evt_le_connect_update */
    BLE_HOST_GAP_EVT_LE_REMOTE_FEATURES_UPDATE, /** < event data refer to struct ble_host_gap_evt_le_remote_features */
    BLE_HOST_GAP_EVT_LE_DATA_LEN_CHANGE,        /** < event data refer to struct ble_host_gap_evt_le_data_len_change */
    BLE_HOST_GAP_EVT_LE_PHY_UPDATE,             /** < event data refer to struct ble_host_gap_evt_le_phy_update */
    BLE_HOST_GAP_EVT_ENCRYPT_EVENT_START = 0x200,
    BLE_HOST_GAP_EVT_LE_ENCRYPT_CHANGE, /** < event data refer to struct ble_host_gap_evt_le_encrypt_change */
    BLE_HOST_GAP_EVT_LE_SCAN_EVENT_START = 0x300,
    BLE_HOST_GAP_EVT_LE_SCAN_REPORT_UNDIRECTED, /** < event data refer to struct ble_host_gap_evt_scan_adv_report_undirected */
    BLE_HOST_GAP_EVT_LE_SCAN_REPORT_DIRECTED,   /** < event data refer to struct ble_host_gap_evt_scan_adv_report_directed */
    BLE_HOST_GAP_EVT_VENDOR_EVENT_START = 0x400,
    BLE_HOST_GAP_EVT_VENDOR_EVENT_REPORT,
};

/** < data structure for BLE_HOST_GAP_EVT_LE_CONNECT */
struct ble_host_gap_evt_le_connect
{
    uint16_t conn_handle;    /** < ACL connection handle, range 0x0000 to 0x0EFF */
    uint8_t  role;           /** < 0x00 is central, 0x01 is peripheral */
    uint8_t  peer_addr_type; /** < peer address type */
    uint8_t  peer_addr[6];   /** < peer address */
    // RPA only used for HCI Enhanced Connection Complete event.
    uint8_t  local_rpa[6];           /** < local resolvable private address */
    uint8_t  peer_rpa[6];            /** < peer resolvable private address */
    uint16_t conn_interval;          /** < connection interval, range: 0x0006 to 0x0C80, in units of 1.25ms */
    uint16_t conn_latency;           /** < connection latency, range: 0x0000 to 0x01F3 */
    uint16_t supervision_timeout;    /** < supervision timeout, range: 0x000A to 0x0C80, in units of 10ms */
    uint8_t  central_clock_accuracy; /** < central clock accuracy */
};

/** < data structure for BLE_HOST_GAP_EVT_LE_DISCONNECT */
struct ble_host_gap_evt_le_disconnect
{
    uint16_t conn_handle; /** < ACL connection handle, range 0x0000 to 0x0EFF */
    uint8_t  reason;      /** < reason for disconnection */
};

/** < data structure for BLE_HOST_GAP_EVT_LE_CONNECT_UPDATE */
struct ble_host_gap_evt_le_connect_update
{
    uint16_t conn_handle;         /** < ACL connection handle, range 0x0000 to 0x0EFF */
    uint16_t conn_interval;       /** < connection interval, range: 0x0006 to 0x0C80, in units of 1.25ms */
    uint16_t conn_latency;        /** < connection latency, range: 0x0000 to 0x01F3 */
    uint16_t supervision_timeout; /** < supervision timeout, range: 0x000A to 0x0C80, in units of 10ms */
};

/** < Data structure for BLE_HOST_GAP_EVT_LE_REMOTE_FEATURES_UPDATE */
struct ble_host_gap_evt_le_remote_features
{
    uint16_t conn_handle; /** < ACL connection handle, range 0x0000 to 0x0EFF */
    uint64_t features;    /** < remote features */
};

/** < Data structure for BLE_HOST_GAP_EVT_LE_DATA_LEN_CHANGE */
struct ble_host_gap_evt_le_data_len_change
{
    uint16_t conn_handle; /** < ACL connection handle, range 0x0000 to 0x0EFF */
    uint16_t tx_octets;   /** < maximum number of payload octets in a LLData PDU can send */
    uint16_t tx_time;     /** < maximum time that the local Controller will take to send a LL packet */
    uint16_t rx_octets;   /** < maximum number of payload octets in a LLData PDU can receive */
    uint16_t rx_time;     /** < maximum time that the local Controller will take to receive a LL packet */
};

/** < Data structure for BLE_HOST_GAP_EVT_LE_PHY_UPDATE */
struct ble_host_gap_evt_le_phy_update
{
    uint16_t conn_handle; /** < ACL connection handle, range 0x0000 to 0x0EFF */
    uint8_t  tx_phy;      /** < transmitter PHY. 0x01: LE 1M, 0x02: LE 2M, 0x03: LE Coded */
    uint8_t  rx_phy;      /** < Receiver PHY. 0x01: LE 1M, 0x02: LE 2M, 0x03: LE Coded */
};

/** < Data structure for BLE_HOST_GAP_EVT_LE_ENCRYPT_CHANGE */
struct ble_host_gap_evt_le_encrypt_change
{
    uint16_t conn_handle; /** < ACL connection handle, range 0x0000 to 0x0EFF */
    uint8_t  status;      /** < encryption status. */
    uint8_t  enable;      /** < encryption Enabled,  0x00: No Encryption, 0x01: Encryption on LE */
};

struct ble_host_gap_evt_scan_adv_report_undirected
{
    bool           legacy;      /** < If true, controller received ADV_IND, otherwise */
    bool           connectable; /** < The device is connectable. */
    uint8_t        addr_type;
    uint8_t        addr[6];
    uint8_t        sid;         /** < 0xFF, No ADI field provided. */
    int8_t         tx_power;    /** < 0x7F, TX power information not available. */
    int8_t         rssi;        /** < 0x7F, RSSI information not available. */
    uint16_t       pa_interval; /** < 0x0000, No periodic advertising. */
    uint16_t       data_len;
    const uint8_t *data;
};

struct ble_host_gap_evt_scan_adv_report_directed
{
    bool           legacy;      /** < If true, controller received ADV_IND, otherwise */
    bool           connectable; /** < The device is connectable. */
    uint8_t        addr_type;
    uint8_t        addr[6];
    uint8_t        direct_addr_type;
    uint8_t        direct_addr[6];
    uint8_t        sid;         /** < 0xFF, No ADI field provided. */
    int8_t         tx_power;    /** < 0x7F, TX power information not available. */
    int8_t         rssi;        /** < 0x7F, RSSI information not available. */
    uint16_t       pa_interval; /** < 0x0000, No periodic advertising. */
    uint16_t       data_len;
    const uint8_t *data;
};

/** < data structure for BLE_HOST_GAP_EVT_LE_ACL_DISCONNECT */
typedef void (*ble_host_gap_evt_handler)(uint32_t event_id, const void *data, void *user_data);

struct ble_host_gap_evt_subscribe_param
{
    union
    {
        struct
        {
            uint32_t gap_le_acl_mask     : 1;
            uint32_t gap_le_encrypt_mask : 1;
            uint32_t gap_le_scan_mask    : 1;
            uint32_t gap_vendor_mask     : 1;
        };

        uint32_t all_masks;
    };

    ble_host_gap_evt_handler handler;
    void                    *user_data;
};

struct ble_host_gap_evt_subscribe
{
    struct ble_host_gap_evt_subscribe             *next_handler;
    const struct ble_host_gap_evt_subscribe_param *param;
};

/**
 * @brief       Add a subscriber to the BLE host GAP event subscription list.
 * @param[in]   sub     - pointer to the subscriber structure.
 * @return      none.
 */
void ble_host_gap_evt_add_subscriber(struct ble_host_gap_evt_subscribe *sub);

/**
 * @brief       Remove a subscriber from the BLE host GAP event subscription list.
 * @param[in]   sub     - pointer to the subscriber structure.
 * @return      none.
 */
void ble_host_gap_evt_remove_subscriber(struct ble_host_gap_evt_subscribe *sub);
