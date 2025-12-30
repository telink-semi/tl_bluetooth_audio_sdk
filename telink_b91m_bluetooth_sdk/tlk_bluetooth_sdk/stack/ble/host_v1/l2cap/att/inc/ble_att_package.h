/********************************************************************************************************
 * @file    ble_att_package.h
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


struct attr_value_tuple_send
{
    uint16_t       length;
    const uint8_t *value;
};

/**
 *   @brief sends an error response PDU(ATT channel) with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] opcode The ATT opcode of the request that caused the error.
 *   @param[in] handle The handle of the attribute that caused the error.
 *   @param[in] reason The reason for the error refer to enum attribute_error_code.
 *
 *   @return BLE_HOST_ERR_SUCC if the error response is sent successfully.
 */
int ble_host_att_send_error_rsp(uint16_t conn_handle, uint8_t opcode, uint16_t handle, uint8_t reason);

/**
 *   @brief sends an exchange MTU request PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] mtu The maximum transmission unit.
 *
 *   @return BLE_HOST_ERR_SUCC if the exchange MTU request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too large.
 *
 *   @note The Exchange MTU sub-procedure shall only be supported on the LE Fixed Channel Unenhanced ATT bearer.
 */
int ble_host_att_send_exchange_mtu_req(uint16_t conn_handle, uint16_t mtu);

/**
 *   @brief sends an exchange MTU response PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] mtu The maximum transmission unit.
 *
 *   @return BLE_HOST_ERR_SUCC if the exchange MTU response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too large.
 *
 *   @note The Exchange MTU sub-procedure shall only be supported on the LE Fixed Channel Unenhanced ATT bearer.
 */
int ble_host_att_send_exchange_mtu_rsp(uint16_t conn_handle, uint16_t mtu);

/**
 *   @brief sends a find information request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] start_handle The starting handle of the range to find information.
 *   @param[in] end_handle The ending handle of the range to find information.
 *
 *   @return BLE_HOST_ERR_SUCC if the find information request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the start_handle or end_handle is invalid.
 */
int ble_host_att_send_find_info_req(uint16_t conn_handle, uint16_t start_handle, uint16_t end_handle);

/**
 *   @brief sends a find information response(ATT channel) PDU fix format is handle and 16-bit Bluetooth UUD.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] info The information to be sent in the response.
 *   @param[in] info_count The number of information to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the find information response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the info or info_count is NULL or 0.
 */
int ble_host_att_send_find_info_rsp_16bit_uuid(uint16_t conn_handle, const struct att_info_16bit_uuid *info, uint16_t info_count);

/**
 *   @brief sends a find information response(ATT channel) PDU fix format is handle and 128-bit Bluetooth UUID.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] info The information to be sent in the response.
 *   @param[in] info_count The number of information to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the find information response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the info or info_count is NULL or 0.
 */
int ble_host_att_send_find_info_rsp_128bit_uuid(uint16_t conn_handle, const struct att_info_128bit_uuid *info, uint16_t info_count);

/**
 *   @brief sends a find by type value request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] start_handle First requested handle number.
 *   @param[in] end_handle Last requested handle number.
 *   @param[in] type The 2 octet UUID to find.
 *   @param[in] value The attribute value to find.
 *   @param[in] value_len The attribute length of the value to find.
 *
 *   @return BLE_HOST_ERR_SUCC if the find by type value request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the start_handle or end_handle is invalid.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and value_len is not 0.
 */
int ble_host_att_send_find_by_type_value_req(uint16_t conn_handle, uint16_t start_handle, uint16_t end_handle, uint16_t type, const uint8_t *value, uint16_t value_len);

/**
 *   @brief sends a find by type value response(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] handles_info The handles information list to be sent in the response.
 *   @param[in] info_count The number of handles information list to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the find by type value response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the handles_info or info_count is NULL or 0.
 */
int ble_host_att_send_find_by_type_value_rsp(uint16_t conn_handle, const struct attr_handle_group *handles_info, uint16_t info_count);

/**
 *   @brief sends a read by type request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] start_handle First requested handle number.
 *   @param[in] end_handle Last requested handle number.
 *   @param[in] uuid The 2 or 16 octet UUID to read.
 *
 *   @return BLE_HOST_ERR_SUCC if the read by type request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the start_handle or end_handle is invalid.
 *      - BLE_L2CAP_ERR_ATT_INVALID_UUID if the uuid is invalid.
 */
int ble_host_att_send_read_by_type_req(uint16_t conn_handle, uint16_t start_handle, uint16_t end_handle, const struct att_uuid *uuid);

/**
 *   @brief sends a read request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] handle The handle to read attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the read request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 */
int ble_host_att_send_read_req(uint16_t conn_handle, uint16_t handle);

/**
 *   @brief sends a read response(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] value The attribute value to be sent in the response.
 *   @param[in] value_len The attribute length of the value to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the read response is sent successfully.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and value_len is not 0.
 */
int ble_host_att_send_read_rsp(uint16_t conn_handle, uint8_t *value, uint16_t value_len);

/**
 *   @brief sends a read blob request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] handle The handle to read attribute value.
 *   @param[in] offset The offset of the first octet to be read.
 *
 *   @return BLE_HOST_ERR_SUCC if the read blob request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 */
int ble_host_att_send_read_blob_req(uint16_t conn_handle, uint16_t handle, uint16_t offset);

/**
 *   @brief sends a read blob response(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] value The attribute value to be sent in the response.
 *   @param[in] value_len The attribute length of the value to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the read blob response is sent successfully.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and value_len is not 0.
 */
int ble_host_att_send_read_blob_rsp(uint16_t conn_handle, uint8_t *value, uint16_t value_len);

/**
 *   @brief sends a read multiple request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] handles The handles to read attribute values.
 *   @param[in] count The number of handles to read attribute values.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 */
int ble_host_att_send_read_mult_req(uint16_t conn_handle, uint16_t *handles, uint16_t count);

/**
 *   @brief sends a read multiple response(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] values The attribute values to be sent in the response.
 *   @param[in] values_len The attribute length of the values to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the values is NULL and values_len is not 0.
 */
int ble_host_att_send_read_mult_rsp(uint16_t conn_handle, uint8_t *values, uint16_t values_len);

/**
 *   @brief  Sends an ATT Read By Group Type request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] start_handle: First requested handle number.
 *   @param[in] end_handle: Last requested handle number.
 *   @param[in] uuid: Attribute Group type, 2 or 16 octet UUID.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_ATT_INVALID_UUID if the UUID is invalid.
 */
int ble_host_att_send_read_by_group_type_req(uint16_t conn_handle, uint16_t start_handle, uint16_t end_handle, const struct att_uuid *uuid);

/**
 *   @brief  Sends an ATT Read By Group Type response(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] length: The size of each attribute data.
 *   @param[in] data: The attribute data to be sent in the response, refer to struct attr_group_type_data.
 *   @param[in] count: The number of attribute data to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple response is sent successfully.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the data is NULL and count is not 0.
 */
int ble_host_att_send_read_by_group_type_rsp(uint16_t conn_handle, uint8_t length, const struct attr_group_type_data *data, uint16_t count);

/**
 *   @brief  Sends an ATT Read Multiple Variable request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] handles: The handles to read attribute values.
 *   @param[in] count: The number of handles to read attribute values.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple variable request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the count is less than 2 or handles is NULL.
 */
int ble_host_att_send_read_mult_variable_req(uint16_t conn_handle, uint16_t *handles, uint16_t count);

/**
 *   @brief  Sends an ATT Read Multiple Variable response(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] values_tuple: The attribute values to be sent in the response.
 *   @param[in] count: The number of attribute values to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple variable response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the values_tuple is NULL and count is not 0.
 */
int ble_host_att_send_read_mult_variable_rsp(uint16_t conn_handle, const struct attr_value_tuple_send *values_tuple, uint16_t count);

/**
 *   @brief  Sends an ATT Write Request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] handle: The handle to write attribute value.
 *   @param[in] value: The attribute value to be written.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the write request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_att_send_write_req(uint16_t conn_handle, uint16_t handle, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Write Response(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the write response is sent successfully.
 */
int ble_host_att_send_write_rsp(uint16_t conn_handle);

/**
 *   @brief  Sends an ATT Write Command(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] handle: The handle to write attribute value.
 *   @param[in] value: The attribute value to be written.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the write command is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_att_send_write_cmd(uint16_t conn_handle, uint16_t handle, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Signed Write Command(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] handle: The handle to write attribute value.
 *   @param[in] value: The attribute value to be written.
 *   @param[in] length: The size of the attribute value.
 *   @param[in] signature: The signature of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the write command is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 *
 *   @note  The Signed Write Without Response sub-procedure shall only be supported on the LE Fixed
 *          Channel Unenhanced ATT bearer.
 */
int ble_host_att_send_signed_write_cmd(uint16_t conn_handle, uint16_t handle, const uint8_t *value, uint16_t length, const uint8_t signature[12]);

/**
 *   @brief  Sends an ATT Prepare Write Request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] handle: The handle to write attribute value.
 *   @param[in] offset: The offset of the attribute value.
 *   @param[in] value: The attribute value to be written.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the prepare write request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_att_send_prepare_write_req(uint16_t conn_handle, uint16_t handle, uint16_t offset, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Prepare Write Response(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] handle: The handle to write attribute value.
 *   @param[in] offset: The offset of the attribute value.
 *   @param[in] value: The attribute value to be written.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the prepare write response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_att_send_prepare_write_rsp(uint16_t conn_handle, uint16_t handle, uint16_t offset, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Execute Write Request(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] flags: The flags of the execute write request.
 *
 *   @return BLE_HOST_ERR_SUCC if the execute write request is sent successfully.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_att_send_execute_write_req(uint16_t conn_handle, uint8_t flags);

/**
 *   @brief  Sends an ATT Execute Write Response(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the execute write response is sent successfully.
 */
int ble_host_att_send_execute_write_rsp(uint16_t conn_handle);

/**
 *   @brief  Sends an ATT Handle Value Notification(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] handle: The handle to notify attribute value.
 *   @param[in] value: The attribute value to be notified.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the handle value notification is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_att_send_handle_value_notification(uint16_t conn_handle, uint16_t handle, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Handle Value Indication(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] handle: The handle to indicate attribute value.
 *   @param[in] value: The attribute value to be indicated.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the handle value indication is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_att_send_handle_value_indication(uint16_t conn_handle, uint16_t handle, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Handle Value Confirmation(ATT channel) PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the handle value confirmation is sent successfully.
 */
int ble_host_att_send_handle_value_confirmation(uint16_t conn_handle);

/***************************** BLE EATT send API *******************/

/**
 *   @brief sends an error response PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] opcode The ATT opcode of the request that caused the error.
 *   @param[in] handle The handle of the attribute that caused the error.
 *   @param[in] reason The reason for the error refer to enum attribute_error_code.
 *
 *   @return BLE_HOST_ERR_SUCC if the error response is sent successfully.
 */
int ble_host_send_att_error_rsp(uint16_t conn_handle, uint16_t cid, uint8_t opcode, uint16_t handle, uint8_t reason);

/**
 *   @brief sends a find information request PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] start_handle The starting handle of the range to find information.
 *   @param[in] end_handle The ending handle of the range to find information.
 *
 *   @return BLE_HOST_ERR_SUCC if the find information request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the start_handle or end_handle is invalid.
 */
int ble_host_send_att_find_info_req(uint16_t conn_handle, uint16_t cid, uint16_t start_handle, uint16_t end_handle);

/**
 *   @brief sends a find information response PDU fix format is handle and 16-bit Bluetooth UUD.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] info The information to be sent in the response.
 *   @param[in] info_count The number of information to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the find information response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the info or info_count is NULL or 0.
 */
int ble_host_send_att_find_info_rsp_16bit_uuid(uint16_t conn_handle, uint16_t cid, const struct att_info_16bit_uuid *info, uint16_t info_count);

/**
 *   @brief sends a find information response PDU fix format is handle and 128-bit Bluetooth UUID.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] info The information to be sent in the response.
 *   @param[in] info_count The number of information to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the find information response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the info or info_count is NULL or 0.
 */
int ble_host_send_att_find_info_rsp_128bit_uuid(uint16_t conn_handle, uint16_t cid, const struct att_info_128bit_uuid *info, uint16_t info_count);

/**
 *   @brief sends a find by type value request PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] start_handle First requested handle number.
 *   @param[in] end_handle Last requested handle number.
 *   @param[in] type The 2 octet UUID to find.
 *   @param[in] value The attribute value to find.
 *   @param[in] value_len The attribute length of the value to find.
 *
 *   @return BLE_HOST_ERR_SUCC if the find by type value request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the start_handle or end_handle is invalid.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and value_len is not 0.
 */
int ble_host_send_att_find_by_type_value_req(uint16_t conn_handle, uint16_t cid, uint16_t start_handle, uint16_t end_handle, uint16_t type, const uint8_t *value,
                                             uint16_t value_len);

/**
 *   @brief sends a find by type value response PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handles_info The handles information list to be sent in the response.
 *   @param[in] info_count The number of handles information list to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the find by type value response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the handles_info or info_count is NULL or 0.
 */
int ble_host_send_att_find_by_type_value_rsp(uint16_t conn_handle, uint16_t cid, const struct attr_handle_group *handles_info, uint16_t info_count);

/**
 *   @brief sends a read by type request PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] start_handle First requested handle number.
 *   @param[in] end_handle Last requested handle number.
 *   @param[in] uuid The 2 or 16 octet UUID to read.
 *
 *   @return BLE_HOST_ERR_SUCC if the read by type request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the start_handle or end_handle is invalid.
 *      - BLE_L2CAP_ERR_ATT_INVALID_UUID if the uuid is invalid.
 */
int ble_host_send_att_read_by_type_req(uint16_t conn_handle, uint16_t cid, uint16_t start_handle, uint16_t end_handle, const struct att_uuid *uuid);

/**
 *   @brief sends a read request PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handle The handle to read attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the read request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 */
int ble_host_send_att_read_req(uint16_t conn_handle, uint16_t cid, uint16_t handle);

/**
 *   @brief sends a read response PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] value The attribute value to be sent in the response.
 *   @param[in] value_len The attribute length of the value to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the read response is sent successfully.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and value_len is not 0.
 */
int ble_host_send_att_read_rsp(uint16_t conn_handle, uint16_t cid, uint8_t *value, uint16_t value_len);

/**
 *   @brief sends a read blob request PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handle The handle to read attribute value.
 *   @param[in] offset The offset of the first octet to be read.
 *
 *   @return BLE_HOST_ERR_SUCC if the read blob request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 */
int ble_host_send_att_read_blob_req(uint16_t conn_handle, uint16_t cid, uint16_t handle, uint16_t offset);

/**
 *   @brief sends a read blob response PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] value The attribute value to be sent in the response.
 *   @param[in] value_len The attribute length of the value to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the read blob response is sent successfully.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and value_len is not 0.
 */
int ble_host_send_att_read_blob_rsp(uint16_t conn_handle, uint16_t cid, uint8_t *value, uint16_t value_len);

/**
 *   @brief sends a read multiple request PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handles The handles to read attribute values.
 *   @param[in] count The number of handles to read attribute values.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 */
int ble_host_send_att_read_mult_req(uint16_t conn_handle, uint16_t cid, uint16_t *handles, uint16_t count);

/**
 *   @brief sends a read multiple response PDU with the given parameters.
 *
 *   @param[in] conn_handle The connection handle.
 *   @param[in] cid The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] values The attribute values to be sent in the response.
 *   @param[in] values_len The attribute length of the values to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the values is NULL and values_len is not 0.
 */
int ble_host_send_att_read_mult_rsp(uint16_t conn_handle, uint16_t cid, uint8_t *values, uint16_t values_len);

/**
 *   @brief  Sends an ATT Read By Group Type request PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] start_handle: First requested handle number.
 *   @param[in] end_handle: Last requested handle number.
 *   @param[in] uuid: Attribute Group type, 2 or 16 octet UUID.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_ATT_INVALID_UUID if the UUID is invalid.
 */
int ble_host_send_att_read_by_group_type_req(uint16_t conn_handle, uint16_t cid, uint16_t start_handle, uint16_t end_handle, const struct att_uuid *uuid);

/**
 *   @brief  Sends an ATT Read By Group Type response PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] length: The size of each attribute data.
 *   @param[in] data: The attribute data to be sent in the response, refer to struct attr_group_type_data.
 *   @param[in] count: The number of attribute data to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple response is sent successfully.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the data is NULL and count is not 0.
 */
int ble_host_send_att_read_by_group_type_rsp(uint16_t conn_handle, uint16_t cid, uint8_t length, const struct attr_group_type_data *data, uint16_t count);

/**
 *   @brief  Sends an ATT Read Multiple Variable request PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handles: The handles to read attribute values.
 *   @param[in] count: The number of handles to read attribute values.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple variable request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the count is less than 2 or handles is NULL.
 */
int ble_host_send_att_read_mult_variable_req(uint16_t conn_handle, uint16_t cid, uint16_t *handles, uint16_t count);

/**
 *   @brief  Sends an ATT Read Multiple Variable response PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] values_tuple: The attribute values to be sent in the response.
 *   @param[in] count: The number of attribute values to be sent in the response.
 *
 *   @return BLE_HOST_ERR_SUCC if the read multiple variable response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the values_tuple is NULL and count is not 0.
 */
int ble_host_send_att_read_mult_variable_rsp(uint16_t conn_handle, uint16_t cid, const struct attr_value_tuple_send *values_tuple, uint16_t count);

/**
 *   @brief  Sends an ATT Write Request PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handle: The handle to write attribute value.
 *   @param[in] value: The attribute value to be written.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the write request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_send_att_write_req(uint16_t conn_handle, uint16_t cid, uint16_t handle, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Write Response PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *
 *   @return BLE_HOST_ERR_SUCC if the write response is sent successfully.
 */
int ble_host_send_att_write_rsp(uint16_t conn_handle, uint16_t cid);

/**
 *   @brief  Sends an ATT Write Command PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handle: The handle to write attribute value.
 *   @param[in] value: The attribute value to be written.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the write command is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_send_att_write_cmd(uint16_t conn_handle, uint16_t cid, uint16_t handle, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Prepare Write Request PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handle: The handle to write attribute value.
 *   @param[in] offset: The offset of the attribute value.
 *   @param[in] value: The attribute value to be written.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the prepare write request is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_send_att_prepare_write_req(uint16_t conn_handle, uint16_t cid, uint16_t handle, uint16_t offset, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Prepare Write Response PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handle: The handle to write attribute value.
 *   @param[in] offset: The offset of the attribute value.
 *   @param[in] value: The attribute value to be written.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the prepare write response is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_ATT_MTU_EXCEEDED if the MTU is too small.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_send_att_prepare_write_rsp(uint16_t conn_handle, uint16_t cid, uint16_t handle, uint16_t offset, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Execute Write Request PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] flags: The flags of the execute write request.
 *
 *   @return BLE_HOST_ERR_SUCC if the execute write request is sent successfully.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_send_att_execute_write_req(uint16_t conn_handle, uint16_t cid, uint8_t flags);

/**
 *   @brief  Sends an ATT Execute Write Response PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *
 *   @return BLE_HOST_ERR_SUCC if the execute write response is sent successfully.
 */
int ble_host_send_att_execute_write_rsp(uint16_t conn_handle, uint16_t cid);

/**
 *   @brief  Sends an ATT Handle Value Notification PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handle: The handle to notify attribute value.
 *   @param[in] value: The attribute value to be notified.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the handle value notification is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_send_att_handle_value_notification(uint16_t conn_handle, uint16_t cid, uint16_t handle, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Handle Value Indication PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handle: The handle to indicate attribute value.
 *   @param[in] value: The attribute value to be indicated.
 *   @param[in] length: The size of the attribute value.
 *
 *   @return BLE_HOST_ERR_SUCC if the handle value indication is sent successfully.
 *      - BLE_L2CAP_ERR_ATT_INVALID_HANDLE if the handle is invalid.
 *      - BLE_L2CAP_ERR_INVALID_PARAMS if the value is NULL and length is not 0.
 */
int ble_host_send_att_handle_value_indication(uint16_t conn_handle, uint16_t cid, uint16_t handle, const uint8_t *value, uint16_t length);

/**
 *   @brief  Sends an ATT Handle Value Confirmation PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *
 *   @return BLE_HOST_ERR_SUCC if the handle value confirmation is sent successfully.
 */
int ble_host_send_att_handle_value_confirmation(uint16_t conn_handle, uint16_t cid);

/**
 *   @brief  Sends an ATT Handle Value Notification PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] cid: The L2CAP channel ID, LE_L2CAP_CID_ATT or EATT CID.
 *   @param[in] handle: The handle to notify attribute value.
 *   @param[in] value: The attribute value to be notified.
 *   @param[in] length: The size of the attribute value.
 *   @param[in] callback: The callback function to be called when the notification is sent.
 *   @param[in] arg: The argument to be passed to the callback function.
 *
 *   @return BLE_HOST_ERR_SUCC if the handle value notification is sent successfully.
 */
int ble_host_send_att_handle_value_notification_callback(uint16_t conn_handle, uint16_t cid, uint16_t handle, const uint8_t *value, uint16_t length,
                                                         void (*callback)(uint16_t conn_handle, void *arg, uint16_t status), void *arg);

/**
 *   @brief  Sends an ATT Handle Value Notification PDU with the given parameters.
 *
 *   @param[in] conn_handle: The connection handle.
 *   @param[in] handle: The handle to notify attribute value.
 *   @param[in] value: The attribute value to be notified.
 *   @param[in] length: The size of the attribute value.
 *   @param[in] callback: The callback function to be called when the notification is sent.
 *   @param[in] arg: The argument to be passed to the callback function.
 *
 *   @return BLE_HOST_ERR_SUCC if the handle value notification is sent successfully.
 */
int ble_host_att_send_handle_value_notification_callback(uint16_t conn_handle, uint16_t handle, const uint8_t *value, uint16_t length,
                                                         void (*callback)(uint16_t conn_handle, void *arg, uint16_t status), void *arg);
