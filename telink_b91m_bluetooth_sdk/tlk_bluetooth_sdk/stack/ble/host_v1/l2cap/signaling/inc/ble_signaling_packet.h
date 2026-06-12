/********************************************************************************************************
 * @file    ble_signaling_packet.h
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
 * @brief       Package a connection parameter update request.
 * @param[in]   p_signaling_deal_info - pointer to the signaling deal information.
 * @param[in]   p_pdu_format          - pointer to the PDU format.
 * @return      the length of the packaged request.
 */
uint16_t ble_host_signaling_conn_param_update_req(const struct ble_host_signaling_deal_info *p_signaling_deal_info, const struct ble_signaling_pdu_format *p_pdu_format);

/**
 * @brief       Package a connection parameter update response.
 * @param[in]   p_signaling_deal_info - pointer to the signaling deal information.
 * @param[in]   p_pdu_format          - pointer to the PDU format.
 * @return      the length of the packaged response.
 */
uint16_t ble_host_signaling_conn_param_update_rsp(const struct ble_host_signaling_deal_info *p_signaling_deal_info, const struct ble_signaling_pdu_format *p_pdu_format);

uint16_t ble_host_signaling_command_reject_rsp(const struct ble_host_signaling_deal_info *p_signaling_deal_info, const struct ble_signaling_pdu_format *p_pdu_format);

uint16_t ble_host_signaling_disconnect_request(const struct ble_host_signaling_deal_info *p_signaling_deal_info, const struct ble_signaling_pdu_format *p_pdu_format);

uint16_t ble_host_signaling_disconnect_response(const struct ble_host_signaling_deal_info *p_signaling_deal_info, const struct ble_signaling_pdu_format *p_pdu_format);

uint16_t ble_host_signaling_le_credit_based_connection_request(const struct ble_host_signaling_deal_info *p_signaling_deal_info,
                                                               const struct ble_signaling_pdu_format     *p_pdu_format);

uint16_t ble_host_signaling_le_credit_based_connection_response(const struct ble_host_signaling_deal_info *p_signaling_deal_info,
                                                                const struct ble_signaling_pdu_format     *p_pdu_format);

uint16_t ble_host_signaling_flow_control_credit_ind(const struct ble_host_signaling_deal_info *p_signaling_deal_info, const struct ble_signaling_pdu_format *p_pdu_format);

uint16_t ble_host_signaling_credit_based_connection_request(const struct ble_host_signaling_deal_info *p_signaling_deal_info, const struct ble_signaling_pdu_format *p_pdu_format);

uint16_t ble_host_signaling_credit_based_connection_response(const struct ble_host_signaling_deal_info *p_signaling_deal_info, const struct ble_signaling_pdu_format *p_pdu_format);

uint16_t ble_host_signaling_credit_based_reconfigure_request(const struct ble_host_signaling_deal_info *p_signaling_deal_info, const struct ble_signaling_pdu_format *p_pdu_format);

uint16_t ble_host_signaling_credit_based_reconfigure_response(const struct ble_host_signaling_deal_info *p_signaling_deal_info,
                                                              const struct ble_signaling_pdu_format     *p_pdu_format);
