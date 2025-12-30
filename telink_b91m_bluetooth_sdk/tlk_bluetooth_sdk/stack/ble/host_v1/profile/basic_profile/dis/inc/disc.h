/********************************************************************************************************
 * @file    disc.h
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
// DISC: Device Information Service Client.

struct ble_disc_register_param
{
};

#define BLE_DISC_INITIAL_PARAMS NULL

/**
 *   @brief Register the device information service client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the DISC module.
 *
 *   @return None.
 */
void ble_basic_register_DIS_control_client(const struct ble_disc_register_param *param);

/**
 *   @brief DIS Client module read Manufacturer Name String.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_disc_read_manufacturer_name_string(uint16_t conn_handle, prf_read_callback callback);

/**
 *   @brief Get the manufacturer name string from the remote device.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[out] manufacturer_name_string Pointer to the variable to store the manufacturer name string from the remote device.
 *   @param[out] manufacturer_name_string_len Pointer to the variable to store the length of the manufacturer name string.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_INPUT_NULL if the manufacturer_name_string is NULL.
 *          -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_disc_get_manufacturer_name_string(uint16_t conn_handle, uint8_t *manufacturer_name_string, uint16_t *manufacturer_name_string_len);

/**
 *   @brief DIS Client module read Model Number String.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_disc_read_model_number_string(uint16_t conn_handle, prf_read_callback callback);

/**
 *   @brief Get the model number string from the remote device.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[out] model_number_string Pointer to the variable to store the model number string from the remote device.
 *   @param[out] model_number_string_len Pointer to the variable to store the length of the model number string.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_INPUT_NULL if the model_number_string is NULL.
 *          -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_disc_get_model_number_string(uint16_t conn_handle, uint8_t *model_number_string, uint16_t *model_number_string_len);

/**
 *   @brief DIS Client module read Serial Number String.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_disc_read_serial_number_string(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the serial number string from the remote device.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[out] serial_number_string Pointer to the variable to store the serial number string from the remote device.
 *    @param[out] serial_number_string_len Pointer to the variable to store the length of the serial number string.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *           -- BLE_PRF_ERR_INPUT_NULL if the serial_number_string is NULL.
 *           -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_disc_get_serial_number_string(uint16_t conn_handle, uint8_t *serial_number_string, uint16_t *serial_number_string_len);

/**
 *    @brief DIS Client module read Hardware Revision String.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_disc_read_hardware_revision_string(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the hardware revision string from the remote device.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[out] hardware_revision_string Pointer to the variable to store the hardware revision string from the remote device.
 *    @param[out] hardware_revision_string_len Pointer to the variable to store the length of the hardware revision string.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *           -- BLE_PRF_ERR_INPUT_NULL if the hardware_revision_string is NULL.
 *           -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_disc_get_hardware_revision_string(uint16_t conn_handle, uint8_t *hardware_revision_string, uint16_t *hardware_revision_string_len);

/**
 *    @brief DIS Client module read Firmware Revision String.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_disc_read_firmware_revision_string(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the firmware revision string from the remote device.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[out] firmware_revision_string Pointer to the variable to store the firmware revision string from the remote device.
 *    @param[out] firmware_revision_string_len Pointer to the variable to store the length of the firmware revision string.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *           -- BLE_PRF_ERR_INPUT_NULL if the firmware_revision_string is NULL.
 *           -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_disc_get_firmware_revision_string(uint16_t conn_handle, uint8_t *firmware_revision_string, uint16_t *firmware_revision_string_len);

/**
 *    @brief DIS Client module read Software Revision String.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_disc_read_software_revision_string(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the software revision string from the remote device.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[out] software_revision_string Pointer to the variable to store the software revision string from the remote device.
 *    @param[out] software_revision_string_len Pointer to the variable to store the length of the software revision string.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *           -- BLE_PRF_ERR_INPUT_NULL if the software_revision_string is NULL.
 *           -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_disc_get_software_revision_string(uint16_t conn_handle, uint8_t *software_revision_string, uint16_t *software_revision_string_len);

/**
 *    @brief DIS Client module read IEEE 11073-20601 Regulatory Certification Data List.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_disc_read_ieee_list(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the IEEE 11073-20601 Regulatory Certification Data List from the remote device.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[out] ieee_list Pointer to the variable to store the IEEE 11073-20601 Regulatory Certification Data List from the remote device.
 *    @param[out] ieee_list_len Pointer to the variable to store the length of the IEEE 11073-20601 Regulatory Certification Data List.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *           -- BLE_PRF_ERR_INPUT_NULL if the ieee_list is NULL.
 *           -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_disc_get_ieee_list(uint16_t conn_handle, uint8_t *ieee_list, uint16_t *ieee_list_len);

/**
 *    @brief DIS Client module read UDI for Medical Devices.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_disc_read_udi_for_medical_devices(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the UDI for Medical Devices from the remote device.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[out] udi_for_medical_devices Pointer to the variable to store the UDI for Medical Devices from the remote device.
 *    @param[out] udi_for_medical_devices_len Pointer to the variable to store the length of the UDI for Medical Devices.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *           -- BLE_PRF_ERR_INPUT_NULL if the udi_for_medical_devices is NULL.
 *           -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_disc_get_udi_for_medical_devices(uint16_t conn_handle, uint8_t *udi_for_medical_devices, uint16_t *udi_for_medical_devices_len);

/**
 *    @brief DIS Client module read System ID.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_disc_read_system_id(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the System ID from the remote device.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[out] system_id Pointer to the variable to store the System ID from the remote device.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *           -- BLE_PRF_ERR_INPUT_NULL if the system_id is NULL.
 *           -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_disc_get_system_id(uint16_t conn_handle, struct ble_dis_system_id *system_id);

/**
 *    @brief DIS Client module read PnP ID.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_disc_read_pnp_id(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the PnP ID from the remote device.
 *
 *    @param[in] conn_handle Connection handle to the server.
 *    @param[out] pnp_id Pointer to the variable to store the PnP ID from the remote device.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *           -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *           -- BLE_PRF_ERR_INPUT_NULL if the pnp_id is NULL.
 *           -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_disc_get_pnp_id(uint16_t conn_handle, struct ble_dis_pnp_id *pnp_id);
