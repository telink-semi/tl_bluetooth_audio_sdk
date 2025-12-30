/********************************************************************************************************
 * @file    bass.h
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

// BASS: Battery Service Server.

struct ble_bass_register_param
{
    uint8_t battery_level; /** < range 0-100 */
    uint8_t power_state;
};

#define BLE_BASS_INITIAL_PARAMS(level, state) (&(const struct ble_bass_register_param){.battery_level = level, .power_state = state})

/**
 *   @brief Register the battery service server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the BAS module.
 *
 *   @return None.
 */
void ble_basic_register_BAS_control_server(const struct ble_bass_register_param *param);

/**
 *   @brief Get the current battery level.
 *
 *   @return The current battery level.
 */
uint8_t ble_bass_get_battery_level(void);

/**
 *   @brief Set the current battery level.
 *
 *   @param[in] level The new battery level, range 0-100.
 *
 *   @return None.
 */
void ble_bass_set_battery_level(uint8_t level);

/**
 *   @brief Update the battery level for a given connection handle.
 *
 *   @param[in] conn_handle The connection handle for which to update the battery level.
 *   @param[in] level The new battery level, range 0-100.
 *
 *   @return BLE_HOST_ERR_SUCC if the update was successful, or an error code otherwise.
 *
 *   @note Even if the connection handle is invalid, the battery level is still updated.
 */
int ble_bass_update_battery_level(uint16_t conn_handle, uint8_t level);

/**
 *   @brief Get the current power state.
 *
 *   @return The current power state.
 */
uint8_t ble_bass_get_power_state(void);

/**
 *   @brief Set the current power state.
 *
 *   @param[in] state The new power state.
 *
 *   @return None.
 */
void ble_bass_set_power_state(uint8_t state);

/**
 *   @brief Update the power state for a given connection handle.
 *
 *   @param[in] conn_handle The connection handle for which to update the power state.
 *   @param[in] power_state The new power state.
 *
 *   @return BLE_HOST_ERR_SUCC if the update was successful, or an error code otherwise.
 *
 *   @note Even if the connection handle is invalid, the power state is still updated.
 */
int ble_bass_update_power_state(uint16_t conn_handle, uint8_t power_state);
