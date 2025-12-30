/********************************************************************************************************
 * @file    ble_gap_cis_peripheral.h
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

/** < GAP Support Maximum CIS Peripheral Count */
#ifndef BLE_HOST_GAP_CIS_PERIPHERAL_SUPPORT_MAX_COUNT
#define BLE_HOST_GAP_CIS_PERIPHERAL_SUPPORT_MAX_COUNT 2
#endif

/** < Enumeration of GAP CIS Peripheral Event ID */
enum ble_host_gap_cis_peripheral_event_id
{
    BLE_HOST_GAP_CIS_PERIPHERAL_RECV_CENTRAL_REQUEST = 0,
    BLE_HOST_GAP_CIS_PERIPHERAL_ESTABLISHED,
    BLE_HOST_GAP_CIS_PERIPHERAL_CIS_DISCONNECTED,
};

/** < Structure of GAP CIS Peripheral receive Central Request Event Message */
struct ble_host_gap_cis_peripheral_evt_central_request
{
    uint16_t acl_conn_handle; /** < ACL Connection handle. */
    uint16_t cis_conn_handle; /** < CIS connection handle. */
    uint8_t  cig_id;          /** < CIG ID. */
    uint8_t  cis_id;          /** < CIS ID. */
};

/** < Structure of GAP CIS Peripheral established Event Message */
struct ble_host_gap_cis_peripheral_evt_established
{
    uint16_t cis_conn_handle; /** < CIS connection handle. */
    uint8_t  status;          /** < Status of CIS establishment. 0x00 - success, other - failure. */
    uint8_t  nse;             /** < Maximum number of subevents in each CIS. */
    uint16_t iso_interval;    /** < ISO interval. range 0x0004 to 0x0C80, unit 1.25ms. */
    uint32_t cig_sync_delay;  /** < CIG synchronization delay. range 0x0000EA to 0x7FFFFF, unit 1us. */
    uint32_t cis_sync_delay;  /** < CIS synchronization delay. range 0x0000EA to 0x7FFFFF, unit 1us. */

    struct transmit_param
    {
        uint8_t  phy;               /** < PHY. 0x01 - BLE 1M, 0x02 - BLE 2M, 0x03 - BLE Coded. */
        uint8_t  bn;                /** < The Burst Number. range 0x01 to 0x0F, 0x00 mean no isochronous data. */
        uint8_t  ft;                /** < The Flush Timeout. range 0x01 to 0xFF. */
        uint16_t max_pdu_size;      /** < Maximum PDU size. range 0x0000 to 0x00FB. */
        uint32_t transport_latency; /** < Transport latency. range 0x0000EA to 0x7FFFFF, unit 1us. */
    } c_to_p, p_to_c;
};

struct ble_host_gap_cis_peripheral_evt_recv_data
{
    uint16_t conn_handle; /** < CIS Connection handle. */
    uint16_t data_len;
    uint8_t *data;
};

/** < Structure of GAP CIS Peripheral CIS Disconnected Event Message */
struct ble_host_gap_cis_peripheral_evt_cis_disconnected
{
    uint16_t conn_handle; /** < CIS Connection handle. */
    uint8_t  reason;      /** < Reason of disconnection. */
};

/**
 *   @brief GAP Layer CIS Peripheral Event Callback.
 *
 *   @param[in] acl_conn_handle ACL Connection handle.
 *   @param[in] event_id BLE host GAP CIS Peripheral Event ID.
 *   @param[in] event_msg Event message.
 *
 *   @return None.
 */
typedef void (*ble_host_gap_cis_peripheral_event_callback)(uint16_t acl_conn_handle, enum ble_host_gap_cis_peripheral_event_id event_id, const void *event_msg);

/**
 *   @brief GAP Layer Initialize CIS Peripheral.
 *
 *   @return None.
 */
void ble_host_gap_cis_peripheral_init(void);

/**
 *   @brief GAP Layer Accept CIS Request.
 *
 *   @param[in] cis_conn_handle CIS Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_cis_peripheral_accept(uint16_t cis_conn_handle);

/**
 *   @brief GAP Layer Reject CIS Request, with reason is BT_ERR_REMOTE_USER_TERMINATED_CONNECTION.
 *
 *   @param[in] cis_conn_handle CIS Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_cis_peripheral_reject(uint16_t cis_conn_handle);

/**
 *   @brief GAP Layer Register CIS Peripheral Event Handler.
 *
 *   @param[in] acl_conn_handle ACL Connection handle.
 *   @param[in] cig_id CIG ID.
 *   @param[in] cis_id CIS ID.
 *   @param[in] callback Event callback function.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_cis_peripheral_register_event_handler(uint16_t acl_conn_handle, uint16_t cig_id, uint16_t cis_id, ble_host_gap_cis_peripheral_event_callback callback);

/**
 *   @brief GAP Layer Reject CIS Request.
 *
 *   @param[in] cis_conn_handle CIS Connection handle.
 *   @param[in] reason Reason for rejecting the CIS request, value define in enum bluetooth_controller_error_code.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_host_gap_cis_peripheral_reject_with_reason(uint16_t cis_conn_handle, uint8_t reason);
