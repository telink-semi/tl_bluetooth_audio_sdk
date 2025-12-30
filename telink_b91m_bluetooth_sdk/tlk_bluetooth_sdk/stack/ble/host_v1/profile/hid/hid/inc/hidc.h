/********************************************************************************************************
 * @file    hidc.h
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
// HIDC: Human Interface Device Service Client.

//HID Client Event ID
enum ble_hidc_event_id
{
    HIDC_EVT_RECV_BOOT_KEYBOARD_INPUT_REPORT, //refer to struct 'struct ble_hidc_recv_boot_keyboard_input_evt.
    HIDC_EVT_RECV_BOOT_MOUSE_INPUT_REPORT,    //refer to struct 'struct ble_hidc_recv_boot_mouse_input_evt'.
    HIDC_EVT_RECV_INPUT_REPORT_DATA,          //refer to struct 'struct ble_hidc_recv_input_report_data'.
};

//refer to HIDC_EVT_RECV_BOOT_KEYBOARD_INPUT_REPORT,
struct ble_hidc_recv_boot_keyboard_input_evt
{
    const uint8_t *value;
    uint16_t       len;
};

//refer to HIDC_EVT_RECV_BOOT_MOUSE_INPUT_REPORT.
struct ble_hidc_recv_boot_mouse_input_evt
{
    const uint8_t *value;
    uint16_t       len;
};

//refer to HIDC_EVT_RECV_INPUT_REPORT_DATA.
struct ble_hidc_recv_input_report_data
{
    uint8_t        report_id;
    const uint8_t *value;
    uint16_t       len;
};

typedef void (*ble_hidc_event_callback)(uint16_t conn_handle, enum ble_hidc_event_id event_id, const void *event_msg);

struct ble_hidc_register_param
{
};

#define BLE_HIDC_INITIAL_PARAMS NULL

/**
 *   @brief Register the human interface device service client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the HIDC module.
 *
 *   @return None.
 */
void ble_hid_register_HID_control_client(const struct ble_hidc_register_param *param);

/**
 *   @brief Register the event callback function for the human interface device service client control.
 *
 *   @param[in] event_callback Pointer to the callback function to be called when an event occurs.
 *
 *   @return None.
 */
void ble_hid_register_HID_client_event_callback(ble_hidc_event_callback event_callback);

/********* Protocol Mode Operations *********/
/**
 *    @brief Write the protocol mode to the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] protocol_mode Protocol mode BLE_HOST_HID_PROTOCOL_MODE_BOOT and BLE_HOST_HID_PROTOCOL_MODE_REPORT.
 *
 *    @return BLE_HOST_ERR_SUCC if the write operation is successful. Otherwise, a negative value is returned.
 */
int ble_hidc_write_protocol_mode(uint16_t conn_handle, uint8_t protocol_mode);

/**
 *    @brief Write the boot protocol mode to the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return BLE_HOST_ERR_SUCC if the write operation is successful. Otherwise, a negative value is returned.
 */
int ble_hidc_write_boot_protocol_mode(uint16_t conn_handle);

/**
 *    @brief Write the report protocol mode to the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return BLE_HOST_ERR_SUCC if the write operation is successful. Otherwise, a negative value is returned.
 */
int ble_hidc_write_report_protocol_mode(uint16_t conn_handle);

/**
 *   @brief HID Client module read protocol mode.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_hidc_read_protocol_mode(uint16_t conn_handle, prf_read_callback callback);

/**
 *   @brief Get the protocol level.
 *
 *   @param[in] conn_handle Connection handle to the server.
 *   @param[out] protocol_mode Pointer to the variable to store the protocol mode.
 *
 *   @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_INPUT_NULL if the protocol_mode is NULL.
 *          -- BLE_PRF_ERR_GET_ATTR_VALUE_NOT_FOUND if remote device doesn't have the attribute.
 */
int ble_hidc_get_protocol_mode(uint16_t conn_handle, uint8_t *protocol_mode);

/********* HID Control Point Operations *********/

/**
 *    @brief Write the HID control point to the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] control_point Control point value, HID_CONTROL_POINT_ENTER_SUSPEND or HID_CONTROL_POINT_EXIT_SUSPEND.
 *
 *    @return BLE_HOST_ERR_SUCC if the write operation is successful. Otherwise, a negative value is returned.
 */
int ble_hidc_write_hid_control_point(uint16_t conn_handle, uint8_t control_point);

/**
 *    @brief Write the enter suspend control point to the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return BLE_HOST_ERR_SUCC if the write operation is successful. Otherwise, a negative value is returned.
 */
int ble_hidc_write_enter_suspend(uint16_t conn_handle);

/**
 *    @brief Write the exit suspend control point to the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return BLE_HOST_ERR_SUCC if the write operation is successful. Otherwise, a negative value is returned.
 */
int ble_hidc_write_exit_suspend(uint16_t conn_handle);

/********* HID Information Operations *********/

/**
 *    @brief Read the HID information from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_hidc_read_hid_information(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the HID information from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[out] hid_information Pointer to the variable to store the HID information.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_INPUT_NULL if the hid_information is NULL.
 */
int ble_hidc_get_hid_information(uint16_t conn_handle, struct ble_hid_information *hid_information);

/********* Boot Operations *********/

/**
 *    @brief Read the boot keyboard input report from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_hidc_read_boot_keyboard_input_report(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the boot keyboard input report from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[out] boot_keyboard_input_report Pointer to the variable to store the boot keyboard input report.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_INPUT_NULL if the boot_keyboard_input_report is NULL.
 */
int ble_hidc_get_boot_keyboard_input_report(uint16_t conn_handle, struct ble_host_hid_boot_keyboard_input *boot_keyboard_input_report);

/**
 *    @brief Write the boot keyboard output report to the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] boot_keyboard_output_report Boot keyboard output report value.
 *
 *    @return BLE_HOST_ERR_SUCC if the write operation is successful. Otherwise, a negative value is returned.
 */
int ble_hidc_write_boot_keyboard_output_report(uint16_t conn_handle, uint8_t boot_keyboard_output_report);

/**
 *    @brief Read the boot keyboard output report from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_hidc_read_boot_keyboard_output_report(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the boot keyboard output report from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[out] boot_keyboard_output_report Pointer to the variable to store the boot keyboard output report.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_INPUT_NULL if the boot_keyboard_output_report is NULL.
 */
int ble_hidc_get_boot_keyboard_output_report(uint16_t conn_handle, uint8_t *boot_keyboard_output_report);

/**
 *    @brief Read the boot mouse input report from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_hidc_read_boot_mouse_input_report(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the boot mouse input report from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[out] boot_mouse_input_report Pointer to the variable to store the boot mouse input report.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_INPUT_NULL if the boot_mouse_input_report is NULL.
 */
int ble_hidc_get_boot_mouse_input_report(uint16_t conn_handle, struct ble_hid_boot_mouse_input *boot_mouse_input_report);

/********* Report Map **********/

/**
 *    @brief Read the report map from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 */
int ble_hidc_read_report_map(uint16_t conn_handle, prf_read_callback callback);

/**
 *    @brief Get the report map from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[out] report_map Pointer to the variable to store the report map.
 *    @param[out] report_map_len Pointer to the variable to store the report map length.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 *          -- BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *          -- BLE_PRF_ERR_INPUT_NULL if the report_map is NULL.
 */
int ble_hidc_get_report_map(uint16_t conn_handle, uint8_t **report_map, uint16_t *report_map_len);

/********* Report Operations *********/

/**
 *    @brief Write the output report without response to the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] report_id Report ID.
 *    @param[in] report output report value.
 *    @param[in] report_len output report length.
 *
 *    @return BLE_HOST_ERR_SUCC if the write operation is successful. Otherwise, a negative value is returned.
 */
int ble_hid_write_output_report(uint16_t conn_handle, uint8_t report_id, const uint8_t *report, uint16_t report_len);

/**
 *    @brief Write the output report with response to the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] report_id Report ID.
 *    @param[in] report output report value.
 *    @param[in] report_len output report length.
 *
 *    @return BLE_HOST_ERR_SUCC if the write operation is successful. Otherwise, a negative value is returned.
 */
int ble_hid_write_output_report_response(uint16_t conn_handle, uint8_t report_id, uint8_t *report, uint16_t report_len, prf_write_callback callback);

/**
 *    @brief Read the feature report from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] report_id Report ID.
 *    @param[in] callback Pointer to the callback function to be called when the read operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 */
int ble_hid_read_feature_report(uint16_t conn_handle, uint8_t report_id, prf_read_callback callback);

/**
 *    @brief Write the feature report to the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] report_id Report ID.
 *    @param[in] report feature report value.
 *    @param[in] report_len feature report length.
 *    @param[in] callback Pointer to the callback function to be called when the write operation is complete.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 */
int ble_hid_write_feature_report(uint16_t conn_handle, uint8_t report_id, uint8_t *report, uint16_t report_len, prf_write_callback callback);

/**
 *    @brief Get the feature report from the HID service on the server.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] report_id Report ID.
 *    @param[out] report Pointer to the variable to store the feature report.
 *    @param[out] report_len Pointer to the variable to store the feature report length.
 *
 *    @return BLE_HOST_ERR_SUCC if the command was sent successfully; otherwise, the error code.
 */
int ble_hid_get_feature_report(uint16_t conn_handle, uint8_t report_id, uint8_t **report, uint16_t *report_len);
