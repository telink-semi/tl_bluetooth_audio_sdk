/********************************************************************************************************
 * @file    attc_packet.h
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

/*****************core v5.4 Vol 3 Part F 3.4.1.1 ATT_ERROR_RSP******************/
/**
 *   @brief L2CAP ATT deal with error response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_error_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.2.2 ATT_EXCHANGE_MTU_RSP******************/
/**
 *   @brief L2CAP ATT deal with exchange MTU response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
*/
uint16_t ble_host_att_deal_exchange_mtu_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.3.2 ATT_FIND_INFORMATION_RSP******************/
/**
 *   @brief L2CAP ATT deal with find information response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_find_information_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.3.4 ATT_FIND_BY_TYPE_VALUE_RSP******************/
/**
 *   @brief L2CAP ATT deal with find by type value response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_find_by_type_value_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.4.2 ATT_READ_BY_TYPE_RSP******************/
/**
 *   @brief L2CAP ATT deal with read by type response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_read_by_type_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.4.4 ATT_READ_RSP******************/
/**
 *   @brief L2CAP ATT deal with read response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_read_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.4.6 ATT_READ_BLOB_RSP******************/
/**
 *   @brief L2CAP ATT deal with read blob response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_read_blob_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.4.8 ATT_READ_MULTIPLE_RSP******************/
/**
 *   @brief L2CAP ATT deal with read multiple response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_read_multiple_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/******************core v5.4 Vol 3 Part F 3.4.4.10 ATT_READ_BY_GROUP_TYPE_RSP******************/
/**
 *   @brief L2CAP ATT deal with read by group type response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_read_by_group_type_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.4.12 ATT_READ_MULTIPLE_VARIABLE_RSP******************/
/**
 *   @brief L2CAP ATT deal with read multiple variable response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_read_multiple_variable_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.5.2 ATT_WRITE_RSP******************/
/**
 *   @brief L2CAP ATT deal with write response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_write_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.6.2 ATT_PREPARE_WRITE_RSP******************/
/**
 *   @brief L2CAP ATT deal with prepare write response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_prepare_write_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.6.4 ATT_EXECUTE_WRITE_RSP******************/
/**
 *   @brief L2CAP ATT deal with execute write response.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_execute_write_rsp(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.7.1 ATT_HANDLE_VALUE_NTF******************/
/**
 *   @brief L2CAP ATT deal with handle value notification.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_handle_value_ntf(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.7.2 ATT_HANDLE_VALUE_IND******************/
/**
 *   @brief L2CAP ATT deal with handle value indication.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return ATT_HANDLE_VALUE_CFM packet.
 */
uint16_t ble_host_att_deal_handle_value_ind(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);

/*****************core v5.4 Vol 3 Part F 3.4.7.3 ATT_MULTIPLE_HANDLE_VALUE_NTF******************/
/**
 *   @brief L2CAP ATT deal with multiple handle value notification.
 *
 *   @param[in] att_deal_info ATT connection information.
 *   @param[in] pdu ATT PDU format.
 *   @param[in] pdu_len ATT PDU length.
 *
 *   @return 0.
 */
uint16_t ble_host_att_deal_multiple_handle_value_ntf(const struct ble_host_att_deal_info *att_deal_info, const struct ble_att_pdu_format *pdu, uint16_t pdu_len);
