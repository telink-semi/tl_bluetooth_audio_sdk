/********************************************************************************************************
 * @file    gtbsc.h
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

enum gtbsc_error_code
{
    BLE_GTBSC_ERR_START = 0x80,
    BLE_GTBSC_ERR_REMOTE_UNSUPPORTEDOPCODE
};

enum ble_gtbsc_event_id
{
    GTBSC_EVT_ID_,
};

typedef void (*ble_gtbsc_event_callback)(uint16_t conn_handle, enum ble_gtbsc_event_id event_id, const void *event_msg);

struct ble_gtbsc_register_param
{
};

/**
 *   @brief Register the LE Audio general telephone bearer service client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the GTBS module.
 *
 *   @return None.
 */
void ble_lea_register_GTBS_control_client(const struct ble_gtbsc_register_param *param);

/**
 *   @brief Register the LE Audio general telephone bearer service client event callback.
 *
 *   @param[in] event_callback Pointer to the callback function to be called when an event occurs.
 *
 *   @return None.
 */
void ble_lea_register_GTBS_client_event_callback(ble_gtbsc_event_callback event_callback);

/**
 * @brief Check if there is an incoming call state
 *
 * This function checks if there is an incoming call (state == 0x00) in the
 * call state list for the specified connection.
 *
 * @param[in] conn_handle      Connection handle of the device
 *
 * @return true if there is an incoming call, false otherwise
 */
bool ble_gtbsc_check_incoming_state(uint16_t conn_handle);

/**
 * @brief Accept an incoming call
 *
 * This function accepts an incoming call by sending an accept command
 * to the call control point. It prioritizes the second call if both
 * calls are incoming, otherwise accepts the first incoming call.
 *
 * @param[in] conn_handle      Connection handle of the device
 *
 * @return true if the accept command was sent successfully, false otherwise
 */
bool ble_gtbsc_accept_incoming_call(uint16_t conn_handle);

/**
 * @brief Check if telephone state is available
 *
 * This function checks if the telephone call state information is available
 * for the specified connection.
 *
 * @param[in] conn_handle      Connection handle of the device
 *
 * @return true if telephone state is available, false otherwise
 */
bool ble_gtbsc_check_telephone_state(uint16_t conn_handle);

/**
 * @brief Terminate a new incoming call
 *
 * This function terminates a new incoming call by sending a terminate command
 * to the call control point. It prioritizes terminating the second call if
 * multiple calls exist, otherwise terminates the first call.
 *
 * @param[in] conn_handle      Connection handle of the device
 *
 * @return true if the terminate command was sent successfully, false otherwise
 */
bool ble_gtbsc_terminate_new_call(uint16_t conn_handle);
