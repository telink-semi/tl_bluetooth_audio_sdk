/********************************************************************************************************
 * @file    tbsc.h
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

enum tbsc_error_code
{
    BLE_TBSC_ERR_START = 0x80,
    BLE_TBSC_ERR_REMOTE_UNSUPPORTEDOPCODE,
};

enum ble_tbsc_event_id
{
    TBSC_EVT_ID_,
};

typedef void (*ble_tbsc_event_callback)(uint16_t conn_handle, enum ble_tbsc_event_id event_id, const void *event_msg);

struct ble_tbsc_register_param
{
};

/**
 *   @brief Register the LE Audio general telephone bearer service client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the TBS module.
 *
 *   @return None.
 */
void ble_lea_register_TBS_control_client(const struct ble_tbsc_register_param *param);

/**
 *   @brief Register the LE Audio general telephone bearer service client event callback.
 *
 *   @param[in] event_callback Pointer to the callback function to be called when an event occurs.
 *
 *   @return None.
 */
void ble_lea_register_TBS_client_event_callback(ble_tbsc_event_callback event_callback);

/**
 * @brief       Accepts a call without waiting for a response.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - call instance index.
 * @return      BLE_PRF_SUCCESS if the command is transmitted, otherwise an error code.
 * @note        Use when no completion status from the peer is required.
 */
int ble_tbsc_write_call_control_accept_without_response(uint16_t conn_handle, uint8_t call_index);

/**
 * @brief       Accepts a call and reports completion through a callback.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - call instance index.
 * @param[in]   callback    - completion callback invoked with ATT status.
 * @return      BLE_PRF_SUCCESS if the write request is queued, otherwise an error code.
 * @note        Requires write-with-response support on the peer characteristic.
 */
int ble_tbsc_write_call_control_accept(uint16_t conn_handle, uint8_t call_index, prf_write_callback callback);

/**
 * @brief       Terminates a call without requesting a response.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - call instance index.
 * @return      BLE_PRF_SUCCESS if the command is transmitted, otherwise an error code.
 * @note        Use when peer acknowledgement is not required.
 */
int ble_tbsc_write_call_control_terminate_without_response(uint16_t conn_handle, uint8_t call_index);

/**
 * @brief       Terminates a call and provides completion status through a callback.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - call instance index.
 * @param[in]   callback    - completion callback invoked with ATT status.
 * @return      BLE_PRF_SUCCESS if the write request is queued, otherwise an error code.
 * @note        Requires write-with-response support on the peer characteristic.
 */
int ble_tbsc_write_call_control_terminate(uint16_t conn_handle, uint8_t call_index, prf_write_callback callback);

/**
 * @brief       Places a call on local hold without waiting for a response.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - call instance index.
 * @return      BLE_PRF_SUCCESS if the command is transmitted, otherwise an error code.
 */
int ble_tbsc_write_call_control_local_hold_without_response(uint16_t conn_handle, uint8_t call_index);

/**
 * @brief       Places a call on local hold and reports completion through a callback.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - call instance index.
 * @param[in]   callback    - completion callback invoked with ATT status.
 * @return      BLE_PRF_SUCCESS if the write request is queued, otherwise an error code.
 */
int ble_tbsc_write_call_control_local_hold(uint16_t conn_handle, uint8_t call_index, prf_write_callback callback);

/**
 * @brief       Retrieves a locally held call without requesting a response.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - call instance index.
 * @return      BLE_PRF_SUCCESS if the command is transmitted, otherwise an error code.
 */
int ble_tbsc_write_call_control_local_retrieve_without_response(uint16_t conn_handle, uint8_t call_index);

/**
 * @brief       Retrieves a locally held call and reports completion through a callback.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - call instance index.
 * @param[in]   callback    - completion callback invoked with ATT status.
 * @return      BLE_PRF_SUCCESS if the write request is queued, otherwise an error code.
 */
int ble_tbsc_write_call_control_local_retrieve(uint16_t conn_handle, uint8_t call_index, prf_write_callback callback);

/**
 * @brief       Originates a call using URI information without waiting for a response.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   uri         - pointer to URI string.
 * @return      BLE_PRF_SUCCESS if the command is transmitted, otherwise an error code.
 */
int ble_tbsc_write_call_control_originate_without_response(uint16_t conn_handle, char *uri);

/**
 * @brief       Originates a call using URI information and reports completion through a callback.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   uri         - pointer to URI string.
 * @param[in]   callback    - completion callback invoked with ATT status.
 * @return      BLE_PRF_SUCCESS if the write request is queued, otherwise an error code.
 */
int ble_tbsc_write_call_control_originate(uint16_t conn_handle, char *uri, prf_write_callback callback);

/**
 * @brief       Joins multiple calls without requesting a response.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - array of call indexes to join.
 * @param[in]   call_count  - number of calls in the array.
 * @return      BLE_PRF_SUCCESS if the command is transmitted, otherwise an error code.
 */
int ble_tbsc_write_call_control_join_without_response(uint16_t conn_handle, uint8_t call_index[], uint16_t call_count);

/**
 * @brief       Joins a single call into a conference without waiting for a response.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - call instance index.
 * @return      BLE_PRF_SUCCESS if the command is transmitted, otherwise an error code.
 */
int ble_tbsc_write_call_control_join_one_without_response(uint16_t conn_handle, uint8_t call_index);

/**
 * @brief       Joins two calls into a conference without waiting for a response.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index1 - first call instance index.
 * @param[in]   call_index2 - second call instance index.
 * @return      BLE_PRF_SUCCESS if the command is transmitted, otherwise an error code.
 */
int ble_tbsc_write_call_control_join_two_without_response(uint16_t conn_handle, uint8_t call_index1, uint8_t call_index2);

/**
 * @brief       Joins multiple calls and reports completion through a callback.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - array of call indexes to join.
 * @param[in]   call_count  - number of calls in the array.
 * @param[in]   callback    - completion callback invoked with ATT status.
 * @return      BLE_PRF_SUCCESS if the write request is queued, otherwise an error code.
 */
int ble_tbsc_write_call_control_join(uint16_t conn_handle, uint8_t call_index[], uint16_t call_count, prf_write_callback callback);

/**
 * @brief       Joins a single call and reports completion through a callback.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index  - call instance index.
 * @param[in]   callback    - completion callback invoked with ATT status.
 * @return      BLE_PRF_SUCCESS if the write request is queued, otherwise an error code.
 */
int ble_tbsc_write_call_control_join_one(uint16_t conn_handle, uint8_t call_index, prf_write_callback callback);

/**
 * @brief       Joins two calls and reports completion through a callback.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   call_index1 - first call instance index.
 * @param[in]   call_index2 - second call instance index.
 * @param[in]   callback    - completion callback invoked with ATT status.
 * @return      BLE_PRF_SUCCESS if the write request is queued, otherwise an error code.
 */
int ble_tbsc_write_call_control_join_two(uint16_t conn_handle, uint8_t call_index1, uint8_t call_index2, prf_write_callback callback);
