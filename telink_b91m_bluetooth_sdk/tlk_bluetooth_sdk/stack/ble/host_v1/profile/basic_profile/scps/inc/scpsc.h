/********************************************************************************************************
 * @file    scpsc.h
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
// SCPSC: Scan Parameters Service Client.

enum ble_scpsc_event_id
{
    SCPSC_EVT_ID_SERVER_REQUIRES_REFRESH = 0x01, /** < refer to None */
};

typedef void (*ble_scpsc_event_callback)(uint16_t conn_handle, enum ble_scpsc_event_id event_id, const void *event_msg);

struct ble_scpsc_register_param
{
};

#define BLE_SCPSC_INITIAL_PARAMS NULL

/**
 *   @brief Register the scan parameters service client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the SCPSC module.
 *
 *   @return None.
 */
void ble_basic_register_ScPS_control_client(const struct ble_scpsc_register_param *param);

/**
 *   @brief Register the event callback function for the scan parameters service client control.
 *
 *   @param[in] event_callback Pointer to the callback function to be called when an event occurs.
 *
 *   @return None.
 */
void ble_basic_register_ScPS_client_event_callback(ble_scpsc_event_callback event_callback);

/**
 *    @brief Write the scan interval and window values to the server.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[in] scan_interval Scan interval value in units of 1.25ms.
 *    @param[in] scan_window Scan window value in units of 1.25ms.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if the server does not support the scan interval and window characteristics.
 *
 */
int ble_scpsc_write_scan_interval_window(uint16_t conn_handle, uint16_t scan_interval, uint16_t scan_window);
