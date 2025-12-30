/********************************************************************************************************
 * @file    vcss.h
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

// VCSS: Volume Control Service Server.

enum ble_vcss_event_id
{
    VCSS_EVT_ID_VOLUME_STATE_CHANGED = 0x01, /** < refer to 'struct vcss_volume_state_changed_event' */
};

struct vcss_volume_state_changed_event
{
    uint8_t volume_setting; /** < Current Volume Setting, 0-255 */
    bool    mute_state;     /** < Current Mute State, true or false */
};

typedef void (*ble_vcss_event_callback)(uint16_t conn_handle, enum ble_vcss_event_id event_id, void *event_data);

struct ble_vcss_register_param
{
    uint8_t step; /** < Volume Setting Change Step;1-255 */

    /** < Volume State Config */
    uint8_t volume; /** < Volume Setting, 0-255 */
    bool    mute;   /** < Mute State, true or false */

    // const struct ble_aicss_register_param *aicss_param;   /** < Pointer to the AICSS register parameter */
    const struct ble_vocss_register_param *vocss_param; /** < Pointer to the VOCSS register parameter */
};

/**
 *   @brief Register the Volume Control Service server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the VCSS module.
 *
 *   @return None.
 */
void ble_lea_register_VCS_control_server(const struct ble_vcss_register_param *param);

/**
 *   @brief Set the event callback for the Volume Control Service server control.
 *
 *   @param[in] callback Pointer to the callback function.
 *
 *   @return None.
 *
 */
void ble_lea_register_VCS_event_callback(ble_vcss_event_callback callback);

/**
 *   @brief Set the volume setting of the VCSS.
 *
 *   @param[in] volume The new volume setting, 0-255.
 *
 *   @return None.
 */
void ble_vcss_set_volume_setting(uint8_t volume);

/**
 *   @brief Set the mute state of the VCSS.
 *
 *   @param[in] mute The new mute state, true(muted) or false(not muted).
 *
 *   @return None.
 */
void ble_vcss_set_mute_state(bool mute);

/**
 *   @brief Get the volume setting of the VCSS.
 *
 *   @return The current volume setting, 0-255.
 */
uint8_t ble_vcss_get_volume_setting(void);

/**
 *   @brief Update the Volume Setting of the Volume Control Service server.
 *
 *   @param[in] conn_handle Connection handle to identify the connection.
 *   @param[in] volume The new volume setting, 0-255.
 *
 *   @return BLE_HOST_ERR_SUCC if the update was successful, or an error code otherwise.
 *
 *   @note Even if the connection handle is invalid, the Volume Setting is still updated.
 */
int ble_vcss_update_volume_setting(uint16_t conn_handle, uint8_t volume);

/**
 *   @brief Update the Mute State of the Volume Control Service server.
 *
 *   @param[in] conn_handle Connection handle to identify the connection.
 *   @param[in] mute The new mute state, true(muted) or false(not muted).
 *
 *   @return BLE_HOST_ERR_SUCC if the update was successful, or an error code otherwise.
 *
 *   @note Even if the connection handle is invalid, the Mute State is still updated.
 */
int ble_vcss_update_mute_state(uint16_t conn_handle, bool mute);

/**
 *   @brief Update the Volume and Mute State of the Volume Control Service server.
 *
 *   @param[in] conn_handle Connection handle to identify the connection.
 *   @param[in] volume The new volume setting, 0-255.
 *   @param[in] mute The new mute state, true(muted) or false(not muted).
 *
 *   @return BLE_HOST_ERR_SUCC if the update was successful, or an error code otherwise.
 *
 *   @note Even if the connection handle is invalid, the Volume and Mute State is still updated.
 */
int ble_vcss_update_volume_and_mute_state(uint16_t conn_handle, uint8_t volume, bool mute);

/**
 *   @brief Update the Volume Flags of the Volume Control Service server.
 *
 *   @param[in] conn_handle Connection handle to identify the connection.
 *   @param[in] flags The new volume flags.
 *
 *   @return BLE_HOST_ERR_SUCC if the update was successful, or an error code otherwise.
 *
 *   @note Even if the connection handle is invalid, the Volume Flags is still updated.
 */
int ble_vcss_update_volume_flags(uint16_t conn_handle, struct vcs_volume_flags flags);

/**
 *   @brief Update the Volume Setting Change of the Volume Control Service server.
 *
 *   @param[in] conn_handle Connection handle to identify the connection.
 *   @param[in] vol_setting_change The new volume setting change.
 *
 *   @return BLE_HOST_ERR_SUCC if the update was successful, or an error code otherwise.
 */
int ble_vcss_update_volume_setting_change(uint16_t conn_handle, int8_t vol_setting_change);
