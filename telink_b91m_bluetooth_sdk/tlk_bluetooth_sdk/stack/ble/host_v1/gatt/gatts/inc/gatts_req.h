/********************************************************************************************************
 * @file    gatts_req.h
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
 *   @brief  Send a notification message to a client.
 *
 *   @param[in]   conn_handle  ACL Connection handle.
 *   @param[in]   attr_handle  Attribute handle.
 *   @param[in]   value        Pointer to the value to be notified.
 *   @param[in]   len          Length of the value to be notified.
 *
 *   @return BLE_HOST_ERR_SUCC if the handle value notification is sent successfully.
 */
int ble_gatts_notify(uint16_t conn_handle, uint16_t attr_handle, const uint8_t *value, uint16_t len);

typedef void (*gatts_notify_callback)(uint16_t conn_handle, bool success);
/**
 *   @brief  Send a notification message to a client with a callback.
 *
 *   @param[in]   conn_handle  ACL Connection handle.
 *   @param[in]   attr_handle  Attribute handle.
 *   @param[in]   value        Pointer to the value to be notified.
 *   @param[in]   len          Length of the value to be notified.
 *   @param[in]   callback     Callback function to be called when the notification is sent.
 *
 *   @return BLE_HOST_ERR_SUCC if the notification is sent successfully.
 */
int ble_gatts_notify_with_callback(uint16_t conn_handle, uint16_t attr_handle, const uint8_t *value, uint16_t len, gatts_notify_callback callback);

typedef void (*gatts_confirm_callback)(uint16_t conn_handle);
/**
 *   @brief  Send an indication message to a client with a callback.
 *
 *   @param[in]   conn_handle  ACL Connection handle.
 *   @param[in]   attr_handle  Attribute handle.
 *   @param[in]   value        Pointer to the value to be indicated.
 *   @param[in]   len          Length of the value to be indicated.
 *   @param[in]   cb           Callback function to be called when the indication is sent.
 *
 *   @return BLE_HOST_ERR_SUCC if the indication is sent successfully.
 */
int ble_gatts_indicate(uint16_t conn_handle, uint16_t attr_handle, const uint8_t *value, uint16_t len, gatts_confirm_callback cb);
