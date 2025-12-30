/********************************************************************************************************
 * @file    ble_attc.h
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
 *   @brief  ATT client receive PDU callback function type.
 *
 *   This function is called by the ATT client when a PDU is received from the server.
 *
 *   @param[in] conn_handle Connection handle.
 *   @param[in] cid ATT channel ID.
 *   @param[in] opcode ATT operation code.
 *   @param[in] pdu Pointer to the PDU data.
 *   @param[in] pdu_len Length of the PDU data.
 *
 *   @return None.
 */
typedef void (*ble_host_attc_recv_pdu_callback)(uint16_t conn_handle, uint16_t cid, uint8_t opcode, const uint8_t *pdu, uint16_t pdu_len);

/**
 *   @brief  Register ATT client receive PDU callback function.
 *
 *   @param[in] callback Pointer to the callback function.
 *
 *   @return None.
 *
 *   @note If Att client module receive response from server, it will call this callback function.
 */
void ble_host_attc_register_rsp_recv_pdu_callback(ble_host_attc_recv_pdu_callback callback);

/**
 *   @brief  Register ATT client server initiated receive PDU callback function.
 *
 *   @param[in] callback Pointer to the callback function.
 *
 *   @return None.
 *
 *   @note If Att client module receive Notification, Indication or multiple Notifications from server,
 *          it will call this callback function.
 */
void ble_host_attc_register_server_initiated_recv_pdu_callback(ble_host_attc_recv_pdu_callback callback);
