/********************************************************************************************************
 * @file    ble_l2cap.h
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
 *  @brief  Definition for L2CAP CID value for the LE-U
 */
enum ble_l2cap_channel_id
{
    LE_L2CAP_CID_ATT       = 0x0004,
    LE_L2CAP_CID_SIGNALING = 0x0005,
    LE_L2CAP_CID_SMP       = 0x0006,
};

enum ble_l2cap_event
{
    L2CAP_EVT_ACL_CONNECTED,    /** < param is NULL */
    L2CAP_EVT_ACL_DISCONNECTED, /** < param is disconnected reason, uint8_t */
};

enum ble_host_l2cap_error_code
{
    BLE_L2CAP_SUCCESS = 0,
    BLE_L2CAP_ERR_INVALID_CONN_HANDLE,
    BLE_L2CAP_ERR_INVALID_PARAMS,
    BLE_L2CAP_ERR_INSUFFICIENT_RESOURCES,
    BLE_L2CAP_ERR_ATT_INVALID_CONN_HANDLE_OR_CID,
    BLE_L2CAP_ERR_ATT_MTU_EXCEEDED,
    BLE_L2CAP_ERR_ATT_INVALID_HANDLE,
    BLE_L2CAP_ERR_ATT_INVALID_UUID,
    BLE_L2CAP_ERR_ATT_NTF_PENDING_FULL,
    BLE_L2CAP_ERR_SMP_INVALID_MAX_KEY_SIZE,
    BLE_L2CAP_ERR_SMP_INVALID_KEY_DISTRIBUTION,
    BLE_L2CAP_ERR_SMP_NOT_SUPPORTED,
    BLE_L2CAP_ERR_SMP_BUSY,
    BLE_L2CAP_ERR_SMP_STATE_INVALID,
    BLE_L2CAP_ERR_SMP_ENCRYPTED,
    BLE_L2CAP_ERR_SMP_NON_STORE,
    BLE_L2CAP_ERR_SMP_NVS_FAILED,
    BLE_L2CAP_ERR_SMP_INVALID_MAX_BOUND_NUM,
    BLE_L2CAP_ERR_SMP_ALREADY_INITIALIZED,
};

typedef void (*l2cap_ctrl_callback_t)(struct ble_host_conn *conn, uint8_t event, const void *param);
typedef void (*l2cap_data_callback_t)(struct ble_host_conn *conn, uint16_t len, uint8_t *p_packet);

typedef void (*l2cap_cid_ctrl_callback_t)(struct ble_host_conn *conn, uint16_t cid, uint8_t event, const void *param);
typedef void (*l2cap_cid_data_callback_t)(struct ble_host_conn *conn, uint16_t cid, uint16_t len, uint8_t *p_packet);

struct ble_l2cap_pdu_format
{
    uint16_t pdu_length;      /** < PDU Length*/
    uint16_t cid;             /** < Channel ID */
    uint8_t  info_payload[0]; /** < Information payload range 0 to 65535 octets */
};

typedef void (*ble_l2cap_tx_complete_cb)(uint16_t conn_handle, void *arg, uint16_t status);

struct ble_host_l2cap_tx_packet
{
    uint16_t                 channel_id;
    uint16_t                 data_length;
    const uint8_t           *p_data; // pointer to the data buffer, global memory.
    ble_l2cap_tx_complete_cb tx_complete_cb;
    void                    *cb_arg;
};

/**
 *   @brief this function is used to initialize the L2CAP module.
 *
 *   @return none.
 */
void ble_host_l2cap_init(void);

/**
 *   @brief this function is used to register ATT, SMP, signaling and callbacks.
 *
 *   @param[in] cid: channel ID, LE_L2CAP_CID_ATT, LE_L2CAP_CID_SIGNALING, LE_L2CAP_CID_SMP.
 *   @param[in] ctrl_callback: control callback function.
 *   @param[in] data_callback: data callback function.
 *
 *   @return none.
 */
void ble_host_l2cap_register_callbacks(uint8_t cid, l2cap_ctrl_callback_t ctrl_callback, l2cap_data_callback_t data_callback);

/**
 *   @brief this function is used to register CoC data callback.
 *
 *   @param[in] ctrl_callback: control callback function.
 *   @param[in] data_callback: data callback function.
 *
 *   @return none.
 */
void ble_host_l2cap_register_cid_data_callback(l2cap_cid_ctrl_callback_t ctrl_callback, l2cap_cid_data_callback_t data_callback);

/**
 *   @brief this function is used to send L2CAP data.
 *
 *   @param[in] conn: pointer to the connection object.
 *   @param[in] tx_packet: pointer to the L2CAP TX packet.
 *
 *   @return BLE_L2CAP_ERR_INVALID_PARAMS if the input parameter is invalid.
 *           BLE_L2CAP_ERR_INSUFFICIENT_RESOURCES if there is no memory to allocate the TX node.
 *           BLE_HOST_ERR_SUCC if the data is sent successfully.
 */
int ble_host_l2cap_send_l2cap_data(struct ble_host_conn *conn, struct ble_host_l2cap_tx_packet *tx_packet);

/**
 *   @brief this function is used to send L2CAP data by connection handle.
 *
 *   @param[in] conn_handle: connection handle.
 *   @param[in] tx_packet: pointer to the L2CAP TX packet.
 *
 *   @return BLE_L2CAP_ERR_INVALID_CONN_HANDLE if the conn_handle is invalid.
 *           BLE_HOST_ERR_SUCC if the data is sent successfully.
 */
int ble_host_l2cap_send_l2cap_data_by_conn_handle(uint16_t conn_handle, struct ble_host_l2cap_tx_packet *tx_packet);

/**
 *   @brief this function is used to send L2CAP data synchronously.
 *
 *   @param[in] conn: pointer to the connection object.
 *   @param[in] tx_packet: pointer to the L2CAP TX packet.
 *
 *   @return BLE_L2CAP_ERR_INVALID_PARAMS if the input parameter is invalid.
 *           BLE_L2CAP_ERR_INSUFFICIENT_RESOURCES if there is no memory to allocate the TX node.
 *           BLE_HOST_ERR_SUCC if the data is sent successfully.
 */
int ble_host_l2cap_send_l2cap_data_sync(struct ble_host_conn *conn, struct ble_host_l2cap_tx_packet *tx_packet);

/**
 *   @brief this function is used to send L2CAP data by connection handle.
 *
 *   @param[in] conn_handle: connection handle.
 *   @param[in] tx_packet: pointer to the L2CAP TX packet.
 *
 *   @return BLE_L2CAP_ERR_INVALID_CONN_HANDLE if the conn_handle is invalid.
 *           BLE_HOST_ERR_SUCC if the data is sent successfully.
 */
int ble_host_l2cap_send_l2cap_data_sync_by_conn_handle(uint16_t conn_handle, struct ble_host_l2cap_tx_packet *tx_packet);
