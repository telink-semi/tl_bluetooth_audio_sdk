/********************************************************************************************************
 * @file    ble_host.h
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
 * @brief Bluetooth Host Error Code
 * @defgroup bt_host_err Bluetooth Host Error Code
 *
 * Defines error codes returned by Bluetooth host. If error comes from specific
 * component (eg ATTC, GAP, GATT, SDP or SMP) it is shifted by base allowing to
 * identify component.
 * @{
 */
#define BLE_ERR_CODE_VALUE(err)  ((err) & 0xFF)
#define BLE_CHECK_HCI_ERR(err)   ((err) & BLE_HOST_ERR_HCI_HEAD)
#define BLE_CHECK_GAP_ERR(err)   ((err) & BLE_HOST_ERR_GAP_HEAD)
#define BLE_CHECK_GATT_ERR(err)  ((err) & BLE_HOST_ERR_GATT_HEAD)
#define BLE_CHECK_L2CAP_ERR(err) ((err) & BLE_HOST_ERR_L2CAP_HEAD)
#define BLE_CHECK_PRF_ERR(err)   ((err) & BLE_HOST_ERR_PRF_HEAD)

#define BLE_HCI_ERR(err)         ((err) | BLE_HOST_ERR_HCI_HEAD)
#define BLE_GAP_ERR(err)         ((err) | BLE_HOST_ERR_GAP_HEAD)
#define BLE_GATT_ERR(err)        ((err) | BLE_HOST_ERR_GATT_HEAD)
#define BLE_L2CAP_ERR(err)       ((err) | BLE_HOST_ERR_L2CAP_HEAD)
#define BLE_PRF_ERR(err)         ((err) | BLE_HOST_ERR_PRF_HEAD)

enum ble_host_err_code
{
    BLE_HOST_ERR_SUCC          = 0,      /** Success */
    BLE_HOST_ERR_INVALID_PARAM = 0x0100, /** Invalid parameter */
    BLE_HOST_ERR_INVALID_CONN_HANDLE,    /** Invalid acl connection handle */
    BLE_HOST_ERR_ACL_NOT_ENCRYPTED,      /** Acl connection not encrypted */
    BLE_HOST_ERR_NULL_POINTER,           /** Null pointer */
    BLE_HOST_ERR_INSUFFICIENT_RESOURCES, /** Insufficient resources */
    BLE_HOST_ERR_BUSY,                   /** Busy */
    BLE_HOST_ERR_NOMEM,                  /** Out of memory */
    BLE_HOST_ERR_NOTSUP,                 /** Command not supported */
    BLE_HOST_ERR_UNKNOWN,                /** Unknown error */
    BLE_HOST_ERR_BADDATA,                /** Invalid data */
    BLE_HOST_ERR_RETRY,                  /** Command can be retried */
    BLE_HOST_ERR_PARAM,                  /** Invalid parameter */
    BLE_HOST_ERR_NOADDR,                 /** No address available */

    BLE_HOST_ERR_HOST_HEAD  = 0x200, /** Host Error code, refer to enum ble_host_error_code */
    BLE_HOST_ERR_HCI_HEAD   = 0x300, /** HCI Error code, refer to enum ble_host_hci_error_code */
    BLE_HOST_ERR_GAP_HEAD   = 0x400, /** GAP Error code, refer to enum ble_host_gap_error_code */
    BLE_HOST_ERR_GATT_HEAD  = 0x500, /** GATT Error code, refer to enum ble_host_gatt_error_code */
    BLE_HOST_ERR_L2CAP_HEAD = 0x600, /** L2CAP Error code, refer to enum ble_host_l2cap_error_code */
    BLE_HOST_ERR_PRF_HEAD   = 0x700, /** Profile Error code, refer to enum ble_host_prf_error_code */
};

/**
 *   @brief Define the ACL Role in BLE Host.
 */
#define BLE_HOST_ACL_ROLE_CENTRAL    0x00
#define BLE_HOST_ACL_ROLE_PERIPHERAL 0x01

/**
 *   @brief Define the BLE Address Type.
 */
#define BLE_HOST_BLE_ADDR_TYPE_PUBLIC         0x00
#define BLE_HOST_BLE_ADDR_TYPE_RANDOM         0x01

#define BLE_HOST_BLE_ADDR_NON_PRIVATE(addr)   (((addr)[5] & 0xC0) == 0x00)
#define BLE_HOST_BLE_ADDR_PRIVATE(addr)       (((addr)[5] & 0xC0) == 0x40)
#define BLE_HOST_BLE_ADDR_RANDOM_RFU(addr)    (((addr)[5] & 0xC0) == 0x80)
#define BLE_HOST_BLE_ADDR_STATIC_RANDOM(addr) (((addr)[5] & 0xC0) == 0xC0)

/** Structure representing a BLE address. */
struct ble_addr
{
    /** The type of the address. */
    uint8_t type;

    /** The value of the address as an array of 6 bytes. */
    uint8_t val[6];
};

#define BLE_ADDR_EMPTY (&(struct ble_addr){0, {0, 0, 0, 0, 0, 0}})

// can not modify this value, it is used in the stack.
enum ble_host_user_data_id
{
    BLE_HOST_ACL_DATA_TX_USER_ID,
    BLE_HOST_L2CAP_USER_ID,
    BLE_HOST_GATT_USER_ID,
    BLE_HOST_APP_DATA1_USER_ID,
    BLE_HOST_APP_DATA2_USER_ID,
    BLE_HOST_USER_DATA_ID_MAX,
};

struct ble_host_conn
{
    uint16_t conn_handle;            //Connection handle
    uint8_t  role;                   //Connected role, 0x00 is central, 0x01 is peripheral
    uint8_t  central_clock_accuracy; //MCA: 0x00: 500ppm, 0x01: 250ppm, 0x02: 150ppm, 0x03: 100ppm 0x04: 75ppm, 0x05: 50ppm, 0x06: 30ppm, 0x07: 20ppm
    uint16_t conn_interval;          //connection interval , range 0x0006 to 0x0C80, unit 1.25ms
    uint16_t conn_latency;           //connection latency , range 0x0000 to 0x01F3
    uint16_t supervision_timeout;    //supervision timeout, range 0x000A to 0x0C80, unit 10ms
    uint8_t  acl_pending_count;      //acl current pending count, waiting controller reply packet count.

    /** < Remote Base Information. */
    uint8_t  tx_phy;     /** < Transmit PHY. 0x01: LE 1M, 0x02: LE 2M, 0x03: LE Coded. */
    uint8_t  rx_phy;     /** < Receive PHY. 0x01: LE 1M, 0x02: LE 2M, 0x03: LE Coded. */
    uint8_t  version;    /** < Bluetooth version. */
    uint16_t company_id; /** < Company Identifier. */
    uint16_t subversion; /** < LE subversion. */
    uint64_t features;   /** < LE features. */
    uint16_t tx_octets;  /** < Max Tx octets. */
    uint16_t tx_time;    /** < Max Tx time. */
    uint16_t rx_octets;  /** < Max Rx octets. */
    uint16_t rx_time;    /** < Max Rx time. */

    /** < Connection state callback. */
    uint8_t conn_state_flag; /** < refer ble gap acl connection state callback flag. */
    uint8_t enc_flag;        /** < Encryption flag. */
    uint8_t enc_key_size;    /** < Encryption key size. */
    uint8_t pairing_mode;    /** < Pairing mode, refer to enum ble_host_smp_pair_method. */
    void   *conn_state_cb;

    /** < User data. */
    void *user_data[BLE_HOST_USER_DATA_ID_MAX];

    /** < Current encryption information. */
    uint32_t smp_index;   /** <index of the SMP context used by this connection, If not bonding or not paired, it is 0x00000000*/
    uint8_t  enc_ltk[16]; /** < encryption long term key. */

    /* own and peer address, id, ota address */
    struct ble_addr own_ota_addr;  //own address over the air, updated after connection complete event
    struct ble_addr peer_ota_addr; //peer address over the air, updated after connection complete event
};

struct ble_host_acl_conn_callbacks
{
    void (*connected)(struct ble_host_conn *conn);
    void (*disconnected)(struct ble_host_conn *conn, uint8_t reason);
};

/**
 *   @brief this function is used to initialize the BLE Host stack.
 *
 *   @return none.
 */
void ble_host_init(void);

/***************  */

/**
 *   @brief this function is used to find a connection by connection handle.
 *
 *   @param[in] conn_handle: connection handle.
 *
 *   @return pointer to the connection structure, or NULL if not found.
 */
struct ble_host_conn *ble_host_conn_find_by_conn_handle(uint16_t conn_handle);

/**
 *   @brief this function is used to check connection handle is connected or not.
 *
 *   @param[in] conn_handle: connection handle.
 *
 *   @return true if connected, false otherwise.
 */
bool ble_host_conn_is_connected(uint16_t conn_handle);

/**
 *   @brief this function is used to check connection handle is peripheral or not.
 *
 *   @param[in] conn_handle: connection handle.
 *
 *   @return true if peripheral, false otherwise.
 */
bool ble_host_conn_is_peripheral(uint16_t conn_handle);

/**
 *   @brief this function is used to check connection handle is central or not.
 *
 *   @param[in] conn_handle: connection handle.
 *
 *   @return true if central, false otherwise.
 */
bool ble_host_conn_is_central(uint16_t conn_handle);

/**
 *   @brief this function is used to register the callbacks for ACL connection events(BLE Host).
 *
 *   @param[in] id: Host ACL connection user data ID.
 *   @param[in] cb: pointer to the callback structure.
 *
 *   @return none.
 */
void ble_host_acl_conn_register_user_data(enum ble_host_user_data_id id, const struct ble_host_acl_conn_callbacks *cb);

/**
 *   @brief this function is used to register the callbacks for ACL connection events(BLE Host) by index.
 *
 *   @param[in] id: Host ACL connection user data ID.
 *
 *   @return none.
 */
void ble_host_acl_conn_unregister_user_data(enum ble_host_user_data_id id);

/**
 *   @brief Get the controller BD address.
 *
 *   @return pointer to the BD address.
 */
uint8_t *ble_host_hci_get_bd_addr(void);

/**
 *    @brief Set the controller BD address.
 *
 *    @param[in] addr: pointer to the BD address.
 *
 *    @note It is not recommended for use as it may affect the value of SMP pairing.
 */
void ble_host_hci_set_bd_addr(const uint8_t addr[6]);

/**
 *    @brief Get the host address.
 *
 *    @return pointer to the host address.
 *
 *    @note If host address is public return controller BD address, otherwise return host address, ble_host_gap_set_random_addr set random address.
 */
uint8_t *ble_host_hci_get_host_addr(void);

/**
 *    @brief Get the address type and address.
 *
 *    @param[out] addr_type: pointer to the address type, BLE_HOST_BLE_ADDR_TYPE_PUBLIC or BLE_HOST_BLE_ADDR_TYPE_RANDOM.
 *    @param[out] addr: pointer to the address.
 *
 *    @return none.
 *
 *    @note If host address is public return controller BD address, otherwise return host address, ble_host_gap_set_random_addr set random address.
 */
void ble_host_hci_get_address(uint8_t *addr_type, uint8_t addr[6]);

/**
 *    @note These functions indicate changes in the ACL connect status, allowing the application layer to overload.
 *            Default functions are output loggings.
 */
// void ble_host_acl_conn_connected(struct ble_host_conn *conn);
// void ble_host_acl_conn_disconnected(uint16_t conn_handle, uint8_t reason);
/**
 *   @brief this function is used to update the connection parameter, host receive LE Connection Update Complete event.
 *
 *   @param[in] conn_handle: connection handle.
 *   @param[in] conn_interval: connection interval, uint 1.25ms.
 *   @param[in] conn_latency: connection latency, uint acl connection interval.
 *   @param[in] supervision_timeout: supervision timeout, uint 10ms.
 */
// void ble_host_acl_conn_parameter_update(uint16_t conn_handle, uint16_t conn_interval, uint16_t conn_latency, uint16_t supervision_timeout);
/**
 *   @brief this function is used to update the data length, host receive LE Data Length Change event.
 *
 *   @param[in] conn_handle: connection handle.
 *   @param[in] tx_octets: max tx octets, LLData PDU.
 *   @param[in] tx_time: max tx time.
 *   @param[in] rx_octets: max rx octets, LLData PDU.
 *   @param[in] rx_time: max rx time.
 */
// void ble_host_acl_data_length_update(uint16_t conn_handle, uint16_t tx_octets, uint16_t tx_time, uint16_t rx_octets, uint16_t rx_time);
/**
 *   @brief this function is used to update the phy, host receive LE PHY Update Complete event.
 *
 *   @param[in] conn_handle: connection handle.
 *   @param[in] tx_phy: transmit phy, 0x01: LE 1M, 0x02: LE 2M, 0x03: LE Coded.
 *   @param[in] rx_phy: receive phy, 0x01: LE 1M, 0x02: LE 2M, 0x03: LE Coded.
 */
// void ble_host_acl_phy_update(uint16_t conn_handle, uint8_t tx_phy, uint8_t rx_phy);
/**
 *   @brief Host receive LE Remote Connection Parameter Request event, if controller support LE Connection Parameter Request Procedure.
 *
 *   @param[in] conn_handle: connection handle.
 *   @param[in] min_interval: min connection interval, uint 1.25ms.
 *   @param[in] max_interval: max connection interval, uint 1.25ms.
 *   @param[in] latency: connection latency, uint acl connection interval.
 *   @param[in] timeout: supervision timeout, uint 10ms.
 *
 *   @note Application layer should call ble_host_hci_le_remote_connection_parameter_request_reply or ble_host_hci_le_remote_connection_parameter_request_negative_reply
 *           to reply or reject the request base current connection parameter.
 *          Host default reply any request with the same parameter.
 */
// void ble_host_acl_receive_conn_param_req(uint16_t conn_handle, uint16_t min_interval, uint16_t max_interval, uint16_t latency, uint16_t timeout);
/**
 *    @brief this function is used to update the channel selection algorithm, host receive LE Channel Selection Algorithm event.
 *
 *    @param[in] conn_handle: connection handle.
 *    @param[in] csa: channel selection algorithm, 0x00: LE CSA#1 used, 0x01: LE CSA#2 used.
 */
// void ble_host_acl_channel_selection_algorithm_update(uint16_t conn_handle, uint8_t csa);

/**
 *   @brief this function is used to get the connection phy.
 *
 *   @param[in] conn_handle: connection handle.
 *   @param[out] tx_phy: pointer to the transmit phy.
 *   @param[out] rx_phy: pointer to the receive phy.
 *
 *   @return true if success, false otherwise.
 */
bool ble_host_conn_get_phy(uint16_t conn_handle, uint8_t *tx_phy, uint8_t *rx_phy);

/**
 * @brief       Get the SMP pairing index for an ACL connection.
 * @param[in]   conn_handle    - the connection handle.
 * @return      the SMP pairing index, or 0 if the connection is not found or not paired.
 */
uint32_t ble_host_acl_conn_get_smp_index(uint16_t conn_handle);

/**
 * @brief       Get the Long Term Key (LTK) for an ACL connection.
 * @param[in]   conn_handle    - the connection handle.
 * @param[out]  ltk             - pointer to the buffer to store the 16-byte LTK.
 * @return      BLE_HOST_ERR_SUCC if successful, BLE_HOST_ERR_INVALID_CONN_HANDLE if connection not found, BLE_HOST_ERR_ACL_NOT_ENCRYPTED if not encrypted.
 */
int ble_host_acl_conn_get_ltk(uint16_t conn_handle, uint8_t ltk[16]);

/**
 * @brief       Get the own address type.
 * @return      the own address type, BLE_HOST_BLE_ADDR_TYPE_PUBLIC or BLE_HOST_BLE_ADDR_TYPE_RANDOM.
 */
uint8_t ble_host_get_own_addr_type(void);

/**
 * @brief       Check if a pairing index exists in any active ACL connection.
 * @param[in]   pairing_index   - the pairing index to check.
 * @return      true if the pairing index exists, false otherwise.
 */
bool ble_host_acl_conn_check_pairing_index(uint32_t pairing_index);

/**
 * @brief       Get the HCI version.
 * @return      the HCI version.
 */
uint8_t ble_host_hci_get_hci_version(void);

/**
 * @brief       Check if the controller supports LE.
 * @return      true if LE is supported, false otherwise.
 */
bool ble_host_hci_get_le_supported_controller(void);

/**
 * @brief       Check if the controller supports specific LE features.
 * @param[in]   features    - the LE features to check (bitmask).
 * @return      true if all specified features are supported, false otherwise.
 */
bool ble_host_hci_check_le_features_support(uint64_t features);

/**
 * @brief       Get the LE features supported by the controller.
 * @return      the LE features bitmask.
 */
uint64_t ble_host_hci_get_le_features(void);

/**
 * @brief       Set the HCI supported commands.
 * @param[in]   sup_cmd     - the supported commands bitmask.
 * @return      none.
 */
void ble_host_hci_set_hci_supported_cmd(uint32_t sup_cmd);

/**
 * @brief       Read the controller basic information including version, features, and BD address.
 * @return      none.
 */
void ble_host_hci_read_controller_basic_info(void);

/**
 * @brief       Set the BD address for Telink Bluetooth Audio Source Controller.
 * @param[in]   mac         - pointer to the 6-byte BD address.
 * @return      none.
 * @note        This function is only for Telink Bluetooth Audio Source Controller.
 */
void ble_host_set_bd_addr_telink(const uint8_t *mac);
