/********************************************************************************************************
 * @file    basc.h
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
// BASC: Battery Service Client.

enum ble_basc_event_id
{
    BASC_EVT_ID_BATTERY_LEVEL_UPDATE       = 0x01, /** < refer to 'struct ble_bas_battery_level_update_evt' */
    BASC_EVT_ID_BATTERY_POWER_STATE_UPDATE = 0x02, /** < refer to 'struct ble_bas_battery_power_state_update_evt' */
};

struct ble_bas_battery_level_update_evt
{
    uint8_t battery_level; /** < Battery level in percentage */
};

struct ble_bas_battery_power_state_update_evt
{
    uint8_t battery_power_state; /** < Battery power state */
};

typedef void (*ble_basc_event_callback)(uint16_t conn_handle, enum ble_basc_event_id event_id, const void *event_msg);

struct ble_basc_register_param
{
};

#define BLE_BASC_INITIAL_PARAMS NULL

/**
 *   @brief Register the battery service client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the BASC module.
 *
 *   @return None.
 */
void ble_basic_register_BAS_control_client(const struct ble_basc_register_param *param);

/**
 *   @brief Register the event callback function for the battery service client control.
 *
 *   @param[in] event_callback Pointer to the callback function to be called when an event occurs.
 *
 *   @return None.
 */
void ble_basic_register_BAS_client_event_callback(ble_basc_event_callback event_callback);

/**
 *   @brief BAS Client module read Battery Level.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_basc_read_battery_level(uint16_t conn_handle, prf_read_callback callback);

/**
 *   @brief BAS Client module read Battery Power State.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_basc_read_power_sate(uint16_t conn_handle, prf_read_callback callback);

/**
 *   @brief Get the battery level.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[out] battery_level Pointer to the variable to store the battery level.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_INPUT_NULL if the battery_level is NULL.
 *          -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_basc_get_battery_level(uint16_t conn_handle, uint8_t *battery_level);

/**
 *   @brief Get the battery power state.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[out] battery_power_state Pointer to the variable to store the battery power state.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_INPUT_NULL if the battery_power_state is NULL.
 *          -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_basc_get_power_state(uint16_t conn_handle, uint8_t *battery_power_state);
