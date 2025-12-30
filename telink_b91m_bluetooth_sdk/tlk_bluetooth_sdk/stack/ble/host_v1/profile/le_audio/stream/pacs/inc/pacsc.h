/********************************************************************************************************
 * @file    pacsc.h
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
 * @brief PACS Client error codes
 */
enum pacsc_error_code
{
    BLE_PACSC_ERR_START                 = 0x80,
    BLE_PACSC_ERR_INVALID_PARAM         = 0x81,
    BLE_PACSC_ERR_NOT_INITIALIZED       = 0x82,
    BLE_PACSC_ERR_DISCOVERY_FAILED      = 0x83,
    BLE_PACSC_ERR_NOTIFICATION_DISABLED = 0x84,
};

/**
 * @brief PACS Client event IDs
 */
enum ble_pacsc_event_id
{
    PACSC_EVT_ID_,
};

/**
 * @brief PACS Client event callback function type
 */
typedef void (*ble_pacsc_event_callback)(uint16_t conn_handle, enum ble_pacsc_event_id event_id, const void *event_msg);

/**
 * @brief Structure containing parameters for registering the PACS client
 */
struct ble_pacsc_register_param
{
};

/**
 *   @brief Register the LE Audio Published Audio Capabilities Service (PACS) client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the PACS module.
 *
 *   @return None.
 */
void ble_lea_register_PACS_control_client(const struct ble_pacsc_register_param *param);

/**
 *   @brief Register the LE Audio Published Audio Capabilities Service (PACS) client event callback.
 *
 *   @param[in] event_callback Pointer to the callback function to be called when an event occurs.
 *
 *   @return None.
 */
void ble_lea_register_PACS_client_event_callback(ble_pacsc_event_callback event_callback);

/**
 *   @brief Get the Sink Audio Locations of the PACS client.
 *
 *   @param[in] conn_handle Connection handle to the PACS client.
 *   @param[out] out_sink_audio_locations Pointer to the variable to store the Sink Audio Locations.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 *          - BLE_PRF_ERR(BLE_PRF_ERR_INVALID_CONN_HANDLE) if the connection handle is invalid.
 *          - BLE_PRF_ERR(BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND) if the remote not support sink audio locations.
 */
int ble_pacsc_get_sink_audio_locations(uint16_t conn_handle, uint32_t *out_sink_audio_locations);

/**
 *   @brief Get the Source Audio Locations of the PACS client.
 *
 *   @param[in] conn_handle Connection handle to the PACS client.
 *   @param[out] out_source_audio_locations Pointer to the variable to store the Source Audio Locations.
 *
 *   @return BLE_HOST_ERR_SUCC if the operation is successful, otherwise an error code.
 *          - BLE_PRF_ERR(BLE_PRF_ERR_INVALID_CONN_HANDLE) if the connection handle is invalid.
 *          - BLE_PRF_ERR(BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND) if the remote not support source audio locations.
 */
int ble_pacsc_get_source_audio_locations(uint16_t conn_handle, uint32_t *out_source_audio_locations);
