/********************************************************************************************************
 * @file    ble_gap_cis.h
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

#ifndef BLE_HOST_GAP_CIS_SUPPORT_MAX_COUNT
#define BLE_HOST_GAP_CIS_SUPPORT_MAX_COUNT 4
#endif

enum ble_host_gap_cis_event
{
    BLE_HOST_GAP_CIS_EVT_ID_ESTABLISHED,        /** < event parameter: struct ble_host_gap_cis_established_event */
    BLE_HOST_GAP_CIS_EVT_ID_ESTABLISHED_FAILED, /** < event parameter: uint8_t reason */
    BLE_HOST_GAP_CIS_EVT_ID_TERMINATED,
};

struct ble_host_gap_cis_established_event
{
    uint32_t cig_sync_delay; /** < CIG sync delay in units of 1us */
    uint32_t cis_sync_delay; /** < CIS sync delay in units of 1us */
    uint16_t iso_interval;   /** < ISO interval in units of 1.25ms */
};

/**
 * @brief       BLE host GAP CIS event callback function pointer type.
 * @param[in]   acl_conn_handle    - the ACL connection handle.
 * @param[in]   cis_conn_handle     - the CIS connection handle.
 * @param[in]   event               - the CIS event type.
 * @param[in]   event_param         - the event parameter pointer.
 * @return      none.
 */
typedef void (*ble_host_gap_cis_event_callback)(uint16_t acl_conn_handle, uint16_t cis_conn_handle, enum ble_host_gap_cis_event event, const void *event_param);

/**
 * @brief       Initialize the BLE host GAP CIS module.
 * @return      none.
 */
void ble_host_gap_cis_init(void);

/**
 * @brief       Register a CIS connection for peripheral role.
 * @param[in]   acl_conn_handle    - the ACL connection handle.
 * @param[in]   cig_id              - the CIG ID.
 * @param[in]   cis_id              - the CIS ID.
 * @param[in]   event_callback      - the event callback function.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_gap_cis_register_peripheral(uint16_t acl_conn_handle, uint8_t cig_id, uint8_t cis_id, ble_host_gap_cis_event_callback event_callback);

/**
 * @brief       Register a CIS connection for central role.
 * @param[in]   cis_conn_handle    - the CIS connection handle.
 * @param[in]   cig_id              - the CIG ID.
 * @param[in]   cis_id              - the CIS ID.
 * @param[in]   event_callback      - the event callback function.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_gap_cis_register_central(uint16_t cis_conn_handle, uint8_t cig_id, uint8_t cis_id, ble_host_gap_cis_event_callback event_callback);

/**
 * @brief       Remove CIS connections for central role with the specified CIG ID.
 * @param[in]   cig_id      - the CIG ID.
 * @return      none.
 */
void ble_host_gap_cis_remove_central(uint8_t cig_id);

/**
 * @brief       Update the ACL connection handle for a CIS connection in central role.
 * @param[in]   cis_conn_handle    - the CIS connection handle.
 * @param[in]   acl_conn_handle     - the ACL connection handle.
 * @return      none.
 */
void ble_host_gap_cis_update_central_acl_conn_handle(uint16_t cis_conn_handle, uint16_t acl_conn_handle);

/**
 * @brief       Disconnect a CIS connection.
 * @param[in]   cis_conn_handle    - the CIS connection handle to disconnect.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_gap_cis_disconnect(uint16_t cis_conn_handle);
