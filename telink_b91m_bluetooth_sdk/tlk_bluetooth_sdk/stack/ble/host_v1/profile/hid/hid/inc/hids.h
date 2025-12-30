/********************************************************************************************************
 * @file    hids.h
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
// HIDS: Human Interface Device Server.

enum ble_hids_event_id
{
    HIDS_EVT_ENTER_BOOT_PROTOCOL_MODE,         // refer to None.
    HIDS_EVT_EXIT_BOOT_PROTOCOL_MODE,          // refer to None.
    HIDS_EVT_RECV_BOOT_KEYBOARD_INPUT_REPORT,  // refer to ble_host_hid_boot_keyboard_input.
    HIDS_EVT_RECV_BOOT_KEYBOARD_OUTPUT_REPORT, //refer to uint16_t.
    HIDS_EVT_RECV_BOOT_MOUSE_INPUT_REPORT,     // refer to ble_hid_boot_mouse_input.
    HIDS_EVT_ENTER_SUSPEND_STATE,              //refer to None.
    HIDS_EVT_EXIT_SUSPEND_STATE,               //refer to None.
    HIDS_EVT_RECV_REPORT,                      //refer to struct ble_hids_receive_report_event.
};

//refer to HIDS_EVT_RECV_REPORT.
struct ble_hids_receive_report_event
{
    uint8_t        report_id;
    uint8_t        report_type;
    uint16_t       len;
    const uint8_t *value;
} __attribute__((packed));

struct ble_hids_register_param
{
    const uint8_t *report_map;     /** < must be a global constant value */
    uint16_t       report_map_len; /** < report map length, maximum is 512 bytes */
    // usually, BCD: 0x0111, b_count_code: 0x00, remote_wake: 0x01, normally_connectable: 0x00
    struct ble_hid_information hid_info;
};

#define BLE_HIDS_INITIAL_PARAMS_DEFAULT(report, report_len) BLE_HIDS_INITIAL_PARAMS(report, report_len, 0x0111, 0x00, 0x01, 0x00)

#define BLE_HIDS_INITIAL_PARAMS(report, report_len, bcd, b_count, wake, normally_conn)                                           \
    (&(const struct ble_hids_register_param){                                                                                    \
        .report_map     = report,                                                                                                \
        .report_map_len = report_len,                                                                                            \
        .hid_info       = {.bcd_HID = bcd, .b_count_code = b_count, .remote_wake = wake, .normally_connectable = normally_conn}, \
    })


typedef void (*ble_hids_event_callback)(uint16_t conn_handle, enum ble_hids_event_id event_id, const void *event_msg);

/**
 *   @brief Register the Human Interface Device server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the HIDSS module.
 *
 *   @return None.
 */
void ble_hid_register_HID_control_server(const struct ble_hids_register_param *param);

/**
 *   @brief Set the event callback for the Human Interface Device server control.
 *
 *   @param[in] callback Pointer to the callback function.
 *
 *   @return None.
 *
 */
void ble_hid_register_HID_server_event_callback(ble_hids_event_callback callback);

/**
 *   @brief Notify the input report.
 *
 *   @param[in] conn_handle Connection handle.
 *   @param[in] report_id Input Report ID.
 *   @param[in] value Pointer to the report data.
 *   @param[in] value_len Length of the report data.
 *
 *   @return BLE_HOST_ERR_SUCC if notify input report successfully, otherwise return error code.
 */
int ble_hids_notify_input_report(uint16_t conn_handle, uint8_t report_id, const uint8_t *value, uint16_t value_len);

/**
 *   @brief Register the HIDS Keyboard and Mouse control server.
 *
 *   @param[in] param Pointer to the structure containing the HIDS registration parameters.
 *
 *   @return None.
 */
void ble_hid_register_HID_km_control_server(const struct ble_hids_register_param *param);

/**
 *   @brief Register the HIDS Keyboard and Mouse server event callback function.
 *
 *   @param[in] event_callback HIDS event callback function.
 *
 *   @return None.
 */
void ble_hid_register_HID_km_server_event_callback(ble_hids_event_callback event_callback);

/**
 *   @brief Set the boot keyboard input value.
 *
 *   @param[in] value Pointer to the structure containing the boot keyboard input value.
 *
 *   @return BLE_HOST_ERR_SUCC if set boot keyboard input value successfully, otherwise return error code.
 */
int ble_hids_km_set_boot_keyboard_input(struct ble_host_hid_boot_keyboard_input *value);

/**
 *   @brief Notify the boot keyboard input value.
 *
 *   @param[in] conn_handle Connection handle.
 *   @param[in] value Pointer to the structure containing the boot keyboard input value.
 *
 *   @return BLE_HOST_ERR_SUCC if notify boot keyboard input value successfully, otherwise return error code.
 */
int ble_hids_km_notify_boot_keyboard_input(uint16_t conn_handle, struct ble_host_hid_boot_keyboard_input *value);

/**
 *   @brief Set the boot mouse input value.
 *
 *   @param[in] value Pointer to the structure containing the boot mouse input value.
 *
 *   @return BLE_HOST_ERR_SUCC if set boot mouse input value successfully, otherwise return error code.
 */
int ble_hids_km_set_boot_mouse_input(struct ble_hid_boot_mouse_input *value);

/**
 *   @brief Notify the boot mouse input value.
 *
 *   @param[in] conn_handle Connection handle.
 *   @param[in] value Pointer to the structure containing the boot mouse input value.
 *
 *   @return BLE_HOST_ERR_SUCC if notify boot mouse input value successfully, otherwise return error code.
 */
int ble_hids_km_notify_boot_mouse_input(uint16_t conn_handle, struct ble_hid_boot_mouse_input *value);

/**
 *   @brief Notify the input report.
 *
 *   @param[in] conn_handle Connection handle.
 *   @param[in] report_id Input Report ID.
 *   @param[in] value Pointer to the report data.
 *   @param[in] value_len Length of the report data.
 *
 *   @return BLE_HOST_ERR_SUCC if notify input report successfully, otherwise return error code.
 */
int ble_hids_km_notify_input_report(uint16_t conn_handle, uint8_t report_id, const uint8_t *value, uint16_t value_len);
