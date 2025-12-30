/********************************************************************************************************
 * @file    scpss.h
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

// ScPSS: Scan Parameters Service Server.

enum ble_scpss_event_id
{
    ScPSS_EVENT_ID_RECV_SCAN_INTERVAL_WINDOW, /** < message refer to @ref struct ble_scps_scan_interval_window_param. */
};

typedef void (*ble_scpss_event_callback)(uint16_t conn_handle, enum ble_scpss_event_id event_id, const void *event_msg);

/**
 *   @brief Register the scan parameters service server control.
 *
 *   @note This function should be called before any other function in the ScPSS module.
 *
 *   @return None.
 */
void ble_basic_register_ScPS_control_server();

/**
 *   @brief Set the event callback for the Scan Parameters Service server control.
 *
 *   @param[in] callback Pointer to the callback function.
 *
 *   @return None.
 *
 */
void ble_basic_register_ScPS_event_callback(ble_scpss_event_callback callback);

/**
 *   @brief Update the server requires refresh.
 *
 *   @param[in] conn_handle Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 */
int ble_scpss_update_server_requires_refresh(uint16_t conn_handle);
