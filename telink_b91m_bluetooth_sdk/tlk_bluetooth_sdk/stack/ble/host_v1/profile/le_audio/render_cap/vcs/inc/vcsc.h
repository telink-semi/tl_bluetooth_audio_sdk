/********************************************************************************************************
 * @file    vcsc.h
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
enum ble_vcsc_event_id
{
    VCSC_EVT_ID_CHANGED_VOLUME_STATE,
};

struct ble_vcsc_volume_state_changed_event
{
    uint8_t volume_setting;
    uint8_t mute;
};

typedef void (*ble_vcsc_event_callback)(uint16_t conn_handle, enum ble_vcsc_event_id event_id, const void *event_msg);

struct ble_vcsc_register_param
{
};

/**
 *   @brief Register the LE Audio volume control service client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the VCS module.
 *
 *   @return None.
 */
void ble_lea_register_VCS_control_client(const struct ble_vcsc_register_param *param);

/**
 *   @brief Register the LE Audio volume control service client event callback.
 *
 *   @param[in] event_callback Pointer to the callback function to be called when an event occurs.
 *
 *   @return None.
 */
void ble_lea_register_VCS_client_event_callback(ble_vcsc_event_callback event_callback);

/**
 *   @brief VCS Client Get volume state.
 *
 *   @param[in] conn_handle Connection handle to the peer device.
 *   @param[out] volume_setting If not NULL and result is BLE_HOST_ERR_SUCC, current volume setting.
 *   @param[out] mute If not NULL and result is BLE_HOST_ERR_SUCC, current mute state.
 *
 *   @return BLE_HOST_ERR_SUCC if read operation is successful, otherwise error code.
 */
int ble_vcsc_get_volume_state(uint16_t conn_handle, uint8_t *volume_setting, uint8_t *mute);

/**
 *   @brief VCS Client Read volume state.
 *
 *   @param[in] conn_handle Connection handle to the peer device.
 *   @param[in] callback Callback function to be called when the read operation is complete.
 *
 *   @return BLE_HOST_ERR_SUCC if read operation is successful, otherwise error code.
 */
int ble_vcsc_read_volume_state(uint16_t conn_handle, prf_read_callback callback);

/**
 *   @brief VCS Client Read volume flags.
 *
 *   @param[in] conn_handle Connection handle to the peer device.
 *   @param[in] callback Callback function to be called when the read operation is complete.
 *
 *   @return BLE_HOST_ERR_SUCC if read operation is successful, otherwise error code.
 */
int ble_vcsc_read_volume_flags(uint16_t conn_handle, prf_read_callback callback);

/**
 *   @brief VCS Client Write relative volume down.
 *
 *   @param[in] conn_handle Connection handle to the peer device.
 *
 *   @return BLE_HOST_ERR_SUCC if write operation is successful, otherwise error code.
 */
int ble_vcsc_write_relative_volume_down(uint16_t conn_handle);

/**
 *   @brief VCS Client Write relative volume up.
 *
 *   @param[in] conn_handle Connection handle to the peer device.
 *
 *   @return BLE_HOST_ERR_SUCC if write operation is successful, otherwise error code.
 */
int ble_vcsc_write_relative_volume_up(uint16_t conn_handle);

/**
 *   @brief VCS Client Write unmute relative volume down.
 *
 *   @param[in] conn_handle Connection handle to the peer device.
 *
 *   @return BLE_HOST_ERR_SUCC if write operation is successful, otherwise error code.
 */
int ble_vcsc_write_unmute_relative_volume_down(uint16_t conn_handle);

/**
 *   @brief VCS Client Write unmute relative volume up.
 *
 *   @param[in] conn_handle Connection handle to the peer device.
 *
 *   @return BLE_HOST_ERR_SUCC if write operation is successful, otherwise error code.
 */
int ble_vcsc_write_unmute_relative_volume_up(uint16_t conn_handle);

/**
 *   @brief VCS Client Write set absolute volume.
 *
 *   @param[in] conn_handle Connection handle to the peer device.
 *   @param[in] volume_setting Absolute volume setting.
 *
 *   @return BLE_HOST_ERR_SUCC if write operation is successful, otherwise error code.
 */
int ble_vcsc_write_set_absolute_volume(uint16_t conn_handle, uint8_t volume_setting);

/**
 *   @brief VCS Client Write unmute.
 *
 *   @param[in] conn_handle Connection handle to the peer device.
 *
 *   @return BLE_HOST_ERR_SUCC if write operation is successful, otherwise error code.
 */
int ble_vcsc_write_unmute(uint16_t conn_handle);

/**
 *   @brief VCS Client Write mute.
 *
 *   @param[in] conn_handle Connection handle to the peer device.
 *
 *   @return BLE_HOST_ERR_SUCC if write operation is successful, otherwise error code.
 */
int ble_vcsc_write_mute(uint16_t conn_handle);
